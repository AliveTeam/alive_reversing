#include "stdafx.h"
#include "PsxRender.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "Renderer/IRenderer.hpp"
#include "../AliveLibCommon/FatalError.hpp"

struct OtUnknown final
{
    s32** field_0_pOtStart;
    s32** field_4;
    s32** field_8_pOt_End;
};

static void DrawOTag_HandlePrimRendering(IRenderer& renderer, PrimAny& any)
{
    switch (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(any.mPrimHeader->rgb_code.code_or_pad))
    {
        case PrimTypeCodes::eLineG2:
            renderer.Draw(*any.mLineG2);
            break;

        case PrimTypeCodes::eLineG4:
            renderer.Draw(*any.mLineG4);
            break;

        case PrimTypeCodes::ePolyG3:
            renderer.Draw(*any.mPolyG3);
            break;

        case PrimTypeCodes::ePolyFT4:
            renderer.Draw(*any.mPolyFT4);
            break;

        case PrimTypeCodes::ePolyG4:
            renderer.Draw(*any.mPolyG4);
            break;

        default:
            ALIVE_FATAL("Unknown prim type %d", static_cast<s32>(any.mPrimHeader->rgb_code.code_or_pad));
            break;
    }
}


struct OTInformation final
{
    PrimHeader** mOt;
    s32 mOtSize;

    // A "root" pointer is one of the root elements of the OT linked list. These are not pointers to actual PrimHeader
    // objects but actually only a pointer to another PrimHeader. Therefore dereferencing any field other than "tag" which
    // is at offset 0 and the pointer to the next item or the terminator value is UB as it will point to random and invalid data.
    // These root pointers can ONLY be used to get the pointer to the next item in the OT.
    bool IsRootPointer(PrimHeader* pItem) const
    {
        const u8* ptr = reinterpret_cast<const u8*>(pItem);
        const u8* pStart = reinterpret_cast<const u8*>(mOt);
        const u8* pEnd = pStart + (mOtSize * sizeof(void*));
        return ptr >= pStart && ptr <= pEnd;
    }
};

static OTInformation gSavedOtInfo[32] = {};

static void Push_OTInformation(PrimHeader** otBuffer, s32 otBufferSize)
{
    for (s32 i = 0; i < ALIVE_COUNTOF(gSavedOtInfo); i++)
    {
        if (gSavedOtInfo[i].mOt == otBuffer)
        {
            // LOG_WARNING("OT record pushed more than once, update existing");
            gSavedOtInfo[i].mOt = otBuffer;
            gSavedOtInfo[i].mOtSize = otBufferSize;
            return;
        }
    }

    for (s32 i = 0; i < ALIVE_COUNTOF(gSavedOtInfo); i++)
    {
        if (gSavedOtInfo[i].mOt == nullptr)
        {
            gSavedOtInfo[i].mOt = otBuffer;
            gSavedOtInfo[i].mOtSize = otBufferSize;
            return;
        }
    }
}

static bool Pop_OTInformation(PrimHeader** otBuffer, OTInformation& info)
{
    for (s32 i = 0; i < ALIVE_COUNTOF(gSavedOtInfo); i++)
    {
        if (gSavedOtInfo[i].mOt == otBuffer)
        {
            info = gSavedOtInfo[i];
            gSavedOtInfo[i] = {};
            return true;
        }
    }
    return false;
}

s32 sScreenXOffSet_BD30E4 = 0;
s32 sScreenYOffset_BD30A4 = 0;

void PSX_ClearOTag(PrimHeader** otBuffer, s32 otBufferSize)
{
    // PSX_OrderingTable_SaveRecord_4F62C0(otBuffer, otBufferSize);
    Push_OTInformation(otBuffer, otBufferSize);

    // Set each element to point to the next
    s32 i = 0;
    for (i = 0; i < otBufferSize - 1; i++)
    {
        // If you think this seems strange you are correct. See OTInformation::IsRootPointer
        // for why.
        otBuffer[i] = reinterpret_cast<PrimHeader*>(&otBuffer[i + 1]);
    }

    // Terminate the list
    otBuffer[i] = reinterpret_cast<PrimHeader*>(static_cast<size_t>(0xFFFFFFFF));
}

static bool DrawOTagImpl(PrimHeader** ppOt)
{
    sScreenXOffSet_BD30E4 = 0;
    sScreenYOffset_BD30A4 = 0;

    OTInformation otInfo = {};
    if (!Pop_OTInformation(ppOt, otInfo))
    {
        ALIVE_FATAL("Failed to look up OT info record");
    }

    IRenderer& renderer = *IRenderer::GetRenderer();

    renderer.StartFrame();

    PrimHeader* pOtItem = ppOt[0];
    while (pOtItem)
    {
        if (pOtItem == reinterpret_cast<PrimHeader*>(static_cast<size_t>(0xFFFFFFFF)))
        {
            break;
        }

        SsSeqCalledTbyT_4FDC80();

        PrimAny any;
        any.mVoid = pOtItem;

        if (!otInfo.IsRootPointer(pOtItem))
        {
            const s32 itemToDrawType = any.mPrimHeader->rgb_code.code_or_pad;
            switch (itemToDrawType)
            {
                case PrimTypeCodes::eSetTPage:
                    renderer.SetTPage(static_cast<s16>(any.mSetTPage->field_C_tpage));
                    break;

                case PrimTypeCodes::ePrimClipper:
                    renderer.SetClip(*any.mPrimClipper);
                    break;

                // Always the lowest command in the list
                case PrimTypeCodes::eScreenOffset:
                    // NOTE: Conditional on dword_55EF94 removed as it is constant 1
                    renderer.SetScreenOffset(*any.mScreenOffset);
                    sScreenXOffSet_BD30E4 = any.mScreenOffset->field_C_xoff * 2;
                    sScreenYOffset_BD30A4 = any.mScreenOffset->field_E_yoff;
                    break;

                case PrimTypeCodes::eLaughingGas:
                    renderer.Draw(*any.mGas);
                    break;

                default:
                    DrawOTag_HandlePrimRendering(renderer, any);
                    break;
            }
        }
        else
        {
            //LOG_INFO("Skip root");
        }

        // To the next item
        pOtItem = any.mPrimHeader->tag; // offset 0
    }

    return false;
}

void PSX_DrawOTag(PrimHeader** ppOt)
{
    if (turn_off_rendering_BD0F20)
    {
        return;
    }

    if (DrawOTagImpl(ppOt))
    {
        return;
    }
}

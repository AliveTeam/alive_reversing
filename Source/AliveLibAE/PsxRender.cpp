#include "stdafx.h"
#include "PsxRender.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Sound/Midi.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Game.hpp"
#include "../relive_lib/Error.hpp"
#include "Compression.hpp"
#include <gmock/gmock.h>
#include "VGA.hpp"
#include "Renderer/IRenderer.hpp"
#include "../AliveLibCommon/FatalError.hpp"

// TODO: Refactor + remove these
#define BYTEn(x, n) (*((u8*) &(x) + n))
#define BYTE1(x) BYTEn(x, 1)

struct OtUnknown final
{
    s32** field_0_pOtStart;
    s32** field_4;
    s32** field_8_pOt_End;
};

OtUnknown sOt_Stack_BD0D88[32] = {};
s32 sOtIdxRollOver_BD0C08 = 0;


s16 sActiveTPage_578318 = -1;
u32 sTexture_page_x_BD0F0C = 0;
u32 sTexture_page_y_BD0F10 = 0;
u32 sTexture_mode_BD0F14 = 0;
u32 tpage_width_57831C = 10;
u32 sTexture_page_abr_BD0F18 = 0;
u16* sTPage_src_ptr_BD0F1C = nullptr;


static void DrawOTag_HandlePrimRendering(IRenderer& renderer, PrimAny& any)
{
    switch (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(any.mPrimHeader->rgb_code.code_or_pad))
    {
        case PrimTypeCodes::eSprt8:
            ALIVE_FATAL("Never expected eSprt8 to be added to the OT");
            break;

        case PrimTypeCodes::eSprt16:
            ALIVE_FATAL("Never expected eSprt16 to be added to the OT");
            break;

        case PrimTypeCodes::eTile1:
            ALIVE_FATAL("Never expected eTile1 to be added to the OT");
            break;

        case PrimTypeCodes::eTile8:
            ALIVE_FATAL("Never expected eTile8 to be added to the OT");
            break;

        case PrimTypeCodes::eTile16:
            ALIVE_FATAL("Never expected eTile16 to be added to the OT");
            break;

        case PrimTypeCodes::eLineF3:
            ALIVE_FATAL("Never expected eLineF3 to be added to the OT");
            break;

        case PrimTypeCodes::eLineF4:
            ALIVE_FATAL("Never expected eLineF4 to be added to the OT");
            break;

        case PrimTypeCodes::eLineG3:
            ALIVE_FATAL("Never expected eLineG3 to be added to the OT");
            break;

        case PrimTypeCodes::ePolyFT3:
            ALIVE_FATAL("Never expected ePolyFT3 to be added to the OT");
            break;

        case PrimTypeCodes::ePolyGT3:
            ALIVE_FATAL("Never expected ePolyGT3 to be added to the OT");
            break;

        case PrimTypeCodes::ePolyGT4:
            ALIVE_FATAL("Never expected ePolyGT4 to be added to the OT");
            break;

        case PrimTypeCodes::eSprt:
            renderer.Draw(*any.mSprt);
            break;

        case PrimTypeCodes::eTile:
            renderer.Draw(*any.mTile);
            break;

        case PrimTypeCodes::eLineF2:
            renderer.Draw(*any.mLineF2);
            break;

        case PrimTypeCodes::eLineG2:
            renderer.Draw(*any.mLineG2);
            break;

        case PrimTypeCodes::eLineG4:
            renderer.Draw(*any.mLineG4);
            break;

        case PrimTypeCodes::ePolyF3:
            renderer.Draw(*any.mPolyF3);
            break;

        case PrimTypeCodes::ePolyG3:
            renderer.Draw(*any.mPolyG3);
            break;

        case PrimTypeCodes::ePolyF4:
            renderer.Draw(*any.mPolyF4);
            break;

        case PrimTypeCodes::ePolyFT4:
            renderer.Draw(*any.mPolyFT4);
            break;

        case PrimTypeCodes::ePolyG4:
            renderer.Draw(*any.mPolyG4);
            break;

        default:
            LOG_ERROR("Unknown prim type " << static_cast<s32>(any.mPrimHeader->rgb_code.code_or_pad));
            ALIVE_FATAL("Unknown prim type");
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

void PSX_ClearOTag_4F6290(PrimHeader** otBuffer, s32 otBufferSize)
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

static bool DrawOTagImpl(PrimHeader** ppOt, s16 drawEnv_of0, s16 drawEnv_of1)
{
    sScreenXOffSet_BD30E4 = 0;
    sScreenYOffset_BD30A4 = 0;
    sActiveTPage_578318 = -1;

    OTInformation otInfo = {};
    if (!Pop_OTInformation(ppOt, otInfo))
    {
        ALIVE_FATAL("Failed to look up OT info record");
    }

    IRenderer& renderer = *IRenderer::GetRenderer();

    renderer.StartFrame(drawEnv_of0, drawEnv_of1);

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

                case PrimTypeCodes::eMoveImage:
                    ALIVE_FATAL("eMoveImage should never be added to the OT");
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

void PSX_DrawOTag_4F6540(PrimHeader** ppOt)
{
    if (!sPsxEmu_EndFrameFnPtr_C1D17C || !sPsxEmu_EndFrameFnPtr_C1D17C(0))
    {
        if (turn_off_rendering_BD0F20)
        {
            if (sPsxEmu_EndFrameFnPtr_C1D17C)
            {
                sPsxEmu_EndFrameFnPtr_C1D17C(1);
            }
            return;
        }

        s16 drawEnv_of0 = 0;
        s16 drawEnv_of1 = 0;


        drawEnv_of0 = sPSX_EMU_DrawEnvState_C3D080.field_8_ofs[0];
        drawEnv_of1 = sPSX_EMU_DrawEnvState_C3D080.field_8_ofs[1];

        if (DrawOTagImpl(ppOt, drawEnv_of0, drawEnv_of1))
        {
            return;
        }

        if (sPsxEmu_EndFrameFnPtr_C1D17C)
        {
            sPsxEmu_EndFrameFnPtr_C1D17C(1);
        }
        return;
    }
}

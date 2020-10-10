#include "stdafx_ao.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Primitives.hpp"
#include "Sys_common.hpp"
#include "ResourceManager.hpp"
#include "VRam.hpp"
#include "Game.hpp"

// Fix pollution from windows.h
#undef min
#undef max

START_NS_AO

EXPORT short *CC Animation_OnFrame_Slig_46F610(void *, __int16 *)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT short* CC Animation_OnFrame_ZBallSmacker_41FB00(void* pObj, short* pData);

EXPORT short *CC Slog_OnFrame_471FD0(void *, __int16 *)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT short *CC Abe_OnFrame_429E30(void *, __int16 *)
{
    NOT_IMPLEMENTED();
    return 0;
}

TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[] = { Abe_OnFrame_429E30 };
TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[] = {Animation_OnFrame_Slig_46F610};
TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[] = { Slog_OnFrame_471FD0 };
TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[] = { Animation_OnFrame_ZBallSmacker_41FB00 };

void AnimationBase::vDecode()
{
    VDecode_403550();
}

void AnimationBase::VDecode_403550()
{
    NOT_IMPLEMENTED();
}

void AnimationBase::vRender(int xpos, int ypos, int** pOt, __int16 width, __int16 height)
{
    VRender_403AE0(xpos, ypos, pOt, width, height);
}

void AnimationBase::VRender_403AE0(int /*xpos*/, int /*ypos*/, int** /*ppOt*/, __int16 /*width*/, __int16 /*height*/)
{
    NOT_IMPLEMENTED();
}

void Animation::vCleanUp()
{
    VCleanUp_403F40();
}

void Animation::VCleanUp_403F40()
{
    if (field_4_flags.Get(AnimFlags::eBit17))
    {
        ResourceManager::FreeResource_455550(field_20_ppBlock);
    }

    gObjList_animations_505564->Remove_Item(this);
    

    // inlined Animation_Pal_Free ?
    if (field_84_vram_rect.w > 0)
    {
        Vram_free_450CE0({ field_84_vram_rect.x, field_84_vram_rect.y }, { field_84_vram_rect.w, field_84_vram_rect.h });
    }

    if (field_90_pal_depth > 0)
    {
        Pal_Free_447870(field_8C_pal_vram_xy, field_90_pal_depth);
    }

    ResourceManager::FreeResource_455550(field_24_dbuf);

}
void CC AnimationBase::AnimateAll_4034F0(DynamicArrayT<AnimationBase>* pAnimList)
{
    for (int i = 0; i < pAnimList->Size(); i++)
    {
        auto pAnim = pAnimList->ItemAt(i);
        if (!pAnim)
        {
            break;
        }

        if (pAnim->field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            if (pAnim->field_E_frame_change_counter > 0)
            {
                pAnim->field_E_frame_change_counter--;
                if (pAnim->field_E_frame_change_counter == 0)
                {
                    pAnim->vDecode();
                }
            }
        }
    }
}

signed __int16 Animation::Set_Animation_Data_402A40(int frameTableOffset, BYTE** pAnimRes)
{
    if (pAnimRes)
    {
        field_20_ppBlock = pAnimRes;
    }

    if (!field_20_ppBlock)
    {
        return 0;
    }

    field_18_frame_table_offset = frameTableOffset;

    AnimationHeader* pAnimationHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    field_10_frame_delay = pAnimationHeader->field_0_fps;

    field_4_flags.Clear(AnimFlags::eBit12_ForwardLoopCompleted);
    field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
    field_4_flags.Clear(AnimFlags::eBit8_Loop);

    if (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag)
    {
        field_4_flags.Set(AnimFlags::eBit8_Loop);
    }

    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    vDecode();

    // Reset to start frame
    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    return 1;
}

void Animation::SetFrame_402AC0(unsigned __int16 newFrame)
{
    if (field_20_ppBlock)
    {
        if (newFrame == -1)
        {
            newFrame = 0;
        }

        AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset); // TODO: Make getting offset to animation header cleaner

        if (newFrame > pHead->field_2_num_frames)
        {
            newFrame = pHead->field_2_num_frames;
        }

        field_E_frame_change_counter = 1;
        field_92_current_frame = newFrame - 1;
    }
}

signed __int16 Animation::Init_402D20(int /*frameTableOffset*/, DynamicArray* /*animList*/, BaseGameObject* /*pGameObj*/, unsigned __int16 /*maxW*/, unsigned __int16 /*maxH*/, BYTE** /*ppAnimData*/, unsigned __int8 /*bFlag_17*/, signed int /*b_StartingAlternationState*/, char /*bEnable_flag10_alternating*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Animation::Get_Frame_Count_403540()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset);  // TODO: Make getting offset to animation header cleaner
    return pHead->field_2_num_frames;
}

ALIVE_VAR(1, 0x4BA090, FrameInfoHeader, sBlankFrameInfoHeader_4BA090, {});

FrameInfoHeader* Animation::Get_FrameHeader_403A00(int frame)
{
    if (!field_20_ppBlock)
    {
        return nullptr;
    }

    if (frame < -1 || frame == -1)
    {
        frame = field_92_current_frame != -1 ? field_92_current_frame : 0;
    }

    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset);  // TODO: Make getting offset to animation header cleaner
    DWORD frameOffset = pHead->mFrameOffsets[frame];

    FrameInfoHeader* pFrame = reinterpret_cast<FrameInfoHeader*>(*field_20_ppBlock + frameOffset);

    // Never seen this get hit, perhaps some sort of PSX specific check as addresses have to be aligned there?
    // TODO: Remove it in the future when proven to be not required?
#if defined(_MSC_VER) && !defined(_WIN64)
    if (reinterpret_cast<DWORD>(pFrame) & 3)
    {
        FrameInfoHeader* Unknown = &sBlankFrameInfoHeader_4BA090;
        return Unknown;
    }
#endif

    return pFrame;
}

void Animation::LoadPal_403090(BYTE** pPalData, int palOffset)
{
    if (pPalData)
    {
        PSX_RECT rect = {};
        BYTE* pPalDataOffset = &(*pPalData)[palOffset];
        switch (field_90_pal_depth)
        {
        case 16:
            rect.x = field_8C_pal_vram_xy.field_0_x;
            rect.y = field_8C_pal_vram_xy.field_2_y;
            rect.w = 16;
            break;

        case 64:
            rect.x = field_8C_pal_vram_xy.field_0_x;
            rect.y = field_8C_pal_vram_xy.field_2_y;
            rect.w = 64;
            break;

        case 256:
            rect.x = field_8C_pal_vram_xy.field_0_x;
            rect.y = field_8C_pal_vram_xy.field_2_y;
            rect.w = 256;
            break;

        default:
            return;
        }
        rect.h = 1;
        PSX_LoadImage16_4962A0(&rect, pPalDataOffset + 4);
    }
}


static void CC Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly)
{
    if (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(pPoly->mBase.header.rgb_code.code_or_pad) == PrimTypeCodes::ePolyFT4)
    {
        pRect->x = pPoly->mBase.vert.x;
        pRect->y = pPoly->mBase.vert.y;
        pRect->w = pPoly->mVerts[2].mVert.x;
        pRect->h = pPoly->mVerts[2].mVert.y;
    }
    else
    {
        pRect->h = 0;
        pRect->w = 0;
        pRect->y = 0;
        pRect->x = 0;
    }
}

EXPORT void Animation::Get_Frame_Rect_402B50(PSX_RECT* pRect)
{
    
    Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];
    if (!field_4_flags.Get(AnimFlags::eBit20_use_xy_offset))
    {
        Poly_FT4_Get_Rect(pRect, pPoly);
        return;
    }

    const auto min_x0_x1 = std::min(X0(pPoly), X1(pPoly));
    const auto min_x2_x3 = std::min(X2(pPoly), X3(pPoly));
    pRect->x = std::min(min_x0_x1, min_x2_x3);

    const auto max_x0_x1 = std::max(X0(pPoly), X1(pPoly));
    const auto max_x2_x3 = std::max(X2(pPoly), X3(pPoly));
    pRect->w = std::max(max_x0_x1, max_x2_x3);

    const auto min_y0_y1 = std::min(Y0(pPoly), Y1(pPoly));
    const auto min_y2_y3 = std::min(Y2(pPoly), Y3(pPoly));
    pRect->y = std::min(min_y0_y1, min_y2_y3);

    const auto max_y0_y1 = std::max(Y0(pPoly), Y1(pPoly));
    const auto max_y2_y3 = std::max(Y2(pPoly), Y3(pPoly));
    pRect->h = std::max(max_y0_y1, max_y2_y3);
}

EXPORT void Animation::Get_Frame_Width_Height_403E80(short* pWidth, short* pHeight)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader_403A00(-1);
    if (field_4_flags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Mode should never be used");
    }
    else
    {
        auto pHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameHeader->field_0_frame_header_offset]);
        *pWidth = pHeader->field_4_width;
        *pHeight = pHeader->field_5_height;
    }
}

EXPORT void Animation::Get_Frame_Offset_403EE0(short* pBoundingX, short* pBoundingY)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader_403A00(-1);
    *pBoundingX = pFrameHeader->field_8_data.offsetAndRect.mOffset.x;
    *pBoundingY = pFrameHeader->field_8_data.offsetAndRect.mOffset.y;
}

void AnimationUnknown::vCleanUp()
{
    // Empty @ 404280
}

void AnimationUnknown::vRender(int /*xpos*/, int /*ypos*/, int** /*pOt*/, __int16 /*width*/, __int16 /*height*/)
{
    // Empty @ 402A20
}

void AnimationUnknown::vDecode()
{
    // Empty @ 402A10
}

void AnimationUnknown::VRender2_403FD0(int /*xpos*/, int /*ypos*/, int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void AnimationUnknown::GetRenderedSize_404220(PSX_RECT* /*pRect*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO

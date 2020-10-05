#include "stdafx_ao.h"
#include "Animation.hpp"
#include "Function.hpp"

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

void AnimationBase::vCleanUp()
{
    VCleanUp_403F40();
}

void AnimationBase::VCleanUp_403F40()
{
    NOT_IMPLEMENTED();
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

void Animation::SetFrame_402AC0(unsigned __int16 /*frame*/)
{
    NOT_IMPLEMENTED();
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

FrameInfoHeader* Animation::Get_FrameHeader_403A00(int /*frame*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
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


EXPORT void Animation::Get_Frame_Rect_402B50(PSX_RECT* /*pRect*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void Animation::Get_Frame_Width_Height_403E80(short* /*pWidth*/, short* /*pHeight*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void Animation::Get_Frame_Offset_403EE0(short* /*pOffX*/, short* /*pOffY*/)
{
    NOT_IMPLEMENTED();
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

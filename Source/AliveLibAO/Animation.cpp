#include "stdafx_ao.h"
#include "Animation.hpp"
#include "Function.hpp"

START_NS_AO

void CC AnimationBase::AnimateAll_4034F0(DynamicArrayT<AnimationBase>* /*pAnimList*/)
{
    NOT_IMPLEMENTED();
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

END_NS_AO

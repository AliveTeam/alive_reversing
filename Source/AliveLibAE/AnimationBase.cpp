#include "stdafx.h"
#include "AnimationBase.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Sys_common.hpp"

void AnimationBase::vDecode_40AC90()
{
    ALIVE_FATAL("Should never be called");
}

void AnimationBase::vRender_40B820(int /*xpos*/, int /*ypos*/, int** /*pOt*/, __int16 /*width*/, signed int /*height*/)
{
    ALIVE_FATAL("Should never be called");
//    return 0;
}

char AnimationBase::Animation_v_40BEE0(__int16 /*a2*/, __int16 /*a3*/, int /*a4*/, __int16 /*a5*/, __int16 /*op1*/)
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animation_v_40BEE0");
    return 0;
}

void CC AnimationBase::AnimateAll_40AC20(DynamicArrayT<AnimationBase>* pAnims)
{
    for (auto i = 0; i < pAnims->Size(); i++)
    {
        AnimationBase* pAnim = pAnims->ItemAt(i);
        if (!pAnim)
        {
            break;
        }

        if (!pAnim->field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            continue;
        }

        if (pAnim->field_E_frame_change_counter > 0)
        {
            pAnim->field_E_frame_change_counter--;
            if (pAnim->field_E_frame_change_counter == 0)
            {
                pAnim->vDecode_40AC90();
            }
        }
    }
}

#include "stdafx.h"
#include "Animation.hpp"
#include "Function.hpp"

void Animation::Animation__vdecode_40AC90()
{
    NOT_IMPLEMENTED();
}

char Animation::Animation_v_40B820(signed int /*a2*/, int /*a3*/, int /*a4*/, __int16 /*a5*/, signed int /*op1*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

// Destructor ?
signed __int16 Animation::Animationv_40C630()
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animationv_40C630");
    return 0;
}

__int16 Animation::Animationv_40B200()
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animationv_40B200");
    return 0;
}

char Animation::Animation_v_40BEE0(__int16 /*a2*/, __int16 /*a3*/, int /*a4*/, __int16 /*a5*/, __int16 /*op1*/)
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animation_v_40BEE0");
    return 0;
}

void CC Animation::AnimateAll_40AC20(DynamicArrayT<Animation>* pAnims)
{
    for (auto i = 0; i < pAnims->Size(); i++)
    {
        Animation* pAnim = pAnims->ItemAt(i);
        if (!pAnim)
        {
            break;
        }

        if (pAnim->field_4_flags & 2)
        {
            if (pAnim->field_E_frame_change_counter > 0)
            {
                pAnim->field_E_frame_change_counter--;
                if (pAnim->field_E_frame_change_counter == 0)
                {
                    pAnim->Animation__vdecode_40AC90();
                }
            }
        }
    }
}

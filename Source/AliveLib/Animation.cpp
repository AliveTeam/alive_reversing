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

void BackgroundAnimation::ctor_40D270(BackgroundAnimation_Params* pPathParams, int a3)
{
    //BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5440F0);

    field_4_typeId = 7;
    field_F8_arg_a3 = a3;

    AnimHeader** pAnimHeader;
    /*
    AnimHeader** pAnimHeader = (AnimHeader **)BaseGameObject::Add_resource_4DC130(
        this,
        Resource_Animation,
        pPathParams->field_10_res_id);
        */

    field_F4_res = pAnimHeader;
    if (!pAnimHeader)
    {
        field_6_flags = field_6_flags & 0xFFF7 | 4;
    }

    const int x = pPathParams->field_8_xpos << 16;
    const int y = pPathParams->field_A_ypos << 16;

    field_B8_xpos = x;
    field_BC_ypos = y;

    field_FC_xpos = x;
    field_100_ypos = y;

    /*
    BaseAnimatedWithPhysicsGameObject_Animation_Init_424E10(
        (*pAnimHeader)->field_4_frame_table_offset,
        (*pAnimHeader)->field_0_max_w,
        (*pAnimHeader)->field_2_max_h,
        (AnimHeader *)pAnimHeader,
        1,
        1u);
    */

    //LOWORD(field_20_animation.field_4_flags) = field_20_animation.field_4_flags & 0xBFFF | ((pPathParams->field_12_is_semi_trans & 1) << 14);
    //BYTE1(field_20_animation.field_4_flags) |= 0x80u;

    field_20_animation.field_B_render_mode = static_cast<BYTE>(pPathParams->field_14_semi_trans_mode);

    const int v9 = pPathParams->field_1A_layer;
    if ((WORD)v9)
    {
        const int v10 = v9 - 1;
        if (!v10)
        {
            field_20_animation.field_C_render_layer = 20;
        }
        if (v10 == 1)
        {
            field_20_animation.field_C_render_layer = 39;
        }
    }
    else
    {
        field_20_animation.field_C_render_layer = 1;
    }
}

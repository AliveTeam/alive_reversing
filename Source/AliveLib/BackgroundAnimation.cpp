#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"

void BackgroundAnimation::ctor_40D270(BackgroundAnimation_Params* pPathParams, int a3)
{
    //BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5440F0);

    field_4_typeId = Types::eBackgroundAnimation;
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
        field_6_flags.Clear(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eDead);
    }

    field_B8_xpos = FP(pPathParams->field_8_xpos);
    field_BC_ypos = FP(pPathParams->field_A_ypos);

    field_FC_xpos = FP(pPathParams->field_8_xpos);
    field_100_ypos = FP(pPathParams->field_A_ypos);

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

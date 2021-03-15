#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"

BaseGameObject* BackgroundAnimation::VDestructor(signed int flags)
{
    return vdtor_40D420(flags);
}

void BackgroundAnimation::VUpdate()
{
    vUpdate_40D450();
}

void BackgroundAnimation::VScreenChanged()
{
    vScreenChanged_40D550();
}

BackgroundAnimation* BackgroundAnimation::ctor_40D270(Path_BackgroundAnimation* pPathParams, TlvItemInfoUnion tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5440F0); // vTbl_BackgroundAnimation_5440F0

    field_4_typeId = Types::eBackgroundAnimation_7;
    field_F8_tlvInfo = tlvInfo;

    field_F4_res = reinterpret_cast<AnimHeader**>(Add_Resource_4DC130(ResourceManager::Resource_Animation, pPathParams->field_10_res_id));
    if (!field_F4_res)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return this;
    }

    field_B8_xpos = FP_FromInteger(pPathParams->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pPathParams->field_8_top_left.field_2_y);

    field_FC_animXPos = FP_FromInteger(pPathParams->field_8_top_left.field_0_x);
    field_100_animYPos = FP_FromInteger(pPathParams->field_8_top_left.field_2_y);

    Animation_Init_424E10(
        (*field_F4_res)->field_4_frame_table_offset,
        (*field_F4_res)->field_0_max_w,
        (*field_F4_res)->field_2_max_h,
        reinterpret_cast<BYTE**>(field_F4_res),
        1,
        1u);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans, pPathParams->field_12_is_semi_trans & 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_20_animation.field_B_render_mode = pPathParams->field_14_semi_trans_mode;

    if (pPathParams->field_1A_layer > Layer::eLayer_0)
    {
        const int translatedLayer = static_cast<int>(pPathParams->field_1A_layer) - 1;
        if (!translatedLayer)
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_20;
        }
        if (translatedLayer == 1)
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_39;
        }
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_1;
    }
    return this;
}

void BackgroundAnimation::vUpdate_40D450()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        field_B8_xpos = FP_FromInteger(sTweakX_5C1BD0) + field_FC_animXPos;
        field_BC_ypos = FP_FromInteger(sTweakY_5C1BD4) + field_100_animYPos;
    }
}

void BackgroundAnimation::vScreenChanged_40D550()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void BackgroundAnimation::dtor_40D4C0()
{
    SetVTable(this, 0x5440F0); // vTbl_BackgroundAnimation_5440F0
    Path::TLV_Reset_4DB8E0(field_F8_tlvInfo.all, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BaseGameObject* BackgroundAnimation::vdtor_40D420(signed int flags)
{
    dtor_40D4C0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

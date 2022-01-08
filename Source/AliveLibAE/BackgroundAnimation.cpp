#include "stdafx.h"
#include "BackgroundAnimation.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"

BaseGameObject* BackgroundAnimation::VDestructor(s32 flags)
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

BackgroundAnimation* BackgroundAnimation::ctor_40D270(Path_BackgroundAnimation* pTlv, TlvItemInfoUnion tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5440F0); // vTbl_BackgroundAnimation_5440F0

    SetType(AETypes::eBackgroundAnimation_7);
    field_F8_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = BgAnimRec(pTlv->field_10_anim_id);
    field_F4_res = reinterpret_cast<AnimHeader**>(Add_Resource_4DC130(ResourceManager::Resource_Animation, anim.mBgAnimId));
    if (!field_F4_res)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return this;
    }


    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_FC_animXPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_100_animYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    Animation_Init_424E10(
        anim.mFrameTableOffset,
        anim.mMaxW,
        anim.mMaxH,
        reinterpret_cast<u8**>(field_F4_res),
        1,
        1u);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->field_12_is_semi_trans == Choice_short::eYes_1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_20_animation.field_B_render_mode = pTlv->field_14_semi_trans_mode;

    if (pTlv->field_1A_layer > Layer::eLayer_0)
    {
        const s32 translatedLayer = static_cast<s32>(pTlv->field_1A_layer) - 1;
        if (!translatedLayer)
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_Above_FG1_Half_20;
        }
        if (translatedLayer == 1)
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_Above_FG1_39;
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

BaseGameObject* BackgroundAnimation::vdtor_40D420(s32 flags)
{
    dtor_40D4C0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

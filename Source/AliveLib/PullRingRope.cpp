#include "stdafx.h"
#include "PullRingRope.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

const TintEntry sPullRingRopeTints_55FD1C[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


PullRingRope* PullRingRope::ctor_49B2D0(Path_PullRingRope* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546A00);

    field_4_typeId = Types::ePullRope_103;

    BYTE** v4 = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPullringResID);
    Animation_Init_424E10(3020, 35, 0x16u, v4, 1, 1);

    SetTint_425600(sPullRingRopeTints_55FD1C, gMap_5C3030.sCurrentLevelId_5C3030);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y + 24);

    field_102_id = pTlv->field_10_id;
    field_104_target_action = pTlv->field_12_target_action;
    field_110_tlvInfo = tlvInfo;
    field_100_state = 0;
    field_F4_stay_in_state_ticks = 0;

    const FP ropeLength = FP_FromInteger(pTlv->field_14_length_of_rope);
    field_BC_ypos += ropeLength;

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    field_106_on_sound = pTlv->field_18_on_sound;
    field_108_off_sound = pTlv->field_1A_off_sound;
    field_10A_sound_direction = pTlv->field_1C_sound_direction;
    field_FC_ring_puller_id = -1;

    auto pRope = alive_new<Rope>();
    if (pRope)
    {
        pRope->ctor_4A0A70(
            FP_GetExponent(field_B8_xpos + FP_FromInteger(2)),
            FP_GetExponent(field_BC_ypos - ropeLength),
            FP_GetExponent(field_BC_ypos),
            field_CC_sprite_scale);

        field_F8_rope_id = pRope->field_8_object_id;
    }

    pRope->field_BC_ypos = FP_NoFractional(field_BC_ypos- (field_CC_sprite_scale * FP_FromInteger(16)));

    field_DC_bApplyShadows |= 2;
    return this;
}

BaseGameObject* PullRingRope::VDestructor(signed int flags)
{
    return vdtor_49B630(flags);
}

void PullRingRope::VUpdate()
{
    vUpdate_49B720();
}

void PullRingRope::VScreenChanged()
{
    vScreenChanged_49BCB0();
}

__int16 PullRingRope::VPull_49BBD0(BaseGameObject* a2)
{
    return vPull_49BBD0(a2);
}

BOOL PullRingRope::Vsub_49BC90()
{
    return vsub_49BC90();
}

void PullRingRope::Vsub_49B610()
{
    return vsub_49B610();
}

PullRingRope* PullRingRope::vdtor_49B630(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

void PullRingRope::vUpdate_49B720()
{
    NOT_IMPLEMENTED();
}

void PullRingRope::vScreenChanged_49BCB0()
{
    NOT_IMPLEMENTED();
}

__int16 PullRingRope::vPull_49BBD0(BaseGameObject* /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BOOL PullRingRope::vsub_49BC90()
{
    return field_100_state != 1;
}

void PullRingRope::vsub_49B610()
{
    field_10C |= 1u;
}

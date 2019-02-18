#include "stdafx.h"
#include "PullRingRope.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "Function.hpp"
#include "ObjectIds.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "PathData.hpp"
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
    field_100_state = State::eState_0;
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

PullRingRope* PullRingRope::vdtor_49B630(signed int flags)
{
    dtor_49B660();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void PullRingRope::dtor_49B660()
{
    SetVTable(this, 0x546A00);

    Path::TLV_Reset_4DB8E0(field_110_tlvInfo, -1, 0, 0);
    
    BaseGameObject* pRope = sObjectIds_5C1B70.Find_449CF0(field_F8_rope_id);
    if (pRope)
    {
        pRope->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void PullRingRope::vUpdate_49B720()
{
    BaseAliveGameObject* pRingPuller = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_FC_ring_puller_id));
    Rope* pRope = static_cast<Rope*>(sObjectIds_5C1B70.Find_449CF0(field_F8_rope_id));
    
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    // Invalidate ring puller if they've died
    if (pRingPuller && pRingPuller->field_6_flags.Get(BaseGameObject::eDead))
    {
        field_FC_ring_puller_id = -1;
    }

    switch (field_100_state)
    {
    case State::eState_1:
        if (field_20_animation.field_92_current_frame == 2)
        {
            SFX_Play_46FA90(56u, 0, 0x10000);
        }

        field_BC_ypos += field_C8_vely;
        pRingPuller->field_BC_ypos += field_C8_vely;
        field_F4_stay_in_state_ticks--;

        if (field_F4_stay_in_state_ticks == 0)
        {
            field_C8_vely = FP_FromInteger(0);
            field_10C &= ~1u;
            field_100_state = State::eState_2;

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_6 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_9)
            {
                SFX_Play_46FA90(0x50u, 0, 0x10000);
            }
        }
        break;

    case State::eState_2:
        if (field_10C & 1)
        {
            field_C8_vely = FP_FromInteger(4) * field_CC_sprite_scale;
            field_FC_ring_puller_id = -1;
            field_100_state = State::eState_3;
            field_F4_stay_in_state_ticks = 3;
            field_20_animation.Set_Animation_Data_409C80(3092, 0);

            const int oldSwitchValue = SwitchStates_Get_466020(field_102_id);
            SwitchStates_Do_Operation_465F00(field_102_id, field_104_target_action);
            if (oldSwitchValue != SwitchStates_Get_466020(field_102_id))
            {
                int leftVol = 0;
                int rightVol = 0;
                if (field_10A_sound_direction == 1)
                {
                    leftVol = 1;
                    rightVol = 0;
                }
                else
                {
                    leftVol = field_10A_sound_direction != 2;
                    rightVol = 1;
                }

                if (SwitchStates_Get_466020(field_102_id))
                {
                    switch (field_106_on_sound)
                    {
                    case 1:
                        SFX_Play_46FB10(20u, 60 * leftVol + 10, 60 * rightVol + 10, 0x10000);
                        break;
                    case 2:
                        SFX_Play_46FB10(8u, 60 * leftVol + 10, 60 * rightVol + 10, 0x10000);
                        break;
                    case 3:
                        SFX_Play_46FB10(57u, 75 * leftVol + 15, 75 * rightVol + 15, 0x10000);
                        break;
                    }
                }
                else
                {
                    switch (field_108_off_sound)
                    {
                    case 1:
                        SFX_Play_46FB10(20u, 60 * leftVol + 10, 60 * rightVol + 10, 0x10000);
                        break;
                    case 2:
                        SFX_Play_46FB10(8u, 60 * leftVol + 10, 60 * rightVol + 10, 0x10000);
                        break;
                    case 3:
                        SFX_Play_46FB10(57u, 75 * leftVol + 15, 75 * rightVol + 15, 0x10000);
                        break;
                    }
                }
            }
        }
        break;

    case State::eState_3:
        field_BC_ypos -= field_C8_vely;
        field_F4_stay_in_state_ticks--;
        if (field_F4_stay_in_state_ticks == 0)
        {
            field_C8_vely = FP_FromInteger(0);
            field_100_state = State::eState_0;
            field_20_animation.Set_Animation_Data_409C80(3020, 0);
        }
        break;

    }

    if (pRope)
    {
        pRope->field_BC_ypos = FP_NoFractional(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(16)));
    }
}

void PullRingRope::vScreenChanged_49BCB0()
{
    // If the person pulling the rope is gone then so are we
    if (!sObjectIds_5C1B70.Find_449CF0(field_FC_ring_puller_id))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

__int16 PullRingRope::vPull_49BBD0(BaseGameObject* pObj)
{
    if (!pObj || field_100_state != State::eState_0)
    {
        return 0;
    }

    field_FC_ring_puller_id = pObj->field_8_object_id;
    field_100_state = State::eState_1;
    field_C8_vely = FP_FromInteger(2) * field_CC_sprite_scale;
    field_F4_stay_in_state_ticks = 6;
    field_20_animation.Set_Animation_Data_409C80(3060, 0);
    SFX_Play_46FA90(56u, 0, 0x10000);
    return 1;

}

BOOL PullRingRope::vsub_49BC90()
{
    return field_100_state != State::eState_1;
}

void PullRingRope::vsub_49B610()
{
    field_10C |= 1u;
}

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

const TintEntry sPullRingRopeTints_55FD1C[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};


PullRingRope* PullRingRope::ctor_49B2D0(Path_PullRingRope* pTlv, s32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546A00);

    SetType(AETypes::ePullRope_103);

    const AnimRecord& rec = AnimRec(AnimId::PullRingRope_Idle);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    SetTint_425600(sPullRingRopeTints_55FD1C, gMap_5C3030.field_0_current_level);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y + 24);

    field_102_switch_id = pTlv->field_10_switch_id;
    field_104_action = pTlv->field_12_action;
    field_110_tlvInfo = tlvInfo;
    field_100_state = States::eIdle_0;
    field_F4_stay_in_state_ticks = 0;

    field_BC_ypos += FP_FromInteger(pTlv->field_14_rope_length);

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_8;
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_27;
        field_D6_scale = 1;
    }

    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    field_106_on_sound = pTlv->field_18_on_sound;
    field_108_off_sound = pTlv->field_1A_off_sound;
    field_10A_sound_direction = pTlv->field_1C_sound_direction;
    field_FC_ring_puller_id = -1;

    auto pRope = ae_new<Rope>();
    if (pRope)
    {
        pRope->ctor_4A0A70(
            FP_GetExponent(field_B8_xpos + FP_FromInteger(2)),
            FP_GetExponent(field_BC_ypos) - pTlv->field_14_rope_length,
            FP_GetExponent(field_BC_ypos),
            field_CC_sprite_scale);

        field_F8_rope_id = pRope->field_8_object_id;
        pRope->field_BC_ypos = FP_NoFractional(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(16)));
    }

    field_DC_bApplyShadows |= 2;
    return this;
}

BaseGameObject* PullRingRope::VDestructor(s32 flags)
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

s16 PullRingRope::VPull_49BBD0(BaseGameObject* pObj)
{
    return vPull_49BBD0(pObj);
}

Bool32 PullRingRope::VIsNotBeingPulled_49BC90()
{
    return vIsNotBeingPulled_49BC90();
}

void PullRingRope::VMarkAsPulled_49B610()
{
    return vMarkAsPulled_49B610();
}

PullRingRope* PullRingRope::vdtor_49B630(s32 flags)
{
    dtor_49B660();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void PullRingRope::dtor_49B660()
{
    SetVTable(this, 0x546A00);

    Path::TLV_Reset_4DB8E0(field_110_tlvInfo, -1, 0, 0);

    BaseGameObject* pRope = sObjectIds_5C1B70.Find(field_F8_rope_id, AETypes::eLiftRope_108);
    if (pRope)
    {
        pRope->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void PullRingRope::vUpdate_49B720()
{
    auto pRingPuller = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_FC_ring_puller_id));
    auto pRope = static_cast<Rope*>(sObjectIds_5C1B70.Find(field_F8_rope_id, AETypes::eLiftRope_108));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    // Invalidate ring puller if they've died
    if (pRingPuller && pRingPuller->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_FC_ring_puller_id = -1;
    }

    switch (field_100_state)
    {
        case States::eBeingPulled_1:
            if (field_20_animation.field_92_current_frame == 2)
            {
                SFX_Play_46FA90(SoundEffect::RingRopePull_56, 0);
            }

            field_BC_ypos += field_C8_vely;
            pRingPuller->field_BC_ypos += field_C8_vely;
            field_F4_stay_in_state_ticks--;

            if (field_F4_stay_in_state_ticks == 0)
            {
                field_C8_vely = FP_FromInteger(0);
                field_10C_is_pulled &= ~1u;
                field_100_state = States::eTriggerEvent_2;

                if (gMap_5C3030.field_0_current_level == LevelIds::eMines_1 || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8 || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5 || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6 || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9)
                {
                    SFX_Play_46FA90(SoundEffect::IndustrialTrigger_80, 0);
                }
            }
            break;

        case States::eTriggerEvent_2:
            if (field_10C_is_pulled & 1)
            {
                field_C8_vely = FP_FromInteger(4) * field_CC_sprite_scale;
                field_FC_ring_puller_id = -1;
                field_100_state = States::eReturnToIdle_3;
                field_F4_stay_in_state_ticks = 3;
                const AnimRecord& rec = AnimRec(AnimId::PullRingRope_UseEnd);
                field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, 0);

                const s32 oldSwitchValue = SwitchStates_Get_466020(field_102_switch_id);
                SwitchStates_Do_Operation_465F00(field_102_switch_id, field_104_action);
                if (oldSwitchValue != SwitchStates_Get_466020(field_102_switch_id))
                {
                    s32 leftVol = 0;
                    s32 rightVol = 0;
                    if (field_10A_sound_direction == PullRingSoundDirection::eLeft_1)
                    {
                        leftVol = 1;
                        rightVol = 0;
                    }
                    else if (field_10A_sound_direction == PullRingSoundDirection::eRight_2)
                    {
                        leftVol = 0;
                        rightVol = 1;
                    }
                    else
                    {
                        leftVol = 1;
                        rightVol = 1;
                    }

                    if (SwitchStates_Get_466020(field_102_switch_id))
                    {
                        switch (field_106_on_sound)
                        {
                            case PullRingSwitchSound::eNone_0:
                                // don't play additional sound effects
                                break;
                            case PullRingSwitchSound::eWellExit_1:
                                SFX_Play_46FB10(SoundEffect::WellExit_20, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eRingUnknownTrigger_2: // ring bell hammer from AO?
                                SFX_Play_46FB10(SoundEffect::RingUnknownTrigger_8, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_46FB10(SoundEffect::DoorEffect_57, 75 * leftVol + 15, 75 * rightVol + 15);
                                break;
                        }
                    }
                    else
                    {
                        switch (field_108_off_sound)
                        {
                            case PullRingSwitchSound::eNone_0:
                                // don't play additional sound effects
                                break;
                            case PullRingSwitchSound::eWellExit_1:
                                SFX_Play_46FB10(SoundEffect::WellExit_20, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eRingUnknownTrigger_2:
                                SFX_Play_46FB10(SoundEffect::RingUnknownTrigger_8, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_46FB10(SoundEffect::DoorEffect_57, 75 * leftVol + 15, 75 * rightVol + 15);
                                break;
                        }
                    }
                }
            }
            break;

        case States::eReturnToIdle_3:
            field_BC_ypos -= field_C8_vely;
            field_F4_stay_in_state_ticks--;
            if (field_F4_stay_in_state_ticks == 0)
            {
                field_C8_vely = FP_FromInteger(0);
                field_100_state = States::eIdle_0;
                const AnimRecord& rec = AnimRec(AnimId::PullRingRope_Idle);
                field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, 0);
            }
            break;

        default:
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
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

s16 PullRingRope::vPull_49BBD0(BaseGameObject* pObj)
{
    if (!pObj || field_100_state != States::eIdle_0)
    {
        return 0;
    }

    field_FC_ring_puller_id = pObj->field_8_object_id;
    field_100_state = States::eBeingPulled_1;
    field_C8_vely = FP_FromInteger(2) * field_CC_sprite_scale;
    field_F4_stay_in_state_ticks = 6;
    const AnimRecord& rec = AnimRec(AnimId::PullRingRope_UseBegin);
    field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, 0);
    SFX_Play_46FA90(SoundEffect::RingRopePull_56, 0);
    return 1;
}

Bool32 PullRingRope::vIsNotBeingPulled_49BC90()
{
    return field_100_state != States::eBeingPulled_1;
}

void PullRingRope::vMarkAsPulled_49B610()
{
    field_10C_is_pulled |= 1u;
}

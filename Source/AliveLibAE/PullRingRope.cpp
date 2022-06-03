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
#include "Grid.hpp"

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


PullRingRope::PullRingRope(Path_PullRingRope* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::ePullRope_103);

    const AnimRecord& rec = AnimRec(AnimId::PullRingRope_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    SetTint(sPullRingRopeTints_55FD1C, gMap.mCurrentLevel);

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

    field_B8_xpos = FP_FromInteger(SnapToXGrid(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    field_106_on_sound = pTlv->field_18_on_sound;
    field_108_off_sound = pTlv->field_1A_off_sound;
    field_10A_sound_direction = pTlv->field_1C_sound_direction;
    field_FC_ring_puller_id = -1;

    auto pRope = ae_new<Rope>(FP_GetExponent(field_B8_xpos + FP_FromInteger(2)),
                              FP_GetExponent(field_BC_ypos) - pTlv->field_14_rope_length,
                              FP_GetExponent(field_BC_ypos),
                              field_CC_sprite_scale);
    if (pRope)
    {
        field_F8_rope_id = pRope->field_8_object_id;
        pRope->field_BC_ypos = FP_NoFractional(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(16)));
    }

    field_DC_bApplyShadows |= 2;
}

PullRingRope::~PullRingRope()
{
    Path::TLV_Reset(field_110_tlvInfo, -1, 0, 0);

    BaseGameObject* pRope = sObjectIds.Find(field_F8_rope_id, AETypes::eLiftRope_108);
    if (pRope)
    {
        pRope->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void PullRingRope::VUpdate()
{
    auto pRingPuller = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_FC_ring_puller_id));
    auto pRope = static_cast<Rope*>(sObjectIds.Find(field_F8_rope_id, AETypes::eLiftRope_108));

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    // Invalidate ring puller if they've died
    if (pRingPuller && pRingPuller->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_FC_ring_puller_id = -1;
    }

    switch (field_100_state)
    {
        case States::eBeingPulled_1:
            if (field_20_animation.field_92_current_frame == 2)
            {
                SFX_Play_Mono(SoundEffect::RingRopePull_56, 0);
            }

            field_BC_ypos += field_C8_vely;
            pRingPuller->field_BC_ypos += field_C8_vely;
            field_F4_stay_in_state_ticks--;

            if (field_F4_stay_in_state_ticks == 0)
            {
                field_C8_vely = FP_FromInteger(0);
                field_10C_is_pulled &= ~1u;
                field_100_state = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == LevelIds::eMines_1 || gMap.mCurrentLevel == LevelIds::eBonewerkz_8 || gMap.mCurrentLevel == LevelIds::eFeeCoDepot_5 || gMap.mCurrentLevel == LevelIds::eBarracks_6 || gMap.mCurrentLevel == LevelIds::eBrewery_9)
                {
                    SFX_Play_Mono(SoundEffect::IndustrialTrigger_80, 0);
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
                field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, 0);

                const s32 oldSwitchValue = SwitchStates_Get(field_102_switch_id);
                SwitchStates_Do_Operation(field_102_switch_id, field_104_action);
                if (oldSwitchValue != SwitchStates_Get(field_102_switch_id))
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

                    if (SwitchStates_Get(field_102_switch_id))
                    {
                        switch (field_106_on_sound)
                        {
                            case PullRingSwitchSound::eNone_0:
                                // don't play additional sound effects
                                break;
                            case PullRingSwitchSound::eWellExit_1:
                                SFX_Play_Stereo(SoundEffect::WellExit_20, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eRingUnknownTrigger_2: // ring bell hammer from AO?
                                SFX_Play_Stereo(SoundEffect::RingUnknownTrigger_8, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_Stereo(SoundEffect::DoorEffect_57, 75 * leftVol + 15, 75 * rightVol + 15);
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
                                SFX_Play_Stereo(SoundEffect::WellExit_20, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eRingUnknownTrigger_2:
                                SFX_Play_Stereo(SoundEffect::RingUnknownTrigger_8, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_Stereo(SoundEffect::DoorEffect_57, 75 * leftVol + 15, 75 * rightVol + 15);
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
                field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, 0);
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

void PullRingRope::VScreenChanged()
{
    // If the person pulling the rope is gone then so are we
    if (!sObjectIds.Find_Impl(field_FC_ring_puller_id))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 PullRingRope::VPull(BaseGameObject* pObj)
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
    field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, 0);
    SFX_Play_Mono(SoundEffect::RingRopePull_56, 0);
    return 1;
}

Bool32 PullRingRope::VIsNotBeingPulled()
{
    return field_100_state != States::eBeingPulled_1;
}

void PullRingRope::VMarkAsPulled()
{
    field_10C_is_pulled |= 1u;
}

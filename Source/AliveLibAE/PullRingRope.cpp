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
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};


PullRingRope::PullRingRope(Path_PullRingRope* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::ePullRope);

    const AnimRecord& rec = AnimRec(AnimId::PullRingRope_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    SetTint(sPullRingRopeTints_55FD1C, gMap.mCurrentLevel);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y + 24);

    field_102_switch_id = pTlv->field_10_switch_id;
    field_104_action = pTlv->field_12_action;
    field_110_tlvInfo = tlvInfo;
    field_100_state = States::eIdle_0;
    field_F4_stay_in_state_ticks = 0;

    mBaseAnimatedWithPhysicsGameObject_YPos += FP_FromInteger(pTlv->field_14_rope_length);

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));

    field_106_on_sound = pTlv->field_18_on_sound;
    field_108_off_sound = pTlv->field_1A_off_sound;
    field_10A_sound_direction = pTlv->field_1C_sound_direction;
    field_FC_ring_puller_id = -1;

    auto pRope = relive_new Rope(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(2)),
                              FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) - pTlv->field_14_rope_length,
                              FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                              mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    if (pRope)
    {
        field_F8_rope_id = pRope->field_8_object_id;
        pRope->mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(16)));
    }

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

PullRingRope::~PullRingRope()
{
    Path::TLV_Reset(field_110_tlvInfo, -1, 0, 0);

    BaseGameObject* pRope = sObjectIds.Find(field_F8_rope_id, ReliveTypes::eLiftRope);
    if (pRope)
    {
        pRope->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void PullRingRope::VUpdate()
{
    auto pRingPuller = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_FC_ring_puller_id));
    auto pRope = static_cast<Rope*>(sObjectIds.Find(field_F8_rope_id, ReliveTypes::eLiftRope));

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
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2)
            {
                SFX_Play_Mono(SoundEffect::RingRopePull_56, 0);
            }

            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            pRingPuller->mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            field_F4_stay_in_state_ticks--;

            if (field_F4_stay_in_state_ticks == 0)
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_10C_is_pulled &= ~1u;
                field_100_state = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                {
                    SFX_Play_Mono(SoundEffect::IndustrialTrigger_80, 0);
                }
            }
            break;

        case States::eTriggerEvent_2:
            if (field_10C_is_pulled & 1)
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(4) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                field_FC_ring_puller_id = -1;
                field_100_state = States::eReturnToIdle_3;
                field_F4_stay_in_state_ticks = 3;
                const AnimRecord& rec = AnimRec(AnimId::PullRingRope_UseEnd);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, 0);

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
            mBaseAnimatedWithPhysicsGameObject_YPos -= mBaseAnimatedWithPhysicsGameObject_VelY;
            field_F4_stay_in_state_ticks--;
            if (field_F4_stay_in_state_ticks == 0)
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_100_state = States::eIdle_0;
                const AnimRecord& rec = AnimRec(AnimId::PullRingRope_Idle);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, 0);
            }
            break;

        default:
            break;
    }

    if (pRope)
    {
        pRope->mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(16)));
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
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(2) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_F4_stay_in_state_ticks = 6;
    const AnimRecord& rec = AnimRec(AnimId::PullRingRope_UseBegin);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, 0);
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

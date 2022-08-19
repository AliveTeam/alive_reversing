#include "stdafx.h"
#include "PullRingRope.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "Function.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
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
    Animation_Init(AnimId::PullRingRope_Idle, ppRes);

    SetTint(sPullRingRopeTints_55FD1C, gMap.mCurrentLevel);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y + 24);

    field_102_switch_id = pTlv->mSwitchId;
    field_104_action = pTlv->mAction;
    field_110_tlvInfo = tlvInfo;
    field_100_state = States::eIdle_0;
    field_F4_stay_in_state_ticks = 0;

    mYPos += FP_FromInteger(pTlv->mRopeLength);

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));

    field_106_on_sound = pTlv->mOnSound;
    field_108_off_sound = pTlv->mOffSound;
    field_10A_sound_direction = pTlv->mSoundDirection;
    field_FC_ring_puller_id = -1;

    auto pRope = relive_new Rope(FP_GetExponent(mXPos + FP_FromInteger(2)),
                              FP_GetExponent(mYPos) - pTlv->mRopeLength,
                              FP_GetExponent(mYPos),
                              mSpriteScale);
    if (pRope)
    {
        field_F8_rope_id = pRope->mBaseGameObjectId;
        pRope->mYPos = FP_NoFractional(mYPos - (mSpriteScale * FP_FromInteger(16)));
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

    if (EventGet(kEventDeathReset))
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
            if (mAnim.mCurrentFrame == 2)
            {
                SfxPlayMono(SoundEffect::RingRopePull_56, 0);
            }

            mYPos += mVelY;
            pRingPuller->mYPos += mVelY;
            field_F4_stay_in_state_ticks--;

            if (field_F4_stay_in_state_ticks == 0)
            {
                mVelY = FP_FromInteger(0);
                field_10C_is_pulled &= ~1u;
                field_100_state = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                {
                    SfxPlayMono(SoundEffect::IndustrialTrigger_80, 0);
                }
            }
            break;

        case States::eTriggerEvent_2:
            if (field_10C_is_pulled & 1)
            {
                mVelY = FP_FromInteger(4) * mSpriteScale;
                field_FC_ring_puller_id = -1;
                field_100_state = States::eReturnToIdle_3;
                field_F4_stay_in_state_ticks = 3;
                mAnim.Set_Animation_Data(AnimId::PullRingRope_UseEnd, nullptr);

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
                            case PullRingSwitchSound::eRingBellHammer_2: // ring bell hammer from AO?
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
                            case PullRingSwitchSound::eRingBellHammer_2:
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
            mYPos -= mVelY;
            field_F4_stay_in_state_ticks--;
            if (field_F4_stay_in_state_ticks == 0)
            {
                mVelY = FP_FromInteger(0);
                field_100_state = States::eIdle_0;
                mAnim.Set_Animation_Data(AnimId::PullRingRope_Idle, nullptr);
            }
            break;

        default:
            break;
    }

    if (pRope)
    {
        pRope->mYPos = FP_NoFractional(mYPos - (mSpriteScale * FP_FromInteger(16)));
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

    field_FC_ring_puller_id = pObj->mBaseGameObjectId;
    field_100_state = States::eBeingPulled_1;
    mVelY = FP_FromInteger(2) * mSpriteScale;
    field_F4_stay_in_state_ticks = 6;
    mAnim.Set_Animation_Data(AnimId::PullRingRope_UseBegin, nullptr);
    SfxPlayMono(SoundEffect::RingRopePull_56, 0);
    return 1;
}

bool PullRingRope::VIsNotBeingPulled()
{
    return field_100_state != States::eBeingPulled_1;
}

void PullRingRope::VMarkAsPulled()
{
    field_10C_is_pulled |= 1u;
}

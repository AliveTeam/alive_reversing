#include "stdafx.h"
#include "PullRingRope.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "Function.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"

const TintEntry sPullRingRopeTints_55FD1C[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
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
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void PullRingRope::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::PullRingRope_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::PullRingRope_UseBegin));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::PullRingRope_UseEnd));
}

PullRingRope::PullRingRope(relive::Path_PullRingRope* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::ePullRingRope);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::PullRingRope_Idle));

    SetTint(sPullRingRopeTints_55FD1C, gMap.mCurrentLevel);

    GetAnimation().SetSemiTrans(true);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);

    field_102_switch_id = pTlv->mSwitchId;
    field_104_action = pTlv->mAction;
    field_110_tlvInfo = tlvId;
    field_100_state = States::eIdle_0;
    field_F4_stay_in_state_ticks = 0;

    mYPos += FP_FromInteger(pTlv->mRopeLength);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        SetScale(Scale::Fg);
    }

    mXPos = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)));

    field_106_on_sound = pTlv->mOnSound;
    field_108_off_sound = pTlv->mOffSound;
    field_10A_sound_direction = pTlv->mSoundDirection;
    field_FC_ring_puller_id = Guid{};

    auto pRope = relive_new Rope(FP_GetExponent(mXPos + FP_FromInteger(2)),
                              FP_GetExponent(mYPos) - pTlv->mRopeLength,
                              FP_GetExponent(mYPos),
                              GetSpriteScale());
    if (pRope)
    {
        field_F8_rope_id = pRope->mBaseGameObjectId;
        pRope->mYPos = FP_NoFractional(mYPos - (GetSpriteScale() * FP_FromInteger(16)));
    }

    SetDoPurpleLightEffect(true);
}

PullRingRope::~PullRingRope()
{
    Path::TLV_Reset(field_110_tlvInfo, -1, 0, 0);

    BaseGameObject* pRope = sObjectIds.Find(field_F8_rope_id, ReliveTypes::eRope);
    if (pRope)
    {
        pRope->SetDead(true);
    }
}

void PullRingRope::VUpdate()
{
    auto pRingPuller = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_FC_ring_puller_id));
    auto pRope = static_cast<Rope*>(sObjectIds.Find(field_F8_rope_id, ReliveTypes::eRope));

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    // Invalidate ring puller if they've died
    if (pRingPuller && pRingPuller->GetDead())
    {
        field_FC_ring_puller_id = Guid{};
    }

    switch (field_100_state)
    {
        case States::eBeingPulled_1:
            if (GetAnimation().GetCurrentFrame() == 2)
            {
                SfxPlayMono(relive::SoundEffects::RingRopePull, 0);
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
                    SfxPlayMono(relive::SoundEffects::IndustrialTrigger, 0);
                }
            }
            break;

        case States::eTriggerEvent_2:
            if (field_10C_is_pulled & 1)
            {
                mVelY = FP_FromInteger(4) * GetSpriteScale();
                field_FC_ring_puller_id = Guid{};
                field_100_state = States::eReturnToIdle_3;
                field_F4_stay_in_state_ticks = 3;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::PullRingRope_UseEnd));

                const s32 oldSwitchValue = SwitchStates_Get(field_102_switch_id);
                SwitchStates_Do_Operation(field_102_switch_id, field_104_action);
                if (oldSwitchValue != SwitchStates_Get(field_102_switch_id))
                {
                    s32 leftVol = 0;
                    s32 rightVol = 0;
                    if (field_10A_sound_direction == relive::Path_PullRingRope::PullRingSoundDirection::eLeft)
                    {
                        leftVol = 1;
                        rightVol = 0;
                    }
                    else if (field_10A_sound_direction == relive::Path_PullRingRope::PullRingSoundDirection::eRight)
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
                            case relive::Path_PullRingRope::PullRingSwitchSound::eNone:
                                // don't play additional sound effects
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eWellExit:
                                SFX_Play_Stereo(relive::SoundEffects::WellExit, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer: // ring bell hammer from AO?
                                SFX_Play_Stereo(relive::SoundEffects::RingUnknownTrigger, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect:
                                SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * leftVol + 15, 75 * rightVol + 15);
                                break;
                        }
                    }
                    else
                    {
                        switch (field_108_off_sound)
                        {
                            case relive::Path_PullRingRope::PullRingSwitchSound::eNone:
                                // don't play additional sound effects
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eWellExit:
                                SFX_Play_Stereo(relive::SoundEffects::WellExit, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer:
                                SFX_Play_Stereo(relive::SoundEffects::RingUnknownTrigger, 60 * leftVol + 10, 60 * rightVol + 10);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect:
                                SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * leftVol + 15, 75 * rightVol + 15);
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
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::PullRingRope_Idle));
            }
            break;

        default:
            break;
    }

    if (pRope)
    {
        pRope->mYPos = FP_NoFractional(mYPos - (GetSpriteScale() * FP_FromInteger(16)));
    }
}

void PullRingRope::VScreenChanged()
{
    // If the person pulling the rope is gone then so are we
    if (!sObjectIds.Find_Impl(field_FC_ring_puller_id))
    {
        SetDead(true);
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
    mVelY = FP_FromInteger(2) * GetSpriteScale();
    field_F4_stay_in_state_ticks = 6;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::PullRingRope_UseBegin));
    SfxPlayMono(relive::SoundEffects::RingRopePull, 0);
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

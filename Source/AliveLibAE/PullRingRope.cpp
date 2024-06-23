#include "stdafx.h"
#include "PullRingRope.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/GameObjects/Rope.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

static const TintEntry sPullRingRopeTints[16] = {
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

    SetTint(sPullRingRopeTints, gMap.mCurrentLevel);

    GetAnimation().SetSemiTrans(true);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);

    mSwitchId = pTlv->mSwitchId;
    mAction = pTlv->mAction;
    mTlvId = tlvId;
    mState = States::eIdle_0;
    mStayInStateTicks = 0;

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

    mXPos = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)));

    mOnSound = pTlv->mOnSound;
    mOffSound = pTlv->mOffSound;
    mSoundDirection = pTlv->mSoundDirection;
    mRingPullerId = Guid{};

    auto pRope = relive_new Rope(FP_GetExponent(mXPos + FP_FromInteger(2)),
                              FP_GetExponent(mYPos) - pTlv->mRopeLength,
                              FP_GetExponent(mYPos),
                              GetSpriteScale());
    if (pRope)
    {
        mRopeId = pRope->mBaseGameObjectId;
        pRope->mYPos = FP_NoFractional(mYPos - (GetSpriteScale() * FP_FromInteger(16)));
    }

    SetDoPurpleLightEffect(true);
}

PullRingRope::~PullRingRope()
{
    Path::TLV_Reset(mTlvId);

    BaseGameObject* pRope = sObjectIds.Find(mRopeId, ReliveTypes::eRope);
    if (pRope)
    {
        pRope->SetDead(true);
    }
}

void PullRingRope::VUpdate()
{
    auto pRingPuller = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mRingPullerId));
    auto pRope = static_cast<Rope*>(sObjectIds.Find(mRopeId, ReliveTypes::eRope));

    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    // Invalidate ring puller if they've died
    if (pRingPuller && pRingPuller->GetDead())
    {
        mRingPullerId = Guid{};
    }

    switch (mState)
    {
        case States::eBeingPulled_1:
            if (GetAnimation().GetCurrentFrame() == 2)
            {
                SfxPlayMono(relive::SoundEffects::RingRopePull, 0);
            }

            mYPos += mVelY;
            pRingPuller->mYPos += mVelY;
            mStayInStateTicks--;

            if (mStayInStateTicks == 0)
            {
                mVelY = FP_FromInteger(0);
                mIsPulled = false;
                mState = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                {
                    SfxPlayMono(relive::SoundEffects::IndustrialTrigger, 0);
                }
            }
            break;

        case States::eTriggerEvent_2:
            if (mIsPulled)
            {
                mVelY = FP_FromInteger(4) * GetSpriteScale();
                mRingPullerId = Guid{};
                mState = States::eReturnToIdle_3;
                mStayInStateTicks = 3;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::PullRingRope_UseEnd));

                const s32 oldSwitchValue = SwitchStates_Get(mSwitchId);
                SwitchStates_Do_Operation(mSwitchId, mAction);
                if (oldSwitchValue != SwitchStates_Get(mSwitchId))
                {
                    s32 leftVol = 0;
                    s32 rightVol = 0;
                    if (mSoundDirection == relive::Path_PullRingRope::PullRingSoundDirection::eLeft)
                    {
                        leftVol = 1;
                        rightVol = 0;
                    }
                    else if (mSoundDirection == relive::Path_PullRingRope::PullRingSoundDirection::eRight)
                    {
                        leftVol = 0;
                        rightVol = 1;
                    }
                    else
                    {
                        leftVol = 1;
                        rightVol = 1;
                    }

                    if (SwitchStates_Get(mSwitchId))
                    {
                        switch (mOnSound)
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
                        switch (mOffSound)
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
            mStayInStateTicks--;
            if (mStayInStateTicks == 0)
            {
                mVelY = FP_FromInteger(0);
                mState = States::eIdle_0;
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
    if (!sObjectIds.Find_Impl(mRingPullerId))
    {
        SetDead(true);
    }
}

s16 PullRingRope::VPull(BaseGameObject* pObj)
{
    if (!pObj || mState != States::eIdle_0)
    {
        return 0;
    }

    mRingPullerId = pObj->mBaseGameObjectId;

    mState = States::eBeingPulled_1;
    mVelY = FP_FromInteger(2) * GetSpriteScale();
    mStayInStateTicks = 6;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::PullRingRope_UseBegin));
    SfxPlayMono(relive::SoundEffects::RingRopePull, 0);
    return 1;
}

bool PullRingRope::VIsNotBeingPulled()
{
    return mState != States::eBeingPulled_1;
}

void PullRingRope::VMarkAsPulled()
{
    mIsPulled = true;
}

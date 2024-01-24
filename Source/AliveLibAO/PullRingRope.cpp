#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "PullRingRope.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "../relive_lib/Events.hpp"
#include "Path.hpp"
#include "../relive_lib/SwitchStates.hpp"

namespace AO {

void PullRingRope::LoadAnimations()
{
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eRuptureFarms:
        case EReliveLevelIds::eBoardRoom:
        case EReliveLevelIds::eRuptureFarmsReturn:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Farms_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Farms_UseBegin));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Farms_UseEnd));
            break;

        case EReliveLevelIds::eDesert:
        case EReliveLevelIds::eDesertTemple:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_UseBegin));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_UseEnd));
            break;

        default:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_UseBegin));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_UseEnd));
            break;
    }
}

PullRingRope::PullRingRope(relive::Path_PullRingRope* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::ePullRingRope);

    LoadAnimations();

    s32 lvl_x_off = 0;
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eRuptureFarms:
        case EReliveLevelIds::eBoardRoom:
        case EReliveLevelIds::eRuptureFarmsReturn:
        {
            Animation_Init(GetAnimRes(AnimId::Pullring_Farms_Idle));
            lvl_x_off = -2;
            break;
        }

        case EReliveLevelIds::eDesert:
        {
            Animation_Init(GetAnimRes(AnimId::Pullring_Desert_Idle));
            lvl_x_off = 2;
            break;
        }

        default:
        {
            Animation_Init(GetAnimRes(AnimId::Pullring_Desert_Idle));
            lvl_x_off = 0;
            break;
        }
    }

    GetAnimation().SetSemiTrans(true);
    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
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

    mSoundDirection = pTlv->mSoundDirection;

    mOnSound = pTlv->mOnSound;
    mOffSound = pTlv->mOffSound;

    mRingPuller = nullptr;

    mRope = relive_new Rope(
        FP_GetExponent(mXPos + FP_FromInteger((lvl_x_off + 1))),
        FP_GetExponent(mYPos) - pTlv->mRopeLength,
        FP_GetExponent(mYPos + (FP_FromInteger(mYOffset))),
        GetSpriteScale());
    if (mRope)
    {
        mRope->mBaseGameObjectRefCount++;
    }
}

PullRingRope::~PullRingRope()
{
    Path::TLV_Reset(mTlvId);

    if (mRingPuller)
    {
        mRingPuller->mBaseGameObjectRefCount--;
    }

    if (mRope)
    {
        mRope->SetDead(true);
        mRope->mBaseGameObjectRefCount--;
    }
}

void PullRingRope::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mRingPuller)
    {
        if (mRingPuller->GetDead())
        {
            mRingPuller->mBaseGameObjectRefCount--;
            mRingPuller = nullptr;
        }
    }

    switch (mState)
    {
        case States::eBeingPulled_1:
            if (GetAnimation().GetCurrentFrame() == 2)
            {
                SfxPlayMono(relive::SoundEffects::RingRopePull, 0);
            }

            mYPos += mVelY;
            mRingPuller->mYPos += mVelY;
            mStayInStateTicks--;

            if (mStayInStateTicks == 0)
            {
                mVelY = FP_FromInteger(0);
                mState = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SfxPlayMono(relive::SoundEffects::IndustrialTrigger, 0);
                }

                const auto oldSwitchValue = SwitchStates_Get(mSwitchId);
                // TODO: OG bug - operation isn't applied to the switch ??
                const auto switchValAfterOperation = SwitchStates_Get(mSwitchId);

                // Due to seemingly OG bug this can never execute
                if (oldSwitchValue != switchValAfterOperation)
                {
                    s32 volLeft = 0;
                    s32 volRight = 0;
                    if (mSoundDirection == relive::Path_PullRingRope::PullRingSoundDirection::eLeft)
                    {
                        volLeft = 1;
                        volRight = 0;
                    }
                    else if (mSoundDirection == relive::Path_PullRingRope::PullRingSoundDirection::eRight)
                    {
                        volLeft =  0;
                        volRight = 1;
                    }
                    else
                    {
                        volLeft = 1;
                        volRight = 1;
                    }

                    if (SwitchStates_Get(mSwitchId))
                    {
                        switch (mOnSound)
                        {
                            case relive::Path_PullRingRope::PullRingSwitchSound::eWellExit:
                                SFX_Play_Stereo(relive::SoundEffects::WellExit, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer:
                                SFX_Play_Stereo(relive::SoundEffects::RingBellHammer, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect:
                                SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                                break;
                        }
                    }
                    else
                    {
                        switch (mOffSound)
                        {
                            case relive::Path_PullRingRope::PullRingSwitchSound::eWellExit:
                                SFX_Play_Stereo(relive::SoundEffects::WellExit, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer:
                                SFX_Play_Stereo(relive::SoundEffects::RingBellHammer, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect:
                                SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                                break;
                        }
                    }
                }
            }
            break;

        case States::eTriggerEvent_2:
            mVelY = FP_FromInteger(4);
            mState = States::eReturnToIdle_3;
            mRingPuller->mBaseGameObjectRefCount--;
            mRingPuller = nullptr;

            mStayInStateTicks = 3;

            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Pullring_Farms_UseEnd));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Pullring_Desert_UseEnd));
            }
            break;

        case States::eReturnToIdle_3:
            mYPos -= mVelY;
            mStayInStateTicks--;
            if (mStayInStateTicks == 0)
            {
                mVelY = FP_FromInteger(0);
                mState = States::eIdle_0;

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Pullring_Farms_Idle));
                }
                else
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Pullring_Desert_Idle));
                }
            }
            break;

        default:
            break;
    }

    mRope->mYPos = FP_NoFractional(FP_FromInteger(mYOffset - 16) + mYPos);
}

void PullRingRope::VScreenChanged()
{
    // If the person pulling the rope is gone then so are we
    if (!mRingPuller)
    {
        SetDead(true);
    }
}

s16 PullRingRope::Pull(BaseAliveGameObject* pFrom)
{
    if (!pFrom || mState != States::eIdle_0)
    {
        return 0;
    }

    mRingPuller = pFrom;
    mRingPuller->mBaseGameObjectRefCount++;

    mState = States::eBeingPulled_1;
    mVelY = FP_FromInteger(2);
    mStayInStateTicks = 6;

    SwitchStates_Do_Operation(mSwitchId, mAction);

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Pullring_Farms_UseBegin));
    }
    else
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Pullring_Desert_UseBegin));
    }
    SfxPlayMono(relive::SoundEffects::RingRopePull, 0);
    return 1;
}

bool PullRingRope::vIsNotBeingPulled()
{
    return mState != States::eBeingPulled_1;
}

} // namespace AO

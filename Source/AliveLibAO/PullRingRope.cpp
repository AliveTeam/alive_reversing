#include "stdafx_ao.h"
#include "Function.hpp"
#include "PullRingRope.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "../relive_lib/Events.hpp"

namespace AO {

void PullRingRope::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Farms_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Farms_UseBegin));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Farms_UseEnd));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_UseBegin));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Pullring_Desert_UseEnd));
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

    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);

    mSwitchId = pTlv->mSwitchId;
    mAction = pTlv->mAction;
    mTlvInfo = tlvId;
    mState = States::eIdle_0;
    field_E4_stay_in_state_ticks = 0;

    mYPos += FP_FromInteger(pTlv->mRopeLength + pTlv->mTopLeftY + 24);
    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        mScale = Scale::Fg;
    }

    field_100_sound_direction = pTlv->mSoundDirection;

    field_FC_on_sound = pTlv->mOnSound;
    field_FE_off_sound = pTlv->mOffSound;

    field_F4_pPuller = nullptr;

    field_F8_pRope = relive_new Rope(
        FP_GetExponent(mXPos + FP_FromInteger((lvl_x_off + 1))),
        FP_GetExponent(mYPos) - pTlv->mRopeLength,
        FP_GetExponent(mYPos + (FP_FromInteger(mYOffset))),
        mSpriteScale);
    if (field_F8_pRope)
    {
        field_F8_pRope->mBaseGameObjectRefCount++;
    }
}

bool PullRingRope::vIsNotBeingPulled()
{
    return mState != States::eBeingPulled_1;
}

void PullRingRope::VScreenChanged()
{
    if (!field_F4_pPuller)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

PullRingRope::~PullRingRope()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);

    if (field_F4_pPuller)
    {
        field_F4_pPuller->mBaseGameObjectRefCount--;
    }

    if (field_F8_pRope)
    {
        field_F8_pRope->mBaseGameObjectFlags.Set(Options::eDead);
        field_F8_pRope->mBaseGameObjectRefCount--;
    }
}

s16 PullRingRope::Pull(BaseAliveGameObject* pFrom)
{
    if (!pFrom)
    {
        return 0;
    }

    if (mState != States::eIdle_0)
    {
        return 0;
    }

    field_F4_pPuller = pFrom;
    field_F4_pPuller->mBaseGameObjectRefCount++;

    mState = States::eBeingPulled_1;
    mVelY = FP_FromInteger(2);
    field_E4_stay_in_state_ticks = 6;

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

void PullRingRope::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_F4_pPuller)
    {
        if (field_F4_pPuller->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_F4_pPuller->mBaseGameObjectRefCount--;
            field_F4_pPuller = nullptr;
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
            field_F4_pPuller->mYPos += mVelY;
            field_E4_stay_in_state_ticks--;

            if (field_E4_stay_in_state_ticks == 0)
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
                    if (field_100_sound_direction == relive::Path_PullRingRope::PullRingSoundDirection::eLeft)
                    {
                        volLeft = 1;
                        volRight = 0;
                    }
                    else if (field_100_sound_direction == relive::Path_PullRingRope::PullRingSoundDirection::eRight)
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
                        switch (field_FC_on_sound)
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
                        switch (field_FE_off_sound)
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
            field_F4_pPuller->mBaseGameObjectRefCount--;
            field_F4_pPuller = nullptr;

            field_E4_stay_in_state_ticks = 3;

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
            field_E4_stay_in_state_ticks--;
            if (field_E4_stay_in_state_ticks == 0)
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

    field_F8_pRope->mYPos = FP_NoFractional(FP_FromInteger(mYOffset - 16) + mYPos);
}

} // namespace AO

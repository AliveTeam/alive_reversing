#include "stdafx.h"
#include "SlapLock.hpp"
#include "SlapLockWhirlWind.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "AbilityRing.hpp"
#include "PossessionFlicker.hpp"
#include "../relive_lib/Particle.hpp"
#include "ParticleBurst.hpp"
#include "Map.hpp"

void SlapLock::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SlapLock_Initiate));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SlapLock_Punched));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SlapLock_Shaking));
}

SlapLock::SlapLock(relive::Path_SlapLock* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eLockedSoul);
    mSlapLockTlv = pTlv;
    mTlvInfo = tlvId;
    mBaseGameObjectTlvInfo = tlvId;

    LoadAnimations();

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }

    Animation_Init(GetAnimRes(AnimId::SlapLock_Initiate));

    if (mSlapLockTlv->mScale != relive::reliveScale::eFull)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }

    mState = SlapLockStates::eShaking_0;
    mTimer1 = (Math_NextRandom() & 7) + sGnFrame + 25;
    mAbilityRingId = Guid{};
    mPossessionFlickerId = Guid{};
    const FP midX = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mXPos = midX;

    const FP ypos = FP_FromInteger(pTlv->mBottomRightY);
    mYPos = ypos;

    mHasGhost = mSlapLockTlv->mHasGhost;

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSlapLock_OrbWhirlWind && static_cast<SlapLockWhirlWind*>(pObj)->SwitchId() == mSlapLockTlv->mTargetTombSwitchId2)
        {
            mHasGhost = relive::reliveChoice::eNo;
        }
    }

    if (SwitchStates_Get(pTlv->mTargetTombSwitchId2))
    {
        mHasGhost = relive::reliveChoice::eNo;
    }

    if (pTlv->mTlvSpecificMeaning == 0)
    {
        return;
    }

    GetAnimation().mFlags.Clear(AnimFlags::eRender);

    mTimer1 = sGnFrame + 60;
    mShinyParticleTimer = sGnFrame + 30;

    if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eYes)
    {
        mState = SlapLockStates::eEmitInvisibilityPowerupRing_4;
    }
    else
    {
        mState = SlapLockStates::eBroken_3;
    }
}

SlapLock::~SlapLock()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

s32 SlapLock::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const SlapLock_State*>(pBuffer);

    auto pTlv = static_cast<relive::Path_SlapLock*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));

    auto pSlapLock = relive_new SlapLock(pTlv, pState->mTlvInfo);
    if (pSlapLock)
    {
        pSlapLock->GetAnimation().mFlags.Set(AnimFlags::eRender, pState->mAnimRender & 1);

        pSlapLock->mTlvInfo = pState->mTlvInfo;

        pTlv->mTlvSpecificMeaning = pState->mTlvState;

        pSlapLock->mState = pState->mState;
        pSlapLock->mTimer1 = pState->mTimer1;
        pSlapLock->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eShot);
        pSlapLock->mShinyParticleTimer = pState->mShinyParticleTimer;
    }

    return sizeof(SlapLock_State);
}

void SlapLock::VScreenChanged()
{
    if (mState == SlapLockStates::eFlickerHero_5 || mState == SlapLockStates::eGiveInvisibilityFromFlicker_6)
    {
        GiveInvisibility();
    }
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void SlapLock::GiveInvisibility()
{
    mSlapLockTlv = static_cast<relive::Path_SlapLock*>(sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));
    if (sActiveHero)
    {
        sActiveHero->mInvisibilityDuration = mSlapLockTlv->mInvisibilityDuration;
        sActiveHero->mHaveShrykull = 0;
        sActiveHero->mHaveInvisibility = 1;
        sActiveHero->mRingPulseTimer = sGnFrame + 200000;
    }
}

s32 SlapLock::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<SlapLock_State*>(pSaveBuffer);

    pState->mType = AETypes::eLockedSoul_61;
    pState->mAnimRender = GetAnimation().mFlags.Get(AnimFlags::eRender) & 1;
    pState->mTlvInfo = mTlvInfo;
    pState->mTlvState = sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo)->mTlvSpecificMeaning;
    pState->mState = mState;
    pState->mTimer1 = mTimer1;
    pState->mShinyParticleTimer = mShinyParticleTimer;
    pState->mAbilityRingId = Guid{};

    if (mAbilityRingId == Guid{})
    {
        return sizeof(SlapLock_State);
    }

    BaseGameObject* pObj = sObjectIds.Find_Impl(mAbilityRingId);
    if (pObj)
    {
        pState->mAbilityRingId = pObj->mBaseGameObjectTlvInfo;
    }
    return sizeof(SlapLock_State);
}

void SlapLock::VUpdate()
{
    mSlapLockTlv = static_cast<relive::Path_SlapLock*>(sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
        {
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);

            if (mSlapLockTlv->mTlvSpecificMeaning)
            {
                SwitchStates_Do_Operation(mSlapLockTlv->mTargetTombSwitchId2, relive::reliveSwitchOp::eSetTrue);
            }

            if (mAbilityRingId != Guid{})
            {
                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == ReliveTypes::eAbilityRing && pObj->mBaseGameObjectTlvInfo == mAbilityRingId)
                    {
                        mAbilityRingId = pObj->mBaseGameObjectId;
                        break;
                    }
                }
            }
        }

        BaseGameObject* pRingObj = sObjectIds.Find_Impl(mAbilityRingId);
        BaseGameObject* pFlickerObj = sObjectIds.Find_Impl(mPossessionFlickerId);

        switch (mState)
        {
            case SlapLockStates::eShaking_0:
            {
                if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eYes)
                {
                    if (!(sGnFrame & 63))
                    {
                        AbilityRing::Factory(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * mSpriteScale),
                            RingTypes::eInvisible_Pulse_Large_8,
                            mSpriteScale);
                    }
                }

                if (static_cast<s32>(sGnFrame) <= mTimer1)
                {
                    return;
                }

                if (mHasGhost == relive::reliveChoice::eNo)
                {
                    return;
                }

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SlapLock_Shaking));

                mState = SlapLockStates::eIdle_1;
                SfxPlayMono(relive::SoundEffects::SpiritLockShake, 0);
                return;
            }
            case SlapLockStates::eIdle_1:
            {
                if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eYes)
                {
                    if (!(sGnFrame & 63))
                    {
                        AbilityRing::Factory(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * mSpriteScale),
                            RingTypes::eInvisible_Pulse_Large_8,
                            mSpriteScale);
                    }
                }

                if (!(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
                {
                    return;
                }

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SlapLock_Initiate));

                mState = SlapLockStates::eShaking_0;
                mTimer1 = Math_NextRandom() + sGnFrame + 25;
                return;
            }
            case SlapLockStates::eSlapped_2:
            {
                if (!(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
                {
                    return;
                }

                GetAnimation().mFlags.Clear(AnimFlags::eRender);

                if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eNo)
                {
                    mShinyParticleTimer = sGnFrame + 60;
                    mState = SlapLockStates::eBroken_3;
                    break;
                }
                else
                {
                    mState = SlapLockStates::eFlickerHero_5;
                    return;
                }
                break;
            }
            case SlapLockStates::eBroken_3:
            {
                if (static_cast<s32>(sGnFrame) <= mShinyParticleTimer)
                {
                    return;
                }

                New_TintShiny_Particle(
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-2, 2)) + FP_FromInteger(1))) + mXPos,
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-3, 3)) - FP_FromInteger(33))) + mYPos,
                    FP_FromDouble(0.3),
                    GetAnimation().GetRenderLayer());

                mShinyParticleTimer = Math_RandomRange(-30, 30) + sGnFrame + 60;
                return;
            }
            case SlapLockStates::eEmitInvisibilityPowerupRing_4:
            {
                if (static_cast<s32>(sGnFrame) > mTimer1)
                {
                    if (!gMap.Is_Point_In_Current_Camera(
                            sActiveHero->mCurrentLevel,
                            sActiveHero->mCurrentPath,
                            sActiveHero->mXPos,
                            sActiveHero->mYPos,
                            1)
                        || sActiveHero->mRingPulseTimer
                        || sActiveHero->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
                    {
                        AbilityRing::Factory(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * mSpriteScale),
                            RingTypes::eInvisible_Pulse_Large_8,
                            mSpriteScale);
                        mTimer1 = Math_RandomRange(1, 10) + sGnFrame + 55;
                    }
                    else
                    {
                        SetInvisibilityTarget();
                        mState = SlapLockStates::eFlickerHero_5;
                    }
                }

                if (static_cast<s32>(sGnFrame) <= mShinyParticleTimer)
                {
                    return;
                }

                New_TintShiny_Particle(
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-2, 2)) + FP_FromInteger(1))) + mXPos,
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-3, 3)) - FP_FromInteger(33))) + mYPos,
                    FP_FromDouble(0.3),
                    GetAnimation().GetRenderLayer());

                mShinyParticleTimer = Math_RandomRange(-30, 30) + sGnFrame + 60;
                return;
            }
            case SlapLockStates::eFlickerHero_5:
            {
                if (pRingObj)
                {
                    return;
                }

                if (sActiveHero->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
                {
                    mState = SlapLockStates::eGiveInvisibility_7;
                }
                else
                {
                    auto pFlicker = relive_new PossessionFlicker(sActiveHero, 8, 128, 255, 128);
                    if (pFlicker)
                    {
                        mPossessionFlickerId = pFlicker->mBaseGameObjectId;
                    }
                    mState = SlapLockStates::eGiveInvisibilityFromFlicker_6;
                }
                return;
            }
            case SlapLockStates::eGiveInvisibilityFromFlicker_6:
            {
                if (pFlickerObj)
                {
                    return;
                }
                GiveInvisibility();
                mShinyParticleTimer = sGnFrame + 60;
                mState = SlapLockStates::eBroken_3;
                break;
            }
            case SlapLockStates::eGiveInvisibility_7:
            {
                GiveInvisibility();
                mShinyParticleTimer = sGnFrame + 60;
                mState = SlapLockStates::eBroken_3;
                break;
            }
            default:
            {
                return;
            }
        }
    }
}

void SlapLock::SetInvisibilityTarget()
{
    AbilityRing::Factory(
        mXPos,
        mYPos - (FP_FromInteger(40) * mSpriteScale),
        RingTypes::eInvisible_Pulse_Emit_9,
        mSpriteScale);

    const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

    AbilityRing* pRing = AbilityRing::Factory(
        FP_FromInteger((bRect.x + bRect.w) / 2),
        FP_FromInteger((bRect.y + bRect.h) / 2),
        RingTypes::eInvisible_Pulse_Give_10,
        sActiveHero->mSpriteScale);

    pRing->mBaseGameObjectTlvInfo = mBaseGameObjectTlvInfo;
    mAbilityRingId = pRing->mBaseGameObjectId;

    pRing->VSetTarget(sActiveHero);
}

s16 SlapLock::VTakeDamage(BaseGameObject* pFrom)
{
    mSlapLockTlv = static_cast<relive::Path_SlapLock*>(sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));

    if (pFrom->Type() != ReliveTypes::eAbe)
    {
        // Only Abe can slap me up
        return 0;
    }

    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_62_Punch_454750)
    {
        // If Abe isn't slapping then he can't hurt me
        return 0;
    }

    if (mState != SlapLockStates::eShaking_0 && mState != SlapLockStates::eIdle_1)
    {
        return 0;
    }

    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    sActiveHero->ToKnockback_44E700(1, 0);

    if (mHasGhost == relive::reliveChoice::eYes)
    {
        mHasGhost = relive::reliveChoice::eNo;
        relive_new SlapLockWhirlWind(
            mSlapLockTlv->mTargetTombSwitchId1,
            mSlapLockTlv->mTargetTombSwitchId2,
            mXPos,
            mYPos - (FP_FromInteger(40) * mSpriteScale),
            mSpriteScale);
    }

    if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eYes)
    {
        SetInvisibilityTarget();
    }

    mState = SlapLockStates::eSlapped_2;
    SwitchStates_Do_Operation(mSlapLockTlv->mSlapOutputSwitchId, relive::reliveSwitchOp::eToggle);
    SfxPlayMono(relive::SoundEffects::SpiritLockBreak, 0, mSpriteScale);
    EventBroadcast(kEventLoudNoise, this);

    relive_new ParticleBurst(
        mXPos,
        mYPos - (FP_FromInteger(40) * mSpriteScale),
        15,
        mSpriteScale,
        BurstType::eGreenSparks_5,
        11);

    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SlapLock_Punched));

    mSlapLockTlv->mTlvSpecificMeaning = 1;
    return 1;
}

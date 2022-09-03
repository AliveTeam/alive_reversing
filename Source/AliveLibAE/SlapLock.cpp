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
#include "Particle.hpp"
#include "ParticleBurst.hpp"

SlapLock::SlapLock(relive::Path_SlapLock* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eLockedSoul);
    mSlapLockTlv = pTlv;
    mTlvInfo = tlvId;
    mBaseGameObjectTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }

    const AnimRecord& rec = AnimRec(AnimId::SlapLock_Initiate);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::SlapLock_Initiate, ppRes);

    if (mSlapLockTlv->mScale != relive::reliveScale::eFull)
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
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

    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

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

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGhostTrpResID_1053, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GHOSTTRP.BAN", nullptr);
    }

    auto pSlapLock = relive_new SlapLock(pTlv, pState->mTlvInfo);
    if (pSlapLock)
    {
        pSlapLock->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->mAnimRender & 1);

        pSlapLock->mTlvInfo = pState->mTlvInfo;

        pTlv->mTlvSpecificMeaning = pState->mTlvState;

        pSlapLock->mState = pState->mState;
        pSlapLock->mTimer1 = pState->mTimer1;
        pSlapLock->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
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
    pState->mAnimRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render) & 1;
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
        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
        {
            mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

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

                mAnim.Set_Animation_Data(AnimId::SlapLock_Shaking, nullptr);

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

                if (!(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
                {
                    return;
                }

                mAnim.Set_Animation_Data(AnimId::SlapLock_Initiate, nullptr);

                mState = SlapLockStates::eShaking_0;
                mTimer1 = Math_NextRandom() + sGnFrame + 25;
                return;
            }
            case SlapLockStates::eSlapped_2:
            {
                if (!(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
                {
                    return;
                }

                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

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
                    mAnim.mRenderLayer);

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
                        || sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
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
                    mAnim.mRenderLayer);

                mShinyParticleTimer = Math_RandomRange(-30, 30) + sGnFrame + 60;
                return;
            }
            case SlapLockStates::eFlickerHero_5:
            {
                if (pRingObj)
                {
                    return;
                }

                if (sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
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

    mAnim.Set_Animation_Data(AnimId::SlapLock_Punched, nullptr);

    mSlapLockTlv->mTlvSpecificMeaning = 1;
    return 1;
}

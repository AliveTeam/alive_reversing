#include "stdafx.h"
#include "SlapLock.hpp"
#include "SlapLockWhirlWind.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "AbilityRing.hpp"
#include "PossessionFlicker.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/GameObjects/ParticleBurst.hpp"
#include "Map.hpp"
#include "QuikSave.hpp"

void SlapLock::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SlapLock_Initiate));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SlapLock_Punched));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SlapLock_Shaking));
}

SlapLock::SlapLock(relive::Path_SlapLock* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eSlapLock);
    mSlapLockTlv = pTlv;
    mTlvInfo = tlvId;
    mBaseGameObjectTlvInfo = tlvId;

    LoadAnimations();

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
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
    mTimer1 = (Math_NextRandom() & 7) + MakeTimer(25);
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

    GetAnimation().SetRender(false);

    mTimer1 = MakeTimer(60);
    mShinyParticleTimer = MakeTimer(30);

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

void SlapLock::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<SlapLockSaveState>();

    auto pTlv = static_cast<relive::Path_SlapLock*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));

    auto pSlapLock = relive_new SlapLock(pTlv, pState->mTlvInfo);
    if (pSlapLock)
    {
        pSlapLock->GetAnimation().SetRender(pState->mAnimRender & 1);

        pSlapLock->mTlvInfo = pState->mTlvInfo;

        pTlv->mTlvSpecificMeaning = pState->mTlvState;

        pSlapLock->mState = pState->mState;
        pSlapLock->mTimer1 = pState->mTimer1;
        pSlapLock->mbGotShot = true;
        pSlapLock->mShinyParticleTimer = pState->mShinyParticleTimer;
    }
}

void SlapLock::VScreenChanged()
{
    if (mState == SlapLockStates::eFlickerHero_5 || mState == SlapLockStates::eGiveInvisibilityFromFlicker_6)
    {
        GiveInvisibility();
    }
    SetDead(true);
}

void SlapLock::GiveInvisibility()
{
    mSlapLockTlv = static_cast<relive::Path_SlapLock*>(gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));
    if (gAbe)
    {
        gAbe->mInvisibilityDuration = mSlapLockTlv->mInvisibilityDuration;
        gAbe->mHaveShrykull = 0;
        gAbe->mHaveInvisibility = 1;
        gAbe->mRingPulseTimer = MakeTimer(200000);
    }
}

void SlapLock::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    SlapLockSaveState data = {};

    data.mType = ReliveTypes::eSlapLock;
    data.mAnimRender = GetAnimation().GetRender() & 1;
    data.mTlvInfo = mTlvInfo;
    data.mTlvState = gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo)->mTlvSpecificMeaning;
    data.mState = mState;
    data.mTimer1 = mTimer1;
    data.mShinyParticleTimer = mShinyParticleTimer;
    data.mAbilityRingId = Guid{};

    if (mAbilityRingId == Guid{})
    {
        pSaveBuffer.Write(data);
        return;
    }

    BaseGameObject* pObj = sObjectIds.Find_Impl(mAbilityRingId);
    if (pObj)
    {
        data.mAbilityRingId = pObj->mBaseGameObjectTlvInfo;
    }
    pSaveBuffer.Write(data);
}

void SlapLock::VUpdate()
{
    mSlapLockTlv = static_cast<relive::Path_SlapLock*>(gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        if (GetRestoredFromQuickSave())
        {
            SetRestoredFromQuickSave(false);

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
                            mYPos - (FP_FromInteger(40) * GetSpriteScale()),
                            RingTypes::eInvisible_Pulse_Large_8,
                            GetSpriteScale());
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
                            mYPos - (FP_FromInteger(40) * GetSpriteScale()),
                            RingTypes::eInvisible_Pulse_Large_8,
                            GetSpriteScale());
                    }
                }

                if (!(GetAnimation().GetIsLastFrame()))
                {
                    return;
                }

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SlapLock_Initiate));

                mState = SlapLockStates::eShaking_0;
                mTimer1 = Math_NextRandom() + MakeTimer(25);
                return;
            }
            case SlapLockStates::eSlapped_2:
            {
                if (!(GetAnimation().GetIsLastFrame()))
                {
                    return;
                }

                GetAnimation().SetRender(false);

                if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eNo)
                {
                    mShinyParticleTimer = MakeTimer(60);
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

                const FP y = (GetSpriteScale() * (FP_FromInteger(Math_RandomRange(-3, 3)) - FP_FromInteger(33))) + mYPos;
                const FP x = (GetSpriteScale() * (FP_FromInteger(Math_RandomRange(-2, 2)) + FP_FromInteger(1))) + mXPos;
                New_TintShiny_Particle(
                    x, y,
                    FP_FromDouble(0.3),
                    GetAnimation().GetRenderLayer());

                mShinyParticleTimer = Math_RandomRange(-30, 30) + MakeTimer(60);
                return;
            }
            case SlapLockStates::eEmitInvisibilityPowerupRing_4:
            {
                if (static_cast<s32>(sGnFrame) > mTimer1)
                {
                    if (!gMap.Is_Point_In_Current_Camera(
                            gAbe->mCurrentLevel,
                            gAbe->mCurrentPath,
                            gAbe->mXPos,
                            gAbe->mYPos,
                            1)
                        || gAbe->mRingPulseTimer
                        || gAbe->GetInvisible())
                    {
                        AbilityRing::Factory(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * GetSpriteScale()),
                            RingTypes::eInvisible_Pulse_Large_8,
                            GetSpriteScale());
                        mTimer1 = Math_RandomRange(1, 10) + MakeTimer(55);
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

                const FP x = (GetSpriteScale() * (FP_FromInteger(Math_RandomRange(-2, 2)) + FP_FromInteger(1))) + mXPos;
                const FP y = (GetSpriteScale() * (FP_FromInteger(Math_RandomRange(-3, 3)) - FP_FromInteger(33))) + mYPos;
                New_TintShiny_Particle(
                    x, y,
                    FP_FromDouble(0.3),
                    GetAnimation().GetRenderLayer());

                mShinyParticleTimer = Math_RandomRange(-30, 30) + MakeTimer(60);
                return;
            }
            case SlapLockStates::eFlickerHero_5:
            {
                if (pRingObj)
                {
                    return;
                }

                if (gAbe->GetInvisible())
                {
                    mState = SlapLockStates::eGiveInvisibility_7;
                }
                else
                {
                    auto pFlicker = relive_new PossessionFlicker(gAbe, 8, 128, 255, 128);
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
                mShinyParticleTimer = MakeTimer(60);
                mState = SlapLockStates::eBroken_3;
                break;
            }
            case SlapLockStates::eGiveInvisibility_7:
            {
                GiveInvisibility();
                mShinyParticleTimer = MakeTimer(60);
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
        mYPos - (FP_FromInteger(40) * GetSpriteScale()),
        RingTypes::eInvisible_Pulse_Emit_9,
        GetSpriteScale());

    const PSX_RECT bRect = gAbe->VGetBoundingRect();

    AbilityRing* pRing = AbilityRing::Factory(
        FP_FromInteger((bRect.x + bRect.w) / 2),
        FP_FromInteger((bRect.y + bRect.h) / 2),
        RingTypes::eInvisible_Pulse_Give_10,
        gAbe->GetSpriteScale());

    pRing->mBaseGameObjectTlvInfo = mBaseGameObjectTlvInfo;
    mAbilityRingId = pRing->mBaseGameObjectId;

    pRing->VSetTarget(gAbe);
}

bool SlapLock::VTakeDamage(BaseGameObject* pFrom)
{
    mSlapLockTlv = static_cast<relive::Path_SlapLock*>(gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));

    if (pFrom->Type() != ReliveTypes::eAbe)
    {
        // Only Abe can slap me up
        return false;
    }

    if (gAbe->mCurrentMotion != eAbeMotions::Motion_62_Punch_454750)
    {
        // If Abe isn't slapping then he can't hurt me
        return false;
    }

    if (mState != SlapLockStates::eShaking_0 && mState != SlapLockStates::eIdle_1)
    {
        return false;
    }

    if (GetDead())
    {
        return false;
    }

    gAbe->ToKnockback_44E700(1, 0);

    if (mHasGhost == relive::reliveChoice::eYes)
    {
        mHasGhost = relive::reliveChoice::eNo;
        relive_new SlapLockWhirlWind(
            mSlapLockTlv->mTargetTombSwitchId1,
            mSlapLockTlv->mTargetTombSwitchId2,
            mXPos,
            mYPos - (FP_FromInteger(40) * GetSpriteScale()),
            GetSpriteScale());
    }

    if (mSlapLockTlv->mGiveInvisibilityPowerup == relive::reliveChoice::eYes)
    {
        SetInvisibilityTarget();
    }

    mState = SlapLockStates::eSlapped_2;
    SwitchStates_Do_Operation(mSlapLockTlv->mSlapOutputSwitchId, relive::reliveSwitchOp::eToggle);
    SfxPlayMono(relive::SoundEffects::SpiritLockBreak, 0, GetSpriteScale());
    EventBroadcast(kEventLoudNoise, this);

    relive_new ParticleBurst(
        mXPos,
        mYPos - (FP_FromInteger(40) * GetSpriteScale()),
        15,
        GetSpriteScale(),
        BurstType::eGreenSparks,
        11, false);

    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SlapLock_Punched));

    mSlapLockTlv->mTlvSpecificMeaning = 1;
    return true;
}

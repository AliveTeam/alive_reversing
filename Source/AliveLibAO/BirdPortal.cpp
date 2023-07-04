#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "BirdPortal.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Abe.hpp"
#include "ScreenClipper.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "Midi.hpp"
#include "Dove.hpp"
#include "OrbWhirlWind.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "AbilityRing.hpp"
#include "Path.hpp"
#include "../relive_lib/ObjectIds.hpp"

namespace AO {

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
}

void BirdPortalTerminator::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_TerminatorIdle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_TerminatorShrink));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_TerminatorGrow));
}

BirdPortalTerminator::BirdPortalTerminator(FP xpos, FP ypos, FP scale, relive::Path_BirdPortal::PortalType /*portalType*/)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eClawOrBirdPortalTerminator);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::BirdPortal_TerminatorGrow));
    
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
    SetSpriteScale(scale);
    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_Half_20);
    }

    SetApplyShadowZoneColour(false);

    mYPos = ypos;
    mXPos = xpos;

    mRGB.SetRGB(255, 128, 64);
}

void BirdPortalTerminator::Fadeout()
{
    const s16 r = mRGB.r;
    mRGB.r = (r >> 1) + (r >> 2);

    const s16 g = mRGB.g;
    mRGB.g = (g >> 1) + (g >> 2);

    const s16 b = mRGB.b;
    mRGB.b = (b >> 1) + (b >> 2);
}

// ==========================================================================

BirdPortal::~BirdPortal()
{
    if (mTerminator1)
    {
        mTerminator1->SetDead(true);
    }
    if (mTerminator2)
    {
        mTerminator2->SetDead(true);
    }

    if (mScreenClipper1)
    {
        mScreenClipper1->SetDead(true);
    }

    if (mScreenClipper2)
    {
        mScreenClipper2->SetDead(true);
    }

    if (mDovesArray)
    {
        for (s32 i = 0; i < mDovesArray->Size(); i++)
        {
            Dove* pObj = mDovesArray->ItemAt(i);
            if (!pObj)
            {
                break;
            }
            pObj->mBaseGameObjectRefCount--;
            pObj->SetDead(true);
        }
        mDovesArray->mUsedSize = 0;
        relive_delete mDovesArray;
    }

    if (mThrowableTotalIndicator)
    {
        mThrowableTotalIndicator->SetDead(true);
        mThrowableTotalIndicator->mBaseGameObjectRefCount--;
        mThrowableTotalIndicator = nullptr;
    }

    if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
    }

    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    if (mState >= PortalStates::PortalExit_SetPosition_17)
    {
        if (sActiveHero)
        {
            if (mSpriteScale == FP_FromInteger(1))
            {
                sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
            }
            else
            {
                sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
            }
        }
    }
}

void BirdPortal::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Sparks));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Flash));
}

BirdPortal::BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eBirdPortal);

    LoadAnimations();

    mTlvInfo = tlvId;

    mExitLevel = pTlv->mExitLevel;
    mEnterSide = pTlv->mEnterSide;
    mExitCamera = pTlv->mExitCamera;
    mExitPath = pTlv->mExitPath;
    mPortalType = pTlv->mPortalType;
    mMovieId = pTlv->mMovieId;
    mMudCountForShrykull = pTlv->mMudCountForShrykull;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
    }

    mCurrentPath = gMap.mCurrentPath;
    mCurrentLevel = gMap.mCurrentLevel;
    mState = PortalStates::CreatePortal_0;
    mTimer = 0;

    mTerminator1 = nullptr;
    mTerminator2 = nullptr;
    mScreenClipper1 = nullptr;
    mScreenClipper2 = nullptr;
    mOrbWhirlWind = nullptr;

    // TODO: Clean up this hack by having a better way to match "any" type of line
    CollisionMask allLinesHack;
    allLinesHack.mMask = 0xFFFFFFFF;

    PathLine* pLine = nullptr;
    FP hitX = {};
    gCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &pLine,
        &hitX,
        &mHitY,
        allLinesHack);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mSfxPlaying = 0;
    mYPos = mHitY - (FP_FromInteger(55) * mSpriteScale);
}

void BirdPortal::CreateDovesAndShrykullNumber()
{
    for (u8 i = 0; i < 6; i++)
    {
        auto pDove = relive_new Dove(
            AnimId::Dove_Flying,
            mXPos,
            mYPos,
            mSpriteScale);
        if (pDove)
        {
            pDove->mBaseGameObjectRefCount++;
            if (mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
            {
                pDove->AsAlmostACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
            }
            else
            {
                pDove->AsACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
            }
            pDove->SetSpriteScale(mSpriteScale);
            mDovesArray->Push_Back(pDove);
        }
    }

    if (mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
    {
        Layer indicatorLayer = Layer::eLayer_0;
        if (mSpriteScale != FP_FromDouble(0.5))
        {
            indicatorLayer = Layer::eLayer_27;
        }
        else
        {
            indicatorLayer = Layer::eLayer_8;
        }
        mThrowableTotalIndicator = relive_new ThrowableTotalIndicator(
            mXPos,
            mYPos + FP_FromInteger(10),
            indicatorLayer,
            mSpriteScale,
            mMudCountForShrykull,
            0);

        if (mThrowableTotalIndicator)
        {
            mThrowableTotalIndicator->mBaseGameObjectRefCount++;
        }
    }
}

void BirdPortal::CreateTerminators()
{
    mTerminator1 = relive_new BirdPortalTerminator(
        mXPos,
        mYPos,
        mSpriteScale,
        mPortalType);

    mTerminator2 = relive_new BirdPortalTerminator(
        mXPos,
        mYPos,
        mSpriteScale,
        mPortalType);
}

void BirdPortal::VUpdate()
{
    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    if (!mSfxPlaying)
    {
        if (mState > PortalStates::CreateTerminators_4 && mState < PortalStates::StopSound_11)
        {
            mSfxPlaying = SFX_Play_Camera(relive::SoundEffects::PortalOpening, 45, direction);
        }
    }

    switch (mState)
    {
        case PortalStates::CreatePortal_0:
            mDovesArray = relive_new DynamicArrayT<Dove>(6);

            CreateDovesAndShrykullNumber();
            mState = PortalStates::IdlePortal_1;
            break;

        case PortalStates::IdlePortal_1:
        {
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                SfxPlayMono(relive::SoundEffects::Dove, 35);
                mTimer = MakeTimer(Math_RandomRange(24, 40));
            }

            auto pTarget = Abe::FindObjectToPossess();
            if (!EventGet(kEventAbeOhm) || pTarget)
            {
                if (IsScaredAway() || EventGet(kEventShooting) || (EventGet(kEventAbeOhm) && pTarget))
                {
                    for (s32 i = 0; i < mDovesArray->Size(); i++)
                    {
                        Dove* pDove = mDovesArray->ItemAt(i);
                        if (!pDove)
                        {
                            break;
                        }
                        pDove->mBaseGameObjectRefCount--;
                        pDove->FlyAway(1);
                    }
                    mDovesArray->mUsedSize = 0;
                    relive_delete mDovesArray;
                    mDovesArray = nullptr;
                    if (mThrowableTotalIndicator)
                    {
                        mThrowableTotalIndicator->SetDead(true);
                        mThrowableTotalIndicator->mBaseGameObjectRefCount--;
                        mThrowableTotalIndicator = nullptr;
                    }

                    SfxPlayMono(relive::SoundEffects::Dove, 70);
                    SetDead(true);
                }
            }
            else
            {
                if (mThrowableTotalIndicator)
                {
                    mThrowableTotalIndicator->SetDead(true);
                    mThrowableTotalIndicator->mBaseGameObjectRefCount--;
                    mThrowableTotalIndicator = nullptr;
                }
                for (s32 i = 0; i < mDovesArray->Size(); i++)
                {
                    Dove* pDove = mDovesArray->ItemAt(i);
                    if (!pDove)
                    {
                        break;
                    }
                    pDove->AsJoin(mXPos, mYPos + (mSpriteScale * FP_FromInteger(20)));
                }

                mTimer = MakeTimer(15);
                mState = PortalStates::JoinDovesInCenter_2;
                EventBroadcast(kEventPortalOpen, this);
                SfxPlayMono(relive::SoundEffects::Dove, 70);
            }
        }
        break;

        case PortalStates::JoinDovesInCenter_2:
            EventBroadcast(kEventPortalOpen, this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                CreateTerminators();
                mTimer = MakeTimer(5);
                mState = PortalStates::KillDoves_3;
            }
            break;

        case PortalStates::KillDoves_3:
            EventBroadcast(kEventPortalOpen, this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                for (s32 i = 0; i < mDovesArray->Size(); i++)
                {
                    Dove* pDove = mDovesArray->ItemAt(i);
                    if (!pDove)
                    {
                        break;
                    }
                    pDove->mBaseGameObjectRefCount--;
                    pDove->SetDead(true);
                }
                mDovesArray->mUsedSize = 0;
                relive_delete mDovesArray;
                mDovesArray = nullptr;
                mState = PortalStates::CreateTerminators_4;
            }
            break;

        case PortalStates::CreateTerminators_4:
            EventBroadcast(kEventPortalOpen, this);
            if (mTerminator1->GetAnimation().GetIsLastFrame())
            {
                mTerminator1->GetAnimation().Set_Animation_Data(mTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mTerminator2->GetAnimation().Set_Animation_Data(mTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mTimer = MakeTimer(12);
                mState = PortalStates::ExpandTerminators_5;
                mSfxPlaying = SfxPlayMono(relive::SoundEffects::PortalOpening, 0);
                SFX_Play_Pitch(relive::SoundEffects::IngameTransition, 115, 300);
            }
            break;

        case PortalStates::ExpandTerminators_5:
            EventBroadcast(kEventPortalOpen, this);
            mTerminator1->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            mTerminator2->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = PortalStates::ActivePortal_6;
            }
            break;

        case PortalStates::ActivePortal_6:
            EventBroadcast(kEventPortalOpen, this);
            if ((mPortalType != relive::Path_BirdPortal::PortalType::eWorker && mPortalType != relive::Path_BirdPortal::PortalType::eShrykull) || EventGet(kEventAbeOhm))
            {
                if ((Math_NextRandom() % 8) == 0)
                {
                    auto pParticle = relive_new Particle(
                        mTerminator2->mXPos,
                        (FP_FromInteger(10) * mSpriteScale) + mTerminator2->mYPos,
                        GetAnimRes(AnimId::BirdPortal_Sparks));

                    if (pParticle)
                    {
                        pParticle->SetApplyShadowZoneColour(false);
                        pParticle->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                        pParticle->SetType(ReliveTypes::eBirdPortalTerminator);
                        pParticle->SetSpriteScale(mSpriteScale);

                        if (sGnFrame % 2)
                        {
                            pParticle->GetAnimation().SetLoopBackwards(true);
                            pParticle->GetAnimation().SetFrame(pParticle->GetAnimation().Get_Frame_Count());
                        }
                    }

                    if (direction == CameraPos::eCamCurrent_0)
                    {
                        SFX_Play_Pitch(relive::SoundEffects::BirdPortalSpark, 50, 2400, mSpriteScale);
                    }
                }
                if (!(sGnFrame % 8))
                {
                    AbilityRing::Factory(
                        mTerminator1->mXPos,
                        mTerminator1->mYPos + FP_FromInteger(7),
                        RingTypes::eShrykull_Pulse_Orange_6);
                    AbilityRing::Factory(
                        mTerminator2->mXPos,
                        mTerminator2->mYPos + FP_FromInteger(7),
                        RingTypes::eShrykull_Pulse_Orange_6);
                }
            }
            else
            {
                VGiveShrykull(0);
                if (mState != PortalStates::ShrykullGetDoves_7)
                {
                    mState = PortalStates::FadeoutTerminators_22;
                    mTimer = MakeTimer(30);
                }
            }
            break;

        case PortalStates::ShrykullGetDoves_7:
            if (mReceivedDovesCount >= 7)
            {
                mState = PortalStates::GetShrykull_9;
                mTimer = MakeTimer(20);
            }
            else
            {
                FP xOff = {};
                if (mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft)
                {
                    xOff = (mSpriteScale * FP_FromInteger(16));
                }
                else
                {
                    xOff = (mSpriteScale * FP_FromInteger(-16));
                }

                const s16 scale32 = FP_GetExponent(mSpriteScale * FP_FromInteger(32));

                if (static_cast<s32>(sGnFrame) >= mTimer)
                {
                    mTimer = MakeTimer(Math_RandomRange(4, 12));

                    auto pDoveMem = relive_new Dove(
                        AnimId::Dove_Flying,
                        mXPos + FP_FromInteger(FP_GetExponent(xOff)),
                        mYPos + FP_FromInteger(Math_RandomRange(-scale32, scale32)),
                        mSpriteScale);
                    if (pDoveMem)
                    {
                        SfxPlayMono(relive::SoundEffects::Dove, 70);
                        pDoveMem->SetSpriteScale(mSpriteScale);
                        pDoveMem->AsJoin(sActiveHero->mXPos, FP_FromInteger(Math_RandomRange(-36, 4)) + sActiveHero->mYPos);
                    }

                    mReceivedDovesCount++;
                    if (mReceivedDovesCount == 6)
                    {
                        mOrbWhirlWind->ToSpin(
                            sActiveHero->mXPos,
                            sActiveHero->mYPos - (sActiveHero->GetSpriteScale() * FP_FromInteger(38)),
                            sActiveHero);
                        mOrbWhirlWind = nullptr;
                    }
                }
            }
            break;

        case PortalStates::GetShrykull_9:
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                sActiveHero->field_168_ring_pulse_timer = MakeTimer(32000);
                sActiveHero->field_16C_bHaveShrykull = true;
                mState = PortalStates::CollapseTerminators_10;
				if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant)
                {
                    sActiveHero->ChangeChantState(0);
                }
            }
            break;

        case PortalStates::CollapseTerminators_10:
            mTerminator1->mYPos += FP_FromDouble(3.5) * mSpriteScale;
            mTerminator2->mYPos -= FP_FromDouble(3.5) * mSpriteScale;

            if (FP_GetExponent(mTerminator1->mYPos) >= FP_GetExponent(mTerminator2->mYPos))
            {
                auto pParticle_1 = relive_new Particle(
                    mTerminator2->mXPos,
                    mTerminator2->mYPos,
                    GetAnimRes(AnimId::BirdPortal_Flash));
                if (pParticle_1)
                {
                    pParticle_1->SetApplyShadowZoneColour(false);
                    pParticle_1->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                    pParticle_1->SetSpriteScale(mSpriteScale);
                }

                mState = PortalStates::StopSound_11;
                mTimer = MakeTimer(5);
                mTerminator1->SetDead(true);
                mTerminator2->SetDead(true);
                mTerminator1 = nullptr;
                mTerminator2 = nullptr;
                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 100, -1800);
            }
            break;

        case PortalStates::StopSound_11:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                if (mSfxPlaying)
                {
                    SND_Stop_Channels_Mask(mSfxPlaying);
                    mSfxPlaying = 0;
                }
                mState = PortalStates::CreateFlash1_12;
            }
            break;

        case PortalStates::CreateFlash1_12:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u);
            mState = PortalStates::CreateFlash2_13;
        }
        break;

        case PortalStates::CreateFlash2_13:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, relive::TBlendModes::eBlend_0, 1);
            mState = PortalStates::CreateFlash3_14;
        }
        break;

        case PortalStates::CreateFlash3_14:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, relive::TBlendModes::eBlend_0, 1);
            mState = PortalStates::KillPortal_15;
            mTimer = MakeTimer(5);
        }
        break;

        case PortalStates::KillPortal_15:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                if (mPortalType == relive::Path_BirdPortal::PortalType::eWorker || mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
                {
                    SetDead(true);
                }
                else
                {
                    mState = PortalStates::AbeInsidePortal_16;
                }
            }
            break;

        case PortalStates::PortalExit_SetPosition_17:
        {
            gScreenManager->EnableRendering();

            CreateTerminators();

            mState = PortalStates::PortalExit_CreateTerminators_18;
            mSfxPlaying = SfxPlayMono(relive::SoundEffects::PortalOpening, 0);
        }
        break;

        case PortalStates::PortalExit_CreateTerminators_18:
            if (mTerminator1->GetAnimation().GetIsLastFrame())
            {
                mTerminator1->GetAnimation().Set_Animation_Data(mTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mTerminator2->GetAnimation().Set_Animation_Data(mTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mState = PortalStates::PortalExit_ExpandTerminators_19;
                mTimer = MakeTimer(12);
            }
            break;

        case PortalStates::PortalExit_ExpandTerminators_19:
            mTerminator1->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            mTerminator2->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = PortalStates::PortalExit_AbeExitting_20;
            }
            break;

        case PortalStates::KillPortalClipper_21:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mTerminator1->GetAnimation().Set_Animation_Data(mTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorShrink));
                mTerminator2->GetAnimation().Set_Animation_Data(mTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorShrink));
                mState = PortalStates::FadeoutTerminators_22;
                mTimer = MakeTimer(30);

                if (mScreenClipper1)
                {
                    mScreenClipper1->SetDead(true);
                    mScreenClipper1 = nullptr;
                }

                if (mSfxPlaying)
                {
                    SND_Stop_Channels_Mask(mSfxPlaying);
                    mSfxPlaying = 0;
                }
            }
            break;

        case PortalStates::FadeoutTerminators_22:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                mTerminator1->Fadeout();
                mTerminator2->Fadeout();
            }
            else
            {
                SetDead(true);
            }
            break;

        default:
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

s16 BirdPortal::IsScaredAway()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            return false;
        }

        switch (pObj->Type())
        {
            case ReliveTypes::eElum:
            case ReliveTypes::eAbe:
            case ReliveTypes::eRingOrLiftMud:
            case ReliveTypes::eParamite:
            case ReliveTypes::eScrab:
            case ReliveTypes::eSlig:
                if (pObj->mCurrentPath != mCurrentPath)
                {
                    continue;
                }

                if (FP_Abs(pObj->mXPos - mXPos) >= FP_FromInteger(75) || FP_Abs(pObj->mYPos - mHitY) >= FP_FromInteger(30) || pObj->GetSpriteScale() != mSpriteScale)
                {
                    continue;
                }
                return true;

            default:
                continue;
        }
    }
    return false;
}

void BirdPortal::VGiveShrykull(s16 bPlaySound)
{
    if (mPortalType == relive::Path_BirdPortal::PortalType::eShrykull && mMudCountForShrykull <= 0)
    {
        mState = PortalStates::ShrykullGetDoves_7;
        mTimer = MakeTimer(12);
        mReceivedDovesCount = 0;

        mOrbWhirlWind = relive_new OrbWhirlWind(
            sActiveHero->mXPos,
            sActiveHero->mYPos - (sActiveHero->GetSpriteScale() * FP_FromInteger(38)),
            sActiveHero->GetSpriteScale());

        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant)
        {
            sActiveHero->ChangeChantState(1);
        }
    }
    else
    {
        mState = PortalStates::CollapseTerminators_10;
        if ((mPortalType == relive::Path_BirdPortal::PortalType::eWorker || mPortalType == relive::Path_BirdPortal::PortalType::eShrykull) && sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant)
        {
            sActiveHero->ChangeChantState(0);
        }
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eBirdPortalTerminator)
        {
            pObj->SetDead(true);
        }
    }

    if (bPlaySound)
    {
        SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 70, -1600);
    }
}

void BirdPortal::VScreenChanged()
{
    if (mState <= PortalStates::IdlePortal_1 || mState >= PortalStates::KillPortalClipper_21 || ((gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath) && (mState != PortalStates::AbeInsidePortal_16 || mPortalType != relive::Path_BirdPortal::PortalType::eAbe || gMap.mNextLevel != mExitLevel || gMap.mNextPath != mExitPath)))
    {
        SetDead(true);
    }

    if (GetDead())
    {
        if (mTerminator1)
        {
            mTerminator1->SetDead(true);
            mTerminator1 = nullptr;
        }

        if (mTerminator2)
        {
            mTerminator2->SetDead(true);
            mTerminator2 = nullptr;
        }

        if (mScreenClipper1)
        {
            mScreenClipper1->SetDead(true);
            mScreenClipper1 = nullptr;
        }

        if (mScreenClipper2)
        {
            mScreenClipper2->SetDead(true);
            mScreenClipper2 = nullptr;
        }
    }
    else
    {
        if (mSfxPlaying)
        {
            SND_Stop_Channels_Mask(mSfxPlaying);
            mSfxPlaying = 0;
        }
    }
}

void BirdPortal::VStopAudio()
{
    if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
    }
}

void BirdPortal::VKillPortalClipper()
{
    if (mScreenClipper1)
    {
        mScreenClipper1->SetDead(true);
        mScreenClipper1 = nullptr;

        mScreenClipper2->SetDead(true);
        mScreenClipper2 = nullptr;
    }
}

bool BirdPortal::VActivePortal()
{
    return mState == PortalStates::ActivePortal_6;
}

bool BirdPortal::VAbeInsidePortal()
{
    return mState == PortalStates::AbeInsidePortal_16;
}

void BirdPortal::VExitPortal()
{
    mCurrentPath = gMap.mCurrentPath;
    mCurrentLevel = gMap.mCurrentLevel;

    auto pPortalExitTlv = static_cast<relive::Path_BirdPortalExit*>(gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eBirdPortalExit, 0));

    if (pPortalExitTlv)
    {
        // TODO: Clean up this hack by having a better way to match "any" type of line
        CollisionMask allLinesHack;
        allLinesHack.mMask = 0xFFFFFFFF;

        PathLine* pLine = nullptr;
        gCollisions->Raycast(
            FP_FromInteger(pPortalExitTlv->mTopLeftX),
            FP_FromInteger(pPortalExitTlv->mTopLeftY),
            FP_FromInteger(pPortalExitTlv->mBottomRightX),
            FP_FromInteger(pPortalExitTlv->mBottomRightY),
            &pLine,
            &mExitX,
            &mExitY,
            allLinesHack);

        mYPos = mExitY - FP_FromInteger(55);

        mExitX = FP_FromInteger(pPortalExitTlv->mTopLeftX);
        mXPos = mExitX;
        mEnterSide = pPortalExitTlv->mExitSide;

        if (pPortalExitTlv->mScale == relive::reliveScale::eHalf)
        {
            mSpriteScale = FP_FromDouble(0.5);
            sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_InBirdPortal_Half_11);
        }
        else
        {
            mSpriteScale = FP_FromInteger(1);
            sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_InBirdPortal_30);
        }

        sActiveHero->SetSpriteScale(mSpriteScale);
        sActiveHero->mCurrentLevel = gMap.mCurrentLevel;
        sActiveHero->mCurrentPath = gMap.mCurrentPath;

        mState = PortalStates::PortalExit_SetPosition_17;
    }
    else
    {
        LOG_WARNING("Bird portal exit object not found!");
    }
}

bool BirdPortal::VPortalExit_AbeExitting()
{
    return mState == PortalStates::PortalExit_AbeExitting_20;
}

void BirdPortal::VIncreaseTimerAndKillPortalClipper()
{
    mState = PortalStates::KillPortalClipper_21;
    mTimer = MakeTimer(30);
}

void BirdPortal::VMudSaved()
{
    mMudCountForShrykull--;
}

void BirdPortal::VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId)
{
    *level = mExitLevel;
    *path = mExitPath;
    *camera = mExitCamera;

    // Positive
    if (mMovieId > 0)
    {
        *movieId = mMovieId;
        *screenChangeEffect = CameraSwapEffects::ePlay1FMV_5;
        return;
    }

    // Zero
    if (mMovieId == 0)
    {
        *screenChangeEffect = CameraSwapEffects::eInstantChange_0;
        return;
    }

    // Negative cases - dead code ??
    if (sActiveHero->mParamoniaDone && sActiveHero->mScrabaniaDone)
    {
        *movieId = 1617 - (10000 * mMovieId);
        *screenChangeEffect = CameraSwapEffects::ePlay1FMV_5;
        return;
    }

    *movieId = 17 - (100 * mMovieId);
    *screenChangeEffect = CameraSwapEffects::ePlay1FMV_5;
}

s16 BirdPortal::VPortalClipper(s16 bUnknown)
{
    if (bUnknown && mState != PortalStates::ActivePortal_6)
    {
        return 0;
    }

    if (mScreenClipper1)
    {
        return 1;
    }

    const s16 portalX = static_cast<s16>(PsxToPCX(gScreenManager->mCamXOff + FP_GetExponent(mXPos) - FP_GetExponent(gScreenManager->mCamPos->x), 11));

    PSX_Point xy = {};
    PSX_Point wh = {};
    if (mEnterSide != relive::Path_BirdPortal::PortalSide::eRight)
    {
        xy.x = 0;
        wh.x = portalX;
    }
    else
    {
        xy.x = portalX;
        wh.x = 640;
    }

    xy.y = 0;
    wh.y = 240;

    mScreenClipper1 = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
    if (mScreenClipper1)
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mScreenClipper1->mOtLayer = Layer::eLayer_BirdPortal_29;
        }
        else
        {
            mScreenClipper1->mOtLayer = Layer::eLayer_BirdPortal_Half_10;
        }
    }

    mScreenClipper2 = relive_new ScreenClipper(PSX_Point{ 0, 0 }, PSX_Point{ 640, 240 }, Layer::eLayer_0);
    if (mScreenClipper2)
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mScreenClipper2->mOtLayer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;
        }
        else
        {
            mScreenClipper2->mOtLayer = Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12;
        }
    }

    return 1;
}

} // namespace AO

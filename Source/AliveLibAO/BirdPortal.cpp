#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "BirdPortal.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Abe.hpp"
#include "../relive_lib/GameObjects/ScreenClipper.hpp"
#include "../relive_lib/GameObjects/ThrowableTotalIndicator.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "Midi.hpp"
#include "../relive_lib/GameObjects/Dove.hpp"
#include "../relive_lib/GameObjects/OrbWhirlWind.hpp"
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
#include "Map.hpp"
#include "BirdPortalTerminator.hpp"

namespace AO {

void BirdPortal::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Sparks));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Flash));
}

BirdPortal::BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0), 
    mPortalType(pTlv->mPortalType), 
    mEnterSide(pTlv->mEnterSide),
    mTlvInfo(tlvId),
    mMovieId(pTlv->mMovieId),
    mExitLevel(pTlv->mExitLevel),
    mExitPath(pTlv->mExitPath),
    mExitCamera(pTlv->mExitCamera),
    mMudCountForShrykull(pTlv->mMudCountForShrykull)
{
    SetType(ReliveTypes::eBirdPortal);

    LoadAnimations();

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
    mYPos = mHitY - (FP_FromInteger(55) * mSpriteScale);
}

BirdPortal::~BirdPortal()
{
    BaseGameObject* pTerminator1 = sObjectIds.Find_Impl(mTerminatorId1);
    if (pTerminator1)
    {
        pTerminator1->SetDead(true);
    }

    BaseGameObject* pTerminator2 = sObjectIds.Find_Impl(mTerminatorId2);
    if (pTerminator2)
    {
        pTerminator2->SetDead(true);
    }

    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    if (pClipper1)
    {
        pClipper1->SetDead(true);
    }

    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);
    if (pClipper2)
    {
        pClipper2->SetDead(true);
    }

    BaseGameObject* pDoves = sObjectIds.Find(mDoveIds[0], ReliveTypes::eDove);
    if (pDoves && mDovesExist)
    {
        for (const auto& doveId : mDoveIds)
        {
            BaseGameObject* pDove = sObjectIds.Find(doveId, ReliveTypes::eDove);
            if (pDove)
            {
                pDove->SetDead(true);
            }
        }
    }

    BaseGameObject* pThrowableIndicator = sObjectIds.Find(mThrowableTotalIndicator, ReliveTypes::eThrowableTotalIndicator);
    if (pThrowableIndicator)
    {
        pThrowableIndicator->SetDead(true);
    }

    if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
    }

    Path::TLV_Reset(mTlvInfo);
    if (mState >= PortalStates::PortalExit_SetPosition_17)
    {
        if (gAbe)
        {
            if (mSpriteScale == FP_FromInteger(1))
            {
                gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
            }
            else
            {
                gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
            }
        }
    }
}

void BirdPortal::CreateDovesAndShrykullNumber()
{
    for (u8 i = 0; i < ALIVE_COUNTOF(mDoveIds); i++)
    {
        auto pDove = relive_new Dove(AnimId::Dove_Flying, mXPos, mYPos, mSpriteScale);

        mDovesExist = true;
        if (mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
        {
            pDove->AsAlmostACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
        }
        else
        {
            pDove->AsACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
        }
        pDove->SetSpriteScale(mSpriteScale);
        mDoveIds[i] = pDove->mBaseGameObjectId;
    }

    if (mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
    {
        const Layer indicatorLayer = mSpriteScale != FP_FromDouble(0.5) ? Layer::eLayer_27 : Layer::eLayer_8;
        auto pIndicator = relive_new ThrowableTotalIndicator(
            mXPos,
            mYPos + FP_FromInteger(10),
            indicatorLayer,
            mSpriteScale,
            mMudCountForShrykull,
            0);
        if (pIndicator)
        {
            mThrowableTotalIndicator = pIndicator->mBaseGameObjectId;
        }
    }
}

void BirdPortal::VUpdate()
{
    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    auto pTerminator1 = static_cast<BirdPortalTerminator*>(sObjectIds.Find_Impl(mTerminatorId1));
    auto pTerminator2 = static_cast<BirdPortalTerminator*>(sObjectIds.Find_Impl(mTerminatorId2));
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

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
                    for (const auto& id : mDoveIds)
                    {
                        Dove* pDove = static_cast<Dove*>(sObjectIds.Find(id, ReliveTypes::eDove));
                        if (pDove)
                        {
                            pDove->FlyAway(1);
                        }
                    }

                    mDovesExist = false;

                    BaseGameObject* pThrowableIndicator = sObjectIds.Find(mThrowableTotalIndicator, ReliveTypes::eThrowableTotalIndicator);
                    if (pThrowableIndicator)
                    {
                        pThrowableIndicator->SetDead(true);
                    }
                    SfxPlayMono(relive::SoundEffects::Dove, 70);
                    SetDead(true);
                }
            }
            else
            {
                BaseGameObject* pThrowableIndicator = sObjectIds.Find(mThrowableTotalIndicator, ReliveTypes::eThrowableTotalIndicator);
                if (pThrowableIndicator)
                {
                    pThrowableIndicator->SetDead(true);
                }

                for (const auto& id : mDoveIds)
                {
                    auto pDove = static_cast<Dove*>(sObjectIds.Find(id, ReliveTypes::eDove));
                    if (pDove)
                    {
                        pDove->AsJoin(mXPos, mYPos + (mSpriteScale * FP_FromInteger(20)));
                    }
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
                for (const auto& id : mDoveIds)
                {
                    BaseGameObject* pDove = sObjectIds.Find_Impl(id);
                    if (pDove)
                    {
                        pDove->SetDead(true);
                    }
                }
                mState = PortalStates::CreateTerminators_4;
            }
            break;

        case PortalStates::CreateTerminators_4:
            EventBroadcast(kEventPortalOpen, this);
            if (pTerminator1->GetAnimation().GetIsLastFrame())
            {
                pTerminator1->GetAnimation().Set_Animation_Data(pTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                pTerminator2->GetAnimation().Set_Animation_Data(pTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mTimer = MakeTimer(12);
                mState = PortalStates::ExpandTerminators_5;
                mSfxPlaying = SfxPlayMono(relive::SoundEffects::PortalOpening, 0);
                SFX_Play_Pitch(relive::SoundEffects::IngameTransition, 115, 300);
            }
            break;

        case PortalStates::ExpandTerminators_5:
            EventBroadcast(kEventPortalOpen, this);
            pTerminator1->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            pTerminator2->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
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
                        pTerminator2->mXPos,
                        (FP_FromInteger(10) * mSpriteScale) + pTerminator2->mYPos,
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
                // add the ring effects from AO because why not :)
                if (!(sGnFrame % 8))
                {
                    AbilityRing::Factory(
                        pTerminator1->mXPos,
                        pTerminator1->mYPos + FP_FromInteger(7),
                        RingTypes::eShrykull_Pulse_Orange_6,
                        FP_FromInteger(1));
                    AbilityRing::Factory(
                        pTerminator2->mXPos,
                        pTerminator2->mYPos + FP_FromInteger(7),
                        RingTypes::eShrykull_Pulse_Orange_6,
                        FP_FromInteger(1));
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
                    xOff = mSpriteScale * FP_FromInteger(16);
                }
                else
                {
                    xOff = mSpriteScale * FP_FromInteger(-16);
                }

                const s16 scale32 = FP_GetExponent(mSpriteScale * FP_FromInteger(32));
                if (static_cast<s32>(sGnFrame) >= mTimer)
                {
                    mTimer = MakeTimer(Math_RandomRange(4, 12));

                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        mXPos + xOff,
                        mYPos + FP_FromInteger(Math_RandomRange(-scale32, scale32)),
                        mSpriteScale);

                    SfxPlayMono(relive::SoundEffects::Dove, 70);
                    pDove->SetSpriteScale(mSpriteScale);
                    pDove->AsJoin(gAbe->mXPos, FP_FromInteger(Math_RandomRange(-36, 4)) + gAbe->mYPos);

                    mReceivedDovesCount++;
                    if (mReceivedDovesCount == 6)
                    {
                        mOrbWhirlWind->ToSpin(
                            gAbe->mXPos,
                            gAbe->mYPos - (gAbe->GetSpriteScale() * FP_FromInteger(38)),
                            gAbe->GetSpriteScale(),
                            gAbe);
                        mOrbWhirlWind = nullptr;
                    }
                }
            }
            break;

        case PortalStates::GetShrykull_9:
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                gAbe->field_168_ring_pulse_timer = MakeTimer(32000);
                gAbe->field_16C_bHaveShrykull = true;
                mState = PortalStates::CollapseTerminators_10;
				if (gAbe->mCurrentMotion == eAbeMotions::Motion_150_Chant)
                {
                    gAbe->ChangeChantState(0);
                }
            }
            break;

        case PortalStates::CollapseTerminators_10:
            pTerminator1->mYPos += FP_FromDouble(3.5) * mSpriteScale;
            pTerminator2->mYPos -= FP_FromDouble(3.5) * mSpriteScale;

            if (FP_GetExponent(pTerminator1->mYPos) >= FP_GetExponent(pTerminator2->mYPos))
            {
                auto pParticle = relive_new Particle(
                    pTerminator2->mXPos,
                    pTerminator2->mYPos,
                    GetAnimRes(AnimId::BirdPortal_Flash));

                pParticle->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                pParticle->SetApplyShadowZoneColour(false);
                pParticle->SetSpriteScale(mSpriteScale);

                mState = PortalStates::StopSound_11;
                mTimer = MakeTimer(5);
                pTerminator1->SetDead(true);
                pTerminator2->SetDead(true);
                mTerminatorId1 = Guid{};
                mTerminatorId2 = Guid{};
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
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, relive::TBlendModes::eBlend_3, 1);
            mState = PortalStates::CreateFlash2_13;
        }
        break;

        case PortalStates::CreateFlash2_13:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, relive::TBlendModes::eBlend_0, 1);
            mState = PortalStates::CreateFlash3_14;
        }
        break;

        case PortalStates::CreateFlash3_14:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, relive::TBlendModes::eBlend_0, 1);
            mState = PortalStates::KillPortal_15;
            mTimer = MakeTimer(5);
        }
        break;

        case PortalStates::KillPortal_15:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                if (mPortalType == relive::Path_BirdPortal::PortalType::eWorker ||
                    mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
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
            if (pTerminator1->GetAnimation().GetIsLastFrame())
            {
                pTerminator1->GetAnimation().Set_Animation_Data(pTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                pTerminator2->GetAnimation().Set_Animation_Data(pTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mState = PortalStates::PortalExit_ExpandTerminators_19;
                mTimer = MakeTimer(12);
            }
            break;

        case PortalStates::PortalExit_ExpandTerminators_19:
            pTerminator1->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            pTerminator2->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = PortalStates::PortalExit_AbeExitting_20;
            }
            break;

        case PortalStates::KillPortalClipper_21:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                pTerminator1->GetAnimation().Set_Animation_Data(pTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorShrink));
                pTerminator2->GetAnimation().Set_Animation_Data(pTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorShrink));
                mState = PortalStates::FadeoutTerminators_22;
                mTimer = MakeTimer(30);

                if (pClipper1)
                {
                    pClipper1->SetDead(true);
                }

                if (pClipper2)
                {
                    pClipper2->SetDead(true);
                }
                mScreenClipperId1 = Guid{};
                mScreenClipperId2 = Guid{};

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
                pTerminator1->Fadeout();
                pTerminator2->Fadeout();
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
            gAbe->mXPos,
            gAbe->mYPos - (gAbe->GetSpriteScale() * FP_FromInteger(38)),
            gAbe->GetSpriteScale(),
            false);

        if (gAbe->mCurrentMotion == eAbeMotions::Motion_150_Chant)
        {
            gAbe->ChangeChantState(1);
        }
    }
    else
    {
        mState = PortalStates::CollapseTerminators_10;
        if ((mPortalType == relive::Path_BirdPortal::PortalType::eWorker || mPortalType == relive::Path_BirdPortal::PortalType::eShrykull) && gAbe->mCurrentMotion == eAbeMotions::Motion_150_Chant)
        {
            gAbe->ChangeChantState(0);
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
    if (mState <= PortalStates::IdlePortal_1 || mState >= PortalStates::KillPortalClipper_21 || ((gMap.LevelChanged() || gMap.PathChanged()) && (mState != PortalStates::AbeInsidePortal_16 || mPortalType != relive::Path_BirdPortal::PortalType::eAbe || gMap.mNextLevel != mExitLevel || gMap.mNextPath != mExitPath)))
    {
        SetDead(true);
    }
    BaseGameObject* pTerminator1 = sObjectIds.Find_Impl(mTerminatorId1);
    BaseGameObject* pTerminator2 = sObjectIds.Find_Impl(mTerminatorId2);
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

    if (GetDead())
    {
        if (pTerminator1)
        {
            pTerminator1->SetDead(true);
        }

        if (pTerminator2)
        {
            pTerminator2->SetDead(true);
        }

        if (pClipper1)
        {
            pClipper1->SetDead(true);
        }

        if (pClipper2)
        {
            pClipper2->SetDead(true);
        }

        mTerminatorId1 = Guid{};
        mTerminatorId2 = Guid{};
        mScreenClipperId1 = Guid{};
        mScreenClipperId2 = Guid{};
    }
    else if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
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
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

    mScreenClipperId1 = Guid{};
    mScreenClipperId2 = Guid{};

    if (pClipper1)
    {
        pClipper1->SetDead(true);
        if (pClipper2)
        {
            pClipper2->SetDead(true);
        }
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
            gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_InBirdPortal_Half_11);
        }
        else
        {
            mSpriteScale = FP_FromInteger(1);
            gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_InBirdPortal_30);
        }

        gAbe->SetSpriteScale(mSpriteScale);
        gAbe->mCurrentLevel = gMap.mCurrentLevel;
        gAbe->mCurrentPath = gMap.mCurrentPath;

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

    // Negative cases
    // mMovieId -3 is for when you return from paramonia and -4 is for scrabania
    if (gAbe->mParamoniaDone && gAbe->mScrabaniaDone)
    {
        *movieId = 1617 - (10000 * mMovieId);
        *screenChangeEffect = CameraSwapEffects::ePlay1FMV_5;
        return;
    }

    *movieId = 17 - (100 * mMovieId);
    *screenChangeEffect = CameraSwapEffects::ePlay1FMV_5;
}
void BirdPortal::CreateTerminators()
{
    auto pTerminator1 = relive_new BirdPortalTerminator(mXPos, mYPos, mSpriteScale, mPortalType);
    if (pTerminator1)
    {
        mTerminatorId1 = pTerminator1->mBaseGameObjectId;
    }

    auto pTerminator2 = relive_new BirdPortalTerminator(mXPos, mYPos, mSpriteScale, mPortalType);
    if (pTerminator2)
    {
        mTerminatorId2 = pTerminator2->mBaseGameObjectId;
    }
}

bool BirdPortal::VPortalClipper(bool bIgnoreClipping)
{
    if (bIgnoreClipping && mState != PortalStates::ActivePortal_6)
    {
        return false;
    }

    if (mScreenClipperId1 != Guid{})
    {
        return true;
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

    // Clip objects entering portal?
    auto pClipper1 = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
    if (pClipper1)
    {
        mScreenClipperId1 = pClipper1->mBaseGameObjectId;
        if (mSpriteScale == FP_FromInteger(1))
        {
            pClipper1->mOtLayer = Layer::eLayer_BirdPortal_29;
        }
        else
        {
            pClipper1->mOtLayer = Layer::eLayer_BirdPortal_Half_10;
        }
    }

    // Clip whole screen when "in" the portal?
    auto pClipper2 = relive_new ScreenClipper(PSX_Point{0, 0}, PSX_Point{640, 240}, Layer::eLayer_0);
    if (pClipper2)
    {
        mScreenClipperId2 = pClipper2->mBaseGameObjectId;
        if (mSpriteScale == FP_FromInteger(1))
        {
            pClipper2->mOtLayer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;
        }
        else
        {
            pClipper2->mOtLayer = Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12;
        }
    }

    return true;
}

} // namespace AO

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
#include "../relive_lib/GameObjects/BirdPortalTerminator.hpp"

namespace AO {

BirdPortal::BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId)
{
    mPortalType = pTlv->mPortalType;
    mEnterSide = pTlv->mEnterSide;
    mTlvInfo = tlvId;
    mMovieId = pTlv->mMovieId;
    mExitLevel = pTlv->mExitLevel;
    mExitPath = pTlv->mExitPath;
    mExitCamera = pTlv->mExitCamera;
    mMudCountForShrykull = pTlv->mMudCountForShrykull;

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
            if (!EventGet(Event::kEventAbeOhm) || pTarget)
            {
                if (IsScaredAway() || EventGet(Event::kEventShooting) || (EventGet(Event::kEventAbeOhm) && pTarget))
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
                EventBroadcast(Event::kEventPortalOpen, this);
                SfxPlayMono(relive::SoundEffects::Dove, 70);
            }
        }
        break;

        case PortalStates::JoinDovesInCenter_2:
            EventBroadcast(Event::kEventPortalOpen, this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                CreateTerminators();
                mTimer = MakeTimer(5);
                mState = PortalStates::KillDoves_3;
            }
            break;

        case PortalStates::KillDoves_3:
            EventBroadcast(Event::kEventPortalOpen, this);
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
            EventBroadcast(Event::kEventPortalOpen, this);
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
            EventBroadcast(Event::kEventPortalOpen, this);
            pTerminator1->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            pTerminator2->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = PortalStates::ActivePortal_6;
            }
            break;

        case PortalStates::ActivePortal_6:
            EventBroadcast(Event::kEventPortalOpen, this);
            if ((mPortalType != relive::Path_BirdPortal::PortalType::eWorker && mPortalType != relive::Path_BirdPortal::PortalType::eShrykull) || EventGet(Event::kEventAbeOhm))
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
                gAbe->mRingPulseTimer = MakeTimer(32000);
                gAbe->mHaveShrykull = true;
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

    if (EventGet(Event::kEventDeathReset))
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

void BirdPortal::VExitPortal()
{
    mCurrentPath = gMap.mCurrentPath;
    mCurrentLevel = gMap.mCurrentLevel;

    auto pPortalExitTlv = static_cast<relive::Path_BirdPortalExit*>(GetMap().TLV_First_Of_Type_In_Camera(ReliveTypes::eBirdPortalExit, 0));
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

        mExitX = FP_FromInteger(pPortalExitTlv->mTopLeftX);
        mXPos = mExitX;
        mYPos = mExitY - FP_FromInteger(55);
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

} // namespace AO

#include "stdafx.h"
#include "BirdPortal.hpp"
#include "Map.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Dove.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Flash.hpp"
#include "../relive_lib/Particle.hpp"
#include "Abe.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "OrbWhirlWind.hpp"
#include "ScreenClipper.hpp"
#include "AbilityRing.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FatalError.hpp"

void BirdPortal::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Sparks));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Flash));
}

BirdPortal::BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mDovesExist = 0;
    SetType(ReliveTypes::eBirdPortal);
    mBaseGameObjectTlvInfo = tlvId;
    mThrowableIndicatorId = Guid{};

    LoadAnimations();

    mTlvInfo = tlvId;

    mEnterSide = pTlv->mEnterSide;
    mExitLevel = pTlv->mExitLevel;
    mExitPath = pTlv->mExitPath;
    mExitCamera = pTlv->mExitCamera;
    mMovieId = pTlv->mMovieId;
    mPortalType = pTlv->mPortalType;
    mMudCountForShrykull = pTlv->mMudCountForShrykull;
    mDeletePortalSwitchId = pTlv->mDeletePortalSwitchId;

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
    mTerminatorId1 = Guid{};
    mTerminatorId2 = Guid{};
    mScreenClipperId1 = Guid{};
    mScreenClipperId2 = Guid{};
    mOrbWhirlWind = nullptr;

    FP hitX = {};
    PathLine* pLine = nullptr;
    sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &pLine,
        &hitX,
        &mHitY,
        mSpriteScale > FP_FromDouble(0.5) ? kFgFloor : kBgFloor);

    mXPos = FP_FromInteger((pTlv->mBottomRightX + pTlv->mTopLeftX) / 2);
    mSfxPlaying = 0;
    mYPos = mHitY - (FP_FromInteger(55) * mSpriteScale);
}

void BirdPortal::CreateDovesAndShrykullNumber()
{
    for (u8 i = 0; i < ALIVE_COUNTOF(mDoveIds); i++)
    {
        auto pDove = relive_new Dove(AnimId::Dove_Flying, mXPos, mYPos, mSpriteScale);
        mDoveIds[i] = pDove->mBaseGameObjectId;

        mDovesExist = 1;

        if (mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
        {
            pDove->AsAlmostACircle(mXPos, (mSpriteScale * FP_FromInteger(30)) + mYPos, 42 * i);
        }
        else
        {
            pDove->AsACircle(mXPos, (mSpriteScale * FP_FromInteger(30)) + mYPos, 42 * i);
        }
        pDove->SetSpriteScale(mSpriteScale);
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
            mThrowableIndicatorId = pIndicator->mBaseGameObjectId;
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
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                SfxPlayMono(relive::SoundEffects::Dove, 35, mSpriteScale);
                mTimer = sGnFrame + Math_RandomRange(24, 40);
            }

            if (EventGet(kEventAbeOhm))
            {
                BaseGameObject* pShrykullNumMuds = sObjectIds.Find_Impl(mThrowableIndicatorId);
                if (pShrykullNumMuds)
                {
                    pShrykullNumMuds->SetDead(true);
                }

                for (auto& id : mDoveIds)
                {
                    auto pDove = static_cast<Dove*>(sObjectIds.Find_Impl(id));
                    if (pDove)
                    {
                        pDove->AsJoin(mXPos, mYPos + (mSpriteScale * FP_FromInteger(20)));
                    }
                }

                mState = PortalStates::JoinDovesInCenter_2;
                mTimer = sGnFrame + 15;
                EventBroadcast(GetEvent(), this);
                SfxPlayMono(relive::SoundEffects::Dove, 70, mSpriteScale);
            }
            else if (IsScaredAway() || EventGet(kEventShooting))
            {
                for (auto& id : mDoveIds)
                {
                    Dove* pDove = static_cast<Dove*>(sObjectIds.Find_Impl(id));
                    if (pDove)
                    {
                        pDove->FlyAway(1);
                    }
                }

                mDovesExist = 0;

                BaseGameObject* pThrowableIndicator = sObjectIds.Find_Impl(mThrowableIndicatorId);
                if (pThrowableIndicator)
                {
                    pThrowableIndicator->SetDead(true);
                }
                SfxPlayMono(relive::SoundEffects::Dove, 70, mSpriteScale);
                SetDead(true);
            }
            break;

        case PortalStates::JoinDovesInCenter_2:
            EventBroadcast(GetEvent(), this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                CreateTerminators();
                mTimer = sGnFrame + 5;
                mState = PortalStates::KillDoves_3;
            }
            break;

        case PortalStates::KillDoves_3:
            EventBroadcast(GetEvent(), this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                for (auto& id : mDoveIds)
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
            EventBroadcast(GetEvent(), this);
            if (pTerminator1->GetAnimation().GetIsLastFrame())
            {
                pTerminator1->GetAnimation().Set_Animation_Data(pTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                pTerminator2->GetAnimation().Set_Animation_Data(pTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mTimer = sGnFrame + 12;
                mState = PortalStates::ExpandTerminators_5;
                mSfxPlaying = SfxPlayMono(relive::SoundEffects::PortalOpening, 0, mSpriteScale);
                SFX_Play_Pitch(relive::SoundEffects::IngameTransition, 115, 300, mSpriteScale);
            }
            break;

        case PortalStates::ExpandTerminators_5:
            EventBroadcast(GetEvent(), this);
            pTerminator1->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            pTerminator2->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = PortalStates::ActivePortal_6;
            }
            break;

        case PortalStates::ActivePortal_6:
            EventBroadcast(GetEvent(), this);
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
                            pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                            pParticle->SetType(ReliveTypes::eBirdPortalTerminator);
                            pParticle->SetSpriteScale(mSpriteScale);

                            if (static_cast<s32>(sGnFrame) % 2)
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
                        pTerminator1->mYPos,
                        RingTypes::eShrykull_Pulse_Orange_6,
                        mSpriteScale);
                    AbilityRing::Factory(
                        pTerminator2->mXPos,
                        pTerminator2->mYPos,
                        RingTypes::eShrykull_Pulse_Orange_6,
                        mSpriteScale);
                }
            }
            else
            {
                VGiveShrykull(0);
                if (mState != PortalStates::ShrykullGetDoves_7)
                {
                    mState = PortalStates::FadeoutTerminators_22;
                    mTimer = sGnFrame + 30;
                }
            }
            break;

        case PortalStates::ShrykullGetDoves_7:
            BirdPortal::KillTerminators();
            if (mReceivedDovesCount >= 7)
            {
                mState = PortalStates::GetShrykull_9;
                mTimer = sGnFrame + 20;
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
                const s16 xOffExp = FP_GetExponent(xOff);
                const s16 scale32 = FP_GetExponent(mSpriteScale * FP_FromInteger(32));

                if (static_cast<s32>(sGnFrame) >= mTimer)
                {
                    mTimer = sGnFrame + Math_RandomRange(4, 12);

                    auto pDove_1 = relive_new Dove(
                        AnimId::Dove_Flying,
                        FP_FromInteger(xOffExp) + mXPos,
                        mYPos + FP_FromInteger(Math_RandomRange(-scale32, scale32)),
                        mSpriteScale);

                    SfxPlayMono(relive::SoundEffects::Dove, 70, mSpriteScale);
                    pDove_1->SetSpriteScale(mSpriteScale);
                    pDove_1->AsJoin(sActiveHero->mXPos, FP_FromInteger(Math_RandomRange(-36, 4)) + sActiveHero->mYPos);
                    mReceivedDovesCount++;
                    if (mReceivedDovesCount == 6)
                    {
                        mOrbWhirlWind->ToSpin(
                            sActiveHero->mXPos,
                            sActiveHero->mYPos - (sActiveHero->GetSpriteScale() * FP_FromInteger(38)),
                            sActiveHero->GetSpriteScale(),
                            sActiveHero);
                        mOrbWhirlWind = nullptr;
                    }
                }
            }
            break;

        case PortalStates::GetShrykull_9:
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                sActiveHero->mRingPulseTimer = sGnFrame + 32000;
                sActiveHero->mHaveShrykull = true;
                sActiveHero->mHaveInvisibility = 0;
                mState = PortalStates::CollapseTerminators_10;
                if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_112_Chant)
                {
                    sActiveHero->ChangeChantState_45BB90(0);
                }
            }
            break;

        case PortalStates::CollapseTerminators_10:
            BirdPortal::KillTerminators();
            pTerminator1->mYPos += (FP_FromDouble(3.5) * mSpriteScale);
            pTerminator2->mYPos -= (FP_FromDouble(3.5) * mSpriteScale);
            if (FP_GetExponent(pTerminator1->mYPos) >= FP_GetExponent(pTerminator2->mYPos))
            {
                auto pParticle = relive_new Particle(
                    pTerminator2->mXPos,
                    pTerminator2->mYPos,
                    GetAnimRes(AnimId::BirdPortal_Flash));
                pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                pParticle->SetApplyShadowZoneColour(false);
                pParticle->SetSpriteScale(mSpriteScale);

                mState = PortalStates::StopSound_11;
                mTimer = sGnFrame + 5;
                pTerminator1->SetDead(true);
                pTerminator2->SetDead(true);
                mTerminatorId1 = Guid{};
                mTerminatorId2 = Guid{};
                SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 100, -1800, mSpriteScale);
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
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, TPageAbr::eBlend_3, 1);
            mState = PortalStates::CreateFlash2_13;
        }
        break;

        case PortalStates::CreateFlash2_13:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, TPageAbr::eBlend_0, 1);
            mState = PortalStates::CreateFlash3_14;
        }
        break;

        case PortalStates::CreateFlash3_14:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, TPageAbr::eBlend_0, 1);
            mState = PortalStates::KillPortal_15;
            mTimer = sGnFrame + 5;
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
            mSfxPlaying = SfxPlayMono(relive::SoundEffects::PortalOpening, 0, mSpriteScale);
        }
        break;

        case PortalStates::PortalExit_CreateTerminators_18:
            if (pTerminator1->GetAnimation().GetIsLastFrame())
            {
                pTerminator1->GetAnimation().Set_Animation_Data(pTerminator1->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                pTerminator2->GetAnimation().Set_Animation_Data(pTerminator2->GetAnimRes(AnimId::BirdPortal_TerminatorIdle));
                mState = PortalStates::PortalExit_ExpandTerminators_19;
                mTimer = sGnFrame + 12;
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
                mTimer = sGnFrame + 30;

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

                if (mSpriteScale == FP_FromInteger(1))
                {
                    sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                }
                else
                {
                    sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
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
            case ReliveTypes::eAbe:
            case ReliveTypes::eRingOrLiftMud:
            case ReliveTypes::eParamite:
            case ReliveTypes::eScrab:
            case ReliveTypes::eSlig:
                if (pObj->mCurrentPath != mCurrentPath)
                {
                    continue;
                }

                if (FP_Abs(pObj->mXPos - mXPos) >= FP_NoFractional((mSpriteScale * FP_FromInteger(75))))
                {
                    continue;
                }

                if (FP_Abs(pObj->mYPos - mHitY) >= FP_FromInteger(30) || pObj->GetSpriteScale() != mSpriteScale)
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
    if (sActiveHero)
    {
        if (mPortalType == relive::Path_BirdPortal::PortalType::eShrykull && mMudCountForShrykull <= 0)
        {
            SND_SEQ_Play(SeqId::SecretMusic_32, 1, 127, 127);
            mState = PortalStates::ShrykullGetDoves_7;
            mTimer = sGnFrame + 12;
            mReceivedDovesCount = 0;

            mOrbWhirlWind = relive_new OrbWhirlWind(
                sActiveHero->mXPos,
                sActiveHero->mYPos - (sActiveHero->GetSpriteScale() * FP_FromInteger(38)),
                sActiveHero->GetSpriteScale(),
                0);

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_112_Chant)
            {
                sActiveHero->ChangeChantState_45BB90(1);
            }
        }
        else
        {
            mState = PortalStates::CollapseTerminators_10;
            if ((mPortalType == relive::Path_BirdPortal::PortalType::eWorker || mPortalType == relive::Path_BirdPortal::PortalType::eShrykull) && sActiveHero->mCurrentMotion == eAbeMotions::Motion_112_Chant)
            {
                sActiveHero->ChangeChantState_45BB90(0);
            }
        }

        if (bPlaySound)
        {
            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 70, -1600, mSpriteScale);
        }
    }
}
void BirdPortal::VScreenChanged()
{
    if (mState <= PortalStates::IdlePortal_1 || mState >= PortalStates::KillPortalClipper_21 || ((gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath) &&

                                                                                                                 (mState != PortalStates::AbeInsidePortal_16 || mPortalType != relive::Path_BirdPortal::PortalType::eAbe || gMap.mNextLevel != mExitLevel || gMap.mNextPath != mExitPath)))
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

s32 BirdPortal::VGetSaveState(u8* pBuffer)
{
    auto pState = reinterpret_cast<BirdPortalSaveState*>(pBuffer);
    auto pTlv = static_cast<relive::Path_BirdPortal*>(gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));

    s16 numMudsForShrykull = 0;
    if (pTlv)
    {
        numMudsForShrykull = pTlv->mMudCountForShrykull;
    }

    pState->mAEType = ReliveTypes::eBirdPortal;
    pState->mTlvInfo = mTlvInfo;
    pState->mState = static_cast<u8>(mState);
    pState->mMudCountForShrykull = static_cast<u8>(numMudsForShrykull - mMudCountForShrykull);

    return sizeof(BirdPortalSaveState);
}

void BirdPortal::VRender(PrimHeader** /*ppOt*/)
{
    // Null
}

s32 BirdPortal::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const BirdPortalSaveState*>(pBuffer);
    auto pTlv = static_cast<relive::Path_BirdPortal*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvInfo));
    if (!pTlv)
    {
        return sizeof(BirdPortalSaveState);
    }

    auto pPortal = relive_new BirdPortal(pTlv, pSaveState->mTlvInfo);
    if (pPortal)
    {
        pPortal->SetUpdateDelay(1);
        pPortal->mMudCountForShrykull -= pSaveState->mMudCountForShrykull;

        const auto savedState = static_cast<PortalStates>(pSaveState->mState);
        switch (savedState)
        {
        case PortalStates::JoinDovesInCenter_2:
        case PortalStates::KillDoves_3:
        case PortalStates::CreateTerminators_4:
        case PortalStates::ExpandTerminators_5:
        case PortalStates::ActivePortal_6:
        {
            pPortal->mState = PortalStates::ActivePortal_6;
            pPortal->CreateTerminators();
            auto pTerminator1 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(pPortal->mTerminatorId1));
            auto pTerminator2 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(pPortal->mTerminatorId2));
            pTerminator1->mYPos -= (FP_FromInteger(45) * pPortal->mSpriteScale);
            pTerminator2->mYPos += (FP_FromInteger(45) * pPortal->mSpriteScale);
            break;
        }

        case PortalStates::Unused_8:
        {
            ALIVE_FATAL("PortalStates::Unused_8 never expected to be used");
            break;
        }

        case PortalStates::ShrykullGetDoves_7:
        case PortalStates::GetShrykull_9:
        {
            pPortal->CreateTerminators();
            auto pTerminator1 = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pPortal->mTerminatorId1));
            auto pTerminator2 = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pPortal->mTerminatorId2));
            pTerminator1->mYPos -= (FP_FromInteger(45) * pPortal->mSpriteScale);
            pTerminator2->mYPos += (FP_FromInteger(45) * pPortal->mSpriteScale);
            pPortal->mState = PortalStates::GetShrykull_9;
            pPortal->mTimer = sGnFrame + 20;
            break;
        }
        default:
            break;
        }
    }

    return sizeof(BirdPortalSaveState);
}

s16 BirdPortal::VPortalClipper(s16 bIgnoreClipping)
{
    if (bIgnoreClipping && mState != PortalStates::ActivePortal_6)
    {
        return 0;
    }

    if (mScreenClipperId1 != Guid{})
    {
        return 1;
    }

    PSX_Point xy = {};
    PSX_Point wh = {};
    if (mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft)
    {
        xy.x = 0;
        xy.y = 0;

        wh.x = PsxToPCX(FP_GetExponent(mXPos - gScreenManager->CamXPos()), 11);
        wh.y = 240;
    }
    else
    {
        xy.x = PsxToPCX(FP_GetExponent(mXPos - gScreenManager->CamXPos()), 11);
        xy.y = 0;

        wh.x = 640;
        wh.y = 240;
    }

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
    auto pClipper2 = relive_new ScreenClipper(PSX_Point{ 0, 0 }, PSX_Point{ 640, 240 }, Layer::eLayer_0);
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

    return 1;
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

void BirdPortal::VMudSaved()
{
    mMudCountForShrykull--;
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

    auto pPortalExitTlv = static_cast<relive::Path_BirdPortalExit*>(gPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eBirdPortalExit, 0));
    if (pPortalExitTlv)
    {
        // TODO: Clean up this hack by having a better way to match "any" type of line
        CollisionMask allLinesHack;
        allLinesHack.mMask = 0xFFFFFFFF;

        PathLine* pLine = nullptr;
        sCollisions->Raycast(
            FP_FromInteger(pPortalExitTlv->mTopLeftX),
            FP_FromInteger(pPortalExitTlv->mTopLeftY),
            FP_FromInteger(pPortalExitTlv->mBottomRightX),
            FP_FromInteger(pPortalExitTlv->mBottomRightY),
            &pLine,
            &mExitX,
            &mExitY,
            allLinesHack);

        mExitX = FP_FromInteger((pPortalExitTlv->mTopLeftX + pPortalExitTlv->mBottomRightX) / 2);
        mXPos = mExitX;
        mYPos = mExitY - FP_FromInteger(55);
        mEnterSide = pPortalExitTlv->mExitSide;

        if (pPortalExitTlv->mScale == relive::reliveScale::eHalf)
        {
            mSpriteScale = FP_FromDouble(0.5);
            sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_InBirdPortal_Half_11);
            sActiveHero->SetScale(Scale::Bg);
        }
        else
        {
            mSpriteScale = FP_FromInteger(1);
            sActiveHero->GetAnimation().SetRenderLayer(Layer::eLayer_InBirdPortal_30);
            sActiveHero->SetScale(Scale::Fg);
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
    mTimer = sGnFrame + 30;
}

void BirdPortal::VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId)
{
    *level = mExitLevel;
    *path = mExitPath;
    *camera = mExitCamera;

    if (mMovieId <= 0)
    {
        *screenChangeEffect = CameraSwapEffects::eInstantChange_0;
    }
    else
    {
        *movieId = mMovieId;
        *screenChangeEffect = CameraSwapEffects::ePlay1FMV_5;
    }
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

    BaseGameObject* pDoves = sObjectIds.Find_Impl(mDoveIds[0]);
    if (pDoves)
    {
        if (mDovesExist)
        {
            for (auto doveId : mDoveIds)
            {
                BaseGameObject* pDove = sObjectIds.Find_Impl(doveId);
                if (pDove)
                {
                    pDove->SetDead(true);
                }
            }
        }
    }

    BaseGameObject* pThrowableIndicator = sObjectIds.Find_Impl(mThrowableIndicatorId);
    if (pThrowableIndicator)
    {
        pThrowableIndicator->SetDead(true);
    }

    if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
    }

    if (SwitchStates_Get(mDeletePortalSwitchId))
    {
        // Never come back
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }
    else
    {
        // Always come back
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
}

void BirdPortal::KillTerminators()
{
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

Event BirdPortal::GetEvent()
{
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eBirdPortal)
        {
            if (pObj == this)
            {
                return kEventPortalOpen;
            }
            else
            {
                return kEventOtherPortalOpen;
            }
        }
    }
    return kEventPortalOpen;
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
    SetType(ReliveTypes::eEyeOrbPart);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::BirdPortal_TerminatorGrow));

    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    SetSpriteScale(scale);
    if (GetSpriteScale() == FP_FromInteger(1))
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

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
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

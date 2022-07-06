#include "stdafx.h"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Dove.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Flash.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "OrbWhirlWind.hpp"
#include "ScreenClipper.hpp"
#include "Sys_common.hpp"
#include "AbilityRing.hpp"

BirdPortal::BirdPortal(Path_BirdPortal* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mDovesExist = 0;
    SetType(ReliveTypes::eBirdPortal);
    mBaseGameObjectTlvInfo = tlvInfo;
    mThrowableIndicatorId = -1;

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kPortalTerminatorResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDovbasicResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kPortliteResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSplineResID);

    mTlvInfo = tlvInfo;

    mEnterSide = pTlv->mEnterSide;
    mExitLevel = MapWrapper::FromAE(pTlv->mExitLevel);
    mExitPath = pTlv->mExitPath;
    mExitCamera = pTlv->mExitCamera;
    mMovieId = pTlv->mMovieId;
    mPortalType = pTlv->mPortalType;
    mMudCountForShrykull = pTlv->mMudCountForShrykull;
    mDeletePortalSwitchId = pTlv->mDeletePortalSwitchId;

    if (pTlv->mScale == Scale_short::eHalf_1)
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
    mTerminatorId1 = -1;
    mTerminatorId2 = -1;
    mScreenClipperId1 = -1;
    mScreenClipperId2 = -1;
    mOrbWhirlWind = nullptr;

    FP hitX = {};
    PathLine* pLine = nullptr;
    sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeft.x),
        FP_FromInteger(pTlv->mTopLeft.y),
        FP_FromInteger(pTlv->mBottomRight.x),
        FP_FromInteger(pTlv->mBottomRight.y),
        &pLine,
        &hitX,
        &mHitY,
        mSpriteScale > FP_FromDouble(0.5) ? kFgFloor : kBgFloor);

    mXPos = FP_FromInteger((pTlv->mBottomRight.x + pTlv->mTopLeft.x) / 2);
    mSfxPlaying = 0;
    mYPos = mHitY - (FP_FromInteger(55) * mSpriteScale);
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
            mSfxPlaying = SFX_Play_Camera(SoundEffect::PortalOpening_58, 45, direction);
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
                SfxPlayMono(SoundEffect::Dove_13, 35, mSpriteScale);
                mTimer = sGnFrame + Math_RandomRange(24, 40);
            }

            if (EventGet(kEventAbeOhm))
            {
                BaseGameObject* pShrykullNumMuds = sObjectIds.Find_Impl(mThrowableIndicatorId);
                if (pShrykullNumMuds)
                {
                    pShrykullNumMuds->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                SfxPlayMono(SoundEffect::Dove_13, 70, mSpriteScale);
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
                    pThrowableIndicator->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                SfxPlayMono(SoundEffect::Dove_13, 70, mSpriteScale);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case PortalStates::JoinDovesInCenter_2:
            EventBroadcast(GetEvent(), this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                CreateTerminators();
                mState = PortalStates::KillDoves_3;
                mTimer = sGnFrame + 5;
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
                        pDove->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    }
                }
                mState = PortalStates::CreateTerminators_4;
            }
            break;

        case PortalStates::CreateTerminators_4:
            EventBroadcast(GetEvent(), this);
            if (pTerminator1->mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                pTerminator1->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                pTerminator2->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                mTimer = sGnFrame + 12;
                mState = PortalStates::ExpandTerminators_5;
                mSfxPlaying = SfxPlayMono(SoundEffect::PortalOpening_58, 0, mSpriteScale);
                SFX_Play_Pitch(SoundEffect::IngameTransition_84, 115, 300, mSpriteScale);
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
            if ((mPortalType != PortalType::eWorker_1 && mPortalType != PortalType::eShrykull_2) || EventGet(kEventAbeOhm))
            {
                if ((Math_NextRandom() % 8) == 0)
                {
                    const AnimRecord& rec = AnimRec(AnimId::BirdPortal_Sparks);
                    u8** ppLightRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, TRUE, FALSE);
                    if (ppLightRes)
                    {
                        auto pParticle = relive_new Particle(pTerminator2->mXPos,
                            (FP_FromInteger(10) * mSpriteScale) + pTerminator2->mYPos,
                            AnimId::BirdPortal_Sparks,
                            ppLightRes);

                        if (pParticle)
                        {
                            pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
                            pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                            pParticle->SetType(ReliveTypes::eBirdPortalTerminator);
                            pParticle->mSpriteScale = mSpriteScale;

                            if (static_cast<s32>(sGnFrame) % 2)
                            {
                                pParticle->mAnim.mFlags.Set(AnimFlags::eBit19_LoopBackwards);
                                pParticle->mAnim.SetFrame(pParticle->mAnim.Get_Frame_Count());
                            }
                        }

                        if (direction == CameraPos::eCamCurrent_0)
                        {
                            SFX_Play_Pitch(SoundEffect::BirdPortalSpark_41, 50, 2400, mSpriteScale);
                        }
                    }
                    ResourceManager::FreeResource_49C330(ppLightRes);
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
                if (mEnterSide == PortalSide::eLeft_1)
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

                    SfxPlayMono(SoundEffect::Dove_13, 70, mSpriteScale);
                    pDove_1->mSpriteScale = mSpriteScale;
                    pDove_1->AsJoin(sActiveHero->mXPos, FP_FromInteger(Math_RandomRange(-36, 4)) + sActiveHero->mYPos);
                    mReceivedDovesCount++;
                    if (mReceivedDovesCount == 6)
                    {
                        mOrbWhirlWind->ToSpin(
                            sActiveHero->mXPos,
                            sActiveHero->mYPos - (sActiveHero->mSpriteScale * FP_FromInteger(38)),
                            sActiveHero->mSpriteScale,
                            sActiveHero);
                        mOrbWhirlWind = nullptr;
                    }
                }
            }
            break;

        case PortalStates::GetShrykull_9:
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                sActiveHero->Get_Shrykull_Resources_45AA20();
                sActiveHero->field_168_ring_pulse_timer = sGnFrame + 32000;
                sActiveHero->field_16C_bHaveShrykull = TRUE;
                sActiveHero->field_16E_bHaveInvisiblity = 0;
                mState = PortalStates::CollapseTerminators_10;
                if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_112_Chant_45B1C0)
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
                const AnimRecord& rec = AnimRec(AnimId::BirdPortal_Flash);
                u8** ppLightRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
                if (ppLightRes)
                {
                    auto pParticle = relive_new Particle(
                        pTerminator2->mXPos,
                        pTerminator2->mYPos,
                        AnimId::BirdPortal_Flash,
                        ppLightRes);
                    pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                    pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
                    pParticle->mSpriteScale = mSpriteScale;
                }

                mState = PortalStates::StopSound_11;
                mTimer = sGnFrame + 5;
                pTerminator1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                pTerminator2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                mTerminatorId1 = -1;
                mTerminatorId2 = -1;
                SFX_Play_Pitch(SoundEffect::MenuNavigation_52, 100, -1800, mSpriteScale);
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
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);
            mState = PortalStates::CreateFlash2_13;
        }
        break;

        case PortalStates::CreateFlash2_13:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, 1, TPageAbr::eBlend_0, 1);
            mState = PortalStates::CreateFlash3_14;
        }
        break;

        case PortalStates::CreateFlash3_14:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255, 255, 255, 0, TPageAbr::eBlend_0, 1);
            mState = PortalStates::KillPortal_15;
            mTimer = sGnFrame + 5;
        }
        break;

        case PortalStates::KillPortal_15:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                if (mPortalType == PortalType::eWorker_1 || mPortalType == PortalType::eShrykull_2)
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                else
                {
                    mState = PortalStates::AbeInsidePortal_16;
                }
            }
            break;

        case PortalStates::PortalExit_SetPosition_17:
        {
            pScreenManager->EnableRendering();
            pScreenManager->InvalidateRectCurrentIdx(
                0,
                0,
                gPsxDisplay.mWidth,
                gPsxDisplay.mHeight);

            CreateTerminators();

            mState = PortalStates::PortalExit_CreateTerminators_18;
            mSfxPlaying = SfxPlayMono(SoundEffect::PortalOpening_58, 0, mSpriteScale);
        }
        break;

        case PortalStates::PortalExit_CreateTerminators_18:
            if (pTerminator1->mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                pTerminator1->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                pTerminator2->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
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
                pTerminator1->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorShrink, nullptr);
                pTerminator2->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorShrink, nullptr);
                mState = PortalStates::FadeoutTerminators_22;
                mTimer = sGnFrame + 30;

                if (pClipper1)
                {
                    pClipper1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }

                if (pClipper2)
                {
                    pClipper2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                mScreenClipperId1 = -1;
                mScreenClipperId2 = -1;

                if (mSpriteScale == FP_FromInteger(1))
                {
                    sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
                }
                else
                {
                    sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
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
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}


void BirdPortal::VScreenChanged()
{
    if (mState <= PortalStates::IdlePortal_1 || mState >= PortalStates::KillPortalClipper_21 || ((gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath) &&

                                                                                                                 (mState != PortalStates::AbeInsidePortal_16 || mPortalType != PortalType::eAbe_0 || gMap.mNextLevel != mExitLevel || gMap.mNextPath != mExitPath)))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    BaseGameObject* pTerminator1 = sObjectIds.Find_Impl(mTerminatorId1);
    BaseGameObject* pTerminator2 = sObjectIds.Find_Impl(mTerminatorId2);
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (pTerminator1)
        {
            pTerminator1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        if (pTerminator2)
        {
            pTerminator2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        if (pClipper1)
        {
            pClipper1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        if (pClipper2)
        {
            pClipper2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        mTerminatorId1 = -1;
        mTerminatorId2 = -1;
        mScreenClipperId1 = -1;
        mScreenClipperId2 = -1;
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
    auto pState = reinterpret_cast<BirdPortal_State*>(pBuffer);
    auto pTlv = static_cast<Path_BirdPortal*>(sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));

    s16 numMudsForShrykull = 0;
    if (pTlv)
    {
        numMudsForShrykull = pTlv->mMudCountForShrykull;
    }

    pState->mAEType = AETypes::eBirdPortal_99;
    pState->mTlvInfo = mTlvInfo;
    pState->mState = static_cast<u8>(mState);
    pState->mMudCountForShrykull = static_cast<u8>(numMudsForShrykull - mMudCountForShrykull);

    return sizeof(BirdPortal_State);
}

void BirdPortal::VRender(PrimHeader** /*ppOt*/)
{
    // Null
}

s32 BirdPortal::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const BirdPortal_State*>(pBuffer);
    auto pTlv = static_cast<Path_BirdPortal*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvInfo));
    if (!pTlv)
    {
        return sizeof(BirdPortal_State);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kPortliteResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("PORTAL.BND", nullptr);
    }

    if (pTlv->mPortalType == PortalType::eShrykull_2)
    {
        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("SPLINE.BAN", nullptr);
        }

        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("SHRYPORT.BND", nullptr);
        }
    }

    auto pPortal = relive_new BirdPortal(pTlv, pSaveState->mTlvInfo);
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
            auto pTerminator1 = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pPortal->mTerminatorId1));
            auto pTerminator2 = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pPortal->mTerminatorId2));
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
    return sizeof(BirdPortal_State);
}

s16 BirdPortal::VPortalClipper(s16 bIgnoreClipping)
{
    if (bIgnoreClipping && mState != PortalStates::ActivePortal_6)
    {
        return 0;
    }

    if (mScreenClipperId1 != -1)
    {
        return 1;
    }

    PSX_Point xy = {};
    PSX_Point wh = {};
    if (mEnterSide == PortalSide::eLeft_1)
    {
        xy.x = 0;
        xy.y = 0;

        wh.x = PsxToPCX(FP_GetExponent(mXPos - pScreenManager->CamXPos()), 11);
        wh.y = 240;
    }
    else
    {
        xy.x = PsxToPCX(FP_GetExponent(mXPos - pScreenManager->CamXPos()), 11);
        xy.y = 0;

        wh.x = 640;
        wh.y = 240;
    }

    // Clip objects entering portal?
    auto pClipper1 = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
    if (pClipper1)
    {
        mScreenClipperId1 = pClipper1->field_8_object_id;
        if (mSpriteScale == FP_FromInteger(1))
        {
            pClipper1->field_48_ot_layer = Layer::eLayer_BirdPortal_29;
        }
        else
        {
            pClipper1->field_48_ot_layer = Layer::eLayer_BirdPortal_Half_10;
        }
    }

    // Clip whole screen when "in" the portal?
    auto pClipper2 = relive_new ScreenClipper(PSX_Point{ 0, 0 }, PSX_Point{ 640, 240 }, Layer::eLayer_0);
    if (pClipper2)
    {
        mScreenClipperId2 = pClipper2->field_8_object_id;
        if (mSpriteScale == FP_FromInteger(1))
        {
            pClipper2->field_48_ot_layer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;
        }
        else
        {
            pClipper2->field_48_ot_layer = Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12;
        }
    }

    return 1;
}

void BirdPortal::VKillPortalClipper()
{
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

    mScreenClipperId1 = -1;
    mScreenClipperId2 = -1;

    if (pClipper1)
    {
        pClipper1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        if (pClipper2)
        {
            pClipper2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

void BirdPortal::VGiveShrykull(s16 bPlaySound)
{
    if (sActiveHero)
    {
        if (mPortalType == PortalType::eShrykull_2 && mMudCountForShrykull <= 0)
        {
            SND_SEQ_Play(SeqId::SecretMusic_32, 1, 127, 127);
            mState = PortalStates::ShrykullGetDoves_7;
            mTimer = sGnFrame + 12;
            mReceivedDovesCount = 0;

            mOrbWhirlWind = relive_new OrbWhirlWind(
                sActiveHero->mXPos,
                sActiveHero->mYPos - (sActiveHero->mSpriteScale * FP_FromInteger(38)),
                sActiveHero->mSpriteScale,
                0);

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_112_Chant_45B1C0)
            {
                sActiveHero->ChangeChantState_45BB90(1);
            }
        }
        else
        {
            mState = PortalStates::CollapseTerminators_10;
            if ((mPortalType == PortalType::eWorker_1 || mPortalType == PortalType::eShrykull_2) && sActiveHero->mCurrentMotion == eAbeMotions::Motion_112_Chant_45B1C0)
            {
                sActiveHero->ChangeChantState_45BB90(0);
            }
        }

        if (bPlaySound)
        {
            SFX_Play_Pitch(SoundEffect::MenuNavigation_52, 70, -1600, mSpriteScale);
        }
    }
}

bool BirdPortal::VAbeInsidePortal()
{
    return mState == PortalStates::AbeInsidePortal_16;
}

void BirdPortal::VExitPortal()
{
    mCurrentPath = gMap.mCurrentPath;
    mCurrentLevel = gMap.mCurrentLevel;

    auto pPortalExitTlv = static_cast<Path_BirdPortalExit*>(sPathInfo->TLV_First_Of_Type_In_Camera(TlvTypes::BirdPortalExit_29, 0));
    if (pPortalExitTlv)
    {
        // TODO: Clean up this hack by having a better way to match "any" type of line
        CollisionMask allLinesHack;
        allLinesHack.mMask = 0xFFFFFFFF;

        PathLine* pLine = nullptr;
        sCollisions->Raycast(
            FP_FromInteger(pPortalExitTlv->mTopLeft.x),
            FP_FromInteger(pPortalExitTlv->mTopLeft.y),
            FP_FromInteger(pPortalExitTlv->mBottomRight.x),
            FP_FromInteger(pPortalExitTlv->mBottomRight.y),
            &pLine,
            &mExitX,
            &mExitY,
            allLinesHack);

        mExitX = FP_FromInteger((pPortalExitTlv->mTopLeft.x + pPortalExitTlv->mBottomRight.x) / 2);
        mXPos = mExitX;
        mYPos = mExitY - FP_FromInteger(55);
        mEnterSide = pPortalExitTlv->mExitSide;

        if (pPortalExitTlv->mScale == Scale_short::eHalf_1)
        {
            mSpriteScale = FP_FromDouble(0.5);
            sActiveHero->mAnim.mRenderLayer = Layer::eLayer_InBirdPortal_Half_11;
            sActiveHero->mScale = Scale::Bg;
        }
        else
        {
            mSpriteScale = FP_FromInteger(1);
            sActiveHero->mAnim.mRenderLayer = Layer::eLayer_InBirdPortal_30;
            sActiveHero->mScale = Scale::Fg;
        }

        sActiveHero->mSpriteScale = mSpriteScale;
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
        pTerminator1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pTerminator2 = sObjectIds.Find_Impl(mTerminatorId2);
    if (pTerminator2)
    {
        pTerminator2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    if (pClipper1)
    {
        pClipper1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);
    if (pClipper2)
    {
        pClipper2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                    pDove->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
        }
    }

    BaseGameObject* pThrowableIndicator = sObjectIds.Find_Impl(mThrowableIndicatorId);
    if (pThrowableIndicator)
    {
        pThrowableIndicator->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

s16 BirdPortal::IsScaredAway()
{
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            return FALSE;
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

                if (FP_Abs(pObj->mYPos - mHitY) >= FP_FromInteger(30) || pObj->mSpriteScale != mSpriteScale)
                {
                    continue;
                }
                return TRUE;

            default:
                continue;
        }
    }
    return FALSE;
}

void BirdPortal::CreateDovesAndShrykullNumber()
{
    for (u8 i = 0; i < ALIVE_COUNTOF(mDoveIds); i++)
    {
        auto pDove = relive_new Dove(AnimId::Dove_Flying, mXPos, mYPos, mSpriteScale);
        mDoveIds[i] = pDove->field_8_object_id;

        mDovesExist = 1;

        if (mPortalType == PortalType::eAbe_0)
        {
            pDove->AsAlmostACircle(mXPos, (mSpriteScale * FP_FromInteger(30)) + mYPos, 42 * i);
        }
        else
        {
            pDove->AsACircle(mXPos, (mSpriteScale * FP_FromInteger(30)) + mYPos, 42 * i);
        }
        pDove->mSpriteScale = mSpriteScale;
    }

    if (mPortalType == PortalType::eShrykull_2)
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
            mThrowableIndicatorId = pIndicator->field_8_object_id;
        }
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
            pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void BirdPortal::CreateTerminators()
{
    auto pTerminator1 = relive_new BirdPortalTerminator(mXPos, mYPos, mSpriteScale, mPortalType);
    if (pTerminator1)
    {
        mTerminatorId1 = pTerminator1->field_8_object_id;
    }

    auto pTerminator2 = relive_new BirdPortalTerminator(mXPos, mYPos, mSpriteScale, mPortalType);
    if (pTerminator2)
    {
        mTerminatorId2 = pTerminator2->field_8_object_id;
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

BirdPortalTerminator::BirdPortalTerminator(FP xpos, FP ypos, FP scale, PortalType /*portalType*/)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eEyeOrbPart);

    const AnimRecord& rec = AnimRec(AnimId::BirdPortal_TerminatorGrow);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::BirdPortal_TerminatorGrow, ppRes);

    mAnim.mRenderMode = TPageAbr::eBlend_1;
    mSpriteScale = scale;
    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Above_FG1_Half_20;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

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

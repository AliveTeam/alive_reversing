#include "stdafx_ao.h"
#include "Function.hpp"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Abe.hpp"
#include "ScreenClipper.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Flash.hpp"
#include "Midi.hpp"
#include "Dove.hpp"
#include "OrbWhirlWind.hpp"
#include "Particle.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "AbilityRing.hpp"
#include "Sys_common.hpp"

namespace AO {

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
}

BirdPortalTerminator::BirdPortalTerminator(FP xpos, FP ypos, FP scale, PortalType /*portalType*/)
{
    mBaseGameObjectTypeId = ReliveTypes::eClawOrBirdPortalTerminator;

    const AnimRecord rec = AO::AnimRec(AnimId::BirdPortal_TerminatorGrow);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::BirdPortal_TerminatorGrow, ppRes);
    
    mAnim.mRenderMode = TPageAbr::eBlend_1;
    mSpriteScale = scale;
    if (scale == FP_FromInteger(1))
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
        mTerminator1->mBaseGameObjectFlags.Set(Options::eDead);
    }
    if (mTerminator2)
    {
        mTerminator2->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mScreenClipper1)
    {
        mScreenClipper1->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mScreenClipper2)
    {
        mScreenClipper2->mBaseGameObjectFlags.Set(Options::eDead);
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
            pObj->mBaseGameObjectFlags.Set(Options::eDead);
        }

        mDovesArray->field_4_used_size = 0;

        relive_delete mDovesArray;
    }

    if (mThrowableTotalIndicator)
    {
        mThrowableTotalIndicator->mBaseGameObjectFlags.Set(Options::eDead);
        mThrowableTotalIndicator->mBaseGameObjectRefCount--;
        mThrowableTotalIndicator = nullptr;
    }

    u8** ppRes = nullptr;
    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kPortalTerminatorAOResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kPortliteAOResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kPortlitAOResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDovbasicAOResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

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
                sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
            }
            else
            {
                sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
            }
        }
    }
}

BirdPortal::BirdPortal(Path_BirdPortal* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eBirdPortal;

    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kPortalTerminatorAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDovbasicAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kPortliteAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kPortlitAOResID, 1, 0);

    mTlvInfo = tlvInfo;

    mExitLevel = MapWrapper::FromAO(pTlv->mExitLevel);
    mEnterSide = pTlv->mEnterSide;
    mExitCamera = pTlv->mExitCamera;
    mExitPath = pTlv->mExitPath;
    mPortalType = pTlv->mPortalType;
    mMovieId = pTlv->mMovieId;
    mMudCountForShrykull = pTlv->mMudCountForShrykull;
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

    mTerminator1 = nullptr;
    mTerminator2 = nullptr;
    mScreenClipper1 = nullptr;
    mScreenClipper2 = nullptr;
    mDovesArray = nullptr;
    mOrbWhirlWind = nullptr;
    mThrowableTotalIndicator = nullptr;

    // TODO: Clean up this hack by having a better way to match "any" type of line
    CollisionMask allLinesHack;
    allLinesHack.mMask = 0xFFFFFFFF;

    PathLine* pLine = nullptr;
    FP hitX = {};
    sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeft.x),
        FP_FromInteger(pTlv->mTopLeft.y),
        FP_FromInteger(pTlv->mBottomRight.x),
        FP_FromInteger(pTlv->mBottomRight.y),
        &pLine,
        &hitX,
        &mHitY,
        allLinesHack);

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
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
            if (mPortalType == PortalType::eAbe_0)
            {
                pDove->AsAlmostACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
            }
            else
            {
                pDove->AsACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
            }

            pDove->mSpriteScale = mSpriteScale;
            mDovesArray->Push_Back(pDove);
        }
    }

    if (mPortalType == PortalType::eShrykull_2)
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
            mSfxPlaying = SFX_Play_Camera(SoundEffect::PortalOpening_67, 45, direction);
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
                SfxPlayMono(SoundEffect::Dove_16, 35, 0);
                mTimer = sGnFrame + Math_RandomRange(24, 40);
            }

            auto pTarget = Abe::FindObjectToPossess_421410();
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

                    mDovesArray->field_4_used_size = 0;
                    relive_delete mDovesArray;
                    mDovesArray = nullptr;

                    if (mThrowableTotalIndicator)
                    {
                        mThrowableTotalIndicator->mBaseGameObjectFlags.Set(Options::eDead);
                        mThrowableTotalIndicator->mBaseGameObjectRefCount--;
                        mThrowableTotalIndicator = nullptr;
                    }

                    SfxPlayMono(SoundEffect::Dove_16, 70, 0);
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
            else
            {
                if (mThrowableTotalIndicator)
                {
                    mThrowableTotalIndicator->mBaseGameObjectFlags.Set(Options::eDead);
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

                mTimer = sGnFrame + 15;
                mState = PortalStates::JoinDovesInCenter_2;
                EventBroadcast(kEventPortalOpen, this);
                SfxPlayMono(SoundEffect::Dove_16, 70, 0);
            }
        }
        break;

        case PortalStates::JoinDovesInCenter_2:
            EventBroadcast(kEventPortalOpen, this);
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                CreateTerminators();
                mTimer = sGnFrame + 5;
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
                    pDove->mBaseGameObjectFlags.Set(Options::eDead);
                }

                mDovesArray->field_4_used_size = 0;
                relive_delete mDovesArray;
                mDovesArray = nullptr;
                mState = PortalStates::CreateTerminators_4;
            }
            break;

        case PortalStates::CreateTerminators_4:
            EventBroadcast(kEventPortalOpen, this);
            if (mTerminator1->mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mTerminator1->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                mTerminator2->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                mTimer = sGnFrame + 12;
                mState = PortalStates::ExpandTerminators_5;
                mSfxPlaying = SfxPlayMono(SoundEffect::PortalOpening_67, 0, 0);
                SFX_Play_Pitch(SoundEffect::IngameTransition_107, 115, 300, 0);
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
            if ((mPortalType != PortalType::eWorker_1 && mPortalType != PortalType::eShrykull_2) || EventGet(kEventAbeOhm))
            {
                if ((Math_NextRandom() % 8) == 0)
                {
                    const AnimRecord& rec = AO::AnimRec(AnimId::BirdPortal_Sparks);
                    u8** ppLightRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
                    if (ppLightRes)
                    {
                        auto pParticle = relive_new Particle(
                            mTerminator2->mXPos,
                            (FP_FromInteger(10) * mSpriteScale) + mTerminator2->mYPos,
                            AnimId::BirdPortal_Sparks,
                            ppLightRes);
                        if (pParticle)
                        {
                            pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
                            pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                            pParticle->mBaseGameObjectTypeId = ReliveTypes::eBirdPortalTerminator;
                            pParticle->mSpriteScale = mSpriteScale;

                            if (sGnFrame % 2)
                            {
                                pParticle->mAnim.mFlags.Set(AnimFlags::eBit19_LoopBackwards);
                                pParticle->mAnim.SetFrame(pParticle->mAnim.Get_Frame_Count());
                            }
                        }

                        if (direction == CameraPos::eCamCurrent_0)
                        {
                            SFX_Play_Pitch(SoundEffect::BirdPortalSpark_48, 50, 2400, 0);
                        }
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
                    mTimer = sGnFrame + 30;
                }
            }
            break;

        case PortalStates::ShrykullGetDoves_7:
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
                    xOff = (mSpriteScale * FP_FromInteger(16));
                }
                else
                {
                    xOff = (mSpriteScale * FP_FromInteger(-16));
                }

                const s16 scale32 = FP_GetExponent(mSpriteScale * FP_FromInteger(32));
                if (static_cast<s32>(sGnFrame) >= mTimer)
                {
                    mTimer = sGnFrame + Math_RandomRange(4, 12);

                    auto pDoveMem = relive_new Dove(
                        AnimId::Dove_Flying,
                        mXPos + FP_FromInteger(FP_GetExponent(xOff)),
                        mYPos + FP_FromInteger(Math_RandomRange(-scale32, scale32)),
                        mSpriteScale);
                    if (pDoveMem)
                    {
                        SfxPlayMono(SoundEffect::Dove_16, 70, 0);
                        pDoveMem->mSpriteScale = mSpriteScale;
                        pDoveMem->AsJoin(sActiveHero->mXPos,
                            FP_FromInteger(Math_RandomRange(-36, 4)) + sActiveHero->mYPos);
                    }

                    mReceivedDovesCount++;
                    if (mReceivedDovesCount == 6)
                    {
                        mOrbWhirlWind->ToSpin(
                            sActiveHero->mXPos,
                            sActiveHero->mYPos - (sActiveHero->mSpriteScale * FP_FromInteger(38)),
                            sActiveHero);
                        mOrbWhirlWind = nullptr;
                    }
                }
            }
            break;

        case PortalStates::GetShrykull_9:
            if (static_cast<s32>(sGnFrame) >= mTimer)
            {
                Abe::Get_Shrykull_Resources_42F480();
                mState = PortalStates::CollapseTerminators_10;
                sActiveHero->field_168_ring_pulse_timer = sGnFrame + 32000;
                sActiveHero->field_16C_bHaveShrykull = TRUE;
                if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant_42FD50)
                {
                    sActiveHero->ChangeChantState_430510(0);
                }
            }
            break;

        case PortalStates::CollapseTerminators_10:
            mTerminator1->mYPos += FP_FromDouble(3.5) * mSpriteScale;
            mTerminator2->mYPos -= FP_FromDouble(3.5) * mSpriteScale;

            if (FP_GetExponent(mTerminator1->mYPos) >= FP_GetExponent(mTerminator2->mYPos))
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::BirdPortal_Flash);
                u8** ppLightRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
                if (ppLightRes)
                {
                    auto pParticle_1 = relive_new Particle(
                        mTerminator2->mXPos,
                        mTerminator2->mYPos,
                        AnimId::BirdPortal_Flash,
                        ppLightRes);
                    if (pParticle_1)
                    {
                        pParticle_1->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
                        pParticle_1->mAnim.mRenderMode = TPageAbr::eBlend_1;
                        pParticle_1->mSpriteScale = mSpriteScale;
                    }

                }

                mState = PortalStates::StopSound_11;
                mTimer = sGnFrame + 5;
                mTerminator1->mBaseGameObjectFlags.Set(Options::eDead);
                mTerminator2->mBaseGameObjectFlags.Set(Options::eDead);
                mTerminator1 = nullptr;
                mTerminator2 = nullptr;
                SFX_Play_Pitch(SoundEffect::MenuNavigation_61, 100, -1800, 0);
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
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, 1, TPageAbr::eBlend_0, 1);
            mState = PortalStates::CreateFlash3_14;
        }
        break;

        case PortalStates::CreateFlash3_14:
        {
            relive_new Flash(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, 0, TPageAbr::eBlend_0, 1);
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
            mSfxPlaying = SfxPlayMono(SoundEffect::PortalOpening_67, 0, 0);
        }
        break;

        case PortalStates::PortalExit_CreateTerminators_18:
            if (mTerminator1->mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mTerminator1->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                mTerminator2->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorIdle, nullptr);
                mState = PortalStates::PortalExit_ExpandTerminators_19;
                mTimer = sGnFrame + 12;
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
                mTerminator1->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorShrink, nullptr);
                mTerminator2->mAnim.Set_Animation_Data(AnimId::BirdPortal_TerminatorShrink, nullptr);
                mState = PortalStates::FadeoutTerminators_22;
                mTimer = sGnFrame + 30;

                if (mScreenClipper1)
                {
                    mScreenClipper1->mBaseGameObjectFlags.Set(Options::eDead);
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

s16 BirdPortal::IsScaredAway()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            return FALSE;
        }

        switch (pObj->mBaseGameObjectTypeId)
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

                if (FP_Abs(pObj->mXPos - mXPos) >= FP_FromInteger(75) || FP_Abs(pObj->mYPos - mHitY) >= FP_FromInteger(30) || pObj->mSpriteScale != mSpriteScale)
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

void BirdPortal::VGiveShrykull(s16 bPlaySound)
{
    if (mPortalType == PortalType::eShrykull_2 && mMudCountForShrykull <= 0)
    {
        mState = PortalStates::ShrykullGetDoves_7;
        mTimer = sGnFrame + 12;
        mReceivedDovesCount = 0;

        mOrbWhirlWind = relive_new OrbWhirlWind(
            sActiveHero->mXPos,
            sActiveHero->mYPos - (sActiveHero->mSpriteScale * FP_FromInteger(38)),
            sActiveHero->mSpriteScale);

        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant_42FD50)
        {
            sActiveHero->ChangeChantState_430510(1);
        }
    }
    else
    {
        mState = PortalStates::CollapseTerminators_10;
        if ((mPortalType == PortalType::eWorker_1 || mPortalType == PortalType::eShrykull_2) && sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant_42FD50)
        {
            sActiveHero->ChangeChantState_430510(0);
        }
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectTypeId == ReliveTypes::eBirdPortalTerminator)
        {
            pObj->mBaseGameObjectFlags.Set(Options::eDead);
        }
    }

    if (bPlaySound)
    {
        SFX_Play_Pitch(SoundEffect::MenuNavigation_61, 70, -1600, 0);
    }
}

void BirdPortal::VScreenChanged()
{
    if (mState <= PortalStates::IdlePortal_1 || mState >= PortalStates::KillPortalClipper_21 || ((gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath) && (mState != PortalStates::AbeInsidePortal_16 || mPortalType != PortalType::eAbe_0 || gMap.mNextLevel != mExitLevel || gMap.mNextPath != mExitPath)))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (mTerminator1)
        {
            mTerminator1->mBaseGameObjectFlags.Set(Options::eDead);
            mTerminator1 = nullptr;
        }

        if (mTerminator2)
        {
            mTerminator2->mBaseGameObjectFlags.Set(Options::eDead);
            mTerminator2 = nullptr;
        }

        if (mScreenClipper1)
        {
            mScreenClipper1->mBaseGameObjectFlags.Set(Options::eDead);
            mScreenClipper1 = nullptr;
        }

        if (mScreenClipper2)
        {
            mScreenClipper2->mBaseGameObjectFlags.Set(Options::eDead);
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
        mScreenClipper1->mBaseGameObjectFlags.Set(Options::eDead);
        mScreenClipper1 = nullptr;

        mScreenClipper2->mBaseGameObjectFlags.Set(Options::eDead);
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

    auto pPortalExitTlv = static_cast<Path_BirdPortalExit*>(gMap.TLV_First_Of_Type_In_Camera(TlvTypes::BirdPortalExit_53, 0));

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

        mYPos = mExitY - FP_FromInteger(55);

        mExitX = FP_FromInteger(pPortalExitTlv->mTopLeft.x);
        mXPos = mExitX;
        mEnterSide = pPortalExitTlv->mExitSide;

        if (pPortalExitTlv->mScale == Scale_short::eHalf_1)
        {
            mSpriteScale = FP_FromDouble(0.5);
            sActiveHero->mAnim.mRenderLayer = Layer::eLayer_InBirdPortal_Half_11;
        }
        else
        {
            mSpriteScale = FP_FromInteger(1);
            sActiveHero->mAnim.mRenderLayer = Layer::eLayer_InBirdPortal_30;
        }

        sActiveHero->mSpriteScale = mSpriteScale;
        mState = PortalStates::PortalExit_SetPosition_17;
        sActiveHero->mCurrentLevel = gMap.mCurrentLevel;
        sActiveHero->mCurrentPath = gMap.mCurrentPath;
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
    if (sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone) && sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabaniaDone))
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

    const s16 portalX = static_cast<s16>(PsxToPCX(pScreenManager->mCamXOff + FP_GetExponent(mXPos) - FP_GetExponent(pScreenManager->mCamPos->x), 11));

    PSX_Point xy = {};
    PSX_Point wh = {};
    if (mEnterSide != PortalSide::eRight_0)
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
            mScreenClipper1->field_38_ot_layer = Layer::eLayer_BirdPortal_29;
        }
        else
        {
            mScreenClipper1->field_38_ot_layer = Layer::eLayer_BirdPortal_Half_10;
        }
    }

    mScreenClipper2 = relive_new ScreenClipper(PSX_Point{ 0, 0 }, PSX_Point{ 640, 240 }, Layer::eLayer_0);
    if (mScreenClipper2)
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mScreenClipper2->field_38_ot_layer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;
        }
        else
        {
            mScreenClipper2->field_38_ot_layer = Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12;
        }
    }

    return 1;
}

} // namespace AO

#include "stdafx.h"
#include "CrawlingSlig.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "SnoozeParticle.hpp"
#include "Input.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "ScreenShake.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Slig.hpp"
#include "FlyingSlig.hpp"
#include "CrawlingSligButton.hpp"
#include "Sfx.hpp"
#include "Grid.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Game.hpp"
#include "../relive_lib/FatalError.hpp"
#include "QuikSave.hpp"

const static TintEntry kCrawlingSligTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

const static AnimId sCrawlingSligAnimIdTable[18] = {
    AnimId::CrawlingSlig_Idle,
    AnimId::CrawlingSlig_UsingButton,
    AnimId::CrawlingSlig_WakingUp,
    AnimId::CrawlingSlig_Crawling,
    AnimId::CrawlingSlig_StartFalling,
    AnimId::CrawlingSlig_Falling,
    AnimId::CrawlingSlig_Landing,
    AnimId::CrawlingSlig_ToShakingToIdle,
    AnimId::CrawlingSlig_Speaking,
    AnimId::CrawlingSlig_Snoozing,
    AnimId::CrawlingSlig_PushingWall,
    AnimId::CrawlingSlig_TurnAround,
    AnimId::CrawlingSlig_Shaking,
    AnimId::CrawlingSlig_Empty,
    AnimId::CrawlingSlig_ShakingToIdle,
    AnimId::CrawlingSlig_EndCrawling,
    AnimId::CrawlingSlig_IdleToPushingWall,
    AnimId::CrawlingSlig_EndPushingWall 
};

const static TCrawlingSligMotionFn sCrawlingSligMotions[18] = {
    &CrawlingSlig::Motion_0_Idle,
    &CrawlingSlig::Motion_1_UsingButton,
    &CrawlingSlig::Motion_2_WakingUp,
    &CrawlingSlig::Motion_3_Crawling,
    &CrawlingSlig::Motion_4_StartFalling,
    &CrawlingSlig::Motion_5_Falling,
    &CrawlingSlig::Motion_6_Landing,
    &CrawlingSlig::Motion_7_ToShakingToIdle,
    &CrawlingSlig::Motion_8_Speaking,
    &CrawlingSlig::Motion_9_Snoozing,
    &CrawlingSlig::Motion_10_PushingWall,
    &CrawlingSlig::Motion_11_TurnAround,
    &CrawlingSlig::Motion_12_Shaking,
    &CrawlingSlig::Motion_13_Empty,
    &CrawlingSlig::Motion_14_ShakingToIdle,
    &CrawlingSlig::Motion_15_EndCrawling,
    &CrawlingSlig::Motion_16_IdleToPushingWall,
    &CrawlingSlig::Motion_17_EndPushingWall
};

void CrawlingSlig::LoadAnimations()
{
    for (auto& animId : sCrawlingSligAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

CrawlingSlig::CrawlingSlig(relive::Path_CrawlingSlig* pTlv, const Guid& guid)
    : BaseAliveGameObject(2),
    field_11C_mPal(std::make_shared<AnimationPal>()),
    mGuid(guid),
    mTlv(*pTlv),
    mSleepingBrain(*this),
    mIdleBrain(*this),
    mPanicGetALockerBrain(*this),
    mPossessedBrain(*this),
    mGetKilledBrain(*this),
    mTransformedBrain(*this)
{
    SetType(ReliveTypes::eCrawlingSlig);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::CrawlingSlig_Idle));

    SetTint(&kCrawlingSligTints[0], gMap.mCurrentLevel);

    SetCanBePossessed(true);

    CreateShadow();

    if (mTlv.mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        SetScale(Scale::Bg);
    }
    else if (mTlv.mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        SetScale(Scale::Fg);
    }

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (mTlv.mStartState == relive::Path_CrawlingSlig::StartState::eAwake)
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, true);
        SetBrain(ICrawlingSligBrain::EBrainTypes::Idle);
    }
    else
    {
        if (GetSpriteScale() == FP_FromInteger(1))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        }
        Set_AnimAndMotion(CrawlingSligMotion::Motion_9_Snoozing, true);
        SetBrain(ICrawlingSligBrain::EBrainTypes::Sleeping);
    }

    if (mTlv.mCrawlDirection == relive::Path_CrawlingSlig::CrawlDirection::eRandom)
    {
        mCrawlDirection = NextRandom() ? relive::Path_CrawlingSlig::CrawlDirection::eRight : relive::Path_CrawlingSlig::CrawlDirection::eLeft;
    }
    else
    {
        mCrawlDirection = mTlv.mCrawlDirection;
    }

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(37),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }
}

void CrawlingSlig::VRender(PrimHeader** ot)
{
    const s16 eyeIndices[] = {60, 62};
    renderWithGlowingEyes(ot, this, field_11C_mPal, 64, field_1A4_r, field_1A6_g, field_1A8_b, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

void CrawlingSlig::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<CrawlingSligSaveState>();

    auto pTlv = static_cast<relive::Path_CrawlingSlig*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mCrawlingSligTlvId));

    auto pCrawlingSlig = relive_new CrawlingSlig(pTlv, pState->mCrawlingSligTlvId);
    if (pCrawlingSlig)
    {
        pCrawlingSlig->mBaseGameObjectTlvInfo = pState->mBaseTlvId;

        if (pState->mControlled)
        {
            sControlledCharacter = pCrawlingSlig;
        }

        pCrawlingSlig->BaseAliveGameObjectPathTLV = nullptr;
        pCrawlingSlig->BaseAliveGameObjectCollisionLine = nullptr;

        pCrawlingSlig->mXPos = pState->mXPos;
        pCrawlingSlig->mYPos = pState->mYPos;
        pCrawlingSlig->mVelX = pState->mVelX;
        pCrawlingSlig->mVelY = pState->mVelY;

        pCrawlingSlig->mVelxScaleFactor = pState->mVelxScaleFactor;

        pCrawlingSlig->mCurrentPath = pState->mCurrentPath;
        pCrawlingSlig->mCurrentLevel = pState->mCurrentLevel;
        pCrawlingSlig->SetSpriteScale(pState->mSpriteScale);

        pCrawlingSlig->field_1A4_r = pState->mR;
        pCrawlingSlig->field_1A6_g = pState->mG;
        pCrawlingSlig->field_1A8_b = pState->mB;

        pCrawlingSlig->mRGB.SetRGB(pState->mR, pState->mG, pState->mB);

        pCrawlingSlig->SetCurrentMotion(pState->mCurrentMotion);

        s16 idx = static_cast<s16>(pState->mCurrentMotion);
        pCrawlingSlig->GetAnimation().Set_Animation_Data(pCrawlingSlig->GetAnimRes(sCrawlingSligAnimIdTable[idx]));
        pCrawlingSlig->GetAnimation().SetCurrentFrame(pState->mCurrentFrame);

        pCrawlingSlig->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

        pCrawlingSlig->GetAnimation().SetFlipX(pState->mFlipX & 1);
        pCrawlingSlig->GetAnimation().SetRender(pState->mRender & 1);

        pCrawlingSlig->SetDrawable(pState->mDrawable & 1);

        if (IsLastFrame(&pCrawlingSlig->GetAnimation()))
        {
            pCrawlingSlig->GetAnimation().SetIsLastFrame(true);
        }

        pCrawlingSlig->mHealth = pState->mHealth;
        pCrawlingSlig->SetCurrentMotion(pState->mCurrentMotion2);
        pCrawlingSlig->SetNextMotion(pState->mNextMotion);
        pCrawlingSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->mLastLineYPos);
        pCrawlingSlig->SetRestoredFromQuickSave(true);
        pCrawlingSlig->mMultiUseTimer = pState->mMultiUseTimer;
        pCrawlingSlig->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
        pCrawlingSlig->mGuid = pState->mCrawlingSligTlvId;
        pCrawlingSlig->SetBrain(pState->mBrainType);
        pCrawlingSlig->mSleepingBrain.SetState(pState->mSleepingBrainState);
        pCrawlingSlig->mPanicGetALockerBrain.SetState(pState->mPanicGetALockerBrainState);
        pCrawlingSlig->mPossessedBrain.SetState(pState->mPossessedBrainState);
        pCrawlingSlig->mGetKilledBrain.SetState(pState->mGetKilledBrainState);
        pCrawlingSlig->mChanting = pState->mChanting;
        pCrawlingSlig->mAbeLevel = pState->mAbeLevel;
        pCrawlingSlig->mAbePath = pState->mAbePath;
        pCrawlingSlig->mAbeCamera = pState->mAbeCamera;
        pCrawlingSlig->mSligButtonId = pState->mSligButtonTlvId;
        pCrawlingSlig->field_1D4_obj_id = pState->field_70_obj_id;
        pCrawlingSlig->mTransformedSligId = pState->mTransformedSligId;
        pCrawlingSlig->mSpeak = pState->mSpeak;
        pCrawlingSlig->mSayHelpTimer = pState->mSayHelpTimer;
    }
}

void CrawlingSlig::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    CrawlingSligSaveState data = {};

    data.mType = ReliveTypes::eCrawlingSlig;
    data.mBaseTlvId = mBaseGameObjectTlvInfo;

    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mVelY = mVelY;

    data.mVelxScaleFactor = mVelxScaleFactor;

    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;
    data.mSpriteScale = GetSpriteScale();

    data.mR = mRGB.r;
    data.mG = mRGB.g;
    data.mB = mRGB.b;
    data.mFlipX = GetAnimation().GetFlipX();
    data.mCurrentMotion = GetCurrentMotion();
    data.mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    data.mDrawable = GetDrawable();
    data.mRender = GetAnimation().GetRender();
    data.mHealth = mHealth;
    data.mCurrentMotion2 = GetCurrentMotion();
    data.mNextMotion = GetNextMotion();
    data.mCollisionLineType = eLineTypes::eNone_m1;

    // TODO: Check correct
    data.mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        data.mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }

    data.mControlled = (this == sControlledCharacter);
    data.mMultiUseTimer = mMultiUseTimer;
    data.mCrawlingSligTlvId = mGuid;
   // data.mBrainType = ICrawlingSligBrain::EBrainTypes::Sleeping;

    data.mBrainType = mCurrentBrain->VGetBrain();

    data.mSleepingBrainState = mSleepingBrain.State();
    data.mPanicGetALockerBrainState = mPanicGetALockerBrain.State();
    data.mPossessedBrainState = mPossessedBrain.State();
    data.mGetKilledBrainState = mGetKilledBrain.State();

    data.mChanting = mChanting;
    data.mAbeLevel = mAbeLevel;
    data.mAbePath = mAbePath;
    data.mAbeCamera = mAbeCamera;
    data.mSligButtonTlvId = mSligButtonId;
    data.field_70_obj_id = field_1D4_obj_id;
    data.mTransformedSligId = mTransformedSligId;
    data.mSpeak = mSpeak;
    data.mSayHelpTimer = mSayHelpTimer;
    
    pSaveBuffer.Write(data);
}

void CrawlingSlig::VPossessed()
{
    SetPossessed(true);
    mChanting = true;
    Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, true);
    SetBrain(ICrawlingSligBrain::EBrainTypes::Possessed);
    mPossessedBrain.SetState(PossessedBrain::EState::eStartPossession);
    mMultiUseTimer = sGnFrame + 35;
    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;
}

void CrawlingSlig::Set_AnimAndMotion(CrawlingSligMotion currentMotion, s16 bClearNextMotion)
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sCrawlingSligAnimIdTable[static_cast<s16>(currentMotion)]));
    mCurrentMotion = static_cast<s16>(currentMotion);

    UpdateAnimBlock();

    if (bClearNextMotion)
    {
        mNextMotion = -1;
    }
}

void CrawlingSlig::UpdateAnimBlock()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sCrawlingSligAnimIdTable[mCurrentMotion]));
}

u8** CrawlingSlig::GetAnimBlock(s32 /*currentMotion*/)
{
    return mBaseGameObjectResArray.ItemAt(0);
}

s16 CrawlingSlig::NextRandom()
{
    return (Math_NextRandom() & 1) == 0;
}

void CrawlingSlig::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        if (GetRestoredFromQuickSave())
        {
            SetRestoredFromQuickSave(false);
            if (BaseAliveGameObjectCollisionLineType == -1)
            {
                BaseAliveGameObjectCollisionLine = 0;
            }
            else
            {
                gCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(20),
                    mXPos,
                    mYPos + FP_FromInteger(20),
                    &BaseAliveGameObjectCollisionLine,
                    &mXPos,
                    &mYPos,
                    CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
            }
            BaseAliveGameObjectCollisionLineType = 0;
            field_1D4_obj_id = RefreshId(field_1D4_obj_id);
            mTransformedSligId = RefreshId(mTransformedSligId);
            mSligButtonId = RefreshId(mSligButtonId);
        }

        if (!Input_IsChanting())
        {
            mChanting = false;
        }

        //mBrainSubState = (this->*mBrainState)();
        mCurrentBrain->VUpdate();

        const FP oldX = mXPos;
        const FP oldY = mYPos;

        InvokeMemberFunction(this, sCrawlingSligMotions,mCurrentMotion);

        if (oldX != mXPos || oldY != mYPos)
        {
            auto pTlv = gPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);

            VOnTlvCollision(pTlv);
        }
    }
}

s16 CrawlingSlig::HandleEnemyStopper(FP /*velX*/)
{
    FP gridSizeDirected = ScaleToGridSize(GetSpriteScale());
    relive::Path_EnemyStopper::StopDirection direction = relive::Path_EnemyStopper::StopDirection::Both;
    if (GetAnimation().GetFlipX())
    {
        direction = relive::Path_EnemyStopper::StopDirection::Left;
        gridSizeDirected = -gridSizeDirected;
    }
    else
    {
        direction = relive::Path_EnemyStopper::StopDirection::Right;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), gridSizeDirected * FP_FromDouble(1.5)))
    {
        return 1;
    }

    const FP gridSize = ScaleToGridSize(GetSpriteScale());
    auto pSlamDoor = static_cast<relive::Path_SlamDoor*>(gPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSizeDirected),
        FP_GetExponent(mYPos - gridSize),
        ReliveTypes::eSlamDoor));
    BaseAliveGameObjectPathTLV = pSlamDoor;

    if (pSlamDoor && ((pSlamDoor->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoor->mSwitchId)) || (pSlamDoor->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoor->mSwitchId))))
    {
        return 1;
    }

    auto pStopper = static_cast<relive::Path_EnemyStopper*>(gPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSizeDirected),
        FP_GetExponent(mYPos - gridSize),
        ReliveTypes::eEnemyStopper));
    BaseAliveGameObjectPathTLV = pStopper;

    return pStopper && (pStopper->mStopDirection == direction || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && SwitchStates_Get(pStopper->mSwitchId);
}

relive::Path_TLV* CrawlingSlig::FindPantsOrWings()
{
    relive::Path_TLV* pTlvIter = gPathInfo->TlvGetAt(nullptr, mXPos, mYPos, mXPos, mYPos);
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == ReliveTypes::eSligGetPants || pTlvIter->mTlvType == ReliveTypes::eSligGetWings)
        {
            return pTlvIter;
        }
        pTlvIter = gPathInfo->TlvGetAt(pTlvIter, mXPos, mYPos, mXPos, mYPos);
    }
    return nullptr;
}

BaseGameObject* CrawlingSlig::FindSligButton()
{
    return FindObjectOfType(ReliveTypes::eSligButton, mXPos, mYPos - (FP_FromInteger(30) * GetSpriteScale()));
}

void CrawlingSlig::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        Set_AnimAndMotion(CrawlingSligMotion::Motion_4_StartFalling, true);
    }
}

void CrawlingSlig::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                mGetKilledBrain.SetState(GetKilledBrain::EState::eDeathDrop);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, 0, this);
                mMultiUseTimer = sGnFrame + 60;
            }
        }

        pTlv = gPathInfo->TlvGetAt(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

bool CrawlingSlig::VTakeDamage(BaseGameObject* pFrom)
{
    if (!BrainIs(ICrawlingSligBrain::EBrainTypes::Transformed))
    {
        switch (pFrom->Type())
        {
            case ReliveTypes::eBullet:
            case ReliveTypes::eDrill:
            case ReliveTypes::eGroundExplosion:
            case ReliveTypes::eRockSpawner:
            case ReliveTypes::eMeatSaw:
            case ReliveTypes::eMineCar:
            case ReliveTypes::eAirExplosion:
                if (!BrainIs(ICrawlingSligBrain::EBrainTypes::GetKilled))
                {
                    SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                    mGetKilledBrain.SetState(GetKilledBrain::EState::eGibsDeath);
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return true;

            case ReliveTypes::eElectricWall:
                Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, 0, this);
                return true;

            case ReliveTypes::eSlig:
            {
                // Take a BEATING
                mMultiUseTimer = sGnFrame + 20;
                mHealth -= FP_FromDouble(0.13);

                if (mHealth <= FP_FromInteger(0))
                {
                    SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                    mGetKilledBrain.SetState(GetKilledBrain::EState::eGibsDeath);
                    return false;
                }

                Set_AnimAndMotion(CrawlingSligMotion::Motion_7_ToShakingToIdle, true);
                Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, 0, this);

                if (BrainIs(ICrawlingSligBrain::EBrainTypes::PanicGetALocker))
                {
                    mPanicGetALockerBrain.SetState(PanicGetALockerBrain::EState::eBeatBySlig);
                }
                else if (BrainIs(ICrawlingSligBrain::EBrainTypes::Possessed))
                {
                    mPossessedBrain.SetState(PossessedBrain::EState::eBeatBySlig);
                }
                else
                {
                    SetBrain(ICrawlingSligBrain::EBrainTypes::PanicGetALocker);
                    mPanicGetALockerBrain.SetState(PanicGetALockerBrain::EState::eBeatBySlig);
                }
            }
                return false;

            case ReliveTypes::eSlog:
                if (!BrainIs(ICrawlingSligBrain::EBrainTypes::GetKilled))
                {
                    SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                    mGetKilledBrain.SetState(GetKilledBrain::EState::eDeathBySlog);
                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);
                    mHealth = FP_FromInteger(0);
                    MapFollowMe(true);
                    mMultiUseTimer = sGnFrame + 15;
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_13_Empty, true);
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return false;

            case ReliveTypes::eElectrocute:
                if (!BrainIs(ICrawlingSligBrain::EBrainTypes::GetKilled))
                {
                    GetAnimation().SetRender(false);
                    mHealth = FP_FromInteger(0);
                    mMultiUseTimer = sGnFrame + 1;
                    SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                    mGetKilledBrain.SetState(GetKilledBrain::EState::eSetDead);
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return true;

            default:
                return true;
        }
    }
    return true;
}

void CrawlingSlig::SetBrain(ICrawlingSligBrain::EBrainTypes brain)
{
    switch (brain)
    {
        case ICrawlingSligBrain::EBrainTypes::Sleeping:
            mCurrentBrain = &mSleepingBrain;
            break;
        case ICrawlingSligBrain::EBrainTypes::Idle:
            mCurrentBrain = &mIdleBrain;
            break;
        case ICrawlingSligBrain::EBrainTypes::PanicGetALocker:
            mCurrentBrain = &mPanicGetALockerBrain;
            break;
        case ICrawlingSligBrain::EBrainTypes::Possessed:
            mCurrentBrain = &mPossessedBrain;
            break;
        case ICrawlingSligBrain::EBrainTypes::GetKilled:
            mCurrentBrain = &mGetKilledBrain;
            break;
        case ICrawlingSligBrain::EBrainTypes::Transformed:
            mCurrentBrain = &mTransformedBrain;
            break;
        default:
            ALIVE_FATAL("Invalid crawling slig brain type %d", static_cast<s32>(brain));
    }
}

bool CrawlingSlig::BrainIs(ICrawlingSligBrain::EBrainTypes brain)
{
    return mCurrentBrain->VGetBrain() == brain;
}

CrawlingSlig::~CrawlingSlig()
{
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        if (gMap.mNextLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }
    if (mHealth > FP_FromInteger(0) || mTlv.mRespawnOnDeath == relive::reliveChoice::eYes)
    {
        Path::TLV_Reset(mGuid, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mGuid, -1, 0, 1);
    }
}

bool CrawlingSlig::PanicOn()
{
    return SwitchStates_Get(mTlv.mPanicSwitchId) != 0;
}

void CrawlingSlig::ToIdle()
{
    mVelxScaleFactor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, false);
    MapFollowMe(true);
}

void SleepingBrain::VUpdate()
{
    if (gMap.GetDirection(
            mCrawlingSlig.mCurrentLevel,
            mCrawlingSlig.mCurrentPath,
            mCrawlingSlig.mXPos,
            mCrawlingSlig.mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mCrawlingSlig, 0, 0);
    }

    if (mBrainState == EState::eSleeping)
    {
        if (mCrawlingSlig.PanicOn())
        {
            mCrawlingSlig.mMultiUseTimer = sGnFrame + Math_RandomRange(15, 45);
            mBrainState = EState::eWakingUp;
        }
        return;
    }

    if (mBrainState == EState::eWakingUp)
    {
        if (static_cast<s32>(sGnFrame) <= mCrawlingSlig.mMultiUseTimer)
        {
            return;
        }

        mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_2_WakingUp);
        mBrainState = eIsAwake;
        return;
    }

    if (mBrainState != EState::eIsAwake ||
        mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_2_WakingUp ||
        !(mCrawlingSlig.GetAnimation().GetIsLastFrame()))
    {
        return;
    }

    if (mCrawlingSlig.GetSpriteScale() == FP_FromInteger(1))
    {
        mCrawlingSlig.GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }
    else
    {
        mCrawlingSlig.GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }

    mCrawlingSlig.ToIdle();
    mCrawlingSlig.SetBrain(ICrawlingSligBrain::EBrainTypes::PanicGetALocker);
    mCrawlingSlig.mPanicGetALockerBrain.SetState(PanicGetALockerBrain::eDetermineCrawlDirection);
}

void IdleBrain::VUpdate()
{
    if (gMap.GetDirection(
            mCrawlingSlig.mCurrentLevel,
            mCrawlingSlig.mCurrentPath,
            mCrawlingSlig.mXPos,
            mCrawlingSlig.mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, &mCrawlingSlig, 0, 0);
    }

    if (mCrawlingSlig.PanicOn())
    {
        mCrawlingSlig.SetBrain(EBrainTypes::PanicGetALocker);
        mCrawlingSlig.mPanicGetALockerBrain.SetState(PanicGetALockerBrain::eDetermineCrawlDirection);
    }
}

void PanicGetALockerBrain::VUpdate()
{
    if (gMap.GetDirection(
            mCrawlingSlig.mCurrentLevel,
            mCrawlingSlig.mCurrentPath,
            mCrawlingSlig.mXPos,
            mCrawlingSlig.mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, &mCrawlingSlig, 0, 0);
    }

    if (!mCrawlingSlig.BaseAliveGameObjectCollisionLine && mBrainState != EState::eFalling)
    {
        mBrainState = EState::eFalling;
    }

    switch (mBrainState)
    {
        case EState::eDetermineCrawlDirection:
            if ((mCrawlingSlig.mCrawlDirection != relive::Path_CrawlingSlig::CrawlDirection::eRight || !(mCrawlingSlig.GetAnimation().GetFlipX())) && (mCrawlingSlig.mCrawlDirection != relive::Path_CrawlingSlig::CrawlDirection::eLeft || mCrawlingSlig.GetAnimation().GetFlipX()))
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                mBrainState = EState::eSearchLocker;
                return;
            }
            mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            mBrainState = EState::eTurnAroundForLocker;
            return;

        case EState::eFalling:
            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle)
            {
                return;
            }
            mBrainState = EState::eDetermineCrawlDirection;
            return;

        case EState::eSearchLocker:
            if (mCrawlingSlig.HandleEnemyStopper(mCrawlingSlig.mVelX))
            {
                mCrawlingSlig.mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                mBrainState = EState::eTurnAround;
                return;
            }

            mCrawlingSlig.mTlvHeader = mCrawlingSlig.FindPantsOrWings();
            if (mCrawlingSlig.mTlvHeader)
            {
                mCrawlingSlig.ToIdle();
                mBrainState = EState::eGetPantsOrWings;
                return;
            }
            else
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mCrawlingSlig.mSayHelpTimer)
                {
                    return;
                }
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
                mCrawlingSlig.mSayHelpTimer = sGnFrame + 60;
                mCrawlingSlig.mSpeak = SligSpeak::eHelp_10;
                mBrainState = EState::eSearchLockerOrTurnAround;
                return;
            }
            break;

        case EState::eTurnAroundForLocker:
            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_11_TurnAround || !(mCrawlingSlig.GetAnimation().GetIsLastFrame()))
            {
                return;
            }
            mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            mBrainState = EState::eSearchLocker;
            return;

        case EState::eSearchLockerOrTurnAround:
        {
            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_8_Speaking || !(mCrawlingSlig.GetAnimation().GetIsLastFrame()))
            {
                return;
            }

            FP gridScale = {};
            if (mCrawlingSlig.GetAnimation().GetFlipX())
            {
                gridScale = -ScaleToGridSize(mCrawlingSlig.GetSpriteScale());
            }
            else
            {
                gridScale = ScaleToGridSize(mCrawlingSlig.GetSpriteScale());
            }

            if (!mCrawlingSlig.WallHit(FP_FromInteger(35), gridScale))
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                mBrainState = EState::eSearchLocker;
                return;
            }
            mCrawlingSlig.mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
            mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            mBrainState = EState::eTurnAround;
            return;
        }

        case EState::eGetPantsOrWings:
            if (!mCrawlingSlig.mTlvHeader)
            {
                mCrawlingSlig.mTlvHeader = mCrawlingSlig.FindPantsOrWings();
            }

            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle || (mCrawlingSlig.mTlvHeader && mCrawlingSlig.mTlvHeader->mTlvSpecificMeaning))
            {
                if (Math_NextRandom() & 1)
                {
                    mCrawlingSlig.mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    mBrainState = EState::eCrawling;
                    return;
                }
                else
                {
                    mCrawlingSlig.mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                    mBrainState = EState::eTurnAround;
                    return;
                }
            }
            else
            {
                mCrawlingSlig.mTlvHeader->mTlvSpecificMeaning &= 0xFF;
                mCrawlingSlig.mTlvHeader->mTlvSpecificMeaning |= 1;

                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                mCrawlingSlig.mMultiUseTimer = sGnFrame + 20;
                mBrainState = EState::eUsingButton;
                return;
            }
            break;

        case EState::eUsingButton:
            if (mCrawlingSlig.mTlvHeader)
            {
                return;
            }
            mCrawlingSlig.mTlvHeader = mCrawlingSlig.FindPantsOrWings();
            return;

        case EState::eTurnAround:
            if (mCrawlingSlig.GetCurrentMotion() == CrawlingSligMotion::Motion_11_TurnAround && mCrawlingSlig.GetAnimation().GetIsLastFrame())
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                mBrainState = EState::eCrawling;
                return;
            }
            return;

        case EState::eCrawling:
            if (mCrawlingSlig.HandleEnemyStopper(mCrawlingSlig.mVelX))
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                mBrainState = EState::eTurnAroundForLocker;
                return;
            }

            if (static_cast<s32>(sGnFrame) > mCrawlingSlig.mMultiUseTimer)
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                mBrainState = EState::eTurnAroundForLocker;
                return;
            }

            if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mCrawlingSlig.mSayHelpTimer)
            {
                return;
            }

            mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
            mCrawlingSlig.mSayHelpTimer = sGnFrame + 60;
            mCrawlingSlig.mSpeak = SligSpeak::eHelp_10;
            mBrainState = EState::eCheckIfWallHit;
            return;

        case EState::eCheckIfWallHit:
        {
            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_8_Speaking || !(mCrawlingSlig.GetAnimation().GetIsLastFrame()))
            {
                return;
            }

            FP gridScale = {};
            if (mCrawlingSlig.GetAnimation().GetFlipX())
            {
                gridScale = -ScaleToGridSize(mCrawlingSlig.GetSpriteScale());
            }
            else
            {
                gridScale = ScaleToGridSize(mCrawlingSlig.GetSpriteScale());
            }

            if (mCrawlingSlig.WallHit(FP_FromInteger(35), gridScale))
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                mBrainState = EState::eTurnAroundForLocker;
                return;
            }
            else
            {
                mCrawlingSlig.SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                mBrainState = EState::eCrawling;
                return;
            }
        }
        break;

        case EState::eBeatBySlig:
            if (mCrawlingSlig.GetCurrentMotion() == CrawlingSligMotion::Motion_14_ShakingToIdle)
            {
                if (mCrawlingSlig.GetAnimation().GetIsLastFrame())
                {
                    mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, true);
                    mBrainState = EState::eDetermineCrawlDirection;
                    return;
                }
            }
            else if (static_cast<s32>(sGnFrame) > mCrawlingSlig.mMultiUseTimer)
            {
                mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
            }
            return;

        default:
            return;
    }
}

void PossessedBrain::VUpdate()
{
    if (gMap.GetDirection(
            mCrawlingSlig.mCurrentLevel,
            mCrawlingSlig.mCurrentPath,
            mCrawlingSlig.mXPos,
            mCrawlingSlig.mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, &mCrawlingSlig, 0, 0);
    }

    switch (mBrainState)
    {
        case EState::eStartPossession:
            if (static_cast<s32>(sGnFrame) <= mCrawlingSlig.mMultiUseTimer)
            {
                return;
            }
            mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
            mBrainState = EState::ePossessed;
            return;

        case EState::ePossessed:
            if (!Input_IsChanting() || mCrawlingSlig.mChanting)
            {
                return;
            }
            mCrawlingSlig.mMultiUseTimer = sGnFrame + 30;
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, true);
            mBrainState = EState::eUnpossessing;
            return;

        case EState::eUnpossessing:
            if (Input_IsChanting())
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    const FP y = mCrawlingSlig.mYPos - (mCrawlingSlig.GetSpriteScale() * FP_FromInteger(Math_RandomRange(0, 30)));
                    const FP x = mCrawlingSlig.mXPos + (mCrawlingSlig.GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20)));
                    New_TintChant_Particle(
                        x, y,
                        mCrawlingSlig.GetSpriteScale(),
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) <= mCrawlingSlig.mMultiUseTimer && sActiveHero->mHealth > FP_FromInteger(0))
                {
                    return;
                }

                sControlledCharacter = sActiveHero;
                mCrawlingSlig.SetPossessed(false);
                gMap.SetActiveCam(mCrawlingSlig.mAbeLevel, mCrawlingSlig.mAbePath, mCrawlingSlig.mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
                mCrawlingSlig.SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                mCrawlingSlig.mGetKilledBrain.SetState(GetKilledBrain::eGibsDeath);
                MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mCrawlingSlig, 0, 0);
                return;
            }
            else
            {
                mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
                mBrainState = EState::ePossessed;
                return;
            }
            break;

        case EState::eBeatBySlig:
            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_14_ShakingToIdle)
            {
                if (static_cast<s32>(sGnFrame) > mCrawlingSlig.mMultiUseTimer)
                {
                    mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
                }
                return;
            }

            if (!mCrawlingSlig.GetAnimation().GetIsLastFrame())
            {
                return;
            }

            mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, 1);
            mBrainState = EState::ePossessed;
            return;

        default:
            break;
    }
}

void GetKilledBrain::VUpdate()
{
    if (gMap.GetDirection(
            mCrawlingSlig.mCurrentLevel,
            mCrawlingSlig.mCurrentPath,
            mCrawlingSlig.mXPos,
            mCrawlingSlig.mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mCrawlingSlig, 0, 0);
    }

    switch (mBrainState)
    {
        case EState::eUnknown:
            if (mCrawlingSlig.GetCurrentMotion() != CrawlingSligMotion::Motion_7_ToShakingToIdle || !mCrawlingSlig.GetAnimation().GetIsLastFrame())
            {
                return;
            }
            mBrainState = EState::eVaporize;
            return;

        case EState::eVaporize:
            if (mCrawlingSlig.mMultiUseTimer < static_cast<s32>((sGnFrame + 80)))
            {
                mCrawlingSlig.SetSpriteScale(mCrawlingSlig.GetSpriteScale() - FP_FromDouble(0.008));
                mCrawlingSlig.mRGB.r -= 2;
                mCrawlingSlig.mRGB.g -= 2;
                mCrawlingSlig.mRGB.b -= 2;
            }

            if (static_cast<s32>(sGnFrame) < mCrawlingSlig.mMultiUseTimer - 24)
            {
                mCrawlingSlig.DeathSmokeEffect(true);
            }

            if (mCrawlingSlig.mMultiUseTimer < static_cast<s32>(sGnFrame))
            {
                mCrawlingSlig.SetDead(true);
            }
            return;

        case EState::eGibsDeath:
        {
            relive_new Gibs(
                GibType::Slig_1,
                mCrawlingSlig.mXPos,
                mCrawlingSlig.mYPos,
                mCrawlingSlig.mVelX,
                mCrawlingSlig.mVelY,
                mCrawlingSlig.GetSpriteScale(),
                0);

            relive_new Blood(
                mCrawlingSlig.mXPos,
                mCrawlingSlig.mYPos - (FP_FromInteger(30) * mCrawlingSlig.GetSpriteScale()),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mCrawlingSlig.GetSpriteScale(),
                20);

            New_Smoke_Particles(
                mCrawlingSlig.mXPos,
                mCrawlingSlig.mYPos - (FP_FromInteger(30) * mCrawlingSlig.GetSpriteScale()),
                mCrawlingSlig.GetSpriteScale(),
                3,
                RGB16{128, 128, 128});

            SfxPlayMono(relive::SoundEffects::KillEffect, 128, mCrawlingSlig.GetSpriteScale());
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90, mCrawlingSlig.GetSpriteScale());

            mCrawlingSlig.GetAnimation().SetRender(false);
            mCrawlingSlig.GetAnimation().SetAnimate(false);

            mCrawlingSlig.Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, true);
            mCrawlingSlig.mVelY = FP_FromInteger(0);
            mCrawlingSlig.mVelX = FP_FromInteger(0);
            mCrawlingSlig.mHealth = FP_FromInteger(0);
            mCrawlingSlig.mMultiUseTimer = sGnFrame + 40;
            mBrainState = EState::eSetDead;
            return;
        }

        case EState::eSetDead:
            if (static_cast<s32>(sGnFrame) > mCrawlingSlig.mMultiUseTimer)
            {
                mCrawlingSlig.SetDead(true);
            }
            return;

        case EState::eDeathBySlog:
            if (static_cast<s32>(sGnFrame) <= mCrawlingSlig.mMultiUseTimer)
            {
                return;
            }
            mCrawlingSlig.mMultiUseTimer = sGnFrame + 90;
            mBrainState = EState::eVaporize;
            return;

        case EState::eDeathDrop:
            if (static_cast<s32>(sGnFrame) < mCrawlingSlig.mMultiUseTimer)
            {
                if (!(static_cast<s32>(mCrawlingSlig.mMultiUseTimer - sGnFrame) % 15))
                {
                    Slig_GameSpeak_SFX(
                        SligSpeak::eHelp_10,
                        // TODO: revisit the logic below
                        static_cast<s16>(2 * (mCrawlingSlig.mMultiUseTimer & (0xFFFF - sGnFrame))),
                        0,
                        &mCrawlingSlig);
                }

                if (static_cast<s32>(sGnFrame) == mCrawlingSlig.mMultiUseTimer - 6)
                {
                    SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
                }
                return;
            }
            else
            {
                Environment_SFX(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, &mCrawlingSlig);
                relive_new ScreenShake(0, 0);
                mCrawlingSlig.mMultiUseTimer = sGnFrame + 30;
                mBrainState = EState::eSetDead;
                return;
            }
            break;

        default:
            return;
    }
}

void TransformedBrain::VUpdate()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(mCrawlingSlig.mTransformedSligId);
    if (gMap.GetDirection(
            mCrawlingSlig.mCurrentLevel,
            mCrawlingSlig.mCurrentPath,
            mCrawlingSlig.mXPos,
            mCrawlingSlig.mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mCrawlingSlig, 0, 0);
    }

    if (!pObj || pObj->GetDead())
    {
        mCrawlingSlig.SetDead(true);
    }
}

void CrawlingSlig::Motion_0_Idle()
{
    HandleCommon();
}

void CrawlingSlig::Motion_1_UsingButton()
{
    // Check for using a slig button which is a button than can trigger an id
    auto pSligButton = static_cast<CrawlingSligButton*>(sObjectIds.Find_Impl(mSligButtonId));
    if (pSligButton && GetAnimation().GetCurrentFrame() == 8)
    {
        pSligButton->UseButton();
        mSligButtonId = Guid{};
    }
    // If not using a button check if we are on a locker to get pants or wings
    else if (mTlvHeader)
    {
        if (GetAnimation().GetCurrentFrame() == 7)
        {
            SfxPlayMono(relive::SoundEffects::CrawlingSligTransformStart, 0);
        }

        if (static_cast<s32>(sGnFrame) == mMultiUseTimer - 1)
        {
            New_DestroyOrCreateObject_Particle(
                mXPos,
                (GetSpriteScale() * FP_FromInteger(45)) + mYPos,
                GetSpriteScale());
        }
        else if (static_cast<s32>(sGnFrame) > mMultiUseTimer)
        {
            if (mTlvHeader->mTlvType == ReliveTypes::eSligGetPants)
            {
                // Transform to a walking slig

                SfxPlayMono(relive::SoundEffects::SligSpawn, 0);

                auto pWalkingSlig = relive_new Slig(static_cast<relive::Path_Slig*>(mTlvHeader), gPathInfo->TLVInfo_From_TLVPtr(mTlvHeader));
                if (pWalkingSlig)
                {
                    mTransformedSligId = pWalkingSlig->mBaseGameObjectId;

                    pWalkingSlig->SetSpriteScale(GetSpriteScale());

                    pWalkingSlig->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

                    if (BrainIs(ICrawlingSligBrain::EBrainTypes::Possessed))
                    {
                        pWalkingSlig->SetPossessed(true);
                        pWalkingSlig->mAbeLevel = mAbeLevel;
                        pWalkingSlig->mAbePath = mAbePath;
                        pWalkingSlig->mAbeCamera = mAbeCamera;
                        pWalkingSlig->SetBrain(&Slig::Brain_2_Possessed);
                        pWalkingSlig->mBrainSubState = 4;
                        sControlledCharacter = pWalkingSlig;
                    }
                }

                mHealth = FP_FromInteger(0);
            }
            else if (mTlvHeader->mTlvType == ReliveTypes::eSligGetWings)
            {
                // Transform to a flying slig

                SfxPlayMono(relive::SoundEffects::FlyingSligSpawn, 0);

                auto pFlyingSlig = relive_new FlyingSlig(static_cast<relive::Path_FlyingSlig*>(mTlvHeader), gPathInfo->TLVInfo_From_TLVPtr(mTlvHeader));
                if (pFlyingSlig)
                {
                    mTransformedSligId = pFlyingSlig->mBaseGameObjectId;
                    pFlyingSlig->mXPos = mXPos;
                    pFlyingSlig->mYPos = mYPos - FP_FromInteger(15);
                    pFlyingSlig->field_294_nextXPos = mXPos;
                    pFlyingSlig->field_298_nextYPos = pFlyingSlig->mYPos;
                    pFlyingSlig->SetSpriteScale(GetSpriteScale());
                    pFlyingSlig->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

                    if (BrainIs(ICrawlingSligBrain::EBrainTypes::Possessed))
                    {
                        pFlyingSlig->ToPlayerControlled();
                        pFlyingSlig->SetPossessed(true);
                        pFlyingSlig->mAbeLevel = mAbeLevel;
                        pFlyingSlig->mAbePath = mAbePath;
                        pFlyingSlig->mAbeCamera = mAbeCamera;
                        sControlledCharacter = pFlyingSlig;
                        pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * GetSpriteScale());
                        pFlyingSlig->field_2AC_up_vel = (-FP_FromDouble(5.5) * GetSpriteScale());
                        pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * GetSpriteScale());
                        pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.3) * GetSpriteScale());
                        pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.8) * GetSpriteScale());
                    }
                    else
                    {
                        pFlyingSlig->SetBrain(&FlyingSlig::Brain_17_FromCrawlingSlig);
                    }
                }
                mHealth = FP_FromInteger(0);
            }
            else
            {
                // Impossible case - slig will go away and not transform into anything ??
            }

            // Final transform
            SetDead(true);
            SetBrain(ICrawlingSligBrain::EBrainTypes::Transformed);
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, true);
            GetAnimation().SetAnimate(false);
            GetAnimation().SetRender(false);
            SetType(ReliveTypes::eNone);
        }
    }
    else
    {
        if (GetAnimation().GetIsLastFrame())
        {
            HandleCommon();
        }
    }
}

void CrawlingSlig::Motion_2_WakingUp()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        HandleCommon();
    }
}

void CrawlingSlig::Motion_3_Crawling()
{
    if (CanCrawl())
    {
        if (GetAnimation().GetCurrentFrame() == 3 || GetAnimation().GetCurrentFrame() == 6)
        {
            Slig_SoundEffect(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
        }
        else if (GetAnimation().GetCurrentFrame() == 11)
        {
            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetNextMotion() == CrawlingSligMotion::Motion_0_Idle || GetNextMotion() == CrawlingSligMotion::Motion_11_TurnAround || GetNextMotion() == CrawlingSligMotion::Motion_7_ToShakingToIdle || GetNextMotion() == CrawlingSligMotion::Motion_8_Speaking)
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_15_EndCrawling, false);
                }
            }
            else
            {
                if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !(Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight)))
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_15_EndCrawling, true);
                }
            }
            MapFollowMe(true);
        }
    }
}

void CrawlingSlig::Motion_4_StartFalling()
{
    Motion_5_Falling();
    if (GetAnimation().GetIsLastFrame())
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_5_Falling, true);
    }
}

void CrawlingSlig::Motion_5_Falling()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * mVelxScaleFactor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * mVelxScaleFactor) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(ICrawlingSligBrain::EBrainTypes::Possessed))
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                PlatformCollide();
                mYPos = hitY;
                mXPos = hitX;
                MapFollowMe(true);
                if ((hitY - BaseAliveGameObjectLastLineYPos) > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5)))
                {
                    SetBrain(ICrawlingSligBrain::EBrainTypes::GetKilled);
                    mGetKilledBrain.SetState(GetKilledBrain::EState::eGibsDeath);
                }
                else
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_6_Landing, true);
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                mVelX = -mVelX / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void CrawlingSlig::Motion_6_Landing()
{
    if (GetAnimation().GetIsLastFrame())
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
    }
}

void CrawlingSlig::Motion_7_ToShakingToIdle()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetNextMotion() == CrawlingSligMotion::Motion_14_ShakingToIdle)
        {
            Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
        }
    }
}

void CrawlingSlig::Motion_8_Speaking()
{
    if (GetAnimation().GetCurrentFrame() == 2 && mSpeak != SligSpeak::eNone)
    {
        if (gMap.mCurrentPath == mCurrentPath && gMap.mCurrentLevel == mCurrentLevel && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            Slig_GameSpeak_SFX(mSpeak, 0, 0, this);
        }
        mSpeak = SligSpeak::eNone;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        HandleCommon();
    }
}

void CrawlingSlig::Motion_9_Snoozing()
{
    if (GetNextMotion() == CrawlingSligMotion::Motion_2_WakingUp)
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_2_WakingUp, true);
    }
    else if (!(sGnFrame & 31))
    {
        if ((sGnFrame / 25) & 1)
        {
            Slig_SoundEffect(SligSfx::eSnooze1_5, this);
        }
        else
        {
            Slig_SoundEffect(SligSfx::eSnooze2_4, this);
        }

        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            FP xOff = {};
            if (GetAnimation().GetFlipX())
            {
                xOff = -(GetSpriteScale() * FP_FromInteger(-10));
            }
            else
            {
                xOff = (GetSpriteScale() * FP_FromInteger(-10));
            }

            const FP yOff = (GetSpriteScale() * FP_FromInteger(-10));
            relive_new SnoozeParticle(
                mXPos + xOff,
                mYPos + yOff,
                GetAnimation().GetRenderLayer(),
                GetAnimation().GetSpriteScale());
        }
    }
}

void CrawlingSlig::Motion_10_PushingWall()
{
    MoveOnLine();

    if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 8)
    {
        MapFollowMe(true);
        mVelX = FP_FromInteger(0);
        Slig_SoundEffect(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (BrainIs(ICrawlingSligBrain::EBrainTypes::Possessed))
    {
        const bool flipX = GetAnimation().GetFlipX();
        if ((!flipX && Input().IsAnyHeld(InputCommands::eLeft)) || (flipX && Input().IsAnyHeld(InputCommands::eRight)) || !(Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight)))
        {
            Set_AnimAndMotion(CrawlingSligMotion::Motion_17_EndPushingWall, true);
        }
        else
        {
            FP yPos = {};
            if (flipX)
            {
                yPos = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                yPos = ScaleToGridSize(GetSpriteScale());
            }

            if (!WallHit(GetSpriteScale() * FP_FromInteger(30), yPos))
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_3_Crawling, true);
            }
        }
    }
    else
    {
        if (GetNextMotion() == CrawlingSligMotion::Motion_0_Idle || GetNextMotion() == CrawlingSligMotion::Motion_1_UsingButton || GetNextMotion() == CrawlingSligMotion::Motion_11_TurnAround || GetNextMotion() == CrawlingSligMotion::Motion_7_ToShakingToIdle || GetNextMotion() == CrawlingSligMotion::Motion_8_Speaking)
        {
            Set_AnimAndMotion(CrawlingSligMotion::Motion_17_EndPushingWall, false);
        }
    }
}

void CrawlingSlig::Motion_11_TurnAround()
{
    if (GetAnimation().GetCurrentFrame() == 8)
    {
        Slig_SoundEffect(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        MapFollowMe(true);
        HandleCommon();
    }
}

void CrawlingSlig::Motion_12_Shaking()
{
    // Do nothing
}

void CrawlingSlig::Motion_13_Empty()
{
    
}

void CrawlingSlig::Motion_14_ShakingToIdle()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (mHealth > FP_FromInteger(0))
        {
            mHealth = FP_FromInteger(1);
        }
        ToIdle();
    }
}

void CrawlingSlig::Motion_15_EndCrawling()
{
    if (GetAnimation().GetIsLastFrame())
    {
        HandleCommon();
    }
}

void CrawlingSlig::Motion_16_IdleToPushingWall()
{
    if (GetAnimation().GetIsLastFrame())
    {
        Slig_GameSpeak_SFX(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
        Set_AnimAndMotion(CrawlingSligMotion::Motion_10_PushingWall, true);
    }
}

void CrawlingSlig::Motion_17_EndPushingWall()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void CrawlingSlig::HandleCommon()
{
    MapFollowMe(true);

    if (BrainIs(ICrawlingSligBrain::EBrainTypes::Possessed) && mPossessedBrain.State() == PossessedBrain::EState::ePossessed)
    {
        if (Input().IsAnyHeld(InputCommands::eRight))
        {
            if (GetAnimation().GetFlipX())
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            }
            else
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            }
        }
        else if (Input().IsAnyHeld(InputCommands::eLeft))
        {
            if (GetAnimation().GetFlipX())
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            }
            else
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            }
        }
        else if (Input().IsAnyPressed(InputCommands::eUp))
        {
            mTlvHeader = FindPantsOrWings();
            if (mTlvHeader)
            {
                if (!mTlvHeader->mTlvSpecificMeaning)
                {
                    mTlvHeader->mTlvSpecificMeaning &= 0xFF;
                    mTlvHeader->mTlvSpecificMeaning |= 1;
                    SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                    mMultiUseTimer = sGnFrame + 20;
                }
            }
            else
            {
                auto pSligButton = FindSligButton();
                if (pSligButton)
                {
                    SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                    mSligButtonId = pSligButton->mBaseGameObjectId;
                }
                else
                {
                    SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
                    mSpeak = SligSpeak::eWhat_9;
                }
            }
        }
        if (Input().IsAnyPressed(InputCommands::eGameSpeak1))
        {
            mSpeak = SligSpeak::eHi_0;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak3))
        {
            mSpeak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak ::eFreeze_8;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak4))
        {
            mSpeak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak ::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak2))
        {
            mSpeak = SligSpeak::eHereBoy_1;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak6))
        {
            mSpeak = SligSpeak::eBullshit_5;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak7))
        {
            mSpeak = SligSpeak::eLookOut_6;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak5))
        {
            mSpeak = SligSpeak::eBullshit2_7;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak8))
        {
            mSpeak = SligSpeak::eLaugh_3;
        }

        if (mSpeak != SligSpeak::eNone)
        {
            SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
        }
    }

    switch (GetNextMotion())
    {
        case CrawlingSligMotion::None: // TODO: None constant
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle)
            {
                SetNextMotion(CrawlingSligMotion::Motion_0_Idle);
                Set_AnimAndMotion(GetNextMotion(), true);
                break;
            }
            break;

        case CrawlingSligMotion::Motion_0_Idle:
        case CrawlingSligMotion::Motion_1_UsingButton:
        case CrawlingSligMotion::Motion_7_ToShakingToIdle:
        case CrawlingSligMotion::Motion_8_Speaking:
        case CrawlingSligMotion::Motion_11_TurnAround:
            Set_AnimAndMotion(GetNextMotion(), true);
            break;

        case CrawlingSligMotion::Motion_3_Crawling:
        {
            FP gridScale = {};
            if (GetAnimation().GetFlipX())
            {
                gridScale = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridScale = ScaleToGridSize(GetSpriteScale());
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(30), gridScale))
            {
                if (sControlledCharacter == this)
                {
                    SetNextMotion(CrawlingSligMotion::Motion_16_IdleToPushingWall);
                }
            }
            Set_AnimAndMotion(GetNextMotion(), true);
            break;
        }

        default:
            return;
    }
}

const FP sCrawlingSligXVels_54471C[15] = {
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(1.17),
    FP_FromDouble(2.56),
    FP_FromDouble(2.85),
    FP_FromDouble(2.23),
    FP_FromDouble(1.87),
    FP_FromDouble(3.47),
    FP_FromDouble(3.61),
    FP_FromDouble(3.96),
    FP_FromDouble(2.69),
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(0)};

s16 CrawlingSlig::CanCrawl()
{
    mVelX = sCrawlingSligXVels_54471C[GetAnimation().GetCurrentFrame()];

    FP gridScale = ScaleToGridSize(GetSpriteScale());
    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
        gridScale = -gridScale;
    }

    mVelX = (mVelX * GetSpriteScale());

    if (sControlledCharacter == this && WallHit(GetSpriteScale() * FP_FromInteger(30), gridScale))
    {
        mVelxScaleFactor = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        Set_AnimAndMotion(CrawlingSligMotion::Motion_10_PushingWall, true);
        const s32 snappedX = SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos));
        mVelX = ((FP_FromInteger(snappedX) - mXPos) / FP_FromInteger(4));
        Slig_GameSpeak_SFX(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
        return false;
    }
    else
    {
        MoveOnLine();
        return BaseAliveGameObjectCollisionLine != nullptr;
    }
}

void CrawlingSlig::MoveOnLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = Guid{};
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PlatformCollide();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            if (GetCurrentMotion() == CrawlingSligMotion::Motion_3_Crawling)
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_4_StartFalling, true);
            }
            mVelxScaleFactor = FP_FromInteger(1);
            mXPos = mXPos + mVelX;
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        Set_AnimAndMotion(CrawlingSligMotion::Motion_5_Falling, true);
    }
}

void CrawlingSlig::PlatformCollide()
{
    const PSX_RECT bRect = VGetBoundingRect();

    PSX_Point xy = {bRect.x, bRect.y};
    PSX_Point wh = {bRect.w, bRect.h};
    xy.y += 5;
    wh.y += 5;

    OnCollisionWith(
        xy,
        wh,
        gPlatformsArray);
}

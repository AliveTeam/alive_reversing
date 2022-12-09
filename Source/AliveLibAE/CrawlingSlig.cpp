#include "stdafx.h"
#include "CrawlingSlig.hpp"
#include "Function.hpp"
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
#include "../relive_lib/Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "ScreenShake.hpp"
#include "Sound/Midi.hpp"
#include "Slig.hpp"
#include "FlyingSlig.hpp"
#include "CrawlingSligButton.hpp"
#include "Sfx.hpp"
#include "Grid.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"
#include "Game.hpp"

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


const static TCrawlingSligBrainFn sCrawlingSligBrainTable[6] = {
    &CrawlingSlig::Brain_0_Sleeping,
    &CrawlingSlig::Brain_1_Idle,
    &CrawlingSlig::Brain_2_PanicGetALocker,
    &CrawlingSlig::Brain_3_Possessed,
    &CrawlingSlig::Brain_4_GetKilled,
    &CrawlingSlig::Brain_5_Transformed,
};

enum Brain_2_PanicGetALocker
{
    eBrain2_DetermineCrawlDirection_0 = 0,
    eBrain2_Falling_1 = 1,
    eBrain2_SearchLocker_2 = 2,
    eBrain2_TurnAroundForLocker_3 = 3,
    eBrain2_SearchLockerOrTurnAround_4 = 4,
    eBrain2_GetPantsOrWings_5 = 5,
    eBrain2_UsingButton_6 = 6,
    eBrain2_TurnAround_7 = 7,
    eBrain2_Crawling_8 = 8,
    eBrain2_CheckIfWallHit_9 = 9,
    eBrain2_BeatBySlig_10 = 10
};

enum Brain_2_Possessed
{
    eBrain3_StartPossession_0 = 0,
    eBrain3_Possessed_1 = 1,
    eBrain3_Unpossessing_2 = 2,
    eBrain3_BeatBySlig_3 = 3
};

enum Brain_4_GetKilled
{
    eBrain4_Unknown_0 = 0,
    eBrain4_Vaporize_1 = 1,
    eBrain4_GibsDeath_2 = 2,
    eBrain4_SetDead_3 = 3,
    eBrain4_DeathBySlog_4 = 4,
    eBrain4_DeathDrop_5 = 5
};

void CrawlingSlig::LoadAnimations()
{
    for (auto& animId : sCrawlingSligAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

CrawlingSlig::CrawlingSlig(relive::Path_CrawlingSlig* pTlv, const Guid& guid)
    : BaseAliveGameObject(2)
{
    SetType(ReliveTypes::eCrawlingSlig);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::CrawlingSlig_Idle));

    SetTint(&kCrawlingSligTints[0], gMap.mCurrentLevel);

    SetCanBePossessed(true);

    field_11C_mPal = std::make_shared<AnimationPal>();

    CreateShadow();

    mGuid = guid;
    mTlv = *pTlv;

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
        SetBrain(&CrawlingSlig::Brain_1_Idle);
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
        SetBrain(&CrawlingSlig::Brain_0_Sleeping);
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
    if (sCollisions->Raycast(
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

s32 CrawlingSlig::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const CrawlingSligSaveState*>(pBuffer);

    auto pTlv = static_cast<relive::Path_CrawlingSlig*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mCrawlingSligTlvId));

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

        pCrawlingSlig->mCurrentMotion = pState->mCurrentMotion;

        pCrawlingSlig->GetAnimation().Set_Animation_Data(pCrawlingSlig->GetAnimRes(sCrawlingSligAnimIdTable[pState->mCurrentMotion]));
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
        pCrawlingSlig->mCurrentMotion = pState->mCurrentMotion2;
        pCrawlingSlig->mNextMotion = pState->mNextMotion;
        pCrawlingSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->mLastLineYPos);
        pCrawlingSlig->SetRestoredFromQuickSave(true);
        pCrawlingSlig->mMultiUseTimer = pState->mMultiUseTimer;
        pCrawlingSlig->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
        pCrawlingSlig->mGuid = pState->mCrawlingSligTlvId;
        pCrawlingSlig->SetBrain(sCrawlingSligBrainTable[pState->mBrainState]);
        pCrawlingSlig->mBrainSubState = pState->mBrainSubState;
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

    return sizeof(CrawlingSligSaveState);
}

s32 CrawlingSlig::VGetSaveState(u8* pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return 0;
    }

    auto pState = reinterpret_cast<CrawlingSligSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eCrawlingSlig;
    pState->mBaseTlvId = mBaseGameObjectTlvInfo;

    pState->mXPos = mXPos;
    pState->mYPos = mYPos;
    pState->mVelX = mVelX;
    pState->mVelY = mVelY;

    pState->mVelxScaleFactor = mVelxScaleFactor;

    pState->mCurrentPath = mCurrentPath;
    pState->mCurrentLevel = mCurrentLevel;
    pState->mSpriteScale = GetSpriteScale();

    pState->mR = mRGB.r;
    pState->mG = mRGB.g;
    pState->mB = mRGB.b;
    pState->mFlipX = GetAnimation().GetFlipX();
    pState->mCurrentMotion = mCurrentMotion;
    pState->mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->mDrawable = GetDrawable();
    pState->mRender = GetAnimation().GetRender();
    pState->mHealth = mHealth;
    pState->mCurrentMotion2 = mCurrentMotion;
    pState->mNextMotion = mNextMotion;
    pState->mCollisionLineType = -1;

    // TODO: Check correct
    pState->mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }

    pState->mControlled = (this == sControlledCharacter);
    pState->mMultiUseTimer = mMultiUseTimer;
    pState->mCrawlingSligTlvId = mGuid;
    pState->mBrainState = 0;

    s32 idx = 0;
    for (const auto& fn : sCrawlingSligBrainTable)
    {
        if (BrainIs(fn))
        {
            pState->mBrainState = idx;
            break;
        }
        idx++;
    }

    pState->mBrainSubState = mBrainSubState;
    pState->mChanting = mChanting;
    pState->mAbeLevel = mAbeLevel;
    pState->mAbePath = mAbePath;
    pState->mAbeCamera = mAbeCamera;
    pState->mSligButtonTlvId = mSligButtonId;
    pState->field_70_obj_id = field_1D4_obj_id;
    pState->mTransformedSligId = mTransformedSligId;
    pState->mSpeak = mSpeak;
    pState->mSayHelpTimer = mSayHelpTimer;
    return sizeof(CrawlingSligSaveState);
}

void CrawlingSlig::VPossessed()
{
    SetPossessed(true);
    mChanting = true;
    Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, true);
    SetBrain(&CrawlingSlig::Brain_3_Possessed);
    mBrainSubState = 0;
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
    return field_10_resources_array.ItemAt(0);
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
                sCollisions->Raycast(
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

        if (!Input_IsChanting_45F260())
        {
            mChanting = false;
        }

        mBrainSubState = (this->*mBrainState)();

        const FP oldX = mXPos;
        const FP oldY = mYPos;

        InvokeMemberFunction(this, sCrawlingSligMotions,mCurrentMotion);

        if (oldX != mXPos || oldY != mYPos)
        {
            auto pTlv = sPathInfo->TlvGetAt(
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
    auto pSlamDoor = static_cast<relive::Path_SlamDoor*>(sPathInfo->TLV_Get_At(
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

    auto pStopper = static_cast<relive::Path_EnemyStopper*>(sPathInfo->TLV_Get_At(
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
    relive::Path_TLV* pTlvIter = sPathInfo->TlvGetAt(nullptr, mXPos, mYPos, mXPos, mYPos);
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == ReliveTypes::eSligGetPants || pTlvIter->mTlvType == ReliveTypes::eSligGetWings)
        {
            return pTlvIter;
        }
        pTlvIter = sPathInfo->TlvGetAt(pTlvIter, mXPos, mYPos, mXPos, mYPos);
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
                SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                mBrainSubState = Brain_4_GetKilled::eBrain4_DeathDrop_5;
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, 0, this);
                mMultiUseTimer = sGnFrame + 60;
            }
        }

        pTlv = sPathInfo->TlvGetAt(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

s16 CrawlingSlig::VTakeDamage(BaseGameObject* pFrom)
{
    if (!BrainIs(&CrawlingSlig::Brain_5_Transformed))
    {
        switch (pFrom->Type())
        {
            case ReliveTypes::eBullet:
            case ReliveTypes::eDrill:
            case ReliveTypes::eGroundExplosion:
            case ReliveTypes::eRockSpawner:
            case ReliveTypes::eMeatSaw:
            case ReliveTypes::eMineCar:
            //case ReliveTypes::eNeverSet:
            case ReliveTypes::eAirExplosion:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    mBrainSubState = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return 1;

            case ReliveTypes::eElectricWall:
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, 0, this);
                return 1;

            case ReliveTypes::eSlig:
            {
                // Take a BEATING
                mMultiUseTimer = sGnFrame + 20;
                mHealth -= FP_FromDouble(0.13);

                if (mHealth <= FP_FromInteger(0))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    mBrainSubState = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                    return 0;
                }

                Set_AnimAndMotion(CrawlingSligMotion::Motion_7_ToShakingToIdle, true);
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, 0, this);

                if (BrainIs(&CrawlingSlig::Brain_2_PanicGetALocker))
                {
                    mBrainSubState = Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10;
                }
                else if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
                {
                    mBrainSubState = Brain_2_Possessed::eBrain3_BeatBySlig_3;
                }
                else
                {
                    SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker);
                    mBrainSubState = Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10;
                }
            }
                return 1;

            case ReliveTypes::eSlog:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    mBrainSubState = Brain_4_GetKilled::eBrain4_DeathBySlog_4;
                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);
                    mHealth = FP_FromInteger(0);
                    MapFollowMe(true);
                    mMultiUseTimer = sGnFrame + 15;
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_13_Empty, true);
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return 1;

            case ReliveTypes::eElectrocute:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled))
                {
                    GetAnimation().SetRender(false);
                    mHealth = FP_FromInteger(0);
                    mMultiUseTimer = sGnFrame + 1;
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    mBrainSubState = Brain_4_GetKilled::eBrain4_SetDead_3;
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return 1;

            default:
                return 1;
        }
    }
    return 1;
}

void CrawlingSlig::SetBrain(TCrawlingSligBrainFn fn)
{
    mBrainState = fn;
}

bool CrawlingSlig::BrainIs(TCrawlingSligBrainFn fn)
{
    return mBrainState == fn;
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

enum Brain_0_Sleeping
{
    eBrain0_Sleeping_0 = 0,
    eBrain0_WakingUp_1 = 1,
    eBrain0_IsAwake_2 = 2
};

s16 CrawlingSlig::Brain_0_Sleeping()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (mBrainSubState == Brain_0_Sleeping::eBrain0_Sleeping_0)
    {
        if (PanicOn())
        {
            mMultiUseTimer = sGnFrame + Math_RandomRange(15, 45);
            return Brain_0_Sleeping::eBrain0_WakingUp_1;
        }
        else
        {
            return mBrainSubState;
        }
    }

    if (mBrainSubState == Brain_0_Sleeping::eBrain0_WakingUp_1)
    {
        if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
        {
            return mBrainSubState;
        }

        SetNextMotion(CrawlingSligMotion::Motion_2_WakingUp);
        return Brain_0_Sleeping::eBrain0_IsAwake_2;
    }

    if (mBrainSubState != Brain_0_Sleeping::eBrain0_IsAwake_2 ||
        GetCurrentMotion() != CrawlingSligMotion::Motion_2_WakingUp ||
        !(GetAnimation().GetIsLastFrame()))
    {
        return mBrainSubState;
    }

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }

    ToIdle();
    SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker);
    return 0;
}

s16 CrawlingSlig::Brain_1_Idle()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    if (PanicOn())
    {
        SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker);
    }
    return 0;
}

s16 CrawlingSlig::Brain_2_PanicGetALocker()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    if (!BaseAliveGameObjectCollisionLine && mBrainSubState != Brain_2_PanicGetALocker::eBrain2_Falling_1)
    {
        mBrainSubState = Brain_2_PanicGetALocker::eBrain2_Falling_1;
    }

    switch (mBrainSubState)
    {
        case Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0:
            if ((mCrawlDirection != relive::Path_CrawlingSlig::CrawlDirection::eRight || !(GetAnimation().GetFlipX())) && (mCrawlDirection != relive::Path_CrawlingSlig::CrawlDirection::eLeft || GetAnimation().GetFlipX()))
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;
            }
            SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;

        case Brain_2_PanicGetALocker::eBrain2_Falling_1:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0;

        case Brain_2_PanicGetALocker::eBrain2_SearchLocker_2:
            if (HandleEnemyStopper(mVelX))
            {
                mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
            }

            mTlvHeader = FindPantsOrWings();
            if (mTlvHeader)
            {
                ToIdle();
                return Brain_2_PanicGetALocker::eBrain2_GetPantsOrWings_5;
            }
            else
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mSayHelpTimer)
                {
                    return mBrainSubState;
                }
                SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
                mSayHelpTimer = sGnFrame + 60;
                mSpeak = SligSpeak::eHelp_10;
                return Brain_2_PanicGetALocker::eBrain2_SearchLockerOrTurnAround_4;
            }
            break;

        case Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_11_TurnAround || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;

        case Brain_2_PanicGetALocker::eBrain2_SearchLockerOrTurnAround_4:
        {
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_8_Speaking || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }

            FP gridScale = {};
            if (GetAnimation().GetFlipX())
            {
                gridScale = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridScale = ScaleToGridSize(GetSpriteScale());
            }

            if (!WallHit(FP_FromInteger(35), gridScale))
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;
            }
            mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
            SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
        }

        case Brain_2_PanicGetALocker::eBrain2_GetPantsOrWings_5:
            if (!mTlvHeader)
            {
                mTlvHeader = FindPantsOrWings();
            }

            if (GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle || (mTlvHeader && mTlvHeader->mTlvSpecificMeaning))
            {
                if (Math_NextRandom() & 1)
                {
                    mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
                }
                else
                {
                    mMultiUseTimer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                    return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
                }
            }
            else
            {
                mTlvHeader->mTlvSpecificMeaning &= 0xFF;
                mTlvHeader->mTlvSpecificMeaning |= 1;

                SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                mMultiUseTimer = sGnFrame + 20;
                return Brain_2_PanicGetALocker::eBrain2_UsingButton_6;
            }
            break;

        case Brain_2_PanicGetALocker::eBrain2_UsingButton_6:
            if (mTlvHeader)
            {
                return mBrainSubState;
            }
            mTlvHeader = FindPantsOrWings();
            return mBrainSubState;

        case Brain_2_PanicGetALocker::eBrain2_TurnAround_7:
            if (GetCurrentMotion() == CrawlingSligMotion::Motion_11_TurnAround && GetAnimation().GetIsLastFrame())
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
            }
            return mBrainSubState;

        case Brain_2_PanicGetALocker::eBrain2_Crawling_8:
            if (HandleEnemyStopper(mVelX))
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }

            if (static_cast<s32>(sGnFrame) > mMultiUseTimer)
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }

            if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mSayHelpTimer)
            {
                return mBrainSubState;
            }

            SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
            mSayHelpTimer = sGnFrame + 60;
            mSpeak = SligSpeak::eHelp_10;
            return Brain_2_PanicGetALocker::eBrain2_CheckIfWallHit_9;

        case Brain_2_PanicGetALocker::eBrain2_CheckIfWallHit_9:
        {
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_8_Speaking || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }

            FP gridScale = {};
            if (GetAnimation().GetFlipX())
            {
                gridScale = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridScale = ScaleToGridSize(GetSpriteScale());
            }

            if (WallHit(FP_FromInteger(35), gridScale))
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }
            else
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
            }
        }
        break;

        case Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10:
            if (GetCurrentMotion() == CrawlingSligMotion::Motion_14_ShakingToIdle)
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, true);
                    mBrainSubState = Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0;
                    return mBrainSubState;
                }
            }
            else if (static_cast<s32>(sGnFrame) > mMultiUseTimer)
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
            }
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

s16 CrawlingSlig::Brain_3_Possessed()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_2_Possessed::eBrain3_StartPossession_0:
            if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
            {
                return mBrainSubState;
            }
            Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
            return Brain_2_Possessed::eBrain3_Possessed_1;

        case Brain_2_Possessed::eBrain3_Possessed_1:
            if (!Input_IsChanting_45F260() || mChanting)
            {
                return mBrainSubState;
            }
            mMultiUseTimer = sGnFrame + 30;
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, true);
            return Brain_2_Possessed::eBrain3_Unpossessing_2;

        case Brain_2_Possessed::eBrain3_Unpossessing_2:
            if (Input_IsChanting_45F260())
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    const FP y = mYPos - (GetSpriteScale() * FP_FromInteger(Math_RandomRange(0, 30)));
                    const FP x = mXPos + (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20)));
                    New_TintChant_Particle(
                        x, y,
                        GetSpriteScale(),
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) <= mMultiUseTimer && sActiveHero->mHealth > FP_FromInteger(0))
                {
                    return mBrainSubState;
                }

                sControlledCharacter = sActiveHero;
                SetPossessed(false);
                gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
                SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                return mBrainSubState;
            }
            else
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
                return Brain_2_Possessed::eBrain3_Possessed_1;
            }
            break;

        case Brain_2_Possessed::eBrain3_BeatBySlig_3:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_14_ShakingToIdle)
            {
                if (static_cast<s32>(sGnFrame) > mMultiUseTimer)
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, true);
                }
                return mBrainSubState;
            }

            if (!(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }

            Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, 1);
            mBrainSubState = Brain_2_Possessed::eBrain3_Possessed_1;
            return mBrainSubState;

        default:
            break;
    }
    return mBrainSubState;
}

s16 CrawlingSlig::Brain_4_GetKilled()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_4_GetKilled::eBrain4_Unknown_0:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_7_ToShakingToIdle || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            return Brain_4_GetKilled::eBrain4_Vaporize_1;

        case Brain_4_GetKilled::eBrain4_Vaporize_1:
            if (mMultiUseTimer < static_cast<s32>((sGnFrame + 80)))
            {
                SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));
                mRGB.r -= 2;
                mRGB.g -= 2;
                mRGB.b -= 2;
            }

            if (static_cast<s32>(sGnFrame) < mMultiUseTimer - 24)
            {
                DeathSmokeEffect(true);
            }

            if (mMultiUseTimer < static_cast<s32>(sGnFrame))
            {
                SetDead(true);
            }
            return mBrainSubState;

        case Brain_4_GetKilled::eBrain4_GibsDeath_2:
        {
            relive_new Gibs(
                GibType::Slig_1,
                mXPos,
                mYPos,
                mVelX,
                mVelY,
                GetSpriteScale(),
                0);

            relive_new Blood(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                20);

            New_Smoke_Particles(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                GetSpriteScale(),
                3,
                RGB16{128, 128, 128});

            SfxPlayMono(relive::SoundEffects::KillEffect, 128, GetSpriteScale());
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90, GetSpriteScale());

            GetAnimation().SetRender(false);
            GetAnimation().SetAnimate(false);

            Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, true);
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            mMultiUseTimer = sGnFrame + 40;
            return Brain_4_GetKilled::eBrain4_SetDead_3;
        }

        case Brain_4_GetKilled::eBrain4_SetDead_3:
            if (static_cast<s32>(sGnFrame) > mMultiUseTimer)
            {
                SetDead(true);
            }
            return mBrainSubState;

        case Brain_4_GetKilled::eBrain4_DeathBySlog_4:
            if (static_cast<s32>(sGnFrame) <= mMultiUseTimer)
            {
                return mBrainSubState;
            }
            mMultiUseTimer = sGnFrame + 90;
            return Brain_4_GetKilled::eBrain4_Vaporize_1;

        case Brain_4_GetKilled::eBrain4_DeathDrop_5:
            if (static_cast<s32>(sGnFrame) < mMultiUseTimer)
            {
                if (!(static_cast<s32>(mMultiUseTimer - sGnFrame) % 15))
                {
                    Slig_GameSpeak_SFX_4C04F0(
                        SligSpeak::eHelp_10,
                        // TODO: revisit the logic below
                        static_cast<s16>(2 * (mMultiUseTimer & (0xFFFF - sGnFrame))),
                        0,
                        this);
                }

                if (static_cast<s32>(sGnFrame) == mMultiUseTimer - 6)
                {
                    SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
                }
                return mBrainSubState;
            }
            else
            {
                Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);
                relive_new ScreenShake(0, 0);
                mMultiUseTimer = sGnFrame + 30;
                return Brain_4_GetKilled::eBrain4_SetDead_3;
            }
            break;

        default:
            return mBrainSubState;
    }
}

s16 CrawlingSlig::Brain_5_Transformed()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(mTransformedSligId);
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (!pObj || pObj->GetDead())
    {
        SetDead(true);
    }

    return 0;
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

                auto pWalkingSlig = relive_new Slig(static_cast<relive::Path_Slig*>(mTlvHeader), sPathInfo->TLVInfo_From_TLVPtr(mTlvHeader));
                if (pWalkingSlig)
                {
                    mTransformedSligId = pWalkingSlig->mBaseGameObjectId;

                    pWalkingSlig->SetSpriteScale(GetSpriteScale());

                    pWalkingSlig->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

                    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
                    {
                        pWalkingSlig->SetPossessed(true);
                        pWalkingSlig->mAbeLevel = mAbeLevel;
                        pWalkingSlig->mAbePath = mAbePath;
                        pWalkingSlig->mAbeCamera = mAbeCamera;
                        pWalkingSlig->SetBrain(&Slig::Brain_Possessed_2_4BBCF0);
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

                auto pFlyingSlig = relive_new FlyingSlig(static_cast<relive::Path_FlyingSlig*>(mTlvHeader), sPathInfo->TLVInfo_From_TLVPtr(mTlvHeader));
                if (pFlyingSlig)
                {
                    mTransformedSligId = pFlyingSlig->mBaseGameObjectId;
                    pFlyingSlig->mXPos = mXPos;
                    pFlyingSlig->mYPos = mYPos - FP_FromInteger(15);
                    pFlyingSlig->field_294_nextXPos = mXPos;
                    pFlyingSlig->field_298_nextYPos = pFlyingSlig->mYPos;
                    pFlyingSlig->SetSpriteScale(GetSpriteScale());
                    pFlyingSlig->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

                    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
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
            SetBrain(&CrawlingSlig::Brain_5_Transformed);
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
            Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
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
                if ((mVelX > FP_FromInteger(0) && Input().isPressed(InputCommands::Enum::eLeft)) || (mVelX < FP_FromInteger(0) && Input().isPressed(InputCommands::Enum::eRight)) || !(Input().isPressed(InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
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

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
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
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    mBrainSubState = Brain_4_GetKilled::eBrain4_GibsDeath_2;
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
            Slig_GameSpeak_SFX_4C04F0(mSpeak, 0, 0, this);
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
            Slig_SoundEffect_4BFFE0(SligSfx::eSnooze1_5, this);
        }
        else
        {
            Slig_SoundEffect_4BFFE0(SligSfx::eSnooze2_4, this);
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
        Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
    {
        const bool flipX = GetAnimation().GetFlipX();
        if ((!flipX && Input().isPressed(InputCommands::Enum::eLeft)) || (flipX && Input().isPressed(InputCommands::Enum::eRight)) || !(Input().isPressed(InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
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
        Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
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
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
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

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed) && mBrainSubState == Brain_2_Possessed::eBrain3_Possessed_1)
    {
        if (Input().isPressed(InputCommands::Enum::eRight))
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
        else if (Input().isPressed(InputCommands::Enum::eLeft))
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
        else if (Input().isHeld(InputCommands::Enum::eUp))
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
        if (Input().isHeld(InputCommands::Enum::eGameSpeak1))
        {
            mSpeak = SligSpeak::eHi_0;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak3))
        {
            mSpeak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak ::eFreeze_8;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak4))
        {
            mSpeak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak ::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak2))
        {
            mSpeak = SligSpeak::eHereBoy_1;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak6))
        {
            mSpeak = SligSpeak::eBullshit_5;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak7))
        {
            mSpeak = SligSpeak::eLookOut_6;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak5))
        {
            mSpeak = SligSpeak::eBullshit2_7;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak8))
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
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
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

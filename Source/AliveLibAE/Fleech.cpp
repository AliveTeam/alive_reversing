#include "stdafx.h"
#include "Fleech.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/SnoozeParticle.hpp"
#include "Dove.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "Gibs.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Grid.hpp"
#include "../relive_lib/Math.hpp"
#include "AnimationCallBacks.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include <algorithm>
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FatalError.hpp"
#include "QuikSave.hpp"

u8 sFleechRandomIdx_5BC20C = 0;
s16 sFleechCount_5BC20E = 0;

const static AnimId sFleechAnimFromMotion[19] = {
    AnimId::Fleech_Sleeping,
    AnimId::Fleech_WakingUp,
    AnimId::Fleech_Unused,
    AnimId::Fleech_Idle,
    AnimId::Fleech_Crawl,
    AnimId::Fleech_PatrolCry,
    AnimId::Fleech_Knockback,
    AnimId::Fleech_StopCrawling,
    AnimId::Fleech_StopMidCrawlCycle,
    AnimId::Fleech_Fall,
    AnimId::Fleech_Land,
    AnimId::Fleech_RaiseHead,
    AnimId::Fleech_Climb,
    AnimId::Fleech_SettleOnGround,
    AnimId::Fleech_ExtendTongueFromEnemy,
    AnimId::Fleech_RetractTongueFromEnemey,
    AnimId::Fleech_DeathByFalling,
    AnimId::Fleech_SleepingWithTongue,
    AnimId::Fleech_Consume };

const TFleechMotionFn sFleechMotionTable[19] = {
    &Fleech::Motion_0_Sleeping,
    &Fleech::Motion_1_WakingUp,
    &Fleech::Motion_2_Unknown,
    &Fleech::Motion_3_Idle,
    &Fleech::Motion_4_Crawl,
    &Fleech::Motion_5_PatrolCry,
    &Fleech::Motion_6_Knockback,
    &Fleech::Motion_7_StopCrawling,      //Stop due to being aligned vertically with target
    &Fleech::Motion_8_StopMidCrawlCycle,
    &Fleech::Motion_9_Fall,
    &Fleech::Motion_10_Land,
    &Fleech::Motion_11_RaiseHead,
    &Fleech::Motion_12_Climb,
    &Fleech::Motion_13_SettleOnGround,
    &Fleech::Motion_14_ExtendTongueFromEnemy,
    &Fleech::Motion_15_RetractTongueFromEnemey,
    &Fleech::Motion_16_DeathByFalling,
    &Fleech::Motion_17_SleepingWithTongue,
    &Fleech::Motion_18_Consume};

const relive::SfxDefinition stru_5518E0[20] = {
    {0u, 6u, 50u, 30u, -127, 127},
    {0u, 6u, 49u, 127u, 0, 0},
    {0u, 6u, 54u, 127u, 0, 0},
    {0u, 1u, 36u, 35u, 0, 0},
    {0u, 1u, 37u, 35u, 0, 0},
    {0u, 6u, 52u, 100u, 0, 0},
    {0u, 6u, 50u, 30u, -127, 0},
    {0u, 6u, 50u, 40u, 500, 600},
    {0u, 6u, 50u, 30u, 300, 400},
    {0u, 3u, 59u, 100u, 256, 384},
    {0u, 4u, 65u, 80u, 0, 0},
    {0u, 4u, 65u, 55u, 0, 0},
    {0u, 4u, 65u, 85u, 0, 0},
    {0u, 4u, 65u, 100u, 127, 127},
    {0u, 6u, 61u, 70u, -127, 127},
    {0u, 6u, 62u, 70u, -127, 127},
    {0u, 6u, 63u, 70u, -127, 127},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

static u8 Fleech_NextRandom()
{
    return gRandomBytes[sFleechRandomIdx_5BC20C++];
}

Fleech::Fleech(relive::Path_Fleech* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(2),
    mTlvInfo(tlvId),
    mAttackAngerIncreaser(pTlv->mAttackAngerIncreaser + 2),
    mWakeUpSwitchId(pTlv->mWakeUpSwitchId),
    mWakeUpSwitchAngerValue(pTlv->mWakeUpSwitchAngerValue),
    mWakeUpSwitchValue(SwitchStates_Get(pTlv->mWakeUpSwitchId) & 0xFFFF),
    mCanWakeUpSwitchId(pTlv->mCanWakeUpSwitchId),
    mLostTargetTimeout(pTlv->mLostTargetTimeout),
    mPatrolBrain(*this),
    mChasingAbeBrain(*this),
    mScaredBrain(*this),
    mDeathBrain(*this)
{
    SetBrain(IFleechBrain::EBrainTypes::Patrol);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mBaseGameObjectTlvInfo = tlvId;

    LoadAnimations();

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
    }

    Init();

    GetAnimation().SetFlipX(pTlv->mFacing == relive::reliveXDirection::eLeft);

    if (pTlv->mAsleep == relive::reliveChoice::eYes)
    {
        mAsleep = true;
    }

    if (pTlv->mGoesToSleep == relive::reliveChoice::eYes)
    {
        mGoesToSleep = true;
    }

    if (pTlv->mPersistant == relive::reliveChoice::eYes)
    {
        mPersistant = true;
    }

    mPatrolRange = FP_GetExponent(FP_FromInteger(pTlv->mPatrolRangeInGrids) * ScaleToGridSize(GetSpriteScale()));

    InitTonguePolys();

    if (pTlv->mHanging == relive::reliveChoice::eYes)
    {
        mHoistX = (pTlv->mBottomRightX + pTlv->mTopLeftX) / 2;
        mAngle = Fleech_NextRandom();
        mYPos -= FP_FromInteger(pTlv->mTopLeftY - pTlv->mBottomRightY);
        TongueHangingFromWall((pTlv->mBottomRightX + pTlv->mTopLeftX) / 2, pTlv->mTopLeftY);
        SetCurrentMotion(eFleechMotions::Motion_17_SleepingWithTongue);
        SetAnim();
    }
    else if (mAsleep)
    {
        SetCurrentMotion(eFleechMotions::Motion_0_Sleeping);
        SetAnim();
    }
    else
    {
        SetCurrentMotion(eFleechMotions::Motion_3_Idle);
        mCurrentAnger = mMaxAnger + (mAttackAngerIncreaser - mMaxAnger) / 2;
    }
}

Guid current_target_object_id_551840 = {};

void Fleech::LoadAnimations()
{
    for (auto& animId : sFleechAnimFromMotion)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

void Fleech::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<FleechSaveState>();

    auto pTlv = static_cast<relive::Path_Fleech*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));

    auto pFleech = relive_new Fleech(pTlv, pState->mTlvInfo);
    if (pFleech)
    {
        pFleech->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

        pFleech->BaseAliveGameObjectPathTLV = nullptr;
        pFleech->BaseAliveGameObjectCollisionLine = nullptr;

        pFleech->BaseAliveGameObject_PlatformId = pState->mPlatformId;

        pFleech->mXPos = pState->mXPos;
        pFleech->mYPos = pState->mYPos;
        pFleech->mVelX = pState->mVelX;
        pFleech->mVelY = pState->mVelY;

        pFleech->mCurrentPath = pState->mCurrentPath;
        pFleech->mCurrentLevel = pState->mCurrentLevel;
        pFleech->SetSpriteScale(pState->mSpriteScale);

        pFleech->mRGB.SetRGB(pState->mRed, pState->mGreen, pState->mBlue);

        pFleech->mCurrentMotion = static_cast<s16>(pState->field_28_current_motion);
        pFleech->GetAnimation().Set_Animation_Data(pFleech->GetAnimRes(sFleechAnimFromMotion[pFleech->mCurrentMotion]));
        pFleech->GetAnimation().SetCurrentFrame(pState->field_2A_anim_current_frame);
        pFleech->GetAnimation().SetFrameChangeCounter(pState->field_2C_frame_change_counter);

        pFleech->GetAnimation().SetFlipX(pState->mFlipX);
        pFleech->GetAnimation().SetRender(pState->mRender);

        pFleech->SetDrawable(pState->mDrawable);

        if (IsLastFrame(&pFleech->GetAnimation()))
        {
            pFleech->GetAnimation().SetIsLastFrame(true);
        }
        pFleech->mHealth = pState->mHealth;
        pFleech->mCurrentMotion = static_cast<s16>(pState->mCurrentMotion);
        pFleech->mNextMotion = static_cast<s16>(pState->mNextMotion);

        pFleech->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->mLastLineYPos);

        pFleech->SetRestoredFromQuickSave(true);

        pFleech->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
        pFleech->mTlvInfo = pState->mTlvInfo;
        pFleech->mFoodObjId = pState->mFoodObjId;
        pFleech->mTongueState = pState->mTongueState;
        pFleech->mTongueSubState = pState->mTongueSubState;
        pFleech->mEnemyXPos = pState->mEnemyXPos;
        pFleech->mEnemyYPos = pState->mEnemyYPos;
        pFleech->mTongueOriginX = pState->mTongueOriginX;
        pFleech->mTongueOriginY = pState->mTongueOriginY;
        pFleech->mTongueDestinationX = pState->mTongueDestinationX;
        pFleech->mTongueDestinationY = pState->mTongueDestinationY;
        pFleech->field_188 = pState->field_5A;

        pFleech->mTongueActive = pState->mTongueActive;
        pFleech->mRenderTongue = pState->mRenderTongue;

        pFleech->SetBrain(pState->mBrainType);
        pFleech->mPatrolBrain.SetState(pState->mPatrolBrainState);
        pFleech->mChasingAbeBrain.SetState(pState->mChasingAbeBrainState);
        pFleech->mScaredBrain.SetState(pState->mScaredBrainState);
        pFleech->field_12C_shrivel_timer = pState->field_64_shrivel_timer;
        pFleech->mReturnToPreviousMotion = pState->mReturnToPreviousMotion;
        sFleechRandomIdx_5BC20C = pState->field_68_fleech_random_idx;
        pFleech->field_130_bDidMapFollowMe = pState->field_6A_bDidMapFollowMe;
        pFleech->field_138_velx_factor = pState->field_70_velx_factor;
        pFleech->mCurrentAnger = pState->field_76_current_anger;
        pFleech->mMaxAnger = pState->mMaxAnger;
        pFleech->mAttackAngerIncreaser = pState->mAttackAngerIncreaser;
        pFleech->mWakeUpSwitchId = pState->mWakeUpSwitchId;
        pFleech->mWakeUpSwitchAngerValue = pState->mWakeUpSwitchAngerValue;
        pFleech->mWakeUpSwitchValue = pState->mWakeUpSwitchValue;
        pFleech->mCanWakeUpSwitchId = pState->mCanWakeUpSwitchId;
        pFleech->field_14C_EventXPos = pState->field_84_EventXPos;
        pFleech->field_14E_ScrabParamiteEventXPos = pState->field_86_ScrabParamiteEventXPos;
        pFleech->mPatrolRange = pState->mPatrolRange;
        pFleech->field_152_old_xpos = pState->field_8A_old_xpos;
        pFleech->field_154 = pState->field_8C;
        pFleech->field_156_rnd_crawl = pState->field_8E_rnd_crawl;
        pFleech->field_158_chase_delay = pState->field_90_chase_delay;
        pFleech->field_15A_chase_timer = pState->field_92_chase_timer;
        pFleech->mLostTargetTimeout = pState->mLostTargetTimeout;
        pFleech->field_15E_lost_target_timer = pState->field_96_lost_target_timer;
        pFleech->mHoistX = pState->mHoistX;
        pFleech->mHoistY = pState->mHoistY;
        pFleech->mAngle = pState->field_9E_angle;
        pFleech->mHoistYDistance = pState->mHoistYDistance;
        pFleech->mHoistXDistance = pState->mHoistXDistance;
        pFleech->mScrabOrParamite = pState->mScrabOrParamite;

        if (pState->field_4_obj_id == pState->field_AC_obj_id)
        {
            current_target_object_id_551840 = pFleech->mBaseGameObjectId;
        }

        pFleech->mHoistDone = pState->mHoistDone;
        pFleech->mChasingOrScaredCrawlingLeft = pState->mChasingOrScaredCrawlingLeft;
        pFleech->mShrivelDeath = pState->mShrivelDeath;
        pFleech->mScaredSound = pState->mScaredSound;
        pFleech->mAsleep = pState->mAsleep;
        pFleech->mGoesToSleep = pState->mGoesToSleep;
        pFleech->mPersistant = pState->mPersistant;
    }
}

void Fleech::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    FleechSaveState data = {};

    data.mType = ReliveTypes::eFleech;
    data.field_4_obj_id = mBaseGameObjectTlvInfo;
    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mVelY = mVelY;
    data.field_70_velx_factor = field_138_velx_factor;
    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;
    data.mSpriteScale = GetSpriteScale();
    data.mRed = mRGB.r;
    data.mGreen = mRGB.g;
    data.mBlue = mRGB.b;
    data.mFlipX = GetAnimation().GetFlipX();
    data.field_28_current_motion = static_cast<eFleechMotions>(mCurrentMotion);
    data.field_2A_anim_current_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.field_2C_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    data.mDrawable = GetDrawable();
    data.mRender = GetAnimation().GetRender();
    data.mHealth = mHealth;
    data.mCurrentMotion = static_cast<eFleechMotions>(mCurrentMotion);
    data.mNextMotion = static_cast<eFleechMotions>(mNextMotion);
    data.mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        data.mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        data.mCollisionLineType = -1;
    }

    if (BaseAliveGameObject_PlatformId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pObj)
        {
            data.mPlatformId = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        data.mPlatformId = Guid{};
    }

    if (mFoodObjId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mFoodObjId);
        if (pObj)
        {
            data.mFoodObjId = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        data.mFoodObjId = Guid{};
    }

    data.mTlvInfo = mTlvInfo;
    data.mTongueState = mTongueState;
    data.mTongueSubState = mTongueSubState;
    data.mEnemyXPos = mEnemyXPos;
    data.mEnemyYPos = mEnemyYPos;
    data.mTongueOriginX = mTongueOriginX;
    data.mTongueOriginY = mTongueOriginY;
    data.mTongueDestinationX = mTongueDestinationX;
    data.mTongueDestinationY = mTongueDestinationY;
    data.field_5A = field_188;
    data.mTongueActive = mTongueActive;
    data.mRenderTongue = mRenderTongue;
    data.mBrainType = mCurrentBrain->VGetBrain();
    data.mPatrolBrainState = mPatrolBrain.State();
    data.mChasingAbeBrainState = mChasingAbeBrain.State();
    data.mScaredBrainState = mScaredBrain.State();
    data.field_64_shrivel_timer = field_12C_shrivel_timer - sGnFrame;
    data.mReturnToPreviousMotion = mReturnToPreviousMotion;
    data.field_68_fleech_random_idx = sFleechRandomIdx_5BC20C;
    data.field_6A_bDidMapFollowMe = field_130_bDidMapFollowMe;
    data.field_70_velx_factor = field_138_velx_factor;
    data.field_76_current_anger = mCurrentAnger;
    data.mMaxAnger = mMaxAnger;
    data.mAttackAngerIncreaser = mAttackAngerIncreaser;
    data.mWakeUpSwitchId = mWakeUpSwitchId;
    data.mWakeUpSwitchAngerValue = mWakeUpSwitchAngerValue;
    data.mWakeUpSwitchValue = mWakeUpSwitchValue;
    data.mCanWakeUpSwitchId = mCanWakeUpSwitchId;
    data.field_84_EventXPos = field_14C_EventXPos;
    data.field_86_ScrabParamiteEventXPos = field_14E_ScrabParamiteEventXPos;
    data.mPatrolRange = mPatrolRange;
    data.field_8A_old_xpos = field_152_old_xpos;
    data.field_8C = field_154;
    data.field_8E_rnd_crawl = field_156_rnd_crawl;
    data.field_90_chase_delay = field_158_chase_delay;
    data.field_92_chase_timer = field_15A_chase_timer;
    data.mLostTargetTimeout = mLostTargetTimeout;
    data.field_96_lost_target_timer = field_15E_lost_target_timer;
    data.mHoistX = mHoistX;
    data.mHoistY = mHoistY;
    data.field_9E_angle = mAngle;
    data.mHoistYDistance = mHoistYDistance;
    data.mHoistXDistance = mHoistXDistance;

    if (mScrabOrParamite != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mScrabOrParamite);
        if (pObj)
        {
            data.mScrabOrParamite = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        data.mScrabOrParamite = Guid{};
    }

    if (current_target_object_id_551840 == mBaseGameObjectId)
    {
        data.field_AC_obj_id = mBaseGameObjectTlvInfo;
    }
    else
    {
        data.field_AC_obj_id = Guid{};
    }

    data.mHoistDone = mHoistDone;
    data.mChasingOrScaredCrawlingLeft = mChasingOrScaredCrawlingLeft;
    data.mShrivelDeath = mShrivelDeath;
    data.mScaredSound = mScaredSound;
    data.mAsleep = mAsleep;
    data.mGoesToSleep = mGoesToSleep;
    data.mPersistant = mPersistant;

    pSaveBuffer.Write(data);
}

void Fleech::Motion_0_Sleeping()
{
    if (mNextMotion == -1)
    {
        if (GetAnimation().GetCurrentFrame() || sGnFrame & 3)
        {
            if (GetAnimation().GetCurrentFrame() == 4 && !(sGnFrame & 3))
            {
                Sound(FleechSound::SleepingExhale_4);

                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    const FP yPos = (GetSpriteScale() * FP_FromInteger(-20)) + mYPos;
                    FP xOff = {};
                    if (GetAnimation().GetFlipX())
                    {
                        xOff = -(GetSpriteScale() * FP_FromInteger(10));
                    }
                    else
                    {
                        xOff = (GetSpriteScale() * FP_FromInteger(10));
                    }
                    relive_new SnoozeParticle(xOff + mXPos, yPos, GetAnimation().GetRenderLayer(), GetAnimation().GetSpriteScale());
                }
            }
        }
        else
        {
            Sound(FleechSound::SleepingInhale_3);
        }
    }
    else
    {
        mCurrentMotion = mNextMotion;
        SetNextMotion(eFleechMotions::eNone_m1);
    }
}

void Fleech::Motion_1_WakingUp()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (mNextMotion != -1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            SetNextMotion(eFleechMotions::eNone_m1);
            Sound(FleechSound::WakeUp_6);
        }
    }
}

// this function is actually used
void Fleech::Motion_2_Unknown()
{
    if (mNextMotion != -1)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = mNextMotion;
            SetNextMotion(eFleechMotions::eNone_m1);
        }
    }
}

void Fleech::Motion_3_Idle()
{
    if (!CanMove())
    {
        if (mNextMotion == -1)
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (GetCurrentMotion() == eFleechMotions::Motion_3_Idle && GetAnimation().GetCurrentFrame() == 0 && !gCollisions->Raycast(mXPos - FP_FromInteger(5), mYPos - FP_FromInteger(5), mXPos + FP_FromInteger(5), mYPos + FP_FromInteger(1), &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                field_138_velx_factor = FP_FromInteger(0);
                BaseAliveGameObjectLastLineYPos = mYPos;
                SetCurrentMotion(eFleechMotions::Motion_9_Fall);
            }
        }
        else
        {
            mCurrentMotion = mNextMotion;
            SetNextMotion(eFleechMotions::eNone_m1);
        }
    }
}

const FP sFleechCrawlVelX_544FA0[7] = {
    FP_FromDouble(3.8364105224609375),
    FP_FromDouble(3.36883544921875),
    FP_FromDouble(4.6882781982421875),
    FP_FromDouble(4.9884185791015625),
    FP_FromDouble(2.5774078369140625),
    FP_FromDouble(3.3113861083984375),
    FP_FromDouble(2.2292022705078125)};

void Fleech::Motion_4_Crawl()
{
    if (GetAnimation().GetCurrentFrame() > 6)
    {
        GetAnimation().SetCurrentFrame(0);
    }

    FP velXTable = {};
    if (GetAnimation().GetFlipX())
    {
        velXTable = -sFleechCrawlVelX_544FA0[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        velXTable = sFleechCrawlVelX_544FA0[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * velXTable);

    const FP k1Directed = FP_FromInteger((GetAnimation().GetFlipX() != 0) ? -1 : 1);
    if (WallHit(FP_FromInteger((GetSpriteScale() >= FP_FromInteger(1)) ? 10 : 5), ScaleToGridSize(GetSpriteScale()) * k1Directed))
    {
        ToIdle();
    }
    else
    {
        MoveAlongFloor();
        if (GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
        {
            if (GetAnimation().GetCurrentFrame() == 4)
            {
                Sound(FleechSound::CrawlRNG1_14);
            }
            else if (GetAnimation().GetCurrentFrame() == 6)
            {
                if (field_130_bDidMapFollowMe == 0)
                {
                    field_130_bDidMapFollowMe = 1;
                    MapFollowMe(true);
                }

                if (GetNextMotion() == eFleechMotions::Motion_3_Idle)
                {
                    SetCurrentMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
                    SetNextMotion(eFleechMotions::eNone_m1);
                }
                else if (mNextMotion != -1)
                {
                    mCurrentMotion = mNextMotion;
                    SetNextMotion(eFleechMotions::eNone_m1);
                }
            }
            else
            {
                field_130_bDidMapFollowMe = 0;
            }
        }
    }
}

void Fleech::Motion_5_PatrolCry()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sound(FleechSound::PatrolCry_0);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Fleech::Motion_6_Knockback()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        ToIdle();
    }
}

void Fleech::Motion_7_StopCrawling()
{
    const FP k1Directed = FP_FromInteger(GetAnimation().GetFlipX() != 0 ? -1 : 1);
    if (WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(GetSpriteScale()) * k1Directed))
    {
        ToIdle();
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);
        if (GetAnimation().GetFlipX())
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
            GetAnimation().SetFlipX(false);
        }
        else
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
            GetAnimation().SetFlipX(true);
        }
        SetCurrentMotion(eFleechMotions::Motion_4_Crawl);
    }
}

void Fleech::Motion_8_StopMidCrawlCycle()
{
    const FP k1Directed = FP_FromInteger(GetAnimation().GetFlipX() ? -1 : 1); // TODO: Correct way around ??
    if (WallHit(
            FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
            ScaleToGridSize(GetSpriteScale()) * k1Directed)
        || GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Fleech::Motion_9_Fall()
{
    mVelY += GetSpriteScale() * FP_FromDouble(1.8);

    if (mVelY > (GetSpriteScale() * FP_FromInteger(20)))
    {
        mVelY = (GetSpriteScale() * FP_FromInteger(20));
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (mVelX > FP_FromDouble(3.6))
        {
            mVelX = FP_FromDouble(3.6);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        if (mVelX < FP_FromDouble(-3.6))
        {
            mVelX = FP_FromDouble(-3.6);
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= GetSpriteScale() * field_138_velx_factor;
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += GetSpriteScale() * field_138_velx_factor;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    const FP xpos = mXPos;
    const FP ypos = mYPos - FP_FromInteger((GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5));
    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (gCollisions->Raycast(
            xpos,
            ypos,
            mXPos,
            mYPos,
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        switch (pLine->mLineType)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                BaseAliveGameObjectCollisionLine = pLine;
                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe(true);
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();
                    OnCollisionWith(
                        {bRect.x, static_cast<s16>(bRect.y + 5)},
                        {bRect.w, static_cast<s16>(FP_GetExponent(mYPos) + 5)},
                        gPlatformsArray);
                }
                if (mYPos - BaseAliveGameObjectLastLineYPos <= FP_FromInteger((GetSpriteScale() >= FP_FromInteger(1) ? 20 : 10)) * FP_FromInteger(15))
                {
                    SetCurrentMotion(eFleechMotions::Motion_10_Land);
                }
                else
                {
                    SetCurrentMotion(eFleechMotions::Motion_16_DeathByFalling);
                }
                break;

            case 1u:
            case 2u:
            case 5u:
            case 6u:
                mXPos = hitX - mVelX;
                mYPos = hitY;
                MapFollowMe(true);
                mVelX = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Fleech::Motion_10_Land()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sound(FleechSound::LandOnFloor_9);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Fleech::Motion_11_RaiseHead()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mHoistDone = false;
        SetCurrentMotion(eFleechMotions::Motion_12_Climb);

        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(-1);

        const s16 yOff = GetSpriteScale() >= FP_FromInteger(1) ? 0 : -10;
        auto pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->TLV_Get_At(
            mHoistX,
            FP_GetExponent(mYPos - FP_FromInteger((yOff + 20))),
            mHoistX,
            FP_GetExponent(mYPos - FP_FromInteger((yOff + 20))),
            ReliveTypes::eHoist));

        if (pHoist->mHoistType == relive::Path_Hoist::Type::eOffScreen)
        {
            const FP doubleYOff = FP_FromInteger(yOff + 20) * FP_FromInteger(2);
            pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->TLV_Get_At(
                mHoistX,
                FP_GetExponent(FP_FromInteger(pHoist->mTopLeftY) - doubleYOff),
                mHoistX,
                FP_GetExponent(FP_FromInteger(pHoist->mTopLeftY) - doubleYOff),
                ReliveTypes::eHoist));

            mHoistY = pHoist->mTopLeftY;
        }
        BaseAliveGameObjectLastLineYPos = mYPos;
        mHoistYDistance = mYPos - FP_FromInteger(mHoistY);
        BaseAliveGameObjectCollisionLine = nullptr;
        mHoistXDistance = FP_Abs(mXPos - FP_FromInteger(mHoistX));

        if (GetAnimation().GetFlipX())
        {
            mAngle = -64;
        }
        else
        {
            mAngle = 64;
        }
        sub_42BA10();
    }
    else if (GetAnimation().GetCurrentFrame() < 4)
    {
        mXPos += FP_FromInteger((GetAnimation().GetFlipX()) != 0 ? 1 : -1);
    }
}

void Fleech::Motion_12_Climb()
{
    if (!TongueActive())
    {
        if (mHoistDone)
        {
            SetCurrentMotion(eFleechMotions::Motion_13_SettleOnGround);
            return;
        }

        mVelY -= GetSpriteScale() * FP_FromDouble(0.7);

        const FP xOff = (Math_Cosine(mAngle) * (mHoistXDistance * ((mYPos - FP_FromInteger(mHoistY)) / mHoistYDistance)));

        FP pX1 = {};
        if (xOff < FP_FromInteger(0))
        {
            pX1 = xOff + mXPos;
        }
        else
        {
            pX1 = mXPos;
        }

        const FP pY1 = mVelY + mYPos - FP_FromInteger(GetSpriteScale() < FP_FromInteger(1) ? 10 : 20);

        FP pX2;
        if (xOff < FP_FromInteger(0))
        {
            pX2 = mXPos;
        }
        else
        {
            pX2 = xOff + mXPos;
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (gCollisions->Raycast(pX1, pY1, pX2, mVelY + mYPos, &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
        {
            switch (pLine->mLineType)
            {
                case eLineTypes::eWallLeft_1:
                case eLineTypes::eBackgroundWallLeft_5:
                    Sound(FleechSound::LandOnFloor_9);
                    // TODO: Somewhat suspect that these branches are equal - OG bug?
                    //if (field_166_angle >= 64u && field_166_angle > 192u)
                    //{
                    //    field_166_angle = -128 - field_166_angle;
                    //}
                    //else
                    //{
                    //    field_166_angle = -128 - field_166_angle;
                    //}
                    mAngle = -128 - mAngle;
                    break;

                case eLineTypes::eWallRight_2:
                case eLineTypes::eBackgroundWallRight_6:
                    Sound(FleechSound::LandOnFloor_9);
                    if (mAngle > 64u && mAngle < 128u)
                    {
                        mAngle = -128 - mAngle;
                    }
                    else if (false /*field_166_angle > 128 && field_166_angle < 192*/) // always false due to the field type - og bug?
                    {
                        mAngle = -128 - mAngle;
                    }
                    else
                    {
                        LOG_INFO("angle not set");
                    }
                    break;

                default:
                    break;
            }
        }

        mXPos = xOff + FP_FromInteger(mHoistX);
        mAngle = mAngle + 16;
        mYPos += mVelY;

        if (mYPos <= FP_FromInteger(mHoistY))
        {
            mXPos = FP_FromInteger(mHoistX);
            mYPos = FP_FromInteger(mHoistY);
            mVelY = FP_FromInteger(0);
            mHoistDone = true;
        }
        return;
    }
}

void Fleech::Motion_13_SettleOnGround()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        SetTongueState5();

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (gCollisions->Raycast(
                mXPos - FP_FromInteger(5),
                mYPos - FP_FromInteger(5),
                mXPos + FP_FromInteger(5),
                mYPos + FP_FromInteger(5),
                &pLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
        {
            BaseAliveGameObjectCollisionLine = pLine;
            MapFollowMe(true);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        Fleech::ToIdle();
    }
}

void Fleech::Motion_14_ExtendTongueFromEnemy()
{
    if (mFoodObjId == gAbe->mBaseGameObjectId && (gAbe->CantBeDamaged_44BAB0() || gAbe->GetInvisible()))
    {
        ToIdle();
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        PullTargetIn();
        SetCurrentMotion(eFleechMotions::Motion_15_RetractTongueFromEnemey);
    }
}

void Fleech::Motion_15_RetractTongueFromEnemey()
{
    if (IgAbe(sObjectIds.Find_Impl(mFoodObjId)) && ((gAbe->CantBeDamaged_44BAB0()) || gAbe->GetInvisible()))
    {
        sub_42B8C0();
        ToIdle();
    }
    else
    {
        if (!TongueActive())
        {
            ToIdle();
        }

        if (GetNextMotion() == eFleechMotions::Motion_18_Consume)
        {
            SetCurrentMotion(eFleechMotions::Motion_18_Consume);
            SetNextMotion(eFleechMotions::eNone_m1);
        }
    }
}

void Fleech::Motion_16_DeathByFalling()
{
    if (mHealth > FP_FromInteger(0))
    {
        relive_new Blood(mXPos, mYPos - FP_FromInteger(8), FP_FromInteger(0), -FP_FromInteger(5), GetSpriteScale(), 50);

        Sound(FleechSound::DeathByHeight_12);
        Sound(FleechSound::Scared_7);

        mHealth = FP_FromInteger(0);
        SetBrain(IFleechBrain::EBrainTypes::Death);
        mShrivelDeath = true;
        SetNextMotion(eFleechMotions::eNone_m1);
        field_12C_shrivel_timer = MakeTimer(127);
        sFleechCount_5BC20E--;
    }
}

void Fleech::Motion_17_SleepingWithTongue()
{
    if (mNextMotion != -1)
    {
        SetTongueState1();
        mCurrentMotion = mNextMotion;
        SetNextMotion(eFleechMotions::eNone_m1);
    }
    else
    {
        if (GetAnimation().GetCurrentFrame() || sGnFrame & 3)
        {
            if (GetAnimation().GetCurrentFrame() == 4 && !(sGnFrame & 3))
            {
                Sound(FleechSound::SleepingExhale_4);
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    const FP yPos = (GetSpriteScale() * FP_FromInteger(-20)) + mYPos;
                    FP xOff = {};
                    if (GetAnimation().GetFlipX())
                    {
                        xOff = -(GetSpriteScale() * FP_FromInteger(10));
                    }
                    else
                    {
                        xOff = (GetSpriteScale() * FP_FromInteger(10));
                    }
                    relive_new SnoozeParticle(xOff + mXPos, yPos, GetAnimation().GetRenderLayer(), GetAnimation().GetSpriteScale());
                }
            }
        }
        else
        {
            Sound(FleechSound::SleepingInhale_3);
        }

        mXPos = FP_FromInteger(mHoistX) + (Math_Cosine(mAngle) * FP_FromInteger(4)) - FP_FromInteger(mXOffset);
        mAngle += 2;
    }
}

void Fleech::Motion_18_Consume()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Sound(FleechSound::Digesting_2);
    }
    else if (GetAnimation().GetCurrentFrame() == 15 && mFoodObjId == gAbe->mBaseGameObjectId)
    {
        gAbe->SetAsDead_459430();

        Sound(FleechSound::Burp_1);

        for (s32 i = 0; i < 3; i++)
        {
            auto pDove = relive_new Dove(
                AnimId::Dove_Flying,
                mXPos,
                mYPos + FP_FromInteger(10),
                GetSpriteScale());

            if (GetAnimation().GetFlipX())
            {
                pDove->mXPos += (GetSpriteScale() * FP_FromInteger(15));
            }
            else
            {
                pDove->mXPos -= (GetSpriteScale() * FP_FromInteger(15));
            }

            pDove->SetSpriteScale(GetSpriteScale());
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

Fleech::~Fleech()
{
    mScrabOrParamite = Guid{};
    if (mTlvInfo != Guid{})
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, mHealth <= FP_FromInteger(0));
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    ResetTarget();

    if (mCurrentMotion == 18)
    {
        if (gAbe)
        {
            if (mFoodObjId == gAbe->mBaseGameObjectId)
            {
                gAbe->SetAsDead_459430();
            }
        }
    }
    mFoodObjId = Guid{};

    if (!mShrivelDeath)
    {
        sFleechCount_5BC20E--;
    }
}

void Fleech::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
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
        mFoodObjId = BaseGameObject::RefreshId(mFoodObjId);
        mScrabOrParamite = BaseGameObject::RefreshId(mScrabOrParamite);
        BaseAliveGameObject_PlatformId = BaseGameObject::RefreshId(BaseAliveGameObject_PlatformId);
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if ((FP_Abs(mXPos - sControlledCharacter->mXPos) <= FP_FromInteger(750) && FP_Abs(mYPos - sControlledCharacter->mYPos) <= FP_FromInteger(520)) || mPersistant)
    {
        const auto oldMotion = mCurrentMotion;

        mCurrentBrain->VUpdate();

        TongueUpdate();

        if (mYPos < FP_FromInteger(0))
        {
            SetDead(true);
        }

        const FP oldY = mYPos;
        const FP oldX = mXPos;

        InvokeMemberFunction(this, sFleechMotionTable, mCurrentMotion);

        if (oldX != mXPos || oldY != mYPos)
        {
            BaseAliveGameObjectPathTLV = gPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion == mCurrentMotion)
        {
            if (mReturnToPreviousMotion)
            {
                mCurrentMotion = mPreviousMotion;
                SetAnim();
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                mReturnToPreviousMotion = false;
            }
        }
        else
        {
            SetAnim();
        }
    }
    else
    {
        SetDead(true);
    }
}

void Fleech::VRender(OrderingTable& ot)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
        RenderEx(ot);
    }
}

void Fleech::RenderEx(OrderingTable& ot)
{
    if (mRenderTongue)
    {
        FP tongueBlock_X[5] = {};
        FP tongueBlock_Y[5] = {};

        const s16 camX = FP_GetExponent(gScreenManager->CamXPos());
        const s16 camY = FP_GetExponent(gScreenManager->CamYPos());

        tongueBlock_X[0] = FP_FromInteger(mTongueOriginX - camX);
        tongueBlock_Y[0] = FP_FromInteger(mTongueOriginY - camY);
        tongueBlock_X[4] = FP_FromInteger(mTongueDestinationX - camX);
        tongueBlock_Y[4] = FP_FromInteger(mTongueDestinationY - camY);

        const FP distanceX_squared = (tongueBlock_X[0] - tongueBlock_X[4]) * (tongueBlock_X[0] - tongueBlock_X[4]);
        const FP distanceY_squared = (tongueBlock_Y[0] - tongueBlock_Y[4]) * (tongueBlock_Y[0] - tongueBlock_Y[4]);
        const FP distanceXY_squareRoot = Math_SquareRoot_FP_Wrapper(distanceY_squared + distanceX_squared);
        const FP Tan_fp = Math_Tan(
            tongueBlock_Y[0] - tongueBlock_Y[4],
            tongueBlock_X[4] - tongueBlock_X[0]);
        const FP distanceCosine = Math_Cosine(static_cast<u8>(FP_GetExponent(Tan_fp)));
        const FP SineTan = Math_Sine(static_cast<u8>(FP_GetExponent(Tan_fp)));

        for (s32 i = 0; i < 4; i++)
        {
            const FP distanceXY_squareRoot_multiplied = distanceXY_squareRoot * FP_FromInteger(i + 1) * FP_FromDouble(0.25);
            const FP cosineIt_times_field188 = Math_Cosine(static_cast<u8>(32 * (i + 1))) * FP_FromInteger(field_188);
            tongueBlock_X[i + 1] = tongueBlock_X[0] + SineTan * distanceXY_squareRoot_multiplied - cosineIt_times_field188 * distanceCosine;
            tongueBlock_Y[i + 1] = tongueBlock_Y[0] + SineTan * cosineIt_times_field188 + distanceCosine * distanceXY_squareRoot_multiplied;
        }

        const FP lastTongueBlockModX = tongueBlock_X[4] - FP_FromInteger(mTongueDestinationX + 0xFFFF * camX);
        const FP lastTongueBlockModY = tongueBlock_Y[4] - FP_FromInteger(mTongueDestinationY + 0xFFFF * camY);
        for (s32 i = 0; i < 4; i++)
        {
            const FP lastTongueBlockModX_mult = lastTongueBlockModX * FP_FromInteger(i + 1);
            const FP lastTongueBlockModY_mult = lastTongueBlockModY * FP_FromInteger(i + 1);
            tongueBlock_X[i + 1] -= lastTongueBlockModX_mult * FP_FromDouble(0.25);
            tongueBlock_Y[i + 1] -= lastTongueBlockModY_mult * FP_FromDouble(0.25);
        }

        for (s32 i = 0; i < 4; i++)
        {
            s16 r = static_cast<s16>((i + 1) * 150 / 4);
            s16 g = static_cast<s16>((i + 1) * 100 / 4);
            s16 b = static_cast<s16>((i + 1) * 100 / 4);
            s16 r2 = static_cast<s16>(i * 150 / 4);
            s16 g2 = static_cast<s16>(i * 100 / 4);
            s16 b2 = static_cast<s16>(i * 100 / 4);

            const FP currTongueBlock_Y = tongueBlock_Y[i];
            const FP currTongueBlock_X = tongueBlock_X[i];

            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(currTongueBlock_X + FP_FromInteger(camX)),
                FP_GetExponent(currTongueBlock_Y + FP_FromInteger(camY)),
                GetScale(),
                &r,
                &g,
                &b);
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(currTongueBlock_X + FP_FromInteger(camX)),
                FP_GetExponent(currTongueBlock_Y + FP_FromInteger(camY)),
                GetScale(),
                &r2,
                &g2,
                &b2);

            Poly_G4* currTonguePoly1 = &mTonguePolys1[i];

            const s32 tonguePolyX1 = PsxToPCX(FP_GetExponent(currTongueBlock_X));
            const s32 tonguePolyY1 = FP_GetExponent(currTongueBlock_Y);
            const s32 tonguePolyX2 = PsxToPCX(FP_GetExponent(tongueBlock_X[i + 1]));
            const s32 tonguePolyY2 = FP_GetExponent(tongueBlock_Y[i + 1]);

            currTonguePoly1->SetXY0(
                static_cast<s16>(tonguePolyX1),
                static_cast<s16>(tonguePolyY1 - 1));
            currTonguePoly1->SetXY1(
                static_cast<s16>(tonguePolyX2),
                static_cast<s16>(tonguePolyY2 - 1));
            currTonguePoly1->SetXY2(
                static_cast<s16>(tonguePolyX1),
                static_cast<s16>(tonguePolyY1 + 1));
            currTonguePoly1->SetXY3(
                static_cast<s16>(tonguePolyX2),
                static_cast<s16>(tonguePolyY2 + 1));

            currTonguePoly1->SetRGB0(
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            currTonguePoly1->SetRGB1(
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            currTonguePoly1->SetRGB2(
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            currTonguePoly1->SetRGB3(
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            
            currTonguePoly1->SetBlendMode(relive::TBlendModes::eBlend_0);

            ot.Add(GetAnimation().GetRenderLayer(), currTonguePoly1);

            Poly_G4* currTonguePoly2 = &mTonguePolys2[i];

            s32 minus_one_one_switch = -1;
            if (FP_GetExponent(Tan_fp) <= 64 || FP_GetExponent(Tan_fp) >= 192)
            {
                minus_one_one_switch = 1;
            }

            currTonguePoly2->SetXY0(
                static_cast<s16>(tonguePolyX1 - minus_one_one_switch),
                static_cast<s16>(tonguePolyY1 - 1));
            currTonguePoly2->SetXY1(
                static_cast<s16>(tonguePolyX2 - minus_one_one_switch),
                static_cast<s16>(tonguePolyY2 - 1));
            currTonguePoly2->SetXY2(
                static_cast<s16>(tonguePolyX1 + minus_one_one_switch),
                static_cast<s16>(tonguePolyY1 + 1));
            currTonguePoly2->SetXY3(
                static_cast<s16>(tonguePolyX2 + minus_one_one_switch),
                static_cast<s16>(tonguePolyY2 + 1));

            currTonguePoly2->SetRGB0(
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            currTonguePoly2->SetRGB1(
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            currTonguePoly2->SetRGB2(
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            currTonguePoly2->SetRGB3(
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));

            currTonguePoly2->SetBlendMode(relive::TBlendModes::eBlend_0);

            ot.Add(GetAnimation().GetRenderLayer(), currTonguePoly2);
        }

        return;
    }
}

void Fleech::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
        mFoodObjId = Guid{};
        mScrabOrParamite = Guid{};
    }
}

void Fleech::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            mHealth = FP_FromInteger(0);
            SetDead(true);
        }
        pTlv = gPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

s16 Fleech::IsScrabOrParamiteNear(FP radius)
{
    BaseAliveGameObject* pNearestScrabOrParamite = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pBaseObj = gBaseGameObjects->ItemAt(i);
        if (!pBaseObj)
        {
            break;
        }

        if (pBaseObj->GetIsBaseAliveGameObject())
        {
            auto pObj = static_cast<BaseAliveGameObject*>(pBaseObj);
            if ((pObj->Type() == ReliveTypes::eScrab || pObj->Type() == ReliveTypes::eParamite) && pObj->mHealth > FP_FromInteger(0))
            {
                if (pObj->GetSpriteScale() == GetSpriteScale())
                {
                    bool check = false;
                    if (mYPos >= pObj->mYPos)
                    {
                        if (mYPos - pObj->mYPos > (FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 20 : 10) * FP_FromInteger(2)))
                        {
                            if (mYPos < pObj->mYPos)
                            {
                                if (pObj->mYPos - mYPos <= (FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 2 : 1)))
                                {
                                    check = true;
                                }
                            }
                        }
                        else
                        {
                            check = true;
                        }
                    }
                    else
                    {
                        if (pObj->mYPos - mYPos <= (FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? +2 : 1)))
                        {
                            check = true;
                        }
                    }

                    if (check)
                    {
                        if (VIsObjNearby(radius, pObj))
                        {
                            if (!WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pObj->mXPos - mXPos))
                            {
                                if (!pNearestScrabOrParamite)
                                {
                                    pNearestScrabOrParamite = pObj;
                                }
                                else if (FP_GetExponent(FP_Abs(pObj->mXPos - mXPos)) < FP_GetExponent(FP_Abs(pNearestScrabOrParamite->mXPos - mXPos)))
                                {
                                    pNearestScrabOrParamite = pObj;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (pNearestScrabOrParamite)
    {
        mScrabOrParamite = pNearestScrabOrParamite->mBaseGameObjectId;
        return 1;
    }

    mScrabOrParamite = Guid{};
    return 0;
}

const TintEntry kFleechTints_551844[16] = {
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

void Animation_OnFrame_Fleech(BaseGameObject* pObj, u32&, const IndexedPoint& point)
{
    reinterpret_cast<Fleech*>(pObj)->VOnFrame(point.mPoint);
}

void Fleech::Init()
{
    Animation_Init(GetAnimRes(AnimId::Fleech_Idle));

    GetAnimation().SetFnPtrArray(gFleech_Anim_Frame_Fns);

    SetType(ReliveTypes::eFleech);

    SetCanExplode(true);
    SetCanSetOffExplosives(true);

    mShrivelDeath = false;
    mScaredSound = false;

    SetDoPurpleLightEffect(true);

    field_12C_shrivel_timer = 0;
    SetNextMotion(eFleechMotions::eNone_m1);
    BaseAliveGameObject_PlatformId = Guid{};
    mReturnToPreviousMotion = false;
    mFoodObjId = Guid{};
    mScrabOrParamite = Guid{};
    field_15E_lost_target_timer = 0;

    SetTint(&kFleechTints_551844[0], gMap.mCurrentLevel);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_SlogFleech_34);
        SetScale(Scale::Fg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_SlogFleech_Half_15);
        SetScale(Scale::Bg);
    }

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    MapFollowMe(true);

    VStackOnObjectsOfType(ReliveTypes::eFleech);

    CreateShadow();

    sFleechCount_5BC20E++;
}

void Fleech::InitTonguePolys()
{
    mTongueActive = false;
    mRenderTongue = false;

    mTongueOriginX = FP_GetExponent(mXPos);
    mTongueOriginY = FP_GetExponent(FP_FromInteger(2) * ((FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? -10 : -5)) + mYPos));

    mTongueState = 1;

    mTongueDestinationX = -1;
    mTongueDestinationY = -1;

    for (s32 i = 0; i < 4; i++)
    {
        mTonguePolys1[i].SetRGB0(150, 100, 100);
        mTonguePolys1[i].SetRGB1(150, 100, 100);
        mTonguePolys1[i].SetRGB2(150, 100, 100);
        mTonguePolys1[i].SetRGB3(150, 100, 100);
        mTonguePolys1[i].SetSemiTransparent(false);

        mTonguePolys2[i].SetRGB0(150, 100, 100);
        mTonguePolys2[i].SetRGB1(150, 100, 100);
        mTonguePolys2[i].SetRGB2(150, 100, 100);
        mTonguePolys2[i].SetRGB3(150, 100, 100);
        mTonguePolys2[i].SetSemiTransparent(true);
    }
}

void Fleech::SetAnim()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sFleechAnimFromMotion[mCurrentMotion]));
}

void Fleech::ResetTarget()
{
    if (current_target_object_id_551840 == mBaseGameObjectId)
    {
        current_target_object_id_551840 = Guid{};
    }
}

s16 Fleech::GotNoTarget()
{
    return current_target_object_id_551840 == Guid{} && !GetElectrocuted();
}

void Fleech::SetTarget()
{
    current_target_object_id_551840 = mBaseGameObjectId;
}

void Fleech::TongueHangingFromWall(s16 target_x, s16 target_y)
{
    mRenderTongue = true;
    mTongueState = 2;
    mTongueDestinationY = target_y;
    mTongueDestinationX = target_x;
    field_188 = 0;
}

void Fleech::TongueUpdate()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFoodObjId));
    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        mRenderTongue = false;
    }

    switch (mTongueState)
    {
        case 1:
            mTongueActive = false;
            mRenderTongue = false;
            return;

        case 2:
        case 4:
            mRenderTongue = true;
            return;

        case 3:
            mRenderTongue = true;
            switch (mTongueSubState++)
            {
                case 0:
                    mTongueDestinationX = mHoistX;
                    mTongueDestinationY = mHoistY;
                    Sound(FleechSound::LedgeHoist_11);
                    break;

                case 1:
                    field_188 = 6;
                    break;

                case 2:
                    field_188 = -6;
                    break;

                case 3:
                    field_188 = 3;
                    break;

                case 4:
                    field_188 = -3;
                    break;

                case 5:
                    field_188 = 0;
                    mTongueState = 4;
                    mTongueActive = false;
                    break;
                default:
                    break;
            }
            return;

        case 5:
        case 10:
            mTongueState = 1;
            return;

        case 6:
            if (pTarget)
            {
                const PSX_RECT bRect = pTarget->VGetBoundingRect();

                mRenderTongue = true;
                mEnemyXPos = FP_GetExponent(pTarget->mXPos);
                mEnemyYPos = (bRect.y + bRect.h) >> 1;
                mTongueDestinationY = (bRect.y + bRect.h) >> 1;
                mTongueDestinationX = mEnemyXPos;

                const FP v12 = (FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 20 : 10) * FP_FromInteger(7));
                if (FP_FromInteger(Math_Distance(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        mTongueDestinationX, mTongueDestinationY))
                    <= v12)
                {
                    switch (mTongueSubState++)
                    {
                        case 0:
                        {
                            Sound(FleechSound::LickTarget_10);
                            relive_new Blood(
                                FP_FromInteger(mEnemyXPos),
                                FP_FromInteger(mEnemyYPos),
                                GetAnimation().GetFlipX() != 0 ? FP_FromInteger(2) : FP_FromInteger(-2),
                                FP_FromInteger(1),
                                GetSpriteScale(), 20);
                            break;
                        }

                        case 1:
                            field_188 = 9;
                            break;

                        case 2:
                            field_188 = -6;
                            ResetTarget();
                            break;

                        case 3:
                            field_188 = 5;
                            break;

                        case 4:
                            field_188 = -3;
                            break;

                        case 5:
                            mTongueState = 9;
                            field_188 = 0;
                            if (pTarget->mHealth > FP_FromInteger(0))
                            {
                                pTarget->VTakeDamage(this);
                                if (pTarget->mHealth <= FP_FromInteger(0))
                                {
                                    mTongueState = 7;
                                    mTongueSubState = 0;
                                    Sound(FleechSound::Devour_8);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    mTongueActive = false;
                    mTongueState = 1;
                }
            }
            else
            {
                sub_42B8C0();
            }
            return;

        case 7:
            if (pTarget)
            {
                const PSX_RECT bRect = pTarget->VGetBoundingRect();

                switch (mTongueSubState++)
                {
                    case 4:
                        SetNextMotion(eFleechMotions::Motion_18_Consume);
                        [[fallthrough]];
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 5:
                        if (mTongueSubState == 5 && pTarget->Type() == ReliveTypes::eScrab)
                        {
                            pTarget->GetAnimation().SetRender(false);
                        }
                        pTarget->mXPos -= (pTarget->mXPos - mXPos) * FP_FromDouble(0.25);
                        pTarget->mYPos -= (pTarget->mYPos - mYPos) * FP_FromDouble(0.25);
                        break;

                    case 6:
                        mRenderTongue = false;
                        mTongueState = 8;
                        pTarget->GetAnimation().SetRender(false);
                        pTarget->mXPos = mXPos;
                        pTarget->mYPos = mYPos;
                        if (IgAbe(pTarget))
                        {
                            gAbe->FleechDeath_459350();
                        }
                        break;

                    default:
                        break;
                }

                mEnemyXPos = FP_GetExponent(pTarget->mXPos);
                mTongueDestinationX = mEnemyXPos;
                mEnemyYPos = (bRect.y + bRect.h) >> 1;
                mTongueDestinationY = (bRect.y + bRect.h) >> 1;
            }
            else
            {
                sub_42B8C0();
            }
            return;

        case 8:
            mTongueActive = false;
            mTongueState = 1;
            return;

        case 9:
            if (pTarget)
            {
                const PSX_RECT bRect = pTarget->VGetBoundingRect();
                mEnemyXPos = FP_GetExponent((mXPos + pTarget->mXPos) * FP_FromDouble(0.5));
                mEnemyYPos = (bRect.y + bRect.h) >> 1;
                mTongueDestinationX = FP_GetExponent((mXPos + pTarget->mXPos) * FP_FromDouble(0.5));
                mTongueDestinationY = (bRect.y + bRect.h) >> 1;
                mTongueActive = false;
                mTongueState = 1;
            }
            else
            {
                sub_42B8C0();
            }
            break;

        default:
            break;
    }
}

void Fleech::ToIdle()
{
    MapFollowMe(true);
    field_138_velx_factor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetCurrentMotion(eFleechMotions::Motion_3_Idle);
    SetNextMotion(eFleechMotions::eNone_m1);
    sFleechRandomIdx_5BC20C++; // NOTE: unused variable removed - don't desync on RNG
}

const relive::SfxDefinition& getSfxDef(FleechSound effectId)
{
    return stru_5518E0[static_cast<s32>(effectId)];
}

static const relive::SfxDefinition& GetRandomizedSound(FleechSound soundId, s16& defaultSndIdxVol)
{
    if (soundId == FleechSound::CrawlRNG1_14)
    {
        const s32 rndIdx = Math_RandomRange(14, 16);
        const relive::SfxDefinition& effectDef = getSfxDef(static_cast<FleechSound>(rndIdx));
        defaultSndIdxVol = effectDef.mDefaultVolume + Math_RandomRange(-10, 10);
        return effectDef;
    }
    else
    {
        const relive::SfxDefinition& effectDef = getSfxDef(soundId);
        defaultSndIdxVol = effectDef.mDefaultVolume;
        return effectDef;
    }
}

s32 Fleech::Sound(FleechSound soundId)
{
    s16 defaultSndIdxVol = 0;
    const relive::SfxDefinition& effectDef = GetRandomizedSound(soundId, defaultSndIdxVol);

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        defaultSndIdxVol = 2 * defaultSndIdxVol / 3;
    }

    s16 volumeLeft = 0;
    s16 volumeRight = defaultSndIdxVol;
    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect(direction, &pRect);
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volumeLeft = volumeRight;
            break;
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            const FP percentHowFar = (FP_FromInteger(pRect.w) - mXPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
        break;
        case CameraPos::eCamRight_4:
        {
            const FP percentHowFar = (mXPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
        break;
        default:
            return 0;
    }

    return SFX_SfxDefinition_Play_Stereo(
        effectDef,
        volumeLeft,
        volumeRight,
        effectDef.mPitchMin,
        effectDef.mPitchMax);
}

u8** Fleech::ResBlockForMotion(s32 /*motion*/)
{
    return mBaseGameObjectResArray.ItemAt(0);
}

s16 Fleech::CanMove()
{
    if (GetNextMotion() == eFleechMotions::Motion_6_Knockback)
    {
        SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        SetNextMotion(eFleechMotions::eNone_m1);
        return 1;
    }

    if (GetNextMotion() != eFleechMotions::Motion_4_Crawl)
    {
        return 0;
    }

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
    }

    const FP yDist = FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5);
    const FP xDist = ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(GetAnimation().GetFlipX() ? -1 : 1); // TODO: Correct way around ?
    if (!WallHit(yDist, xDist) && !HandleEnemyStopperOrSlamDoor(1))
    {
        SetCurrentMotion(eFleechMotions::Motion_4_Crawl);
        SetNextMotion(eFleechMotions::eNone_m1);
        return 1;
    }

    ToIdle();
    return 0;
}

s16 Fleech::HandleEnemyStopperOrSlamDoor(s32 velX)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    FP nextXPos = {};
    if (GetAnimation().GetFlipX())
    {
        nextXPos = mXPos - (kGridSize * FP_FromInteger(velX));
    }
    else
    {
        nextXPos = (kGridSize * FP_FromInteger(velX)) + mXPos;
    }

    FP stopperXPos = {};
    if (mXPos <= nextXPos)
    {
        stopperXPos = nextXPos;
    }
    else
    {
        stopperXPos = mXPos;
    }

    auto pStopper = static_cast<relive::Path_EnemyStopper*>(gPathInfo->TLV_Get_At(
        FP_GetExponent(stopperXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(stopperXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eEnemyStopper));

    if (pStopper && (pStopper->mStopDirection == (nextXPos >= mXPos ? relive::Path_EnemyStopper::StopDirection::Right : relive::Path_EnemyStopper::StopDirection::Left)) && SwitchStates_Get(pStopper->mSwitchId))
    {
        return 1;
    }

    FP slamDoorXPos = mXPos;
    if (mXPos <= nextXPos)
    {
        slamDoorXPos = nextXPos;
    }

    if (mXPos > nextXPos)
    {
        slamDoorXPos = nextXPos;
    }

    auto pSlamDoor = static_cast<relive::Path_SlamDoor*>(gPathInfo->TLV_Get_At(
        FP_GetExponent(slamDoorXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(slamDoorXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eSlamDoor));

    return (pSlamDoor && ((pSlamDoor->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoor->mSwitchId)) || (pSlamDoor->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoor->mSwitchId))));
}

s32 Fleech::UpdateWakeUpSwitchValue()
{
    const s16 curSwitchValue = static_cast<s16>(SwitchStates_Get(mWakeUpSwitchId));
    const s16 wakeUpValue = mWakeUpSwitchValue;

    if (curSwitchValue == wakeUpValue)
    {
        return 0;
    }

    if (curSwitchValue)
    {
        mWakeUpSwitchValue = curSwitchValue;
        return (curSwitchValue - wakeUpValue) ? 1 : 0;
    }
    else
    {
        mWakeUpSwitchValue = 0;
        return 1;
    }
}

bool Fleech::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return false;
    }

    sub_42B8C0();
    ResetTarget();

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eSlig:
        {
            Sound(FleechSound::Scared_7);
            mHealth = FP_FromInteger(0);

            relive_new Gibs(GibType::Fleech_10, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(), 50);

            SetDead(true);
        }
        break;


        case ReliveTypes::eRockSpawner:
            Sound(FleechSound::Scared_7);
            mHealth = FP_FromInteger(0);
            SetBrain(IFleechBrain::EBrainTypes::Death);
            SetNextMotion(eFleechMotions::eNone_m1);
            field_12C_shrivel_timer = MakeTimer(127);
            SetCurrentMotion(eFleechMotions::Motion_3_Idle);
            SetAnim();
            GetAnimation().SetAnimate(true);
            break;

        case ReliveTypes::eParamite:
            Sound(FleechSound::Dismember_13);
            [[fallthrough]];

        case ReliveTypes::eScrab:
        {
            relive_new Gibs(GibType::Fleech_10, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);

            if (static_cast<BaseAliveGameObject*>(pFrom)->GetAnimation().GetFlipX())
            {
                relive_new Blood(mXPos, mYPos - FP_FromInteger(8), -FP_FromInteger(5), -FP_FromInteger(5), GetSpriteScale(), 50);
            }
            else
            {
                relive_new Blood(mXPos, mYPos - FP_FromInteger(8), FP_FromInteger(5), -FP_FromInteger(5), GetSpriteScale(), 50);
            }

            if (!mScaredSound)
            {
                mScaredSound = true;
                Sound(FleechSound::Scared_7);
            }

            mHealth = FP_FromInteger(0);
            SetBrain(IFleechBrain::EBrainTypes::Death);
            SetCurrentMotion(eFleechMotions::Motion_3_Idle);
            field_12C_shrivel_timer = MakeTimer(127);
            SetNextMotion(eFleechMotions::eNone_m1);
            SetAnim();
            GetAnimation().SetAnimate(true);
            mShrivelDeath = true;
            SetDead(true);
            sFleechCount_5BC20E--;
        }
        break;

        case ReliveTypes::eElectrocute:
            SetDead(true);
            mHealth = FP_FromInteger(0);
            SetBrain(IFleechBrain::EBrainTypes::Death);
            break;

        default:
            Sound(FleechSound::Scared_7);
            SetElectrocuted(true);
            sub_42B8C0();
            break;
    }
    return true;
}

void Fleech::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }
}

void Fleech::SetTongueState1()
{
    mTongueState = 1;
}

void Fleech::IncreaseAnger()
{
    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        BaseAnimatedWithPhysicsGameObject* pEvent = IsEventInRange(kEventSpeaking, mXPos, mYPos, AsEventScale(GetScale()));

        if (!pEvent)
        {
            pEvent = IsEventInRange(kEventAlarm, mXPos, mYPos, AsEventScale(GetScale()));
        }

        if (!pEvent)
        {
            pEvent = IsEventInRange(kEventLoudNoise, mXPos, mYPos, AsEventScale(GetScale()));
        }

        if (pEvent)
        {
            if ((!IgAbe(pEvent) || !gAbe->GetInvisible()) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, pEvent->mXPos, pEvent->mYPos, 0))
            {
                mCurrentAnger += mAttackAngerIncreaser;
                if (VOnSameYLevel(static_cast<BaseAnimatedWithPhysicsGameObject*>(pEvent)))
                {
                    if (pEvent->Type() == ReliveTypes::eScrab || pEvent->Type() == ReliveTypes::eParamite)
                    {
                        field_14E_ScrabParamiteEventXPos = FP_GetExponent(pEvent->mXPos);
                    }
                    else
                    {
                        field_14C_EventXPos = FP_GetExponent(pEvent->mXPos);
                    }
                }
            }
        }

        pEvent = IsEventInRange(kEventNoise, mXPos, mYPos, AsEventScale(GetScale()));
        if (pEvent)
        {
            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(6),static_cast<BaseAnimatedWithPhysicsGameObject*>(pEvent)))
            {
                if ((!IgAbe(pEvent) || !gAbe->GetInvisible()) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, pEvent->mXPos, pEvent->mYPos, 0))
                {
                    mCurrentAnger += mMaxAnger;
                }
            }
        }
    }
}

s16 Fleech::AngerFleech(IBaseAliveGameObject* pObj)
{
    if (!pObj || (IgAbe(pObj) && gAbe->GetInvisible()))
    {
        return false;
    }

    if (FP_Abs(pObj->mXPos - mXPos) >= ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(10) ||
        FP_Abs(pObj->mYPos - mYPos) >= ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(1) ||
        pObj->GetSpriteScale() != GetSpriteScale())
    {
        return false;
    }

    return true;
}

s32 Fleech::TongueActive()
{
    return mTongueActive;
}

void Fleech::PullTargetIn()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFoodObjId));
    if (pTarget)
    {
        mTongueActive = true;
        mRenderTongue = true;
        mTongueState = 6;
        mTongueSubState = 0;

        const PSX_RECT bRect = pTarget->VGetBoundingRect();

        mEnemyXPos = FP_GetExponent(pTarget->mXPos);
        mEnemyYPos = (bRect.y + bRect.h) / 2;
        mTongueDestinationX = FP_GetExponent(((FP_FromInteger(mEnemyXPos) + mXPos) / FP_FromInteger(2)));
        field_188 = 0;
        mTongueDestinationY = FP_GetExponent(((FP_FromInteger(mEnemyYPos) + mYPos) / FP_FromInteger(2)));
    }
}

void Fleech::sub_42B8C0()
{
    ResetTarget();

    if (mTongueState > 1)
    {
        mTongueState = 10;
        mTongueDestinationX = FP_GetExponent(((FP_FromInteger(mTongueDestinationX)) + mXPos) / FP_FromInteger(2));
        field_188 = 0;
        mTongueDestinationY = FP_GetExponent(((FP_FromInteger(mTongueDestinationY)) + mYPos) / FP_FromInteger(2));
    }
    else
    {
        mRenderTongue = false;
        mTongueState = 1;
    }
}

void Fleech::sub_42BA10()
{
    mTongueActive = true;
    mRenderTongue = true;
    mTongueState = 3;
    mTongueSubState = 0;
    mTongueDestinationX = FP_GetExponent(((FP_FromInteger(mHoistX)) + mXPos) / FP_FromInteger(2));
    field_188 = 0;
    mTongueDestinationY = FP_GetExponent(((FP_FromInteger(mHoistY)) + mYPos) / FP_FromInteger(2));
}

void Fleech::SetTongueState5()
{
    mTongueState = 5;
}

IBaseAliveGameObject* Fleech::FindMudOrAbe()
{
    IBaseAliveGameObject* pRet = nullptr;
    FP lastDist = FP_FromInteger(gPsxDisplay.mWidth);
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if ((pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eAbe) && pObj->GetScale() == GetScale() && pObj->mHealth > FP_FromInteger(0))
        {
            const FP dist = FP_FromInteger(
                Math_Distance(
                    FP_GetExponent(pObj->mXPos),
                    FP_GetExponent(pObj->mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos)));

            if (dist < lastDist && FP_GetExponent(mXPos) / 375 == (FP_GetExponent(pObj->mXPos) / 375) && FP_GetExponent(mYPos) / 260 == (FP_GetExponent(pObj->mYPos) / 260))
            {
                lastDist = dist;
                pRet = pObj;
            }
        }
    }
    return pRet;
}

void Fleech::MoveAlongFloor()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    const FP prev_xpos = mXPos;
    const FP prev_ypos = mYPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        PathLine* pOldLine = BaseAliveGameObjectCollisionLine;
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine && (GetScale() == Scale::Fg ? kFgFloor : kBgFloor).Mask() == CollisionMask(BaseAliveGameObjectCollisionLine->mLineType).Mask())
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
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                OnCollisionWith(xy, wh, gPlatformsArray);
            }
        }
        else if (!BrainIs(IFleechBrain::EBrainTypes::Patrol))
        {
            VOnTrapDoorOpen();
            field_138_velx_factor = FP_FromInteger(0);
            BaseAliveGameObjectLastLineYPos = mYPos;
            mXPos = prev_xpos + mVelX;
            SetCurrentMotion(eFleechMotions::Motion_9_Fall);
        }
        else
        {
            mXPos = prev_xpos;
            mYPos = prev_ypos;
            BaseAliveGameObjectCollisionLine = pOldLine;
            SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = prev_ypos;
        field_138_velx_factor = FP_FromInteger(0);
        SetCurrentMotion(eFleechMotions::Motion_9_Fall);
    }
}

// attack radius?
s16 Fleech::IsNear(IBaseAliveGameObject* pObj)
{
    if (pObj && GetSpriteScale() == pObj->GetSpriteScale() &&
        FP_GetExponent(FP_Abs(mXPos - pObj->mXPos)) <= 750 &&
        FP_GetExponent(FP_Abs(mYPos - pObj->mYPos)) <= 260)
    {
        return true;
    }
    return false;
}

void Fleech::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mCurrentAnger += mAttackAngerIncreaser;
}

bool Fleech::Collision(s16 alwaysOne)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP quaterScaled = (kGridSize * FP_FromDouble(0.25));

    FP x2 = {};
    if (GetAnimation().GetFlipX())
    {
        x2 = mXPos - (quaterScaled * FP_FromInteger(alwaysOne >= 0 ? 1 : -1));
    }
    else
    {
        x2 = (kGridSize * FP_FromInteger(alwaysOne)) + mXPos;
    }

    FP x1 = {};
    if (GetAnimation().GetFlipX())
    {
        x1 = mXPos - (kGridSize * FP_FromInteger(alwaysOne));
    }
    else
    {
        x1 = (quaterScaled * FP_FromInteger(alwaysOne >= 0 ? 1 : -1)) + mXPos;
    }

    const FP y2 = mYPos + FP_FromInteger(2);
    const FP y1 = mYPos - FP_FromInteger(2);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return gCollisions->Raycast(x1, y1, x2, y2, &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor) == 0;
}

relive::Path_Hoist* Fleech::TryGetHoist(s32 xDistance, s16 bIgnoreDirection)
{
    if (GetCurrentMotion() == eFleechMotions::Motion_9_Fall)
    {
        return nullptr;
    }

    const FP yAmount = FP_FromInteger(GetSpriteScale() < FP_FromInteger(1) ? 10 : 20);
    const FP y1 = mYPos - yAmount;
    const FP y2 = y1 - (yAmount * FP_FromInteger(1));

    const FP xSnapped = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)));
    FP xCheck = {};
    if (GetAnimation().GetFlipX())
    {
        xCheck = xSnapped - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(xDistance));
    }
    else
    {
        xCheck = (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(xDistance)) + xSnapped;
    }

    auto pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->TLV_Get_At(
        FP_GetExponent(std::min(xCheck, mXPos)),
        FP_GetExponent(y2),
        FP_GetExponent(std::max(xCheck, mXPos)),
        FP_GetExponent(y1),
        ReliveTypes::eHoist));

    if (!pHoist)
    {
        return nullptr;
    }

    if (WallHit(
            FP_FromInteger(GetSpriteScale() < FP_FromInteger(1) ? 5 : 10),
            FP_FromInteger(pHoist->mTopLeftX + (GetSpriteScale() < FP_FromInteger(1) ? 6 : 12)) - mXPos))
    {
        return nullptr;
    }

    if (HandleEnemyStopperOrSlamDoor(xDistance))
    {
        return nullptr;
    }

    if (pHoist->mScale != (GetSpriteScale() < FP_FromInteger(1) ? relive::reliveScale::eHalf : relive::reliveScale::eFull) || mYPos - FP_FromInteger(pHoist->mTopLeftY) > FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 20 : 10) * FP_FromDouble(5.5))
    {
        return nullptr;
    }

    if (bIgnoreDirection)
    {
        return pHoist;
    }

    if (pHoist->mGrabDirection == (GetAnimation().GetFlipX() ? relive::Path_Hoist::GrabDirection::eFacingLeft : relive::Path_Hoist::GrabDirection::eFacingRight) || pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingAnyDirection)
    {
        return pHoist;
    }

    return nullptr;
}

void Fleech::VOnFrame(const Point32& point)
{
    FP xpos = {};
    if (GetAnimation().GetFlipX())
    {
        xpos = mXPos - (GetSpriteScale() * FP_FromInteger(point.x));
    }
    else
    {
        xpos = (GetSpriteScale() * FP_FromInteger(point.x)) + mXPos;
    }

    mTongueOriginX = FP_GetExponent(xpos) + mXOffset;
    mTongueOriginY = FP_GetExponent((GetSpriteScale() * FP_FromInteger(point.y)) + mYPos);
}

void Fleech::SetBrain(IFleechBrain::EBrainTypes brain)
{
    switch (brain)
    {
        case IFleechBrain::EBrainTypes::Patrol:
            mCurrentBrain = &mPatrolBrain;
            break;
        case IFleechBrain::EBrainTypes::ChasingAbe:
            mCurrentBrain = &mChasingAbeBrain;
            break;
        case IFleechBrain::EBrainTypes::Scared:
            mCurrentBrain = &mScaredBrain;
            break;
        case IFleechBrain::EBrainTypes::Death:
            mCurrentBrain = &mDeathBrain;
            break;
        default:
            ALIVE_FATAL("Invalid fleech brain type %d", static_cast<s32>(brain));
    }
}

bool Fleech::BrainIs(IFleechBrain::EBrainTypes brain)
{
    return mCurrentBrain->VGetBrain() == brain;
}

static const MusicController::MusicTypes sPatrolBrainMusicTypes[] = {
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eNone_0};


void PatrolBrain::VUpdate()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mFoodObjId));
    if (!pTarget || pTarget->GetDead() || pTarget->mHealth <= FP_FromInteger(0) || pTarget->GetInvisible())
    {
        mFleech.mFoodObjId = Guid{};
        pTarget = nullptr;
    }

    if (gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0))
    {
        MusicController::static_PlayMusic(sPatrolBrainMusicTypes[mBrainState], &mFleech, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mFleech, 0, 0);
    }

    switch (mBrainState)
    {
        case EState::eInit:
            mBrainState = Brain_Patrol_State_0();
            return;

        case EState::eSleeping:
            mBrainState = Brain_Patrol_State_1();
            return;

        case EState::eWakingUp:
            mBrainState = Brain_Patrol_State_2();
            return;

        case EState::eGoingBackToSleep:
            mBrainState = Brain_Patrol_State_3();
            return;

        case EState::eAlerted:
            mBrainState = Brain_Patrol_State_4(pTarget);
            return;

        case EState::eHearingScrabOrParamite:
            mBrainState = Brain_Patrol_State_5();
            return;

        case EState::eDangerNearby:
            mBrainState = Brain_Patrol_State_6();
            return;

        case EState::eBackToAlerted:
            mBrainState = Brain_Patrol_State_7();
            return;

        case EState::eAlertedByAbe:
            mBrainState = Brain_Patrol_State_8(pTarget);
            return;

        case EState::eNearHoist:
            mBrainState = Brain_Patrol_State_9();
            return;

        case EState::eClimbHoist:
            mBrainState = Brain_Patrol_State_10();
            return;

        default:
            return;
    }
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_0()
{
    mFleech.field_156_rnd_crawl = Fleech_NextRandom() & 0x3F;
    mFleech.field_15A_chase_timer = 0;
    mFleech.field_152_old_xpos = FP_GetExponent(mFleech.mXPos);
    mFleech.field_14C_EventXPos = -1;
    mFleech.field_14E_ScrabParamiteEventXPos = -1;

    if (mFleech.mCurrentAnger > mFleech.mMaxAnger)
    {
        return EState::eAlerted;
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_0_Sleeping)
    {
        return EState::eSleeping;
    }

    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_17_SleepingWithTongue && !mFleech.mGoesToSleep)
    {
        return EState::eAlerted;
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_0_Sleeping || mFleech.GetCurrentMotion() == eFleechMotions::Motion_17_SleepingWithTongue)
    {
        return EState::eSleeping;
    }

    return EState::eGoingBackToSleep;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_1()
{
    if (!SwitchStates_Get(mFleech.mCanWakeUpSwitchId))
    {
        return mBrainState;
    }

    if (!(static_cast<s32>(sGnFrame) % 16))
    {
        if (mFleech.mCurrentAnger > 0)
        {
            mFleech.mCurrentAnger--;
        }
    }

    mFleech.IncreaseAnger();

    if (mFleech.UpdateWakeUpSwitchValue())
    {
        const s16 v11 = (mFleech.mAttackAngerIncreaser - mFleech.mMaxAnger) / 2;
        if (mFleech.mCurrentAnger < v11 + mFleech.mMaxAnger)
        {
            mFleech.mCurrentAnger = mFleech.mMaxAnger + v11;
        }
    }

    if (!mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(8)))
    {
        if (mFleech.mCurrentAnger <= mFleech.mMaxAnger)
        {
            return mBrainState;
        }

        if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_17_SleepingWithTongue)
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_9_Fall);
            mFleech.BaseAliveGameObjectLastLineYPos = mFleech.mYPos;
            return EState::eAlerted;
        }
    }
    else
    {
        BaseGameObject* pDangerObj = sObjectIds.Find_Impl(mFleech.mScrabOrParamite);
        if (pDangerObj && pDangerObj->Type() != ReliveTypes::eParamite)
        {
            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_17_SleepingWithTongue)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_9_Fall);
                mFleech.BaseAliveGameObjectLastLineYPos = mFleech.mYPos;
                return EState::eAlerted;
            }
        }
        else
        {
            mFleech.mScrabOrParamite = Guid{};
            if (mFleech.mCurrentAnger <= mFleech.mMaxAnger)
            {
                return mBrainState;
            }

            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_17_SleepingWithTongue)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_9_Fall);
                mFleech.BaseAliveGameObjectLastLineYPos = mFleech.mYPos;
                return EState::eAlerted;
            }
        }
    }
    mFleech.SetNextMotion(eFleechMotions::Motion_1_WakingUp);
    return EState::eWakingUp;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_2()
{
    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_1_WakingUp)
    {
        return mBrainState;
    }

    if (mFleech.BaseAliveGameObjectCollisionLine)
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_5_PatrolCry);
    }
    else
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_9_Fall);
        mFleech.BaseAliveGameObjectLastLineYPos = mFleech.mYPos;
    }
    return EState::eAlerted;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_3()
{
    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_2_Unknown);
    }

    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_2_Unknown)
    {
        return mBrainState;
    }

    mFleech.mCurrentAnger = 0;
    mFleech.SetNextMotion(eFleechMotions::Motion_0_Sleeping);
    return EState::eSleeping;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_4(IBaseAliveGameObject* pTarget)
{
    if (mFleech.mFoodObjId == Guid{})
    {
        pTarget = mFleech.FindMudOrAbe();
        if (pTarget)
        {
            mFleech.mFoodObjId = pTarget->mBaseGameObjectId;
        }
    }

    if (!(static_cast<s32>(sGnFrame) % 32))
    {
        if (mFleech.mCurrentAnger > 0)
        {
            mFleech.mCurrentAnger--;
        }
    }

    if (mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(8)))
    {
        auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
        if (pDangerObj == sControlledCharacter)
        {
            mFleech.SetBrain(EBrainTypes::Scared);
            mFleech.mScaredBrain.SetState(ScaredBrain::EState::eScared_0);
            return mBrainState; // TODO: check if this should be 0 if the playback desyncs
        }

        if (mFleech.VIsObjNearby(FP_FromInteger(2) * ScaleToGridSize(mFleech.GetSpriteScale()), pDangerObj))
        {
            const s16 v27 = (mFleech.mAttackAngerIncreaser - mFleech.mMaxAnger) / 2;
            if (mFleech.mCurrentAnger < v27 + mFleech.mMaxAnger)
            {
                mFleech.mCurrentAnger = mFleech.mMaxAnger + v27;
            }
            mFleech.field_14E_ScrabParamiteEventXPos = FP_GetExponent(pDangerObj->mXPos); // TODO: abs ?
        }
    }

    // TODO: Check OFSUB branches
    if (mFleech.field_14E_ScrabParamiteEventXPos >= 0)
    {
        if ((FP_FromInteger(mFleech.field_14E_ScrabParamiteEventXPos) > mFleech.mXPos && !mFleech.GetAnimation().GetFlipX()) || (FP_FromInteger(mFleech.field_14E_ScrabParamiteEventXPos) < mFleech.mXPos && mFleech.GetAnimation().GetFlipX()))
        {
            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_7_StopCrawling);
            }
            else if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
            }
        }
        return EState::eHearingScrabOrParamite;
    }

    mFleech.IncreaseAnger();

    if (mFleech.AngerFleech(pTarget))
    {
        mFleech.mCurrentAnger += mFleech.mAttackAngerIncreaser;
    }

    if (mFleech.UpdateWakeUpSwitchValue())
    {
        mFleech.mCurrentAnger += mFleech.mWakeUpSwitchAngerValue;
    }

    if (pTarget)
    {
        if (!pTarget->GetInvisible() && mFleech.VOnSameYLevel(pTarget) && gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, pTarget->mXPos, pTarget->mYPos, 0) && gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0) && !mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pTarget->mXPos - mFleech.mXPos))
        {
            mFleech.mCurrentAnger = mFleech.mAttackAngerIncreaser + 1;
            return EState::eAlertedByAbe;
        }

        if (mFleech.mCurrentAnger > mFleech.mAttackAngerIncreaser && !pTarget->GetInvisible() && mFleech.mScrabOrParamite == Guid{})
        {
            return EState::eAlertedByAbe;
        }
    }

    mFleech.field_15A_chase_timer = 0;
    relive::Path_Hoist* pHoist = mFleech.TryGetHoist(0, 0);
    if (pHoist)
    {
        if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
        }
        else if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
        }
        mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
        mFleech.mHoistY = pHoist->mTopLeftY;
        return EState::eNearHoist;
    }

    if (!(Fleech_NextRandom() % 32) && mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
    {
        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        return mBrainState;
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
    {
        if (mFleech.GetAnimation().GetFlipX())
        {
            if (mFleech.mXPos < FP_FromInteger(mFleech.field_154))
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
            }
        }
        else if (mFleech.mXPos > FP_FromInteger(mFleech.field_154))
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
        }
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
    {
        if (mFleech.field_14C_EventXPos >= 0)
        {
            if (mFleech.mPatrolRange > 0)
            {
                if (FP_FromInteger(mFleech.field_14C_EventXPos) <= mFleech.mXPos)
                {
                    s16 patrolRangeDelta = FP_GetExponent(mFleech.mXPos) - mFleech.mPatrolRange;
                    if (mFleech.field_14C_EventXPos > patrolRangeDelta)
                    {
                        patrolRangeDelta = mFleech.field_14C_EventXPos;
                    }
                    mFleech.field_154 = patrolRangeDelta;

                    if (!mFleech.GetAnimation().GetFlipX())
                    {
                        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                    }
                }
                else
                {
                    s16 patrolRangeDelta = mFleech.mPatrolRange + FP_GetExponent(mFleech.mXPos);
                    if (mFleech.field_14C_EventXPos <= patrolRangeDelta)
                    {
                        patrolRangeDelta = mFleech.field_14C_EventXPos;
                    }
                    mFleech.field_154 = patrolRangeDelta;

                    if (mFleech.GetAnimation().GetFlipX())
                    {
                        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                    }
                }

                mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                mFleech.field_14C_EventXPos = -1;
            }
            else
            {
                // TODO: Check __OFSUB__ on else branch
                if (FP_FromInteger(mFleech.field_14C_EventXPos) > mFleech.mXPos)
                {
                    if (mFleech.GetAnimation().GetFlipX())
                    {
                        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                        mFleech.field_14C_EventXPos = -1;
                    }
                }
                else if (FP_FromInteger(mFleech.field_14C_EventXPos) < mFleech.mXPos)
                {
                    if (!mFleech.GetAnimation().GetFlipX())
                    {
                        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                        mFleech.field_14C_EventXPos = -1;
                    }
                }
            }
        }
        else
        {
            if (mFleech.mPatrolRange > 0)
            {
                if (mFleech.field_156_rnd_crawl > 0)
                {
                    mFleech.field_156_rnd_crawl--;
                }
                else
                {
                    if (mFleech.GetAnimation().GetFlipX())
                    {
                        mFleech.field_154 = FP_GetExponent(mFleech.mXPos) - Fleech_NextRandom() * (FP_GetExponent(mFleech.mXPos) + mFleech.mPatrolRange - mFleech.field_152_old_xpos) / 255;
                    }
                    else
                    {
                        mFleech.field_154 = FP_GetExponent(mFleech.mXPos) + Fleech_NextRandom() * (mFleech.mPatrolRange + mFleech.field_152_old_xpos - FP_GetExponent(mFleech.mXPos)) / 255;
                    }
                    mFleech.field_156_rnd_crawl = Fleech_NextRandom() & 0x3F;
                    mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                }
            }
        }
    }

    if (!mFleech.mGoesToSleep || (mFleech.mCurrentAnger >= mFleech.mMaxAnger && gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0)))
    {
        if ((Fleech_NextRandom() % 64) || mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
        {
            return mBrainState;
        }
        mFleech.SetCurrentMotion(eFleechMotions::Motion_5_PatrolCry);
        return mBrainState;
    }
    mFleech.mCurrentAnger = 0;
    mFleech.SetNextMotion(eFleechMotions::Motion_2_Unknown);
    return EState::eGoingBackToSleep;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_5()
{
    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_7_StopCrawling || mFleech.GetCurrentMotion() == eFleechMotions::Motion_6_Knockback)
    {
        return mBrainState;
    }

    if ((FP_FromInteger(mFleech.field_14E_ScrabParamiteEventXPos) > mFleech.mXPos && !mFleech.GetAnimation().GetFlipX()) ||
        (FP_FromInteger(mFleech.field_14E_ScrabParamiteEventXPos) < mFleech.mXPos && mFleech.GetAnimation().GetFlipX()))
    {
        if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_4_Crawl)
        {
            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
            }
        }
        else
        {
            mFleech.SetCurrentMotion(eFleechMotions::Motion_7_StopCrawling);
        }
        return mBrainState;
    }

    mFleech.field_14E_ScrabParamiteEventXPos = -1;
    mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
    mFleech.CanMove();
    return EState::eDangerNearby;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_6()
{
    auto pDangerObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_4_Crawl ||
        (pDangerObj && (mFleech.VIsObjNearby(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(5), pDangerObj))))
    {
        if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
        {
            return mBrainState;
        }
    }
    else
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
    }
    return EState::eBackToAlerted;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_7()
{
    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
    {
        return mBrainState;
    }
    return EState::eAlerted;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_8(IBaseAliveGameObject* pTarget)
{
    if (IgAbe(pTarget) && gAbe->GetInvisible())
    {
        return EState::eInit;
    }

    if (mFleech.field_15A_chase_timer < mFleech.field_158_chase_delay)
    {
        mFleech.field_15A_chase_timer++;
        return mBrainState;
    }

    mFleech.SetBrain(EBrainTypes::ChasingAbe);
    mFleech.mChasingAbeBrain.SetState(ChasingAbeBrain::EState::eInit);
    return mBrainState;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_9()
{
    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
    {
        return mBrainState;
    }
    if ((mFleech.GetAnimation().GetFlipX() && FP_FromInteger(mFleech.mHoistX) > mFleech.mXPos) || (!mFleech.GetAnimation().GetFlipX() && FP_FromInteger(mFleech.mHoistX) < mFleech.mXPos))
    {
        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        return mBrainState;
    }
    mFleech.SetNextMotion(eFleechMotions::Motion_11_RaiseHead);
    return EState::eClimbHoist;
}

PatrolBrain::EState PatrolBrain::Brain_Patrol_State_10()
{
    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
    {
        return mBrainState;
    }
    mFleech.field_152_old_xpos = FP_GetExponent(mFleech.mXPos);
    return EState::eAlerted;
}

void ChasingAbeBrain::VUpdate()
{
    auto pObj = static_cast<IBaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mFoodObjId));
    if (pObj)
    {
        if (pObj->GetDead() || (IgAbe(pObj) && gAbe->GetInvisible()))
        {
            mFleech.mFoodObjId = Guid{};
            pObj = nullptr;
        }
    }

    if (gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_7, &mFleech, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mFleech, 0, 0);
    }

    switch (mBrainState)
    {
        case EState::eInit:
            mBrainState = Brain_ChasingAbe_State_0(pObj);
            return;
        case EState::eChasingAbe_1:
            mBrainState = Brain_ChasingAbe_State_1(pObj);
            return;
        case EState::eUnknown_2:
            mBrainState = Brain_ChasingAbe_State_2(pObj);
            return;
        case EState::eContinueChaseAfterFall_3:
        case EState::eGoBackToChasingAbe_16:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                return;
            }
            mBrainState = EState::eChasingAbe_1;
            return;

        case EState::eBlockedByWall_4:
        {
            if (!pObj || pObj->mHealth <= FP_FromInteger(0))
            {
                mBrainState = EState::eBackToPatrol_13;
                return;
            }

            IBaseAliveGameObject* pMudOrAbe = mFleech.FindMudOrAbe();
            if (pMudOrAbe)
            {
                if (pMudOrAbe->mBaseGameObjectId != mFleech.mFoodObjId)
                {
                    pObj = pMudOrAbe;
                    mFleech.mFoodObjId = pMudOrAbe->mBaseGameObjectId;
                }
            }

            mFleech.mChasingOrScaredCrawlingLeft = false;
            if (!mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mFleech.GetSpriteScale())))
            {
                mBrainState = EState::eChasingAbe_1;
                return;
            }

            relive::Path_Hoist* pHoist = mFleech.TryGetHoist(0, false);
            if (pHoist)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
                mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() < FP_FromInteger(1) ? 6 : 12);
                mFleech.mHoistY = pHoist->mTopLeftY;
                mBrainState = EState::ePrepareToHoist_14;
                return;
            }
            [[fallthrough]];
        }

        case EState::eUnknown_5:
            if (!pObj || pObj->mHealth <= FP_FromInteger(0))
            {
                mBrainState = EState::eBackToPatrol_13;
                return;
            }

            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                return;
            }

            if (IgAbe(pObj) &&
                mFleech.VOnSameYLevel(gAbe) &&
                gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, gAbe->mXPos, gAbe->mYPos, 0) &&
                gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0) &&
                !mFleech.WallHit(FP_FromInteger((mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5)), gAbe->mXPos - mFleech.mXPos))
            {
                mBrainState = EState::eChasingAbe_1;
                return;
            }

            if (!mFleech.IsNear(pObj))
            {
                mBrainState = EState::eUnknown_2;
                return;
            }

            if (mFleech.mChasingOrScaredCrawlingLeft)
            {
                if (pObj->mXPos <= mFleech.mXPos - FP_FromInteger(2))
                {
                    return;
                }
            }
            else
            {
                if (pObj->mXPos >= mFleech.mXPos + FP_FromInteger(2))
                {
                    return;
                }
            }
            mBrainState = EState::eChasingAbe_1;
            return;

        case EState::eScrabOrParamiteNearby_6:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_7_StopCrawling && mFleech.GetCurrentMotion() != eFleechMotions::Motion_6_Knockback && mFleech.mNextMotion != -1)
            {
                return;
            }
            mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
            mBrainState = EState::eUnknown_7;
            return;

        case EState::eUnknown_7:
        {
            auto v70 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_4_Crawl || (v70 && mFleech.VIsObjNearby(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(5), v70)))
            {
                if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle && mFleech.mNextMotion != -1)
                {
                    return;
                }
                mFleech.SetNextMotion(eFleechMotions::Motion_6_Knockback);
            }
            else
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
            }
            mBrainState = EState::eUnknown_9;
            return;
        }

        case EState::eFleechUnknown_8:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                return;
            }
            mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
            mBrainState = EState::eUnknown_9;
            return;

        case EState::eUnknown_9:
            mBrainState = Brain_ChasingAbe_State_9(pObj);
            return;

        case EState::eAbeIsInTongueRange_10:
            if (pObj)
            {
                if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                {
                    mBrainState = EState::eIsAbeDead_11;
                    return;
                }
                else if (mFleech.GetNextMotion() == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                {
                    return;
                }

                mFleech.ResetTarget();
                mBrainState = eChasingAbe_1;
                return;
            }
            mFleech.sub_42B8C0();
            mBrainState = EState::eBackToPatrol_13;
            return;

        case EState::eIsAbeDead_11:
            if (pObj)
            {
                if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
                {
                    return;
                }

                mFleech.ResetTarget();

                if (pObj->mHealth <= FP_FromInteger(0))
                {
                    mBrainState = EState::eBackToPatrol_13;
                    return;
                }

                if (mFleech.VIsObj_GettingNear_On_X(pObj))
                {
                    mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                    mBrainState = EState::eUnknown_12;
                    return;
                }

                mBrainState = EState::eChasingAbe_1;
                return;
            }
            mFleech.sub_42B8C0();
            mBrainState = EState::eBackToPatrol_13;
            return;

        case EState::eUnknown_12:
            if (pObj)
            {
                if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
                {
                    return;
                }
                if (!mFleech.Collision(1))
                {
                    mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                }
                mBrainState = EState::eChasingAbe_1;
                return;
            }
            mFleech.sub_42B8C0();
            mBrainState = EState::eBackToPatrol_13;
            return;

        case EState::eBackToPatrol_13:
            mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
            mFleech.SetBrain(EBrainTypes::Patrol);
            mFleech.mCurrentAnger = mFleech.mMaxAnger + (mFleech.mAttackAngerIncreaser - mFleech.mMaxAnger) / 2;
            mFleech.mPatrolBrain.SetState(PatrolBrain::EState::eInit);
            return;

        case EState::ePrepareToHoist_14:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                return;
            }
            if ((mFleech.GetAnimation().GetFlipX() && FP_FromInteger(mFleech.mHoistX) > mFleech.mXPos) || (!mFleech.GetAnimation().GetFlipX() && FP_FromInteger(mFleech.mHoistX) < mFleech.mXPos))
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                return;
            }
            mFleech.SetNextMotion(eFleechMotions::Motion_11_RaiseHead);
            mBrainState = EState::eHoistDone_15;
            return;

        case EState::eHoistDone_15:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_13_SettleOnGround)
            {
                return;
            }
            mBrainState = EState::eGoBackToChasingAbe_16;
            return;

        default:
            return;
    }
}

ChasingAbeBrain::EState ChasingAbeBrain::Brain_ChasingAbe_State_9(IBaseAliveGameObject* pObj)
{
    if (!mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(6)))
    {
        return EState::eChasingAbe_1;
    }

    auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
    if (pDangerObj == sControlledCharacter)
    {
        if (mFleech.Collision(1) || mFleech.HandleEnemyStopperOrSlamDoor(1) || mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(mFleech.GetAnimation().GetFlipX() != 0 ? -1 : 1)))
        {
            mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
            mFleech.SetBrain(EBrainTypes::Scared);
            return EState::eInit; // TODO: set scared brain state to 0 instead
        }
    }

    if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
        return mBrainState;
    }

    if (mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(5)))
    {
        auto v82 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
        if (mFleech.VIsFacingMe(v82))
        {
            mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        }
        return EState::eFleechUnknown_8;
    }

    if (!pObj || !mFleech.IsNear(pObj))
    {
        return EState::eBackToPatrol_13;
    }

    if (pObj->mXPos < mFleech.mXPos)
    {
        if (pDangerObj->mXPos > mFleech.mXPos)
        {
            return EState::eChasingAbe_1;
        }
    }

    if (pDangerObj->mXPos < mFleech.mXPos)
    {
        return EState::eChasingAbe_1;
    }

    if (pObj->mXPos <= mFleech.mXPos)
    {
        if (pDangerObj->mXPos > mFleech.mXPos)
        {
            return EState::eChasingAbe_1;
        }
    }

    if (!(Fleech_NextRandom() % 32) && mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
    {
        mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        return mBrainState;
    }

    if ((Fleech_NextRandom() % 64) || mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
    {
        return mBrainState;
    }
    mFleech.SetCurrentMotion(eFleechMotions::Motion_5_PatrolCry);
    return mBrainState;
}

ChasingAbeBrain::EState ChasingAbeBrain::Brain_ChasingAbe_State_2(IBaseAliveGameObject* pObj)
{
    if (!pObj || pObj->mHealth <= FP_FromInteger(0))
    {
        return EState::eBackToPatrol_13;
    }

    if (mFleech.IsNear(pObj))
    {
        return EState::eChasingAbe_1;
    }

    if (mFleech.VIsFacingMe(pObj) || mFleech.GetCurrentMotion() == eFleechMotions::Motion_7_StopCrawling || mFleech.GetCurrentMotion() == eFleechMotions::Motion_6_Knockback)
    {
        if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_4_Crawl)
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
        }

        if (mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(5)))
        {
            auto v56 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
            if (mFleech.VIsFacingMe(v56))
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
            }
            mFleech.Sound(FleechSound::Scared_7);
            return EState::eScrabOrParamiteNearby_6;
        }

        if (mFleech.field_15E_lost_target_timer < mFleech.mLostTargetTimeout)
        {
            mFleech.field_15E_lost_target_timer++;
            relive::Path_Hoist* pHoist = mFleech.TryGetHoist(1, false);
            if (pHoist)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
                mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() < FP_FromInteger(1) ? 6 : 12);
                mFleech.mHoistY = pHoist->mTopLeftY;
                return EState::ePrepareToHoist_14;
            }

            if (Fleech_NextRandom() % 64)
            {
                return mBrainState;
            }

            mFleech.SetCurrentMotion(eFleechMotions::Motion_5_PatrolCry);
            return mBrainState;
        }

        mFleech.field_15E_lost_target_timer = 0;
        mFleech.SetBrain(EBrainTypes::Patrol);
        mFleech.mCurrentAnger = mFleech.mAttackAngerIncreaser - 1;
        mFleech.mPatrolBrain.SetState(PatrolBrain::EState::eInit);
        return mBrainState;
    }
    else if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl) // TODO: Check v52 was cur motion
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
        return mBrainState;
    }
    else
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_6_Knockback);
        return mBrainState;
    }
}

ChasingAbeBrain::EState ChasingAbeBrain::Brain_ChasingAbe_State_0(IBaseAliveGameObject* pObj)
{
    if (!pObj)
    {
        mFleech.mFoodObjId = Guid{};
        IBaseAliveGameObject* pMudOrAbe = mFleech.FindMudOrAbe();
        if (!pMudOrAbe)
        {
            return EState::eBackToPatrol_13;
        }
        mFleech.mFoodObjId = pMudOrAbe->mBaseGameObjectId;
    }
    mFleech.field_15E_lost_target_timer = 0;
    mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
    mFleech.Sound(FleechSound::PatrolCry_0);
    return EState::eChasingAbe_1;
}

ChasingAbeBrain::EState ChasingAbeBrain::Brain_ChasingAbe_State_1(IBaseAliveGameObject* pObj)
{
    if (!pObj || pObj->mHealth <= FP_FromInteger(0))
    {
        return EState::eBackToPatrol_13;
    }

    // Is moving?
    if (mFleech.mVelX != FP_FromInteger(0))
    {
        // Check for blocked by wall
        const FP k1Directed = FP_FromInteger((mFleech.GetAnimation().GetFlipX()) != 0 ? -1 : 1);
        if (mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mFleech.GetSpriteScale()) * k1Directed))
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
            if (mFleech.mVelX < FP_FromInteger(0))
            {
                mFleech.mChasingOrScaredCrawlingLeft = true;
            }
            return EState::eBlockedByWall_4;
        }

        // Check for enemy stopper or slam door
        if (mFleech.HandleEnemyStopperOrSlamDoor(1))
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
            if (mFleech.mVelX < FP_FromInteger(0))
            {
                mFleech.mChasingOrScaredCrawlingLeft = true;
            }

            FP xOffset = mFleech.mXPos;
            if (mFleech.GetAnimation().GetFlipX())
            {
                xOffset -= ScaleToGridSize(mFleech.GetSpriteScale());
            }
            else
            {
                xOffset += ScaleToGridSize(mFleech.GetSpriteScale());
            }

            FP slamDoorX = xOffset;
            if (mFleech.mXPos <= xOffset)
            {
                slamDoorX = mFleech.mXPos;
            }

            FP slamDoorW = mFleech.mXPos;
            if (mFleech.mXPos <= xOffset)
            {
                slamDoorW = xOffset;
            }

            relive::Path_TLV* pSlamDoor = gPathInfo->TLV_Get_At(
                FP_GetExponent(slamDoorX),
                FP_GetExponent(mFleech.mYPos),
                FP_GetExponent(slamDoorW),
                FP_GetExponent(mFleech.mYPos),
                ReliveTypes::eSlamDoor);

            if (pSlamDoor)
            {
                return EState::eBlockedByWall_4;
            }
            return EState::eUnknown_5;
        }
    }

    // Check for danger object
    if (mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(5)))
    {
        auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
        if (mFleech.VIsFacingMe(pDangerObj))
        {
            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
                mFleech.Sound(FleechSound::Scared_7);
                return EState::eScrabOrParamiteNearby_6;
            }

            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_6_Knockback);
            }
        }

        mFleech.Sound(FleechSound::Scared_7);
        return EState::eScrabOrParamiteNearby_6;
    }

    // Check for food object
    IBaseAliveGameObject* pAbeOrMud = mFleech.FindMudOrAbe();
    if (pAbeOrMud)
    {
        if (pAbeOrMud->mBaseGameObjectId != mFleech.mFoodObjId)
        {
            pObj = pAbeOrMud;
            mFleech.mFoodObjId = pAbeOrMud->mBaseGameObjectId;
        }
    }

    // Can we get to them on this level?
    if (mFleech.VOnSameYLevel(pObj))
    {
        if (mFleech.VIsObjNearby(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(3), pObj))
        {
            if (pObj->GetSpriteScale() == mFleech.GetSpriteScale() &&
                mFleech.VIsFacingMe(pObj) &&
                !mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pObj->mXPos - mFleech.mXPos) &&
                mFleech.GotNoTarget() &&
                gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0))
            {
                mFleech.SetTarget();
                mFleech.SetNextMotion(eFleechMotions::Motion_14_ExtendTongueFromEnemy);
                return EState::eAbeIsInTongueRange_10;
            }
        }
    }

    // Can we get to a hanging abe?
    if (IgAbe(pObj) && pObj->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 && mFleech.mYPos > pObj->mYPos)
    {
        if (mFleech.mYPos - pObj->mYPos <= (ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(6)))
        {
            if (mFleech.VIsObjNearby(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(2), pObj))
            {
                if (pObj->GetSpriteScale() == mFleech.GetSpriteScale()
                    && mFleech.VIsFacingMe(pObj) && !mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pObj->mXPos - mFleech.mXPos) && mFleech.GotNoTarget() && gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0))
                {
                    mFleech.SetTarget();
                    mFleech.SetNextMotion(eFleechMotions::Motion_14_ExtendTongueFromEnemy);
                    return EState::eAbeIsInTongueRange_10;
                }
            }
        }
    }

    if (pObj->mYPos >= mFleech.mYPos - (FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 18 : 9)) || pObj->GetSpriteScale() != mFleech.GetSpriteScale())
    {
        return Brain_ChasingAbe_State1_Helper(pObj);
    }

    // Find hoist in front us?
    relive::Path_Hoist* pHoist = mFleech.TryGetHoist(1, false);
    if (pHoist)
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
        mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
        mFleech.mHoistY = pHoist->mTopLeftY;
        return EState::ePrepareToHoist_14;
    }

    // Find host in front or behind us?
    pHoist = mFleech.TryGetHoist(0, true);
    if (pHoist)
    {
        if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
        {
            // TODO: Check left VS flip is correct
            if ((pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingLeft && mFleech.GetAnimation().GetFlipX()) && pHoist->mGrabDirection != relive::Path_Hoist::GrabDirection::eFacingAnyDirection)
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
            }

            mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
            mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
            mFleech.mHoistY = pHoist->mTopLeftY;
            return EState::ePrepareToHoist_14;
        }
        else
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
            return mBrainState;
        }
    }

    // Look 12 ahead fora hoist
    s32 k12BlocksCheck = 1;
    do
    {
        pHoist = mFleech.TryGetHoist(k12BlocksCheck, true);
        if (pHoist)
        {
            mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
            mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
            mFleech.mHoistY = pHoist->mTopLeftY;
            return mBrainState;
        }

        ++k12BlocksCheck;
    }
    while (k12BlocksCheck <= 12);

    // Look 8 behind for a hoist
    s32 k8BlocksCheck = 1;
    while (1)
    {
        pHoist = mFleech.TryGetHoist(-k8BlocksCheck, 1);
        if (pHoist)
        {
            switch (mFleech.GetCurrentMotion())
            {
                case eFleechMotions::Motion_4_Crawl:
                    mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
                    break;

                case eFleechMotions::Motion_3_Idle:
                    mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                    break;

                case eFleechMotions::Motion_6_Knockback:
                case eFleechMotions::Motion_7_StopCrawling:
                    mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                    break;
            }

            mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
            mFleech.mHoistY = pHoist->mTopLeftY;
            return mBrainState;
        }

        if (++k8BlocksCheck > 8)
        {
            return Brain_ChasingAbe_State1_Helper(pObj);
        }
    }
}

ChasingAbeBrain::EState ChasingAbeBrain::Brain_ChasingAbe_State1_Helper(IBaseAliveGameObject* pObj)
{
    if (pObj->mYPos < mFleech.mYPos - FP_FromInteger((mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 18 : 9)) && pObj->GetSpriteScale() == mFleech.GetSpriteScale() && mFleech.IsNear(pObj))
    {
        const FP v45 = mFleech.mXPos - pObj->mXPos;
        if (FP_Abs(v45) < ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromDouble(0.5))
        {
            return EState::eBackToPatrol_13;
        }
    }

    if (!mFleech.VIsFacingMe(pObj) && mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
    {
        if (mFleech.VIsFacingMe(pObj))
        {
            const FP v48 = FP_FromInteger((mFleech.GetAnimation().GetFlipX()) != 0 ? -1 : 1);
            if (mFleech.WallHit(
                    FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
                    ScaleToGridSize(mFleech.GetSpriteScale()) * v48))
            {
                if (mFleech.GetAnimation().GetFlipX())
                {
                    mFleech.mChasingOrScaredCrawlingLeft = true;
                }
                return EState::eBlockedByWall_4;
            }
            mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
        }
        else
        {
            mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
        }
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_7_StopCrawling)
    {
        mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
    }

    if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_9_Fall)
    {
        return EState::eContinueChaseAfterFall_3;
    }

    if (mFleech.IsNear(pObj))
    {
        return mBrainState;
    }

    if (!mFleech.VIsFacingMe(pObj))
    {
        if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_7_StopCrawling && mFleech.GetCurrentMotion() != eFleechMotions::Motion_6_Knockback)
        {
            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
                return EState::eUnknown_2;
            }
            mFleech.SetNextMotion(eFleechMotions::Motion_6_Knockback);
        }
    }
    return EState::eUnknown_2;
}

static const MusicController::MusicTypes sScaredBrainMusicTypes[16] = {
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogTension_5,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eSlogChaseTension_6,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eNone_0,
    MusicController::MusicTypes::eNone_0};

void ScaredBrain::VUpdate()
{
    auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
    if (pDangerObj && pDangerObj->mHealth > FP_FromInteger(0))
    {
        // Danger target is dead, check if there is another one who is still alive.
        mFleech.IsScrabOrParamiteNear(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(8));
    }

    pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mFleech.mScrabOrParamite));
    if (pDangerObj)
    {
        if (pDangerObj->GetDead())
        {
            mFleech.mScrabOrParamite = Guid{};
            pDangerObj = 0;
        }
    }

    if (gMap.Is_Point_In_Current_Camera(mFleech.mCurrentLevel, mFleech.mCurrentPath, mFleech.mXPos, mFleech.mYPos, 0))
    {
        MusicController::static_PlayMusic(sScaredBrainMusicTypes[mBrainState], &mFleech, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mFleech, 0, 0);
    }

    switch (mBrainState)
    {
        case EState::eScared_0:
            if (!pDangerObj || pDangerObj != sControlledCharacter)
            {
                mBrainState = EState::ePatrolArea_9;
                return;
            }

            if (!mFleech.VIsFacingMe(pDangerObj))
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                mFleech.Sound(FleechSound::Scared_7);
                mBrainState = EState::eReactToDanger_1;
                return;
            }

            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                mFleech.Sound(FleechSound::Scared_7);
                mBrainState = EState::eReactToDanger_1;
                return;
            }

            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_4_Crawl)
            {
                mFleech.Sound(FleechSound::Scared_7);
                mBrainState = EState::eReactToDanger_1;
                return;
            }

            mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
            mFleech.Sound(FleechSound::Scared_7);
            mBrainState = EState::eReactToDanger_1;
            return;

        case EState::eReactToDanger_1:
        {
            if (!pDangerObj || pDangerObj->mHealth <= FP_FromInteger(0) || pDangerObj != sControlledCharacter)
            {
                mBrainState = EState::ePatrolArea_9;
                return;
            }

            if (mFleech.mVelX != FP_FromInteger(0))
            {
                const FP v9 = FP_FromInteger((mFleech.GetAnimation().GetFlipX()) != 0 ? -1 : 1);
                if (mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(0) ? 10 : 5), ScaleToGridSize(mFleech.GetSpriteScale()) * v9))
                {
                    mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
                    if (mFleech.mVelX < FP_FromInteger(0))
                    {
                        mFleech.mChasingOrScaredCrawlingLeft = true;
                    }
                    mBrainState = EState::eLookForHoist_3;
                    return;
                }

                if (mFleech.mVelX != FP_FromInteger(0) && (mFleech.Collision(1) || mFleech.HandleEnemyStopperOrSlamDoor(1)))
                {
                    mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
                    if (mFleech.mVelX < FP_FromInteger(0))
                    {
                        mFleech.mChasingOrScaredCrawlingLeft = true;
                    }
                    mBrainState = EState::eCornered_4;
                    return;
                }
            }

            relive::Path_Hoist* pHoist = mFleech.TryGetHoist(1, 0);
            if (pHoist)
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_8_StopMidCrawlCycle);
                mFleech.mHoistX = pHoist->mTopLeftX + 12;
                mFleech.mHoistY = pHoist->mTopLeftY;
                mBrainState = EState::ePrepareToHoist_10;
                return;
            }

            if (mFleech.VIsFacingMe(pDangerObj))
            {
                if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_4_Crawl)
                {
                    mFleech.SetNextMotion(eFleechMotions::Motion_7_StopCrawling);
                }
            }

            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
            {
                if (mFleech.VIsFacingMe(pDangerObj))
                {
                    mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                }
                else
                {
                    const FP k1Directed = FP_FromInteger(mFleech.GetAnimation().GetFlipX() != 0 ? -1 : 1);
                    if (mFleech.WallHit(
                            FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
                            ScaleToGridSize(mFleech.GetSpriteScale()) * k1Directed))
                    {
                        if (mFleech.GetAnimation().GetFlipX())
                        {
                            mFleech.mChasingOrScaredCrawlingLeft = true;
                        }
                        mBrainState = EState::eCornered_4;
                        return;
                    }
                    mFleech.SetNextMotion(eFleechMotions::Motion_4_Crawl);
                }
            }

            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_9_Fall)
            {
                return;
            }
            mBrainState = EState::eCrawl_2;
            return;
        }

        case EState::eCrawl_2:
            if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_3_Idle)
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_4_Crawl);
                mBrainState = EState::eReactToDanger_1;
                return;
            }
            return;

        case EState::eLookForHoist_3:
        {
            const FP v22 = FP_FromInteger(mFleech.mChasingOrScaredCrawlingLeft != 0 ? -1 : 1);
            if (!mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mFleech.GetSpriteScale()) * v22))
            {
                mBrainState = EState::eReactToDanger_1;
                return;
            }

            relive::Path_Hoist* pHoist = mFleech.TryGetHoist(0, 0);
            if (pHoist)
            {
                if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
                {
                    mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
                }

                mFleech.mHoistX = pHoist->mTopLeftX + (mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
                mFleech.mHoistY = pHoist->mTopLeftY;
                mBrainState = EState::ePrepareToHoist_10;
                return;
            }
            [[fallthrough]];
        }

        case EState::eCornered_4:
        {
            if (!pDangerObj || pDangerObj != sControlledCharacter)
            {
                mBrainState = EState::ePatrolArea_9;
                return;
            }

            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                return;
            }

            if (mFleech.mChasingOrScaredCrawlingLeft)
            {
                if (pDangerObj->mXPos < mFleech.mXPos)
                {
                    mBrainState = EState::eReactToDanger_1;
                    return;
                }
            }
            else if (pDangerObj->mXPos > mFleech.mXPos)
            {
                mBrainState = EState::eReactToDanger_1;
                return;
            }

            if (mFleech.VOnSameYLevel(pDangerObj) &&
                mFleech.VIsObjNearby(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(3), pDangerObj) &&
                pDangerObj->GetSpriteScale() == mFleech.GetSpriteScale() &&
                mFleech.VIsFacingMe(pDangerObj) &&
                !mFleech.WallHit(FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pDangerObj->mXPos - mFleech.mXPos) &&
                mFleech.GotNoTarget())
            {
                mFleech.mFoodObjId = mFleech.mScrabOrParamite;
                mBrainState = EState::eCorneredPrepareAttack_5;
                return;
            }
            else
            {
                if ((Fleech_NextRandom() % 32) || mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
                {
                    return;
                }
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                return;
            }
            break;
        }

        case EState::eCorneredPrepareAttack_5:
            if (!pDangerObj || pDangerObj != sControlledCharacter)
            {
                mBrainState = EState::ePatrolArea_9;
                return;
            }

            if (mFleech.VIsFacingMe(pDangerObj) || mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                if (!mFleech.VOnSameYLevel(pDangerObj))
                {
                    return;
                }

                if (!mFleech.VIsObjNearby(ScaleToGridSize(mFleech.GetSpriteScale()) * FP_FromInteger(3), pDangerObj))
                {
                    return;
                }

                if (pDangerObj->GetSpriteScale() != mFleech.GetSpriteScale())
                {
                    return;
                }

                if (!mFleech.VIsFacingMe(pDangerObj))
                {
                    return;
                }

                if (mFleech.WallHit(
                        FP_FromInteger(mFleech.GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
                        pDangerObj->mXPos - mFleech.mXPos)
                    || !mFleech.GotNoTarget())
                {
                    return;
                }

                mFleech.SetTarget();
                mFleech.SetNextMotion(eFleechMotions::Motion_14_ExtendTongueFromEnemy);
                mBrainState = EState::eCorneredAttack_6;
                return;
            }
            else
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                return;
            }
            break;

        case EState::eCorneredAttack_6:
            if (pDangerObj)
            {
                if (mFleech.GetCurrentMotion() == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                {
                    mBrainState = EState::eCheckIfEnemyDead_7;
                    return;
                }
                else
                {
                    if (mFleech.GetNextMotion() == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                    {
                        return;
                    }
                    mFleech.ResetTarget();
                    mBrainState = EState::eReactToDanger_1;
                    return;
                }
            }
            else
            {
                mFleech.sub_42B8C0();
                mBrainState = EState::ePatrolArea_9;
                return;
            }
            break;

        case EState::eCheckIfEnemyDead_7:
            if (pDangerObj)
            {
                if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
                {
                    return;
                }

                mFleech.ResetTarget();

                if (pDangerObj->mHealth <= FP_FromInteger(0))
                {
                    mBrainState = EState::ePatrolArea_9;
                    return;
                }
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                mBrainState = EState::eEnemyStillAlive_8;
                return;
            }
            else
            {
                mFleech.sub_42B8C0();
                mBrainState = EState::ePatrolArea_9;
                return;
            }
            break;

        case EState::eEnemyStillAlive_8:
            if (pDangerObj)
            {
                if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
                {
                    return;
                }
                mBrainState = EState::eCorneredPrepareAttack_5;
                return;
            }
            else
            {
                mFleech.sub_42B8C0();
                mBrainState = EState::ePatrolArea_9;
                return;
            }
            break;

        case EState::ePatrolArea_9:
            mFleech.SetNextMotion(eFleechMotions::Motion_3_Idle);
            mFleech.mScrabOrParamite = Guid{};
            mFleech.mFoodObjId = Guid{};
            mFleech.SetBrain(EBrainTypes::Patrol);
            mFleech.mPatrolBrain.SetState(PatrolBrain::EState::eInit);
            return;

        case EState::ePrepareToHoist_10:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_3_Idle)
            {
                return;
            }
            if ((!mFleech.GetAnimation().GetFlipX() || FP_FromInteger(mFleech.mHoistX) <= mFleech.mXPos) &&
                (mFleech.GetAnimation().GetFlipX() || FP_FromInteger(mFleech.mHoistX) >= mFleech.mXPos))
            {
                mFleech.SetNextMotion(eFleechMotions::Motion_11_RaiseHead);
                mBrainState = EState::eHoisting_11;
                return;
            }
            else
            {
                mFleech.SetCurrentMotion(eFleechMotions::Motion_6_Knockback);
                return;
            }
            break;

        case EState::eHoisting_11:
            if (mFleech.GetCurrentMotion() != eFleechMotions::Motion_13_SettleOnGround)
            {
                return;
            }
            mBrainState = EState::ePatrolArea_9;
            return;

        default:
            return;
    }
}

void DeathBrain::VUpdate()
{
    mFleech.mFoodObjId = Guid{};
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, &mFleech, 0, 0);

    if (mFleech.field_12C_shrivel_timer < static_cast<s32>(sGnFrame + 80))
    {
        mFleech.SetSpriteScale(mFleech.GetSpriteScale() - FP_FromDouble(0.022));
        mFleech.mRGB.r -= 2;
        mFleech.mRGB.g -= 2;
        mFleech.mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < mFleech.field_12C_shrivel_timer - 24)
    {
        mFleech.DeathSmokeEffect(false);
    }

    if (mFleech.GetSpriteScale() < FP_FromInteger(0))
    {
        mFleech.SetDead(true);
    }

    //return 100;
}

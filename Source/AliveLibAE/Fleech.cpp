#include "stdafx.h"
#include "Fleech.hpp"
#include "Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "SnoozeParticle.hpp"
#include "Dove.hpp"
#include "Blood.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "Gibs.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Sound/Midi.hpp"
#include "Grid.hpp"
#include "../relive_lib/Math.hpp"
#include "AnimationCallBacks.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include <algorithm>
#include "../relive_lib/Collisions.hpp"

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

enum eFleechMotions
{
    Motion_0_Sleeping,
    Motion_1_WakingUp,
    Motion_2_Unknown,
    Motion_3_Idle,
    Motion_4_Crawl,
    Motion_5_PatrolCry,
    Motion_6_Knockback,
    Motion_7_StopCrawling,
    Motion_8_StopMidCrawlCycle,
    Motion_9_Fall,
    Motion_10_Land,
    Motion_11_RaiseHead,
    Motion_12_Climb,
    Motion_13_SettleOnGround,
    Motion_14_ExtendTongueFromEnemy,
    Motion_15_RetractTongueFromEnemey,
    Motion_16_DeathByFalling,
    Motion_17_SleepingWithTongue,
    Motion_18_Consume
};

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


const TFleechBrainFn sFleechBrainTable[4] = {
    &Fleech::Brain_0_Patrol,
    &Fleech::Brain_1_ChasingAbe,
    &Fleech::Brain_2_Scared,
    &Fleech::Brain_3_Death};

enum eFleechBrains
{
    eBrain_0_Patrol = 0,
    eBrain_1_ChasingAbe = 1,
    eBrain_2_Scared = 2,
    eBrain_3_Death = 3
};

static u8 Fleech_NextRandom()
{
    return gRandomBytes[sFleechRandomIdx_5BC20C++];
}

Fleech::Fleech(relive::Path_Fleech* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(2)
{
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

    mTlvInfo = tlvId;
    mBrainState = eFleechBrains::eBrain_0_Patrol;
    field_11C_obj_id = Guid{};
    field_170_danger_obj = Guid{};

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

    field_140_max_anger = 2;
    field_158_chase_delay = 10;

    field_142_attack_anger_increaser = pTlv->mAttackAngerIncreaser + 2;
    field_144_wake_up_switch_id = pTlv->mWakeUpSwitchId;
    field_146_wake_up_switch_anger_value = pTlv->mWakeUpSwitchAngerValue;
    field_148_wake_up_switch_value = SwitchStates_Get(pTlv->mWakeUpSwitchId) & 0xFFFF;
    field_14A_can_wake_up_switch_id = pTlv->mCanWakeUpSwitchId;
    field_150_patrol_range = FP_GetExponent(FP_FromInteger(pTlv->mPatrolRangeInGrids) * ScaleToGridSize(GetSpriteScale()));
    field_15C_lost_target_timeout = pTlv->mLostTargetTimeout;

    InitTonguePolys();

    field_13E_current_anger = 0;

    if (pTlv->mHanging == relive::reliveChoice::eYes)
    {
        field_160_hoistX = (pTlv->mBottomRightX + pTlv->mTopLeftX) / 2;
        field_166_angle = Fleech_NextRandom();
        mYPos -= FP_FromInteger(pTlv->mTopLeftY - pTlv->mBottomRightY);
        TongueHangingFromWall((pTlv->mBottomRightX + pTlv->mTopLeftX) / 2, pTlv->mTopLeftY);
        mCurrentMotion = eFleechMotions::Motion_17_SleepingWithTongue;
        SetAnim();
    }
    else if (mAsleep)
    {
        mCurrentMotion = eFleechMotions::Motion_0_Sleeping;
        SetAnim();
    }
    else
    {
        mCurrentMotion = eFleechMotions::Motion_3_Idle;
        field_13E_current_anger = field_140_max_anger + (field_142_attack_anger_increaser - field_140_max_anger) / 2;
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

s32 Fleech::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const FleechSaveState*>(pBuffer);

    auto pTlv = static_cast<relive::Path_Fleech*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));

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

        pFleech->mCurrentPath = pState->mPathNumber;
        pFleech->mCurrentLevel = pState->mLvlNumber;
        pFleech->SetSpriteScale(pState->mSpriteScale);

        pFleech->mRGB.SetRGB(pState->mRed, pState->mGreen, pState->mBlue);

        pFleech->mCurrentMotion = pState->field_28_current_motion;
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
        pFleech->mCurrentMotion = pState->mCurrentMotion;
        pFleech->mNextMotion = pState->mNextMotion;

        pFleech->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->mLastLineYPos);

        pFleech->SetRestoredFromQuickSave(true);

        pFleech->BaseAliveGameObjectCollisionLineType = pState->mCollisionLineType;
        pFleech->mTlvInfo = pState->mTlvInfo;
        pFleech->field_11C_obj_id = pState->field_44_obj_id;
        pFleech->field_120_unused = pState->field_48_unused;
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

        pFleech->mBrainState = pState->field_5E_brain_state;
        pFleech->mBrainSubState = pState->field_60_state;
        pFleech->field_12C_shrivel_timer = pState->field_64_shrivel_timer;
        pFleech->field_128 = pState->field_62;
        sFleechRandomIdx_5BC20C = pState->field_68_fleech_random_idx;
        pFleech->field_130_bDidMapFollowMe = pState->field_6A_bDidMapFollowMe;
        pFleech->field_134_unused = pState->field_6C_unused;
        pFleech->field_138_velx_factor = pState->field_70_velx_factor;
        pFleech->field_13C_unused = pState->field_74_unused;
        pFleech->field_13E_current_anger = pState->field_76_current_anger;
        pFleech->field_140_max_anger = pState->field_78_max_anger;
        pFleech->field_142_attack_anger_increaser = pState->field_7A_attack_anger;
        pFleech->field_144_wake_up_switch_id = pState->field_7C_wakeup_id;
        pFleech->field_146_wake_up_switch_anger_value = pState->field_7E_wake_up_switch_anger_value;
        pFleech->field_148_wake_up_switch_value = pState->field_80_wake_up_switch_value;
        pFleech->field_14A_can_wake_up_switch_id = pState->field_82_can_wake_up_id;
        pFleech->field_14C_EventXPos = pState->field_84_EventXPos;
        pFleech->field_14E_ScrabParamiteEventXPos = pState->field_86_ScrabParamiteEventXPos;
        pFleech->field_150_patrol_range = pState->field_88_patrol_range;
        pFleech->field_152_old_xpos = pState->field_8A_old_xpos;
        pFleech->field_154 = pState->field_8C;
        pFleech->field_156_rnd_crawl = pState->field_8E_rnd_crawl;
        pFleech->field_158_chase_delay = pState->field_90_chase_delay;
        pFleech->field_15A_chase_timer = pState->field_92_chase_timer;
        pFleech->field_15C_lost_target_timeout = pState->field_94_lost_target_timeout;
        pFleech->field_15E_lost_target_timer = pState->field_96_lost_target_timer;
        pFleech->field_160_hoistX = pState->field_98_hoistX;
        pFleech->field_162_hoistY = pState->field_9A_hoistY;
        pFleech->field_164_always_0 = pState->field_9C_always_0;
        pFleech->field_166_angle = pState->field_9E_angle;
        pFleech->field_168_hoistY_distance = pState->field_A0_hoistY_distance;
        pFleech->field_16C_hoistX_distance = pState->field_A4_hoistX_distance;
        pFleech->field_170_danger_obj = pState->field_A8;

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

    return sizeof(FleechSaveState);
}

s32 Fleech::VGetSaveState(u8* pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return 0;
    }

    auto pState = reinterpret_cast<FleechSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eFleech;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;
    pState->mXPos = mXPos;
    pState->mYPos = mYPos;
    pState->mVelX = mVelX;
    pState->mVelY = mVelY;
    pState->field_70_velx_factor = field_138_velx_factor;
    pState->mPathNumber = mCurrentPath;
    pState->mLvlNumber = mCurrentLevel;
    pState->mSpriteScale = GetSpriteScale();
    pState->mRed = mRGB.r;
    pState->mGreen = mRGB.g;
    pState->mBlue = mRGB.b;
    pState->mFlipX = GetAnimation().GetFlipX();
    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_anim_current_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->field_2C_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->mDrawable = GetDrawable();
    pState->mRender = GetAnimation().GetRender();
    pState->mHealth = mHealth;
    pState->mCurrentMotion = mCurrentMotion;
    pState->mNextMotion = mNextMotion;
    pState->mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->mCollisionLineType = -1;
    }

    if (BaseAliveGameObject_PlatformId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pObj)
        {
            pState->mPlatformId = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        pState->mPlatformId = Guid{};
    }

    if (field_11C_obj_id != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_11C_obj_id);
        if (pObj)
        {
            pState->field_44_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        pState->field_44_obj_id = Guid{};
    }

    pState->mTlvInfo = mTlvInfo;
    pState->field_48_unused = field_120_unused;
    pState->mTongueState = mTongueState;
    pState->mTongueSubState = mTongueSubState;
    pState->mEnemyXPos = mEnemyXPos;
    pState->mEnemyYPos = mEnemyYPos;
    pState->mTongueOriginX = mTongueOriginX;
    pState->mTongueOriginY = mTongueOriginY;
    pState->mTongueDestinationX = mTongueDestinationX;
    pState->mTongueDestinationY = mTongueDestinationY;
    pState->field_5A = field_188;
    pState->mTongueActive = mTongueActive;
    pState->mRenderTongue = mRenderTongue;
    pState->field_5E_brain_state = mBrainState;
    pState->field_60_state = mBrainSubState;
    pState->field_64_shrivel_timer = field_12C_shrivel_timer - sGnFrame;
    pState->field_62 = field_128;
    pState->field_68_fleech_random_idx = sFleechRandomIdx_5BC20C;
    pState->field_6A_bDidMapFollowMe = field_130_bDidMapFollowMe;
    pState->field_6C_unused = field_134_unused;
    pState->field_70_velx_factor = field_138_velx_factor;
    pState->field_74_unused = field_13C_unused;
    pState->field_76_current_anger = field_13E_current_anger;
    pState->field_78_max_anger = field_140_max_anger;
    pState->field_7A_attack_anger = field_142_attack_anger_increaser;
    pState->field_7C_wakeup_id = field_144_wake_up_switch_id;
    pState->field_7E_wake_up_switch_anger_value = field_146_wake_up_switch_anger_value;
    pState->field_80_wake_up_switch_value = field_148_wake_up_switch_value;
    pState->field_82_can_wake_up_id = field_14A_can_wake_up_switch_id;
    pState->field_84_EventXPos = field_14C_EventXPos;
    pState->field_86_ScrabParamiteEventXPos = field_14E_ScrabParamiteEventXPos;
    pState->field_88_patrol_range = field_150_patrol_range;
    pState->field_8A_old_xpos = field_152_old_xpos;
    pState->field_8C = field_154;
    pState->field_8E_rnd_crawl = field_156_rnd_crawl;
    pState->field_90_chase_delay = field_158_chase_delay;
    pState->field_92_chase_timer = field_15A_chase_timer;
    pState->field_94_lost_target_timeout = field_15C_lost_target_timeout;
    pState->field_96_lost_target_timer = field_15E_lost_target_timer;
    pState->field_98_hoistX = field_160_hoistX;
    pState->field_9A_hoistY = field_162_hoistY;
    pState->field_9C_always_0 = field_164_always_0;
    pState->field_9E_angle = field_166_angle;
    pState->field_A0_hoistY_distance = field_168_hoistY_distance;
    pState->field_A4_hoistX_distance = field_16C_hoistX_distance;

    if (field_170_danger_obj != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_170_danger_obj);
        if (pObj)
        {
            pState->field_A8 = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        pState->field_A8 = Guid{};
    }

    if (current_target_object_id_551840 == mBaseGameObjectId)
    {
        pState->field_AC_obj_id = mBaseGameObjectTlvInfo;
    }
    else
    {
        pState->field_AC_obj_id = Guid{};
    }

    pState->mHoistDone = mHoistDone;
    pState->mChasingOrScaredCrawlingLeft = mChasingOrScaredCrawlingLeft;
    pState->mShrivelDeath = mShrivelDeath;
    pState->mScaredSound = mScaredSound;
    pState->mAsleep = mAsleep;
    pState->mGoesToSleep = mGoesToSleep;
    pState->mPersistant = mPersistant;

    return sizeof(FleechSaveState);
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
        mNextMotion = -1;
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
            mNextMotion = -1;
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
            mNextMotion = -1;
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
            if (mCurrentMotion == eFleechMotions::Motion_3_Idle && GetAnimation().GetCurrentFrame() == 0 && !sCollisions->Raycast(mXPos - FP_FromInteger(5), mYPos - FP_FromInteger(5), mXPos + FP_FromInteger(5), mYPos + FP_FromInteger(1), &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                field_138_velx_factor = FP_FromInteger(0);
                BaseAliveGameObjectLastLineYPos = mYPos;
                mCurrentMotion = eFleechMotions::Motion_9_Fall;
            }
        }
        else
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
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
        if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
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

                if (mNextMotion == eFleechMotions::Motion_3_Idle)
                {
                    mCurrentMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
                    mNextMotion = -1;
                }
                else if (mNextMotion != -1)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = -1;
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
        field_13C_unused = 1;
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
        mCurrentMotion = eFleechMotions::Motion_4_Crawl;
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
    if (sCollisions->Raycast(
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
                    mCurrentMotion = eFleechMotions::Motion_10_Land;
                }
                else
                {
                    mCurrentMotion = eFleechMotions::Motion_16_DeathByFalling;
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
        mCurrentMotion = eFleechMotions::Motion_12_Climb;

        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(-1);

        const s16 yOff = GetSpriteScale() >= FP_FromInteger(1) ? 0 : -10;
        auto pHoist = static_cast<relive::Path_Hoist*>(sPathInfo->TLV_Get_At(
            field_160_hoistX,
            FP_GetExponent(mYPos - FP_FromInteger((yOff + 20))),
            field_160_hoistX,
            FP_GetExponent(mYPos - FP_FromInteger((yOff + 20))),
            ReliveTypes::eHoist));

        if (pHoist->mHoistType == relive::Path_Hoist::Type::eOffScreen)
        {
            const FP doubleYOff = FP_FromInteger(yOff + 20) * FP_FromInteger(2);
            pHoist = static_cast<relive::Path_Hoist*>(sPathInfo->TLV_Get_At(
                field_160_hoistX,
                FP_GetExponent(FP_FromInteger(pHoist->mTopLeftY) - doubleYOff),
                field_160_hoistX,
                FP_GetExponent(FP_FromInteger(pHoist->mTopLeftY) - doubleYOff),
                ReliveTypes::eHoist));

            field_162_hoistY = pHoist->mTopLeftY;
        }
        BaseAliveGameObjectLastLineYPos = mYPos;
        field_168_hoistY_distance = mYPos - FP_FromInteger(field_162_hoistY);
        BaseAliveGameObjectCollisionLine = nullptr;
        field_16C_hoistX_distance = FP_Abs(mXPos - FP_FromInteger(field_160_hoistX));

        if (GetAnimation().GetFlipX())
        {
            field_166_angle = -64;
        }
        else
        {
            field_166_angle = 64;
        }
        field_164_always_0 = 0;
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
            mCurrentMotion = eFleechMotions::Motion_13_SettleOnGround;
            return;
        }

        const FP velYOff = FP_FromInteger(field_164_always_0) * FP_FromDouble(0.045);
        mVelY -= GetSpriteScale() * (FP_FromDouble(0.7) - velYOff);
        if (field_164_always_0 > 0)
        {
            field_164_always_0--;
        }

        const FP xOff = (Math_Cosine_496CD0(field_166_angle) * (field_16C_hoistX_distance * ((mYPos - FP_FromInteger(field_162_hoistY)) / field_168_hoistY_distance)));

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
        if (sCollisions->Raycast(pX1, pY1, pX2, mVelY + mYPos, &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
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
                    field_166_angle = -128 - field_166_angle;
                    break;

                case eLineTypes::eWallRight_2:
                case eLineTypes::eBackgroundWallRight_6:
                    Sound(FleechSound::LandOnFloor_9);
                    if (field_166_angle > 64u && field_166_angle < 128u)
                    {
                        field_166_angle = -128 - field_166_angle;
                    }
                    else if (false /*field_166_angle > 128 && field_166_angle < 192*/) // always false due to the field type - og bug?
                    {
                        field_166_angle = -128 - field_166_angle;
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

        mXPos = xOff + FP_FromInteger(field_160_hoistX);
        field_166_angle = field_166_angle + 16;
        mYPos += mVelY;

        if (mYPos <= FP_FromInteger(field_162_hoistY))
        {
            mXPos = FP_FromInteger(field_160_hoistX);
            mYPos = FP_FromInteger(field_162_hoistY);
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
        if (sCollisions->Raycast(
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
    if (field_11C_obj_id == sActiveHero->mBaseGameObjectId && (sActiveHero->CantBeDamaged_44BAB0() || sActiveHero->GetInvisible()))
    {
        ToIdle();
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        PullTargetIn();
        mCurrentMotion = eFleechMotions::Motion_15_RetractTongueFromEnemey;
    }
}

void Fleech::Motion_15_RetractTongueFromEnemey()
{
    if (IsActiveHero(sObjectIds.Find_Impl(field_11C_obj_id)) && ((sActiveHero->CantBeDamaged_44BAB0()) || sActiveHero->GetInvisible()))
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

        if (mNextMotion == eFleechMotions::Motion_18_Consume)
        {
            mCurrentMotion = eFleechMotions::Motion_18_Consume;
            mNextMotion = -1;
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
        mBrainState = eFleechBrains::eBrain_3_Death;
        mShrivelDeath = true;
        mNextMotion = -1;
        field_12C_shrivel_timer = sGnFrame + 127;
        sFleechCount_5BC20E--;
    }
}

void Fleech::Motion_17_SleepingWithTongue()
{
    if (mNextMotion != -1)
    {
        SetTongueState1();
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
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

        mXPos = FP_FromInteger(field_160_hoistX) + (Math_Cosine_496CD0(field_166_angle) * FP_FromInteger(4)) - FP_FromInteger(mXOffset);
        field_166_angle += 2;
    }
}

void Fleech::Motion_18_Consume()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Sound(FleechSound::Digesting_2);
    }
    else if (GetAnimation().GetCurrentFrame() == 15 && field_11C_obj_id == sActiveHero->mBaseGameObjectId)
    {
        sActiveHero->SetAsDead_459430();

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
    field_170_danger_obj = Guid{};
    if (mTlvInfo != Guid{})
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, mHealth <= FP_FromInteger(0));
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    ResetTarget();

    if (mCurrentMotion == 18)
    {
        if (sActiveHero)
        {
            if (field_11C_obj_id == sActiveHero->mBaseGameObjectId)
            {
                sActiveHero->SetAsDead_459430();
            }
        }
    }
    field_11C_obj_id = Guid{};

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
        field_11C_obj_id = BaseGameObject::RefreshId(field_11C_obj_id);
        field_170_danger_obj = BaseGameObject::RefreshId(field_170_danger_obj);
        BaseAliveGameObject_PlatformId = BaseGameObject::RefreshId(BaseAliveGameObject_PlatformId);
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if ((FP_Abs(mXPos - sControlledCharacter->mXPos) <= FP_FromInteger(750) && FP_Abs(mYPos - sControlledCharacter->mYPos) <= FP_FromInteger(520)) || mPersistant)
    {
        const auto oldMotion = mCurrentMotion;

        mBrainSubState = InvokeMemberFunction(this, sFleechBrainTable, mBrainState);

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
            BaseAliveGameObjectPathTLV = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion == mCurrentMotion)
        {
            if (field_128)
            {
                mCurrentMotion = mPreviousMotion;
                SetAnim();
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_128 = 0;
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

void Fleech::VRender(PrimHeader** ot)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
        RenderEx(ot);
    }
}

void Fleech::RenderEx(PrimHeader** ot)
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
        const FP Tan_fp = Math_Tan_496F70(
            tongueBlock_Y[0] - tongueBlock_Y[4],
            tongueBlock_X[4] - tongueBlock_X[0]);
        const FP distanceCosine = Math_Cosine_496CD0(static_cast<u8>(FP_GetExponent(Tan_fp)));
        const FP SineTan = Math_Sine_496DD0(static_cast<u8>(FP_GetExponent(Tan_fp)));

        for (s32 i = 0; i < 4; i++)
        {
            const FP distanceXY_squareRoot_multiplied = distanceXY_squareRoot * FP_FromInteger(i + 1) * FP_FromDouble(0.25);
            const FP cosineIt_times_field188 = Math_Cosine_496CD0(static_cast<u8>(32 * (i + 1))) * FP_FromInteger(field_188);
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

            Poly_G4* currTonguePoly1 = &mTonguePolys1[i][gPsxDisplay.mBufferIndex];

            const s32 tonguePolyX1 = PsxToPCX(FP_GetExponent(currTongueBlock_X));
            const s32 tonguePolyY1 = FP_GetExponent(currTongueBlock_Y);
            const s32 tonguePolyX2 = PsxToPCX(FP_GetExponent(tongueBlock_X[i + 1]));
            const s32 tonguePolyY2 = FP_GetExponent(tongueBlock_Y[i + 1]);

            SetXY0(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX1),
                static_cast<s16>(tonguePolyY1 - 1));
            SetXY1(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX2),
                static_cast<s16>(tonguePolyY2 - 1));
            SetXY2(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX1),
                static_cast<s16>(tonguePolyY1 + 1));
            SetXY3(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX2),
                static_cast<s16>(tonguePolyY2 + 1));

            SetRGB0(
                currTonguePoly1,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB1(
                currTonguePoly1,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            SetRGB2(
                currTonguePoly1,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB3(
                currTonguePoly1,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));

            OrderingTable_Add(OtLayer(ot, GetAnimation().GetRenderLayer()), &currTonguePoly1->mBase.header);

            Poly_G4* currTonguePoly2 = &mTonguePolys2[i][gPsxDisplay.mBufferIndex];

            s32 minus_one_one_switch = -1;
            if (FP_GetExponent(Tan_fp) <= 64 || FP_GetExponent(Tan_fp) >= 192)
            {
                minus_one_one_switch = 1;
            }

            SetXY0(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX1 - minus_one_one_switch),
                static_cast<s16>(tonguePolyY1 - 1));
            SetXY1(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX2 - minus_one_one_switch),
                static_cast<s16>(tonguePolyY2 - 1));
            SetXY2(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX1 + minus_one_one_switch),
                static_cast<s16>(tonguePolyY1 + 1));
            SetXY3(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX2 + minus_one_one_switch),
                static_cast<s16>(tonguePolyY2 + 1));

            SetRGB0(
                currTonguePoly2,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB1(
                currTonguePoly2,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            SetRGB2(
                currTonguePoly2,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB3(
                currTonguePoly2,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));

            OrderingTable_Add(OtLayer(ot, GetAnimation().GetRenderLayer()), &currTonguePoly2->mBase.header);
        }
        const s32 tPage = PSX_getTPage(TPageAbr::eBlend_0);
        Init_SetTPage(&field_40C[gPsxDisplay.mBufferIndex], 1, 0, tPage);
        OrderingTable_Add(OtLayer(ot, GetAnimation().GetRenderLayer()), &field_40C[gPsxDisplay.mBufferIndex].mBase);
        return;
    }
}

void Fleech::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
        field_11C_obj_id = Guid{};
        field_170_danger_obj = Guid{};
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
        pTlv = sPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
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
        field_170_danger_obj = pNearestScrabOrParamite->mBaseGameObjectId;
        return 1;
    }

    field_170_danger_obj = Guid{};
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

void Animation_OnFrame_Fleech_449A60(BaseGameObject* pObj, u32&, const IndexedPoint& point)
{
    reinterpret_cast<Fleech*>(pObj)->VOnFrame(point.mPoint);
}

void Fleech::Init()
{
    Animation_Init(GetAnimRes(AnimId::Fleech_Idle));

    GetAnimation().SetFnPtrArray(kFleech_Anim_Frame_Fns_55EFD0);

    SetType(ReliveTypes::eFleech);

    SetCanExplode(true);
    SetCanSetOffExplosives(true);

    mShrivelDeath = false;
    mScaredSound = false;

    SetDoPurpleLightEffect(true);

    field_12C_shrivel_timer = 0;
    mBrainSubState = 0;
    mNextMotion = -1;
    BaseAliveGameObject_PlatformId = Guid{};
    field_128 = 0;
    field_11C_obj_id = Guid{};
    field_170_danger_obj = Guid{};
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
    if (sCollisions->Raycast(
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
        for (s32 j = 0; j < 2; j++)
        {
            PolyG4_Init(&mTonguePolys1[i][j]);
            SetRGB0(&mTonguePolys1[i][j], 150, 100, 100);
            SetRGB1(&mTonguePolys1[i][j], 150, 100, 100);
            SetRGB2(&mTonguePolys1[i][j], 150, 100, 100);
            SetRGB3(&mTonguePolys1[i][j], 150, 100, 100);
            Poly_Set_SemiTrans(&mTonguePolys1[i][j].mBase.header, false);

            PolyG4_Init(&mTonguePolys2[i][j]);
            SetRGB0(&mTonguePolys2[i][j], 150, 100, 100);
            SetRGB1(&mTonguePolys2[i][j], 150, 100, 100);
            SetRGB2(&mTonguePolys2[i][j], 150, 100, 100);
            SetRGB3(&mTonguePolys2[i][j], 150, 100, 100);
            Poly_Set_SemiTrans(&mTonguePolys2[i][j].mBase.header, true);
        }
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
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
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
                    mTongueDestinationX = field_160_hoistX;
                    mTongueDestinationY = field_162_hoistY;
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
                        mNextMotion = eFleechMotions::Motion_18_Consume;
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
                        if (IsActiveHero(pTarget))
                        {
                            sActiveHero->FleechDeath_459350();
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
    mCurrentMotion = eFleechMotions::Motion_3_Idle;
    mNextMotion = -1;
    field_134_unused = 60 * gRandomBytes[sFleechRandomIdx_5BC20C++] / 256 + sGnFrame + 120;
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
        defaultSndIdxVol = effectDef.field_C_default_volume + Math_RandomRange(-10, 10);
        return effectDef;
    }
    else
    {
        const relive::SfxDefinition& effectDef = getSfxDef(soundId);
        defaultSndIdxVol = effectDef.field_C_default_volume;
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
        effectDef.field_E_pitch_min,
        effectDef.field_10_pitch_max);
}

u8** Fleech::ResBlockForMotion(s32 /*motion*/)
{
    return mBaseGameObjectResArray.ItemAt(0);
}

s16 Fleech::CanMove()
{
    if (mNextMotion == eFleechMotions::Motion_6_Knockback)
    {
        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        mNextMotion = -1;
        return 1;
    }

    if (mNextMotion != eFleechMotions::Motion_4_Crawl)
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
        mCurrentMotion = eFleechMotions::Motion_4_Crawl;
        mNextMotion = -1;
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

    auto pStopper = static_cast<relive::Path_EnemyStopper*>(sPathInfo->TLV_Get_At(
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

    auto pSlamDoor = static_cast<relive::Path_SlamDoor*>(sPathInfo->TLV_Get_At(
        FP_GetExponent(slamDoorXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(slamDoorXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eSlamDoor));

    return (pSlamDoor && ((pSlamDoor->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoor->mSwitchId)) || (pSlamDoor->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoor->mSwitchId))));
}

s32 Fleech::UpdateWakeUpSwitchValue()
{
    const s16 curSwitchValue = static_cast<s16>(SwitchStates_Get(field_144_wake_up_switch_id));
    const s16 wakeUpValue = field_148_wake_up_switch_value;

    if (curSwitchValue == wakeUpValue)
    {
        return 0;
    }

    if (curSwitchValue)
    {
        field_148_wake_up_switch_value = curSwitchValue;
        return (curSwitchValue - wakeUpValue) ? 1 : 0;
    }
    else
    {
        field_148_wake_up_switch_value = 0;
        return 1;
    }
}

s16 Fleech::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 0;
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
            mBrainState = eFleechBrains::eBrain_3_Death;
            mNextMotion = -1;
            field_12C_shrivel_timer = sGnFrame + 127;
            mCurrentMotion = eFleechMotions::Motion_3_Idle;
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
            mBrainState = eFleechBrains::eBrain_3_Death;
            mCurrentMotion = eFleechMotions::Motion_3_Idle;
            field_12C_shrivel_timer = sGnFrame + 127;
            mNextMotion = -1;
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
            mBrainState = eFleechBrains::eBrain_3_Death;
            break;

        default:
            Sound(FleechSound::Scared_7);
            SetElectrocuted(true);
            sub_42B8C0();
            break;
    }
    return 1;
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
            if ((!IsActiveHero(pEvent) || !sActiveHero->GetInvisible()) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, pEvent->mXPos, pEvent->mYPos, 0))
            {
                field_13E_current_anger += field_142_attack_anger_increaser;
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
                if ((!IsActiveHero(pEvent) || !sActiveHero->GetInvisible()) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, pEvent->mXPos, pEvent->mYPos, 0))
                {
                    field_13E_current_anger += field_140_max_anger;
                }
            }
        }
    }
}

s16 Fleech::AngerFleech(IBaseAliveGameObject* pObj)
{
    if (!pObj || (IsActiveHero(pObj) && sActiveHero->GetInvisible()))
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
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
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
    mTongueDestinationX = FP_GetExponent(((FP_FromInteger(field_160_hoistX)) + mXPos) / FP_FromInteger(2));
    field_188 = 0;
    mTongueDestinationY = FP_GetExponent(((FP_FromInteger(field_162_hoistY)) + mYPos) / FP_FromInteger(2));
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
        else if (mBrainState != eFleechBrains::eBrain_0_Patrol)
        {
            VOnTrapDoorOpen();
            field_138_velx_factor = FP_FromInteger(0);
            BaseAliveGameObjectLastLineYPos = mYPos;
            mXPos = prev_xpos + mVelX;
            mCurrentMotion = eFleechMotions::Motion_9_Fall;
        }
        else
        {
            mXPos = prev_xpos;
            mYPos = prev_ypos;
            BaseAliveGameObjectCollisionLine = pOldLine;
            mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = prev_ypos;
        field_138_velx_factor = FP_FromInteger(0);
        mCurrentMotion = eFleechMotions::Motion_9_Fall;
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
    field_13E_current_anger += field_142_attack_anger_increaser;
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
    return sCollisions->Raycast(x1, y1, x2, y2, &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor) == 0;
}

relive::Path_Hoist* Fleech::TryGetHoist(s32 xDistance, s16 bIgnoreDirection)
{
    if (mCurrentMotion == eFleechMotions::Motion_9_Fall)
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

    auto pHoist = static_cast<relive::Path_Hoist*>(sPathInfo->TLV_Get_At(
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

const s8 byte_551984[] = {
    0,
    0,
    5,
    0,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    0,
    0,
    0,
    0};

enum Brain_0_Patrol
{
    State_0_Init = 0,
    eSleeping_1 = 1,
    State_2 = 2,
    eGoingBackToSleep = 3,
    eAlerted_4 = 4,
    eHearingScrabOrParamite_5 = 5,
    State_6 = 6,
    State_7 = 7,
    eAlertedByAbe_8 = 8,
    State_9 = 9,
    State_10 = 10,
};

s16 Fleech::Brain_0_Patrol()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
    if (!pTarget || pTarget->GetDead() || pTarget->mHealth <= FP_FromInteger(0) || pTarget->GetInvisible())
    {
        field_11C_obj_id = Guid{};
        pTarget = nullptr;
    }

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        MusicController::static_PlayMusic(static_cast<MusicController::MusicTypes>(byte_551984[mBrainSubState]), this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_0_Patrol::State_0_Init:
            return Brain_Patrol_State_0();

        case Brain_0_Patrol::eSleeping_1:
            return Brain_Patrol_State_1();

        case Brain_0_Patrol::State_2:
            return Brain_Patrol_State_2();

        case Brain_0_Patrol::eGoingBackToSleep:
            return Brain_Patrol_State_3();

        case Brain_0_Patrol::eAlerted_4:
            return Brain_Patrol_State_4(pTarget);

        case Brain_0_Patrol::eHearingScrabOrParamite_5:
            return Brain_Patrol_State_5();

        case Brain_0_Patrol::State_6:
            return Brain_Patrol_State_6();

        case Brain_0_Patrol::State_7:
            return Brain_Patrol_State_7();

        case Brain_0_Patrol::eAlertedByAbe_8:
            return Brain_Patrol_State_8(pTarget);

        case Brain_0_Patrol::State_9:
            return Brain_Patrol_State_9();

        case Brain_0_Patrol::State_10:
            return Brain_Patrol_State_10();

        default:
            return mBrainSubState;
    }
}

s16 Fleech::Brain_Patrol_State_0()
{
    field_156_rnd_crawl = Fleech_NextRandom() & 0x3F;
    field_15A_chase_timer = 0;
    field_152_old_xpos = FP_GetExponent(mXPos);
    field_14C_EventXPos = -1;
    field_14E_ScrabParamiteEventXPos = -1;

    if (field_13E_current_anger > field_140_max_anger)
    {
        return Brain_0_Patrol::eAlerted_4;
    }

    if (mCurrentMotion == eFleechMotions::Motion_0_Sleeping)
    {
        return Brain_0_Patrol::eSleeping_1;
    }

    if (mCurrentMotion != eFleechMotions::Motion_17_SleepingWithTongue && !mGoesToSleep)
    {
        return Brain_0_Patrol::eAlerted_4;
    }

    if (mCurrentMotion == eFleechMotions::Motion_0_Sleeping || mCurrentMotion == eFleechMotions::Motion_17_SleepingWithTongue)
    {
        return Brain_0_Patrol::eSleeping_1;
    }

    return Brain_0_Patrol::eGoingBackToSleep;
}

s16 Fleech::Brain_Patrol_State_1()
{
    if (!SwitchStates_Get(field_14A_can_wake_up_switch_id))
    {
        return mBrainSubState;
    }

    if (!(static_cast<s32>(sGnFrame) % 16))
    {
        if (field_13E_current_anger > 0)
        {
            field_13E_current_anger--;
        }
    }

    IncreaseAnger();

    if (UpdateWakeUpSwitchValue())
    {
        const s16 v11 = (field_142_attack_anger_increaser - field_140_max_anger) / 2;
        if (field_13E_current_anger < v11 + field_140_max_anger)
        {
            field_13E_current_anger = field_140_max_anger + v11;
        }
    }

    if (!IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(8)))
    {
        if (field_13E_current_anger <= field_140_max_anger)
        {
            return mBrainSubState;
        }

        if (mCurrentMotion == eFleechMotions::Motion_17_SleepingWithTongue)
        {
            mNextMotion = eFleechMotions::Motion_9_Fall;
            BaseAliveGameObjectLastLineYPos = mYPos;
            return Brain_0_Patrol::eAlerted_4;
        }
    }
    else
    {
        BaseGameObject* pDangerObj = sObjectIds.Find_Impl(field_170_danger_obj);
        if (pDangerObj && pDangerObj->Type() != ReliveTypes::eParamite)
        {
            if (mCurrentMotion == eFleechMotions::Motion_17_SleepingWithTongue)
            {
                mNextMotion = eFleechMotions::Motion_9_Fall;
                BaseAliveGameObjectLastLineYPos = mYPos;
                return Brain_0_Patrol::eAlerted_4;
            }
        }
        else
        {
            field_170_danger_obj = Guid{};
            if (field_13E_current_anger <= field_140_max_anger)
            {
                return mBrainSubState;
            }

            if (mCurrentMotion == eFleechMotions::Motion_17_SleepingWithTongue)
            {
                mNextMotion = eFleechMotions::Motion_9_Fall;
                BaseAliveGameObjectLastLineYPos = mYPos;
                return Brain_0_Patrol::eAlerted_4;
            }
        }
    }
    mNextMotion = eFleechMotions::Motion_1_WakingUp;
    return Brain_0_Patrol::State_2;
}

s16 Fleech::Brain_Patrol_State_2()
{
    if (mCurrentMotion != eFleechMotions::Motion_1_WakingUp)
    {
        return mBrainSubState;
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        mNextMotion = eFleechMotions::Motion_5_PatrolCry;
    }
    else
    {
        mNextMotion = eFleechMotions::Motion_9_Fall;
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
    return Brain_0_Patrol::eAlerted_4;
}

s16 Fleech::Brain_Patrol_State_3()
{
    if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
    {
        mNextMotion = eFleechMotions::Motion_2_Unknown;
    }

    if (mCurrentMotion != eFleechMotions::Motion_2_Unknown)
    {
        return mBrainSubState;
    }

    field_13E_current_anger = 0;
    mNextMotion = eFleechMotions::Motion_0_Sleeping;
    return Brain_0_Patrol::eSleeping_1;
}

s16 Fleech::Brain_Patrol_State_4(IBaseAliveGameObject* pTarget)
{
    if (field_11C_obj_id == Guid{})
    {
        pTarget = FindMudOrAbe();
        if (pTarget)
        {
            field_11C_obj_id = pTarget->mBaseGameObjectId;
        }
    }

    if (!(static_cast<s32>(sGnFrame) % 32))
    {
        if (field_13E_current_anger > 0)
        {
            field_13E_current_anger--;
        }
    }

    if (IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(8)))
    {
        auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
        if (pDangerObj == sControlledCharacter)
        {
            mBrainState = eFleechBrains::eBrain_2_Scared;
            return 0;
        }

        if (VIsObjNearby(FP_FromInteger(2) * ScaleToGridSize(GetSpriteScale()), pDangerObj))
        {
            const s16 v27 = (field_142_attack_anger_increaser - field_140_max_anger) / 2;
            if (field_13E_current_anger < v27 + field_140_max_anger)
            {
                field_13E_current_anger = field_140_max_anger + v27;
            }
            field_14E_ScrabParamiteEventXPos = FP_GetExponent(pDangerObj->mXPos); // TODO: abs ?
        }
    }

    // TODO: Check OFSUB branches
    if (field_14E_ScrabParamiteEventXPos >= 0)
    {
        if ((FP_FromInteger(field_14E_ScrabParamiteEventXPos) > mXPos && !GetAnimation().GetFlipX()) || (FP_FromInteger(field_14E_ScrabParamiteEventXPos) < mXPos && GetAnimation().GetFlipX()))
        {
            if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
            {
                mCurrentMotion = eFleechMotions::Motion_7_StopCrawling;
            }
            else if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
            }
        }
        return Brain_0_Patrol::eHearingScrabOrParamite_5;
    }

    IncreaseAnger();

    if (AngerFleech(pTarget))
    {
        field_13E_current_anger += field_142_attack_anger_increaser;
    }

    if (UpdateWakeUpSwitchValue())
    {
        field_13E_current_anger += field_146_wake_up_switch_anger_value;
    }

    if (pTarget)
    {
        if (!pTarget->GetInvisible() && VOnSameYLevel(pTarget) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, pTarget->mXPos, pTarget->mYPos, 0) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pTarget->mXPos - mXPos))
        {
            field_13E_current_anger = field_142_attack_anger_increaser + 1;
            return Brain_0_Patrol::eAlertedByAbe_8;
        }

        if (field_13E_current_anger > field_142_attack_anger_increaser && !pTarget->GetInvisible() && field_170_danger_obj == Guid{})
        {
            return Brain_0_Patrol::eAlertedByAbe_8;
        }
    }

    field_15A_chase_timer = 0;
    relive::Path_Hoist* pHoist = TryGetHoist(0, 0);
    if (pHoist)
    {
        if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
        {
            mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
        }
        else if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
        {
            mNextMotion = eFleechMotions::Motion_3_Idle;
        }
        field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
        field_162_hoistY = pHoist->mTopLeftY;
        return 9;
    }

    if (!(Fleech_NextRandom() % 32) && mCurrentMotion == eFleechMotions::Motion_3_Idle)
    {
        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        return mBrainSubState;
    }

    if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
    {
        if (GetAnimation().GetFlipX())
        {
            if (mXPos < FP_FromInteger(field_154))
            {
                mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
            }
        }
        else if (mXPos > FP_FromInteger(field_154))
        {
            mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
        }
    }

    if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
    {
        if (field_14C_EventXPos >= 0)
        {
            if (field_150_patrol_range > 0)
            {
                if (FP_FromInteger(field_14C_EventXPos) <= mXPos)
                {
                    s16 patrolRangeDelta = FP_GetExponent(mXPos) - field_150_patrol_range;
                    if (field_14C_EventXPos > patrolRangeDelta)
                    {
                        patrolRangeDelta = field_14C_EventXPos;
                    }
                    field_154 = patrolRangeDelta;

                    if (!GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                    }
                }
                else
                {
                    s16 patrolRangeDelta = field_150_patrol_range + FP_GetExponent(mXPos);
                    if (field_14C_EventXPos <= patrolRangeDelta)
                    {
                        patrolRangeDelta = field_14C_EventXPos;
                    }
                    field_154 = patrolRangeDelta;

                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                    }
                }

                mNextMotion = eFleechMotions::Motion_4_Crawl;
                field_14C_EventXPos = -1;
            }
            else
            {
                // TODO: Check __OFSUB__ on else branch
                if (FP_FromInteger(field_14C_EventXPos) > mXPos)
                {
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                        field_14C_EventXPos = -1;
                    }
                }
                else if (FP_FromInteger(field_14C_EventXPos) < mXPos)
                {
                    if (!GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                        field_14C_EventXPos = -1;
                    }
                }
            }
        }
        else
        {
            if (field_150_patrol_range > 0)
            {
                if (field_156_rnd_crawl > 0)
                {
                    field_156_rnd_crawl--;
                }
                else
                {
                    if (GetAnimation().GetFlipX())
                    {
                        field_154 = FP_GetExponent(mXPos) - Fleech_NextRandom() * (FP_GetExponent(mXPos) + field_150_patrol_range - field_152_old_xpos) / 255;
                    }
                    else
                    {
                        field_154 = FP_GetExponent(mXPos) + Fleech_NextRandom() * (field_150_patrol_range + field_152_old_xpos - FP_GetExponent(mXPos)) / 255;
                    }
                    field_156_rnd_crawl = Fleech_NextRandom() & 0x3F;
                    mNextMotion = eFleechMotions::Motion_4_Crawl;
                }
            }
        }
    }

    if (!mGoesToSleep || (field_13E_current_anger >= field_140_max_anger && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0)))
    {
        if ((Fleech_NextRandom() % 64) || mCurrentMotion != eFleechMotions::Motion_3_Idle)
        {
            return mBrainSubState;
        }
        mCurrentMotion = eFleechMotions::Motion_5_PatrolCry;
        return mBrainSubState;
    }
    field_13E_current_anger = 0;
    mNextMotion = eFleechMotions::Motion_2_Unknown;
    return Brain_0_Patrol::eGoingBackToSleep;
}

s16 Fleech::Brain_Patrol_State_5()
{
    if (mCurrentMotion == eFleechMotions::Motion_7_StopCrawling || mCurrentMotion == eFleechMotions::Motion_6_Knockback)
    {
        return mBrainSubState;
    }

    if ((FP_FromInteger(field_14E_ScrabParamiteEventXPos) > mXPos && !GetAnimation().GetFlipX()) ||
        (FP_FromInteger(field_14E_ScrabParamiteEventXPos) < mXPos && GetAnimation().GetFlipX()))
    {
        if (mCurrentMotion != eFleechMotions::Motion_4_Crawl)
        {
            if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
            }
        }
        else
        {
            mCurrentMotion = eFleechMotions::Motion_7_StopCrawling;
        }
        return mBrainSubState;
    }

    field_14E_ScrabParamiteEventXPos = -1;
    mNextMotion = eFleechMotions::Motion_4_Crawl;
    CanMove();
    return Brain_0_Patrol::State_6;
}

s16 Fleech::Brain_Patrol_State_6()
{
    auto pDangerObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (mCurrentMotion != eFleechMotions::Motion_4_Crawl ||
        (pDangerObj && (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5), pDangerObj))))
    {
        if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
        {
            return mBrainSubState;
        }
    }
    else
    {
        mNextMotion = eFleechMotions::Motion_3_Idle;
    }
    return Brain_0_Patrol::State_7;
}

s16 Fleech::Brain_Patrol_State_7()
{
    if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
    {
        return mBrainSubState;
    }
    return Brain_0_Patrol::eAlerted_4;
}

s16 Fleech::Brain_Patrol_State_8(IBaseAliveGameObject* pTarget)
{
    if (IsActiveHero(pTarget) && sActiveHero->GetInvisible())
    {
        return Brain_0_Patrol::State_0_Init;
    }

    if (field_15A_chase_timer < field_158_chase_delay)
    {
        field_15A_chase_timer++;
        return mBrainSubState;
    }

    mBrainState = eFleechBrains::eBrain_1_ChasingAbe;
    return Brain_0_Patrol::State_0_Init;
}

s16 Fleech::Brain_Patrol_State_9()
{
    if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
    {
        return mBrainSubState;
    }
    if ((GetAnimation().GetFlipX() && FP_FromInteger(field_160_hoistX) > mXPos) || (!GetAnimation().GetFlipX() && FP_FromInteger(field_160_hoistX) < mXPos))
    {
        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        return mBrainSubState;
    }
    mNextMotion = eFleechMotions::Motion_11_RaiseHead;
    return Brain_0_Patrol::State_10;
}

s16 Fleech::Brain_Patrol_State_10()
{
    if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
    {
        return mBrainSubState;
    }
    field_152_old_xpos = FP_GetExponent(mXPos);
    return Brain_0_Patrol::eAlerted_4;
}

const s8 byte_551784[] = {
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    0};

enum Brain_1_ChasingAbe
{
    eInit_0 = 0,
    eChasingAbe_1 = 1,
    eUnknown_2 = 2,
    eContinueChaseAfterFall_3 = 3,
    eBlockedByWall_4 = 4,
    eUnknown_5 = 5,
    eScrabOrParamiteNearby_6 = 6,
    eUnknown_7 = 7,
    eFleechUnknown_8 = 8,
    eUnknown_9 = 9,
    eAbeIsInTongueRange_10 = 10,
    eIsAbeDead_11 = 11,
    eUnknown_12 = 12,
    eBackToPatrol_13 = 13,
    ePrepareToHoist_14 = 14,
    eHoistDone_15 = 15,
    eGoBackToChasingAbe_16 = 16
};

s16 Fleech::Brain_1_ChasingAbe()
{
    auto pObj = static_cast<IBaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
    if (pObj)
    {
        if (pObj->GetDead() || (IsActiveHero(pObj) && sActiveHero->GetInvisible()))
        {
            field_11C_obj_id = Guid{};
            pObj = nullptr;
        }
    }

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        MusicController::static_PlayMusic(static_cast<MusicController::MusicTypes>(byte_551784[mBrainSubState]), this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_1_ChasingAbe::eInit_0:
            return Brain_ChasingAbe_State_0(pObj);
        case Brain_1_ChasingAbe::eChasingAbe_1:
            return Brain_ChasingAbe_State_1(pObj);
        case Brain_1_ChasingAbe::eUnknown_2:
            return Brain_ChasingAbe_State_2(pObj);
        case Brain_1_ChasingAbe::eContinueChaseAfterFall_3:
        case Brain_1_ChasingAbe::eGoBackToChasingAbe_16:
            if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                return mBrainSubState;
            }
            return Brain_1_ChasingAbe::eChasingAbe_1;

        case Brain_1_ChasingAbe::eBlockedByWall_4:
        {
            if (!pObj || pObj->mHealth <= FP_FromInteger(0))
            {
                return Brain_1_ChasingAbe::eBackToPatrol_13;
            }

            IBaseAliveGameObject* pMudOrAbe = FindMudOrAbe();
            if (pMudOrAbe)
            {
                if (pMudOrAbe->mBaseGameObjectId != field_11C_obj_id)
                {
                    pObj = pMudOrAbe;
                    field_11C_obj_id = pMudOrAbe->mBaseGameObjectId;
                }
            }

            mChasingOrScaredCrawlingLeft = false;
            if (!WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(GetSpriteScale())))
            {
                return Brain_1_ChasingAbe::eChasingAbe_1;
            }

            relive::Path_Hoist* pHoist = TryGetHoist(0, false);
            if (pHoist)
            {
                mNextMotion = eFleechMotions::Motion_3_Idle;
                field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() < FP_FromInteger(1) ? 6 : 12);
                field_162_hoistY = pHoist->mTopLeftY;
                return Brain_1_ChasingAbe::ePrepareToHoist_14;
            }
            [[fallthrough]];
        }

        case Brain_1_ChasingAbe::eUnknown_5:
            if (!pObj || pObj->mHealth <= FP_FromInteger(0))
            {
                return Brain_1_ChasingAbe::eBackToPatrol_13;
            }

            if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                return mBrainSubState;
            }

            if (IsActiveHero(pObj) &&
                VOnSameYLevel(sActiveHero) &&
                gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, sActiveHero->mXPos, sActiveHero->mYPos, 0) &&
                gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) &&
                !WallHit(FP_FromInteger((GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5)), sActiveHero->mXPos - mXPos))
            {
                return Brain_1_ChasingAbe::eChasingAbe_1;
            }

            if (!IsNear(pObj))
            {
                return Brain_1_ChasingAbe::eUnknown_2;
            }

            if (mChasingOrScaredCrawlingLeft)
            {
                if (pObj->mXPos <= mXPos - FP_FromInteger(2))
                {
                    return mBrainSubState;
                }
            }
            else
            {
                if (pObj->mXPos >= mXPos + FP_FromInteger(2))
                {
                    return mBrainSubState;
                }
            }
            return Brain_1_ChasingAbe::eChasingAbe_1;

        case Brain_1_ChasingAbe::eScrabOrParamiteNearby_6:
            if (mCurrentMotion != eFleechMotions::Motion_7_StopCrawling && mCurrentMotion != eFleechMotions::Motion_6_Knockback && mNextMotion != -1)
            {
                return mBrainSubState;
            }
            mNextMotion = eFleechMotions::Motion_4_Crawl;
            return Brain_1_ChasingAbe::eUnknown_7;

        case Brain_1_ChasingAbe::eUnknown_7:
        {
            auto v70 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
            if (mCurrentMotion != eFleechMotions::Motion_4_Crawl || (v70 && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5), v70)))
            {
                if (mCurrentMotion != eFleechMotions::Motion_3_Idle && mNextMotion != -1)
                {
                    return mBrainSubState;
                }
                mNextMotion = eFleechMotions::Motion_6_Knockback;
            }
            else
            {
                mNextMotion = eFleechMotions::Motion_7_StopCrawling;
            }
            return Brain_1_ChasingAbe::eUnknown_9;
        }

        case Brain_1_ChasingAbe::eFleechUnknown_8:
            if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                return mBrainSubState;
            }
            mNextMotion = eFleechMotions::Motion_4_Crawl;
            return Brain_1_ChasingAbe::eUnknown_9;

        case Brain_1_ChasingAbe::eUnknown_9:
            return Brain_ChasingAbe_State_9(pObj);

        case Brain_1_ChasingAbe::eAbeIsInTongueRange_10:
            if (pObj)
            {
                if (mCurrentMotion == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                {
                    return Brain_1_ChasingAbe::eIsAbeDead_11;
                }
                else if (mNextMotion == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                {
                    return mBrainSubState;
                }

                ResetTarget();
                return Brain_1_ChasingAbe::eChasingAbe_1;
            }
            sub_42B8C0();
            return Brain_1_ChasingAbe::eBackToPatrol_13;

        case Brain_1_ChasingAbe::eIsAbeDead_11:
            if (pObj)
            {
                if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
                {
                    return mBrainSubState;
                }

                ResetTarget();

                if (pObj->mHealth <= FP_FromInteger(0))
                {
                    return Brain_1_ChasingAbe::eBackToPatrol_13;
                }

                if (VIsObj_GettingNear_On_X(pObj))
                {
                    mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                    return Brain_1_ChasingAbe::eUnknown_12;
                }

                return Brain_1_ChasingAbe::eChasingAbe_1;
            }
            sub_42B8C0();
            return Brain_1_ChasingAbe::eBackToPatrol_13;

        case Brain_1_ChasingAbe::eUnknown_12:
            if (pObj)
            {
                if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
                {
                    return mBrainSubState;
                }
                if (!Collision(1))
                {
                    mNextMotion = eFleechMotions::Motion_4_Crawl;
                }
                return Brain_1_ChasingAbe::eChasingAbe_1;
            }
            sub_42B8C0();
            return Brain_1_ChasingAbe::eBackToPatrol_13;

        case Brain_1_ChasingAbe::eBackToPatrol_13:
            mNextMotion = eFleechMotions::Motion_3_Idle;
            mBrainState = eFleechBrains::eBrain_0_Patrol;
            field_13E_current_anger = field_140_max_anger + (field_142_attack_anger_increaser - field_140_max_anger) / 2;
            return Brain_1_ChasingAbe::eInit_0;

        case Brain_1_ChasingAbe::ePrepareToHoist_14:
            if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                return mBrainSubState;
            }
            if ((GetAnimation().GetFlipX() && FP_FromInteger(field_160_hoistX) > mXPos) || (!GetAnimation().GetFlipX() && FP_FromInteger(field_160_hoistX) < mXPos))
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                return mBrainSubState;
            }
            mNextMotion = eFleechMotions::Motion_11_RaiseHead;
            return Brain_1_ChasingAbe::eHoistDone_15;

        case Brain_1_ChasingAbe::eHoistDone_15:
            if (mCurrentMotion != eFleechMotions::Motion_13_SettleOnGround)
            {
                return mBrainSubState;
            }
            return Brain_1_ChasingAbe::eGoBackToChasingAbe_16;

        default:
            return mBrainSubState;
    }
}

s16 Fleech::Brain_ChasingAbe_State_9(IBaseAliveGameObject* pObj)
{
    if (!IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(6)))
    {
        return 1;
    }

    auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (pDangerObj == sControlledCharacter)
    {
        if (Collision(1) || HandleEnemyStopperOrSlamDoor(1) || WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(GetAnimation().GetFlipX() != 0 ? -1 : 1)))
        {
            mCurrentMotion = eFleechMotions::Motion_6_Knockback;
            mBrainState = eFleechBrains::eBrain_2_Scared;
            return 0;
        }
    }

    if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
    {
        mNextMotion = eFleechMotions::Motion_3_Idle;
        return mBrainSubState;
    }

    if (IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5)))
    {
        auto v82 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
        if (VIsFacingMe(v82))
        {
            mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        }
        return 8;
    }

    if (!pObj || !IsNear(pObj))
    {
        return 13;
    }

    if (pObj->mXPos < mXPos)
    {
        if (pDangerObj->mXPos > mXPos)
        {
            return 1;
        }
    }

    if (pDangerObj->mXPos < mXPos)
    {
        return 1;
    }

    if (pObj->mXPos <= mXPos)
    {
        if (pDangerObj->mXPos > mXPos)
        {
            return 1;
        }
    }

    if (!(Fleech_NextRandom() % 32) && mCurrentMotion == eFleechMotions::Motion_3_Idle)
    {
        mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        return mBrainSubState;
    }

    if ((Fleech_NextRandom() % 64) || mCurrentMotion != eFleechMotions::Motion_3_Idle)
    {
        return mBrainSubState;
    }
    mCurrentMotion = eFleechMotions::Motion_5_PatrolCry;
    return mBrainSubState;
}

s16 Fleech::Brain_ChasingAbe_State_2(IBaseAliveGameObject* pObj)
{
    if (!pObj || pObj->mHealth <= FP_FromInteger(0))
    {
        return Brain_1_ChasingAbe::eBackToPatrol_13;
    }

    if (IsNear(pObj))
    {
        return Brain_1_ChasingAbe::eChasingAbe_1;
    }

    if (VIsFacingMe(pObj) || mCurrentMotion == eFleechMotions::Motion_7_StopCrawling || mCurrentMotion == eFleechMotions::Motion_6_Knockback)
    {
        if (mCurrentMotion != eFleechMotions::Motion_4_Crawl)
        {
            mNextMotion = eFleechMotions::Motion_4_Crawl;
        }

        if (IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5)))
        {
            auto v56 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
            if (VIsFacingMe(v56))
            {
                mNextMotion = eFleechMotions::Motion_7_StopCrawling;
            }
            Sound(FleechSound::Scared_7);
            return Brain_1_ChasingAbe::eScrabOrParamiteNearby_6;
        }

        if (field_15E_lost_target_timer < field_15C_lost_target_timeout)
        {
            field_15E_lost_target_timer++;
            relive::Path_Hoist* pHoist = TryGetHoist(1, false);
            if (pHoist)
            {
                mNextMotion = eFleechMotions::Motion_3_Idle;
                field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() < FP_FromInteger(1) ? 6 : 12);
                field_162_hoistY = pHoist->mTopLeftY;
                return 14;
            }

            if (Fleech_NextRandom() % 64)
            {
                return mBrainSubState;
            }

            mCurrentMotion = eFleechMotions::Motion_5_PatrolCry;
            return mBrainSubState;
        }

        field_15E_lost_target_timer = 0;
        mBrainState = eFleechBrains::eBrain_0_Patrol;
        field_13E_current_anger = field_142_attack_anger_increaser - 1;
        return Brain_1_ChasingAbe::eInit_0;
    }
    else if (mCurrentMotion == eFleechMotions::Motion_4_Crawl) // TODO: Check v52 was cur motion
    {
        mNextMotion = eFleechMotions::Motion_7_StopCrawling;
        return mBrainSubState;
    }
    else
    {
        mNextMotion = eFleechMotions::Motion_6_Knockback;
        return mBrainSubState;
    }
}

s16 Fleech::Brain_ChasingAbe_State_0(IBaseAliveGameObject* pObj)
{
    if (!pObj)
    {
        field_11C_obj_id = Guid{};
        IBaseAliveGameObject* pMudOrAbe = FindMudOrAbe();
        if (!pMudOrAbe)
        {
            return 13;
        }
        field_11C_obj_id = pMudOrAbe->mBaseGameObjectId;
    }
    field_120_unused = 0;
    field_15E_lost_target_timer = 0;
    mNextMotion = eFleechMotions::Motion_4_Crawl;
    Sound(FleechSound::PatrolCry_0);
    return 1;
}

s16 Fleech::Brain_ChasingAbe_State_1(IBaseAliveGameObject* pObj)
{
    if (!pObj || pObj->mHealth <= FP_FromInteger(0))
    {
        return Brain_1_ChasingAbe::eBackToPatrol_13;
    }

    // Is moving?
    if (mVelX != FP_FromInteger(0))
    {
        // Check for blocked by wall
        const FP k1Directed = FP_FromInteger((GetAnimation().GetFlipX()) != 0 ? -1 : 1);
        if (WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(GetSpriteScale()) * k1Directed))
        {
            mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
            if (mVelX < FP_FromInteger(0))
            {
                mChasingOrScaredCrawlingLeft = true;
            }
            return Brain_1_ChasingAbe::eBlockedByWall_4;
        }

        // Check for enemy stopper or slam door
        if (HandleEnemyStopperOrSlamDoor(1))
        {
            mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
            if (mVelX < FP_FromInteger(0))
            {
                mChasingOrScaredCrawlingLeft = true;
            }

            FP xOffset = mXPos;
            if (GetAnimation().GetFlipX())
            {
                xOffset -= ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                xOffset += ScaleToGridSize(GetSpriteScale());
            }

            FP slamDoorX = xOffset;
            if (mXPos <= xOffset)
            {
                slamDoorX = mXPos;
            }

            FP slamDoorW = mXPos;
            if (mXPos <= xOffset)
            {
                slamDoorW = xOffset;
            }

            relive::Path_TLV* pSlamDoor = sPathInfo->TLV_Get_At(
                FP_GetExponent(slamDoorX),
                FP_GetExponent(mYPos),
                FP_GetExponent(slamDoorW),
                FP_GetExponent(mYPos),
                ReliveTypes::eSlamDoor);

            if (pSlamDoor)
            {
                return Brain_1_ChasingAbe::eBlockedByWall_4;
            }
            return Brain_1_ChasingAbe::eUnknown_5;
        }
    }

    // Check for danger object
    if (IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5)))
    {
        auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
        if (VIsFacingMe(pDangerObj))
        {
            if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
            {
                mNextMotion = eFleechMotions::Motion_7_StopCrawling;
                Sound(FleechSound::Scared_7);
                return Brain_1_ChasingAbe::eScrabOrParamiteNearby_6;
            }

            if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
            {
                mNextMotion = eFleechMotions::Motion_6_Knockback;
            }
        }

        Sound(FleechSound::Scared_7);
        return Brain_1_ChasingAbe::eScrabOrParamiteNearby_6;
    }

    // Check for food object
    IBaseAliveGameObject* pAbeOrMud = FindMudOrAbe();
    if (pAbeOrMud)
    {
        if (pAbeOrMud->mBaseGameObjectId != field_11C_obj_id)
        {
            pObj = pAbeOrMud;
            field_11C_obj_id = pAbeOrMud->mBaseGameObjectId;
        }
    }

    // Can we get to them on this level?
    if (VOnSameYLevel(pObj))
    {
        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pObj))
        {
            if (pObj->GetSpriteScale() == GetSpriteScale() &&
                VIsFacingMe(pObj) &&
                !WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pObj->mXPos - mXPos) &&
                GotNoTarget() &&
                gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                SetTarget();
                mNextMotion = eFleechMotions::Motion_14_ExtendTongueFromEnemy;
                return Brain_1_ChasingAbe::eAbeIsInTongueRange_10;
            }
        }
    }

    // Can we get to a hanging abe?
    if (IsActiveHero(pObj) && pObj->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 && mYPos > pObj->mYPos)
    {
        if (mYPos - pObj->mYPos <= (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(6)))
        {
            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), pObj))
            {
                if (pObj->GetSpriteScale() == GetSpriteScale()
                    && VIsFacingMe(pObj) && !WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pObj->mXPos - mXPos) && GotNoTarget() && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    SetTarget();
                    mNextMotion = eFleechMotions::Motion_14_ExtendTongueFromEnemy;
                    return Brain_1_ChasingAbe::eAbeIsInTongueRange_10;
                }
            }
        }
    }

    if (pObj->mYPos >= mYPos - (FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 18 : 9)) || pObj->GetSpriteScale() != GetSpriteScale())
    {
        return Brain_ChasingAbe_State1_Helper(pObj);
    }

    // Find hoist in front us?
    relive::Path_Hoist* pHoist = TryGetHoist(1, false);
    if (pHoist)
    {
        mNextMotion = eFleechMotions::Motion_3_Idle;
        field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
        field_162_hoistY = pHoist->mTopLeftY;
        return Brain_1_ChasingAbe::ePrepareToHoist_14;
    }

    // Find host in front or behind us?
    pHoist = TryGetHoist(0, true);
    if (pHoist)
    {
        if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
        {
            // TODO: Check left VS flip is correct
            if ((pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingLeft && GetAnimation().GetFlipX()) && pHoist->mGrabDirection != relive::Path_Hoist::GrabDirection::eFacingAnyDirection)
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
            }

            mNextMotion = eFleechMotions::Motion_3_Idle;
            field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->mTopLeftY;
            return Brain_1_ChasingAbe::ePrepareToHoist_14;
        }
        else
        {
            mNextMotion = eFleechMotions::Motion_3_Idle;
            return mBrainSubState;
        }
    }

    // Look 12 ahead fora hoist
    s32 k12BlocksCheck = 1;
    do
    {
        pHoist = TryGetHoist(k12BlocksCheck, true);
        if (pHoist)
        {
            mNextMotion = eFleechMotions::Motion_4_Crawl;
            field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->mTopLeftY;
            return mBrainSubState;
        }

        ++k12BlocksCheck;
    }
    while (k12BlocksCheck <= 12);

    // Look 8 behind for a hoist
    s32 k8BlocksCheck = 1;
    while (1)
    {
        pHoist = TryGetHoist(-k8BlocksCheck, 1);
        if (pHoist)
        {
            switch (mCurrentMotion)
            {
                case eFleechMotions::Motion_4_Crawl:
                    mNextMotion = eFleechMotions::Motion_7_StopCrawling;
                    break;

                case eFleechMotions::Motion_3_Idle:
                    mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                    break;

                case eFleechMotions::Motion_6_Knockback:
                case eFleechMotions::Motion_7_StopCrawling:
                    mNextMotion = eFleechMotions::Motion_4_Crawl;
                    break;
            }

            field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->mTopLeftY;
            return mBrainSubState;
        }

        if (++k8BlocksCheck > 8)
        {
            return Brain_ChasingAbe_State1_Helper(pObj);
        }
    }
}

s16 Fleech::Brain_ChasingAbe_State1_Helper(IBaseAliveGameObject* pObj)
{
    if (pObj->mYPos < mYPos - FP_FromInteger((GetSpriteScale() >= FP_FromInteger(1) ? 18 : 9)) && pObj->GetSpriteScale() == GetSpriteScale() && IsNear(pObj))
    {
        const FP v45 = mXPos - pObj->mXPos;
        if (FP_Abs(v45) < ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5))
        {
            return Brain_1_ChasingAbe::eBackToPatrol_13;
        }
    }

    if (!VIsFacingMe(pObj) && mCurrentMotion == eFleechMotions::Motion_4_Crawl)
    {
        mNextMotion = eFleechMotions::Motion_7_StopCrawling;
    }

    if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
    {
        if (VIsFacingMe(pObj))
        {
            const FP v48 = FP_FromInteger((GetAnimation().GetFlipX()) != 0 ? -1 : 1);
            if (WallHit(
                    FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
                    ScaleToGridSize(GetSpriteScale()) * v48))
            {
                if (GetAnimation().GetFlipX())
                {
                    mChasingOrScaredCrawlingLeft = true;
                }
                return Brain_1_ChasingAbe::eBlockedByWall_4;
            }
            mNextMotion = eFleechMotions::Motion_4_Crawl;
        }
        else
        {
            mCurrentMotion = eFleechMotions::Motion_6_Knockback;
        }
    }

    if (mCurrentMotion == eFleechMotions::Motion_7_StopCrawling)
    {
        mNextMotion = eFleechMotions::Motion_4_Crawl;
    }

    if (mCurrentMotion == eFleechMotions::Motion_9_Fall)
    {
        return Brain_1_ChasingAbe::eContinueChaseAfterFall_3;
    }

    if (IsNear(pObj))
    {
        return mBrainSubState;
    }

    if (!VIsFacingMe(pObj))
    {
        if (mCurrentMotion != eFleechMotions::Motion_7_StopCrawling && mCurrentMotion != eFleechMotions::Motion_6_Knockback)
        {
            if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
            {
                mNextMotion = eFleechMotions::Motion_7_StopCrawling;
                return Brain_1_ChasingAbe::eUnknown_2;
            }
            mNextMotion = eFleechMotions::Motion_6_Knockback;
        }
    }
    return Brain_1_ChasingAbe::eUnknown_2;
}

const s8 byte_5518B0[16] = {
    6,
    6,
    6,
    6,
    6,
    5,
    5,
    5,
    5,
    6,
    6,
    6,
    6,
    0,
    0,
    0};

enum Brain_2_Scared
{
    eScared_0 = 0,
    eReactToDanger_1 = 1,
    eCrawl_2 = 2,
    eLookForHoist_3 = 3,
    eCornered_4 = 4,
    eCorneredPrepareAttack_5 = 5,
    eCorneredAttack_6 = 6,
    eCheckIfEnemyDead_7 = 7,
    eEnemyStillAlive_8 = 8,
    ePatrolArea_9 = 9,
    ePrepareToHoist_10 = 10,
    eHoisting_11 = 11,
};

s16 Fleech::Brain_2_Scared()
{
    auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (pDangerObj && pDangerObj->mHealth > FP_FromInteger(0))
    {
        // Danger target is dead, check if there is another one who is still alive.
        IsScrabOrParamiteNear(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(8));
    }

    pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (pDangerObj)
    {
        if (pDangerObj->GetDead())
        {
            field_170_danger_obj = Guid{};
            pDangerObj = 0;
        }
    }

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        MusicController::static_PlayMusic(static_cast<MusicController::MusicTypes>(byte_5518B0[mBrainSubState]), this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_2_Scared::eScared_0:
            if (!pDangerObj || pDangerObj != sControlledCharacter)
            {
                return Brain_2_Scared::ePatrolArea_9;
            }

            if (!VIsFacingMe(pDangerObj))
            {
                mNextMotion = eFleechMotions::Motion_4_Crawl;
                Sound(FleechSound::Scared_7);
                return Brain_2_Scared::eReactToDanger_1;
            }

            if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                mNextMotion = eFleechMotions::Motion_4_Crawl;
                Sound(FleechSound::Scared_7);
                return Brain_2_Scared::eReactToDanger_1;
            }

            if (mCurrentMotion != eFleechMotions::Motion_4_Crawl)
            {
                Sound(FleechSound::Scared_7);
                return Brain_2_Scared::eReactToDanger_1;
            }

            mNextMotion = eFleechMotions::Motion_7_StopCrawling;
            Sound(FleechSound::Scared_7);
            return Brain_2_Scared::eReactToDanger_1;

        case Brain_2_Scared::eReactToDanger_1:
        {
            if (!pDangerObj || pDangerObj->mHealth <= FP_FromInteger(0) || pDangerObj != sControlledCharacter)
            {
                return Brain_2_Scared::ePatrolArea_9;
            }

            if (mVelX != FP_FromInteger(0))
            {
                const FP v9 = FP_FromInteger((GetAnimation().GetFlipX()) != 0 ? -1 : 1);
                if (WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(0) ? 10 : 5), ScaleToGridSize(GetSpriteScale()) * v9))
                {
                    mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
                    if (mVelX < FP_FromInteger(0))
                    {
                        mChasingOrScaredCrawlingLeft = true;
                    }
                    return Brain_2_Scared::eLookForHoist_3;
                }

                if (mVelX != FP_FromInteger(0) && (Collision(1) || HandleEnemyStopperOrSlamDoor(1)))
                {
                    mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
                    if (mVelX < FP_FromInteger(0))
                    {
                        mChasingOrScaredCrawlingLeft = true;
                    }
                    return Brain_2_Scared::eCornered_4;
                }
            }

            relive::Path_Hoist* pHoist = TryGetHoist(1, 0);
            if (pHoist)
            {
                mNextMotion = eFleechMotions::Motion_8_StopMidCrawlCycle;
                field_160_hoistX = pHoist->mTopLeftX + 12;
                field_162_hoistY = pHoist->mTopLeftY;
                return Brain_2_Scared::ePrepareToHoist_10;
            }

            if (VIsFacingMe(pDangerObj))
            {
                if (mCurrentMotion == eFleechMotions::Motion_4_Crawl)
                {
                    mNextMotion = eFleechMotions::Motion_7_StopCrawling;
                }
            }

            if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
            {
                if (VIsFacingMe(pDangerObj))
                {
                    mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                }
                else
                {
                    const FP k1Directed = FP_FromInteger(GetAnimation().GetFlipX() != 0 ? -1 : 1);
                    if (WallHit(
                            FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
                            ScaleToGridSize(GetSpriteScale()) * k1Directed))
                    {
                        if (GetAnimation().GetFlipX())
                        {
                            mChasingOrScaredCrawlingLeft = true;
                        }
                        return Brain_2_Scared::eCornered_4;
                    }
                    mNextMotion = eFleechMotions::Motion_4_Crawl;
                }
            }

            if (mCurrentMotion != eFleechMotions::Motion_9_Fall)
            {
                return mBrainSubState;
            }
            return Brain_2_Scared::eCrawl_2;
        }

        case Brain_2_Scared::eCrawl_2:
            if (mCurrentMotion == eFleechMotions::Motion_3_Idle)
            {
                mCurrentMotion = eFleechMotions::Motion_4_Crawl;
                return Brain_2_Scared::eReactToDanger_1;
            }
            return mBrainSubState;

        case Brain_2_Scared::eLookForHoist_3:
        {
            const FP v22 = FP_FromInteger(mChasingOrScaredCrawlingLeft != 0 ? -1 : 1);
            if (!WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(GetSpriteScale()) * v22))
            {
                return Brain_2_Scared::eReactToDanger_1;
            }

            relive::Path_Hoist* pHoist = TryGetHoist(0, 0);
            if (pHoist)
            {
                if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
                {
                    mNextMotion = eFleechMotions::Motion_3_Idle;
                }

                field_160_hoistX = pHoist->mTopLeftX + (GetSpriteScale() >= FP_FromInteger(1) ? 12 : 6);
                field_162_hoistY = pHoist->mTopLeftY;
                return Brain_2_Scared::ePrepareToHoist_10;
            }
            [[fallthrough]];
        }

        case Brain_2_Scared::eCornered_4:
        {
            if (!pDangerObj || pDangerObj != sControlledCharacter)
            {
                return Brain_2_Scared::ePatrolArea_9;
            }

            if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                return mBrainSubState;
            }

            if (mChasingOrScaredCrawlingLeft)
            {
                if (pDangerObj->mXPos < mXPos)
                {
                    return Brain_2_Scared::eReactToDanger_1;
                }
            }
            else if (pDangerObj->mXPos > mXPos)
            {
                return Brain_2_Scared::eReactToDanger_1;
            }

            if (VOnSameYLevel(pDangerObj) && 
                VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pDangerObj) &&
                pDangerObj->GetSpriteScale() == GetSpriteScale() &&
                VIsFacingMe(pDangerObj) &&
                !WallHit(FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5), pDangerObj->mXPos - mXPos) &&
                GotNoTarget())
            {
                field_11C_obj_id = field_170_danger_obj;
                return Brain_2_Scared::eCorneredPrepareAttack_5;
            }
            else
            {
                if ((Fleech_NextRandom() % 32) || mCurrentMotion != eFleechMotions::Motion_3_Idle)
                {
                    return mBrainSubState;
                }
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                return mBrainSubState;
            }
            break;
        }

        case Brain_2_Scared::eCorneredPrepareAttack_5:
            if (!pDangerObj || pDangerObj != sControlledCharacter)
            {
                return Brain_2_Scared::ePatrolArea_9;
            }

            if (VIsFacingMe(pDangerObj) || mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                if (!VOnSameYLevel(pDangerObj))
                {
                    return mBrainSubState;
                }

                if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pDangerObj))
                {
                    return mBrainSubState;
                }

                if (pDangerObj->GetSpriteScale() != GetSpriteScale())
                {
                    return mBrainSubState;
                }

                if (!VIsFacingMe(pDangerObj))
                {
                    return mBrainSubState;
                }

                if (WallHit(
                        FP_FromInteger(GetSpriteScale() >= FP_FromInteger(1) ? 10 : 5),
                        pDangerObj->mXPos - mXPos)
                    || !GotNoTarget())
                {
                    return mBrainSubState;
                }

                SetTarget();
                mNextMotion = eFleechMotions::Motion_14_ExtendTongueFromEnemy;
                return Brain_2_Scared::eCorneredAttack_6;
            }
            else
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                return mBrainSubState;
            }
            break;

        case Brain_2_Scared::eCorneredAttack_6:
            if (pDangerObj)
            {
                if (mCurrentMotion == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                {
                    return Brain_2_Scared::eCheckIfEnemyDead_7;
                }
                else
                {
                    if (mNextMotion == eFleechMotions::Motion_14_ExtendTongueFromEnemy)
                    {
                        return mBrainSubState;
                    }
                    ResetTarget();
                    return Brain_2_Scared::eReactToDanger_1;
                }
            }
            else
            {
                sub_42B8C0();
                return Brain_2_Scared::ePatrolArea_9;
            }
            break;

        case Brain_2_Scared::eCheckIfEnemyDead_7:
            if (pDangerObj)
            {
                if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
                {
                    return mBrainSubState;
                }

                ResetTarget();

                if (pDangerObj->mHealth <= FP_FromInteger(0))
                {
                    return Brain_2_Scared::ePatrolArea_9;
                }
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                return Brain_2_Scared::eEnemyStillAlive_8;
            }
            else
            {
                sub_42B8C0();
                return Brain_2_Scared::ePatrolArea_9;
            }
            break;

        case Brain_2_Scared::eEnemyStillAlive_8:
            if (pDangerObj)
            {
                if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
                {
                    return mBrainSubState;
                }
                return Brain_2_Scared::eCorneredPrepareAttack_5;
            }
            else
            {
                sub_42B8C0();
                return Brain_2_Scared::ePatrolArea_9;
            }
            break;

        case Brain_2_Scared::ePatrolArea_9:
            mNextMotion = eFleechMotions::Motion_3_Idle;
            field_170_danger_obj = Guid{};
            field_11C_obj_id = Guid{};
            mBrainState = eFleechBrains::eBrain_0_Patrol;
            return Brain_2_Scared::eScared_0;

        case Brain_2_Scared::ePrepareToHoist_10:
            if (mCurrentMotion != eFleechMotions::Motion_3_Idle)
            {
                return mBrainSubState;
            }
            if ((!GetAnimation().GetFlipX() || FP_FromInteger(field_160_hoistX) <= mXPos) &&
                (GetAnimation().GetFlipX() || FP_FromInteger(field_160_hoistX) >= mXPos))
            {
                mNextMotion = eFleechMotions::Motion_11_RaiseHead;
                return Brain_2_Scared::eHoisting_11;
            }
            else
            {
                mCurrentMotion = eFleechMotions::Motion_6_Knockback;
                return mBrainSubState;
            }
            break;

        case Brain_2_Scared::eHoisting_11:
            if (mCurrentMotion != eFleechMotions::Motion_13_SettleOnGround)
            {
                return mBrainSubState;
            }
            return Brain_2_Scared::ePatrolArea_9;

        default:
            return mBrainSubState;
    }
}

s16 Fleech::Brain_3_Death()
{
    field_11C_obj_id = Guid{};
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (field_12C_shrivel_timer < static_cast<s32>(sGnFrame + 80))
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.022));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_12C_shrivel_timer - 24)
    {
        DeathSmokeEffect(false);
    }

    if (GetSpriteScale() < FP_FromInteger(0))
    {
        SetDead(true);
    }

    return 100;
}

#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/FatalError.hpp"

namespace relive
{
    struct Path_Fleech;
    struct Path_Hoist;
}
class Fleech;

enum class eFleechMotions
{
    eNone_m1 = -1,
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

enum class FleechSound : u8
{
    PatrolCry_0 = 0,
    Burp_1 = 1,
    Digesting_2 = 2,
    SleepingInhale_3 = 3,
    SleepingExhale_4 = 4,
    Unknown_5 = 5, // unused? sounds like a whipping sound
    WakeUp_6 = 6,
    Scared_7 = 7,
    Devour_8 = 8,
    LandOnFloor_9 = 9,
    LickTarget_10 = 10,
    LedgeHoist_11 = 11,
    DeathByHeight_12 = 12,
    Dismember_13 = 13,
    CrawlRNG1_14 = 14,
    CrawlRNG2_15 = 15,
    CrawlRNG3_16 = 16,
};

class IFleechBrain
{
public:
    enum class EBrainTypes
    {
        Patrol = 0,
        ChasingAbe = 1,
        Scared = 2,
        Death = 3
    };
    virtual ~IFleechBrain() { }
    virtual void VUpdate() = 0;
    virtual EBrainTypes VGetBrain() = 0;
};

class PatrolBrain final : public IFleechBrain
{
public:
    enum EState
    {
        eInit = 0,
        eSleeping = 1,
        eWakingUp = 2,
        eGoingBackToSleep = 3,
        eAlerted = 4,
        eHearingScrabOrParamite = 5,
        eDangerNearby = 6,
        eBackToAlerted = 7,
        eAlertedByAbe = 8,
        eNearHoist = 9,
        eClimbHoist = 10,
    };

    explicit PatrolBrain(Fleech& fleech) : mFleech(fleech) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::Patrol; }

    void SetState(EState state) { mBrainState = state; }
    EState State() { return mBrainState; }

private:
    EState Brain_Patrol_State_0();
    EState Brain_Patrol_State_1();
    EState Brain_Patrol_State_2();
    EState Brain_Patrol_State_3();
    EState Brain_Patrol_State_4(BaseAliveGameObject* pTarget);
    EState Brain_Patrol_State_5();
    EState Brain_Patrol_State_6();
    EState Brain_Patrol_State_7();
    EState Brain_Patrol_State_8(BaseAliveGameObject* pTarget);
    EState Brain_Patrol_State_9();
    EState Brain_Patrol_State_10();

private:
    Fleech& mFleech;
    EState mBrainState = EState::eInit;
};

class ChasingAbeBrain final : public IFleechBrain
{
public:
    enum EState
    {
        eInit = 0,
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
        eGoBackToChasingAbe_16 = 16,
    };

    explicit ChasingAbeBrain(Fleech& fleech) : mFleech(fleech) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::ChasingAbe; }

    void SetState(EState state) { mBrainState = state; }
    EState State() { return mBrainState; }

private:
    EState Brain_ChasingAbe_State_0(BaseAliveGameObject* pObj);

    EState Brain_ChasingAbe_State_1(BaseAliveGameObject* pObj);
    EState Brain_ChasingAbe_State1_Helper(BaseAliveGameObject* pObj);

    EState Brain_ChasingAbe_State_2(BaseAliveGameObject* pObj);
    EState Brain_ChasingAbe_State_9(BaseAliveGameObject* pObj);

private:
    Fleech& mFleech;
    EState mBrainState = EState::eInit;
};

class ScaredBrain final : public IFleechBrain
{
public:
    enum EState
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

    explicit ScaredBrain(Fleech& fleech) : mFleech(fleech) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::Scared; }

    void SetState(EState state) { mBrainState = state; }
    EState State() { return mBrainState; }

private:
    Fleech& mFleech;
    EState mBrainState = EState::eScared_0;
};

class DeathBrain final : public IFleechBrain
{
public:

    explicit DeathBrain(Fleech& fleech) : mFleech(fleech) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::Death; }

private:
    Fleech& mFleech;
};

struct FleechSaveState final : public SaveStateBase
{
    FleechSaveState()
        : SaveStateBase(ReliveTypes::eFleech, sizeof(*this))
    { }
    Guid field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    FP mSpriteScale;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    bool mFlipX;
    eFleechMotions field_28_current_motion;
    s16 field_2A_anim_current_frame;
    s16 field_2C_frame_change_counter;
    bool mRender;
    bool mDrawable;
    FP mHealth;
    eFleechMotions mCurrentMotion;
    eFleechMotions mNextMotion;
    s16 mLastLineYPos;
    s16 mCollisionLineType;
    Guid mPlatformId;
    Guid mTlvInfo;
    Guid mFoodObjId;
    s16 mTongueState;
    s16 mTongueSubState;
    s16 mEnemyXPos;
    s16 mEnemyYPos;
    s16 mTongueOriginX;
    s16 mTongueOriginY;
    s16 mTongueDestinationX;
    s16 mTongueDestinationY;
    s16 field_5A;
    bool mTongueActive;
    bool mRenderTongue;
    IFleechBrain::EBrainTypes mBrainType;
    PatrolBrain::EState mPatrolBrainState;
    ChasingAbeBrain::EState mChasingAbeBrainState;
    ScaredBrain::EState mScaredBrainState;
    bool mReturnToPreviousMotion;
    s32 field_64_shrivel_timer;
    s8 field_68_fleech_random_idx;
    s8 field_69;
    s16 field_6A_bDidMapFollowMe;
    FP field_70_velx_factor;
    s16 field_76_current_anger;
    s16 mMaxAnger;
    s16 mAttackAngerIncreaser;
    s16 mWakeUpSwitchId;
    s16 mWakeUpSwitchAngerValue;
    s16 mWakeUpSwitchValue;
    s16 mCanWakeUpSwitchId;
    s16 field_84_EventXPos;
    s16 field_86_ScrabParamiteEventXPos;
    s16 mPatrolRange;
    s16 field_8A_old_xpos;
    s16 field_8C;
    s16 field_8E_rnd_crawl;
    s16 field_90_chase_delay;
    s16 field_92_chase_timer;
    s16 mLostTargetTimeout;
    s16 field_96_lost_target_timer;
    s16 mHoistX;
    s16 mHoistY;
    s8 field_9E_angle;
    s8 field_9F;
    FP mHoistYDistance;
    FP mHoistXDistance;
    Guid mScrabOrParamite;
    Guid field_AC_obj_id;
    bool mHoistDone;
    bool mChasingOrScaredCrawlingLeft;
    bool mShrivelDeath;
    bool mScaredSound;
    bool mAsleep;
    bool mGoesToSleep;
    bool mPersistant;
    s16 field_B2;
};

using TFleechBrainFn = s16 (Fleech::*)();
using TFleechMotionFn = void (Fleech::*)();

class Fleech final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sFleechAnimFromMotion[19] = {
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
        AnimId::Fleech_Consume};
    Fleech(relive::Path_Fleech* pTlv, const Guid& tlvId);
    ~Fleech();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    static void CreateFromSaveState(SerializedObjectData& pBuffer);
    virtual s16 VGetMotion(eMotionType motionType) override
    {
        switch (motionType)
        {
            case eMotionType::ePreviousMotion:
                return static_cast<s16>(mPreviousMotion);
            case eMotionType::eCurrentMotion:
                return static_cast<s16>(mCurrentMotion);
            case eMotionType::eNextMotion:
                return static_cast<s16>(mNextMotion);
            default:
                ALIVE_FATAL("Invalid motion type %d", static_cast<s32>(motionType));
        }
    }
    void Motion_0_Sleeping();
    void Motion_1_WakingUp();
    void Motion_2_Unknown();
    void Motion_3_Idle();
    void Motion_4_Crawl();
    void Motion_5_PatrolCry();
    void Motion_6_Knockback();
    void Motion_7_StopCrawling();
    void Motion_8_StopMidCrawlCycle();
    void Motion_9_Fall();
    void Motion_10_Land();
    void Motion_11_RaiseHead();
    void Motion_12_Climb();
    void Motion_13_SettleOnGround();
    void Motion_14_ExtendTongueFromEnemy();
    void Motion_15_RetractTongueFromEnemey();
    void Motion_16_DeathByFalling();
    void Motion_17_SleepingWithTongue();
    void Motion_18_Consume();


private:
    void SetBrain(IFleechBrain::EBrainTypes brain);
    bool BrainIs(IFleechBrain::EBrainTypes brain);

    eFleechMotions GetNextMotion() const
    {
        return static_cast<eFleechMotions>(mNextMotion);
    }
    eFleechMotions GetCurrentMotion() const
    {
        return static_cast<eFleechMotions>(mCurrentMotion);
    }

    void RenderEx(OrderingTable& ot);
    s16 IsScrabOrParamiteNear(FP radius);
    void Init();
    void InitTonguePolys();
    void SetAnim();
    void ResetTarget();
    s16 GotNoTarget();
    void SetTarget();
    void TongueHangingFromWall(s16 a2, s16 a3);
    void TongueUpdate();
    void ToIdle();
    s32 Sound(FleechSound soundId);
    u8** ResBlockForMotion(s32 motion);
    s16 CanMove();
    s16 HandleEnemyStopperOrSlamDoor(s32 velX);
    s32 UpdateWakeUpSwitchValue();
    void sub_42B8C0();
    void SetTongueState1();
    void IncreaseAnger();
    s16 AngerFleech(BaseAliveGameObject* pObj);
    s32 TongueActive();
    void PullTargetIn();
    void sub_42BA10();
    void SetTongueState5();
    BaseAliveGameObject* FindMudOrAbe();
    void MoveAlongFloor(); // MoveOnLine?
    s16 IsNear(BaseAliveGameObject* pObj);
    bool Collision(s16 alwaysOne);
    relive::Path_Hoist* TryGetHoist(s32 xDistance, s16 bIgnoreDirection);

public:
    void VOnFrame(const Point32& point);

private:
    Guid mTlvInfo;
    s16 mAttackAngerIncreaser = 0;
    s16 mWakeUpSwitchId = 0;
    s16 mWakeUpSwitchAngerValue = 0;
    s16 mWakeUpSwitchValue = 0;
    s16 mCanWakeUpSwitchId = 0;
    s16 mLostTargetTimeout = 0;
    s16 mPatrolRange = 0;
    Guid mFoodObjId = Guid{};
    bool mReturnToPreviousMotion = false;
    s32 field_12C_shrivel_timer = 0;
    s16 field_130_bDidMapFollowMe = 0;
    FP field_138_velx_factor = {};
    s16 mCurrentAnger = 0;
    u16 mMaxAnger = 2;
    s16 field_14C_EventXPos = 0;
    s16 field_14E_ScrabParamiteEventXPos = 0;
    s16 field_152_old_xpos = 0;
    s16 field_154 = 0;
    s16 field_156_rnd_crawl = 0;
    s16 field_158_chase_delay = 10;
    s16 field_15A_chase_timer = 0;
    s16 field_15E_lost_target_timer = 0;
    s16 mHoistX = 0;
    s16 mHoistY = 0;
    s8 mAngle = 0;
    FP mHoistYDistance = {};
    FP mHoistXDistance = {};
    Guid mScrabOrParamite = Guid{};
    bool mHoistDone = false;
    bool mChasingOrScaredCrawlingLeft = false;
    bool mShrivelDeath = false;
    bool mScaredSound = false;
    bool mAsleep = false;
    bool mGoesToSleep = false;
    bool mPersistant = false;
    s16 mTongueState = 0;
    s16 mTongueSubState = 0;
    s16 mEnemyXPos = 0;
    s16 mEnemyYPos = 0;
    s16 mTongueOriginX = 0;
    s16 mTongueOriginY = 0;
    s16 mTongueDestinationX = 0;
    s16 mTongueDestinationY = 0;
    s16 field_188 = 0;
    bool mTongueActive = false;
    bool mRenderTongue = false;
    Poly_G4 mTonguePolys1[4] = {};
    Poly_G4 mTonguePolys2[4] = {};
    friend class PatrolBrain;
    friend class ChasingAbeBrain;
    friend class ScaredBrain;
    friend class DeathBrain;
    PatrolBrain mPatrolBrain;
    ChasingAbeBrain mChasingAbeBrain;
    ScaredBrain mScaredBrain;
    DeathBrain mDeathBrain;

    IFleechBrain* mCurrentBrain = nullptr;
    eFleechMotions mPreviousMotion = eFleechMotions::Motion_0_Sleeping;
    eFleechMotions mCurrentMotion = eFleechMotions::Motion_0_Sleeping;
    eFleechMotions mNextMotion = eFleechMotions::Motion_0_Sleeping;
};

void Animation_OnFrame_Fleech(BaseGameObject* pObj, u32&, const IndexedPoint&);

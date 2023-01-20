#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_Fleech;
    struct Path_Hoist;
}

enum eFleechBrains
{
    eBrain_0_Patrol = 0,
    eBrain_1_ChasingAbe = 1,
    eBrain_2_Scared = 2,
    eBrain_3_Death = 3
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

struct FleechSaveState final
{
    ReliveTypes mType;
    s16 field_2;
    Guid field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mPathNumber;
    EReliveLevelIds mLvlNumber;
    FP mSpriteScale;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    bool mFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_current_frame;
    s16 field_2C_frame_change_counter;
    bool mRender;
    bool mDrawable;
    FP mHealth;
    s16 mCurrentMotion;
    s16 mNextMotion;
    s16 mLastLineYPos;
    s16 mCollisionLineType;
    Guid mPlatformId;
    Guid mTlvInfo;
    Guid field_44_obj_id;
    s16 field_48_unused;
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
    s16 field_5E_brain_state;
    s16 field_60_state;
    s16 field_62;
    s32 field_64_shrivel_timer;
    s8 field_68_fleech_random_idx;
    s8 field_69;
    s16 field_6A_bDidMapFollowMe;
    FP field_70_velx_factor;
    s16 field_76_current_anger;
    s16 field_78_max_anger;
    s16 field_7A_attack_anger;
    s16 field_7C_wakeup_id;
    s16 field_7E_wake_up_switch_anger_value;
    s16 field_80_wake_up_switch_value;
    s16 field_82_can_wake_up_id;
    s16 field_84_EventXPos;
    s16 field_86_ScrabParamiteEventXPos;
    s16 field_88_patrol_range;
    s16 field_8A_old_xpos;
    s16 field_8C;
    s16 field_8E_rnd_crawl;
    s16 field_90_chase_delay;
    s16 field_92_chase_timer;
    s16 field_94_lost_target_timeout;
    s16 field_96_lost_target_timer;
    s16 field_98_hoistX;
    s16 field_9A_hoistY;
    s8 field_9E_angle;
    s8 field_9F;
    FP field_A0_hoistY_distance;
    FP field_A4_hoistX_distance;
    Guid field_A8;
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

class Fleech;
using TFleechBrainFn = s16 (Fleech::*)();
using TFleechMotionFn = void (Fleech::*)();

class Fleech final : public BaseAliveGameObject
{
public:
    Fleech(relive::Path_Fleech* pTlv, const Guid& tlvId);
    ~Fleech();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    static s32 CreateFromSaveState(const u8* pBuffer);

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

public:
    s16 Brain_0_Patrol();
    s16 Brain_Patrol_State_0();
    s16 Brain_Patrol_State_1();
    s16 Brain_Patrol_State_2();
    s16 Brain_Patrol_State_3();
    s16 Brain_Patrol_State_4(IBaseAliveGameObject* pTarget);
    s16 Brain_Patrol_State_5();
    s16 Brain_Patrol_State_6();
    s16 Brain_Patrol_State_7();
    s16 Brain_Patrol_State_8(IBaseAliveGameObject* pTarget);
    s16 Brain_Patrol_State_9();
    s16 Brain_Patrol_State_10();

    s16 Brain_1_ChasingAbe();
    s16 Brain_ChasingAbe_State_0(IBaseAliveGameObject* pObj);

	s16 Brain_ChasingAbe_State_1(IBaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State1_Helper(IBaseAliveGameObject* pObj);

	s16 Brain_ChasingAbe_State_2(IBaseAliveGameObject* pObj);
    s16 Brain_ChasingAbe_State_9(IBaseAliveGameObject* pObj);

    s16 Brain_2_Scared();
    s16 Brain_3_Death();

private:
    void RenderEx(PrimHeader** ot);
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
    s16 AngerFleech(IBaseAliveGameObject* pObj);
    s32 TongueActive();
    void PullTargetIn();
    void sub_42BA10();
    void SetTongueState5();
    IBaseAliveGameObject* FindMudOrAbe();
    void MoveAlongFloor(); // MoveOnLine?
    s16 IsNear(IBaseAliveGameObject* pObj);
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
    Guid field_11C_obj_id = Guid{};
    s16 mBrainState = eFleechBrains::eBrain_0_Patrol;
    u16 mBrainSubState = 0;
    s16 field_128 = 0;
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
    Poly_G4 mTonguePolys1[4][2] = {};
    Poly_G4 mTonguePolys2[4][2] = {};
    Prim_SetTPage field_40C[2] = {};
};

void Animation_OnFrame_Fleech_449A60(BaseGameObject* pObj, u32&, const IndexedPoint&);

#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Primitives.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

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

struct Path_Fleech final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mFacing;
    Choice_short mAsleep;
    s16 padding1;
    s16 padding2;
    s16 mAttackAngerIncreaser;
    s16 padding3;
    s16 mWakeUpSwitchId;
    Choice_short mHanging;
    s16 mLostTargetTimeout;
    Choice_short mGoesToSleep;
    s16 mPatrolRangeInGrids;
    s16 mWakeUpSwitchAngerValue;
    s16 mCanWakeUpSwitchId;
    Choice_short mPersistant;
    s16 padding4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Fleech, 0x30);

struct Fleech_State final
{
    AETypes field_0_type;
    s16 field_2;
    s32 field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mPathNumber;
    LevelIds mLvlNumber;
    FP mSpriteScale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 field_26_bFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_current_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP mHealth;
    s16 mCurrentMotion;
    s16 mNextMotion;
    s16 mLastLineYPos;
    s16 mCollisionLineType;
    s32 mPlatformId;
    s32 mTlvInfo;
    s32 field_44_obj_id;
    s16 field_48_unused; //TODO: Saves and sets another unused field, field_120 -- Nemin (7/5/2020)
    s16 mTongueState;
    s16 mTongueSubState;
    s16 mEnemyXPos;
    s16 mEnemyYPos;
    s16 mTongueOriginX;
    s16 mTongueOriginY;
    s16 mTongueDestinationX;
    s16 mTongueDestinationY;
    s16 field_5A;
    s8 field_5C_tongue_active_flag;
    s8 field_5D_render_flag;
    s16 field_5E_brain_state;
    s16 field_60_state;
    s16 field_62;
    s32 field_64_shrivel_timer;
    s8 field_68_fleech_random_idx;
    s8 field_69;
    s16 field_6A_bDidMapFollowMe;
    s32 field_6C_unused;
    FP field_70_velx_factor;
    s16 field_74_unused;
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
    s16 field_9C_always_0;
    s8 field_9E_angle;
    s8 field_9F;
    FP field_A0_hoistY_distance;
    FP field_A4_hoistX_distance;
    s32 field_A8;
    s32 field_AC_obj_id;

    enum FleechStateFlags
    {
        eHoistDone = 0x1,
        eChasingOrScaredCrawlingLeft = 0x2,
        eShrivelDeath = 0x4,
        eScaredSound = 0x8,
        eAsleep = 0x10,
        eGoesToSleep = 0x20,
        ePersistant = 0x40,
    };

    BitField16<FleechStateFlags> mFleechStateFlags;
    s16 field_B2;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Fleech_State, 0xB4);

class Fleech;
using TFleechBrainFn = s16 (Fleech::*)();
using TFleechMotionFn = void (Fleech::*)();

class Fleech final : public BaseAliveGameObject
{
public:
    Fleech(Path_Fleech* pTlv, s32 tlvInfo);
    ~Fleech();
    

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnTlvCollision(Path_TLV* pTlv) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
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
    s16 Brain_Patrol_State_4(BaseAliveGameObject* pTarget);
    s16 Brain_Patrol_State_5();
    s16 Brain_Patrol_State_6();
    s16 Brain_Patrol_State_7();
    s16 Brain_Patrol_State_8(BaseAliveGameObject* pTarget);
    s16 Brain_Patrol_State_9();
    s16 Brain_Patrol_State_10();

    s16 Brain_1_ChasingAbe();
	s16 Brain_ChasingAbe_State_0(BaseAliveGameObject* pObj);

	s16 Brain_ChasingAbe_State_1(BaseAliveGameObject* pObj);
	s16 Brain_ChasingAbe_State1_Helper(BaseAliveGameObject* pObj);

	s16 Brain_ChasingAbe_State_2(BaseAliveGameObject* pObj);
    s16 Brain_ChasingAbe_State_9(BaseAliveGameObject* pObj);

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
    s16 AngerFleech(BaseAliveGameObject* pObj);
    s32 TongueActive();
    void PullTargetIn();
    void sub_42BA10();
    void SetTongueState5();
    BaseAliveGameObject* FindMudOrAbe();
    void MoveAlongFloor(); // MoveOnLine?
    s16 IsNear(BaseAliveGameObject* pObj);
    bool Collision(s16 alwaysOne);
    Path_Hoist* TryGetHoist(s32 xDistance, s16 bIgnoreDirection);

public:
    void VOnFrame(s16* pData);

private:
    s32 mTlvInfo = 0;
    s32 field_11C_obj_id = 0;
    s16 field_120_unused = 0;
    s16 field_122 = 0;
    s16 field_124_brain_state = 0;
    u16 field_126_brain_sub_state = 0;
    s16 field_128 = 0;
    s16 field_12A = 0;
    s32 field_12C_shrivel_timer = 0;
    s16 field_130_bDidMapFollowMe = 0;
    s16 field_132 = 0;
    s32 field_134_unused = 0;
    FP field_138_velx_factor = {};
    s16 field_13C_unused = 0;
    s16 field_13E_current_anger = 0;
    u16 field_140_max_anger = 0;
    s16 field_142_attack_anger_increaser = 0;
    s16 field_144_wake_up_switch_id = 0;
    s16 field_146_wake_up_switch_anger_value = 0;
    s16 field_148_wake_up_switch_value = 0;
    s16 field_14A_can_wake_up_switch_id = 0;
    s16 field_14C_EventXPos = 0;
    s16 field_14E_ScrabParamiteEventXPos = 0;
    s16 field_150_patrol_range = 0;
    s16 field_152_old_xpos = 0;
    s16 field_154 = 0;
    s16 field_156_rnd_crawl = 0;
    s16 field_158_chase_delay = 0;
    s16 field_15A_chase_timer = 0;
    s16 field_15C_lost_target_timeout = 0;
    s16 field_15E_lost_target_timer = 0;
    s16 field_160_hoistX = 0;
    s16 field_162_hoistY = 0;
    s16 field_164_always_0 = 0;
    s8 field_166_angle = 0;
    s8 field_167 = 0;
    FP field_168_hoistY_distance = {};
    FP field_16C_hoistX_distance = {};
    s32 field_170_danger_obj = 0;
    enum FleechFlags : s16
    {
        eHoistDone = 0x1,
        eChasingOrScaredCrawlingLeft = 0x2,
        eShrivelDeath = 0x4,
        eScaredSound = 0x8,
        eAsleep = 0x10,
        eGoesToSleep = 0x20,
        ePersistant = 0x40
    };
    BitField16<FleechFlags> mFleechFlags = {};
    s16 field_176 = 0;
    s16 mTongueState = 0;
    s16 mTongueSubState = 0;
    s16 mEnemyXPos = 0;
    s16 mEnemyYPos = 0;
    s16 mTongueOriginX = 0;
    s16 mTongueOriginY = 0;
    s16 mTongueDestinationX = 0;
    s16 mTongueDestinationY = 0;
    s16 field_188 = 0;

    enum TongueFlags
    {
        eTongueActive = 0x1,
        eRender = 0x2,
    };
    BitField16<TongueFlags> mTongueFlags = {};
    Poly_G4 mTonguePolys1[4][2] = {};
    Poly_G4 mTonguePolys2[4][2] = {};
    Prim_SetTPage field_40C[2] = {};
};
ALIVE_ASSERT_SIZEOF(Fleech, 0x42C);

s32 Animation_OnFrame_Fleech_449A60(BaseGameObject* pObj, s16* pData);

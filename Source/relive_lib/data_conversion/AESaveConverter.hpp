#pragma once

#include "guid.hpp"
#include "../ObjectTypes.hpp"
#include "../../AliveLibCommon/FixedPoint_common.hpp"
#include "../../AliveLibCommon/BitField.hpp"

#include "../../AliveLibAE/SlamDoor.hpp"

// Any enum/struct in the AEData namespace is related to OG data and can't ever be changed
// otherwise interpreting the OG data will break.
namespace AEData 
{ 
struct PSX_RECT final
{
    s16 x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

struct TlvOffsetLevelIdPathId final
{
    u16 tlvOffset;
    u8 levelId;
    u8 pathId;
};

struct TlvOffsetCombinedLevelIdPathId final
{
    u16 tlvOffset;
    u16 levelIdPathId;
};

union TlvItemInfoUnion
{
    u32 all;
    TlvOffsetCombinedLevelIdPathId combined;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

enum eMudMotions : u16
{
    Motion_0_Idle,
    Motion_1_WalkLoop,
    Motion_2_StandingTurn,

    // NOTE: 3-6 are all the same entry, added to avoid compile issue mentioned above.
    M_Speak_3_472FA0,
    M_Speak_4_472FA0,
    M_Speak_5_472FA0,
    M_Speak_6_472FA0,

    Motion_7_WalkBegin,
    Motion_8_WalkToIdle,
    Motion_9_MidWalkToIdle,
    Motion_10_LeverUse,
    Motion_11_Chisel,
    Motion_12_StartChisel,
    Motion_13_StopChisel,
    Motion_14_CrouchScrub,
    Motion_15_CrouchIdle,
    Motion_16_CrouchTurn,
    Motion_17_StandToCrouch,
    Motion_18_CrouchToStand,
    Motion_19_WalkToRun,
    Motion_20_MidWalkToRun,
    Motion_21_RunLoop,
    Motion_22_RunToWalk,
    Motion_23_MidRunToWalk,
    Motion_24_RunSlideStop,
    Motion_25_RunSlideTurn,
    Motion_26_RunTurnToRun,
    Motion_27_SneakLoop,
    Motion_28_MidWalkToSneak,
    Motion_29_SneakToWalk,
    Motion_30_WalkToSneak,
    Motion_31_MidSneakToWalk,
    Motion_32_SneakBegin,
    Motion_33_SneakToIdle,
    Motion_34_MidSneakToIdle,
    Motion_35_RunJumpBegin,
    Motion_36_RunJumpMid,
    Motion_37_StandToRun,
    Motion_38_Punch,
    Motion_39_HoistBegin,
    Motion_40_HoistLand,
    Motion_41_LandSoft1,
    Motion_42_LandSoft2,
    Motion_43_DunnoBegin,
    Motion_44_DunnoEnd,
    Motion_45_KnockForward,
    Motion_46_Knockback,
    Motion_47_KnockbackGetUp,
    Motion_48_WalkOffEdge,
    Motion_49_Fall,
    Motion_50_Chant,
    Motion_51_ChantEnd,
    Motion_52_ToDuck,
    Motion_53_Duck,
    Motion_54_DuckToCrouch,
    Motion_55_DuckKnockback,
    Motion_56_SlapOwnHead,
    Motion_57_TurnWheelBegin,
    Motion_58_TurnWheelLoop,
    Motion_59_TurnWheelEnd
};

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,
    eUnknown_0 = 0,
    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eFart_3 = 3,
    eUnknown_4 = 4,
    Slig_BS_5 = 5,
    Slig_LookOut_6 = 6,
    Slig_BS2_7 = 7,
    Slig_Laugh_8 = 8,
    eHello_9 = 9,
    eFollowMe_10 = 10,
    eAnger_11 = 11,
    eWait_12 = 12,
    eUnknown_13 = 13,
    eUnknown_14 = 14,
    eUnknown_15 = 15,
    eUnknown_16 = 16,
    eUnknown_17 = 17,
    eUnknown_18 = 18,
    eUnknown_19 = 19,
    eUnknown_20 = 20,
    eWork_21 = 21,
    eStopIt_22 = 22,
    eAllYa_23 = 23,
    eSorry_24 = 24,
    eUnknown_25 = 25,
    eUnknown_26 = 26,
    Slig_Hi_27 = 27,
    Slig_HereBoy_28 = 28,
    Slig_GetEm_29 = 29,
    eUnknown_30 = 30,
    Slig_Freeze_31 = 31,
    eDynamicCollision_32 = 32,
    eUnknown_33 = 33,
    eUnknown_34 = 34,
    eUnknown_35 = 35, // Glukkon_None_35?
    Glukkon_Hey_36 = 36,
    Glukkon_DoIt_37 = 37,
    Glukkon_StayHere_38 = 38,
    Glukkon_Commere_39 = 39,
    Glukkon_AllOYa_40 = 40,
    Glukkon_Heh_41 = 41,
    Glukkon_Help_42 = 42,
    Glukkon_Laugh_43 = 43,
    Glukkon_KillEm_44 = 44,
    Glukkon_Unknown_45 = 45,
    Glukkon_Unknown_46 = 46,
    Glukkon_What_47 = 47,

    Paramite_Howdy_48 = 48,
    Paramite_Stay_49 = 49,
    Paramite_CMon_or_Attack_50 = 50,
    Paramite_DoIt_51 = 51,
    Paramite_AllYa_52 = 52,

    Scrab_Howl_53 = 53,
    Scrab_Shriek_54 = 54,
};

enum class Mud_Emotion : s16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eAggressive_2 = 2,
    eSad_3 = 3,
    eSuicidal_4 = 4,
    eHappy_5 = 5, // TODO: Not used ??
    eWired_6 = 6,
    eSick_7 = 7
};

enum class Scale : s16
{
    Bg = 0,
    Fg = 1,
};

// Shared enum for sligs
enum class SligSpeak : s8
{
    eNone = -1,
    eHi_0 = 0,
    eHereBoy_1 = 1,
    eGetHim_2 = 2,
    eLaugh_3 = 3,
    eStay_4 = 4,
    eBullshit_5 = 5,
    eLookOut_6 = 6,
    eBullshit2_7 = 7,
    eFreeze_8 = 8,
    eWhat_9 = 9,
    eHelp_10 = 10,
    eBlurgh_11 = 11,
    eGotYa_12 = 12,
    eOuch1_13 = 13,
    eOuch2_14 = 14,
};

enum class MudSounds : s16
{
    eNone = -1,
    eEmpty_0 = 0,
    eEmpty_1 = 1,
    eEmpty_2 = 2,
    eHelloNeutral_3 = 3,
    eFollowMe_4 = 4,
    eAnger_5 = 5,
    eWait_6 = 6,
    eFart_7 = 7,
    eGiggle_8 = 8,
    eHurt2_9 = 9,
    eLaugh_10 = 10,
    eGoodbye_11 = 11,
    eOkay_12 = 12,
    eNuhUh_13 = 13,
    eOops_14 = 14,
    eDeathDropScream_15 = 15,
    eHurt1_16 = 16,
    eAllOYa_17 = 17,
    eHiAngry_18 = 18,
    eHiHappy_19 = 19,
    eHiSad_20 = 20,
    eNoAngry_21 = 21,
    eNoSad_22 = 22,
    eFartPuh_23 = 23,
    eSick_24 = 24,
    eWork_25 = 25,
    eStopIt_26 = 26,
    eSorry_27 = 27,
    eSadUgh_28 = 28,
};

enum class LevelIds : s16;

struct Slig_Spawner_State final
{
    AETypes mType;
    s16 padding1;
    s32 mTlvInfo;
    enum class SpawnerStates : s16
    {
        eInactive_0 = 0,
        eSligSpawned_1 = 1,
    };
    SpawnerStates mState;
    s16 padding2;
    s32 mSpawnedSligId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slig_Spawner_State, 0x10);

struct LiftMover_State final
{
    AETypes field_0_type_id; // never read?
    s16 field_2_padding;
    s32 field_4_tlvInfo;
    enum class LiftMoverStates : s16
    {
        eInactive_0 = 0,
        eStartMovingDown_1 = 1,
        eMovingDown_2 = 2,
        eStartMovingUp_3 = 3,
        eMovingUp_4 = 4,
        eMovingDone_5 = 5,
    };
    LiftMoverStates field_8_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftMover_State, 0xC);

struct Bone_SaveState final
{
    AETypes mAEType;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;

    enum BoneStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_bHitObject = 0x10,
    };

    BitField16<BoneStateFlags> field_20_flags;
    s16 field_22_padding;
    s32 field_24_base_id;
    s16 mCollisionLineType;
    s16 mBaseThrowableCount;
    enum class BoneStates : s16
    {
        eSpawned_0 = 0,
        eAirborne_1 = 1,
        eCollided_2 = 2,
        eOnGround_3 = 3,
        eEdible_4 = 4,
        eFalling_5 = 5
    };
    BoneStates mState;
    s16 mVolumeModifier;
    FP mInitialXPos;
    FP mInitialYPos;
    s32 mTimeToLiveTimer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Bone_SaveState, 0x3C);

struct MinesAlarm_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MinesAlarm_State, 0x8);

struct CrawlingSlig_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 field_26_bFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_cur_frame;
    s16 field_2C_anim_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_cur_motion;
    s16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s16 field_3A_line_type;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 field_40_bIsControlled;
    s8 field_41_padding;
    s16 field_42_padding;
    s32 field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 field_50_brain_sub_state;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_velx_scale_factor;
    s16 field_5C_padding;
    s16 field_5E_bChanting;
    LevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    s16 field_66_unused;
    s32 field_68_unused;
    s32 field_6C_slig_button_id;
    s32 field_70_obj_id;
    s32 field_74_obj_id;
    SligSpeak field_78_speak;
    s8 field_79_padding;
    s16 field_7A_unused_counter;
    s32 field_7C_say_help_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(CrawlingSlig_State, 0x80);

struct Drill_State final
{
    s16 field_0;
    s16 field_2_padding;
    s32 field_4;
    s32 field_8_tlvInfo;
    s32 field_C_off_timer;
    enum class DrillStates : s16
    {
        State_0_Restart_Cycle = 0,
        State_1_Going_Down = 1,
        State_2_GoingUp = 2,
    };
    DrillStates field_10_state;
    s16 field_12_xyoff;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Drill_State, 0x14);

struct EvilFart_State final
{
    AETypes field_0_type;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mAnimRender;
    s8 mDrawable;
    LevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;

    enum Flags_2C
    {
        eBit1_bControlled = 0x1,
        eBit2_FartExploded = 0x2,
    };
    BitField16<Flags_2C> field_2C;

    s16 mPossessedAliveTimer;
    enum class FartStates : s16
    {
        eIdle_0 = 0,
        eFlying_1 = 1,
        eDechanting_2 = 2
    };
    FartStates mState;
    s16 field_32_padding;
    s32 mUnpossessionTimer;
    s32 mBackToAbeTimer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(EvilFart_State, 60);

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

struct FlyingSlig_State final
{
    AETypes field_0_type;
    s16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_oldr;
    s16 field_1E_oldg;
    s16 field_20_oldb;
    s16 field_22_bAnimFlipX;
    s16 field_24_current_state;
    s16 field_26_current_frame;
    s16 field_28_frame_change_counter;
    u8 field_2A_bAnimRender;
    u8 field_2B_bDrawable;
    FP field_2C_current_health;
    s16 field_30_current_state;
    s16 field_32_delayed_state;
    s16 field_34_lastLineYPos;
    s16 field_36_line_idx;
    u8 field_38_launch_switch_id;
    u8 field_39_padding;
    enum Flags_3A
    {
        eBit1_bPossessed = 0x1,
        eBit2_Throw = 0x2,
        eBit3_bAlertedAndNotFacingAbe = 0x4,
        eBit4_DoAction = 0x8,
        eBit5_Chanting = 0x10,
        eBit6_Speaking_flag2 = 0x20,
        eBit7_Speaking_flag1 = 0x40,
        eBit8_bLastLine = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11_padding = 0x400,
    };
    BitField16<Flags_3A> field_3A;
    s32 field_3C_tlvInfo;
    s32 field_40_timer;
    s32 field_44_grenade_delay;
    s32 field_48_collision_reaction_timer;
    FP field_4C_xSpeed;
    FP field_50_ySpeed;
    SligSpeak field_54_next_speak;
    s8 field_55_padding;
    s16 field_56_voice_pitch_min;
    s32 field_58_obj_id;
    FP field_5C;
    FP field_60;
    FP field_64;
    FP field_68_line_length;
    FP field_6C;
    FP field_70_lever_pull_range_xpos;
    FP field_74_lever_pull_range_ypos;
    s32 field_78_unused;
    s32 field_7C_unused;
    s32 field_80_unused;
    s32 field_84_unused;
    FP field_88_nextXPos;
    FP field_8C_nextYPos;
    s32 field_90_fns1_idx;
    s16 field_94_padding;
    s16 field_96_padding;
    s16 field_98_unused;
    LevelIds field_9A_abe_level;
    s16 field_9C_abe_path;
    s16 field_9E_abe_camera;
    s16 field_A0_bobbing_values_table_index;
    s16 field_A2_padding;
    s32 field_A4_bobbing_values_index;
    FP field_A8_bobbing_value;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSlig_State, 0xAC);

struct FlyingSligSpawner_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_tlvInfo;
    s32 field_8_bSpawned;
    s32 field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawner_State, 0x10);

struct GameEnderController_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
    s32 field_8_timer;
    enum class GameEnderController_States : s16
    {
        eInit_0 = 0,
        eDetermineEnding_1 = 1,
        eFinish_2 = 2,
        eBadEnding_3 = 3,
        eGoodEnding_4 = 4,
        eAngelicEnding_5 = 5,
        eAngelicEndingCredits_6 = 6,
        ePadding_7 = 7,
        ePadding_8 = 8,
        ePadding_9 = 9,
    };
    GameEnderController_States field_C_state;
    s16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GameEnderController_State, 0x10);

struct SlapLockWhirlWind_State final
{
    AETypes mType;
    s16 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLockWhirlWind_State, 0x4);

struct SlapLock_State final
{
    AETypes mType;
    s16 mAnimRender;
    s32 mTlvInfo;
    s8 mTlvState;
    s8 padding;
    enum class SlapLockStates : s16
    {
        eShaking_0 = 0,
        eIdle_1 = 1,
        eSlapped_2 = 2,
        eBroken_3 = 3,
        eEmitInvisibilityPowerupRing_4 = 4,
        eFlickerHero_5 = 5,
        eGiveInvisibilityFromFlicker_6 = 6,
        eGiveInvisibility_7 = 7,
    };
    SlapLockStates mState;
    s32 mTimer1;
    s32 mAbilityRingId;
    s32 mShinyParticleTimer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLock_State, 0x18);

struct Greeter_State final
{
    AETypes field_0_type;
    s16 field_2_r;
    s16 field_4_g;
    s16 field_6_b;
    s16 field_8_path_number;
    LevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    s16 field_20_current_frame;
    s16 field_22_frame_change_counter;
    s8 field_24_bAnimRender;
    s8 field_25_bDrawable;
    s8 field_26_padding;
    s8 field_27_padding;
    s32 field_28_tlvInfo;
    s32 field_2C_unused;
    s32 field_30_last_turn_time;
    s32 field_34_timer;
    s16 field_38_timesShot;
    s16 field_3A_bDontSetDestroyed;
    s16 field_3C_bChasing;
    s16 field_3E_padding;
    FP field_40_speed;
    enum class GreeterBrainStates : s16
    {
        eBrain_0_Patrol,
        eBrain_1_PatrolTurn,
        eBrain_2_Speak,
        eBrain_3_ChaseSpeak,
        eBrain_4_Chase,
        eBrain_5_Knockback,
        eBrain_6_ToChase,
        eBrain_7_Fall
    };
    GreeterBrainStates field_44_brain_state;
    s16 field_46_targetOnLeft;
    s16 field_48_targetOnRight;
    s16 field_4A_padding;
    FP field_4C_motion_laser_xpos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Greeter_State, 0x50);

struct Grenade_SaveState final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    LevelIds field_1E_lvl_number;

    enum Flags_20
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_unused = 0x10,
        eBit6_bExplodeNow = 0x20,
        eBit7_bBlowUpOnCollision = 0x40,
    };
    BitField16<Flags_20> field_20_flags;
    s16 field_22_padding;
    s32 field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_savedcount;
    enum class GrenadeStates : s16
    {
        eFallingToBeCollected_0 = 0,
        eWaitToBeCollected_1 = 1,
        eDoesNothing_2 = 2,
        eCountingDown_3 = 3,
        eFalling_4 = 4,
        eHitGround_5 = 5,
        eWaitForExplodeEnd_6 = 6,
        eExploded_7 = 7,
        eDoesNothing_8 = 8,
        eFallingBlowUpOnGround_9 = 9,
    };
    GrenadeStates field_2C_state;
    s16 field_2E;
    s16 field_30_explode_timer;
    s16 field_32_padding;
    FP field_34_xpos;
    FP field_38_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Grenade_SaveState, 0x3C);

struct Glukkon_SaveState final
{
    AETypes field_0_id;
    s16 field_2_padding;
    s32 field_4_object_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_xvel;
    FP field_14_yvel;
    s16 field_18_path;
    LevelIds field_1A_level;
    FP field_1C_sprite_scale;
    u16 mRingRed;
    u16 mRingGreen;
    u16 mRingBlue;
    u16 field_26_flipX;
    u16 field_28_current_motion;
    u16 field_2A_current_frame;
    u16 field_2C_frame_change_counter;
    u8 field_2E_render;
    u8 field_2F_drawable;
    FP field_30_health;
    u16 field_34_current_motion;
    u16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s8 field_3A_line_type;
    u8 field_3B_padding;
    s32 field_3C_padding;
    u16 field_40_bIsActiveChar;
    s16 field_42_padding;
    s32 field_44_tlvInfo;
    s32 field_48_brain_state_idx;
    s32 field_4C_padding;
    s16 field_50_brain_sub_state;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_falling_velx_scale_factor;
    s16 field_5C_padding;
    s16 field_5E_prevent_depossession;
    LevelIds field_60_level;
    s16 field_62_path;
    s16 field_64_camera;
    enum class GlukkonSpeak : s8
    {
        None = -1,
        Hey_0 = 0,
        DoIt_1 = 1,
        StayHere_2 = 2,
        Commere_3 = 3,
        AllOYa_4 = 4,
        Heh_5 = 5,
        Help_6 = 6,
        Laugh_7 = 7,
        KillEm_8 = 8,
        Unused_9 = 9,
        Unused_10 = 10,
        What_11 = 11,
        Unused_12 = 12,
        Unused_13 = 13,
        Unused_14 = 14
    };
    GlukkonSpeak field_66_speak;
    s16 field_68_gamespeak_pitch;
    s16 field_6A_padding;
    FP field_6C_previous_ypos;
    s32 field_70_randomish_speak_timer;
    s32 field_74_turn_or_help_timer;
    s32 field_78_panic_timer;
    s16 field_7C;
    s16 field_7E_padding;
    s32 field_80_knockback_delay_after_getting_shot_timer;
    s32 field_84_getting_shot_timer;
    s32 field_88_obj_id;
    s16 field_8C_can_be_possessed;
    AETypes field_8E_type_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Glukkon_SaveState, 144);

struct Abe_SaveState final
{
    AETypes mAEType;
    s16 field_2_padding;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    u16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mSpriteScale;
    Scale mScale;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    u16 bAnimFlipX;
    u16 mCurrentMotion;
    u16 mCurrentFrame;
    u16 mFrameChangeCounter;
    s8 mRenderLayer;
    s8 mAnimRender;
    s8 mIsDrawable;
    s8 field_2F_padding;
    FP mHealth;
    u16 mCurrentMotion2; // the same as mCurrentMotion
    u16 mNextMotion;
    u16 mLastLineYPos;
    s16 mCollisionLineType;
    u32 mPlatformId;
    u16 mIsElectrocuted;
    u16 mIsInvisible;
    s8 mIsAbeControlled;
    s8 field_45_padding;
    s16 field_46_padding;
    FP field_48_x_vel_slow_by;
    u32 field_4C_unused;
    u32 field_50_state;
    u32 field_54_timer;
    u32 field_58_abe_timer;
    u32 mRegenHealthTimer;
    Mud_Emotion mMood;
    MudSounds mSay;
    u32 mAutoSayTimer;
    u32 mRingPulseTimer;
    s8 mBaseThrowableCount;
    s8 bShrivel;
    s8 mHaveShrykull;
    s8 bHaveInvisiblity;
    u16 mPrevHeld;
    u16 mReleasedButtons;
    u16 mKnockdownMotion;
    s16 field_76_padding;
    u32 mRollingMotionTimer;
    u32 mDeathFadeOutId;
    u32 mCircularFadeId;
    u32 mOrbWhirlWindId;
    u32 mPossessedObjectId;
    u32 mThrowableId;
    u32 mPullRingRopeId;
    u32 mSlappableOrPickupId;
    u32 mWorkWheelId;
    u32 mInvisibilityTimer;
    u16 field_A0_unused;
    u16 mInvisibilityDuration;
    s8 mHandStoneCamIdx;
    s8 field_A5_padding;
    s16 field_A6_padding;
    ReliveTypes mHandStoneType;
    u16 mFmvId;
    u16 mHandStoneCam1;
    u16 mHandStoneCam2;
    u16 mHandStoneCam3;
    u16 field_B4_unused;
    u16 field_B6_unused;
    u16 field_B8_unused;
    u16 field_BA_unused;
    u16 field_BC_unused;
    u16 field_BE_unused;
    u16 mHasEvilFart;
    LevelIds mDstWellLevel;
    u16 mDstWellPath;
    u16 mDstWellCamera;
    u16 door_id;
    s8 mThrowDirection;
    s8 field_CB_padding;
    u16 mBirdPortalSubState;
    s16 field_CE_padding;
    u32 mBirdPortalId;
    enum Flags_D4
    {
        eD4_Bit1_lift_point_dead_while_using_lift = 0x1,
        eD4_Bit2_return_to_previous_motion = 0x2,
        eD4_Bit3_WalkToRun = 0x4,
        eD4_Bit4_unused = 0x8,
        eD4_Bit5_prevent_chanting = 0x10,
        eD4_Bit6_land_softly = 0x20,
        eD4_Bit7_unused = 0x40,
        eD4_Bit8_laugh_at_chant_end = 0x80,
        eD4_Bit9_unused = 0x100,
        eD4_Bit10_play_ledge_grab_sounds = 0x200,
        eD4_Bit11_unused = 0x400,
        eD4_Bit12_have_healing = 0x800,
        eD4_eBit13_teleporting = 0x1000,
        eD4_eBit14_is_mudanchee_vault_ender = 0x2000,
        eD4_eBit15_is_mudomo_vault_ender = 0x4000,
        eD4_eBit16_shadow_enabled = 0x8000,
    };
    BitField16<Flags_D4> field_D4_flags;

    enum Flags_D6
    {
        eD6_Bit1_shadow_at_bottom = 0x1
    };
    BitField16<Flags_D6> field_D6_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Abe_SaveState, 216);

struct LiftPoint_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    s32 field_C_tlvInfo;
    s32 field_10_pTlv;
    FP field_14_floorYLevel;
    enum class LiftPointStopType : s16
    {
        eTopFloor_0 = 0,
        eBottomFloor_1 = 1,
        eMiddleFloor_2 = 2,
        eMiddleLockFloor_3 = 3,
        eStartPointOnly_4 = 4,
    };
    LiftPointStopType field_18_lift_point_stop_type;
    enum Flags
    {
        eBit1_bMoving = 0x1,
        eBit2_bTopFloor = 0x2,
        eBit3_bMiddleFloor = 0x4,
        eBit4_bBottomFloor = 0x8,
        eBit5_bMoveToFloorLevel = 0x10,
        eBit6 = 0x20,
        eBit7_KeepOnMiddleFloor = 0x40,
    };
    BitField16<Flags> field_1A;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftPoint_State, 0x1C);

struct Mudokon_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_bFlipX;
    s16 field_24_current_motion;
    s16 field_26_anim_current_frame;
    s16 field_28_anim_frame_change_counter;
    s8 field_2A_bAnimRender;
    s8 field_2B_bDrawable;
    FP field_2C_health;
    s16 field_30_current_motion;
    s16 field_32_next_motion;
    s16 field_34_lastLineYPos;
    s16 field_36_line_type;
    s16 field_38_padding;
    s16 field_3A_padding;
    s8 field_3C_can_be_possessed;
    s8 field_3D_bIsPlayer;
    s16 field_3E_padding;
    s32 field_40_tlvInfo;
    FP field_44_velx_slow_by;
    s32 field_48_unused;
    s32 field_4C_portal_id;
    s16 field_50_angry_trigger;
    s16 field_52_padding;
    s32 field_54_savedfield124; //TODO: Find out what field_124 is. It appears to be some sort of timer. -- Nemin (5/7/2020)
    s32 field_58_angry_timer;
    s16 field_5C_unused;
    s16 field_5E_voice_pitch;
    s32 field_60_wheel_id;
    s32 field_64_unused;
    MudSounds field_68;
    s16 field_6A_maxXOffset;


    enum Flags_6A
    {
        eBit1_padding = 0x1,
        eBit2_unused = 0x2,
        eBit3_padding = 0x4,
        eBit4_not_rescued = 0x8,
        eBit5_save_state = 0x10,
        eBit6_alerted = 0x20,
        eBit7_blind = 0x40,
        eBit8_following = 0x80,
        eBit9_standing_for_sad_or_angry = 0x100,
        eBit10_stopped_at_wheel = 0x200,
        eBit11_do_angry = 0x400,
        eBit12_seen_while_sick = 0x800,
        eBit13_stop_trigger = 0x1000,
        eBit14_unused = 0x2000,
        eBit15_return_to_previous_motion = 0x4000,
        eBit16_get_depressed = 0x8000
    };
    BitField16<Flags_6A> field_6C;

    enum Flags_6E
    {
        e6E_Bit1_alert_enemies = 0x1,
        e6E_Bit2 = 0x2,
        e6E_Bit3_make_sad_noise = 0x4,
        e6E_Bit4_ring_timeout = 0x8,
        e6E_Bit5 = 0x10,
        e6E_Bit6 = 0x20
    };
    BitField16<Flags_6E> field_6E;

    s16 field_70_brain_sub_state2;
    s16 field_72_stand_idle_timer;
    enum class MudAction : s16
    {
        eHelloOrAllYa_0 = 0,
        eFollowMe_1 = 1,
        eWait_2 = 2,
        eUnused_3 = 3,
        eStopIt_4 = 4,
        eUnused_5 = 5,
        eFart_6 = 6,
        eSlapOrWater_7 = 7,
        eSorry_8 = 8,
        eMudAbuse_9 = 9,
        eComfort_10 = 10,
        eSmackIntoWall_11 = 11,
        eLaugh_12 = 12,
        eDuck_13 = 13,
        eMudDied_14 = 14,
        eUnknown_15 = 15,
        eUnknown_16 = 16,
        eNone_17 = 17,
    };
    MudAction field_74_delayed_speak;
    Mud_Emotion field_76_emo_tlb;
    GameSpeakEvents field_78;
    eMudMotions field_7A_motion;
    enum Mud_Brain_State : s16
    {
        Brain_0_GiveRings,
        Brain_1_Chisel,
        Brain_2_CrouchScrub,
        Brain_3_TurnWheel,
        Brain_4_ListeningToAbe,
        Brain_5_ShrivelDeath,
        Brain_6_Escape,
        Brain_7_FallAndSmackDeath,
        Brain_8_AngryWorker,
        Brain_9_Sick
    };
    Mud_Brain_State field_7C_brain_state;
    s16 field_7E_brain_sub_state;
    s32 field_80_timer;
    s32 field_84_response_entry_idx;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Mudokon_State, 0x88);

struct Meat_SaveState final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    LevelIds field_1E_lvl_number;
    enum MeatStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<MeatStateFlags> field_20_flags;
    s16 field_22_pad;
    s32 field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    enum class MeatStates : s16
    {
        eCreated_0 = 0,
        eIdle_1 = 1,
        eBeingThrown_2 = 2,
        eBecomeAPickUp_3 = 3,
        eWaitForPickUp_4 = 4,
        eFall_5 = 5,
    };
    MeatStates field_2C_state;
    s16 field_2E_pad;
    FP field_30_xpos;
    FP field_34_ypos;
    s32 field_38_savedfield12C; // TODO: Figure out what field_12C is. -- Nemin (5/7/2020)
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Meat_SaveState, 60);

struct MineCar_SaveState final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_sprite_scale;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_xFlip;
    s32 field_24_frame_table;
    s16 field_28_current_motion;
    s16 field_2A_current_anim_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_render;
    s8 field_2F_drawable;
    s8 field_30_unused;
    s8 field_31_padding;
    s16 field_32_unused;
    s16 field_34_unused;
    s16 field_36_unused;
    s32 field_38_frame_table_offset2;
    FP field_3C_health;
    s16 field_40_current_motion;
    s16 field_42_next_motion;
    s16 field_44_last_line_ypos;
    s16 field_46_collision_line_type;
    s16 field_48_padding;
    s16 field_4A_padding;
    s32 field_4C_tlvInfo;
    enum class MineCarStates : s16
    {
        eParkedWithoutAbe_0 = 0,
        eParkedWithAbe_1 = 1,
        eMoving_2 = 2,
        eFalling_3 = 3,
    };
    MineCarStates field_50_state;
    enum class MineCarDirs : s16
    {
        eDown_0 = 0,
        eRight_1 = 1,
        eLeft_2 = 2,
        eUp_3 = 3,
    };
    MineCarDirs field_52_turn_direction;
    s16 field_54_unused;
    s16 field_56_unused;
    s16 field_58_falling_counter;
    s16 field_5A_bAbeInCar;
    s32 field_5C_frame_mod_16;
    s16 field_60_spawned_path;
    s16 field_62_spanwed_camera;
    s16 field_64_throw_item_key1;
    s16 field_66_continue_move_input;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MineCar_SaveState, 0x68);

struct Paramite_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_flip_x;
    s16 field_24_current_motion;
    s16 field_26_anim_current_frame;
    s16 field_28_frame_change_counter;
    s8 field_2A_render;
    s8 field_2B_drawable;
    FP field_2C_health;
    s16 field_30_current_motion;
    s16 field_32_next_motion;
    s16 field_34_last_line_ypos;
    s16 field_36_line_type;
    s16 field_38_padding;
    s16 field_3A_padding;
    s32 field_3C_tlvInfo;
    s32 field_40_meat_id;
    s32 field_44_web_id;
    s32 field_48_obj_id;
    s32 field_4C_pull_ring_rope_id;
    s32 field_50_brain_idx;
    s16 field_54_padding;
    s16 field_56_padding;
    s16 field_58_brain_ret;
    s16 field_5A_padding;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_velx_offset;
    s32 field_68_timer;
    LevelIds field_6C_return_level;
    s16 field_6E_return_path;
    s16 field_70_return_camera;
    s16 field_72_input;
    s16 field_74_next_brain_ret;

    enum Flags_76 : s16
    {
        eBit1_unused = 0x1,
        eBit2_running = 0x2,
        eBit3_hissed_or_left_screen = 0x4,
        eBit4_prevent_depossession = 0x8,
        eBit5_spawned = 0x10,
        eBit6_alerted = 0x20,
        eBit7_can_be_possessed = 0x40,
    };

    BitField16<Flags_76> field_76_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Paramite_State, 0x78);

struct BirdPortal_State final
{
    AETypes mAEType;
    u8 mState;
    u8 mMudCountForShrykull;
    s32 mTlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(BirdPortal_State, 8);

struct ThrowableArray_SaveState final
{
    s16 field_0_unused;
    s16 field_2_item_count;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ThrowableArray_SaveState, 0x4);


enum class RingTypes : s16
{
    // Red rings
    eExplosive_Pulse_0 = 0,
    eExplosive_Emit_1 = 1,        // Actually blows stuff up
    eExplosive_Emit_Effect_2 = 2, // Graphical effect only
    eExplosive_Give_3 = 3,

    // Blue rings
    eShrykull_Pulse_Small_4 = 4,
    eShrykull_Pulse_Large_5 = 5,
    eShrykull_Pulse_Orange_6 = 6, // Never used?

    // Green rings
    eInvisible_Pulse_Small_7 = 7,
    eInvisible_Pulse_Large_8 = 8,
    eInvisible_Pulse_Emit_9 = 9,
    eInvisible_Pulse_Give_10 = 10,

    // Yellow rings
    eHealing_Emit_Effect_11 = 11, // Graphical effect only
    eHealing_Emit_12 = 12,        // Actually heal muds
    eHealing_Give_13 = 13,
    eHealing_Pulse_14 = 14,
};

struct AbilityRing_State final
{
    AETypes mRingObjectType;
    FP mRingXPos;
    FP mRingYPos;
    RingTypes mRingType;
    FP mRingScale;
    s32 mRingTlvInfo;
    FP mRingRight;
    s32 mRingCount;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(AbilityRing_State, 0x28);

struct RockSaveState final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    LevelIds field_1E_lvl_number;
    enum RockStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<RockStateFlags> field_20_flags;
    s16 field_22_padding;
    s32 field_24_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    enum class RockStates : s16
    {
        eNone_0 = 0,
        eFallingOutOfRockSack_1 = 1,
        eRolling_2 = 2,
        eOnGround_3 = 3,
        eBouncing_4 = 4,
        eFallingOutOfWorld_5 = 5,
    };
    RockStates field_2C_state;
    s16 field_2E_volume;
    FP field_30_xpos;
    FP field_34_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(RockSaveState, 0x38);

struct Scrab_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 field_26_bAnimFlipX;
    s16 field_28_current_motion;
    s16 field_2A_current_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bAnimRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_current_motion;
    s16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s16 field_3A_line_type;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 field_40_bIsControlled;
    s8 field_41_padding;
    s16 field_42_padding;
    s32 field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 field_50_sub_state;
    s16 field_52_padding;
    s32 field_54_obj_id;
    s32 field_58_target_obj_id;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_falling_velx_scale_factor;
    s16 field_68_motion_resource_block_index;
    s16 field_6A_padding;
    s32 field_6C_spotting_abe_timer;
    s32 field_70_attack_delay_timer;
    s32 field_74_movement_timer;
    s32 field_78_sfx_bitmask;
    s16 field_7C_prevent_depossession;
    LevelIds field_7E_return_level;
    s16 field_80_return_path;
    s16 field_82_return_camera;
    s16 field_84_input;
    s16 field_86_padding;
    s32 field_88_unused;
    s16 field_8C_shred_power_active;
    GameSpeakEvents field_8E_speak;
    FP field_90_max_xpos;
    FP field_94_max_ypos;
    s16 field_98_speak_counter;
    s16 field_9A_unused;
    s16 field_9C_unused;

    enum Flags_9E
    {
        eBit1_attacking = 0x1,
        eBit2_unused = 0x2,
        eBit3_unused = 0x4,
        eBit4_force_update_animation = 0x8,
        eBit5_roar_randomly = 0x10,
        eBit6_persistant = 0x20,
    };
    BitField16<Flags_9E> field_9E_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Scrab_State, 0xA0);

struct ScrabSpawner_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_tlvInfo;
    enum class ScrabSpawnerStates : s32
    {
        eInactive_0 = 0,
        eScrabSpawned_1 = 1
    };
    ScrabSpawnerStates field_8_state;
    s32 field_C_spawned_scrab_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSpawner_State, 0x10);

struct Quicksave_Obj_SlamDoor final
{
    AETypes mType;
    TlvItemInfoUnion mTlvInfo;

    static ::Quicksave_Obj_SlamDoor From(const Quicksave_Obj_SlamDoor& data)
    {
        ::Quicksave_Obj_SlamDoor d;
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo.all);
        return d;
    }

};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_Obj_SlamDoor, 8);

struct Slig_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    Scale field_1C_scale;
    s16 field_1E_r;
    s16 field_20_g;
    s16 field_22_b;
    s16 field_24_bFlipX;
    s16 field_26_current_motion;
    s16 field_28_current_frame;
    s16 field_2A_frame_change_counter;
    s8 field_2C_bRender;
    s8 field_2D_bDrawable;
    s16 field_2E_padding;
    FP field_30_health;
    s16 field_34_current_motion;
    s16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s16 field_3A_collision_line_type;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 field_40_bActiveChar;
    s8 field_41_padding;
    s16 field_42_brain_sub_state;
    s16 field_44_pitch_min;
    s16 field_46_padding;
    s32 field_48_timer;
    s16 field_4C_return_to_previous_motion;
    s16 field_4E_checked_if_off_screen;
    s16 field_50_input;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_falling_velx_scale_factor;
    s32 field_5C_tlvInfo;
    s16 field_60_res_idx;
    s16 field_62_shot_motion;
    PSX_RECT field_64_zone_rect;
    s16 field_6C_unused;
    s16 field_6E_unused;
    s16 field_70_unused;
    LevelIds field_72_return_level;
    s16 field_74_return_path;
    s16 field_76_return_camera;
    s32 field_78_death_by_being_shot_timer;
    s32 field_7C_explode_timer;
    s32 field_80_brain_state_idx;
    s16 field_84_padding;
    s16 field_86_padding;
    s32 field_88_unused;
    s32 field_8C_num_times_to_shoot;
    s16 field_90_force_alive_state;
    s16 field_92_spotted_possessed_slig;
    s32 field_94_glukkon_id;
    s16 field_98_state_after_speak;
    s16 field_9A_attention_timeout;
    s16 field_9C_unused;
    s16 field_9E_next_command_arg1;
    s16 field_A0_cmd_idx;
    enum Flags_A2
    {
        eBit1_Padding = 0x1,
        eBit2_FollowGlukkon = 0x2,
        eBit3_StoppedForLeverOrLift = 0x4,
        eBit4_GlukkonCalledAllOYa = 0x8,
        eBit5_HeardGlukkon = 0x10,
    };
    BitField16<Flags_A2> field_A2_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slig_State, 0xA4);

struct Slog_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_objectId;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 field_26_bAnimFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_cur_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_current_motion;
    s16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s16 field_3A_line_type;
    s32 field_3C_id;
    s32 field_40_tlvInfo;
    s32 field_44_obj_id;
    s16 field_48_state_idx;
    s16 field_4A_brain_state_result;
    s32 field_4C_timer;
    FP field_50_falling_velx_scale_factor;
    s32 field_54_obj_id;
    s16 field_58_has_woofed;
    s16 field_5A_waiting_counter;
    s16 field_5C_response_index;
    s16 field_5E_response_part;
    s16 field_60_anger_level;
    s16 field_62_jump_counter;
    s32 field_64_scratch_timer;
    s32 field_68_growl_timer;
    s32 field_6C_bone_id;
    s16 field_70_jump_delay;
    u8 field_72_slog_random_index;
    u8 field_73_padding;

    enum Flags_74
    {
        eBit1_BitingTarget = 0x1,
        eBit2_Possessed = 0x2,
        eBit3_Asleep = 0x4,
        eBit4_MovedOffScreen = 0x8,
        eBit5_StopRunning = 0x10,
        eBit6_Shot = 0x20,
        eBit7_Hungry = 0x40,
        eBit8_CommandedToAttack = 0x80,
        eBit9_HitByAbilityRing = 0x100,
        eBit10_ListenToSligs = 0x200,
    };
    BitField16<Flags_74> field_74_flags;
    s16 field_76_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slog_State, 0x78);

struct Slurg_State final
{
    AETypes mType;
    s16 padding1;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mSlurgSpriteScale;
    s16 mFlipX;
    s16 mCurrentMotion;
    s16 mAnimCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mDrawable;
    s8 mRender;
    s16 padding2;
    s32 mFrameTableOffset;
    s32 mTlvInfo;
    enum class SlurgStates : s16
    {
        eMoving_0 = 0,
        ePausing_1 = 1,
        eBurst_2 = 2
    };
    SlurgStates mSlurgState;
    enum SlurgFlags
    {
        eGoingRight = 0x1, // TODO: double check me
        eMoving = 0x2,
    };
    BitField16<SlurgFlags> mSlurgFlags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_State, 0x2C);

struct TimerTrigger_State final
{
    AETypes field_0_type;
    s32 field_4_tlvInfo;
    s32 field_8_delay_timer_base;
    enum class TimerTriggerStates : s16
    {
        eWaitForEnabled_0 = 0,
        eWaitForFirstTrigger_1 = 1,
        eCheckForStartAgain_2 = 2,
        eWaitForSecondTrigger_3 = 3,
    };
    TimerTriggerStates field_C_state;
    s16 field_E_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TimerTrigger_State, 0x10);

struct TrapDoor_State final
{
    AETypes field_0_type;
    enum class TrapDoorState : s16
    {
        eClosed_0 = 0,
        eOpening_1 = 1,
        eOpen_2 = 2,
        eClosing_3 = 3,
    };
    TrapDoorState field_2_state;
    s32 field_4_open_time;
    s32 field_8_tlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TrapDoor_State, 0xC);

struct SaveState_UXB final
{
    AETypes mType;
    s16 field_2_padding;
    TlvItemInfoUnion mTlvInfo;
    u32 mNextStateTimer;
    enum class UXBState : u16
    {
        eDelay = 0,
        eActive = 1,
        eExploding = 2,
        eDeactivated = 3
    };
    UXBState mCurrentState;
    UXBState mStartingState;
    u16 mDisabledResources;
    u16 mPatternIndex;
    u16 mRedBlinkCount;
    u16 mIsRed;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SaveState_UXB, 24);

struct WorkWheel_SaveState final
{
    AETypes field_0_id;
    s16 padding_1;
    s32 field_4_tlvInfo;
    s16 field_8_snd_counter;
    s16 padding_2;
    enum class WheelStates : s16
    {
        eIdle_0 = 0,
        eTurning_1 = 1,
    };
    WheelStates field_C_state;
    s16 padding_3;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(WorkWheel_SaveState, 0x10);
}

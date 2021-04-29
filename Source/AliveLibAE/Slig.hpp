#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

#define SLIG_MOTIONS_ENUM(ENTRY)         \
    ENTRY(M_StandIdle_0_4B4EC0)          \
    ENTRY(M_StandToWalk_1_4B5F70)        \
    ENTRY(M_Walking_2_4B5BC0)            \
    ENTRY(M_StandToRun_3_4B62F0)         \
    ENTRY(M_Running_4_4B6000)            \
    ENTRY(M_TurnAroundStanding_5_4B6390) \
    ENTRY(M_Shoot_6_4B55A0)              \
    ENTRY(M_Falling_7_4B42D0)            \
    ENTRY(M_SlidingToStand_8_4B6520)     \
    ENTRY(M_SlidingTurn_9_4B6680)        \
    ENTRY(M_SlidingTurnToWalk_10_4B6800) \
    ENTRY(M_SlidingTurnToRun_11_4B6850)  \
    ENTRY(M_ReloadGun_12_4B5530)         \
    ENTRY(M_ShootToStand_13_4B5580)      \
    ENTRY(M_SteppingToStand_14_4B8480)   \
    ENTRY(M_StandingToStep_15_4B83B0)    \
    ENTRY(M_DepossessingAbort_16_4B8250) \
    ENTRY(M_GameSpeak_17_4B5290)         \
    ENTRY(M_WalkToStand_18_4B5FC0)       \
    ENTRY(M_Recoil_19_4B8270)            \
    ENTRY(M_SpeakHereBoy_20_4B5330)      \
    ENTRY(M_SpeakHi_21_4B53D0)           \
    ENTRY(M_SpeakFreeze_22_4B53F0)       \
    ENTRY(M_SpeakGitIm_23_4B5410)        \
    ENTRY(M_SpeakLaugh_24_4B5430)        \
    ENTRY(M_SpeakBullShit1_25_4B5450)    \
    ENTRY(M_SpeakLookOut_26_4B5470)      \
    ENTRY(M_SpeakBullShit2_27_4B5490)    \
    ENTRY(M_SpeakPanic_28_4B54B0)        \
    ENTRY(M_SpeakWhat_29_4B54D0)         \
    ENTRY(M_SpeakAIFreeze_30_4B54F0)     \
    ENTRY(M_Blurgh_31_4B5510)            \
    ENTRY(M_Sleeping_32_4B89A0)          \
    ENTRY(M_SleepingToStand_33_4B8C50)   \
    ENTRY(M_Knockback_34_4B68A0)         \
    ENTRY(M_KnockbackToStand_35_4B6A30)  \
    ENTRY(M_Depossessing_36_4B7F30)      \
    ENTRY(M_Possess_37_4B72C0)           \
    ENTRY(M_OutToFall_38_4B4570)         \
    ENTRY(M_FallingInitiate_39_4B4640)   \
    ENTRY(M_LandingSoft_40_4B4530)       \
    ENTRY(M_LandingFatal_41_4B4680)      \
    ENTRY(M_ShootZ_42_4B7560)            \
    ENTRY(M_ShootZtoStand_43_4B77E0)     \
    ENTRY(M_Smash_44_4B6B90)             \
    ENTRY(M_PullLever_45_4B8950)         \
    ENTRY(M_LiftGrip_46_4B3700)          \
    ENTRY(M_LiftUngrip_47_4B3820)        \
    ENTRY(M_LiftGripping_48_4B3850)      \
    ENTRY(M_LiftUp_49_4B3930)            \
    ENTRY(M_LiftDown_50_4B3960)          \
    ENTRY(M_Beat_51_4B6C00)

#define MAKE_ENUM(VAR) VAR,
enum eSligMotions : s32
{
    SLIG_MOTIONS_ENUM(MAKE_ENUM)
};

enum class SligSfx : s16
{
    // Normal Slig
    eToStand_0 = 0,
    eStandingTurn_1 = 1,
    eWalkingStep_2 = 2,
    eRunningStep_3 = 3,
    eSnooze2_4 = 4,
    eSnooze1_5 = 5,
    eReload1_6 = 6,
    eReload2_7 = 7,

    // Flying Slig
    eThrowGrenade_8 = 8,
    ePropeller1_9 = 9,
    ePropeller2_10 = 10,
    ePropeller3_11 = 11, // Apparently unused
    eCollideWithWall1_12 = 12,
    eCollideWithWall2_13 = 13,

    ePadding_14 = 14,
    ePadding_15 = 15,
    ePadding_16 = 16
};

struct Path_Slig final : public Path_TLV
{
    Scale_short field_10_scale;

    // TODO: Check these - GameEnder is probably only AO for instance ?
    enum class StartState : s16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        RunOffScreen_4 = 4,
        GameEnder_5 = 5,
        ListeningToGlukkon_6 = 6,
    };
    StartState field_12_start_state;

    s16 field_14_pause_time;
    s16 field_16_pause_left_min;
    s16 field_18_pause_left_max;
    s16 field_1A_pause_right_min;
    s16 field_1C_pause_right_max;
    s16 field_1E_chal_number;
    s16 field_20_chal_timer;
    s16 field_22_num_times_to_shoot;
    s16 field_24_padding; // TODO: or part of above field like in AO
    s16 field_26_code1;
    s16 field_28_code2;
    Choice_short field_2A_chase_abe_when_spotted;
    XDirection_short field_2C_start_direction;
    s16 field_2E_panic_timeout;
    s16 field_30_num_panic_sounds;
    s16 field_32_panic_sound_timeout;
    s16 field_34_stop_chase_delay;
    s16 field_36_time_to_wait_before_chase;
    s16 field_38_slig_id;
    s16 field_3A_listen_time;
    s16 field_3C_percent_say_what;
    s16 field_3E_percent_beat_mud;
    s16 field_40_talk_to_abe;
    s16 field_42_dont_shoot;
    s16 field_44_Z_shoot_delay;
    Choice_short field_46_stay_awake;
    s16 field_48_disable_resources;
    s16 field_4A_noise_wake_up_distance;
    s16 field_4C_id;
    s16 field_4E_unknown;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x50);

// This is a left bound, right bound and a persist.
struct Path_SligBound final : public Path_TLV
{
    s16 field_10_slig_id;
    s16 field_12_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SligBound, 0x14);

using Path_SligLeftBound = Path_SligBound;
using Path_SligRightBound = Path_SligBound;
using Path_SligPersist = Path_SligBound;


enum class LevelIds : s16;

class Slig;
using TSligAIFn = s16 (Slig::*)();
using TSligMotionFn = void (Slig::*)();

struct Slig_State
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
    s16 field_1C_scale;
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
    s16 field_42_ai_sub_state;
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
    LevelIds field_72_level;
    s16 field_74_path;
    s16 field_76_camera;
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

enum class GameSpeakEvents : s16;

class LiftPoint;

class Slig : public BaseAliveGameObject
{
public:
    EXPORT Slig* ctor_4B1370(Path_Slig* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VScreenChanged() override;

    virtual void VPossessed_408F70() override;

    virtual void VUnPosses_408F90() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual void VOnTrapDoorOpen() override;

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual s16 vIsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther) override;

    virtual s16 vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_4B3B50(const u8* pBuffer);

    EXPORT static s16 CCSTD IsAbeEnteringDoor_4BB990(BaseAliveGameObject* pThis);

private:
    EXPORT s32 vGetSaveState_4BFB10(Slig_State* pState);

public:
    EXPORT void M_StandIdle_0_4B4EC0();
    EXPORT void M_StandToWalk_1_4B5F70();
    EXPORT void M_Walking_2_4B5BC0();
    EXPORT void M_StandToRun_3_4B62F0();
    EXPORT void M_Running_4_4B6000();
    EXPORT void M_TurnAroundStanding_5_4B6390();
    EXPORT void M_Shoot_6_4B55A0();
    EXPORT void M_Falling_7_4B42D0();
    EXPORT void M_SlidingToStand_8_4B6520();
    EXPORT void M_SlidingTurn_9_4B6680();
    EXPORT void M_SlidingTurnToWalk_10_4B6800();
    EXPORT void M_SlidingTurnToRun_11_4B6850();
    EXPORT void M_ReloadGun_12_4B5530();
    EXPORT void M_ShootToStand_13_4B5580();
    EXPORT void M_SteppingToStand_14_4B8480();
    EXPORT void M_StandingToStep_15_4B83B0();
    EXPORT void M_DepossessingAbort_16_4B8250();
    EXPORT void M_GameSpeak_17_4B5290();
    EXPORT void M_WalkToStand_18_4B5FC0();
    EXPORT void M_Recoil_19_4B8270();
    EXPORT void M_SpeakHereBoy_20_4B5330();
    EXPORT void M_SpeakHi_21_4B53D0();
    EXPORT void M_SpeakFreeze_22_4B53F0();
    EXPORT void M_SpeakGitIm_23_4B5410();
    EXPORT void M_SpeakLaugh_24_4B5430();
    EXPORT void M_SpeakBullShit1_25_4B5450();
    EXPORT void M_SpeakLookOut_26_4B5470();
    EXPORT void M_SpeakBullShit2_27_4B5490();
    EXPORT void M_SpeakPanic_28_4B54B0();
    EXPORT void M_SpeakWhat_29_4B54D0();
    EXPORT void M_SpeakAIFreeze_30_4B54F0();
    EXPORT void M_Blurgh_31_4B5510();
    EXPORT void M_Sleeping_32_4B89A0();
    EXPORT void M_SleepingToStand_33_4B8C50();
    EXPORT void M_Knockback_34_4B68A0();
    EXPORT void M_KnockbackToStand_35_4B6A30();
    EXPORT void M_Depossessing_36_4B7F30();
    EXPORT void M_Possess_37_4B72C0();
    EXPORT void M_OutToFall_38_4B4570();
    EXPORT void M_FallingInitiate_39_4B4640();
    EXPORT void M_LandingSoft_40_4B4530();
    EXPORT void M_LandingFatal_41_4B4680();
    EXPORT void M_ShootZ_42_4B7560();
    EXPORT void M_ShootZtoStand_43_4B77E0();
    EXPORT void M_Smash_44_4B6B90();
    EXPORT void M_PullLever_45_4B8950();
    EXPORT void M_LiftGrip_46_4B3700();
    EXPORT void M_LiftUngrip_47_4B3820();
    EXPORT void M_LiftGripping_48_4B3850();
    EXPORT void M_LiftUp_49_4B3930();
    EXPORT void M_LiftDown_50_4B3960();
    EXPORT void M_Beat_51_4B6C00();

public:
    EXPORT s16 AI_Death_0_4BBFB0();
    EXPORT s16 AI_ReturnControlToAbeAndDie_1_4BC410();
    EXPORT s16 AI_Possessed_2_4BBCF0();
    EXPORT s16 AI_DeathDropDeath_3_4BC1E0();
    EXPORT s16 AI_ListeningToGlukkon_4_4B9D20();

    s16 AI_ListeningToGlukkon_LostAttention();

    s16 AI_ListeningToGlukkon_Shooting();

    s16 AI_ListeningToGlukkon_StoppingOnLift(LiftPoint* pPlatformObj);

    s16 AI_ListeningToGlukkon_PullingLever();

    s16 AI_ListenToGlukkon_StoppingNextToLever();

    s16 AI_ListenToGlukkon_Speaking();

    s16 AI_ListenToGlukkon_NextLever(BaseAliveGameObject* pGlukkonObj);

    s16 AI_ListenToGlukkon_Moving(BaseAliveGameObject* pGlukkonObj);

    s16 AI_ListenToGlukkon_IdleListen(BaseAliveGameObject* pGlukkonObj, LiftPoint* pPlatformObj);

    s16 AI_ListenToGlukkon_GettingAttention(BaseAliveGameObject* pGlukkonObj);

    EXPORT s16 AI_Empty_5_4B3220();
    EXPORT s16 AI_Empty_6_4B3420();
    EXPORT s16 AI_SpottedEnemy_7_4B3240();
    EXPORT s16 AI_Empty_8_4B3120();
    EXPORT s16 AI_Empty_9_4B3440();
    EXPORT s16 AI_EnemyDead_10_4B3460();
    EXPORT s16 AI_KilledEnemy_10_4B35C0();
    EXPORT s16 AI_PanicTurning_12_4BC490();
    EXPORT s16 AI_PanicRunning_13_4BC780();
    EXPORT s16 AI_PanicYelling_14_4BCA70();
    EXPORT s16 AI_Idle_15_4BD800();
    EXPORT s16 AI_StopChasing_16_4BCE30();
    EXPORT s16 AI_Chasing_17_4BCBD0();
    EXPORT s16 AI_StartChasing_18_4BCEB0();
    EXPORT s16 AI_Turning_19_4BDDD0();
    EXPORT s16 AI_StoppingNextToMudokon_20_4BF1E0();
    EXPORT s16 AI_Walking_21_4BE0C0();
    EXPORT s16 AI_GetAlertedTurn_22_4BE990();
    EXPORT s16 AI_GetAlerted_23_4BEC40();
    EXPORT s16 AI_BeatingUp_24_4BF2B0();
    EXPORT s16 AI_DiscussionWhat_25_4BF380();
    EXPORT s16 AI_Empty_26_4BF620();
    EXPORT s16 AI_Empty_27_4BF600();
    EXPORT s16 AI_ZShooting_28_4BFA70();
    EXPORT s16 AI_Shooting_29_4BF750();
    EXPORT s16 AI_ZSpottedEnemy_30_4BFA30();
    EXPORT s16 AI_WakingUp_31_4B9390();
    EXPORT s16 AI_Inactive_32_4B9430();
    EXPORT s16 AI_Paused_33_4B8DD0();
    EXPORT s16 AI_Sleeping_34_4B9170();
    EXPORT s16 AI_GameEnder_35_4BF640();

private:
    EXPORT void Init_4BB0D0();

    EXPORT void dtor_4B1CF0();


    EXPORT Slig* vdtor_4B1790(s32 flags);

    EXPORT void vUpdate_4B17C0();

    EXPORT void vRender_4B1F80(PrimHeader** ot);

    EXPORT void vScreenChanged_4B1E20();

    EXPORT void vPossessed_4B2F10();

    EXPORT void vUnPosses_4B3050();

    EXPORT void vOnTlvCollision_4B2FB0(Path_TLV* pTlv);

    EXPORT void vShot_4B2EA0();

    EXPORT void vUpdateAnim_4B1320();


public:
    EXPORT BOOL vUnderGlukkonCommand_4B1760();

private:
    EXPORT void WakeUp_4B93B0();

    EXPORT void ShouldStilBeAlive_4BBC00();

    EXPORT u8** ResForMotion_4B1E90(s16 motion);

    EXPORT void ToTurn_4BE090();

    EXPORT void RespondWithWhat_4BF400();

    EXPORT void ToShoot_4BF9A0();

    EXPORT void ToZShoot_4BF9E0();

    EXPORT void PauseALittle_4BDD00();

    EXPORT void ToStand_4B4A20();

    EXPORT void BlowToGibs_4B8020();

    EXPORT s16 MainMovement_4B4720();

    enum class MovementDirection
    {
        eLeft = 0,
        eRight = 1
    };

    void PullLever();
    void ShootOrShootZ();

    s16 ToShootZ();
    s16 LeftRigtMovement(MovementDirection direction);
    s16 GrabNearbyLift();

    EXPORT s16 HandlePlayerControlled_4B7800();

    EXPORT s16 GetNextMotionIncGameSpeak_4B5080(s32 input);

    EXPORT void WaitOrWalk_4BE870();

    EXPORT void ToAbeDead_4B3580();

    EXPORT void ToUnderGlukkonCommand_4B9660();

    EXPORT void ToKilledAbe_4B3600();

    EXPORT BOOL IsWallBetween_4BB8B0(BaseAliveGameObject* pLeft, BaseAliveGameObject* pRight);

    EXPORT GameSpeakEvents LastGlukkonSpeak_4B3090();

    EXPORT s16 ListenToGlukkonCommands_4B95D0();

    EXPORT void PlatformCollide_4B4E00();

    EXPORT void FallKnockBackOrSmash_4B4A90();

    EXPORT void TurnOrSayWhat_4BEBC0();

    EXPORT void GameSpeakResponse_4BF470();

    EXPORT void GoAlertedOrSayWhat_4BF140();

    EXPORT static s32 CCSTD IsFacingEffectiveLeft_4BB780(Slig* pSlig);

    EXPORT void MoveOnLine_4B4C40();

    EXPORT void PlayerControlStopWalkingIfRequired_4B8540();

    EXPORT void CheckPlatformVanished_4B3640();

    EXPORT void vOnTrapDoorOpen_4B3690();

    EXPORT s16 MoveLift_4B3990(FP ySpeed);

    EXPORT void SlowDown_4B6450(FP speed);

    EXPORT void ToChase_4BCFF0();

    EXPORT s16 HandleEnemyStopper_4BBA00(s32 gridBlocks);

    EXPORT void ToPanicYelling_4BCBA0();

    EXPORT void ToPanicTurn_4BC750();

    EXPORT static s16 CC SligStableDelay_4B99B0();

    EXPORT void PlayerControlRunningSlideStopOrTurnFrame12_4B8790();

    EXPORT void PlayerControlRunningSlideStopOrTurnFrame4_4B85D0();

    EXPORT BaseAliveGameObject* FindBeatTarget_4BD070(s32 a2, s32 gridBlocks);

    EXPORT void TurnOrWalk_4BD6A0(s32 a2);

    EXPORT void ToPanicRunning_4BCA30();

    EXPORT void RespondToEnemyOrPatrol_4B3140();


    EXPORT s16 FindSwitch_4B9A50();

    EXPORT s16 NearOrFacingActiveChar_4B9930(BaseAliveGameObject* pObj);

    EXPORT static s16 CCSTD InZCover_4BB7C0(BaseAliveGameObject* pObj);

    EXPORT static BOOL CCSTD RenderLayerIs_4BBBC0(BaseAliveGameObject* pThis);

    EXPORT void NextCommand_4B9A00(s16 speakTableIndex, s16 responseState);

    EXPORT s16 HeardGlukkonToListenTo_4B9690(GameSpeakEvents glukkonSpeak);

    EXPORT s16 vTakeDamage_4B2470(BaseGameObject* pFrom);

    EXPORT s16 vIsFacingMe_4B23D0(BaseAnimatedWithPhysicsGameObject* pWho);

    EXPORT s16 vOnSameYLevel_4BB6C0(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT s16 FindLiftPoint_4B9B40();

public:
    void SetBrain(TSligAIFn fn);
    bool BrainIs(TSligAIFn fn);

private:
    s32 field_118_tlvInfo;

public:
    s16 field_11C_ai_sub_state;

private:
    s16 field_11E_pitch_min;
    s32 field_120_timer;
    s16 field_124_return_to_previous_motion;
    s16 field_126_checked_if_off_screen;
    s32 field_128_input;
    s32 field_12C_timer;
    FP field_130_falling_velx_scale_factor;
    s16 field_134_res_idx;
    s16 field_136_shot_motion;
    PSX_RECT field_138_zone_rect;
    s16 field_140_unused;
    s16 field_142_unused;
    s16 field_144_unused;

public:
    LevelIds field_146_level;
    s16 field_148_path;
    s16 field_14A_camera;
    s32 field_14C_death_by_being_shot_timer;
    s32 field_150_explode_timer;

private:
    TSligAIFn field_154_brain_state;
    s16 field_158_num_times_to_shoot;
    s16 field_15A_unused;
    s16 field_15C_force_alive_state;
    s16 field_15E_spotted_possessed_slig;
    s32 field_160_last_event_index;
    s16 field_164_padding;
    s16 field_166_padding;
    s16 field_168_padding;
    s16 field_16A_padding;
    s16 field_16C_padding;
    s16 field_16E_padding;
    s16 field_170_padding;
    s16 field_172_padding;
    s16 field_174_unused;
    s16 field_176_unused;
    s16 field_178_pPalAlloc[64];
    PSX_RECT field_1F8_pal_rect;
    s16 field_200_red;
    s16 field_202_green;
    s16 field_204_blue;
    s16 field_206_padding;
    s32 field_208_glukkon_obj_id;
    s16 field_20C_state_after_speak;
    s16 field_20E_attention_timeout;
    s16 field_210_unused;
    s16 field_212_next_command_arg1;
    s16 field_214_cmd_idx;

    enum Flags_216
    {
        eBit1_FollowGlukkon = 0x1,
        eBit2_StoppedForLeverOrLift = 0x2,
        eBit3_GlukkonCalledAllOYa = 0x4,
        eBit4_HeardGlukkon = 0x8,
        eBit5_Padding = 0x10,
    };
    BitField16<Flags_216> field_216_flags;

    Path_Slig field_218_tlv_data;
    PSX_Point field_268_points[10];
    s16 field_290_points_count;
    s16 field_292_prevent_depossession;
    s32 field_294_next_gamespeak_motion;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x298);

EXPORT s32 CC Animation_OnFrame_Slig_4C0600(void* pObj, s16* pData);
EXPORT void CC Slig_SoundEffect_4BFFE0(SligSfx effect, BaseAliveGameObject* pObj);

void renderWithGlowingEyes(PrimHeader** ot, BaseAliveGameObject* actor, s16* pPalAlloc, s16 palSize, PSX_RECT* palRect,
                           s16& r, s16& g, s16& b,
                           const s16* eyeColourIndices, s16 eyeColourIndicesSize);

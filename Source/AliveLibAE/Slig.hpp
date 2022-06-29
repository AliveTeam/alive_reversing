#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "../relive_lib/MapWrapper.hpp"

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
    ENTRY(M_SpeakGetHim_23_4B5410)        \
    ENTRY(M_SpeakLaugh_24_4B5430)        \
    ENTRY(M_SpeakBullshit1_25_4B5450)    \
    ENTRY(M_SpeakLookOut_26_4B5470)      \
    ENTRY(M_SpeakBullshit2_27_4B5490)    \
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
        Patrol_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        ChaseAndDisappear_4 = 4,
        Unused_5 = 5,
        ListeningToGlukkon_6 = 6,
    };
    StartState field_12_start_state;

    s16 field_14_pause_time;
    s16 field_16_pause_left_min;
    s16 field_18_pause_left_max;
    s16 field_1A_pause_right_min;
    s16 field_1C_pause_right_max;
    enum class ShootPossessedSligs : s16
    {
        eYes_0 = 0,
        eYes_1 = 1,
        eNo_3 = 3, // it really had to be the magic number 3 *sigh*
    };
    ShootPossessedSligs field_1E_shoot_possessed_sligs;
    s16 field_20_shoot_on_sight_delay;
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
    s16 field_38_slig_bound_id;
    s16 field_3A_alerted_listen_time;
    s16 field_3C_percent_say_what;
    s16 field_3E_percent_beat_mud;
    s16 field_40_talk_to_abe;
    s16 field_42_dont_shoot;
    s16 field_44_Z_shoot_delay;
    Choice_short field_46_stay_awake;
    s16 field_48_disable_resources;
    s16 field_4A_noise_wake_up_distance;
    s16 field_4C_slig_spawner_switch_id;
    Choice_short field_4E_unlimited_spawns;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x50);

// This is a left bound, right bound and a persist.
struct Path_SligBound final : public Path_TLV
{
    s16 field_10_slig_bound_id;
    s16 field_12_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SligBound, 0x14);

using Path_SligLeftBound = Path_SligBound;
using Path_SligRightBound = Path_SligBound;
using Path_SligPersist = Path_SligBound;


enum class LevelIds : s16;

class Slig;
using TSligBrainFn = s16 (Slig::*)();
using TSligMotionFn = void (Slig::*)();

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

enum class GameSpeakEvents : s16;

class LiftPoint;

class Slig final : public BaseAliveGameObject
{
public:
    Slig(Path_Slig* pTlv, s32 tlvInfo);
    ~Slig();

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VScreenChanged() override;
    virtual void VPossessed() override;
    virtual void VUnPosses() override;
    virtual void VOnTlvCollision(Path_TLV* pTlv) override;
    virtual void VOnTrapDoorOpen() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);
    static s16 IsAbeEnteringDoor_4BB990(BaseAliveGameObject* pThis);

    void M_StandIdle_0_4B4EC0();
    void M_StandToWalk_1_4B5F70();
    void M_Walking_2_4B5BC0();
    void M_StandToRun_3_4B62F0();
    void M_Running_4_4B6000();
    void M_TurnAroundStanding_5_4B6390();
    void M_Shoot_6_4B55A0();
    void M_Falling_7_4B42D0();
    void M_SlidingToStand_8_4B6520();
    void M_SlidingTurn_9_4B6680();
    void M_SlidingTurnToWalk_10_4B6800();
    void M_SlidingTurnToRun_11_4B6850();
    void M_ReloadGun_12_4B5530();
    void M_ShootToStand_13_4B5580();
    void M_SteppingToStand_14_4B8480();
    void M_StandingToStep_15_4B83B0();
    void M_DepossessingAbort_16_4B8250();
    void M_GameSpeak_17_4B5290();
    void M_WalkToStand_18_4B5FC0();
    void M_Recoil_19_4B8270();
    void M_SpeakHereBoy_20_4B5330();
    void M_SpeakHi_21_4B53D0();
    void M_SpeakFreeze_22_4B53F0();
    void M_SpeakGetHim_23_4B5410();
    void M_SpeakLaugh_24_4B5430();
    void M_SpeakBullshit1_25_4B5450();
    void M_SpeakLookOut_26_4B5470();
    void M_SpeakBullshit2_27_4B5490();
    void M_SpeakPanic_28_4B54B0();
    void M_SpeakWhat_29_4B54D0();
    void M_SpeakAIFreeze_30_4B54F0();
    void M_Blurgh_31_4B5510();
    void M_Sleeping_32_4B89A0();
    void M_SleepingToStand_33_4B8C50();
    void M_Knockback_34_4B68A0();
    void M_KnockbackToStand_35_4B6A30();
    void M_Depossessing_36_4B7F30();
    void M_Possess_37_4B72C0();
    void M_OutToFall_38_4B4570();
    void M_FallingInitiate_39_4B4640();
    void M_LandingSoft_40_4B4530();
    void M_LandingFatal_41_4B4680();
    void M_ShootZ_42_4B7560();
    void M_ShootZtoStand_43_4B77E0();
    void M_Smash_44_4B6B90();
    void M_PullLever_45_4B8950();
    void M_LiftGrip_46_4B3700();
    void M_LiftUngrip_47_4B3820();
    void M_LiftGripping_48_4B3850();
    void M_LiftUp_49_4B3930();
    void M_LiftDown_50_4B3960();
    void M_Beat_51_4B6C00();

public:
    s16 Brain_Death_0_4BBFB0();
    s16 Brain_ReturnControlToAbeAndDie_1_4BC410();
    s16 Brain_Possessed_2_4BBCF0();
    s16 Brain_DeathDropDeath_3_4BC1E0();
    s16 Brain_ListeningToGlukkon_4_4B9D20();
    s16 Brain_ListeningToGlukkon_LostAttention();
    s16 Brain_ListeningToGlukkon_Shooting();
    s16 Brain_ListeningToGlukkon_StoppingOnLift(LiftPoint* pPlatformObj);
    s16 Brain_ListeningToGlukkon_PullingLever();
    s16 Brain_ListenToGlukkon_StoppingNextToLever();
    s16 Brain_ListenToGlukkon_Speaking();
    s16 Brain_ListenToGlukkon_NextLever(BaseAliveGameObject* pGlukkonObj);
    s16 Brain_ListenToGlukkon_Moving(BaseAliveGameObject* pGlukkonObj);
    s16 Brain_ListenToGlukkon_IdleListen(BaseAliveGameObject* pGlukkonObj, LiftPoint* pPlatformObj);
    s16 Brain_ListenToGlukkon_GettingAttention(BaseAliveGameObject* pGlukkonObj);

    s16 Brain_Empty_5_4B3220();
    s16 Brain_Empty_6_4B3420();
    s16 Brain_SpottedEnemy_7_4B3240();
    s16 Brain_Empty_8_4B3120();
    s16 Brain_Empty_9_4B3440();
    s16 Brain_EnemyDead_10_4B3460();
    s16 Brain_KilledEnemy_10_4B35C0();
    s16 Brain_PanicTurning_12_4BC490();
    s16 Brain_PanicRunning_13_4BC780();
    s16 Brain_PanicYelling_14_4BCA70();
    s16 Brain_Idle_15_4BD800();
    s16 Brain_StopChasing_16_4BCE30();
    s16 Brain_Chasing_17_4BCBD0();
    s16 Brain_StartChasing_18_4BCEB0();
    s16 Brain_Turning_19_4BDDD0();
    s16 Brain_StoppingNextToMudokon_20_4BF1E0();
    s16 Brain_Walking_21_4BE0C0();
    s16 Brain_GetAlertedTurn_22_4BE990();
    s16 Brain_GetAlerted_23_4BEC40();
    s16 Brain_BeatingUp_24_4BF2B0();
    s16 Brain_DiscussionWhat_25_4BF380();
    s16 Brain_Empty_26_4BF620();
    s16 Brain_Empty_27_4BF600();
    s16 Brain_ZShooting_28_4BFA70();
    s16 Brain_Shooting_29_4BF750();
    s16 Brain_ZSpottedEnemy_30_4BFA30();
    s16 Brain_WakingUp_31_4B9390();
    s16 Brain_Inactive_32_4B9430();
    s16 Brain_Paused_33_4B8DD0();
    s16 Brain_Sleeping_34_4B9170();
    s16 Brain_ChaseAndDisappear_35_4BF640();

private:
    void Init();

    void vShot_4B2EA0();

    void vUpdateAnim_4B1320();


public:
    Bool32 vUnderGlukkonCommand_4B1760();

private:
    void WakeUp_4B93B0();

    void ShouldStillBeAlive_4BBC00();

    u8** ResForMotion_4B1E90(s16 motion);

    void ToTurn_4BE090();

    void RespondWithWhat_4BF400();

    void ToShoot_4BF9A0();

    void ToZShoot_4BF9E0();

    void PauseALittle_4BDD00();

    void ToStand_4B4A20();

    void BlowToGibs_4B8020();

    s16 MainMovement_4B4720();

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

    s16 HandlePlayerControlled_4B7800();

    s16 GetNextMotionIncGameSpeak_4B5080(s32 input);

    void WaitOrWalk_4BE870();

    void ToAbeDead_4B3580();

    void ToUnderGlukkonCommand_4B9660();

    void ToKilledAbe_4B3600();

    Bool32 IsWallBetween_4BB8B0(BaseAliveGameObject* pLeft, BaseAliveGameObject* pRight);

    GameSpeakEvents LastGlukkonSpeak_4B3090();

    s16 ListenToGlukkonCommands_4B95D0();

    void PlatformCollide_4B4E00();

    void FallKnockBackOrSmash_4B4A90();

    void TurnOrSayWhat_4BEBC0();

    void GameSpeakResponse_4BF470();

    void GoAlertedOrSayWhat_4BF140();

    static s32 IsFacingEffectiveLeft_4BB780(Slig* pSlig);

    void MoveOnLine_4B4C40();

    void PlayerControlStopWalkingIfRequired_4B8540();

    void CheckPlatformVanished_4B3640();

    s16 MoveLift_4B3990(FP ySpeed);

    void SlowDown_4B6450(FP speed);

    void ToChase_4BCFF0();

    s16 HandleEnemyStopper_4BBA00(s32 gridBlocks);

    void ToPanicYelling_4BCBA0();

    void ToPanicTurn_4BC750();

    static s16 SligStableDelay_4B99B0();

    void PlayerControlRunningSlideStopOrTurnFrame12_4B8790();

    void PlayerControlRunningSlideStopOrTurnFrame4_4B85D0();

    BaseAliveGameObject* FindBeatTarget_4BD070(ReliveTypes typeToBeat, s32 gridBlocks);

    void TurnOrWalk_4BD6A0(s32 a2);

    void ToPanicRunning_4BCA30();

    void RespondToEnemyOrPatrol_4B3140();


    s16 FindSwitch_4B9A50();

    s16 NearOrFacingActiveChar_4B9930(BaseAliveGameObject* pObj);

    static s16 InZCover_4BB7C0(BaseAliveGameObject* pObj);

    static Bool32 RenderLayerIs_4BBBC0(BaseAliveGameObject* pThis);

    void NextCommand_4B9A00(s16 speakTableIndex, s16 responseState);

    s16 HeardGlukkonToListenTo_4B9690(GameSpeakEvents glukkonSpeak);

    s16 vIsFacingMe_4B23D0(BaseAnimatedWithPhysicsGameObject* pWho);

    s16 vOnSameYLevel_4BB6C0(BaseAnimatedWithPhysicsGameObject* pOther);

    s16 FindLiftPoint_4B9B40();

public:
    void SetBrain(TSligBrainFn fn);
    bool BrainIs(TSligBrainFn fn);

private:
    s32 field_118_tlvInfo = 0;

public:
    s16 field_11C_brain_sub_state = 0;

private:
    s16 field_11E_pitch_min = 0;
    s32 field_120_timer = 0;
    s16 field_124_return_to_previous_motion = 0;
    s16 field_126_checked_if_off_screen = 0;
    s32 field_128_input = 0;
    s32 field_12C_timer = 0;
    FP field_130_falling_velx_scale_factor = {};
    s16 field_134_res_idx = 0;
    s16 field_136_shot_motion = 0;
    PSX_RECT field_138_zone_rect = {};
    s16 field_140_unused = 0;
    s16 field_142_unused = 0;
    s16 field_144_unused = 0;

public:
    EReliveLevelIds field_146_return_level = EReliveLevelIds::eNone;
    s16 field_148_return_path = 0;
    s16 field_14A_return_camera = 0;
    s32 field_14C_death_by_being_shot_timer = 0;
    s32 field_150_explode_timer = 0;

private:
    TSligBrainFn field_154_brain_state = nullptr;
    s16 field_158_num_times_to_shoot = 0;
    s16 field_15A_unused = 0;
    s16 field_15C_force_alive_state = 0;
    s16 field_15E_spotted_possessed_slig = 0;
    s32 field_160_last_event_index = 0;
    s16 field_174_unused = 0;
    s16 field_176_unused = 0;
    s16 field_178_pPalAlloc[64] = {};
    PSX_RECT field_1F8_pal_rect = {};
    s16 field_200_red = 0;
    s16 field_202_green = 0;
    s16 field_204_blue = 0;
    s32 field_208_glukkon_obj_id = 0;
    s16 field_20C_state_after_speak = 0;
    s16 field_20E_attention_timeout = 0;
    s16 field_210_unused = 0;
    s16 field_212_next_command_arg1 = 0;
    s16 field_214_cmd_idx = 0;

    enum Flags_216
    {
        eBit1_FollowGlukkon = 0x1,
        eBit2_StoppedForLeverOrLift = 0x2,
        eBit3_GlukkonCalledAllOYa = 0x4,
        eBit4_HeardGlukkon = 0x8,
        eBit5_Padding = 0x10,
    };
    BitField16<Flags_216> field_216_flags = {};

    Path_Slig field_218_tlv_data = {};
    PSX_Point field_268_points[10] = {};
    s16 field_290_points_count = 0;
    s16 field_292_prevent_depossession = 0;
    s32 field_294_next_gamespeak_motion = 0;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x298);

s32 Animation_OnFrame_Slig_4C0600(BaseGameObject* pObj, s16* pData);
void Slig_SoundEffect_4BFFE0(SligSfx effect, BaseAliveGameObject* pObj);

void renderWithGlowingEyes(PrimHeader** ot, BaseAliveGameObject* actor, s16* pPalAlloc, s16 palSize, PSX_RECT* palRect,
                           s16& r, s16& g, s16& b,
                           const s16* eyeColourIndices, s16 eyeColourIndicesSize);

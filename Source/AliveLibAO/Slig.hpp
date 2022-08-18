#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

enum SligFlags_DisabledRes
{
    eDisabledRes_Bit1_SligLever = 0x1,
    eDisabledRes_Bit2_SligLift = 0x2,
    eDisabledRes_Bit3_SligZ = 0x4,
    eDisabledRes_Bit4 = 0x8,
    eDisabledRes_Bit5 = 0x10,
    eDisabledRes_Bit6 = 0x20,
    eDisabledRes_Bit7_SligSleep = 0x40,
    eDisabledRes_Bit8_SligKnfd = 0x80,
    eDisabledRes_Bit9_SligEdge = 0x100,
    eDisabledRes_Bit10_SligSmash = 0x200,
    eDisabledRes_Bit11_SligBeat = 0x400
};

struct Path_Slig final : public Path_TLV
{
    enum class StartState : s16
    {
        Listening_0 = 0,
        Patrol_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        ChaseAndDisappear_4 = 4,
        FallingToChase_5 = 5,
    };
    Scale_short field_18_scale;
    StartState field_1A_start_state;
    s16 field_1C_pause_time;
    s16 field_1E_pause_left_min;
    s16 field_20_pause_left_max;
    s16 field_22_pause_right_min;
    s16 field_24_pause_right_max;
    enum class ShootPossessedSligs : s16
    {
        eNo_0 = 0,
        eYes_1 = 1,
        eYes_2 = 2, // used in an OG level, breaks lvl exporting if removed
    };
    ShootPossessedSligs field_26_shoot_possessed_sligs;
    s16 field_28_shoot_on_sight_delay;
    s16 field_2A_num_times_to_shoot;
    s16 field_2C_unknown; // TODO: Part of above field, check me?
    s16 field_2E_code1;
    s16 field_30_code2;
    Choice_short field_32_chase_abe;
    XDirection_short field_34_start_direction;
    s16 field_36_panic_timeout;
    s16 field_38_num_panic_sounds;
    s16 field_3A_panic_sound_timeout;
    s16 field_3C_stop_chase_delay;
    s16 field_3E_time_to_wait_before_chase;
    s16 field_40_slig_bound_id;
    s16 field_42_listen_time;
    s16 field_44_percent_say_what;
    s16 field_46_percent_beat_mud;
    s16 field_48_talk_to_abe;
    s16 field_4A_dont_shoot;
    s16 field_4C_z_shoot_delay;
    Choice_short field_4E_stay_awake;
    BitField16<SligFlags_DisabledRes> field_50_disabled_resources;
    s16 field_52_noise_wake_up_distance;
    s16 field_54_slig_spawner_switch_id;
    s16 field_56_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x58);

#define SLIG_MOTIONS_ENUM_AO(ENTRY)              \
    ENTRY(Motion_0_StandIdle_467640)          \
    ENTRY(Motion_1_StandToWalk_4695D0)        \
    ENTRY(Motion_2_Walking_469130)            \
    ENTRY(Motion_3_StandToRun_469C00)         \
    ENTRY(Motion_4_Running_469690)            \
    ENTRY(Motion_5_TurnAroundStanding_469C80) \
    ENTRY(Motion_6_Shoot_468820)              \
    ENTRY(Motion_7_Falling_46A1A0)            \
    ENTRY(Motion_8_Unknown_4673E0)            \
    ENTRY(Motion_9_SlidingToStand_469DF0)     \
    ENTRY(Motion_10_SlidingTurn_469F10)       \
    ENTRY(Motion_11_SlidingTurnToWalk_46A120) \
    ENTRY(Motion_12_SlidingTurnToRun_46A160)  \
    ENTRY(Motion_13_ReloadGun_4687B0)            \
    ENTRY(Motion_14_ShootToStand_468810)      \
    ENTRY(Motion_15_SteppingToStand_469080)   \
    ENTRY(Motion_16_StandingToStep_468FD0)    \
    ENTRY(Motion_17_DepossessingAbort_468750) \
    ENTRY(Motion_18_GameSpeak_467B10)         \
    ENTRY(Motion_19_WalkToStand_469610)       \
    ENTRY(Motion_20_Recoil_468D30)            \
    ENTRY(Motion_21_SpeakHereBoy_467BD0)      \
    ENTRY(Motion_22_SpeakHi_467C90)           \
    ENTRY(Motion_23_SpeakFreeze_467D50)       \
    ENTRY(Motion_24_SpeakGetHim_467E10)       \
    ENTRY(Motion_25_SpeakLaugh_467ED0)        \
    ENTRY(Motion_26_SpeakBullshit1_467F90)    \
    ENTRY(Motion_27_SpeakLookOut_468050)      \
    ENTRY(Motion_28_SpeakBullshit2_468110)    \
    ENTRY(Motion_29_SpeakPanic_4681D0)        \
    ENTRY(Motion_30_SpeakWhat_468290)         \
    ENTRY(Motion_31_SpeakAIFreeze_468350)     \
    ENTRY(Motion_32_Blurgh_468410)            \
    ENTRY(Motion_33_Sleeping_46A410)          \
    ENTRY(Motion_34_SleepingToStand_46A5F0)   \
    ENTRY(Motion_35_Knockback_46A720)         \
    ENTRY(Motion_36_KnockbackToStand_46A7F0)  \
    ENTRY(Motion_37_Depossessing_4684D0)      \
    ENTRY(Motion_38_Possess_46B050)           \
    ENTRY(Motion_39_OutToFall_46A9E0)         \
    ENTRY(Motion_40_FallingInitiate_46AA60)   \
    ENTRY(Motion_41_LandingSoft_46A390)       \
    ENTRY(Motion_42_LandingFatal_46AFE0)      \
    ENTRY(Motion_43_ShootZ_468E30)            \
    ENTRY(Motion_44_ShootZtoStand_468F70)     \
    ENTRY(Motion_45_Smash_46A990)             \
    ENTRY(State_46_PullLever_46A590)         \
    ENTRY(Motion_47_LiftUp_4665A0)            \
    ENTRY(Motion_48_LiftDown_4665C0)          \
    ENTRY(Motion_49_LiftGrip_4663A0)          \
    ENTRY(Motion_50_LiftUngrip_466460)        \
    ENTRY(Motion_51_LiftGripping_466480)      \
    ENTRY(Motion_52_Beat_46AA90)

#define MAKE_ENUM(VAR) VAR,
enum eSligMotions : s32
{
    SLIG_MOTIONS_ENUM_AO(MAKE_ENUM)
};

struct SligResources final
{
    u8** res[17];
};

enum class SligSpeak : u8
{
    eHi_0 = 0,
    eHereBoy_1 = 1,
    eGetHim_2 = 2,
    eStay_3 = 3,
    eBullshit_4 = 4,
    eLookOut_5 = 5,
    eBullshit2_6 = 6,
    eLaugh_7 = 7,
    eFreeze_8 = 8,
    eWhat_9 = 9,
    eHelp_10 = 10,
    eBlurgh_11 = 11
};

enum class SligSfx : s8
{
    eToStand_0 = 0,
    eStandingTurn_1 = 1,
    eWalkingStep_2 = 2,
    eRunningStep_3 = 3,
    eSnooze1_4 = 4,
    eSnooze2_5 = 5,

    // These are all unused
    eHi_6 = 6,
    eHereBoy_7 = 7,
    eGetHim_8 = 8,
    eStay_9 = 9,
    eBullshit_10 = 10,
    eLookOut_11 = 11,
    eBullshit2_12 = 12,
    eLaugh_13 = 13,
    eFreeze_14 = 14,
    eWhat_15 = 15,
    eHelp_16 = 16,
    eBlurgh_17 = 17,
};

struct Path_ZSligCover final : public Path_TLV
{
    // No fields
};

// This is a left bound, right bound and a persist.
struct Path_SligBound final : public Path_TLV
{
    s16 field_18_slig_id;
    BitField16<SligFlags_DisabledRes> field_1A_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SligBound, 0x1C);

using Path_SligLeftBound = Path_SligBound;
using Path_SligRightBound = Path_SligBound;
using Path_SligPersist = Path_SligBound;

class Slig final : public BaseAliveGameObject
{
public:
    Slig(Path_Slig* pTlv, s32 tlvInfo);
    ~Slig();

    void Init();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUnPosses() override;
    virtual void VPossessed() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(Path_TLV* pTlv) override;
    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    void VUpdateAnimData();
    void Vshot();
    void MoveOnLine();
    void ToKnockBack();
    void PlayerControlRunningSlideStopOrTurnFrame4();
    void PlayerControlRunningSlideStopOrTurnFrame12();
    void SlowDown(FP speed);
    s16 HandlePlayerControlled();
    void OperateLift();
    s16 MainMovement_467020();
    void Slig_SoundEffect_46F310(SligSfx sfxIdx);
    u8** ResBlockForMotion_4654D0(s16 motion);
    bool VIs8_465630(s16 motion);
    void ToShoot_46F1D0();
    void ToZShoot_46F200();
    void ShouldStilBeAlive_46C0D0();
    void ToAbeDead_466270();
    void WaitOrWalk_46E440();
    s32 IsFacingEffectiveLeft_46BD70(Slig* pSlig);
    void PauseALittle_46DBD0();
    void ToTurn_46DE70();
    void ToPanicRunning_46CC20();
    void ToPanic_46CD40();
    void ToChase_46D080();
    void ToKilledAbe_4662E0();
    s16 FindBeatTarget_46D0E0(s32 typeToFind, s32 gridBlocks);
    s16 HandleEnemyStopper_46BF30(s32 gridBlocks);
    void RespondToEnemyOrPatrol_465DF0();
    void TurnOrWalk_46D5B0(s32 a2);
    void ToPanicTurn();
    void PlayerControlRunningSlideStopOrTurn(s16 last_anim_frame);
    s16 GetNextMotionIncGameSpeak_467700(u16 input);
    static bool RenderLayerIs_46C0A0(BaseAliveGameObject* pThis);
    static s16 IsAbeEnteringDoor_46BEE0(BaseAliveGameObject* pThis);
    static s16 IsWallBetween_46BE60(Slig* pLeft, BaseAliveGameObject* pRight);
    static void Slig_GameSpeak_SFX_46F560(SligSpeak effectId, s32 defaultVol, s32 pitch_min, BaseAliveGameObject* pObj);
    static s16 IsInInvisibleZone_418870(BaseAnimatedWithPhysicsGameObject* pObj);
    void ToStand();
    static s16 IsInZCover_46BDA0(BaseAnimatedWithPhysicsGameObject* pObj);
    void CheckPlatformVanished();
    s16 MoveLift_4665E0(FP ySpeed);
    void GameSpeakResponse_46ED60();


    // Motions
    void Motion_0_StandIdle_467640();
    void Motion_1_StandToWalk_4695D0();
    void Motion_2_Walking_469130();
    void Motion_3_StandToRun_469C00();
    void Motion_4_Running_469690();
    void Motion_5_TurnAroundStanding_469C80();
    void Motion_6_Shoot_468820();
    void Motion_7_Falling_46A1A0();
    void Motion_8_Unknown_4673E0();
    void Motion_9_SlidingToStand_469DF0();
    void Motion_10_SlidingTurn_469F10();
    void Motion_11_SlidingTurnToWalk_46A120();
    void Motion_12_SlidingTurnToRun_46A160();
    void Motion_13_ReloadGun_4687B0();
    void Motion_14_ShootToStand_468810();
    void Motion_15_SteppingToStand_469080();
    void Motion_16_StandingToStep_468FD0();
    void Motion_17_DepossessingAbort_468750();
    void Motion_18_GameSpeak_467B10();
    void Motion_19_WalkToStand_469610();
    void Motion_20_Recoil_468D30();
    void Motion_21_SpeakHereBoy_467BD0();
    void Motion_22_SpeakHi_467C90();
    void Motion_23_SpeakFreeze_467D50();
    void Motion_24_SpeakGetHim_467E10();
    void Motion_25_SpeakLaugh_467ED0();
    void Motion_26_SpeakBullshit1_467F90();
    void Motion_27_SpeakLookOut_468050();
    void Motion_28_SpeakBullshit2_468110();
    void Motion_29_SpeakPanic_4681D0();
    void Motion_30_SpeakWhat_468290();
    void Motion_31_SpeakAIFreeze_468350();
    void Motion_32_Blurgh_468410();
    void Motion_33_Sleeping_46A410();
    void Motion_34_SleepingToStand_46A5F0();
    void Motion_35_Knockback_46A720();
    void Motion_36_KnockbackToStand_46A7F0();
    void Motion_37_Depossessing_4684D0();
    void Motion_38_Possess_46B050();
    void Motion_39_OutToFall_46A9E0();
    void Motion_40_FallingInitiate_46AA60();
    void Motion_41_LandingSoft_46A390();
    void Motion_42_LandingFatal_46AFE0();
    void Motion_43_ShootZ_468E30();
    void Motion_44_ShootZtoStand_468F70();
    void Motion_45_Smash_46A990();
    void Motion_46_ToIdle_46A590();
    void Motion_47_LiftUp_4665A0();
    void Motion_48_LiftDown_4665C0();
    void Motion_49_LiftGrip_4663A0();
    void Motion_50_LiftUngrip_466460();
    void Motion_51_LiftGripping_466480();
    void Motion_52_Beat_46AA90();

    // Brains
    s16 Brain_SpottedEnemy_465EB0();
    s16 Brain_Paused_466030();
    s16 Brain_EnemyDead_466190();
    s16 Brain_KilledEnemy_4662A0();
    s16 Brain_Unknown_46B250();
    s16 Brain_Sleeping_46B4E0();
    void WakeUp();
    s16 Brain_WakingUp_46B700();
    s16 Brain_Inactive_46B780();
    s16 Brain_Possessed_46C190();
    s16 Brain_Death_46C3A0();
    s16 Brain_DeathDropDeath_46C5A0();
    s16 Brain_ReturnControlToAbeAndDie_46C760();
    s16 Brain_PanicTurning_46C7C0();
    s16 Brain_PanicRunning_46CA20();
    s16 Brain_PanicYelling_46CC50();
    s16 Brain_Chasing_46CD60();
    s16 Brain_StopChasing_46CF20();
    s16 Brain_StartChasing_46CF90();
    s16 Brain_Idle_46D6E0();
    s16 Brain_Turning_46DC70();
    s16 Brain_Walking_46DE90();
    s16 Brain_GetAlertedTurn_46E520();
    s16 Brain_GetAlerted_46E800();
    s16 Brain_StoppingNextToMudokon_46EBB0();
    s16 Brain_BeatingUp_46EC40();
    s16 Brain_Discussion_46ECE0();
    s16 Brain_ChaseAndDisappear_46EEE0();
    s16 Brain_Shooting_46EFD0();
    s16 Brain_ZSpottedEnemy_46F260();
    s16 Brain_ZShooting_46F290();

    void BlowToGibs_4685A0();

    using TBrainFn = decltype(&Slig::Brain_ZShooting_46F290);

    void SetBrain(TBrainFn fn);
    void SetBrain2(TBrainFn fn);

    bool BrainIs(TBrainFn fn);

    s16 field_10C_unused = 0;
    s16 field_10E_brain_sub_state = 0;
    s16 field_110_pitch_min = 0;
    s32 field_114_timer = 0;
    s32 field_118_unused = 0;
    s16 field_11C_current_camera = 0; // set but never read
    s16 field_11E_return_to_previous_motion = 0;
    s16 field_120_checked_if_off_screen = 0;
    s16 field_122_unused = 0;
    s16 field_126_input = 0;
    s32 field_128_timer = 0;
    FP field_12C_falling_velx_scale_factor = {};
    s16 field_130_game_ender_pause_time = 0;
    s32 field_134_tlvInfo = 0;
    s16 field_138_res_idx = 0;
    s16 field_13A_shot_motion = 0;
    PSX_RECT field_13C_zone_rect = {};
    s16 field_144_unused = 0;
    s16 field_146_unused = 0;
    s16 field_148_unused = 0;
    s16 field_14A_unused = 0;
    s16 field_14C_unused = 0;
    EReliveLevelIds field_14E_level = EReliveLevelIds::eNone;
    s16 field_150_path = 0;
    s16 field_152_camera = 0;
    s32 field_154_death_by_being_shot_timer = 0;
    s32 field_158_explode_timer = 0;
    s32 field_15C_last_event_index = 0;
    s16 field_170_unused = 0;
    s16 field_172_unused = 0;
    Path_Slig field_174_tlv = {};
    PSX_Point field_1CC_points[10] = {};
    s16 field_1F4_points_count = 0;
    TBrainFn field_1F8_fn = nullptr;
    TBrainFn field_1FC_fn2 = nullptr;
    s16 field_200_num_times_to_shoot = 0;
    s32 field_204_unused = 0;
    s32 field_208_unused = 0;
    s16 field_20C_force_alive_state = 0;
    s16 field_20E_spotted_possessed_slig = 0;
    SligResources field_210_resources = {};
    s16 field_254_prevent_depossession = 0;
    s32 field_258_next_gamespeak_motion = 0;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x25C);

} // namespace AO

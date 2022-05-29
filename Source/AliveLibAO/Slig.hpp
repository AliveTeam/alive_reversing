#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
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
    BitField16<SligFlags_DisabledRes> field_50_disable_resources;
    s16 field_52_noise_wake_up_distance;
    s16 field_54_slig_spawner_switch_id;
    s16 field_56_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Slig, 0x58);

#define SLIG_MOTIONS_ENUM(ENTRY)              \
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
    SLIG_MOTIONS_ENUM(MAKE_ENUM)
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

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_465480();

    EXPORT void Init_46B890();

    virtual void VUpdate() override;

    EXPORT void VUpdate_465050();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_465590(PrimHeader** ppOt);

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_466350();

    virtual void VUnPosses() override;

    EXPORT void VUnPosses_465D80();

    virtual void VPossessed() override;

    EXPORT void VPossessed_465C80();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_465640(BaseGameObject* pFrom);

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOn_TLV_Collision_465CF0(Path_TLV* pTlv);

    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther) override;

    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    EXPORT s16 VOnSameYLevel_46BD00(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT s16 VIsFacingMe_4655B0(BaseAnimatedWithPhysicsGameObject* pObj);

    EXPORT void VUpdateAnimData_464D00();

    EXPORT void Vshot_465C30();

    EXPORT void MoveOnLine_467490();

    EXPORT void ToKnockBack_467300();

    EXPORT void PlayerControlRunningSlideStopOrTurnFrame4_469900();

    EXPORT void PlayerControlRunningSlideStopOrTurnFrame12_469A80();

    EXPORT void SlowDown_469D50(FP speed);

    EXPORT s16 HandlePlayerControlled_4667B0();

    void OperateLift();

    EXPORT s16 MainMovement_467020();

    EXPORT void Slig_SoundEffect_46F310(SligSfx sfxIdx);

    EXPORT u8** ResBlockForMotion_4654D0(s16 motion);

    EXPORT Bool32 VIs8_465630(s16 motion);

    EXPORT void ToShoot_46F1D0();
    EXPORT void ToZShoot_46F200();

    EXPORT void ShouldStilBeAlive_46C0D0();

    EXPORT void ToAbeDead_466270();

    EXPORT void WaitOrWalk_46E440();

    EXPORT s32 IsFacingEffectiveLeft_46BD70(Slig* pSlig);

    EXPORT void PauseALittle_46DBD0();

    EXPORT void ToTurn_46DE70();

    EXPORT void ToPanicRunning_46CC20();

    EXPORT void ToPanic_46CD40();

    EXPORT void ToChase_46D080();

    EXPORT void ToKilledAbe_4662E0();

    EXPORT s16 FindBeatTarget_46D0E0(s32 typeToFind, s32 gridBlocks);

    EXPORT s16 HandleEnemyStopper_46BF30(s32 gridBlocks);

    EXPORT void RespondToEnemyOrPatrol_465DF0();

    EXPORT void TurnOrWalk_46D5B0(s32 a2);

    void ToPanicTurn();

    void PlayerControlRunningSlideStopOrTurn(s16 last_anim_frame);

    EXPORT s16 GetNextMotionIncGameSpeak_467700(u16 input);

    EXPORT static Bool32 CCSTD RenderLayerIs_46C0A0(BaseAliveGameObject* pThis);

    EXPORT static s16 CCSTD IsAbeEnteringDoor_46BEE0(BaseAliveGameObject* pThis);

    EXPORT static s16 CCSTD IsWallBetween_46BE60(Slig* pLeft, BaseAliveGameObject* pRight);

    static EXPORT void CC Slig_GameSpeak_SFX_46F560(SligSpeak effectId, s32 defaultVol, s32 pitch_min, BaseAliveGameObject* pObj);

    static EXPORT s16 CCSTD IsInInvisibleZone_418870(BaseAnimatedWithPhysicsGameObject* pObj);

    void ToStand();

    static EXPORT s16 CCSTD IsInZCover_46BDA0(BaseAnimatedWithPhysicsGameObject* pObj);

    void CheckPlatformVanished();

    EXPORT s16 MoveLift_4665E0(FP ySpeed);

    EXPORT void GameSpeakResponse_46ED60();

    // Motions
    EXPORT void Motion_0_StandIdle_467640();
    EXPORT void Motion_1_StandToWalk_4695D0();
    EXPORT void Motion_2_Walking_469130();
    EXPORT void Motion_3_StandToRun_469C00();
    EXPORT void Motion_4_Running_469690();
    EXPORT void Motion_5_TurnAroundStanding_469C80();
    EXPORT void Motion_6_Shoot_468820();
    EXPORT void Motion_7_Falling_46A1A0();
    EXPORT void Motion_8_Unknown_4673E0();
    EXPORT void Motion_9_SlidingToStand_469DF0();
    EXPORT void Motion_10_SlidingTurn_469F10();
    EXPORT void Motion_11_SlidingTurnToWalk_46A120();
    EXPORT void Motion_12_SlidingTurnToRun_46A160();
    EXPORT void Motion_13_ReloadGun_4687B0();
    EXPORT void Motion_14_ShootToStand_468810();
    EXPORT void Motion_15_SteppingToStand_469080();
    EXPORT void Motion_16_StandingToStep_468FD0();
    EXPORT void Motion_17_DepossessingAbort_468750();
    EXPORT void Motion_18_GameSpeak_467B10();
    EXPORT void Motion_19_WalkToStand_469610();
    EXPORT void Motion_20_Recoil_468D30();
    EXPORT void Motion_21_SpeakHereBoy_467BD0();
    EXPORT void Motion_22_SpeakHi_467C90();
    EXPORT void Motion_23_SpeakFreeze_467D50();
    EXPORT void Motion_24_SpeakGetHim_467E10();
    EXPORT void Motion_25_SpeakLaugh_467ED0();
    EXPORT void Motion_26_SpeakBullshit1_467F90();
    EXPORT void Motion_27_SpeakLookOut_468050();
    EXPORT void Motion_28_SpeakBullshit2_468110();
    EXPORT void Motion_29_SpeakPanic_4681D0();
    EXPORT void Motion_30_SpeakWhat_468290();
    EXPORT void Motion_31_SpeakAIFreeze_468350();
    EXPORT void Motion_32_Blurgh_468410();
    EXPORT void Motion_33_Sleeping_46A410();
    EXPORT void Motion_34_SleepingToStand_46A5F0();
    EXPORT void Motion_35_Knockback_46A720();
    EXPORT void Motion_36_KnockbackToStand_46A7F0();
    EXPORT void Motion_37_Depossessing_4684D0();
    EXPORT void Motion_38_Possess_46B050();
    EXPORT void Motion_39_OutToFall_46A9E0();
    EXPORT void Motion_40_FallingInitiate_46AA60();
    EXPORT void Motion_41_LandingSoft_46A390();
    EXPORT void Motion_42_LandingFatal_46AFE0();
    EXPORT void Motion_43_ShootZ_468E30();
    EXPORT void Motion_44_ShootZtoStand_468F70();
    EXPORT void Motion_45_Smash_46A990();
    EXPORT void Motion_46_ToIdle_46A590();
    EXPORT void Motion_47_LiftUp_4665A0();
    EXPORT void Motion_48_LiftDown_4665C0();
    EXPORT void Motion_49_LiftGrip_4663A0();
    EXPORT void Motion_50_LiftUngrip_466460();
    EXPORT void Motion_51_LiftGripping_466480();
    EXPORT void Motion_52_Beat_46AA90();

    // Brains
    EXPORT s16 Brain_SpottedEnemy_465EB0();
    EXPORT s16 Brain_Paused_466030();
    EXPORT s16 Brain_EnemyDead_466190();
    EXPORT s16 Brain_KilledEnemy_4662A0();
    EXPORT s16 Brain_Unknown_46B250();
    EXPORT s16 Brain_Sleeping_46B4E0();
    void WakeUp();
    EXPORT s16 Brain_WakingUp_46B700();
    EXPORT s16 Brain_Inactive_46B780();
    EXPORT s16 Brain_Possessed_46C190();
    EXPORT s16 Brain_Death_46C3A0();
    EXPORT s16 Brain_DeathDropDeath_46C5A0();
    EXPORT s16 Brain_ReturnControlToAbeAndDie_46C760();
    EXPORT s16 Brain_PanicTurning_46C7C0();
    EXPORT s16 Brain_PanicRunning_46CA20();
    EXPORT s16 Brain_PanicYelling_46CC50();
    EXPORT s16 Brain_Chasing_46CD60();
    EXPORT s16 Brain_StopChasing_46CF20();
    EXPORT s16 Brain_StartChasing_46CF90();
    EXPORT s16 Brain_Idle_46D6E0();
    EXPORT s16 Brain_Turning_46DC70();
    EXPORT s16 Brain_Walking_46DE90();
    EXPORT s16 Brain_GetAlertedTurn_46E520();
    EXPORT s16 Brain_GetAlerted_46E800();
    EXPORT s16 Brain_StoppingNextToMudokon_46EBB0();
    EXPORT s16 Brain_BeatingUp_46EC40();
    EXPORT s16 Brain_Discussion_46ECE0();
    EXPORT s16 Brain_ChaseAndDisappear_46EEE0();
    EXPORT s16 Brain_Shooting_46EFD0();
    EXPORT s16 Brain_ZSpottedEnemy_46F260();
    EXPORT s16 Brain_ZShooting_46F290();

    EXPORT void BlowToGibs_4685A0();

    using TBrainFn = decltype(&Slig::Brain_ZShooting_46F290);

    void SetBrain(TBrainFn fn);
    void SetBrain2(TBrainFn fn);

    bool BrainIs(TBrainFn fn);

    s16 field_10C_unused;
    s16 field_10E_brain_sub_state;
    s16 field_110_pitch_min;
    s16 field_112_padding;
    s32 field_114_timer;
    s32 field_118_unused;
    s16 field_11C_current_camera; // set but never read
    s16 field_11E_return_to_previous_motion;
    s16 field_120_checked_if_off_screen;
    s16 field_122_unused;
    s16 field_124_padding;
    s16 field_126_input;
    s32 field_128_timer;
    FP field_12C_falling_velx_scale_factor;
    s16 field_130_game_ender_pause_time;
    s16 field_132_padding;
    s32 field_134_tlvInfo;
    s16 field_138_res_idx;
    s16 field_13A_shot_motion;
    PSX_RECT field_13C_zone_rect;
    s16 field_144_unused;
    s16 field_146_unused;
    s16 field_148_unused;
    s16 field_14A_unused;
    s16 field_14C_unused;
    LevelIds field_14E_level;
    s16 field_150_path;
    s16 field_152_camera;
    s32 field_154_death_by_being_shot_timer;
    s32 field_158_explode_timer;
    s32 field_15C_last_event_index;
    s32 field_160_padding;
    s32 field_164_padding;
    s32 field_168_padding;
    s32 field_16C_padding;
    s16 field_170_unused;
    s16 field_172_unused;
    Path_Slig field_174_tlv;
    PSX_Point field_1CC_points[10];
    s16 field_1F4_points_count;
    s16 field_1F6_padding;
    TBrainFn field_1F8_fn;
    TBrainFn field_1FC_fn2;
    s16 field_200_num_times_to_shoot;
    s16 field_202; // pad ?
    s32 field_204_unused;
    s32 field_208_unused;
    s16 field_20C_force_alive_state;
    s16 field_20E_spotted_possessed_slig;
    SligResources field_210_resources;
    s16 field_254_prevent_depossession;
    s16 field_256_padding;
    s32 field_258_next_gamespeak_motion;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x25C);

} // namespace AO

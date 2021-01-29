#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

enum SligFlags_DisabledRes
{
    eDisabledRes_Bit1 = 0x1,
    eDisabledRes_Bit2 = 0x2,
    eDisabledRes_Bit3 = 0x4,
    eDisabledRes_Bit4 = 0x8,
    eDisabledRes_Bit5 = 0x10,
    eDisabledRes_Bit6 = 0x20,
    eDisabledRes_Bit7 = 0x40,
    eDisabledRes_Bit8 = 0x80,
    eDisabledRes_Bit9 = 0x100,
    eDisabledRes_Bit10 = 0x200,
    eDisabledRes_Bit11 = 0x400
};

struct Path_Slig : public Path_TLV
{
    enum class StartState : __int16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        GameEnder_4 = 4,
        Paused_5 = 5,
    };
    __int16 field_18_scale;
    StartState field_1A_start_state;
    __int16 field_1C_pause_time;
    __int16 field_1E_pause_left_min;
    __int16 field_20_pause_left_max;
    __int16 field_22_pause_right_min;
    __int16 field_24_pause_right_max;
    __int16 field_26_chal_type;
    __int16 field_28_chal_time;
    __int16 field_2A_number_of_times_to_shoot;
    __int16 field_2C_unknown; // TODO: Part of above field, check me?
    __int16 field_2E_code1;
    __int16 field_30_code2;
    __int16 field_32_chase_abe;
    __int16 field_34_start_direction;
    __int16 field_36_panic_timeout;
    __int16 field_38_num_panic_sounds;
    __int16 field_3A_panic_sound_timeout;
    __int16 field_3C_stop_chase_delay;
    __int16 field_3E_time_to_wait_before_chase;
    __int16 field_40_slig_id;
    __int16 field_42_listen_time;
    __int16 field_44_percent_say_what;
    __int16 field_46_percent_beat_mud;
    __int16 field_48_talk_to_abe;
    __int16 field_4A_dont_shoot;
    __int16 field_4C_z_shoot_delay;
    __int16 field_4E_stay_awake;
    BitField16<SligFlags_DisabledRes> field_50_disable_resources;
    __int16 field_52_noise_wake_up_distance;
    __int16 field_54_id;
    __int16 field_56_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Slig, 0x58);

#define SLIG_STATES_ENUM(ENTRY) \
    ENTRY(State_0_StandIdle_467640) \
    ENTRY(State_1_StandToWalk_4695D0) \
    ENTRY(State_2_Walking_469130) \
    ENTRY(State_3_StandToRun_469C00) \
    ENTRY(State_4_Running_469690) \
    ENTRY(State_5_TurnAroundStanding_469C80) \
    ENTRY(State_6_Shoot_468820) \
    ENTRY(State_7_Falling_46A1A0) \
    ENTRY(State_8_Unknown_4673E0) \
    ENTRY(State_9_SlidingToStand_469DF0) \
    ENTRY(State_10_SlidingTurn_469F10) \
    ENTRY(State_11_SlidingTurnToWalk_46A120) \
    ENTRY(State_12_SlidingTurnToRun_46A160) \
    ENTRY(State_13_Reload_4687B0) \
    ENTRY(State_14_ShootToStand_468810) \
    ENTRY(State_15_SteppingToStand_469080) \
    ENTRY(State_16_StandingToStep_468FD0) \
    ENTRY(State_17_DepossessingAbort_468750) \
    ENTRY(State_18_GameSpeak_467B10) \
    ENTRY(State_19_WalkToStand_469610) \
    ENTRY(State_20_Recoil_468D30) \
    ENTRY(State_21_SpeakHereBoy_467BD0) \
    ENTRY(State_22_SpeakHi_467C90) \
    ENTRY(State_23_SpeakFreeze_467D50) \
    ENTRY(State_24_SpeakGetHim_467E10) \
    ENTRY(State_25_SpeakLaugh_467ED0) \
    ENTRY(State_26_SpeakBullshit1_467F90) \
    ENTRY(State_27_SpeakLookOut_468050) \
    ENTRY(State_28_SpeakBullshit2_468110) \
    ENTRY(State_29_SpeakPanic_4681D0) \
    ENTRY(State_30_SpeakWhat_468290) \
    ENTRY(State_31_SpeakAIFreeze_468350) \
    ENTRY(State_32_Blurgh_468410) \
    ENTRY(State_33_Sleeping_46A410) \
    ENTRY(State_34_SleepingToStand_46A5F0) \
    ENTRY(State_35_Knockback_46A720) \
    ENTRY(State_36_KnockbackToStand_46A7F0) \
    ENTRY(State_37_Depossessing_4684D0) \
    ENTRY(State_38_Possess_46B050) \
    ENTRY(State_39_OutToFall_46A9E0) \
    ENTRY(State_40_FallingInitiate_46AA60) \
    ENTRY(State_41_LandingSoft_46A390) \
    ENTRY(State_42_LandingFatal_46AFE0) \
    ENTRY(State_43_ShootZ_468E30) \
    ENTRY(State_44_ShootZtoStand_468F70) \
    ENTRY(State_45_Smash_46A990) \
    ENTRY(State_46_PullLever_46A590) \
    ENTRY(State_47_LiftUp_4665A0) \
    ENTRY(State_48_LiftDown_4665C0) \
    ENTRY(State_49_LiftGrip_4663A0) \
    ENTRY(State_50_LiftUngrip_466460) \
    ENTRY(State_51_LiftGripping_466480) \
    ENTRY(State_52_Beat_46AA90)

#define MAKE_ENUM(VAR) VAR,
enum eSligStates : int
{
    SLIG_STATES_ENUM(MAKE_ENUM)
};

struct SligResources
{
    BYTE** res[17];
};

enum class SligSpeak : unsigned __int8
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

enum class SligSfx : __int8
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
struct Path_Slig_Bound : public Path_TLV
{
    __int16 field_18_slig_id;
    BitField16<SligFlags_DisabledRes> field_1A_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig_Bound, 0x1C);

using Path_Slig_LeftBound = Path_Slig_Bound;
using Path_Slig_RightBound = Path_Slig_Bound;
using Path_Slig_Persist = Path_Slig_Bound;

class Slig : public BaseAliveGameObject
{
public:
    EXPORT Slig* ctor_464D40(Path_Slig* pTlv, int tlvInfo);
    
    EXPORT BaseGameObject* dtor_465320();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Slig* Vdtor_465DC0(signed int flags);

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

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_465640(BaseGameObject* pFrom);

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOn_TLV_Collision_465CF0(Path_TLV* pTlv);

    virtual __int16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther) override;

    virtual __int16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    EXPORT __int16 VOnSameYLevel_46BD00(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT __int16 VIsFacingMe_4655B0(BaseAnimatedWithPhysicsGameObject* pObj);

    EXPORT void VUpdateAnimData_464D00();

    EXPORT void Vshot_465C30();

    EXPORT void MoveOnLine_467490();

    EXPORT void ToKnockBack_467300();

    EXPORT void PlayerControlRunningSlideStopOrTurnFrame4_469900();

    EXPORT void PlayerControlRunningSlideStopOrTurnFrame12_469A80();

    EXPORT void SlowDown_469D50(FP speed);

    EXPORT signed __int16 HandlePlayerControlled_4667B0();

    void OperateLift();

    EXPORT signed __int16 MainMovement_467020();

    EXPORT void Slig_SoundEffect_46F310(SligSfx sfxIdx);

    EXPORT BYTE** ResBlockForMotion_4654D0(__int16 motion);

    EXPORT BOOL VIs8_465630(short motion);

    EXPORT void ToShoot_46F1D0();
    EXPORT void ToZShoot_46F200();

    EXPORT void ShouldStilBeAlive_46C0D0();

    EXPORT void ToAbeDead_466270();

    EXPORT void WaitOrWalk_46E440();

    EXPORT int IsFacingEffectiveLeft_46BD70(Slig* pSlig);

    EXPORT void PauseALittle_46DBD0();

    EXPORT void ToTurn_46DE70();

    EXPORT void ToPanicRunning_46CC20();

    EXPORT void ToPanic_46CD40();

    EXPORT void ToChase_46D080();

    EXPORT void ToKilledAbe_4662E0();

    EXPORT __int16 FindBeatTarget_46D0E0(int typeToFind, int gridBlocks);

    EXPORT __int16 HandleEnemyStopper_46BF30(int gridBlocks);

    EXPORT void ShootTurnTowardsOrKillSound_465DF0();

    EXPORT void TurnOrWalk_46D5B0(int a2);

    void ToPanicTurn();

    void PlayerControlRunningSlideStopOrTurn(short last_anim_frame);

    EXPORT __int16 GetNextMotionIncGameSpeak_467700(unsigned __int16 input);

    EXPORT static BOOL CCSTD RenderLayerIs_46C0A0(BaseAliveGameObject* pThis);

    EXPORT static __int16 CCSTD IsAbeEnteringDoor_46BEE0(BaseAliveGameObject* pThis);

    EXPORT static __int16 CCSTD IsWallBetween_46BE60(Slig* pLeft, BaseAliveGameObject* pRight);

    static EXPORT void CC Slig_GameSpeak_SFX_46F560(SligSpeak effectId, int defaultVol, int pitch_min, BaseAliveGameObject* pObj);

    static EXPORT __int16 CCSTD IsInInvisibleZone_418870(BaseAnimatedWithPhysicsGameObject* pObj);

    void ToStand();

    static EXPORT __int16 CCSTD IsInZCover_46BDA0(BaseAnimatedWithPhysicsGameObject* pObj);

    void CheckPlatformVanished();

    EXPORT __int16 MoveLift_4665E0(FP ySpeed);

    EXPORT void GameSpeakResponse_46ED60();

    // States
    EXPORT void State_0_StandIdle_467640();
    EXPORT void State_1_StandToWalk_4695D0();
    EXPORT void State_2_Walking_469130();
    EXPORT void State_3_StandToRun_469C00();
    EXPORT void State_4_Running_469690();
    EXPORT void State_5_TurnAroundStanding_469C80();
    EXPORT void State_6_Shoot_468820();
    EXPORT void State_7_Falling_46A1A0();
    EXPORT void State_8_Unknown_4673E0();
    EXPORT void State_9_SlidingToStand_469DF0();
    EXPORT void State_10_SlidingTurn_469F10();
    EXPORT void State_11_SlidingTurnToWalk_46A120();
    EXPORT void State_12_SlidingTurnToRun_46A160();
    EXPORT void State_13_Reload_4687B0();
    EXPORT void State_14_ShootToStand_468810();
    EXPORT void State_15_SteppingToStand_469080();
    EXPORT void State_16_StandingToStep_468FD0();
    EXPORT void State_17_DepossessingAbort_468750();
    EXPORT void State_18_GameSpeak_467B10();
    EXPORT void State_19_WalkToStand_469610();
    EXPORT void State_20_Recoil_468D30();
    EXPORT void State_21_SpeakHereBoy_467BD0();
    EXPORT void State_22_SpeakHi_467C90();
    EXPORT void State_23_SpeakFreeze_467D50();
    EXPORT void State_24_SpeakGetHim_467E10();
    EXPORT void State_25_SpeakLaugh_467ED0();
    EXPORT void State_26_SpeakBullshit1_467F90();
    EXPORT void State_27_SpeakLookOut_468050();
    EXPORT void State_28_SpeakBullshit2_468110();
    EXPORT void State_29_SpeakPanic_4681D0();
    EXPORT void State_30_SpeakWhat_468290();
    EXPORT void State_31_SpeakAIFreeze_468350();
    EXPORT void State_32_Blurgh_468410();
    EXPORT void State_33_Sleeping_46A410();
    EXPORT void State_34_SleepingToStand_46A5F0();
    EXPORT void State_35_Knockback_46A720();
    EXPORT void State_36_KnockbackToStand_46A7F0();
    EXPORT void State_37_Depossessing_4684D0();
    EXPORT void State_38_Possess_46B050();
    EXPORT void State_39_OutToFall_46A9E0();
    EXPORT void State_40_FallingInitiate_46AA60();
    EXPORT void State_41_LandingSoft_46A390();
    EXPORT void State_42_LandingFatal_46AFE0();
    EXPORT void State_43_ShootZ_468E30();
    EXPORT void State_44_ShootZtoStand_468F70();
    EXPORT void State_45_Smash_46A990();
    EXPORT void State_46_ToIdle_46A590();
    EXPORT void State_47_LiftUp_4665A0();
    EXPORT void State_48_LiftDown_4665C0();
    EXPORT void State_49_LiftGrip_4663A0();
    EXPORT void State_50_LiftUngrip_466460();
    EXPORT void State_51_LiftGripping_466480();
    EXPORT void State_52_Beat_46AA90();

    // Brains
    EXPORT __int16 Brain_SpottedEnemy_465EB0();
    EXPORT __int16 Brain_Paused_466030();
    EXPORT __int16 Brain_EnemyDead_466190();
    EXPORT __int16 Brain_KilledEnemy_4662A0();
    EXPORT __int16 Brain_Unknown_46B250();
    EXPORT __int16 Brain_Sleeping_46B4E0();
    void WakeUp();
    EXPORT __int16 Brain_WakingUp_46B700();
    EXPORT __int16 Brain_Inactive_46B780();
    EXPORT __int16 Brain_Possessed_46C190();
    EXPORT __int16 Brain_Death_46C3A0();
    EXPORT __int16 Brain_DeathDropDeath_46C5A0();
    EXPORT __int16 Brain_ReturnControlToAbeAndDie_46C760();
    EXPORT __int16 Brain_PanicTurning_46C7C0();
    EXPORT __int16 Brain_PanicRunning_46CA20();
    EXPORT __int16 Brain_PanicYelling_46CC50();
    EXPORT __int16 Brain_Chasing_46CD60();
    EXPORT __int16 Brain_StopChasing_46CF20();
    EXPORT __int16 Brain_StartChasing_46CF90();
    EXPORT __int16 Brain_Idle_46D6E0();
    EXPORT __int16 Brain_Turning_46DC70();
    EXPORT __int16 Brain_Walking_46DE90();
    EXPORT __int16 Brain_GetAlertedTurn_46E520();
    EXPORT __int16 Brain_GetAlerted_46E800();
    EXPORT __int16 Brain_StoppingNextToMudokon_46EBB0();
    EXPORT __int16 Brain_BeatingUp_46EC40();
    EXPORT __int16 Brain_Discussion_46ECE0();
    EXPORT __int16 Brain_GameEnder_46EEE0();
    EXPORT __int16 Brain_Shooting_46EFD0();
    EXPORT __int16 Brain_ZSpottedEnemy_46F260();
    EXPORT __int16 Brain_ZShooting_46F290();

    EXPORT void BlowToGibs_4685A0();

    using TBrainFn = decltype(&Slig::Brain_ZShooting_46F290);

    void SetBrain(TBrainFn fn);
    void SetBrain2(TBrainFn fn);

    bool BrainIs(TBrainFn fn);

    __int16 field_10C;
    __int16 field_10E_brain_state;
    __int16 field_110_pitch_min;
    __int16 field_112;
    int field_114_timer;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120_checked_if_off_screen;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126_input;
    int field_128_timer;
    FP field_12C_falling_velx_scale_factor;
    __int16 field_130;
    __int16 field_132;
    int field_134_tlvInfo;
    __int16 field_138_res_idx;
    __int16 field_13A;
    PSX_RECT field_13C_zone_rect;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    LevelIds field_14E_level;
    __int16 field_150_path;
    __int16 field_152_camera;
    int field_154_death_by_being_shot_timer;
    int field_158_explode_timer;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    __int16 field_170;
    __int16 field_172;
    Path_Slig field_174_tlv;
    PSX_Point field_1CC_points[10];
    __int16 field_1F4_points_count;
    __int16 field_1F6;
    TBrainFn field_1F8_fn;
    TBrainFn field_1FC_fn2;
    __int16 field_200_num_times_to_shoot;
    __int16 field_202; // pad ?
    int field_204;
    int field_208;
    __int16 field_20C_force_alive_state;
    __int16 field_20E_spotted_possessed_slig;
    SligResources field_210_resources;
    __int16 field_254_prevent_depossession;
    __int16 field_256;
    int field_258_next_gamespeak_motion;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x25C);

}

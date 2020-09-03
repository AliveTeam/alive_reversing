#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

struct Path_Slig : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_start_state;
    __int16 field_1C_pause_time;
    __int16 field_1E_pause_left_min;
    __int16 field_20_pause_left_max;
    __int16 field_22_pause_right_min;
    __int16 field_24_pause_right_max;
    __int16 field_26_chal_type;
    __int16 field_28_chal_time;
    __int16 field_2A_number_of_times_to_shoot;
    __int16 field_2C_unknown;
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
    __int16 field_50_disable_resources;
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
    ENTRY(State_24_SpeakGitIm_467E10) \
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
    ENTRY(State_51_LiftGrip_466480) \
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

    EXPORT void VUpdate_465050();

    EXPORT void VUpdateAnimData_464D00();

    EXPORT void Vsub_465C30();

    EXPORT void CheckFloorGone_467490();

    EXPORT void ToKnockBack_467300();

    EXPORT __int16 MainMovement_467020();

    EXPORT BYTE** ResBlockForMotion_4654D0(__int16 motion);

    EXPORT BOOL VIs8_465630(short motion);

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
    EXPORT void State_24_SpeakGitIm_467E10();
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
    EXPORT void State_46_PullLever_46A590();
    EXPORT void State_47_LiftUp_4665A0();
    EXPORT void State_48_LiftDown_4665C0();
    EXPORT void State_49_LiftGrip_4663A0();
    EXPORT void State_50_LiftUngrip_466460();
    EXPORT void State_51_LiftGrip_466480();
    EXPORT void State_52_Beat_46AA90();

    // Brains
    EXPORT __int16 Brain_465EB0();
    EXPORT __int16 Brain_466030();
    EXPORT __int16 Brain_466190();
    EXPORT __int16 Brain_4662A0();
    EXPORT __int16 Brain_46B250();
    EXPORT __int16 Brain_46B4E0();
    EXPORT __int16 Brain_46B700();
    EXPORT __int16 Brain_46B780();
    EXPORT __int16 Brain_46C190();
    EXPORT __int16 Brain_46C3A0();
    EXPORT __int16 Brain_46C5A0();
    EXPORT __int16 Brain_46C760();
    EXPORT __int16 Brain_46C7C0();
    EXPORT __int16 Brain_46CA20();
    EXPORT __int16 Brain_46CC50();
    EXPORT __int16 Brain_46CD60();
    EXPORT __int16 Brain_46CF20();
    EXPORT __int16 Brain_46CF90();
    EXPORT __int16 Brain_46D6E0();
    EXPORT __int16 Brain_46DC70();
    EXPORT __int16 Brain_46DE90();
    EXPORT __int16 Brain_46E520();
    EXPORT __int16 Brain_46E800();
    EXPORT __int16 Brain_46EBB0();
    EXPORT __int16 Brain_46EC40();
    EXPORT __int16 Brain_46ECE0();
    EXPORT __int16 Brain_46EEE0();
    EXPORT __int16 Brain_46EFD0();
    EXPORT __int16 Brain_46F260();
    EXPORT __int16 Brain_46F290();

    EXPORT void BlowToGibs_4685A0();

    using TBrainFn = decltype(&Slig::Brain_46F290);

    void SetBrain(TBrainFn fn);

    bool BrainIs(TBrainFn fn);

    __int16 field_10C;
    __int16 field_10E_brain_state;
    __int16 field_110;
    __int16 field_112;
    int field_114_timer;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    int field_128_timer;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134_tlvInfo;
    __int16 field_138_res_idx;
    __int16 field_13A;
    int field_13C;
    int field_140;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    LevelIds field_14E_level;
    __int16 field_150_path;
    __int16 field_152_camera;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    __int16 field_170;
    __int16 field_172;
    Path_Slig field_174_tlv;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    int field_1E8;
    int field_1EC;
    int field_1F0;
    int field_1F4;
    TBrainFn field_1F8_fn;
    int field_1FC;
    int field_200;
    int field_204;
    int field_208;
    __int16 field_20C;
    __int16 field_20E;
    SligResources field_210_resources;
    __int16 field_254;
    __int16 field_256;
    int field_258;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x25C);

END_NS_AO

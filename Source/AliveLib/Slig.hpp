#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

#define SLIG_MOTIONS_ENUM(ENTRY) \
    ENTRY(M_StandIdle_0_4B4EC0) \
    ENTRY(M_StandToWalk_1_4B5F70) \
    ENTRY(M_Walking_2_4B5BC0) \
    ENTRY(M_StandToRun_3_4B62F0) \
    ENTRY(M_Running_4_4B6000) \
    ENTRY(M_TurnAroundStanding_5_4B6390) \
    ENTRY(M_Shoot_6_4B55A0) \
    ENTRY(M_Falling_7_4B42D0) \
    ENTRY(M_SlidingToStand_8_4B6520) \
    ENTRY(M_SlidingTurn_9_4B6680) \
    ENTRY(M_SlidingTurnToWalk_10_4B6800) \
    ENTRY(M_SlidingTurnToRun_11_4B6850) \
    ENTRY(M_ReloadGun_12_4B5530) \
    ENTRY(M_ShootToStand_13_4B5580) \
    ENTRY(M_SteppingToStand_14_4B8480) \
    ENTRY(M_StandingToStep_15_4B83B0) \
    ENTRY(M_DepossessingAbort_16_4B8250) \
    ENTRY(M_GameSpeak_17_4B5290) \
    ENTRY(M_WalkToStand_18_4B5FC0) \
    ENTRY(M_Recoil_19_4B8270) \
    ENTRY(M_SpeakHereBoy_20_4B5330) \
    ENTRY(M_SpeakHi_21_4B53D0) \
    ENTRY(M_SpeakFreeze_22_4B53F0) \
    ENTRY(M_SpeakGitIm_23_4B5410) \
    ENTRY(M_SpeakLaugh_24_4B5430) \
    ENTRY(M_SpeakBullShit1_25_4B5450) \
    ENTRY(M_SpeakLookOut_26_4B5470) \
    ENTRY(M_SpeakBullShit2_27_4B5490) \
    ENTRY(M_SpeakPanic_28_4B54B0) \
    ENTRY(M_SpeakWhat_29_4B54D0) \
    ENTRY(M_SpeakAIFreeze_30_4B54F0) \
    ENTRY(M_Blurgh_31_4B5510) \
    ENTRY(M_Sleeping_32_4B89A0) \
    ENTRY(M_SleepingToStand_33_4B8C50) \
    ENTRY(M_Knockback_34_4B68A0) \
    ENTRY(M_KnockbackToStand_35_4B6A30) \
    ENTRY(M_Depossessing_36_4B7F30) \
    ENTRY(M_Possess_37_4B72C0) \
    ENTRY(M_OutToFall_38_4B4570) \
    ENTRY(M_FallingInitiate_39_4B4640) \
    ENTRY(M_LandingSoft_40_4B4530) \
    ENTRY(M_LandingFatal_41_4B4680) \
    ENTRY(M_ShootZ_42_4B7560) \
    ENTRY(M_ShootZtoStand_43_4B77E0) \
    ENTRY(M_Smash_44_4B6B90) \
    ENTRY(M_PullLever_45_4B8950) \
    ENTRY(M_LiftGrip_46_4B3700) \
    ENTRY(M_LiftUngrip_47_4B3820) \
    ENTRY(M_LiftGripping_48_4B3850) \
    ENTRY(M_LiftUp_49_4B3930) \
    ENTRY(M_LiftDown_50_4B3960) \
    ENTRY(M_Beat_51_4B6C00)

#define MAKE_ENUM(VAR) VAR,
enum eSligMotions : int
{
    SLIG_MOTIONS_ENUM(MAKE_ENUM)
};

struct Path_Slig : public Path_TLV
{
    __int16 field_10_scale;

    // TODO: Check these - GameEnder is probably only AO for instance ?
    enum class StartState : __int16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        RunOffScreen_4 = 4,
        GameEnder_5 = 5,
        Unknown_6 = 6,
    };
    StartState field_12_start_state;

    __int16 field_14_pause_time;
    __int16 field_16_pause_left_min;
    __int16 field_18_pause_left_max;
    __int16 field_1A_pause_right_min;
    __int16 field_1C_pause_right_max;
    __int16 field_1E_chal_number;
    __int16 field_20_chal_timer;
    __int16 field_22_num_times_to_shoot;
    __int16 field_24_unknown;
    __int16 field_26_code1;
    __int16 field_28_code2;
    __int16 field_2A_chase_abe;
    __int16 field_2C_start_direction;
    __int16 field_2E_panic_timeout;
    __int16 field_30_num_panic_sounds;
    __int16 field_32_panic_sound_timeout;
    __int16 field_34_stop_chase_delay;
    __int16 field_36_time_to_wait_before_chase;
    __int16 field_38_slig_id;
    __int16 field_3A_listen_time;
    __int16 field_3C_percent_say_what;
    __int16 field_3E_percent_beat_mud;
    __int16 field_40_talk_to_abe;
    __int16 field_42_dont_shoot;
    __int16 field_44_Z_shoot_delay;
    __int16 field_46_stay_awake;
    __int16 field_48_disable_resources;
    __int16 field_4A_noise_wake_up_distance;
    __int16 field_4C_id;
    __int16 field_4E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x50);


enum class LevelIds : __int16;

class Slig;
using TSligAIFn = __int16 (Slig::*)();
using TSligMotionFn = void (Slig::*)();


class Slig : public BaseAliveGameObject
{
public:
    EXPORT Slig* ctor_4B1370(Path_Slig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    virtual void VPossessed_408F70() override;

    virtual void VUnPosses_408F90() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual void VOnTrapDoorOpen() override;

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
    EXPORT __int16 AI_Death_0_4BBFB0();
    EXPORT __int16 AI_ReturnControlToAbeAndDie_1_4BC410();
    EXPORT __int16 AI_Possessed_2_4BBCF0();
    EXPORT __int16 AI_DeathDropDeath_3_4BC1E0();
    EXPORT __int16 AI_ListeningToGlukkon_4_4B9D20();
    EXPORT __int16 AI_Empty_5_4B3220();
    EXPORT __int16 AI_Empty_6_4B3420();
    EXPORT __int16 AI_SpottedEnemy_7_4B3240();
    EXPORT __int16 AI_Empty_8_4B3120();
    EXPORT __int16 AI_Empty_9_4B3440();
    EXPORT __int16 AI_AbeDead_10_4B3460();
    EXPORT __int16 AI_KilledAbe_11_4B35C0();
    EXPORT __int16 AI_PanicTurning_12_4BC490();
    EXPORT __int16 AI_PanicRunning_13_4BC780();
    EXPORT __int16 AI_PanicYelling_14_4BCA70();
    EXPORT __int16 AI_Idle_15_4BD800();
    EXPORT __int16 AI_StopChasing_16_4BCE30();
    EXPORT __int16 AI_Chasing2_17_4BCBD0();
    EXPORT __int16 AI_Chasing1_18_4BCEB0();
    EXPORT __int16 AI_Turning_19_4BDDD0();
    EXPORT __int16 AI_StoppingNextToMudokon_20_4BF1E0();
    EXPORT __int16 AI_Walking_21_4BE0C0();
    EXPORT __int16 AI_GetAlertedTurn_22_4BE990();
    EXPORT __int16 AI_GetAlerted_23_4BEC40();
    EXPORT __int16 AI_BeatingUp_24_4BF2B0();
    EXPORT __int16 AI_DiscussionWhat_25_4BF380();
    EXPORT __int16 AI_Empty_26_4BF620();
    EXPORT __int16 AI_Empty_27_4BF600();
    EXPORT __int16 AI_ShootingFromBackground_28_4BFA70();
    EXPORT __int16 AI_Shooting_29_4BF750();
    EXPORT __int16 AI_SpottedEnemyFromBackground_30_4BFA30();
    EXPORT __int16 AI_WakingUp_31_4B9390();
    EXPORT __int16 AI_Inactive_32_4B9430();
    EXPORT __int16 AI_Paused_33_4B8DD0();
    EXPORT __int16 AI_Sleeping_34_4B9170();
    EXPORT __int16 AI_GameEnder_35_4BF640();

private:
    EXPORT void Init_4BB0D0();

    EXPORT void dtor_4B1CF0();


    EXPORT Slig* vdtor_4B1790(signed int flags);

    EXPORT void vUpdate_4B17C0();

    EXPORT void vScreenChanged_4B1E20();

    EXPORT void vPossessed_4B2F10();

    EXPORT void vUnPosses_4B3050();

    EXPORT void vOnTlvCollision_4B2FB0(Path_TLV* pTlv);

    EXPORT void vsub_4B2EA0();

    EXPORT void vUpdateAnim_4B1320();

    EXPORT BOOL vUnderGlukkonCommand_4B1760();

    EXPORT void WakeUp_4B93B0();

    EXPORT void ShouldStilBeAlive_4BBC00();

    EXPORT BYTE** ResForMotion_4B1E90(__int16 motion);

    EXPORT void ToTurn_4BE090();

    EXPORT void RespondWithWhat_4BF400();

    EXPORT void ToShoot_4BF9A0();

    EXPORT void ToZShoot_4BF9E0();

    EXPORT void PauseALittle_4BDD00();

    EXPORT void ToStand_4B4A20();

    EXPORT void BlowUp_4B8020();

    EXPORT __int16 MainMovement_4B4720();

    EXPORT __int16 HandlePlayerControlled_4B7800();

    EXPORT __int16 GetNextMotionIncGameSpeak_4B5080(int input);

    EXPORT void WaitOrWalk_4BE870();

    EXPORT void ToAbeDead_4B3580();

    EXPORT void ToUnderGlukkonCommand_4B9660();

    EXPORT void ToKilledAbe_4B3600();

    EXPORT BOOL IsWallBetween_4BB8B0(BaseAliveGameObject* pLeft, BaseAliveGameObject* pRight);

    EXPORT __int16 ListenToGlukkonCommands_4B95D0();

    EXPORT void PlatformCollide_4B4E00();

    EXPORT void FallKnockBackOrSmash_4B4A90();

    EXPORT void TurnOrSayWhat_4BEBC0();

    EXPORT void GameSpeakResponse_4BF470();

    EXPORT void GoAlertedOrSayWhat_4BF140();

    EXPORT static int CCSTD IsFacingEffectiveLeft_4BB780(Slig* pSlig);

    EXPORT void MoveOnLine_4B4C40();

    EXPORT void PlayerControlStopWalkingIfRequired_4B8540();

    EXPORT void CheckPlatformVanished_4B3640();

    EXPORT void vOnTrapDoorOpen_4B3690();

    EXPORT __int16 MoveLift_4B3990(FP ySpeed);

    EXPORT void SlowDown_4B6450(FP speed);

    EXPORT void ToChase_4BCFF0();

    EXPORT __int16 HandleEnemyStopper_4BBA00(int gridBlocks);

    EXPORT void ToPanicYelling_4BCBA0();

    EXPORT void ToPanicTurn_4BC750();

private:
    void SetBrain(TSligAIFn fn);
    bool BrainIs(TSligAIFn fn);
private:
    __int16 field_116;
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E;
    int field_120_timer;
    __int16 field_124;
    __int16 field_126;
    int field_128_input;
    int field_12C;
    FP field_130;
    __int16 field_134_res_idx;
    __int16 field_136;
    PSX_RECT field_138_zone_rect;
    __int16 field_140;
    __int16 field_142;
    __int16 field_144;
    LevelIds field_146_level;
    __int16 field_148_path;
    __int16 field_14A_camera;
    int field_14C;
    int field_150;
    TSligAIFn field_154_brain_state;
    int field_158;
    __int16 field_15C;
    __int16 field_15E;
    int field_160;
    __int16 field_164;
    __int16 field_166;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A;
    __int16 field_17C;
    __int16 field_17E;
    __int16 field_180;
    __int16 field_182;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    __int16 field_18C;
    __int16 field_18E;
    __int16 field_190;
    __int16 field_192;
    __int16 field_194;
    __int16 field_196;
    __int16 field_198;
    __int16 field_19A;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    __int16 field_1A4;
    __int16 field_1A6;
    __int16 field_1A8;
    __int16 field_1AA;
    __int16 field_1AC;
    __int16 field_1AE;
    __int16 field_1B0;
    __int16 field_1B2;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8;
    __int16 field_1BA;
    __int16 field_1BC;
    __int16 field_1BE;
    __int16 field_1C0;
    __int16 field_1C2;
    __int16 field_1C4;
    __int16 field_1C6;
    __int16 field_1C8;
    __int16 field_1CA;
    __int16 field_1CC;
    __int16 field_1CE;
    __int16 field_1D0;
    __int16 field_1D2;
    __int16 field_1D4;
    __int16 field_1D6;
    __int16 field_1D8;
    __int16 field_1DA;
    __int16 field_1DC;
    __int16 field_1DE;
    __int16 field_1E0;
    __int16 field_1E2;
    __int16 field_1E4;
    __int16 field_1E6;
    __int16 field_1E8;
    __int16 field_1EA;
    __int16 field_1EC;
    __int16 field_1EE;
    __int16 field_1F0;
    __int16 field_1F2;
    __int16 field_1F4;
    __int16 field_1F6;
    __int16 field_1F8;
    __int16 field_1FA;
    __int16 field_1FC;
    __int16 field_1FE;
    __int16 field_200;
    __int16 field_202;
    __int16 field_204;
    __int16 field_206;
    int field_208;
    __int16 field_20C;
    __int16 field_20E;
    __int16 field_210;
    __int16 field_212;
    __int16 field_214;
    __int16 field_216;
    Path_Slig field_218_tlv_data;
    PSX_Point field_268_points[10];
    __int16 field_290_points_count;
    __int16 field_292;
    int field_294;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x298);

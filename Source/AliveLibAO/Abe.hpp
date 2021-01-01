#pragma once

#include "FunctionFwd.hpp"
#include "BellSong.hpp"
#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Input.hpp"

namespace AO {

#define ABE_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_423520) \
    ENTRY(State_1_WalkLoop_423F90) \
    ENTRY(State_2_StandingTurn_426040) \
    ENTRY(State_3_Fall_42E7F0) \
    ENTRY(State_4_WalkToIdle_4243C0) \
    ENTRY(State_5_MidWalkToIdle_424490) \
    ENTRY(State_6_WalkBegin_424300) \
    ENTRY(State_7_Speak_42F950) \
    ENTRY(State_8_Speak_42F9D0) \
    ENTRY(State_9_Speak_42FA50) \
    ENTRY(State_10_Speak_42FAD0) \
    ENTRY(State_11_Speak_42FB50) \
    ENTRY(State_12_Speak_42FBD0) \
    ENTRY(State_13_Speak_42FC50) \
    ENTRY(State_14_Speak_42FCD0) \
    ENTRY(State_15_Null_42A210) \
    ENTRY(State_16_HoistBegin_426E40) \
    ENTRY(State_17_HoistIdle_4269E0) \
    ENTRY(State_18_HoistLand_426EB0) \
    ENTRY(State_19_CrouchIdle_4284C0) \
    ENTRY(State_20_CrouchToStand_428AF0) \
    ENTRY(State_21_StandToCrouch_427F40) \
    ENTRY(State_22_CrouchSpeak_428A30) \
    ENTRY(State_23_CrouchSpeak_428A90) \
    ENTRY(State_24_RollBegin_427A20) \
    ENTRY(State_25_RollLoop_427BB0) \
    ENTRY(State_26_RollEnd_427EA0) \
    ENTRY(State_27_RunSlideStop_425B60) \
    ENTRY(State_28_RunTurn_425CE0) \
    ENTRY(State_29_HopBegin_4267B0) \
    ENTRY(State_30_HopMid_4264D0) \
    ENTRY(State_31_HopLand_426940) \
    ENTRY(State_32_RunJumpBegin_427440) \
    ENTRY(State_33_RunJumpMid_426FA0) \
    ENTRY(State_34_RunJumpLand_427560) \
    ENTRY(State_35_RunLoop_425060) \
    ENTRY(State_36_DunnoBegin_423260) \
    ENTRY(State_37_DunnoMid_4232C0) \
    ENTRY(State_38_DunnoEnd_423310) \
    ENTRY(State_39_CrouchTurn_4288C0) \
    ENTRY(State_40_RunToRoll_427AE0) \
    ENTRY(State_41_StandingToRun_425530) \
    ENTRY(State_42_SneakLoop_424BB0) \
    ENTRY(State_43_WalkToSneak_424790) \
    ENTRY(State_44_SneakToWalk_4249A0) \
    ENTRY(State_45_MidWalkToSneak_424890) \
    ENTRY(State_46_MidSneakToWalk_424AA0) \
    ENTRY(State_47_SneakBegin_424ED0) \
    ENTRY(State_48_SneakToIdle_424F80) \
    ENTRY(State_49_MidSneakToIdle_424FF0) \
    ENTRY(State_50_WalkToRun_424560) \
    ENTRY(State_51_MidWalkToRun_424670) \
    ENTRY(State_52_RunToWalk_4255E0) \
    ENTRY(State_53_MidRunToWalk_4256E0) \
    ENTRY(State_54_RunTurnToRun_425EA0) \
    ENTRY(State_55_RunTurnToWalk_425F70) \
    ENTRY(State_56_RunJumpLandRun_4278E0) \
    ENTRY(State_57_RunJumpLand_Walk_427980) \
    ENTRY(State_58_ToSpeak_42F8D0) \
    ENTRY(State_59_DeathDropFall_42CBE0) \
    ENTRY(State_60_Dead_42C4C0) \
    ENTRY(State_61_Respawn_42CD20) \
    ENTRY(State_62_LoadedSaveSpawn_45ADD0) \
    ENTRY(State_63_TurnToRun_42A0A0) \
    ENTRY(State_64_LedgeAscend_428B60) \
    ENTRY(State_65_LedgeDescend_428C00) \
    ENTRY(State_66_LedgeHang_428D90) \
    ENTRY(State_67_ToOffScreenHoist_428C50) \
    ENTRY(State_68_LedgeHangWobble_428E50) \
    ENTRY(State_69_RingRopePull_4299B0) \
    ENTRY(State_70_Knockback_428FB0) \
    ENTRY(State_71_KnockbackGetUp_429240) \
    ENTRY(State_72_PushWall_4292A0) \
    ENTRY(State_73_RollingKnockback_4291D0) \
    ENTRY(State_74_JumpIntoWell_430EC0) \
    ENTRY(State_75_ToInsideOfAWellLocal_431090) \
    ENTRY(State_76_ToWellShotOut_431720) \
    ENTRY(State_77_WellBegin_430F10) \
    ENTRY(State_78_InsideWellLocal_4310A0) \
    ENTRY(State_79_WellShotOut_431730) \
    ENTRY(State_80_430EF0) \
    ENTRY(State_81_InsideWellExpress_431320) \
    ENTRY(State_82_WellExpressShotOut_4315A0) \
    ENTRY(State_83_430F00) \
    ENTRY(State_84_431080) \
    ENTRY(State_85_431710) \
    ENTRY(State_86_FallLandDie_42EDD0) \
    ENTRY(State_87_428FA0) \
    ENTRY(State_88_HandstoneBegin_430590) \
    ENTRY(State_89_HandstoneEnd_430E80) \
    ENTRY(State_90_GrenadeMachineUse_430EA0) \
    ENTRY(State_91_FallingFromGrab_429780) \
    ENTRY(State_92_ForceDownFromHoist_4297C0) \
    ENTRY(State_93_WalkOffEdge_429840) \
    ENTRY(State_94_RunOffEdge_429860) \
    ENTRY(State_95_SneakOffEdge_429880) \
    ENTRY(State_96_HopToFall_4298A0) \
    ENTRY(State_97_RunJumpToFall_429930) \
    ENTRY(State_98_LandSoft_42ED40) \
    ENTRY(State_99_LedgeHoistUp_426DC0) \
    ENTRY(State_100_RollOffLedge_429950) \
    ENTRY(State_101_LeverUse_429970) \
    ENTRY(State_102_ElumWalkLoop_42DCA0) \
    ENTRY(State_103_ElumIdle_42DCD0) \
    ENTRY(State_104_ElumRunSlideStop_42DF00) \
    ENTRY(State_105_ElumRunTurn_42DF10) \
    ENTRY(State_106_42DF20) \
    ENTRY(State_107_ElumHopBegin_42DF30) \
    ENTRY(State_108_ElumHopMid_42DF40) \
    ENTRY(State_109_ElumHopLand_42DF50) \
    ENTRY(State_110_ElumRunJumpBegin_42DF60) \
    ENTRY(State_111_ElumRunJumpMid_42DF70) \
    ENTRY(State_112_ElumRunJumpLand_42DF80) \
    ENTRY(State_113_ElumTurn_42DF90) \
    ENTRY(State_114_ElumRunLoop_42DFA0) \
    ENTRY(State_115_ElumSpeak_4299F0) \
    ENTRY(State_116_42DFB0) \
    ENTRY(State_117_ElumWalkBegin_42DFC0) \
    ENTRY(State_118_ElumRunBegin_42DFD0) \
    ENTRY(State_119_42DFE0) \
    ENTRY(State_120_ElumRunToWalk_42DFF0) \
    ENTRY(State_121_ElumMidRunToWalk_42E000) \
    ENTRY(State_122_ElumRunTurnToRun_42E010) \
    ENTRY(State_123_ElumRunTurnToWalk_42E020) \
    ENTRY(State_124_ElumWalkEnd_42DCB0) \
    ENTRY(State_125_ElumMidWalkEnd_42DCC0) \
    ENTRY(State_126_ElumBeesStruggling_42E080) \
    ENTRY(State_127_SlapBomb_429A20) \
    ENTRY(State_128_KnockForward_429330) \
    ENTRY(State_129_RollingKnockForward_4294F0) \
    ENTRY(State_130_KnockForwardGetUp_429560) \
    ENTRY(State_131_LiftUseUp_42F150) \
    ENTRY(State_132_LiftUseDown_42F170) \
    ENTRY(State_133_LiftGrabBegin_42EF20) \
    ENTRY(State_134_LiftGrabEnd_42EFE0) \
    ENTRY(State_135_LiftGrabIdle_42F000) \
    ENTRY(State_136_ElumMountEnd_42E110) \
    ENTRY(State_137_ElumUnmountBegin_42E2B0) \
    ENTRY(State_138_ElumUnmountEnd_42E390) \
    ENTRY(State_139_ElumMountBegin_42E090) \
    ENTRY(State_140_BeesStruggling_423F30) \
    ENTRY(State_141_BeesStrugglingOnLift_42F390) \
    ENTRY(State_142_RockThrowStandingHold_429CE0) \
    ENTRY(State_143_RockThrowStandingThrow_429FD0) \
    ENTRY(State_144_RockThrowStandingEnd_429DE0) \
    ENTRY(State_145_RockThrowCrouchingHold_428930) \
    ENTRY(State_146_RockThrowCrouchingThrow_4289F0) \
    ENTRY(State_147_ShotRolling_4295C0) \
    ENTRY(State_148_Shot_4296A0) \
    ENTRY(State_149_PickupItem_42A030) \
    ENTRY(State_150_Chant_42FD50) \
    ENTRY(State_151_ChantEnd_430530) \
    ENTRY(State_152_ElumFallOffEdge_42E030) \
    ENTRY(State_153_ElumFall_42E040) \
    ENTRY(State_154_ElumLand_42E050) \
    ENTRY(State_155_ElumJumpToFall_42E060) \
    ENTRY(State_156_DoorEnter_42D370) \
    ENTRY(State_157_DoorExit_42D780) \
    ENTRY(State_158_ElumKnockback_42E070) \
    ENTRY(State_159_Idle_RubEyes_423360) \
    ENTRY(State_160_Idle_Stretch_Arms_4233A0) \
    ENTRY(State_161_Idle_Yawn_4233E0) \
    ENTRY(State_162_ToShrykull_42F410) \
    ENTRY(State_163_ShrykullEnd_42F520) \
    ENTRY(State_164_PoisonGasDeath_42A120)

#define MAKE_ENUM(VAR) VAR,
enum eAbeStates : short
{
    ABE_STATES_ENUM(MAKE_ENUM)
};

class DeathFadeOut;
class OrbWhirlWind;
class BirdPortal;
class PullRingRope;
class CircularFade;
class BaseThrowable;
class Rope;
class Bullet;

enum Flags_2A8
{
    e2A8_Bit1 = 0x1,
    e2A8_Bit2 = 0x2,
    e2A8_Bit3_WalkToRun = 0x4,
    e2A8_Bit4_Fall_To_Well = 0x8,
    e2A8_Bit5 = 0x10,
    e2A8_Bit6_bShrivel = 0x20,
    e2A8_Bit7 = 0x40,
    e2A8_Bit8_bLandSoft = 0x80,
    e2A8_Bit9 = 0x100,
    e2A8_Bit10 = 0x200,
    e2A8_Bit11_bLaughAtChantEnd = 0x400,
    e2A8_Bit12_bParamoniaDone = 0x800,
    e2A8_eBit13_bScrabaniaDone = 0x1000,
    e2A8_eBit14_bGotShrykullFromBigFace = 0x2000,
    e2A8_eBit15_bGiveShrykullFromBigFace = 0x4000,
    e2A8_eBit16_AbeSpawnDir = 0x8000,
};

enum Flags_2AA
{
    e2AA_Bit1 = 0x1,
    e2AA_Bit2_bSfxPlaying = 0x2,
    e2AA_Bit3_ElumMountBegin = 0x4,
    e2AA_Bit4_ElumMountEnd = 0x8,
    e2AA_Bit5_ElumUnmountBegin = 0x10
};

enum class StoneStates : __int16
{
    eUnknown_0 = 0,
    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eUnknown_3 = 3,
    eUnknown_4 = 4,
    eUnknown_5 = 5,
    eUnknown_6 = 6,
    eUnknown_7 = 7,

    eUnknown_12 = 12,
    eUnknown_13 = 13
};

enum class ChantStates : __int16
{
    eUnknown_0 = 0,
    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eUnknown_3 = 3,
    eUnknown_4 = 4,
    eUnknown_5 = 5,
    eUnknown_6 = 6,
    eUnknown_7 = 7,

    eUnknown_12 = 12,
    eUnknown_13 = 13
};

enum class DoorStates : __int16
{
    eAbeComesIn_0 = 0,
    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eUnknown_3 = 3,
    eUnknown_4 = 4,
    eUnknown_5 = 5,
    eAbeComesOut_6 = 6
};

union AllInternalStates
{
    ChantStates chant;
    StoneStates stone;
    DoorStates door;
    __int16 raw;
};

enum class MudSounds : unsigned __int8
{
    eUnknown_0 = 0, // empty?
    eWhistle1_1 = 1,
    eWhistle2_2 = 2,
    eHello_3 = 3,
    eFollowMe_4 = 4,
    eAngry_5 = 5,
    eWait_6 = 6,
    eFart_7 = 7,
    eLaugh1_8 = 8,
    ePassword_9 = 9,
    eKnockbackOuch_10 = 10,
    eLaugh2_11 = 11,
    eGoodbye_12 = 12,
    eOkay_13 = 13,
    eRefuse_14 = 14,
    eDunno_15 = 15,
    eOops_16 = 16,
    eDeathDropScream_17 = 17,
    eBeesStruggle_18 = 18,
    eUnknown_19 = 19 // empty?
};

enum class EnvironmentSfx : unsigned __int8
{
    eSlideStop_0 = 0,
    eWalkingFootstep_1 = 1,
    eRunningFootstep_2 = 2,
    eSneakFootstep_3 = 3,
    eRunSlide_4 = 4,
    eLandingSoft_5 = 5,
    eHitGroundSoft_6 = 6,
    eDeathNoise_7 = 7,
    eRollingNoise_8 = 8,
    eGenericMovement_9 = 9,
    eExhaustingHoistNoise_10 = 10,
    eRunJumpOrLedgeHoist_11 = 11,
    eUnknown_12 = 12, // empty?
    eKnockback_13 = 13,
    eElumHitWall_14 = 14,
    eFallingDeathScreamHitGround_15 = 15,
    eExhaustingElumMount_16 = 16,
    eMountElumSmackNoise_17 = 17,
    eElumGetMountedNoise_18 = 18,
    eAbeMountedElumNoise_19 = 19,
    eElumHowl_20 = 20,
    eElumOkay_21 = 21
};

struct SaveData;

struct Path_ContinuePoint : public Path_TLV
{
    __int16 field_18_zone_number;
    __int16 field_1A_clear_from_id;
    __int16 field_1C_clear_to_id;
    __int16 field_1E_elum_restarts;
    __int16 field_20_abe_direction;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ContinuePoint, 0x24);

struct AbeResources
{
    BYTE **res[65];
};

struct Path_Stone_camera
{
    LevelIds level_1;
    __int16 path_2;
    __int16 camera_3;
};

struct Path_BellsongStone_data
{
    __int16 scale;
    BellsongTypes type;
    __int16 code1;
    __int16 code2;
    __int16 id;
    __int16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_BellsongStone_data, 12);

struct Path_Handstone_data
{
    __int16 scale;
    Path_Stone_camera cameras[3];
};
ALIVE_ASSERT_SIZEOF(Path_Handstone_data, 0x14);

struct Path_Moviestone_data
{
    __int16 fmvId;
    __int16 scale;
};
ALIVE_ASSERT_SIZEOF(Path_Moviestone_data, 4);

union AllStone
{
    Path_Handstone_data dataHandstone;
    Path_BellsongStone_data dataBellsong;
    Path_Moviestone_data dataMovie;
    unsigned short demoId;
};

struct Path_Stone : public Path_TLV
{
    AllStone field_18_data;
};
ALIVE_ASSERT_SIZEOF(Path_Stone, 0x2C);

class Abe : public BaseAliveGameObject
{
public: 
    BOOL Is_Celling_Above();

    EXPORT Abe* ctor_420770(int frameTableOffset, int a3, int a4, int a5);

    EXPORT BaseGameObject* dtor_420C80();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_42EED0();

    // Virtual impls
    EXPORT BaseGameObject* vdtor_422A70(signed int flags);
    EXPORT void vUpdate_41FDB0();
    EXPORT void vRender_420F30(int** pOrderingTable);
    EXPORT void vScreenChanged_422640();

    EXPORT void VOn_Tlv_Collision_421130(Path_TLV *pTlv);


    // Non virtuals

    void CheckForPortalAndRunJump();

    static EXPORT void Free_Shrykull_Resources_42F4C0();

    EXPORT void FreeElumRes_420F80();

    EXPORT void ToDeathDropFall_42C3D0();

    EXPORT BOOL IsStanding_41FC10();

    EXPORT void FollowLift_42EE90();

    EXPORT void ExitShrykull_42F440(__int16 bResetRingTimer);

    EXPORT __int16 RunTryEnterWell_425880();

    EXPORT void sub_430510(__int16 a2);

    EXPORT static BaseAliveGameObject* CC FindObjectToPosses_421410();

    EXPORT static void CC Get_Shrykull_Resources_42F480();

    EXPORT void ToDieFinal_42C400();

    EXPORT void ToKnockback_422D90(__int16 bUnknownSound, __int16 bDelayedAnger);

    EXPORT BYTE** StateToAnimResource_4204F0(short motion);

    EXPORT void ToIdle_422D50();

    EXPORT void MoveForward_422FC0();

    EXPORT __int16 MoveLiftUpOrDown_42F190(FP ySpeed);

    EXPORT void ElumFree_4228F0();

    EXPORT short DoGameSpeak_42F5C0(unsigned __int16 input);

    EXPORT void SyncToElum_42D850(__int16 elumMotion);

    EXPORT void PickUpThrowabe_Or_PressBomb_428260(FP fpX, int fpY, __int16 bStandToCrouch);

    EXPORT void CrouchingGameSpeak_427F90();

    EXPORT void FallOnBombs_4231B0();

    EXPORT __int16 ToLeftRightMovement_422AA0();

    EXPORT void MoveWithVelocity_4257F0(FP speed);

    EXPORT void ToNewElumSyncMotion_422520(short elum_frame);

    EXPORT void SetActiveControlledCharacter_421480();

    EXPORT PullRingRope* GetPullRope_422580();

    EXPORT void Free_Resources_422870();

    EXPORT static void CC Load_Basic_Resources_4228A0();

    EXPORT void LoadMountElumResources_42E690();

    EXPORT void ElumKnockForward_42E780(int not_used);

    EXPORT __int16 TryMountElum_42E600();

    EXPORT void BulletDamage_4220B0(Bullet* a2);

    EXPORT __int16 RunTryEnterDoor_4259C0();

    BOOL NearDoorIsOpen();

    EXPORT void IntoPortalStates_4262A0();

    EXPORT void TryHoist_423420();

    EXPORT __int16 HandleDoAction_429A70();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom);

    EXPORT __int16 VTakeDamage_4214E0(BaseGameObject* pFrom);

    void New_RandomizedChant_Particle();

    // States
    EXPORT void State_0_Idle_423520();
    EXPORT void State_1_WalkLoop_423F90();
    EXPORT void State_2_StandingTurn_426040();
    EXPORT void State_3_Fall_42E7F0();
    EXPORT void State_4_WalkToIdle_4243C0();
    EXPORT void State_5_MidWalkToIdle_424490();
    EXPORT void State_6_WalkBegin_424300();
    EXPORT void State_7_Speak_42F950();
    EXPORT void State_8_Speak_42F9D0();
    EXPORT void State_9_Speak_42FA50();
    EXPORT void State_10_Speak_42FAD0();
    EXPORT void State_11_Speak_42FB50();
    EXPORT void State_12_Speak_42FBD0();
    EXPORT void State_13_Speak_42FC50();
    EXPORT void State_14_Speak_42FCD0();
    EXPORT void State_15_Null_42A210();
    EXPORT void State_16_HoistBegin_426E40();
    EXPORT void State_17_HoistIdle_4269E0();
    EXPORT void State_18_HoistLand_426EB0();
    EXPORT void State_19_CrouchIdle_4284C0();
    EXPORT void State_20_CrouchToStand_428AF0();
    EXPORT void State_21_StandToCrouch_427F40();
    EXPORT void State_22_CrouchSpeak_428A30();
    EXPORT void State_23_CrouchSpeak_428A90();
    EXPORT void State_24_RollBegin_427A20();
    EXPORT void State_25_RollLoop_427BB0();
    EXPORT void State_26_RollEnd_427EA0();
    EXPORT void State_27_RunSlideStop_425B60();
    EXPORT void State_28_RunTurn_425CE0();
    EXPORT void State_29_HopBegin_4267B0();
    EXPORT void State_30_HopMid_4264D0();
    EXPORT void State_31_HopLand_426940();
    EXPORT void State_32_RunJumpBegin_427440();
    EXPORT void State_33_RunJumpMid_426FA0();
    EXPORT void State_34_RunJumpLand_427560();
    EXPORT void State_35_RunLoop_425060();
    EXPORT void State_36_DunnoBegin_423260();
    EXPORT void State_37_DunnoMid_4232C0();
    EXPORT void State_38_DunnoEnd_423310();
    EXPORT void State_39_CrouchTurn_4288C0();
    EXPORT void State_40_RunToRoll_427AE0();
    EXPORT void State_41_StandingToRun_425530();
    EXPORT void State_42_SneakLoop_424BB0();
    EXPORT void State_43_WalkToSneak_424790();
    EXPORT void State_44_SneakToWalk_4249A0();
    EXPORT void State_45_MidWalkToSneak_424890();
    EXPORT void State_46_MidSneakToWalk_424AA0();
    EXPORT void State_47_SneakBegin_424ED0();
    EXPORT void State_48_SneakToIdle_424F80();
    EXPORT void State_49_MidSneakToIdle_424FF0();
    EXPORT void State_50_WalkToRun_424560();
    EXPORT void State_51_MidWalkToRun_424670();
    EXPORT void State_52_RunToWalk_4255E0();
    EXPORT void State_53_MidRunToWalk_4256E0();
    EXPORT void State_54_RunTurnToRun_425EA0();
    EXPORT void State_55_RunTurnToWalk_425F70();
    EXPORT void State_56_RunJumpLandRun_4278E0();
    EXPORT void State_57_RunJumpLand_Walk_427980();
    EXPORT void State_58_ToSpeak_42F8D0();
    EXPORT void State_59_DeathDropFall_42CBE0();
    EXPORT void State_60_Dead_42C4C0();
    EXPORT void State_61_Respawn_42CD20();
    EXPORT void State_62_LoadedSaveSpawn_45ADD0();
    EXPORT void State_63_TurnToRun_42A0A0();
    EXPORT void State_64_LedgeAscend_428B60();
    EXPORT void State_65_LedgeDescend_428C00();
    EXPORT void State_66_LedgeHang_428D90();
    EXPORT void State_67_ToOffScreenHoist_428C50();
    EXPORT void State_68_LedgeHangWobble_428E50();
    EXPORT void State_69_RingRopePull_4299B0();
    EXPORT void State_70_Knockback_428FB0();
    EXPORT void State_71_KnockbackGetUp_429240();
    EXPORT void State_72_PushWall_4292A0();
    EXPORT void State_73_RollingKnockback_4291D0();
    EXPORT void State_74_JumpIntoWell_430EC0();
    EXPORT void State_75_ToInsideOfAWellLocal_431090();
    EXPORT void State_76_ToWellShotOut_431720();
    EXPORT void State_77_WellBegin_430F10();
    EXPORT void State_78_InsideWellLocal_4310A0();
    EXPORT void State_79_WellShotOut_431730();
    EXPORT void State_80_430EF0();
    EXPORT void State_81_InsideWellExpress_431320();
    EXPORT void State_82_WellExpressShotOut_4315A0();
    EXPORT void State_83_430F00();
    EXPORT void State_84_431080();
    EXPORT void State_85_431710();
    EXPORT void State_86_FallLandDie_42EDD0();
    EXPORT void State_87_428FA0();
    EXPORT void State_88_HandstoneBegin_430590();
    EXPORT void State_89_HandstoneEnd_430E80();
    EXPORT void State_90_GrenadeMachineUse_430EA0();
    EXPORT void State_91_FallingFromGrab_429780();
    EXPORT void State_92_ForceDownFromHoist_4297C0();
    EXPORT void State_93_WalkOffEdge_429840();
    EXPORT void State_94_RunOffEdge_429860();
    EXPORT void State_95_SneakOffEdge_429880();
    EXPORT void State_96_HopToFall_4298A0();
    EXPORT void State_97_RunJumpToFall_429930();
    EXPORT void State_98_LandSoft_42ED40();
    EXPORT void State_99_LedgeHoistUp_426DC0();
    EXPORT void State_100_RollOffLedge_429950();
    EXPORT void State_101_LeverUse_429970();
    EXPORT void State_102_ElumWalkLoop_42DCA0();
    EXPORT void State_103_ElumIdle_42DCD0();
    EXPORT void State_104_ElumRunSlideStop_42DF00();
    EXPORT void State_105_ElumRunTurn_42DF10();
    EXPORT void State_106_42DF20();
    EXPORT void State_107_ElumHopBegin_42DF30();
    EXPORT void State_108_ElumHopMid_42DF40();
    EXPORT void State_109_ElumHopLand_42DF50();
    EXPORT void State_110_ElumRunJumpBegin_42DF60();
    EXPORT void State_111_ElumRunJumpMid_42DF70();
    EXPORT void State_112_ElumRunJumpLand_42DF80();
    EXPORT void State_113_ElumTurn_42DF90();
    EXPORT void State_114_ElumRunLoop_42DFA0();
    EXPORT void State_115_ElumSpeak_4299F0();
    EXPORT void State_116_42DFB0();
    EXPORT void State_117_ElumWalkBegin_42DFC0();
    EXPORT void State_118_ElumRunBegin_42DFD0();
    EXPORT void State_119_42DFE0();
    EXPORT void State_120_ElumRunToWalk_42DFF0();
    EXPORT void State_121_ElumMidRunToWalk_42E000();
    EXPORT void State_122_ElumRunTurnToRun_42E010();
    EXPORT void State_123_ElumRunTurnToWalk_42E020();
    EXPORT void State_124_ElumWalkEnd_42DCB0();
    EXPORT void State_125_ElumMidWalkEnd_42DCC0();
    EXPORT void State_126_ElumBeesStruggling_42E080();
    EXPORT void State_127_SlapBomb_429A20();
    EXPORT void State_128_KnockForward_429330();
    EXPORT void State_129_RollingKnockForward_4294F0();
    EXPORT void State_130_KnockForwardGetUp_429560();
    EXPORT void State_131_LiftUseUp_42F150();
    EXPORT void State_132_LiftUseDown_42F170();
    EXPORT void State_133_LiftGrabBegin_42EF20();
    EXPORT void State_134_LiftGrabEnd_42EFE0();
    EXPORT void State_135_LiftGrabIdle_42F000();
    EXPORT void State_136_ElumMountEnd_42E110();
    EXPORT void State_137_ElumUnmountBegin_42E2B0();
    EXPORT void State_138_ElumUnmountEnd_42E390();
    EXPORT void State_139_ElumMountBegin_42E090();
    EXPORT void State_140_BeesStruggling_423F30();
    EXPORT void State_141_BeesStrugglingOnLift_42F390();
    EXPORT void State_142_RockThrowStandingHold_429CE0();
    EXPORT void State_143_RockThrowStandingThrow_429FD0();
    EXPORT void State_144_RockThrowStandingEnd_429DE0();
    EXPORT void State_145_RockThrowCrouchingHold_428930();
    EXPORT void State_146_RockThrowCrouchingThrow_4289F0();
    EXPORT void State_147_ShotRolling_4295C0();
    EXPORT void State_148_Shot_4296A0();
    EXPORT void State_149_PickupItem_42A030();
    EXPORT void State_150_Chant_42FD50();
    EXPORT void State_151_ChantEnd_430530();
    EXPORT void State_152_ElumFallOffEdge_42E030();
    EXPORT void State_153_ElumFall_42E040();
    EXPORT void State_154_ElumLand_42E050();
    EXPORT void State_155_ElumJumpToFall_42E060();
    EXPORT void State_156_DoorEnter_42D370();
    EXPORT void State_157_DoorExit_42D780();
    EXPORT void State_158_ElumKnockback_42E070();
    EXPORT void State_159_Idle_RubEyes_423360();
    EXPORT void State_160_Idle_Stretch_Arms_4233A0();
    EXPORT void State_161_Idle_Yawn_4233E0();
    EXPORT void State_162_ToShrykull_42F410();
    EXPORT void State_163_ShrykullEnd_42F520();
    EXPORT void State_164_PoisonGasDeath_42A120();

    __int16 field_10C_prev_held;
    __int16 field_10E_released_buttons;
    AllInternalStates field_110_state;
    __int16 field_112_prev_motion;
    int field_114_gnFrame;
    int field_118;
    int field_11C_regen_health_timer;
    FP field_120;
    int field_124;
    __int16 field_128;
    __int16 field_12A;
    int field_12C_timer;
    __int16 field_130_say;
    __int16 field_132;
    int field_134_auto_say_timer;
    PSX_Point field_138_zone_top_left;
    PSX_Point field_13C_zone_bottom_right;
    __int16 field_140_saved_camera;
    __int16 field_142_saved_path;
    LevelIds field_144_saved_level;
    __int16 field_146_zone_number;
    __int16 field_148_clear_from_id;
    __int16 field_14A_clear_to_id;
    FP field_14C_saved_sprite_scale;
    int field_150_saved_ring_timer; // todo: check
    __int16 field_154_bSavedHaveShrykull;
    __int16 field_156;
    DeathFadeOut* field_158_pDeathFadeout;
    BaseThrowable* field_15C_pThrowable;
    PullRingRope* field_160_pRope;
    CircularFade* field_164_pCircularFade;
    int field_168_ring_pulse_timer;
    __int16 field_16C_bHaveShrykull;
    __int16 field_16E_cameraIdx;
    int field_170_hand_stone_type;
    AllStone field_174_pathStone;
    OrbWhirlWind* field_188_pOrbWhirlWind;
    BaseAliveGameObject* field_18C_pObjToPosses;
    LevelIds field_190_level;
    __int16 field_192_path;
    __int16 field_194_camera;
    __int16 field_196_door_id;
    BaseThrowable* field_198_pThrowable;
    char field_19C_throwable_count;
    char field_19D_throw_direction;
    __int16 field_19E_portal_sub_state;
    BirdPortal* field_1A0_portal;
    AbeResources field_1A4_resources;
    BitField16<Flags_2A8> field_2A8_flags;
    BitField16<Flags_2AA> field_2AA_flags;
    SaveData* field_2AC_pSaveData;
    int field_2B0;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x2B4);

ALIVE_VAR_EXTERN(Abe*, sActiveHero_507678);
ALIVE_VAR_EXTERN(BaseAliveGameObject*, sControlledCharacter_50767C);
ALIVE_VAR_EXTERN(short, gAbeInvulnerableCheat_5076E4);
ALIVE_VAR_EXTERN(short, gAbeInvisibleCheat_5076F8);

EXPORT int CC Environment_SFX_42A220(EnvironmentSfx sfxId, signed int volume, int pitchMin, BaseAliveGameObject* pAliveObj);

EXPORT int CC Mudokon_SFX_42A4D0(MudSounds idx, int volume, int pitch, BaseAliveGameObject* pHero);

EXPORT int CC XGrid_Index_To_XPos_41FA60(FP scale, int xGridIndex);

};

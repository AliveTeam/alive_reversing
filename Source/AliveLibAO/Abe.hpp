#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BellSong.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Input.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

    
struct Path_ResetPath final : public Path_TLV
{
    s16 field_18_clearIds;
    s16 field_1A_from;
    s16 field_1C_to;
    s16 field_1E_exclude;
    s16 field_20_clearObjects;
    s16 field_22_path;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ResetPath, 0x24);

#define AO_ABE_MOTIONS_ENUM(ENTRY)                   \
    ENTRY(Motion_0_Idle_423520)                      \
    ENTRY(Motion_1_WalkLoop_423F90)                  \
    ENTRY(Motion_2_StandingTurn_426040)              \
    ENTRY(Motion_3_Fall_42E7F0)                      \
    ENTRY(Motion_4_WalkToIdle_4243C0)                \
    ENTRY(Motion_5_MidWalkToIdle_424490)             \
    ENTRY(Motion_6_WalkBegin_424300)                 \
    ENTRY(Motion_7_Speak_42F950)                     \
    ENTRY(Motion_8_Speak_42F9D0)                     \
    ENTRY(Motion_9_Speak_42FA50)                     \
    ENTRY(Motion_10_Speak_42FAD0)                    \
    ENTRY(Motion_11_Speak_42FB50)                    \
    ENTRY(Motion_12_Speak_42FBD0)                    \
    ENTRY(Motion_13_Speak_42FC50)                    \
    ENTRY(Motion_14_Speak_42FCD0)                    \
    ENTRY(Motion_15_Null_42A210)                     \
    ENTRY(Motion_16_HoistBegin_426E40)               \
    ENTRY(Motion_17_HoistIdle_4269E0)                \
    ENTRY(Motion_18_HoistLand_426EB0)                \
    ENTRY(Motion_19_CrouchIdle_4284C0)               \
    ENTRY(Motion_20_CrouchToStand_428AF0)            \
    ENTRY(Motion_21_StandToCrouch_427F40)            \
    ENTRY(Motion_22_CrouchSpeak_428A30)              \
    ENTRY(Motion_23_CrouchSpeak_428A90)              \
    ENTRY(Motion_24_RollBegin_427A20)                \
    ENTRY(Motion_25_RollLoop_427BB0)                 \
    ENTRY(Motion_26_RollEnd_427EA0)                  \
    ENTRY(Motion_27_RunSlideStop_425B60)             \
    ENTRY(Motion_28_RunTurn_425CE0)                  \
    ENTRY(Motion_29_HopBegin_4267B0)                 \
    ENTRY(Motion_30_HopMid_4264D0)                   \
    ENTRY(Motion_31_HopLand_426940)                  \
    ENTRY(Motion_32_RunJumpBegin_427440)             \
    ENTRY(Motion_33_RunJumpMid_426FA0)               \
    ENTRY(Motion_34_RunJumpLand_427560)              \
    ENTRY(Motion_35_RunLoop_425060)                  \
    ENTRY(Motion_36_DunnoBegin_423260)               \
    ENTRY(Motion_37_DunnoMid_4232C0)                 \
    ENTRY(Motion_38_DunnoEnd_423310)                 \
    ENTRY(Motion_39_CrouchTurn_4288C0)               \
    ENTRY(Motion_40_RunToRoll_427AE0)                \
    ENTRY(Motion_41_StandingToRun_425530)            \
    ENTRY(Motion_42_SneakLoop_424BB0)                \
    ENTRY(Motion_43_WalkToSneak_424790)              \
    ENTRY(Motion_44_SneakToWalk_4249A0)              \
    ENTRY(Motion_45_MidWalkToSneak_424890)           \
    ENTRY(Motion_46_MidSneakToWalk_424AA0)           \
    ENTRY(Motion_47_SneakBegin_424ED0)               \
    ENTRY(Motion_48_SneakToIdle_424F80)              \
    ENTRY(Motion_49_MidSneakToIdle_424FF0)           \
    ENTRY(Motion_50_WalkToRun_424560)                \
    ENTRY(Motion_51_MidWalkToRun_424670)             \
    ENTRY(Motion_52_RunToWalk_4255E0)                \
    ENTRY(Motion_53_MidRunToWalk_4256E0)             \
    ENTRY(Motion_54_RunTurnToRun_425EA0)             \
    ENTRY(Motion_55_RunTurnToWalk_425F70)            \
    ENTRY(Motion_56_RunJumpLandRun_4278E0)           \
    ENTRY(Motion_57_RunJumpLand_Walk_427980)         \
    ENTRY(Motion_58_ToSpeak_42F8D0)                  \
    ENTRY(Motion_59_DeathDropFall_42CBE0)            \
    ENTRY(Motion_60_Dead_42C4C0)                     \
    ENTRY(Motion_61_Respawn_42CD20)                  \
    ENTRY(Motion_62_LoadedSaveSpawn_45ADD0)          \
    ENTRY(Motion_63_TurnToRun_42A0A0)                \
    ENTRY(Motion_64_LedgeAscend_428B60)              \
    ENTRY(Motion_65_LedgeDescend_428C00)             \
    ENTRY(Motion_66_LedgeHang_428D90)                \
    ENTRY(Motion_67_ToOffScreenHoist_428C50)         \
    ENTRY(Motion_68_LedgeHangWobble_428E50)          \
    ENTRY(Motion_69_RingRopePullHang_4299B0)             \
    ENTRY(Motion_70_Knockback_428FB0)                \
    ENTRY(Motion_71_KnockbackGetUp_429240)           \
    ENTRY(Motion_72_PushWall_4292A0)                 \
    ENTRY(Motion_73_RollingKnockback_4291D0)         \
    ENTRY(Motion_74_JumpIntoWell_430EC0)             \
    ENTRY(Motion_75_ToInsideOfAWellLocal_431090)     \
    ENTRY(Motion_76_ToWellShotOut_431720)            \
    ENTRY(Motion_77_WellBegin_430F10)                \
    ENTRY(Motion_78_InsideWellLocal_4310A0)          \
    ENTRY(Motion_79_WellShotOut_431730)              \
    ENTRY(Motion_80_430EF0)                          \
    ENTRY(Motion_81_InsideWellExpress_431320)        \
    ENTRY(Motion_82_WellExpressShotOut_4315A0)       \
    ENTRY(Motion_83_430F00)                          \
    ENTRY(Motion_84_ToInsideWellLocal_431080)                          \
    ENTRY(Motion_85_ToWellShotOut_431710)                          \
    ENTRY(Motion_86_FallLandDie_42EDD0)              \
    ENTRY(Motion_87_ToFall_428FA0)                          \
    ENTRY(Motion_88_HandstoneBegin_430590)           \
    ENTRY(Motion_89_HandstoneEnd_430E80)             \
    ENTRY(Motion_90_GrenadeMachineUse_430EA0)        \
    ENTRY(Motion_91_FallingFromGrab_429780)          \
    ENTRY(Motion_92_ForceDownFromHoist_4297C0)       \
    ENTRY(Motion_93_WalkOffEdge_429840)              \
    ENTRY(Motion_94_RunOffEdge_429860)               \
    ENTRY(Motion_95_SneakOffEdge_429880)             \
    ENTRY(Motion_96_HopToFall_4298A0)                \
    ENTRY(Motion_97_RunJumpToFall_429930)            \
    ENTRY(Motion_98_LandSoft_42ED40)                 \
    ENTRY(Motion_99_HoistBeginLong_426DC0)             \
    ENTRY(Motion_100_RollOffLedge_429950)            \
    ENTRY(Motion_101_LeverUse_429970)                \
    ENTRY(Motion_102_ElumWalkLoop_42DCA0)            \
    ENTRY(Motion_103_ElumIdle_42DCD0)                \
    ENTRY(Motion_104_ElumRunSlideStop_42DF00)        \
    ENTRY(Motion_105_ElumRunTurn_42DF10)             \
    ENTRY(Motion_106_Null_42DF20)                         \
    ENTRY(Motion_107_ElumHopBegin_42DF30)            \
    ENTRY(Motion_108_ElumHopMid_42DF40)              \
    ENTRY(Motion_109_ElumHopLand_42DF50)             \
    ENTRY(Motion_110_ElumRunJumpBegin_42DF60)        \
    ENTRY(Motion_111_ElumRunJumpMid_42DF70)          \
    ENTRY(Motion_112_ElumRunJumpLand_42DF80)         \
    ENTRY(Motion_113_ElumTurn_42DF90)                \
    ENTRY(Motion_114_ElumRunLoop_42DFA0)             \
    ENTRY(Motion_115_ElumSpeak_4299F0)               \
    ENTRY(Motion_116_Null_42DFB0)                         \
    ENTRY(Motion_117_ElumWalkBegin_42DFC0)           \
    ENTRY(Motion_118_ElumRunBegin_42DFD0)            \
    ENTRY(Motion_119_Null_42DFE0)                         \
    ENTRY(Motion_120_ElumRunToWalk_42DFF0)           \
    ENTRY(Motion_121_ElumMidRunToWalk_42E000)        \
    ENTRY(Motion_122_ElumRunTurnToRun_42E010)        \
    ENTRY(Motion_123_ElumRunTurnToWalk_42E020)       \
    ENTRY(Motion_124_ElumWalkEnd_42DCB0)             \
    ENTRY(Motion_125_ElumMidWalkEnd_42DCC0)          \
    ENTRY(Motion_126_ElumBeesStruggling_42E080)      \
    ENTRY(Motion_127_SlapBomb_429A20)                \
    ENTRY(Motion_128_KnockForward_429330)            \
    ENTRY(Motion_129_RollingKnockForward_4294F0)     \
    ENTRY(Motion_130_KnockForwardGetUp_429560)       \
    ENTRY(Motion_131_LiftUseUp_42F150)               \
    ENTRY(Motion_132_LiftUseDown_42F170)             \
    ENTRY(Motion_133_LiftGrabBegin_42EF20)           \
    ENTRY(Motion_134_LiftGrabEnd_42EFE0)             \
    ENTRY(Motion_135_LiftGrabIdle_42F000)            \
    ENTRY(Motion_136_ElumMountEnd_42E110)            \
    ENTRY(Motion_137_ElumUnmountBegin_42E2B0)        \
    ENTRY(Motion_138_ElumUnmountEnd_42E390)          \
    ENTRY(Motion_139_ElumMountBegin_42E090)          \
    ENTRY(Motion_140_BeesStruggling_423F30)          \
    ENTRY(Motion_141_BeesStrugglingOnLift_42F390)    \
    ENTRY(Motion_142_RockThrowStandingHold_429CE0)   \
    ENTRY(Motion_143_RockThrowStandingThrow_429FD0)  \
    ENTRY(Motion_144_RockThrowStandingEnd_429DE0)    \
    ENTRY(Motion_145_RockThrowCrouchingHold_428930)  \
    ENTRY(Motion_146_RockThrowCrouchingThrow_4289F0) \
    ENTRY(Motion_147_ShotRolling_4295C0)             \
    ENTRY(Motion_148_Shot_4296A0)                    \
    ENTRY(Motion_149_PickupItem_42A030)              \
    ENTRY(Motion_150_Chant_42FD50)                   \
    ENTRY(Motion_151_ChantEnd_430530)                \
    ENTRY(Motion_152_ElumFallOffEdge_42E030)         \
    ENTRY(Motion_153_ElumFall_42E040)                \
    ENTRY(Motion_154_ElumLand_42E050)                \
    ENTRY(Motion_155_ElumJumpToFall_42E060)          \
    ENTRY(Motion_156_DoorEnter_42D370)               \
    ENTRY(Motion_157_DoorExit_42D780)                \
    ENTRY(Motion_158_ElumKnockback_42E070)           \
    ENTRY(Motion_159_Idle_RubEyes_423360)            \
    ENTRY(Motion_160_Idle_Stretch_Arms_4233A0)       \
    ENTRY(Motion_161_Idle_Yawn_4233E0)               \
    ENTRY(Motion_162_ToShrykull_42F410)              \
    ENTRY(Motion_163_ShrykullEnd_42F520)             \
    ENTRY(Motion_164_PoisonGasDeath_42A120)

#define MAKE_ENUM(VAR) VAR,
enum eAbeMotions : s16
{
    AO_ABE_MOTIONS_ENUM(MAKE_ENUM)
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
    e2A8_Bit2_return_to_previous_motion = 0x2,
    e2A8_Bit3_WalkToRun = 0x4,
    e2A8_Bit4_snap_abe = 0x8,
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

// elum flags?
enum Flags_2AA
{
    e2AA_Bit1 = 0x1,
    e2AA_Bit2_bSfxPlaying = 0x2,
    e2AA_Bit3_ElumMountBegin = 0x4,
    e2AA_Bit4_ElumMountEnd = 0x8,
    e2AA_Bit5_ElumUnmountBegin = 0x10
};

enum class StoneStates : s16
{
    eHandstoneStart_0 = 0,
    eGetHandstoneType_1 = 1,
    eHandstoneMovieDone_2 = 2,
    eFreeDemoPlaybackResources_3 = 3,
    eBellSongDone_4 = 4,
    eHandstoneEnd_5 = 5,
    eWaitForInput_6 = 6,
    eSetActiveCamToAbeOrWaitForInput_7 = 7,

    eSetActiveCamToAbe_12 = 12,
    eCircularFadeExit_13 = 13
};

enum class ChantStates : s16
{
    eIdleChanting_0 = 0,
    ePossessVictim_1 = 1,
    ePossessedVictim_2 = 2,
    eWaitForUnpossessing_3 = 3,
    eUnpossessing_4 = 4,
    ePadding_5 = 5,
    eChantingForBirdPortal_6 = 6,
    ePadding_7 = 7,

    ePadding_12 = 12,
    ePadding_13 = 13
};

enum class AbeDoorStates : s16
{
    eAbeComesIn_0 = 0,
    eUnused_1 = 1,
    eWaitABit_2 = 2,
    eClearTlvIds_3 = 3,
    eSetNewActiveCamera_4 = 4,
    eSetNewAbePosition_5 = 5,
    eAbeComesOut_6 = 6
};

enum class PortalSubStates : s16
{
    eJumpingInsidePortal_0 = 0,
    eSetNewActiveCamera_1 = 1,
    eHopOutOfPortal_2 = 2,
    ePadding_3 = 3,
    eSetNewAbePosition_4 = 4
};

union AllInternalStates
{
    ChantStates chant;
    StoneStates stone;
    AbeDoorStates door;
    s16 raw;
};

enum class MudSounds : u8
{
    eUnknown_0 = 0, // empty?
    eWhistleHigh_1 = 1,
    eWhistleLow_2 = 2,
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

enum class EnvironmentSfx : u8
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

struct Path_RingCancel : public Path_TLV
{
};

struct Path_Edge final : public Path_TLV
{
    enum class GrabDirection : s16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    GrabDirection field_18_grab_direction;
    Choice_short field_1A_can_grab;
};

struct Path_SoftLanding final : public Path_TLV
{ };

struct Path_ContinuePoint final : public Path_TLV
{
    s16 field_18_zone_number;
    s16 field_1A_clear_from_id;
    s16 field_1C_clear_to_id;
    Choice_short field_1E_elum_restarts;
    XDirection_short field_20_abe_direction;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ContinuePoint, 0x24);

struct AbeResources final
{
    u8** res[65];
};

struct Path_Stone_camera final
{
    LevelIds level;
    s16 path;
    s16 camera;
};

struct Path_BellsongStone_data final
{
    Scale_short scale;
    BellsongTypes type;
    s16 code1;
    s16 code2;
    s16 switch_id;
    s16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_BellsongStone_data, 12);

struct Path_Handstone_data final
{
    Scale_short scale;
    Path_Stone_camera camera1;
    Path_Stone_camera camera2;
    Path_Stone_camera camera3;
};
ALIVE_ASSERT_SIZEOF(Path_Handstone_data, 0x14);

struct Path_Moviestone_data final
{
    s16 fmvId;
    Scale_short scale;
};
ALIVE_ASSERT_SIZEOF(Path_Moviestone_data, 4);

union AllStone
{
    Path_Handstone_data dataHandstone;
    Path_BellsongStone_data dataBellsong;
    Path_Moviestone_data dataMovie;
    u16 demoId;
};

struct Path_Stone final : public Path_TLV
{
    AllStone field_18_data;
};
ALIVE_ASSERT_SIZEOF(Path_Stone, 0x2C);

class Abe final : public BaseAliveGameObject
{
public:
    Bool32 Is_Celling_Above();

    EXPORT Abe* ctor_420770(s32 frameTableOffset, s32 a3, s32 a4, s32 a5);

    EXPORT BaseGameObject* dtor_420C80();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_42EED0();

    // Virtual impls
    EXPORT BaseGameObject* vdtor_422A70(s32 flags);
    EXPORT void vUpdate_41FDB0();
    EXPORT void vRender_420F30(PrimHeader** ppOt);
    EXPORT void vScreenChanged_422640();

    EXPORT void VOn_Tlv_Collision_421130(Path_TLV* pTlv);


    // Non virtuals

    bool CheckForPortalAndRunJump();

    static EXPORT void Free_Shrykull_Resources_42F4C0();

    EXPORT void FreeElumRes_420F80();

    EXPORT void ToDeathDropFall_42C3D0();

    EXPORT Bool32 IsStanding_41FC10();

    EXPORT void FollowLift_42EE90();

    EXPORT void ExitShrykull_42F440(s16 bResetRingTimer);

    EXPORT s16 RunTryEnterWell_425880();

    EXPORT void ChangeChantState_430510(s16 bKeepChanting);

    EXPORT static BaseAliveGameObject* CC FindObjectToPossess_421410();

    EXPORT static void CC Get_Shrykull_Resources_42F480();

    EXPORT void ToDieFinal_42C400();

    EXPORT void ToKnockback_422D90(s16 bUnknownSound, s16 bDelayedAnger);

    EXPORT u8** StateToAnimResource_4204F0(s16 motion);

    EXPORT void ToIdle_422D50();

    EXPORT void MoveForward_422FC0();

    EXPORT s16 MoveLiftUpOrDown_42F190(FP ySpeed);

    EXPORT void ElumFree_4228F0();

    EXPORT s16 DoGameSpeak_42F5C0(u16 input);

    EXPORT void SyncToElum_42D850(s16 elumMotion);

    EXPORT void PickUpThrowabe_Or_PressBomb_428260(FP fpX, s32 fpY, s16 bStandToCrouch);

    EXPORT void CrouchingGameSpeak_427F90();

    EXPORT void FallOnBombs_4231B0();

    EXPORT s16 ToLeftRightMovement_422AA0();

    EXPORT void MoveWithVelocity_4257F0(FP speed);

    EXPORT void ToNewElumSyncMotion_422520(s16 elum_frame);

    EXPORT void SetActiveControlledCharacter_421480();

    EXPORT PullRingRope* GetPullRope_422580();

    EXPORT void Free_Resources_422870();

    EXPORT static void CC Load_Basic_Resources_4228A0();

    EXPORT void LoadMountElumResources_42E690();

    EXPORT void ElumKnockForward_42E780(s32 not_used);

    EXPORT s16 TryMountElum_42E600();

    EXPORT void BulletDamage_4220B0(Bullet* a2);

    EXPORT s16 RunTryEnterDoor_4259C0();

    Bool32 NearDoorIsOpen();

    EXPORT void IntoPortalStates_4262A0();

    EXPORT void TryHoist_423420();

    EXPORT s16 HandleDoAction_429A70();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_4214E0(BaseGameObject* pFrom);

    void New_RandomizedChant_Particle();

    // Motions
    EXPORT void Motion_0_Idle_423520();
    EXPORT void Motion_1_WalkLoop_423F90();
    EXPORT void Motion_2_StandingTurn_426040();
    EXPORT void Motion_3_Fall_42E7F0();
    EXPORT void Motion_4_WalkToIdle_4243C0();
    EXPORT void Motion_5_MidWalkToIdle_424490();
    EXPORT void Motion_6_WalkBegin_424300();
    EXPORT void Motion_7_Speak_42F950();
    EXPORT void Motion_8_Speak_42F9D0();
    EXPORT void Motion_9_Speak_42FA50();
    EXPORT void Motion_10_Speak_42FAD0();
    EXPORT void Motion_11_Speak_42FB50();
    EXPORT void Motion_12_Speak_42FBD0();
    EXPORT void Motion_13_Speak_42FC50();
    EXPORT void Motion_14_Speak_42FCD0();
    EXPORT void Motion_15_Null_42A210();
    EXPORT void Motion_16_HoistBegin_426E40();
    EXPORT void Motion_17_HoistIdle_4269E0();
    EXPORT void Motion_18_HoistLand_426EB0();
    EXPORT void Motion_19_CrouchIdle_4284C0();
    EXPORT void Motion_20_CrouchToStand_428AF0();
    EXPORT void Motion_21_StandToCrouch_427F40();
    EXPORT void Motion_22_CrouchSpeak_428A30();
    EXPORT void Motion_23_CrouchSpeak_428A90();
    EXPORT void Motion_24_RollBegin_427A20();
    EXPORT void Motion_25_RollLoop_427BB0();
    EXPORT void Motion_26_RollEnd_427EA0();
    EXPORT void Motion_27_RunSlideStop_425B60();
    EXPORT void Motion_28_RunTurn_425CE0();
    EXPORT void Motion_29_HopBegin_4267B0();
    EXPORT void Motion_30_HopMid_4264D0();
    EXPORT void Motion_31_HopLand_426940();
    EXPORT void Motion_32_RunJumpBegin_427440();
    EXPORT void Motion_33_RunJumpMid_426FA0();
    EXPORT void Motion_34_RunJumpLand_427560();
    EXPORT void Motion_35_RunLoop_425060();
    EXPORT void Motion_36_DunnoBegin_423260();
    EXPORT void Motion_37_DunnoMid_4232C0();
    EXPORT void Motion_38_DunnoEnd_423310();
    EXPORT void Motion_39_CrouchTurn_4288C0();
    EXPORT void Motion_40_RunToRoll_427AE0();
    EXPORT void Motion_41_StandingToRun_425530();
    EXPORT void Motion_42_SneakLoop_424BB0();
    EXPORT void Motion_43_WalkToSneak_424790();
    EXPORT void Motion_44_SneakToWalk_4249A0();
    EXPORT void Motion_45_MidWalkToSneak_424890();
    EXPORT void Motion_46_MidSneakToWalk_424AA0();
    EXPORT void Motion_47_SneakBegin_424ED0();
    EXPORT void Motion_48_SneakToIdle_424F80();
    EXPORT void Motion_49_MidSneakToIdle_424FF0();
    EXPORT void Motion_50_WalkToRun_424560();
    EXPORT void Motion_51_MidWalkToRun_424670();
    EXPORT void Motion_52_RunToWalk_4255E0();
    EXPORT void Motion_53_MidRunToWalk_4256E0();
    EXPORT void Motion_54_RunTurnToRun_425EA0();
    EXPORT void Motion_55_RunTurnToWalk_425F70();
    EXPORT void Motion_56_RunJumpLandRun_4278E0();
    EXPORT void Motion_57_RunJumpLand_Walk_427980();
    EXPORT void Motion_58_ToSpeak_42F8D0();
    EXPORT void Motion_59_DeathDropFall_42CBE0();
    EXPORT void Motion_60_Dead_42C4C0();
    EXPORT void Motion_61_Respawn_42CD20();
    EXPORT void Motion_62_LoadedSaveSpawn_45ADD0();
    EXPORT void Motion_63_TurnToRun_42A0A0();
    EXPORT void Motion_64_LedgeAscend_428B60();
    EXPORT void Motion_65_LedgeDescend_428C00();
    EXPORT void Motion_66_LedgeHang_428D90();
    EXPORT void Motion_67_ToOffScreenHoist_428C50();
    EXPORT void Motion_68_LedgeHangWobble_428E50();
    EXPORT void Motion_69_RingRopePullHang_4299B0();
    EXPORT void Motion_70_Knockback_428FB0();
    EXPORT void Motion_71_KnockbackGetUp_429240();
    EXPORT void Motion_72_PushWall_4292A0();
    EXPORT void Motion_73_RollingKnockback_4291D0();
    EXPORT void Motion_74_JumpIntoWell_430EC0();
    EXPORT void Motion_75_ToInsideOfAWellLocal_431090();
    EXPORT void Motion_76_ToWellShotOut_431720();
    EXPORT void Motion_77_WellBegin_430F10();
    EXPORT void Motion_78_InsideWellLocal_4310A0();
    EXPORT void Motion_79_WellShotOut_431730();
    EXPORT void Motion_80_ToWellBegin_430EF0();
    EXPORT void Motion_81_InsideWellExpress_431320();
    EXPORT void Motion_82_WellExpressShotOut_4315A0();
    EXPORT void Motion_83_ToWellBegin_430F00();
    EXPORT void Motion_84_ToInsideWellLocal_431080();
    EXPORT void Motion_85_ToWellShotOut_431710();
    EXPORT void Motion_86_FallLandDie_42EDD0();
    EXPORT void Motion_87_ToFall_428FA0();
    EXPORT void Motion_88_HandstoneBegin_430590();
    EXPORT void Motion_89_HandstoneEnd_430E80();
    EXPORT void Motion_90_GrenadeMachineUse_430EA0();
    EXPORT void Motion_91_FallingFromGrab_429780();
    EXPORT void Motion_92_ForceDownFromHoist_4297C0();
    EXPORT void Motion_93_WalkOffEdge_429840();
    EXPORT void Motion_94_RunOffEdge_429860();
    EXPORT void Motion_95_SneakOffEdge_429880();
    EXPORT void Motion_96_HopToFall_4298A0();
    EXPORT void Motion_97_RunJumpToFall_429930();
    EXPORT void Motion_98_LandSoft_42ED40();
    EXPORT void Motion_99_HoistBeginLong_426DC0();
    EXPORT void Motion_100_RollOffLedge_429950();
    EXPORT void Motion_101_LeverUse_429970();
    EXPORT void Motion_102_ElumWalkLoop_42DCA0();
    EXPORT void Motion_103_ElumIdle_42DCD0();
    EXPORT void Motion_104_ElumRunSlideStop_42DF00();
    EXPORT void Motion_105_ElumRunTurn_42DF10();
    EXPORT void Motion_106_Null_42DF20();
    EXPORT void Motion_107_ElumHopBegin_42DF30();
    EXPORT void Motion_108_ElumHopMid_42DF40();
    EXPORT void Motion_109_ElumHopLand_42DF50();
    EXPORT void Motion_110_ElumRunJumpBegin_42DF60();
    EXPORT void Motion_111_ElumRunJumpMid_42DF70();
    EXPORT void Motion_112_ElumRunJumpLand_42DF80();
    EXPORT void Motion_113_ElumTurn_42DF90();
    EXPORT void Motion_114_ElumRunLoop_42DFA0();
    EXPORT void Motion_115_ElumSpeak_4299F0();
    EXPORT void Motion_116_Null_42DFB0();
    EXPORT void Motion_117_ElumWalkBegin_42DFC0();
    EXPORT void Motion_118_ElumRunBegin_42DFD0();
    EXPORT void Motion_119_Null_42DFE0();
    EXPORT void Motion_120_ElumRunToWalk_42DFF0();
    EXPORT void Motion_121_ElumMidRunToWalk_42E000();
    EXPORT void Motion_122_ElumRunTurnToRun_42E010();
    EXPORT void Motion_123_ElumRunTurnToWalk_42E020();
    EXPORT void Motion_124_ElumWalkEnd_42DCB0();
    EXPORT void Motion_125_ElumMidWalkEnd_42DCC0();
    EXPORT void Motion_126_ElumBeesStruggling_42E080();
    EXPORT void Motion_127_SlapBomb_429A20();
    EXPORT void Motion_128_KnockForward_429330();
    EXPORT void Motion_129_RollingKnockForward_4294F0();
    EXPORT void Motion_130_KnockForwardGetUp_429560();
    EXPORT void Motion_131_LiftUseUp_42F150();
    EXPORT void Motion_132_LiftUseDown_42F170();
    EXPORT void Motion_133_LiftGrabBegin_42EF20();
    EXPORT void Motion_134_LiftGrabEnd_42EFE0();
    EXPORT void Motion_135_LiftGrabIdle_42F000();
    EXPORT void Motion_136_ElumMountEnd_42E110();
    EXPORT void Motion_137_ElumUnmountBegin_42E2B0();
    EXPORT void Motion_138_ElumUnmountEnd_42E390();
    EXPORT void Motion_139_ElumMountBegin_42E090();
    EXPORT void Motion_140_BeesStruggling_423F30();
    EXPORT void Motion_141_BeesStrugglingOnLift_42F390();
    EXPORT void Motion_142_RockThrowStandingHold_429CE0();
    EXPORT void Motion_143_RockThrowStandingThrow_429FD0();
    EXPORT void Motion_144_RockThrowStandingEnd_429DE0();
    EXPORT void Motion_145_RockThrowCrouchingHold_428930();
    EXPORT void Motion_146_RockThrowCrouchingThrow_4289F0();
    EXPORT void Motion_147_ShotRolling_4295C0();
    EXPORT void Motion_148_Shot_4296A0();
    EXPORT void Motion_149_PickupItem_42A030();
    EXPORT void Motion_150_Chant_42FD50();
    EXPORT void Motion_151_ChantEnd_430530();
    EXPORT void Motion_152_ElumFallOffEdge_42E030();
    EXPORT void Motion_153_ElumFall_42E040();
    EXPORT void Motion_154_ElumLand_42E050();
    EXPORT void Motion_155_ElumJumpToFall_42E060();
    EXPORT void Motion_156_DoorEnter_42D370();
    EXPORT void Motion_157_DoorExit_42D780();
    EXPORT void Motion_158_ElumKnockback_42E070();
    EXPORT void Motion_159_Idle_RubEyes_423360();
    EXPORT void Motion_160_Idle_Stretch_Arms_4233A0();
    EXPORT void Motion_161_Idle_Yawn_4233E0();
    EXPORT void Motion_162_ToShrykull_42F410();
    EXPORT void Motion_163_ShrykullEnd_42F520();
    EXPORT void Motion_164_PoisonGasDeath_42A120();

    s16 field_10C_prev_held;
    s16 field_10E_released_buttons;
    AllInternalStates field_110_state;
    s16 field_112_prev_motion;
    s32 field_114_gnFrame;
    s32 field_118_timer;
    s32 field_11C_regen_health_timer;
    FP field_120_x_vel_slow_by;
    s32 field_124_unused;
    s16 field_128_resource_idx;
    s16 field_12A_unused;
    s32 field_12C_timer;
    s16 field_130_say;
    s16 field_132_padding;
    s32 field_134_auto_say_timer;
    PSX_Point field_138_zone_top_left;
    PSX_Point field_13C_zone_bottom_right;
    s16 field_140_saved_camera;
    s16 field_142_saved_path;
    LevelIds field_144_saved_level;
    s16 field_146_zone_number;
    s16 field_148_clear_from_id;
    s16 field_14A_clear_to_id;
    FP field_14C_saved_sprite_scale;
    s32 field_150_saved_ring_timer; // todo: check
    s16 field_154_bSavedHaveShrykull;
    s16 field_156_padding;
    DeathFadeOut* field_158_pDeathFadeout;
    BaseThrowable* field_15C_pThrowable;
    PullRingRope* field_160_pRope;
    CircularFade* field_164_pCircularFade;
    s32 field_168_ring_pulse_timer;
    s16 field_16C_bHaveShrykull;
    s16 field_16E_cameraIdx;
    TlvTypes32 field_170_hand_stone_type;
    AllStone field_174_pathStone;
    OrbWhirlWind* field_188_pOrbWhirlWind;
    BaseAliveGameObject* field_18C_pObjToPossess;
    LevelIds field_190_level;
    s16 field_192_path;
    s16 field_194_camera;
    s16 field_196_door_id;
    BaseThrowable* field_198_pThrowable;
    s8 field_19C_throwable_count;
    s8 field_19D_throw_direction;
    PortalSubStates field_19E_portal_sub_state;
    BirdPortal* field_1A0_portal;
    AbeResources field_1A4_resources;
    BitField16<Flags_2A8> field_2A8_flags;
    BitField16<Flags_2AA> field_2AA_flags;
    SaveData* field_2AC_pSaveData;
    s32 field_2B0_padding;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x2B4);

ALIVE_VAR_EXTERN(Abe*, sActiveHero_507678);
ALIVE_VAR_EXTERN(BaseAliveGameObject*, sControlledCharacter_50767C);
ALIVE_VAR_EXTERN(s16, gAbeInvulnerableCheat_5076E4);
ALIVE_VAR_EXTERN(s16, gAbeInvisibleCheat_5076F8);

EXPORT s32 CC Environment_SFX_42A220(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj);

EXPORT s32 CC Mudokon_SFX_42A4D0(MudSounds idx, s32 volume, s32 pitch, BaseAliveGameObject* pHero);

EXPORT s32 CC XGrid_Index_To_XPos_41FA60(FP scale, s32 xGridIndex);

} // namespace AO

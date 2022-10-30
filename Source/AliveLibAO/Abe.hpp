#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BellSong.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Input.hpp"
#include "Path.hpp"
#include "../relive_lib/data_conversion/PathTlvsAO.hpp"

namespace AO {

#define AO_ABE_MOTIONS_ENUM(ENTRY)                   \
    ENTRY(Motion_0_Idle)                      \
    ENTRY(Motion_1_WalkLoop)                  \
    ENTRY(Motion_2_StandingTurn)              \
    ENTRY(Motion_3_Fall)                      \
    ENTRY(Motion_4_WalkToIdle)                \
    ENTRY(Motion_5_MidWalkToIdle)             \
    ENTRY(Motion_6_WalkBegin)                 \
    ENTRY(Motion_7_Speak)                     \
    ENTRY(Motion_8_Speak)                     \
    ENTRY(Motion_9_Speak)                     \
    ENTRY(Motion_10_Speak)                    \
    ENTRY(Motion_11_Speak)                    \
    ENTRY(Motion_12_Speak)                    \
    ENTRY(Motion_13_Speak)                    \
    ENTRY(Motion_14_Speak)                    \
    ENTRY(Motion_15_Null)                     \
    ENTRY(Motion_16_HoistBegin)               \
    ENTRY(Motion_17_HoistIdle)                \
    ENTRY(Motion_18_HoistLand)                \
    ENTRY(Motion_19_CrouchIdle)               \
    ENTRY(Motion_20_CrouchToStand)            \
    ENTRY(Motion_21_StandToCrouch)            \
    ENTRY(Motion_22_CrouchSpeak)              \
    ENTRY(Motion_23_CrouchSpeak)              \
    ENTRY(Motion_24_RollBegin)                \
    ENTRY(Motion_25_RollLoop)                 \
    ENTRY(Motion_26_RollEnd)                  \
    ENTRY(Motion_27_RunSlideStop)             \
    ENTRY(Motion_28_RunTurn)                  \
    ENTRY(Motion_29_HopBegin)                 \
    ENTRY(Motion_30_HopMid)                   \
    ENTRY(Motion_31_HopLand)                  \
    ENTRY(Motion_32_RunJumpBegin)             \
    ENTRY(Motion_33_RunJumpMid)               \
    ENTRY(Motion_34_RunJumpLand)              \
    ENTRY(Motion_35_RunLoop)                  \
    ENTRY(Motion_36_DunnoBegin)               \
    ENTRY(Motion_37_DunnoMid)                 \
    ENTRY(Motion_38_DunnoEnd)                 \
    ENTRY(Motion_39_CrouchTurn)               \
    ENTRY(Motion_40_RunToRoll)                \
    ENTRY(Motion_41_StandingToRun)            \
    ENTRY(Motion_42_SneakLoop)                \
    ENTRY(Motion_43_WalkToSneak)              \
    ENTRY(Motion_44_SneakToWalk)              \
    ENTRY(Motion_45_MidWalkToSneak)           \
    ENTRY(Motion_46_MidSneakToWalk)           \
    ENTRY(Motion_47_SneakBegin)               \
    ENTRY(Motion_48_SneakToIdle)              \
    ENTRY(Motion_49_MidSneakToIdle)           \
    ENTRY(Motion_50_WalkToRun)                \
    ENTRY(Motion_51_MidWalkToRun)             \
    ENTRY(Motion_52_RunToWalk)                \
    ENTRY(Motion_53_MidRunToWalk)             \
    ENTRY(Motion_54_RunTurnToRun)             \
    ENTRY(Motion_55_RunTurnToWalk)            \
    ENTRY(Motion_56_RunJumpLandRun)           \
    ENTRY(Motion_57_RunJumpLand_Walk)         \
    ENTRY(Motion_58_ToSpeak)                  \
    ENTRY(Motion_59_DeathDropFall)            \
    ENTRY(Motion_60_Dead)                     \
    ENTRY(Motion_61_Respawn)                  \
    ENTRY(Motion_62_LoadedSaveSpawn)          \
    ENTRY(Motion_63_TurnToRun)                \
    ENTRY(Motion_64_LedgeAscend)              \
    ENTRY(Motion_65_LedgeDescend)             \
    ENTRY(Motion_66_LedgeHang)                \
    ENTRY(Motion_67_ToOffScreenHoist)         \
    ENTRY(Motion_68_LedgeHangWobble)          \
    ENTRY(Motion_69_RingRopePullHang)             \
    ENTRY(Motion_70_Knockback)                \
    ENTRY(Motion_71_KnockbackGetUp)           \
    ENTRY(Motion_72_PushWall)                 \
    ENTRY(Motion_73_RollingKnockback)         \
    ENTRY(Motion_74_JumpIntoWell)             \
    ENTRY(Motion_75_ToInsideOfAWellLocal)     \
    ENTRY(Motion_76_ToWellShotOut)            \
    ENTRY(Motion_77_WellBegin)                \
    ENTRY(Motion_78_InsideWellLocal)          \
    ENTRY(Motion_79_WellShotOut)              \
    ENTRY(Motion_80_430EF0)                          \
    ENTRY(Motion_81_InsideWellExpress)        \
    ENTRY(Motion_82_WellExpressShotOut)       \
    ENTRY(Motion_83_430F00)                          \
    ENTRY(Motion_84_ToInsideWellLocal)                          \
    ENTRY(Motion_85_ToWellShotOut)                          \
    ENTRY(Motion_86_FallLandDie)              \
    ENTRY(Motion_87_ToFall)                          \
    ENTRY(Motion_88_HandstoneBegin)           \
    ENTRY(Motion_89_HandstoneEnd)             \
    ENTRY(Motion_90_GrenadeMachineUse)        \
    ENTRY(Motion_91_FallingFromGrab)          \
    ENTRY(Motion_92_ForceDownFromHoist)       \
    ENTRY(Motion_93_WalkOffEdge)              \
    ENTRY(Motion_94_RunOffEdge)               \
    ENTRY(Motion_95_SneakOffEdge)             \
    ENTRY(Motion_96_HopToFall)                \
    ENTRY(Motion_97_RunJumpToFall)            \
    ENTRY(Motion_98_LandSoft)                 \
    ENTRY(Motion_99_HoistBeginLong)             \
    ENTRY(Motion_100_RollOffLedge)            \
    ENTRY(Motion_101_LeverUse)                \
    ENTRY(Motion_102_ElumWalkLoop)            \
    ENTRY(Motion_103_ElumIdle)                \
    ENTRY(Motion_104_ElumRunSlideStop)        \
    ENTRY(Motion_105_ElumRunTurn)             \
    ENTRY(Motion_106_Null)                         \
    ENTRY(Motion_107_ElumHopBegin)            \
    ENTRY(Motion_108_ElumHopMid)              \
    ENTRY(Motion_109_ElumHopLand)             \
    ENTRY(Motion_110_ElumRunJumpBegin)        \
    ENTRY(Motion_111_ElumRunJumpMid)          \
    ENTRY(Motion_112_ElumRunJumpLand)         \
    ENTRY(Motion_113_ElumTurn)                \
    ENTRY(Motion_114_ElumRunLoop)             \
    ENTRY(Motion_115_ElumSpeak)               \
    ENTRY(Motion_116_Null)                         \
    ENTRY(Motion_117_ElumWalkBegin)           \
    ENTRY(Motion_118_ElumRunBegin)            \
    ENTRY(Motion_119_Null)                         \
    ENTRY(Motion_120_ElumRunToWalk)           \
    ENTRY(Motion_121_ElumMidRunToWalk)        \
    ENTRY(Motion_122_ElumRunTurnToRun)        \
    ENTRY(Motion_123_ElumRunTurnToWalk)       \
    ENTRY(Motion_124_ElumWalkEnd)             \
    ENTRY(Motion_125_ElumMidWalkEnd)          \
    ENTRY(Motion_126_ElumBeesStruggling)      \
    ENTRY(Motion_127_SlapBomb)                \
    ENTRY(Motion_128_KnockForward)            \
    ENTRY(Motion_129_RollingKnockForward)     \
    ENTRY(Motion_130_KnockForwardGetUp)       \
    ENTRY(Motion_131_LiftUseUp)               \
    ENTRY(Motion_132_LiftUseDown)             \
    ENTRY(Motion_133_LiftGrabBegin)           \
    ENTRY(Motion_134_LiftGrabEnd)             \
    ENTRY(Motion_135_LiftGrabIdle)            \
    ENTRY(Motion_136_ElumMountEnd)            \
    ENTRY(Motion_137_ElumUnmountBegin)        \
    ENTRY(Motion_138_ElumUnmountEnd)          \
    ENTRY(Motion_139_ElumMountBegin)          \
    ENTRY(Motion_140_BeesStruggling)          \
    ENTRY(Motion_141_BeesStrugglingOnLift)    \
    ENTRY(Motion_142_RockThrowStandingHold)   \
    ENTRY(Motion_143_RockThrowStandingThrow)  \
    ENTRY(Motion_144_RockThrowStandingEnd)    \
    ENTRY(Motion_145_RockThrowCrouchingHold)  \
    ENTRY(Motion_146_RockThrowCrouchingThrow) \
    ENTRY(Motion_147_ShotRolling)             \
    ENTRY(Motion_148_Shot)                    \
    ENTRY(Motion_149_PickupItem)              \
    ENTRY(Motion_150_Chant)                   \
    ENTRY(Motion_151_ChantEnd)                \
    ENTRY(Motion_152_ElumFallOffEdge)         \
    ENTRY(Motion_153_ElumFall)                \
    ENTRY(Motion_154_ElumLand)                \
    ENTRY(Motion_155_ElumJumpToFall)          \
    ENTRY(Motion_156_DoorEnter)               \
    ENTRY(Motion_157_DoorExit)                \
    ENTRY(Motion_158_ElumKnockback)           \
    ENTRY(Motion_159_Idle_RubEyes)            \
    ENTRY(Motion_160_Idle_Stretch_Arms)       \
    ENTRY(Motion_161_Idle_Yawn)               \
    ENTRY(Motion_162_ToShrykull)              \
    ENTRY(Motion_163_ShrykullEnd)             \
    ENTRY(Motion_164_PoisonGasDeath)

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
    eAbeSpawnDir = 0x8000,
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

class Abe final : public BaseAliveGameObject
{
public:
    bool Is_Celling_Above();

    void LoadAnimations();

    Abe();
    ~Abe();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    BirdPortal* VIntoBirdPortal(s16 gridBlocks) override;
    virtual void VOnTrapDoorOpen() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;


    bool CheckForPortalAndRunJump();
    void FreeElumRes_420F80();
    void ToDeathDropFall_42C3D0();
    bool IsStanding_41FC10();
    void FollowLift_42EE90();
    void ExitShrykull_42F440(s16 bResetRingTimer);
    s16 RunTryEnterWell_425880();
    void ChangeChantState_430510(s16 bKeepChanting);
    static IBaseAliveGameObject* FindObjectToPossess_421410();
    void ToDieFinal_42C400();
    void ToKnockback_422D90(s16 bUnknownSound, s16 bDelayedAnger);
    void ToIdle_422D50();
    void MoveForward_422FC0();
    s16 MoveLiftUpOrDown_42F190(FP ySpeed);
    void ElumFree_4228F0();
    s16 DoGameSpeak_42F5C0(u16 input);
    void SyncToElum(s16 elumMotion);
    void PickUpThrowabe_Or_PressBomb_428260(FP fpX, s32 fpY, s16 bStandToCrouch);
    void CrouchingGameSpeak_427F90();
    void FallOnBombs_4231B0();
    s16 ToLeftRightMovement_422AA0();
    void MoveWithVelocity_4257F0(FP speed);
    void ToNewElumSyncMotion(s32 elum_frame);
    void SetActiveControlledCharacter_421480();
    PullRingRope* GetPullRope_422580();
    void ElumKnockForward_42E780(s32 not_used);
    s16 TryMountElum_42E600();
    void BulletDamage_4220B0(Bullet* a2);
    s16 RunTryEnterDoor_4259C0();
    bool NearDoorIsOpen();
    void IntoPortalStates_4262A0();
    void TryHoist_423420();
    s16 HandleDoAction_429A70();

    // Motions
    void Motion_0_Idle();
    void Motion_1_WalkLoop();
    void Motion_2_StandingTurn();
    void Motion_3_Fall();
    void Motion_4_WalkToIdle();
    void Motion_5_MidWalkToIdle();
    void Motion_6_WalkBegin();
    void Motion_7_Speak();
    void Motion_8_Speak();
    void Motion_9_Speak();
    void Motion_10_Speak();
    void Motion_11_Speak();
    void Motion_12_Speak();
    void Motion_13_Speak();
    void Motion_14_Speak();
    void Motion_15_Null();
    void Motion_16_HoistBegin();
    void Motion_17_HoistIdle();
    void Motion_18_HoistLand();
    void Motion_19_CrouchIdle();
    void Motion_20_CrouchToStand();
    void Motion_21_StandToCrouch();
    void Motion_22_CrouchSpeak();
    void Motion_23_CrouchSpeak();
    void Motion_24_RollBegin();
    void Motion_25_RollLoop();
    void Motion_26_RollEnd();
    void Motion_27_RunSlideStop();
    void Motion_28_RunTurn();
    void Motion_29_HopBegin();
    void Motion_30_HopMid();
    void Motion_31_HopLand();
    void Motion_32_RunJumpBegin();
    void Motion_33_RunJumpMid();
    void Motion_34_RunJumpLand();
    void Motion_35_RunLoop();
    void Motion_36_DunnoBegin();
    void Motion_37_DunnoMid();
    void Motion_38_DunnoEnd();
    void Motion_39_CrouchTurn();
    void Motion_40_RunToRoll();
    void Motion_41_StandingToRun();
    void Motion_42_SneakLoop();
    void Motion_43_WalkToSneak();
    void Motion_44_SneakToWalk();
    void Motion_45_MidWalkToSneak();
    void Motion_46_MidSneakToWalk();
    void Motion_47_SneakBegin();
    void Motion_48_SneakToIdle();
    void Motion_49_MidSneakToIdle();
    void Motion_50_WalkToRun();
    void Motion_51_MidWalkToRun();
    void Motion_52_RunToWalk();
    void Motion_53_MidRunToWalk();
    void Motion_54_RunTurnToRun();
    void Motion_55_RunTurnToWalk();
    void Motion_56_RunJumpLandRun();
    void Motion_57_RunJumpLand_Walk();
    void Motion_58_ToSpeak();
    void Motion_59_DeathDropFall();
    void Motion_60_Dead();
    void Motion_61_Respawn();
    void Motion_62_LoadedSaveSpawn();
    void Motion_63_TurnToRun();
    void Motion_64_LedgeAscend();
    void Motion_65_LedgeDescend();
    void Motion_66_LedgeHang();
    void Motion_67_ToOffScreenHoist();
    void Motion_68_LedgeHangWobble();
    void Motion_69_RingRopePullHang();
    void Motion_70_Knockback();
    void Motion_71_KnockbackGetUp();
    void Motion_72_PushWall();
    void Motion_73_RollingKnockback();
    void Motion_74_JumpIntoWell();
    void Motion_75_ToInsideOfAWellLocal();
    void Motion_76_ToWellShotOut();
    void Motion_77_WellBegin();
    void Motion_78_InsideWellLocal();
    void Motion_79_WellShotOut();
    void Motion_80_ToWellBegin();
    void Motion_81_InsideWellExpress();
    void Motion_82_WellExpressShotOut();
    void Motion_83_ToWellBegin();
    void Motion_84_ToInsideWellLocal();
    void Motion_85_ToWellShotOut();
    void Motion_86_FallLandDie();
    void Motion_87_ToFall();
    void Motion_88_HandstoneBegin();
    void Motion_89_HandstoneEnd();
    void Motion_90_GrenadeMachineUse();
    void Motion_91_FallingFromGrab();
    void Motion_92_ForceDownFromHoist();
    void Motion_93_WalkOffEdge();
    void Motion_94_RunOffEdge();
    void Motion_95_SneakOffEdge();
    void Motion_96_HopToFall();
    void Motion_97_RunJumpToFall();
    void Motion_98_LandSoft();
    void Motion_99_HoistBeginLong();
    void Motion_100_RollOffLedge();
    void Motion_101_LeverUse();
    void Motion_102_ElumWalkLoop();
    void Motion_103_ElumIdle();
    void Motion_104_ElumRunSlideStop();
    void Motion_105_ElumRunTurn();
    void Motion_106_Null();
    void Motion_107_ElumHopBegin();
    void Motion_108_ElumHopMid();
    void Motion_109_ElumHopLand();
    void Motion_110_ElumRunJumpBegin();
    void Motion_111_ElumRunJumpMid();
    void Motion_112_ElumRunJumpLand();
    void Motion_113_ElumTurn();
    void Motion_114_ElumRunLoop();
    void Motion_115_ElumSpeak();
    void Motion_116_Null();
    void Motion_117_ElumWalkBegin();
    void Motion_118_ElumRunBegin();
    void Motion_119_Null();
    void Motion_120_ElumRunToWalk();
    void Motion_121_ElumMidRunToWalk();
    void Motion_122_ElumRunTurnToRun();
    void Motion_123_ElumRunTurnToWalk();
    void Motion_124_ElumWalkEnd();
    void Motion_125_ElumMidWalkEnd();
    void Motion_126_ElumBeesStruggling();
    void Motion_127_SlapBomb();
    void Motion_128_KnockForward();
    void Motion_129_RollingKnockForward();
    void Motion_130_KnockForwardGetUp();
    void Motion_131_LiftUseUp();
    void Motion_132_LiftUseDown();
    void Motion_133_LiftGrabBegin();
    void Motion_134_LiftGrabEnd();
    void Motion_135_LiftGrabIdle();
    void Motion_136_ElumMountEnd();
    void Motion_137_ElumUnmountBegin();
    void Motion_138_ElumUnmountEnd();
    void Motion_139_ElumMountBegin();
    void Motion_140_BeesStruggling();
    void Motion_141_BeesStrugglingOnLift();
    void Motion_142_RockThrowStandingHold();
    void Motion_143_RockThrowStandingThrow();
    void Motion_144_RockThrowStandingEnd();
    void Motion_145_RockThrowCrouchingHold();
    void Motion_146_RockThrowCrouchingThrow();
    void Motion_147_ShotRolling();
    void Motion_148_Shot();
    void Motion_149_PickupItem();
    void Motion_150_Chant();
    void Motion_151_ChantEnd();
    void Motion_152_ElumFallOffEdge();
    void Motion_153_ElumFall();
    void Motion_154_ElumLand();
    void Motion_155_ElumJumpToFall();
    void Motion_156_DoorEnter();
    void Motion_157_DoorExit();
    void Motion_158_ElumKnockback();
    void Motion_159_Idle_RubEyes();
    void Motion_160_Idle_Stretch_Arms();
    void Motion_161_Idle_Yawn();
    void Motion_162_ToShrykull();
    void Motion_163_ShrykullEnd();
    void Motion_164_PoisonGasDeath();

    s16 field_10C_prev_held = 0;
    s16 field_10E_released_buttons = 0;
    AllInternalStates field_110_state = {};
    s16 field_112_prev_motion = 0;
    s32 field_114_gnFrame = 0;
    s32 field_118_timer = 0;
    s32 field_11C_regen_health_timer = 0;
    FP field_120_x_vel_slow_by = {};
    s32 field_124_unused = 0;
    s16 field_12A_unused = 0;
    s32 field_12C_timer = 0;
    s16 field_130_say = 0;
    s32 field_134_auto_say_timer = 0;
    PSX_Point mContinueTopLeft = {};
    PSX_Point mContinueBottomRight = {};
    s16 mContinueCamera = 0;
    s16 mContinuePath = 0;
    EReliveLevelIds mContinueLevel = EReliveLevelIds::eNone;
    s16 mContinueZoneNumber = 0;
    s16 mContinueClearFromId = 0;
    s16 mContinueClearToId = 0;
    FP mContinueSpriteScale = {};
    s32 field_150_saved_ring_timer = 0; // todo: check
    s16 field_154_bSavedHaveShrykull = 0;
    DeathFadeOut* field_158_pDeathFadeout = nullptr;
    BaseThrowable* field_15C_pThrowable = nullptr;
    PullRingRope* field_160_pRope = nullptr;
    CircularFade* field_164_pCircularFade = nullptr;
    s32 field_168_ring_pulse_timer = 0;
    s16 field_16C_bHaveShrykull = 0;
    s16 field_16E_cameraIdx = 0;
    ReliveTypes mHandStoneType = {};
    relive::Path_HandStone* mHandStone = nullptr;
    relive::Path_MovieStone* mMovieStone = nullptr;
    relive::Path_BellsongStone* mBellsongStone = nullptr;
    OrbWhirlWind* field_188_pOrbWhirlWind = nullptr;
    IBaseAliveGameObject* field_18C_pObjToPossess = nullptr;
    EReliveLevelIds field_190_level = EReliveLevelIds::eNone;
    s16 field_192_path = 0;
    s16 field_194_camera = 0;
    s16 field_196_door_id = 0;
    BaseThrowable* field_198_pThrowable;
    s8 field_19C_throwable_count;
    s8 field_19D_throw_direction = 0;
    PortalSubStates field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
    BirdPortal* field_1A0_portal = nullptr;
    BitField16<Flags_2A8> field_2A8_flags = {};
    BitField16<Flags_2AA> field_2AA_flags = {};
    SaveData* field_2AC_pSaveData = nullptr;
    bool mRidingElum = false;
};

extern Abe* sActiveHero;
extern s16 gAbeInvulnerableCheat;
extern s16 gAbeInvisibleCheat;

s32 Environment_SFX_42A220(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj);

s32 Mudokon_SFX(MudSounds idx, s32 volume, s32 pitch, BaseAliveGameObject* pHero);

s32 XGrid_Index_To_XPos(FP scale, s32 xGridIndex);

} // namespace AO

extern IBaseAliveGameObject* sControlledCharacter;

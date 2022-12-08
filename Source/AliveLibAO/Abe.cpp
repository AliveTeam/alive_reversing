#include "stdafx_ao.h"
#include "Function.hpp"
#include "Abe.hpp"
#include "BellSong.hpp"
#include "Blood.hpp"
#include "BoomMachine.hpp"
#include "Bullet.hpp"
#include "CheatController.hpp"
#include "DeathBirdParticle.hpp"
#include "Door.hpp"
#include "Dove.hpp"
#include "ThrowableArray.hpp"
#include "Elum.hpp"
#include "../relive_lib/Flash.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "Movie.hpp"
#include "CircularFade.hpp"
#include "Fade.hpp"
#include "Throwable.hpp"
#include "OrbWhirlWind.hpp"
#include "../relive_lib/Particle.hpp"
#include "PossessionFlicker.hpp"
#include "PullRingRope.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "LiftPoint.hpp"
#include "Input.hpp"
#include "PullRingRope.hpp"
#include "ScreenShake.hpp"
#include "Sfx.hpp"
#include "MusicController.hpp"
#include "AbilityRing.hpp"
#include "MusicTrigger.hpp"
#include "Math.hpp"
#include "Input.hpp"
#include "Grenade.hpp"
#include "../relive_lib/Collisions.hpp"
#include "BirdPortal.hpp"
#include "SaveGame.hpp"
#include "BeeSwarm.hpp"
#include "Shrykull.hpp"
#include "Lever.hpp"
#include "GameSpeak.hpp"
#include "ZBall.hpp"
#include "Gibs.hpp"
#include "../relive_lib/Camera.hpp"
#include "AnimationCallBacks.hpp"
#include "Grid.hpp"
#include "../AliveLibAE/Sound/Midi.hpp"
#include "../AliveLibCommon/FatalError.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "Path.hpp"
#include "../relive_lib/data_conversion/PathTlvsAO.hpp"

namespace AO {

s16 gAbeInvulnerableCheat = 0;
s16 gAbeInvisibleCheat = 0;

extern SaveData gSaveBuffer;

using TAbeMotionFunction = decltype(&Abe::Motion_0_Idle);

const TAbeMotionFunction sAbeMotionMachineTable_4C5F08[] = {
    &Abe::Motion_0_Idle,
    &Abe::Motion_1_WalkLoop,
    &Abe::Motion_2_StandingTurn,
    &Abe::Motion_3_Fall,
    &Abe::Motion_4_WalkToIdle,
    &Abe::Motion_5_MidWalkToIdle,
    &Abe::Motion_6_WalkBegin,
    &Abe::Motion_7_Speak,
    &Abe::Motion_8_Speak,
    &Abe::Motion_9_Speak,
    &Abe::Motion_10_Speak,
    &Abe::Motion_11_Speak,
    &Abe::Motion_12_Speak,
    &Abe::Motion_13_Speak,
    &Abe::Motion_14_Speak,
    &Abe::Motion_15_Null,
    &Abe::Motion_16_HoistBegin,
    &Abe::Motion_17_HoistIdle,
    &Abe::Motion_18_HoistLand,
    &Abe::Motion_19_CrouchIdle,
    &Abe::Motion_20_CrouchToStand,
    &Abe::Motion_21_StandToCrouch,
    &Abe::Motion_22_CrouchSpeak,
    &Abe::Motion_23_CrouchSpeak,
    &Abe::Motion_24_RollBegin,
    &Abe::Motion_25_RollLoop,
    &Abe::Motion_26_RollEnd,
    &Abe::Motion_27_RunSlideStop,
    &Abe::Motion_28_RunTurn,
    &Abe::Motion_29_HopBegin,
    &Abe::Motion_30_HopMid,
    &Abe::Motion_31_HopLand,
    &Abe::Motion_32_RunJumpBegin,
    &Abe::Motion_33_RunJumpMid,
    &Abe::Motion_34_RunJumpLand,
    &Abe::Motion_35_RunLoop,
    &Abe::Motion_36_DunnoBegin,
    &Abe::Motion_37_DunnoMid,
    &Abe::Motion_38_DunnoEnd,
    &Abe::Motion_39_CrouchTurn,
    &Abe::Motion_40_RunToRoll,
    &Abe::Motion_41_StandingToRun,
    &Abe::Motion_42_SneakLoop,
    &Abe::Motion_43_WalkToSneak,
    &Abe::Motion_44_SneakToWalk,
    &Abe::Motion_45_MidWalkToSneak,
    &Abe::Motion_46_MidSneakToWalk,
    &Abe::Motion_47_SneakBegin,
    &Abe::Motion_48_SneakToIdle,
    &Abe::Motion_49_MidSneakToIdle,
    &Abe::Motion_50_WalkToRun,
    &Abe::Motion_51_MidWalkToRun,
    &Abe::Motion_52_RunToWalk,
    &Abe::Motion_53_MidRunToWalk,
    &Abe::Motion_54_RunTurnToRun,
    &Abe::Motion_55_RunTurnToWalk,
    &Abe::Motion_56_RunJumpLandRun,
    &Abe::Motion_57_RunJumpLand_Walk,
    &Abe::Motion_58_ToSpeak,
    &Abe::Motion_59_DeathDropFall,
    &Abe::Motion_60_Dead,
    &Abe::Motion_61_Respawn,
    &Abe::Motion_62_LoadedSaveSpawn,
    &Abe::Motion_63_TurnToRun,
    &Abe::Motion_64_LedgeAscend,
    &Abe::Motion_65_LedgeDescend,
    &Abe::Motion_66_LedgeHang,
    &Abe::Motion_67_ToOffScreenHoist,
    &Abe::Motion_68_LedgeHangWobble,
    &Abe::Motion_69_RingRopePullHang,
    &Abe::Motion_70_Knockback,
    &Abe::Motion_71_KnockbackGetUp,
    &Abe::Motion_72_PushWall,
    &Abe::Motion_73_RollingKnockback,
    &Abe::Motion_74_JumpIntoWell,
    &Abe::Motion_75_ToInsideOfAWellLocal,
    &Abe::Motion_76_ToWellShotOut,
    &Abe::Motion_77_WellBegin,
    &Abe::Motion_78_InsideWellLocal,
    &Abe::Motion_79_WellShotOut,
    &Abe::Motion_80_ToWellBegin,
    &Abe::Motion_81_InsideWellExpress,
    &Abe::Motion_82_WellExpressShotOut,
    &Abe::Motion_83_ToWellBegin,
    &Abe::Motion_84_ToInsideWellLocal,
    &Abe::Motion_85_ToWellShotOut,
    &Abe::Motion_86_FallLandDie,
    &Abe::Motion_87_ToFall,
    &Abe::Motion_88_HandstoneBegin,
    &Abe::Motion_89_HandstoneEnd,
    &Abe::Motion_90_GrenadeMachineUse,
    &Abe::Motion_91_FallingFromGrab,
    &Abe::Motion_92_ForceDownFromHoist,
    &Abe::Motion_93_WalkOffEdge,
    &Abe::Motion_94_RunOffEdge,
    &Abe::Motion_95_SneakOffEdge,
    &Abe::Motion_96_HopToFall,
    &Abe::Motion_97_RunJumpToFall,
    &Abe::Motion_98_LandSoft,
    &Abe::Motion_99_HoistBeginLong,
    &Abe::Motion_100_RollOffLedge,
    &Abe::Motion_101_LeverUse,
    &Abe::Motion_102_ElumWalkLoop,
    &Abe::Motion_103_ElumIdle,
    &Abe::Motion_104_ElumRunSlideStop,
    &Abe::Motion_105_ElumRunTurn,
    &Abe::Motion_106_Null,
    &Abe::Motion_107_ElumHopBegin,
    &Abe::Motion_108_ElumHopMid,
    &Abe::Motion_109_ElumHopLand,
    &Abe::Motion_110_ElumRunJumpBegin,
    &Abe::Motion_111_ElumRunJumpMid,
    &Abe::Motion_112_ElumRunJumpLand,
    &Abe::Motion_113_ElumTurn,
    &Abe::Motion_114_ElumRunLoop,
    &Abe::Motion_115_ElumSpeak,
    &Abe::Motion_116_Null,
    &Abe::Motion_117_ElumWalkBegin,
    &Abe::Motion_118_ElumRunBegin,
    &Abe::Motion_119_Null,
    &Abe::Motion_120_ElumRunToWalk,
    &Abe::Motion_121_ElumMidRunToWalk,
    &Abe::Motion_122_ElumRunTurnToRun,
    &Abe::Motion_123_ElumRunTurnToWalk,
    &Abe::Motion_124_ElumWalkEnd,
    &Abe::Motion_125_ElumMidWalkEnd,
    &Abe::Motion_126_ElumBeesStruggling,
    &Abe::Motion_127_SlapBomb,
    &Abe::Motion_128_KnockForward,
    &Abe::Motion_129_RollingKnockForward,
    &Abe::Motion_130_KnockForwardGetUp,
    &Abe::Motion_131_LiftUseUp,
    &Abe::Motion_132_LiftUseDown,
    &Abe::Motion_133_LiftGrabBegin,
    &Abe::Motion_134_LiftGrabEnd,
    &Abe::Motion_135_LiftGrabIdle,
    &Abe::Motion_136_ElumMountEnd,
    &Abe::Motion_137_ElumUnmountBegin,
    &Abe::Motion_138_ElumUnmountEnd,
    &Abe::Motion_139_ElumMountBegin,
    &Abe::Motion_140_BeesStruggling,
    &Abe::Motion_141_BeesStrugglingOnLift,
    &Abe::Motion_142_RockThrowStandingHold,
    &Abe::Motion_143_RockThrowStandingThrow,
    &Abe::Motion_144_RockThrowStandingEnd,
    &Abe::Motion_145_RockThrowCrouchingHold,
    &Abe::Motion_146_RockThrowCrouchingThrow,
    &Abe::Motion_147_ShotRolling,
    &Abe::Motion_148_Shot,
    &Abe::Motion_149_PickupItem,
    &Abe::Motion_150_Chant,
    &Abe::Motion_151_ChantEnd,
    &Abe::Motion_152_ElumFallOffEdge,
    &Abe::Motion_153_ElumFall,
    &Abe::Motion_154_ElumLand,
    &Abe::Motion_155_ElumJumpToFall,
    &Abe::Motion_156_DoorEnter,
    &Abe::Motion_157_DoorExit,
    &Abe::Motion_158_ElumKnockback,
    &Abe::Motion_159_Idle_RubEyes,
    &Abe::Motion_160_Idle_Stretch_Arms,
    &Abe::Motion_161_Idle_Yawn,
    &Abe::Motion_162_ToShrykull,
    &Abe::Motion_163_ShrykullEnd,
    &Abe::Motion_164_PoisonGasDeath};

const AnimId sAbeMotionAnimIds[166] = {
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Walk,
    AnimId::Mudokon_StandingTurn,
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_WalkToIdle,
    AnimId::Mudokon_WalkToIdle,
    AnimId::Mudokon_WalkBegin,
    AnimId::Mudokon_Speak1,
    AnimId::Mudokon_Speak3,
    AnimId::Mudokon_Speak2,
    AnimId::Mudokon_SpeakFart,
    AnimId::Mudokon_Speak1,
    AnimId::Mudokon_Speak2,
    AnimId::Mudokon_Speak3,
    AnimId::Mudokon_SpeakFart,
    AnimId::Mudokon_AO_M_15_Null,
    AnimId::Mudokon_HoistBegin,
    AnimId::Mudokon_HoistIdle,
    AnimId::Mudokon_LandSoft,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchToStand,
    AnimId::Mudokon_StandToCrouch,
    AnimId::Mudokon_CrouchSpeak1,
    AnimId::Mudokon_CrouchSpeak2,
    AnimId::Mudokon_RollBegin,
    AnimId::Mudokon_Rolling,
    AnimId::Mudokon_RollingEnd_Unused,
    AnimId::Mudokon_RunSlideStop,
    AnimId::Mudokon_RunSlideTurn,
    AnimId::Mudokon_HopBegin,
    AnimId::Mudokon_HopMid,
    AnimId::Mudokon_HopLand,
    AnimId::Mudokon_RunJumpBegin,
    AnimId::Mudokon_RunJumpMid,
    AnimId::Mudokon_RunJumpLand,
    AnimId::Mudokon_Run,
    AnimId::Mudokon_DunnoBegin,
    AnimId::Mudokon_DunnoMid,
    AnimId::Mudokon_DunnoEnd,
    AnimId::Mudokon_CrouchTurn,
    AnimId::Mudokon_RunToRoll,
    AnimId::Mudokon_StandToRun,
    AnimId::Mudokon_Sneak,
    AnimId::Mudokon_WalkToSneak,
    AnimId::Mudokon_SneakToWalk,
    AnimId::Mudokon_MidWalkToSneak,
    AnimId::Mudokon_MidSneakToWalk,
    AnimId::Mudokon_SneakBegin,
    AnimId::Mudokon_SneakToIdle,
    AnimId::Mudokon_MidSneakToIdle,
    AnimId::Mudokon_WalkToRun,
    AnimId::Mudokon_MidWalkToRun,
    AnimId::Mudokon_RunToWalk,
    AnimId::Mudokon_MidRunToWalk,
    AnimId::Mudokon_RunTurnToRun,
    AnimId::Mudokon_RunTurnToWalk,
    AnimId::Mudokon_RunJumpLandRun,
    AnimId::Mudokon_RunJumpLandWalk,
    AnimId::Mudokon_ToSpeak,
    AnimId::Mudokon_CrouchIdle, // double check these
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_TurnToRun,
    AnimId::Mudokon_LedgeAscend,
    AnimId::Mudokon_LedgeDescend,
    AnimId::Mudokon_LedgeHang,
    AnimId::Mudokon_LedgeHang,
    AnimId::Mudokon_LedgeHangWobble,
    AnimId::Mudokon_RingRopePullHang,
    AnimId::Mudokon_Knockback,
    AnimId::Mudokon_KnockbackGetUp,
    AnimId::Mudokon_PushWall,
    AnimId::Mudokon_RollingKnockback,
    AnimId::Mudokon_JumpIntoWell,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_WellBegin,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_WellBegin,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_WellBegin,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_Well_Idle,
    AnimId::Mudokon_FallLandDie,
    AnimId::Mudokon_Fall,
    AnimId::Mudokon_HandstoneBegin, 
    AnimId::Mudokon_HandstoneEnd,
    AnimId::Mudokon_GrenadeMachineUse,
    AnimId::Mudokon_FallingFromGrab,
    AnimId::Mudokon_FallingFromGrab,
    AnimId::Mudokon_WalkOffEdge,
    AnimId::Mudokon_RunOffEdge,
    AnimId::Mudokon_SneakOffEdge,
    AnimId::Mudokon_HopToFall,
    AnimId::Mudokon_RunJumpToFall,
    AnimId::Mudokon_AO_LandSoft_Long,
    AnimId::Mudokon_AO_HoistBegin_Long,
    AnimId::Mudokon_RollOffEdge,
    AnimId::Mudokon_LeverUse,
    AnimId::Mudokon_ElumWalkLoop,
    AnimId::Mudokon_ElumIdle,
    AnimId::Mudokon_ElumRunSlideStop,
    AnimId::Mudokon_ElumRunTurn,
    AnimId::Mudokon_AO_M_106_Null,
    AnimId::Mudokon_ElumHopBegin,
    AnimId::Mudokon_ElumHopMid,
    AnimId::Mudokon_ElumHopLand,
    AnimId::Mudokon_ElumRunJumpBegin,
    AnimId::Mudokon_ElumRunJumpMid,
    AnimId::Mudokon_ElumRunJumpLand,
    AnimId::Mudokon_ElumTurn,
    AnimId::Mudokon_ElumRunLoop,
    AnimId::Mudokon_ElumSpeak,
    AnimId::Mudokon_AO_M_116_Null,
    AnimId::Mudokon_ElumWalkBegin,
    AnimId::Mudokon_ElumRunBegin,
    AnimId::Mudokon_AO_M_119_Null,
    AnimId::Mudokon_ElumRunToWalk,
    AnimId::Mudokon_ElumMidRunToWalk,
    AnimId::Mudokon_ElumRunTurnToRun,
    AnimId::Mudokon_ElumRunTurnToWalk,
    AnimId::Mudokon_ElumWalkEnd,
    AnimId::Mudokon_ElumMidWalkEnd,
    AnimId::Mudokon_ElumBeesStruggling,
    AnimId::Mudokon_SlapBomb,
    AnimId::Mudokon_KnockForward,
    AnimId::Mudokon_RollingKnockForward,
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_LiftUseUp,
    AnimId::Mudokon_LiftUseDown,
    AnimId::Mudokon_LiftGrabBegin,
    AnimId::Mudokon_LiftGrabEnd,
    AnimId::Mudokon_LiftGrabIdle,
    AnimId::Mudokon_ElumMountEnd,
    AnimId::Mudokon_ElumUnmountBegin,
    AnimId::Mudokon_ElumUnmountEnd,
    AnimId::Mudokon_ElumMountBegin,
    AnimId::Mudokon_Struggle,
    AnimId::Mudokon_Struggle,
    AnimId::Mudokon_RockThrowStandingHold,
    AnimId::Mudokon_RockThrowStandingThrow,
    AnimId::Mudokon_RockThrowStandingEnd,
    AnimId::Mudokon_RockThrowCrouchingHold,
    AnimId::Mudokon_RockThrowCrouchingThrow,
    AnimId::Mudokon_ZShotRolling,
    AnimId::Mudokon_ZShot,
    AnimId::Mudokon_PickupItem,
    AnimId::Mudokon_Chant,
    AnimId::Mudokon_ChantEnd,
    AnimId::Mudokon_ElumFallOffEdge,
    AnimId::Mudokon_ElumFall,
    AnimId::Mudokon_ElumLand,
    AnimId::Mudokon_ElumJumpToFall,
    AnimId::Mudokon_DoorEnter,
    AnimId::Mudokon_DoorExit,
    AnimId::Mudokon_ElumKnockback,
    AnimId::Mudokon_Idle_RubEyes,
    AnimId::Mudokon_Idle_StretchArms,
    AnimId::Mudokon_Idle_Yawn,
    AnimId::Mudokon_ToShrykull,
    AnimId::Mudokon_EndShrykull,
    AnimId::Mudokon_PoisonGasDeath,
    AnimId::None};

Abe* sActiveHero = nullptr;

const relive::SfxDefinition sSFXList_4C6638[49] = {
    {0, 3, 69, 60, -1, 1},
    {0, 3, 70, 60, -1, 1},
    {0, 3, 59, 67, -1, 1},
    {0, 3, 61, 67, -1, 1},
    {0, 3, 58, 52, -1, 1},
    {0, 3, 67, 45, -1, 1},
    {0, 3, 57, 60, -1, 1},
    {0, 3, 65, 50, -1, 1},
    {0, 3, 68, 60, -1, 1},
    {0, 3, 72, 70, -1, 1},
    {0, 4, 36, 50, -127, 0},
    {0, 4, 37, 50, -127, 0},
    {0, 4, 38, 50, -127, 0},
    {0, 4, 36, 55, 0, 127},
    {0, 4, 37, 55, 0, 127},
    {0, 4, 38, 55, 0, 127},
    {0, 4, 59, 50, -1, 1},
    {0, 4, 72, 127, -1, 1},
    {0, 4, 80, 127, -1, 1},
    {0, 11, 61, 60, -1, 0},
    {0, 11, 62, 60, -1, 1},
    {0, 11, 64, 60, -1, 1},
    {0, 11, 65, 50, -1, 1},
    {0, 11, 64, 50, -1, 1},
    {0, 11, 66, 50, -1, 1},
    {0, 23, 60, 90, 0, 0},
    {0, 23, 61, 90, 0, 0},
    {0, 23, 62, 90, 0, 0},
    {0, 23, 63, 90, 0, 0},
    {0, 23, 64, 90, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 58, 61, 97, 0, 0},
    {0, 58, 60, 97, 0, 0},
    {0, 59, 60, 127, 0, 0},
    {0, 59, 61, 127, 0, 0},
    {0, 59, 62, 127, 0, 0},
    {0, 59, 63, 110, 0, 0},
    {0, 60, 61, 127, 0, 0},
    {0, 60, 63, 127, 0, 0},
    {0, 61, 55, 127, 0, 0},
    {0, 61, 56, 127, 0, 0},
    {0, 61, 57, 127, 0, 0},
    {0, 61, 58, 127, 0, 0},
    {0, 61, 59, 127, 0, 0},
    {0, 61, 60, 127, 0, 0},
    {0, 61, 61, 127, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 65, 60, 127, 0, 0},
    {0, 65, 64, 80, 0, 0}};

s32 Environment_SFX_42A220(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj)
{
    s16 sndIndex = 0;
    switch (sfxId)
    {
        case EnvironmentSfx::eSlideStop_0:
            sndIndex = 11;
            volume = 40;
            break;
        case EnvironmentSfx::eWalkingFootstep_1:
            sndIndex = Math_RandomRange(10, 12);
            if (volume == 0)
            {
                volume = Math_RandomRange(54, 58);
            }
            break;
        case EnvironmentSfx::eRunningFootstep_2:
            sndIndex = Math_RandomRange(13, 15);
            if (volume == 0)
            {
                volume = Math_RandomRange(66, 70);
            }
            break;
        case EnvironmentSfx::eSneakFootstep_3:
            sndIndex = 5;
            break;
        case EnvironmentSfx::eRunSlide_4:
            sndIndex = 4;
            break;
        case EnvironmentSfx::eLandingSoft_5:
        {
            s32 v4 = 0;
            s32 v5 = 0;
            if (volume > 0 || !pAliveObj || pAliveObj->GetSpriteScale() != FP_FromDouble(0.5))
            {
                v4 = SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[2], volume, pitchMin, 0x7FFF);
                v5 = SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[48], volume, pitchMin, 0x7FFF);
            }
            else
            {
                v4 = SFX_SfxDefinition_Play_Mono(
                    sSFXList_4C6638[2],
                    sSFXList_4C6638[2].field_C_default_volume / 2,
                    pitchMin,
                    0x7FFF);
                v5 = SFX_SfxDefinition_Play_Mono(
                    sSFXList_4C6638[48],
                    sSFXList_4C6638[18].field_C_default_volume / 2,
                    pitchMin,
                    0x7FFF);
            }
            return v5 | v4;
        }
        case EnvironmentSfx::eHitGroundSoft_6:
            sndIndex = 2;
            break;
        case EnvironmentSfx::eDeathNoise_7:
            sndIndex = 16;
            break;
        case EnvironmentSfx::eRollingNoise_8:
            sndIndex = 6;
            break;
        case EnvironmentSfx::eGenericMovement_9:
        case EnvironmentSfx::eRunJumpOrLedgeHoist_11:
        {
            s32 result = 0;
            if (pAliveObj && pAliveObj->GetSpriteScale() == FP_FromDouble(0.5))
            {
                result = SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 20);
            }
            else
            {
                result = SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 35);
            }
            return result;
        }
        case EnvironmentSfx::eExhaustingHoistNoise_10:
        case EnvironmentSfx::eExhaustingElumMount_16:
            sndIndex = 19;
            break;
        case EnvironmentSfx::eUnknown_12:
            sndIndex = 25;
            break;
        case EnvironmentSfx::eKnockback_13:
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom
                || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                sndIndex = 2;
            }
            else
            {
                sndIndex = 9;
            }
            break;
        case EnvironmentSfx::eElumHitWall_14:
            sndIndex = 17;
            break;
        case EnvironmentSfx::eFallingDeathScreamHitGround_15:
            return 0;
        case EnvironmentSfx::eMountElumSmackNoise_17:
            sndIndex = 20;
            break;
        case EnvironmentSfx::eElumGetMountedNoise_18:
            sndIndex = 21;
            break;
        case EnvironmentSfx::eAbeMountedElumNoise_19:
            sndIndex = 22;
            break;
        case EnvironmentSfx::eElumHowl_20:
            sndIndex = 23;
            break;
        case EnvironmentSfx::eElumOkay_21:
            sndIndex = 24;
            break;
        default:
            break;
    }
    if (volume == 0)
    {
        volume = sSFXList_4C6638[sndIndex].field_C_default_volume;
    }
    if (pAliveObj)
    {
        if (pAliveObj->GetSpriteScale() == FP_FromDouble(0.5))
        {
            volume *= 2;
            volume /= 3;
        }
    }
    return SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[sndIndex], volume, pitchMin, 0x7FFF);
}

s32 Mudokon_SFX(MudSounds idx, s32 volume, s32 pitch, BaseAliveGameObject* pHero)
{
    if (idx == MudSounds::eLaugh1_8
        && pHero == sActiveHero
        && (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom))
    {
        idx = MudSounds::eLaugh2_11;
    }
    if (idx == MudSounds::eOops_16)
    {
        if (pHero && pHero->GetSpriteScale() == FP_FromDouble(0.5))
        {
            SND_SEQ_Play_477760(SeqId::eAbeOops_23, 1, 85, 85);
            return 0;
        }
        else
        {
            SND_SEQ_Play_477760(SeqId::eAbeOops_23, 1, 110, 110);
            return 0;
        }
    }
    else
    {
        auto idxShort = static_cast<s16>(idx);
        if (volume == 0)
        {
            volume = sSFXList_4C6638[idxShort + 30].field_C_default_volume;
        }
        if (pHero)
        {
            if (pHero->GetSpriteScale() == FP_FromDouble(0.5))
            {
                volume *= 2;
                volume /= 3;
            }
        }
        return SFX_SfxDefinition_Play_Mono(sSFXList_4C6638[idxShort + 30], volume, pitch, pitch);
    }
}

BirdPortal* Abe::VIntoBirdPortal(s16 gridBlocks)
{
    auto pPortal = BaseAliveGameObject::VIntoBirdPortal(gridBlocks);
    if (pPortal && pPortal->mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
    {
        return pPortal;
    }
    return nullptr;
}


void Abe::VOnTrapDoorOpen()
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
        {
            VSetMotion(eAbeMotions::Motion_93_WalkOffEdge);
        }

        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

const TintEntry sAbeTints_4C6438[] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eDesert, 125u, 125u, 95u},
    {EReliveLevelIds::eDesertTemple, 120u, 120u, 90u},
    {EReliveLevelIds::eNone, 102u, 102u, 102u}};


void Abe::LoadAnimations()
{
    for (auto& animId : sAbeMotionAnimIds)
    {
        if (animId != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

Abe::Abe()
{
    SetType(ReliveTypes::eAbe);
    SetSurviveDeathReset(true);
    Init_GameStates();

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mudokon_Walk));

    GetAnimation().SetFnPtrArray(kAbe_Anim_Frame_Fns_4CEBEC);

    PSX_Point pPoint = {};
    gMap.GetCurrentCamCoords(&pPoint);
    mXPos = FP_FromInteger(pPoint.x + XGrid_Index_To_XPos(GetSpriteScale(), 4));
    mYPos = FP_FromInteger(pPoint.y + 240);
    field_120_x_vel_slow_by = FP_FromInteger(0);
    BaseAliveGameObjectLastLineYPos = mYPos;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    BaseAliveGameObjectCollisionLine = nullptr;
    mCurrentMotion = eAbeMotions::Motion_3_Fall;

    field_112_prev_motion = -1;
    GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
    field_15C_pThrowable = nullptr;
    field_19C_throwable_count = 0;
    field_198_pThrowable = nullptr;
    field_1A0_portal = 0;
    mFade = nullptr;
    mCircularFade = nullptr;
    field_188_pOrbWhirlWind = nullptr;
    field_18C_pObjToPossess = nullptr;
    mContinueTopLeft = {};
    mContinueBottomRight = {};
    mContinueCamera = -1;
    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanSetOffExplosives);

    field_2AA_flags.Clear(Flags_2AA::e2AA_Bit1);
    field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);
    field_2AA_flags.Clear(Flags_2AA::e2AA_Bit3_ElumMountBegin);
    field_2AA_flags.Clear(Flags_2AA::e2AA_Bit4_ElumMountEnd);
    field_2AA_flags.Clear(Flags_2AA::e2AA_Bit5_ElumUnmountBegin);

    mContinueSpriteScale = GetSpriteScale();
    mContinueLevel = gMap.mCurrentLevel;
    mContinuePath = -1;
    mContinueClearFromId = 0;
    mContinueClearToId = 0;
    mContinueZoneNumber = 0;
    field_150_saved_ring_timer = 0;
    field_154_bSavedHaveShrykull = 0;

    field_2A8_flags.Raw().all = 0;
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);

    // Changes Abe's "default" colour depending on the level we are in
    SetTint(sAbeTints_4C6438, gMap.mCurrentLevel);

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);

    field_10C_prev_held = 0; // lowest to base class
    mBaseAliveGameObjectLastAnimFrame = 0;
    field_110_state.raw = 0;
    field_168_ring_pulse_timer = 0;
    field_114_gnFrame = sGnFrame;
    BaseAliveGameObjectPathTLV = nullptr;
    field_160_pRope = nullptr;
    field_130_say = -1;
    field_134_auto_say_timer = 0;
    field_EC_bBeesCanChase = 1;

    // Set Abe to be the current player controlled object
    sControlledCharacter = this;

    CreateShadow();
}

Abe::~Abe()
{
    MusicController::ClearObject(this);

    SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);

    if (mFade)
    {
        mFade->mBaseGameObjectRefCount--;
        mFade->SetDead(true);
        mFade = nullptr;
    }

    if (field_15C_pThrowable)
    {
        field_15C_pThrowable->mBaseGameObjectRefCount--;
        field_15C_pThrowable->SetDead(true);
        field_15C_pThrowable = nullptr;
    }

    if (field_160_pRope)
    {
        field_160_pRope->mBaseGameObjectRefCount--;
        field_160_pRope->SetDead(true);
        field_160_pRope = nullptr;
    }

    if (mCircularFade)
    {
        mCircularFade->mBaseGameObjectRefCount--;
        mCircularFade->SetDead(true);
        mCircularFade = nullptr;
    }

    if (field_188_pOrbWhirlWind)
    {
        field_188_pOrbWhirlWind->mBaseGameObjectRefCount--;
        field_188_pOrbWhirlWind->SetDead(true);
        field_188_pOrbWhirlWind = nullptr;
    }

    if (field_18C_pObjToPossess)
    {
        field_18C_pObjToPossess->mBaseGameObjectRefCount--;
        field_18C_pObjToPossess->SetDead(true);
        field_18C_pObjToPossess = nullptr;
    }

    if (field_198_pThrowable)
    {
        field_198_pThrowable->mBaseGameObjectRefCount--;
        field_198_pThrowable->SetDead(true);
        field_198_pThrowable = nullptr;
    }

    sActiveHero = nullptr;

    // OG fix for going back to menu with DDCheat on and then it crashes reading a deleted pointer
    // ditto for the ending where MeatSaw/Invisible switch screen change/update funcs read it.
    sControlledCharacter = nullptr;
}

const u32 sAbe_xVel_table_4BB118[8] = {262144, 262144, 0, 4294705152, 4294705152, 4294705152, 0, 262144};
const u32 sAbe_yVel_table_4BB138[8] = {0, 4294705152, 4294705152, 4294705152, 0, 262144, 262144, 262144};

void Abe::VUpdate()
{
    if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit1))
    {
        if (gAbeInvulnerableCheat)
        {
            mHealth = FP_FromInteger(1);
        }

        if (!Input_IsChanting())
        {
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit7);
        }

        if (gDDCheat_FlyingEnabled && sControlledCharacter == this)
        {
            VOnTrapDoorOpen();

            const FP old_y = mYPos;
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit6_bShrivel);
            BaseAliveGameObjectLastLineYPos = old_y;
            mCurrentMotion = eAbeMotions::Motion_87_ToFall;
            BaseAliveGameObjectCollisionLine = nullptr;

            if (Input().IsAnyPressed(0xF000u))
            {
                const s32 dir = Input().Dir();
                mVelX = FP_FromRaw(sAbe_xVel_table_4BB118[dir] * 2);
                mVelY = FP_FromRaw(sAbe_yVel_table_4BB138[dir]);

                if (Input().IsAnyPressed(sInputKey_Run))
                {
                    mVelX += FP_FromRaw(sAbe_xVel_table_4BB118[dir]);
                    mVelY += FP_FromRaw(sAbe_yVel_table_4BB138[dir]);
                }

                mXPos += mVelX;
                mYPos += mVelY;

                PSX_Point mapSize = {};
                gMap.Get_map_size(&mapSize);

                if (mXPos < FP_FromInteger(0))
                {
                    mXPos = FP_FromInteger(0);
                }

                if (mXPos >= FP_FromInteger(mapSize.x))
                {
                    mXPos = FP_FromInteger(mapSize.x) - FP_FromInteger(1);
                }

                if (mYPos < FP_FromInteger(0))
                {
                    mYPos = FP_FromInteger(0);
                }

                if (mYPos >= FP_FromInteger(mapSize.y))
                {
                    mYPos = FP_FromInteger(mapSize.y) - FP_FromInteger(1);
                    SetActiveCameraDelayedFromDir();
                    return;
                }
            }
            else
            {
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
            }
            SetActiveCameraDelayedFromDir();
        }
        else
        {
            GetAnimation().SetAnimate(true);


            // Execute the current state
            s16 motion_idx = mCurrentMotion;
            const FP oldX = mXPos;
            const FP oldY = mYPos;

            InvokeMemberFunction(this, sAbeMotionMachineTable_4C5F08, motion_idx);

            if (BaseAliveGameObjectCollisionLine)
            {
                // Snap to a whole number so we are "on" the line
                mYPos = FP_NoFractional(mYPos);
            }

            if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
            {
                // Did position change?
                if (oldX != mXPos || oldY != mYPos || sControlledCharacter == gElum)
                {
                    // Get the TLV we are on
                    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                        nullptr,
                        mXPos,
                        mYPos,
                        mXPos,
                        mYPos);

                    VOnTlvCollision(BaseAliveGameObjectPathTLV);
                }

                if (mbGotShot)
                {
                    motion_idx = field_112_prev_motion;
                    ToKnockback_422D90(1, 0);
                    mCurrentMotion = motion_idx;
                    mNextMotion = 0;
                    field_112_prev_motion = 0;
                    mbMotionChanged = true;
                    mbGotShot = false;
#if ORIGINAL_GAME_FIXES || ORIGINAL_GAME_FIX_DEATH_DELAY_AO
                    field_2A8_flags.Clear(Flags_2A8::e2A8_Bit2_return_to_previous_motion); // OG Change - Fixes "Death Delay Glitch"
#endif
                }

                if (EventGet(kEventScreenShake))
                {
                    if (IsStanding_41FC10())
                    {
                        ToKnockback_422D90(1, 0);
                    }
                }

                if (motion_idx != mCurrentMotion || mbMotionChanged)
                {
                    mbMotionChanged = false;

                    if (mCurrentMotion != eAbeMotions::Motion_15_Null && !field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[mCurrentMotion]));

                        field_12C_timer = sGnFrame;

                        if (motion_idx == eAbeMotions::Motion_15_Null || motion_idx == eAbeMotions::Motion_62_LoadedSaveSpawn)
                        {
                            GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                        }
                    }
                }
                else if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit2_return_to_previous_motion))
                {
                    mCurrentMotion = mPreviousMotion;
                    GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[mCurrentMotion]));

                    field_12C_timer = sGnFrame;
                    GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                    field_2A8_flags.Clear(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
                }

                if (field_11C_regen_health_timer <= static_cast<s32>(sGnFrame) && mHealth > FP_FromInteger(0))
                {
                    mHealth = FP_FromInteger(1);
                }

                if (field_168_ring_pulse_timer)
                {
                    if (GetAnimation().GetRender())
                    {
                        if (static_cast<s32>(sGnFrame) <= field_168_ring_pulse_timer || field_16C_bHaveShrykull)
                        {
                            if (!(sGnFrame % 32))
                            {
                                const PSX_RECT bRect = VGetBoundingRect();
                                AbilityRing::Factory(
                                    FP_FromInteger((bRect.w + bRect.x) / 2),
                                    FP_FromInteger((bRect.h + bRect.y) / 2),
                                    field_16C_bHaveShrykull != 0 ? RingTypes::eShrykull_Pulse_Small_4 : RingTypes::eExplosive_Pulse_0);
                                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 25, 2650);
                            }
                        }
                        else
                        {
                            field_168_ring_pulse_timer = 0;
                        }
                    }
                }

                if (EventGet(kEventMudokonDead))
                {
                    field_130_say = 16;
                    field_134_auto_say_timer = sGnFrame + Math_RandomRange(22, 30);
                    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 90);
                }

                if (field_130_say >= 0 && static_cast<s32>(sGnFrame) >= field_134_auto_say_timer)
                {
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            mXPos,
                            mYPos,
                            0))
                    {
                        if (mCurrentMotion != eAbeMotions::Motion_150_Chant
                            && mCurrentMotion != eAbeMotions::Motion_7_Speak
                            && mCurrentMotion != eAbeMotions::Motion_8_Speak
                            && mCurrentMotion != eAbeMotions::Motion_9_Speak
                            && mCurrentMotion != eAbeMotions::Motion_10_Speak
                            && mCurrentMotion != eAbeMotions::Motion_11_Speak
                            && mCurrentMotion != eAbeMotions::Motion_12_Speak
                            && mCurrentMotion != eAbeMotions::Motion_13_Speak
                            && mCurrentMotion != eAbeMotions::Motion_14_Speak)
                        {
                            if (mCurrentMotion == eAbeMotions::Motion_0_Idle || mCurrentMotion == eAbeMotions::Motion_15_Null)
                            {
                                mbMotionChanged = true;
                                if (field_130_say == 5 || field_130_say == 16)
                                {
                                    mCurrentMotion = eAbeMotions::Motion_10_Speak;
                                }
                                else
                                {
                                    mCurrentMotion = eAbeMotions::Motion_9_Speak;
                                }
                            }
                            if (field_130_say == 5)
                            {
                                EventBroadcast(kEventMudokonComfort, sActiveHero);
                            }
                            Mudokon_SFX(static_cast<MudSounds>(field_130_say), 0, 0, this);
                        }
                    }
                    field_130_say = -1;
                }

                if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone) && field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabaniaDone))
                {
                    if (!field_2A8_flags.Get(Flags_2A8::e2A8_eBit15_bGiveShrykullFromBigFace) && gMap.mNextLevel == EReliveLevelIds::eLines)
                    {
                        LOG_INFO("Set e2A8_eBit15_bGiveShrykullFromBigFace");
                        field_2A8_flags.Set(Flags_2A8::e2A8_eBit15_bGiveShrykullFromBigFace);
                    }
                }

                if (field_2A8_flags.Get(Flags_2A8::e2A8_eBit15_bGiveShrykullFromBigFace))
                {
                    static bool bLogged = false;
                    if (!bLogged)
                    {
                        LOG_INFO("Have e2A8_eBit15_bGiveShrykullFromBigFace %d", static_cast<s32>(gMap.mCurrentLevel));
                        bLogged = true;
                    }

                    if (!field_2A8_flags.Get(Flags_2A8::e2A8_eBit14_bGotShrykullFromBigFace) && gMap.mCurrentLevel == EReliveLevelIds::eLines)
                    {
                        LOG_INFO("Set mHaveShrykull true");
                        field_16C_bHaveShrykull = true;
                        field_168_ring_pulse_timer = sGnFrame + 32000;
                        field_2A8_flags.Set(Flags_2A8::e2A8_eBit14_bGotShrykullFromBigFace);
                    }
                }
            }
        }
    }
}

void Abe::VRender(PrimHeader** ppOt)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!(field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel)))
    {
        GetAnimation().SetSpriteScale(GetSpriteScale());
    }

    if (mCurrentMotion != eAbeMotions::Motion_15_Null && mCurrentMotion != eAbeMotions::Motion_78_InsideWellLocal && mCurrentMotion != eAbeMotions::Motion_81_InsideWellExpress && mCurrentMotion != eAbeMotions::Motion_84_ToInsideWellLocal && mCurrentMotion != eAbeMotions::Motion_75_ToInsideOfAWellLocal)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Abe::FreeElumRes_420F80()
{
    if (mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin)
    {
        field_2AA_flags.Set(Flags_2AA::e2AA_Bit3_ElumMountBegin);
    }
    else
    {
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit3_ElumMountBegin);
    }

    if (mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd)
    {
        field_2AA_flags.Set(Flags_2AA::e2AA_Bit4_ElumMountEnd);
    }
    else
    {
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit4_ElumMountEnd);
    }

    if (mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin)
    {
        field_2AA_flags.Set(Flags_2AA::e2AA_Bit5_ElumUnmountBegin);
    }
    else
    {
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit5_ElumUnmountBegin);
    }

    ElumFree_4228F0();
}

void Abe::ToDeathDropFall_42C3D0()
{
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit6_bShrivel);
    mCurrentMotion = eAbeMotions::Motion_59_DeathDropFall;
    field_114_gnFrame = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 1, 0);
}

bool Abe::IsStanding_41FC10()
{
    return mCurrentMotion == eAbeMotions::Motion_0_Idle
        || mCurrentMotion == eAbeMotions::Motion_1_WalkLoop
        || mCurrentMotion == eAbeMotions::Motion_27_RunSlideStop
        || mCurrentMotion == eAbeMotions::Motion_28_RunTurn
        || mCurrentMotion == eAbeMotions::Motion_35_RunLoop
        || mCurrentMotion == eAbeMotions::Motion_2_StandingTurn
        || mCurrentMotion == eAbeMotions::Motion_36_DunnoBegin
        || mCurrentMotion == eAbeMotions::Motion_37_DunnoMid
        || mCurrentMotion == eAbeMotions::Motion_38_DunnoEnd
        || mCurrentMotion == eAbeMotions::Motion_4_WalkToIdle
        || mCurrentMotion == eAbeMotions::Motion_5_MidWalkToIdle
        || mCurrentMotion == eAbeMotions::Motion_6_WalkBegin
        || mCurrentMotion == eAbeMotions::Motion_41_StandingToRun
        || mCurrentMotion == eAbeMotions::Motion_42_SneakLoop
        || mCurrentMotion == eAbeMotions::Motion_43_WalkToSneak
        || mCurrentMotion == eAbeMotions::Motion_44_SneakToWalk
        || mCurrentMotion == eAbeMotions::Motion_45_MidWalkToSneak
        || mCurrentMotion == eAbeMotions::Motion_46_MidSneakToWalk
        || mCurrentMotion == eAbeMotions::Motion_47_SneakBegin
        || mCurrentMotion == eAbeMotions::Motion_48_SneakToIdle
        || mCurrentMotion == eAbeMotions::Motion_49_MidSneakToIdle
        || mCurrentMotion == eAbeMotions::Motion_50_WalkToRun
        || mCurrentMotion == eAbeMotions::Motion_51_MidWalkToRun
        || mCurrentMotion == eAbeMotions::Motion_52_RunToWalk
        || mCurrentMotion == eAbeMotions::Motion_53_MidRunToWalk
        || mCurrentMotion == eAbeMotions::Motion_54_RunTurnToRun
        || mCurrentMotion == eAbeMotions::Motion_55_RunTurnToWalk
        || mCurrentMotion == eAbeMotions::Motion_58_ToSpeak
        || mCurrentMotion == eAbeMotions::Motion_7_Speak
        || mCurrentMotion == eAbeMotions::Motion_8_Speak
        || mCurrentMotion == eAbeMotions::Motion_9_Speak
        || mCurrentMotion == eAbeMotions::Motion_10_Speak
        || mCurrentMotion == eAbeMotions::Motion_11_Speak
        || mCurrentMotion == eAbeMotions::Motion_12_Speak
        || mCurrentMotion == eAbeMotions::Motion_13_Speak
        || mCurrentMotion == eAbeMotions::Motion_14_Speak
        || mCurrentMotion == eAbeMotions::Motion_101_LeverUse
        || mCurrentMotion == eAbeMotions::Motion_140_BeesStruggling
        || mCurrentMotion == eAbeMotions::Motion_143_RockThrowStandingThrow
        || mCurrentMotion == eAbeMotions::Motion_142_RockThrowStandingHold
        || mCurrentMotion == eAbeMotions::Motion_144_RockThrowStandingEnd
        || mCurrentMotion == eAbeMotions::Motion_150_Chant
        || mCurrentMotion == eAbeMotions::Motion_151_ChantEnd
        || mCurrentMotion == eAbeMotions::Motion_159_Idle_RubEyes
        || mCurrentMotion == eAbeMotions::Motion_160_Idle_Stretch_Arms
        || mCurrentMotion == eAbeMotions::Motion_161_Idle_Yawn
        || mCurrentMotion == eAbeMotions::Motion_164_PoisonGasDeath;
}

void Abe::FollowLift_42EE90()
{
    PlatformBase* pLift = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLift)
    {
        mVelY = pLift->mVelY;
        if (pLift->GetDead())
        {
            pLift->VOnPickUpOrSlapped();
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit1);
        }
        SetActiveCameraDelayedFromDir();
    }
}

void Abe::ExitShrykull_42F440(s16 bResetRingTimer)
{
    GetAnimation().SetAnimate(true);
    GetAnimation().SetRender(true);

    mCurrentMotion = eAbeMotions::Motion_163_ShrykullEnd;

    mbMotionChanged = true;
    field_114_gnFrame = 1;

    if (bResetRingTimer)
    {
        field_168_ring_pulse_timer = 0;
    }
}

s16 Abe::RunTryEnterWell_425880()
{
    if (!Input().IsAnyPressed(sInputKey_Up) || GetAnimation().GetCurrentFrame() < 4)
    {
        return 0;
    }

    auto pWellLocal = static_cast<relive::Path_WellLocal*>(gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal));
    if (pWellLocal)
    {
        if ((pWellLocal->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) ||
            (pWellLocal->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
        {
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_snap_abe);
            BaseAliveGameObjectPathTLV = pWellLocal;
            mCurrentMotion = eAbeMotions::Motion_77_WellBegin;
            return 1;
        }
    }

    auto pWellExpress = static_cast<relive::Path_WellExpress*>(gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellExpress));
    if (pWellExpress)
    {
        if ((pWellExpress->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) ||
            (pWellExpress->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
        {
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_snap_abe);
            BaseAliveGameObjectPathTLV = pWellExpress;
            mCurrentMotion = eAbeMotions::Motion_80_430EF0;
        }
    }

    return 0;
}

void Abe::ChangeChantState_430510(s16 bKeepChanting)
{
    if (bKeepChanting)
    {
        field_110_state.chant = ChantStates::eChantingForBirdPortal_6;
    }
    else
    {
        field_110_state.chant = ChantStates::eIdleChanting_0;
    }
}

IBaseAliveGameObject* Abe::FindObjectToPossess_421410()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetIsBaseAliveGameObject())
        {
            if (pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eCanBePossessed))
            {
                if (pObj->Type() != ReliveTypes::eSlig || (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObj->mHealth > FP_FromInteger(0)))
                {
                    return pObj;
                }
            }
        }
    }
    return nullptr;
}

void Abe::ToDieFinal_42C400()
{
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit6_bShrivel);
    mCurrentMotion = eAbeMotions::Motion_60_Dead;
    field_114_gnFrame = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_14, this, 1, 0);
}

void Abe::ToKnockback_422D90(s16 bKnockbackSound, s16 bDelayedAnger)
{
    if (sControlledCharacter->Type() != ReliveTypes::eSlig || mHealth <= FP_FromInteger(0))
    {
        SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit3_ElumMountBegin);
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit4_ElumMountEnd);
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit5_ElumUnmountBegin);

        if (field_188_pOrbWhirlWind)
        {
            field_188_pOrbWhirlWind->ToStop();
            field_188_pOrbWhirlWind = nullptr;
        }

        if (mVelX != FP_FromInteger(0))
        {
            mXPos -= mVelX;
        }

        MapFollowMe(1);

        mVelX = FP_FromInteger(0);
        if (mVelY < FP_FromInteger(0))
        {
            mVelY = FP_FromInteger(0);
        }

        field_114_gnFrame = sGnFrame + 10;

        if (bKnockbackSound)
        {
            Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, Math_RandomRange(-127, 127), this);
            Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
        }

        if (mRidingElum)
        {
            sControlledCharacter = this;
            BaseAliveGameObjectLastLineYPos = mYPos;
            BaseAliveGameObjectCollisionLine = nullptr;
            mYPos -= (GetSpriteScale() * FP_FromInteger(20));
            VOnTrapDoorOpen();
            FreeElumRes_420F80();
        }

        mCurrentMotion = eAbeMotions::Motion_70_Knockback;

        if (bDelayedAnger)
        {
            field_130_say = 5;
            field_134_auto_say_timer = sGnFrame + 27;
        }

        if (field_198_pThrowable)
        {
            field_198_pThrowable->VToDead();
            field_198_pThrowable = nullptr;
            if (!gInfiniteGrenades)
            {
                field_19C_throwable_count++;
            }
        }
    }
}

void Abe::ToIdle_422D50()
{
    field_120_x_vel_slow_by = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    field_114_gnFrame = sGnFrame;
    mCurrentMotion = eAbeMotions::Motion_0_Idle;
    field_10C_prev_held = 0;
    field_10E_released_buttons = 0;
}

void Abe::MoveForward_422FC0()
{
    FollowLift_42EE90();

    const FP old_x = mXPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);
    }

    auto pTrapdoor = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const CollisionMask mask = GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor;
    if (BaseAliveGameObjectCollisionLine && (mask.Mask() == CollisionMask(BaseAliveGameObjectCollisionLine->mLineType).Mask()))
    {
        if (pTrapdoor)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes ::eDynamicCollision_32 &&
                BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                pTrapdoor->VRemove(this);
                BaseAliveGameObject_PlatformId = Guid{};
            }
        }
        else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PSX_RECT bRect = VGetBoundingRect();
            bRect.y += 5;
            bRect.h += 5;
            bRect.w += 5; // TODO: Seems wrong - same in AE

            OnCollisionWith(
                {bRect.x, bRect.y},
                {bRect.w, bRect.h},
                gPlatformsArray);
        }
    }
    else
    {
        BaseAliveGameObjectCollisionLine = nullptr;

        if (pTrapdoor)
        {
            pTrapdoor->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }

        field_10C_prev_held = 0;
        switch (mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop:
            case eAbeMotions::Motion_6_WalkBegin:
            case eAbeMotions::Motion_4_WalkToIdle:
            case eAbeMotions::Motion_5_MidWalkToIdle:
                mCurrentMotion = eAbeMotions::Motion_93_WalkOffEdge;
                break;

            case eAbeMotions::Motion_35_RunLoop:
            case eAbeMotions::Motion_41_StandingToRun:
                mCurrentMotion = eAbeMotions::Motion_94_RunOffEdge;
                break;

            case eAbeMotions::Motion_40_RunToRoll:
            case eAbeMotions::Motion_24_RollBegin:
            case eAbeMotions::Motion_25_RollLoop:
            case eAbeMotions::Motion_26_RollEnd:
                mCurrentMotion = eAbeMotions::Motion_100_RollOffLedge;
                break;

            default:
                mCurrentMotion = eAbeMotions::Motion_95_SneakOffEdge;
                break;
        }

        mXPos = old_x + mVelX;
        BaseAliveGameObjectLastLineYPos = mYPos;

        // TODO: OG bug, dead code due to switch default case ?
        if (mCurrentMotion == eAbeMotions::Motion_70_Knockback || mCurrentMotion == eAbeMotions::Motion_128_KnockForward)
        {
            field_120_x_vel_slow_by = FP_FromDouble(0.67); // TODO: Check
        }
    }
}

void Abe::ElumFree_4228F0()
{
    if (field_2AA_flags.Get(Flags_2AA::e2AA_Bit4_ElumMountEnd))
    {
        if (gElum->GetCurrentMotion() != eElumMotions::Motion_1_Idle)
        {
            gElum->Vsub_416120();
        }
    }

    if (field_2AA_flags.Get(Flags_2AA::e2AA_Bit5_ElumUnmountBegin))
    {
        if (gElum->GetCurrentMotion() != eElumMotions::Motion_1_Idle)
        {
            gElum->Vsub_416120();
        }
    }
}

s16 Abe::DoGameSpeak_42F5C0(u16 input)
{
    if (Input_IsChanting())
    {
        field_114_gnFrame = sGnFrame + 90;
        SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant1_11, 0, 1);
        field_110_state.chant = ChantStates::eIdleChanting_0;
        return eAbeMotions::Motion_150_Chant;
    }

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Faster check for GameSpeak
    const bool leftGameSpeakPressed = (input & eLeftGamespeak);
    const bool rightGameSpeakPressed = (input & eRightGameSpeak);
#else
    const bool leftGameSpeakPressed = Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler);
    const bool rightGameSpeakPressed = Input().IsAnyPressed(sInputKey_RightGameSpeakEnabler);
#endif

    if (leftGameSpeakPressed)
    {
        if (input & sInputKey_GameSpeak2)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eFollowMe_10);
            Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_14_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_14_Speak;
        }
        if (input & sInputKey_GameSpeak4)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eWait_12);
            Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_14_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_13_Speak;
        }
        if (input & sInputKey_GameSpeak1)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eHello_9);
            Mudokon_SFX(MudSounds::eHello_3, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_9_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_9_Speak;
        }
        if (input & sInputKey_GameSpeak3)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eAnger_11);
            Mudokon_SFX(MudSounds::eAngry_5, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_10_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_10_Speak;
        }
    }

    if (rightGameSpeakPressed)
    {
        if (input & sInputKey_GameSpeak6)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eWhistleHigh_1);
            Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_9_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_9_Speak;
        }
        if (input & sInputKey_GameSpeak5)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eWhistleLow_2);
            Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_8_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_8_Speak;
        }
        if (input & sInputKey_GameSpeak8)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eLaugh_4);
            Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            if (mCurrentMotion == eAbeMotions::Motion_12_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_12_Speak;
        }
        if (input & sInputKey_GameSpeak7)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eFart_3);
            Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
            if (gEnableFartGasCheat)
            {
                FP xPos = mXPos;
                if (GetAnimation().GetFlipX())
                {
                    xPos += (FP_FromInteger(12) * GetSpriteScale());
                }
                else
                {
                    xPos -= (FP_FromInteger(12) * GetSpriteScale());
                }
                New_Smoke_Particles(
                    xPos,
                    mYPos - (FP_FromInteger(24) * GetSpriteScale()),
                    FP_FromDouble(0.5) * GetSpriteScale(),
                    3,
                    RGB16{ 32, 128, 32 });
            }
            field_130_say = 8;
            field_134_auto_say_timer = sGnFrame + 15;
            if (mCurrentMotion == eAbeMotions::Motion_10_Speak)
            {
                mbMotionChanged = true;
            }
            return eAbeMotions::Motion_10_Speak;
        }
    }
    return -1;
}

void Abe::SyncToElum(s16 elumMotion)
{
    if (mCurrentMotion != eAbeMotions::Motion_137_ElumUnmountBegin && mCurrentMotion != eAbeMotions::Motion_138_ElumUnmountEnd)
    {
        switch (static_cast<eElumMotions>(elumMotion))
        {
            case eElumMotions::Motion_1_Idle:
                mCurrentMotion = eAbeMotions::Motion_103_ElumIdle;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_3_WalkLoop:
                mCurrentMotion = eAbeMotions::Motion_102_ElumWalkLoop;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_4_Turn:
                mCurrentMotion = eAbeMotions::Motion_113_ElumTurn;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_5_WalkToIdle:
                mCurrentMotion = eAbeMotions::Motion_124_ElumWalkEnd;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_6_MidWalkToIdle:
                mCurrentMotion = eAbeMotions::Motion_125_ElumMidWalkEnd;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_7_IdleToWalk1_413200:
                mCurrentMotion = eAbeMotions::Motion_116_Null;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_8_IdleToWalk2_413270:
                mCurrentMotion = eAbeMotions::Motion_117_ElumWalkBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_12_RunTurn:
                mCurrentMotion = eAbeMotions::Motion_105_ElumRunTurn;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_13_RunTurnToWalk:
                mCurrentMotion = eAbeMotions::Motion_123_ElumRunTurnToWalk;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_21_Land:
                mCurrentMotion = eAbeMotions::Motion_153_ElumFall;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_22_RunOffEdge:
                mCurrentMotion = eAbeMotions::Motion_154_ElumLand;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_23_WalkOffEdge:
                mCurrentMotion = eAbeMotions::Motion_152_ElumFallOffEdge;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_24_JumpToFall:
                mCurrentMotion = eAbeMotions::Motion_155_ElumJumpToFall;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_30_HopBegin:
                mCurrentMotion = eAbeMotions::Motion_107_ElumHopBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_31_HopMid:
                mCurrentMotion = eAbeMotions::Motion_108_ElumHopMid;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_32_HopLand:
                mCurrentMotion = eAbeMotions::Motion_109_ElumHopLand;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_33_RunJumpBegin:
                mCurrentMotion = eAbeMotions::Motion_110_ElumRunJumpBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = nullptr;
                break;

            case eElumMotions::Motion_34_RunJumpMid:
                mCurrentMotion = eAbeMotions::Motion_111_ElumRunJumpMid;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_35_RunJumpLand:
                mCurrentMotion = eAbeMotions::Motion_112_ElumRunJumpLand;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_36_RunLoop:
                mCurrentMotion = eAbeMotions::Motion_114_ElumRunLoop;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;
                break;

            case eElumMotions::Motion_37_RunSlideStop:
                mCurrentMotion = eAbeMotions::Motion_104_ElumRunSlideStop;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_38_RunTurnToRun:
                mCurrentMotion = eAbeMotions::Motion_122_ElumRunTurnToRun;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_39_IdleToRun:
                mCurrentMotion = eAbeMotions::Motion_118_ElumRunBegin;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_40_WalkToRun:
                mCurrentMotion = eAbeMotions::Motion_119_Null;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_41_MidWalkToRun:
                mCurrentMotion = eAbeMotions::Motion_120_ElumRunToWalk;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_43_MidRunToWalk:
                mCurrentMotion = eAbeMotions::Motion_121_ElumMidRunToWalk;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            case eElumMotions::Motion_50_Knockback:
                mCurrentMotion = eAbeMotions::Motion_158_ElumKnockback;
                ToNewElumSyncMotion(gElum->GetAnimation().GetCurrentFrame());
                break;

            default:
                break;
        }
    }
}

void Abe::PickUpThrowabe_Or_PressBomb_428260(FP fpX, s32 fpY, s16 bStandToCrouch)
{
    BaseAliveGameObject* pSlapableOrCollectable = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetInteractive())
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->mCollectionRect.x && fpX <= pAliveObj->mCollectionRect.w)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->mCollectionRect.y && yPos <= pAliveObj->mCollectionRect.h)
                {
                    pSlapableOrCollectable = pAliveObj;
                    pSlapableOrCollectable->mBaseGameObjectRefCount++;
                    field_15C_pThrowable = pSlapableOrCollectable;
                    break;
                }
            }
        }
    }


    if (pSlapableOrCollectable)
    {
        bool tryToSlapOrCollect = false;

        switch (pSlapableOrCollectable->Type())
        {
            case ReliveTypes::eTimedMine:
            case ReliveTypes::eUXB:
                mCurrentMotion = eAbeMotions::Motion_127_SlapBomb;
                if (bStandToCrouch)
                {
                    field_15C_pThrowable->mBaseGameObjectRefCount--;
                    field_15C_pThrowable = nullptr;
                }
                tryToSlapOrCollect = true;
                break;

            case ReliveTypes::eGrenade:
            case ReliveTypes::eMeat:
            case ReliveTypes::eRock:
                mCurrentMotion = eAbeMotions::Motion_149_PickupItem;
                field_19C_throwable_count += static_cast<s8>(static_cast<BaseThrowable*>(field_15C_pThrowable)->VGetCount());

                if (!bThrowableIndicatorExists_504C70)
                {
                    const FP v16 = (GetSpriteScale() * FP_FromInteger(-30)) + mYPos;
                    relive_new ThrowableTotalIndicator(
                                                                          (GetSpriteScale() * FP_FromInteger(0)) + mXPos,
                                                                               v16,
                                                                               GetAnimation().GetRenderLayer(),
                                                                               GetAnimation().GetSpriteScale(),
                                                                               field_19C_throwable_count,
                                                                               1);
                }
                tryToSlapOrCollect = true;
                break;

            case ReliveTypes::eMine:
                field_15C_pThrowable->mBaseGameObjectRefCount--;
                field_15C_pThrowable = nullptr;
                tryToSlapOrCollect = true;
                break;

            default:
                return;
        }

        if (tryToSlapOrCollect)
        {
            if (mCurrentMotion == eAbeMotions::Motion_149_PickupItem)
            {
                if (bStandToCrouch)
                {
                    SfxPlayMono(relive::SoundEffects::PickupItem, 0, GetSpriteScale());
                    field_15C_pThrowable->mBaseGameObjectRefCount--;
                    field_15C_pThrowable->VOnPickUpOrSlapped();
                    field_15C_pThrowable = nullptr;
                    mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
                }
            }
        }
    }
}

void Abe::CrouchingGameSpeak_427F90()
{
    field_10C_prev_held |= Input().Held();

    if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler))
    {
        if (field_10C_prev_held & sInputKey_GameSpeak2)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eFollowMe_10);
            Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
        }
        else if (field_10C_prev_held & sInputKey_GameSpeak4)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eWait_12);
            Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
        else if (field_10C_prev_held & sInputKey_GameSpeak1)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eHello_9);
            Mudokon_SFX(MudSounds::eHello_3, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
        }
        else if (field_10C_prev_held & sInputKey_GameSpeak3)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eAnger_11);
            Mudokon_SFX(MudSounds::eAngry_5, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
    }
    else if (Input().IsAnyPressed(sInputKey_RightGameSpeakEnabler))
    {
        if (field_10C_prev_held & sInputKey_GameSpeak6)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eWhistleHigh_1);
            Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
        else if (field_10C_prev_held & sInputKey_GameSpeak5)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eWhistleLow_2);
            Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
        }
        else if (field_10C_prev_held & sInputKey_GameSpeak8)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eLaugh_4);
            Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        }
        else if (field_10C_prev_held & sInputKey_GameSpeak7)
        {
            gEventSystem->VPushEvent(GameSpeakEvents::eFart_3);
            Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
            if (gEnableFartGasCheat)
            {
                FP xPos = {};
                if (GetAnimation().GetFlipX())
                {
                    xPos = mXPos + FP_FromDouble(10) * GetSpriteScale();
                }
                else
                {
                    xPos = mXPos - FP_FromDouble(10) * GetSpriteScale();
                }
                New_Smoke_Particles(
                    xPos,
                    mYPos - FP_FromDouble(6) * GetSpriteScale(),
                    GetSpriteScale() * FP_FromDouble(0.5),
                    3,
                    RGB16{ 32, 128, 32 });
            }
            field_130_say = 8;
            mCurrentMotion = eAbeMotions::Motion_22_CrouchSpeak;
            field_134_auto_say_timer = sGnFrame + 15;
        }
    }
}

void Abe::FallOnBombs_4231B0()
{
    const PSX_RECT bOurRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->Type() == ReliveTypes::eMine || pObjIter->Type() == ReliveTypes::eUXB)
        {
            const PSX_RECT objRect = pObjIter->VGetBoundingRect();
            if (bOurRect.x <= objRect.w
                && bOurRect.w >= objRect.x
                && bOurRect.h >= objRect.y
                && bOurRect.y <= objRect.h)
            {
                pObjIter->VTakeDamage(this);
            }
        }
    }
}

s16 Abe::ToLeftRightMovement_422AA0()
{
    mVelY = FP_FromInteger(0);
    if (sControlledCharacter != this)
    {
        return 0;
    }

    const FP gridSize = ScaleToGridSize(GetSpriteScale());
    const bool flipX = GetAnimation().GetFlipX();

    if ((flipX && Input().IsAnyPressed(sInputKey_Right)) || (!flipX && Input().IsAnyPressed(sInputKey_Left)))
    {
        mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
        return 1;
    }

    if (Input().IsAnyPressed(sInputKey_Right) || Input().IsAnyPressed(sInputKey_Left))
    {
        const FP directionX = FP_FromInteger(Input().IsAnyPressed(sInputKey_Right) ? 1 : -1);

        if (Input().IsAnyPressed(sInputKey_Run))
        {
            mVelX = directionX * (gridSize / FP_FromInteger(4));
            mCurrentMotion = eAbeMotions::Motion_41_StandingToRun;
        }
        else if (Input().IsAnyPressed(sInputKey_Sneak))
        {
            mVelX = directionX * (gridSize / FP_FromInteger(10));
            mCurrentMotion = eAbeMotions::Motion_47_SneakBegin;
        }
        else
        {
            mVelX = directionX * (gridSize / FP_FromInteger(9));
            mCurrentMotion = eAbeMotions::Motion_6_WalkBegin;
        }

        if (!WallHit(GetSpriteScale() * FP_FromInteger(50), directionX * gridSize))
        {
            if (!WallHit(GetSpriteScale() * FP_FromInteger(20), directionX * gridSize))
            {
                return 1;
            }
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(20), directionX * gridSize))
        {
            mVelX = FP_FromInteger(0);
            mCurrentMotion = eAbeMotions::Motion_72_PushWall;
            Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
            return 0;
        }

        mCurrentMotion = eAbeMotions::Motion_21_StandToCrouch;
        return 1;
    }

    return 0;
}


void Abe::MoveWithVelocity_4257F0(FP speed)
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * speed);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * speed) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    if (FP_GetExponent(mVelX))
    {
        MoveForward_422FC0();
    }
}

void Abe::ToNewElumSyncMotion(s32 elum_frame)
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[mCurrentMotion]));

    field_12C_timer = sGnFrame;
    GetAnimation().SetFrame(elum_frame + 1);
    GetAnimation().SetFlipX(gElum->GetAnimation().GetFlipX());
}

void Abe::SetActiveControlledCharacter_421480()
{
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit7);
    sControlledCharacter = this;
}

PullRingRope* Abe::GetPullRope_422580()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::ePullRingRope)
        {
            PullRingRope* pRope = static_cast<PullRingRope*>(pObj);

            const PSX_RECT bRect = pRope->VGetBoundingRect();
            if ((mYPos - pRope->mYPos - (GetSpriteScale() * FP_FromInteger(80))) <= FP_FromInteger(0))
            {
                if (mXPos > FP_FromInteger(bRect.x) && mXPos < FP_FromInteger(bRect.w))
                {
                    return pRope;
                }
            }
        }
    }
    return nullptr;
}

void Abe::ElumKnockForward_42E780(s32 /*not_used*/)
{
    ToKnockback_422D90(1, 1);
    mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
    mNextMotion = eAbeMotions::Motion_0_Idle;
    mbMotionChanged = true;
    GetAnimation().Set_Animation_Data(GetAnimRes(sAbeMotionAnimIds[mCurrentMotion]));

    sControlledCharacter = sActiveHero;
    gElum->field_154_bAbeForcedDownFromElum = 1;
}

s16 Abe::TryMountElum_42E600()
{
    if (gElum)
    {
        if (FP_Abs(mXPos - gElum->mXPos) <= FP_FromInteger(2) && FP_Abs(mYPos - gElum->mYPos) <= FP_FromInteger(2))
        {
            if (gElum->GetAnimation().GetFlipX() != GetAnimation().GetFlipX())
            {
                mNextMotion = eAbeMotions::Motion_139_ElumMountBegin;
                return eAbeMotions::Motion_2_StandingTurn;
            }

            if (gElum->GetCurrentMotion() != eElumMotions::Motion_1_Idle
                || gElum->mBrainIdx == 1
                || gElum->field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                return eAbeMotions::Motion_0_Idle;
            }

            mRidingElum = true;
            return eAbeMotions::Motion_139_ElumMountBegin;
        }
    }
    return eAbeMotions::Motion_0_Idle;
}

void Abe::BulletDamage_4220B0(Bullet* pBullet)
{
    const PSX_RECT rect = VGetBoundingRect();

    enum class ShootKind
    {
        eEverythingElse_0 = 0,
        eHanging_1 = 1,
        eRolling_2 = 2,
    };

    if (mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin
        || mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd
        || mCurrentMotion == eAbeMotions::Motion_138_ElumUnmountEnd
        || mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin
        || !gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            1)
        || mHealth <= FP_FromInteger(0))
    {
        return;
    }

    ShootKind shootKind = ShootKind::eEverythingElse_0;
    if (mCurrentMotion == eAbeMotions::Motion_24_RollBegin
        || mCurrentMotion == eAbeMotions::Motion_25_RollLoop
        || mCurrentMotion == eAbeMotions::Motion_26_RollEnd
        || mCurrentMotion == eAbeMotions::Motion_39_CrouchTurn
        || mCurrentMotion == eAbeMotions::Motion_19_CrouchIdle)
    {
        shootKind = ShootKind::eRolling_2;
    }
    else if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
             || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
             || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
             || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
             || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
    {
        shootKind = ShootKind::eHanging_1;
    }

    mHealth = FP_FromInteger(0);
    mbGotShot = true;

    switch (pBullet->mBulletType)
    {
        case BulletType::ePossessedSlig_0:
        case BulletType::eNormalBullet_1:
        {
            FP bloodXOff = {};
            if (pBullet->mXDistance > FP_FromInteger(0))
            {
                bloodXOff = FP_FromInteger(-24);
            }
            else
            {
                bloodXOff = FP_FromInteger(24);
            }

            relive_new Blood(
                mXPos,
                pBullet->mYPos,
                bloodXOff,
                FP_FromInteger(0),
                GetSpriteScale(),
                50);

            switch (shootKind)
            {
                case ShootKind::eEverythingElse_0:
                {
                    if (!mRidingElum)
                    {
                        ToKnockback_422D90(1, 1);
                    }
                    else
                    {
                        ElumKnockForward_42E780(1);
                    }

                    if (GetAnimation().GetFlipX() != (pBullet->mXDistance > FP_FromInteger(0)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                    mbMotionChanged = true;
                    mbGotShot = false;

                    if (pBullet->mXDistance >= FP_FromInteger(0))
                    {
                        mVelX = GetSpriteScale() * FP_FromDouble(7.8);
                    }
                    else
                    {
                        mVelX = -GetSpriteScale() * FP_FromDouble(7.8);
                    }
                    break;
                }
                case ShootKind::eHanging_1:
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    break;
                }
                case ShootKind::eRolling_2:
                {
                    if (GetAnimation().GetFlipX() == (pBullet->mXDistance > FP_FromInteger(0)))
                    {
                        mNextMotion = eAbeMotions::Motion_73_RollingKnockback;
                    }
                    else
                    {
                        mNextMotion = eAbeMotions::Motion_129_RollingKnockForward;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case BulletType::eZBullet_2:
        {
            if (GetSpriteScale() == FP_FromDouble(0.5))
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }
            if (Bullet::InZBulletCover(FP_FromInteger(rect.x), FP_FromInteger(rect.y), rect))
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }
            if (shootKind != ShootKind::eEverythingElse_0)
            {
                if (shootKind == ShootKind::eHanging_1)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                }
                else if (shootKind == ShootKind::eRolling_2)
                {
                    mNextMotion = eAbeMotions::Motion_147_ShotRolling;
                }
            }
            if (mCurrentMotion != eAbeMotions::Motion_114_ElumRunLoop)
            {
                if (!mRidingElum)
                {
                    mNextMotion = eAbeMotions::Motion_148_Shot;
                }
                else
                {
                    ElumKnockForward_42E780(1);
                    mCurrentMotion = eAbeMotions::Motion_148_Shot;
                    mbMotionChanged = true;
                    mbGotShot = false;
                }
            }
            else
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }

                relive_new Blood(
                    mXPos,
                    mYPos - FP_FromInteger(45),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    FP_FromInteger(1),
                    50);

            break;
        }
        default:
            break;
    }

    if (mbGotShot)
    {
        field_112_prev_motion = mNextMotion;
    }

    Environment_SFX_42A220(EnvironmentSfx::eElumHitWall_14, 0, 0x7FFF, this);
    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 127, 0, this);
    Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
    SFX_Play_Pitch(relive::SoundEffects::Eating1, 0, -500, GetSpriteScale());
    SfxPlayMono(relive::SoundEffects::KillEffect, 0, GetSpriteScale());
}

bool Abe::NearDoorIsOpen()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eDoor)
        {
            auto pDoor = static_cast<Door*>(pObj);
            PSX_RECT Rect = VGetBoundingRect();
            PSX_RECT Rect2 = pDoor->VGetBoundingRect();

            if (Rect.x <= Rect2.w && Rect.w >= Rect2.x && Rect.h >= Rect2.y && Rect.y <= Rect2.h)
            {
                return pDoor->vIsOpen_40E800();
            }
        }
    }

    // We didn't find a door - so for some reason that makes no sense return that it is open...
    return true;
}

s16 Abe::RunTryEnterDoor_4259C0()
{
    if (!Input().IsAnyPressed(sInputKey_Up))
    {
        return 0;
    }
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return 0;
    }
    if (GetAnimation().GetCurrentFrame() < 4)
    {
        return 0;
    }

    // Are we actually on a door?
    relive::Path_TLV* pDoorTlv = gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eDoor);

    if (!pDoorTlv)
    {
        return 0;
    }

    if (!NearDoorIsOpen())
    {
        return 0;
    }

    BaseAliveGameObjectPathTLV = pDoorTlv;
    field_110_state.raw = 0;
    mCurrentMotion = eAbeMotions::Motion_156_DoorEnter;
    mXPos = FP_FromInteger((pDoorTlv->mBottomRightX + pDoorTlv->mTopLeftX) / 2);
    MapFollowMe(true);
    return 1;
}

s16 Abe::MoveLiftUpOrDown_42F190(FP yVelocity)
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!pLiftPoint)
    {
        return eAbeMotions::Motion_135_LiftGrabIdle;
    }

    pLiftPoint->Move(FP_FromInteger(0), yVelocity, 0);
    FollowLift_42EE90();

    if (gBeeInstanceCount && gBeesNearAbe)
    {
        return eAbeMotions::Motion_141_BeesStrugglingOnLift;
    }
    if (sControlledCharacter == this && !GetAnimation().GetIsLastFrame() && GetAnimation().GetCurrentFrame() != 5)
    {
        return mCurrentMotion;
    }
    if (yVelocity >= FP_FromInteger(0))
    {
        if (yVelocity > FP_FromInteger(0))
        {
            if (pLiftPoint->OnBottomFloor())
            {
                return eAbeMotions::Motion_135_LiftGrabIdle;
            }
            if (Input().IsAnyPressed(sInputKey_Down))
            {
                return eAbeMotions::Motion_132_LiftUseDown;
            }
            if (Input().IsAnyPressed(sInputKey_Up))
            {
                return eAbeMotions::Motion_131_LiftUseUp;
            }
        }
    }
    else
    {
        if (pLiftPoint->OnTopFloor())
        {
            return eAbeMotions::Motion_135_LiftGrabIdle;
        }
        if (Input().IsAnyPressed(sInputKey_Up))
        {
            return eAbeMotions::Motion_131_LiftUseUp;
        }
        if (Input().IsAnyPressed(sInputKey_Down))
        {
            return eAbeMotions::Motion_132_LiftUseDown;
        }
    }

    if (Input().Pressed() && pLiftPoint->OnAnyFloor() && !(pLiftPoint->field_12C_bMoving & 1))
    {
        return eAbeMotions::Motion_134_LiftGrabEnd;
    }

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eAbeMotions::Motion_135_LiftGrabIdle;
}

void Abe::VScreenChanged()
{
    if (sControlledCharacter == this || sControlledCharacter == gElum)
    {
        mCurrentLevel = gMap.mNextLevel;
        mCurrentPath = gMap.mNextPath;
    }

    // Level has changed?
    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        // Set the correct tint for this map
        SetTint(sAbeTints_4C6438, gMap.mNextLevel);

        if (gMap.mCurrentLevel != EReliveLevelIds::eMenu)
        {
            if (field_19C_throwable_count > 0)
            {
                if (gThrowableArray)
                {
                    gThrowableArray->Remove(field_19C_throwable_count);
                }
            }

            field_19C_throwable_count = 0;
            field_168_ring_pulse_timer = 0;
        }

        if (gMap.mNextLevel == EReliveLevelIds::eCredits || gMap.mNextLevel == EReliveLevelIds::eMenu)
        {
            // Remove Abe for menu/credits levels?
            SetDead(true);
        }
    }
}

void Abe::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eContinuePoint)
        {
            relive::Path_ContinuePoint* pContinuePointTlv = static_cast<relive::Path_ContinuePoint*>(pTlv);

            if ((pContinuePointTlv->mZoneNumber != mContinueZoneNumber || mContinueLevel != gMap.mCurrentLevel) && !mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted) && mCurrentMotion != eAbeMotions::Motion_156_DoorEnter)
            {
                mContinueZoneNumber = pContinuePointTlv->mZoneNumber;
                mContinueClearFromId = pContinuePointTlv->mClearFromId;
                mContinueClearToId = pContinuePointTlv->mClearToId;

                mContinueTopLeft.x = pContinuePointTlv->mTopLeftX;
                mContinueTopLeft.y = pContinuePointTlv->mTopLeftY;
                mContinueBottomRight.x = pContinuePointTlv->mBottomRightX;
                mContinueBottomRight.y = pContinuePointTlv->mBottomRightY;
                mContinueSpriteScale = GetSpriteScale();

                field_2A8_flags.Set(Flags_2A8::eAbeSpawnDir, pContinuePointTlv->mAbeSpawnDir == relive::Path_ContinuePoint::spawnDirection::eLeft);

                const s32 bHaveShry = field_168_ring_pulse_timer - sGnFrame;
                field_150_saved_ring_timer = bHaveShry < 0 ? 0 : bHaveShry;
                field_154_bSavedHaveShrykull = field_16C_bHaveShrykull;

                mContinueLevel = gMap.mCurrentLevel;
                mContinuePath = gMap.mCurrentPath;
                mContinueCamera = gMap.mCurrentCamera;

                if (gRestartRuptureFarmsSavedMuds == 0 && gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn && gMap.mCurrentPath == 19 && gMap.mCurrentCamera == 3)
                {
                    gRestartRuptureFarmsKilledMuds = sKilledMudokons;
                    gRestartRuptureFarmsSavedMuds = sRescuedMudokons;
                }

                SaveGame::SaveToMemory(&gSaveBuffer);

                const FP camXPos = FP_NoFractional(pScreenManager->CamXPos());

                FP indicator_xpos = {};
                if (mXPos - camXPos >= FP_FromInteger(384 / 2)) // mid screen x
                {
                    indicator_xpos = mXPos - ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    indicator_xpos = ScaleToGridSize(GetSpriteScale()) + mXPos;
                }
                const FP indicator_ypos = mYPos + (GetSpriteScale() * FP_FromInteger(-50));

                relive_new ThrowableTotalIndicator(indicator_xpos, indicator_ypos, GetAnimation().GetRenderLayer(),
                                                                            GetAnimation().GetSpriteScale(), 11, 1);
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);

            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            EventBroadcast(kEventLoudNoise, this);
            EventBroadcast(kEventSpeaking, this);

            if (mRidingElum && sControlledCharacter != this)
            {
                sControlledCharacter = sActiveHero;
                FreeElumRes_420F80();
            }
            ToDeathDropFall_42C3D0();
        }

        // To next TLV
        pTlv = gMap.TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

s16 Abe::HandleDoAction_429A70()
{
    s16 mountMotion = TryMountElum_42E600();
    if (mountMotion != eAbeMotions::Motion_0_Idle)
    {
        return mountMotion;
    }

    relive::Path_TLV* pTlv = gMap.TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    while (pTlv)
    {
        switch (pTlv->mTlvType)
        {
            case ReliveTypes::eWellLocal:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_77_WellBegin;

            case ReliveTypes::eLever:
                if (FP_FromInteger(FP_GetExponent(mXPos) - pTlv->mTopLeftX) < ScaleToGridSize(GetSpriteScale()))
                {
                    // Wrong dir
                    if (GetAnimation().GetFlipX())
                    {
                        return eAbeMotions::Motion_36_DunnoBegin;
                    }

                    // Get switch
                    auto pSwitch = static_cast<Lever*>(FindObjectOfType(
                        ReliveTypes::eLever,
                        mXPos + ScaleToGridSize(GetSpriteScale()),
                        mYPos - FP_FromInteger(5)));

                    // Pull it
                    if (pSwitch)
                    {
                        pSwitch->VPull(mXPos < pSwitch->mXPos);
                        return eAbeMotions::Motion_101_LeverUse;
                    }
                }
                else if (FP_FromInteger(pTlv->mBottomRightX - FP_GetExponent(mXPos)) < ScaleToGridSize(GetSpriteScale()))
                {
                    // Wrong dir
                    if (!GetAnimation().GetFlipX())
                    {
                        return eAbeMotions::Motion_36_DunnoBegin;
                    }

                    // Get switch
                    auto pSwitch = static_cast<Lever*>(FindObjectOfType(
                        ReliveTypes::eLever,
                        mXPos - ScaleToGridSize(GetSpriteScale()),
                        mYPos - FP_FromInteger(5)));

                    // Pull it
                    if (pSwitch)
                    {
                        pSwitch->VPull(mXPos < pSwitch->mXPos);
                        return eAbeMotions::Motion_101_LeverUse;
                    }
                }
                break;

            case ReliveTypes::eWellExpress:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_80_430EF0;

            case ReliveTypes::eBoomMachine:
            {
                auto pBoomMachine = static_cast<BoomMachine*>(FindObjectOfType(
                    ReliveTypes::eBoomMachine,
                    mXPos,
                    mYPos - (GetSpriteScale() * FP_FromInteger(25))));

                if (pBoomMachine && pBoomMachine->VIsButtonOn())
                {
                    pBoomMachine->VHandleButton();
                    return eAbeMotions::Motion_90_GrenadeMachineUse;
                }
                return eAbeMotions::Motion_36_DunnoBegin;
            }

            default:
                break;
        }

        pTlv = gMap.TLV_Get_At(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
    return eAbeMotions::Motion_36_DunnoBegin;
}

s16 Abe::VTakeDamage(BaseGameObject* pFrom)
{
    SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);

    const auto old_say = field_130_say;
    field_130_say = -1;

    if (field_188_pOrbWhirlWind)
    {
        field_188_pOrbWhirlWind->ToStop();
        field_188_pOrbWhirlWind = nullptr;
    }

    if (field_18C_pObjToPossess)
    {
        field_18C_pObjToPossess->mBaseGameObjectRefCount--;
        field_18C_pObjToPossess = nullptr;
    }

    // CantBeDamaged_44BAB0() helper func in AE
    switch (mCurrentMotion)
    {
        case eAbeMotions::Motion_74_JumpIntoWell:
        case eAbeMotions::Motion_75_ToInsideOfAWellLocal:
        case eAbeMotions::Motion_76_ToWellShotOut:
        case eAbeMotions::Motion_77_WellBegin:
        case eAbeMotions::Motion_78_InsideWellLocal:
        case eAbeMotions::Motion_79_WellShotOut:
        case eAbeMotions::Motion_80_430EF0:
        case eAbeMotions::Motion_81_InsideWellExpress:
        case eAbeMotions::Motion_82_WellExpressShotOut:
        case eAbeMotions::Motion_83_430F00:
        case eAbeMotions::Motion_84_ToInsideWellLocal:
        case eAbeMotions::Motion_85_ToWellShotOut:
        case eAbeMotions::Motion_156_DoorEnter:
        case eAbeMotions::Motion_157_DoorExit:
        case eAbeMotions::Motion_162_ToShrykull:
        case eAbeMotions::Motion_163_ShrykullEnd:
            return 0;
    }

    if (gAbeInvulnerableCheat || field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel) || !GetAnimation().GetRender())
    {
        return 0;
    }

    field_11C_regen_health_timer = sGnFrame + 900;

    auto oldHp = mHealth;

    switch (pFrom->Type())
    {
        case ReliveTypes::eBat:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth -= FP_FromInteger(1);
                if (mHealth > FP_FromInteger(0))
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, this);
                }
                else
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, this);
                    Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    mHealth = FP_FromInteger(0);
                    mbGotShot = true;
                    field_112_prev_motion = eAbeMotions::Motion_128_KnockForward;
                    mNextMotion = eAbeMotions::Motion_128_KnockForward;
                    if (mCurrentMotion != eAbeMotions::Motion_64_LedgeAscend
                        && mCurrentMotion != eAbeMotions::Motion_66_LedgeHang
                        && mCurrentMotion != eAbeMotions::Motion_68_LedgeHangWobble
                        && mCurrentMotion != eAbeMotions::Motion_65_LedgeDescend
                        && mCurrentMotion != eAbeMotions::Motion_67_ToOffScreenHoist)
                    {
                        break;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                        mbMotionChanged = true;
                        mbGotShot = false;
                        field_114_gnFrame = 0;
                        return 1;
                    }
                }
            }
            break;

        case ReliveTypes::eGasCountDown:
            if (mHealth > FP_FromInteger(0))
            {
                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    mbMotionChanged = true;
                    return 1;
                }

                if (IsStanding_41FC10())
                {
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_164_PoisonGasDeath;
                    mbMotionChanged = true;
                    field_114_gnFrame = 1;
                }
                else
                {
                    if (mCurrentMotion != eAbeMotions::Motion_70_Knockback && mCurrentMotion != eAbeMotions::Motion_71_KnockbackGetUp)
                    {
                        ToKnockback_422D90(1, 1);
                        mbMotionChanged = true;
                    }
                }
            }
            break;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eOops_16, 0, 0, this);
            break;

        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
            if (GetAnimation().GetRender())
            {
                if (mHealth > FP_FromInteger(0))
                {
                    mbGotShot = true;
                    field_112_prev_motion = eAbeMotions::Motion_70_Knockback;
                    mNextMotion = eAbeMotions::Motion_70_Knockback;
                }

                mHealth = FP_FromInteger(0);
                mRGB.SetRGB(30, 30, 30);

                relive_new Gibs(
                    GibType::Abe_0,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale());

                GetAnimation().SetRender(false);
                GetShadow()->mEnabled = false;
            }
            break;

        case ReliveTypes::eSecurityClaw:
        case ReliveTypes::eSecurityOrb:
            field_130_say = 5;
            field_134_auto_say_timer = sGnFrame + 27;
            // The zap makes Abe drop his stuff everywhere
            for (s32 i = 0; i < field_19C_throwable_count; i++)
            {
                field_198_pThrowable = Make_Throwable(mXPos, mYPos - FP_FromInteger(30), 0);

                const FP rand1 = FP_FromRaw((Math_NextRandom() - 127) << 11); // TODO: Wat?
                const FP rand2 = (FP_FromDouble(0.03125) * FP_FromRaw(Math_NextRandom())) - FP_FromInteger(2);
                field_198_pThrowable->VThrow(rand1, rand2);
                field_198_pThrowable->SetSpriteScale(GetSpriteScale());
                field_198_pThrowable->VTimeToExplodeRandom();
                field_198_pThrowable = nullptr;
            }
            field_19C_throwable_count = 0;
            break;

        case ReliveTypes::eRockSpawner:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);

                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    return 1;
                }
                ToKnockback_422D90(1, 1);
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            }
            break;

        case ReliveTypes::eMeatSaw:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);
                ToKnockback_422D90(1, 1);
                mRGB.SetRGB(30, 30, 30);

                relive_new Gibs(
                    GibType::Abe_0,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale());

                relive_new Gibs(
                    GibType::Abe_0,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale());

                GetAnimation().SetRender(false);
            }
            break;

        case ReliveTypes::eParamite:
        case ReliveTypes::eScrab:
        case ReliveTypes::eSlog:
            if (mHealth > FP_FromInteger(0))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);
                mHealth = FP_FromInteger(0);
                const PSX_RECT abeRect = VGetBoundingRect();
                if (pAliveObj->mVelX <= FP_FromInteger(0))
                {
                    relive_new Blood(
                        mXPos,
                        FP_FromInteger(abeRect.h + abeRect.y) / FP_FromInteger(2),
                        FP_FromInteger(-24),
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        50);
                }
                else
                {
                    relive_new Blood(
                        mXPos,
                        FP_FromInteger(abeRect.h + abeRect.y) / FP_FromInteger(2),
                        FP_FromInteger(24),
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        50);
                }

                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    return 1;
                }

                if (mCurrentMotion >= eAbeMotions::Motion_102_ElumWalkLoop
                    && mCurrentMotion <= eAbeMotions::Motion_125_ElumMidWalkEnd)
                {
                    mNextMotion = eAbeMotions::Motion_148_Shot;
                    field_112_prev_motion = eAbeMotions::Motion_148_Shot;
                    mbGotShot = true;
                    return 1;
                }

                ToKnockback_422D90(1, 1);
                mbMotionChanged = true;

                if (pAliveObj->mXPos < mXPos)
                {
                    if (!(GetAnimation().GetFlipX()))
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }
                else if (pAliveObj->mXPos > mXPos)
                {
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }

                if (pAliveObj->GetAnimation().GetFlipX())
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(-7.8));
                }
                else
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(7.8));
                }

                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                if (pAliveObj->Type() != ReliveTypes::eParamite)
                {
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
                }
            }
            break;

        case ReliveTypes::eAbilityRing:
            return 0;

        case ReliveTypes::eRollingBall:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);
                if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                    || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                    || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                    || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                    || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    mbMotionChanged = true;
                    mbGotShot = false;
                    field_114_gnFrame = 0;
                    return 1;
                }

                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

                ToKnockback_422D90(1, 1);

                if (pAliveObj->mXPos < mXPos)
                {
                    if (!GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }
                else if (pAliveObj->mXPos > mXPos)
                {
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                    }
                }

                if (pAliveObj->mVelX >= FP_FromInteger(0))
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(7.8));
                }
                else
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(-7.8));
                }

                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            }
            break;

        case ReliveTypes::eUnknown:
            break;

        case ReliveTypes::eZBall:
            if (mHealth > FP_FromInteger(0))
            {
                mbGotShot = true;
                mHealth = FP_FromInteger(0);
                if (static_cast<ZBall*>(pFrom)->mFrameAbove12 == 1)
                {
                    field_112_prev_motion = eAbeMotions::Motion_148_Shot;
                    mNextMotion = eAbeMotions::Motion_148_Shot;
                }
                else
                {
                    field_112_prev_motion = eAbeMotions::Motion_147_ShotRolling;
                    mNextMotion = eAbeMotions::Motion_147_ShotRolling;
                }
            }
            break;

        case ReliveTypes::eBeeSwarm:
            if (mHealth > FP_FromInteger(0))
            {
                mHealth -= FP_FromDouble(0.15);
                if (mHealth > FP_FromInteger(0))
                {
                    const auto rnd_sfx = Math_RandomRange(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10;
                    const FP v16 = (FP_FromInteger(1) - sActiveHero->mHealth) / FP_FromDouble(0.15);
                    const s16 calc_pitch = Math_RandomRange(200 * FP_GetExponent(v16), 200 * FP_GetExponent(v16) + 1);
                    Mudokon_SFX(rnd_sfx, 0, calc_pitch, this);
                }
                else
                {
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 1000, this);
                    Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
                    mHealth = FP_FromInteger(0);
                    mbGotShot = true;
                    field_112_prev_motion = eAbeMotions::Motion_128_KnockForward;
                    mNextMotion = eAbeMotions::Motion_128_KnockForward;
                    if (mCurrentMotion == eAbeMotions::Motion_64_LedgeAscend
                        || mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
                        || mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble
                        || mCurrentMotion == eAbeMotions::Motion_65_LedgeDescend
                        || mCurrentMotion == eAbeMotions::Motion_67_ToOffScreenHoist)
                    {
                        mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                        mbMotionChanged = true;
                        mbGotShot = false;
                        field_114_gnFrame = 0;
                        return 1;
                    }

                    if (mRidingElum)
                    {
                        ElumKnockForward_42E780(1);
                        mbGotShot = false;
                    }
                }

                if (mCurrentMotion == eAbeMotions::Motion_103_ElumIdle)
                {
                    mCurrentMotion = eAbeMotions::Motion_126_ElumBeesStruggling;
                    mbMotionChanged = true;
                }
            }
            break;

        case ReliveTypes::eElectrocute:
            GetAnimation().SetRender(false);
            ToDieFinal_42C400();
            break;

        case ReliveTypes::eBullet:
            // NOTE: This was in the default case! The type may not be bullet in there which would corrupt memory or crash
            BulletDamage_4220B0(static_cast<Bullet*>(pFrom));
            if (!mbGotShot)
            {
                field_130_say = old_say;
                oldHp = FP_FromInteger(0);
            }
            break;

        default:
            break;
    }

    if (sControlledCharacter->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::ePossessed))
    {
        if (mHealth == FP_FromInteger(0))
        {
            sControlledCharacter->VUnPosses();
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit7);
            sControlledCharacter = this;
        }
    }

    return oldHp > FP_FromInteger(0) ? 1 : 0;
}

static bool IsSameScaleAsHoist(relive::Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    auto width = pHoist->Width();

    //The width is used to determine the layer as there is no layer property in the TLV
    if (pObj->GetSpriteScale() == FP_FromInteger(1))
    {
        return width > 18;
    }
    else
    {
        return width <= 18;
    }
}

static bool IsFacingSameDirectionAsHoist(relive::Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingLeft && !pObj->GetAnimation().GetFlipX())
    {
        return false;
    }
    else if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingRight && pObj->GetAnimation().GetFlipX())
    {
        return false;
    }
    return true;
}

void Abe::TryHoist_423420()
{
    mCurrentMotion = eAbeMotions::Motion_16_HoistBegin;

    auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eHoist));

    if (pHoist && IsSameScaleAsHoist(pHoist, this))
    {
        if (FP_FromInteger(pHoist->Height()) <= GetSpriteScale() * FP_FromInteger(95))
        {
            mCurrentMotion = eAbeMotions::Motion_16_HoistBegin;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_99_HoistBeginLong;
        }
        if (!IsFacingSameDirectionAsHoist(pHoist, this))
        {
            mNextMotion = mCurrentMotion;
            mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
        }
        BaseAliveGameObjectPathTLV = pHoist;
    }
}

void Abe::Motion_0_Idle()
{
    FollowLift_42EE90();
    if (Input_IsChanting() && !field_2A8_flags.Get(Flags_2A8::e2A8_Bit7))
    {
        if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
        {
            mCurrentMotion = eAbeMotions::Motion_162_ToShrykull;
        }
        else
        {
            field_114_gnFrame = sGnFrame + 90;
            mCurrentMotion = eAbeMotions::Motion_150_Chant;
            SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant1_11, 0, 1);
        }
        field_110_state.raw = 0;
        return;
    }
    if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler))
    {
        const auto held = Input().Held();
        if (held & 0xF0)
        {
            field_10C_prev_held = held;
            mCurrentMotion = eAbeMotions::Motion_58_ToSpeak;
            return;
        }
    }
    if (Input().IsAnyPressed(sInputKey_Hop))
    {
        if (Input().IsAnyPressed(sInputKey_Up))
        {
            TryHoist_423420();
        }
        else if (!Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler))
        {
            mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
            field_1A0_portal = VIntoBirdPortal(2);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }
        }
        return;
    }

    if (ToLeftRightMovement_422AA0())
    {
        return;
    }

    if (Input().IsAnyPressed(sInputKey_Down))
    {
        // Check for a lift rope (going down)
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
            const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);

            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && FP_Abs(mXPos - liftPlatformXMidPoint) < halfGrid)
            {
                // AO exclusive - Abe only uses lift facing one side
                if (GetAnimation().GetFlipX())
                {
                    mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
                    mNextMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                }
                return;
            }
        }


        const auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos + FP_FromInteger(16)),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos + FP_FromInteger(16)),
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (!IsSameScaleAsHoist(pHoist, this))
            {
                return;
            }

            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                mCurrentMotion = eAbeMotions::Motion_65_LedgeDescend;
            }
            else
            {
                mNextMotion = eAbeMotions::Motion_65_LedgeDescend;
                mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
            }
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_21_StandToCrouch;
        }
        return;
    }
    if (Input().IsAnyPressed(sInputKey_FartRoll))
    {
        mCurrentMotion = eAbeMotions::Motion_21_StandToCrouch;
    }

    bool handleDoActionOrThrow = false;
    if (Input().IsAnyPressed(sInputKey_Up))
    {
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = FP_Abs(mXPos - liftPlatformXMidPoint);
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    // AO exclusive - Abe only uses lift facing one side
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_2_StandingTurn;
                        mNextMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_133_LiftGrabBegin;
                    }
                    return;
                }
            }
        }

        mCurrentMotion = TryMountElum_42E600();
        if (mCurrentMotion != eAbeMotions::Motion_0_Idle)
        {
            return;
        }
        auto pTlv = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        while (pTlv)
        {
            switch (pTlv->mTlvType)
            {
                case ReliveTypes::eDoor:
                {
                    if (NearDoorIsOpen() && !mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
                    {
                        BaseAliveGameObjectPathTLV = pTlv;
                        field_110_state.raw = 0;
                        mCurrentMotion = eAbeMotions::Motion_156_DoorEnter;
                    }
                    else
                    {
                        if (Input().IsAnyHeld(sInputKey_Up))
                        {
                            mCurrentMotion = eAbeMotions::Motion_36_DunnoBegin;
                        }
                    }
                    return;
                }
                case ReliveTypes::eWellLocal:
                {
                    auto well = static_cast<relive::Path_WellLocal*>(pTlv);
                    if ((well->mScale != relive::reliveScale::eFull || GetSpriteScale() != FP_FromInteger(1)) &&
                        (well->mScale != relive::reliveScale::eHalf || GetSpriteScale() != FP_FromDouble(0.5)))
                    {
                        break;
                    }
                    field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_snap_abe);
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_77_WellBegin;
                    return;
                }
                case ReliveTypes::eWellExpress:
                {
                    auto well = static_cast<relive::Path_WellExpress*>(pTlv);
                    if ((well->mScale != relive::reliveScale::eFull || GetSpriteScale() != FP_FromInteger(1)) &&
                        (well->mScale != relive::reliveScale::eHalf || GetSpriteScale() != FP_FromDouble(0.5)))
                    {
                        break;
                    }
                    field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_snap_abe);
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_80_430EF0;
                    return;
                }
                case ReliveTypes::eMovieHandStone:
                case ReliveTypes::eBellSongStone:
                case ReliveTypes::eDemoPlaybackStone:
                case ReliveTypes::eHandStone:
                {
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_88_HandstoneBegin;
                    field_110_state.stone = StoneStates::eHandstoneStart_0;
                    return;
                }
                case ReliveTypes::eBoomMachine:
                {
                    auto pMachineButton = static_cast<BoomMachine*>(FindObjectOfType(
                        ReliveTypes::eBoomMachine,
                        mXPos,
                        mYPos - GetSpriteScale() * FP_FromInteger(25)));
                    if (pMachineButton)
                    {
                        pMachineButton->VHandleButton();
                        mCurrentMotion = eAbeMotions::Motion_90_GrenadeMachineUse;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_36_DunnoBegin;
                    }
                    break;
                }
                default:
                    break;
            }

            pTlv = gMap.TLV_Get_At(
                pTlv,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
        if (mCurrentMotion == eAbeMotions::Motion_0_Idle)
        {
            TryHoist_423420();
        }
        handleDoActionOrThrow = true;
    }

    if (!Input().IsAnyPressed(sInputKey_Up) || handleDoActionOrThrow)
    {
        if (Input().IsAnyHeld(sInputKey_ThrowItem) && mCurrentMotion == eAbeMotions::Motion_0_Idle)
        {
            if (field_19C_throwable_count > 0 || gInfiniteGrenades)
            {
                field_198_pThrowable = Make_Throwable(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    0);

                if (bThrowableIndicatorExists_504C70 == 0)
                {
                    const FP xOffSet = GetAnimation().GetFlipX() ? FP_FromInteger(15) : FP_FromInteger(-15) * GetSpriteScale();

                    relive_new ThrowableTotalIndicator(mXPos + xOffSet,
                                                                                    mYPos + GetSpriteScale() * FP_FromInteger(-50),
                                                                                    GetAnimation().GetRenderLayer(),
                                                                                    GetAnimation().GetSpriteScale(),
                                                                                    field_19C_throwable_count,
                                                                                    true);
                }
                mCurrentMotion = eAbeMotions::Motion_142_RockThrowStandingHold;

                if (!gInfiniteGrenades)
                {
                    field_19C_throwable_count--;
                }
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_36_DunnoBegin;
            }
        }
        else
        {
            if (Input().IsAnyHeld(sInputKey_DoAction))
            {
                mCurrentMotion = HandleDoAction_429A70();
            }
            else if (gBeeInstanceCount && gBeesNearAbe)
            {
                mCurrentMotion = eAbeMotions::Motion_140_BeesStruggling;
            }
            else if (sGnFrame - field_114_gnFrame > 150)
            {
                field_114_gnFrame = sGnFrame;
                const s16 rnd = Math_RandomRange(0, 2);
                u16 loaded = 0;

                //loaded |= 1;
                //loaded |= 2;
                //loaded |= 4;
                if ((1 << rnd) & loaded)
                {
                    switch (rnd)
                    {
                        case 0:
                        {
                            mCurrentMotion = eAbeMotions::Motion_161_Idle_Yawn;
                            break;
                        }
                        case 1:
                        {
                            mCurrentMotion = eAbeMotions::Motion_159_Idle_RubEyes;
                            break;
                        }
                        case 2:
                        {
                            mCurrentMotion = eAbeMotions::Motion_160_Idle_Stretch_Arms;
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void Abe::Motion_1_WalkLoop()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward_422FC0();

    if (mCurrentMotion == eAbeMotions::Motion_1_WalkLoop)
    {
        switch (GetAnimation().GetCurrentFrame())
        {
            case 2:
            {
                if ((mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right)))
                {
                    mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                    field_10C_prev_held = 0;
                    return;
                }

                if (field_10C_prev_held & sInputKey_Hop)
                {
                    mNextMotion = eAbeMotions::Motion_30_HopMid;
                    mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                    field_10C_prev_held = 0;
                    return;
                }

                if (!Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
                {
                    mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                    field_10C_prev_held = 0;
                }
                else
                {
                    FP directedScale = {};
                    if (GetAnimation().GetFlipX())
                    {
                        directedScale = -ScaleToGridSize(GetSpriteScale());
                    }
                    else
                    {
                        directedScale = ScaleToGridSize(GetSpriteScale());
                    }

                    if (WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale * FP_FromDouble(1.5)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle;
                        field_10C_prev_held = 0;
                        return;
                    }
                    else
                    {
                        field_10C_prev_held = 0;
                    }
                }
                break;
            }

            case 11:
                if ((mVelX <= FP_FromInteger(0) || !(Input().IsAnyPressed(sInputKey_Left))) && (mVelX >= FP_FromInteger(0) || !(Input().IsAnyPressed(sInputKey_Right))))
                {
                    if (field_10C_prev_held & sInputKey_Hop)
                    {
                        mNextMotion = eAbeMotions::Motion_30_HopMid;
                    }
                    else if (Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
                    {
                        FP directedScale = {};
                        if (GetAnimation().GetFlipX())
                        {
                            directedScale = -ScaleToGridSize(GetSpriteScale());
                        }
                        else
                        {
                            directedScale = ScaleToGridSize(GetSpriteScale());
                        }

                        if (!WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale * FP_FromDouble(1.5)))
                        {
                            field_10C_prev_held = 0;
                            return;
                        }
                    }
                }
                mCurrentMotion = eAbeMotions::Motion_4_WalkToIdle;
                field_10C_prev_held = 0;
                return;

            case 5:
                Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);

                if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit3_WalkToRun))
                {
                    field_2A8_flags.Set(Flags_2A8::e2A8_Bit3_WalkToRun);
                    MapFollowMe(1);
                }

                if (Input().IsAnyPressed(sInputKey_Run))
                {
                    mCurrentMotion = eAbeMotions::Motion_51_MidWalkToRun;
                }
                else if (Input().IsAnyPressed(sInputKey_Sneak))
                {
                    mCurrentMotion = eAbeMotions::Motion_43_WalkToSneak;
                }
                break;

            case 14:
                Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);

                if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit3_WalkToRun))
                {
                    field_2A8_flags.Set(Flags_2A8::e2A8_Bit3_WalkToRun);
                    MapFollowMe(1);
                }

                if (Input().IsAnyPressed(sInputKey_Run))
                {
                    mCurrentMotion = eAbeMotions::Motion_50_WalkToRun;
                }
                else if (Input().IsAnyPressed(sInputKey_Sneak))
                {
                    mCurrentMotion = eAbeMotions::Motion_45_MidWalkToSneak;
                }
                break;

            default:
                field_2A8_flags.Clear(Flags_2A8::e2A8_Bit3_WalkToRun);
                break;
        }
    }
}

void Abe::Motion_2_StandingTurn()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        if (Input().IsAnyPressed(sInputKey_Run) && Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
        {
#if ORIGINAL_GAME_FIXES || ORIGINAL_GAME_FIX_AUTO_TURN
            mNextMotion = eAbeMotions::Motion_0_Idle; // OG Change - Fixes "Auto-Turn" bug
#endif

            mCurrentMotion = eAbeMotions::Motion_63_TurnToRun;
            GetAnimation().ToggleFlipX();

            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            return;
        }
    }

    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (mNextMotion)
        {
            // OG bug: this local variable allows you to "store" your next state if you face the opposite
            // ledge direction and press down/up and then interrupt it by running away before you hoist.
            const s16 kNext_state = mNextMotion;
            if (mNextMotion != eAbeMotions::Motion_139_ElumMountBegin)
            {
                if (mNextMotion == eAbeMotions::Motion_101_LeverUse)
                {
                    Lever* pSwitch;
                    if (GetAnimation().GetFlipX())
                    {
                        pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos - ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)));
                    }
                    else
                    {
                        pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, mXPos + ScaleToGridSize(GetSpriteScale()), mYPos - FP_FromInteger(5)));
                    }

                    if (pSwitch)
                    {
                        pSwitch->VPull(mXPos < pSwitch->mXPos);
                    }
                }
                mNextMotion = eAbeMotions::Motion_0_Idle;
                mCurrentMotion = kNext_state;
                return;
            }

            if (gElum)
            {
                if (gElum->GetCurrentMotion() == eElumMotions::Motion_1_Idle && !(gElum->field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1)))
                {
                    mRidingElum = true;
                    mNextMotion = eAbeMotions::Motion_0_Idle;
                    mCurrentMotion = kNext_state;
                    return;
                }
            }
            mNextMotion = eAbeMotions::Motion_0_Idle;
        }
        else
        {
            if (ToLeftRightMovement_422AA0())
            {
                GetAnimation().Set_Animation_Data(GetAnimation().mAnimRes);
                return;
            }
        }
        ToIdle_422D50();
    }
}

static bool isEdgeGrabbable(relive::Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingLeft && pObj->GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingRight && !pObj->GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingAnyDirection)
    {
        return true;
    }

    return false;
}

void Abe::Motion_3_Fall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * field_120_x_vel_slow_by);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * field_120_x_vel_slow_by);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    const s32 bCollision = InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    if (BaseAliveGameObjectPathTLV)
    {
        if (mHealth > FP_FromInteger(0))
        {
            if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eWellLocal || BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eWellExpress)
            {
                // The well must be on the same scale/layer
                relive::Path_WellBase* pWellBase = static_cast<relive::Path_WellBase*>(BaseAliveGameObjectPathTLV);
                if ((pWellBase->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) ||
                    (pWellBase->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
                {
                    field_2A8_flags.Set(Flags_2A8::e2A8_Bit4_snap_abe);
                    mCurrentMotion = eAbeMotions::Motion_74_JumpIntoWell;
                    return;
                }
            }
        }
    }

    if (bCollision)
    {
        switch (pPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                mXPos = hitX;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pPathLine;
                MapFollowMe(1);
                field_114_gnFrame = sGnFrame + 30;
                if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(240))
                {
                    BaseAliveGameObjectLastLineYPos += FP_FromInteger(240);
                }

                relive::Path_SoftLanding* pSoftLanding = static_cast<relive::Path_SoftLanding*>(gMap.VTLV_Get_At(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eSoftLanding));

                if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit8_bLandSoft)
                    || (pSoftLanding && mHealth > FP_FromInteger(0))
                    || ((mYPos - BaseAliveGameObjectLastLineYPos) < (GetSpriteScale() * FP_FromInteger(180))
                        && (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)))
                {
                    mCurrentMotion = eAbeMotions::Motion_98_LandSoft;
                }
                else
                {
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_86_FallLandDie;
                    field_11C_regen_health_timer = sGnFrame + 900;
                }
                mPreviousMotion = eAbeMotions::Motion_3_Fall;

                PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(25)), FP_GetExponent(mYPos - FP_FromInteger(25))};
                PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(25)), FP_GetExponent(mYPos + FP_FromInteger(25))};
                OnCollisionWith(
                    xy,
                    wh,
                    gPlatformsArray);
                break;
            }
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mXPos = hitX;
                mYPos = hitY;
                ToKnockback_422D90(1, 1);
                break;
            default:
                return;
        }
        return;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return;
    }

    bool tryToHang = false;
    relive::Path_Edge* pEdge = static_cast<relive::Path_Edge*>(gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(80)),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eEdge));

    if (pEdge)
    {
        if (pEdge->mCanGrab == relive::reliveChoice::eYes && isEdgeGrabbable(pEdge, this))
        {
            tryToHang = true;
        }
        BaseAliveGameObjectPathTLV = pEdge;
    }
    else
    {
        relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(20)),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(20)),
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                tryToHang = true;
            }
        }

        BaseAliveGameObjectPathTLV = pHoist;
    }

    if (tryToHang)
    {
        if (mVelX == FP_FromInteger(0))
        {
            return;
        }

        mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mTopLeftX + BaseAliveGameObjectPathTLV->mBottomRightX) / 2);

        MapFollowMe(true);

        if (sCollisions->Raycast(
                mXPos,
                mYPos - GetSpriteScale() * FP_FromInteger(20) - GetSpriteScale() * FP_FromInteger(80),
                mXPos,
                mYPos,
                &pPathLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
        {
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pPathLine;
            mCurrentMotion = eAbeMotions::Motion_68_LedgeHangWobble;
            GetShadow()->mShadowAtBottom = true;
        }
    }
}

void Abe::Motion_4_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward_422FC0();

    if (GetAnimation().GetCurrentFrame() != 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(1);

            if (mNextMotion == eAbeMotions::Motion_30_HopMid)
            {
                mNextMotion = eAbeMotions::Motion_0_Idle;
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;

                field_1A0_portal = VIntoBirdPortal(2);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }
            }
            else
            {
                ToIdle_422D50();
            }
        }
    }
    else
    {
        Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
    }
}

void Abe::Motion_5_MidWalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward_422FC0();

    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(1);
            if (mNextMotion == eAbeMotions::Motion_30_HopMid)
            {
                mNextMotion = 0;
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
                field_1A0_portal = VIntoBirdPortal(2);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }
            }
            else
            {
                ToIdle_422D50();
            }
        }
    }
    else
    {
        Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 0x7FFF, this);
    }
}

void Abe::Motion_6_WalkBegin()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_7_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_8_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_9_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_10_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_11_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_12_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_13_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_14_Speak()
{
    Motion_58_ToSpeak();
}

void Abe::Motion_15_Null()
{
    // Empty
}

void Abe::Motion_16_HoistBegin()
{
    FollowLift_42EE90();

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        const FP velY = GetSpriteScale() * FP_FromInteger(-8);
        mVelY = velY;
        mYPos += velY;
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_17_HoistIdle()
{
    BaseGameObject* pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();
    if (bCollision)
    {
        switch (pPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                mXPos = hitX;
                mYPos = hitY;

                MapFollowMe(1);

                BaseAliveGameObjectCollisionLine = pPathLine;

                mCurrentMotion = eAbeMotions::Motion_18_HoistLand;
                mPreviousMotion = eAbeMotions::Motion_17_HoistIdle;

                PSX_RECT rect = VGetBoundingRect();
                rect.y += 5;
                rect.h += 5;

                OnCollisionWith(
                    {rect.x, rect.y},
                    {rect.w, rect.h},
                    gPlatformsArray);
                break;
            }
        }
        return;
    }

    field_160_pRope = GetPullRope_422580();
    if (field_160_pRope)
    {
        if (field_160_pRope->Pull(this))
        {
            mCurrentMotion = eAbeMotions::Motion_69_RingRopePullHang;
            mNextMotion = eAbeMotions::Motion_0_Idle;
            field_160_pRope->mBaseGameObjectRefCount++;
            return;
        }
        field_160_pRope = nullptr;
    }

    if (mVelY >= FP_FromInteger(0))
    {
        auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eHoist));
        BaseAliveGameObjectPathTLV = pHoist;
        if (pHoist && IsSameScaleAsHoist(pHoist, this))
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                if (pHoist->mHoistType == relive::Path_Hoist::Type::eOffScreen)
                {
                    if (gMap.SetActiveCameraDelayed(MapDirections::eMapTop_2, this, -1))
                    {
                        PSX_Prevent_Rendering_4945B0();
                        mCurrentMotion = eAbeMotions::Motion_67_ToOffScreenHoist;
                        return;
                    }
                    mYPos -= GetSpriteScale() * FP_FromInteger(80);
                    GetShadow()->mShadowAtBottom = true;
                }

                mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
                Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);


                if (sCollisions->Raycast(
                        mXPos,
                        mYPos - GetSpriteScale() * FP_FromInteger(80),
                        mXPos,
                        mYPos,
                        &pPathLine,
                        &hitX,
                        &hitY,
                        GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
                {
                    mYPos = hitY;
                    BaseAliveGameObjectCollisionLine = pPathLine;
                    mVelY = FP_FromInteger(0);
                    if (!pPlatform)
                    {
                        if (pPathLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                            pPathLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                        {
                            PSX_RECT rect = VGetBoundingRect();
                            rect.y += 5;
                            rect.h += 5;

                            OnCollisionWith(
                                {rect.x, rect.y},
                                {rect.w, rect.h},
                                gPlatformsArray);
                        }
                    }
                    GetShadow()->mShadowAtBottom = true;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
    }
}

void Abe::Motion_18_HoistLand()
{
    FollowLift_42EE90();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPreviousMotion == 3)
        {
            Environment_SFX_42A220(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }

        if (Input().IsAnyPressed(sInputKey_Hop))
        {
            if (Input().IsAnyPressed(sInputKey_Up))
            {
                mCurrentMotion = eAbeMotions::Motion_16_HoistBegin;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
                field_1A0_portal = VIntoBirdPortal(2);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_19_CrouchIdle()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        mCurrentMotion = eAbeMotions::Motion_100_RollOffLedge;
        return;
    }

    FollowLift_42EE90();

    if (Input().IsAllPressed(5) && Input().IsAllHeld(5))
    {
        Mudokon_SFX(MudSounds::eDunno_15, 0, 0, this);
        mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
        return;
    }

    // Crouching game speak
    CrouchingGameSpeak_427F90();

    field_10C_prev_held = 0;
    field_10E_released_buttons = 0;

    if (mCurrentMotion == eAbeMotions::Motion_22_CrouchSpeak || mCurrentMotion == eAbeMotions::Motion_23_CrouchSpeak)
    {
        EventBroadcast(kEventSpeaking, this);
        return;
    }


    // Hit bombs/pick up items ?
    if (Input().IsAnyHeld(sInputKey_DoAction))
    {
        if (!Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
        {
            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            PickUpThrowabe_Or_PressBomb_428260(
                gridSize + mXPos,
                FP_GetExponent(mYPos - FP_FromInteger(5)),
                0);
        }
    }


    // Crouching throw stuff
    if (Input().IsAnyHeld(sInputKey_ThrowItem))
    {
        if (mCurrentMotion == eAbeMotions::Motion_19_CrouchIdle)
        {
            if (field_19C_throwable_count > 0 || gInfiniteGrenades)
            {
                field_198_pThrowable = Make_Throwable(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    0);

                if (!bThrowableIndicatorExists_504C70)
                {
                    const FP yOff = mYPos + (GetSpriteScale() * FP_FromInteger(-30));
                    const FP xOff = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromInteger(-10) : FP_FromInteger(10));
                    relive_new ThrowableTotalIndicator(mXPos + xOff,
                                                                             yOff,
                                                                             GetAnimation().GetRenderLayer(),
                                                                             GetAnimation().GetSpriteScale(),
                                                                             field_19C_throwable_count,
                                                                             1);
                }

                mCurrentMotion = eAbeMotions::Motion_145_RockThrowCrouchingHold;

                if (!gInfiniteGrenades)
                {
                    field_19C_throwable_count--;
                }

                return;
            }
        }

        if (!field_19C_throwable_count && !gInfiniteGrenades)
        {
            Mudokon_SFX(MudSounds::eDunno_15, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_23_CrouchSpeak;
            return;
        }
    }

    // Try to stand up
    if (Input().IsAnyHeld(sInputKey_Up) || Input().IsAnyHeld(sInputKey_FartRoll))
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (!sCollisions->Raycast(
                mXPos,
                mYPos,
                mXPos,
                mYPos - (GetSpriteScale() * FP_FromInteger(60)),
                &pLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgCeiling : kBgCeiling))
        {
            mCurrentMotion = eAbeMotions::Motion_20_CrouchToStand;
            return;
        }
    }

    // Crouching turns
    if (Input().IsAnyPressed(sInputKey_Right))
    {
        if (GetAnimation().GetFlipX())
        {
            mCurrentMotion = eAbeMotions::Motion_39_CrouchTurn;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_24_RollBegin;
            field_10E_released_buttons = 0;
        }
    }

    if (Input().IsAnyPressed(sInputKey_Left))
    {
        if (GetAnimation().GetFlipX())
        {
            mCurrentMotion = eAbeMotions::Motion_24_RollBegin;
            field_10E_released_buttons = 0;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_39_CrouchTurn;
        }
    }
}

void Abe::Motion_20_CrouchToStand()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 3)
    {
        if (Input().IsAnyPressed(0xA000)) // TODO: Flags
        {
            ToLeftRightMovement_422AA0();
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_21_StandToCrouch()
{
    FollowLift_42EE90();

    if (GetAnimation().GetIsLastFrame())
    {
        PickUpThrowabe_Or_PressBomb_428260(
            mXPos,
            FP_GetExponent(mYPos - FP_FromInteger(5)),
            1);
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }
}

void Abe::Motion_22_CrouchSpeak()
{
    Motion_23_CrouchSpeak();
}

void Abe::Motion_23_CrouchSpeak()
{
    field_10C_prev_held |= Input().Held();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;

        CrouchingGameSpeak_427F90();

        if (mCurrentMotion == eAbeMotions::Motion_22_CrouchSpeak || mCurrentMotion == eAbeMotions::Motion_23_CrouchSpeak)
        {
            EventBroadcast(kEventSpeaking, this);
        }

        field_10C_prev_held = 0;
    }
}

void Abe::Motion_24_RollBegin()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_422D90(1, 1);

        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward_422FC0();

        if (GetAnimation().GetIsLastFrame())
        {
            if (mCurrentMotion == eAbeMotions::Motion_24_RollBegin)
            {
                mCurrentMotion = eAbeMotions::Motion_25_RollLoop;
            }
        }
    }
}

bool Abe::Is_Celling_Above()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(
               mXPos,
               mYPos,
               mXPos,
               mYPos - (GetSpriteScale() * FP_FromInteger(60)),
               &pLine,
               &hitX,
               &hitY,
               GetSpriteScale() != FP_FromDouble(0.5) ? kFgCeiling : kBgCeiling)
        != 0;
}

void Abe::Motion_25_RollLoop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_released_buttons |= Input().Released();

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_422D90(1, 1);
        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward_422FC0();

        if (mCurrentMotion == eAbeMotions::Motion_25_RollLoop)
        {
            if (GetAnimation().GetCurrentFrame() == 1 || GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 9)
            {
                if (!Input().IsAnyPressed(sInputKey_Run)
                    || Input().IsAnyPressed(sInputKey_FartRoll)
                    || Is_Celling_Above()
                    || field_12C_timer + 9 >= static_cast<s32>(sGnFrame))
                {
                    if (field_10E_released_buttons && sInputKey_FartRoll)
                    {
                        if (!Is_Celling_Above() && field_12C_timer + 9 < static_cast<s32>(sGnFrame))
                        {
                            ToLeftRightMovement_422AA0();
                            field_10E_released_buttons = 0;
                        }
                    }
                }
                else
                {
                    ToLeftRightMovement_422AA0();
                    field_10C_prev_held = 0;
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 8)
            {
                MapFollowMe(true);

                if ((mVelX > FP_FromInteger(0) && !Input().IsAnyPressed(sInputKey_Right)) || (mVelX < FP_FromInteger(0) && !Input().IsAnyPressed(sInputKey_Left)))
                {
                    mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
                    mVelX = FP_FromInteger(0);
                }
            }

            if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 6)
            {
                Environment_SFX_42A220(EnvironmentSfx::eRollingNoise_8, 0, 0x7FFF, this);
            }
        }
    }
}

void Abe::Motion_26_RollEnd()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_422D90(1, 1);
        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward_422FC0();

        if (mCurrentMotion == eAbeMotions::Motion_26_RollEnd)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(1);
                mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

void Abe::Motion_27_RunSlideStop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        MoveWithVelocity_4257F0(FP_FromDouble(0.375));
        if (mCurrentMotion == eAbeMotions::Motion_27_RunSlideStop
            && !RunTryEnterWell_425880()
            && !RunTryEnterDoor_4259C0())
        {
            if (GetAnimation().GetCurrentFrame() >= 9)
            {
                if (GetAnimation().GetCurrentFrame() == 15)
                {
                    Environment_SFX_42A220(EnvironmentSfx::eSlideStop_0, 0, 0x7FFF, this);
                    MapFollowMe(1);

                    if (!ToLeftRightMovement_422AA0())
                    {
                        ToIdle_422D50();
                    }
                }
            }
            else if ((GetAnimation().GetFlipX() && Input().IsAnyPressed(sInputKey_Right)) || (!GetAnimation().GetFlipX() && Input().IsAnyPressed(sInputKey_Left)))
            {
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
                mPreviousMotion = eAbeMotions::Motion_28_RunTurn;
                mBaseAliveGameObjectLastAnimFrame = GetAnimation().GetCurrentFrame();
            }
        }
    }
}

void Abe::Motion_28_RunTurn()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        MoveWithVelocity_4257F0(FP_FromDouble(0.375));
        if (mCurrentMotion == eAbeMotions::Motion_28_RunTurn
            && !RunTryEnterWell_425880()
            && !RunTryEnterDoor_4259C0())
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);

                const FP gridSize = ScaleToGridSize(GetSpriteScale());
                if (GetAnimation().GetFlipX())
                {
                    if (Input().IsAnyPressed(sInputKey_Run))
                    {
                        mVelX = gridSize / FP_FromInteger(4);
                        mCurrentMotion = eAbeMotions::Motion_54_RunTurnToRun;
                    }
                    else
                    {
                        mVelX = gridSize / FP_FromInteger(9);
                        mCurrentMotion = eAbeMotions::Motion_55_RunTurnToWalk;
                    }
                }
                else
                {
                    FP velX = {};
                    if (Input().IsAnyPressed(sInputKey_Run))
                    {
                        velX = gridSize / FP_FromInteger(4);
                        mCurrentMotion = eAbeMotions::Motion_54_RunTurnToRun;
                    }
                    else
                    {
                        velX = gridSize / FP_FromInteger(9);
                        mCurrentMotion = eAbeMotions::Motion_55_RunTurnToWalk;
                    }
                    mVelX = -velX;
                }
            }
        }
    }
}

void Abe::Motion_29_HopBegin()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 9)
    {
        const FP velX = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromInteger(-17) : FP_FromInteger(17));
        mVelX = velX;

        if (!field_1A0_portal)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                mVelX = FP_FromInteger(0);
                ToKnockback_422D90(1, 1);
                return;
            }
        }
        mXPos += mVelX;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        const FP velX = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        mVelX = velX;
        mXPos += velX;

        const FP velY = GetSpriteScale() * FP_FromDouble(-2.7);
        mVelY = velY;
        mYPos += velY;

        VOnTrapDoorOpen();

        mCurrentMotion = eAbeMotions::Motion_30_HopMid;
        BaseAliveGameObjectCollisionLine = nullptr;

        if (!field_1A0_portal)
        {
            field_1A0_portal = VIntoBirdPortal(2);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }
        }
    }
}

void Abe::IntoPortalStates_4262A0()
{
    switch (field_19E_portal_sub_state)
    {
        case PortalSubStates::eJumpingInsidePortal_0:
        {
            PSX_RECT bRect = VGetBoundingRect();
            if ((mVelX > FP_FromInteger(0) && FP_FromInteger(bRect.x) > field_1A0_portal->mXPos) || (mVelX < FP_FromInteger(0) && FP_FromInteger(bRect.w) < field_1A0_portal->mXPos))
            {
                GetAnimation().SetRender(false);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                field_1A0_portal->VKillPortalClipper();
                field_1A0_portal->VGiveShrykull(true);
                field_19E_portal_sub_state = PortalSubStates::eSetNewActiveCamera_1;
            }
            mVelY += GetSpriteScale() * FP_FromDouble(1.8);
            mXPos += mVelX;
            mYPos += mVelY;
            return;
        }
        case PortalSubStates::eSetNewActiveCamera_1:
        {
            if (field_1A0_portal->VAbeInsidePortal())
            {
                EReliveLevelIds level = {};
                u16 path = 0;
                u16 camera = 0;
                CameraSwapEffects screenChangeEffect = {};
                u16 movieId = 0;
                field_1A0_portal->VGetMapChange(&level, &path, &camera, &screenChangeEffect, &movieId);
                gMap.SetActiveCam(level, path, camera, screenChangeEffect, movieId, false);
                field_19E_portal_sub_state = PortalSubStates::eSetNewAbePosition_4;
            }
            break;
        }
        case PortalSubStates::eHopOutOfPortal_2:
        {
            if (field_1A0_portal->VPortalExit_AbeExitting())
            {
                field_1A0_portal->VPortalClipper(0);
                GetAnimation().SetRender(true);
                mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
                field_1A0_portal->VIncreaseTimerAndKillPortalClipper();
                field_1A0_portal = 0;
            }
            break;
        }
        case PortalSubStates::eSetNewAbePosition_4:
        {
            field_1A0_portal->VExitPortal();
            field_19E_portal_sub_state = PortalSubStates::eHopOutOfPortal_2;
            GetAnimation().SetFlipX(field_1A0_portal->mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft);

            if (GetAnimation().GetFlipX())
            {
                mXPos = ScaleToGridSize(GetSpriteScale()) + field_1A0_portal->mExitX;
            }
            else
            {
                mXPos = field_1A0_portal->mExitX - ScaleToGridSize(GetSpriteScale());
            }

            mYPos = field_1A0_portal->mExitY;
            mVelY = FP_FromInteger(0);
            field_120_x_vel_slow_by = FP_FromInteger(0);
            break;
        }
        default:
            return;
    }
}

void Abe::Motion_30_HopMid()
{
    if (field_1A0_portal)
    {
        IntoPortalStates_4262A0();
    }
    else
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
        {
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            if (gMap.VTLV_Get_At(
                    FP_GetExponent(mVelX + mXPos),
                    FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                    FP_GetExponent(mVelX + mXPos),
                    FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                    ReliveTypes::eElumStart))
            {
                SfxPlayMono(relive::SoundEffects::RingBellHammer, 0);
            }
            mNextMotion = eAbeMotions::Motion_0_Idle;
            ToKnockback_422D90(1, 1);
        }
        else
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            // this has to be called before SetActiveCameraDelayedFromDir,
            // due to both of them modifying the same private fields in a fixed order
            bool hasCollidedWithAir = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.80));

            SetActiveCameraDelayedFromDir();

            if (hasCollidedWithAir)
            {
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                switch (pLine->mLineType)
                {
                    case eLineTypes::eFloor_0:
                    case eLineTypes::eBackgroundFloor_4:
                    case eLineTypes::eDynamicCollision_32:
                    case eLineTypes::eBackgroundDynamicCollision_36:
                    {
                        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
                        BaseAliveGameObjectCollisionLine = pLine;
                        mVelY = FP_FromInteger(0);
                        mCurrentMotion = eAbeMotions::Motion_31_HopLand;

                        PSX_RECT rect = VGetBoundingRect();
                        rect.y += 5;
                        rect.h += 5;

                        OnCollisionWith(
                            {rect.x, rect.y},
                            {rect.w, rect.h},
                            gPlatformsArray);
                        mXPos = hitX;
                        mYPos = hitY;
                        mVelX = FP_FromInteger(0);
                        MapFollowMe(1);
                        mNextMotion = eAbeMotions::Motion_0_Idle;
                        return;
                    }
                    default:
                        break;
                }
            }
            if (GetAnimation().GetIsLastFrame())
            {
                if (mCurrentMotion == eAbeMotions::Motion_30_HopMid)
                {
                    mVelX = FP_FromRaw(mVelX.fpValue / 2);
                    if (GetAnimation().GetFlipX())
                    {
                        mXPos += GetSpriteScale() * FP_FromInteger(5);
                    }
                    else
                    {
                        mXPos -= GetSpriteScale() * FP_FromInteger(5);
                    }
                    field_120_x_vel_slow_by = FP_FromDouble(0.55);
                    mYPos += GetSpriteScale() * FP_FromInteger(2);
                    mCurrentMotion = eAbeMotions::Motion_96_HopToFall;
                    mNextMotion = eAbeMotions::Motion_0_Idle;
                }
            }
        }
    }
}

void Abe::Motion_31_HopLand()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 2 && Input().IsAnyPressed(sInputKey_Hop))
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
        mPreviousMotion = eAbeMotions::Motion_29_HopBegin;
        mBaseAliveGameObjectLastAnimFrame = 5;
    }
    else
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (!ToLeftRightMovement_422AA0())
            {
                ToIdle_422D50();
            }
        }
    }
}

void Abe::Motion_32_RunJumpBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        FollowLift_42EE90();

        mXPos += mVelX;
        if (GetAnimation().GetCurrentFrame() == 0)
        {
            Environment_SFX_42A220(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
        }

        if (GetAnimation().GetIsLastFrame())
        {
            BaseAliveGameObjectLastLineYPos = mYPos;

            if (GetAnimation().GetFlipX())
            {
                mVelX = (GetSpriteScale() * FP_FromDouble(-7.6));
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromDouble(7.6));
            }

            mVelY = (GetSpriteScale() * FP_FromDouble(-9.6));
            mYPos += mVelY;
            VOnTrapDoorOpen();
            mCurrentMotion = eAbeMotions::Motion_33_RunJumpMid;
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

void Abe::Motion_33_RunJumpMid()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (field_1A0_portal)
    {
        IntoPortalStates_4262A0();
        return;
    }
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        if (gMap.VTLV_Get_At(
                FP_GetExponent(mVelX + mXPos),
                FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                FP_GetExponent(mVelX + mXPos),
                FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(50)),
                ReliveTypes::eElumStart))
        {
            SfxPlayMono(relive::SoundEffects::RingBellHammer, 0);
        }
        mNextMotion = eAbeMotions::Motion_0_Idle;
        ToKnockback_422D90(1, 1);
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eAbeMotions::Motion_34_RunJumpLand;
                mXPos = hitX;
                mYPos = hitY;

                PSX_RECT rect = VGetBoundingRect();
                rect.y += 5;
                rect.h += 5;

                if (pLine->mLineType == eLineTypes::eDynamicCollision_32)
                {
                    OnCollisionWith(
                        {rect.x, rect.y},
                        {rect.w, rect.h},
                        gPlatformsArray);
                }
                mNextMotion = eAbeMotions::Motion_0_Idle;
                return;
            }
        }
    }
    else
    {
        auto pHoist = static_cast<relive::Path_Hoist*>(gMap.VTLV_Get_At(
            FP_GetExponent(mXPos - mVelX),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos - mVelX),
            FP_GetExponent(mYPos),
            ReliveTypes::eHoist));

        bool checkCollision = false;
        if (pHoist)
        {
            BaseAliveGameObjectPathTLV = pHoist;

            if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this)) && pHoist->mHoistType != relive::Path_Hoist::Type::eOffScreen)
            {
                checkCollision = true;
            }
        }
        else
        {
            auto pEdgeTlv = static_cast<relive::Path_Edge*>(gMap.VTLV_Get_At(
                FP_GetExponent(mXPos - mVelX),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos - mVelX),
                FP_GetExponent(mYPos),
                ReliveTypes::eEdge));
            BaseAliveGameObjectPathTLV = pEdgeTlv;
            if (pEdgeTlv && pEdgeTlv->mCanGrab == relive::reliveChoice::eYes)
            {
                if (isEdgeGrabbable(pEdgeTlv, this))
                {
                    checkCollision = true;
                }
            }
        }
        if (checkCollision)
        {
            if (sCollisions->Raycast(
                    mXPos,
                    mYPos - GetSpriteScale() * FP_FromInteger(100),
                    mXPos,
                    mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
            {
                mXPos = FP_FromInteger(
                    (BaseAliveGameObjectPathTLV->mBottomRightX + BaseAliveGameObjectPathTLV->mTopLeftX) / 2);
                MapFollowMe(true);
                mYPos = hitY;
                mCurrentMotion = eAbeMotions::Motion_68_LedgeHangWobble;
                GetShadow()->mShadowAtBottom = true;
                BaseAliveGameObjectCollisionLine = pLine;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
                mNextMotion = eAbeMotions::Motion_0_Idle;
                if (!pLiftPoint)
                {
                    if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                        pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                    {
                        PSX_RECT rect = VGetBoundingRect();
                        rect.y += 5;
                        rect.h += 5;

                        OnCollisionWith(
                            {rect.x, rect.y},
                            {rect.w, rect.h},
                            gPlatformsArray);
                    }
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (mCurrentMotion == eAbeMotions::Motion_33_RunJumpMid)
        {
            field_120_x_vel_slow_by = FP_FromDouble(0.75);
            mCurrentMotion = eAbeMotions::Motion_97_RunJumpToFall;
            mNextMotion = eAbeMotions::Motion_0_Idle;
        }
    }
}

void Abe::Motion_34_RunJumpLand()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        MapFollowMe(1);

        if (Input().IsAnyPressed(sInputKey_Left))
        {
            if (!(sInputKey_Hop & field_10C_prev_held))
            {
                if (GetAnimation().GetFlipX())
                {
                    if (Input().IsAnyPressed(sInputKey_Run))
                    {
                        mCurrentMotion = eAbeMotions::Motion_56_RunJumpLandRun;
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    }
                    else
                    {
                        if (WallHit(GetSpriteScale() * FP_FromInteger(50), -ScaleToGridSize(GetSpriteScale())))
                        {
                            mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                        }
                        else
                        {
                            mCurrentMotion = eAbeMotions::Motion_57_RunJumpLand_Walk;
                            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        }
                    }
                    return;
                }

                mCurrentMotion = eAbeMotions::Motion_28_RunTurn;
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                return;
            }

            field_1A0_portal = VIntoBirdPortal(3);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }

            mCurrentMotion = eAbeMotions::Motion_32_RunJumpBegin;
            field_10C_prev_held = 0;
            return;
        }

        if (Input().IsAnyPressed(sInputKey_Right))
        {
            if (sInputKey_Hop & field_10C_prev_held)
            {
                field_1A0_portal = VIntoBirdPortal(3);
                if (field_1A0_portal)
                {
                    field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                }

                mCurrentMotion = eAbeMotions::Motion_32_RunJumpBegin;
                field_10C_prev_held = 0;
                return;
            }

            if (GetAnimation().GetFlipX())
            {
                mCurrentMotion = eAbeMotions::Motion_28_RunTurn;
                Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                return;
            }

            if (Input().IsAnyPressed(sInputKey_Run))
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                mCurrentMotion = eAbeMotions::Motion_56_RunJumpLandRun;
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(50), ScaleToGridSize(GetSpriteScale())))
                {
                    mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                    mCurrentMotion = eAbeMotions::Motion_57_RunJumpLand_Walk;
                }
            }
        }
        else
        {
            if (!(sInputKey_Hop & field_10C_prev_held))
            {
                mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
                if (GetAnimation().GetFlipX())
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                }
                else
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
                }
                return;
            }

            field_1A0_portal = VIntoBirdPortal(3);
            if (field_1A0_portal)
            {
                field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
            }

            mCurrentMotion = eAbeMotions::Motion_29_HopBegin;
            field_10C_prev_held = 0;
        }
    }
}

bool Abe::CheckForPortalAndRunJump()
{
    if (field_10C_prev_held & sInputKey_Hop)
    {
        field_1A0_portal = VIntoBirdPortal(3);
        if (field_1A0_portal)
        {
            field_19E_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
        }

        mCurrentMotion = eAbeMotions::Motion_32_RunJumpBegin;
        field_10C_prev_held = 0;
        return true;
    }
    return false;
}

void Abe::Motion_35_RunLoop()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback_422D90(1, 1);
        return;
    }

    MoveForward_422FC0();

    if (mCurrentMotion != eAbeMotions::Motion_35_RunLoop)
    {
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 8)
    {
        if (!field_2A8_flags.Get(e2A8_Bit3_WalkToRun))
        {
            field_2A8_flags.Set(e2A8_Bit3_WalkToRun);
            MapFollowMe(true);
        }

        CheckForPortalAndRunJump();
    }
    else if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRunningFootstep_2, 0, 0x7FFF, this);

        // Snap
        if (!field_2A8_flags.Get(e2A8_Bit3_WalkToRun))
        {
            field_2A8_flags.Set(e2A8_Bit3_WalkToRun);
            MapFollowMe(true);
        }

        // Check turning in middle of run (pressing reverse direction of movement)
        if ((mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right)))
        {
            mCurrentMotion = eAbeMotions::Motion_28_RunTurn;
            Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            field_10C_prev_held = 0;
            return;
        }

        // Check jumping into a portal
        if (CheckForPortalAndRunJump())
        {
            return;
        }

        // Running to roll
        if (field_10C_prev_held & sInputKey_FartRoll)
        {
            mCurrentMotion = eAbeMotions::Motion_40_RunToRoll;

            // For some reason dont clear released in the frame 12 case
            if (GetAnimation().GetCurrentFrame() == 4)
            {
                field_10E_released_buttons = 0;
            }
            field_10C_prev_held = 0;
            return;
        }

        // No longer running
        if (!Input().IsAnyPressed(sInputKey_Right) && !Input().IsAnyPressed(sInputKey_Left))
        {
            mCurrentMotion = eAbeMotions::Motion_27_RunSlideStop;
            Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            field_10C_prev_held = 0;
            return;
        }

        // Continue running
        if (Input().IsAnyPressed(sInputKey_Run))
        {
            field_10C_prev_held = 0;
            return;
        }

        // Stop running
        FP gridSize = {};
        if (mVelX >= FP_FromInteger(0))
        {
            gridSize = ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize = -ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize))
        {
            ToKnockback_422D90(1, 1);
        }
        else
        {
            if (GetAnimation().GetCurrentFrame() == 4)
            {
                mCurrentMotion = eAbeMotions::Motion_52_RunToWalk;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_53_MidRunToWalk;
            }
        }

        field_10C_prev_held = 0;
    }
    else
    {
        field_2A8_flags.Clear(e2A8_Bit3_WalkToRun);
    }
}

void Abe::Motion_36_DunnoBegin()
{
    FollowLift_42EE90();

    if (GetAnimation().GetIsLastFrame())
    {
        Mudokon_SFX(MudSounds::eDunno_15, 0, 0, this);

        if (Input().IsAnyPressed(sInputKey_DoAction | sInputKey_ThrowItem))
        {
            mCurrentMotion = eAbeMotions::Motion_37_DunnoMid;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_38_DunnoEnd;
        }
    }
}

void Abe::Motion_37_DunnoMid()
{
    FollowLift_42EE90();

    if (!Input().IsAnyPressed(sInputKey_DoAction | sInputKey_ThrowItem) || GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_38_DunnoEnd;
    }
}

void Abe::Motion_38_DunnoEnd()
{
    FollowLift_42EE90();

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_39_CrouchTurn()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().ToggleFlipX();

            if (mNextMotion == eAbeMotions::Motion_0_Idle)
            {
                mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
            }
            else
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = 0;
            }
        }
    }
    else
    {
        Environment_SFX_42A220(EnvironmentSfx::eGenericMovement_9, 0, 0x7FFF, this);
    }
}

void Abe::Motion_40_RunToRoll()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_422D90(1, 1);
        mCurrentMotion = eAbeMotions::Motion_73_RollingKnockback;
    }
    else
    {
        MoveForward_422FC0();

        if (GetAnimation().GetIsLastFrame())
        {
            if (mCurrentMotion == eAbeMotions::Motion_40_RunToRoll)
            {
                mCurrentMotion = eAbeMotions::Motion_25_RollLoop;
            }
        }
    }
}

void Abe::Motion_41_StandingToRun()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10C_prev_held |= Input().Pressed();

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_42_SneakLoop()
{
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
        return;
    }

    MoveForward_422FC0();

    if (mCurrentMotion == eAbeMotions::Motion_42_SneakLoop)
    {
        if (GetAnimation().GetCurrentFrame() == 3)
        {
            FP directedScale = {};
            if (GetAnimation().GetFlipX())
            {
                directedScale = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                directedScale = ScaleToGridSize(GetSpriteScale());
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale) || (mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right)) || !Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
            {
                mCurrentMotion = eAbeMotions::Motion_48_SneakToIdle;
            }
            return;
        }

        if (GetAnimation().GetCurrentFrame() == 6)
        {
            Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
            MapFollowMe(1);

            if (Input().IsAnyPressed(sInputKey_Right | sInputKey_Left) && !Input().IsAnyPressed(sInputKey_Sneak))
            {
                mCurrentMotion = eAbeMotions::Motion_44_SneakToWalk;
                field_10C_prev_held = 0;
                return;
            }

            field_10C_prev_held = 0;
            return;
        }

        if (GetAnimation().GetCurrentFrame() != 13)
        {
            if (GetAnimation().GetCurrentFrame() != 16)
            {
                return;
            }

            Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
            MapFollowMe(1);

            if (Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
            {
                if (!Input().IsAnyPressed(sInputKey_Sneak))
                {
                    mCurrentMotion = eAbeMotions::Motion_46_MidSneakToWalk;
                }
            }
            field_10C_prev_held = 0;
            return;
        }

        FP directedScale = {};
        if (GetAnimation().GetFlipX())
        {
            directedScale = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            directedScale = ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(50), directedScale) || (mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right)) || !Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
        {
            mCurrentMotion = eAbeMotions::Motion_49_MidSneakToIdle;
        }
    }
}

void Abe::Motion_43_WalkToSneak()
{
    field_10C_prev_held |= Input().Pressed();

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_42_SneakLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_44_SneakToWalk()
{
    field_10C_prev_held |= Input().Pressed();

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_45_MidWalkToSneak()
{
    field_10C_prev_held |= Input().Pressed();

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
        mPreviousMotion = 42;
        mBaseAliveGameObjectLastAnimFrame = 10;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_46_MidSneakToWalk()
{
    field_10C_prev_held |= Input().Pressed();

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
        mPreviousMotion = 1;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_47_SneakBegin()
{
    field_10C_prev_held |= Input().Pressed();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_42_SneakLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_48_SneakToIdle()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
    }

    MoveForward_422FC0();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);
        ToIdle_422D50();
    }
}

void Abe::Motion_49_MidSneakToIdle()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX_42A220(EnvironmentSfx::eSneakFootstep_3, 0, 0x7FFF, this);
    }

    MoveForward_422FC0();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);

        ToIdle_422D50();
    }
}

void Abe::Motion_50_WalkToRun()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
        field_10C_prev_held = 0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_51_MidWalkToRun()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
        mPreviousMotion = 35;
        mBaseAliveGameObjectLastAnimFrame = 8;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
        MapFollowMe(1);
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_52_RunToWalk()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_53_MidRunToWalk()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2_return_to_previous_motion);
        mPreviousMotion = 1;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();
    }
}

void Abe::Motion_54_RunTurnToRun()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
            GetAnimation().ToggleFlipX();
        }
    }
}

void Abe::Motion_55_RunTurnToWalk()
{
    field_10C_prev_held |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);


    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
            GetAnimation().ToggleFlipX();
        }
    }
}

void Abe::Motion_56_RunJumpLandRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
        }
    }
}

void Abe::Motion_57_RunJumpLand_Walk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_422D50();
    }
    else
    {
        MoveForward_422FC0();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
        }
    }
}

void Abe::Motion_58_ToSpeak()
{
    FollowLift_42EE90();

    field_10C_prev_held |= Input().Held();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = DoGameSpeak_42F5C0(field_10C_prev_held);
        if (mCurrentMotion == -1)
        {
            ToIdle_422D50();
        }
        else
        {
            EventBroadcast(kEventSpeaking, this);
        }
        field_10C_prev_held = 0;
    }
}

void Abe::Motion_59_DeathDropFall()
{
    GetAnimation().SetAnimate(false);

    FollowLift_42EE90();

    if (field_114_gnFrame == 0)
    {
        field_120_x_vel_slow_by = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        field_118_timer = sGnFrame + 90;
        field_114_gnFrame = 1;
    }
    else if (field_114_gnFrame == 1)
    {
        if (static_cast<s32>(sGnFrame) == field_118_timer - 30)
        {
            SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
        }
        else if (static_cast<s32>(sGnFrame) == field_118_timer - 24)
        {
            Environment_SFX_42A220(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

            relive_new ScreenShake(true);
        }
        else if (static_cast<s32>(sGnFrame) >= field_118_timer)
        {
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit6_bShrivel);
            mCurrentMotion = eAbeMotions::Motion_60_Dead;
            field_114_gnFrame = 0;
            mHealth = FP_FromInteger(0);
            MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_14, this, 1, 0);
        }
    }
}

void Abe::Motion_60_Dead()
{
    GetAnimation().SetAnimate(false);
    FollowLift_42EE90();

    switch (field_114_gnFrame)
    {
        case 0:
        {
            EventBroadcast(kEventHeroDying, this);
            field_118_timer = sGnFrame + 30;
            field_120_x_vel_slow_by = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            field_10C_prev_held = 0;
            field_114_gnFrame++;

            auto aux = 0;
            if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
            {
                aux = 60;
            }
            else
            {
                aux = 15;
            }
            const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
            const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
            relive_new DeathBirdParticle(
                xpos,
                ypos,
                (Math_NextRandom() % 8) + field_118_timer + aux,
                true,
                GetSpriteScale());

            return;
        }
        case 1:
        {
            EventBroadcast(kEventHeroDying, this);
            if (!(sGnFrame % 4))
            {
                auto aux = 0;
                if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
                {
                    aux = 60;
                }
                else
                {
                    aux = 15;
                }
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
                relive_new DeathBirdParticle(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_118_timer + aux,
                    false,
                    GetSpriteScale());
            }
            SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));

            mRGB.r -= 2;
            mRGB.g -= 2;
            mRGB.b -= 2;
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                field_118_timer = sGnFrame + 60;
                if (BaseAliveGameObjectPathTLV)
                {
                    if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
                    {
                        field_118_timer = (sGnFrame + 60) + 45;
                    }
                }
                field_114_gnFrame++;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_14, this, 1, 0);
            }
            return;
        }
        case 2:
        {
            EventBroadcast(kEventHeroDying, this);
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                field_114_gnFrame++;
            }
            return;
        }
        case 3:
        {
            EventBroadcast(kEventHeroDying, this);
            if (mFade)
            {
                mFade->SetDead(true);
                mFade->mBaseGameObjectRefCount--;
            }
            mFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8, TPageAbr::eBlend_2);
            mFade->mBaseGameObjectRefCount++;

            if (mCircularFade)
            {
                mCircularFade->SetDead(true);
                mCircularFade = nullptr;
            }
            field_114_gnFrame++;
            return;
        }
        case 4:
        {
            EventBroadcast(kEventHeroDying, this);
            if (mFade->mDone)
            {
                VOnTrapDoorOpen();
                BaseAliveGameObjectCollisionLine = nullptr;
                if (gElum)
                {
                    if (!gElum->mRespawnOnDead)
                    {
                        gElum->SetDead(true);
                    }
                }

                if (mRidingElum)
                {
                    mRidingElum = false;
                }

                field_118_timer = sGnFrame + 8;
                field_114_gnFrame++;
            }
            return;
        }
        case 5:
        {
            EventBroadcast(kEventDeathReset, this);
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                mCurrentMotion = eAbeMotions::Motion_61_Respawn;
                field_118_timer = sGnFrame + 2;
                field_114_gnFrame = 0;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 1, 0);
                if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
                {

                }
                else if (mContinueLevel != EReliveLevelIds::eRuptureFarmsReturn || mContinuePath != 6)
                {
                    field_168_ring_pulse_timer = 0;
                }
                field_130_say = -1;
                GetShadow()->mEnabled = false;
            }
            return;
        }
        default:
            return;
    }
}

void Abe::Motion_61_Respawn()
{
    // This will crash the game if abe didnt touch a checkpoint yet

    if (gAttract)
    {
        // Depending on the randomn seed value abe can die in a demo, if so the checkpoint save will load
        // depending on the saved camera number on returning the main menu this can crash. Hack fix it
        // by killing abe so the bad save cant get loaded before we return to the menu.
        LOG_WARNING("Destroying abe to prevent game crash, he isnt supposed to die in a demo!");
        SetDead(true);
        sActiveHero = nullptr;
        sControlledCharacter = nullptr;
        return;
    }

    auto pLiftPoint = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    GetAnimation().SetAnimate(false);
    FollowLift_42EE90();
    EventBroadcast(kEventResetting, this);

    switch (field_114_gnFrame)
    {
        case 0:
        {
            mHealth = FP_FromInteger(1);
            if (field_19C_throwable_count != 0)
            {
                if (gThrowableArray)
                {
                    gThrowableArray->Remove(field_19C_throwable_count);
                }
            }
            field_19C_throwable_count = 0;

            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                if (pLiftPoint)
                {
                    if (pLiftPoint->GetDrawable())
                    {
                        pLiftPoint->VRemove(this);
                        BaseAliveGameObject_PlatformId = Guid{};
                    }
                }
                if (mContinueCamera > 300u)
                {
                    mContinueCamera = gMap.mCurrentCamera;
                    mContinuePath = gMap.mCurrentPath;
                    mContinueLevel = gMap.mCurrentLevel;

                    PSX_Point camPos = {};
                    gMap.GetCurrentCamCoords(&camPos);
                    mContinueTopLeft.x = camPos.x + 512;
                    mContinueTopLeft.y = camPos.y + 240;
                }
                SaveGame::LoadFromMemory(&gSaveBuffer, 0);
                if (field_19C_throwable_count)
                {
                    LoadRockTypes(MapWrapper::FromAO(gSaveBuffer.field_234_current_level), gSaveBuffer.field_236_current_path);
                    if (!gThrowableArray)
                    {
                        gThrowableArray = relive_new ThrowableArray();
                    }
                    gThrowableArray->Add(field_19C_throwable_count);
                }
                field_114_gnFrame = 1;
                mCurrentMotion = eAbeMotions::Motion_61_Respawn;
            }
            break;
        }
        case 1:
        {
            mCurrentLevel = mContinueLevel;
            mCurrentPath = mContinuePath;
            mXPos = FP_FromInteger(mContinueTopLeft.x);
            mYPos = FP_FromInteger(mContinueTopLeft.y) + GetSpriteScale() * FP_FromInteger(30);

            BaseAliveGameObjectLastLineYPos = mYPos;

            GetAnimation().SetFlipX(field_2A8_flags.Get(Flags_2A8::eAbeSpawnDir));
            MapFollowMe(true);
            SetTint(sAbeTints_4C6438, gMap.mCurrentLevel);
            if (gElum)
            {
                gElum->SetSpriteScale(GetSpriteScale());
            }
            if (GetSpriteScale() == FP_FromDouble(0.5))
            {
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                SetScale(Scale::Bg);
                if (gElum)
                {
                    gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesMudsElum_Half_9);
                    gElum->SetScale(Scale::Bg);
                }
            }
            else
            {
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                SetScale(Scale::Fg);
                if (gElum)
                {
                    gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesElumMuds_28);
                    gElum->SetScale(Scale::Fg);
                }
            }
            GetAnimation().SetRender(false);
            EventBroadcast(kEventDeathResetEnd, this);

            //TODO not used?
            //if (dword_507720)
            //{
            //    *(_BYTE *) (dword_507720 + 6) |= 4u;
            //}
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eElectrocuted);

            mFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 1, 8);
            mFade->mBaseGameObjectRefCount--;
            mFade = nullptr;
            mNextMotion = eAbeMotions::Motion_0_Idle;
            field_118_timer = sGnFrame + 60;
            field_114_gnFrame = 2;
            break;
        }
        case 2:
        {
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                auto xDiff = pScreenManager->CamXPos();
                auto yDiff = pScreenManager->CamYPos();

                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        xDiff + FP_FromInteger(Math_NextRandom() * 2),
                        yDiff - FP_FromInteger(Math_NextRandom() % 32),
                        GetSpriteScale());
                    if (pDove)
                    {
                        pDove->AsJoin(
                            mXPos + FP_FromInteger((Math_NextRandom() % 16) - 8),
                            mYPos + FP_FromInteger((Math_NextRandom() % 16) - 20));
                        pDove->SetSpriteScale(GetSpriteScale());
                    }
                }
                SfxPlayMono(relive::SoundEffects::RespawnDove, 0, GetSpriteScale());
                field_118_timer = sGnFrame + 45;
                field_114_gnFrame = 3;
            }
            break;
        }
        case 3:
        {
            if (field_118_timer - sGnFrame == 10)
            {
                SfxPlayMono(relive::SoundEffects::Respawn, 90);
            }
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                New_DestroyOrCreateObject_Particle(
                    mXPos,
                    GetSpriteScale() * FP_FromInteger(25) + mYPos,
                    GetSpriteScale());
                field_114_gnFrame = 4;
                field_118_timer = sGnFrame + 2;
            }
            break;
        }
        case 4:
        {
            if (static_cast<s32>(sGnFrame) > field_118_timer)
            {
                GetShadow()->mEnabled = true;
                GetShadow()->mShadowAtBottom = false;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);
                GetAnimation().SetRender(true);
                SetDrawable(true);
                mCurrentMotion = eAbeMotions::Motion_3_Fall;

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
                mbGotShot = false;
                field_2A8_flags.Clear(Flags_2A8::e2A8_Bit6_bShrivel);
                field_114_gnFrame = sGnFrame;
            }
            break;
        }
        default:
            return;
    }
}

void Abe::Motion_62_LoadedSaveSpawn()
{
    EventBroadcast(kEventResetting, this);

    if (field_114_gnFrame)
    {
        auto pSaveData = field_2AC_pSaveData;
        mYPos = FP_FromInteger(pSaveData->field_228_ypos);
        mXPos = FP_FromInteger(pSaveData->field_224_xpos);

        PathLine* pLine2 = nullptr;
        FP hitX2 = {};
        FP hitY2 = {};
        if (sCollisions->Raycast(
                sActiveHero->mXPos,
                sActiveHero->mYPos - FP_FromInteger(60),
                sActiveHero->mXPos,
                sActiveHero->mYPos + FP_FromInteger(60),
                &pLine2,
                &hitX2,
                &hitY2,
                CollisionMask(static_cast<eLineTypes>(pSaveData->field_23A_mode_mask))))
        {
            sActiveHero->BaseAliveGameObjectCollisionLine = pLine2;
            sActiveHero->mYPos = hitY2;
            sActiveHero->mCurrentMotion = eAbeMotions::Motion_0_Idle;
        }
        else
        {
            sActiveHero->mCurrentMotion = eAbeMotions::Motion_3_Fall;
        }
        sActiveHero->field_2A8_flags.Clear(Flags_2A8::e2A8_Bit8_bLandSoft);
        sActiveHero->BaseAliveGameObjectLastLineYPos = sActiveHero->mYPos;
        sActiveHero->field_110_state.raw = static_cast<s16>(pSaveData->field_244_stone_state);
        sActiveHero->field_114_gnFrame = pSaveData->field_248_gnFrame;
        sActiveHero->mBaseAliveGameObjectLastAnimFrame = pSaveData->field_240_last_anim_frame;
        sActiveHero->GetAnimation().SetFlipX(pSaveData->field_23C_ah_flipX & 1);
        sActiveHero->MapFollowMe(true);
        sActiveHero->GetAnimation().SetRender(true);
        if (sActiveHero->field_19C_throwable_count)
        {
            if (!gThrowableArray)
            {
                LoadRockTypes(MapWrapper::FromAO(gSaveBuffer.field_234_current_level), gSaveBuffer.field_236_current_path);

                gThrowableArray = relive_new ThrowableArray();
            }
            gThrowableArray->Add(sActiveHero->field_19C_throwable_count);
        }
        if (pSaveData->field_264_bInfiniteGrenades == -1)
        {
            LoadRockTypes(EReliveLevelIds::eRuptureFarmsReturn, 19);
            if (!gThrowableArray)
            {
                gThrowableArray = relive_new ThrowableArray();
            }
            gThrowableArray->Add(1);
            gInfiniteGrenades = true;
        }
        else
        {
            gInfiniteGrenades = false;
        }
        if (pSaveData->field_25A_bElumExists)
        {
            if (!gElum)
            {
                Elum::Spawn(Guid{});
            }

            if (gElum)
            {
                gElum->SetUpdatable(false);
                gElum->GetAnimation().SetRender(false);
                gElum->mContinueRect = pSaveData->field_28C_elum_continue_rect;
                gElum->mPreviousContinueZoneNumber = pSaveData->field_294_continue_zone_number;
                gElum->mAbeZoneNumber = pSaveData->field_296_elum_zone_number;
                gElum->mContinuePath = pSaveData->field_298_elum_continue_path;
                gElum->mContinueLevel = MapWrapper::FromAO(pSaveData->field_29A_continue_level);
                gElum->mContinueSpriteScale = pSaveData->field_29C_elum_sprite_scale;
                gElum->mRespawnOnDead = pSaveData->field_25E_bElumRespawnOnDead;
                gElum->mCurrentLevel = MapWrapper::FromAO(pSaveData->field_260_elum_lvl_number);
                gElum->mCurrentPath = pSaveData->field_262_elum_path_number;
                gElum->mXPos = FP_FromInteger(pSaveData->field_268_elum_xpos);
                gElum->mYPos = FP_FromInteger(pSaveData->field_26C_elum_ypos);
                gElum->GetAnimation().SetFlipX(pSaveData->field_272_elum_flipX & 1);
                gElum->mBaseAliveGameObjectLastAnimFrame = 0;
                gElum->field_120_bUnknown = 1;
                gElum->mDontFollowAbe = pSaveData->field_276_bDontFollowAbe;
                gElum->mBrainIdx = pSaveData->field_278_brain_idx;
                gElum->mBrainSubState = pSaveData->field_27A_elum_brain_state;
                gElum->field_12C_honey_xpos = static_cast<s16>(pSaveData->field_27C_honey_xpos);
                gElum->field_146_honey_ypos = pSaveData->field_280_honey_ypos;

                gElum->field_170_flags.Set(Elum::Flags_170::eFoundHoney_Bit4, pSaveData->field_28B_elum_FoundHoney & 1);
                gElum->field_170_flags.Set(Elum::Flags_170::eFalling_Bit3, pSaveData->field_28A_elum_Falling & 1);
                gElum->field_170_flags.Set(Elum::Flags_170::eStungByBees_Bit2, pSaveData->field_289_elum_StungByBees & 1);
                if (gElum->mCurrentPath == sActiveHero->mCurrentPath)
                {
                    if (pSaveData->field_270_elum_line_type != -1)
                    {
                        PathLine* pLine = nullptr;
                        FP hitX = {};
                        FP hitY = {};
                        if (sCollisions->Raycast(
                            gElum->mXPos,
                            gElum->mYPos - FP_FromInteger(60),
                            gElum->mXPos,
                            gElum->mYPos + FP_FromInteger(60),
                            &pLine,
                            &hitX,
                            &hitY,
                            CollisionMask(static_cast<eLineTypes>(pSaveData->field_270_elum_line_type))))
                        {
                            gElum->BaseAliveGameObjectCollisionLine = pLine;
                            gElum->SetCurrentMotion(eElumMotions::Motion_1_Idle);
                            gElum->SetPreviousMotion(eElumMotions::Motion_1_Idle);
                        }
                        else
                        {
                            gElum->SetCurrentMotion(eElumMotions::Motion_21_Land);
                            gElum->SetPreviousMotion(eElumMotions::Motion_21_Land);
                        }
                    }
                }
                else
                {
                    //TODO fix this madness
                    gElum->BaseAliveGameObjectCollisionLine = reinterpret_cast<PathLine*>(-2);
                }

                if (gElum->field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
                {
                    gElum->SetCurrentMotion(eElumMotions::Motion_25_LickingHoney);
                    gElum->SetPreviousMotion(eElumMotions::Motion_25_LickingHoney);
                }
                gElum->MapFollowMe(true);
                gElum->SetUpdatable(true);
                gElum->GetAnimation().SetRender(true);
            }
        }
    }
    else
    {
        field_114_gnFrame = 1;
    }
}

void Abe::Motion_63_TurnToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        MoveForward_422FC0();

        if (mCurrentMotion == eAbeMotions::Motion_63_TurnToRun)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eAbeMotions::Motion_35_RunLoop;
            }
        }
    }
}

void Abe::Motion_64_LedgeAscend()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX_42A220(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 0x7FFF, this);
    }
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
        GetShadow()->mShadowAtBottom = false;
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);
        ToIdle_422D50();
    }
}

void Abe::Motion_65_LedgeDescend()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 0x7FFF, this);
        GetShadow()->mShadowAtBottom = !GetShadow()->mShadowAtBottom;
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
    }
}

void Abe::Motion_66_LedgeHang()
{
    FollowLift_42EE90();

    GetShadow()->mShadowAtBottom = true;

    if (Input().IsAnyPressed(sInputKey_Up))
    {
        mCurrentMotion = eAbeMotions::Motion_64_LedgeAscend;
    }
    else if (Input().IsAnyPressed(sInputKey_Down))
    {
        VOnTrapDoorOpen();

        BaseAliveGameObjectCollisionLine = nullptr;
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
        mYPos += (GetSpriteScale() * FP_FromInteger(80)) + FP_FromInteger(3);
        BaseAliveGameObjectLastLineYPos = mYPos;
        GetShadow()->mShadowAtBottom = false;
    }
}

void Abe::Motion_67_ToOffScreenHoist()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    mYPos -= (GetSpriteScale() * FP_FromInteger(80));

    GetShadow()->mShadowAtBottom = !GetShadow()->mShadowAtBottom;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos - (GetSpriteScale() * FP_FromInteger(80)),
            mXPos,
            mYPos,
            &pLine,
            &hitX,
            &hitY,
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
        BaseAliveGameObjectCollisionLine = pLine;
        mVelY = FP_FromInteger(0);
        if (!pLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    gPlatformsArray);
            }
        }
        mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
    }
    else
    {
        mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
    }
}

void Abe::Motion_68_LedgeHangWobble()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
        {
            field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
            Environment_SFX_42A220(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
        }
    }
    else if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
        {
            field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
            Mudokon_SFX(MudSounds::eBeesStruggle_18, 45, -200, this);
        }
    }
    else
    {
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    FollowLift_42EE90();

    if (Input().IsAnyPressed(sInputKey_Up))
    {
        field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);
        mCurrentMotion = eAbeMotions::Motion_64_LedgeAscend;
    }
    else
    {
        if (Input().IsAnyPressed(sInputKey_Down))
        {
            field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);

            VOnTrapDoorOpen();
            BaseAliveGameObjectCollisionLine = nullptr;

            mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
            mYPos += (GetSpriteScale() * FP_FromInteger(80)) + FP_FromInteger(3);
            GetShadow()->mShadowAtBottom = false;
            BaseAliveGameObjectLastLineYPos = mYPos;
        }
        else if (GetAnimation().GetIsLastFrame())
        {
            field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);
            mCurrentMotion = eAbeMotions::Motion_66_LedgeHang;
        }
    }
}

void Abe::Motion_69_RingRopePullHang()
{
    if (field_160_pRope->vIsNotBeingPulled())
    {
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
        field_160_pRope->mBaseGameObjectRefCount--;
        field_160_pRope = nullptr;
        mVelY = FP_FromInteger(0);
    }
}

void Abe::Motion_70_Knockback()
{
    ElumFree_4228F0();

    if (field_1A0_portal)
    {
        field_1A0_portal->VKillPortalClipper();
        field_1A0_portal = nullptr;
    }

    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_4231B0();
    }

    if (GetAnimation().GetRender())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                mVelX = FP_FromInteger(0);
            }

            MoveWithVelocity_4257F0(FP_FromDouble(0.7));

            if ((gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                 || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn
                 || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom)
                && GetAnimation().GetCurrentFrame() == 7)
            {
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            Motion_3_Fall();

            if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie || mCurrentMotion == eAbeMotions::Motion_98_LandSoft)
            {
                field_114_gnFrame = sGnFrame + 10;
                mCurrentMotion = eAbeMotions::Motion_70_Knockback;
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
            }
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if ((BaseAliveGameObjectCollisionLine || !GetAnimation().GetRender()))
        {
            if (mRidingElum)
            {
                mRidingElum = false;
            }

            if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
            {
                if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
                {
                    mCurrentMotion = eAbeMotions::Motion_71_KnockbackGetUp;
                }
                else
                {
                    ToDieFinal_42C400();
                }
            }
        }
    }
}

void Abe::Motion_71_KnockbackGetUp()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_72_PushWall()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 10)
    {
        if (Math_NextRandom() <= 127u)
        {
            Environment_SFX_42A220(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 0x7FFF, this);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_73_RollingKnockback()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_4231B0();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
            {
                mCurrentMotion = eAbeMotions::Motion_71_KnockbackGetUp;
            }
            else
            {
                ToDieFinal_42C400();
            }
        }
    }
}

void Abe::Motion_74_JumpIntoWell()
{
    GetShadow()->mEnabled = false;

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
    }

    Motion_77_WellBegin();
}

void Abe::Motion_75_ToInsideOfAWellLocal()
{
    Motion_78_InsideWellLocal();
}

void Abe::Motion_76_ToWellShotOut()
{
    Motion_79_WellShotOut();
}

void Abe::Motion_77_WellBegin()
{
    EventBroadcast(kEventNoise, this);
    if (GetAnimation().GetCurrentFrame() > 10)
    {
        GetShadow()->mEnabled = false;

        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellLocal);
        if (!BaseAliveGameObjectPathTLV)
        {
            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress);
        }

        const s16 xpos = FP_GetExponent(mXPos);
        const auto tlv_mid_x = (BaseAliveGameObjectPathTLV->mBottomRightX + BaseAliveGameObjectPathTLV->mTopLeftX) / 2;
        if (xpos > tlv_mid_x)
        {
            mXPos -= GetSpriteScale();
        }
        else if (xpos < tlv_mid_x)
        {
            mXPos += GetSpriteScale();
        }
    }
    if (GetAnimation().GetCurrentFrame() == 11)
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
        }
    }
    if (GetAnimation().GetIsLastFrame())
    {
        field_114_gnFrame = 15;
        SfxPlayMono(relive::SoundEffects::WellEnter, 0, GetSpriteScale());

        switch (mCurrentMotion)
        {
            case eAbeMotions::Motion_77_WellBegin:
            {
                mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
                break;
            }
            case eAbeMotions::Motion_80_430EF0:
            {
                mCurrentMotion = eAbeMotions::Motion_81_InsideWellExpress;
                break;
            }
            case eAbeMotions::Motion_83_430F00:
            {
                mCurrentMotion = eAbeMotions::Motion_84_ToInsideWellLocal;
                break;
            }
            case eAbeMotions::Motion_74_JumpIntoWell:
            {
                mCurrentMotion = eAbeMotions::Motion_75_ToInsideOfAWellLocal;
                break;
            }
            default:
            {
                ALIVE_FATAL("Unrecognized state in Abe::Motion_77_WellBegin_430F10 called! %d", mCurrentMotion);
                break;
            }
        }
    }
}

FP gPointlessWellScale_4C73CC = FP_FromDouble(1.8);

void Abe::Motion_78_InsideWellLocal()
{
    const s32 old_gnFrame = field_114_gnFrame;
    field_114_gnFrame--;

    if (old_gnFrame == 0)
    {
        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellLocal);

        if (!BaseAliveGameObjectPathTLV)
        {
            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress);
        }

        auto pWellBase = static_cast<relive::Path_WellBase*>(BaseAliveGameObjectPathTLV);

        field_120_x_vel_slow_by = FP_FromInteger(0);

        if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit4_snap_abe))
        {
            // Snap to exit point
            mVelX = (GetSpriteScale() * (FP_FromInteger(pWellBase->mExitX) / FP_FromInteger(100)));
            mVelY = (GetSpriteScale() * (FP_FromInteger(pWellBase->mExitY) / FP_FromInteger(100)));
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_snap_abe);
        }
        else
        {
            if (SwitchStates_Get(pWellBase->mSwitchId))
            {
                auto pLocalWell = static_cast<relive::Path_WellLocal*>(BaseAliveGameObjectPathTLV);
                mVelX = (GetSpriteScale() * FP_FromInteger(pLocalWell->mOnDestX) / FP_FromInteger(100));
                mVelY = (GetSpriteScale() * FP_FromInteger(pLocalWell->mOnDestY) / FP_FromInteger(100));
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromInteger(pWellBase->mOffDestX) / FP_FromInteger(100));
                mVelY = (GetSpriteScale() * FP_FromInteger(pWellBase->mOffDestY) / FP_FromInteger(100));
            }
        }

        MapFollowMe(1);

        mYPos += mVelY;
        gPointlessWellScale_4C73CC = FP_FromDouble(1.8);

        if (mVelX >= FP_FromInteger(0))
        {
            GetAnimation().SetFlipX(false);
        }
        else
        {
            GetAnimation().SetFlipX(true);
        }

        SfxPlayMono(relive::SoundEffects::WellExit, 0, GetSpriteScale());

        mCurrentMotion++; // can be motion 76 and 79 maybe more?

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
        }

        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_79_WellShotOut()
{
    if (mVelY >= FP_FromInteger(0))
    {
        Motion_3_Fall();
    }
    else
    {
        mVelY += (GetSpriteScale() * gPointlessWellScale_4C73CC);

        mXPos += mVelX;
        mYPos += mVelY;

        SetActiveCameraDelayedFromDir();
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }

    if (GetAnimation().GetForwardLoopCompleted()
        || (mCurrentMotion != eAbeMotions::Motion_79_WellShotOut
            && mCurrentMotion != eAbeMotions::Motion_85_ToWellShotOut
            && mCurrentMotion != eAbeMotions::Motion_76_ToWellShotOut))
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
        }

        GetShadow()->mEnabled = true;
    }

    if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie)
    {
        mHealth = FP_FromInteger(1);
        mCurrentMotion = eAbeMotions::Motion_98_LandSoft;
    }
}

void Abe::Motion_80_ToWellBegin()
{
    Motion_77_WellBegin();
}

void Abe::Motion_81_InsideWellExpress()
{
    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellExpress);
    }


    if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit4_snap_abe))
    {
        mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
        return;
    }

    relive::Path_WellExpress* pExpressWell = static_cast<relive::Path_WellExpress*>(BaseAliveGameObjectPathTLV);
    if (SwitchStates_Get(pExpressWell->mSwitchId))
    {
        field_190_level = pExpressWell->mOnDestLevel;
        field_192_path = pExpressWell->mOnDestPath;
        field_194_camera = pExpressWell->mOnDestCamera;
        field_196_door_id = pExpressWell->mOnOtherWellId;
    }
    else
    {
        field_190_level = pExpressWell->mOffDestLevel;
        field_192_path = pExpressWell->mOffDestPath;
        field_194_camera = pExpressWell->mOffDestCamera;
        field_196_door_id = pExpressWell->mOffOtherWellId;
    }

    field_120_x_vel_slow_by = FP_FromInteger(0);

    if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
    {
        if (field_190_level == EReliveLevelIds::eForest)
        {
            if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone))
            {
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit3_WalkToRun);
                field_114_gnFrame = 0;
                mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
                Motion_78_InsideWellLocal();
                return;
            }
            LOG_INFO("set bParamoniaDone");
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit12_bParamoniaDone);
        }

        if (field_190_level == EReliveLevelIds::eDesert)
        {
            if (field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabaniaDone))
            {
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit3_WalkToRun);
                field_114_gnFrame = 0;
                mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
                Motion_78_InsideWellLocal();
                return;
            }
            LOG_INFO("set bScrabinaDone");
            field_2A8_flags.Set(Flags_2A8::e2A8_eBit13_bScrabaniaDone);
        }
    }
    if (field_190_level != gMap.mCurrentLevel
        || field_192_path != gMap.mCurrentPath
        || field_194_camera != gMap.mCurrentCamera)
    {
        field_114_gnFrame = 1;
        if (pExpressWell->mMovieId)
        {
            gMap.SetActiveCam(field_190_level, field_192_path, field_194_camera, CameraSwapEffects::ePlay1FMV_5, pExpressWell->mMovieId, 0);
        }
        else
        {
            gMap.SetActiveCam(field_190_level, field_192_path, field_194_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
        mCurrentMotion = eAbeMotions::Motion_82_WellExpressShotOut;
    }
    else
    {
        Motion_82_WellExpressShotOut();
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        mYPos = mYPos - mVelY;
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit3_WalkToRun);
        mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;
    }
}

void Abe::Motion_82_WellExpressShotOut()
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    relive::Path_WellBase* pWell = nullptr;
    relive::Path_TLV* pTlvIter = nullptr;
    do
    {
        pTlvIter = gMap.TLV_Get_At(
            pTlvIter,
            FP_FromInteger(camCoords.x + 256),
            FP_FromInteger(camCoords.y + 120),
            FP_FromInteger(camCoords.x + 624),
            FP_FromInteger(camCoords.y + 360));
        if (!pTlvIter)
        {
            break;
        }

        if ((pTlvIter->mTlvType == ReliveTypes::eWellLocal || pTlvIter->mTlvType == ReliveTypes::eWellExpress) && static_cast<relive::Path_WellBase*>(pTlvIter)->mOtherWellId == field_196_door_id)
        {
            pWell = static_cast<relive::Path_WellBase*>(pTlvIter);
            break;
        }
    }
    while (pTlvIter);


    mCurrentLevel = gMap.mCurrentLevel;
    mCurrentPath = gMap.mCurrentPath;

    if (pWell)
    {
        if (pWell->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            SetScale(Scale::Bg);
        }
        else
        {
            SetSpriteScale(FP_FromInteger(1));
            SetScale(Scale::Fg);
        }

        mCurrentMotion = eAbeMotions::Motion_78_InsideWellLocal;

        BaseAliveGameObjectPathTLV = pWell;
        mXPos = FP_FromInteger((pWell->mTopLeftX + pWell->mBottomRightX) / 2);
        mYPos = FP_FromInteger(pWell->mBottomRightY);
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit4_snap_abe);
    }
    else
    {
        mXPos = FP_FromInteger(camCoords.x + 450);
        mYPos = FP_FromInteger(camCoords.y + 320);
        mVelX = (GetSpriteScale() * -FP_FromDouble(2.7));
        mVelY = (GetSpriteScale() * -FP_FromInteger(15));
    }
}

void Abe::Motion_83_ToWellBegin()
{
    Motion_77_WellBegin();
}

void Abe::Motion_84_ToInsideWellLocal()
{
    Motion_78_InsideWellLocal();
}

void Abe::Motion_85_ToWellShotOut()
{
    Motion_79_WellShotOut();
}

void Abe::Motion_86_FallLandDie()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 85);
        SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 95, 95);
        relive_new ScreenShake(true);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (static_cast<s32>(sGnFrame) >= field_114_gnFrame)
        {
            ToDieFinal_42C400();
        }
    }
}

void Abe::Motion_87_ToFall()
{
    Motion_3_Fall();
}

s32 gCounter_507728 = 0;
u8** gpDemoPlaybackRes_50772C = nullptr;
s32 sAbeSound_507730 = 0;
BellSong* sBellSong = nullptr;

void Abe::Motion_88_HandstoneBegin()
{
    switch (field_110_state.stone)
    {
        case StoneStates::eHandstoneStart_0:
        {
            if (GetAnimation().GetForwardLoopCompleted())
            {
                mCircularFade = relive_new CircularFade(
                    mXPos,
                    mYPos,
                    GetSpriteScale(),
                    1,
                    0);

                mCircularFade->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

                field_110_state.stone = StoneStates::eGetHandstoneType_1;
                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eDemoPlaybackStone);
                if (!BaseAliveGameObjectPathTLV)
                    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        ReliveTypes::eBellSongStone);
                if (!BaseAliveGameObjectPathTLV)
                {
                    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        ReliveTypes::eMovieHandStone);
                    sAbeSound_507730 = SFX_Play_Pitch(relive::SoundEffects::HandstoneTransition, 127, -300);
                    if (!BaseAliveGameObjectPathTLV)
                        BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(
                            FP_GetExponent(mXPos),
                            FP_GetExponent(mYPos),
                            FP_GetExponent(mXPos),
                            FP_GetExponent(mYPos),
                            ReliveTypes::eHandStone);
                }

                if (BaseAliveGameObjectPathTLV)
                {
                    mHandStoneType = BaseAliveGameObjectPathTLV->mTlvType;
                    switch (mHandStoneType)
                    {
                        case ReliveTypes::eMovieHandStone:
                        {
                            mMovieStone = static_cast<relive::Path_MovieStone*>(BaseAliveGameObjectPathTLV);
                            break;
                        }
                        case ReliveTypes::eBellSongStone:
                        {
                            mBellsongStone = static_cast<relive::Path_BellsongStone*>(BaseAliveGameObjectPathTLV);
                            break;
                        }
                        case ReliveTypes::eDemoPlaybackStone:
                        {
                            ALIVE_FATAL("never expected eDemoPlaybackStone to be used");
                            //field_174_pathStone.demoId = ptlv->field_18_data.demoId;
                            break;
                        }
                        case ReliveTypes::eHandStone:
                        {
                            mHandStone = static_cast<relive::Path_HandStone*>(BaseAliveGameObjectPathTLV);
                            break;
                        }
                        default:
                            return;
                    }
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_89_HandstoneEnd;
                }
            }
            break;
        }
        case StoneStates::eGetHandstoneType_1:
        {
            if (mCircularFade->VDone())
            {
                switch (mHandStoneType)
                {
                    case ReliveTypes::eMovieHandStone:
                    {
                        auto pFmvInfo = Path_Get_FMV_Record_434680(
                            gMap.mCurrentLevel,
                            mMovieStone->mMovieId);

                        relive_new Movie(pFmvInfo->field_0_pName);

                        field_110_state.stone = StoneStates::eHandstoneMovieDone_2;
                        break;
                    }
                    case ReliveTypes::eBellSongStone:
                    {
                        sBellSong = relive_new BellSong(
                            mBellsongStone->mType,
                            Code_Convert(mBellsongStone->mCode1, mBellsongStone->mCode2));

                        SwitchStates_Do_Operation(mBellsongStone->mSwitchId, relive::reliveSwitchOp::eSetTrue);
                        field_110_state.stone = StoneStates::eBellSongDone_4;
                        break;
                    }
                    case ReliveTypes::eDemoPlaybackStone:
                        ALIVE_FATAL("never expected eDemoPlaybackStone to be used");
                        /*field_164_pCircularFade->SetDead(true);
                        field_164_pCircularFade = nullptr;
                        mState.stone = StoneStates::eFreeDemoPlaybackResources_3;
                        gCounter_507728 = 2;
                        gpDemoPlaybackRes_50772C = ResourceManager::GetLoadedResource(ResourceManager::Resource_Plbk, field_174_pathStone.demoId, 1, 0);
                        if (gpDemoPlaybackRes_50772C)
                        {
                            relive_new DemoPlayback(gpDemoPlaybackRes_50772C, 1);
                        }
                        break;*/
                    case ReliveTypes::eHandStone:
                    {
                        GetAnimation().SetRender(false);
                        field_110_state.stone = StoneStates::eWaitForInput_6;
                        field_16E_cameraIdx = 1;
                        mCircularFade->SetDead(true);
                        mCircularFade = 0;
                        mFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 0, 8, TPageAbr::eBlend_2);
                        field_190_level = gMap.mCurrentLevel;
                        field_192_path = gMap.mCurrentPath;
                        field_194_camera = gMap.mCurrentCamera;
                        gMap.SetActiveCam(
                            mHandStone->mLevel1,
                            mHandStone->mPath1,
                            mHandStone->mCameraId1,
                            CameraSwapEffects::eInstantChange_0, 0, 0);
                        break;
                    }
                    default:
                        return;
                }
            }
            break;
        }
        case StoneStates::eHandstoneMovieDone_2:
        {
            if (sMovie_ref_count_9F309C == 0)
            {
                gPsxDisplay.PutCurrentDispEnv();
                pScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->field_C_pCamRes);
                pScreenManager->EnableRendering();
                mCircularFade->VFadeIn(0, 0);
                field_110_state.stone = StoneStates::eHandstoneEnd_5;
            }
            break;
        }
        case StoneStates::eFreeDemoPlaybackResources_3:
        {
            gCounter_507728--;
            if (gCounter_507728 == 0)
            {
                field_110_state.stone = StoneStates::eHandstoneEnd_5;
                mCircularFade = relive_new CircularFade(
                    mXPos,
                    mYPos,
                    GetSpriteScale(),
                    0,
                    0);

                mCircularFade->GetAnimation().SetFlipX(GetAnimation().GetFlipX());
            }
            break;
        }
        case StoneStates::eBellSongDone_4:
        {
            if (sBellSong->mDone)
            {
                sBellSong->SetDead(true);
                mCircularFade->VFadeIn(0, 0);
                field_110_state.stone = StoneStates::eHandstoneEnd_5;
            }
            break;
        }
        case StoneStates::eHandstoneEnd_5:
        {
            if (mCircularFade->VDone())
            {
                mCircularFade->SetDead(true);
                mCurrentMotion = eAbeMotions::Motion_89_HandstoneEnd;
                mCircularFade = 0;
                if (sAbeSound_507730)
                {
                    SND_Stop_Channels_Mask(sAbeSound_507730);
                    sAbeSound_507730 = 0;
                }
            }
            break;
        }
        case StoneStates::eWaitForInput_6:
        {
            if (mFade->mDone)
            {
                if (Input().IsAnyHeld(0xF0))
                {
                    mFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8);
                    field_110_state.stone = StoneStates::eSetActiveCamToAbeOrWaitForInput_7;
                    SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                }
            }
            break;
        }
        case StoneStates::eSetActiveCamToAbeOrWaitForInput_7:
        {
            if (mFade->mDone)
            {
                Path_Stone_camera camera = {};
                switch (field_16E_cameraIdx)
                {
                    case 0:
                        camera.level = MapWrapper::ToAO(mHandStone->mLevel1);
                        camera.path = mHandStone->mPath1;
                        camera.camera = mHandStone->mCameraId1;
                        break;

                    case 1:
                        camera.level = MapWrapper::ToAO(mHandStone->mLevel2);
                        camera.path = mHandStone->mPath2;
                        camera.camera = mHandStone->mCameraId2;
                        break;

                    case 2:
                        camera.level = MapWrapper::ToAO(mHandStone->mLevel3);
                        camera.path = mHandStone->mPath3;
                        camera.camera = mHandStone->mCameraId3;
                        break;

                    default:
                        LOG_ERROR("CameraIdx out of bounds %d", field_16E_cameraIdx);
                }
                if (field_16E_cameraIdx > 2 || (camera.level == LevelIds::eForestChase_14 || camera.level == LevelIds::eDesertEscape_15))
                {
                    field_110_state.stone = StoneStates::eSetActiveCamToAbe_12;
                }
                else
                {
                    mFade->SetDead(true);
                    field_110_state.stone = StoneStates::eWaitForInput_6;
                    field_16E_cameraIdx++;
                    mFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 0, 8, TPageAbr::eBlend_2);
                    gMap.SetActiveCam(MapWrapper::FromAO(camera.level), camera.path, camera.camera, CameraSwapEffects::eInstantChange_0, 0, 0);
                }
            }
            break;
        }
        case StoneStates::eSetActiveCamToAbe_12:
            if (mFade->mDone)
            {
                GetAnimation().SetRender(true);
                field_110_state.stone = StoneStates::eCircularFadeExit_13;
                gMap.SetActiveCam(field_190_level, field_192_path, field_194_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            break;
        case StoneStates::eCircularFadeExit_13:
        {
            mFade->SetDead(true);
            mFade = nullptr;

            mCircularFade = relive_new CircularFade(mXPos, mYPos, GetSpriteScale(), 0, 0);
            field_110_state.stone = StoneStates::eHandstoneEnd_5;
            mCircularFade->GetAnimation().SetFlipX(GetAnimation().GetFlipX());

            if (sAbeSound_507730)
            {
                SND_Stop_Channels_Mask(sAbeSound_507730);
                sAbeSound_507730 = 0;
            }
            break;
        }
        default:
            return;
    }
}

void Abe::Motion_89_HandstoneEnd()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_0_Idle;
    }
}

void Abe::Motion_90_GrenadeMachineUse()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_0_Idle;
    }
}

void Abe::Motion_91_FallingFromGrab()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }

    Motion_3_Fall();

    if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie)
    {
        mCurrentMotion = eAbeMotions::Motion_98_LandSoft;
    }

    mPreviousMotion = eAbeMotions::Motion_91_FallingFromGrab;
}

void Abe::Motion_92_ForceDownFromHoist()
{
    if (field_114_gnFrame == 0)
    {
        mYPos += (GetSpriteScale() * FP_FromInteger(80));
        GetShadow()->mShadowAtBottom = false;
        VOnTrapDoorOpen();
        BaseAliveGameObjectCollisionLine = nullptr;
        BaseAliveGameObjectLastLineYPos = mYPos;
        field_114_gnFrame++;
    }

    Motion_3_Fall();
}

void Abe::Motion_93_WalkOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_94_RunOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_95_SneakOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_96_HopToFall()
{
    if (GetAnimation().GetFlipX())
    {
        mXPos += (GetSpriteScale() * FP_FromInteger(5));
    }
    else
    {
        mXPos -= (GetSpriteScale() * FP_FromInteger(5));
    }

    mVelY += (GetSpriteScale() * FP_FromInteger(4));

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }

    Motion_3_Fall();
}

void Abe::Motion_97_RunJumpToFall()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_98_LandSoft()
{
    FollowLift_42EE90();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (!(field_2A8_flags.Get(Flags_2A8::e2A8_Bit8_bLandSoft)))
        {
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
        }

        field_2A8_flags.Clear(Flags_2A8::e2A8_Bit8_bLandSoft);

        if (mPreviousMotion == eAbeMotions::Motion_3_Fall)
        {
            Environment_SFX_42A220(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }

        if (Input().IsAnyPressed(0xA000u))
        {
            ToLeftRightMovement_422AA0();
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_99_HoistBeginLong()
{
    FollowLift_42EE90();

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelY = (GetSpriteScale() * FP_FromInteger(-8)) * FP_FromDouble(1.3);
        mYPos += mVelY;
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_17_HoistIdle;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_100_RollOffLedge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_87_ToFall;
    }
    Motion_3_Fall();
}

void Abe::Motion_101_LeverUse()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_102_ElumWalkLoop()
{
    // Empty
}

void Abe::Motion_103_ElumIdle()
{
    if (!Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler) || !Input().IsAnyHeld(0xF0))
    {
        if (Input().Held() == sInputKey_Down && !gDDCheat_FlyingEnabled)
        {
            mCurrentMotion = eAbeMotions::Motion_137_ElumUnmountBegin;
        }
    }
    else
    {
        mCurrentMotion = eAbeMotions::Motion_115_ElumSpeak;
        if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler))
        {
            if (Input().IsAnyHeld(sInputKey_GameSpeak2))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eFollowMe_10);
                Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
            }
            else if (Input().IsAnyHeld(sInputKey_GameSpeak4))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eWait_12);
                Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
            }
            else if (Input().IsAnyHeld(sInputKey_GameSpeak1))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eHello_9);
                Mudokon_SFX(MudSounds::eHello_3, 0, 0, this);
            }
            else if (Input().IsAnyHeld(sInputKey_GameSpeak3))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eAnger_11);
                Mudokon_SFX(MudSounds::eAngry_5, 0, 0, this);
            }
        }

        if (Input().IsAnyPressed(sInputKey_RightGameSpeakEnabler))
        {
            if (Input().IsAnyHeld(sInputKey_GameSpeak6))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eWhistleHigh_1);
                Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
            }
            else if (Input().IsAnyHeld(sInputKey_GameSpeak5))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eWhistleLow_2);
                Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
            }
            else if (Input().IsAnyHeld(sInputKey_GameSpeak8))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eLaugh_4);
                Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            }
            else if (Input().IsAnyHeld(sInputKey_GameSpeak7))
            {
                gEventSystem->VPushEvent(GameSpeakEvents::eFart_3);
                Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
            }
        }
    }
}

void Abe::Motion_104_ElumRunSlideStop()
{
    // Empty
}

void Abe::Motion_105_ElumRunTurn()
{
    // Empty
}

void Abe::Motion_106_Null()
{
    // Empty
}

void Abe::Motion_107_ElumHopBegin()
{
    // Empty
}

void Abe::Motion_108_ElumHopMid()
{
    // Empty
}

void Abe::Motion_109_ElumHopLand()
{
    // Empty
}

void Abe::Motion_110_ElumRunJumpBegin()
{
    // Empty
}

void Abe::Motion_111_ElumRunJumpMid()
{
    // Empty
}

void Abe::Motion_112_ElumRunJumpLand()
{
    // Empty
}

void Abe::Motion_113_ElumTurn()
{
    // Empty
}

void Abe::Motion_114_ElumRunLoop()
{
    // Empty
}

void Abe::Motion_115_ElumSpeak()
{
    EventBroadcast(kEventSpeaking, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_103_ElumIdle;
    }
}

void Abe::Motion_116_Null()
{
    // Empty
}

void Abe::Motion_117_ElumWalkBegin()
{
    // Empty
}

void Abe::Motion_118_ElumRunBegin()
{
    // Empty
}

void Abe::Motion_119_Null()
{
    // Empty
}

void Abe::Motion_120_ElumRunToWalk()
{
    // Empty
}

void Abe::Motion_121_ElumMidRunToWalk()
{
    // Empty
}

void Abe::Motion_122_ElumRunTurnToRun()
{
    // Empty
}

void Abe::Motion_123_ElumRunTurnToWalk()
{
    // Empty
}

void Abe::Motion_124_ElumWalkEnd()
{
    // Empty
}

void Abe::Motion_125_ElumMidWalkEnd()
{
    // Empty
}

void Abe::Motion_126_ElumBeesStruggling()
{
    // Empty
}

void Abe::Motion_127_SlapBomb()
{
    if (sActiveHero->GetAnimation().GetCurrentFrame() >= 6)
    {
        if (field_15C_pThrowable)
        {
            field_15C_pThrowable->mBaseGameObjectRefCount--;
            field_15C_pThrowable->VOnPickUpOrSlapped();
            field_15C_pThrowable = nullptr;
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }
}

void Abe::Motion_128_KnockForward()
{
    ElumFree_4228F0();

    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_4231B0();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    FollowLift_42EE90();

    if (GetAnimation().GetRender())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                mVelX = FP_FromInteger(0);
            }
            MoveWithVelocity_4257F0(FP_FromDouble(0.7));
        }
        else
        {
            Motion_3_Fall();

            if (mCurrentMotion == eAbeMotions::Motion_86_FallLandDie || mCurrentMotion == eAbeMotions::Motion_98_LandSoft)
            {
                mCurrentMotion = eAbeMotions::Motion_128_KnockForward;
                field_114_gnFrame = sGnFrame + 10;
            }
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if ((BaseAliveGameObjectCollisionLine || !GetAnimation().GetRender()) /*&& !field_104_pending_resource_count*/)
        {
            if (mRidingElum)
            {
                mRidingElum = false;
            }

            if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
            {
                if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
                {
                    mCurrentMotion = eAbeMotions::Motion_130_KnockForwardGetUp;
                }
                else
                {
                    ToDieFinal_42C400();
                }
            }
        }
    }
}

void Abe::Motion_129_RollingKnockForward()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_4231B0();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (field_114_gnFrame <= static_cast<s32>(sGnFrame))
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvulnerableCheat)
            {
                mCurrentMotion = eAbeMotions::Motion_130_KnockForwardGetUp;
            }
            else
            {
                ToDieFinal_42C400();
            }
        }
    }
}

void Abe::Motion_130_KnockForwardGetUp()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_131_LiftUseUp()
{
    mCurrentMotion = MoveLiftUpOrDown_42F190(FP_FromInteger(-4));
}

void Abe::Motion_132_LiftUseDown()
{
    mCurrentMotion = MoveLiftUpOrDown_42F190(FP_FromInteger(4));
}

void Abe::Motion_133_LiftGrabBegin()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);

    mVelY = FP_FromInteger(0);
    if (GetAnimation().GetIsLastFrame())
    {
        if (Input().IsAnyPressed(sInputKey_Up))
        {
            if (!pLiftPoint->OnTopFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_131_LiftUseUp;
                return;
            }
        }
        else if (Input().IsAnyPressed(sInputKey_Down))
        {
            if (!pLiftPoint->OnBottomFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_132_LiftUseDown;
                return;
            }
        }
        mCurrentMotion = eAbeMotions::Motion_135_LiftGrabIdle;
    }
}

void Abe::Motion_134_LiftGrabEnd()
{
    mVelY = FP_FromInteger(0);
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_135_LiftGrabIdle()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    FollowLift_42EE90();

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);

    mVelY = FP_FromInteger(0);

    if (gBeeInstanceCount > 0 && gBeesNearAbe)
    {
        mCurrentMotion = eAbeMotions::Motion_141_BeesStrugglingOnLift;
    }
    else
    {
        if (Input().IsAnyPressed(sInputKey_Up))
        {
            if (!pLiftPoint->OnTopFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_131_LiftUseUp;
            }
        }
        else if (Input().IsAnyPressed(sInputKey_Down))
        {
            if (!pLiftPoint->OnBottomFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_132_LiftUseDown;
            }
        }
        else
        {
            if (pLiftPoint->OnAnyFloor())
            {
                mCurrentMotion = eAbeMotions::Motion_134_LiftGrabEnd;
            }
        }
    }
}

void Abe::Motion_136_ElumMountEnd()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 0, GetSpriteScale());
            }
            break;

        case 5:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                Environment_SFX_42A220(EnvironmentSfx::eExhaustingElumMount_16, 0, 0x7FFF, this);
            }
            break;

        case 12:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                Environment_SFX_42A220(EnvironmentSfx::eMountElumSmackNoise_17, 0, 0x7FFF, this);
                SfxPlayMono(relive::SoundEffects::MountingElum, 0, GetSpriteScale());
            }
            break;

        case 15:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                Environment_SFX_42A220(EnvironmentSfx::eElumGetMountedNoise_18, 0, 0x7FFF, this);
            }
            break;

        default:
            field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);
            break;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_103_ElumIdle;
        sControlledCharacter = gElum;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eAbeOnElum_1, nullptr, 0, 0);
        sActiveHero->GetShadow()->mEnabled = false;
        Environment_SFX_42A220(EnvironmentSfx::eAbeMountedElumNoise_19, 0, 0x7FFF, this);
    }
}

void Abe::Motion_137_ElumUnmountBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_138_ElumUnmountEnd;
    }
}

void Abe::Motion_138_ElumUnmountEnd()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 4:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 17, GetSpriteScale());
            }
            break;

        case 12:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                Environment_SFX_42A220(EnvironmentSfx::eExhaustingElumMount_16, 0, 0x7FFF, this);
            }
            break;

        case 19:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                Environment_SFX_42A220(EnvironmentSfx::eElumGetMountedNoise_18, 0, 0x7FFF, this);
            }
            break;

        case 20:
            if (!field_2AA_flags.Get(Flags_2AA::e2AA_Bit2_bSfxPlaying))
            {
                field_2AA_flags.Set(Flags_2AA::e2AA_Bit2_bSfxPlaying);
                SfxPlayMono(relive::SoundEffects::MountingElum, 0, GetSpriteScale());
            }
            break;

        default:
            field_2AA_flags.Clear(Flags_2AA::e2AA_Bit2_bSfxPlaying);
            break;
    }

    if (GetAnimation().GetCurrentFrame() >= 28)
    {
        mRidingElum = false;
        BaseAliveGameObjectCollisionLine = gElum->BaseAliveGameObjectCollisionLine;

        LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
        if (pLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes ::eDynamicCollision_32 &&
                BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                mPreviousMotion = mCurrentMotion;
                VOnTrapDoorOpen();
                mCurrentMotion = mPreviousMotion;
            }
        }
        else
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;
                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    gPlatformsArray);
            }
        }
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
        sActiveHero->GetShadow()->mEnabled = false;
        ToIdle_422D50();
    }
}

void Abe::Motion_139_ElumMountBegin()
{
    if (/*!field_104_pending_resource_count &&*/ GetAnimation().GetIsLastFrame())
    {
        /*
        while (!ResourceManager::FreeResource_455550(field_1A4_resources.res[61]))
        {
            // Empty
        }

        field_1A4_resources.res[61] = {};
        field_104_pending_resource_count = 1;

        ResourceManager::LoadResourceFile("ABEWELM.BND", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        */
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_136_ElumMountEnd;
    }
}

void Abe::Motion_140_BeesStruggling()
{
    Motion_0_Idle();
    if ((!gBeeInstanceCount || !gBeesNearAbe) && mCurrentMotion == eAbeMotions::Motion_140_BeesStruggling)
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_141_BeesStrugglingOnLift()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(12), 0);

    mVelY = pLiftPoint->mVelY;
    if (pLiftPoint->GetDead())
    {
        VOnTrapDoorOpen();
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit1);
    }
    SetActiveCameraDelayedFromDir();

    mVelY = pLiftPoint->mVelY;

    if (pLiftPoint->OnBottomFloor())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_142_RockThrowStandingHold()
{
    if (GetAnimation().GetCurrentFrame() >= 4)
    {
        if (Input().IsAnyPressed(sInputKey_Right | sInputKey_Left | sInputKey_Up | sInputKey_Down))
        {
            if (Input().IsAnyPressed(sInputKey_Right))
            {
                if (GetAnimation().GetFlipX())
                {
                    field_19D_throw_direction = 0;
                }
                else
                {
                    field_19D_throw_direction = 2;
                }
            }
            else if (Input().IsAnyPressed(sInputKey_Left))
            {
                if (GetAnimation().GetFlipX())
                {
                    field_19D_throw_direction = 2;
                }
                else
                {
                    field_19D_throw_direction = 0;
                }
            }
            else if (Input().IsAnyPressed(sInputKey_Up))
            {
                field_19D_throw_direction = 1;
            }
            else
            {
                field_19D_throw_direction = 3;
            }
            mCurrentMotion = eAbeMotions::Motion_143_RockThrowStandingThrow;
        }
    }

    if (Input().IsAnyReleased(sInputKey_ThrowItem))
    {
        field_198_pThrowable->VToDead();
        field_198_pThrowable = nullptr;
        mCurrentMotion = eAbeMotions::Motion_144_RockThrowStandingEnd;
        if (!gInfiniteGrenades)
        {
            field_19C_throwable_count++;
        }
    }
}

void Abe::Motion_143_RockThrowStandingThrow()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::AirStream, 0, GetSpriteScale());
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_144_RockThrowStandingEnd()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_145_RockThrowCrouchingHold()
{
    if (GetAnimation().GetCurrentFrame() >= 4)
    {
        if (Input().IsAnyPressed(sInputKey_Right | sInputKey_Left | sInputKey_Up | sInputKey_Down))
        {
            field_19D_throw_direction = 4;
            mCurrentMotion = eAbeMotions::Motion_146_RockThrowCrouchingThrow;
            if (field_198_pThrowable->Type() == ReliveTypes::eMeat)
            {
                field_19D_throw_direction = 5;
            }
        }
    }

    if (Input().IsAnyReleased(sInputKey_ThrowItem))
    {
        field_198_pThrowable->VToDead();
        field_198_pThrowable = nullptr;
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
        if (!gInfiniteGrenades)
        {
            field_19C_throwable_count++;
        }
    }
}

void Abe::Motion_146_RockThrowCrouchingThrow()
{
    FollowLift_42EE90();

    if (!GetAnimation().GetCurrentFrame())
    {
        SfxPlayMono(relive::SoundEffects::AirStream, 0, GetSpriteScale());
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }
}

void Abe::Motion_147_ShotRolling()
{
    ElumFree_4228F0();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    Motion_3_Fall();

    if (mCurrentMotion != eAbeMotions::Motion_147_ShotRolling && !gAbeInvulnerableCheat)
    {
        mCurrentMotion = eAbeMotions::Motion_147_ShotRolling;
        BaseAliveGameObjectCollisionLine = nullptr;
        mYPos += (GetSpriteScale() * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (GetAnimation().GetForwardLoopCompleted())
        {
            mYPos += FP_FromInteger(240);
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
            ToDeathDropFall_42C3D0();
        }
    }
}

void Abe::Motion_148_Shot()
{
    ElumFree_4228F0();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    Motion_3_Fall();

    if (mCurrentMotion != eAbeMotions::Motion_148_Shot && !gAbeInvulnerableCheat)
    {
        mCurrentMotion = eAbeMotions::Motion_148_Shot;
        BaseAliveGameObjectCollisionLine = nullptr;
        mYPos += (GetSpriteScale() * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (GetAnimation().GetForwardLoopCompleted())
        {
            mYPos += FP_FromInteger(240);
            Mudokon_SFX(MudSounds::eDeathDropScream_17, 0, 0, this);
            ToDeathDropFall_42C3D0();
        }
    }
}

void Abe::Motion_149_PickupItem()
{
    if (GetAnimation().GetCurrentFrame() == 7)
    {
        SfxPlayMono(relive::SoundEffects::PickupItem, 0, GetSpriteScale());
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_19_CrouchIdle;
    }

    if (sActiveHero->GetAnimation().GetCurrentFrame() >= 5)
    {
        if (field_15C_pThrowable)
        {
            field_15C_pThrowable->mBaseGameObjectRefCount--;
            field_15C_pThrowable->VOnPickUpOrSlapped();
            field_15C_pThrowable = nullptr;
        }
    }
}

void Abe::Motion_150_Chant()
{
    FollowLift_42EE90();
    if (field_110_state.chant != ChantStates::eWaitForUnpossessing_3 && field_110_state.chant != ChantStates::eUnpossessing_4)
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::eMudokonChant1_11, 0, 0);
    }

    switch (field_110_state.chant)
    {
        case ChantStates::eIdleChanting_0:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            auto pObjToPossess = FindObjectToPossess_421410();
            if (field_168_ring_pulse_timer)
            {
                if (!field_16C_bHaveShrykull)
                {
                    const PSX_RECT rect = VGetBoundingRect();
                    AbilityRing::Factory(
                        FP_FromInteger((rect.w + rect.x) / 2),
                        FP_FromInteger((rect.h + rect.y) / 2),
                        RingTypes::eExplosive_Emit_1);
                    field_168_ring_pulse_timer = 0;
                }
            }

            if ((GetAnimation().GetIsLastFrame()
                 || GetAnimation().GetCurrentFrame() == 3)
                && !Input_IsChanting())
            {
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
                if (field_188_pOrbWhirlWind)
                {
                    field_188_pOrbWhirlWind->ToStop();
                    field_188_pOrbWhirlWind = nullptr;
                }
                return;
            }
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }
            if (static_cast<s32>(sGnFrame) >= field_114_gnFrame - 70)
            {
                if (pObjToPossess)
                {
                    if (!field_188_pOrbWhirlWind)
                    {
                        FP xPos = {};
                        if (GetAnimation().GetFlipX())
                        {
                            xPos = mXPos + FP_FromInteger(-4);
                        }
                        else
                        {
                            xPos = mXPos + FP_FromInteger(4);
                        }
                        field_188_pOrbWhirlWind = relive_new OrbWhirlWind(
                            xPos,
                            mYPos - GetSpriteScale() * FP_FromInteger(38),
                            GetSpriteScale());;
                    }
                }
                else
                {
                    field_114_gnFrame = sGnFrame + 70;
                    if (field_188_pOrbWhirlWind)
                    {
                        field_188_pOrbWhirlWind->ToStop();
                        field_188_pOrbWhirlWind = nullptr;
                    }
                }
            }

            if (static_cast<s32>(sGnFrame) > field_114_gnFrame)
            {
                field_18C_pObjToPossess = pObjToPossess;
                if (field_18C_pObjToPossess)
                {
                    field_18C_pObjToPossess->mBaseGameObjectRefCount++;
                    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                    field_114_gnFrame = sGnFrame + 30;
                    field_110_state.chant = ChantStates::ePossessVictim_1;

                    const PSX_RECT rect = field_18C_pObjToPossess->VGetBoundingRect();
                    field_188_pOrbWhirlWind->ToSpin(
                        FP_FromInteger((rect.w + rect.x) / 2),
                        FP_FromInteger((rect.h + rect.y) / 2),
                        pObjToPossess);

                    relive_new PossessionFlicker(sActiveHero, 30, 128, 255, 255);
                }
            }
            break;
        }
        case ChantStates::ePossessVictim_1:
        {
            if (static_cast<s32>(sGnFrame) > field_114_gnFrame)
            {
                field_110_state.chant = ChantStates::ePossessedVictim_2;
                return;
            }
            if (field_18C_pObjToPossess)
            {
                if (field_18C_pObjToPossess->GetDead())
                {
                    field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                    field_18C_pObjToPossess = nullptr;
                }
            }
            if (field_18C_pObjToPossess)
            {
                if (field_18C_pObjToPossess->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    return;
                }
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
                field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                field_18C_pObjToPossess = nullptr;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
            }
            if (field_188_pOrbWhirlWind)
            {
                field_188_pOrbWhirlWind->ToStop();
                field_188_pOrbWhirlWind = nullptr;
            }
            break;
        }
        case ChantStates::ePossessedVictim_2:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            field_188_pOrbWhirlWind = nullptr;
            if (field_18C_pObjToPossess)
            {
                if (field_18C_pObjToPossess->GetDead())
                {
                    field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                    field_18C_pObjToPossess = nullptr;
                }
            }
            if (field_18C_pObjToPossess)
            {
                sControlledCharacter = field_18C_pObjToPossess;
                field_18C_pObjToPossess->VPossessed();
                field_18C_pObjToPossess->mBaseGameObjectRefCount--;
                field_18C_pObjToPossess = nullptr;
                if (sControlledCharacter->Type() == ReliveTypes::eSlig)
                {
                    field_2A8_flags.Set(Flags_2A8::e2A8_Bit11_bLaughAtChantEnd);
                }

                relive_new PossessionFlicker(sControlledCharacter, 60, 128, 255, 255);

                SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 400);
                field_110_state.chant = ChantStates::eWaitForUnpossessing_3;
            }
            else if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
            }
            break;
        }
        case ChantStates::eWaitForUnpossessing_3:
        {
            if (sControlledCharacter == this)
            {
                relive_new PossessionFlicker(sControlledCharacter, 15, 128, 255, 255);

                field_110_state.chant = ChantStates::eUnpossessing_4;
                field_114_gnFrame = sGnFrame + 15;
            }
            break;
        }
        case ChantStates::eUnpossessing_4:
        {
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }
            if (static_cast<s32>(sGnFrame) > field_114_gnFrame)
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    mCurrentMotion = eAbeMotions::Motion_151_ChantEnd;
                }
            }
            break;
        }
        case ChantStates::eChantingForBirdPortal_6:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }

            break;
        }
        default:
            return;
    }
}

void Abe::Motion_151_ChantEnd()
{
    SND_Seq_Stop_477A60(SeqId::eMudokonChant1_11);

    FollowLift_42EE90();

    if (GetAnimation().GetIsLastFrame())
    {
        if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit11_bLaughAtChantEnd))
        {
            mCurrentMotion = eAbeMotions::Motion_12_Speak;
            Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit11_bLaughAtChantEnd);
        }
        else
        {
            ToIdle_422D50();
        }
    }
}

void Abe::Motion_152_ElumFallOffEdge()
{
    // Empty
}

void Abe::Motion_153_ElumFall()
{
    // Empty
}

void Abe::Motion_154_ElumLand()
{
    // Empty
}

void Abe::Motion_155_ElumJumpToFall()
{
    // Empty
}

void Abe::Motion_156_DoorEnter()
{
    switch (field_110_state.door)
    {
        case AbeDoorStates::eAbeComesIn_0:
        {
            if (GetAnimation().GetIsLastFrame())
            {
                field_110_state.door = AbeDoorStates::eWaitABit_2;
                GetAnimation().SetRender(false);
                field_118_timer = sGnFrame + 3;
            }
            return;
        }
        case AbeDoorStates::eUnused_1:
        {
            if (mFade->mDone)
            {
                field_110_state.door = AbeDoorStates::eWaitABit_2;
                field_118_timer = sGnFrame + 5;
            }
            ALIVE_FATAL("never expected AbeDoorStates::eUnused_1 to be called");
            return;
        }
        case AbeDoorStates::eWaitABit_2:
        {
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                field_110_state.door = AbeDoorStates::eClearTlvIds_3;
                field_118_timer = sGnFrame + 3;
            }
            return;
        }
        case AbeDoorStates::eClearTlvIds_3:
        {
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                field_110_state.door = AbeDoorStates::eSetNewActiveCamera_4;
                auto pTlv = static_cast<relive::Path_ResetPath*>(gMap.VTLV_Get_At(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eResetPath));
                BaseAliveGameObjectPathTLV = pTlv;
                if (pTlv)
                {
                    if (pTlv->mClearIds == relive::reliveChoice::eYes)
                    {
                        for (s16 i = pTlv->mFrom; i <= pTlv->mTo; i++)
                        {
                            if (i != pTlv->mExclude && i > 1)
                            {
                                SwitchStates_Set(i, 0);
                            }
                        }
                    }
                    if (pTlv->mClearObjects == relive::reliveChoice::eYes)
                    {
                        gMap.ResetPathObjects(pTlv->mPath);
                    }
                }
            }
            return;
        }
        case AbeDoorStates::eSetNewActiveCamera_4:
        {
            auto pDoorTlv = static_cast<relive::Path_Door*>(gMap.VTLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eDoor));
            BaseAliveGameObjectPathTLV = pDoorTlv;
            gMap.field_1E_door = 1;
            const auto changeEffect = kPathChangeEffectToInternalScreenChangeEffect[pDoorTlv->mWipeEffect];
            s16 flag = 0;
            if (changeEffect == CameraSwapEffects::ePlay1FMV_5 || changeEffect == CameraSwapEffects::eUnknown_11)
            {
                flag = 1;
            }
            gMap.SetActiveCam(
                pDoorTlv->mNextLevel,
                pDoorTlv->mNextPath,
                pDoorTlv->mNextCamera,
                changeEffect,
                pDoorTlv->mMovieId,
                flag);
            field_110_state.door = AbeDoorStates::eSetNewAbePosition_5;
            field_196_door_id = pDoorTlv->mTargetDoorId;
            return;
        }
        case AbeDoorStates::eSetNewAbePosition_5:
        {
            mCurrentLevel = gMap.mCurrentLevel;
            mCurrentPath = gMap.mCurrentPath;
            gMap.field_1E_door = 0;
            auto pPathDoor = static_cast<relive::Path_Door*>(gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0));
            BaseAliveGameObjectPathTLV = pPathDoor;

            while (pPathDoor->mDoorId != field_196_door_id)
            {
                pPathDoor = static_cast<relive::Path_Door*>(Path_TLV::TLV_Next_Of_Type_446500(BaseAliveGameObjectPathTLV, ReliveTypes::eDoor));
                BaseAliveGameObjectPathTLV = pPathDoor;
                
                if (!BaseAliveGameObjectPathTLV)
                {
                    ALIVE_FATAL("Couldn't find target door. If this is a custom level, check if the level, path and camera is correct.");
                }
            }

            if (pPathDoor->mDoorType == relive::Path_Door::DoorTypes::eBasicDoor)
            {
                if (pPathDoor->mScale == relive::reliveScale::eHalf)
                {
                    SetSpriteScale(FP_FromDouble(0.5));
                    GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                    SetScale(Scale::Bg);
                }
                else
                {
                    SetSpriteScale(FP_FromInteger(1));
                    GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                    SetScale(Scale::Fg);
                }
            }
            else if (pPathDoor->mDoorType == relive::Path_Door::DoorTypes::eTrialDoor || pPathDoor->mDoorType == relive::Path_Door::DoorTypes::eHubDoor)
            {
                if (gMap.mCurrentLevel != EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SetSpriteScale(FP_FromDouble(0.5));
                    SetScale(Scale::Bg);
                }
                else
                {
                    SetSpriteScale(FP_FromInteger(1));
                    SetScale(Scale::Fg);
                }
            }
            GetAnimation().SetFlipX(pPathDoor->mExitDirection == relive::reliveXDirection::eRight);
            mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->Width()) / 2)
                          + FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftX);
            MapFollowMe(true);

            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY),
                    &BaseAliveGameObjectCollisionLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos = hitY;
            }

            field_110_state.door = AbeDoorStates::eAbeComesOut_6;
            field_118_timer = sGnFrame + 30;
            return;
        }
        case AbeDoorStates::eAbeComesOut_6:
        {
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                GetAnimation().SetRender(true);
                field_110_state.door = AbeDoorStates::eAbeComesIn_0;
                mCurrentMotion = eAbeMotions::Motion_157_DoorExit;
            }
            return;
        }
        default:
            return;
    }
}

void Abe::Motion_157_DoorExit()
{
    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);

        ToIdle_422D50();
    }
}

void Abe::Motion_158_ElumKnockback()
{
    // Empty
}

void Abe::Motion_159_Idle_RubEyes()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_422D50();
    }
}

void Abe::Motion_160_Idle_Stretch_Arms()
{
    Motion_159_Idle_RubEyes();
}

void Abe::Motion_161_Idle_Yawn()
{
    Motion_159_Idle_RubEyes();
}

void Abe::Motion_162_ToShrykull()
{
    if (field_110_state.raw == 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().SetAnimate(false);
            GetAnimation().SetRender(false);

            field_110_state.raw = 1;

            relive_new Shrykull();
        }
    }
}

void Abe::Motion_163_ShrykullEnd()
{
    if (field_114_gnFrame)
    {
        field_114_gnFrame = field_114_gnFrame - 1;
    }
    else if (GetAnimation().GetForwardLoopCompleted())
    {
        mCurrentMotion = eAbeMotions::Motion_9_Speak;
        Mudokon_SFX(MudSounds::eLaugh1_8, 0, 0, this);
    }
}

void Abe::Motion_164_PoisonGasDeath()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 128);
            break;
        case 9:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 384);
            break;
        case 28:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 640);
            break;
        case 32:
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, 0, this);
            break;
        case 50:
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 100, -200, this);
            break;
        case 53:
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 50, -200, this);
            break;
        default:
            break;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        const s32 v2 = field_114_gnFrame;
        field_114_gnFrame--;
        if (v2 == 0)
        {
            ToDieFinal_42C400();
        }
    }
}

} // namespace AO

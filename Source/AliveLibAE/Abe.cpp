#include "stdafx.h"
#include "Abe.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Sound/Midi.hpp"
#include "Sfx.hpp"
#include "DebugHelpers.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ObjectIds.hpp"
#include "Input.hpp"
#include "Events.hpp"
#include "QuikSave.hpp"
#include "MainMenu.hpp"
#include "ThrowableArray.hpp"
#include "MusicController.hpp"
#include "Path.hpp"
#include "GameSpeak.hpp"
#include "TrapDoor.hpp"
#include "MusicTrigger.hpp"
#include "ScreenShake.hpp"
#include "InvisibleEffect.hpp"
#include "Gibs.hpp"
#include "Throwable.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "AbilityRing.hpp"
#include "OrbWhirlWind.hpp"
#include "Blood.hpp"
#include "PullRingRope.hpp"
#include "CircularFade.hpp"
#include "DeathFadeOut.hpp"
#include "Movie.hpp"
#include "PossessionFlicker.hpp"
#include "Door.hpp"
#include "QuikSave.hpp"
#include "DeathBirdParticle.hpp"
#include "WorkWheel.hpp"
#include "LevelLoader.hpp"
#include "Particle.hpp"
#include "Lever.hpp"
#include "Throwable.hpp"
#include "LiftPoint.hpp"
#include "PullRingRope.hpp"
#include "BrewMachine.hpp"
#include "MineCar.hpp"
#include "EvilFart.hpp"
#include "Particle.hpp"
#include "Mudokon.hpp"
#include "Electrocute.hpp"
#include "BirdPortal.hpp"
#include "BoomMachine.hpp"
#include "Shrykull.hpp"
#include "Bullet.hpp"
#include "Spark.hpp"
#include "TestAnimation.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"
#include "../relive_lib/Math.hpp"

using TAbeMotionFunction = decltype(&Abe::Motion_0_Idle_44EEB0);

#define MAKE_STRINGS(VAR) #VAR,
const char_type* const sAbeMotionNames[] = {
    ABE_MOTIONS_ENUM(MAKE_STRINGS)};

const TAbeMotionFunction sAbeMotionMachineTable_554910[] = {
    &Abe::Motion_0_Idle_44EEB0,
    &Abe::Motion_1_WalkLoop_44FBA0,
    &Abe::Motion_2_StandingTurn_451830,
    &Abe::Motion_3_Fall_459B60,
    &Abe::Motion_4_WalkToIdle_44FFC0,
    &Abe::Motion_5_MidWalkToIdle_450080,
    &Abe::Motion_6_WalkBegin_44FEE0,
    &Abe::Motion_7_Speak_45B140,
    &Abe::Motion_8_Speak_45B160,
    &Abe::Motion_9_Speak_45B180,
    &Abe::Motion_10_Fart_45B1A0,
    &Abe::Motion_11_ToSpeak_45B0A0,
    &Abe::Motion_12_Null_4569C0,
    &Abe::Motion_13_HoistBegin_452B20,
    &Abe::Motion_14_HoistIdle_452440,
    &Abe::Motion_15_HoistLand_452BA0,
    &Abe::Motion_16_LandSoft_45A360,
    &Abe::Motion_17_CrouchIdle_456BC0,
    &Abe::Motion_18_CrouchToStand_454600,
    &Abe::Motion_19_StandToCrouch_453DC0,
    &Abe::Motion_20_CrouchSpeak_454550,
    &Abe::jMotion_21_ToCrouchSpeak_4545E0,
    &Abe::Motion_22_RollBegin_4539A0,
    &Abe::Motion_23_RollLoop_453A90,
    &Abe::Motion_24_453D00,
    &Abe::Motion_25_RunSlideStop_451330,
    &Abe::Motion_26_RunTurn_451500,
    &Abe::Motion_27_HopBegin_4521C0,
    &Abe::Motion_28_HopMid_451C50,
    &Abe::Motion_29_HopLand_4523D0,
    &Abe::Motion_30_RunJumpBegin_4532E0,
    &Abe::Motion_31_RunJumpMid_452C10,
    &Abe::Motion_32_RunJumpLand_453460,
    &Abe::Motion_33_RunLoop_4508E0,
    &Abe::Motion_34_DunnoBegin_44ECF0,
    &Abe::Motion_35_DunnoEnd_44ED10,
    &Abe::Motion_36_Null_45BC50,
    &Abe::Motion_37_CrouchTurn_454390,
    &Abe::jMotion_38_RunToRoll_453A70,
    &Abe::Motion_39_StandingToRun_450D40,
    &Abe::Motion_40_SneakLoop_450550,
    &Abe::Motion_41_WalkToSneak_450250,
    &Abe::Motion_42_SneakToWalk_4503D0,
    &Abe::Motion_43_MidWalkToSneak_450380,
    &Abe::Motion_44_MidSneakToWalk_450500,
    &Abe::Motion_45_SneakBegin_4507A0,
    &Abe::Motion_46_SneakToIdle_450870,
    &Abe::jMotion_47_MidSneakToIdle_4508C0,
    &Abe::Motion_48_WalkToRun_4500A0,
    &Abe::Motion_49_MidWalkToRun_450200,
    &Abe::Motion_50_RunToWalk_450E20,
    &Abe::Motion_51_MidRunToWalk_450F50,
    &Abe::Motion_52_RunTurnToRun_451710,
    &Abe::Motion_53_RunTurnToWalk_451800,
    &Abe::Motion_54_RunJumpLandRun_4538F0,
    &Abe::Motion_55_RunJumpLandWalk_453970,
    &Abe::Motion_56_DeathDropFall_4591F0,
    &Abe::Motion_57_Dead_4589A0,
    &Abe::Motion_58_DeadPre_4593E0,
    &Abe::Motion_59_Null_459450,
    &Abe::Motion_60_Unused_4A3200,
    &Abe::Motion_61_TurnToRun_456530,
    &Abe::Motion_62_Punch_454750,
    &Abe::Motion_63_Sorry_454670,
    &Abe::Motion_64_AfterSorry_454730,
    &Abe::Motion_65_LedgeAscend_4548E0,
    &Abe::Motion_66_LedgeDescend_454970,
    &Abe::Motion_67_LedgeHang_454E20,
    &Abe::Motion_68_ToOffScreenHoist_454B80,
    &Abe::Motion_69_LedgeHangWobble_454EF0,
    &Abe::Motion_70_RingRopePullHang_455AF0,
    &Abe::Motion_71_Knockback_455090,
    &Abe::Motion_72_KnockbackGetUp_455340,
    &Abe::Motion_73_PushWall_4553A0,
    &Abe::Motion_74_RollingKnockback_455290,
    &Abe::Motion_75_JumpIntoWell_45C7B0,
    &Abe::Motion_76_ToInsideOfAWellLocal_45CA40,
    &Abe::Motion_77_ToWellShotOut_45D130,
    &Abe::Motion_78_WellBegin_45C810,
    &Abe::Motion_79_InsideWellLocal_45CA60,
    &Abe::Motion_80_WellShotOut_45D150,
    &Abe::jMotion_81_WellBegin_45C7F0,
    &Abe::Motion_82_InsideWellExpress_45CC80,
    &Abe::Motion_83_WellExpressShotOut_45CF70,
    &Abe::Motion_84_FallLandDie_45A420,
    &Abe::jMotion_85_Fall_455070,
    &Abe::Motion_86_HandstoneBegin_45BD00,
    &Abe::Motion_87_HandstoneEnd_45C4F0,
    &Abe::Motion_88_GrenadeMachineUse_45C510,
    &Abe::Motion_89_BrewMachineBegin_4584C0,
    &Abe::Motion_90_BrewMachineEnd_4585B0,
    &Abe::Motion_91_FallingFromGrab_4557B0,
    &Abe::Motion_92_ForceDownFromHoist_455800,
    &Abe::Motion_93_WalkOffEdge_455970,
    &Abe::Motion_94_RunOffEdge_4559A0,
    &Abe::Motion_95_SneakOffEdge_4559C0,
    &Abe::Motion_96_HopToFall_4559E0,
    &Abe::Motion_97_RunJumpToFall_455A80,
    &Abe::Motion_98_RollOffEdge_455AA0,
    &Abe::Motion_99_LeverUse_455AC0,
    &Abe::Motion_100_SlapBomb_455B60,
    &Abe::Motion_101_KnockForward_455420,
    &Abe::Motion_102_RollingKnockForward_455310,
    &Abe::jMotion_103_KnockForwardGetUp_455380,
    &Abe::Motion_104_RockThrowStandingHold_455DF0,
    &Abe::Motion_105_RockThrowStandingThrow_456460,
    &Abe::Motion_106_RockThrowStandingEnd_455F20,
    &Abe::Motion_107_RockThrowCrouchingHold_454410,
    &Abe::Motion_108_RockThrowCrouchingThrow_454500,
    &Abe::Motion_109_ZShotRolling_455550,
    &Abe::Motion_110_ZShot_455670,
    &Abe::Motion_111_PickupItem_4564A0,
    &Abe::Motion_112_Chant_45B1C0,
    &Abe::Motion_113_ChantEnd_45BBE0,
    &Abe::Motion_114_DoorEnter_459470,
    &Abe::Motion_115_DoorExit_459A40,
    &Abe::Motion_116_MineCarEnter_458780,
    &Abe::Motion_117_InMineCar_4587C0,
    &Abe::Motion_118_MineCarExit_458890,
    &Abe::Motion_119_ToShrykull_45A990,
    &Abe::Motion_120_EndShrykull_45AB00,
    &Abe::Motion_121_LiftGrabBegin_45A600,
    &Abe::Motion_122_LiftGrabEnd_45A670,
    &Abe::Motion_123_LiftGrabIdle_45A6A0,
    &Abe::Motion_124_LiftUseUp_45A780,
    &Abe::Motion_125_LiftUseDown_45A7B0,
    &Abe::Motion_126_TurnWheelBegin_456700,
    &Abe::Motion_127_TurnWheelLoop_456750,
    &Abe::Motion_128_TurnWheelEnd_4569A0,
    &Abe::Motion_129_PoisonGasDeath_4565C0};

const SfxDefinition sAbeSFXList_555250[41] = {
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 24u, 60u, 115u, 0, 0},
    {0u, 24u, 61u, 110u, 0, 0},
    {0u, 24u, 62u, 127u, 0, 0},
    {0u, 24u, 63u, 90u, 0, 0},
    {0u, 24u, 66u, 127u, 0, 0},
    {0u, 24u, 67u, 100u, 0, 0},
    {0u, 24u, 56u, 127u, 0, 0},
    {0u, 24u, 57u, 100u, 0, 0},
    {0u, 23u, 48u, 127u, 0, 0},
    {0u, 24u, 59u, 127u, 0, 0},
    {0u, 24u, 58u, 127u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 24u, 65u, 127u, 0, 0},
    {0u, 24u, 64u, 80u, 0, 0},
    {0u, 23u, 60u, 90u, 0, 0},
    {0u, 23u, 68u, 127u, 0, 0},
    {0u, 23u, 69u, 127u, 0, 0},
    {0u, 23u, 70u, 127u, 0, 0},
    {0u, 23u, 72u, 127u, 0, 0},
    {0u, 23u, 73u, 127u, 0, 0},
    {0u, 23u, 58u, 127u, 0, 0},
    {0u, 23u, 64u, 127u, 0, 0},
    {0u, 23u, 66u, 115u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 23u, 63u, 115u, 0, 0},
    {0u, 23u, 62u, 90u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

const AnimId sAbeAnimIdTable[130] = {
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Walk,
    AnimId::Mudokon_StandingTurn,
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_WalkToIdle,
    AnimId::Mudokon_MidWalkToIdle,
    AnimId::Mudokon_WalkBegin,
    AnimId::Mudokon_Speak1,
    AnimId::Mudokon_Speak2,
    AnimId::Mudokon_Speak3,
    AnimId::Mudokon_SpeakFart,
    AnimId::Mudokon_ToSpeak,
    AnimId::Mudokon_Null,
    AnimId::Mudokon_HoistBegin,
    AnimId::Mudokon_HoistIdle,
    AnimId::Mudokon_LandSoft,
    AnimId::Mudokon_LandSoft,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchToStand,
    AnimId::Mudokon_StandToCrouch,
    AnimId::Mudokon_CrouchSpeak,
    AnimId::Mudokon_ToCrouchSpeak,
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
    AnimId::Mudokon_DunnoEnd,
    AnimId::Mudokon_DunnoBegin,
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
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_CrouchIdle,
    AnimId::Mudokon_TurnToRun,
    AnimId::Mudokon_Punch,
    AnimId::Mudokon_Sorry,
    AnimId::Mudokon_AfterSorry,
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
    AnimId::Mudokon_FallLandDie,
    AnimId::Mudokon_Fall,
    AnimId::Mudokon_HandstoneBegin,
    AnimId::Mudokon_HandstoneEnd,
    AnimId::Mudokon_GrenadeMachineUse,
    AnimId::Mudokon_HandstoneBegin,
    AnimId::Mudokon_HandstoneEnd,
    AnimId::Mudokon_FallingFromGrab,
    AnimId::Mudokon_FallingFromGrab,
    AnimId::Mudokon_WalkOffEdge,
    AnimId::Mudokon_RunOffEdge,
    AnimId::Mudokon_SneakOffEdge,
    AnimId::Mudokon_HopToFall,
    AnimId::Mudokon_RunJumpToFall,
    AnimId::Mudokon_RollOffEdge,
    AnimId::Mudokon_LeverUse,
    AnimId::Mudokon_SlapBomb,
    AnimId::Mudokon_KnockForward,
    AnimId::Mudokon_RollingKnockForward,
    AnimId::Mudokon_Idle, // KnockForwardGetUp?
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
    AnimId::Mudokon_DoorEnter,
    AnimId::Mudokon_DoorExit,
    AnimId::Mudokon_MineCarEnter,
    AnimId::Mudokon_MineCarEnter,
    AnimId::Mudokon_MineCarExit,
    AnimId::Mudokon_ToShrykull,
    AnimId::Mudokon_EndShrykull,
    AnimId::Mudokon_LiftGrabBegin,
    AnimId::Mudokon_LiftGrabEnd,
    AnimId::Mudokon_LiftGrabIdle,
    AnimId::Mudokon_LiftUseUp,
    AnimId::Mudokon_LiftUseDown,
    AnimId::Mudokon_TurnWheelBegin,
    AnimId::Mudokon_TurnWheel,
    AnimId::Mudokon_TurnWheelEnd,
    AnimId::Mudokon_PoisonGasDeath};

const TintEntry sTintTable_Abe_554D20[15] = {
    {EReliveLevelIds::eMines, 102u, 102u, 102u},
    {EReliveLevelIds::eNecrum, 102u, 102u, 80u},
    {EReliveLevelIds::eMudomoVault, 120u, 90u, 120u},
    {EReliveLevelIds::eMudancheeVault, 102u, 70u, 90u},
    {EReliveLevelIds::eFeeCoDepot, 120u, 102u, 82u},
    {EReliveLevelIds::eBarracks, 102u, 102u, 102u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 70u, 90u},
    {EReliveLevelIds::eBonewerkz, 102u, 102u, 102u},
    {EReliveLevelIds::eBrewery, 102u, 102u, 102u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 102u, 102u},
    {EReliveLevelIds::eMudomoVault_Ender, 120u, 90u, 120u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 120u, 102u, 82u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 102u, 102u},
    {EReliveLevelIds::eBonewerkz_Ender, 120u, 90u, 80u},
    {EReliveLevelIds::eNone, 102u, 102u, 102u}};

const SfxDefinition sSFXList_555160[] = {
    {0u, 3u, 69u, 60u, -1, 1},
    {0u, 3u, 70u, 60u, -1, 1},
    {0u, 3u, 59u, 67u, -1, 1},
    {0u, 3u, 61u, 67u, -1, 1},
    {0u, 3u, 58u, 52u, -1, 1},
    {0u, 3u, 67u, 45u, -1, 1},
    {0u, 3u, 57u, 60u, -1, 1},
    {0u, 3u, 65u, 50u, -1, 1},
    {0u, 3u, 68u, 60u, -1, 1},
    {0u, 3u, 72u, 70u, -1, 1},
    {0u, 3u, 36u, 50u, -127, 0},
    {0u, 3u, 37u, 50u, -127, 0},
    {0u, 3u, 38u, 50u, -127, 0},
    {0u, 3u, 36u, 55u, 0, 127},
    {0u, 3u, 37u, 55u, 0, 127},
    {0u, 3u, 38u, 55u, 0, 127},
    {0u, 3u, 61u, 50u, -1, 1},
    {0u, 3u, 73u, 127u, -1, 1},
    {0u, 4u, 80u, 127u, -1, 1},
    {0u, 65u, 64u, 60u, -1, 0},
    {0u, 11u, 62u, 60u, -1, 1},
    {0u, 11u, 64u, 60u, -1, 1},
    {0u, 11u, 65u, 50u, -1, 1},
    {0u, 11u, 64u, 50u, -1, 1},
    {0u, 11u, 66u, 50u, -1, 1},
    {0u, 23u, 60u, 90u, 0, 0},
    {0u, 23u, 61u, 90u, 0, 0},
    {0u, 23u, 62u, 90u, 0, 0},
    {0u, 23u, 63u, 90u, 0, 0},
    {0u, 23u, 64u, 90u, 0, 0}};


bool IsActiveHero(BaseGameObject* pObj)
{
    if (sActiveHero)
    {
        return sActiveHero == pObj;
    }
    return false;
}

s32 Environment_SFX_457A40(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj)
{
    s32 sndVolume;
    s16 sndIndex = 0;

    switch (sfxId)
    {
        case EnvironmentSfx::eSlideStop_0:
            sndIndex = 11;
            sndVolume = 40;
            break;

        case EnvironmentSfx::eWalkingFootstep_1:
            sndVolume = volume;
            sndIndex = Math_RandomRange(10, 12);
            if (volume)
            {
                break;
            }
            sndVolume = Math_RandomRange(54, 58);
            if (pAliveObj)
            {
                if (pAliveObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
                {
                    sndVolume *= 3;
                }
            }
            break;

        case EnvironmentSfx::eRunningFootstep_2:
            sndVolume = volume;
            sndIndex = Math_RandomRange(13, 15);
            if (volume)
            {
                break;
            }

            sndVolume = Math_RandomRange(66, 70);

            if (pAliveObj)
            {
                if (pAliveObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
                {
                    sndVolume *= 3;
                }
            }
            break;

        case EnvironmentSfx::eSneakFootstep_3:
            sndIndex = 5;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eRunSlide_4:
            sndIndex = 4;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eLandingSoft_5:
            if (volume || !pAliveObj || pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5))
            {
                return SFX_SfxDefinition_Play_Mono(&sSFXList_555160[2], static_cast<s16>(volume), static_cast<s16>(pitchMin), 0x7FFF) | SFX_SfxDefinition_Play_Mono(&sAbeSFXList_555250[16], static_cast<s16>(volume), static_cast<s16>(pitchMin), 0x7FFF);
            }
            else
            {
                return SFX_SfxDefinition_Play_Mono(&sSFXList_555160[2], sSFXList_555160[2].field_3_default_volume / 2, static_cast<s16>(pitchMin), 0x7FFF) | SFX_SfxDefinition_Play_Mono(&sAbeSFXList_555250[16], sSFXList_555160[16].field_3_default_volume / 2, static_cast<s16>(pitchMin), 0x7FFF);
            }

        case EnvironmentSfx::eHitGroundSoft_6:
            sndIndex = 2;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eDeathNoise_7:
            sndIndex = 16;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eRollingNoise_8:
            sndIndex = 6;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eGenericMovement_9:
        case EnvironmentSfx::eRunJumpOrLedgeHoist_11:
            if (pAliveObj && pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                return SfxPlayMono(SoundEffect::AbeGenericMovement_32, 20);
            }
            else
            {
                return SfxPlayMono(SoundEffect::AbeGenericMovement_32, 35);
            }

        case EnvironmentSfx::eExhaustingHoistNoise_10:
            sndIndex = 19;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eAllOYa_12:
            sndIndex = 25;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eKnockback_13:
            if (gMap.mCurrentLevel == EReliveLevelIds::eMines
                || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
                || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
                || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
                || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
            {
                sndIndex = 2;
            }
            else
            {
                sndIndex = 9;
            }
            sndVolume = volume;
            break;

        case EnvironmentSfx::eElumHitWall_14:
            sndIndex = 17;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eFallingDeathScreamHitGround_15:
            return 0;

        default:
            sndVolume = volume;
            break;
    }

    if (!sndVolume)
    {
        sndVolume = sSFXList_555160[sndIndex].field_3_default_volume;
    }

    if (!pAliveObj)
    {
        return SFX_SfxDefinition_Play_Mono(&sSFXList_555160[sndIndex], static_cast<s16>(sndVolume), static_cast<s16>(pitchMin), 0x7FFF);
    }

    if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        sndVolume = 2 * sndVolume / 3;
    }

    if (!IsActiveHero(pAliveObj))
    {
        switch (gMap.GetDirection(
            pAliveObj->mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            pAliveObj->mBaseAnimatedWithPhysicsGameObject_PathNumber,
            pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos,
            pAliveObj->mBaseAnimatedWithPhysicsGameObject_YPos))
        {
            case CameraPos::eCamCurrent_0:
                return SFX_SfxDefinition_Play_Mono(&sSFXList_555160[sndIndex], static_cast<s16>(sndVolume), static_cast<s16>(pitchMin), 0x7FFF);
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
                return SFX_SfxDefinition_Play_Mono(&sSFXList_555160[sndIndex], static_cast<s16>(2 * sndVolume / 3), static_cast<s16>(pitchMin), 0x7FFF);
            case CameraPos::eCamLeft_3:
                return SFX_SfxDefinition_Play_Stereo(
                    &sSFXList_555160[sndIndex],
                    static_cast<s16>(2 * sndVolume / 9),
                    static_cast<s16>(2 * sndVolume / 9),
                    static_cast<s16>(pitchMin),
                    0x7FFF);
                break;
            case CameraPos::eCamRight_4:
                return SFX_SfxDefinition_Play_Stereo(
                    &sSFXList_555160[sndIndex],
                    static_cast<s16>(2 * sndVolume / 3),
                    static_cast<s16>(2 * sndVolume / 3),
                    static_cast<s16>(pitchMin),
                    0x7FFF);
                break;
            default:
                return 0;
        }
    }
    else
    {
        return SFX_SfxDefinition_Play_Mono(&sSFXList_555160[sndIndex], static_cast<s16>(sndVolume), static_cast<s16>(pitchMin), 0x7FFF);
    }
}

const FP_Point sThrowVelocities_555118[9] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)},
    {FP_FromInteger(0), FP_FromInteger(0)},
    {FP_FromInteger(0), FP_FromInteger(0)},
    {FP_FromInteger(0), FP_FromInteger(0)}};


s32 Animation_OnFrame_Abe_455F80(BaseGameObject* pPtr, s16* pData)
{
    auto pAbe = static_cast<Abe*>(pPtr);
    auto pFramePos = reinterpret_cast<PSX_Point*>(pData);

    auto pThrowable = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(sActiveHero->field_158_throwable_id));

    auto tableX = sThrowVelocities_555118[pAbe->field_1A3_throw_direction].x * pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    const auto tableY = sThrowVelocities_555118[pAbe->field_1A3_throw_direction].y * pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    FP xOff = {};
    if (sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        tableX = -tableX;
        xOff = -(pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pFramePos->x));
    }
    else
    {
        xOff = pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pFramePos->x);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            pAbe->mBaseAnimatedWithPhysicsGameObject_XPos,
            pAbe->mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pFramePos->y),
            xOff + pAbe->mBaseAnimatedWithPhysicsGameObject_XPos,
            pAbe->mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pFramePos->y),
            &pLine,
            &hitX,
            &hitY,
            pAbe->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls))
    {
        xOff = hitX - pAbe->mBaseAnimatedWithPhysicsGameObject_XPos;
        tableX = -tableX;
    }

    if (pThrowable)
    {
        pThrowable->mBaseAnimatedWithPhysicsGameObject_XPos = xOff + sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos;
        pThrowable->mBaseAnimatedWithPhysicsGameObject_YPos = (pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pFramePos->y)) + sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos;
        pThrowable->VThrow(tableX, tableY);
        pThrowable->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        pThrowable->mBaseAnimatedWithPhysicsGameObject_Scale = pAbe->mBaseAnimatedWithPhysicsGameObject_Scale;
        sActiveHero->field_158_throwable_id = -1;
    }

    return 1;
}

enum AbeResources
{
    eAbeBSic = 0,
    eAbeBSic1 = 1,

    eAbeWell = 15,
};

s32 XGrid_Index_To_XPos_4498F0(FP scale, s32 xGridIndex)
{
    if (scale == FP_FromDouble(0.5))
    {
        // 12.5 = half grid size
        return (13 * xGridIndex) - 8;
    }

    if (scale == FP_FromDouble(1.0))
    {
        // 25 = full grid size
        return (25 * xGridIndex) - 13;
    }

    // Default to middle of the screen
    return (374 / 2);
}

ALIVE_VAR(1, 0x5c1b8c, BaseAliveGameObject*, sControlledCharacter_5C1B8C, nullptr);

static constexpr s32 kResourceArraySize = 28;

Abe::Abe(s32 /*frameTableOffset*/, s32 /*r*/, s32 /*g*/, s32 /*b*/)
    : BaseAliveGameObject(kResourceArraySize)
{
    SetType(ReliveTypes::eAbe);

    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectTlvInfo = -65536;

    // Set the well level to the current level for the path start quick save
    field_19A_to_level = gMap.mCurrentLevel;

    Init_GameStates_43BF40();

    // Zero out the resource array
    for (s32 i = 0; i < kResourceArraySize; i++)
    {
        field_10_resources_array.SetAt(i, nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC.BAN", nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC1.BAN", nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic1, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEEDGE.BAN", nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNFD.BAN", nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEOMM.BAN", nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNBK.BAN", nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, TRUE, FALSE);
    }

    // OG BUG FIX: Speed runners will sometimes go through the floor to land in a well in a camera that didn't have its objects loaded yet.
    // When falling into a well the abe well entering animation isn't loaded if there wasn't a well in the current 4 cameras. As such the animation is set to
    // junk memory and sometimes you can get stuck because the frame delay in the random memory is some stupidly high value, also unsurprisingly this
    // memory corruption can lead to a crash later on. I think this gets freed by the base class OK.
    const auto& wellRec = AnimRec(AnimId::Mudokon_Well_Idle);
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, wellRec.mResourceId, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170(wellRec.mBanName, nullptr);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, wellRec.mResourceId, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeWell, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, wellRec.mResourceId, FALSE, FALSE));


    ResourceManager::LoadResourceFile_49C170("ABENOELM.BND", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbefallResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbesmashResID);

    ResourceManager::LoadResourceFile_49C170("OMMFLARE.BAN", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kOmmflareResID);

    ResourceManager::LoadResourceFile_49C170("SQBSMK.BAN", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSquibSmokeResID);

    ResourceManager::LoadResourceFile_49C170("DUST.BAN", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDustResID);

    const AnimRecord& bloodDropRec = AnimRec(AnimId::BloodDrop);
    ResourceManager::LoadResourceFile_49C170(bloodDropRec.mBanName, nullptr);
    Add_Resource(ResourceManager::Resource_Animation, bloodDropRec.mResourceId);

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID);

    ResourceManager::LoadResourceFile_49C170("DEADFLR.BAN", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDeathFlareResID);

    ResourceManager::LoadResourceFile_49C170("DOVBASIC.BAN", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDovbasicResID);

    ResourceManager::LoadResourceFile_49C170("SPOTLITE.BAN", nullptr);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID);

    field_128.field_10_resource_index = 1;

    const AnimRecord& rec = AnimRec(AnimId::Mudokon_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mFnPtrArray = kAbe_Anim_Frame_Fns_55EF98;

    PSX_Point point = {};
    gMap.GetCurrentCamCoords(&point);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(point.x + XGrid_Index_To_XPos_4498F0(mBaseAnimatedWithPhysicsGameObject_SpriteScale, 4));
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(point.y + 120);

    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
    field_128.field_C_unused = 0;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    BaseAliveGameObjectCollisionLine = nullptr;
    mCurrentMotion = eAbeMotions::Motion_3_Fall_459B60;
    field_122_knockdown_motion = -1;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    field_198_has_evil_fart = 0;
    field_1A2_throwable_count = 0;
    field_158_throwable_id = -1;
    field_154_possessed_object_id = -1;
    field_150_OrbWhirlWind_id = -1;
    field_14C_circular_fade_id = -1;
    field_148_fade_obj_id = -1;
    field_1A8_portal_id = -1;
    field_164_wheel_id = -1;
    field_160_slappable_or_pick_item_id = -1;
    field_15C_pull_rope_id = -1;

    field_1AE_flags.Clear(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender);
    field_1AE_flags.Clear(Flags_1AE::e1AE_Bit2_do_quicksave);

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_1AC_flags.Raw().all = 0;

    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6_prevent_chanting);
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);

    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_shrivel);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_WalkToRun);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift);

    // Changes Abe's "default" colour depending on the level we are in
    SetTint(&sTintTable_Abe_554D20[0], gMap.mCurrentLevel);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_0;

    field_118_prev_held = 0;
    mBaseAliveGameObjectLastAnimFrame = 0;
    field_120_state.raw = 0;
    field_168_ring_pulse_timer = 0;
    field_16E_bHaveInvisiblity = 0;
    field_170_invisible_timer = 0;
    field_176_invisibility_duration = 0;
    field_178_invisible_effect_id = -1;
    field_124_timer = sGnFrame;
    BaseAliveGameObjectPathTLV = nullptr;
    field_128.field_12_mood = Mud_Emotion::eNormal_0;
    field_128.field_18_say = MudSounds::eNone;
    field_144_auto_say_timer = 0;

    // Set Abe to be the current player controlled object
    sControlledCharacter_5C1B8C = this;

    // Create shadow
    mShadow = relive_new Shadow();

    // Animation test code
    //auto testAnim = relive_new TestAnimation(); testAnim->ctor();

}

Abe::~Abe()
{
    BaseGameObject* pFadeObject = sObjectIds.Find_Impl(field_148_fade_obj_id);
    BaseGameObject* pCircularFade = sObjectIds.Find_Impl(field_14C_circular_fade_id);
    BaseGameObject* pOrbWhirlWind = sObjectIds.Find_Impl(field_150_OrbWhirlWind_id);
    BaseGameObject* pPossessedObject = sObjectIds.Find_Impl(field_154_possessed_object_id);
    BaseGameObject* pThrowable = sObjectIds.Find_Impl(field_158_throwable_id);
    BaseGameObject* pPullRope = sObjectIds.Find_Impl(field_15C_pull_rope_id);
    BaseGameObject* pItem = sObjectIds.Find_Impl(field_160_slappable_or_pick_item_id);
    BaseGameObject* pInvisibleEffect = sObjectIds.Find_Impl(field_178_invisible_effect_id);

    SND_SEQ_Stop(SeqId::MudokonChant1_10);

    if (pFadeObject)
    {
        pFadeObject->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_148_fade_obj_id = -1;
    }

    if (pItem)
    {
        field_160_slappable_or_pick_item_id = -1;
    }

    if (pPullRope)
    {
        pPullRope->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_15C_pull_rope_id = -1;
    }

    if (pCircularFade)
    {
        pCircularFade->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_14C_circular_fade_id = -1;
    }

    if (pOrbWhirlWind)
    {
        pOrbWhirlWind->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_150_OrbWhirlWind_id = -1;
    }

    if (pPossessedObject)
    {
        field_154_possessed_object_id = -1;
    }

    if (pThrowable)
    {
        pThrowable->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_158_throwable_id = -1;
    }

    if (pInvisibleEffect)
    {
        pInvisibleEffect->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_178_invisible_effect_id = -1;
    }

    field_164_wheel_id = -1;

    sActiveHero = nullptr;
}

const char_type* sAbe_ResNames_545830[22] = {
    "ABEBASIC.BAN",
    "ABEBSIC1.BAN",
    "ABEPULL.BAN",
    "ABEPICK.BAN",
    "ABEBOMB.BAN",
    "ABETHROW.BAN",
    "ABESMASH.BAN",
    "ABEFALL.BAN",
    "ABESTONE.BAN",
    "ABEKNBK.BAN",
    "ABEKNFD.BAN",
    "ABEKNOKZ.BAN",
    "ABEHOIST.BAN",
    "ABEEDGE.BAN",
    "ABEDOOR.BAN",
    "ABEWELL.BAN",
    "ABEOMM.BAN",
    "ABELIFT.BAN",
    "ABECAR.BAN",
    "ABEMORPH.BAN",
    "ABEWORK.BAN",
    "ABEGAS.BAN",
};

const s32 sAbeResourceIDTable_554D60[22] = {
    AEResourceID::kAbebasicResID, // 10
    AEResourceID::kAbebsic1ResID, // 55
    AEResourceID::kAbepullResID,  // 11
    AEResourceID::kAbepickResID,  // 12
    AEResourceID::kAbebombResID,  // 13
    AEResourceID::kAbethrowResID, // 14
    AEResourceID::kAbesmashResID, // 19
    AEResourceID::kAbefallResID,  // 20
    AEResourceID::kAbestoneResID, // 21
    AEResourceID::kAbeknbkResID,  // 26
    AEResourceID::kAbeknfdResID,  // 27
    AEResourceID::kAbeknokzResID, // 28
    AEResourceID::kAbehoistResID, // 42
    AEResourceID::kAbeedgeResID,  // 43
    AEResourceID::kAbedoorResID,  // 45
    AEResourceID::kAbewellResID,  // 47
    AEResourceID::kAbeommResID,   // 48
    AEResourceID::kAbeliftResID,  // 53
    AEResourceID::kAbeCarResId,   // 113
    AEResourceID::kAbemorphResID, // 117
    AEResourceID::kAbeworkResID,  // 515
    AEResourceID::kAbegasResID,   // 118
};


enum ResourceIndices
{
    eBasic_0 = 0,
    eBasic1_1 = 1,
    ePull_2 = 2,
    ePick_3 = 3,
    eBomb_4 = 4,
    eThrow_5 = 5,
    eSmash_6 = 6,
    eFall_7 = 7,
    eStone_8 = 8,
    eKnockBack_9 = 9,
    eKnockFd_10 = 10,
    eKnockZ_11 = 11,
    eHosit_12 = 12,
    eEdge_13 = 13,
    eDoor_14 = 14,
    eWell_15 = 15,
    eChant_16 = 16,
    eLift_17 = 17,
    eMineCar_18 = 18,
    eMorph_19 = 19,
    eWork_20 = 20,
    eGas_21 = 21,
};

s32 Abe::CreateFromSaveState(const u8* pData)
{
    const Abe_SaveState* pSaveState = reinterpret_cast<const Abe_SaveState*>(pData);

    Abe* pAbe = sActiveHero;
    if (!sActiveHero)
    {
        pAbe = relive_new Abe(58808, 85, 57, 55);
        sActiveHero = pAbe;
    }

    if (pSaveState->field_44_is_abe_controlled)
    {
        sControlledCharacter_5C1B8C = pAbe;
    }

    sActiveHero->BaseAliveGameObjectPathTLV = nullptr;
    sActiveHero->BaseAliveGameObjectCollisionLine = nullptr;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos = pSaveState->field_4_xpos;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos = pSaveState->field_8_ypos;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_VelX = pSaveState->field_c_velx;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_VelY = pSaveState->field_10_vely;
    sActiveHero->field_128.field_8_x_vel_slow_by = pSaveState->field_48_x_vel_slow_by;
    sActiveHero->field_128.field_C_unused = pSaveState->field_4C_unused;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_PathNumber = pSaveState->field_14_path_number;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pSaveState->field_16_lvl_number);
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pSaveState->field_18_sprite_scale;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Scale = pSaveState->field_1C_scale;

    sActiveHero->mCurrentMotion = pSaveState->current_motion;

    u8** animFromState = sActiveHero->MotionToAnimResource_44AAB0(sActiveHero->mCurrentMotion);
    if (!animFromState)
    {
        u32 id = sAbeResourceIDTable_554D60[sActiveHero->field_128.field_10_resource_index];
        ResourceManager::LoadResourceFile_49C170(sAbe_ResNames_545830[sActiveHero->field_128.field_10_resource_index], 0);
        sActiveHero->field_10_resources_array.SetAt(sActiveHero->field_128.field_10_resource_index, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, id, TRUE, FALSE));
        animFromState = sActiveHero->field_10_resources_array.ItemAt(sActiveHero->field_128.field_10_resource_index);
    }

    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sAbeAnimIdTable[sActiveHero->mCurrentMotion], animFromState);
    //sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data_409C80(sAbeAnimIdTable[sActiveHero->mCurrentMotion], animFromState);

    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame = pSaveState->anim_current_frame;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pSaveState->anim_frame_change_counter;

    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pSaveState->bAnimFlipX & 1);
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pSaveState->bAnimRender & 1);
    sActiveHero->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pSaveState->bDrawable & 1);

    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = static_cast<Layer>(pSaveState->anim_render_layer);

    if (IsLastFrame(&sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim))
    {
        sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    FrameInfoHeader* pFrameInfoHeader = sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1);
    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock, pFrameHeader->field_0_clut_offset);

    sActiveHero->SetTint(sTintTable_Abe_554D20, gMap.mCurrentLevel);
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_0;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
    sActiveHero->mHealth = pSaveState->field_30_health;
    sActiveHero->mCurrentMotion = pSaveState->field_34_animation_num;
    sActiveHero->mNextMotion = pSaveState->next_motion;
    sActiveHero->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->last_line_ypos);
    sActiveHero->BaseAliveGameObject_PlatformId = pSaveState->platform_obj_id;
    sActiveHero->field_120_state.raw = static_cast<u16>(pSaveState->field_50_state);
    sActiveHero->field_124_timer = pSaveState->field_54_timer;
    sActiveHero->field_128.field_0_abe_timer = pSaveState->field_58_abe_timer;
    sActiveHero->field_128.field_4_regen_health_timer = pSaveState->field_5C_regen_health_timer;
    sActiveHero->field_128.field_12_mood = pSaveState->mood;
    sActiveHero->field_128.field_18_say = pSaveState->say;
    sActiveHero->field_144_auto_say_timer = pSaveState->auto_say_timer;
    sActiveHero->field_1A2_throwable_count = pSaveState->field_6c_rock_bone_count;
    sActiveHero->field_168_ring_pulse_timer = pSaveState->ring_pulse_timer;
    sActiveHero->field_16C_bHaveShrykull = pSaveState->bHaveShrykull;

    if (sActiveHero->field_168_ring_pulse_timer && sActiveHero->field_16C_bHaveShrykull)
    {
        if (!sActiveHero->field_10_resources_array.ItemAt(25))
        {
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("SHRYPORT.BND", nullptr);
            }
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("SPLINE.BAN", nullptr);
            }
            sActiveHero->Get_Shrykull_Resources_45AA20();
        }
    }
    else
    {
        if (sActiveHero->field_10_resources_array.ItemAt(25))
        {
            sActiveHero->Free_Shrykull_Resources_45AA90();
        }
    }

    sActiveHero->field_16E_bHaveInvisiblity = pSaveState->bHaveInvisiblity;
    sActiveHero->BaseAliveGameObjectCollisionLineType = pSaveState->field_3a_collision_line_id;

    sActiveHero->field_118_prev_held = InputObject::PsxButtonsToKeyboardInput_45EE40(pSaveState->prev_held);
    sActiveHero->field_11C_released_buttons = InputObject::PsxButtonsToKeyboardInput_45EE40(pSaveState->released_buttons);
    sActiveHero->field_122_knockdown_motion = pSaveState->field_74_knockdown_motion;
    sActiveHero->field_128.field_14_rolling_motion_timer = sGnFrame - pSaveState->field_78_rolling_motion_timer;
    sActiveHero->field_148_fade_obj_id = pSaveState->fade_obj_id;
    sActiveHero->field_14C_circular_fade_id = pSaveState->circular_fade_id;
    sActiveHero->field_150_OrbWhirlWind_id = pSaveState->orb_whirl_wind_id;
    sActiveHero->field_154_possessed_object_id = pSaveState->possesed_object_id;
    sActiveHero->field_158_throwable_id = pSaveState->throwabe_obj_id;
    sActiveHero->field_15C_pull_rope_id = pSaveState->pull_ring_rope_id;
    sActiveHero->field_160_slappable_or_pick_item_id = pSaveState->slappable_or_pickup_id;
    sActiveHero->field_164_wheel_id = pSaveState->wheel_id;
    sActiveHero->field_178_invisible_effect_id = -1;
    sActiveHero->field_170_invisible_timer = pSaveState->invisible_timer;
    sActiveHero->field_174_unused = pSaveState->field_A0_unused;
    sActiveHero->field_176_invisibility_duration = pSaveState->field_A2_invisibility_duration;

    sActiveHero->field_17C_cam_idx = pSaveState->field_A4_cam_idx;
    sActiveHero->field_180_hand_stone_type = pSaveState->hand_stone_type;
    sActiveHero->field_184_fmv_id = pSaveState->fmv_id;
    sActiveHero->field_186_to_camera_id[0] = pSaveState->cam_id_1;
    sActiveHero->field_186_to_camera_id[1] = pSaveState->cam_id_2;
    sActiveHero->field_186_to_camera_id[2] = pSaveState->cam_id_3;
    sActiveHero->field_18C_unused = pSaveState->field_B4_unused;

    sActiveHero->field_18E_unused = pSaveState->field_B6_unused;
    sActiveHero->field_190_unused = pSaveState->field_B8_unused;
    sActiveHero->field_192_unused = pSaveState->field_BA_unused;
    sActiveHero->field_194_unused = pSaveState->field_BC_unused;
    sActiveHero->field_196_unused = pSaveState->field_BE_unused;
    sActiveHero->field_198_has_evil_fart = pSaveState->bHaveEvilFart;
    sActiveHero->field_19A_to_level = MapWrapper::FromAE(pSaveState->to_level);
    sActiveHero->field_19C_to_path = pSaveState->to_path;
    sActiveHero->field_19E_to_camera = pSaveState->to_camera;
    sActiveHero->field_1A0_door_id = pSaveState->door_id;
    sActiveHero->field_1A3_throw_direction = pSaveState->field_ca_throw_direction;
    sActiveHero->field_1A4_portal_sub_state = static_cast<PortalSubStates>(pSaveState->field_CC_portal_sub_state);
    sActiveHero->field_1A8_portal_id = pSaveState->bird_portal_id;

    sActiveHero->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit7_Electrocuted, pSaveState->bElectrocuted & 1);
    sActiveHero->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit8_bInvisible, pSaveState->field_42_bInvisible & 1);
    sActiveHero->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit10_Teleporting, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit13_teleporting));

    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit1_lift_point_dead_while_using_lift));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit2_return_to_previous_motion));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_WalkToRun, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit3_WalkToRun));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit4_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit4_unused));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel, pSaveState->bShrivel);
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit6_prevent_chanting, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit5_prevent_chanting));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit6_land_softly));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit8_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit7_unused));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_laugh_at_chant_end, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit8_laugh_at_chant_end));

    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_Bit12_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit9_unused));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit10_play_ledge_grab_sounds));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_eBit14_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit11_unused));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_eBit15_have_healing, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit12_have_healing));
    sActiveHero->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit10_Teleporting, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit13_teleporting));
    sActiveHero->field_1AC_flags.Set(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit14_is_mudanchee_vault_ender));

    sActiveHero->field_1AE_flags.Set(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit15_is_mudomo_vault_ender));
    sActiveHero->mShadow->field_14_flags.Set(Shadow::Flags::eBit2_Enabled, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit16_shadow_enabled));

    sActiveHero->mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom, pSaveState->field_D6_flags.Get(Abe_SaveState::eD6_Bit1_shadow_at_bottom));

    if (sActiveHero->field_198_has_evil_fart)
    {
        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("EVILFART.BAN", nullptr);
        }

        if (!sActiveHero->field_10_resources_array.ItemAt(22))
        {
            sActiveHero->field_10_resources_array.SetAt(22, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, TRUE, FALSE));
        }

        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
        }

        if (!sActiveHero->field_10_resources_array.ItemAt(24))
        {
            sActiveHero->field_10_resources_array.SetAt(24, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, TRUE, FALSE));
        }

        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
        }

        if (!sActiveHero->field_10_resources_array.ItemAt(23))
        {
            sActiveHero->field_10_resources_array.SetAt(23, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, TRUE, FALSE));
        }
    }

    return sizeof(Abe_SaveState);
}

const FP sAbe_xVel_table_545770[8] = {
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4)};

const FP sAbe_yVel_table_545790[8] = {
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4)};

ALIVE_VAR(1, 0x5c1bda, s16, gAbeBulletProof_5C1BDA, 0);


void Abe::VUpdate()
{
    if (gAbeBulletProof_5C1BDA) // Some flag to reset HP?
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit7_Electrocuted);
        mHealth = FP_FromDouble(1.0);
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));

            BaseAliveGameObjectCollisionLineType = -1;
        }

        BaseAliveGameObject_PlatformId = BaseGameObject::RefreshId(BaseAliveGameObject_PlatformId);
        field_148_fade_obj_id = BaseGameObject::RefreshId(field_148_fade_obj_id);
        field_14C_circular_fade_id = BaseGameObject::RefreshId(field_14C_circular_fade_id);
        field_1A8_portal_id = BaseGameObject::RefreshId(field_1A8_portal_id);
        field_150_OrbWhirlWind_id = BaseGameObject::RefreshId(field_150_OrbWhirlWind_id);
        field_154_possessed_object_id = BaseGameObject::RefreshId(field_154_possessed_object_id);
        field_158_throwable_id = BaseGameObject::RefreshId(field_158_throwable_id);
        field_15C_pull_rope_id = BaseGameObject::RefreshId(field_15C_pull_rope_id);
        field_160_slappable_or_pick_item_id = BaseGameObject::RefreshId(field_160_slappable_or_pick_item_id);
        field_164_wheel_id = BaseGameObject::RefreshId(field_164_wheel_id);

        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            if (!field_170_invisible_timer)
            {
                field_170_invisible_timer = sGnFrame + 2;
            }

            auto pInvisibleEffect = relive_new InvisibleEffect(this);
            field_178_invisible_effect_id = pInvisibleEffect->field_8_object_id;
            pInvisibleEffect->InstantInvisibility();
        }
    }

    if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit12_unused))
    {
        return;
    }

    if (gAbeBulletProof_5C1BDA)
    {
        mHealth = FP_FromDouble(1.0);
    }

    if (!Input_IsChanting_45F260())
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit6_prevent_chanting);
        field_174_unused = 1;
    }

    const s32 totalAliveSavedMuds = sRescuedMudokons_5C1BC2 - sKilledMudokons_5C1BC0;
    if (totalAliveSavedMuds > 0)
    {
        // "Happy" voice
        field_128.field_12_mood = Mud_Emotion::eHappy_5;
    }
    else if (totalAliveSavedMuds < 0)
    {
        // "Sad" voice
        field_128.field_12_mood = Mud_Emotion::eSad_3;
    }
    else
    {
        // "Normal" voice
        field_128.field_12_mood = Mud_Emotion::eNormal_0;
    }

    // Handle DDCheat mode
    if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
    {
        VOnTrapDoorOpen();

        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_shrivel);
        mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
        BaseAliveGameObjectCollisionLine = nullptr;

        if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (InputCommands::Enum::eRight | InputCommands::Enum::eLeft | InputCommands::Enum::eDown | InputCommands::Enum::eUp))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = sAbe_xVel_table_545770[(u8) Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            mBaseAnimatedWithPhysicsGameObject_VelY = sAbe_yVel_table_545790[(u8) Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];

            if (sInputKey_Run_5550E8 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += sAbe_xVel_table_545770[(u8) Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                mBaseAnimatedWithPhysicsGameObject_VelX += sAbe_xVel_table_545770[(u8) Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                mBaseAnimatedWithPhysicsGameObject_VelY += sAbe_yVel_table_545790[(u8) Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            }

            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

            // Keep within map max min bounds
            if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_YPos < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(0);
            }

            // Keep within map max bounds
            PSX_Point mapSize = {};
            gMap.Get_map_size(&mapSize);

            FP mapWidth = FP_FromInteger(mapSize.x);
            if (mBaseAnimatedWithPhysicsGameObject_XPos >= mapWidth)
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = mapWidth - FP_FromDouble(1.0);
            }

            FP mapHeight = FP_FromInteger(mapSize.y);
            if (mBaseAnimatedWithPhysicsGameObject_YPos >= mapHeight)
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = mapHeight - FP_FromDouble(1.0);
                SetActiveCameraDelayedFromDir();
                return;
            }
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        }

        SetActiveCameraDelayedFromDir();
    }
    else
    {
        // Handle none DDCheat mode

        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);

        s16 motion_idx = mCurrentMotion;

        // Execute the current motion
        const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        (this->*(sAbeMotionMachineTable_554910)[motion_idx])();

        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave) || field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel))
        {
            return;
        }

        if (BaseAliveGameObjectCollisionLine)
        {
            // Snap to a whole number so we are "on" the line
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(mBaseAnimatedWithPhysicsGameObject_YPos);
        }

        // Did position change?
        if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos || oldYPos != mBaseAnimatedWithPhysicsGameObject_YPos)
        {
            // Get the TLV we are on
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
        {
            motion_idx = field_122_knockdown_motion;
            ToKnockback_44E700(1, 1);
            if (motion_idx != -1)
            {
                mCurrentMotion = motion_idx;
            }

            mNextMotion = 0;
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
            field_122_knockdown_motion = eAbeMotions::Motion_0_Idle_44EEB0;
            mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        }

        if (EventGet(kEventScreenShake) && mHealth > FP_FromInteger(0))
        {
            if (IsStanding_449D30())
            {
                ToKnockback_44E700(1, 0);
            }
        }

        if (field_128.field_18_say != MudSounds::eNone && static_cast<s32>(sGnFrame) >= field_144_auto_say_timer)
        {
            if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0)
                || (mCurrentMotion == eAbeMotions::Motion_112_Chant_45B1C0)
                || mCurrentMotion == eAbeMotions::Motion_7_Speak_45B140
                || mCurrentMotion == eAbeMotions::Motion_8_Speak_45B160
                || mCurrentMotion == eAbeMotions::Motion_9_Speak_45B180
                || mCurrentMotion == eAbeMotions::Motion_10_Fart_45B1A0)
            {
                // Prevents f64 laugh when blowing up a slig as we prevent saying anything while chanting or various other states
                // or when abe isn't in the active screen
                field_128.field_18_say = MudSounds::eNone;
            }
            else
            {
                if (mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || mCurrentMotion == eAbeMotions::Motion_12_Null_4569C0)
                {
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    switch (field_128.field_18_say)
                    {
                        case MudSounds::eOops_14:
                            mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                            break;
                        case MudSounds::eAnger_5:
                            mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
                            break;
                        case MudSounds::eSadUgh_28:
                            mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
                            break;
                        default:
                            mCurrentMotion = eAbeMotions::Motion_9_Speak_45B180;
                            break;
                    }
                }

                if (field_128.field_18_say == MudSounds::eAnger_5)
                {
                    // Other evil muds laugh at the abe grr
                    EventBroadcast(kEventMudokonLaugh, sActiveHero);
                }

                if (field_128.field_18_say == MudSounds::eSadUgh_28)
                {
                    // This one has another volume for whatever reason
                    Mudokon_SFX(field_128.field_18_say, 80, 0, this);
                }
                else
                {
                    Mudokon_SFX(field_128.field_18_say, 0, 0, this);
                }

                field_128.field_18_say = MudSounds::eNone;
            }
        }

        if (motion_idx != mCurrentMotion || mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2))
        {
            mBaseAliveGameObjectFlags.Clear(Flags_114::e114_MotionChanged_Bit2);
            if (mCurrentMotion != eAbeMotions::Motion_12_Null_4569C0 && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sAbeAnimIdTable[mCurrentMotion], MotionToAnimResource_44AAB0(mCurrentMotion));

                field_128.field_14_rolling_motion_timer = sGnFrame;

                if (motion_idx == eAbeMotions::Motion_12_Null_4569C0 || motion_idx == eAbeMotions::Motion_60_Unused_4A3200)
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
                }
            }
        }

        if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit2_return_to_previous_motion))
        {
            mCurrentMotion = mPreviousMotion;

            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sAbeAnimIdTable[mCurrentMotion], MotionToAnimResource_44AAB0(mCurrentMotion));

            field_128.field_14_rolling_motion_timer = sGnFrame;
            mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        }

        if (field_128.field_4_regen_health_timer <= static_cast<s32>(sGnFrame) && mHealth > FP_FromInteger(0))
        {
            mHealth = FP_FromDouble(1.0);
        }

        // Draw power up ring "pulse"
        if (field_168_ring_pulse_timer > 0)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    if (field_168_ring_pulse_timer > static_cast<s32>(sGnFrame))
                    {
                        if (!(sGnFrame % 32))
                        {
                            RingTypes ringType = RingTypes::eExplosive_Pulse_0;
                            if (field_16C_bHaveShrykull)
                            {
                                ringType = RingTypes::eShrykull_Pulse_Small_4;
                            }
                            else if (field_16E_bHaveInvisiblity)
                            {
                                ringType = RingTypes::eInvisible_Pulse_Small_7;
                            }
                            else if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing))
                            {
                                ringType = RingTypes::eHealing_Pulse_14;
                            }

                            const PSX_RECT rect = VGetBoundingRect();
                            AbilityRing::Factory(
                                FP_FromInteger((rect.x + rect.w) / 2),
                                FP_FromInteger((rect.y + rect.h) / 2),
                                ringType, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                            SFX_Play_Pitch(SoundEffect::PossessEffect_17, 25, 2650);
                        }
                    }
                    else
                    {
                        if (field_168_ring_pulse_timer > 0 && field_16C_bHaveShrykull > 0)
                        {
                            Free_Shrykull_Resources_45AA90();
                        }
                        field_168_ring_pulse_timer = 0;
                    }
                }
            }
        }


        InvisibleEffect* pObj_field_178 = static_cast<InvisibleEffect*>(sObjectIds.Find(field_178_invisible_effect_id, ReliveTypes::eInvisibleEffect));
        if (pObj_field_178 && field_170_invisible_timer > 0)
        {
            if (static_cast<s32>(sGnFrame) > field_170_invisible_timer)
            {
                field_170_invisible_timer = 0;
                pObj_field_178->BecomeVisible();
            }
        }

        // After the trials give Abe the healing power for the Necrum muds.
        if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender))
        {
            if (field_1AE_flags.Get(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender))
            {
                if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum)
                {
                    field_168_ring_pulse_timer = sGnFrame + 200000;
                    field_16C_bHaveShrykull = 0;
                    field_16E_bHaveInvisiblity = 0;
                    field_1AE_flags.Clear(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender);
                    field_1AC_flags.Clear(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender);
                    field_1AC_flags.Set(Flags_1AC::e1AC_eBit15_have_healing);
                }
            }
        }

        if (EventGet(kEventMudokonDied))
        {
            field_128.field_18_say = MudSounds::eOops_14;
            field_144_auto_say_timer = sGnFrame + Math_RandomRange(22, 30);

            relive_new MusicTrigger(MusicTriggerMusicType::eDeathDrumShort_1, TriggeredBy::eTimer_0, 90, 0);
        }

        if (EventGet(kEventMudokonComfort))
        {
            field_128.field_18_say = MudSounds::eGiggle_8;
            field_144_auto_say_timer = sGnFrame + Math_RandomRange(22, 30);
        }

        if (EventGet(kScrabOrParamiteDied))
        {
            field_128.field_18_say = MudSounds::eOops_14;
            field_144_auto_say_timer = sGnFrame + Math_RandomRange(22, 30);
        }

        if (field_1AE_flags.Get(Flags_1AE::e1AE_Bit2_do_quicksave))
        {
            field_1AE_flags.Clear(Flags_1AE::e1AE_Bit2_do_quicksave);
            sActiveQuicksaveData_BAF7F8.field_204_world_info.field_A_save_num = field_1B0_save_num;
            Quicksave_SaveWorldInfo(&sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info);
            VGetSaveState(reinterpret_cast<u8*>(&sActiveQuicksaveData_BAF7F8.field_284_restart_path_abe_state));
            sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states = sSwitchStates_5C1A28;
            DoQuicksave();
        }
    }
}

BirdPortal* Abe::VIntoBirdPortal(s16 gridBlocks)
{
    auto pPortal = BaseAliveGameObject::VIntoBirdPortal(gridBlocks);
    if (pPortal && pPortal->field_24_portal_type == PortalType::eAbe_0)
    {
        return pPortal;
    }
    return nullptr;
}

void Abe::VOnTrapDoorOpen()
{
    // Handles falling when previously was on a platform, stop turning a wheel if we where turning one etc.
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds.Find(field_164_wheel_id, ReliveTypes::eWheel));
    if (pPlatform)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)))
        {
            VSetMotion(eAbeMotions::Motion_93_WalkOffEdge_455970);
        }

        pPlatform->VRemove(this);

        BaseAliveGameObject_PlatformId = -1;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        if (pWheel)
        {
            pWheel->VStopTurning(FALSE);
        }
    }
}

void Abe::ToKnockback_44E700(s16 bKnockbackSound, s16 bDelayedAnger)
{
    OrbWhirlWind* pfield_150 = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(field_150_OrbWhirlWind_id));
    BaseThrowable* pfield_158 = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(field_158_throwable_id));
    WorkWheel* pfield_164 = static_cast<WorkWheel*>(sObjectIds.Find(field_164_wheel_id, ReliveTypes::eWheel));
    if (sControlledCharacter_5C1B8C == this || mHealth <= FP_FromInteger(0))
    {
        // Chant music/orb kill ?
        SND_SEQ_Stop(SeqId::MudokonChant1_10);
        if (pfield_150)
        {
            pfield_150->ToStop();
            field_150_OrbWhirlWind_id = -1;
        }

        if (pfield_164)
        {
            pfield_164->VStopTurning(1);
            field_164_wheel_id = -1;
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos -= mBaseAnimatedWithPhysicsGameObject_VelX;
        }

        MapFollowMe(TRUE);

        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

        if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        }

        // NOTE: This always seems to be set to true. This parameter might not be needed.
        if (bKnockbackSound)
        {
            Mudokon_SFX(MudSounds::eHurt2_9, 0, Math_RandomRange(-127, 127), this);
            Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
        }

        mCurrentMotion = eAbeMotions::Motion_71_Knockback_455090;

        if (bDelayedAnger)
        {
            field_128.field_18_say = MudSounds::eAnger_5;    // anger in..
            field_144_auto_say_timer = sGnFrame + 27; // 27 ticks
        }

        if (pfield_158)
        {
            pfield_158->VToDead();
            field_158_throwable_id = -1;
            if (!gInfiniteGrenades_5C1BDE)
            {
                field_1A2_throwable_count++;
            }
        }
    }
}

void Abe::VRender(PrimHeader** ppOt)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    }

    if (mCurrentMotion != eAbeMotions::Motion_79_InsideWellLocal_45CA60 && mCurrentMotion != eAbeMotions::Motion_82_InsideWellExpress_45CC80 && mCurrentMotion != eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Abe::VScreenChanged()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mLevel;
        mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mPath;
    }

    // Level has changed?
    if (gMap.mCurrentLevel != gMap.mLevel)
    {
        // Hack to make Abe say hello in the first screen of the mines
        if (gMap.mLevel == EReliveLevelIds::eMines && !gAttract_5C1BA0)
        {
            field_128.field_18_say = MudSounds::eHelloNeutral_3;
            field_144_auto_say_timer = sGnFrame + 35;
        }

        // Set the correct tint for this map
        SetTint(sTintTable_Abe_554D20, gMap.mLevel);

        if (gMap.mCurrentLevel != EReliveLevelIds::eNone)
        {
            if (field_1A2_throwable_count > 0)
            {
                if (gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C->Remove(field_1A2_throwable_count);
                }
            }

            field_1A2_throwable_count = 0;

            if (field_168_ring_pulse_timer > 0 && field_16C_bHaveShrykull)
            {
                Free_Shrykull_Resources_45AA90();
            }

            field_168_ring_pulse_timer = 0;
        }

        if (gMap.mLevel == EReliveLevelIds::eNecrum)
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender);
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender)
            {
                field_1AE_flags.Set(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender);
            }
        }

        if (gMap.mLevel == EReliveLevelIds::eCredits || gMap.mLevel == EReliveLevelIds::eMenu)
        {
            // Remove Abe for menu/credits levels?
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    // If level or path changed then kill rings and farts
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        field_168_ring_pulse_timer = 0;
        if (gMap.mCurrentLevel != EReliveLevelIds::eNone)
        {
            field_198_has_evil_fart = 0;
        }
    }

    if (gMap.mCurrentLevel != gMap.mLevel && !(mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave)))
    {
        for (s8& val : sSavedKilledMudsPerZulag_5C1B50.mData)
        {
            val = 0;
        }
    }
}

s32 Abe::VGetSaveState(u8* pSaveBuffer)
{
    Abe_SaveState* pSaveState = reinterpret_cast<Abe_SaveState*>(pSaveBuffer);

    pSaveState->field_0_id = AETypes::eAbe_69;
    pSaveState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pSaveState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pSaveState->field_c_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pSaveState->field_10_vely = mBaseAnimatedWithPhysicsGameObject_VelY;
    pSaveState->field_48_x_vel_slow_by = field_128.field_8_x_vel_slow_by;
    pSaveState->field_4C_unused = field_128.field_C_unused;
    pSaveState->field_14_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pSaveState->field_16_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pSaveState->field_18_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    pSaveState->field_1C_scale = mBaseAnimatedWithPhysicsGameObject_Scale;
    pSaveState->field_1e_r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pSaveState->field_20_g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pSaveState->field_22_b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit11_Electrocuting))
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            auto pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() == ReliveTypes::eElectrocute)
            {
                auto pElectrocute = static_cast<const Electrocute*>(pObj);
                if (pElectrocute->field_20_target_obj_id == field_8_object_id)
                {
                    pSaveState->field_1e_r = pElectrocute->field_24_r;
                    pSaveState->field_20_g = pElectrocute->field_26_g;
                    pSaveState->field_22_b = pElectrocute->field_28_b;
                    break;
                }
            }
        }
    }

    pSaveState->bAnimFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pSaveState->current_motion = mCurrentMotion;
    pSaveState->anim_current_frame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    pSaveState->anim_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter == 0)
    {
        pSaveState->anim_frame_change_counter = 1;
    }

    pSaveState->bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pSaveState->bAnimRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pSaveState->anim_render_layer = static_cast<s8>(mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer);
    pSaveState->field_30_health = mHealth;
    pSaveState->field_34_animation_num = mCurrentMotion;
    pSaveState->next_motion = mNextMotion;
    pSaveState->last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    pSaveState->bElectrocuted = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted);

    pSaveState->field_42_bInvisible = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible);

    if (BaseAliveGameObjectCollisionLine)
    {
        pSaveState->field_3a_collision_line_id = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveState->field_3a_collision_line_id = -1;
    }

    pSaveState->platform_obj_id = BaseAliveGameObject_PlatformId;

    if (BaseAliveGameObject_PlatformId != -1)
    {
        auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pObj)
        {
            pSaveState->platform_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->field_44_is_abe_controlled = (this == sControlledCharacter_5C1B8C);
    pSaveState->field_50_state = field_120_state.raw;
    pSaveState->field_54_timer = field_124_timer;
    pSaveState->field_58_abe_timer = field_128.field_0_abe_timer;
    pSaveState->field_5C_regen_health_timer = field_128.field_4_regen_health_timer;
    pSaveState->mood = field_128.field_12_mood;
    pSaveState->say = field_128.field_18_say;
    pSaveState->auto_say_timer = field_144_auto_say_timer;
    pSaveState->ring_pulse_timer = field_168_ring_pulse_timer;
    pSaveState->field_6c_rock_bone_count = field_1A2_throwable_count;
    pSaveState->bHaveShrykull = static_cast<s8>(field_16C_bHaveShrykull);
    pSaveState->bHaveInvisiblity = static_cast<s8>(field_16E_bHaveInvisiblity);

    pSaveState->prev_held = InputObject::KeyboardInputToPsxButtons_45EF70(field_118_prev_held);
    pSaveState->released_buttons = InputObject::KeyboardInputToPsxButtons_45EF70(field_11C_released_buttons);

    pSaveState->field_74_knockdown_motion = field_122_knockdown_motion;
    pSaveState->field_78_rolling_motion_timer = sGnFrame - field_128.field_14_rolling_motion_timer;
    pSaveState->fade_obj_id = field_148_fade_obj_id;

    if (field_148_fade_obj_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_148_fade_obj_id);
        if (pObj)
        {
            pSaveState->fade_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->circular_fade_id = field_14C_circular_fade_id;

    if (field_14C_circular_fade_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_14C_circular_fade_id);
        if (pObj)
        {
            pSaveState->circular_fade_id = pObj->mBaseGameObjectTlvInfo;
        }
    }
    pSaveState->orb_whirl_wind_id = field_150_OrbWhirlWind_id;

    if (field_150_OrbWhirlWind_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_150_OrbWhirlWind_id);
        if (pObj)
        {
            pSaveState->orb_whirl_wind_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->possesed_object_id = field_154_possessed_object_id;

    if (field_154_possessed_object_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_154_possessed_object_id);
        if (pObj)
        {
            pSaveState->possesed_object_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->throwabe_obj_id = field_158_throwable_id;

    if (field_158_throwable_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_158_throwable_id);
        if (pObj)
        {
            pSaveState->throwabe_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->pull_ring_rope_id = field_15C_pull_rope_id;

    if (field_15C_pull_rope_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_15C_pull_rope_id);
        if (pObj)
        {
            pSaveState->pull_ring_rope_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->slappable_or_pickup_id = field_160_slappable_or_pick_item_id;

    if (field_160_slappable_or_pick_item_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_160_slappable_or_pick_item_id);
        if (pObj)
        {
            pSaveState->slappable_or_pickup_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->wheel_id = field_164_wheel_id;

    if (field_164_wheel_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_164_wheel_id);
        if (pObj)
        {
            pSaveState->wheel_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->bird_portal_id = field_1A8_portal_id;

    if (field_1A8_portal_id != -1)
    {
        auto pObj = sObjectIds.Find_Impl(field_1A8_portal_id);
        if (pObj)
        {
            pSaveState->bird_portal_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState->invisible_timer = field_170_invisible_timer;
    pSaveState->field_A0_unused = field_174_unused;
    pSaveState->field_A2_invisibility_duration = field_176_invisibility_duration;
    pSaveState->field_A4_cam_idx = field_17C_cam_idx;
    pSaveState->hand_stone_type = field_180_hand_stone_type;
    pSaveState->fmv_id = field_184_fmv_id;
    pSaveState->cam_id_1 = field_186_to_camera_id[0];
    pSaveState->cam_id_2 = field_186_to_camera_id[1];
    pSaveState->cam_id_3 = field_186_to_camera_id[2];
    pSaveState->field_B4_unused = field_18C_unused;
    pSaveState->field_B6_unused = field_18E_unused;
    pSaveState->field_B8_unused = field_190_unused;
    pSaveState->field_BA_unused = field_192_unused;
    pSaveState->field_BC_unused = field_194_unused;
    pSaveState->field_BE_unused = field_196_unused;
    pSaveState->bHaveEvilFart = field_198_has_evil_fart;
    pSaveState->to_level = MapWrapper::ToAE(field_19A_to_level);
    pSaveState->to_path = field_19C_to_path;
    pSaveState->to_camera = field_19E_to_camera;
    pSaveState->door_id = field_1A0_door_id;
    pSaveState->field_ca_throw_direction = field_1A3_throw_direction;
    pSaveState->field_CC_portal_sub_state = static_cast<u16>(field_1A4_portal_sub_state);

    pSaveState->bElectrocuted = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted);
    pSaveState->field_42_bInvisible = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible);
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit13_teleporting, sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting));

    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit1_lift_point_dead_while_using_lift, field_1AC_flags.Get(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit2_return_to_previous_motion, field_1AC_flags.Get(Flags_1AC::e1AC_Bit2_return_to_previous_motion));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit3_WalkToRun, field_1AC_flags.Get(Flags_1AC::e1AC_Bit3_WalkToRun));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit4_unused, field_1AC_flags.Get(Flags_1AC::e1AC_Bit4_unused));
    pSaveState->bShrivel = field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel);
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit5_prevent_chanting, field_1AC_flags.Get(Flags_1AC::e1AC_Bit6_prevent_chanting));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit6_land_softly, field_1AC_flags.Get(Flags_1AC::e1AC_Bit7_land_softly));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit7_unused, field_1AC_flags.Get(Flags_1AC::e1AC_Bit8_unused));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit8_laugh_at_chant_end, field_1AC_flags.Get(Flags_1AC::e1AC_Bit9_laugh_at_chant_end));

    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit9_unused, field_1AC_flags.Get(Flags_1AC::e1AC_Bit12_unused));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit10_play_ledge_grab_sounds, field_1AC_flags.Get(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit11_unused, field_1AC_flags.Get(Flags_1AC::e1AC_eBit14_unused));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit12_have_healing, field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit13_teleporting, mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit14_is_mudanchee_vault_ender, field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender));

    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit15_is_mudomo_vault_ender, field_1AE_flags.Get(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit16_shadow_enabled, mShadow->field_14_flags.Get(Shadow::Flags::eBit2_Enabled));

    pSaveState->field_D6_flags.Set(Abe_SaveState::eD6_Bit1_shadow_at_bottom, mShadow->field_14_flags.Get(Shadow::Flags::eBit1_ShadowAtBottom));

    return sizeof(Abe_SaveState);
}

s16 Abe::VTakeDamage(BaseGameObject* pFrom)
{
    // Stop chant sound music.
    SND_SEQ_Stop(SeqId::MudokonChant1_10);

    const MudSounds oldSay = field_128.field_18_say;
    field_128.field_18_say = MudSounds::eNone;

    OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(field_150_OrbWhirlWind_id));
    if (pWhirlWind)
    {
        pWhirlWind->ToStop();
        field_150_OrbWhirlWind_id = -1;
    }

    if (CantBeDamaged_44BAB0())
    {
        return 0;
    }

    if (gAbeBulletProof_5C1BDA)
    {
        return 0;
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        return 0;
    }

    field_128.field_4_regen_health_timer = sGnFrame + 180;
    s16 ret = mHealth > FP_FromInteger(0);

    switch (pFrom->Type())
    {
        case ReliveTypes::eGasClock:
            if (mHealth > FP_FromInteger(0))
            {
                if (ForceDownIfHoisting_44BA30())
                {
                    return 1;
                }

                if (IsStanding_449D30())
                {
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_129_PoisonGasDeath_4565C0;
                    field_124_timer = 1;
                }
                else
                {
                    if (mCurrentMotion != eAbeMotions::Motion_71_Knockback_455090 && mCurrentMotion != eAbeMotions::Motion_72_KnockbackGetUp_455340)
                    {
                        ToKnockback_44E700(1, 1);
                        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    }
                }
            }
            break;

        case ReliveTypes::eDrill:
        {
            if (mHealth <= FP_FromInteger(0))
            {
                return 0;
            }

            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            mHealth = FP_FromInteger(0);
            ToKnockback_44E700(1, 1);

            mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(30, 30, 30);

            relive_new Gibs(GibType::Abe_0,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                false);

            // Note Check on word_5CC88C <= 3846 appeared always true, removed.
            relive_new Gibs(GibType::Abe_0,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                false);

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        }
        break;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eOops_14, 0, 0, this);
            break;

        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eExplosion:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                if (mHealth > FP_FromInteger(0))
                {
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
                    field_122_knockdown_motion = eAbeMotions::Motion_71_Knockback_455090;
                    mNextMotion = eAbeMotions::Motion_71_Knockback_455090;
                }

                mHealth = FP_FromInteger(0);

                mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(30, 30, 30);

                relive_new Gibs(GibType::Abe_0,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    0);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                mShadow->field_14_flags.Clear(Shadow::Flags::eBit2_Enabled);
            }
            break;

        case ReliveTypes::eSecurityClaw:
        case ReliveTypes::eSecurityOrb:
            field_128.field_18_say = MudSounds::eAnger_5;
            field_144_auto_say_timer = sGnFrame + 27;
            if (mCurrentMotion != eAbeMotions::Motion_123_LiftGrabIdle_45A6A0 && mCurrentMotion != eAbeMotions::Motion_124_LiftUseUp_45A780 && mCurrentMotion != eAbeMotions::Motion_125_LiftUseDown_45A7B0)
            {
                ToKnockback_44E700(1, 1);
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            }

            // The zap makes Abe drop his stuff everywhere
            for (s32 i = 0; i < field_1A2_throwable_count; i++)
            {
                BaseThrowable* pThrowable = Make_Throwable_49AF30(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30),
                    0);

                // Random explode time ?
                const FP rand1 = FP_FromRaw((Math_NextRandom() - 127) << 11); // TODO: Wat?
                const FP rand2 = (FP_FromDouble(0.03125) * FP_FromRaw(Math_NextRandom())) - FP_FromInteger(2);
                pThrowable->VThrow(rand1, rand2);

                pThrowable->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                pThrowable->mBaseAnimatedWithPhysicsGameObject_Scale = mBaseAnimatedWithPhysicsGameObject_Scale;
                pThrowable->VTimeToExplodeRandom(); // Start count down ?
            }
            field_1A2_throwable_count = 0;
            break;

        case ReliveTypes::eRockSpawner:
            if (mHealth > FP_FromInteger(0))
            {
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                mHealth = FP_FromInteger(0);

                if (ForceDownIfHoisting_44BA30())
                {
                    return 1;
                }
                ToKnockback_44E700(1, 1);
                SfxPlayMono(SoundEffect::KillEffect_64, 127);
            }
            break;

        case ReliveTypes::eFleech:
            if (mHealth > FP_FromInteger(0))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

                mHealth -= FP_FromDouble(0.15075); // Yes it has to be this accurate to match

                if (mHealth < FP_FromInteger(0))
                {
                    mHealth = FP_FromInteger(0);
                    Mudokon_SFX(MudSounds::eLaugh_10, 0, 1000, this);
                }

                if (mHealth > FP_FromInteger(0))
                {
                    // The damage sound from a Fleech keeps getting high and higher pitch till death
                    const FP hpRandSoundRange = (FP_FromInteger(1) - sActiveHero->mHealth) / FP_FromDouble(0.15);
                    const s16 pitchRand = Math_RandomRange(
                        200 * (FP_GetExponent(hpRandSoundRange)),
                        40 * (5 * (FP_GetExponent(hpRandSoundRange)) + 5));

                    Mudokon_SFX(MudSounds::eHurt2_9, 0, pitchRand, this);
                    return 1;
                }

                const PSX_RECT bRect = VGetBoundingRect();

                relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos,
                    // Put YPos in the middle of who is getting damaged
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_XPos - pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(0)) ? -24 : 24),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    50);

                if (ForceDownIfHoisting_44BA30())
                {
                    return 1;
                }

                ToKnockback_44E700(1, 1);
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mCurrentMotion = eAbeMotions::Motion_101_KnockForward_455420;
                }

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos && !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                {
                    mCurrentMotion = eAbeMotions::Motion_101_KnockForward_455420;
                }

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-7.8);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(7.8);
                }

                SfxPlayMono(SoundEffect::KillEffect_64, 127);
                SfxPlayMono(SoundEffect::FallingItemHit_47, 90);
            }
            break;

        case ReliveTypes::eGreeter:
            if (mHealth <= FP_FromInteger(0))
            {
                break;
            }

            Mudokon_SFX(MudSounds::eHurt2_9, 0, 0, this);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
            field_122_knockdown_motion = eAbeMotions::Motion_101_KnockForward_455420;
            mNextMotion = eAbeMotions::Motion_101_KnockForward_455420;
            mHealth = FP_FromInteger(0);

            if (!ForceDownIfHoisting_44BA30())
            {
                break;
            }
            return 1;

        case ReliveTypes::eParamite:
        case ReliveTypes::eScrab:
        case ReliveTypes::eSlog:
            if (mHealth > FP_FromInteger(0))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

                mHealth = FP_FromInteger(0);

                const PSX_RECT bRect = VGetBoundingRect();

                relive_new Blood(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    (pAliveObj->mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0)) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    50);

                if (ForceDownIfHoisting_44BA30())
                {
                    return 1;
                }

                ToKnockback_44E700(1, 1);
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_101_KnockForward_455420;
                    }
                }

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        mCurrentMotion = eAbeMotions::Motion_101_KnockForward_455420;
                    }
                }

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-7.8);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(7.8);
                }

                SfxPlayMono(SoundEffect::KillEffect_64, 127);

                if (pFrom->Type() != ReliveTypes::eParamite)
                {
                    SfxPlayMono(SoundEffect::FallingItemHit_47, 90);
                }
            }
            break;

        case ReliveTypes::eAbilityRing:
            return 0;

        case ReliveTypes::eMudokon:
            if (mHealth > FP_FromInteger(0) && mCurrentMotion != eAbeMotions::Motion_71_Knockback_455090)
            {
                mHealth -= FP_FromDouble(0.07);
                if (mHealth <= FP_FromInteger(0) || (mCurrentMotion != eAbeMotions::Motion_123_LiftGrabIdle_45A6A0 && mCurrentMotion != eAbeMotions::Motion_124_LiftUseUp_45A780 && mCurrentMotion != eAbeMotions::Motion_125_LiftUseDown_45A7B0))
                {
                    ToKnockback_44E700(1, 1);
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    if (mHealth <= FP_FromInteger(0))
                    {
                        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
                        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
                        mHealth = FP_FromInteger(0);
                    }
                }
            }
            break;

        case ReliveTypes::eSlamDoor:
            if (mHealth > FP_FromInteger(0))
            {
                ToKnockback_44E700(1, 1);
                VCheckCollisionLineStillValid(5);
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
            }
            break;

        case ReliveTypes::eElectrocute:
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            ToDieFinal_458910();
            break;

        default:
            if (pFrom->Type() != ReliveTypes::eBullet)
            {
                LOG_ERROR("Expected default case to be bullets only but got: " << static_cast<s32>(pFrom->Type()));
            }
            BulletDamage_44C980(static_cast<Bullet*>(pFrom));
            if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
            {
                ret = 0;
                field_128.field_18_say = oldSay;
            }
            break;
    }

    if (sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit4_bPossesed))
    {
        if (mHealth == FP_FromInteger(0))
        {
            sControlledCharacter_5C1B8C->VUnPosses();
            // We are the "active" player again
            GiveControlBackToMe_44BA10();
        }
    }

    return ret;
}

void Abe::VOnTlvCollision(Path_TLV* pTlv)
{
    for (; pTlv; pTlv = sPath_dword_BB47C0->TlvGetAt(
                     pTlv,
                     mBaseAnimatedWithPhysicsGameObject_XPos,
                     mBaseAnimatedWithPhysicsGameObject_YPos,
                     mBaseAnimatedWithPhysicsGameObject_XPos,
                     mBaseAnimatedWithPhysicsGameObject_YPos))
    {
        if (pTlv->mTlvType32 == TlvTypes::ContinuePoint_0)
        {
            auto pContinuePoint = static_cast<Path_ContinuePoint*>(pTlv);
            if (pContinuePoint->mTlvState == 0)
            {
                if ((pContinuePoint->field_10_scale != Path_ContinuePoint::Scale::eHalf_1 || mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1)) && (pContinuePoint->field_10_scale != Path_ContinuePoint::Scale::eFull_2 || mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
                    && mHealth > FP_FromInteger(0) && !(mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted)))
                {
                    pContinuePoint->mTlvState = 1;
                    field_1AE_flags.Set(Flags_1AE::e1AE_Bit2_do_quicksave);
                    field_1B0_save_num = pContinuePoint->field_12_save_file_id;
                }
            }
        }
        else if (pTlv->mTlvType32 == TlvTypes::DeathDrop_4)
        {
            if (sControlledCharacter_5C1B8C->Type() != ReliveTypes::eMineCar || gMap.mCurrentLevel != EReliveLevelIds::eMines)
            {
                Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                EventBroadcast(kEventLoudNoise, this);
                EventBroadcast(kEventSpeaking, this);
                ToDie_4588D0();
            }
        }
        else if (pTlv->mTlvType32 == TlvTypes::ResetSwitchRange_76)
        {
            auto pResetSwitchRange = static_cast<Path_ResetSwitchRange*>(pTlv);
            if (pResetSwitchRange->mTlvState == 0 || pResetSwitchRange->field_1C_bEnabled == Choice_short::eYes_1)
            {
                pResetSwitchRange->mTlvState = 1;
                if (pResetSwitchRange->field_10_reset_switch_ids == Choice_short::eYes_1)
                {
                    for (s16 i = pResetSwitchRange->field_12_start_switch_id; i <= pResetSwitchRange->field_14_end_switch_id; i++)
                    {
                        if (i != pResetSwitchRange->field_16_skip_switch_id && i > 1)
                        {
                            SwitchStates_Set(i, 0);
                        }
                    }
                }
                if (pResetSwitchRange->field_18_free_path_res == Choice_short::eYes_1)
                {
                    Path::Reset_TLVs(pResetSwitchRange->field_1A_path_to_free_id);
                }
            }
        }
    }
}

BaseAliveGameObject* Abe::FindObjectToPossess_44B7B0()
{
    BaseAliveGameObject* pTargetObj = nullptr;
    BaseAliveGameObject* pInRangeFart = nullptr;
    BaseAliveGameObject* pInRangeGlukkon = nullptr;

    s16 maxDistance = 32767;
    FP lastScale = {};
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit3_Can_Be_Possessed))
        {
            switch (pObj->Type())
            {
                    // Third priority
                case ReliveTypes::eCrawlingSlig:
                case ReliveTypes::eFlyingSlig:
                case ReliveTypes::eParamite:
                case ReliveTypes::eScrab:
                case ReliveTypes::eSlig:
                    if (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObj->mHealth > FP_FromInteger(0))
                    {
                        const s16 distance = static_cast<s16>(Math_Distance(
                            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                            FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos),
                            FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos)));

                        if (lastScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                        {
                            if (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && distance < maxDistance)
                            {
                                pTargetObj = pObj;
                                maxDistance = distance;
                            }
                        }
                        else if (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                        {
                            pTargetObj = pObj;
                            maxDistance = distance;
                            lastScale = pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                        }
                        else if (distance < maxDistance)
                        {
                            pTargetObj = pObj;
                            maxDistance = distance;
                            lastScale = pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                        }
                    }
                    break;

                    // First priority
                case ReliveTypes::eEvilFart:
                    pInRangeFart = pObj;
                    break;

                    // Second priority
                case ReliveTypes::eGlukkon:
                    if (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                    {
                        pInRangeGlukkon = pObj;
                    }
                    break;

                default:
                    continue;
            }
        }
    }

    if (pInRangeFart)
    {
        return pInRangeFart;
    }

    if (pInRangeGlukkon)
    {
        return pInRangeGlukkon;
    }

    return pTargetObj;
}

void Abe::Load_Basic_Resources_44D460()
{
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, FALSE, FALSE))
    {
        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("ABEBSIC.BAN", nullptr);
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE);
        }
        field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, FALSE, FALSE));
    }
}

void Abe::Free_Resources_44D420()
{
    if (field_10_resources_array.field_4_used_size)
    {
        if (field_10_resources_array.ItemAt(0))
        {
            ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(0));
            field_10_resources_array.SetAt(0, nullptr);
        }
    }
}

bool Abe::IsStanding_449D30()
{
    return mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0
        || mCurrentMotion == eAbeMotions::Motion_2_StandingTurn_451830
        || mCurrentMotion == eAbeMotions::Motion_34_DunnoBegin_44ECF0
        || mCurrentMotion == eAbeMotions::Motion_35_DunnoEnd_44ED10
        || mCurrentMotion == eAbeMotions::Motion_11_Speak_45B0A0
        || mCurrentMotion == eAbeMotions::Motion_7_Speak_45B140
        || mCurrentMotion == eAbeMotions::Motion_8_Speak_45B160
        || mCurrentMotion == eAbeMotions::Motion_9_Speak_45B180
        || mCurrentMotion == eAbeMotions::Motion_10_Fart_45B1A0
        || mCurrentMotion == eAbeMotions::Motion_99_LeverUse_455AC0
        || mCurrentMotion == eAbeMotions::Motion_105_RockThrowStandingThrow_456460
        || mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0
        || mCurrentMotion == eAbeMotions::Motion_106_RockThrowStandingEnd_455F20
        || mCurrentMotion == eAbeMotions::Motion_112_Chant_45B1C0
        || mCurrentMotion == eAbeMotions::Motion_113_ChantEnd_45BBE0;
}

void Abe::Free_Shrykull_Resources_45AA90()
{
    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(25));
    field_10_resources_array.SetAt(25, nullptr);

    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(26));
    field_10_resources_array.SetAt(26, nullptr);

    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(27));
    field_10_resources_array.SetAt(27, nullptr);
}

u8** Abe::MotionToAnimResource_44AAB0(s16 motion)
{
    s16 mapped = ResourceIndices::eBasic_0;
    if (motion < eAbeMotions::Motion_12_Null_4569C0)
    {
        mapped = ResourceIndices::eBasic1_1;
    }
    else if (motion < eAbeMotions::Motion_65_LedgeAscend_4548E0)
    {
        mapped = ResourceIndices::eBasic_0;
    }
    else if (motion < eAbeMotions::Motion_71_Knockback_455090)
    {
        mapped = ResourceIndices::eHosit_12;
    }
    else if (motion < eAbeMotions::Motion_75_JumpIntoWell_45C7B0)
    {
        mapped = ResourceIndices::eKnockBack_9;
    }
    else if (motion < eAbeMotions::Motion_84_FallLandDie_45A420)
    {
        mapped = ResourceIndices::eWell_15;
    }
    else if (motion < eAbeMotions::jMotion_85_Fall_455070)
    {
        mapped = ResourceIndices::eSmash_6;
    }
    else if (motion < eAbeMotions::Motion_86_HandstoneBegin_45BD00)
    {
        mapped = ResourceIndices::eFall_7;
    }
    else if (motion < eAbeMotions::Motion_91_FallingFromGrab_4557B0)
    {
        mapped = ResourceIndices::eStone_8;
    }
    else if (motion < eAbeMotions::Motion_99_LeverUse_455AC0)
    {
        mapped = ResourceIndices::eEdge_13;
    }
    else if (motion < eAbeMotions::Motion_100_SlapBomb_455B60)
    {
        mapped = ResourceIndices::ePull_2;
    }
    else if (motion < eAbeMotions::Motion_101_KnockForward_455420)
    {
        mapped = ResourceIndices::eBomb_4;
    }
    else if (motion < eAbeMotions::Motion_104_RockThrowStandingHold_455DF0)
    {
        mapped = ResourceIndices::eKnockFd_10;
    }
    else if (motion < eAbeMotions::Motion_109_ZShotRolling_455550)
    {
        mapped = ResourceIndices::eThrow_5;
    }
    else if (motion < eAbeMotions::Motion_111_PickupItem_4564A0)
    {
        mapped = ResourceIndices::eKnockZ_11;
    }
    else if (motion < eAbeMotions::Motion_112_Chant_45B1C0)
    {
        mapped = ResourceIndices::ePick_3;
    }
    else if (motion < eAbeMotions::Motion_114_DoorEnter_459470)
    {
        mapped = ResourceIndices::eChant_16;
    }
    else if (motion < eAbeMotions::Motion_116_MineCarEnter_458780)
    {
        mapped = ResourceIndices::eDoor_14;
    }
    else if (motion < eAbeMotions::Motion_119_ToShrykull_45A990)
    {
        mapped = ResourceIndices::eMineCar_18;
    }
    else if (motion < eAbeMotions::Motion_121_LiftGrabBegin_45A600)
    {
        mapped = ResourceIndices::eMorph_19;
    }
    else if (motion < eAbeMotions::Motion_126_TurnWheelBegin_456700)
    {
        mapped = ResourceIndices::eLift_17;
    }
    else if (motion < eAbeMotions::Motion_129_PoisonGasDeath_4565C0)
    {
        mapped = ResourceIndices::eWork_20;
    }
    else
    {
        mapped = ResourceIndices::eGas_21;
        if (motion >= 130) // max motions
        {
            // Impossible case?
            LOG_ERROR("Motion is out of bounds !!! " << motion);
            mapped = static_cast<s16>(motion);
        }
    }

    if (mapped == field_128.field_10_resource_index)
    {
        return field_10_resources_array.ItemAt(field_128.field_10_resource_index);
    }

    // Check to never free basic res
    if (field_128.field_10_resource_index != ResourceIndices::eBasic_0 && field_128.field_10_resource_index != 1)
    {
        ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(field_128.field_10_resource_index));
        field_10_resources_array.SetAt(field_128.field_10_resource_index, nullptr);
    }

    // Check to never load basic res
    if (mapped != ResourceIndices::eBasic_0 && mapped != 1)
    {
        field_10_resources_array.SetAt(mapped, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, sAbeResourceIDTable_554D60[mapped], TRUE, FALSE));
    }

    field_128.field_10_resource_index = mapped;
    return field_10_resources_array.ItemAt(mapped);
}

static bool IsSameScaleAsHoist(Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->field_16_scale == Scale_short::eFull_0 && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Bg)
    {
        return false;
    }
    else if (pHoist->field_16_scale == Scale_short::eHalf_1 && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
    {
        return false;
    }
    return true;
}

static bool IsSameScaleAsEdge(Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->field_14_scale == Scale_int::eFull_0 && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Bg)
    {
        return false;
    }
    else if (pEdge->field_14_scale == Scale_int::eHalf_1 && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
    {
        return false;
    }
    return true;
}

static bool IsFacingSameDirectionAsHoist(Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingLeft && !pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return false;
    }
    else if (pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingRight && pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return false;
    }

    return true;
}

static bool isEdgeGrabbable(Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->field_10_grab_direction == Path_Edge::GrabDirection::eFacingLeft && pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return true;
    }
    else if (pEdge->field_10_grab_direction == Path_Edge::GrabDirection::eFacingRight && !pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return true;
    }
    else if (pEdge->field_10_grab_direction == Path_Edge::GrabDirection::eFacingAnyDirection)
    {
        return true;
    }

    return false;
}

void Abe::Motion_0_Idle_44EEB0()
{
    if (Input_IsChanting_45F260() && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit6_prevent_chanting)))
    {
        if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
        {
            mCurrentMotion = eAbeMotions::Motion_119_ToShrykull_45A990;
        }
        else
        {
            // Go to chanting.
            field_124_timer = sGnFrame + 90;
            mCurrentMotion = eAbeMotions::Motion_112_Chant_45B1C0;
            SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 1);
        }
        field_120_state.raw = 0;
        return;
    }

    // Go to game speak motion.
    const u32 held = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (held & (InputCommands::Enum::eChant | InputCommands::Enum::eGameSpeak8 | InputCommands::Enum::eGameSpeak7 | InputCommands::Enum::eGameSpeak6 | InputCommands::Enum::eGameSpeak5 | InputCommands::Enum::eGameSpeak4 | InputCommands::Enum::eGameSpeak3 | InputCommands::Enum::eGameSpeak2 | InputCommands::Enum::eGameSpeak1))
    {
        field_118_prev_held = held;
        mCurrentMotion = eAbeMotions::Motion_11_Speak_45B0A0;
        return;
    }

    if (Input().isPressed(sInputKey_Hop_5550E0))
    {
        // Some strange alternative way of hoisting, hangover from PSX AO Demo?
        if (Input().isPressed(sInputKey_Up_5550D8))
        {
            TryHoist_44ED30();
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;

            BaseGameObject* pObj = VIntoBirdPortal(2);
            if (pObj)
            {
                field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                field_1A8_portal_id = pObj->field_8_object_id;
            }
        }
        return;
    }

    if (ToLeftRightMovement_44E340())
    {
        // To turn/walk/sneak/run
        return;
    }

    if (Input().isPressed(sInputKey_Down_5550DC))
    {
        // Check for a lift rope (going down)
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                const FP halfGrid = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = (mBaseAnimatedWithPhysicsGameObject_XPos - liftPlatformXMidPoint) >= FP_FromInteger(0) ? mBaseAnimatedWithPhysicsGameObject_XPos - liftPlatformXMidPoint : liftPlatformXMidPoint - mBaseAnimatedWithPhysicsGameObject_XPos;
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    mCurrentMotion = eAbeMotions::Motion_121_LiftGrabBegin_45A600;
                    return;
                }
            }
        }

        // Look below for a down hoist
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + 16,
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + 16,
            TlvTypes::Hoist_2));

        if (pHoist)
        {
            if (!IsSameScaleAsHoist(pHoist, this))
            {
                return;
            }

            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                // Yeah go down
                mCurrentMotion = eAbeMotions::Motion_66_LedgeDescend_454970;
            }
            else
            {
                if (pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingAnyDirection)
                {
                    // We can hoist down from any side
                    mCurrentMotion = eAbeMotions::Motion_66_LedgeDescend_454970;
                }
                else
                {
                    // Otherwise gotta turn around
                    mNextMotion = eAbeMotions::Motion_66_LedgeDescend_454970;
                    mCurrentMotion = eAbeMotions::Motion_2_StandingTurn_451830;
                }
            }
        }
        else
        {
            // Isn't a hoist so just crouch
            mCurrentMotion = eAbeMotions::Motion_19_StandToCrouch_453DC0;
        }
        return;
    }

    if (Input().isHeld(sInputKey_FartRoll_5550F0))
    {
        // Do the fart sound
        Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eFart_3);

        // Let others hear the fart
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);

        if (field_198_has_evil_fart)
        {
            // An evil fart
            field_198_has_evil_fart = FALSE;
            Create_Fart_421D20();

            if (field_10_resources_array.ItemAt(22))
            {
                ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(22));
                field_10_resources_array.SetAt(22, nullptr);
                mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
                return;
            }
        }
        else
        {
            const FP fartScale = FP_FromDouble(0.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
            const FP fartYPos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(24) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            FP fartXPos = {};

            // A normal fart, figure out the direction of Abe's Arse
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                fartXPos = mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                fartXPos = mBaseAnimatedWithPhysicsGameObject_XPos - (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            New_Smoke_Particles(fartXPos, fartYPos, fartScale, 3, 32u, 128u, 32u);
        }

        mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
        return;
    }

    bool handleDoActionOrThrow = false;
    if (Input().isPressed(sInputKey_Up_5550D8))
    {
        // Check for lift rope.. (going up)
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                const FP halfGrid = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - liftPlatformXMidPoint);
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    mCurrentMotion = eAbeMotions::Motion_121_LiftGrabBegin_45A600;
                    return;
                }
            }
        }

        // Get the first TLV
        Path_TLV* pTlv = sPath_dword_BB47C0->TlvGetAt(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);

        // Handle objects that accept "up"
        while (pTlv)
        {
            switch (pTlv->mTlvType32.mType)
            {
                case TlvTypes::Door_5:
                    if (NearDoorIsOpen_44EE10() && !mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
                    {
                        BaseAliveGameObjectPathTLV = pTlv;
                        field_120_state.door = AbeDoorStates::eAbeComesIn_0;
                        mCurrentMotion = eAbeMotions::Motion_114_DoorEnter_459470;
                    }
                    else
                    {
                        // Check for pressed + held so that Abe does a dunno only once, otherwise he will
                        // loop the dunno while up button is down.
                        if (Input().isHeld(sInputKey_Up_5550D8))
                        {
                            mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                        }
                    }
                    return;

                case TlvTypes::LocalWell_8:
                {
                    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
                    {
                        break;
                    }

                    // Bail if scale doesn't match
                    Path_WellLocal* pWell = static_cast<Path_WellLocal*>(pTlv);
                    if ((pWell->field_0_scale != Scale_short::eFull_0 || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(1.0)) && (pWell->field_0_scale != Scale_short::eHalf_1 || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5)))
                    {
                        break;
                    }

                    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_WalkToRun);
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_78_WellBegin_45C810;
                }
                    return;

                case TlvTypes::WellExpress_23:
                {
                    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
                    {
                        break;
                    }

                    // Bail if scale doesn't match
                    Path_WellBase* pWell = static_cast<Path_WellBase*>(pTlv);
                    if ((pWell->field_0_scale != Scale_short::eFull_0 || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(1.0)) && (pWell->field_0_scale != Scale_short::eHalf_1 || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5)))
                    {
                        break;
                    }

                    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_WalkToRun);
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::jMotion_81_WellBegin_45C7F0;
                }
                    return;

                case TlvTypes::MovieHandStone_27:
                case TlvTypes::HandStone_61:
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_86_HandstoneBegin_45BD00;
                    field_120_state.stone = StoneStates::eHandstoneBegin_0;
                    return;

                case TlvTypes::BoomMachine_59:
                {
                    auto pMachineButton = static_cast<BoomMachine*>(FindObjectOfType(
                        ReliveTypes::eGrenadeMachine,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(25)));
                    if (pMachineButton)
                    {
                        pMachineButton->VHandleButton();
                        mCurrentMotion = eAbeMotions::Motion_88_GrenadeMachineUse_45C510;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                    }
                }
                break;

                case TlvTypes::WorkWheel_79:
                {
                    bool bCanUseWheel = true;
                    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
                    {
                        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                        if (!pObj)
                        {
                            break;
                        }

                        if (pObj->Type() == ReliveTypes::eMudokon && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale)
                        {
                            FP xDiff = pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos;
                            if (xDiff < FP_FromInteger(0))
                            {
                                xDiff = mBaseAnimatedWithPhysicsGameObject_XPos - pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
                            }

                            FP gridWidth = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                            if (xDiff < gridWidth)
                            {
                                FP yDiff = pObj->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos;
                                if (yDiff < FP_FromInteger(0))
                                {
                                    yDiff = mBaseAnimatedWithPhysicsGameObject_YPos - pObj->mBaseAnimatedWithPhysicsGameObject_YPos;
                                }

                                if (yDiff < gridWidth)
                                {
                                    bCanUseWheel = false;
                                    break;
                                }
                            }
                        }
                    }

                    if (bCanUseWheel)
                    {
                        mCurrentMotion = eAbeMotions::Motion_126_TurnWheelBegin_456700;
                        BaseGameObject* pObj_148 = FindObjectOfType(ReliveTypes::eWheel, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50)));
                        if (pObj_148)
                        {
                            field_164_wheel_id = pObj_148->field_8_object_id;
                        }
                    }
                }
                break;

                case TlvTypes::BrewMachine_101:
                    mCurrentMotion = eAbeMotions::Motion_89_BrewMachineBegin_4584C0;
                    field_120_state.raw = 0;
                    break;

                default:
                    break;
            }

            // To next TLV (if any)
            pTlv = sPath_dword_BB47C0->TlvGetAt(
                pTlv,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
        }

        if (!TryEnterMineCar_4569E0())
        {
            if (mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0)
            {
                TryHoist_44ED30();
            }
            handleDoActionOrThrow = true;
        }
    }

    if (!Input().isPressed(sInputKey_Up_5550D8) || handleDoActionOrThrow)
    {
        if ((sInputKey_ThrowItem_5550F4 & held) && mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0)
        {
            if (field_1A2_throwable_count > 0 || gInfiniteGrenades_5C1BDE)
            {
                field_158_throwable_id = Make_Throwable_49AF30(
                                             mBaseAnimatedWithPhysicsGameObject_XPos,
                                             mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(40),
                                             0)
                                             ->field_8_object_id;

                if (!bThrowableIndicatorExists_5C112C)
                {
                    const FP xOffSet = ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) ? FP_FromInteger(15) : FP_FromInteger(-15)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    relive_new ThrowableTotalIndicator(
                        mBaseAnimatedWithPhysicsGameObject_XPos + xOffSet,
                        mBaseAnimatedWithPhysicsGameObject_YPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-50)),
                        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
                        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale,
                        field_1A2_throwable_count,
                        TRUE);
                }

                mCurrentMotion = eAbeMotions::Motion_104_RockThrowStandingHold_455DF0;

                if (!gInfiniteGrenades_5C1BDE)
                {
                    field_1A2_throwable_count--;
                }
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
            }
        }
        else
        {
            if (Input().isHeld(sInputKey_DoAction_5550E4)) // not throwing, maybe pressing up and pressing action, so do action
            {
                mCurrentMotion = HandleDoAction_455BD0();
            }
        }
    }
}

void Abe::Motion_1_WalkLoop_44FBA0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward_44E9A0();

    if (mCurrentMotion != eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        return;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9);
    }

    const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 2 && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 11)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 5 && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 14)
        {
            return;
        }

        Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);

        MapFollowMe(TRUE);

        if (sInputKey_Run_5550E8 & pressed)
        {
            // Walk to run
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
            {
                mCurrentMotion = eAbeMotions::Motion_49_MidWalkToRun_450200;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_48_WalkToRun_4500A0;
            }
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            // Walk to sneak
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 5)
            {
                mCurrentMotion = eAbeMotions::Motion_43_MidWalkToSneak_450380;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_41_WalkToSneak_450250;
            }
        }
        return;
    }

    // Direction changed
    bool bEndWalking = false;
    if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && (sInputKey_Left_5550D4 & pressed)) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && (sInputKey_Right_5550D0 & pressed)))
    {
        bEndWalking = true;
    }

    // Hopping
    if (sInputKey_Hop_5550E0 & field_118_prev_held)
    {
        mNextMotion = eAbeMotions::Motion_28_HopMid_451C50;
        bEndWalking = true;
    }

    if (!bEndWalking)
    {
        if ((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed)
        {
            // Check if walking into something
            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            const FP offX = gridSize * FP_FromDouble(1.5);
            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), offX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), offX))
            {
                bEndWalking = true;
            }
        }
        else
        {
            // No longer trying to walk
            bEndWalking = true;
        }
    }

    if (bEndWalking)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2)
        {
            mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle_450080;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_4_WalkToIdle_44FFC0;
        }
        field_118_prev_held = 0;
    }
}

void Abe::Motion_2_StandingTurn_451830()
{
    const s16 currentFrame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (currentFrame == 4 && (sInputKey_Run_5550E8 & pressed) && (sInputKey_Right_5550D0 | sInputKey_Left_5550D4) & pressed)
    {
#if ORIGINAL_GAME_FIXES || ORIGINAL_GAME_FIX_AUTO_TURN
        mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0; // OG Change - Fixes "Auto-Turn" bug
#endif

        mCurrentMotion = eAbeMotions::Motion_61_TurnToRun_456530;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
        }
    }
    else
    {
        // Play "turning" sound effect
        if (currentFrame == 0)
        {
            Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
            if (mNextMotion)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = 0;
            }
            else
            {
                ToIdle_44E6B0();
            }
        }
    }
}

void Abe::Motion_3_Fall_459B60()
{
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_128.field_8_x_vel_slow_by);
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_128.field_8_x_vel_slow_by);
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    const s32 bCollision = InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.80));
    SetActiveCameraDelayedFromDir();

    // Are we falling into a local well?
    BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::LocalWell_8);

    if (!BaseAliveGameObjectPathTLV)
    {
        // No, are we falling into an express well?
        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::WellExpress_23);
    }

    // Handle falling into a well
    if (BaseAliveGameObjectPathTLV)
    {
        if (mHealth > FP_FromInteger(0))
        {
            if (BaseAliveGameObjectPathTLV->mTlvType32 == TlvTypes::LocalWell_8 || BaseAliveGameObjectPathTLV->mTlvType32 == TlvTypes::WellExpress_23)
            {
                // The well must be on the same scale/layer
                Path_WellBase* pWellBase = static_cast<Path_WellBase*>(BaseAliveGameObjectPathTLV);
                if ((pWellBase->field_0_scale == Scale_short::eFull_0 && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
                    || (pWellBase->field_0_scale == Scale_short::eHalf_1 && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5)))
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_WalkToRun);
                    mCurrentMotion = eAbeMotions::Motion_75_JumpIntoWell_45C7B0;
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
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                BaseAliveGameObjectCollisionLine = pPathLine;
                MapFollowMe(TRUE);
                field_124_timer = sGnFrame + 30;

                // See if there is a soft landing at our feet (given we known we just hit the floor)
                Path_SoftLanding* pSoftLanding = static_cast<Path_SoftLanding*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    TlvTypes::SoftLanding_75));

                if (pSoftLanding)
                {
                    if (!SwitchStates_Get(static_cast<s16>(pSoftLanding->field_10_switch_id)))
                    {
                        pSoftLanding = nullptr;
                    }
                }

                if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit7_land_softly)
                    || (pSoftLanding && mHealth > FP_FromInteger(0))                                   // If we are dead soft landing won't save us
                    || ((mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos) < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(180)) // Check we didn't fall far enough to be killed
                        && (mHealth > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)))                   //TODO possibly OG bug: those conditions should probably be grouped the following way: ((A || B || C ) && D)
                {
                    mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
                }
                else
                {
                    // Slam into the floor and break all your bones
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_84_FallLandDie_45A420;
                    field_128.field_4_regen_health_timer = sGnFrame + 900;
                }

                mPreviousMotion = eAbeMotions::Motion_3_Fall_459B60;

                PSX_Point xy{FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(10)), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10))};
                PSX_Point wh{FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(10)), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10))};
                VOnCollisionWith(
                    xy,
                    wh,
                    ObjList_5C1B78,
                    1,
                    reinterpret_cast<TCollisionCallBack>(&BaseAliveGameObject::OnTrapDoorIntersection)); // Danger danger.. but will probably work.. can't see how else they would have got this to work
            }
            break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                ToKnockback_44E700(1, 1);
                break;
        }
        return;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return;
    }

    // Look down 75 for an edge
    Path_Edge* pEdge = static_cast<Path_Edge*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75))),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::Edge_3));

    bool tryToHang = false;
    if (pEdge)
    {
        if (pEdge->field_12_bCan_grab == Choice_short::eYes_1 && IsSameScaleAsEdge(pEdge, this) && (isEdgeGrabbable(pEdge, this)))
        {
            tryToHang = true;
        }
        BaseAliveGameObjectPathTLV = pEdge;
    }
    else // Didn't find and edge to grab so check if falling onto a hoist
    {
        // Look down 20 for a hoist
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)),
            TlvTypes::Hoist_2));

        if (pHoist)
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this) && IsSameScaleAsHoist(pHoist, this))
            {
                tryToHang = true;
            }
        }

        BaseAliveGameObjectPathTLV = pHoist;
    }

    if (tryToHang)
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX == FP_FromInteger(0))
        {
            return;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mTopLeft.x + BaseAliveGameObjectPathTLV->mBottomRight.x) / 2);

        MapFollowMe(TRUE);

        if (!sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y - 10), // TODO: Negative ??
                mBaseAnimatedWithPhysicsGameObject_XPos,
                FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y + 10),
                &pPathLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
        {
            return;
        }

        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        BaseAliveGameObjectCollisionLine = pPathLine;
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        if (BaseAliveGameObjectPathTLV->mTlvType32.mType != TlvTypes::Hoist_2 || (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y - 1 * BaseAliveGameObjectPathTLV->mTopLeft.y)) >= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(70)))
        {
            mCurrentMotion = eAbeMotions::Motion_69_LedgeHangWobble_454EF0;
            mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
        }
        else
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
            mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
            mBaseAliveGameObjectLastAnimFrame = 12;
            mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
        }
    }
}

void Abe::Motion_4_WalkToIdle_44FFC0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveForward_44E9A0();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe(TRUE);

            if (mNextMotion == eAbeMotions::Motion_28_HopMid_451C50)
            {
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
                BaseGameObject* pObj = VIntoBirdPortal(2);
                if (pObj)
                {
                    field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                    field_1A8_portal_id = pObj->field_8_object_id;
                }
            }
            else
            {
                ToIdle_44E6B0();
            }
        }
    }
    else
    {
        Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
    }
}

void Abe::Motion_5_MidWalkToIdle_450080()
{
    // TODO Jump func omitted jState_WalkEndLeftFoot_40459
    Motion_4_WalkToIdle_44FFC0();
}

void Abe::Motion_6_WalkBegin_44FEE0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    // They hear me walking, they hating
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_7_Speak_45B140()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_8_Speak_45B160()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_9_Speak_45B180()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_10_Fart_45B1A0()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_11_ToSpeak_45B0A0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = DoGameSpeak_45AB70(field_118_prev_held);
        if (mCurrentMotion == -1)
        {
            ToIdle_44E6B0();
        }
        else
        {
            if (mCurrentMotion != eAbeMotions::Motion_62_Punch_454750)
            {
                EventBroadcast(kEventSpeaking, this);
            }
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        }
        field_118_prev_held = 0;
    }
}

void Abe::Motion_12_Null_4569C0()
{
}

void Abe::Motion_13_HoistBegin_452B20()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        const FP velY = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-8);
        mBaseAnimatedWithPhysicsGameObject_VelY = velY;
        mBaseAnimatedWithPhysicsGameObject_YPos += velY;
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_14_HoistIdle_452440;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_14_HoistIdle_452440()
{
    //sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id); // NOTE: Return never used
    BaseGameObject* pfield_110_id = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (Is_Celling_Above_44E8D0())
    {
        ToKnockback_44E700(1, 1);
        return;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
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
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));

                MapFollowMe(1);

                BaseAliveGameObjectCollisionLine = pLine;

                mCurrentMotion = eAbeMotions::Motion_15_HoistLand_452BA0;
                mPreviousMotion = eAbeMotions::Motion_14_HoistIdle_452440;

                VOnCollisionWith(
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)},
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5)))},
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                break;
            }
        }
        return;
    }

    PullRingRope* pPullRope = GetPullRope_44D120();
    if (pPullRope)
    {
        if (pPullRope->VPull(this))
        {
            mCurrentMotion = eAbeMotions::Motion_70_RingRopePullHang_455AF0;
            mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
            field_15C_pull_rope_id = pPullRope->field_8_object_id;
            return;
        }
        field_15C_pull_rope_id = -1;
    }

    Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::Hoist_2));
    BaseAliveGameObjectPathTLV = pHoist;

    if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
    {
        // Going down and no hoist, can't do anything
        if (!pHoist)
        {
            return;
        }

        // Hoist is too far away
        if (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y + -BaseAliveGameObjectPathTLV->mTopLeft.y) > (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(90)) || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
        {
            return;
        }
    }

    if (pHoist)
    {
        if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this) || pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingAnyDirection))
        {
            if (pHoist->field_10_type == Path_Hoist::Type::eOffScreen)
            {
                if (gMap.SetActiveCameraDelayed(Map::MapDirections::eMapTop_2, this, -1))
                {
                    PSX_Prevent_Rendering_4945B0();
                    mCurrentMotion = eAbeMotions::Motion_68_ToOffScreenHoist_454B80;
                    return;
                }

                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);

                if (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y - 1 * BaseAliveGameObjectPathTLV->mTopLeft.y)
                    >= mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(70))
                {
                    mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                    mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
                    mBaseAliveGameObjectLastAnimFrame = 12;
                }
                mBaseAnimatedWithPhysicsGameObject_YPos -= mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75);
                mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
            }
            else
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
                if (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y - 1 * BaseAliveGameObjectPathTLV->mTopLeft.y) >= mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(70))
                {
                    mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                    mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
                    mBaseAliveGameObjectLastAnimFrame = 12;
                }
            }

            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y - 10),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y + 10),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                if (pfield_110_id)
                {
                    if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || 
                        BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                    {
                        VOnCollisionWith(
                            {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)},
                            {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5))},
                            ObjList_5C1B78,
                            1,
                            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                    }
                }
                mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_14_HoistIdle_452440;
            }
        }
    }
    else
    {
        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

void Abe::Motion_15_HoistLand_452BA0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2)
    {
        if (mPreviousMotion == eAbeMotions::Motion_3_Fall_459B60)
        {
            Environment_SFX_457A40(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Landing and standing
        ToIdle_44E6B0();
    }
}

void Abe::Motion_16_LandSoft_45A360()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit7_land_softly)))
        {
            // Hitting the floor events.
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
        }

        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit7_land_softly);

        // Hitting the floor sounds.
        if (mPreviousMotion == eAbeMotions::Motion_3_Fall_459B60)
        {
            Environment_SFX_457A40(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            // If Abe is controlling something else then must be standing and chanting.
            mCurrentMotion = eAbeMotions::Motion_112_Chant_45B1C0;
        }

        if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (InputCommands::Enum::eRight | InputCommands::Enum::eLeft))
        {
            ToLeftRightMovement_44E340();
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_17_CrouchIdle_456BC0()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        mCurrentMotion = eAbeMotions::Motion_98_RollOffEdge_455AA0;
        return;
    }

    field_118_prev_held = 0;
    field_11C_released_buttons = 0;

    // Crouching game speak
    if (CrouchingGameSpeak_453E10())
    {
        field_118_prev_held = 0;
        EventBroadcast(kEventSpeaking, this);
        return;
    }

    // Hit bombs/pick up items ?
    if (sInputKey_DoAction_5550E4 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held)
    {
        if (!((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed))
        {
            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            PickUpThrowabe_Or_PressBomb_454090(gridSize + mBaseAnimatedWithPhysicsGameObject_XPos, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)), 0);
        }
    }

    const u32 held = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    // Crouching throw stuff
    if (sInputKey_ThrowItem_5550F4 & held
        && mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0
        && (field_1A2_throwable_count > 0 || gInfiniteGrenades_5C1BDE))
    {
        field_158_throwable_id = Make_Throwable_49AF30(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(40), 0)->field_8_object_id;
        if (!bThrowableIndicatorExists_5C112C)
        {
            const FP yOff = mBaseAnimatedWithPhysicsGameObject_YPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-30));
            const FP xOff = mBaseAnimatedWithPhysicsGameObject_SpriteScale * (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-10) : FP_FromInteger(10));
            relive_new ThrowableTotalIndicator(
                mBaseAnimatedWithPhysicsGameObject_XPos + xOff,
                yOff,
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
                mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale,
                field_1A2_throwable_count,
                1);
        }

        mCurrentMotion = eAbeMotions::Motion_107_RockThrowCrouchingHold_454410;

        if (!gInfiniteGrenades_5C1BDE)
        {
            field_1A2_throwable_count--;
        }
    }
    else
    {
        // Try to stand up
        if (held & sInputKey_Up_5550D8 && !Is_Celling_Above_44E8D0())
        {
            mCurrentMotion = eAbeMotions::Motion_18_CrouchToStand_454600;
            return;
        }

        // Crouching farts
        if (sInputKey_FartRoll_5550F0 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held)
        {
            pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eFart_3);

            Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);

            if (field_198_has_evil_fart)
            {
                field_198_has_evil_fart = 0;
                Create_Fart_421D20();

                if (field_10_resources_array.ItemAt(22))
                {
                    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(22));
                    field_10_resources_array.SetAt(22, nullptr);
                }
            }
            else
            {
                const FP scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5);
                const FP ypos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                FP xpos = {};
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    xpos = mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                else
                {
                    xpos = mBaseAnimatedWithPhysicsGameObject_XPos - (FP_FromInteger(10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                New_Smoke_Particles(xpos, ypos, scale, 3, 32u, 128u, 32u);
            }

            mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        }
        else
        {
            // Crouching turns
            if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Right_5550D0)
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mCurrentMotion = eAbeMotions::Motion_37_CrouchTurn_454390;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_22_RollBegin_4539A0;
                    field_11C_released_buttons = 0;
                }
            }

            if (sInputKey_Left_5550D4 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mCurrentMotion = eAbeMotions::Motion_22_RollBegin_4539A0;
                    field_11C_released_buttons = 0;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_37_CrouchTurn_454390;
                }
            }
        }
    }
}

void Abe::Motion_18_CrouchToStand_454600()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
    {
        if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (InputCommands::Enum::eRight | InputCommands::Enum::eLeft))
        {
            ToLeftRightMovement_44E340();
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
        field_118_prev_held = 0;
    }
}

void Abe::Motion_19_StandToCrouch_453DC0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PickUpThrowabe_Or_PressBomb_454090(mBaseAnimatedWithPhysicsGameObject_XPos, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)), 1);
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }
}

// TODO: End crouching game speak/fart?
void Abe::Motion_20_CrouchSpeak_454550()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;

        CrouchingGameSpeak_453E10();

        if (mCurrentMotion == eAbeMotions::Motion_20_CrouchSpeak_454550 || mCurrentMotion == eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0)
        {
            EventBroadcast(kEventSpeaking, this);
        }
        field_118_prev_held = 0;
    }
}

void Abe::jMotion_21_ToCrouchSpeak_4545E0()
{
    // Note j_401389 omitted
    Motion_20_CrouchSpeak_454550();
}

void Abe::Motion_22_RollBegin_4539A0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
    }

    const FP xpos = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20);
    if (WallHit(xpos, mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback_44E700(1, 1);
        mCurrentMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eAbeMotions::Motion_23_RollLoop_453A90;
        }
        MoveForward_44E9A0();
    }
}

void Abe::Motion_23_RollLoop_453A90()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_11C_released_buttons |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_10_released;

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback_44E700(1, 1);
        mCurrentMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
    }
    else
    {
        MoveForward_44E9A0();

        if (mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 1 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 9)
            {
                if (!Input().isPressed(sInputKey_Run_5550E8)
                    || Input().isPressed(sInputKey_FartRoll_5550F0)
                    || Is_Celling_Above_44E8D0()
                    || field_128.field_14_rolling_motion_timer + 9 >= static_cast<s32>(sGnFrame))
                {
                    if (field_11C_released_buttons)
                    {
                        if (!Is_Celling_Above_44E8D0() && field_128.field_14_rolling_motion_timer + 9 < static_cast<s32>(sGnFrame))
                        {
                            ToLeftRightMovement_44E340();
                            field_11C_released_buttons = 0;
                        }
                    }
                }
                else
                {
                    ToLeftRightMovement_44E340();
                    field_118_prev_held = 0;
                }
            }
            else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 8)
            {
                MapFollowMe(TRUE);

                if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && !Input().isPressed(sInputKey_Right_5550D0)) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && !Input().isPressed(sInputKey_Left_5550D4)))
                {
                    mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                }
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 6)
            {
                Environment_SFX_457A40(EnvironmentSfx::eRollingNoise_8, 0, 32767, this);
            }
        }
    }
}

//TODO: probably unused?
void Abe::Motion_24_453D00()
{
    LOG_WARNING("never expected Motion_24_453D00 (roll loop end) to be called");
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback_44E700(1, 1);
        mCurrentMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
    }
    else
    {
        MoveForward_44E9A0();
        if (mCurrentMotion == eAbeMotions::Motion_24_453D00)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(TRUE);
                mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
        }
    }
}

void Abe::Motion_25_RunSlideStop_451330()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));

        if (mCurrentMotion == eAbeMotions::Motion_25_RunSlideStop_451330 && !RunTryEnterWell_451060() && !RunTryEnterDoor_451220())
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 15)
            {
                MapFollowMe(FALSE);
            }

            const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 9)
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 15)
                {
                    Environment_SFX_457A40(EnvironmentSfx::eSlideStop_0, 0, 32767, this);
                    MapFollowMe(TRUE);

                    if (!ToLeftRightMovement_44E340())
                    {
                        ToIdle_44E6B0();
                    }
                }
            }
            else if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && (sInputKey_Right_5550D0 & pressed)) || (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && (sInputKey_Left_5550D4 & pressed)))
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                mPreviousMotion = eAbeMotions::Motion_26_RunTurn_451500;
                mBaseAliveGameObjectLastAnimFrame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
            }
        }
    }
}

void Abe::Motion_26_RunTurn_451500()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));
        if (mCurrentMotion == eAbeMotions::Motion_26_RunTurn_451500
            && !RunTryEnterWell_451060()
            && !RunTryEnterDoor_451220())
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame) || (MapFollowMe(FALSE) && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                MapFollowMe(TRUE);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (sInputKey_Run_5550E8 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
                        mCurrentMotion = eAbeMotions::Motion_52_RunTurnToRun_451710;
                    }
                    else
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9);
                        mCurrentMotion = eAbeMotions::Motion_53_RunTurnToWalk_451800;
                    }
                }
                else
                {
                    if (sInputKey_Run_5550E8 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                        mCurrentMotion = eAbeMotions::Motion_52_RunTurnToRun_451710;
                    }
                    else
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
                        mCurrentMotion = eAbeMotions::Motion_53_RunTurnToWalk_451800;
                    }
                }
            }
        }
    }
}

void Abe::Motion_27_HopBegin_4521C0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 9)
    {
        // Change velocity at this frame
        const FP velX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-17) : FP_FromInteger(17));

        mBaseAnimatedWithPhysicsGameObject_VelX = velX;

        if (field_1A8_portal_id == -1)
        {
            // Check for hopping into a wall
            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                ToKnockback_44E700(1, 1);
                return;
            }
        }
        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        SetActiveCameraDelayedFromDir();
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        const FP velX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        mBaseAnimatedWithPhysicsGameObject_VelX = velX;
        mBaseAnimatedWithPhysicsGameObject_XPos += velX;

        const FP velY = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-2.7);
        mBaseAnimatedWithPhysicsGameObject_VelY = velY;
        mBaseAnimatedWithPhysicsGameObject_YPos += velY;

        VOnTrapDoorOpen();

        mCurrentMotion = eAbeMotions::Motion_28_HopMid_451C50;
        BaseAliveGameObjectCollisionLine = nullptr;

        if (field_1A8_portal_id == -1)
        {
            BaseGameObject* pObj = VIntoBirdPortal(2);
            if (pObj)
            {
                field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                field_1A8_portal_id = pObj->field_8_object_id;
            }
        }
    }
}

void Abe::Motion_28_HopMid_451C50()
{
    if (field_1A8_portal_id != -1)
    {
        IntoPortalStates_451990();
        return;
    }

    // Hopped into a wall?
    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);
        mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    // See if we've hit a path line floor type
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.80));
    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);

        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

                VOnCollisionWith(
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)},
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5))},
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);

                MapFollowMe(TRUE);
                mCurrentMotion = eAbeMotions::Motion_29_HopLand_4523D0;
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                return;
        }
    }
    else
    {
        // Dead so can't continue
        if (mHealth <= FP_FromInteger(0))
        {
            return;
        }

        // Check for an edge
        Path_Edge* pEdgeTlv = static_cast<Path_Edge*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75))),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::Edge_3));

        BaseAliveGameObjectPathTLV = pEdgeTlv;

        if (pEdgeTlv && pEdgeTlv->field_12_bCan_grab == Choice_short::eYes_1 && IsSameScaleAsEdge(pEdgeTlv, this) && ((isEdgeGrabbable(pEdgeTlv, this) && mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0))))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pEdgeTlv->mTopLeft.x + pEdgeTlv->mBottomRight.x) / 2);

            MapFollowMe(TRUE);

            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y - 10),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y + 10),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mCurrentMotion = eAbeMotions::Motion_69_LedgeHangWobble_454EF0;
                mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
            }
        }
    }

    if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return;
    }

    if (mCurrentMotion != eAbeMotions::Motion_28_HopMid_451C50)
    {
        return;
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromRaw(mBaseAnimatedWithPhysicsGameObject_VelX.fpValue / 2);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5));
    }

    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(2);

    field_128.field_8_x_vel_slow_by = FP_FromDouble(0.55);
    mCurrentMotion = eAbeMotions::Motion_96_HopToFall_4559E0;
    mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
}

void Abe::Motion_29_HopLand_4523D0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2
        && sInputKey_Hop_5550E0 & Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        mPreviousMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
        mBaseAliveGameObjectLastAnimFrame = 5;
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_30_RunJumpBegin_4532E0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    // Check if about to jump into a wall
    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(FP_FromInteger(10), mBaseAnimatedWithPhysicsGameObject_VelX)) // TODO: OG bug, why no scaling?
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
        {
            Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

            const FP velX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-7.6) : FP_FromDouble(7.6);
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * velX;

            const FP velY = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-9.6);
            mBaseAnimatedWithPhysicsGameObject_VelY = velY;
            mBaseAnimatedWithPhysicsGameObject_YPos += velY;

            VOnTrapDoorOpen();

            mCurrentMotion = eAbeMotions::Motion_31_RunJumpMid_452C10;
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

void Abe::Motion_31_RunJumpMid_452C10()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
    if (field_1A8_portal_id != -1)
    {
        IntoPortalStates_451990();
        return;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(FP_FromInteger(10), mBaseAnimatedWithPhysicsGameObject_VelX) || Is_Celling_Above_44E8D0())
    {
        mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.80));

    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eAbeMotions::Motion_32_RunJumpLand_453460;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                if (pLine->mLineType == eLineTypes::eDynamicCollision_32)
                {
                    VOnCollisionWith(
                        {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)},
                        {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5))},
                        ObjList_5C1B78,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                }
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                return;
        }
    }
    else
    {
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_VelX),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_VelX),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::Hoist_2));

        BaseAliveGameObjectPathTLV = pHoist;

        bool checkCollision = false;
        if (pHoist)
        {
            BaseAliveGameObjectPathTLV = pHoist;

            if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this) || pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingAnyDirection) && pHoist->field_10_type != Path_Hoist::Type::eOffScreen)
            {
                checkCollision = true;
            }
        }
        else
        {
            Path_Edge* pEdgeTlv = static_cast<Path_Edge*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60))),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::Edge_3));

            BaseAliveGameObjectPathTLV = pEdgeTlv;

            if (pEdgeTlv && pEdgeTlv->field_12_bCan_grab == Choice_short::eYes_1)
            {
                if (IsSameScaleAsEdge(pEdgeTlv, this) && (isEdgeGrabbable(pEdgeTlv, this)))
                {
                    checkCollision = true;
                }
            }
        }

        if (checkCollision)
        {
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y - 10),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y + 10),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mTopLeft.x + BaseAliveGameObjectPathTLV->mBottomRight.x) / 2);

                MapFollowMe(TRUE);
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
                BaseAliveGameObjectCollisionLine = pLine;
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

                if (BaseAliveGameObjectPathTLV->mTlvType32.mType != TlvTypes::Hoist_2 || FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y - 1 * BaseAliveGameObjectPathTLV->mTopLeft.y) >= mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(70))
                {
                    mCurrentMotion = eAbeMotions::Motion_69_LedgeHangWobble_454EF0;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                    mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
                    mBaseAliveGameObjectLastAnimFrame = 12;
                }

                if (!pLiftPoint)
                {
                    if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                        BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                    {
                        VOnCollisionWith(
                            {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)},
                            {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5))},
                            ObjList_5C1B78,
                            1,
                            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                    }
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
        }
    }

    if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return;
    }

    if (mCurrentMotion != eAbeMotions::Motion_31_RunJumpMid_452C10)
    {
        return;
    }

    field_128.field_8_x_vel_slow_by = FP_FromDouble(0.75);
    mCurrentMotion = eAbeMotions::Motion_97_RunJumpToFall_455A80;
    mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
}

void Abe::Motion_32_RunJumpLand_453460()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);

        MapFollowMe(TRUE);

        const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
        if (sInputKey_Left_5550D4 & pressed)
        {
            if (sInputKey_Hop_5550E0 & field_118_prev_held)
            {
                BaseGameObject* pPortal = VIntoBirdPortal(3);
                if (pPortal)
                {
                    field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                    field_1A8_portal_id = pPortal->field_8_object_id;
                    mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
                    field_118_prev_held = 0;
                    return;
                }
                mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                if (pressed & sInputKey_Run_5550E8)
                {
                    mCurrentMotion = eAbeMotions::Motion_54_RunJumpLandRun_4538F0;
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                }
                else
                {
                    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
                        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_55_RunJumpLandWalk_453970;
                        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
                    }
                }
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_26_RunTurn_451500;
                mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            }
        }
        else if (pressed & sInputKey_Right_5550D0)
        {
            if (sInputKey_Hop_5550E0 & field_118_prev_held)
            {
                BaseGameObject* pPortal = VIntoBirdPortal(3);
                if (pPortal)
                {
                    field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                    field_1A8_portal_id = pPortal->field_8_object_id;
                }
                mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mCurrentMotion = eAbeMotions::Motion_26_RunTurn_451500;
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
            }
            else if (pressed & sInputKey_Run_5550E8)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
                mCurrentMotion = eAbeMotions::Motion_54_RunJumpLandRun_4538F0;
            }
            else
            {
                if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)))
                {
                    mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
                    mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9);
                    mCurrentMotion = eAbeMotions::Motion_55_RunJumpLandWalk_453970;
                }
            }
        }
        else if (sInputKey_Hop_5550E0 & field_118_prev_held)
        {
            BaseGameObject* pPortal = VIntoBirdPortal(2);
            if (pPortal)
            {
                field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
                field_1A8_portal_id = pPortal->field_8_object_id;
            }
            mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
            field_118_prev_held = 0;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
            }
            Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
        }
    }
}

void Abe::DoRunJump()
{
    BaseGameObject* pObj = VIntoBirdPortal(3);
    if (pObj)
    {
        field_1A4_portal_sub_state = PortalSubStates::eJumpingInsidePortal_0;
        field_1A8_portal_id = pObj->field_8_object_id;
    }

    field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
    mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
    field_118_prev_held = 0;
}

void Abe::Motion_33_RunLoop_4508E0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
    }

    // Ran into wall?
    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
        ToKnockback_44E700(1, 1);
        return;
    }

    MoveForward_44E9A0();

    if (mCurrentMotion != eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
        return;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 8)
    {
        MapFollowMe(TRUE);
        if (field_118_prev_held & sInputKey_Hop_5550E0)
        {
            DoRunJump();
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);

        MapFollowMe(TRUE);

        // Turning around?
        if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && Input().isPressed(sInputKey_Left_5550D4)) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && Input().isPressed(sInputKey_Right_5550D0)))
        {
            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
            mCurrentMotion = eAbeMotions::Motion_26_RunTurn_451500;
            Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
            field_118_prev_held = 0;
            return;
        }

        if (field_118_prev_held & sInputKey_Hop_5550E0)
        {
            DoRunJump();
            return;
        }

        // Run to roll
        if (field_118_prev_held & sInputKey_FartRoll_5550F0)
        {
            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
            mCurrentMotion = eAbeMotions::jMotion_38_RunToRoll_453A70;
            field_11C_released_buttons = 0;
            field_118_prev_held = 0;
            return;
        }

        if (Input().isPressed(sInputKey_Left_5550D4) || Input().isPressed(sInputKey_Right_5550D0))
        {
            if (Input().isPressed(sInputKey_Run_5550E8))
            {
                field_118_prev_held = 0;
                return;
            }

            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            // Run to walk and hit wall
            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), gridSize) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize))
            {
                ToKnockback_44E700(1, 1);
            }
            else
            {
                // Run to walk
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
                {
                    mCurrentMotion = eAbeMotions::Motion_50_RunToWalk_450E20;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_51_MidRunToWalk_450F50;
                }
            }
        }
        else
        {
            // No longer running or even moving, so slide stop
            mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
            Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
        }

        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
        field_118_prev_held = 0;
    }
}

void Abe::Motion_34_DunnoBegin_44ECF0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_35_DunnoEnd_44ED10;
    }
}

void Abe::Motion_35_DunnoEnd_44ED10()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_36_Null_45BC50()
{
}

void Abe::Motion_37_CrouchTurn_454390()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 0)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
            if (mNextMotion)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = 0;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
            }
        }
    }
    else
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }
}

void Abe::jMotion_38_RunToRoll_453A70()
{
    Motion_22_RollBegin_4539A0();
}

// Almost the same as Motion_6_WalkBegin_44FEE0
void Abe::Motion_39_StandingToRun_450D40()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_40_SneakLoop_450550()
{
    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(TRUE);
    }
    else
    {
        MoveForward_44E9A0();

        if (mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550)
        {
            const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 13)
            {
                FP gridSize = {};
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                else
                {
                    gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }

                // Hit wall, changed direction or no longer trying to sneak
                if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), gridSize) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize)
                    || (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && (sInputKey_Left_5550D4 & pressed))
                    || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && (sInputKey_Right_5550D0 & pressed))
                    || !((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed))
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 3)
                    {
                        mCurrentMotion = eAbeMotions::Motion_46_SneakToIdle_450870;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::jMotion_47_MidSneakToIdle_4508C0;
                    }
                }
            }
            else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 6 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 16)
            {
                Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe(TRUE);
                if ((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed)
                {
                    // Sneak to walk
                    if (!(pressed & sInputKey_Sneak_5550EC))
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 6)
                        {
                            mCurrentMotion = eAbeMotions::Motion_44_MidSneakToWalk_450500;
                        }
                        else
                        {
                            mCurrentMotion = eAbeMotions::Motion_42_SneakToWalk_4503D0;
                        }
                    }
                }
                field_118_prev_held = 0;
            }
        }
    }
}

void Abe::Motion_41_WalkToSneak_450250()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(10);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_40_SneakLoop_450550;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_42_SneakToWalk_4503D0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_43_MidWalkToSneak_450380()
{
    Motion_41_WalkToSneak_450250();

    if (mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        mCurrentMotion = eAbeMotions::Motion_43_MidWalkToSneak_450380;
        mPreviousMotion = eAbeMotions::Motion_40_SneakLoop_450550;
        mBaseAliveGameObjectLastAnimFrame = 10;
    }
}

void Abe::Motion_44_MidSneakToWalk_450500()
{
    Motion_42_SneakToWalk_4503D0();

    if (mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        mCurrentMotion = eAbeMotions::Motion_44_MidSneakToWalk_450500;
        mPreviousMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }
}

void Abe::Motion_45_SneakBegin_4507A0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_40_SneakLoop_450550;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_46_SneakToIdle_450870()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
    }

    MoveForward_44E9A0();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe(TRUE);
        ToIdle_44E6B0();
    }
}

void Abe::jMotion_47_MidSneakToIdle_4508C0()
{
    // TODO: Skipped jmp func jAbe::State_SneakEnd_40330F
    Motion_46_SneakToIdle_450870();
}

void Abe::Motion_48_WalkToRun_4500A0()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(4);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
        field_118_prev_held = 0;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_49_MidWalkToRun_450200()
{
    Motion_48_WalkToRun_4500A0();

    if (mCurrentMotion == eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        mCurrentMotion = eAbeMotions::Motion_49_MidWalkToRun_450200;
        mPreviousMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
        mBaseAliveGameObjectLastAnimFrame = 8;
    }
}

void Abe::Motion_50_RunToWalk_450E20()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_51_MidRunToWalk_450F50()
{
    Motion_50_RunToWalk_450E20();

    if (mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        mCurrentMotion = eAbeMotions::Motion_51_MidRunToWalk_450F50;
        mPreviousMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }
}

void Abe::Motion_52_RunTurnToRun_451710()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Abe::Motion_53_RunTurnToWalk_451800()
{
    Motion_52_RunTurnToRun_451710();

    if (mCurrentMotion == eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }
}

void Abe::Motion_54_RunJumpLandRun_4538F0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
        }
    }
}

void Abe::Motion_55_RunJumpLandWalk_453970()
{
    Motion_54_RunJumpLandRun_4538F0();

    if (mCurrentMotion == eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }
}

void Abe::Motion_56_DeathDropFall_4591F0()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    if (field_124_timer == 0)
    {
        field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
        field_128.field_C_unused = 0;
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        field_128.field_0_abe_timer = sGnFrame + 90;
        field_124_timer++;
    }
    else if (field_124_timer == 1)
    {
        if (static_cast<s32>(sGnFrame) == field_128.field_0_abe_timer - 30)
        {
            SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        }
        else if (static_cast<s32>(sGnFrame) == field_128.field_0_abe_timer - 24)
        {
            SfxPlayMono(SoundEffect::KillEffect_64, 85);
            relive_new ScreenShake(true, false);
        }
        else if (static_cast<s32>(sGnFrame) >= field_128.field_0_abe_timer)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::Motion_57_Dead_4589A0()
{
    DeathFadeOut* pDeathFade_1 = static_cast<DeathFadeOut*>(sObjectIds.Find_Impl(field_148_fade_obj_id));
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(field_14C_circular_fade_id));

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);

    if (BaseAliveGameObject_PlatformId != -1)
    {
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (!pLiftPoint)
        {
            BaseAliveGameObject_PlatformId = -1;
        }
        else if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
        {
            static_cast<LiftPoint*>(pLiftPoint)->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        }
    }

    // TODO: states
    switch (field_124_timer)
    {
        case 0:
            EventBroadcast(kEventAbeDead, this);
            EventBroadcast(kEventHeroDying, this);
            field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
            field_128.field_C_unused = 0;
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            field_118_prev_held = 0;
            field_124_timer++;
            field_128.field_0_abe_timer = sGnFrame + 30;
            if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType32 == TlvTypes::DeathDrop_4)
            {
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger((Math_NextRandom() % 64) - 32) + mBaseAnimatedWithPhysicsGameObject_XPos;
                relive_new DeathBirdParticle(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_128.field_0_abe_timer + 60,
                    1,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mBaseAnimatedWithPhysicsGameObject_XPos;
                relive_new DeathBirdParticle(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_128.field_0_abe_timer + 15,
                    1,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            return;

        case 1:
            EventBroadcast(kEventHeroDying, this);
            if (!(sGnFrame % 4))
            {
                if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType32 == TlvTypes::DeathDrop_4)
                {
                    const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(15);
                    const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mBaseAnimatedWithPhysicsGameObject_XPos;
                    relive_new DeathBirdParticle(
                        xpos,
                        ypos,
                        (Math_NextRandom() % 8) + field_128.field_0_abe_timer + 60,
                        0,
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                else
                {
                    const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(15);
                    const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mBaseAnimatedWithPhysicsGameObject_XPos;
                    relive_new DeathBirdParticle(
                        xpos,
                        ypos,
                        (Math_NextRandom() % 8) + field_128.field_0_abe_timer + 15,
                        0,
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
            }

            mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.008);

            mBaseAnimatedWithPhysicsGameObject_RGB.r -= 2;
            mBaseAnimatedWithPhysicsGameObject_RGB.g -= 2;
            mBaseAnimatedWithPhysicsGameObject_RGB.b -= 2;

            if (static_cast<s32>(sGnFrame) > field_128.field_0_abe_timer)
            {
                field_128.field_0_abe_timer = sGnFrame + 60;

                if (BaseAliveGameObjectPathTLV)
                {
                    if (BaseAliveGameObjectPathTLV->mTlvType32 == TlvTypes::DeathDrop_4)
                    {
                        field_128.field_0_abe_timer = (sGnFrame + 60) + 45;
                    }
                }
                ++field_124_timer;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
            }
            return;
        case 2:
            EventBroadcast(kEventHeroDying, this);
            if (static_cast<s32>(sGnFrame) > field_128.field_0_abe_timer)
            {
                ++field_124_timer;
            }
            return;
        case 3:
        {
            EventBroadcast(kEventHeroDying, this);
            if (pDeathFade_1)
            {
                pDeathFade_1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                field_148_fade_obj_id = -1;
            }
            auto pDeathFade = relive_new DeathFadeOut(Layer::eLayer_FadeFlash_40, 1, 0, 8, TPageAbr::eBlend_2);
            if (pDeathFade)
            {
                field_148_fade_obj_id = pDeathFade->field_8_object_id;
            }

            if (pCircularFade)
            {
                pCircularFade->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            ++field_124_timer;
        }
            return;

        case 4:
            EventBroadcast(kEventHeroDying, this);

            if (!pDeathFade_1->field_7E_bDone)
            {
                return;
            }

            if (field_10A_unused)
            {
                return;
            }

            VOnTrapDoorOpen();

            BaseAliveGameObjectCollisionLine = nullptr;
            field_128.field_0_abe_timer = sGnFrame + 8;
            ++field_124_timer;
            //dword_5C2C64 = 0; // TODO: Never read ?
            return;

        case 5:
            EventBroadcast(kEventDeathReset, this);
            if (static_cast<s32>(sGnFrame) <= field_128.field_0_abe_timer)
            {
                return;
            }
            Make_Circular_Fade_4CE8C0(
                FP_FromInteger(sActiveQuicksaveData_BAF7F8.field_204_world_info.field_C_controlled_x),
                FP_FromInteger(sActiveQuicksaveData_BAF7F8.field_204_world_info.field_E_controlled_y),
                sActiveQuicksaveData_BAF7F8.field_204_world_info.field_10_controlled_scale != 0 ? FP_FromDouble(1.0) : FP_FromDouble(0.5),
                0,
                1,
                1);
            Quicksave_LoadActive();
            return;
        default:
            return;
    }
}

void Abe::Motion_58_DeadPre_4593E0()
{
    if (field_120_state.raw == 1)
    {
        mCurrentMotion = eAbeMotions::Motion_57_Dead_4589A0;
        field_124_timer = 2;
        field_128.field_0_abe_timer = sGnFrame + 60;
    }
    else
    {
        EventBroadcast(kEventHeroDying, this);
    }
}

void Abe::Motion_59_Null_459450()
{
}

void Abe::Motion_60_Unused_4A3200()
{
    
}

void Abe::Motion_61_TurnToRun_456530()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        // Was going to run into something
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveForward_44E9A0();

        if (mCurrentMotion == eAbeMotions::Motion_61_TurnToRun_456530)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
            }
        }
    }
}

void Abe::Motion_62_Punch_454750()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
    {
        FP gridSize = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
        else
        {
            gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
        }

        const FP kFP5 = FP_FromInteger(5);
        BaseGameObject* pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - kFP5);
        while (pSlapTarget)
        {
            // Is it in a motion where we can slap it?
            const s16 mud_motion = static_cast<Mudokon*>(pSlapTarget)->mCurrentMotion;
            if (mud_motion != 46 && mud_motion != 47)
            {
                // Can slap, so exit
                break;
            }

            // Try to get the next "stacked" mud - e.g if we have like 20 muds on 1 grid block this will iterate through them
            pSlapTarget = GetStackedSlapTarget(pSlapTarget->field_8_object_id, ReliveTypes::eMudokon, gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eLockedSoul, gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eSlig, gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eSlig, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eGlukkon, gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - kFP5);
        }

        if (pSlapTarget)
        {
            static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage(this);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_63_Sorry_454670()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        FP yOff = {};
        FP xOff = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            yOff = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
            xOff = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
        else
        {
            yOff = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
            xOff = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
        }

        auto pMud = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eMudokon, xOff, yOff));
        if (pMud)
        {
            pMud->VTakeDamage(this);
        }

        Mudokon_SFX(MudSounds::eSorry_27, 0, 0, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_64_AfterSorry_454730;
    }
}

void Abe::Motion_64_AfterSorry_454730()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_65_LedgeAscend_4548E0()
{
    const s16 curFrameNum = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    if (curFrameNum == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 32767, this);
    }
    else if (curFrameNum == 4)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        mShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Now the ascend is done go back to stand idle
        mShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
        MapFollowMe(TRUE);
        ToIdle_44E6B0();
    }
}

void Abe::Motion_66_LedgeDescend_454970()
{
    const s16 curFrameNum = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    if (curFrameNum == 2)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
    }
    else if (curFrameNum == 21)
    {
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(16)),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(16)),
            TlvTypes::Hoist_2));

        if (pHoist)
        {
            if (FP_FromInteger((pHoist->mBottomRight.y - 1 * pHoist->mTopLeft.y)) < mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(70))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = (FP_FromInteger(60) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos;

                PathLine* pLine = nullptr;
                FP hitX = {};
                FP hitY = {};

                if (sCollisions->Raycast(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5),
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5),
                        &pLine,
                        &hitX,
                        &hitY,
                        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
                    mShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(60) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                }
            }
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
    }
}

void Abe::Motion_67_LedgeHang_454E20()
{
    mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
    const s32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        mCurrentMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
    }
    else if (pressed & sInputKey_Down_5550DC)
    {
        VOnTrapDoorOpen();
        BaseAliveGameObjectCollisionLine = nullptr;
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab_4557B0;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75);
        mShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
}

void Abe::Motion_68_ToOffScreenHoist_454B80()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    // Get the current hoist - even though there is no need to?
    Path_TLV* pHoist = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::Hoist_2);

    // Find the hoist we are "connecting" to
    BaseAliveGameObjectPathTLV = pHoist;
    mBaseAnimatedWithPhysicsGameObject_YPos -= mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75);
    mShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);

    const FP ypos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y) - (FP_FromInteger(40) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    pHoist = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(ypos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(ypos),
        TlvTypes::Hoist_2);

    BaseAliveGameObjectPathTLV = pHoist;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (pHoist && sCollisions->Raycast(mBaseAnimatedWithPhysicsGameObject_XPos, FP_FromInteger(pHoist->mTopLeft.y - 10), mBaseAnimatedWithPhysicsGameObject_XPos, FP_FromInteger(pHoist->mTopLeft.y + 10), &pLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
    {
        BaseAliveGameObjectCollisionLine = pLine;
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        if (!pLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                VOnCollisionWith(
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)},
                    {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5))},
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }
        mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
    }
    else
    {
        mCurrentMotion = eAbeMotions::Motion_14_HoistIdle_452440;
    }
}

void Abe::Motion_69_LedgeHangWobble_454EF0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds)))
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
            Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2)
        {
            if (!(field_1AC_flags.Get(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds)))
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
                Mudokon_SFX(MudSounds::eHurt1_16, 45, -200, this);
            }
        }
        else
        {
            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        }
    }

    // Going up the ledge on wobble?
    const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        mCurrentMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
    }
    // Going down the ledge wobble?
    else if (pressed & sInputKey_Down_5550DC)
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        VOnTrapDoorOpen();
        BaseAliveGameObjectCollisionLine = nullptr;
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab_4557B0;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75);
        mShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
    // Now stabilized when wobble anim is done
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
    }
}

void Abe::Motion_70_RingRopePullHang_455AF0()
{
    PullRingRope* pPullRing = static_cast<PullRingRope*>(sObjectIds.Find_Impl(field_15C_pull_rope_id));
    if (pPullRing)
    {
        if (!pPullRing->VIsNotBeingPulled())
        {
            return;
        }
        pPullRing->VMarkAsPulled();
    }

    field_15C_pull_rope_id = -1;
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab_4557B0;
}

void Abe::Motion_71_Knockback_455090()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
    {
        FallOnBombs_44EC10();
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }

            MoveWithVelocity_450FA0(FP_FromDouble(0.67));

            if ((gMap.mCurrentLevel == EReliveLevelIds::eMines
                 || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
                 || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
                 || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
                 || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
            {
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 6)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(5);
            }

            Motion_3_Fall_459B60();

            if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420)
            {
                mCurrentMotion = eAbeMotions::Motion_71_Knockback_455090;
                SfxPlayMono(SoundEffect::KillEffect_64, 85);
                SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 95, 95);
            }
            else if (mCurrentMotion == eAbeMotions::Motion_16_LandSoft_45A360)
            {
                mCurrentMotion = eAbeMotions::Motion_71_Knockback_455090;
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!(mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2)) && (BaseAliveGameObjectCollisionLine || !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))))
        {
            if (mHealth > FP_FromInteger(0) || gAbeBulletProof_5C1BDA || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
            {
                mCurrentMotion = eAbeMotions::Motion_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::Motion_72_KnockbackGetUp_455340()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_73_PushWall_4553A0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 10)
    {
        if (Math_NextRandom() <= 127)
        {
            Environment_SFX_457A40(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 32767, this);
        }
    }
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_74_RollingKnockback_455290()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
    {
        FallOnBombs_44EC10();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!(mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2)))
        {
            if (mHealth > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)
            {
                mCurrentMotion = eAbeMotions::Motion_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::Motion_75_JumpIntoWell_45C7B0()
{
    mShadow->field_14_flags.Clear(Shadow::eBit2_Enabled);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;
    }
    jMotion_81_WellBegin_4017F8();
}

void Abe::Motion_76_ToInsideOfAWellLocal_45CA40()
{
    Motion_79_InsideWellLocal_45CA60();
}

void Abe::Motion_77_ToWellShotOut_45D130()
{
    Motion_80_WellShotOut_45D150();
}

void Abe::Motion_78_WellBegin_45C810()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame > 10)
    {
        mShadow->field_14_flags.Clear(Shadow::eBit2_Enabled);

        // Get a local well
        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::LocalWell_8);

        if (!BaseAliveGameObjectPathTLV)
        {
            // No local well, must be an express well
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::WellExpress_23);
        }

        const s16 xpos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
        const s16 tlv_mid_x = (BaseAliveGameObjectPathTLV->mTopLeft.x + BaseAliveGameObjectPathTLV->mBottomRight.x) / 2;
        if (xpos > tlv_mid_x)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos -= mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }
        else if (xpos < tlv_mid_x)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 11)
    {
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_124_timer = 15;

        SfxPlayMono(SoundEffect::WellEnter_21, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::WellExpress_23))
        {
            mCurrentMotion = eAbeMotions::Motion_82_InsideWellExpress_45CC80;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_79_InsideWellLocal_45CA60;
        }
    }
}

void Abe::Motion_79_InsideWellLocal_45CA60()
{
    const s32 gnFrame = field_124_timer;
    field_124_timer = gnFrame - 1;
    if (!gnFrame)
    {
        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::LocalWell_8);

        if (!BaseAliveGameObjectPathTLV)
        {
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::WellExpress_23);
        }

        field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_WalkToRun);

        Path_WellBase* pBaseWell = static_cast<Path_WellBase*>(BaseAliveGameObjectPathTLV);
        if (pBaseWell->mTlvType32 == TlvTypes::LocalWell_8)
        {
            Path_WellLocal* pLocal = static_cast<Path_WellLocal*>(pBaseWell);
            if (SwitchStates_Get(pBaseWell->field_2_switch_id))
            {
                Calc_Well_Velocity_45C530(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    pLocal->field_1C_on_dx,
                    pLocal->field_1E_on_dy);
            }
            else
            {
                Calc_Well_Velocity_45C530(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    pLocal->field_18_off_dx,
                    pLocal->field_1A_off_dy);
            }
        }
        else if (pBaseWell->mTlvType32 == TlvTypes::WellExpress_23)
        {
            Path_WellExpress* pExpress = static_cast<Path_WellExpress*>(pBaseWell);
            Calc_Well_Velocity_45C530(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                pExpress->field_18_exit_x,
                pExpress->field_1A_exit_y);
        }

        MapFollowMe(TRUE);

        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

        if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
        }

        SfxPlayMono(SoundEffect::WellExit_20, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        ++mCurrentMotion;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;
        }

        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_80_WellShotOut_45D150()
{
    if (mBaseAnimatedWithPhysicsGameObject_VelY >= FP_FromInteger(0))
    {
        Motion_3_Fall_459B60();
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelY += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8);

        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

        SetActiveCameraDelayedFromDir();

        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted) || (mCurrentMotion != eAbeMotions::Motion_80_WellShotOut_45D150 && mCurrentMotion != eAbeMotions::Motion_77_ToWellShotOut_45D130))
    {
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
        }

        mShadow->field_14_flags.Set(Shadow::Flags::eBit2_Enabled);
    }

    if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420)
    {
        mHealth = FP_FromInteger(1);
        mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
    }
}

void Abe::jMotion_81_WellBegin_45C7F0()
{
    jMotion_81_WellBegin_4017F8();
}

void Abe::Motion_82_InsideWellExpress_45CC80()
{
    BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::LocalWell_8);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::WellExpress_23);
    }

    Path_WellExpress* pExpressWell = static_cast<Path_WellExpress*>(BaseAliveGameObjectPathTLV);
    if (SwitchStates_Get(pExpressWell->field_2_switch_id))
    {
        field_19A_to_level = MapWrapper::FromAE(pExpressWell->field_24_enabled_well_level);
        field_19C_to_path = pExpressWell->field_26_enabled_well_path;
        field_19E_to_camera = pExpressWell->field_28_enabled_well_camera;
        field_1A0_door_id = pExpressWell->field_2A_enabled_well_id;
    }
    else
    {
        field_19A_to_level = MapWrapper::FromAE(pExpressWell->field_1C_disabled_well_level);
        field_19C_to_path = pExpressWell->field_1E_disabled_well_path;
        field_19E_to_camera = pExpressWell->field_20_disabled_well_camera;
        field_1A0_door_id = pExpressWell->field_22_disabled_well_id;
    }

    field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (field_19A_to_level != gMap.mCurrentLevel || field_19C_to_path != gMap.mCurrentPath || field_19E_to_camera != gMap.mCurrentCamera)
    {
        field_124_timer = 1;

        if (pExpressWell->field_32_movie_id)
        {
            gMap.SetActiveCam(field_19A_to_level, field_19C_to_path, field_19E_to_camera, CameraSwapEffects::ePlay1FMV_5, pExpressWell->field_32_movie_id, 0);
        }
        else
        {
            gMap.SetActiveCam(field_19A_to_level, field_19C_to_path, field_19E_to_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }

        // FeeCo hack!
        if (field_19A_to_level == EReliveLevelIds::eFeeCoDepot && field_19C_to_path == 1 && field_19E_to_camera == 1)
        {
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(1187);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(270);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
            mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            mCurrentMotion = eAbeMotions::Motion_83_WellExpressShotOut_45CF70;
        }
    }
    else
    {
        Motion_83_WellExpressShotOut_45CF70();
        mBaseAnimatedWithPhysicsGameObject_YPos -= mBaseAnimatedWithPhysicsGameObject_VelY * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_WalkToRun);
        mCurrentMotion = eAbeMotions::Motion_79_InsideWellLocal_45CA60;
    }
}

void Abe::Motion_83_WellExpressShotOut_45CF70()
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    Path_TLV* pTlvIter = nullptr;
    Path_WellBase* pWell = nullptr;
    for (;;)
    {
        pTlvIter = sPath_dword_BB47C0->TlvGetAt(
            pTlvIter,
            FP_FromInteger(camPos.x),
            FP_FromInteger(camPos.y),
            FP_FromInteger(camPos.x + 368),
            FP_FromInteger(camPos.y + 240));

        // At the end, exit.
        if (!pTlvIter)
        {
            break;
        }

        // Is it a well?
        if (pTlvIter->mTlvType32 == TlvTypes::LocalWell_8 || pTlvIter->mTlvType32 == TlvTypes::WellExpress_23)
        {
            // Is it the target of the previous well?
            Path_WellBase* pWellBase = static_cast<Path_WellBase*>(pTlvIter);
            if (pWellBase->field_4_other_well_id == field_1A0_door_id)
            {
                pWell = pWellBase;
                break;
            }
        }
    }

    mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
    mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;

    if (pWell)
    {
        if (pWell->field_0_scale == Scale_short::eHalf_1)
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
            mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pWell->mTopLeft.x + pWell->mBottomRight.x) / 2);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pWell->mBottomRight.y);

        BaseAliveGameObjectPathTLV = pWell;

        field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_WalkToRun);
        mCurrentMotion = eAbeMotions::Motion_79_InsideWellLocal_45CA60;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camPos.x + 184);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(camPos.y + 80);
        mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-2.68);
        mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-15);
    }
}

void Abe::Motion_84_FallLandDie_45A420()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        SfxPlayMono(SoundEffect::KillEffect_64, 85);
        SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 95, 95);
        relive_new ScreenShake(true, false);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<s32>(sGnFrame) >= field_124_timer)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::jMotion_85_Fall_455070()
{
    Motion_3_Fall_459B60();
}

ALIVE_VAR(1, 0x5c2c68, s32, sHandstoneSoundChannels_5C2C68, 0);

void Abe::Motion_86_HandstoneBegin_45BD00()
{
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(field_14C_circular_fade_id));
    DeathFadeOut* pFade = static_cast<DeathFadeOut*>(sObjectIds.Find_Impl(field_148_fade_obj_id));

    switch (field_120_state.stone)
    {
        case StoneStates::eHandstoneBegin_0:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                // Add ref
                ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, TRUE, 0);

                CircularFade* pCircularFade2 = Make_Circular_Fade_4CE8C0(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    1,
                    0,
                    0);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    pCircularFade2->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    pCircularFade2->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                }

                field_14C_circular_fade_id = pCircularFade2->field_8_object_id;
                field_120_state.stone = StoneStates::eGetHandstoneType_1;

                SfxPlayMono(SoundEffect::IngameTransition_84, 90);

                BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                    TlvTypes::MovieHandStone_27);

                sHandstoneSoundChannels_5C2C68 = SFX_Play_Pitch(SoundEffect::HandstoneTransition_12, 127, -300);

                s32 switch_id = 0;
                Path_MovieStone* pMovieStoneTlv = static_cast<Path_MovieStone*>(BaseAliveGameObjectPathTLV);
                if (!pMovieStoneTlv)
                {
                    Path_HandStone* pHandStoneTlv = static_cast<Path_HandStone*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                        TlvTypes::HandStone_61));

                    BaseAliveGameObjectPathTLV = pHandStoneTlv;

                    if (pHandStoneTlv)
                    {
                        switch_id = pHandStoneTlv->field_18_trigger_switch_id;

                        field_184_fmv_id = static_cast<s16>(pHandStoneTlv->field_10_scale); // TODO: Never used for this type?

                        field_186_to_camera_id[0] = pHandStoneTlv->field_12_camera_id1;
                        field_186_to_camera_id[1] = pHandStoneTlv->field_12_camera_id2;
                        field_186_to_camera_id[2] = pHandStoneTlv->field_12_camera_id3;

                        field_18C_unused = static_cast<s16>(pHandStoneTlv->field_18_trigger_switch_id); // TODO: Never used?
                    }
                }
                else
                {
                    switch_id = pMovieStoneTlv->field_14_trigger_switch_id;

                    field_184_fmv_id = pMovieStoneTlv->field_10_movie_number;
                    field_186_to_camera_id[0] = static_cast<s16>(pMovieStoneTlv->field_12_scale); // TODO: Never used?
                    field_186_to_camera_id[1] = static_cast<s16>(pMovieStoneTlv->field_14_trigger_switch_id);    // TODO: Never used?
                }

                if (BaseAliveGameObjectPathTLV)
                {
                    if (switch_id > 1)
                    {
                        SwitchStates_Set(static_cast<s16>(switch_id), 1);
                    }

                    field_180_hand_stone_type = BaseAliveGameObjectPathTLV->mTlvType32.mType;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_87_HandstoneEnd_45C4F0;
                }
            }
            break;

        case StoneStates::eGetHandstoneType_1:
            if (pCircularFade->VDone())
            {
                if (field_180_hand_stone_type == TlvTypes::MovieHandStone_27)
                {
                    pScreenManager->EnableRendering();

                    FmvInfo* pFmvRec = Path_Get_FMV_Record(gMap.mCurrentLevel, field_184_fmv_id);
                    u32 pos = 0;

                    Get_fmvs_sectors(pFmvRec->field_0_pName, 0, 0, &pos, 0, 0);
                    sLevelId_dword_5CA408 = static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel));

                    relive_new Movie(pFmvRec->field_4_id, pos, static_cast<s16>(pFmvRec->field_6_flags & 1), static_cast<s16>(pFmvRec->field_8_flags), pFmvRec->field_A_volume);
                    field_120_state.stone = StoneStates::eHandstoneMovieDone_2;
                }
                else if (field_180_hand_stone_type == TlvTypes::HandStone_61)
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                    field_17C_cam_idx = 1;
                    field_120_state.stone = StoneStates::eWaitForInput_4;
                    pCircularFade->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    field_14C_circular_fade_id = -1;
                    DeathFadeOut* pFade33 = relive_new DeathFadeOut(Layer::eLayer_FadeFlash_40, 0, 0, 8, TPageAbr::eBlend_2);
                    if (pFade33)
                    {
                        field_148_fade_obj_id = pFade33->field_8_object_id;
                    }

                    field_19E_to_camera = gMap.mCurrentCamera;
                    gMap.SetActiveCam(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, field_186_to_camera_id[0], CameraSwapEffects::eInstantChange_0, 0, 0);
                }
            }
            break;

        case StoneStates::eHandstoneMovieDone_2:
            if (sMovie_ref_count_BB4AE4 == 0)
            {
                gPsxDisplay.PutCurrentDispEnv();
                pScreenManager->DecompressCameraToVRam((u16**) gMap.field_2C_camera_array[0]->field_C_pCamRes);
                pScreenManager->EnableRendering();
                pCircularFade->VFadeIn(0, 0);
                field_120_state.stone = StoneStates::eHandstoneEnd_3;
            }
            break;

        case StoneStates::eHandstoneEnd_3:
            if (pCircularFade->VDone())
            {
                pCircularFade->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                field_14C_circular_fade_id = -1;

                mCurrentMotion = eAbeMotions::Motion_87_HandstoneEnd_45C4F0;

                if (sHandstoneSoundChannels_5C2C68)
                {
                    SND_Stop_Channels_Mask(sHandstoneSoundChannels_5C2C68);
                    sHandstoneSoundChannels_5C2C68 = 0;
                }

                u8** ppResToFree = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, 0, 0);
                ResourceManager::FreeResource_49C330(ppResToFree);
            }
            break;

        case StoneStates::eWaitForInput_4:
            if (pFade->field_7E_bDone)
            {
                if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & 0x300000)
                {
                    pFade->Init(Layer::eLayer_FadeFlash_40, 1, 0, 8);
                    field_120_state.stone = StoneStates::eCamChangeTransition_5;
                    SfxPlayMono(SoundEffect::IngameTransition_84, 90);
                }
            }
            break;

        case StoneStates::eCamChangeTransition_5:
            if (pFade->field_7E_bDone)
            {
                if (field_17C_cam_idx < 3 && field_186_to_camera_id[field_17C_cam_idx] != 0)
                {
                    field_120_state.stone = StoneStates::eWaitForInput_4;

                    pFade->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    pFade = relive_new DeathFadeOut(Layer::eLayer_FadeFlash_40, 0, 0, 8, TPageAbr::eBlend_2);
                    if (pFade)
                    {
                        field_148_fade_obj_id = pFade->field_8_object_id;
                    }

                    gMap.SetActiveCam(
                        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                        mBaseAnimatedWithPhysicsGameObject_PathNumber,
                        field_186_to_camera_id[field_17C_cam_idx++],
                        CameraSwapEffects::eInstantChange_0,
                        0,
                        0);
                }
                else
                {
                    field_120_state.stone = StoneStates::eSetActiveCamToAbe_6;
                }
            }
            break;

        case StoneStates::eSetActiveCamToAbe_6:
            if (pFade->field_7E_bDone)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                field_120_state.stone = StoneStates::eCircularFadeExit_7;
                gMap.SetActiveCam(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    field_19E_to_camera,
                    CameraSwapEffects::eInstantChange_0,
                    0,
                    0);
            }
            break;

        case StoneStates::eCircularFadeExit_7:
        {
            pFade->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_148_fade_obj_id = -1;

            CircularFade* pCircularFade2 = Make_Circular_Fade_4CE8C0(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0, 0, 0);
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                pCircularFade2->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
            else
            {
                pCircularFade2->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            }

            field_14C_circular_fade_id = pCircularFade2->field_8_object_id;
            field_120_state.stone = StoneStates::eHandstoneEnd_3;

            if (sHandstoneSoundChannels_5C2C68)
            {
                SND_Stop_Channels_Mask(sHandstoneSoundChannels_5C2C68);
                sHandstoneSoundChannels_5C2C68 = 0;
            }
        }
        break;

        default:
            return;
    }
}

void Abe::Motion_87_HandstoneEnd_45C4F0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_88_GrenadeMachineUse_45C510()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_89_BrewMachineBegin_4584C0()
{
    if (field_120_state.raw > 0)
    {
        if (field_120_state.raw <= 36u)
        {
            if (field_120_state.raw > 11u && !((field_120_state.raw - 12) % 6))
            {
                SFX_Play_Pitch(SoundEffect::BrewMachineUseEnd_119, 0, 32 * field_120_state.raw);
            }
            field_120_state.raw++;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_90_BrewMachineEnd_4585B0;
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 8)
    {
        if (GetEvilFart_4585F0(FALSE))
        {
            SfxPlayMono(SoundEffect::BrewMachineUseStart_116, 0);
        }
        else
        {
            SfxPlayMono(SoundEffect::BrewMachineUseEmpty_118, 0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (GetEvilFart_4585F0(TRUE))
        {
            SfxPlayMono(SoundEffect::BrewMachineUseMid_117, 0);
            field_120_state.raw = 1;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_90_BrewMachineEnd_4585B0;
        }
    }
}

void Abe::Motion_90_BrewMachineEnd_4585B0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        field_124_timer = 1;
    }
}

// Let go of an edge/hoist/pull ring
void Abe::Motion_91_FallingFromGrab_4557B0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
    }

    Motion_3_Fall_459B60();

    if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420)
    {
        mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
    }

    mPreviousMotion = eAbeMotions::Motion_91_FallingFromGrab_4557B0;
}

void Abe::Motion_92_ForceDownFromHoist_455800()
{
    if (!field_124_timer)
    {
        mShadow->field_14_flags.Clear(Shadow::eBit1_ShadowAtBottom);
        VOnTrapDoorOpen();
        FP hitX = {};
        FP hitY = {};
        if (sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75)),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10),
                &BaseAliveGameObjectCollisionLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
            == 1)
        {
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            mCurrentMotion = eAbeMotions::Motion_84_FallLandDie_45A420;
            field_128.field_4_regen_health_timer = sGnFrame + 900;
            mPreviousMotion = eAbeMotions::Motion_3_Fall_459B60;
            return;
        }
        mBaseAnimatedWithPhysicsGameObject_YPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75));
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        BaseAliveGameObjectCollisionLine = nullptr;
        field_124_timer = field_124_timer + 1;
    }
    Motion_3_Fall_459B60();
}

void Abe::Motion_93_WalkOffEdge_455970()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
    }
    Motion_3_Fall_459B60();
}

void Abe::Motion_94_RunOffEdge_4559A0()
{
    Motion_93_WalkOffEdge_455970();
}

void Abe::Motion_95_SneakOffEdge_4559C0()
{
    Motion_93_WalkOffEdge_455970();
}

void Abe::Motion_96_HopToFall_4559E0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5));
    }

    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(4));
    Motion_93_WalkOffEdge_455970();
}

void Abe::Motion_97_RunJumpToFall_455A80()
{
    Motion_93_WalkOffEdge_455970();
}

void Abe::Motion_98_RollOffEdge_455AA0()
{
    Motion_93_WalkOffEdge_455970();
}

void Abe::Motion_99_LeverUse_455AC0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
        {
            mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::Motion_100_SlapBomb_455B60()
{
    BaseAliveGameObject* pItem = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_160_slappable_or_pick_item_id));
    if (sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 6)
    {
        if (pItem)
        {
            pItem->VOnPickUpOrSlapped();
            field_160_slappable_or_pick_item_id = -1;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }
}

void Abe::Motion_101_KnockForward_455420()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 12)
    {
        FallOnBombs_44EC10();
    }

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
            MoveWithVelocity_450FA0(FP_FromDouble(0.7));
        }
        else
        {
            Motion_3_Fall_459B60();
            if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420 || mCurrentMotion == eAbeMotions::Motion_16_LandSoft_45A360)
            {
                mCurrentMotion = eAbeMotions::Motion_101_KnockForward_455420;
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2) && (BaseAliveGameObjectCollisionLine || !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render)))
        {
            if (mHealth > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)
            {
                mCurrentMotion = eAbeMotions::jMotion_103_KnockForwardGetUp_455380;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::Motion_102_RollingKnockForward_455310()
{
    Motion_74_RollingKnockback_455290();
    if (mCurrentMotion == eAbeMotions::Motion_72_KnockbackGetUp_455340)
    {
        mCurrentMotion = eAbeMotions::jMotion_103_KnockForwardGetUp_455380;
    }
}

void Abe::jMotion_103_KnockForwardGetUp_455380()
{
    Motion_72_KnockbackGetUp_455340();
}

void Abe::Motion_104_RockThrowStandingHold_455DF0()
{
    auto pRock = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(field_158_throwable_id));
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 4)
    {
        if (Input().isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0 | sInputKey_Up_5550D8 | sInputKey_Down_5550DC))
        {
            if (Input().isPressed(sInputKey_Right_5550D0))
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_1A3_throw_direction = 0;
                }
                else
                {
                    field_1A3_throw_direction = 2;
                }
            }
            else if (Input().isPressed(sInputKey_Left_5550D4))
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_1A3_throw_direction = 2;
                }
                else
                {
                    field_1A3_throw_direction = 0;
                }
            }
            else if (Input().isPressed(sInputKey_Up_5550D8))
            {
                field_1A3_throw_direction = 1;
            }
            else
            {
                // Down
                field_1A3_throw_direction = 3;
            }
            mCurrentMotion = eAbeMotions::Motion_105_RockThrowStandingThrow_456460;
        }
    }

    if (!Input().isPressed(sInputKey_ThrowItem_5550F4)) // ?? isn't released like in the crouching motion ??
    {
        pRock->VToDead();
        field_158_throwable_id = -1;
        mCurrentMotion = eAbeMotions::Motion_106_RockThrowStandingEnd_455F20;
        if (!gInfiniteGrenades_5C1BDE)
        {
            field_1A2_throwable_count++;
        }
    }
}

void Abe::Motion_105_RockThrowStandingThrow_456460()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        SfxPlayMono(SoundEffect::AirStream_23, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_106_RockThrowStandingEnd_455F20()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_107_RockThrowCrouchingHold_454410()
{
    auto pRock = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(field_158_throwable_id));
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 4)
    {
        if (Input().isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0 | sInputKey_Up_5550D8 | sInputKey_Down_5550DC))
        {
            field_1A3_throw_direction = 4;
            mCurrentMotion = eAbeMotions::Motion_108_RockThrowCrouchingThrow_454500;
            if (pRock->Type() == ReliveTypes::eMeat)
            {
                field_1A3_throw_direction = 5;
            }
        }
    }

    if (Input().IsReleased(sInputKey_ThrowItem_5550F4))
    {
        pRock->VToDead();
        field_158_throwable_id = -1;
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
        if (!gInfiniteGrenades_5C1BDE)
        {
            field_1A2_throwable_count++;
        }
    }
}

void Abe::Motion_108_RockThrowCrouchingThrow_454500()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        SfxPlayMono(SoundEffect::AirStream_23, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }
}

void Abe::Motion_109_ZShotRolling_455550()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
    Motion_3_Fall_459B60();

    if (mCurrentMotion != eAbeMotions::Motion_109_ZShotRolling_455550 && !gAbeBulletProof_5C1BDA)
    {
        if (BaseAliveGameObject_PlatformId != -1)
        {
            VOnTrapDoorOpen();
        }
        mCurrentMotion = eAbeMotions::Motion_109_ZShotRolling_455550;
        BaseAliveGameObjectCollisionLine = nullptr;
        mBaseAnimatedWithPhysicsGameObject_YPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            if (!mBaseAliveGameObjectFlags.Get(Flags_114::e114_MotionChanged_Bit2))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos += FP_FromInteger(240);
                Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
                ToDie_4588D0();
            }
        }
    }
}

void Abe::Motion_110_ZShot_455670()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
    Motion_3_Fall_459B60();

    if (mCurrentMotion != eAbeMotions::Motion_110_ZShot_455670 && !gAbeBulletProof_5C1BDA)
    {
        if (BaseAliveGameObject_PlatformId != -1)
        {
            BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                static_cast<LiftPoint*>(pLiftPoint)->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
            }
            VOnTrapDoorOpen();
        }
        mCurrentMotion = eAbeMotions::Motion_110_ZShot_455670;
        BaseAliveGameObjectCollisionLine = nullptr;
        mBaseAnimatedWithPhysicsGameObject_YPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos += FP_FromInteger(240);
        Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
        ToDie_4588D0();
    }
}

void Abe::Motion_111_PickupItem_4564A0()
{
    auto pRock = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_160_slappable_or_pick_item_id));

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
    {
        SfxPlayMono(SoundEffect::PickupItem_28, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }

    if (sActiveHero->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 5)
    {
        if (pRock)
        {
            pRock->VOnPickUpOrSlapped();
            field_160_slappable_or_pick_item_id = -1;
        }
    }
}

void Abe::Motion_112_Chant_45B1C0()
{
    BaseAliveGameObject* pPossessTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_154_possessed_object_id));
    OrbWhirlWind* pOrbWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(field_150_OrbWhirlWind_id));

    if (field_120_state.chant != ChantStates::eWaitForUnpossessing_3 && field_120_state.chant != ChantStates::eUnpossessing_4)
    {
        SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 0);
    }

    if (!pOrbWhirlWind)
    {
        field_150_OrbWhirlWind_id = -1;
    }

    switch (field_120_state.chant)
    {
        case ChantStates::eIdleChanting_0:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);

            BaseAliveGameObject* pObj = FindObjectToPossess_44B7B0(); // Find a victim.

            if (field_168_ring_pulse_timer)
            {
                if (!field_16C_bHaveShrykull && !field_16E_bHaveInvisiblity && !(field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing)))
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    AbilityRing::Factory(
                        FP_FromInteger((bRect.x + bRect.w) / 2),
                        FP_FromInteger((bRect.y + bRect.h) / 2),
                        RingTypes::eExplosive_Emit_1,
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                    field_168_ring_pulse_timer = 0;
                }
            }

            if (field_168_ring_pulse_timer)
            {
                if (field_16E_bHaveInvisiblity)
                {
                    field_168_ring_pulse_timer = 0;
                    field_16E_bHaveInvisiblity = 0;

                    if (field_170_invisible_timer)
                    {
                        field_170_invisible_timer += field_176_invisibility_duration;
                    }
                    else
                    {
                        field_170_invisible_timer = sGnFrame + field_176_invisibility_duration;
                    }

                    field_174_unused = 0;

                    InvisibleEffect* pInvisible = static_cast<InvisibleEffect*>(sObjectIds.Find_Impl(field_178_invisible_effect_id));
                    if (!pInvisible || pInvisible->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                    {
                        pInvisible = relive_new InvisibleEffect(this);
                        field_178_invisible_effect_id = pInvisible->field_8_object_id;
                    }
                    pInvisible->BecomeInvisible();
                }

                if (field_168_ring_pulse_timer)
                {
                    if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing))
                    {
                        bool bAliveMudIsInSameScreen = false;
                        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
                        {
                            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                            if (!pObjIter)
                            {
                                break;
                            }

                            if (pObjIter->Type() == ReliveTypes::eMudokon)
                            {
                                if (pObjIter->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit3_Can_Be_Possessed)) // TODO: Is sick flag ?
                                {
                                    if (pObjIter->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObjIter->mHealth > FP_FromInteger(0))
                                    {
                                        bAliveMudIsInSameScreen = true;
                                    }
                                }
                            }
                        }

                        if (bAliveMudIsInSameScreen)
                        {
                            const PSX_RECT bRect = VGetBoundingRect();

                            AbilityRing::Factory(
                                FP_FromInteger((bRect.x + bRect.w) / 2),
                                FP_FromInteger((bRect.y + bRect.h) / 2),
                                RingTypes::eHealing_Emit_12,
                                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit15_have_healing);
                            field_168_ring_pulse_timer = 0;
                        }
                    }
                }
            }

            // Stopped chanting?
            if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame) || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3) && !Input_IsChanting_45F260())
            {
                mCurrentMotion = eAbeMotions::Motion_113_ChantEnd_45BBE0;
                field_154_possessed_object_id = -1;
                if (pOrbWhirlWind)
                {
                    pOrbWhirlWind->ToStop();
                    field_150_OrbWhirlWind_id = -1;
                }
                return;
            }

            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }

            if (static_cast<s32>(sGnFrame) >= field_124_timer - 70)
            {
                if (pObj)
                {
                    if (!pOrbWhirlWind)
                    {
                        const FP yPos = mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(38));
                        const FP xOff = mBaseAnimatedWithPhysicsGameObject_SpriteScale * ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-4) : FP_FromInteger(4));
                        pOrbWhirlWind = relive_new OrbWhirlWind(
                            xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                            yPos,
                            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                            0);

                        field_150_OrbWhirlWind_id = pOrbWhirlWind->field_8_object_id;
                    }
                }
                else
                {
                    field_124_timer = sGnFrame + 70;
                    if (pOrbWhirlWind)
                    {
                        pOrbWhirlWind->ToStop();
                        pOrbWhirlWind = nullptr;
                        field_150_OrbWhirlWind_id = -1;
                    }
                }
            }


            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }

            if (!pObj)
            {
                return;
            }

            field_154_possessed_object_id = pObj->field_8_object_id;
            SFX_Play_Pitch(SoundEffect::PossessEffect_17, 0, -600);
            field_120_state.chant = ChantStates::ePossessVictim_1;
            field_124_timer = sGnFrame + 30;

            const PSX_RECT bRect = pObj->VGetBoundingRect();
            pOrbWhirlWind->ToSpin(
                FP_FromInteger((bRect.w - bRect.x) / 2),
                FP_FromInteger((bRect.h - bRect.y) / 2),
                pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                pObj);

            relive_new PossessionFlicker(sActiveHero, 30, 128, 255, 255);
        }
            return;

        case ChantStates::ePossessVictim_1:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);

            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                if (!pPossessTarget || pPossessTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || pPossessTarget->mHealth <= FP_FromInteger(0) || pPossessTarget->Is_In_Current_Camera() != CameraPos::eCamCurrent_0)
                {
                    mCurrentMotion = eAbeMotions::Motion_113_ChantEnd_45BBE0;
                    field_154_possessed_object_id = -1;
                    if (pOrbWhirlWind)
                    {
                        pOrbWhirlWind->ToStop();
                        field_150_OrbWhirlWind_id = -1;
                    }
                }
            }
            else
            {
                field_120_state.chant = ChantStates::ePossessedVictim_2;
            }
        }
            return;

        case ChantStates::ePossessedVictim_2:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            field_150_OrbWhirlWind_id = -1;

            if (!pPossessTarget)
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    mCurrentMotion = eAbeMotions::Motion_113_ChantEnd_45BBE0;
                }
                return;
            }

            if (pPossessTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_154_possessed_object_id = -1;
            }

            if (pPossessTarget->mHealth <= FP_FromInteger(0))
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    mCurrentMotion = eAbeMotions::Motion_113_ChantEnd_45BBE0;
                }
                return;
            }

            sControlledCharacter_5C1B8C = pPossessTarget;

            pPossessTarget->VPossessed();

            field_154_possessed_object_id = -1;

            if (sControlledCharacter_5C1B8C->Type() == ReliveTypes::eSlig || sControlledCharacter_5C1B8C->Type() == ReliveTypes::eFlyingSlig || sControlledCharacter_5C1B8C->Type() == ReliveTypes::eCrawlingSlig || sControlledCharacter_5C1B8C->Type() == ReliveTypes::eGlukkon)
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_laugh_at_chant_end);
            }

            relive_new PossessionFlicker(sControlledCharacter_5C1B8C, 60, 128, 255, 255);

            SND_SEQ_Stop(SeqId::MudokonChant1_10);
            SFX_Play_Pitch(SoundEffect::PossessEffect_17, 70, 400);
            field_120_state.chant = ChantStates::eWaitForUnpossessing_3;
        }
            return;

        case ChantStates::eWaitForUnpossessing_3:
        {
            if (sControlledCharacter_5C1B8C != this)
            {
                return;
            }

            relive_new PossessionFlicker(sControlledCharacter_5C1B8C, 15, 128, 255, 255);

            field_120_state.chant = ChantStates::eUnpossessing_4;
            field_124_timer = sGnFrame + 15;
        }
            return;

        case ChantStates::eUnpossessing_4:
        {
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }

            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mCurrentMotion = eAbeMotions::Motion_113_ChantEnd_45BBE0;
            }
        }
            return;

        case ChantStates::eChantingForBirdPortal_6:
        {
            EventBroadcast(kEventSpeaking, this);
            EventBroadcast(kEventAbeOhm, this);
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }
        }
            return;

        default:
            return;
    }
}

void Abe::Motion_113_ChantEnd_45BBE0()
{
    SND_SEQ_Stop(SeqId::MudokonChant1_10);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit9_laugh_at_chant_end))
        {
            mCurrentMotion = eAbeMotions::Motion_9_Speak_45B180;
            Mudokon_SFX(MudSounds::eGiggle_8, 0, 0, this);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit9_laugh_at_chant_end);
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::Motion_114_DoorEnter_459470()
{
    switch (field_120_state.door)
    {
        case AbeDoorStates::eAbeComesIn_0:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_120_state.door = AbeDoorStates::eWaitABit_2;
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                field_128.field_0_abe_timer = sGnFrame + 3;
            }
            return;

        case AbeDoorStates::eWaitABit_2:
            if (field_128.field_0_abe_timer <= static_cast<s32>(sGnFrame))
            {
                field_120_state.door = AbeDoorStates::eToState4_3;
                field_128.field_0_abe_timer = sGnFrame + 3;
            }
            return;

        case AbeDoorStates::eToState4_3:
            if (field_128.field_0_abe_timer <= static_cast<s32>(sGnFrame))
            {
                field_120_state.door = AbeDoorStates::eSetNewActiveCamera_4;
            }
            return;

        case AbeDoorStates::eSetNewActiveCamera_4:
        {
            Path_Door* pDoorTlv = static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::Door_5));

            BaseAliveGameObjectPathTLV = pDoorTlv;

            if (pDoorTlv->field_42_clear_throwables == Choice_short::eYes_1)
            {
                if (field_1A2_throwable_count > 0 && gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C->Remove(field_1A2_throwable_count);
                    field_1A2_throwable_count = 0;
                }
            }

            // An OWI hack. When both Mudomo and Mundanchee are done, force back to Necrum Mines.
            bool hackChange = false;
            if (gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender)
            {
                if (gMap.mCurrentPath == 13 && gMap.mCurrentCamera == 14 && field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender))
                {
                    hackChange = true;
                }
            }
            else if (gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
            {
                if (gMap.mCurrentPath == 11 && gMap.mCurrentCamera == 2 && field_1AE_flags.Get(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender))
                {
                    hackChange = true;
                }
            }

            if (hackChange)
            {
                // Plays FMV where the weirdos give Abe the drunk mud healing power and then dumps Abe at the portal that leads
                // back to Necrum mines.
                gMap.SetActiveCam(EReliveLevelIds::eNecrum, 3, 10, CameraSwapEffects::ePlay1FMV_5, 22, 0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(2287);
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(800);
                field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
                mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
                return;
            }

            gMap.mDoorTransition = 1;
            s16 bForceChange = 0;
            const CameraSwapEffects effect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[pDoorTlv->field_32_wipe_effect];
            if (effect == CameraSwapEffects::ePlay1FMV_5 || effect == CameraSwapEffects::eUnknown_11)
            {
                bForceChange = 1;
            }

            gMap.SetActiveCam(
                MapWrapper::FromAE(pDoorTlv->field_10_level),
                pDoorTlv->field_12_path,
                pDoorTlv->field_14_camera,
                effect,
                pDoorTlv->field_34_movie_number,
                bForceChange);

            field_120_state.door = AbeDoorStates::eSetNewAbePosition_5;
            field_1A0_door_id = pDoorTlv->field_1C_target_door_id;
        }
            return;

        case AbeDoorStates::eSetNewAbePosition_5:
        {
            gMap.mDoorTransition = 0;
            mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
            mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;

            Path_Door* pDoorTlv2 = static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera(TlvTypes::Door_5, 0));
            BaseAliveGameObjectPathTLV = pDoorTlv2;
            Path_Door* pTargetDoorTlv = pDoorTlv2;
            if (pTargetDoorTlv->field_18_door_number != field_1A0_door_id)
            {
                do
                {
                    Path_Door* pDoorIter = static_cast<Path_Door*>(Path::TLV_Next_Of_Type(BaseAliveGameObjectPathTLV, TlvTypes::Door_5));
                    BaseAliveGameObjectPathTLV = pDoorIter;
                    pTargetDoorTlv = pDoorIter;

                    if (!pTargetDoorTlv)
                    {
                        ALIVE_FATAL("Couldn't find target door. If this is a custom level, check if the level, path and camera is correct.");
                    }
                }
                while (pTargetDoorTlv->field_18_door_number != field_1A0_door_id);
            }

            if (pTargetDoorTlv->field_16_scale == Scale_short::eHalf_1)
            {
                mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(1.0);
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
            }

            // The door controls which way Abe faces when he exits it.
            if (pTargetDoorTlv->field_3E_abe_direction == XDirection_short::eRight_1)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            }

            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.x) + FP_FromInteger((BaseAliveGameObjectPathTLV->mBottomRight.x - BaseAliveGameObjectPathTLV->mTopLeft.x) / 2);


            MapFollowMe(TRUE);

            PathLine* pathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y),
                    &pathLine,
                    &hitX,
                    &hitY,
                    (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
            {
                BaseAliveGameObjectCollisionLine = pathLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
                field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y);
                BaseAliveGameObjectLastLineYPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y);
            }

            field_168_ring_pulse_timer = 0;
            InvisibleEffect* pInvisibleEffect = static_cast<InvisibleEffect*>(sObjectIds.Find_Impl(field_178_invisible_effect_id));
            if (pInvisibleEffect)
            {
                if (!(pInvisibleEffect->mBaseGameObjectFlags.Get(BaseGameObject::eDead)))
                {
                    pInvisibleEffect->ClearInvisibility();
                    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit8_bInvisible);
                    field_178_invisible_effect_id = -1;
                    field_170_invisible_timer = 0;
                }
            }

            mHealth = FP_FromInteger(1);
            field_120_state.door = AbeDoorStates::eAbeComesOut_6;
            field_128.field_0_abe_timer = sGnFrame + 30;
        }
            return;

        case AbeDoorStates::eAbeComesOut_6:
            if (field_128.field_0_abe_timer > static_cast<s32>(sGnFrame))
            {
                return;
            }

            field_120_state.door = AbeDoorStates::eAbeComesIn_0;
            if (BaseAliveGameObjectCollisionLine)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                mCurrentMotion = eAbeMotions::Motion_115_DoorExit_459A40;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                // Abe opens the door and he falls through the floor.
                mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
            }
            return;

        default:
            return;
    }
}

void Abe::Motion_115_DoorExit_459A40()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Now that we've exited the door, should it close behind us?
        if (static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                                        TlvTypes::Door_5))
                ->field_40_close_on_exit == Choice_short::eYes_1)
        {
            // TODO: Ret ignored even in real ??
            FindObjectOfType(
                ReliveTypes::eDoor,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5));

            // Yes, so find it
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == ReliveTypes::eDoor)
                {
                    Door* pDoor = static_cast<Door*>(pObj);
                    if (pDoor->field_FA_door_number == field_1A0_door_id)
                    {
                        // And close it
                        pDoor->vClose();
                        break;
                    }
                }
            }
        }

        BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);
        ToIdle_44E6B0();
    }
}

void Abe::Motion_116_MineCarEnter_458780()
{
    if (field_120_state.raw == 0)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            field_120_state.raw = 1;
            mCurrentMotion = eAbeMotions::Motion_117_InMineCar_4587C0;
        }
    }
}

void Abe::Motion_117_InMineCar_4587C0()
{
    if (Input().isPressed(sInputKey_DoAction_5550E4))
    {
        auto pMineCar = static_cast<MineCar*>(sControlledCharacter_5C1B8C);
        if (pMineCar->field_11C_state == MineCarStates::eParkedWithAbe_1 && pMineCar->field_1BC_turn_direction == MineCarDirs::eUp_3)
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(2),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(2),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

                mCurrentMotion = eAbeMotions::Motion_118_MineCarExit_458890;
            }
        }
    }
}

void Abe::Motion_118_MineCarExit_458890()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
        mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        field_124_timer = 1;
    }
}

void Abe::Motion_119_ToShrykull_45A990()
{
    if (field_120_state.raw == 0)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);

            field_120_state.raw = 1;

            relive_new Shrykull();
        }
    }
}

void Abe::Motion_120_EndShrykull_45AB00()
{
    if (field_124_timer)
    {
        field_124_timer = field_124_timer - 1;
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_168_ring_pulse_timer == 0)
        {
            Free_Shrykull_Resources_45AA90();
        }

        // Abe finds transforming into a god rather funny.
        mCurrentMotion = eAbeMotions::Motion_9_Speak_45B180;
        Mudokon_SFX(MudSounds::eGiggle_8, 0, 0, this);
    }
}

void Abe::Motion_121_LiftGrabBegin_45A600()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eAbeMotions::Motion_123_LiftGrabIdle_45A6A0;
    }
}

void Abe::Motion_122_LiftGrabEnd_45A670()
{
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_123_LiftGrabIdle_45A6A0()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    FollowLift_45A500();

    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed)
    {
        if (!pLiftPoint->vOnTopFloor())
        {
            mCurrentMotion = eAbeMotions::Motion_124_LiftUseUp_45A780;
        }
    }
    else if (pressed & sInputKey_Down_5550DC)
    {
        if (!pLiftPoint->vOnBottomFloor())
        {
            mCurrentMotion = eAbeMotions::Motion_125_LiftUseDown_45A7B0;
        }
    }
    else if (pLiftPoint->vOnAnyFloor())
    {
        // You ain't letting go unless you are on a floor where you can walk off.
        mCurrentMotion = eAbeMotions::Motion_122_LiftGrabEnd_45A670;
    }
}

void Abe::Motion_124_LiftUseUp_45A780()
{
    mCurrentMotion = MoveLiftUpOrDown_45A7E0(FP_FromInteger(-4));
}

void Abe::Motion_125_LiftUseDown_45A7B0()
{
    mCurrentMotion = MoveLiftUpOrDown_45A7E0(FP_FromInteger(4));
}

void Abe::Motion_126_TurnWheelBegin_456700()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_164_wheel_id));
        if (pWheel)
        {
            pWheel->VStartTurning();
        }
        mCurrentMotion = eAbeMotions::Motion_127_TurnWheelLoop_456750;
        field_120_state.wheel = WorkWheelStates::eTurningWheel_0;
    }
}

void Abe::Motion_127_TurnWheelLoop_456750()
{
    if (field_120_state.wheel == WorkWheelStates::eTurningWheel_0 || field_120_state.wheel == WorkWheelStates::eCheckForNoLongerTurningWheel_1) // The state we enter the main state at.
    {
        Path_LevelLoader* pLevelLoader = static_cast<Path_LevelLoader*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera(TlvTypes::LevelLoader_86, 0));
        if (pLevelLoader && SwitchStates_Get(pLevelLoader->field_10_switch_id))
        {
            field_120_state.wheel = WorkWheelStates::eMapChanging_2;
            SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            relive_new MusicTrigger(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 0);
            return;
        }
        else
        {
            // Must ALSO do logic below in this instance.
            field_120_state.wheel = WorkWheelStates::eCheckForNoLongerTurningWheel_1;
        }
    }

    if (field_120_state.wheel == WorkWheelStates::eCheckForNoLongerTurningWheel_1)
    {
        if (!(Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Up_5550D8))
        {
            // Not holding up anymore, stop.
            WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_164_wheel_id));
            if (pWheel)
            {
                pWheel->VStopTurning(1);
            }
            field_164_wheel_id = -1;
            mCurrentMotion = eAbeMotions::Motion_128_TurnWheelEnd_4569A0;
        }
    }
    else if (field_120_state.wheel == WorkWheelStates::eMapChanging_2)
    {
        // This happens for the Mines Tunnel 1 ender.
        if (!gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            // When we've changed from the camera with the wheels to tunnel 2 this forces the falling state into the well.
            // Another tasty OWI hack.
            auto pWorkWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(field_164_wheel_id));
            if (pWorkWheel) // Most likely always nullptr here, maybe the whole "stop wheel" was an inlined function.
            {
                pWorkWheel->VStopTurning(1);
            }
            field_164_wheel_id = -1;

            auto pPathAbeStart = static_cast<Path_AbeStart*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera(TlvTypes::AbeStart_22, 0));
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pPathAbeStart->mTopLeft.x + pPathAbeStart->mBottomRight.x) / 2);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pPathAbeStart->mBottomRight.y);
            BaseAliveGameObjectLastLineYPos = FP_FromInteger(pPathAbeStart->mBottomRight.y);
            // TODO: OG bug, scale not read from the TLV ??

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
        }
    }
}

void Abe::Motion_128_TurnWheelEnd_4569A0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_129_PoisonGasDeath_4565C0()
{
    // Play various choke/cough/death sounds as the dying animation progresses.
    switch (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
    {
        case 0:
            SFX_Play_Pitch(SoundEffect::Choke_81, 127, 128);
            break;
        case 9:
            SFX_Play_Pitch(SoundEffect::Choke_81, 127, 384);
            break;
        case 28:
            SFX_Play_Pitch(SoundEffect::Choke_81, 127, 640);
            break;
        case 32:
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, 0, this);
            break;
        case 50:
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 100, -200, this);
            break;
        case 53:
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 50, -200, this);
            break;
        default:
            break;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        const s32 previousValue = field_124_timer;
        field_124_timer = previousValue - 1;
        if (previousValue == 0)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::jMotion_81_WellBegin_4017F8()
{
    Motion_78_WellBegin_45C810();
}

void Abe::FleechDeath_459350()
{
    BaseGameObject* pInvisibleEffect = sObjectIds.Find_Impl(field_178_invisible_effect_id);
    if (pInvisibleEffect)
    {
        if (!pInvisibleEffect->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            static_cast<InvisibleEffect*>(pInvisibleEffect)->ClearInvisibility();
            field_178_invisible_effect_id = -1;
        }
    }
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel);
    mCurrentMotion = eAbeMotions::Motion_58_DeadPre_4593E0;
    field_120_state.raw = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
    BaseAliveGameObjectCollisionLine = nullptr;
}

void Abe::ToDie_4588D0()
{
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel);
    mCurrentMotion = eAbeMotions::Motion_56_DeathDropFall_4591F0;
    field_124_timer = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 1, 0);
}

void Abe::ToIdle_44E6B0()
{
    field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    field_124_timer = sGnFrame;
    mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
    field_118_prev_held = 0;
    field_11C_released_buttons = 0;
    MapFollowMe(TRUE);
}

void Abe::PickUpThrowabe_Or_PressBomb_454090(FP fpX, s32 fpY, s32 bStandToCrouch)
{
    BaseAliveGameObject* pSlappableOrCollectable = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8))
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->mCollectionRect.x && fpX <= pAliveObj->mCollectionRect.w)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->mCollectionRect.y && yPos <= pAliveObj->mCollectionRect.h)
                {
                    pSlappableOrCollectable = pAliveObj;
                    field_160_slappable_or_pick_item_id = pAliveObj->field_8_object_id;
                }
            }
        }
    }

    if (pSlappableOrCollectable)
    {
        bool trySlapOrCollect = false;
        switch (pSlappableOrCollectable->Type())
        {
            case ReliveTypes::eTimedMine_or_MovingBomb:
            case ReliveTypes::eUXB:
                mCurrentMotion = eAbeMotions::Motion_100_SlapBomb_455B60;
                if (bStandToCrouch)
                {
                    field_160_slappable_or_pick_item_id = -1;
                }
                trySlapOrCollect = true;
                break;

            case ReliveTypes::eBone:
            case ReliveTypes::eGrenade:
            case ReliveTypes::eMeat:
            case ReliveTypes::eRock:
                mCurrentMotion = eAbeMotions::Motion_111_PickupItem_4564A0;
                field_1A2_throwable_count += static_cast<s8>(static_cast<BaseThrowable*>(pSlappableOrCollectable)->VGetCount()); // TODO: Check types are correct.
                if (!bThrowableIndicatorExists_5C112C)
                {
                    const FP yoff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-30)) + mBaseAnimatedWithPhysicsGameObject_YPos;
                    const FP xoff = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(0);
                    relive_new ThrowableTotalIndicator(
                        xoff + mBaseAnimatedWithPhysicsGameObject_XPos,
                        yoff,
                        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
                        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale,
                        field_1A2_throwable_count,
                        1);
                }
                trySlapOrCollect = true;
                break;

            case ReliveTypes::eMine:
                field_160_slappable_or_pick_item_id = -1;
                trySlapOrCollect = true;
                break;
            default:
                break;
        }

        if (trySlapOrCollect)
        {
            if (mCurrentMotion == eAbeMotions::Motion_111_PickupItem_4564A0)
            {
                if (bStandToCrouch)
                {
                    SfxPlayMono(SoundEffect::PickupItem_28, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    pSlappableOrCollectable->VOnPickUpOrSlapped();
                    field_160_slappable_or_pick_item_id = -1;
                    mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
                }
            }
        }
    }
}

void Abe::Get_Shrykull_Resources_45AA20()
{
    field_10_resources_array.SetAt(25, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID, TRUE, FALSE));
    field_10_resources_array.SetAt(26, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kShrmorphResID, TRUE, FALSE));
    field_10_resources_array.SetAt(27, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, TRUE, FALSE));
}

s16 Abe::ToLeftRightMovement_44E340()
{
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    if (sControlledCharacter_5C1B8C != this)
    {
        return 0;
    }

    const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const bool flipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);

    if ((flipX && (pressed & sInputKey_Right_5550D0)) || (!flipX && (pressed & sInputKey_Left_5550D4)))
    {
        mCurrentMotion = eAbeMotions::Motion_2_StandingTurn_451830;
        return 1;
    }

    if ((pressed & sInputKey_Right_5550D0) || (pressed & sInputKey_Left_5550D4))
    {
        const FP directionX = FP_FromInteger((pressed & sInputKey_Right_5550D0) ? 1 : -1);

        if (pressed & sInputKey_Run_5550E8)
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = directionX * (gridSize / FP_FromInteger(4));
            mCurrentMotion = eAbeMotions::Motion_39_StandingToRun_450D40;
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = directionX * (gridSize / FP_FromInteger(10));
            mCurrentMotion = eAbeMotions::Motion_45_SneakBegin_4507A0;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = directionX * (gridSize / FP_FromInteger(9));
            mCurrentMotion = eAbeMotions::Motion_6_WalkBegin_44FEE0;
        }

        if (!WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), directionX * gridSize))
        {
            if (!WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), directionX * gridSize))
            {
                return 1;
            }
        }

        // Walking into wall?
        if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), directionX * gridSize))
        {
            PushWall_44E890();
            return 0;
        }

        mCurrentMotion = eAbeMotions::Motion_19_StandToCrouch_453DC0;
        return 1;
    }

    return 0;
}

void Abe::TryHoist_44ED30()
{
    mCurrentMotion = eAbeMotions::Motion_13_HoistBegin_452B20;

    const FP xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP ypos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(10); // Look up 10 for a hoist.

    Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(xpos),
        FP_GetExponent(ypos),
        FP_GetExponent(xpos),
        FP_GetExponent(ypos),
        TlvTypes::Hoist_2));

    if (pHoist)
    {
        if (IsSameScaleAsHoist(pHoist, this))
        {
            if (!IsFacingSameDirectionAsHoist(pHoist, this) && pHoist->field_12_grab_direction != Path_Hoist::GrabDirection::eFacingAnyDirection)
            {
                // No, so auto turn around to face it.
                mNextMotion = mCurrentMotion;
                mCurrentMotion = eAbeMotions::Motion_2_StandingTurn_451830;
            }
        }

        BaseAliveGameObjectPathTLV = pHoist;
    }
}

void Abe::Create_Fart_421D20()
{
    relive_new EvilFart();
}

s16 Abe::TryEnterMineCar_4569E0()
{
    if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Up_5550D8)
    {
        for (s32 idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                const PSX_RECT abeRect = VGetBoundingRect();
                const PSX_RECT mineCarRect = pObj->VGetBoundingRect();

                if (PSX_Rects_overlap_no_adjustment(&abeRect, &mineCarRect) && pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && pObj->Type() == ReliveTypes::eMineCar)
                {
                    const FP distanceCheck = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(0.5);
                    if (mBaseAnimatedWithPhysicsGameObject_XPos - pObj->mBaseAnimatedWithPhysicsGameObject_XPos < distanceCheck)
                    {
                        if (pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos < distanceCheck)
                        {
                            field_120_state.raw = 0;
                            mCurrentMotion = eAbeMotions::Motion_116_MineCarEnter_458780;
                            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((mineCarRect.x + mineCarRect.w) / 2);
                            MapFollowMe(TRUE);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

s32 Abe::NearDoorIsOpen_44EE10()
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
            if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - pDoor->mBaseAnimatedWithPhysicsGameObject_XPos) < FP_FromInteger(15) && FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pDoor->mBaseAnimatedWithPhysicsGameObject_YPos) < FP_FromInteger(20))
            {
                return pDoor->vIsOpen();
            }
        }
    }
    // We didn't find a door - so for some reason that makes no sense return that it is open...
    return TRUE;
}

s16 Abe::HandleDoAction_455BD0()
{
    Path_TLV* pTlv = sPath_dword_BB47C0->TlvGetAt(
        nullptr,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);

    if (!pTlv)
    {
        // Why ya trying to use nothing?
        return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
    }

    for (;;)
    {
        switch (pTlv->mTlvType32.mType)
        {
            case TlvTypes::LocalWell_8:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_78_WellBegin_45C810;

            case TlvTypes::Lever_17:
            {
                FP xpos = {};
                FP ypos = {};
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    xpos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
                    ypos = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                else
                {
                    xpos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5);
                    ypos = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
                }

                Lever* pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, ypos, xpos));
                if (!pSwitch || !(pSwitch->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pSwitch->mBaseAnimatedWithPhysicsGameObject_XPos)))
                {
                    return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                }

                return eAbeMotions::Motion_99_LeverUse_455AC0;
            }

            case TlvTypes::WellExpress_23:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_78_WellBegin_45C810;

            case TlvTypes::BoomMachine_59:
            {
                auto pGrenadeMachine = static_cast<BoomMachine*>(FindObjectOfType(ReliveTypes::eGrenadeMachine, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(25))));
                if (!pGrenadeMachine || !(pGrenadeMachine->VIsButtonOn()))
                {
                    return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                }
                pGrenadeMachine->VHandleButton();
                return eAbeMotions::Motion_88_GrenadeMachineUse_45C510;
            }

            default:
                // Next TLV, exit if we're at the end.
                pTlv = sPath_dword_BB47C0->TlvGetAt(
                    pTlv,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos);

                if (!pTlv)
                {
                    return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                }
                continue;
        }
    }
}

void Abe::PushWall_44E890()
{
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mCurrentMotion = eAbeMotions::Motion_73_PushWall_4553A0;
    Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
}

void Abe::MoveForward_44E9A0()
{
    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mBaseAnimatedWithPhysicsGameObject_XPos,
            &mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_VelX);
    }

    TrapDoor* pTrapdoor = static_cast<TrapDoor*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (BaseAliveGameObjectCollisionLine && (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor).Mask() == CollisionMask(BaseAliveGameObjectCollisionLine->mLineType).Mask())
    {
        // Handle trap door collision.
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (pTrapdoor)
            {
                pTrapdoor->VRemove(this);
                BaseAliveGameObject_PlatformId = -1;
            }

            const PSX_RECT bRect = VGetBoundingRect();
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {static_cast<s16>(bRect.w + 5), 
                 static_cast<s16>(bRect.h + 5)}, // TODO: Is it really on both ??
                ObjList_5C1B78,
                1,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
        else if (pTrapdoor)
        {
            pTrapdoor->VRemove(this);
            BaseAliveGameObject_PlatformId = -1;
        }
    }
    else
    {
        BaseAliveGameObjectCollisionLine = 0;
        if (pTrapdoor)
        {
            pTrapdoor->VRemove(this);
            BaseAliveGameObject_PlatformId = -1;
        }

        field_118_prev_held = 0;

        switch (mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop_44FBA0:
            case eAbeMotions::Motion_6_WalkBegin_44FEE0:
            case eAbeMotions::Motion_4_WalkToIdle_44FFC0:
            case eAbeMotions::Motion_5_MidWalkToIdle_450080:
                mCurrentMotion = eAbeMotions::Motion_93_WalkOffEdge_455970;
                break;

            case eAbeMotions::Motion_33_RunLoop_4508E0:
            case eAbeMotions::Motion_39_StandingToRun_450D40:
                mCurrentMotion = eAbeMotions::Motion_94_RunOffEdge_4559A0;
                break;

            case eAbeMotions::jMotion_38_RunToRoll_453A70:
            case eAbeMotions::Motion_22_RollBegin_4539A0:
            case eAbeMotions::Motion_23_RollLoop_453A90:
            case eAbeMotions::Motion_24_453D00:
                mCurrentMotion = eAbeMotions::Motion_98_RollOffEdge_455AA0;
                break;

            default:
                mCurrentMotion = eAbeMotions::Motion_95_SneakOffEdge_4559C0;
                break;
        }

        field_128.field_8_x_vel_slow_by = FP_FromDouble(0.3); // TODO: Check.
        mBaseAnimatedWithPhysicsGameObject_XPos = oldXPos + mBaseAnimatedWithPhysicsGameObject_VelX;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        // TODO: OG bug, dead code due to switch default case?
        if (mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 || mCurrentMotion == eAbeMotions::Motion_101_KnockForward_455420)
        {
            field_128.field_8_x_vel_slow_by = FP_FromDouble(0.67); // TODO: Check.
        }
    }
}

s16 Abe::CrouchingGameSpeak_453E10()
{
    field_118_prev_held |= Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (sInputKey_GameSpeak2_5550F8 & field_118_prev_held)
    {
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eFollowMe_10);
        Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
        mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        return TRUE;
    }
    else if (sInputKey_GameSpeak3_555100 & field_118_prev_held)
    {
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eWait_12);
        Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eHello_9);

        if (field_128.field_12_mood == Mud_Emotion::eHappy_5 || field_128.field_12_mood == Mud_Emotion::eWired_6)
        {
            Mudokon_SFX(MudSounds::eHiHappy_19, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        }
        else
        {
            if (field_128.field_12_mood == Mud_Emotion::eSad_3)
            {
                Mudokon_SFX(MudSounds::eHiSad_20, 0, 0, this);
            }
            else
            {
                Mudokon_SFX(MudSounds::eHelloNeutral_3, 0, 0, this);
            }
            mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        }
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak4_5550FC)
    {
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eWork_21);
        Mudokon_SFX(MudSounds::eWork_25, 0, 0, this);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak6_555108)
    {
        Mudokon_SFX(MudSounds::eAllOYa_17, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eAllYa_23);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak5_55510C)
    {
        Mudokon_SFX(MudSounds::eAnger_5, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eAnger_11);
        mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak8_555110)
    {
        Mudokon_SFX(MudSounds::eStopIt_26, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eStopIt_22);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak7_555114)
    {
        Mudokon_SFX(MudSounds::eSadUgh_28, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eSorry_24);
        mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool Abe::Is_Celling_Above_44E8D0()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(
               mBaseAnimatedWithPhysicsGameObject_XPos,
               mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5),
               mBaseAnimatedWithPhysicsGameObject_XPos,
               mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(45)),
               &pLine,
               &hitX,
               &hitY,
               mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgCeiling : kBgCeiling)
        != 0;
}

void Abe::MoveWithVelocity_450FA0(FP velocityX)
{
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        const FP newVelX = mBaseAnimatedWithPhysicsGameObject_VelX - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velocityX);
        mBaseAnimatedWithPhysicsGameObject_VelX = newVelX;
        if (newVelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        const FP newVelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velocityX) + mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_VelX = newVelX;
        if (newVelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        MoveForward_44E9A0();
    }
}

s16 Abe::RunTryEnterDoor_451220()
{
    // Can't be entering a door if we're not pressing up.
    if (!Input().isPressed(sInputKey_Up_5550D8))
    {
        return 0;
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame < 4)
    {
        return 0;
    }

    // Are we actually on a door?
    Path_TLV* pDoorTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::Door_5);

    if (!pDoorTlv)
    {
        return 0;
    }

    if (!NearDoorIsOpen_44EE10())
    {
        return 0;
    }

    BaseAliveGameObjectPathTLV = pDoorTlv;
    field_120_state.raw = 0;
    mCurrentMotion = eAbeMotions::Motion_114_DoorEnter_459470;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pDoorTlv->mTopLeft.x + pDoorTlv->mBottomRight.x) / 2);
    MapFollowMe(TRUE);
    return 1;
}

s16 Abe::RunTryEnterWell_451060()
{
    if (!Input().isPressed(sInputKey_Up_5550D8) || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted) || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame < 4)
    {
        return 0;
    }

    auto pWellLocal = static_cast<Path_WellLocal*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::LocalWell_8));
    if (pWellLocal)
    {
        if (!(mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting)))
        {
            if ((pWellLocal->field_0_scale == Scale_short::eFull_0 && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1)) || (pWellLocal->field_0_scale == Scale_short::eHalf_1 && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5)))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_WalkToRun);
                BaseAliveGameObjectPathTLV = pWellLocal;
                mCurrentMotion = eAbeMotions::Motion_78_WellBegin_45C810;
                return 1;
            }
        }
    }

    auto pWellExpress = static_cast<Path_WellExpress*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::WellExpress_23));
    if (pWellExpress)
    {
        if (!(mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting)))
        {
            if ((pWellExpress->field_0_scale == Scale_short::eFull_0 && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1)) || (pWellExpress->field_0_scale == Scale_short::eHalf_1 && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5)))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_WalkToRun);
                BaseAliveGameObjectPathTLV = pWellExpress;
                mCurrentMotion = eAbeMotions::jMotion_81_WellBegin_45C7F0;
                return 1;
            }
        }
    }

    return 0;
}

void Abe::ToDieFinal_458910()
{
    InvisibleEffect* pInvisibleEffect = static_cast<InvisibleEffect*>(sObjectIds.Find_Impl(field_178_invisible_effect_id));
    if (pInvisibleEffect)
    {
        if (!(pInvisibleEffect->mBaseGameObjectFlags.Get(BaseGameObject::eDead)))
        {
            pInvisibleEffect->ClearInvisibility();
            field_178_invisible_effect_id = -1;
        }
    }

    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
    mCurrentMotion = eAbeMotions::Motion_57_Dead_4589A0;
    field_124_timer = 0;
    mHealth = FP_FromInteger(0);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
}

s16 Abe::DoGameSpeak_45AB70(s32 input)
{
    s16 nextMotion = -1;
    if (Input_IsChanting_45F260())
    {
        // Fixes an OG bug where Abe doesn't transform into Shrykull when you immediately chant after using GameSpeak.
        if (!(field_168_ring_pulse_timer && field_16C_bHaveShrykull))
        {
            field_124_timer = sGnFrame + 90;
            SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 1);
            field_120_state.chant = ChantStates::eIdleChanting_0;
            nextMotion = eAbeMotions::Motion_112_Chant_45B1C0;
        }
    }
    else if (input & sInputKey_GameSpeak2_5550F8)
    {
        Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eFollowMe_10);
        nextMotion = eAbeMotions::Motion_7_Speak_45B140;
    }
    else if (input & sInputKey_GameSpeak3_555100)
    {
        Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eWait_12);
        nextMotion = eAbeMotions::Motion_8_Speak_45B160;
    }
    else if (input & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eHello_9);
        if (field_128.field_12_mood == Mud_Emotion::eHappy_5 || field_128.field_12_mood == Mud_Emotion::eWired_6)
        {
            Mudokon_SFX(MudSounds::eHiHappy_19, 0, 0, this);
            nextMotion = eAbeMotions::Motion_9_Speak_45B180;
        }
        else
        {
            if (field_128.field_12_mood == Mud_Emotion::eSad_3)
            {
                Mudokon_SFX(MudSounds::eHiSad_20, 0, 0, this);
            }
            else
            {
                Mudokon_SFX(MudSounds::eHelloNeutral_3, 0, 0, this);
            }
            nextMotion = eAbeMotions::Motion_9_Speak_45B180;
        }
    }
    else if (input & sInputKey_GameSpeak4_5550FC)
    {
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eWork_21);
        Mudokon_SFX(MudSounds::eWork_25, 0, 0, this);
        nextMotion = eAbeMotions::Motion_10_Fart_45B1A0;
    }
    else if (input & sInputKey_GameSpeak6_555108)
    {
        Mudokon_SFX(MudSounds::eAllOYa_17, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eAllYa_23);
        nextMotion = eAbeMotions::Motion_9_Speak_45B180;
    }
    else if (input & sInputKey_GameSpeak5_55510C)
    {
        FP gridSize = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
        else
        {
            gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }

        if (FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            nextMotion = eAbeMotions::Motion_62_Punch_454750;
        }
        else if (FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos + (gridSize * FP_FromInteger(2)), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
        {
            nextMotion = eAbeMotions::Motion_62_Punch_454750;
        }
        else
        {
            // NOTE: Extra check for locks, it must also be being rendered in order to for us to try to hit it.
            BaseAnimatedWithPhysicsGameObject* pLockedSoul = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eLockedSoul, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)));
            if (pLockedSoul && pLockedSoul->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eSlig, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eSlig, mBaseAnimatedWithPhysicsGameObject_XPos + (gridSize * FP_FromInteger(2)), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eGlukkon, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eGlukkon, mBaseAnimatedWithPhysicsGameObject_XPos + (gridSize * FP_FromInteger(2)), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else
            {
                // Didn't hit anything, just anger.
                Mudokon_SFX(MudSounds::eAnger_5, 0, 0, this);
                pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eAnger_11);
                nextMotion = eAbeMotions::Motion_8_Speak_45B160;
            }
        }
    }
    else if (input & sInputKey_GameSpeak8_555110)
    {
        Mudokon_SFX(MudSounds::eStopIt_26, 0, 0, this);
        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eStopIt_22);
        nextMotion = eAbeMotions::Motion_7_Speak_45B140;
    }
    else if (input & sInputKey_GameSpeak7_555114)
    {
        FP gridSize = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
        else
        {
            gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }

        pEventSystem_5BC11C->PushEvent(GameSpeakEvents::eSorry_24);
        if (FindObjectOfType(ReliveTypes::eMudokon, mBaseAnimatedWithPhysicsGameObject_XPos + gridSize, mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(40))))
        {
            nextMotion = eAbeMotions::Motion_63_Sorry_454670;
        }
        else
        {
            Mudokon_SFX(MudSounds::eSadUgh_28, 0, 0, this);
            nextMotion = eAbeMotions::Motion_10_Fart_45B1A0; // TODO: Correct but isn't fart in this case ??
        }
    }

    return nextMotion;
}

s16 Abe::CantBeDamaged_44BAB0()
{
    switch (mCurrentMotion)
    {
        case eAbeMotions::Motion_75_JumpIntoWell_45C7B0:
        case eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40:
        case eAbeMotions::Motion_77_ToWellShotOut_45D130:
        case eAbeMotions::Motion_78_WellBegin_45C810:
        case eAbeMotions::Motion_79_InsideWellLocal_45CA60:
        case eAbeMotions::Motion_80_WellShotOut_45D150:
        case eAbeMotions::jMotion_81_WellBegin_45C7F0:
        case eAbeMotions::Motion_82_InsideWellExpress_45CC80:
        case eAbeMotions::Motion_83_WellExpressShotOut_45CF70:
        case eAbeMotions::Motion_114_DoorEnter_459470:
        case eAbeMotions::Motion_115_DoorExit_459A40:
        case eAbeMotions::Motion_119_ToShrykull_45A990:
        case eAbeMotions::Motion_120_EndShrykull_45AB00:
            return TRUE;
    }

    // TODO: Unknown what this is checking, condition should probably be inverted.
    if ((!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)) && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render)) || mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void Abe::FallOnBombs_44EC10()
{
    const PSX_RECT bOurRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
        {
            const PSX_RECT objRect = pObj->VGetBoundingRect();
            if (bOurRect.x <= objRect.w && bOurRect.w >= objRect.x && bOurRect.h >= objRect.y && bOurRect.y <= objRect.h)
            {
                pObj->VTakeDamage(this);
            }
        }
    }
}

s16 Abe::ForceDownIfHoisting_44BA30()
{
    if (mCurrentMotion != eAbeMotions::Motion_65_LedgeAscend_4548E0 && mCurrentMotion != eAbeMotions::Motion_67_LedgeHang_454E20 && mCurrentMotion != eAbeMotions::Motion_69_LedgeHangWobble_454EF0 && mCurrentMotion != eAbeMotions::Motion_66_LedgeDescend_454970 && mCurrentMotion != eAbeMotions::Motion_68_ToOffScreenHoist_454B80)
    {
        return 0;
    }

    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist_455800;

    field_124_timer = 0;

    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);

    return 1;
}

void Abe::BulletDamage_44C980(Bullet* pBullet)
{
    const PSX_RECT rect = VGetBoundingRect();

    enum class ShootKind
    {
        eEverythingElse_0 = 0,
        eHanging_1 = 1,
        eRolling_2 = 2,
    };

    if (Is_In_Current_Camera() != CameraPos::eCamCurrent_0 || mHealth <= FP_FromInteger(0))
    {
        return;
    }

    ShootKind shootKind = ShootKind::eEverythingElse_0;
    if (mCurrentMotion == eAbeMotions::Motion_22_RollBegin_4539A0 || mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90 || mCurrentMotion == eAbeMotions::Motion_24_453D00 || mCurrentMotion == eAbeMotions::Motion_37_CrouchTurn_454390 || mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0)
    {
        shootKind = ShootKind::eRolling_2;
    }
    else if (mCurrentMotion == eAbeMotions::Motion_65_LedgeAscend_4548E0 || mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 || mCurrentMotion == eAbeMotions::Motion_69_LedgeHangWobble_454EF0 || mCurrentMotion == eAbeMotions::Motion_66_LedgeDescend_454970 || mCurrentMotion == eAbeMotions::Motion_68_ToOffScreenHoist_454B80)
    {
        shootKind = ShootKind::eHanging_1;
    }

    mHealth -= FP_FromDouble(0.34);
    if (mHealth > FP_FromInteger(0))
    {
        FP xOffset = {};
        if (mHealth <= FP_FromDouble(0.5))
        {
            if (pBullet->field_30_x_distance > FP_FromInteger(0))
            {
                xOffset = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                xOffset = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
            }
        }
        else
        {
            if (pBullet->field_30_x_distance > FP_FromInteger(0))
            {
                xOffset = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2));
            }
            else
            {
                xOffset = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2));
            }
        }

        PathLine* pathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions->Raycast(
                xOffset,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5),
                xOffset,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5),
                &pathLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
            == 1)
        {
            if (pBullet->field_20_type != BulletType::ePossessedSligZBullet_1 && pBullet->field_20_type != BulletType::eZBullet_3)
            {
                relive_new Spark(hitX, hitY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 9, -31, 159, SparkType::eSmallChantParticle_0);
                New_Smoke_Particles(hitX, hitY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 3, 128u, 128u, 128u);
            }
        }
        return;
    }

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);

    switch (pBullet->field_20_type)
    {
        case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
        case BulletType::eNormalBullet_2:
        {
            FP bloodXOffset = {};
            if (pBullet->field_30_x_distance <= FP_FromInteger(0))
            {
                bloodXOffset = FP_FromInteger(-24);
            }
            else
            {
                bloodXOffset = FP_FromInteger(24);
            }
            
            relive_new Blood(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                pBullet->field_2C_ypos,
                bloodXOffset,
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                50);

            switch (shootKind)
            {
                case ShootKind::eEverythingElse_0:
                {
                    ToKnockback_44E700(1, 1);
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != (pBullet->field_30_x_distance > FP_FromInteger(0)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_101_KnockForward_455420;
                    }
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(7.8);
                    if (pBullet->field_30_x_distance < FP_FromInteger(0))
                    {
                        mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    }
                    break;
                }
                case ShootKind::eHanging_1:
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist_455800;
                    field_124_timer = 0;
                    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                    break;
                }
                case ShootKind::eRolling_2:
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) == (pBullet->field_30_x_distance > FP_FromInteger(0)))
                    {
                        mNextMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
                    }
                    else
                    {
                        mNextMotion = eAbeMotions::Motion_102_RollingKnockForward_455310;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }

        case BulletType::ePossessedSligZBullet_1:
        case BulletType::eZBullet_3:
        {
            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                mHealth = FP_FromInteger(1);
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                return;
            }

            const FP boundsY = FP_FromInteger(rect.y);
            if (Bullet::InZBulletCover(mBaseAnimatedWithPhysicsGameObject_XPos, boundsY, rect) || !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, boundsY, 0))
            {
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                mHealth = FP_FromInteger(1);
                return;
            }

            FP yOffset = {};
            if (shootKind == ShootKind::eEverythingElse_0)
            {
                yOffset = (FP_FromInteger(-45) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                mNextMotion = eAbeMotions::Motion_110_ZShot_455670;
            }
            else if (shootKind == ShootKind::eHanging_1)
            {
                yOffset = (FP_FromInteger(45) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist_455800;
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
                mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_124_timer = 0;
            }
            else if (shootKind == ShootKind::eRolling_2)
            {
                yOffset = (FP_FromInteger(-25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                mNextMotion = eAbeMotions::Motion_109_ZShotRolling_455550;
            }

            relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos, yOffset + mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 50);
            break;
        }

        default:
            break;
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit1_bShot))
    {
        field_122_knockdown_motion = mNextMotion;
    }

    Environment_SFX_457A40(EnvironmentSfx::eElumHitWall_14, 0, 32767, this);
    Mudokon_SFX(MudSounds::eHurt2_9, 127, 0, this);
    Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
    SFX_Play_Pitch(SoundEffect::Eating1_65, 0, -500, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    SfxPlayMono(SoundEffect::KillEffect_64, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
}

void Abe::GiveControlBackToMe_44BA10()
{
    sControlledCharacter_5C1B8C = this;
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6_prevent_chanting);
}

PullRingRope* Abe::GetPullRope_44D120()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find a rope.
        if (pObj->Type() == ReliveTypes::ePullRope)
        {
            // Is it on the same scale as us?
            PullRingRope* pRope = static_cast<PullRingRope*>(pObj);
            if (pRope->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
            {
                const PSX_RECT bRect = pRope->VGetBoundingRect();

                // Check we are near its ypos.
                if ((mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(75))) <= pRope->mBaseAnimatedWithPhysicsGameObject_YPos && mBaseAnimatedWithPhysicsGameObject_YPos > pRope->mBaseAnimatedWithPhysicsGameObject_YPos)
                {
                    // Check we are near its xpos.
                    if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(bRect.x) && mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(bRect.w))
                    {
                        // Found a rope we can pull.
                        return pRope;
                    }
                }
            }
        }
    }
    return nullptr;
}

void Abe::IntoPortalStates_451990()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(field_1A8_portal_id));
    if (pBirdPortal)
    {
        PSX_RECT bRect = {};
        switch (field_1A4_portal_sub_state)
        {
            case PortalSubStates::eJumpingInsidePortal_0:
                bRect = VGetBoundingRect();
                if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && FP_FromInteger(bRect.x) > pBirdPortal->field_2C_xpos) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && FP_FromInteger(bRect.w) < pBirdPortal->field_2C_xpos))
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                    pBirdPortal->VKillPortalClipper();
                    pBirdPortal->VGiveShrykull(TRUE);
                    field_1A4_portal_sub_state = PortalSubStates::eSetNewActiveCamera_1;
                }

                mBaseAnimatedWithPhysicsGameObject_VelY += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8);
                mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
                mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
                return;

            case PortalSubStates::eSetNewActiveCamera_1:
                if (pBirdPortal->VAbeInsidePortal())
                {
                    EReliveLevelIds level = {};
                    u16 path = 0;
                    u16 camera = 0;
                    CameraSwapEffects screenChangeEffect = {};
                    u16 movieId = 0;

                    pBirdPortal->VGetMapChange(&level, &path, &camera, &screenChangeEffect, &movieId);
                    gMap.SetActiveCam(level, path, camera, screenChangeEffect, movieId, FALSE);
                    field_1A4_portal_sub_state = PortalSubStates::eSetNewAbePosition_4;
                }
                break;

            case PortalSubStates::eHopOutOfPortal_2:
                if (pBirdPortal->VPortalExit_AbeExitting())
                {
                    pBirdPortal->VPortalClipper(0);
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                    mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
                    pBirdPortal->VIncreaseTimerAndKillPortalClipper();
                    field_1A8_portal_id = -1;
                }
                break;

            case PortalSubStates::eSetNewAbePosition_4:
                pBirdPortal->VExitPortal();
                field_1A4_portal_sub_state = PortalSubStates::eHopOutOfPortal_2;

                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pBirdPortal->field_26_side == PortalSide::eLeft_1);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + pBirdPortal->field_34_exit_x;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = pBirdPortal->field_34_exit_x - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                mBaseAnimatedWithPhysicsGameObject_YPos = pBirdPortal->field_38_exit_y;
                BaseAliveGameObjectLastLineYPos = pBirdPortal->field_38_exit_y;
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Abe::Calc_Well_Velocity_45C530(s16 xPosSource, s16 yPosSource, s16 xPosDest, s16 yPosDest)
{
    PSX_Point abeSpawnPos = {};
    gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);

    const FP gravity = mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1) ? FP_FromDouble(1.8) : FP_FromDouble(0.9);
    const FP xPosDistance = FP_FromInteger(xPosDest - xPosSource);
    FP yPosRealDistance = {};
    if (yPosDest > 0)
    {
        const s32 yPosSourceFull = abeSpawnPos.y + yPosSource;
        if (yPosDest > yPosSourceFull)
        {
            const FP yPosDistance = FP_FromInteger(yPosDest - yPosSourceFull);
            FP yPosDistanceOffset = {};
            if (yPosDistance <= (FP_FromInteger(41) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                yPosDistanceOffset = FP_FromInteger(0);
            }
            else
            {
                yPosDistanceOffset = yPosDistance - (FP_FromInteger(41) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            const FP spriteScaleFactor = FP_FromInteger(20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
            FP yPosDistanceCalc = (yPosDistanceOffset / spriteScaleFactor) + FP_FromDouble(20.01);
            if (xPosDest > 0)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (xPosDistance - FP_FromInteger(abeSpawnPos.x)) / yPosDistanceCalc;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(2.796) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
            }
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromDouble(-16.1) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
            return;
        }

        const s32 yPosFullDistanceInverse = yPosSourceFull - yPosDest;
        if (yPosFullDistanceInverse >= 0)
        {
            yPosRealDistance = FP_FromInteger(yPosFullDistanceInverse);
        }
        else
        {
            yPosRealDistance = FP_FromInteger(yPosDest - yPosSourceFull);
        }
    }
    else
    {
        yPosRealDistance = FP_FromInteger(0);
    }

    const FP yPosRealDistanceFull = (FP_FromInteger(80) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + yPosRealDistance;
    const FP gravityFactor = FP_FromInteger(8) * gravity;
    const FP gravityCalc = Math_SquareRoot_FP_Wrapper(((gravityFactor * yPosRealDistanceFull) + (gravity * gravity)));
    mBaseAnimatedWithPhysicsGameObject_VelY = (gravityCalc - gravity) * FP_FromDouble(0.5);

    const FP yVelocityAfterGravity = mBaseAnimatedWithPhysicsGameObject_VelY / gravity;
    mBaseAnimatedWithPhysicsGameObject_VelY = -mBaseAnimatedWithPhysicsGameObject_VelY;

    if (xPosDest > 0)
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (xPosDistance - FP_FromInteger(abeSpawnPos.x)) / (yVelocityAfterGravity + FP_FromDouble(8.9));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(2.796) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    }
}

void Abe::FollowLift_45A500()
{
    LiftPoint* pLift = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLift)
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = pLift->mBaseAnimatedWithPhysicsGameObject_VelY;
        if (pLift->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            VOnTrapDoorOpen();
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift);
        }
        SetActiveCameraDelayedFromDir();
    }
}

s16 Abe::MoveLiftUpOrDown_45A7E0(FP yVelocity)
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!pLiftPoint)
    {
        return eAbeMotions::Motion_123_LiftGrabIdle_45A6A0;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), yVelocity, 0);
    FollowLift_45A500();

    if (sControlledCharacter_5C1B8C == this && !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)) && mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 5)
    {
        return mCurrentMotion;
    }

    if (yVelocity >= FP_FromInteger(0))
    {
        if (yVelocity > FP_FromInteger(0))
        {
            if (pLiftPoint->vOnBottomFloor())
            {
                return eAbeMotions::Motion_123_LiftGrabIdle_45A6A0;
            }

            const u32 pressed = Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (sInputKey_Down_5550DC & pressed)
            {
                return eAbeMotions::Motion_125_LiftUseDown_45A7B0;
            }

            if (pressed & sInputKey_Up_5550D8)
            {
                return eAbeMotions::Motion_124_LiftUseUp_45A780;
            }
        }
    }
    else
    {
        if (pLiftPoint->vOnTopFloor())
        {
            return eAbeMotions::Motion_123_LiftGrabIdle_45A6A0;
        }

        if (Input().isPressed(sInputKey_Up_5550D8))
        {
            return eAbeMotions::Motion_124_LiftUseUp_45A780;
        }

        if (Input().isPressed(sInputKey_Down_5550DC))
        {
            return eAbeMotions::Motion_125_LiftUseDown_45A7B0;
        }
    }

    if (Input().field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed && pLiftPoint->vOnAnyFloor() && !(pLiftPoint->field_12C_bMoving & 1))
    {
        return eAbeMotions::Motion_122_LiftGrabEnd_45A670;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eAbeMotions::Motion_123_LiftGrabIdle_45A6A0;
}

s16 Abe::GetEvilFart_4585F0(s16 bDontLoad)
{
    const PSX_RECT abeRect = VGetBoundingRect();

    BrewMachine* pBrewMachine = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            return 0;
        }

        if (pObj->Type() == ReliveTypes::eBrewMachine)
        {
            pBrewMachine = static_cast<BrewMachine*>(pObj);

            const PSX_RECT bRect = pBrewMachine->VGetBoundingRect();

            if (RectsOverlap(abeRect, bRect) && pBrewMachine->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && pBrewMachine->field_144_total_brew_count > 0 && field_198_has_evil_fart == FALSE)
            {
                break;
            }

            pBrewMachine = nullptr;
        }
    }

    if (!pBrewMachine)
    {
        return 0;
    }

    if (!bDontLoad)
    {
        return 1;
    }

    field_10_resources_array.SetAt(22, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, TRUE, FALSE));
    field_10_resources_array.SetAt(23, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(24, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, TRUE, FALSE));
    pBrewMachine->field_144_total_brew_count--;
    field_198_has_evil_fart = TRUE;

    return 1;
}

void Abe::ChangeChantState_45BB90(s16 bLaughAtChantEnd)
{
    if (bLaughAtChantEnd)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_laugh_at_chant_end);
        field_120_state.chant = ChantStates::eChantingForBirdPortal_6; // Holds chant, then laughs.
    }
    else if (sControlledCharacter_5C1B8C == this)
    {
        field_120_state.chant = ChantStates::eIdleChanting_0; // Chants briefly, then stops.
    }
    else
    {
        field_120_state.chant = ChantStates::eWaitForUnpossessing_3; // Chants briefly with a possession flicker, then stops.
    }
}

void Abe::SetAsDead_459430()
{
    field_120_state.raw = 1;
}

void Abe::ExitShrykull_45A9D0(s16 bResetRingTimer)
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);

    mCurrentMotion = eAbeMotions::Motion_120_EndShrykull_45AB00;
    field_124_timer = 1;

    if (bResetRingTimer)
    {
        field_168_ring_pulse_timer = 0;
    }
}

static void playAbeSFX(MudSounds idx, s16 volume, s32 pitch)
{
    SFX_SfxDefinition_Play_Mono(
        &sAbeSFXList_555250[static_cast<u8>(idx)],
        volume,
        static_cast<s16>(pitch), static_cast<s16>(pitch));
}

void Mudokon_SFX(MudSounds idx, s16 volume, s32 pitch, BaseAliveGameObject* pHero)
{
    switch (idx)
    {
        case MudSounds::eOops_14:
        {
            if (pHero && pHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                SND_SEQ_Play(SeqId::AbeOops_16, 1, 90, 90);
            }
            else
            {
                SND_SEQ_Play(SeqId::AbeOops_16, 1, 127, 127);
            }
            break;
        }
        case MudSounds::eStopIt_26:
        {
            if (pHero && pHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                SND_SEQ_Play(SeqId::AbeStopIt_18, 1, 80, 80);
            }
            else
            {
                SND_SEQ_Play(SeqId::AbeStopIt_18, 1, 115, 115);
            }
            break;
        }
        case MudSounds::eGiggle_8:
        {
            if (IsActiveHero(pHero) && gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                idx = MudSounds::eLaugh_10;
            }

            [[fallthrough]];
        }
        default:
        {
            auto idxToVal = static_cast<u8>(idx);
            if (!volume)
            {
                volume = sAbeSFXList_555250[idxToVal].field_3_default_volume;
            }

            // OG bug - isn't null checked in other cases before de-ref?
            if (!pHero)
            {
                playAbeSFX(idx, volume, pitch);
                return;
            }

            if (pHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                volume = 2 * volume / 3;
            }

            if (IsActiveHero(pHero))
            {
                playAbeSFX(idx, volume, pitch);
                return;
            }

            switch (gMap.GetDirection(
                pHero->mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                pHero->mBaseAnimatedWithPhysicsGameObject_PathNumber,
                pHero->mBaseAnimatedWithPhysicsGameObject_XPos,
                pHero->mBaseAnimatedWithPhysicsGameObject_YPos))
            {
                case CameraPos::eCamCurrent_0:
                {
                    playAbeSFX(idx, volume, pitch);
                    break;
                }
                case CameraPos::eCamTop_1:
                case CameraPos::eCamBottom_2:
                {
                    playAbeSFX(idx, 2 * volume / 3, pitch);
                    break;
                }
                case CameraPos::eCamLeft_3:
                {
                    SFX_SfxDefinition_Play_Stereo(
                        &sAbeSFXList_555250[idxToVal],
                        2 * volume / 3,
                        2 * volume / 9,
                        static_cast<s16>(pitch), static_cast<s16>(pitch));
                    break;
                }
                case CameraPos::eCamRight_4:
                {
                    SFX_SfxDefinition_Play_Stereo(
                        &sAbeSFXList_555250[idxToVal],
                        2 * volume / 9,
                        2 * volume / 3,
                        static_cast<s16>(pitch), static_cast<s16>(pitch));
                    break;
                }
                default:
                    break;
            }
        }
    }
}

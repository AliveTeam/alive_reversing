#include "stdafx.h"
#include "Abe.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Midi.hpp"
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
#include "Switch.hpp"
#include "Throwable.hpp"
#include "LiftPoint.hpp"

using TAbeStateFunction = decltype(&Abe::State_0_Idle_44EEB0);

#define ABE_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_44EEB0) \
    ENTRY(State_1_WalkLoop_44FBA0) \
    ENTRY(State_2_StandingTurn_451830) \
    ENTRY(State_3_Fall_459B60) \
    ENTRY(State_4_WalkEndLeftFoot_44FFC0) \
    ENTRY(State_5_WalkEndRightFoot_00450080) \
    ENTRY(State_6_WalkBegin_44FEE0) \
    ENTRY(State_7_Speak_45B140) \
    ENTRY(State_8_Speak_45B160) \
    ENTRY(State_9_SpeakMovement_45B180) \
    ENTRY(State_10_Fart_45B1A0) \
    ENTRY(State_11_Speak_45B0A0) \
    ENTRY(State_12_Null_4569C0) \
    ENTRY(State_13_HoistBegin_452B20) \
    ENTRY(State_14_HoistIdle_452440) \
    ENTRY(State_15_HoistLand_452BA0) \
    ENTRY(State_16_LandSoft_45A360) \
    ENTRY(State_17_CrouchIdle_456BC0) \
    ENTRY(State_18_CrouchToStand_454600) \
    ENTRY(State_19_StandToCrouch_453DC0) \
    ENTRY(State_20_454550) \
    ENTRY(jState_21_4545E0) \
    ENTRY(State_22_RollBegin_4539A0) \
    ENTRY(State_23_RollLoop_453A90) \
    ENTRY(State_24_453D00) \
    ENTRY(State_25_RunSlideStop_451330) \
    ENTRY(State_26_RunTurn_451500) \
    ENTRY(State_27_HopBegin_4521C0) \
    ENTRY(State_28_HopMid_451C50) \
    ENTRY(State_29_HopLand_4523D0) \
    ENTRY(State_30_RunJumpBegin_4532E0) \
    ENTRY(State_31_RunJumpMid_452C10) \
    ENTRY(State_32_RunJumpLand_453460) \
    ENTRY(State_33_RunLoop_4508E0) \
    ENTRY(State_34_DunnoBegin_44ECF0) \
    ENTRY(State_35_DunnoEnd_44ED10) \
    ENTRY(State_36_Null_45BC50) \
    ENTRY(State_37_CrouchTurn_454390) \
    ENTRY(jState_38_RunToRoll_453A70) \
    ENTRY(State_39_StandingToRun_450D40) \
    ENTRY(State_40_SneakLoop_450550) \
    ENTRY(State_41_450250) \
    ENTRY(State_42_4503D0) \
    ENTRY(State_43_450380) \
    ENTRY(State_44_450500) \
    ENTRY(State_45_SneakBegin_4507A0) \
    ENTRY(State_46_SneakEnd_450870) \
    ENTRY(jState_47_SneakEnd_4508C0) \
    ENTRY(State_48_4500A0) \
    ENTRY(State_49_450200) \
    ENTRY(State_50_RunToWalk1_450E20) \
    ENTRY(State_51_RunToWalk2_450F50) \
    ENTRY(State_52_RunTurn_ToRun_451710) \
    ENTRY(State_53_RunTurn_ToWalk_451800) \
    ENTRY(State_54_RunJumpLandRun_4538F0) \
    ENTRY(State_55_RunJumpLandWalk_453970) \
    ENTRY(State_56_4591F0) \
    ENTRY(State_57_Dead_4589A0) \
    ENTRY(State_58_DeadPre_4593E0) \
    ENTRY(State_59_Null_459450) \
    ENTRY(State_60_4A3200) \
    ENTRY(State_61_TurnToRun_456530) \
    ENTRY(State_62_Punch_454750) \
    ENTRY(State_63_Sorry_454670) \
    ENTRY(State_64_454730) \
    ENTRY(State_65_LedgeAscend_End_4548E0) \
    ENTRY(State_66_LedgeDescend_454970) \
    ENTRY(State_67_LedgeHang_454E20) \
    ENTRY(State_68_ToOffScreenHoist_454B80) \
    ENTRY(State_69_LedgeHangWobble_454EF0) \
    ENTRY(State_70_RingRopePull_455AF0) \
    ENTRY(State_71_Knockback_455090) \
    ENTRY(State_72_KnockbackGetUp_455340) \
    ENTRY(State_73_PushWall_4553A0) \
    ENTRY(State_74_Rolling_KnockedBack_455290) \
    ENTRY(State_75_Jump_Into_Well_45C7B0) \
    ENTRY(State_76_45CA40) \
    ENTRY(State_77_45D130) \
    ENTRY(State_78_WellBegin_45C810) \
    ENTRY(State_79_Inside_Of_A_Well_Local_45CA60) \
    ENTRY(State_80_WellShotOut_45D150) \
    ENTRY(jState_81_WellBegin_45C7F0) \
    ENTRY(State_82_Inside_Of_A_Well_Express_45CC80) \
    ENTRY(State_83_Shoot_Out_Of_A_Well_45CF70) \
    ENTRY(State_84_FallLandDie_45A420) \
    ENTRY(jState_85_Fall_455070) \
    ENTRY(State_86_HandstoneBegin_45BD00) \
    ENTRY(State_87_HandstoneEnd_45C4F0) \
    ENTRY(State_88_GrenadeMachineUse_45C510) \
    ENTRY(State_89_BrewMachineBegin_4584C0) \
    ENTRY(State_90_BrewMachineEnd_4585B0) \
    ENTRY(State_91_FallingFromGrab_4557B0) \
    ENTRY(State_92_ForceDown_From_Hoist_455800) \
    ENTRY(State_93_FallLedgeBegin_455970) \
    ENTRY(State_94_RunOffEdge_4559A0) \
    ENTRY(State_95_SneakOffEdge_4559C0) \
    ENTRY(State_96_HopToFall_4559E0) \
    ENTRY(State_97_RunJumpFall_455A80) \
    ENTRY(State_98_RollOffEdge_455AA0) \
    ENTRY(State_99_LeverUse_455AC0) \
    ENTRY(State_100_Slap_Bomb_455B60) \
    ENTRY(State_101_KnockForward_455420) \
    ENTRY(State_102_455310) \
    ENTRY(jState_103_KnockbackGetUp_455380) \
    ENTRY(State_104_RockThrowStandingHold_455DF0) \
    ENTRY(State_105_RockThrowStandingThrow_456460) \
    ENTRY(State_106_RockThrowStandingEnd_455F20) \
    ENTRY(State_107_RockThrowCrouchingHold_454410) \
    ENTRY(State_108_RockThrowCrouchingThrow_454500) \
    ENTRY(State_109_455550) \
    ENTRY(State_110_455670) \
    ENTRY(State_111_GrabRock_4564A0) \
    ENTRY(State_112_Chant_45B1C0) \
    ENTRY(State_113_ChantEnd_45BBE0) \
    ENTRY(State_114_DoorEnter_459470) \
    ENTRY(State_115_DoorExit_459A40) \
    ENTRY(State_116_MineCarEnter_458780) \
    ENTRY(State_117_In_MineCar_4587C0) \
    ENTRY(State_118_MineCarExit_458890) \
    ENTRY(State_119_45A990) \
    ENTRY(State_120_45AB00) \
    ENTRY(State_121_LiftGrabBegin_45A600) \
    ENTRY(State_122_LiftGrabEnd_45A670) \
    ENTRY(State_123_LiftGrabIdle_45A6A0) \
    ENTRY(State_124_LiftUseUp_45A780) \
    ENTRY(State_125_LiftUseDown_45A7B0) \
    ENTRY(State_126_TurnWheelBegin_456700) \
    ENTRY(State_127_TurnWheelLoop_456750) \
    ENTRY(State_128_TurnWheelEnd_4569A0) \
    ENTRY(State_129_PoisonGasDeath_4565C0)

#define MAKE_ENUM(VAR) VAR,
enum eAbeStates : int
{
    ABE_STATES_ENUM(MAKE_ENUM)
};

#define MAKE_STRINGS(VAR) #VAR,
const char* const sAbeStateNames[130] =
{
    ABE_STATES_ENUM(MAKE_STRINGS)
};

TAbeStateFunction sAbeStateMachineTable_554910[130] =
{
    &Abe::State_0_Idle_44EEB0,
    &Abe::State_1_WalkLoop_44FBA0,
    &Abe::State_2_StandingTurn_451830,
    &Abe::State_3_Fall_459B60,
    &Abe::State_4_WalkEndLeftFoot_44FFC0,
    &Abe::State_5_WalkEndRightFoot_00450080,
    &Abe::State_6_WalkBegin_44FEE0,
    &Abe::State_7_Speak_45B140,
    &Abe::State_8_Speak_45B160,
    &Abe::State_9_SpeakMovement_45B180,
    &Abe::State_10_Fart_45B1A0,
    &Abe::State_11_Speak_45B0A0,
    &Abe::State_12_Null_4569C0,
    &Abe::State_13_HoistBegin_452B20,
    &Abe::State_14_HoistIdle_452440,
    &Abe::State_15_HoistLand_452BA0,
    &Abe::State_16_LandSoft_45A360,
    &Abe::State_17_CrouchIdle_456BC0,
    &Abe::State_18_CrouchToStand_454600,
    &Abe::State_19_StandToCrouch_453DC0,
    &Abe::State_20_454550,
    &Abe::jState_21_4545E0,
    &Abe::State_22_RollBegin_4539A0,
    &Abe::State_23_RollLoop_453A90,
    &Abe::State_24_453D00,
    &Abe::State_25_RunSlideStop_451330,
    &Abe::State_26_RunTurn_451500,
    &Abe::State_27_HopBegin_4521C0,
    &Abe::State_28_HopMid_451C50,
    &Abe::State_29_HopLand_4523D0,
    &Abe::State_30_RunJumpBegin_4532E0,
    &Abe::State_31_RunJumpMid_452C10,
    &Abe::State_32_RunJumpLand_453460,
    &Abe::State_33_RunLoop_4508E0,
    &Abe::State_34_DunnoBegin_44ECF0,
    &Abe::State_35_DunnoEnd_44ED10,
    &Abe::State_36_Null_45BC50,
    &Abe::State_37_CrouchTurn_454390,
    &Abe::jState_38_RunToRoll_453A70,
    &Abe::State_39_StandingToRun_450D40,
    &Abe::State_40_SneakLoop_450550,
    &Abe::State_41_450250,
    &Abe::State_42_4503D0,
    &Abe::State_43_450380,
    &Abe::State_44_450500,
    &Abe::State_45_SneakBegin_4507A0,
    &Abe::State_46_SneakEnd_450870,
    &Abe::jState_47_SneakEnd_4508C0,
    &Abe::State_48_4500A0,
    &Abe::State_49_450200,
    &Abe::State_50_RunToWalk1_450E20,
    &Abe::State_51_RunToWalk2_450F50,
    &Abe::State_52_RunTurn_ToRun_451710,
    &Abe::State_53_RunTurn_ToWalk_451800,
    &Abe::State_54_RunJumpLandRun_4538F0,
    &Abe::State_55_RunJumpLandWalk_453970,
    &Abe::State_56_4591F0,
    &Abe::State_57_Dead_4589A0,
    &Abe::State_58_DeadPre_4593E0,
    &Abe::State_59_Null_459450,
    &Abe::State_60_4A3200,
    &Abe::State_61_TurnToRun_456530,
    &Abe::State_62_Punch_454750,
    &Abe::State_63_Sorry_454670,
    &Abe::State_64_454730,
    &Abe::State_65_LedgeAscend_End_4548E0,
    &Abe::State_66_LedgeDescend_454970,
    &Abe::State_67_LedgeHang_454E20,
    &Abe::State_68_ToOffScreenHoist_454B80,
    &Abe::State_69_LedgeHangWobble_454EF0,
    &Abe::State_70_RingRopePull_455AF0,
    &Abe::State_71_Knockback_455090,
    &Abe::State_72_KnockbackGetUp_455340,
    &Abe::State_73_PushWall_4553A0,
    &Abe::State_74_Rolling_KnockedBack_455290,
    &Abe::State_75_Jump_Into_Well_45C7B0,
    &Abe::State_76_45CA40,
    &Abe::State_77_45D130,
    &Abe::State_78_WellBegin_45C810,
    &Abe::State_79_Inside_Of_A_Well_Local_45CA60,
    &Abe::State_80_WellShotOut_45D150,
    &Abe::jState_81_WellBegin_45C7F0,
    &Abe::State_82_Inside_Of_A_Well_Express_45CC80,
    &Abe::State_83_Shoot_Out_Of_A_Well_45CF70,
    &Abe::State_84_FallLandDie_45A420,
    &Abe::jState_85_Fall_455070,
    &Abe::State_86_HandstoneBegin_45BD00,
    &Abe::State_87_HandstoneEnd_45C4F0,
    &Abe::State_88_GrenadeMachineUse_45C510,
    &Abe::State_89_BrewMachineBegin_4584C0,
    &Abe::State_90_BrewMachineEnd_4585B0,
    &Abe::State_91_FallingFromGrab_4557B0,
    &Abe::State_92_ForceDown_From_Hoist_455800,
    &Abe::State_93_FallLedgeBegin_455970,
    &Abe::State_94_RunOffEdge_4559A0,
    &Abe::State_95_SneakOffEdge_4559C0,
    &Abe::State_96_HopToFall_4559E0,
    &Abe::State_97_RunJumpFall_455A80,
    &Abe::State_98_RollOffEdge_455AA0,
    &Abe::State_99_LeverUse_455AC0,
    &Abe::State_100_Slap_Bomb_455B60,
    &Abe::State_101_KnockForward_455420,
    &Abe::State_102_455310,
    &Abe::jState_103_KnockbackGetUp_455380,
    &Abe::State_104_RockThrowStandingHold_455DF0,
    &Abe::State_105_RockThrowStandingThrow_456460,
    &Abe::State_106_RockThrowStandingEnd_455F20,
    &Abe::State_107_RockThrowCrouchingHold_454410,
    &Abe::State_108_RockThrowCrouchingThrow_454500,
    &Abe::State_109_455550,
    &Abe::State_110_455670,
    &Abe::State_111_GrabRock_4564A0,
    &Abe::State_112_Chant_45B1C0,
    &Abe::State_113_ChantEnd_45BBE0,
    &Abe::State_114_DoorEnter_459470,
    &Abe::State_115_DoorExit_459A40,
    &Abe::State_116_MineCarEnter_458780,
    &Abe::State_117_In_MineCar_4587C0,
    &Abe::State_118_MineCarExit_458890,
    &Abe::State_119_45A990,
    &Abe::State_120_45AB00,
    &Abe::State_121_LiftGrabBegin_45A600,
    &Abe::State_122_LiftGrabEnd_45A670,
    &Abe::State_123_LiftGrabIdle_45A6A0,
    &Abe::State_124_LiftUseUp_45A780,
    &Abe::State_125_LiftUseDown_45A7B0,
    &Abe::State_126_TurnWheelBegin_456700,
    &Abe::State_127_TurnWheelLoop_456750,
    &Abe::State_128_TurnWheelEnd_4569A0,
    &Abe::State_129_PoisonGasDeath_4565C0
};

const SfxDefinition sAbeSFXList_555250[41] =
{
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 24u, 60u, 115u, 0, 0 },
    { 0u, 24u, 61u, 110u, 0, 0 },
    { 0u, 24u, 62u, 127u, 0, 0 },
    { 0u, 24u, 63u, 90u, 0, 0 },
    { 0u, 24u, 66u, 127u, 0, 0 },
    { 0u, 24u, 67u, 100u, 0, 0 },
    { 0u, 24u, 56u, 127u, 0, 0 },
    { 0u, 24u, 57u, 100u, 0, 0 },
    { 0u, 23u, 48u, 127u, 0, 0 },
    { 0u, 24u, 59u, 127u, 0, 0 },
    { 0u, 24u, 58u, 127u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 24u, 65u, 127u, 0, 0 },
    { 0u, 24u, 64u, 80u, 0, 0 },
    { 0u, 23u, 60u, 90u, 0, 0 },
    { 0u, 23u, 68u, 127u, 0, 0 },
    { 0u, 23u, 69u, 127u, 0, 0 },
    { 0u, 23u, 70u, 127u, 0, 0 },
    { 0u, 23u, 72u, 127u, 0, 0 },
    { 0u, 23u, 73u, 127u, 0, 0 },
    { 0u, 23u, 58u, 127u, 0, 0 },
    { 0u, 23u, 64u, 127u, 0, 0 },
    { 0u, 23u, 66u, 115u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 23u, 63u, 115u, 0, 0 },
    { 0u, 23u, 62u, 90u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};

const int sAbeFrameOffsetTable_554B18[130] =
{
    58888, 58808, 59064, 58888, 58788, 58748, 58768, 59028, 58992, 58956, 58920,
    59112, 270240, 269976, 270024, 269928, 269928, 270092, 270060, 269876, 271080,
    271120, 269688, 269708, 269764, 270860, 270328, 271004, 271056, 270964,
    270468, 269804, 270488, 270748, 270180, 270616, 270180, 270428, 270708, 270252,
    270508, 270688, 270596, 270840, 270728, 270288, 270160, 270308, 270120, 270268,
    270668, 270820, 270408, 270140, 269784, 269856, 270092, 270092, 270092, 270092,
    270092, 270944, 271152, 271192, 271248, 57440, 57580, 57540, 57540, 57712,
    57680, 49740, 49800, 49912, 49856, 20572, 20552, 20552, 20404, 20552, 20552, 20404,
    20552, 20552, 8104, 5724, 16096, 16040, 16220, 16096, 16040, 32012, 32012, 32040,
    32068, 32112, 32168, 32152, 32132, 11396, 6520, 17240, 17180, 58888, 31504, 31392,
    31632, 31576, 31432, 24580, 24628, 6004, 9992, 10040, 19040, 19088, 8540, 8540, 8588,
    8732, 8772, 22548, 22572, 22464, 22596, 22496, 11856, 11816, 11888, 28824
};

const TintEntry sTintTable_Abe_554D20[15] =
{
    { 1, 102u, 102u, 102u },
    { 2, 102u, 102u, 80u },
    { 3, 120u, 90u, 120u },
    { 4, 102u, 70u, 90u },
    { 5, 120u, 102u, 82u },
    { 6, 102u, 102u, 102u },
    { 7, 102u, 70u, 90u },
    { 8, 102u, 102u, 102u },
    { 9, 102u, 102u, 102u },
    { 10, 102u, 102u, 102u },
    { 11, 120u, 90u, 120u },
    { 12, 120u, 102u, 82u },
    { 13, 102u, 102u, 102u },
    { 14, 120u, 90u, 80u },
    { -1, 102u, 102u, 102u }
};

// Frame call backs ??
EXPORT int CC sub_455F40(void*, signed __int16*)
{
    return 1;
}

EXPORT int CC sub_455F60(void*, signed __int16*)
{
    return 1;
}

EXPORT int CC sub_455F80(void*, signed __int16*)
{
    // Does ray casting checks?
    NOT_IMPLEMENTED();
    return 1;
}

EXPORT int CC sub_4561B0(void*, signed __int16*)
{
    // Slurg squish check/particles?
    NOT_IMPLEMENTED();
    return 1;
}

EXPORT int CC sub_434130(void*, signed __int16*)
{
    // ??
    NOT_IMPLEMENTED();
    return 1;
}

const SfxDefinition sSFXList_555160[] =
{
    { 0u, 3u, 69u, 60u, -1, 1 },
    { 0u, 3u, 70u, 60u, -1, 1 },
    { 0u, 3u, 59u, 67u, -1, 1 },
    { 0u, 3u, 61u, 67u, -1, 1 },
    { 0u, 3u, 58u, 52u, -1, 1 },
    { 0u, 3u, 67u, 45u, -1, 1 },
    { 0u, 3u, 57u, 60u, -1, 1 },
    { 0u, 3u, 65u, 50u, -1, 1 },
    { 0u, 3u, 68u, 60u, -1, 1 },
    { 0u, 3u, 72u, 70u, -1, 1 },
    { 0u, 3u, 36u, 50u, -127, 0 },
    { 0u, 3u, 37u, 50u, -127, 0 },
    { 0u, 3u, 38u, 50u, -127, 0 },
    { 0u, 3u, 36u, 55u, 0, 127 },
    { 0u, 3u, 37u, 55u, 0, 127 },
    { 0u, 3u, 38u, 55u, 0, 127 },
    { 0u, 3u, 61u, 50u, -1, 1 },
    { 0u, 3u, 73u, 127u, -1, 1 },
    { 0u, 4u, 80u, 127u, -1, 1 },
    { 0u, 65u, 64u, 60u, -1, 0 },
    { 0u, 11u, 62u, 60u, -1, 1 },
    { 0u, 11u, 64u, 60u, -1, 1 },
    { 0u, 11u, 65u, 50u, -1, 1 },
    { 0u, 11u, 64u, 50u, -1, 1 },
    { 0u, 11u, 66u, 50u, -1, 1 },
    { 0u, 23u, 60u, 90u, 0, 0 },
    { 0u, 23u, 61u, 90u, 0, 0 },
    { 0u, 23u, 62u, 90u, 0, 0 },
    { 0u, 23u, 63u, 90u, 0, 0 },
    { 0u, 23u, 64u, 90u, 0, 0 }
};



EXPORT int CC Abe_SFX_2_457A40(char sfxId, int volume, int pitchMin, BaseAliveGameObject* pAliveObj)
{
    NOT_IMPLEMENTED();

    int sndVolume; // edi

    short sndIndex = 0;

    switch (sfxId)
    {
    case 0:
        sndIndex = 11;
        sndVolume = 40;
        break;
    case 1:
        sndVolume = volume;
        sndIndex = Math_RandomRange_496AB0(10, 12);
        if (volume)
        {
            break;
        }
        sndVolume = Math_RandomRange_496AB0(54, 58);
        if (pAliveObj)
        {
            if (pAliveObj->field_114_flags.Get(Flags_114::e114_Bit8))
            {
                sndVolume *= 3;
            }
        }
        goto LABEL_19;
    case 2:
        sndVolume = volume;
        sndIndex = Math_RandomRange_496AB0(13, 15);
        if (volume)
        {
            break;
        }
        sndVolume = Math_RandomRange_496AB0(66, 70);
        if (pAliveObj)
        {
            if (pAliveObj->field_114_flags.Get(Flags_114::e114_Bit8))
            {
                sndVolume *= 3;
            }
        }
        goto LABEL_19;
    case 3:
        sndIndex = 5;
        break;
    case 4:
        sndIndex = 4;
        break;
    case 5:
        if (volume || !pAliveObj || pAliveObj->field_CC_sprite_scale != FP_FromDouble(0.5))
        {
            return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[2], static_cast<short>(volume), static_cast<short>(pitchMin), 0x7FFF) | 
                SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[16], static_cast<short>(volume), static_cast<short>(pitchMin), 0x7FFF);
        }
        else
        {
            return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[2], sSFXList_555160[2].field_3_default_volume / 2, static_cast<short>(pitchMin), 0x7FFF) |
                SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[16], sSFXList_555160[16].field_3_default_volume / 2, static_cast<short>(pitchMin), 0x7FFF);
        }
    case 6:
        sndIndex = 2;
        break;
    case 7:
        sndIndex = 16;
        break;
    case 8:
        sndIndex = 6;
        break;
    case 9:
    case 0xB:
        if (pAliveObj && pAliveObj->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            return SFX_Play_46FA90(0x20u, 20, 0x10000);
        }
        else
        {
            return SFX_Play_46FA90(0x20u, 35, 0x10000);
        }
    case 0xA:
        sndIndex = 19;
        break;
    case 0xC:
        sndIndex = 25;
        break;
    case 0xD:
        if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1
            || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8
            || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5
            || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_6
            || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_9)
        {
            sndIndex = 2;
        }
        else
        {
            sndIndex = 9;
        }
        break;
    case 0xE:
        sndIndex = 17;
        break;
    case 0xF:
        return 0;
        break;
    default:
        break;
    }

    sndVolume = volume;
LABEL_19:
    if (!sndVolume)
    {
        sndVolume = (char)sSFXList_555160[sndIndex].field_3_default_volume;
    }
    if (!pAliveObj)
    {
        return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(sndVolume), static_cast<short>(pitchMin), 0x7FFF);
    }
    if (pAliveObj->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        sndVolume = 2 * sndVolume / 3;
    }

    if (pAliveObj != sActiveHero_5C1B68)
    {
        switch (gMap_5C3030.sub_4811A0(
            pAliveObj->field_C2_lvl_number,
            pAliveObj->field_C0_path_number,
            pAliveObj->field_B8_xpos,
            pAliveObj->field_BC_ypos))
        {
        case 0:
            return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(sndVolume), static_cast<short>(pitchMin), 0x7FFF);
        case 1:
        case 2:
            return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(2 * sndVolume / 3), static_cast<short>(pitchMin), 0x7FFF);
        case 3:
            return SFX_SfxDefinition_Play_4CA700(
                &sSFXList_555160[sndIndex],
                static_cast<short>(2 * sndVolume / 9),
                static_cast<short>(2 * sndVolume / 9),
                static_cast<short>(pitchMin),
                0x7FFF);
            break;
        case 4:
            return SFX_SfxDefinition_Play_4CA700(
                &sSFXList_555160[sndIndex],
                static_cast<short>(2 * sndVolume / 3),
                static_cast<short>(2 * sndVolume / 3),
                static_cast<short>(pitchMin),
                0x7FFF);
            break;
        default:
            return 0;
            break;
        }
    }
    else
    {
        return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(sndVolume), static_cast<short>(pitchMin), 0x7FFF);
    }
}

using TFrameCallBackType = decltype(&sub_434130);

// TODO: Array is possibly bigger, called by AnimationEx::Invoke_CallBacks_40B7A0
ALIVE_ARY(1, 0x55EF98, TFrameCallBackType, 5, off_55EF98,
{
    sub_455F40,
    sub_455F60,
    sub_455F80,
    sub_4561B0,
    sub_434130
});

ALIVE_VAR(1, 0x5c1bde, WORD, gInfiniteGrenades_5C1BDE, 0);
ALIVE_VAR(1, 0x5c112c, WORD, bThrowableIndicatorExists_5C112C, 0);


enum AbeResources
{
    eAbeBSic = 0,
    eAbeBSic1 = 1,
};

EXPORT int CC GridXMidPos_4498F0(FP scale, int unknown)
{
    if (scale == FP_FromDouble(0.5))
    {
        // 12.5 = half grid size
        return (13 * unknown) - 8;
    }

    if (scale == FP_FromDouble(1.0))
    {
        // 25 = full grid size
        return (25 * unknown) - 13;
    }

    // Default to middle of the screen
    return (374 / 2);
}

ALIVE_VAR(1, 0x5c1b8c, BaseAliveGameObject*, sControlledCharacter_5C1B8C, nullptr);

Abe* Abe::ctor_44AD10(int frameTableOffset, int /*a3*/, int /*a4*/, int /*a5*/)
{
    const int kResourceArraySize = 28;

    ctor_408240(kResourceArraySize);

    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC
    
    field_4_typeId = Types::eType_Abe_69;

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset);
    field_C_objectId = -65536;

    Init_GameStates_43BF40();

    // Zero out the resource array
    for (int i = 0; i < kResourceArraySize; i++)
    {
        field_10_resources_array.SetAt(i, nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC1.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEEDGE.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNFD.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeommResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEOMM.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeommResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNBK.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, TRUE, FALSE);
    }

    ResourceManager::LoadResourceFile_49C170("ABENOELM.BND", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbefallResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbesmashResID);

    ResourceManager::LoadResourceFile_49C170("OMMFLARE.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID);

    ResourceManager::LoadResourceFile_49C170("SQBSMK.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSquibSmokeResID);

    ResourceManager::LoadResourceFile_49C170("DUST.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDustResID);

    ResourceManager::LoadResourceFile_49C170("BLOODROP.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kBloodropResID);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kRockShadowResID);

    ResourceManager::LoadResourceFile_49C170("DEADFLR.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID);

    ResourceManager::LoadResourceFile_49C170("DOVBASIC.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDovbasicResID);

    ResourceManager::LoadResourceFile_49C170("SPOTLITE.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSpotliteResID);

    field_128.field_10 = 1;

    Animation_Init_424E10(
        frameTableOffset,
        135, // Width
        80,  // Height
        field_10_resources_array.ItemAt(AbeResources::eAbeBSic1),
        1,
        1u);

    field_20_animation.field_1C_fn_ptr_array = off_55EF98;

    PSX_Point point = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&point);

    field_B8_xpos = FP_FromInteger(point.field_0_x + GridXMidPos_4498F0(field_CC_sprite_scale, 4));
    field_BC_ypos = FP_FromInteger(point.field_2_y + 120);

    field_F8 = field_BC_ypos;
    field_128.field_8 = FP_FromInteger(0);
    field_128.field_C = 0;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_100_pCollisionLine = nullptr;
    field_106_current_state = eAbeStates::State_3_Fall_459B60;
    field_122 = -1;
    field_20_animation.field_C_render_layer = 32;
    field_198_has_evil_fart = 0;
    field_1A2_rock_or_bone_count = 0;
    field_158_throwable_id = -1;
    field_154_possesed_object_id = -1;
    field_150_OrbWhirlWind_id = -1;
    field_14C_circular_fade_id = -1;
    field_148_pFade = -1;
    field_1A8_portal_id = -1;
    field_164_wheel_id = -1;
    field_160_slapable_or_pick_item_id = -1;
    field_15C_pull_rope_id = -1;

    field_1AE.Clear(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender);
    field_1AE.Clear(Flags_1AE::e1AE_Bit2_bDoQuickSave);

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_1AC_flags.Raw().all = 0;
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6);
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);

    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_bShrivel);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_Fall_To_Well);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit1);


    // Changes Abe's "default" colour depending on the level we are in
    SetTint_425600(&sTintTable_Abe_554D20[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = 0;

    field_118_prev_held = 0;
    field_F6_anim_frame = 0;
    field_120_state = 0;
    field_168_ring_pulse_timer = 0;
    field_16E_bHaveInvisiblity = 0;
    field_170_invisible_timer = 0;
    field_174 = 0;
    field_176 = 0;
    field_178_invisible_effect_id = -1;
    field_124_gnFrame = sGnFrame_5C1B84;
    field_FC_pPathTLV = nullptr;
    field_128.field_12_mood = 0;
    field_128.field_18_say = AbeSay::eNothing;
    field_144_auto_say_timer = 0;

    // Set Abe to be the current player controlled object
    sControlledCharacter_5C1B8C = this;

    // Create shadow
    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    return this;
}

void Abe::dtor_44B380()
{
    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC

    BaseGameObject* pField_148 = sObjectIds_5C1B70.Find_449CF0(field_148_pFade);
    BaseGameObject* pField_14C = sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id);
    BaseGameObject* pField_150 = sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id);
    BaseGameObject* pField_154 = sObjectIds_5C1B70.Find_449CF0(field_154_possesed_object_id);
    BaseGameObject* pField_158 = sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id);
    BaseGameObject* pField_15C = sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id);
    BaseGameObject* pField_160 = sObjectIds_5C1B70.Find_449CF0(field_160_slapable_or_pick_item_id);
    BaseGameObject* pField_178 = sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id);

    SND_SEQ_Stop_4CAE60(0xAu);

    if (pField_148)
    {
        pField_148->field_6_flags.Set(BaseGameObject::eDead);
        field_148_pFade = -1;
    }

    if (pField_160)
    {
        field_160_slapable_or_pick_item_id = -1;
    }

    if (pField_15C)
    {
        pField_15C->field_6_flags.Set(BaseGameObject::eDead);
        field_15C_pull_rope_id = -1;
    }

    if (pField_14C)
    {
        pField_14C->field_6_flags.Set(BaseGameObject::eDead);
        field_14C_circular_fade_id = -1;
    }

    if (pField_150)
    {
        pField_150->field_6_flags.Set(BaseGameObject::eDead);
        field_150_OrbWhirlWind_id = -1;
    }

    if (pField_154)
    {
        field_154_possesed_object_id = -1;
    }

    if (pField_158)
    {
        pField_158->field_6_flags.Set(BaseGameObject::eDead);
        field_158_throwable_id = -1;
    }

    if (pField_178)
    {
        pField_178->field_6_flags.Set(BaseGameObject::eDead);
        field_178_invisible_effect_id = -1;
    }

    field_164_wheel_id = -1;

    sActiveHero_5C1B68 = spAbe_554D5C;

    dtor_4080B0();
}

struct Quicksave_Obj_Abe
{
    BaseGameObject::Types field_0_id;
    __int16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_c_velx;
    FP field_10_vely;
    WORD field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    WORD field_1C_scale;
    WORD field_1e_r;
    WORD field_20_g;
    WORD field_22_b;
    WORD word24;
    WORD word26;
    WORD word28;
    WORD word2A;
    char byte2C;
    char byte2D;
    char byte2E;
    char field_2F;
    FP field_30_health;
    WORD field_34_animation_num;
    WORD word36;
    WORD word38;
    short field_3a_collision_line_id;
    DWORD dword3C;
    WORD word40;
    WORD word42;
    char field_44_is_abe_controlled;
    char field_45;
    __int16 field_46;
    FP dword48;
    DWORD dword4C;
    DWORD dword50;
    DWORD dword54;
    DWORD dword58;
    DWORD dword5C;
    WORD word60;
    AbeSay word62;
    DWORD dword64;
    DWORD dword68;
    char field_6c_rock_bone_count;
    char byte6D;
    char byte6E;
    char byte6F;
    WORD word70;
    WORD word72;
    WORD word74;
    __int16 field_76;
    DWORD dword78;
    DWORD dword7C;
    DWORD dword80;
    DWORD dword84;
    DWORD dword88;
    DWORD dword8C;
    DWORD dword90;
    DWORD dword94;
    DWORD dword98;
    DWORD dword9C;
    WORD wordA0;
    WORD wordA2;
    char byteA4;
    char field_A5;
    __int16 field_A6;
    DWORD dwordA8;
    WORD wordAC;
    WORD wordAE;
    WORD wordB0;
    WORD wordB2;
    WORD wordB4;
    WORD wordB6;
    WORD wordB8;
    WORD wordBA;
    WORD wordBC;
    WORD wordBE;
    WORD wordC0;
    LevelIds wordC2;
    WORD wordC4;
    WORD wordC6;
    WORD wordC8;
    char field_ca_throw_direction;
    char field_CB;
    WORD wordCC;
    __int16 field_CE;
    DWORD dwordD0;
    WORD wordD4;
    WORD wordD6;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_Obj_Abe, 216);

const char* sAbe_ResNames_545830[22] =
{
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

const int sAbeResourceIDTable_554D60[22] =
{
    ResourceID::kAbebasicResID,     // 10
    ResourceID::kAbebsic1ResID,     // 55
    ResourceID::kAbepullResID,      // 11
    ResourceID::kAbepickResID,      // 12
    ResourceID::kAbebombResID,      // 13
    ResourceID::kAbethrowResID,     // 14
    ResourceID::kAbesmashResID,     // 19
    ResourceID::kAbefallResID,      // 20
    ResourceID::kAbestoneResID,     // 21
    ResourceID::kAbeknbkResID,      // 26
    ResourceID::kAbeknfdResID,      // 27
    ResourceID::kAbeknokzResID,     // 28
    ResourceID::kAbehoistResID,     // 42
    ResourceID::kAbeedgeResID,      // 43
    ResourceID::kAbedoorResID,      // 45
    ResourceID::kAbewellResID,      // 47
    ResourceID::kAbeommResID,       // 48
    ResourceID::kAbeliftResID,      // 53
    ResourceID::kAbeCarResId,       // 113
    ResourceID::kAbemorphResID,     // 117
    ResourceID::kAbeworkResID,      // 515
    ResourceID::kAbegasResID,       // 118
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

signed int CC Abe::CreateFromSaveState_44D4F0(const BYTE* pData)
{
    NOT_IMPLEMENTED();

    const Quicksave_Obj_Abe* pSaveState = reinterpret_cast<const Quicksave_Obj_Abe*>(pData);

    Abe* pAbe = sActiveHero_5C1B68;
    if (sActiveHero_5C1B68 == spAbe_554D5C)
    {
        pAbe = alive_new<Abe>();
        if (pAbe)
        {
            pAbe->ctor_44AD10(58808, 85, 57, 55);
        }
        sActiveHero_5C1B68 = pAbe;
    }

    if (pSaveState->field_44_is_abe_controlled)
    {
        sControlledCharacter_5C1B8C = pAbe;
    }

    pAbe->field_FC_pPathTLV = nullptr;
    sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;
    sActiveHero_5C1B68->field_B8_xpos = pSaveState->field_4_xpos;
    sActiveHero_5C1B68->field_BC_ypos = pSaveState->field_8_ypos;
    sActiveHero_5C1B68->field_C4_velx = pSaveState->field_c_velx;
    sActiveHero_5C1B68->field_C8_vely = pSaveState->field_10_vely;
    sActiveHero_5C1B68->field_128.field_8 = pSaveState->dword48;
    sActiveHero_5C1B68->field_128.field_C = pSaveState->dword4C;
    sActiveHero_5C1B68->field_C0_path_number = pSaveState->field_14_path_number;
    sActiveHero_5C1B68->field_C2_lvl_number = pSaveState->field_16_lvl_number;
    sActiveHero_5C1B68->field_CC_sprite_scale = pSaveState->field_18_sprite_scale;
    sActiveHero_5C1B68->field_D6_scale = pSaveState->field_1C_scale;

    sActiveHero_5C1B68->field_114_flags.Clear(Flags_114::e114_Bit7);
    if (pSaveState->word40 & 1)
    {
        sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit7);
    }

    sActiveHero_5C1B68->field_114_flags.Clear(Flags_114::e114_Bit8);
    if (pSaveState->word42 & 1)
    {
        sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit8);
    }

    sActiveHero_5C1B68->field_106_current_state = pSaveState->word26;
    BYTE** animFromState = sActiveHero_5C1B68->StateToAnimResource_44AAB0(sActiveHero_5C1B68->field_106_current_state);
    if (!animFromState)
    {
        DWORD id = sAbeResourceIDTable_554D60[sActiveHero_5C1B68->field_128.field_10];
        ResourceManager::LoadResourceFile_49C170(sAbe_ResNames_545830[sActiveHero_5C1B68->field_128.field_10], 0);
        sActiveHero_5C1B68->field_10_resources_array.SetAt(sActiveHero_5C1B68->field_128.field_10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, id, TRUE, FALSE));
        animFromState = sActiveHero_5C1B68->field_10_resources_array.ItemAt(sActiveHero_5C1B68->field_128.field_10);
    }

    sActiveHero_5C1B68->field_20_animation.Set_Animation_Data_409C80(sAbeFrameOffsetTable_554B18[sActiveHero_5C1B68->field_106_current_state], animFromState);
    sActiveHero_5C1B68->field_20_animation.field_92_current_frame = pSaveState->word28;
    sActiveHero_5C1B68->field_20_animation.field_E_frame_change_counter = pSaveState->word2A;

    sActiveHero_5C1B68->field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    if (pSaveState->word24 & 1)
    {
        sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    sActiveHero_5C1B68->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    if (pSaveState->byte2D & 1)
    {
        sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
    }

    sActiveHero_5C1B68->field_6_flags.Clear(BaseGameObject::eDrawable);
    if (pSaveState->byte2E & 1)
    {
        sActiveHero_5C1B68->field_6_flags.Set(BaseGameObject::eDrawable);
    }

    sActiveHero_5C1B68->field_20_animation.field_C_render_layer = pSaveState->byte2C;
    
    AnimationHeader* pAnimHeader = reinterpret_cast<AnimationHeader*>(&(*sActiveHero_5C1B68->field_20_animation.field_20_ppBlock)[sActiveHero_5C1B68->field_20_animation.field_18_frame_table_offset]);
    if (sActiveHero_5C1B68->field_20_animation.field_92_current_frame == pAnimHeader->field_2_num_frames - 1)
    {
        sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    FrameInfoHeader* pFrameInfoHeader = sActiveHero_5C1B68->field_20_animation.Get_FrameHeader_40B730(-1);
    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*sActiveHero_5C1B68->field_20_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
    sActiveHero_5C1B68->field_20_animation.Load_Pal_40A530(sActiveHero_5C1B68->field_20_animation.field_20_ppBlock, pFrameHeader->field_0_clut_offset);


    sActiveHero_5C1B68->SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.sCurrentLevelId_5C3030);
    sActiveHero_5C1B68->field_20_animation.field_B_render_mode = 0;
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    sActiveHero_5C1B68->field_10C_health = pSaveState->field_30_health;
    sActiveHero_5C1B68->field_106_current_state = pSaveState->field_34_animation_num;
    sActiveHero_5C1B68->field_108_delayed_state = pSaveState->word36;
    sActiveHero_5C1B68->field_F8 = FP_FromInteger(pSaveState->word38);
    sActiveHero_5C1B68->field_110_id = pSaveState->dword3C;
    sActiveHero_5C1B68->field_120_state = static_cast<WORD>(pSaveState->dword50);
    sActiveHero_5C1B68->field_124_gnFrame = pSaveState->dword54;
    sActiveHero_5C1B68->field_128.field_0_gnFrame = pSaveState->dword58;
    sActiveHero_5C1B68->field_128.field_4_regen_health_timer = pSaveState->dword5C;
    sActiveHero_5C1B68->field_128.field_12_mood = pSaveState->word60;
    sActiveHero_5C1B68->field_128.field_18_say = pSaveState->word62;
    sActiveHero_5C1B68->field_144_auto_say_timer = pSaveState->dword64;
    sActiveHero_5C1B68->field_1A2_rock_or_bone_count = pSaveState->field_6c_rock_bone_count;
    sActiveHero_5C1B68->field_168_ring_pulse_timer = pSaveState->dword68;
    sActiveHero_5C1B68->field_16C_bHaveShrykull = pSaveState->byte6E;

    if (sActiveHero_5C1B68->field_168_ring_pulse_timer && sActiveHero_5C1B68->field_16C_bHaveShrykull)
    {
        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(25))
        {
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbemorphResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("SHRYPORT.BND", nullptr);
            }
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSplineResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("SPLINE.BAN", nullptr);
            }
            sActiveHero_5C1B68->Get_Shrykull_Resources_45AA20();
        }
    }
    else
    {
        if (sActiveHero_5C1B68->field_10_resources_array.ItemAt(25))
        {
            sActiveHero_5C1B68->Free_Shrykull_Resources_45AA90();
        }
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_bShrivel);
    if (pSaveState->byte6D & 1)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_bShrivel);
    }

    sActiveHero_5C1B68->field_16E_bHaveInvisiblity = pSaveState->byte6F;
    sActiveHero_5C1B68->field_104 = pSaveState->field_3a_collision_line_id;

    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit9);
    sActiveHero_5C1B68->field_118_prev_held = InputObject::Command_To_Raw_45EE40(pSaveState->word70);
    sActiveHero_5C1B68->field_11C = InputObject::Command_To_Raw_45EE40(pSaveState->word72);
    sActiveHero_5C1B68->field_122 = pSaveState->word74;
    sActiveHero_5C1B68->field_128.field_14 = sGnFrame_5C1B84 - pSaveState->dword78;
    sActiveHero_5C1B68->field_148_pFade = pSaveState->dword7C;
    sActiveHero_5C1B68->field_14C_circular_fade_id = pSaveState->dword80;
    sActiveHero_5C1B68->field_150_OrbWhirlWind_id = pSaveState->dword84;
    sActiveHero_5C1B68->field_154_possesed_object_id = pSaveState->dword88;
    sActiveHero_5C1B68->field_158_throwable_id = pSaveState->dword8C;
    sActiveHero_5C1B68->field_15C_pull_rope_id = pSaveState->dword90;
    sActiveHero_5C1B68->field_160_slapable_or_pick_item_id = pSaveState->dword94;
    sActiveHero_5C1B68->field_164_wheel_id = pSaveState->dword98;
    sActiveHero_5C1B68->field_178_invisible_effect_id = -1;
    sActiveHero_5C1B68->field_170_invisible_timer = pSaveState->dword9C;
    sActiveHero_5C1B68->field_174 = pSaveState->wordA0;
    sActiveHero_5C1B68->field_176 = pSaveState->wordA2;

    sActiveHero_5C1B68->field_17C_cam_idx = pSaveState->byteA4;
    sActiveHero_5C1B68->field_180_stone_type = pSaveState->dwordA8;
    sActiveHero_5C1B68->field_184_fmv_id = pSaveState->wordAC;
    sActiveHero_5C1B68->field_186_to_camera_id[0] = pSaveState->wordAE;
    sActiveHero_5C1B68->field_186_to_camera_id[1] = pSaveState->wordB0;
    sActiveHero_5C1B68->field_186_to_camera_id[2] = pSaveState->wordB2;
    sActiveHero_5C1B68->field_18C_not_used = pSaveState->wordB4;

    sActiveHero_5C1B68->field_18E = pSaveState->wordB6;
    sActiveHero_5C1B68->field_190 = pSaveState->wordB8;
    sActiveHero_5C1B68->field_192 = pSaveState->wordBA;
    sActiveHero_5C1B68->field_194 = pSaveState->wordBC;
    sActiveHero_5C1B68->field_196 = pSaveState->wordBE;
    sActiveHero_5C1B68->field_198_has_evil_fart = pSaveState->wordC0;
    sActiveHero_5C1B68->field_19A_to_level = pSaveState->wordC2;
    sActiveHero_5C1B68->field_19C_to_path = pSaveState->wordC4;
    sActiveHero_5C1B68->field_19E_to_camera = pSaveState->wordC6;
    sActiveHero_5C1B68->field_1A0_door_id = pSaveState->wordC8;
    sActiveHero_5C1B68->field_1A3_throw_direction = pSaveState->field_ca_throw_direction;
    sActiveHero_5C1B68->field_1A4 = pSaveState->wordCC;
    sActiveHero_5C1B68->field_1A8_portal_id = pSaveState->dwordD0;
    
    /* TODO: .. only the last one is going to "win" here.. !?
    // LOWORD(pUnknown->field_20_animation.field_4_flags) ^= ((unsigned __int8)LOWORD(pUnknown->field_20_animation.field_4_flags) ^ LOBYTE(field_20_animation.field_4_flags)) & 0x10;
    // This is doing unknown->flags |= 0x10 if field_20_animation.field_4_flags)) & 0x10 else unsets it
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 1;
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 2;
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 4;
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 8;
    */

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_bShrivel);
    if (pSaveState->wordD4 & 0x10)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_bShrivel);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit6);
    if (pSaveState->wordD4 & 0x20)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit6);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit7);
    if (pSaveState->wordD4 & 0x40)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit9_bLaughAtChantEnd);
    if (pSaveState->wordD4 & 0x80)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_bLaughAtChantEnd);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit12);
    if (pSaveState->wordD4 & 0x100)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit12);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13);
    if (pSaveState->wordD4 & 0x200)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit13);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
    if (pSaveState->wordD4 & 0x400)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit14);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_eBit15_bHaveHealing);
    if (pSaveState->wordD4 & 0x800)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit15_bHaveHealing);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender);
    if (pSaveState->wordD4 & 0xE000)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender);
    }

    sActiveHero_5C1B68->field_114_flags.Clear(Flags_114::e114_Bit10);
    if ((pSaveState->wordD4 >> 3) & 0x200)
    {
        sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit10);
    }

    sActiveHero_5C1B68->field_1AE.Clear(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender);
    if ((pSaveState->wordD4 >> 14) & 1)
    {
        sActiveHero_5C1B68->field_1AE.Set(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender);
    }

    // TODO: Move to shadow object
    sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit2);
    if ((pSaveState->wordD4 >> 14) & 2)
    {
        sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit2);
    }

    /* TODO: Will wipe out previously set bits ??
    LOWORD(sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags) ^= ((unsigned __int8)LOWORD(sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags) ^ LOBYTE(pSaveState->wordD6)) & 1;
    sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags &= ~1;
    if ((pSaveState->wordD6) & 1)
    {
        sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags |= 1;
    }
    */

    if (sActiveHero_5C1B68->field_198_has_evil_fart)
    {
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 6017, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("EVILFART.BAN", nullptr);
        }

        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(22))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(22, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 6017, TRUE, FALSE)); // TODO: ResId
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
        }

        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(24))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(24, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, TRUE, FALSE));
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
        }

        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(23))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(23, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, TRUE, FALSE));
        }
    }

    return sizeof(Quicksave_Obj_Abe);
}

BaseGameObject* Abe::VDestructor(signed int flags)
{
    return vdtor_44B350(flags);
}

void Abe::VUpdate()
{
    Update_449DC0();
}

void Abe::VRender(int** pOrderingTable)
{
    vRender_44B580(pOrderingTable);
}

void Abe::VScreenChanged()
{
    vScreenChanged_44D240();
}

int Abe::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_457110(pSaveBuffer);
}

__int16 Abe::VTakeDamage_408730(BaseAnimatedWithPhysicsGameObject* pFrom)
{
    return vTakeDamage_44BB50(pFrom);
}

__int16 Abe::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    return vOn_TLV_Collision_44B5D0(pTlv);
}

BaseGameObject* Abe::Vsub_408FD0(__int16 a2)
{
    return vsub_44E970(a2);
}

void Abe::Vnull_4081F0()
{
    vsub_45A570();
}

BaseGameObject* Abe::vdtor_44B350(signed int flags)
{
    dtor_44B380();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

const FP sAbe_xVel_table_545770[8] = 
{
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4)
};

const FP sAbe_yVel_table_545790[8] =
{
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4)
};


const InputCommands sInputKey_Right_5550D0 = eRight;
const InputCommands sInputKey_Left_5550D4 = eLeft;
const InputCommands sInputKey_Up_5550D8 = eUp;
const InputCommands sInputKey_Down_5550DC = eDown;
const InputCommands sInputKey_Hop_5550E0 = eHop;
const InputCommands sInputKey_DoAction_5550E4 = eDoAction;
const InputCommands sInputKey_Run_5550E8 = eRun;
const InputCommands sInputKey_Sneak_5550EC = eSneak;
const InputCommands sInputKey_FartRoll_5550F0 = eFartOrRoll;
const InputCommands sInputKey_ThrowItem_5550F4 = eThrowItem;
const InputCommands sInputKey_GameSpeak2_5550F8 = eGameSpeak2;
const InputCommands sInputKey_GameSpeak4_5550FC = eGameSpeak4;
const InputCommands sInputKey_GameSpeak3_555100 = eGameSpeak3;
const InputCommands sInputKey_GameSpeak1_555104 = eGameSpeak1;
const InputCommands sInputKey_GameSpeak6_555108 = eGameSpeak6;
const InputCommands sInputKey_GameSpeak5_55510C = eGameSpeak5;
const InputCommands sInputKey_GameSpeak8_555110 = eGameSpeak8;
const InputCommands sInputKey_GameSpeak7_555114 = eGameSpeak7;


ALIVE_VAR(1, 0x5c1bda, short, word_5C1BDA, 0);


void Abe::Update_449DC0()
{
    if (word_5C1BDA) // Some flag to reset HP?
    {
        field_114_flags.Clear(Flags_114::e114_Bit7);
        field_10C_health = FP_FromDouble(1.0);
    }

    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104 != -1)
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104);

            field_104 = -1;
        }

        field_110_id = BaseGameObject::Find_Flags_4DC170(field_110_id);
        field_148_pFade = BaseGameObject::Find_Flags_4DC170(field_148_pFade);
        field_14C_circular_fade_id = BaseGameObject::Find_Flags_4DC170(field_14C_circular_fade_id);
        field_1A8_portal_id = BaseGameObject::Find_Flags_4DC170(field_1A8_portal_id);
        field_150_OrbWhirlWind_id = BaseGameObject::Find_Flags_4DC170(field_150_OrbWhirlWind_id);
        field_154_possesed_object_id = BaseGameObject::Find_Flags_4DC170(field_154_possesed_object_id);
        field_158_throwable_id = BaseGameObject::Find_Flags_4DC170(field_158_throwable_id);
        field_15C_pull_rope_id = BaseGameObject::Find_Flags_4DC170(field_15C_pull_rope_id);
        field_160_slapable_or_pick_item_id = BaseGameObject::Find_Flags_4DC170(field_160_slapable_or_pick_item_id);
        field_164_wheel_id = BaseGameObject::Find_Flags_4DC170(field_164_wheel_id);

        if (field_114_flags.Get(Flags_114::e114_Bit8))
        {
            if (!field_170_invisible_timer)
            {
                field_170_invisible_timer = sGnFrame_5C1B84 + 2;
            }

            auto pClass = alive_new<InvisibleEffect>();
            pClass->ctor_45F280(this);
            field_178_invisible_effect_id = pClass->field_8_object_id;
            pClass->sub_45FA00();
        }
    }

    if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit12))
    {
        return;
    }

    if (word_5C1BDA)
    {
        field_10C_health = FP_FromDouble(1.0);
    }

    if (!Input_IsChanting_45F260())
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit6);
        field_174 = 1;
    }

    const int totalAliveSavedMuds = sRescuedMudokons_5C1BC2 - sKilledMudokons_5C1BC0;
    if (totalAliveSavedMuds > 0)
    {
        // "Happy" voice
        field_128.field_12_mood = 5;
    }
    else if (totalAliveSavedMuds < 0)
    {
        // "Sad" voice
        field_128.field_12_mood = 3;
    }
    else
    {
        // "Normal" voice
        field_128.field_12_mood = 0;
    }

    // Handle DDCheat mode
    if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
    {
        Vnull_4081F0();

        field_F8 = field_BC_ypos;
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_bShrivel);
        field_106_current_state = eAbeStates::jState_85_Fall_455070;
        field_100_pCollisionLine = nullptr;

        if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (eRight | eLeft | eDown | eUp))
        {
            field_C4_velx = sAbe_xVel_table_545770[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            field_C8_vely = sAbe_yVel_table_545790[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];

            if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                field_C4_velx += sAbe_xVel_table_545770[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                field_C4_velx += sAbe_xVel_table_545770[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                field_C8_vely += sAbe_yVel_table_545790[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            }

            field_B8_xpos += field_C4_velx;
            field_BC_ypos += field_C8_vely;

            // Keep within map max min bounds
            if (field_B8_xpos < FP_FromInteger(0))
            {
                field_B8_xpos = FP_FromInteger(0);
            }

            if (field_BC_ypos < FP_FromInteger(0))
            {
                field_BC_ypos = FP_FromInteger(0);
            }

            // Keep within map max bounds
            PSX_Point mapSize = {};
            gMap_5C3030.Get_map_size_480640(&mapSize);

            FP mapWidth = FP_FromInteger(mapSize.field_0_x);
            if (field_B8_xpos >= mapWidth)
            {
                field_B8_xpos = mapWidth - FP_FromDouble(1.0);
            }

            FP mapHeight = FP_FromInteger(mapSize.field_2_y);
            if (field_BC_ypos >= mapHeight)
            {
                field_BC_ypos = mapHeight - FP_FromDouble(1.0);
                sub_408C40();
                return;
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
        }

        sub_408C40();
    }
    else
    {
        // Handle none DDCheat mode

        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

        short state_idx = field_106_current_state;

        // Execute the current state
        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;
        (this->*(sAbeStateMachineTable_554910)[state_idx])();

        if (field_114_flags.Get(Flags_114::e114_Bit9) || field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_bShrivel))
        {
            return;
        }

        if (field_100_pCollisionLine)
        {
            // Snap to a whole number so we are "on" the line
            field_BC_ypos = FP_NoFractional(field_BC_ypos);
        }

        // Did position change?
        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            // Get the TLV we are on
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        if (field_114_flags.Get(Flags_114::e114_Bit1))
        {
            state_idx = field_122;
            ToKnockback_44E700(1, 1);
            if (state_idx != -1)
            {
                field_106_current_state = state_idx;
            }

            field_108_delayed_state = 0;
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2);
            field_122 = eAbeStates::State_0_Idle_44EEB0;
            field_114_flags.Clear(Flags_114::e114_Bit1);
            field_114_flags.Set(Flags_114::e114_Bit2);
        }

        if (Event_Get_422C00(kEventScreenShake) && field_10C_health > FP_FromInteger(0))
        {
            if (IsStanding_449D30())
            {
                ToKnockback_44E700(1, 0);
            }
        }

        if (field_128.field_18_say != AbeSay::eNothing && static_cast<int>(sGnFrame_5C1B84) >= field_144_auto_say_timer)
        {
            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0)
                || (field_106_current_state == eAbeStates::State_112_Chant_45B1C0)
                || field_106_current_state == eAbeStates::State_7_Speak_45B140
                || field_106_current_state == eAbeStates::State_8_Speak_45B160
                || field_106_current_state == eAbeStates::State_9_SpeakMovement_45B180
                || field_106_current_state == eAbeStates::State_10_Fart_45B1A0)
            {
                // Prevents double laugh when blowing up a slig as we prevent saying anything while chanting or various other states
                // or when abe isn't in the active screen
                field_128.field_18_say = AbeSay::eNothing;
            }
            else
            {
                if (field_106_current_state == eAbeStates::State_0_Idle_44EEB0 || field_106_current_state == eAbeStates::State_12_Null_4569C0)
                {
                    field_114_flags.Set(Flags_114::e114_Bit2);
                    switch (field_128.field_18_say)
                    {
                    case AbeSay::eOops_14:
                        field_106_current_state = eAbeStates::State_34_DunnoBegin_44ECF0;
                        break;
                    case AbeSay::eAnger_5:
                        field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
                        break;
                    case AbeSay::eSympathy_28:
                        field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
                        break;
                    default:
                        field_106_current_state = eAbeStates::State_9_SpeakMovement_45B180;
                        break;
                    }
                }

                if (field_128.field_18_say == AbeSay::eAnger_5)
                {
                    // Other evil muds laugh at the abe grr
                    Event_Broadcast_422BC0(kEventMudokonLaugh, sActiveHero_5C1B68);
                }

                if (field_128.field_18_say == AbeSay::eSympathy_28)
                {
                    // This one has another volume for whatever reason
                    Abe_SFX_457EC0(static_cast<unsigned char>(field_128.field_18_say), 80, 0, this);
                }
                else
                {
                    Abe_SFX_457EC0(static_cast<unsigned char>(field_128.field_18_say), 0, 0, this);
                }

                field_128.field_18_say = AbeSay::eNothing;
            }
        }

        if (state_idx != field_106_current_state || field_114_flags.Get(Flags_114::e114_Bit2))
        {
            field_114_flags.Clear(Flags_114::e114_Bit2);
            if (field_106_current_state != eAbeStates::State_12_Null_4569C0 && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_bShrivel)))
            {
                field_20_animation.Set_Animation_Data_409C80(
                    sAbeFrameOffsetTable_554B18[field_106_current_state],
                    StateToAnimResource_44AAB0(field_106_current_state));

                field_128.field_14 = sGnFrame_5C1B84;

                if (state_idx == eAbeStates::State_12_Null_4569C0 || state_idx == eAbeStates::State_60_4A3200)
                {
                    field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                }
            }
        }

        if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit2))
        {
            field_106_current_state = field_F4;
            field_20_animation.Set_Animation_Data_409C80(
                sAbeFrameOffsetTable_554B18[field_106_current_state],
                StateToAnimResource_44AAB0(field_106_current_state));

            field_128.field_14 = sGnFrame_5C1B84;
            field_20_animation.SetFrame_409D50(field_F6_anim_frame);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2);
        }

        if (field_128.field_4_regen_health_timer <= static_cast<int>(sGnFrame_5C1B84) && field_10C_health > FP_FromInteger(0))
        {
            field_10C_health = FP_FromDouble(1.0);
        }

        // Draw power up ring "pulse"
        if (field_168_ring_pulse_timer > 0)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    if (field_168_ring_pulse_timer > static_cast<int>(sGnFrame_5C1B84))
                    {
                        if (!(sGnFrame_5C1B84 % 32))
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
                            else if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_bHaveHealing))
                            {
                                ringType = RingTypes::eHealing_Pulse_14;
                            }

                            PSX_RECT rect = {};
                            GetBoundingRect_424FD0(&rect, 1);
                            AbilityRing::Factory_482F80(
                                FP_FromInteger((rect.x + rect.w) / 2),
                                FP_FromInteger((rect.y + rect.h) / 2),
                                ringType, field_CC_sprite_scale);

                            SFX_Play_46FBA0(0x11u, 25, 2650, 0x10000);
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


        InvisibleEffect* pObj_field_178 = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
        if (pObj_field_178 && field_170_invisible_timer > 0)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_170_invisible_timer)
            {
                field_170_invisible_timer = 0;
                pObj_field_178->sub_45FA30();
            }
        }

        // After the trials give abe the healing power for the necrum muds
        if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender))
        {
            if (field_1AE.Get(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender))
            {
                if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eNecrum_2)
                {
                    field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
                    field_16C_bHaveShrykull = 0;
                    field_16E_bHaveInvisiblity = 0;
                    field_1AE.Clear(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender);
                    field_1AC_flags.Clear(Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender);
                    field_1AC_flags.Set(Flags_1AC::e1AC_eBit15_bHaveHealing);
                }
            }
        }

        if (Event_Get_422C00(kEventMudokonDied))
        {
            field_128.field_18_say = AbeSay::eOops_14;
            field_144_auto_say_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);

            // Do the death jingle
            alive_new<MusicTrigger>()->ctor_47FF10(1, 0, 90, 0);
        }

        if (Event_Get_422C00(kEventMudokonComfort))
        {
            field_128.field_18_say = AbeSay::eLaugh_8;
            field_144_auto_say_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        }

        if (Event_Get_422C00(kEventMudokonComfort | kEventSpeaking))
        {
            field_128.field_18_say = AbeSay::eOops_14;
            field_144_auto_say_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        }

        if (field_1AE.Get(Flags_1AE::e1AE_Bit2_bDoQuickSave))
        {
            field_1AE.Clear(Flags_1AE::e1AE_Bit2_bDoQuickSave);
            sActiveQuicksaveData_BAF7F8.field_204_world_info.field_A_unknown_1 = static_cast<short>(field_1B0_save_num);
            Quicksave_SaveWorldInfo_4C9310(&sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info);
            vGetSaveState_457110(reinterpret_cast<BYTE*>(&sActiveQuicksaveData_BAF7F8.field_284_restart_path_abe_state));
            sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states = sSwitchStates_5C1A28;
            Quicksave_4C90D0();
        }
    }
}

BaseGameObject* Abe::vsub_44E970(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void Abe::vsub_45A570()
{
    // Handles falling when previously was on a platform, stop turning a wheel if we where turning one etc.
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id));
    if (pPlatform)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_bShrivel)))
        {
            VUpdateState_4081C0(eAbeStates::State_93_FallLedgeBegin_455970);
        }

        pPlatform->VRemove(this);

        field_110_id = -1;
        field_F8 = field_BC_ypos;

        if (pWheel)
        {
            pWheel->VStopTurning(FALSE);
        }
    }
}

void Abe::ToKnockback_44E700(__int16 bUnknownSound, __int16 bDelayedAnger)
{
    OrbWhirlWind* pfield_150 = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));
    BaseThrowable* pfield_158 = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id));
    WorkWheel* pfield_164 = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id));
    if (sControlledCharacter_5C1B8C == this || field_10C_health <= FP_FromInteger(0))
    {
        // Chant music/orb kill ?
        SND_SEQ_Stop_4CAE60(10u);
        if (pfield_150)
        {
            pfield_150->sub_4E4050();
            field_150_OrbWhirlWind_id = -1;
        }

        if (pfield_164)
        {
            pfield_164->VStopTurning(1);
            field_164_wheel_id = -1;
        }

        if (field_C4_velx != FP_FromInteger(0))
        {
            field_B8_xpos -= field_C4_velx;
        }

        MapFollowMe_408D10(TRUE);

        field_C4_velx = FP_FromInteger(0);

        if (field_C8_vely < FP_FromInteger(0))
        {
            field_C8_vely = FP_FromInteger(0);
        }

        if (bUnknownSound)
        {
            Abe_SFX_457EC0(9u, 0, Math_RandomRange_496AB0(-127, 127), this);
            Abe_SFX_2_457A40(13, 0, 32767, this);
        }

        field_106_current_state = eAbeStates::State_71_Knockback_455090;

        if (bDelayedAnger)
        {
            field_128.field_18_say = AbeSay::eAnger_5; // anger in..
            field_144_auto_say_timer = sGnFrame_5C1B84 + 27; // 27 ticks
        }

        if (pfield_158)
        {
            pfield_158->Vsub_4114B0();
            field_158_throwable_id = -1;
            if (!gInfiniteGrenades_5C1BDE)
            {
                field_1A2_rock_or_bone_count++;
            }
        }
    }
}

void Abe::vRender_44B580(int** pOrderingTable)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_bShrivel)))
    {
        field_20_animation.field_14_scale = field_CC_sprite_scale;
    }

    if (field_106_current_state != eAbeStates::State_79_Inside_Of_A_Well_Local_45CA60 && field_106_current_state != eAbeStates::State_82_Inside_Of_A_Well_Express_45CC80 && field_106_current_state != eAbeStates::State_76_45CA40)
    {
        Render_424B90(pOrderingTable);
    }
}

void Abe::vScreenChanged_44D240()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        field_C2_lvl_number = gMap_5C3030.field_A_5C303A_levelId;
        field_C0_path_number = gMap_5C3030.field_C_5C303C_pathId;
    }

    // Level has changed?
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        if (gMap_5C3030.field_A_5C303A_levelId == LevelIds::eMines_1 && !word_5C1BA0)
        {
            field_128.field_18_say = AbeSay::e3;
            field_144_auto_say_timer = sGnFrame_5C1B84 + 35;
        }

        // Set the correct tint for this map
        SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.field_A_5C303A_levelId);

        if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eNone)
        {
            if (field_1A2_rock_or_bone_count > 0)
            {
                if (gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C->Remove_49AA00(field_1A2_rock_or_bone_count);
                }
            }
            
            field_1A2_rock_or_bone_count = 0;
            
            if (field_168_ring_pulse_timer > 0 && field_16C_bHaveShrykull)
            {
                Free_Shrykull_Resources_45AA90();
            }

            field_168_ring_pulse_timer = 0;
        }

        if (gMap_5C3030.field_A_5C303A_levelId == LevelIds::eNecrum_2)
        {
            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudancheeVault_Ender_7)
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender);
            }

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudomoVault_Ender_11)
            {
                field_1AE.Set(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender);
            }
        }

        if (gMap_5C3030.field_A_5C303A_levelId == LevelIds::eCredits_16 || gMap_5C3030.field_A_5C303A_levelId == LevelIds::eMenu_0)
        {
            // Remove Abe for menu/credits levels?
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_168_ring_pulse_timer = 0;
        if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eNone)
        {
            field_198_has_evil_fart = 0;
        }
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId && !(field_114_flags.Get(Flags_114::e114_Bit9)))
    {
        for (BYTE& val : sSavedKilledMudsPerPath_5C1B50.mData)
        {
            val = 0;
        }
        byte_5C1B64 = 0;
    }
}

int Abe::vGetSaveState_457110(BYTE* pSaveBuffer)
{
    NOT_IMPLEMENTED();

    Quicksave_Obj_Abe* pSaveState = reinterpret_cast<Quicksave_Obj_Abe*>(pSaveBuffer);

    pSaveState->field_0_id = Types::eType_Abe_69;
    pSaveState->field_4_xpos = field_B8_xpos;
    pSaveState->field_8_ypos = field_BC_ypos;
    pSaveState->field_c_velx = field_C4_velx;
    pSaveState->field_10_vely = field_C8_vely;
    pSaveState->dword48 = field_128.field_8;
    pSaveState->dword4C = field_128.field_C;
    pSaveState->field_14_path_number = field_C0_path_number;
    pSaveState->field_16_lvl_number = field_C2_lvl_number;
    pSaveState->field_18_sprite_scale = field_CC_sprite_scale;
    pSaveState->field_1C_scale = field_D6_scale;
    pSaveState->field_1e_r = field_D0_r;
    pSaveState->field_20_g = field_D2_g;
    pSaveState->field_22_b = field_D4_b;

    if (field_114_flags.Get(Flags_114::e114_Bit11))
    {
        for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            /* TODO
            if (pObj->field_4_typeId == 150)
            {
                if (v5[1].field_0_VTbl == field_8_object_id)
                {
                    pSaveState->field_1e_r = v5[1].field_4_typeId;
                    pSaveState->field_20_g = v5[1].field_6_flags;
                    pSaveState->field_22_b = v5[1].field_8_flags_ex;
                    break;
                }
            }*/
        }
    }

    //pSaveState->word24 = (field_20_animation.field_4_flags) >> 4) & 1;
    pSaveState->word26 = field_106_current_state;
    pSaveState->word28 = field_20_animation.field_92_current_frame;
    pSaveState->word2A = field_20_animation.field_E_frame_change_counter;
    
    if (!field_20_animation.field_E_frame_change_counter)
    {
        pSaveState->word2A = 1;
    }

    //pSaveState->byte2E = (LOBYTE(this->field_6_flags) >> 3) & 1;
    //pSaveState->byte2D = (LOBYTE(this->field_20_animation.field_4_flags) >> 2) & 1;
    pSaveState->byte2C = static_cast<char>(field_20_animation.field_C_render_layer);
    pSaveState->field_30_health = field_10C_health;
    pSaveState->field_34_animation_num = field_106_current_state;
    pSaveState->word36 = field_108_delayed_state;
    pSaveState->word38 = FP_GetExponent(field_F8);
    //pSaveState->word40 = (LOBYTE(this->field_114_flags) >> 6) & 1;
    //pSaveState->word42 = (unsigned __int8)(LOBYTE(this->field_114_flags) >> 7);

    if (field_100_pCollisionLine)
    {
        pSaveState->field_3a_collision_line_id = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pSaveState->field_3a_collision_line_id = -1;
    }
    
    pSaveState->dword3C = field_110_id;

    if (field_110_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj)
        {
            pSaveState->dword3C = pObj->field_C_objectId;
        }
    }

    pSaveState->field_44_is_abe_controlled = (this == sControlledCharacter_5C1B8C);
    pSaveState->dword50 = field_120_state;
    pSaveState->dword54 = field_124_gnFrame;
    pSaveState->dword58 = field_128.field_0_gnFrame;
    pSaveState->dword5C = field_128.field_4_regen_health_timer;
    pSaveState->word60 = field_128.field_12_mood;
    pSaveState->word62 = field_128.field_18_say;
    pSaveState->dword64 = field_144_auto_say_timer;
    pSaveState->dword68 = field_168_ring_pulse_timer;
    pSaveState->field_6c_rock_bone_count = field_1A2_rock_or_bone_count;
    //pSaveState->byte6D = (LOBYTE(this->field_1AC_flags) >> 4) & 1;
    pSaveState->byte6E = static_cast<char>(field_16C_bHaveShrykull);
    pSaveState->byte6F = static_cast<char>(field_16E_bHaveInvisiblity);
    //pSaveState->word70 = sub_45EF70(field_118_prev_held);
    //pSaveState->word72 = sub_45EF70(field_11C);
    pSaveState->word74 = field_122;
    pSaveState->dword78 = sGnFrame_5C1B84 - field_128.field_14;
    pSaveState->dword7C = field_148_pFade;
    
    if (field_148_pFade != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_148_pFade);
        if (pObj)
        {
            pSaveState->dword7C = pObj->field_C_objectId;
        }
    }

    pSaveState->dword80 = field_14C_circular_fade_id;

    if (field_14C_circular_fade_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id);
        if (pObj)
        {
            pSaveState->dword80 = pObj->field_C_objectId;
        }
    }
    pSaveState->dword84 = field_150_OrbWhirlWind_id;

    if (field_150_OrbWhirlWind_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id);
        if (pObj)
        {
            pSaveState->dword84 = pObj->field_C_objectId;
        }
    }
    
    pSaveState->dword88 = field_154_possesed_object_id;

    if (field_154_possesed_object_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_154_possesed_object_id);
        if (pObj)
        {
            pSaveState->dword88 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword8C = field_158_throwable_id;

    if (field_158_throwable_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id);
        if (pObj)
        {
            pSaveState->dword8C = pObj->field_C_objectId;
        }
    }

    pSaveState->dword90 = field_15C_pull_rope_id;

    if (field_15C_pull_rope_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id);
        if (pObj)
        {
            pSaveState->dword90 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword94 = field_160_slapable_or_pick_item_id;

    if (field_160_slapable_or_pick_item_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_160_slapable_or_pick_item_id);
        if (pObj)
        {
            pSaveState->dword94 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword98 = field_164_wheel_id;

    if (field_164_wheel_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id);
        if (pObj)
        {
            pSaveState->dword98 = pObj->field_C_objectId;
        }
    }

    pSaveState->dwordD0 = field_1A8_portal_id;

    if (field_1A8_portal_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_1A8_portal_id);
        if (pObj)
        {
            pSaveState->dwordD0 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword9C = field_170_invisible_timer;
    pSaveState->wordA0 = field_174;
    pSaveState->wordA2 = field_176;
    pSaveState->byteA4 = field_17C_cam_idx;
    pSaveState->dwordA8 = field_180_stone_type;
    pSaveState->wordAC = field_184_fmv_id;
    pSaveState->wordAE = field_186_to_camera_id[0];
    pSaveState->wordB0 = field_186_to_camera_id[1];
    pSaveState->wordB2 = field_186_to_camera_id[2];
    pSaveState->wordB4 = field_18C_not_used;
    pSaveState->wordB6 = field_18E;
    pSaveState->wordB8 = field_190;
    pSaveState->wordBA = field_192;
    pSaveState->wordBC = field_194;
    pSaveState->wordBE = field_196;
    pSaveState->wordC0 = field_198_has_evil_fart;
    pSaveState->wordC2 = field_19A_to_level;
    pSaveState->wordC4 = field_19C_to_path;
    pSaveState->wordC6 = field_19E_to_camera;
    pSaveState->wordC8 = field_1A0_door_id;
    pSaveState->field_ca_throw_direction = field_1A3_throw_direction;
    pSaveState->wordCC = field_1A4;

    /*
    v17 = pSaveState->wordD4 ^ (LOBYTE(this->field_1AC_flags) ^ (unsigned __int8)pSaveState->wordD4) & 1;
    pSaveState->wordD4 = v17;
    v18 = v17 ^ (LOBYTE(this->field_1AC_flags) ^ (unsigned __int8)v17) & 2;
    pSaveState->wordD4 = v18;
    v19 = v18 ^ (LOBYTE(this->field_1AC_flags) ^ (unsigned __int8)v18) & 4;
    pSaveState->wordD4 = v19;
    v20 = v19 ^ (LOBYTE(this->field_1AC_flags) ^ (unsigned __int8)v19) & 8;
    pSaveState->wordD4 = v20;
    v21 = v20 & ~0x10 | (LOBYTE(this->field_1AC_flags) >> 1) & 0x10;
    pSaveState->wordD4 = v21;
    v22 = v21 & ~0xFFFF0020;
    v23 = v22 | (LOBYTE(this->field_1AC_flags) >> 1) & 0x20;
    pSaveState->wordD4 = v23;
    v24 = v23 & ~0xFFFF0040;
    v25 = v24 | (LOBYTE(this->field_1AC_flags) >> 1) & 0x40;
    pSaveState->wordD4 = v25;
    LOWORD(v22) = this->field_1AC_flags;
    LOWORD(v22) = v25 & ~0x80 | (v22 >> 1) & 0x80;
    pSaveState->wordD4 = v22;
    LOWORD(v24) = this->field_1AC_flags;
    v26 = v22 & ~0xFFFF0100;
    LOWORD(v24) = v26 | (v24 >> 3) & 0x100;
    pSaveState->wordD4 = v24;
    LOWORD(v26) = this->field_1AC_flags;
    v27 = v24 & ~0xFFFF0200;
    LOWORD(v26) = v27 | (v26 >> 3) & 0x200;
    pSaveState->wordD4 = v26;
    LOWORD(v27) = this->field_1AC_flags;
    v28 = v26 & ~0xFFFF0400;
    LOWORD(v27) = v28 | (v27 >> 3) & 0x400;
    pSaveState->wordD4 = v27;
    LOWORD(v28) = this->field_1AC_flags;
    LOWORD(v28) = v27 & ~0x800 | (v28 >> 3) & 0x800;
    pSaveState->wordD4 = v28;
    v29 = v28 & ~0xFFFF1000;
    LOWORD(v27) = v29 | 8 * (this->field_114_flags & 0x200);
    pSaveState->wordD4 = v27;
    LOWORD(v29) = this->field_1AC_flags;
    LOWORD(v29) = v27 & ~0x2000 | (v29 >> 2) & 0x2000;
    pSaveState->wordD4 = v29;
    LOWORD(v27) = v29 & ~0x4000 | ((this->field_1AE & 1) << 14);
    pSaveState->wordD4 = v27;
    pSaveState->wordD4 = (((LOBYTE(this->field_E0_176_ptr->field_14_flags) >> 1) & 1) << 15) | v27 & ~0x8000u;
    pSaveState->wordD6 ^= (this->field_E0_176_ptr->field_14_flags & 1 ^ (unsigned __int8)pSaveState->wordD6) & 1;
    */
    return sizeof(Quicksave_Obj_Abe);
}

__int16 Abe::vTakeDamage_44BB50(BaseAnimatedWithPhysicsGameObject* pFrom)
{
    // Stop chant sound music
    SND_SEQ_Stop_4CAE60(10u);
    
    const AbeSay oldSay = field_128.field_18_say;
    field_128.field_18_say = AbeSay::eNothing;

    OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));
    if (pWhirlWind)
    {
        pWhirlWind->sub_4E4050();
        field_150_OrbWhirlWind_id = -1;
    }

    if (CantBeDamaged_44BAB0())
    {
        return 0;
    }

    if (word_5C1BDA)
    {
        return 0;
    }

    if (field_114_flags.Get(Flags_114::e114_Bit10))
    {
        return 0;
    }

    field_128.field_4_regen_health_timer = sGnFrame_5C1B84 + 180;
    __int16 ret = field_10C_health > FP_FromInteger(0);

    switch (pFrom->field_4_typeId)
    {
    case Types::eGasClock_23:
        if (field_10C_health > FP_FromInteger(0))
        {
            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            if (IsStanding_449D30())
            {
                field_114_flags.Set(Flags_114::e114_Bit2);
                field_10C_health = FP_FromInteger(0);
                field_106_current_state = eAbeStates::State_129_PoisonGasDeath_4565C0;
                field_124_gnFrame = 1;
            }
            else
            {
                if (field_106_current_state != eAbeStates::State_71_Knockback_455090 &&
                    field_106_current_state != eAbeStates::State_72_KnockbackGetUp_455340)
                {
                    ToKnockback_44E700(1, 1);
                    field_114_flags.Set(Flags_114::e114_Bit2);
                }
            }
        }
        break;

    case Types::eGrinder_30:
    {
        if (field_10C_health <= FP_FromInteger(0))
        {
            return 0;
        }

        field_114_flags.Set(Flags_114::e114_Bit2);
        field_10C_health = FP_FromInteger(0);
        ToKnockback_44E700(1, 1);

        field_D4_b = 30;
        field_D2_g = 30;
        field_D0_r = 30;

        auto pGibs = alive_new<Gibs>();
        pGibs->ctor_40FB40(
            0,
            field_B8_xpos,
            field_BC_ypos,
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_CC_sprite_scale,
            0);

        // Note Check on word_5CC88C <= 3846 appeared always true, removed.
        auto pMoreGibs = alive_new<Gibs>();
        pMoreGibs->ctor_40FB40(
            0,
            field_B8_xpos,
            field_BC_ypos,
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_CC_sprite_scale,
            0);

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
        break;

    case Types::eElectricWall_39:
        Abe_SFX_457EC0(14u, 0, 0, this); // Say Oops
        break;

    case Types::eBaseBomb_46:
    case Types::eType_89:
    case Types::eExplosion_109:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_114_flags.Set(Flags_114::e114_Bit1);
                field_122 = eAbeStates::State_71_Knockback_455090;
                field_108_delayed_state = eAbeStates::State_71_Knockback_455090;
            }

            field_10C_health = FP_FromInteger(0);

            field_D4_b = 30;
            field_D2_g = 30;
            field_D0_r = 30;

            auto pGibs = alive_new<Gibs>();
            pGibs->ctor_40FB40(
                0,
                field_B8_xpos,
                field_BC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit2);
        }
        break;

    case Types::eType_47:
    case Types::eAntiChant_83:
        field_128.field_18_say = AbeSay::eAnger_5;
        field_144_auto_say_timer = sGnFrame_5C1B84 + 27;
        if (field_106_current_state != eAbeStates::State_123_LiftGrabIdle_45A6A0 &&
            field_106_current_state != eAbeStates::State_124_LiftUseUp_45A780 &&
            field_106_current_state != eAbeStates::State_125_LiftUseDown_45A7B0)
        {
            ToKnockback_44E700(1, 1);
            field_114_flags.Set(Flags_114::e114_Bit2);
        }

        // The zap makes Abe drop his stuff everywhere
        for (int i = 0; i < field_1A2_rock_or_bone_count; i++)
        {
            BaseThrowable* pThrowable = Make_Throwable_49AF30(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(30),
                FP_FromInteger(0));

            // Random explode time ?
            const FP rand1 = FP_FromRaw((Math_NextRandom() - 127) << 11); // TODO: Wat?
            const FP rand2 = (FP_FromDouble(0.03125) * FP_FromRaw(Math_NextRandom())) - FP_FromInteger(2);
            pThrowable->Vsub_49E460(rand1, rand2);

            pThrowable->field_CC_sprite_scale = field_CC_sprite_scale;
            pThrowable->field_D6_scale = field_D6_scale;
            pThrowable->Vnull_411490(); // Start count down ?
        }
        field_1A2_rock_or_bone_count = 0;
        break;

    case Types::eRockSpawner_48:
        if (field_10C_health > FP_FromInteger(0))
        {
            field_114_flags.Set(Flags_114::e114_Bit2);
            field_10C_health = FP_FromInteger(0);

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }
            ToKnockback_44E700(1, 1);
            SFX_Play_46FA90(0x40u, 127, 0x10000);
        }
        break;

    case Types::eFleech_50:
        if (field_10C_health > FP_FromInteger(0))
        {
            field_10C_health -= FP_FromDouble(0.15075); // Yes it has to be this accurate to match

            if (field_10C_health < FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                Abe_SFX_457EC0(0x10u, 0, 1000, this);
            }
            
            if (field_10C_health > FP_FromInteger(0))
            {
                // The damage sound from a Fleech keeps getting high and higher pitch till death
                const FP hpRandSoundRange = (FP_FromInteger(1) - sActiveHero_5C1B68->field_10C_health) / FP_FromDouble(0.15);
                const short pitchRand = Math_RandomRange_496AB0(
                    200 * (FP_GetExponent(hpRandSoundRange)),
                    40 * (5 * (FP_GetExponent(hpRandSoundRange)) + 5));

                Abe_SFX_457EC0(9u, 0, pitchRand, this);
                return 1;
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            auto pBlood = alive_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_40F0B0(
                    field_B8_xpos,
                    // Put YPos in the middle of who is getting damaged
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    FP_FromInteger((field_B8_xpos - pFrom->field_B8_xpos < FP_FromInteger(0))  ? -24 : 24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            ToKnockback_44E700(1, 1);
            field_114_flags.Set(Flags_114::e114_Bit2);

            if (pFrom->field_B8_xpos < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_106_current_state = eAbeStates::State_101_KnockForward_455420;
            }

            if (pFrom->field_B8_xpos > field_B8_xpos && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
            {
                field_106_current_state = eAbeStates::State_101_KnockForward_455420;
            }

            if (pFrom->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-7.8);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
            }

            SFX_Play_46FA90(64u, 127, 0x10000);
            SFX_Play_46FA90(47u, 90, 0x10000);
        }
        break;

    case Types::eGreeter_64:
        if (field_10C_health <= FP_FromInteger(0))
        {
            break;
        }

        Abe_SFX_457EC0(9u, 0, 0, this);
        Abe_SFX_2_457A40(7, 0, 0x7FFF, this);
        field_114_flags.Set(Flags_114::e114_Bit1);
        field_122 = eAbeStates::State_101_KnockForward_455420;
        field_108_delayed_state = eAbeStates::State_101_KnockForward_455420;
        field_10C_health = FP_FromInteger(0);

        if (!ForceDownIfHoisting_44BA30())
        {
            break;
        }
        return 1;

    case Types::eParamite_96:
    case Types::eScrab_112:
    case Types::eSlog_126:
        if (field_10C_health > FP_FromInteger(0))
        {
            field_10C_health = FP_FromInteger(0);

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            auto pBlood = alive_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_40F0B0(
                    field_B8_xpos,
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    (pFrom->field_C4_velx <= FP_FromInteger(0)) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            ToKnockback_44E700(1, 1);
            field_114_flags.Set(Flags_114::e114_Bit2);

            if (pFrom->field_B8_xpos < field_B8_xpos)
            {
                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                {
                    field_106_current_state = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pFrom->field_B8_xpos > field_B8_xpos)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_state = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pFrom->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-7.8);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
            }

            SFX_Play_46FA90(0x40u, 127, 0x10000);

            if (pFrom->field_4_typeId != Types::eParamite_96)
            {
                SFX_Play_46FA90(0x2Fu, 90, 0x10000);
            }
        }
        break;

    case Types::eType_104:
        return 0;

    case Types::eType_107:
        if (field_10C_health > FP_FromInteger(0))
        {
            field_114_flags.Set(Flags_114::e114_Bit2);
            field_10C_health = FP_FromInteger(0);

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            ToKnockback_44E700(1, 1);

            if (pFrom->field_B8_xpos < field_B8_xpos)
            {
                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                {
                    field_106_current_state = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pFrom->field_B8_xpos > field_B8_xpos)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_state = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pFrom->field_C4_velx >= FP_FromInteger(0))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-7.8);
            }

            SFX_Play_46FA90(0x40u, 127, 0x10000);
        }
        break;

    case Types::eMudokon_110:
        if (field_10C_health > FP_FromInteger(0) && field_106_current_state != eAbeStates::State_71_Knockback_455090)
        {
            field_10C_health -= FP_FromDouble(0.07);
            if (field_106_current_state != eAbeStates::State_123_LiftGrabIdle_45A6A0 &&
                field_106_current_state != eAbeStates::State_124_LiftUseUp_45A780 &&
                field_106_current_state != eAbeStates::State_125_LiftUseDown_45A7B0 || field_10C_health <= FP_FromInteger(0))
            {
                ToKnockback_44E700(1, 1);
                field_114_flags.Set(Flags_114::e114_Bit2);
                if (field_10C_health <= FP_FromInteger(0))
                {
                    Abe_SFX_457EC0(9u, 0, 1000, this);
                    Abe_SFX_2_457A40(7, 0, 32767, this);
                    field_10C_health = FP_FromInteger(0);
                }
            }
        }
        break;

    case Types::eSlamDoor_122:
        if (field_10C_health > FP_FromInteger(0))
        {
            ToKnockback_44E700(1, 1);
            Vsub_408A40(5);
            field_114_flags.Set(Flags_114::e114_Bit2);
        }
        break;

    case Types::eType_150:
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        ToDieFinal_458910();
        break;

    default:
        sub_44C980(pFrom);
        if (!(field_114_flags.Get(Flags_114::e114_Bit1)))
        {
            ret = 0;
            field_128.field_18_say = oldSay;
        }
        break;
    }

    if (sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed))
    {
        if (field_10C_health <= FP_FromInteger(0))
        {
            sControlledCharacter_5C1B8C->VUnPosses_408F90();
            // We are the "active" player again
            GiveControlBackToMe_44BA10();
        }
    }

    return ret;
}

__int16 Abe::vOn_TLV_Collision_44B5D0(Path_TLV* /*a2a*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseAliveGameObject* Abe::FindObjectToPosses_44B7B0()
{
    BaseAliveGameObject* pTargetObj = nullptr;
    BaseAliveGameObject* pInRangeFart = nullptr;
    BaseAliveGameObject* pInRangeGlukkon = nullptr;

    short maxDistance = 32767;
    FP lastScale = {};
    for (int i=0; i<gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed))
        {
            switch (pObj->field_4_typeId)
            {
            // Third priority
            case Types::eCrawlingSlig_26:
            case Types::eFlyingSlig_54:
            case Types::eParamite_96:
            case Types::eScrab_112:
            case Types::eSlig_125:
                if (pObj->Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent && pObj->field_10C_health > FP_FromInteger(0))
                {
                    const short distance = static_cast<short>(Math_Distance_496EB0(
                        FP_GetExponent(field_B8_xpos),
                        FP_GetExponent(field_BC_ypos),
                        FP_GetExponent(pObj->field_B8_xpos),
                        FP_GetExponent(pObj->field_BC_ypos)));

                    if (lastScale == field_CC_sprite_scale)
                    {
                        if (pObj->field_CC_sprite_scale == field_CC_sprite_scale && distance < maxDistance)
                        {
                            pTargetObj = pObj;
                            maxDistance = distance;
                        }
                    }
                    else if (pObj->field_CC_sprite_scale == field_CC_sprite_scale)
                    {
                        pTargetObj = pObj;
                        maxDistance = distance;
                        lastScale = pObj->field_CC_sprite_scale;
                    }
                    else if (distance < maxDistance)
                    {
                        pTargetObj = pObj;
                        maxDistance = distance;
                        lastScale = pObj->field_CC_sprite_scale;
                    }
                }
                break;

            // First priority
            case Types::eType_45_EvilFart:
                pInRangeFart = pObj;
                break;

            // Second priority
            case Types::eGlukkon_67:
                if (pObj->Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent)
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
    NOT_IMPLEMENTED();
}

void Abe::Free_Resources_44D420()
{
    NOT_IMPLEMENTED();
}

EXPORT BOOL Abe::IsStanding_449D30()
{
    return
           field_106_current_state == eAbeStates::State_0_Idle_44EEB0
        || field_106_current_state == eAbeStates::State_2_StandingTurn_451830
        || field_106_current_state == eAbeStates::State_34_DunnoBegin_44ECF0
        || field_106_current_state == eAbeStates::State_35_DunnoEnd_44ED10
        || field_106_current_state == eAbeStates::State_11_Speak_45B0A0
        || field_106_current_state == eAbeStates::State_7_Speak_45B140
        || field_106_current_state == eAbeStates::State_8_Speak_45B160
        || field_106_current_state == eAbeStates::State_9_SpeakMovement_45B180
        || field_106_current_state == eAbeStates::State_10_Fart_45B1A0
        || field_106_current_state == eAbeStates::State_99_LeverUse_455AC0
        || field_106_current_state == eAbeStates::State_105_RockThrowStandingThrow_456460
        || field_106_current_state == eAbeStates::State_104_RockThrowStandingHold_455DF0
        || field_106_current_state == eAbeStates::State_106_RockThrowStandingEnd_455F20
        || field_106_current_state == eAbeStates::State_112_Chant_45B1C0
        || field_106_current_state == eAbeStates::State_113_ChantEnd_45BBE0;

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

BYTE** Abe::StateToAnimResource_44AAB0(short state)
{
    short mapped = ResourceIndices::eBasic_0;
    if (state < eAbeStates::State_12_Null_4569C0)
    {
        mapped = ResourceIndices::eBasic1_1;
    }
    else if (state < eAbeStates::State_65_LedgeAscend_End_4548E0)
    {
        mapped = ResourceIndices::eBasic_0;
    }
    else if (state < eAbeStates::State_71_Knockback_455090)
    {
        mapped = ResourceIndices::eHosit_12;
    }
    else if (state < eAbeStates::State_75_Jump_Into_Well_45C7B0)
    {
        mapped = ResourceIndices::eKnockBack_9;
    }
    else if (state < eAbeStates::State_84_FallLandDie_45A420)
    {
        mapped = ResourceIndices::eWell_15;
    }
    else if (state < eAbeStates::jState_85_Fall_455070)
    {
        mapped = ResourceIndices::eSmash_6;
    }
    else if (state < eAbeStates::State_86_HandstoneBegin_45BD00)
    {
    mapped = ResourceIndices::eFall_7;
    }
    else if (state < eAbeStates::State_91_FallingFromGrab_4557B0)
    {
        mapped = ResourceIndices::eStone_8;
    }
    else if (state < eAbeStates::State_99_LeverUse_455AC0)
    {
        mapped = ResourceIndices::eEdge_13;
    }
    else if (state < eAbeStates::State_100_Slap_Bomb_455B60)
    {
        mapped = ResourceIndices::ePull_2;
    }
    else if (state < eAbeStates::State_101_KnockForward_455420)
    {
        mapped = ResourceIndices::eBomb_4;
    }
    else if (state < eAbeStates::State_104_RockThrowStandingHold_455DF0)
    {
        mapped = ResourceIndices::eKnockFd_10;
    }
    else if (state < eAbeStates::State_109_455550)
    {
        mapped = ResourceIndices::eThrow_5;
    }
    else if (state < eAbeStates::State_111_GrabRock_4564A0)
    {
        mapped = ResourceIndices::eKnockZ_11;
    }
    else if (state < eAbeStates::State_112_Chant_45B1C0)
    {
        mapped = ResourceIndices::ePick_3;
    }
    else if (state < eAbeStates::State_114_DoorEnter_459470)
    {
        mapped = ResourceIndices::eChant_16;
    }
    else if (state < eAbeStates::State_116_MineCarEnter_458780)
    {
        mapped = ResourceIndices::eDoor_14;
    }
    else if (state < eAbeStates::State_119_45A990)
    {
        mapped = ResourceIndices::eMineCar_18;
    }
    else if (state < eAbeStates::State_121_LiftGrabBegin_45A600)
    {
        mapped = ResourceIndices::eMorph_19;
    }
    else if (state < eAbeStates::State_126_TurnWheelBegin_456700)
    {
        mapped = ResourceIndices::eLift_17;
    }
    else if (state < eAbeStates::State_129_PoisonGasDeath_4565C0)
    {
        mapped = ResourceIndices::eWork_20;
    }
    else
    {
        mapped = ResourceIndices::eGas_21;
        if (state >= 130) // max states
        {
            // Impossible case?
            LOG_ERROR("State is out of bounds !!! " << state);
            mapped = static_cast<short>(state);
        }
    }

    if (mapped == field_128.field_10)
    {
        return field_10_resources_array.ItemAt(field_128.field_10);
    }

    // Check to never free basic res
    if (field_128.field_10 != ResourceIndices::eBasic_0 && field_128.field_10 != 1)
    {
        ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(field_128.field_10));
        field_10_resources_array.SetAt(field_128.field_10, nullptr);
    }

    // Check to never load basic res
    if (mapped != ResourceIndices::eBasic_0 && mapped != 1)
    {
        field_10_resources_array.SetAt(mapped, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, sAbeResourceIDTable_554D60[mapped], TRUE, FALSE));
    }

    field_128.field_10 = mapped;
    return field_10_resources_array.ItemAt(mapped);
}

static bool IsSameScaleAsHoist(Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->field_16_scale == Path_Hoist::Scale::eFull && pObj->field_D6_scale == 0)
    {
        return false;
    }
    else if (pHoist->field_16_scale == Path_Hoist::Scale::eHalf && pObj->field_D6_scale == 1)
    {
        return false;
    }
    return true;
}

static bool IsSameScaleAsEdge(Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->field_14_scale == Path_Edge::Scale::eFull && pObj->field_D6_scale == 0)
    {
        return false;
    }
    else if (pEdge->field_14_scale == Path_Edge::Scale::eHalf && pObj->field_D6_scale == 1)
    {
        return false;
    }
    return true;
}

static bool IsFacingSameDirectionAsHoist(Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->field_12_edge_type == Path_Hoist::EdgeType::eLeft && !pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return false;
    }
    else if (pHoist->field_12_edge_type == Path_Hoist::EdgeType::eRight && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return false;
    }

    return true;
}

static bool IsFacingSameDirectionAsEdge(Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->field_10_type == Path_Edge::Type::eLeft && !pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return false;
    }
    else if (pEdge->field_10_type == Path_Edge::Type::eRight && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return false;
    }

    return true;
}

class BoomMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_445E10(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_445F50();
    }

    virtual void VScreenChanged() override
    {
        vScreenChange_446020();
    }

    virtual BOOL Vsub_445DF0()
    {
        return vsub_445DF0();
    }

    virtual BaseGameObject* Vsub_445F00()
    {
        return vsub_445F00();
    }
private:
    EXPORT void vUpdate_445F50()
    {
        NOT_IMPLEMENTED();
    }

    EXPORT void vScreenChange_446020()
    {
        NOT_IMPLEMENTED();
    }

    EXPORT BOOL virtual vsub_445DF0()
    {
        NOT_IMPLEMENTED();
        return 0;
    }

    EXPORT BaseGameObject* vsub_445F00()
    {
        NOT_IMPLEMENTED();
        return nullptr;
    }

    EXPORT void dtor_445E40()
    {
        SetVTable(this, 0x5455C4); // vTbl_GrenadeMachine_5455C4

        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_F8);
        if (pObj)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead);;
        }
        Path::TLV_Reset_4DB8E0(field_F4, -1, 0, 0);

        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT BoomMachine* vdtor_445E10(signed int flags)
    {
        dtor_445E40();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);

void Abe::State_0_Idle_44EEB0()
{
    if (Input_IsChanting_45F260() && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit6)))
    {
        if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
        {
            field_106_current_state = eAbeStates::State_119_45A990;
            field_120_state = 0;
        }
        else
        {
            // Goto chanting
            field_124_gnFrame = sGnFrame_5C1B84 + 90;
            field_106_current_state = eAbeStates::State_112_Chant_45B1C0;
            field_120_state = 0;
            SND_SEQ_PlaySeq_4CA960(0xAu, 0, 1);
        }
        return;
    }

    // Goto game speak state
    const DWORD held = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (held & (eChant | eGameSpeak8 | eGameSpeak7 | eGameSpeak6 | eGameSpeak5 | eGameSpeak4 | eGameSpeak3 | eGameSpeak2 | eGameSpeak1))
    {
        field_118_prev_held = held;
        field_106_current_state = eAbeStates::State_11_Speak_45B0A0;
        return;
    }

    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Hop_5550E0 & pressed)
    {
        // Some strange alternative way of hoisting, hangover from PSX AO Demo?
        if (pressed & sInputKey_Up_5550D8)
        {
            TryHoist_44ED30();
        }
        else
        {
            field_106_current_state = eAbeStates::State_27_HopBegin_4521C0;

            BaseGameObject* pObj = Vsub_408FD0(2);
            if (pObj)
            {
                field_1A4 = 0;
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

    if (sInputKey_Down_5550DC & pressed)
    {
        // Check for a lift rope (going down)
        BaseGameObject* pObj_field_110 = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj_field_110)
        {
            if (pObj_field_110->field_4_typeId == Types::eLiftPoint_78)
            {
                const FP halfGrid = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((field_100_pCollisionLine->field_0_rect.x + field_100_pCollisionLine->field_0_rect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = (field_B8_xpos - liftPlatformXMidPoint) >= FP_FromInteger(0) ? field_B8_xpos - liftPlatformXMidPoint : liftPlatformXMidPoint - field_B8_xpos;
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    field_106_current_state = eAbeStates::State_121_LiftGrabBegin_45A600;
                    return;
                }
            }
        }
        
        // Look below for a down hoist
        Path_Hoist* pHoist =  static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos) + 16,
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos) + 16,
            Path_Hoist::kType));

        if (pHoist)
        {
            if (!IsSameScaleAsHoist(pHoist, this))
            {
                return;
            }

            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                // Yeah go down
                field_106_current_state = eAbeStates::State_66_LedgeDescend_454970;
            }
            else
            {
                if (pHoist->field_12_edge_type == Path_Hoist::EdgeType::eBoth)
                {
                    // We can hoist down from any side
                    field_106_current_state = eAbeStates::State_66_LedgeDescend_454970;
                }
                else
                {
                    // Otherwise gotta turn around
                    field_108_delayed_state = eAbeStates::State_66_LedgeDescend_454970;
                    field_106_current_state = eAbeStates::State_2_StandingTurn_451830;
                }
            }
        }
        else
        {
            // Isn't a hoist so just crouch
            field_106_current_state = eAbeStates::State_19_StandToCrouch_453DC0;
        }
        return;
    }

    if (sInputKey_FartRoll_5550F0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held)
    {
        // Do the fart sound
        Abe_SFX_457EC0(7u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(3);

        // Let others hear the fart
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
       
        if (field_198_has_evil_fart)
        {
            // An evil fart
            field_198_has_evil_fart = FALSE;
            Create_Fart_421D20();

            if (field_10_resources_array.ItemAt(22))
            {
                ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(22));
                field_10_resources_array.SetAt(22, nullptr);
                field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
                return;
            }
        }
        else
        {
            const FP fartScale = FP_FromDouble(0.5) * field_CC_sprite_scale;
            const FP fartYPos = field_BC_ypos - (FP_FromInteger(24) * field_CC_sprite_scale);
            FP fartXPos = {};

            // A normal fart, figure out the direction of Abe's Arse
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                fartXPos = field_B8_xpos + (FP_FromInteger(12) * field_CC_sprite_scale);
            }
            else
            {
                fartXPos = field_B8_xpos - (FP_FromInteger(12) * field_CC_sprite_scale);
            }

            New_Particles_426C70(fartXPos, fartYPos, fartScale, 3, 32u, 128u, 32u);
        }

        field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
        return;
    }

    bool handleDoActionOrThrow = false;
    if (pressed & sInputKey_Up_5550D8)
    {
        // Check for lift rope.. (going up)
        BaseGameObject* pObj_field_110_2 = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj_field_110_2)
        {
            if (pObj_field_110_2->field_4_typeId == Types::eLiftPoint_78)
            {
                const FP halfGrid = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((field_100_pCollisionLine->field_0_rect.x + field_100_pCollisionLine->field_0_rect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = (field_B8_xpos - liftPlatformXMidPoint >= FP_FromInteger(0)) ? field_B8_xpos - liftPlatformXMidPoint : liftPlatformXMidPoint - field_B8_xpos;
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    field_106_current_state = eAbeStates::State_121_LiftGrabBegin_45A600;
                    return;
                }
            }
        }

        // Get the first TLV
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);

        // Handle objects that accept "up"
        while (pTlv)
        {
            switch (pTlv->field_4_type)
            {
            case Path_Door::kType:
                if (!sub_44EE10() || field_114_flags.Get(Flags_114::e114_Bit7))
                {
                    if (sInputKey_Up_5550D8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held) // OG bug, already checked ??
                    {
                        field_106_current_state = eAbeStates::State_34_DunnoBegin_44ECF0;
                    }
                }
                else
                {
                    field_FC_pPathTLV = pTlv;
                    field_120_state = 0;
                    field_106_current_state = eAbeStates::State_114_DoorEnter_459470;
                }
                return;
                
            case Path_Well_Local::kType:
            {
                if (field_114_flags.Get(Flags_114::e114_Bit10))
                {
                    break;
                }

                // Bail if scale doesn't match
                Path_Well_Local* pWell = static_cast<Path_Well_Local*>(pTlv);
                if ((pWell->field_0_scale != 0 || field_CC_sprite_scale != FP_FromDouble(1.0)) && (pWell->field_0_scale != 1 || field_CC_sprite_scale != FP_FromDouble(0.5)))
                {
                    break;
                }

                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_Fall_To_Well);
                field_FC_pPathTLV = pTlv;
                field_106_current_state = eAbeStates::State_78_WellBegin_45C810;
            }
            return;
                
            case Path_Well_Express::kType:
            {
                if (field_114_flags.Get(Flags_114::e114_Bit10))
                {
                    break;
                }

                // Bail if scale doesn't match
                Path_Well_Express* pWell = static_cast<Path_Well_Express*>(pTlv);
                if ((pWell->field_0_scale != 0 || field_CC_sprite_scale != FP_FromDouble(1.0)) && (pWell->field_0_scale != 1 || field_CC_sprite_scale != FP_FromDouble(0.5)))
                {
                    break;
                }

                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_Fall_To_Well);
                field_FC_pPathTLV = pTlv;
                field_106_current_state = eAbeStates::jState_81_WellBegin_45C7F0;
            }
            return;
                
            case 27: // 027_Movie_stone
            case 61: // 061_Hand_stone
                field_FC_pPathTLV = pTlv;
                field_106_current_state = eAbeStates::State_86_HandstoneBegin_45BD00;
                field_120_state = 0;
                return;

            case 59:  // 059_Grenade_machine
            {
                BoomMachine* pMachineButton = static_cast<BoomMachine*>(FindObjectOfType_425180(Types::eGrenadeMachine_66, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(25))));
                if (pMachineButton)
                {
                    pMachineButton->Vsub_445F00();
                    field_106_current_state = eAbeStates::State_88_GrenadeMachineUse_45C510;
                }
                else
                {
                    field_106_current_state = eAbeStates::State_34_DunnoBegin_44ECF0;
                }
            }
            break;
                
            case 79:  // 079_Wheel
            {
                bool bCanUseWheel = true;
                for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
                {
                    BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }
                    
                    if (pObj->field_4_typeId == Types::eMudokon_110 && pObj->field_D6_scale == field_D6_scale)
                    {
                        FP xDiff = pObj->field_B8_xpos - field_B8_xpos;
                        if (xDiff < FP_FromInteger(0))
                        {
                            xDiff = field_B8_xpos - pObj->field_B8_xpos;
                        }

                        FP gridWidth = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                        if (xDiff < gridWidth)
                        {
                            FP yDiff = pObj->field_BC_ypos - field_BC_ypos;
                            if (yDiff < FP_FromInteger(0))
                            {
                                yDiff = field_BC_ypos - pObj->field_BC_ypos;
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
                    field_106_current_state = eAbeStates::State_126_TurnWheelBegin_456700;
                    BaseGameObject* pObj_148 = FindObjectOfType_425180(Types::eWheel_148, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)));
                    if (pObj_148)
                    {
                        field_164_wheel_id = pObj_148->field_8_object_id;
                    }
                }
            }
            break;
            
            case 101: // 101_Fart_machine
                field_106_current_state = eAbeStates::State_89_BrewMachineBegin_4584C0;
                field_120_state = 0;
                break;

            default:
                break;
            }

            // To next TLV (if any)
            pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                pTlv,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
        }

        if (!TryEnterMineCar_4569E0())
        {
            if (field_106_current_state == eAbeStates::State_0_Idle_44EEB0)
            {
                TryHoist_44ED30();
            }
            handleDoActionOrThrow = true;
        }
    }

    if (!(pressed & sInputKey_Up_5550D8) || handleDoActionOrThrow)
    {
        // TODO: Clean up the logic of these 2 statements
        if (!(sInputKey_ThrowItem_5550F4 & held) || field_106_current_state != eAbeStates::State_0_Idle_44EEB0)
        {
            if (held & sInputKey_DoAction_5550E4) // not throwing, maybe pressing up and pressing action, so do action
            {
                field_106_current_state = HandleDoAction_455BD0();
            }
        }
        else if (field_1A2_rock_or_bone_count > 0 || gInfiniteGrenades_5C1BDE)
        {
            field_158_throwable_id = Make_Throwable_49AF30(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(40),
                FP_FromInteger(0))->field_8_object_id;

            if (!bThrowableIndicatorExists_5C112C)
            {
                ThrowableTotalIndicator* pThrowable = alive_new<ThrowableTotalIndicator>();
                if (pThrowable)
                {
                    const FP xOffSet = (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ? FP_FromInteger(-15) : FP_FromInteger(15) * field_CC_sprite_scale;

                    pThrowable->ctor_431CB0(
                        field_B8_xpos + xOffSet,
                        field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-50)),
                        field_20_animation.field_C_render_layer,
                        field_20_animation.field_14_scale,
                        field_1A2_rock_or_bone_count,
                        TRUE);
                }
            }

            field_106_current_state = eAbeStates::State_104_RockThrowStandingHold_455DF0;
            
            if (!gInfiniteGrenades_5C1BDE)
            {
                field_1A2_rock_or_bone_count--;
            }
        }
        else
        {
            field_106_current_state = eAbeStates::State_34_DunnoBegin_44ECF0;
        }
    }
}

void Abe::State_1_WalkLoop_44FBA0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    sub_44E9A0();

    if (field_106_current_state != eAbeStates::State_1_WalkLoop_44FBA0)
    {
        return;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
    }

    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (field_20_animation.field_92_current_frame != 2 && field_20_animation.field_92_current_frame != 11)
    {
        if (field_20_animation.field_92_current_frame != 5 && field_20_animation.field_92_current_frame != 14)
        {
            return;
        }

        Abe_SFX_2_457A40(1, 0, 32767, this);

        MapFollowMe_408D10(TRUE);

        if (sInputKey_Run_5550E8 & pressed)
        {
            // Walk to run
            if (field_20_animation.field_92_current_frame == 5)
            {
                field_106_current_state = eAbeStates::State_49_450200;
            }
            else
            {
                field_106_current_state = eAbeStates::State_48_4500A0;
            }
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            // Walk to sneak
            if (field_20_animation.field_92_current_frame != 5)
            {
                field_106_current_state = eAbeStates::State_43_450380;
            }
            else
            {
                field_106_current_state = eAbeStates::State_41_450250;
            }
        }
        return;
    }

    // Direction changed
    bool bEndWalking = false;
    if ((field_C4_velx > FP_FromInteger(0) && (sInputKey_Left_5550D4 & pressed)) ||
        (field_C4_velx < FP_FromInteger(0) && (sInputKey_Right_5550D0 & pressed)))
    {
        bEndWalking = true;
    }

    // Hopping
    if (sInputKey_Hop_5550E0 & field_118_prev_held)
    {
        field_108_delayed_state = eAbeStates::State_28_HopMid_451C50;
        bEndWalking = true;
    }

    if (!bEndWalking)
    {
        if ((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed)
        {
            // Check if walking into something
            FP gridSize = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            const FP offX = gridSize * FP_FromDouble(1.5);
            if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), offX) ||
                Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), offX))
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
        if (field_20_animation.field_92_current_frame == 2)
        {
            field_106_current_state = eAbeStates::State_5_WalkEndRightFoot_00450080;
        }
        else
        {
            field_106_current_state = eAbeStates::State_4_WalkEndLeftFoot_44FFC0;
        }
        field_118_prev_held = 0;
    }
}

void Abe::State_2_StandingTurn_451830()
{
    const __int16 currentFrame = field_20_animation.field_92_current_frame;
    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (currentFrame == 4 && (sInputKey_Run_5550E8 & pressed) && (sInputKey_Right_5550D0 | sInputKey_Left_5550D4) & pressed)
    {
        field_106_current_state = eAbeStates::State_61_TurnToRun_456530;
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
        }
    }
    else
    {
        // Play "turning" sound effect
        if (currentFrame == 0)
        {
            Abe_SFX_2_457A40(9, 0, 32767, this);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_108_delayed_state)
            {
                field_106_current_state = field_108_delayed_state;
                field_108_delayed_state = 0;
            }
            else
            {
                ToIdle_44E6B0();
            }
        }
    }
}

void Abe::State_3_Fall_459B60()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx -= (field_CC_sprite_scale * field_128.field_8);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx += (field_CC_sprite_scale * field_128.field_8);
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    const int bCollision = InAirCollision_408810(&pPathLine, &hitX, &hitY, FP_FromDouble(1.80)); // 0x1CCCC
    sub_408C40();

    // Are we falling into a local well?
    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Well_Local::kType);

    if (!field_FC_pPathTLV)
    {
        // No, are we falling into an express well?
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Well_Express::kType);
    }

    // Handle falling into a well
    if (field_FC_pPathTLV)
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            if (field_FC_pPathTLV->field_4_type == Path_Well_Local::kType || field_FC_pPathTLV->field_4_type == Path_Well_Express::kType)
            {
                // The well must be on the same scale/layer
                Path_Well_Base* pWellBase = static_cast<Path_Well_Base*>(field_FC_pPathTLV);
                if ((pWellBase->field_0_scale == 0 && field_CC_sprite_scale == FP_FromDouble(1.0))
                 || (pWellBase->field_0_scale == 1 && field_CC_sprite_scale == FP_FromDouble(0.5)))
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_Fall_To_Well);
                    field_106_current_state = eAbeStates::State_75_Jump_Into_Well_45C7B0;
                    return;
                }
            }
        }
    }

    Path_SoftLanding* pSoftLanding = nullptr;
    if (bCollision)
    {
        switch (pPathLine->field_8_type)
        {
        case eFloor_0:
        case eBackGroundFloor_4:
        case 32u: // TODO: These type are never seen, internal only ??
        case 36u:
        {
            field_B8_xpos = hitX;
            field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
            field_100_pCollisionLine = pPathLine;
            MapFollowMe_408D10(TRUE);
            field_124_gnFrame = sGnFrame_5C1B84 + 30;

            // See if there is a soft landing at our feet (given we known we just hit the floor)
            pSoftLanding =
                static_cast<Path_SoftLanding*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    Path_SoftLanding::kType));

            if (pSoftLanding)
            {
                if (!SwitchStates_Get_466020(static_cast<short>(pSoftLanding->field_10_id)))
                {
                    pSoftLanding = nullptr;
                }
            }

            if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit7)
                || (pSoftLanding && field_10C_health > FP_FromInteger(0)) // If we are dead soft landing won't save us
                || (field_BC_ypos - field_F8) < (field_CC_sprite_scale * FP_FromInteger(180)) // Check we didn't fall far enough to be killed
                && (field_10C_health > FP_FromInteger(0) || word_5C1BDA))
            {
                field_106_current_state = eAbeStates::State_16_LandSoft_45A360;
            }
            else
            {
                // Slam into the floor and break all your bones
                field_10C_health = FP_FromInteger(0);
                field_106_current_state = eAbeStates::State_84_FallLandDie_45A420;
                field_128.field_4_regen_health_timer = sGnFrame_5C1B84 + 900;
            }

            field_F4 = eAbeStates::State_3_Fall_459B60;

            PSX_Point xy{ FP_GetExponent(field_B8_xpos - FP_FromInteger(10)), FP_GetExponent(field_BC_ypos - FP_FromInteger(10)) };
            PSX_Point wh{ FP_GetExponent(field_B8_xpos + FP_FromInteger(10)), FP_GetExponent(field_BC_ypos + FP_FromInteger(10)) };
            vOnCollisionWith_424EE0(
                xy,
                wh,
                ObjList_5C1B78,
                1,
               reinterpret_cast<TCollisionCallBack>(&BaseAliveGameObject::OnTrapDoorIntersection_408BA0)); // Danger danger.. but will probably work.. can't see how else they would have got this to work
        }
            break;

        case eWallLeft_1:
        case eWallRight_2: 
        case eBackGroundWallLeft_5:
        case eBackGroundWallRight_6: 
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            ToKnockback_44E700(1, 1);
            break;
        }
        return;
    }

    if (field_10C_health <= FP_FromInteger(0))
    {
        return;
    }

    // Look down 75 for an edge
    Path_Edge* pEdge = static_cast<Path_Edge*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(75))),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Edge::kType));

    field_FC_pPathTLV = pEdge;
    bool tryToHang = false;
    if (pEdge)
    {
        if (pEdge->field_12_can_grab && IsSameScaleAsEdge(pEdge, this) && 
           (IsFacingSameDirectionAsEdge(pEdge, this) || pEdge->field_10_type == Path_Edge::Type::eBoth))
        {
            tryToHang = true;
        }
    }

    // Didn't find and edge to grab so check if falling onto a hoist
    if (!tryToHang)
    {
        // Look down 20 for a hoist
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - field_CC_sprite_scale * FP_FromInteger(20)),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - field_CC_sprite_scale * FP_FromInteger(20)),
            Path_Hoist::kType));

        if (pHoist)
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this) && IsSameScaleAsHoist(pHoist, this))
            {
                tryToHang = true;
            }
        }

        field_FC_pPathTLV = pHoist;
    }

    if (tryToHang)
    {
        if (field_C4_velx == FP_FromInteger(0))
        {
            return;
        }

        field_B8_xpos = FP_FromInteger((field_FC_pPathTLV->field_8_top_left.field_0_x + field_FC_pPathTLV->field_C_bottom_right.field_0_x) / 2);

        MapFollowMe_408D10(TRUE);

        if (!sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y + 65526), // TODO: Negative ??
            field_B8_xpos,
            FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y + 10),
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
        {
            return;
        }

        field_BC_ypos = hitY;
        field_100_pCollisionLine = pPathLine;
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        if (field_FC_pPathTLV->field_4_type != Path_Hoist::kType ||
            (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y)) >= (field_CC_sprite_scale * FP_FromInteger(70)))
        {
            field_106_current_state = eAbeStates::State_69_LedgeHangWobble_454EF0;
            field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
        }
        else
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
            field_F4 = eAbeStates::State_65_LedgeAscend_End_4548E0;
            field_F6_anim_frame = 12;
            field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
        }
    }
}

void Abe::State_4_WalkEndLeftFoot_44FFC0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    sub_44E9A0();

    if (field_20_animation.field_92_current_frame)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_408D10(TRUE);

            if (field_108_delayed_state == eAbeStates::State_28_HopMid_451C50)
            {
                field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
                field_106_current_state = eAbeStates::State_27_HopBegin_4521C0;
                BaseGameObject* pObj = Vsub_408FD0(2);
                if (pObj)
                {
                    field_1A4 = 0;
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
        Abe_SFX_2_457A40(1, 0, 32767, this);
    }
}

void Abe::State_5_WalkEndRightFoot_00450080()
{
    // TODO Jump func omitted jState_WalkEndLeftFoot_40459
    State_4_WalkEndLeftFoot_44FFC0();
}

void Abe::State_6_WalkBegin_44FEE0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    // They hear me walking, they hating
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_1_WalkLoop_44FBA0;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        sub_44E9A0();
    }
}

void Abe::State_7_Speak_45B140()
{
    // TODO: Note jState_11_Speak_40388C omitted
    State_11_Speak_45B0A0();
}

void Abe::State_8_Speak_45B160()
{
    // TODO: Note jState_11_Speak_40388C omitted
    State_11_Speak_45B0A0();
}

void Abe::State_9_SpeakMovement_45B180()
{
    // TODO: Note jState_11_Speak_40388C omitted
    State_11_Speak_45B0A0();
}

void Abe::State_10_Fart_45B1A0()
{
    // TODO: Note jState_11_Speak_40388C omitted
    State_11_Speak_45B0A0();
}

void Abe::State_11_Speak_45B0A0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = DoGameSpeak_45AB70(field_118_prev_held);
        if (field_106_current_state == -1)
        {
            ToIdle_44E6B0();
            field_118_prev_held = 0;
        }
        else
        {
            if (field_106_current_state != eAbeStates::State_62_Punch_454750)
            {
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }

            field_114_flags.Set(Flags_114::e114_Bit2);
            field_118_prev_held = 0;
        }
    }
}

void Abe::State_12_Null_4569C0()
{

}

void Abe::State_13_HoistBegin_452B20()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F8 = field_BC_ypos;
        const FP velY = field_CC_sprite_scale * FP_FromInteger(-8);
        field_C8_vely = velY;
        field_BC_ypos += velY;
        Vnull_4081F0();
        field_106_current_state = eAbeStates::State_14_HoistIdle_452440;
        field_100_pCollisionLine = nullptr;
    }
}

void Abe::State_14_HoistIdle_452440()
{
    //sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id); // NOTE: Return never used
    BaseGameObject* pfield_110_id = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    if (Is_Celling_Above_44E8D0())
    {
        ToKnockback_44E700(1, 1);
        return;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const __int16 bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.80));
    sub_408C40();

    if (bCollision)
    {
        switch (pLine->field_8_type)
        {
        case eFloor_0:
        case eBackGroundFloor_4:
        case 32u: // trap doors ??
        case 36u:
        {
            field_B8_xpos = hitX;
            field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));

            MapFollowMe_408D10(1);

            field_100_pCollisionLine = pLine;

            field_106_current_state = eAbeStates::State_15_HoistLand_452BA0;
            field_F4 = eAbeStates::State_14_HoistIdle_452440;

            vOnCollisionWith_424EE0(
            { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_B8_xpos) },
            { FP_GetExponent(field_BC_ypos), FP_GetExponent((field_BC_ypos + FP_FromInteger(5))) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
        break;
        }
        return;
    }

    PullRingRope* pPullRope = GetPullRope_44D120();
    if (pPullRope)
    {
        if (pPullRope->VPull_49BBD0(this))
        {
            field_106_current_state = eAbeStates::State_70_RingRopePull_455AF0;
            field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
            field_15C_pull_rope_id = pPullRope->field_8_object_id;
            return;
        }
        field_15C_pull_rope_id = -1;
    }

    Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Hoist::kType));
    field_FC_pPathTLV = pHoist;

    if (field_C8_vely < FP_FromInteger(0))
    {
        // Going down and no hoist, can't do anything
        if (!pHoist)
        {
            return;
        }

        // Hoist is too far away
        if (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y -1 * field_FC_pPathTLV->field_8_top_left.field_2_y) >
            (field_CC_sprite_scale * FP_FromInteger(90)) || field_20_animation.field_92_current_frame )
        {
            return;
        }
    }

    if (pHoist)
    {
        if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this) || pHoist->field_12_edge_type == Path_Hoist::EdgeType::eBoth))
        {
            if (pHoist->field_10_type == Path_Hoist::Type::eOffScreen)
            {
                if (gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapTop, this, -1))
                {
                    sub_4945B0();
                    field_106_current_state = eAbeStates::State_68_ToOffScreenHoist_454B80;
                    return;
                }

                Abe_SFX_2_457A40(1, 0, 127, this);

                if (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y)
                    >= field_CC_sprite_scale * FP_FromInteger(70))
                {
                    field_106_current_state = eAbeStates::State_67_LedgeHang_454E20;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
                    field_F4 = eAbeStates::State_65_LedgeAscend_End_4548E0;
                    field_F6_anim_frame = 12;
                }
                field_BC_ypos -= field_CC_sprite_scale * FP_FromInteger(75);
                field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
            }
            else
            {
                Abe_SFX_2_457A40(1, 0, 127, this);
                if (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y) >=
                    field_CC_sprite_scale * FP_FromInteger(70))
                {
                    field_106_current_state = eAbeStates::State_67_LedgeHang_454E20;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
                    field_F4 = eAbeStates::State_65_LedgeAscend_End_4548E0;
                    field_F6_anim_frame = 12;
                }
            }

            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y - 10),
                field_B8_xpos,
                FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y + 10),
                &pLine,
                &hitX,
                &hitY,
                field_D6_scale != 0 ? 1 : 16))
            {
                field_100_pCollisionLine = pLine;
                field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                field_C8_vely = FP_FromInteger(0);
                if (pfield_110_id)
                {
                    if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
                    {
                        vOnCollisionWith_424EE0(
                        { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos) },
                        { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
                            ObjList_5C1B78,
                            1,
                            (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
                    }
                }
                field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
            }
            else
            {
                field_106_current_state = eAbeStates::State_14_HoistIdle_452440;
            }
        }
    }
    else
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }
}

void Abe::State_15_HoistLand_452BA0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_F4 == eAbeStates::State_3_Fall_459B60)
        {
            Abe_SFX_2_457A40(5, 0, 32767, this);
        }
        else
        {
            Abe_SFX_2_457A40(6, 0, 32767, this);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Landing and standing
        ToIdle_44E6B0();
    }
}

void Abe::State_16_LandSoft_45A360()
{
    if (field_20_animation.field_92_current_frame == 2)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit7)))
        {
            // Hitting the floor events
            Event_Broadcast_422BC0(kEventNoise, this);
            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
        }
        
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit7);

        // Hitting the floor sounds
        if (field_F4 == eAbeStates::State_3_Fall_459B60)
        {
            Abe_SFX_2_457A40(5, 0, 0x7FFF, this);
        }
        else
        {
            Abe_SFX_2_457A40(6, 0, 0x7FFF, this);
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            // If Abe is controlling something else then must be standing and chanting
            field_106_current_state = eAbeStates::State_112_Chant_45B1C0;
        }

        if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (eRight | eLeft))
        {
            ToLeftRightMovement_44E340();
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_17_CrouchIdle_456BC0()
{
    if (!field_100_pCollisionLine)
    {
        field_106_current_state = eAbeStates::State_98_RollOffEdge_455AA0;
        return;
    }

    field_118_prev_held = 0;
    field_11C = 0;

    // Crouching game speak
    if (CrouchingGameSpeak_453E10())
    {
        field_118_prev_held = 0;
        Event_Broadcast_422BC0(kEventSpeaking, this);
        return;
    }

    // Hit bombs/pick up items ?
    if (sInputKey_DoAction_5550E4 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held)
    {
        if (!((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed))
        {
            FP gridSize = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            PickUpThrowabe_Or_PressBomb_454090(gridSize + field_B8_xpos, FP_GetExponent(field_BC_ypos - FP_FromInteger(5)), 0);
        }
    }

    const DWORD held = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    
    // Crouching throw stuff
    if (sInputKey_ThrowItem_5550F4 & held
        && field_106_current_state == eAbeStates::State_17_CrouchIdle_456BC0
        && (field_1A2_rock_or_bone_count > 0 || gInfiniteGrenades_5C1BDE))
    {
        field_158_throwable_id = Make_Throwable_49AF30(field_B8_xpos, field_BC_ypos - FP_FromInteger(40), FP_FromInteger(0))->field_8_object_id;
        if (!bThrowableIndicatorExists_5C112C)
        {
            auto pRockCountGraphic = alive_new<ThrowableTotalIndicator>();
            if (pRockCountGraphic)
            {
                const FP yOff = field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-30));
                const FP xOff = field_CC_sprite_scale * (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-10) : FP_FromInteger(10));
                pRockCountGraphic->ctor_431CB0(
                    field_B8_xpos + xOff,
                    yOff,
                    field_20_animation.field_C_render_layer,
                    field_20_animation.field_14_scale,
                    field_1A2_rock_or_bone_count,
                    1);
            }
        }
        
        field_106_current_state = eAbeStates::State_107_RockThrowCrouchingHold_454410;
        
        if (!gInfiniteGrenades_5C1BDE)
        {
            field_1A2_rock_or_bone_count--;
        }
    }
    else
    {
        // Try to stand up
        if (held & sInputKey_Up_5550D8 && !Is_Celling_Above_44E8D0())
        {
            field_106_current_state = eAbeStates::State_18_CrouchToStand_454600;
            return;
        }

        // Crouching farts
        if (sInputKey_FartRoll_5550F0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(3);

            Abe_SFX_457EC0(7u, 0, 0, this);

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
                const FP scale = field_CC_sprite_scale * FP_FromDouble(0.5);
                const FP ypos = field_BC_ypos - (FP_FromInteger(6) * field_CC_sprite_scale);
                FP xpos = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    xpos = field_B8_xpos + (FP_FromInteger(10) * field_CC_sprite_scale);
                }
                else
                {
                    xpos = field_B8_xpos - (FP_FromInteger(10) * field_CC_sprite_scale);
                }
                New_Particles_426C70(xpos, ypos, scale, 3, 32u, 128u, 32u);
            }

            field_106_current_state = eAbeStates::State_20_454550;
        }
        else
        {
            // Crouching turns
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Right_5550D0)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_state = eAbeStates::State_37_CrouchTurn_454390;
                }
                else
                {
                    field_106_current_state = eAbeStates::State_22_RollBegin_4539A0;
                    field_11C = 0;
                }
            }

            if (sInputKey_Left_5550D4 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_state = eAbeStates::State_22_RollBegin_4539A0;
                    field_11C = 0;
                }
                else
                {
                    field_106_current_state = eAbeStates::State_37_CrouchTurn_454390;
                }
            }
        }
    }
}

void Abe::State_18_CrouchToStand_454600()
{
    if (field_20_animation.field_92_current_frame == 3)
    {
        if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (eRight | eLeft))
        {
            ToLeftRightMovement_44E340();
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
        field_118_prev_held = 0;
    }
}

void Abe::State_19_StandToCrouch_453DC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PickUpThrowabe_Or_PressBomb_454090(field_B8_xpos, FP_GetExponent(field_BC_ypos - FP_FromInteger(5)), 1);
        field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
    }
}

// TODO: End crouching game speak/fart?
void Abe::State_20_454550()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
        
        CrouchingGameSpeak_453E10();

        if (field_106_current_state == eAbeStates::State_20_454550 || field_106_current_state == eAbeStates::jState_21_4545E0)
        {
            Event_Broadcast_422BC0(kEventSpeaking, this);
        }
        field_118_prev_held = 0;
    }
}

void Abe::jState_21_4545E0()
{
    // Note j_401389 omitted
    State_20_454550();
}

void Abe::State_22_RollBegin_4539A0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
    }

    const FP xpos = field_CC_sprite_scale * FP_FromInteger(20);
    if (Raycast_408750(xpos, field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
        field_106_current_state = eAbeStates::State_74_Rolling_KnockedBack_455290;
    }
    else
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_state = eAbeStates::State_23_RollLoop_453A90;
        }
        sub_44E9A0();
    }
}

void Abe::State_23_RollLoop_453A90()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    
    field_11C |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_10_released;

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
        field_106_current_state = eAbeStates::State_74_Rolling_KnockedBack_455290;
    }
    else
    {
        sub_44E9A0();

        if (field_106_current_state == eAbeStates::State_23_RollLoop_453A90)
        {
            const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (field_20_animation.field_92_current_frame == 1 || field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 9)
            {
                if (!(sInputKey_Run_5550E8 & pressed)
                    || (pressed & sInputKey_FartRoll_5550F0)
                    || Is_Celling_Above_44E8D0()
                    || field_128.field_14 + 9 >= static_cast<int>(sGnFrame_5C1B84))
                {
                    if (field_11C)
                    {
                        if (!Is_Celling_Above_44E8D0() && field_128.field_14 + 9 < static_cast<int>(sGnFrame_5C1B84))
                        {
                            ToLeftRightMovement_44E340();
                            field_11C = 0;
                        }
                    }
                }
                else
                {
                    ToLeftRightMovement_44E340();
                    field_118_prev_held = 0;
                }
            }
            else if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 8)
            {
                MapFollowMe_408D10(TRUE);

                if (field_C4_velx > FP_FromInteger(0) && !(sInputKey_Right_5550D0 & pressed) ||
                    field_C4_velx < FP_FromInteger(0) && !(sInputKey_Left_5550D4 & pressed))
                {
                    field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
                    field_C4_velx = FP_FromInteger(0);
                }
            }

            if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 6)
            {
                Abe_SFX_2_457A40(8, 0, 32767, this);
            }
        }
    }
}

void Abe::State_24_453D00()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
        field_106_current_state = eAbeStates::State_74_Rolling_KnockedBack_455290;
    }
    else
    {
        sub_44E9A0();
        if (field_106_current_state == eAbeStates::State_24_453D00)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(TRUE);
                field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Abe::State_25_RunSlideStop_451330()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));

        if (field_106_current_state == eAbeStates::State_25_RunSlideStop_451330 && !RunTryEnterWell_451060() && !RunTryEnterDoor_451220())
        {
            if (field_20_animation.field_92_current_frame != 15)
            {
                MapFollowMe_408D10(FALSE);
            }

            const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (field_20_animation.field_92_current_frame >= 9)
            {
                if (field_20_animation.field_92_current_frame == 15)
                {
                    Abe_SFX_2_457A40(0, 0, 32767, this);
                    MapFollowMe_408D10(TRUE);

                    if (!ToLeftRightMovement_44E340())
                    {
                        ToIdle_44E6B0();
                    }
                }
            }
            else if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && (sInputKey_Right_5550D0 & pressed)) ||
                    (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && (sInputKey_Left_5550D4 & pressed)))
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
                field_F4 = eAbeStates::State_26_RunTurn_451500;
                field_F6_anim_frame = field_20_animation.field_92_current_frame;
            }
        }
    }
}

void Abe::State_26_RunTurn_451500()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));
        if (field_106_current_state == eAbeStates::State_26_RunTurn_451500
            && !RunTryEnterWell_451060()
            && !RunTryEnterDoor_451220())
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) || 
                (MapFollowMe_408D10(FALSE) && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                MapFollowMe_408D10(TRUE);

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                    {
                        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                        field_106_current_state = eAbeStates::State_52_RunTurn_ToRun_451710;
                    }
                    else
                    {
                        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
                        field_106_current_state = eAbeStates::State_53_RunTurn_ToWalk_451800;
                    }
                }
                else
                {
                    if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                    {
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                        field_106_current_state = eAbeStates::State_52_RunTurn_ToRun_451710;
                    }
                    else
                    {
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                        field_106_current_state = eAbeStates::State_53_RunTurn_ToWalk_451800;
                    }
                }
            }
        }
    }
}

void Abe::State_27_HopBegin_4521C0()
{
    if (field_20_animation.field_92_current_frame == 9)
    {
        // Change velocity at this frame
        const FP velX = field_CC_sprite_scale * (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-17) : FP_FromInteger(17));

        field_C4_velx = velX;

        if (field_1A8_portal_id == -1)
        {
            // Check for hopping into a wall
            if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
                Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
            {
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                field_C4_velx = FP_FromInteger(0);
                ToKnockback_44E700(1, 1);
                return;
            }
        }
        field_B8_xpos += field_C4_velx;
        sub_408C40();
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F8 = field_BC_ypos;
        const FP velX = field_CC_sprite_scale * (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        field_C4_velx = velX;
        field_B8_xpos += velX;

        const FP velY = field_CC_sprite_scale * FP_FromDouble(-2.7);
        field_C8_vely = velY;
        field_BC_ypos += velY;
        
        Vnull_4081F0();
        
        field_106_current_state = eAbeStates::State_28_HopMid_451C50;
        field_100_pCollisionLine = nullptr;

        if (field_1A8_portal_id == -1)
        {
            BaseGameObject* pObj = Vsub_408FD0(2);
            if (pObj)
            {
                field_1A4 = 0;
                field_1A8_portal_id = pObj->field_8_object_id;
            }
        }
    }
}

void Abe::State_28_HopMid_451C50()
{
    if (field_1A8_portal_id != -1)
    {
        IntoPortalStates_451990();
        return;
    }

    // Hopped into a wall?
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
        field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    // See if we've hit a path line floor type
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const __int16 bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.80));
    sub_408C40();

    if (bCollision)
    {
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

        switch (pLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            Abe_SFX_2_457A40(6, 0, 32767, this);
            field_100_pCollisionLine = pLine;
            field_B8_xpos = hitX;
            field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);

            vOnCollisionWith_424EE0(
            { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos) },
            { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);

            MapFollowMe_408D10(TRUE);
            field_106_current_state = eAbeStates::State_29_HopLand_4523D0;
            field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
            return;
        }
    }
    else
    {
        // Dead so can't continue
        if (field_10C_health <= FP_FromInteger(0))
        {
            return;
        }

        // Check for an edge
        Path_Edge* pEdgeTlv = static_cast<Path_Edge*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(75))),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Edge::kType));

        field_FC_pPathTLV = pEdgeTlv;

        if (pEdgeTlv && pEdgeTlv->field_12_can_grab && IsSameScaleAsEdge(pEdgeTlv, this) &&
            ((IsFacingSameDirectionAsEdge(pEdgeTlv, this) || pEdgeTlv->field_10_type == Path_Edge::Type::eBoth) && field_C4_velx != FP_FromInteger(0)))
        {
            field_B8_xpos = FP_FromInteger((pEdgeTlv->field_8_top_left.field_0_x + pEdgeTlv->field_C_bottom_right.field_0_x) / 2);

            MapFollowMe_408D10(TRUE);

            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y - 10),
                field_B8_xpos,
                FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y + 10),
                &pLine,
                &hitX,
                &hitY,
                field_D6_scale != 0 ? 1 : 16))
            {
                field_BC_ypos = hitY;
                field_100_pCollisionLine = pLine;
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                field_106_current_state = eAbeStates::State_69_LedgeHangWobble_454EF0;
                field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
            }
        }
    }

    if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return;
    }

    if (field_106_current_state != eAbeStates::State_28_HopMid_451C50)
    {
        return;
    }

    field_C4_velx = FP_FromRaw(field_C4_velx.fpValue / 2);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B8_xpos = (field_CC_sprite_scale * FP_FromInteger(5)) + field_B8_xpos;
    }
    else
    {
        field_B8_xpos = field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(5));
    }

    field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(2);

    field_128.field_8 = FP_FromDouble(0.55);
    field_106_current_state = eAbeStates::State_96_HopToFall_4559E0;
    field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
}

void Abe::State_29_HopLand_4523D0()
{
    if (field_20_animation.field_92_current_frame == 2
        && sInputKey_Hop_5550E0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
        field_F4 = eAbeStates::State_27_HopBegin_4521C0;
        field_F6_anim_frame = 5;
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_30_RunJumpBegin_4532E0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    // Check if about to jump into a wall
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(FP_FromInteger(10), field_C4_velx)) // TODO: OG bug, why no scaling?
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        field_B8_xpos += field_C4_velx;

        if (field_20_animation.field_92_current_frame == 0)
        {
            Abe_SFX_2_457A40(11, 0, 32767, this);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_F8 = field_BC_ypos;

            const FP velX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-7.6) : FP_FromDouble(7.6);
            field_C4_velx = field_CC_sprite_scale * velX;

            const FP velY = field_CC_sprite_scale * FP_FromDouble(-9.6);
            field_C8_vely = velY;
            field_BC_ypos += velY;

            Vnull_4081F0();

            field_106_current_state = eAbeStates::State_31_RunJumpMid_452C10;
            field_100_pCollisionLine = nullptr;
        }
    }

}

void Abe::State_31_RunJumpMid_452C10()
{
    //NOT_IMPLEMENTED();

    BaseGameObject* pfield_110_id = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    if (field_1A8_portal_id != -1)
    {
        IntoPortalStates_451990();
        return;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(FP_FromInteger(10), field_C4_velx) ||
        Is_Celling_Above_44E8D0())
    {
        field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const __int16 bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.80));

    sub_408C40();

    if (bCollision)
    {
        switch (pLine->field_8_type)
        {
        case eLineTypes::eFloor_0:
        case eLineTypes::eBackGroundFloor_4:
        case 32u:
        case 36u:
            field_100_pCollisionLine = pLine;
            field_106_current_state = eAbeStates::State_32_RunJumpLand_453460;
            field_B8_xpos = hitX;
            field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
            if (pLine->field_8_type == 32)
            {
                vOnCollisionWith_424EE0(
                { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos) },
                { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
            field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
            return;
        }
    }
    else
    {
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos - field_C4_velx),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos - field_C4_velx),
            FP_GetExponent(field_BC_ypos),
            Path_Hoist::kType));

        field_FC_pPathTLV = pHoist;

        bool checkCollision = false;
        if (pHoist)
        {
            field_FC_pPathTLV = pHoist;

            if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this) || pHoist->field_12_edge_type == Path_Hoist::EdgeType::eBoth) &&
                 pHoist->field_10_type != Path_Hoist::Type::eOffScreen)
            {
                checkCollision = true;
            }
        }
        else
        {
            Path_Edge* pEdgeTlv = static_cast<Path_Edge*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60))),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                Path_Edge::kType));

            field_FC_pPathTLV = pEdgeTlv;

            if (pEdgeTlv && pEdgeTlv->field_12_can_grab)
            {
                if (IsSameScaleAsEdge(pEdgeTlv, this) && (IsFacingSameDirectionAsEdge(pEdgeTlv, this) || pEdgeTlv->field_10_type == Path_Edge::Type::eBoth))
                {
                    checkCollision = true;
                }
            }
        }

        if (checkCollision)
        {
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y - 10),
                field_B8_xpos,
                FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y + 10),
                &pLine,
                &hitX,
                &hitY,
                field_D6_scale != 0 ? 1 : 16))
            {
                field_B8_xpos = FP_FromInteger((field_FC_pPathTLV->field_8_top_left.field_0_x + field_FC_pPathTLV->field_C_bottom_right.field_0_x) / 2);

                MapFollowMe_408D10(TRUE);
                field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
                field_100_pCollisionLine = pLine;
                field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);

                if (field_FC_pPathTLV->field_4_type != Path_Hoist::kType ||
                    FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y) >=
                    field_CC_sprite_scale * FP_FromInteger(70))
                {
                    field_106_current_state = eAbeStates::State_69_LedgeHangWobble_454EF0;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
                    field_F4 = eAbeStates::State_65_LedgeAscend_End_4548E0;
                    field_F6_anim_frame = 12;
                }

                if (!pfield_110_id)
                {
                    if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
                    {
                        vOnCollisionWith_424EE0(
                            { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos) },
                            { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
                            ObjList_5C1B78,
                            1,
                            (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
                    }
                }
            }
        }
        else
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
        }
    }

    if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return;
    }

    if (field_106_current_state != eAbeStates::State_31_RunJumpMid_452C10)
    {
        return;
    }

    field_128.field_8 = FP_FromDouble(0.75);
    field_106_current_state = eAbeStates::State_97_RunJumpFall_455A80;
    field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
}

void Abe::State_32_RunJumpLand_453460()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Abe_SFX_2_457A40(6, 0, 32767, this);
        
        MapFollowMe_408D10(TRUE);

        const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
        if (sInputKey_Left_5550D4 & pressed)
        {
            if (sInputKey_Hop_5550E0 & field_118_prev_held)
            {
                BaseGameObject* pPortal = Vsub_408FD0(3);
                if (pPortal)
                {
                    field_1A4 = 0;
                    field_1A8_portal_id = pPortal->field_8_object_id;
                    field_106_current_state = eAbeStates::State_30_RunJumpBegin_4532E0;
                    field_118_prev_held = 0;
                    return;
                }
                field_106_current_state = eAbeStates::State_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (pressed & sInputKey_Run_5550E8)
                {
                    field_106_current_state = eAbeStates::State_54_RunJumpLandRun_4538F0;
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                }
                else
                {
                    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), -ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                    {
                        field_106_current_state = eAbeStates::State_25_RunSlideStop_451330;
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                    }
                    else
                    {
                        field_106_current_state = eAbeStates::State_55_RunJumpLandWalk_453970;
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                    }
                }
            }
            else
            {
                field_106_current_state = eAbeStates::State_26_RunTurn_451500;
                field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                Abe_SFX_2_457A40(4, 0, 0x7FFF, this);
            }
        }
        else if (pressed & sInputKey_Right_5550D0)
        {
            if (sInputKey_Hop_5550E0 & field_118_prev_held)
            {
                BaseGameObject* pPortal = Vsub_408FD0(3);
                if (pPortal)
                {
                    field_1A4 = 0;
                    field_1A8_portal_id = pPortal->field_8_object_id;
                }
                field_106_current_state = eAbeStates::State_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_106_current_state = eAbeStates::State_26_RunTurn_451500;
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                Abe_SFX_2_457A40(4, 0, 32767, this);
            }
            else if (pressed & sInputKey_Run_5550E8)
            {
                field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                field_106_current_state = eAbeStates::State_54_RunJumpLandRun_4538F0;
            }
            else
            {
                if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                {
                    field_106_current_state = eAbeStates::State_25_RunSlideStop_451330;
                    field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                }
                else
                {
                    field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
                    field_106_current_state = eAbeStates::State_55_RunJumpLandWalk_453970;
                }
            }
        }
        else if (sInputKey_Hop_5550E0 & field_118_prev_held)
        {
            BaseGameObject* pPortal = Vsub_408FD0(2);
            if (pPortal)
            {
                field_1A4 = 0;
                field_1A8_portal_id = pPortal->field_8_object_id;
            }
            field_106_current_state = eAbeStates::State_27_HopBegin_4521C0;
            field_118_prev_held = 0;
        }
        else
        {
            field_106_current_state = eAbeStates::State_25_RunSlideStop_451330;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
            }
            Abe_SFX_2_457A40(4, 0, 32767, this);
        }
    }
}

void Abe::State_33_RunLoop_4508E0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
    }

    // Ran into wall?
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) || 
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
        ToKnockback_44E700(1, 1);
        return;
    }
    
    sub_44E9A0();
    
    if (field_106_current_state != eAbeStates::State_33_RunLoop_4508E0)
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
        return;
    }

    if (field_20_animation.field_92_current_frame && field_20_animation.field_92_current_frame != 8)
    {
        if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 12)
        {
            Abe_SFX_2_457A40(2, 0, 32767, this);
            
            MapFollowMe_408D10(TRUE);

            const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

            // Turning around?
            if (field_C4_velx > FP_FromInteger(0) && sInputKey_Left_5550D4 & pressed ||
                field_C4_velx < FP_FromInteger(0) && sInputKey_Right_5550D0 & pressed)
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
                field_106_current_state = eAbeStates::State_26_RunTurn_451500;
                Abe_SFX_2_457A40(4, 0, 32767, this);
                field_118_prev_held = 0;
                return;
            }

            // Run jump
            if (field_118_prev_held & sInputKey_Hop_5550E0)
            {
                BaseGameObject* pObj = Vsub_408FD0(3);
                if (pObj)
                {
                    field_1A4 = 0;
                    field_1A8_portal_id = pObj->field_8_object_id;
                }

                field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
                field_106_current_state = eAbeStates::State_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            // Run to roll
            if (field_118_prev_held & sInputKey_FartRoll_5550F0)
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
                field_106_current_state = eAbeStates::jState_38_RunToRoll_453A70;
                field_11C = 0;
                field_118_prev_held = 0;
                return;
            }

            if ((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed)
            {
                if (pressed & sInputKey_Run_5550E8)
                {
                    field_118_prev_held = 0;
                    return;
                }

                FP gridSize = {};
                if (field_C4_velx >= FP_FromInteger(0))
                {
                    gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }
                else
                {
                    gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }

                // Run to walk and hit wall
                if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize) ||
                    Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), gridSize))
                {
                    ToKnockback_44E700(1, 1);
                }
                else
                {
                    // Run to walk
                    if (field_20_animation.field_92_current_frame != 4)
                    {
                        field_106_current_state = eAbeStates::State_50_RunToWalk1_450E20;
                    }
                    else
                    {
                        field_106_current_state = eAbeStates::State_51_RunToWalk2_450F50;
                    }
                }
            }
            else
            {
                // No longer running or even moving, so slide stop
                field_106_current_state = eAbeStates::State_25_RunSlideStop_451330;
                Abe_SFX_2_457A40(4, 0, 32767, this);
            }

            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
            field_118_prev_held = 0;
            return;
        }
    }
    else
    {
        MapFollowMe_408D10(TRUE);

        if (field_118_prev_held & sInputKey_Hop_5550E0)
        {
            BaseGameObject* pObj = Vsub_408FD0(3);
            if (pObj)
            {
                field_1A4 = 0;
                field_1A8_portal_id = pObj->field_8_object_id;
            }
            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14);
            field_106_current_state = eAbeStates::State_30_RunJumpBegin_4532E0;
            field_118_prev_held = 0;
        }
    }
}

void Abe::State_34_DunnoBegin_44ECF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_35_DunnoEnd_44ED10;
    }
}

void Abe::State_35_DunnoEnd_44ED10()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_36_Null_45BC50()
{

}

void Abe::State_37_CrouchTurn_454390()
{
    if (field_20_animation.field_92_current_frame != 0)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_108_delayed_state)
            {
                field_106_current_state = field_108_delayed_state;
                field_108_delayed_state = 0;
            }
            else
            {
                field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
            }
        }
    }
    else
    {
        Abe_SFX_2_457A40(9, 0, 32767, this);
    }
}

void Abe::jState_38_RunToRoll_453A70()
{
    State_22_RollBegin_4539A0();
}

// Almost the same as State_6_WalkBegin_44FEE0
void Abe::State_39_StandingToRun_450D40()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_33_RunLoop_4508E0;
    }

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        sub_44E9A0();
    }
}

void Abe::State_40_SneakLoop_450550()
{
    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        sub_44E9A0();

        if (field_106_current_state == eAbeStates::State_40_SneakLoop_450550)
        {
            const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

            if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 13)
            {
                FP gridSize = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }
                else
                {
                    gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }
                
                // Hit wall, changed direction or no longer trying to sneak
                if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize) ||
                    Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), gridSize)
                    || (field_C4_velx > FP_FromInteger(0) && (sInputKey_Left_5550D4 & pressed))
                    || (field_C4_velx < FP_FromInteger(0) && (sInputKey_Right_5550D0 & pressed))
                    || !((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed))
                {
                    if (field_20_animation.field_92_current_frame != 3)
                    {
                        field_106_current_state = eAbeStates::State_46_SneakEnd_450870;
                    }
                    else
                    {
                        field_106_current_state = eAbeStates::jState_47_SneakEnd_4508C0;
                    }
                }
            }
            else if (field_20_animation.field_92_current_frame == 6 || field_20_animation.field_92_current_frame == 16)
            {
                Abe_SFX_2_457A40(3, 0, 32767, this);
                MapFollowMe_408D10(TRUE);
                if ((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed)
                {
                    // Sneak to walk
                    if (!(pressed & sInputKey_Sneak_5550EC))
                    {
                        if (field_20_animation.field_92_current_frame != 6)
                        {
                            field_106_current_state = eAbeStates::State_44_450500;
                        }
                        else
                        {
                            field_106_current_state = eAbeStates::State_42_4503D0;
                        }
                    }
                }
                field_118_prev_held = 0;
            }
        }
    }
}

// walk to sneak ??
void Abe::State_41_450250()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(10);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_40_SneakLoop_450550;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        sub_44E9A0();
    }
}

// TODO: sneak to walk ??
void Abe::State_42_4503D0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
    }
    
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_1_WalkLoop_44FBA0;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        sub_44E9A0();
    }
}

void Abe::State_43_450380()
{
    State_41_450250();

    if (field_106_current_state == eAbeStates::State_40_SneakLoop_450550)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
        field_106_current_state = eAbeStates::State_43_450380;
        field_F4 = eAbeStates::State_40_SneakLoop_450550;
        field_F6_anim_frame = 10;
    }
}

void Abe::State_44_450500()
{
    State_42_4503D0();

    if (field_106_current_state == eAbeStates::State_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
        field_106_current_state = eAbeStates::State_44_450500;
        field_F4 = eAbeStates::State_1_WalkLoop_44FBA0;
        field_F6_anim_frame = 9;
    }
}

void Abe::State_45_SneakBegin_4507A0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_40_SneakLoop_450550;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        sub_44E9A0();
    }
}

void Abe::State_46_SneakEnd_450870()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Abe_SFX_2_457A40(3, 0, 32767, this);
    }
    
    sub_44E9A0();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_408D10(TRUE);
        ToIdle_44E6B0();
    }
}

void Abe::jState_47_SneakEnd_4508C0()
{
    // TODO: Skipped jmp func jAbe::State_SneakEnd_40330F
    State_46_SneakEnd_450870();
}

// walk to run ?
void Abe::State_48_4500A0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_33_RunLoop_4508E0;
        field_118_prev_held = 0;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        sub_44E9A0();
    }
}

// walk to run 2?
void Abe::State_49_450200()
{
    State_48_4500A0();

    if (field_106_current_state == eAbeStates::State_33_RunLoop_4508E0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
        field_106_current_state = eAbeStates::State_49_450200;
        field_F4 = eAbeStates::State_33_RunLoop_4508E0;
        field_F6_anim_frame = 8;
    }
}

void Abe::State_50_RunToWalk1_450E20()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_1_WalkLoop_44FBA0;
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        sub_44E9A0();
    }
}

void Abe::State_51_RunToWalk2_450F50()
{
    State_50_RunToWalk1_450E20();

    if (field_106_current_state == eAbeStates::State_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
        field_106_current_state = eAbeStates::State_51_RunToWalk2_450F50;
        field_F4 = eAbeStates::State_1_WalkLoop_44FBA0;
        field_F6_anim_frame = 9;
    }
}

void Abe::State_52_RunTurn_ToRun_451710()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        sub_44E9A0();

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_state = eAbeStates::State_33_RunLoop_4508E0;
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Abe::State_53_RunTurn_ToWalk_451800()
{
    State_52_RunTurn_ToRun_451710();

    if (field_106_current_state == eAbeStates::State_33_RunLoop_4508E0)
    {
        field_106_current_state = eAbeStates::State_1_WalkLoop_44FBA0;
    }
}

void Abe::State_54_RunJumpLandRun_4538F0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        sub_44E9A0();
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_state = eAbeStates::State_33_RunLoop_4508E0;
        }
    }
}

void Abe::State_55_RunJumpLandWalk_453970()
{
    State_54_RunJumpLandRun_4538F0();

    if (field_106_current_state == eAbeStates::State_33_RunLoop_4508E0)
    {
        field_106_current_state = eAbeStates::State_1_WalkLoop_44FBA0;
    }
}

void Abe::State_56_4591F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_57_Dead_4589A0()
{
    NOT_IMPLEMENTED(); // WIP

    DeathFadeOut* pDeathFade_1 = static_cast<DeathFadeOut*>(sObjectIds_5C1B70.Find_449CF0(field_148_pFade));
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id));

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);

    if (field_110_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj)
        {
            if (pObj->field_4_typeId == BaseGameObject::Types::eLiftPoint_78)
            {
                // TODO: Lift LiftPoint::ctor_461030
                /*
                ((void(__thiscall *)(BaseGameObject *, _DWORD, _DWORD, _DWORD))pObj->field_0_VTbl->VRock.Rock__vsub_49E330)(
                    pObj,
                    0,
                    0,
                    0);
                */
            }
        }
        else
        {
            field_110_id = -1;
        }
    }

    switch (field_124_gnFrame)
    {
    case 0:
        Event_Broadcast_422BC0(kEventShooting | kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventHeroDying, this);
        field_128.field_8 = FP_FromInteger(0);
        field_128.field_C = 0;
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        field_118_prev_held = 0;
        field_124_gnFrame = field_124_gnFrame + 1;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 30;
        if (field_FC_pPathTLV && field_FC_pPathTLV->field_4_type == 4) // TODO: constant
        {
            auto pBird = alive_new<DeathBirdParticle>();
            if (pBird)
            {
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + field_BC_ypos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger((Math_NextRandom() % 64) - 32) + field_B8_xpos;
                pBird->ctor_43ECB0(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_128.field_0_gnFrame + 60,
                    1,
                    field_CC_sprite_scale);
            }
        }
        else
        {
            auto pBird = alive_new<DeathBirdParticle>();
            if (pBird)
            {
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + field_BC_ypos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + field_B8_xpos;
                pBird->ctor_43ECB0(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_128.field_0_gnFrame + 15,
                    1,
                    field_CC_sprite_scale);
            }
        }
        return;

    case 1:
        Event_Broadcast_422BC0(kEventHeroDying, this);
        if (!(sGnFrame_5C1B84 % 4))
        {
            if (field_FC_pPathTLV && field_FC_pPathTLV->field_4_type == 4) // TODO: Constant
            {
                auto pBird = alive_new<DeathBirdParticle>();
                if (pBird)
                {
                    const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + field_BC_ypos + FP_FromInteger(15);
                    const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + field_B8_xpos;
                    pBird->ctor_43ECB0(
                        xpos,
                        ypos,
                        (Math_NextRandom() % 8) + field_128.field_0_gnFrame + 60,
                        0,
                        field_CC_sprite_scale);
                }
            }
            else
            {
                auto pBird = alive_new<DeathBirdParticle>();
                if (pBird)
                {
                    const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + field_BC_ypos + FP_FromInteger(15);
                    const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + field_B8_xpos;
                    pBird->ctor_43ECB0(
                        xpos,
                        ypos,
                        (Math_NextRandom() % 8) + field_128.field_0_gnFrame + 15,
                        0,
                        field_CC_sprite_scale);
                }
            }
        }

        field_CC_sprite_scale -= FP_FromDouble(0.008);

        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;

        if (static_cast<int>(sGnFrame_5C1B84) <= field_128.field_0_gnFrame)
        {
            return;
        }

        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 60;

        if (field_FC_pPathTLV)
        {
            if (field_FC_pPathTLV->field_4_type == 4)
            {
                field_128.field_0_gnFrame = (sGnFrame_5C1B84 + 60) + 45;
            }
        }
        ++field_124_gnFrame;
        MusicController::sub_47FD60(11, this, 1, 0);
        return;
    case 2:
        Event_Broadcast_422BC0(kEventHeroDying, this);
        if ((signed int)sGnFrame_5C1B84 > field_128.field_0_gnFrame)
        {
            ++field_124_gnFrame;
        }
        return;
    case 3:
    {
        Event_Broadcast_422BC0(kEventHeroDying, this);
        if (pDeathFade_1)
        {
            pDeathFade_1->field_6_flags.Set(BaseGameObject::eDead);
            field_148_pFade = -1;
        }
        auto pDeathFade = alive_new<DeathFadeOut>();
        if (pDeathFade)
        {
            pDeathFade->ctor_427030(40, 1, 0, 8, 2);
            field_148_pFade = pDeathFade->field_8_object_id;
        }
        
        if (pCircularFade)
        {
            pCircularFade->field_6_flags.Set(BaseGameObject::eDead);
        }
        ++field_124_gnFrame;
    }
        return;

    case 4:
        Event_Broadcast_422BC0(kEventHeroDying, this);
        
        if (!pDeathFade_1->field_7E_bDone)
        {
            return;
        }
        
        if (field_10A)
        {
            return;
        }

        Vnull_4081F0();
        
        field_100_pCollisionLine = nullptr;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 8;
        ++field_124_gnFrame;
        // TODO
        //dword_5C2C64 = 0; // TODO: Never read ?
        return;

    case 5:
        Event_Broadcast_422BC0(kEventDeathReset, this);
        if (static_cast<int>(sGnFrame_5C1B84) <= field_128.field_0_gnFrame)
        {
            return;
        }
        Make_Circular_Fade_4CE8C0(
            FP_FromInteger(sActiveQuicksaveData_BAF7F8.field_204_world_info.field_C_controlled_x),
            FP_FromInteger(sActiveQuicksaveData_BAF7F8.field_204_world_info.field_E_controlled_y),
            sActiveQuicksaveData_BAF7F8.field_204_world_info.field_10_controlled_scale != 0 ? FP_FromDouble(1.0): FP_FromDouble(0.5),
            0,
            1,
            1);
        Quicksave_LoadActive_4C9170();
        return;
    default:
        return;
    }
}

void Abe::State_58_DeadPre_4593E0()
{
    if (field_120_state == 1)
    {
        field_106_current_state = eAbeStates::State_57_Dead_4589A0;
        field_124_gnFrame = 2;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 60;
    }
    else
    {
        Event_Broadcast_422BC0(kEventHeroDying, this);
    }
}

void Abe::State_59_Null_459450()
{

}

void Abe::State_60_4A3200()
{
    NOT_IMPLEMENTED();
}

void Abe::State_61_TurnToRun_456530()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        // Was going to run into something
        ToKnockback_44E700(1, 1);
    }
    else
    {
        sub_44E9A0();

        if (field_106_current_state == eAbeStates::State_61_TurnToRun_456530)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_state = eAbeStates::State_33_RunLoop_4508E0;
            }
        }
    }
}

void Abe::State_62_Punch_454750()
{
    NOT_IMPLEMENTED();
}

void Abe::State_63_Sorry_454670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_64_454730()
{
    NOT_IMPLEMENTED();
}

void Abe::State_65_LedgeAscend_End_4548E0()
{
    const __int16 curFrameNum = field_20_animation.field_92_current_frame;
    if (curFrameNum == 0)
    {
        Abe_SFX_2_457A40(10, 0, 32767, this);
    }
    else if (curFrameNum == 4)
    {
        Abe_SFX_2_457A40(11, 0, 32767, this);
        field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit1);
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Now the ascend is done go back to stand idle
        field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit1);
        MapFollowMe_408D10(TRUE);
        ToIdle_44E6B0();
    }
}

void Abe::State_66_LedgeDescend_454970()
{
    const __int16 curFrameNum = field_20_animation.field_92_current_frame;
    if (curFrameNum == 2)
    {
        Abe_SFX_2_457A40(11, 0, 32767, this);
        field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
    }
    else if (curFrameNum == 21)
    {
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos + FP_FromInteger(16)),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos + FP_FromInteger(16)),
            Path_Hoist::kType));

        if (pHoist)
        {
            if (FP_FromInteger((pHoist->field_C_bottom_right.field_2_y -1 * pHoist->field_8_top_left.field_2_y)) <
                field_CC_sprite_scale * FP_FromInteger(70))
            {
                field_BC_ypos = (FP_FromInteger(60) * field_CC_sprite_scale) + field_BC_ypos;

                PathLine* pLine = nullptr;
                FP hitX = {};
                FP hitY = {};

                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    field_B8_xpos,
                    field_BC_ypos - FP_FromInteger(5),
                    field_B8_xpos,
                    field_BC_ypos + FP_FromInteger(5),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_D6_scale != 0 ? 1 : 16))
                {
                    field_100_pCollisionLine = pLine;
                    field_BC_ypos = hitY;
                    field_106_current_state = eAbeStates::State_16_LandSoft_45A360;
                    field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit1);
                }
                else
                {
                    field_BC_ypos -= FP_FromInteger(60) * field_CC_sprite_scale;
                }
            }
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_67_LedgeHang_454E20;
    }
}

void Abe::State_67_LedgeHang_454E20()
{
    field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);
    const int pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed || field_114_flags.Get(Flags_114::e114_Bit10))
    {
        field_106_current_state = eAbeStates::State_65_LedgeAscend_End_4548E0;
    }
    else if (pressed & sInputKey_Down_5550DC)
    {
        Vnull_4081F0();
        field_100_pCollisionLine = nullptr;
        field_106_current_state = eAbeStates::State_91_FallingFromGrab_4557B0;
        field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(75);
        field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit1);
        field_F8 = field_BC_ypos;
    }
}

void Abe::State_68_ToOffScreenHoist_454B80()
{
    BaseGameObject* pfield_110_id = sObjectIds_5C1B70.Find_449CF0(field_110_id);

    // Get the current hoist - even though there is no need to?
    Path_TLV* pHoist = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Hoist::kType);

    // Find the hoist we are "connecting" to
    field_FC_pPathTLV = pHoist;
    field_BC_ypos -= field_CC_sprite_scale * FP_FromInteger(75);
    field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit1);

    const FP ypos = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y) - (FP_FromInteger(40) * field_CC_sprite_scale);
    pHoist = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(ypos),
        Path_Hoist::kType);

    field_FC_pPathTLV = pHoist;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (pHoist && sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            FP_FromInteger(pHoist->field_8_top_left.field_2_y - 10),
            field_B8_xpos,
            FP_FromInteger(pHoist->field_8_top_left.field_2_y + 10),
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
    {
        field_100_pCollisionLine = pLine;
        field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
        field_C8_vely = FP_FromInteger(0);
        if (!pfield_110_id)
        {
            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                vOnCollisionWith_424EE0(
                    { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos)},
                    { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        field_106_current_state = eAbeStates::State_67_LedgeHang_454E20;
    }
    else
    {
        field_106_current_state = eAbeStates::State_14_HoistIdle_452440;
    }
}

void Abe::State_69_LedgeHangWobble_454EF0()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_eBit13)))
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_eBit13);
            Abe_SFX_2_457A40(1, 0, 127, this);
        }
    }
    else
    {
        if (field_20_animation.field_92_current_frame == 2)
        {
            if (!(field_1AC_flags.Get(Flags_1AC::e1AC_eBit13)))
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit13);
                Abe_SFX_457EC0(0x10u, 45, -200, this);
            }
        }
        else
        {
            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13);
        }
    }
    
    // Going up the ledge on wobble?
    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed || field_114_flags.Get(Flags_114::e114_Bit10))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13);
        field_106_current_state = eAbeStates::State_65_LedgeAscend_End_4548E0;
    }
    // Going down the ledge wobble?
    else if (pressed & sInputKey_Down_5550DC)
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13);
        Vnull_4081F0();
        field_100_pCollisionLine = nullptr;
        field_106_current_state = eAbeStates::State_91_FallingFromGrab_4557B0;
        field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(75);
        field_E0_176_ptr->field_14_flags.Clear(Shadow::Flags::eBit1);
        field_F8 = field_BC_ypos;
    }
    // Now stabilized when wobble anim is done
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13);
        field_106_current_state = eAbeStates::State_67_LedgeHang_454E20;
    }
}

void Abe::State_70_RingRopePull_455AF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_71_Knockback_455090()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        sub_44EC10();
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_100_pCollisionLine)
        {
            if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
            {
                field_C4_velx = FP_FromInteger(0);
            }
            
            MoveWithVelocity_450FA0(FP_FromDouble(0.67));

            if ((gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_6
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_9)
                && field_20_animation.field_92_current_frame == 7)
            {
                Abe_SFX_2_457A40(6, 80, -200, this);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            if (field_20_animation.field_92_current_frame >= 6)
            {
                field_20_animation.SetFrame_409D50(5);
            }

            State_3_Fall_459B60();

            if (field_106_current_state == eAbeStates::State_84_FallLandDie_45A420)
            {
                field_106_current_state = eAbeStates::State_71_Knockback_455090;
                SFX_Play_46FA90(0x40u, 85, 0x10000);
                SND_SEQ_Play_4CAB10(9u, 1, 95, 95);
            }
            else if (field_106_current_state == eAbeStates::State_16_LandSoft_45A360)
            {
                field_106_current_state = eAbeStates::State_71_Knockback_455090;
                Abe_SFX_2_457A40(6, 80, -200, this);
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!(field_114_flags.Get(Flags_114::e114_Bit2)) && (field_100_pCollisionLine || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))))
        {
            if (field_10C_health > FP_FromInteger(0) || word_5C1BDA || field_114_flags.Get(Flags_114::e114_Bit7))
            {
                field_106_current_state = eAbeStates::State_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::State_72_KnockbackGetUp_455340()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_73_PushWall_4553A0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    if (field_20_animation.field_92_current_frame == 10)
    {
        if (Math_NextRandom() <= 127)
        {
            Abe_SFX_2_457A40(10, 0, 32767, this);
        }
    }
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_74_Rolling_KnockedBack_455290()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        sub_44EC10();
    }

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!(field_114_flags.Get(Flags_114::e114_Bit2)))
        {
            if (field_10C_health > FP_FromInteger(0) || word_5C1BDA)
            {
                field_106_current_state = eAbeStates::State_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }

}

void Abe::State_75_Jump_Into_Well_45C7B0()
{
    field_E0_176_ptr->field_14_flags.Clear(Shadow::eBit2);

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        field_20_animation.field_C_render_layer = 3;
    }
    else
    {
        field_20_animation.field_C_render_layer = 22;
    }
    jState_81_WellBegin_4017F8();
}

void Abe::State_76_45CA40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_77_45D130()
{
    NOT_IMPLEMENTED();
}

void Abe::State_78_WellBegin_45C810()
{
    if (field_20_animation.field_92_current_frame > 10)
    {
        field_E0_176_ptr->field_14_flags.Clear(Shadow::eBit2);

        // Get a local well
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Well_Local::kType);

        if (!field_FC_pPathTLV)
        {
            // No local well, must be an express well
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                Path_Well_Express::kType);
        }

        const short xpos = FP_GetExponent(field_B8_xpos);
        const short tlv_mid_x = (field_FC_pPathTLV->field_8_top_left.field_0_x + field_FC_pPathTLV->field_C_bottom_right.field_0_x) / 2;
        if (xpos > tlv_mid_x)
        {
            field_B8_xpos -= FP_FromInteger(1) * field_CC_sprite_scale;
        }
        else if (xpos < tlv_mid_x)
        {
            field_B8_xpos += FP_FromInteger(1) * field_CC_sprite_scale;
        }
    }

    if (field_20_animation.field_92_current_frame == 11)
    {
        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 3;
        }
        else
        {
            field_20_animation.field_C_render_layer = 22;
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_124_gnFrame = 15;

        SFX_Play_46FA90(21u, 0, field_CC_sprite_scale.fpValue);

        if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Well_Express::kType))
        {
            field_106_current_state = eAbeStates::State_82_Inside_Of_A_Well_Express_45CC80;
        }
        else
        {
            field_106_current_state = eAbeStates::State_79_Inside_Of_A_Well_Local_45CA60;
        }
    }
}

void Abe::State_79_Inside_Of_A_Well_Local_45CA60()
{
    const int gnFrame = field_124_gnFrame;
    field_124_gnFrame = gnFrame - 1;
    if (!gnFrame)
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Well_Local::kType);

        if (!field_FC_pPathTLV)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                Path_Well_Express::kType);
        }

        field_128.field_8 = FP_FromInteger(0);
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_Fall_To_Well);

        Path_Well_Base* pBaseWell = static_cast<Path_Well_Base*>(field_FC_pPathTLV);
        if (pBaseWell->field_4_type == Path_Well_Express::kType && !SwitchStates_Get_466020(pBaseWell->field_2_trigger_id))
        {
            Path_Well_Express* pExpress = static_cast<Path_Well_Express*>(pBaseWell);
            Calc_Well_Velocity_45C530(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                pExpress->field_18_exit_x, // TODO: Overlaps with pLocal->field_18_off_dx make part of well base ??
                pExpress->field_1A_exit_y);
        }
        else
        {
            Path_Well_Local* pLocal = static_cast<Path_Well_Local*>(pBaseWell);
            Calc_Well_Velocity_45C530(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                pLocal->field_1C_on_dx,
                pLocal->field_1E_on_dy);
        }

        MapFollowMe_408D10(TRUE);
        
        field_BC_ypos += field_C8_vely;

        if (field_C4_velx >= FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        
        SFX_Play_46FA90(0x14u, 0, field_CC_sprite_scale.fpValue);

        ++field_106_current_state;
        field_F8 = field_BC_ypos;

        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 3;
        }
        else
        {
            field_20_animation.field_C_render_layer = 22;
        }

        field_100_pCollisionLine = nullptr;
    }
}

void Abe::State_80_WellShotOut_45D150()
{
    if (field_C8_vely >= FP_FromInteger(0))
    {
        State_3_Fall_459B60();
    }
    else
    {
        field_C8_vely += field_CC_sprite_scale * FP_FromDouble(1.8);
        
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;

        sub_408C40();

        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);

    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted) ||
        (field_106_current_state != eAbeStates::State_80_WellShotOut_45D150 && field_106_current_state != eAbeStates::State_77_45D130))
    {
        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 13;
        }
        else
        {
            field_20_animation.field_C_render_layer = 32;
        }

        field_E0_176_ptr->field_14_flags.Set(Shadow::Flags::eBit2);
    }

    if (field_106_current_state == eAbeStates::State_84_FallLandDie_45A420)
    {
        field_10C_health = FP_FromInteger(1);
        field_106_current_state = eAbeStates::State_16_LandSoft_45A360;
    }
}

void Abe::jState_81_WellBegin_45C7F0()
{
    jState_81_WellBegin_4017F8();
}

void Abe::State_82_Inside_Of_A_Well_Express_45CC80()
{
    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Well_Local::kType);

    if (!field_FC_pPathTLV)
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Well_Express::kType);
    }

    Path_Well_Express* pExpressWell = static_cast<Path_Well_Express*>(field_FC_pPathTLV);
    if (SwitchStates_Get_466020(pExpressWell->field_2_trigger_id))
    {
        field_19A_to_level = pExpressWell->field_24_on_level;
        field_19C_to_path = pExpressWell->field_26_on_path;
        field_19E_to_camera = pExpressWell->field_28_on_camera;
        field_1A0_door_id = pExpressWell->field_2A_on_well_id;
    }
    else
    {
        field_19A_to_level = pExpressWell->field_1C_off_level;
        field_19C_to_path = pExpressWell->field_1E_off_path;
        field_19E_to_camera = pExpressWell->field_20_off_camera;
        field_1A0_door_id = pExpressWell->field_22_off_well_id;
    }

    field_128.field_8 = FP_FromInteger(0);
    field_F8 = field_BC_ypos;

    if (field_19A_to_level != gMap_5C3030.sCurrentLevelId_5C3030|| 
        field_19C_to_path != gMap_5C3030.sCurrentPathId_5C3032||
        field_19E_to_camera != gMap_5C3030.sCurrentCamId_5C3034)
    {
        field_124_gnFrame = 1;

        if (pExpressWell->field_32_movie_id)
        {
            gMap_5C3030.SetActiveCam_480D30(field_19A_to_level, field_19C_to_path, field_19E_to_camera, CameraSwapEffects::eEffect5_1_FMV, pExpressWell->field_32_movie_id, 0);
        }
        else
        {
            gMap_5C3030.SetActiveCam_480D30(field_19A_to_level, field_19C_to_path, field_19E_to_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }

        // FeeCo hack!
        if (field_19A_to_level == LevelIds::eFeeCoDepot_5 && field_19C_to_path == 1 && field_19E_to_camera == 1)
        {
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_B8_xpos = FP_FromInteger(1187);
            field_BC_ypos = FP_FromInteger(270);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);
            field_106_current_state = eAbeStates::jState_85_Fall_455070;
            field_20_animation.field_C_render_layer = 32;
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_106_current_state = eAbeStates::State_83_Shoot_Out_Of_A_Well_45CF70;
        }
    }
    else
    {
        State_83_Shoot_Out_Of_A_Well_45CF70();
        field_BC_ypos -= field_C8_vely * field_CC_sprite_scale;
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_Fall_To_Well);
        field_106_current_state = eAbeStates::State_79_Inside_Of_A_Well_Local_45CA60;
    }
}

void Abe::State_83_Shoot_Out_Of_A_Well_45CF70()
{
    PSX_Point camPos = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&camPos);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    Path_TLV* pTlvIter = nullptr;
    Path_Well_Base* pWell = nullptr;
    for (;;)
    {
        pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlvIter,
            FP_FromInteger(camPos.field_0_x),
            FP_FromInteger(camPos.field_2_y),
            FP_FromInteger(camPos.field_0_x + 368),
            FP_FromInteger(camPos.field_2_y + 240));

        // At the end, exit.
        if (!pTlvIter)
        {
            break;
        }

        // Is it a well?
        if (pTlvIter->field_4_type == Path_Well_Local::kType || pTlvIter->field_4_type == Path_Well_Express::kType)
        {
            // Is it the target of the previous well?
            Path_Well_Base* pWellBase = static_cast<Path_Well_Base*>(pTlvIter);
            if (pWellBase->field_4_well_id == field_1A0_door_id)
            {
                pWell = pWellBase;
                break;
            }
        }
    }

    field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
    field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;

    if (pWell)
    {
        if (pWell->field_0_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_D6_scale = 0;
        }
        else
        {
            field_CC_sprite_scale = FP_FromInteger(1);
            field_D6_scale = 1;
        }

        field_B8_xpos = FP_FromInteger((pWell->field_8_top_left.field_0_x + pWell->field_C_bottom_right.field_0_x) / 2);
        field_BC_ypos = FP_FromInteger(pWell->field_C_bottom_right.field_2_y);
        
        field_FC_pPathTLV = pWell;

        field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_Fall_To_Well);
        field_106_current_state = eAbeStates::State_79_Inside_Of_A_Well_Local_45CA60;
    }
    else
    {
        field_B8_xpos = FP_FromInteger(camPos.field_0_x + 184);
        field_BC_ypos = FP_FromInteger(camPos.field_2_y + 80);
        field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-2.68);
        field_C8_vely = field_CC_sprite_scale * FP_FromInteger(-15);
    }

}

void Abe::State_84_FallLandDie_45A420()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play_46FA90(0x40u, 85, 0x10000);
        SND_SEQ_Play_4CAB10(9u, 1, 95, 95);
        auto pShake = alive_new<ScreenShake>();
        if (pShake)
        {
            pShake->ctor_4ACF70(1, 0);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<int>(sGnFrame_5C1B84) >= field_124_gnFrame)
        {
            ToDieFinal_458910();
        }
    }

}

void Abe::jState_85_Fall_455070()
{
    State_3_Fall_459B60();
}

ALIVE_VAR(1, 0x5c2c68, int, sHandstoneSoundChannels_5C2C68, 0);

void Abe::State_86_HandstoneBegin_45BD00()
{
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id));
    DeathFadeOut* pFade = static_cast<DeathFadeOut*>(sObjectIds_5C1B70.Find_449CF0(field_148_pFade));

    switch (field_120_state)
    {
    case 0u:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            // Add ref
            ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSpotliteResID, TRUE, 0);

            CircularFade* pUnknown = Make_Circular_Fade_4CE8C0(
                field_B8_xpos,
                field_BC_ypos,
                field_CC_sprite_scale,
                1,
                0,
                0);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pUnknown->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            }
            else
            {
                pUnknown->field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            }

            field_14C_circular_fade_id = pUnknown->field_8_object_id;
            field_120_state = 1;
            
            SFX_Play_46FA90(0x54u, 90, 0x10000);

            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                Path_MovieStone::kType);

            sHandstoneSoundChannels_5C2C68 = SFX_Play_46FBA0(0xCu, 127, -300, 0x10000);
            
            int id = 0;
            Path_MovieStone* pMovieStoneTlv = static_cast<Path_MovieStone*>(field_FC_pPathTLV);
            if (!pMovieStoneTlv)
            {
                Path_HandStone* pHandStoneTlv = static_cast<Path_HandStone*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    Path_HandStone::kType));

                field_FC_pPathTLV = pHandStoneTlv;

                if (pHandStoneTlv)
                {
                    id = pHandStoneTlv->field_18_trigger_id;

                    field_184_fmv_id = pHandStoneTlv->field_10_scale; // TODO: Never used for this type?
                    for (int i = 0; i < 3; i++)
                    {
                        field_186_to_camera_id[i] = pHandStoneTlv->field_12_camera_ids[i];
                    }
                    field_18C_not_used = static_cast<short>(pHandStoneTlv->field_18_trigger_id); // TODO: Never used?
                }
            }
            else
            {
                id = pMovieStoneTlv->field_14_id;

                field_184_fmv_id = pMovieStoneTlv->field_10_movie_number;
                field_186_to_camera_id[0] = pMovieStoneTlv->field_12_scale; // TODO: Never used?
                field_186_to_camera_id[1] = static_cast<short>(pMovieStoneTlv->field_14_id);    // TODO: Never used?
            }

            if (field_FC_pPathTLV)
            {
                if (id > 1)
                {
                    SwitchStates_Set_465FF0(static_cast<short>(id), 1);
                }

                field_180_stone_type = field_FC_pPathTLV->field_4_type;
            }
            else
            {
                field_106_current_state = eAbeStates::State_87_HandstoneEnd_45C4F0;
            }
        }
        break;

    case 1u:
        if (pCircularFade->Vsub_4CE0B0())
        {
            if (field_180_stone_type == Path_MovieStone::kType)
            {
                pScreenManager_5BB5F4->field_40_flags |= 0x10000;

                FmvInfo* pFmvRec = Path_Get_FMV_Record_460F70(gMap_5C3030.sCurrentLevelId_5C3030, field_184_fmv_id);
                DWORD pos = 0;

                Get_fmvs_sectors_494460(pFmvRec->field_0_pName, 0, 0, &pos, 0, 0);
                sLevelId_dword_5CA408 = static_cast<DWORD>(gMap_5C3030.sCurrentLevelId_5C3030);

                Movie* pMovie = alive_new<Movie>();
                if (pMovie)
                {
                    pMovie->ctor_4DFDE0(pFmvRec->field_4_id, pos, pFmvRec->field_6_flags & 1, pFmvRec->field_8, pFmvRec->field_A_volume);
                }
                field_120_state = 2;
            }
            else if (field_180_stone_type == Path_HandStone::kType)
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_17C_cam_idx = 1;
                field_120_state = 4;
                pCircularFade->field_6_flags.Set(BaseGameObject::eDead);
                field_14C_circular_fade_id = -1;
                DeathFadeOut* pFade33 = alive_new<DeathFadeOut>();
                if (pFade33)
                {
                    pFade33->ctor_427030(40, 0, 0, 8, 2);
                }
 
                field_148_pFade = pFade33->field_8_object_id;
                field_19E_to_camera = gMap_5C3030.sCurrentCamId_5C3034;
                gMap_5C3030.SetActiveCam_480D30(field_C2_lvl_number, field_C0_path_number, field_186_to_camera_id[0], CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            }
        }
        break;

    case 2u:
        if (sMovie_ref_count_BB4AE4 == 0)
        {
            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager_5BB5F4->DecompressToVRam_40EF60((unsigned __int16 **)gMap_5C3030.field_2C_5C305C_camera_array[0]->field_C_pCamRes);
            pScreenManager_5BB5F4->field_40_flags |= 0x10000;
            pCircularFade->Vsub_4CE300(0, 0);
            field_120_state = 3;
        }
        break;

    case 3u:
        if (pCircularFade->Vsub_4CE0B0())
        {
            pCircularFade->field_6_flags.Set(BaseGameObject::eDead);
            field_14C_circular_fade_id = -1;

            field_106_current_state = eAbeStates::State_87_HandstoneEnd_45C4F0;

            if (sHandstoneSoundChannels_5C2C68)
            {
                SND_Stop_Channels_Mask_4CA810(sHandstoneSoundChannels_5C2C68);
                sHandstoneSoundChannels_5C2C68 = 0;
            }

            BYTE** ppResToFree = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSpotliteResID, 0, 0);
            ResourceManager::FreeResource_49C330(ppResToFree);
        }
        break;

    case 4u:
        if (pFade->field_7E_bDone)
        {
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held & 0x300000)
            {
                pFade->Init_427140(40, 1, 0, 8);
                field_120_state = 5;
                SFX_Play_46FA90(0x54u, 90, 0x10000);
            }
        }
        break;

    case 5u:
        if (pFade->field_7E_bDone)
        {
            if (field_17C_cam_idx < 3 && field_186_to_camera_id[field_17C_cam_idx] != 0)
            {
                field_120_state = 4;
              
                pFade->field_6_flags.Set(BaseGameObject::eDead);
                pFade = alive_new<DeathFadeOut>();

                if (pFade)
                {
                    pFade->ctor_427030(40, 0, 0, 8, 2);
                }
                
                field_148_pFade = pFade->field_8_object_id;

                gMap_5C3030.SetActiveCam_480D30(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_186_to_camera_id[field_17C_cam_idx++],
                    CameraSwapEffects::eEffect0_InstantChange,
                    0,
                    0);
            }
            else
            {
                field_120_state = 6;
            }
        }
        break;

    case 6u:
        if (pFade->field_7E_bDone)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_120_state = 7;
            gMap_5C3030.SetActiveCam_480D30(
                field_C2_lvl_number,
                field_C0_path_number,
                field_19E_to_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
        break;

    case 7u:
    {
        pFade->field_6_flags.Set(BaseGameObject::eDead);
        field_148_pFade = -1;

        CircularFade* pCircularFade2 = Make_Circular_Fade_4CE8C0(field_B8_xpos, field_BC_ypos, field_CC_sprite_scale, 0, 0, 0);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            pCircularFade2->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            pCircularFade2->field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }

        field_14C_circular_fade_id = pCircularFade2->field_8_object_id;
        field_120_state = 3;

        if (sHandstoneSoundChannels_5C2C68)
        {
            SND_Stop_Channels_Mask_4CA810(sHandstoneSoundChannels_5C2C68);
            sHandstoneSoundChannels_5C2C68 = 0;
        }
    }
        break;

    default:
        return;
    }
}

void Abe::State_87_HandstoneEnd_45C4F0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_88_GrenadeMachineUse_45C510()
{
    NOT_IMPLEMENTED();
}

void Abe::State_89_BrewMachineBegin_4584C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_90_BrewMachineEnd_4585B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_114_flags.Set(Flags_114::e114_Bit2);
        field_106_current_state = eAbeStates::State_0_Idle_44EEB0;
        field_124_gnFrame = 1;
    }
}

// Let go of an edge/hoist/pull ring
void Abe::State_91_FallingFromGrab_4557B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::jState_85_Fall_455070;
    }

    State_3_Fall_459B60();

    if (field_106_current_state == eAbeStates::State_84_FallLandDie_45A420)
    {
        field_106_current_state = eAbeStates::State_16_LandSoft_45A360;
    }

    field_F4 = eAbeStates::State_91_FallingFromGrab_4557B0;
}

void Abe::State_92_ForceDown_From_Hoist_455800()
{
    NOT_IMPLEMENTED();
}

void Abe::State_93_FallLedgeBegin_455970()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::jState_85_Fall_455070;
    }
    State_3_Fall_459B60();
}

void Abe::State_94_RunOffEdge_4559A0()
{
    State_93_FallLedgeBegin_455970();
}

void Abe::State_95_SneakOffEdge_4559C0()
{
    State_93_FallLedgeBegin_455970();
}

void Abe::State_96_HopToFall_4559E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B8_xpos = (field_CC_sprite_scale * FP_FromInteger(5)) + field_B8_xpos;
    }
    else
    {
        field_B8_xpos = field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(5));
    }

    field_C8_vely += (field_CC_sprite_scale * FP_FromInteger(4));
    State_93_FallLedgeBegin_455970();
}

void Abe::State_97_RunJumpFall_455A80()
{
    State_93_FallLedgeBegin_455970();
}

void Abe::State_98_RollOffEdge_455AA0()
{
    State_93_FallLedgeBegin_455970();
}

void Abe::State_99_LeverUse_455AC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_114_flags.Get(Flags_114::e114_Bit10))
        {
            field_106_current_state = eAbeStates::State_34_DunnoBegin_44ECF0;
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::State_100_Slap_Bomb_455B60()
{
    BaseAliveGameObject* pItem = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_160_slapable_or_pick_item_id));
    if (sActiveHero_5C1B68->field_20_animation.field_92_current_frame >= 6)
    {
        if (pItem)
        {
            pItem->VOnPickUpOrSlapped();
            field_160_slapable_or_pick_item_id = -1;
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
    }

}

void Abe::State_101_KnockForward_455420()
{
    NOT_IMPLEMENTED();
}

void Abe::State_102_455310()
{
    State_74_Rolling_KnockedBack_455290();
    if (field_106_current_state == eAbeStates::State_72_KnockbackGetUp_455340)
    {
        field_106_current_state = eAbeStates::jState_103_KnockbackGetUp_455380;
    }
}

void Abe::jState_103_KnockbackGetUp_455380()
{
    NOT_IMPLEMENTED();
}

void Abe::State_104_RockThrowStandingHold_455DF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_105_RockThrowStandingThrow_456460()
{
    NOT_IMPLEMENTED();
}

void Abe::State_106_RockThrowStandingEnd_455F20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_107_RockThrowCrouchingHold_454410()
{
    NOT_IMPLEMENTED();
}

void Abe::State_108_RockThrowCrouchingThrow_454500()
{
    NOT_IMPLEMENTED();
}

void Abe::State_109_455550()
{
    NOT_IMPLEMENTED();
}

void Abe::State_110_455670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_111_GrabRock_4564A0()
{
    NOT_IMPLEMENTED();
}

class Particle;

EXPORT Particle* New_Particle_45BC70(BaseAliveGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void Abe::State_112_Chant_45B1C0()
{
    BaseAliveGameObject* pfield_154 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_154_possesed_object_id));
    OrbWhirlWind* pfield_150 = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));

    if (field_120_state != 3 && field_120_state != 4)
    {
        SND_SEQ_PlaySeq_4CA960(10u, 0, 0);
    }

    if (!pfield_150)
    {
        field_150_OrbWhirlWind_id = -1;
    }

    switch (field_120_state)
    {
    case 0u:
    {
        Event_Broadcast_422BC0(kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventAbeOhm, this);
        
        BaseAliveGameObject* pObj = FindObjectToPosses_44B7B0(); // Find a victim

        if (field_168_ring_pulse_timer)
        {
            if (!field_16C_bHaveShrykull && !field_16E_bHaveInvisiblity && !(field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_bHaveHealing)))
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                AbilityRing::Factory_482F80(
                    FP_FromInteger((bRect.x + bRect.w) / 2),
                    FP_FromInteger((bRect.y + bRect.h) / 2),
                    RingTypes::eExplosive_Emit_1,
                    field_CC_sprite_scale);

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
                    field_170_invisible_timer = field_170_invisible_timer + field_176;
                }
                else
                {
                    field_170_invisible_timer = sGnFrame_5C1B84 + field_176;
                }

                field_174 = 0;

                InvisibleEffect* pInvisible = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
                if (!pInvisible || pInvisible->field_6_flags.Get(BaseGameObject::eDead))
                {
                    pInvisible = alive_new<InvisibleEffect>();
                    if (pInvisible)
                    {
                        pInvisible->ctor_45F280(this);
                    }
                    field_178_invisible_effect_id = pInvisible->field_8_object_id;
                }
                pInvisible->sub_45F9E0();
            }

            if (field_168_ring_pulse_timer)
            {
                if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_bHaveHealing))
                {
                    bool bAliveMudIsInSameScreen = false;
                    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
                    {
                        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        if (pObjIter->field_4_typeId == BaseGameObject::Types::eMudokon_110)
                        {
                            if (pObjIter->field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed)) // TODO: Is sick flag ?
                            {
                                if (pObjIter->Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent && pObjIter->field_10C_health > FP_FromInteger(0))
                                {
                                    bAliveMudIsInSameScreen = true;
                                }
                            }
                        }
                    }

                    if (bAliveMudIsInSameScreen)
                    {
                        PSX_RECT bRect = {};
                        vGetBoundingRect_424FD0(&bRect, 1);

                        AbilityRing::Factory_482F80(
                            FP_FromInteger((bRect.x + bRect.w) / 2),
                            FP_FromInteger((bRect.y + bRect.h) / 2),
                            RingTypes::eHealing_Emit_12,
                            field_CC_sprite_scale);

                        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit15_bHaveHealing);
                        field_168_ring_pulse_timer = 0;
                    }
                }
            }
        }

        // Stopped chanting?
        if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) || field_20_animation.field_92_current_frame == 3) && !Input_IsChanting_45F260())
        {
            field_106_current_state = eAbeStates::State_113_ChantEnd_45BBE0;
            field_154_possesed_object_id = -1;
            if (pfield_150)
            {
                pfield_150->sub_4E4050();
                field_150_OrbWhirlWind_id = -1;
            }
            return;
        }

        if (!(sGnFrame_5C1B84 % 4))
        {
            New_Particle_45BC70(this);
        }

        if (static_cast<int>(sGnFrame_5C1B84) >= field_124_gnFrame - 70)
        {
            if (pObj)
            {
                if (!pfield_150)
                {
                    auto pWhirlWind = alive_new<OrbWhirlWind>();
                    if (pWhirlWind)
                    {
                        const FP yPos = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(38));
                        const FP xOff = field_CC_sprite_scale * ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-4) : FP_FromInteger(4));
                        pWhirlWind->ctor_4E3C90(
                            xOff + field_B8_xpos,
                            yPos,
                            field_CC_sprite_scale,
                            0);
                    }
                    else
                    {
                        pWhirlWind = 0;
                    }
                    pfield_150 = pWhirlWind;
                    field_150_OrbWhirlWind_id = pWhirlWind->field_8_object_id;
                }
            }
            else
            {
                field_124_gnFrame = sGnFrame_5C1B84 + 70;
                if (pfield_150)
                {
                    pfield_150->sub_4E4050();
                    pfield_150 = nullptr;
                    field_150_OrbWhirlWind_id = -1;
                }
            }
        }


        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_gnFrame)
        {
            return;
        }

        if (!pObj)
        {
            return;
        }

        field_154_possesed_object_id = pObj->field_8_object_id;
        SFX_Play_46FBA0(0x11u, 0, -600, 0x10000);
        field_120_state = 1;
        field_124_gnFrame = sGnFrame_5C1B84 + 30;

        PSX_RECT bRect = {};
        pObj->vGetBoundingRect_424FD0(&bRect, 1);

        pfield_150->sub_4E3FD0(
            FP_FromInteger((bRect.w - bRect.x) / 2),
            FP_FromInteger((bRect.h - bRect.y) / 2),
            pObj->field_CC_sprite_scale,
            pObj);

        auto pClass_544FC8 = alive_new<PossessionFlicker>();
        if (pClass_544FC8)
        {
            pClass_544FC8->ctor_4319E0(sActiveHero_5C1B68, 30, 128, 255, 255);
        }
    }
    return;

    case 1u:
    {
        Event_Broadcast_422BC0(kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventAbeOhm, this);

        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_gnFrame)
        {
            if (!pfield_154 ||
                pfield_154->field_6_flags.Get(BaseGameObject::eDead) ||
                pfield_154->field_10C_health <= FP_FromInteger(0) ||
                pfield_154->Is_In_Current_Camera_424A70() != Map::CameraPos::eCamCurrent)
            {
                field_106_current_state = eAbeStates::State_113_ChantEnd_45BBE0;
                field_154_possesed_object_id = -1;
                if (pfield_150)
                {
                    pfield_150->sub_4E4050();
                    field_150_OrbWhirlWind_id = -1;
                }
            }
        }
        else
        {
            field_120_state = 2;
        }
    }
        return;

    case 2u:
    {
        Event_Broadcast_422BC0(kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventAbeOhm, this);
        field_150_OrbWhirlWind_id = -1;

        if (!pfield_154)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_state = eAbeStates::State_113_ChantEnd_45BBE0;
            }
            return;
        }

        if (pfield_154->field_6_flags.Get(BaseGameObject::eDead))
        {
            field_154_possesed_object_id = -1;
        }

        if (pfield_154->field_10C_health <= FP_FromInteger(0))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_state = eAbeStates::State_113_ChantEnd_45BBE0;
            }
            return;
        }

        sControlledCharacter_5C1B8C = pfield_154;

        pfield_154->Vnull_408F70();
        
        field_154_possesed_object_id = -1;

        if (sControlledCharacter_5C1B8C->field_4_typeId == BaseGameObject::Types::eSlig_125 ||
            sControlledCharacter_5C1B8C->field_4_typeId == BaseGameObject::Types::eFlyingSlig_54 ||
            sControlledCharacter_5C1B8C->field_4_typeId == BaseGameObject::Types::eCrawlingSlig_26 ||
            sControlledCharacter_5C1B8C->field_4_typeId == BaseGameObject::Types::eGlukkon_67)
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_bLaughAtChantEnd);
        }

        auto pFlicker = alive_new<PossessionFlicker>();
        if (pFlicker)
        {
            pFlicker->ctor_4319E0(sControlledCharacter_5C1B8C, 60, 128, 255, 255);
        }
        SND_SEQ_Stop_4CAE60(0xAu);
        SFX_Play_46FBA0(0x11u, 70, 400, 0x10000);
        field_120_state = 3;
    }
        return;

    case 3u:
    {
        if (sControlledCharacter_5C1B8C != this)
        {
            return;
        }

        auto pFlicker = alive_new<PossessionFlicker>();
        if (pFlicker)
        {
            pFlicker->ctor_4319E0(sControlledCharacter_5C1B8C, 15, 128, 255, 255);
        }

        field_120_state = 4;
        field_124_gnFrame = sGnFrame_5C1B84 + 15;
    }
        return;

    case 4u:
    {
        if (!(sGnFrame_5C1B84 % 4))
        {
            New_Particle_45BC70(this);
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_gnFrame)
        {
            return;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_state = eAbeStates::State_113_ChantEnd_45BBE0;
        }
    }
        return;

    case 6u:
    {
        Event_Broadcast_422BC0(kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventAbeOhm, this);
        if (!(sGnFrame_5C1B84 % 4))
        {
            New_Particle_45BC70(this);
        }
    }
        return;

    default:
        return;
    }
}

void Abe::State_113_ChantEnd_45BBE0()
{
    SND_SEQ_Stop_4CAE60(10u);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit9_bLaughAtChantEnd))
        {
            field_106_current_state = eAbeStates::State_9_SpeakMovement_45B180;
            Abe_SFX_457EC0(8u, 0, 0, this);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit9_bLaughAtChantEnd);
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::State_114_DoorEnter_459470()
{
    switch (field_120_state)
    {
    case 0u:
        if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return;
        }
        field_120_state = 2;
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 3;
        return;

    case 2u:
        if (field_128.field_0_gnFrame > static_cast<int>(sGnFrame_5C1B84))
        {
            return;
        }
        field_120_state = 3;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 3;
        return;

    case 3u:
        if (field_128.field_0_gnFrame <= static_cast<int>(sGnFrame_5C1B84))
        {
            field_120_state = 4;
        }
        return;

    case 4u:
    {
        Path_Door* pDoorTlv = static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Door::kType));

        field_FC_pPathTLV = pDoorTlv;

        if (pDoorTlv->field_42_cancel_throwables)
        {
            if (field_1A2_rock_or_bone_count > 0 && gpThrowableArray_5D1E2C)
            {
                gpThrowableArray_5D1E2C->Remove_49AA00(field_1A2_rock_or_bone_count);
                field_1A2_rock_or_bone_count = 0;
            }
        }

        // An OWI hack.. when both mudomo and mundanchee are done force back to necrum mines
        bool hackChange = false;
        if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudomoVault_Ender_11)
        {
            if (gMap_5C3030.sCurrentPathId_5C3032 == 13 &&
                gMap_5C3030.sCurrentCamId_5C3034 == 14 &&
                field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender))
            {
                hackChange = true;
            }
        }
        else if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudancheeVault_Ender_7)
        {
            if (gMap_5C3030.sCurrentPathId_5C3032 == 11 &&
                gMap_5C3030.sCurrentCamId_5C3034 == 2 &&
                field_1AE.Get(Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender))
            {
                hackChange = true;
            }
        }

        if (hackChange)
        {
            // Plays FMV where the weridos give Abe the drunk mud healing power and then dumps Abe at the portal that leads
            // back to Necrum mines.
            gMap_5C3030.SetActiveCam_480D30(LevelIds::eNecrum_2, 3, 10, CameraSwapEffects::eEffect5_1_FMV, 22, 0);
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_B8_xpos = FP_FromInteger(2287);
            field_BC_ypos = FP_FromInteger(800);
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_106_current_state = eAbeStates::jState_85_Fall_455070;
            field_CC_sprite_scale = FP_FromInteger(1);
            field_D6_scale = 1;
            field_20_animation.field_C_render_layer = 32;
            return;
        }

        gMap_5C3030.field_1E_door = 1;
        __int16 bForceChange = 0;
        const CameraSwapEffects effect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[pDoorTlv->field_32_wipe_effect];
        if (effect == CameraSwapEffects::eEffect5_1_FMV || effect == CameraSwapEffects::eEffect11)
        {
            bForceChange = 1;
        }

        gMap_5C3030.SetActiveCam_480D30(
            pDoorTlv->field_10_level,
            pDoorTlv->field_12_path,
            pDoorTlv->field_14_camera,
            effect,
            pDoorTlv->field_34_movie_number,
            bForceChange);

        field_120_state = 5;
        field_1A0_door_id = pDoorTlv->field_1C_target_door_number;
    }
    return;

    case 5u:
    {
        gMap_5C3030.field_1E_door = 0;
        field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
        field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;

        Path_Door* pDoorTlv2 = static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(Path_Door::kType, 0));
        field_FC_pPathTLV = pDoorTlv2;
        Path_Door* pTargetDoorTlv = pDoorTlv2;
        if (pTargetDoorTlv->field_18_door_number != field_1A0_door_id)
        {
            do
            {
                Path_Door* pDoorIter = static_cast<Path_Door*>(Path::TLV_Next_Of_Type_4DB720(field_FC_pPathTLV, Path_Door::kType));
                field_FC_pPathTLV = pDoorIter;
                pTargetDoorTlv = pDoorIter;
            } while (pTargetDoorTlv->field_18_door_number != field_1A0_door_id);
        }

        if (pTargetDoorTlv->field_16_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 13;
            field_D6_scale = 0;
        }
        else
        {
            field_CC_sprite_scale = FP_FromDouble(1.0);
            field_20_animation.field_C_render_layer = 32;
            field_D6_scale = 1;
        }

        // The door controls which way abe faces when he exits it
        if (pTargetDoorTlv->field_3E_abe_direction & 1)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }

        field_B8_xpos = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_0_x) + 
                        FP_FromInteger((field_FC_pPathTLV->field_C_bottom_right.field_0_x - field_FC_pPathTLV->field_8_top_left.field_0_x) / 2);


        MapFollowMe_408D10(TRUE);

        PathLine* pathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y),
            field_B8_xpos,
            FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y),
            &pathLine,
            &hitX,
            &hitY,
            (field_D6_scale == 1) ? 15 : 240))
        {
            field_100_pCollisionLine = pathLine;
            field_BC_ypos = hitY;
        }
        else
        {
            field_100_pCollisionLine = nullptr;
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);
            field_BC_ypos = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y);
            field_F8 = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y);
        }

        field_168_ring_pulse_timer = 0;
        InvisibleEffect* pInvisibleEffect = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
        if (pInvisibleEffect)
        {
            if (!(pInvisibleEffect->field_6_flags.Get(BaseGameObject::eDead)))
            {
                pInvisibleEffect->sub_45FA50();
                field_114_flags.Clear(Flags_114::e114_Bit8);
                field_178_invisible_effect_id = -1;
                field_170_invisible_timer = 0;
            }
        }

        field_10C_health = FP_FromInteger(1);
        field_120_state = 6;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 30;
    }
        return;

    case 6u:
        if (field_128.field_0_gnFrame > static_cast<int>(sGnFrame_5C1B84))
        {
            return;
        }

        field_120_state = 0;
        if (field_100_pCollisionLine)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_106_current_state = eAbeStates::State_115_DoorExit_459A40;
        }
        else
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            // Abe opens the door and he falls through the floor
            field_106_current_state = eAbeStates::jState_85_Fall_455070;
        }
        return;

    default:
        return;
    }

}

void Abe::State_115_DoorExit_459A40()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Now that we've exited the door, should it close behind us?
        if (static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            Path_Door::kType))->field_40_close_after_use)
        {
            // TODO: Ret ignored even in real ??
            FindObjectOfType_425180(
                BaseGameObject::Types::eDoor_33,
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(5));

            // Yes, so find it
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId == BaseGameObject::Types::eDoor_33)
                {
                    Door* pDoor = static_cast<Door*>(pObj);
                    if (pDoor->field_FA_door_number == field_1A0_door_id)
                    {
                        // And close it
                        pDoor->vClose_41EB50();
                        break;
                    }
                }
            }
        }

        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);

        VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        ToIdle_44E6B0();
    }

}

void Abe::State_116_MineCarEnter_458780()
{
    if (field_120_state == 0)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_120_state = 1;
            field_106_current_state = eAbeStates::State_117_In_MineCar_4587C0;
        }
    }
}

void Abe::State_117_In_MineCar_4587C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_118_MineCarExit_458890()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_114_flags.Set(Flags_114::e114_Bit2);
        field_106_current_state = eAbeStates::State_0_Idle_44EEB0;
        field_124_gnFrame = 1;
    }
}

void Abe::State_119_45A990()
{
    NOT_IMPLEMENTED();
}

void Abe::State_120_45AB00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_121_LiftGrabBegin_45A600()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    }

    field_C8_vely = FP_FromInteger(0);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_state = eAbeStates::State_123_LiftGrabIdle_45A6A0;
    }
}

void Abe::State_122_LiftGrabEnd_45A670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_123_LiftGrabIdle_45A6A0()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));

    FollowLift_45A500();

    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    }

    field_C8_vely = FP_FromInteger(0);

    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed)
    {
        if (!pLiftPoint->vOnTopFloor_461890())
        {
            field_106_current_state = eAbeStates::State_124_LiftUseUp_45A780;
        }
    }
    else if (pressed & sInputKey_Down_5550DC)
    {
        if (!pLiftPoint->vOnBottomFloor_4618F0())
        {
            field_106_current_state = eAbeStates::State_125_LiftUseDown_45A7B0;
        }
    }
    else if (pLiftPoint->vOnAnyFloor_461920())
    {
        // You ain't letting go unless you are on a floor where you can walk off..
        field_106_current_state = eAbeStates::State_122_LiftGrabEnd_45A670;
    }

}

void Abe::State_124_LiftUseUp_45A780()
{
    NOT_IMPLEMENTED();
}

void Abe::State_125_LiftUseDown_45A7B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_126_TurnWheelBegin_456700()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id));
        if (pWheel)
        {
            pWheel->VStartTurning();
        }
        field_106_current_state = eAbeStates::State_127_TurnWheelLoop_456750;
        field_120_state = 0;
    }
}

void Abe::State_127_TurnWheelLoop_456750()
{
    enum eStates
    {
        eTurningWheel = 0,
        eCheckForNoLongerTurningWheel = 1,
        eMapChanging = 2,
    };

    if (field_120_state == eTurningWheel || field_120_state == eCheckForNoLongerTurningWheel) // The state we enter the main state at
    {
        Path_LevelLoader* pLevelLoader = static_cast<Path_LevelLoader*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(Path_LevelLoader::kType, 0));
        if (pLevelLoader && SwitchStates_Get_466020(pLevelLoader->field_10_id))
        {
            field_120_state = eMapChanging;
            SND_SEQ_Play_4CAB10(31u, 1, 127, 127);
            auto pMusicTrigger = alive_new<MusicTrigger>();
            if (pMusicTrigger)
            {
                pMusicTrigger->ctor_47FF10(5, 0, 0, 0);
            }
            return;
        }
        else
        {
            // Must ALSO do logic below in this instance
            field_120_state = eCheckForNoLongerTurningWheel;
        }
    }

    if (field_120_state == eCheckForNoLongerTurningWheel)
    {
        if (!(sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Up_5550D8))
        {
            // Not holding up anymore, stop
            WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id));
            if (pWheel)
            {
                pWheel->VStopTurning(1);
            }
            field_164_wheel_id = -1;
            field_106_current_state = eAbeStates::State_128_TurnWheelEnd_4569A0;
        }
    }
    else if (field_120_state == eMapChanging)
    {
        // This happens for the mines tunnel 1 ender
        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            // When we've changed from the camera with the wheels to tunnel 2 this forces the falling state into the well
            // another tasty OWI hack..
            WorkWheel* pWorkWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id));
            if (pWorkWheel) // Most likely always nullptr here, maybe the whole "stop wheel" was an inlined func
            {
                pWorkWheel->VStopTurning(1);
            }
            field_164_wheel_id = -1;

            Path_AbeStart* pPathAbeStart = static_cast<Path_AbeStart*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(Path_AbeStart::kType, 0));
            field_B8_xpos = FP_FromInteger((pPathAbeStart->field_8_top_left.field_0_x + pPathAbeStart->field_C_bottom_right.field_0_x) / 2);
            field_BC_ypos = FP_FromInteger(pPathAbeStart->field_C_bottom_right.field_2_y);
            field_F8 = FP_FromInteger(pPathAbeStart->field_C_bottom_right.field_2_y);
            // TODO: OG bug, scale not read from the TLV ??

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            field_106_current_state = eAbeStates::jState_85_Fall_455070;
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);
        }
    }
}

void Abe::State_128_TurnWheelEnd_4569A0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_129_PoisonGasDeath_4565C0()
{
    // Play various choke/cough/death sounds as the dying animation progresses
    switch (field_20_animation.field_92_current_frame)
    {
    case 0:
        SFX_Play_46FBA0(81u, 127, 128, 0x10000);
        break;
    case 9:
        SFX_Play_46FBA0(81u, 127, 384, 0x10000);
        break;
    case 28:
        SFX_Play_46FBA0(81u, 127, 640, 0x10000);
        break;
    case 32:
        Abe_SFX_2_457A40(6, 80, 0, this);
        break;
    case 50:
        Abe_SFX_2_457A40(6, 100, -200, this);
        break;
    case 53:
        Abe_SFX_2_457A40(6, 50, -200, this);
        break;
    default:
        break;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        const int previousValue = field_124_gnFrame;
        field_124_gnFrame = previousValue - 1;
        if (previousValue == 0)
        {
            ToDieFinal_458910();
        }
    }

}

void Abe::jState_81_WellBegin_4017F8()
{
    State_78_WellBegin_45C810();
}

void Abe::ToDie_4588D0()
{
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_bShrivel);
    field_106_current_state = eAbeStates::State_56_4591F0;
    field_124_gnFrame = 0;
    field_10C_health = FP_FromInteger(0);
    MusicController::sub_47FD60(0, this, 1, 0);
}

void Abe::ToIdle_44E6B0()
{
    field_128.field_8 = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_124_gnFrame = sGnFrame_5C1B84;
    field_106_current_state = eAbeStates::State_0_Idle_44EEB0;
    field_118_prev_held = 0;
    field_11C = 0;
    MapFollowMe_408D10(TRUE);
}

void Abe::PickUpThrowabe_Or_PressBomb_454090(FP fpX, int fpY, int bStandToCrouch)
{
    BaseAliveGameObject* pSlapableOrCollectable = nullptr;
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eInteractive))
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->field_E4 && fpX <= pAliveObj->field_EC)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->field_E8 && yPos <= pAliveObj->field_F0_prev_base)
                {
                    pSlapableOrCollectable = pAliveObj;
                    field_160_slapable_or_pick_item_id = pAliveObj->field_8_object_id;
                }
            }
        }
    }

    if (pSlapableOrCollectable)
    {
        bool trySlapOrCollect = false;
        switch (pSlapableOrCollectable->field_4_typeId)
        {
        case Types::eTimedMine_10:
        case Types::eUXB_143:
            field_106_current_state = eAbeStates::State_100_Slap_Bomb_455B60;
            if (bStandToCrouch)
            {
                field_160_slapable_or_pick_item_id = -1;
            }
            trySlapOrCollect = true;
            break;

        case Types::eBone_11:
        case Types::eGrenade_65:
        case Types::eMeat_84:
        case Types::eRock_105:
            field_106_current_state = eAbeStates::State_111_GrabRock_4564A0;
            field_1A2_rock_or_bone_count += static_cast<char>(static_cast<BaseThrowable*>(pSlapableOrCollectable)->Vsub_448080()); // TOOD: Check types are correct
            if (!bThrowableIndicatorExists_5C112C)
            {
                auto pThrowableIndicator = alive_new<ThrowableTotalIndicator>();
                if (pThrowableIndicator)
                {
                    FP yoff = (field_CC_sprite_scale * FP_FromInteger(-30)) + field_BC_ypos;
                    FP xoff = field_CC_sprite_scale * FP_FromInteger(0);
                    pThrowableIndicator->ctor_431CB0(
                        xoff + field_B8_xpos,
                        yoff,
                        field_20_animation.field_C_render_layer,
                        field_20_animation.field_14_scale,
                        field_1A2_rock_or_bone_count,
                        1);
                }
            }
            trySlapOrCollect = true;
            break;

        case Types::eMine_88:
            field_160_slapable_or_pick_item_id = -1;
            trySlapOrCollect = true;
            break;
        default:
            break;
        }

        if (trySlapOrCollect)
        {
            if (field_106_current_state == eAbeStates::State_111_GrabRock_4564A0)
            {
                if (bStandToCrouch)
                {
                    SFX_Play_46FA90(28u, 0, field_CC_sprite_scale.fpValue);
                    pSlapableOrCollectable->VOnPickUpOrSlapped();
                    field_160_slapable_or_pick_item_id = -1;
                    field_106_current_state = eAbeStates::State_17_CrouchIdle_456BC0;
                }
            }
        }
    }
}

void Abe::Get_Shrykull_Resources_45AA20()
{
    field_10_resources_array.SetAt(25, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbemorphResID, TRUE, FALSE));
    field_10_resources_array.SetAt(26, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kShrmorphResID, TRUE, FALSE));
    field_10_resources_array.SetAt(27, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSplineResID, TRUE, FALSE));
}

__int16 Abe::ToLeftRightMovement_44E340()
{
    field_C8_vely = FP_FromInteger(0);
    if (sControlledCharacter_5C1B8C != this)
    {
        return 0;
    }

    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    const FP gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (sInputKey_Right_5550D0 & pressed)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_state = eAbeStates::State_2_StandingTurn_451830;
            return 1;
        }

        if (pressed & sInputKey_Run_5550E8)
        {
            field_C4_velx = gridSize / FP_FromInteger(4);
            field_106_current_state = eAbeStates::State_39_StandingToRun_450D40;
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            field_C4_velx = gridSize / FP_FromInteger(10);
            field_106_current_state = eAbeStates::State_45_SneakBegin_4507A0;
        }
        else
        {
            field_C4_velx = gridSize / FP_FromInteger(9);
            field_106_current_state = eAbeStates::State_6_WalkBegin_44FEE0;
        }

        if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize))
        {
            if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), gridSize))
            {
                return 1;
            }
        }

        // Walking into wall?
        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), gridSize))
        {
            PushWall_44E890();
            return 0;
        }

        field_106_current_state = eAbeStates::State_19_StandToCrouch_453DC0;
        return 1;
    }
    else if (pressed & sInputKey_Left_5550D4)
    {
        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_state = eAbeStates::State_2_StandingTurn_451830;
            return 1;
        }

        if (pressed & sInputKey_Run_5550E8)
        {
            field_C4_velx = -(gridSize / FP_FromInteger(4));
            field_106_current_state = eAbeStates::State_39_StandingToRun_450D40;
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            field_C4_velx = -(gridSize / FP_FromInteger(10));
            field_106_current_state = eAbeStates::State_45_SneakBegin_4507A0;
        }
        else
        {
            field_C4_velx = -(gridSize / FP_FromInteger(9));
            field_106_current_state = eAbeStates::State_6_WalkBegin_44FEE0;
        }

        if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), -gridSize))
        {
            if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), -gridSize))
            {
                return 1;
            }
        }

        // Walking into wall?
        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), -gridSize))
        {
            PushWall_44E890();
            return 0;
        }

        field_106_current_state = eAbeStates::State_19_StandToCrouch_453DC0;
        return 1;
    }

    return 0;
}

void Abe::TryHoist_44ED30()
{
    field_106_current_state = eAbeStates::State_13_HoistBegin_452B20;

    const FP xpos = field_B8_xpos;
    const FP ypos = field_BC_ypos - FP_FromInteger(10); // Look up 10 for a hoist

    Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(xpos),
        FP_GetExponent(ypos),
        FP_GetExponent(xpos),
        FP_GetExponent(ypos),
        Path_Hoist::kType));

    if (pHoist)
    {
        if (IsSameScaleAsHoist(pHoist, this))
        {
            if (!IsFacingSameDirectionAsHoist(pHoist, this) && pHoist->field_12_edge_type != Path_Hoist::EdgeType::eBoth)
            {
                // No so auto turn around to face it
                field_108_delayed_state = field_106_current_state;
                field_106_current_state = eAbeStates::State_2_StandingTurn_451830;
            }
        }

        field_FC_pPathTLV = pHoist;
    }
}

void CC Abe::Create_Fart_421D20()
{
    NOT_IMPLEMENTED();
}

__int16 Abe::TryEnterMineCar_4569E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

int Abe::sub_44EE10()
{
    // Looks for a door object and checks something
    NOT_IMPLEMENTED();
    return 1;
}

__int16 Abe::HandleDoAction_455BD0()
{
    NOT_IMPLEMENTED(); // TODO: Need to impl case 59

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
        nullptr,
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos);

    if (!pTlv)
    {
        // Why ya trying to use nothing?
        return eAbeStates::State_34_DunnoBegin_44ECF0;
    }

    for (;;)
    {
        switch (pTlv->field_4_type)
        {
        case Path_Well_Local::kType:
            field_FC_pPathTLV = pTlv;
            return eAbeStates::State_78_WellBegin_45C810;

        case Path_Switch::kType:
        {
            FP xpos = {};
            FP ypos = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                xpos = field_BC_ypos - FP_FromInteger(5);
                ypos = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                xpos = field_BC_ypos - FP_FromInteger(5);
                ypos = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
            }

            Switch* pSwitch = static_cast<Switch*>(FindObjectOfType_425180(BaseGameObject::Types::eLever_139, ypos, xpos));
            if (!pSwitch || !(pSwitch->Vsub_4D6050(field_B8_xpos < pSwitch->field_B8_xpos)))
            {
                return eAbeStates::State_34_DunnoBegin_44ECF0;
            }

            return eAbeStates::State_99_LeverUse_455AC0;
        }

        case Path_Well_Express::kType:
            field_FC_pPathTLV = pTlv;
            return eAbeStates::State_78_WellBegin_45C810;

        // TODO: Need the Object and TLV structures to impl this case
        case 59: // 059_Grenade_machine
        {
            /*
            pGrenadeMachine = FindObjectOfType_425180(
                66,
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(25)));
            if (!pGrenadeMachine || !(pGrenadeMachine->field_3C_vnull_408F90()))
            {
                return eAbeStates::State_34_DunnoBegin_44ECF0;
            }
            (pGrenadeMachine->field_0_VTbl->VBaseAliveGameObject.field_40_vnull_408F70)(pGrenadeMachine);
            */
            return eAbeStates::State_88_GrenadeMachineUse_45C510;
        }

        default:
            // Next TLV, exit if we're at the end
            pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                pTlv,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);

            if (!pTlv)
            {
                return eAbeStates::State_34_DunnoBegin_44ECF0;
            }
            continue;
        }
    }
}

void Abe::PushWall_44E890()
{
    field_C4_velx = FP_FromInteger(0);
    field_106_current_state = eAbeStates::State_73_PushWall_4553A0;
    Abe_SFX_2_457A40(9, 0, 32767, this);
}

void Abe::sub_44E9A0()
{
    const FP oldXPos = field_B8_xpos;
    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(
            &field_B8_xpos,
            &field_BC_ypos,
            field_C4_velx);
    }

    TrapDoor* pfield_110 = static_cast<TrapDoor*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (field_100_pCollisionLine &&  (field_D6_scale != 0 ? 1 : 16) & (1 << field_100_pCollisionLine->field_8_type))
    {
        // Handle trap door collision
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36) // TODO: Enum type
        {
            if (pfield_110)
            {
                pfield_110->Vsub_4DDDB0(this);
                field_110_id = -1;
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { static_cast<short>(bRect.w + 5), static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
        else if (pfield_110)
        {
            pfield_110->Vsub_4DDDB0(this);
            field_110_id = -1;
        }
    }
    else
    {
        field_100_pCollisionLine = 0;
        if (pfield_110)
        {
            pfield_110->Vsub_4DDDB0(this);
            field_110_id = -1;
        }

        field_118_prev_held = 0;

        switch (field_106_current_state)
        {
        case eAbeStates::State_1_WalkLoop_44FBA0:
        case eAbeStates::State_6_WalkBegin_44FEE0:
        case eAbeStates::State_4_WalkEndLeftFoot_44FFC0:
        case eAbeStates::State_5_WalkEndRightFoot_00450080:
            field_106_current_state = eAbeStates::State_93_FallLedgeBegin_455970;
            break;
        case eAbeStates::State_33_RunLoop_4508E0:
        case eAbeStates::State_39_StandingToRun_450D40:
            field_106_current_state = eAbeStates::State_94_RunOffEdge_4559A0;
            break;
        case eAbeStates::jState_38_RunToRoll_453A70:
        case eAbeStates::State_22_RollBegin_4539A0:
        case eAbeStates::State_23_RollLoop_453A90:
        case eAbeStates::State_24_453D00:
            field_106_current_state = eAbeStates::State_98_RollOffEdge_455AA0;
            break;
        default:
            field_106_current_state = eAbeStates::State_95_SneakOffEdge_4559C0;
            break;
        }
        
        field_128.field_8 = FP_FromDouble(0.3); // TODO: Check
        field_B8_xpos = oldXPos + field_C4_velx;
        field_F8 = field_BC_ypos;

        // TODO: OG bug, dead code due to switch default case ?
        if (field_106_current_state == eAbeStates::State_71_Knockback_455090 || field_106_current_state == eAbeStates::State_101_KnockForward_455420)
        {
            field_128.field_8 = FP_FromDouble(0.67); // TODO: Check
        }
    }
}

__int16 Abe::CrouchingGameSpeak_453E10()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (sInputKey_GameSpeak2_5550F8 & field_118_prev_held)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(10);
        Abe_SFX_457EC0(4u, 0, 0, this);
        field_106_current_state = eAbeStates::State_20_454550;
        return TRUE;
    }
    else if (sInputKey_GameSpeak3_555100 & field_118_prev_held)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(12);
        Abe_SFX_457EC0(6u, 0, 0, this);
        field_106_current_state = eAbeStates::jState_21_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(9);

        if (field_128.field_12_mood == 5 || field_128.field_12_mood == 6)
        {
            Abe_SFX_457EC0(0x13u, 0, 0, this);
            field_106_current_state = eAbeStates::State_20_454550;
        }
        else
        {
            if (field_128.field_12_mood == 3)
            {
                Abe_SFX_457EC0(0x14u, 0, 0, this);
            }
            else
            {
                Abe_SFX_457EC0(3u, 0, 0, this);
            }
            field_106_current_state = eAbeStates::State_20_454550;
        }
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak4_5550FC)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(21);
        Abe_SFX_457EC0(0x19u, 0, 0, this);
        field_106_current_state = eAbeStates::jState_21_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak6_555108)
    {
        Abe_SFX_457EC0(0x11u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(23);
        field_106_current_state = eAbeStates::jState_21_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak5_55510C)
    {
        Abe_SFX_457EC0(5u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(11);
        field_106_current_state = eAbeStates::State_20_454550;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak8_555110)
    {
        Abe_SFX_457EC0(0x1Au, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(22);
        field_106_current_state = eAbeStates::jState_21_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak7_555114)
    {
        Abe_SFX_457EC0(0x1Cu, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(24);
        field_106_current_state = eAbeStates::State_20_454550;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Abe::Is_Celling_Above_44E8D0()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - FP_FromInteger(5),
        field_B8_xpos,
        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(45)),
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 8 : 128) != 0;
}

void Abe::MoveWithVelocity_450FA0(FP velocityX)
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        const FP newVelX = field_C4_velx - (field_CC_sprite_scale * velocityX);
        field_C4_velx = newVelX;
        if (newVelX < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        const FP newVelX = (field_CC_sprite_scale * velocityX) + field_C4_velx;
        field_C4_velx = newVelX;
        if (newVelX > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    if (FP_GetExponent(field_C4_velx))
    {
        sub_44E9A0();
    }
}

__int16 Abe::RunTryEnterDoor_451220()
{
    // Can't be entering a door if we're not pressing up
    if (!(sInputKey_Up_5550D8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed))
    {
        return 0;
    }

    if (field_114_flags.Get(Flags_114::e114_Bit7))
    {
        return 0;
    }

    if (field_20_animation.field_92_current_frame < 4)
    {
        return 0;
    }
    
    // Are we actually on a door?
    Path_TLV* pDoorTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Door::kType);

    if (!pDoorTlv || !sub_44EE10())
    {
        return 0;
    }

    field_FC_pPathTLV = pDoorTlv;
    field_120_state = 0;
    field_106_current_state = eAbeStates::State_114_DoorEnter_459470;
    field_B8_xpos = FP_FromInteger((pDoorTlv->field_8_top_left.field_0_x + pDoorTlv->field_C_bottom_right.field_0_x) / 2);
    MapFollowMe_408D10(TRUE);
    return 1;
}

__int16 Abe::RunTryEnterWell_451060()
{
    if (!(sInputKey_Up_5550D8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed) ||
        field_114_flags.Get(Flags_114::e114_Bit7) ||
        field_20_animation.field_92_current_frame < 4)
    {
        return 0;
    }

    Path_Well_Local* pWellLocal = static_cast<Path_Well_Local*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Well_Local::kType));
    if (pWellLocal)
    {
        if (!(field_114_flags.Get(Flags_114::e114_Bit10)))
        {
            if ((pWellLocal->field_0_scale == 0 && field_CC_sprite_scale == FP_FromInteger(1)) ||
                (pWellLocal->field_0_scale == 1 && field_CC_sprite_scale == FP_FromDouble(0.5)))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_Fall_To_Well);
                field_FC_pPathTLV = pWellLocal;
                field_106_current_state = eAbeStates::State_78_WellBegin_45C810;
                return 1;
            }
        }
    }

    Path_Well_Express* pWellExpress = static_cast<Path_Well_Express*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        Path_Well_Express::kType));
    if (pWellExpress)
    {
        if (!(field_114_flags.Get(Flags_114::e114_Bit10)))
        {
            if ((pWellExpress->field_0_scale == 0 && field_CC_sprite_scale == FP_FromInteger(1)) ||
                (pWellExpress->field_0_scale == 1 && field_CC_sprite_scale == FP_FromDouble(0.5)))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_Fall_To_Well);
                field_FC_pPathTLV = pWellExpress;
                field_106_current_state = eAbeStates::jState_81_WellBegin_45C7F0;
                return 1;
            }
        }
    }

    return 0;
}

void Abe::ToDieFinal_458910()
{
    InvisibleEffect* pObj = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
    if (pObj)
    {
        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
        {
            pObj->sub_45FA50();
            field_178_invisible_effect_id = -1;
        }
    }

    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_bShrivel);
    field_114_flags.Clear(Flags_114::e114_Bit1);
    field_106_current_state = eAbeStates::State_57_Dead_4589A0;
    field_124_gnFrame = 0;
    field_10C_health = FP_FromInteger(0);

    // Death jingle ?
    MusicController::sub_47FD60(11, this, 1, 0);
}

short Abe::DoGameSpeak_45AB70(int input)
{
    short nextState = -1;
    if ((sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & eChant) == eChant)
    {
        field_124_gnFrame = sGnFrame_5C1B84 + 90;
        SND_SEQ_PlaySeq_4CA960(10u, 0, 1);
        field_120_state = 0;
        nextState = eAbeStates::State_112_Chant_45B1C0;
    }
    else if (sInputKey_GameSpeak2_5550F8 & input)
    {
        Abe_SFX_457EC0(4u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(10);
        nextState = eAbeStates::State_7_Speak_45B140;
    }
    else if (input & sInputKey_GameSpeak3_555100)
    {
        Abe_SFX_457EC0(6u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(12);
        nextState = eAbeStates::State_8_Speak_45B160;
    }
    else if (input & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(9);
        if (field_128.field_12_mood == 5 || field_128.field_12_mood == 6)
        {
            Abe_SFX_457EC0(19u, 0, 0, this);
            nextState = eAbeStates::State_9_SpeakMovement_45B180;
        }
        else
        {
            if (field_128.field_12_mood == 3)
            {
                Abe_SFX_457EC0(20u, 0, 0, this);
            }
            else
            {
                Abe_SFX_457EC0(3u, 0, 0, this);
            }
            nextState = eAbeStates::State_9_SpeakMovement_45B180;
        }
    }
    else if (input & sInputKey_GameSpeak4_5550FC)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(21);
        Abe_SFX_457EC0(25u, 0, 0, this);
        nextState = eAbeStates::State_10_Fart_45B1A0;
    }
    else if (sInputKey_GameSpeak6_555108 & input)
    {
        Abe_SFX_457EC0(17u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(23);
        nextState = eAbeStates::State_9_SpeakMovement_45B180;
    }
    else if (input & sInputKey_GameSpeak5_55510C)
    {
        FP gridSize = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + gridSize, field_BC_ypos - FP_FromInteger(5)))
        {
            nextState = eAbeStates::State_62_Punch_454750;
        }
        else if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + (gridSize * FP_FromInteger(2)), field_BC_ypos - FP_FromInteger(5)))
        {
            nextState = eAbeStates::State_62_Punch_454750;
        }
        else
        {
            // NOTE: Extra check for locks, it must also be being rendered in order to for us to try to hit it
            BaseAnimatedWithPhysicsGameObject* pObj = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eLockedSoul_61, field_B8_xpos + gridSize, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale)));
            if (pObj && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                nextState = eAbeStates::State_62_Punch_454750;
            }
            else if (FindObjectOfType_425180(Types::eSlig_125, field_B8_xpos + gridSize, field_BC_ypos - FP_FromInteger(5)))
            {
                nextState = eAbeStates::State_62_Punch_454750;
            }
            else if (FindObjectOfType_425180(Types::eSlig_125, field_B8_xpos + (gridSize *  FP_FromInteger(2)), field_BC_ypos - FP_FromInteger(5)))
            {
                nextState = eAbeStates::State_62_Punch_454750;
            }
            else if (FindObjectOfType_425180(Types::eGlukkon_67, field_B8_xpos + gridSize, field_BC_ypos - FP_FromInteger(5)))
            {
                nextState = eAbeStates::State_62_Punch_454750;
            }
            else if (FindObjectOfType_425180(Types::eGlukkon_67, field_B8_xpos + (gridSize * FP_FromInteger(2)), field_BC_ypos - FP_FromInteger(5)))
            {
                nextState = eAbeStates::State_62_Punch_454750;
            }
            else
            {
                // Didn't hit anything, just anger
                Abe_SFX_457EC0(5u, 0, 0, this);
                pEventSystem_5BC11C->PushEvent_4218D0(11);
                nextState = eAbeStates::State_8_Speak_45B160;
            }
        }
    }
    else if (input & sInputKey_GameSpeak8_555110)
    {
        Abe_SFX_457EC0(26u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(22);
        nextState = 7;
    }
    else if (input & sInputKey_GameSpeak7_555114)
    {
        FP gridSize = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        pEventSystem_5BC11C->PushEvent_4218D0(24);
        if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + gridSize, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(40))))
        {
            nextState = eAbeStates::State_63_Sorry_454670;
        }
        else
        {
            Abe_SFX_457EC0(28u, 0, 0, this);
            nextState = eAbeStates::State_10_Fart_45B1A0; // TODO: Correct but isn't fart in this case ??
        }
    }

    return nextState;
}

__int16 Abe::CantBeDamaged_44BAB0()
{
    switch (field_106_current_state)
    {
    case eAbeStates::State_75_Jump_Into_Well_45C7B0:
    case eAbeStates::State_76_45CA40:
    case eAbeStates::State_77_45D130:
    case eAbeStates::State_78_WellBegin_45C810:
    case eAbeStates::State_79_Inside_Of_A_Well_Local_45CA60:
    case eAbeStates::State_80_WellShotOut_45D150:
    case eAbeStates::jState_81_WellBegin_45C7F0:
    case eAbeStates::State_82_Inside_Of_A_Well_Express_45CC80:
    case eAbeStates::State_83_Shoot_Out_Of_A_Well_45CF70:
    case eAbeStates::State_114_DoorEnter_459470:
    case eAbeStates::State_115_DoorExit_459A40:
    case eAbeStates::State_119_45A990:
    case eAbeStates::State_120_45AB00:
        return TRUE;
    }

    // TODO: Unknown what this is checking, condition should probably be inverted
    if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_bShrivel)) && field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render) || field_114_flags.Get(Flags_114::e114_Bit7))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

__int16 Abe::sub_44EC10()
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT __int16 Abe::ForceDownIfHoisting_44BA30()
{
    if (field_106_current_state != eAbeStates::State_65_LedgeAscend_End_4548E0 &&
        field_106_current_state != eAbeStates::State_67_LedgeHang_454E20 &&
        field_106_current_state != eAbeStates::State_69_LedgeHangWobble_454EF0 &&
        field_106_current_state != eAbeStates::State_66_LedgeDescend_454970 &&
        field_106_current_state != eAbeStates::State_68_ToOffScreenHoist_454B80)
    {
        return 0;
    }

    field_106_current_state = eAbeStates::State_92_ForceDown_From_Hoist_455800;

    field_124_gnFrame = 0;

    field_114_flags.Clear(Flags_114::e114_Bit1);
    field_114_flags.Set(Flags_114::e114_Bit2);

    return 1;
}

__int16 Abe::sub_44C980(BaseAnimatedWithPhysicsGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void Abe::GiveControlBackToMe_44BA10()
{
    sControlledCharacter_5C1B8C = this;
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6);
}

PullRingRope* Abe::GetPullRope_44D120()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find a rope
        if (pObj->field_4_typeId == BaseGameObject::Types::ePullRope_103)
        {
            // Is it on the same scale as us?
            PullRingRope* pRope = static_cast<PullRingRope*>(pObj);
            if (pRope->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                PSX_RECT bRect = {};
                pRope->vGetBoundingRect_424FD0(&bRect, 1);

                // Check we are near its ypos
                if ((field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(75))) <= pRope->field_BC_ypos &&
                    field_BC_ypos > pRope->field_BC_ypos)
                {
                    // Check we are near its xpos
                    if (field_B8_xpos > FP_FromInteger(bRect.x) && field_B8_xpos < FP_FromInteger(bRect.w))
                    {
                        // Found a rope we can pull
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
    NOT_IMPLEMENTED();
}

void Abe::Calc_Well_Velocity_45C530(short x1, short y1, short x2, short y2)
{
    PSX_Point abeSpawnPos = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&abeSpawnPos);

    const FP gravity = field_CC_sprite_scale == FP_FromInteger(1) ? FP_FromDouble(1.8) : FP_FromDouble(0.9);
    const FP xd = FP_FromInteger(x2 - x1);
    FP v16 = {};
    if (y2 > 0)
    {
        const int y1Offset = abeSpawnPos.field_2_y + y1;
        if (y2 > y1Offset)
        {
            const FP yd = FP_FromInteger(y2 - y1Offset);
            FP v10 = {};
            if (yd <= (FP_FromInteger(41) * field_CC_sprite_scale))
            {
                v10 = FP_FromInteger(0);
            }
            else
            {
                v10 = yd - (FP_FromInteger(41) * field_CC_sprite_scale);
            }

            const FP v11 = FP_FromInteger(20) * field_CC_sprite_scale;
            FP v12 = (v10 / v11) + FP_FromDouble(20.01);
            if (x2 > 0)
            {
                field_C4_velx = (xd - FP_FromInteger(abeSpawnPos.field_0_x)) / v12;
            }
            else
            {
                field_C4_velx = FP_FromDouble(2.796) * field_CC_sprite_scale;
            }
            FP v14 = field_CC_sprite_scale;
            field_C8_vely = FP_FromDouble(-16.1) * v14;
            return;
        }

        const int v15 = y1Offset - y2;
        if (v15 >= 0)
        {
            v16 = FP_FromInteger(v15);
        }
        else
        {
            v16 = FP_FromInteger(y2 - y1Offset);
        }
    }
    else
    {
        v16 = FP_FromInteger(0);
    }

    const FP v17 = (FP_FromInteger(80) * field_CC_sprite_scale) + v16;
    const FP v18 = FP_FromInteger(8) * gravity;
    const FP v20 = Math_SquareRoot_FP_496E90(((v18 * v17) + (gravity * gravity)).fpValue);
    field_C8_vely = (v20 - gravity) * FP_FromDouble(0.5);

    const FP v22 = field_C8_vely / gravity;
    field_C8_vely = -field_C8_vely;

    if (x2 > 0)
    {
        field_C4_velx = (xd - FP_FromInteger(abeSpawnPos.field_0_x)) / (v22 + FP_FromDouble(8.9));
    }
    else
    {
        field_C4_velx = FP_FromDouble(2.796) * field_CC_sprite_scale;
    }
}

void Abe::FollowLift_45A500()
{
    LiftPoint* pLift = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pLift)
    {
        field_C8_vely = pLift->field_C8_vely;
        if (pLift->field_6_flags.Get(BaseGameObject::eDead))
        {
            Vnull_4081F0();
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit1);
        }
        sub_408C40();
    }
}

// TODO: Clean up
EXPORT void CC Abe_SFX_457EC0(unsigned __int8 idx, __int16 volume, int pitch, Abe* pHero)
{
    //NOT_IMPLEMENTED();

    //DEV_CONSOLE_PRINTF("Abe SFX: %i", idx);

    switch (idx)
    {
    case 14u:
        if (pHero && pHero->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            SND_SEQ_Play_4CAB10(0x10u, 1, 90, 90);
        }
        else
        {
            SND_SEQ_Play_4CAB10(0x10u, 1, 127, 127);
        }
        break;
    case 26u:
        if (pHero && pHero->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            SND_SEQ_Play_4CAB10(0x12u, 1, 80, 80);
        }
        else
        {
            SND_SEQ_Play_4CAB10(0x12u, 1, 115, 115);
        }
        break;
    case 8u:
        if (pHero == sActiveHero_5C1B68 && gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_Ender_10)
        {
            idx = 10;
        }
        // Fall through
    default:
        if (!volume)
        {
            volume = sAbeSFXList_555250[idx].field_3_default_volume;
        }

        // OG bug - isn't null checked in other cases before de-ref?
        if (!pHero)
        {
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume,  static_cast<short>(pitch),  static_cast<short>(pitch));
            return;
        }

        if (pHero->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            volume = 2 * volume / 3;
        }

        if (pHero == sActiveHero_5C1B68)
        {
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, static_cast<short>(pitch), static_cast<short>(pitch));
            return;
        }

        switch (gMap_5C3030.sub_4811A0(
            pHero->field_C2_lvl_number,
            pHero->field_C0_path_number,
            pHero->field_B8_xpos,
            pHero->field_BC_ypos))
        {
        case 0:
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, static_cast<short>(pitch), static_cast<short>(pitch));
            break;
        case 1:
        case 2:
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], 2 * volume / 3, static_cast<short>(pitch), static_cast<short>(pitch));
            break;
        case 3:
            // TODO: Deoptimise math
            SFX_SfxDefinition_Play_4CA700(
                &sAbeSFXList_555250[idx],
                ((unsigned int)((unsigned __int64)(2863311532ULL * (signed __int16)volume) >> 32) >> 31)
                + ((unsigned __int64)(2863311532ULL * (signed __int16)volume) >> 32),
                2 * (signed __int16)volume / 9,
                static_cast<short>(pitch), static_cast<short>(pitch));
            break;
        case 4:
            // TODO: Deoptimise math
            SFX_SfxDefinition_Play_4CA700(
                &sAbeSFXList_555250[idx],
                ((unsigned int)((unsigned __int64)(1908874354ULL * (signed __int16)volume) >> 32) >> 31)
                + ((signed int)((unsigned __int64)(1908874354ULL * (signed __int16)volume) >> 32) >> 1),
                2 * (signed __int16)volume / 3,
                static_cast<short>(pitch), static_cast<short>(pitch));
            break;
        default:
            break;
        }
    }
}

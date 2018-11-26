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

using TAbeStateFunction = decltype(&Abe::State_0_Idle_44EEB0);

#define ABE_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_44EEB0) \
    ENTRY(State_1_WalkLoop_44FBA0) \
    ENTRY(State_2_StandingTurn_451830) \
    ENTRY(State_3_Fall_459B60) \
    ENTRY(State_4_WalkEndLeftFoot_44FFC0) \
    ENTRY(State_5_WalkEndRightFoot_00450080) \
    ENTRY(State_6_WalkBegin_44FEE0) \
    ENTRY(State_7_45B140) \
    ENTRY(State_8_45B160) \
    ENTRY(State_9_45B180) \
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
    ENTRY(jState_38_RollBegin_453A70) \
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
    ENTRY(State_52_451710) \
    ENTRY(State_53_451800) \
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
    ENTRY(State_65_LedgeAscend_4548E0) \
    ENTRY(State_66_LedgeDescend_454970) \
    ENTRY(State_67_LedgeHang_454E20) \
    ENTRY(State_68_454B80) \
    ENTRY(State_69_LedgeHangWobble_454EF0) \
    ENTRY(State_70_RingRopePull_455AF0) \
    ENTRY(State_71_Knockback_455090) \
    ENTRY(State_72_KnockbackGetUp_455340) \
    ENTRY(State_73_PushWall_4553A0) \
    ENTRY(State_74_455290) \
    ENTRY(State_75_Jump_Into_Well_45C7B0) \
    ENTRY(State_76_45CA40) \
    ENTRY(State_77_45D130) \
    ENTRY(State_78_WellBegin_45C810) \
    ENTRY(State_79_WellInside_45CA60) \
    ENTRY(State_80_WellShotOut_45D150) \
    ENTRY(jState_81_WellBegin_45C7F0) \
    ENTRY(State_82_45CC80) \
    ENTRY(State_83_45CF70) \
    ENTRY(State_84_FallLandDie_45A420) \
    ENTRY(jState_85_Fall_455070) \
    ENTRY(State_86_HandstoneBegin_45BD00) \
    ENTRY(State_87_HandstoneEnd_45C4F0) \
    ENTRY(State_88_GrenadeMachineUse_45C510) \
    ENTRY(State_89_BrewMachineBegin_4584C0) \
    ENTRY(State_90_BrewMachineEnd_4585B0) \
    ENTRY(State_91_RingRopePullEnd_4557B0) \
    ENTRY(State_92_455800) \
    ENTRY(State_93_FallLedgeBegin_455970) \
    ENTRY(jState_94_FallLedgeBegin_4559A0) \
    ENTRY(jState_95_FallLedgeBegin_4559C0) \
    ENTRY(State_96_4559E0) \
    ENTRY(jState_97_FallLedgeBegin_455A80) \
    ENTRY(jState_98_FallLedgeBegin_455AA0) \
    ENTRY(State_99_LeverUse_455AC0) \
    ENTRY(State_100_455B60) \
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
    ENTRY(State_117_4587C0) \
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
    &Abe::State_7_45B140,
    &Abe::State_8_45B160,
    &Abe::State_9_45B180,
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
    &Abe::jState_38_RollBegin_453A70,
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
    &Abe::State_52_451710,
    &Abe::State_53_451800,
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
    &Abe::State_65_LedgeAscend_4548E0,
    &Abe::State_66_LedgeDescend_454970,
    &Abe::State_67_LedgeHang_454E20,
    &Abe::State_68_454B80,
    &Abe::State_69_LedgeHangWobble_454EF0,
    &Abe::State_70_RingRopePull_455AF0,
    &Abe::State_71_Knockback_455090,
    &Abe::State_72_KnockbackGetUp_455340,
    &Abe::State_73_PushWall_4553A0,
    &Abe::State_74_455290,
    &Abe::State_75_Jump_Into_Well_45C7B0,
    &Abe::State_76_45CA40,
    &Abe::State_77_45D130,
    &Abe::State_78_WellBegin_45C810,
    &Abe::State_79_WellInside_45CA60,
    &Abe::State_80_WellShotOut_45D150,
    &Abe::jState_81_WellBegin_45C7F0,
    &Abe::State_82_45CC80,
    &Abe::State_83_45CF70,
    &Abe::State_84_FallLandDie_45A420,
    &Abe::jState_85_Fall_455070,
    &Abe::State_86_HandstoneBegin_45BD00,
    &Abe::State_87_HandstoneEnd_45C4F0,
    &Abe::State_88_GrenadeMachineUse_45C510,
    &Abe::State_89_BrewMachineBegin_4584C0,
    &Abe::State_90_BrewMachineEnd_4585B0,
    &Abe::State_91_RingRopePullEnd_4557B0,
    &Abe::State_92_455800,
    &Abe::State_93_FallLedgeBegin_455970,
    &Abe::jState_94_FallLedgeBegin_4559A0,
    &Abe::jState_95_FallLedgeBegin_4559C0,
    &Abe::State_96_4559E0,
    &Abe::jState_97_FallLedgeBegin_455A80,
    &Abe::jState_98_FallLedgeBegin_455AA0,
    &Abe::State_99_LeverUse_455AC0,
    &Abe::State_100_455B60,
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
    &Abe::State_117_4587C0,
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

SfxDefinition sAbeSFXList_555250[41] =
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

int sAbeFrameOffsetTable_554B18[130] =
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

TintEntry sTintTable_Abe_554D20[15] =
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

EXPORT int CC Abe_SFX_2_457A40(char /*a1*/, int /*a2*/, int /*a3*/, BaseAliveGameObject* /*a4*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

// Fart/dust cloud particle spawner
EXPORT int CC sub_426C70(FP /*xpos*/, FP /*ypos*/, FP /*scale*/, __int16 /*count*/, unsigned __int8 /*r*/, unsigned __int8 /*g*/, unsigned __int8 /*b*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT BaseAliveGameObject* __stdcall Make_Throwable_49AF30(FP /*xpos*/, FP /*ypos*/, FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
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

ALIVE_VAR(1, 0x5c1bde, WORD, word_5C1BDE, 0);
ALIVE_VAR(1, 0x5c112c, WORD, word_5C112C, 0);


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

Abe* Abe::ctor_44AD10(int frameTableOffset, int /*a3*/, int /*a4*/, int /*a5*/)
{
    const int kResourceArraySize = 28;

    ctor_408240(kResourceArraySize);

    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC
    
    field_4_typeId = Types::eType_Abe;

    field_6_flags.Set(BaseGameObject::eBit08);
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
    field_158 = -1;
    field_154 = -1;
    field_150_OrbWhirlWind_id = -1;
    field_14C = -1;
    field_148 = -1;
    field_1A8 = -1;
    field_164 = -1;
    field_160 = -1;
    field_15C = -1;
    field_1AE &= ~3u;

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_1AC_flags.Raw().all = 0;
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6);
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit7);

    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit1);


    // Changes Abe's "default" colour depending on the level we are in
    SetTint_425600(&sTintTable_Abe_554D20[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = 0;

    field_118_prev_held = 0;
    field_F6 = 0;
    field_120_state = 0;
    field_168 = 0;
    field_16E = 0;
    field_170 = 0;
    field_174 = 0;
    field_176 = 0;
    field_178_invisible_effect_id = -1;
    field_124_gnFrame = sGnFrame_5C1B84;
    field_FC_pPathTLV = nullptr;
    field_128.field_12_mood = 0;
    field_128.field_18 = -1;
    field_144 = 0;

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

    BaseGameObject* pField_148 = sObjectIds_5C1B70.Find_449CF0(field_148);
    BaseGameObject* pField_14C = sObjectIds_5C1B70.Find_449CF0(field_14C);
    BaseGameObject* pField_150 = sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id);
    BaseGameObject* pField_154 = sObjectIds_5C1B70.Find_449CF0(field_154);
    BaseGameObject* pField_158 = sObjectIds_5C1B70.Find_449CF0(field_158);
    BaseGameObject* pField_15C = sObjectIds_5C1B70.Find_449CF0(field_15C);
    BaseGameObject* pField_160 = sObjectIds_5C1B70.Find_449CF0(field_160);
    BaseGameObject* pField_178 = sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id);

    SND_SEQ_Stop_4CAE60(0xAu);

    if (pField_148)
    {
        pField_148->field_6_flags.Set(BaseGameObject::eDead);
        field_148 = -1;
    }

    if (pField_160)
    {
        field_160 = -1;
    }

    if (pField_15C)
    {
        pField_15C->field_6_flags.Set(BaseGameObject::eDead);
        field_15C = -1;
    }

    if (pField_14C)
    {
        pField_14C->field_6_flags.Set(BaseGameObject::eDead);
        field_14C = -1;
    }

    if (pField_150)
    {
        pField_150->field_6_flags.Set(BaseGameObject::eDead);
        field_150_OrbWhirlWind_id = -1;
    }

    if (pField_154)
    {
        field_154 = -1;
    }

    if (pField_158)
    {
        pField_158->field_6_flags.Set(BaseGameObject::eDead);
        field_158 = -1;
    }

    if (pField_178)
    {
        pField_178->field_6_flags.Set(BaseGameObject::eDead);
        field_178_invisible_effect_id = -1;
    }

    field_164 = -1;

    sActiveHero_5C1B68 = spAbe_554D5C;

    dtor_4080B0();
}

struct Quicksave_Obj_Abe
{
    WORD field_0_id;
    __int16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_c_velx;
    FP field_10_vely;
    WORD field_14_path_number;
    WORD field_16_lvl_number;
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
    WORD word62;
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
    WORD wordC2;
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
ALIVE_ASSERT_SIZEOF(Quicksave_Obj_Abe, 216);

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
    
    // TODO
    if (sActiveHero_5C1B68->field_20_animation.field_92_current_frame ==
        *(unsigned __int16 *)&(*sActiveHero_5C1B68->field_20_animation.field_20_ppBlock)[sActiveHero_5C1B68->field_20_animation.field_18_frame_table_offset + 2] - 1)
    {
        sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    //FrameInfoHeader* pFrameHeader = sActiveHero_5C1B68->field_20_animation.Get_FrameHeader_40B730(-1);

    /* TODO
    sActiveHero_5C1B68->field_20_animation.Load_Pal_40A530(
        sActiveHero_5C1B68->field_20_animation.field_20_ppBlock,
        *(DWORD *)&(*sActiveHero_5C1B68->field_20_animation.field_20_ppBlock)[pFrameHeader->field_0_frame_header_offset]);
    */

    sActiveHero_5C1B68->SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.sCurrentLevelId_5C3030);
    sActiveHero_5C1B68->field_20_animation.field_B_render_mode = 0;
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    sActiveHero_5C1B68->field_10C_health = pSaveState->field_30_health;
    sActiveHero_5C1B68->field_106_current_state = pSaveState->field_34_animation_num;
    sActiveHero_5C1B68->field_108_delayed_state = pSaveState->word36;
    sActiveHero_5C1B68->field_F8 = FP_FromInteger(pSaveState->word38);
    sActiveHero_5C1B68->field_110 = pSaveState->dword3C;
    sActiveHero_5C1B68->field_120_state = static_cast<WORD>(pSaveState->dword50);
    sActiveHero_5C1B68->field_124_gnFrame = pSaveState->dword54;
    sActiveHero_5C1B68->field_128.field_0_gnFrame = pSaveState->dword58;
    sActiveHero_5C1B68->field_128.field_4 = pSaveState->dword5C;
    sActiveHero_5C1B68->field_128.field_12_mood = pSaveState->word60;
    sActiveHero_5C1B68->field_128.field_18 = pSaveState->word62;
    sActiveHero_5C1B68->field_144 = pSaveState->dword64;
    sActiveHero_5C1B68->field_1A2_rock_or_bone_count = pSaveState->field_6c_rock_bone_count;
    sActiveHero_5C1B68->field_168 = pSaveState->dword68;
    sActiveHero_5C1B68->field_16C = pSaveState->byte6E;

    if (sActiveHero_5C1B68->field_168 && sActiveHero_5C1B68->field_16C)
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

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5);
    if (pSaveState->byte6D & 1)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit5);
    }

    sActiveHero_5C1B68->field_16E = pSaveState->byte6F;
    sActiveHero_5C1B68->field_104 = pSaveState->field_3a_collision_line_id;

    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit9);
    sActiveHero_5C1B68->field_118_prev_held = InputObject::Command_To_Raw_45EE40(pSaveState->word70);
    sActiveHero_5C1B68->field_11C = InputObject::Command_To_Raw_45EE40(pSaveState->word72);
    sActiveHero_5C1B68->field_122 = pSaveState->word74;
    sActiveHero_5C1B68->field_128.field_14 = sGnFrame_5C1B84 - pSaveState->dword78;
    sActiveHero_5C1B68->field_148 = pSaveState->dword7C;
    sActiveHero_5C1B68->field_14C = pSaveState->dword80;
    sActiveHero_5C1B68->field_150_OrbWhirlWind_id = pSaveState->dword84;
    sActiveHero_5C1B68->field_154 = pSaveState->dword88;
    sActiveHero_5C1B68->field_158 = pSaveState->dword8C;
    sActiveHero_5C1B68->field_15C = pSaveState->dword90;
    sActiveHero_5C1B68->field_160 = pSaveState->dword94;
    sActiveHero_5C1B68->field_164 = pSaveState->dword98;
    sActiveHero_5C1B68->field_178_invisible_effect_id = -1;
    sActiveHero_5C1B68->field_170 = pSaveState->dword9C;
    sActiveHero_5C1B68->field_174 = pSaveState->wordA0;
    sActiveHero_5C1B68->field_176 = pSaveState->wordA2;
    sActiveHero_5C1B68->field_17C = pSaveState->byteA4;
    sActiveHero_5C1B68->field_180 = pSaveState->dwordA8;
    sActiveHero_5C1B68->field_184 = pSaveState->wordAC;
    sActiveHero_5C1B68->field_186 = pSaveState->wordAE;
    sActiveHero_5C1B68->field_188 = pSaveState->wordB0;
    sActiveHero_5C1B68->field_18A = pSaveState->wordB2;
    sActiveHero_5C1B68->field_18C = pSaveState->wordB4;
    sActiveHero_5C1B68->field_18E = pSaveState->wordB6;
    sActiveHero_5C1B68->field_190 = pSaveState->wordB8;
    sActiveHero_5C1B68->field_192 = pSaveState->wordBA;
    sActiveHero_5C1B68->field_194 = pSaveState->wordBC;
    sActiveHero_5C1B68->field_196 = pSaveState->wordBE;
    sActiveHero_5C1B68->field_198_has_evil_fart = pSaveState->wordC0;
    sActiveHero_5C1B68->field_19A = pSaveState->wordC2;
    sActiveHero_5C1B68->field_19C = pSaveState->wordC4;
    sActiveHero_5C1B68->field_19E = pSaveState->wordC6;
    sActiveHero_5C1B68->field_1A0_door_id = pSaveState->wordC8;
    sActiveHero_5C1B68->field_1A3_throw_direction = pSaveState->field_ca_throw_direction;
    sActiveHero_5C1B68->field_1A4 = pSaveState->wordCC;
    sActiveHero_5C1B68->field_1A8 = pSaveState->dwordD0;
    
    /* TODO: .. only the last one is going to "win" here.. !?
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 1;
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 2;
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 4;
    sActiveHero_5C1B68->field_1AC_flags ^= ((unsigned __int8)sActiveHero_5C1B68->field_1AC_flags ^ LOBYTE(pSaveState->wordD4)) & 8;
    */

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5);
    if (pSaveState->wordD4 & 0x10)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit5);
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

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_Bit9);
    if (pSaveState->wordD4 & 0x80)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit9);
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

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_eBit15);
    if (pSaveState->wordD4 & 0x800)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit15);
    }

    sActiveHero_5C1B68->field_1AC_flags.Clear(Flags_1AC::e1AC_eBit16);
    if (pSaveState->wordD4 & 0xE000)
    {
        sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit16);
    }

    sActiveHero_5C1B68->field_114_flags.Clear(Flags_114::e114_Bit10);
    if ((pSaveState->wordD4 >> 3) & 0x200)
    {
        sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit10);
    }

    sActiveHero_5C1B68->field_1AE &= ~1;
    if ((pSaveState->wordD4 >> 14) & 1)
    {
        sActiveHero_5C1B68->field_1AE |= 1;
    }

    // TODO: Move to shadow object
    sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags &= ~2;
    if ((pSaveState->wordD4 >> 14) & 2)
    {
        sActiveHero_5C1B68->field_E0_176_ptr->field_14_flags |= 2;
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

void Abe::VDestructor(signed int flags)
{
    vdtor_44B350(flags);
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

int Abe::GetSaveState_4DC110(BYTE* pSaveBuffer)
{
    return vGetSaveState_457110(pSaveBuffer);
}

__int16 Abe::Vsub_408730(int arg0)
{
    return vsub_Kill_44BB50(reinterpret_cast<BaseAliveGameObject*>(arg0));
}

__int16 Abe::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    return vOn_TLV_Collision_44B5D0(pTlv);
}

BaseGameObject* Abe::Vsub_408FD0(__int16 a2)
{
    return vsub_44E970(a2);
}

int Abe::Vnull_4081F0()
{
    return reinterpret_cast<int>(vsub_45A570());
}

void Abe::vdtor_44B350(signed int flags)
{
    dtor_44B380();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
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

EXPORT signed int CC PSX_StoreImage_4F5E90(const PSX_RECT* /*rect*/, WORD* /*pData*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Pal_Copy_483560(const PSX_Point* pPoint, __int16 w, WORD* pPalData, PSX_RECT* rect)
{
    rect->x = pPoint->field_0_x;
    rect->y = pPoint->field_2_y;
    rect->w = w;
    rect->h = 1;
    PSX_StoreImage_4F5E90(rect, pPalData);
}

class Class_545A60 : public BaseGameObject
{
public:
    EXPORT Class_545A60* ctor_45F280(Abe* pAbe)
    {
        BaseGameObject_ctor_4DBFA0(TRUE, 0);
        SetVTable(this, 0x545A60);

        field_4_typeId = BaseGameObject::Types::eType_75;

        field_44_objId = pAbe->field_8_object_id;

        field_24_pAlloc = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(pAbe->field_20_animation.field_90_pal_depth * sizeof(WORD)));
        Pal_Copy_483560(
            &pAbe->field_20_animation.field_8C_pal_vram_x,
            pAbe->field_20_animation.field_90_pal_depth,
            field_24_pAlloc,
            &field_28);

        field_30_pPalAlloc = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(pAbe->field_20_animation.field_90_pal_depth * sizeof(WORD)));
        Pal_Copy_483560(
            &pAbe->field_20_animation.field_8C_pal_vram_x,
            pAbe->field_20_animation.field_90_pal_depth,
            field_30_pPalAlloc,
            &field_34);

        field_4A_flags = 0;

        if (pAbe->field_20_animation.field_4_flags.Get(AnimFlags::eBit15_bSemiTrans))
        {
            field_4A_flags |= 0x1;
        }
        if (pAbe->field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
        {
            field_4A_flags |= 0x2;
        }

        field_4A_flags &= ~4u;
        field_48 = pAbe->field_20_animation.field_B_render_mode;
        field_20_state_or_op = 0;

        return this;
    }

    EXPORT void dtor_45F410()
    {
        SetVTable(this, 0x545A60);

        if (field_24_pAlloc)
        {
            Mem_Free_495560(field_24_pAlloc);
        }

        if (field_30_pPalAlloc)
        {
            Mem_Free_495560(field_30_pPalAlloc);
        }

        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT void sub_45FA00()
    {
        field_4A_flags |= 4u;
        field_1C_update_delay = 1;
        field_20_state_or_op = 1;
    }

    EXPORT void sub_45FA30()
    {
        field_20_state_or_op = 4;
    }

    EXPORT void sub_45FA50()
    {
        field_1C_update_delay = 1;
        field_20_state_or_op = 5;
    }

    EXPORT void sub_45F9E0()
    {
        field_1C_update_delay = 1;
        field_20_state_or_op = 1;
    }

    EXPORT void vUpdate_45F4A0()
    {
        NOT_IMPLEMENTED();
    }

    EXPORT void vdtor_45F3E0(signed int flags)
    {
        dtor_45F410();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VDestructor(signed int flags) override
    {
        vdtor_45F3E0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_45F4A0();
    }

    virtual void VScreenChanged() override
    {
        // Null @ 45F9C0
    }

private:
    unsigned __int16 field_20_state_or_op;
    __int16 field_22;
    WORD* field_24_pAlloc;
    PSX_RECT field_28;
    WORD* field_30_pPalAlloc;
    PSX_RECT field_34;
    int field_3C;
    int field_40;
    int field_44_objId;
    char field_48;
    char field_49;
    __int16 field_4A_flags;
};
ALIVE_ASSERT_SIZEOF(Class_545A60, 0x4C);

class MusicTrigger : public BaseGameObject
{
public:
    EXPORT void ctor_47FF10(__int16 type, __int16 a3, int /*a4*/, __int16 delay)
    {
        BaseGameObject_ctor_4DBFA0(TRUE, 0);
        SetVTable(this, 0x5463DC);
        Init_47FFB0(type, a3, delay);
        field_2C = 0;
        field_30 = 0;
        field_2E = 0;
        field_32 = 0;
        field_20 = -1;
    }

    EXPORT void Init_47FFB0(__int16 type, __int16 a3, __int16 delay)
    {
        field_24_flags &= ~7u;
        field_4_typeId = BaseGameObject::Types::eMusicTrigger;
        field_28_counter = 0;

        switch (type)
        {
        case 0:
            field_26 = 3;
            field_28_counter = 400;
            break;
        case 1:
            field_26 = 10;
            field_28_counter = 30;
            break;
        case 2:
            field_26 = 13;
            field_28_counter = 30;
            break;
        case 3:
            field_26 = 8;
            field_24_flags |= 4;
            break;
        case 4:
            field_26 = 7;
            field_24_flags |= 4;
            break;
        case 5:
            field_26 = 2;
            field_28_counter = delay;
            break;
        case 6:
            field_26 = 12;
            field_28_counter = 30;
            break;
        default:
            break;
        }

        if (a3)
        {
            if (a3 == 1)
            {
                field_24_flags |= 1u;
            }
        }
        else
        {
            field_1C_update_delay = delay;
        }
    }

    EXPORT void vdtor_47FEE0(signed int flags)
    {
        dtor_4800C0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    EXPORT void dtor_4800C0()
    {
        SetVTable(this, 0x5463DC);
        if (field_24_flags & 4)
        {
            MusicController::sub_47FD60(0, this, 0, 0);
        }
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT void vScreenChange_4802A0()
    {
        if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    EXPORT void vUpdate_480140()
    {
        if (Event_Get_422C00(kEventHeroDying))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        if (field_24_flags & 1)
        {
            FP xpos = sControlledCharacter_5C1B8C->field_B8_xpos;
            FP ypos = sControlledCharacter_5C1B8C->field_BC_ypos;

            if (xpos >= FP_FromInteger(field_2C) && xpos <= FP_FromInteger(field_30) &&
               (ypos >= FP_FromInteger(field_2E) && ypos <= FP_FromInteger(field_32)))
            {
                field_24_flags &= ~1;
                MusicController::sub_47FD60(field_26, this, (field_24_flags >> 2) & 1,  1);
                field_24_flags |= 2u;
                if (field_28_counter >= 0)
                {
                    field_28_counter += sGnFrame_5C1B84;
                }
            }            
        }
        else if (field_24_flags & 2)
        {
            if (field_28_counter < 0 || static_cast<int>(sGnFrame_5C1B84) < field_28_counter)
            {
                MusicController::sub_47FD60(field_26, this, (field_24_flags >> 2) & 1, 0);
            }
            else
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }
        else
        {
            MusicController::sub_47FD60(field_26, this, (field_24_flags >> 2) & 1, 1);
            field_24_flags |= 2u;
            field_28_counter += sGnFrame_5C1B84;
        }
    }

    virtual void VDestructor(signed int flags) override
    {
        vdtor_47FEE0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_480140();
    }

    virtual void VScreenChanged() override
    {
        vScreenChange_4802A0();
    }

private:
    int field_20;
    __int16 field_24_flags;
    __int16 field_26;
    int field_28_counter;
    __int16 field_2C;
    __int16 field_2E;
    __int16 field_30;
    __int16 field_32;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x34);

class AbilityRing
{
public:
    EXPORT static BaseGameObject * CC Factory_482F80(int /*xpos*/, int /*ypos*/, int /*type*/, FP /*scale*/)
    {
        NOT_IMPLEMENTED();
        return nullptr;
    }
};

// TODO: Figure out what this is and impl
class Class_544FE4 : public BaseGameObject
{
public:
    EXPORT Class_544FE4* ctor_431CB0(FP /*xpos*/, FP /*ypos*/, __int16 /*layer*/, FP /*scale*/, __int16 /*count*/, __int16 /*bUnknown*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    virtual void VDestructor(signed int flags) override
    {
        vdtor_431DE0(flags);
    }

    EXPORT void dtor_431E10()
    {
        NOT_IMPLEMENTED();
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT void vdtor_431DE0(signed int flags)
    {
        dtor_431E10();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

private:
    int field_20_xpos1;
    int field_24_ypos1;
    int field_28_xpos2;
    int field_2C_ypos2;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    __int16 field_40_layer;
    __int16 field_42;
    __int16 field_44;
    __int16 field_46;
    __int16 field_48_count;
    __int16 field_4A;
    int field_4C_prims;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    __int16 field_18C;
    __int16 field_18E_bUnknown;
};
ALIVE_ASSERT_SIZEOF(Class_544FE4, 0x190);

class Class_5480D4; // TODO

class OrbWhirlWind : public BaseGameObject
{
public:
    // TODO: Virtuals.. pretty much everything else..

    EXPORT void sub_4E4050()
    {
        NOT_IMPLEMENTED();
    }

    int field_20;
    __int16 field_24;
    __int16 field_26_bUnknown;
    __int16 field_28_obj_array_idx;
    //__int16 field_2A; // padding ?
    Class_5480D4 *field_2C_objArray[16];
    int field_6C_xpos;
    int field_70_ypos;
    int field_74_scale;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWind, 0x78);

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

        field_110 = BaseGameObject::Find_Flags_4DC170(field_110);
        field_148 = BaseGameObject::Find_Flags_4DC170(field_148);
        field_14C = BaseGameObject::Find_Flags_4DC170(field_14C);
        field_1A8 = BaseGameObject::Find_Flags_4DC170(field_1A8);
        field_150_OrbWhirlWind_id = BaseGameObject::Find_Flags_4DC170(field_150_OrbWhirlWind_id);
        field_154 = BaseGameObject::Find_Flags_4DC170(field_154);
        field_158 = BaseGameObject::Find_Flags_4DC170(field_158);
        field_15C = BaseGameObject::Find_Flags_4DC170(field_15C);
        field_160 = BaseGameObject::Find_Flags_4DC170(field_160);
        field_164 = BaseGameObject::Find_Flags_4DC170(field_164);

        if (field_114_flags.Get(Flags_114::e114_Bit8))
        {
            if (!field_170)
            {
                field_170 = sGnFrame_5C1B84 + 2;
            }

            auto pClass = alive_new<Class_545A60>();
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
    if (totalAliveSavedMuds == 0)
    {
        // "Normal" voice
        field_128.field_12_mood = 5;
    }
    else if (totalAliveSavedMuds >= 0)
    {
        // "Happy" voice
        field_128.field_12_mood = 0;
    }
    else if (totalAliveSavedMuds <= 0)
    {
        // "Sad" voice
        field_128.field_12_mood = 3;
    }

    // Handle DDCheat mode
    if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
    {
        Vnull_4081F0();

        field_F8 = field_BC_ypos;
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5);
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

        if (field_114_flags.Get(Flags_114::e114_Bit9) || field_1AC_flags.Get(Flags_1AC::e1AC_Bit6))
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
            Knockback_44E700(1, 1);
            if (state_idx != -1)
            {
                field_106_current_state = state_idx;
            }

            field_108_delayed_state = 0;
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2);
            field_122 = 0;
            field_114_flags.Clear(Flags_114::e114_Bit1);
            field_114_flags.Set(Flags_114::e114_Bit2);
        }

        if (Event_Get_422C00(kEventScreenShake) && field_10C_health > FP_FromInteger(0))
        {
            if (sub_449D30())
            {
                Knockback_44E700(1, 0);
            }
        }

        if (field_128.field_18 >= 0 && static_cast<int>(sGnFrame_5C1B84) >= field_144)
        {
            if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0)
                || (field_106_current_state == eAbeStates::State_112_Chant_45B1C0)
                || field_106_current_state == eAbeStates::State_7_45B140
                || field_106_current_state == eAbeStates::State_8_45B160
                || field_106_current_state == eAbeStates::State_9_45B180
                || field_106_current_state == eAbeStates::State_10_Fart_45B1A0)
            {
                if (field_106_current_state == eAbeStates::State_0_Idle_44EEB0 || field_106_current_state == eAbeStates::State_12_Null_4569C0)
                {
                    field_114_flags.Set(Flags_114::e114_Bit2);
                    switch (field_128.field_18)
                    {
                    case 14: // Says "oops"
                        field_106_current_state = eAbeStates::State_34_DunnoBegin_44ECF0;
                        break;
                    case 5: // Says "grr"
                        field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
                        break;
                    case 28: // Says "sympathy" (not the sorry sound but the depressed sound)
                        field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
                        break;
                    default:
                        field_106_current_state = eAbeStates::State_9_45B180;
                        break;
                    }
                }

                if (field_128.field_18 == 5)
                {
                    // Other evil muds laugh at the abe grr
                    Event_Broadcast_422BC0(kEventMudokonLaugh, sActiveHero_5C1B68);
                }

                if (field_128.field_18 == 28)
                {
                    // This one has another volume for whatever reason
                    Abe_SFX_457EC0(static_cast<unsigned char>(field_128.field_18), 80, 0, this);
                }
                else
                {
                    Abe_SFX_457EC0(static_cast<unsigned char>(field_128.field_18), 0, 0, this);
                }
            }

            field_128.field_18 = -1;
        }

        if (state_idx != field_106_current_state || field_114_flags.Get(Flags_114::e114_Bit2))
        {
            field_114_flags.Clear(Flags_114::e114_Bit2);
            if (field_106_current_state != eAbeStates::State_12_Null_4569C0 && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5)))
            {
                field_20_animation.Set_Animation_Data_409C80(
                    sAbeFrameOffsetTable_554B18[field_106_current_state],
                    StateToAnimResource_44AAB0(field_106_current_state));

                field_128.field_14 = sGnFrame_5C1B84;

                if (state_idx == eAbeStates::State_12_Null_4569C0 || state_idx == eAbeStates::State_60_4A3200)
                {
                    field_20_animation.SetFrame_409D50(field_F6);
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
            field_20_animation.SetFrame_409D50(field_F6);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2);
        }

        if (field_128.field_4 <= static_cast<int>(sGnFrame_5C1B84) && field_10C_health > FP_FromInteger(0))
        {
            field_10C_health = FP_FromDouble(1.0);
        }

        if (field_168)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    if (static_cast<int>(sGnFrame_5C1B84) <= field_168)
                    {
                        if (!(sGnFrame_5C1B84 % 32))
                        {
                            int ringType = 0;
                            if (field_16C)
                            {
                                ringType = 4;
                            }
                            else if (field_16E)
                            {
                                ringType = 7;
                            }
                            else if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15))
                            {
                                ringType = 14;
                            }

                            PSX_RECT rect = {};
                            GetBoundingRect_424FD0(&rect, 1);
                            AbilityRing::Factory_482F80((rect.x + rect.w) / 2, (rect.y + rect.h) / 2, ringType, field_CC_sprite_scale);

                            SFX_Play_46FBA0(0x11u, 25, 2650, 0x10000);
                        }
                    }
                    else
                    {
                        if (field_168 > 0 && field_16C > 0)
                        {
                            Free_Shrykull_Resources_45AA90();
                        }
                        field_168 = 0;
                    }
                }
            }
        }


        Class_545A60* pObj_field_178 = static_cast<Class_545A60*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
        if (pObj_field_178 && field_170 > 0)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_170)
            {
                field_170 = 0;
                pObj_field_178->sub_45FA30();
            }
        }

        if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit16))
        {
            if (field_1AE & 1)
            {
                if (gMap_5C3030.sCurrentLevelId_5C3030 == 2)
                {
                    field_168 = sGnFrame_5C1B84 + 200000;
                    field_16C = 0;
                    field_16E = 0;
                    field_1AE &= ~1;
                    field_1AC_flags.Clear(Flags_1AC::e1AC_eBit16);
                    field_1AC_flags.Set(Flags_1AC::e1AC_eBit15);
                }
            }
        }

        if (Event_Get_422C00(kEventMudokonDied))
        {
            field_128.field_18 = 14;
            field_144 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);

            // Do the death jingle
            alive_new<MusicTrigger>()->ctor_47FF10(1, 0, 90, 0);
        }

        if (Event_Get_422C00(kEventMudokonComfort))
        {
            field_128.field_18 = 8;
            field_144 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        }

        if (Event_Get_422C00(kEventMudokonComfort | kEventSpeaking))
        {
            field_128.field_18 = 14;
            field_144 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        }

        if (field_1AE & 2)
        {
            field_1AE &= ~2;
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

BaseGameObject* Abe::vsub_45A570()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

// TODO :Needs grenade object stub for correct impl

void Abe::Knockback_44E700(__int16 bUnknownSound, __int16 bDelayedAnger)
{
    NOT_IMPLEMENTED();

    OrbWhirlWind* pfield_150 = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));
    BaseGameObject* pfield_158 = sObjectIds_5C1B70.Find_449CF0(field_158);
    BaseGameObject* pfield_164 = sObjectIds_5C1B70.Find_449CF0(field_164);
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
            // TODO: Resolve virtual
            //(*(void(__thiscall **)(int, signed int))(*(_DWORD *)pfield_164 + 64))(pfield_164, 1);
            field_164 = -1;
        }

        if (field_C4_velx > FP_FromInteger(0))
        {
            field_B8_xpos -= field_C4_velx;
        }

        sub_408D10(TRUE);

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

        field_106_current_state = 71;

        if (bDelayedAnger)
        {
            field_128.field_18 = 5; // anger in..
            field_144 = sGnFrame_5C1B84 + 27; // 27 ticks
        }

        if (pfield_158)
        {
            // Virtual 29 = GrenadeBase__vsub_448080
            // TODO
            /*
            (*((void(__thiscall **)(BaseGameObject *))&pfield_158->field_0_VTbl->VAbe + 29))(pfield_158);
            */
            field_158 = -1;
            if (!word_5C1BDE)
            {
                field_1A2_rock_or_bone_count++;
            }
        }
    }
}

void Abe::vRender_44B580(int** pOrderingTable)
{
    if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5)))
    {
        field_20_animation.field_14_scale = field_CC_sprite_scale;
    }

    if (field_106_current_state != eAbeStates::State_79_WellInside_45CA60 && field_106_current_state != eAbeStates::State_82_45CC80 && field_106_current_state != eAbeStates::State_76_45CA40)
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
        if (gMap_5C3030.field_A_5C303A_levelId == 1 && !word_5C1BA0)
        {
            field_128.field_18 = 3;
            field_144 = sGnFrame_5C1B84 + 35;
        }

        // Set the correct tint for this map
        SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.field_A_5C303A_levelId);

        if (gMap_5C3030.sCurrentLevelId_5C3030)
        {
            if (field_1A2_rock_or_bone_count > 0)
            {
                if (gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C->Remove_49AA00(field_1A2_rock_or_bone_count);
                }
            }
            
            field_1A2_rock_or_bone_count = 0;
            
            if (field_168 > 0 && field_16C)
            {
                Free_Shrykull_Resources_45AA90();
            }

            field_168 = 0;
        }

        if (gMap_5C3030.field_A_5C303A_levelId == 2)
        {
            if (gMap_5C3030.sCurrentLevelId_5C3030 == 7)
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit16);
            }

            if (gMap_5C3030.sCurrentLevelId_5C3030 == 11)
            {
                field_1AE |= 1u;
            }
        }

        if (gMap_5C3030.field_A_5C303A_levelId == 16 || gMap_5C3030.field_A_5C303A_levelId == 0)
        {
            // Remove Abe for menu/credits levels?
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_168 = 0;
        if (gMap_5C3030.sCurrentLevelId_5C3030)
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

    pSaveState->field_0_id = Types::eType_Abe;
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
    
    pSaveState->dword3C = field_110;

    if (field_110 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_110);
        if (pObj)
        {
            pSaveState->dword3C = pObj->field_C_objectId;
        }
    }

    pSaveState->field_44_is_abe_controlled = (this == sControlledCharacter_5C1B8C);
    pSaveState->dword50 = field_120_state;
    pSaveState->dword54 = field_124_gnFrame;
    pSaveState->dword58 = field_128.field_0_gnFrame;
    pSaveState->dword5C = field_128.field_4;
    pSaveState->word60 = field_128.field_12_mood;
    pSaveState->word62 = field_128.field_18;
    pSaveState->dword64 = field_144;
    pSaveState->dword68 = field_168;
    pSaveState->field_6c_rock_bone_count = field_1A2_rock_or_bone_count;
    //pSaveState->byte6D = (LOBYTE(this->field_1AC_flags) >> 4) & 1;
    pSaveState->byte6E = static_cast<char>(field_16C);
    pSaveState->byte6F = static_cast<char>(field_16E);
    //pSaveState->word70 = sub_45EF70(field_118_prev_held);
    //pSaveState->word72 = sub_45EF70(field_11C);
    pSaveState->word74 = field_122;
    pSaveState->dword78 = sGnFrame_5C1B84 - field_128.field_14;
    pSaveState->dword7C = field_148;
    
    if (field_148 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_148);
        if (pObj)
        {
            pSaveState->dword7C = pObj->field_C_objectId;
        }
    }

    pSaveState->dword80 = field_14C;

    if (field_14C != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_14C);
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
    
    pSaveState->dword88 = field_154;

    if (field_154 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_158);
        if (pObj)
        {
            pSaveState->dword88 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword8C = field_158;

    if (field_158 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_158);
        if (pObj)
        {
            pSaveState->dword8C = pObj->field_C_objectId;
        }
    }

    pSaveState->dword90 = field_15C;

    if (field_15C != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_15C);
        if (pObj)
        {
            pSaveState->dword90 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword94 = field_160;

    if (field_160 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_160);
        if (pObj)
        {
            pSaveState->dword94 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword98 = field_164;

    if (field_164 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_164);
        if (pObj)
        {
            pSaveState->dword98 = pObj->field_C_objectId;
        }
    }

    pSaveState->dwordD0 = field_1A8;

    if (field_1A8 != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_1A8);
        if (pObj)
        {
            pSaveState->dwordD0 = pObj->field_C_objectId;
        }
    }

    pSaveState->dword9C = field_170;
    pSaveState->wordA0 = field_174;
    pSaveState->wordA2 = field_176;
    pSaveState->byteA4 = field_17C;
    pSaveState->dwordA8 = field_180;
    pSaveState->wordAC = field_184;
    pSaveState->wordAE = field_186;
    pSaveState->wordB0 = field_188;
    pSaveState->wordB2 = field_18A;
    pSaveState->wordB4 = field_18C;
    pSaveState->wordB6 = field_18E;
    pSaveState->wordB8 = field_190;
    pSaveState->wordBA = field_192;
    pSaveState->wordBC = field_194;
    pSaveState->wordBE = field_196;
    pSaveState->wordC0 = field_198_has_evil_fart;
    pSaveState->wordC2 = field_19A;
    pSaveState->wordC4 = field_19C;
    pSaveState->wordC6 = field_19E;
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

bool Abe::vsub_Kill_44BB50(BaseGameObject * /*otherObj*/)
{
    NOT_IMPLEMENTED();
    return false;
}

__int16 Abe::vOn_TLV_Collision_44B5D0(Path_TLV* /*a2a*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Abe::sub_44B7B0()
{
#ifdef STUPID_FUN
    // THIS IS A HACK TO MAKE ABE POSSESS ANYTHING :D
    for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
    {
        BaseAliveGameObject* pBaseGameObject = reinterpret_cast<BaseAliveGameObject*>(gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx));
        if (!pBaseGameObject)
        {
            break;
        }

        if (pBaseGameObject->field_4_typeId == 83)
        {
            pBaseGameObject->field_6_flags |= 4;
        }

        PSX_Point currentScreenCoords;
        gMap_5C3030.GetCurrentCamCoords_480680(&currentScreenCoords);
        if (pBaseGameObject != sActiveHero_5C1B68 && pBaseGameObject->field_6_flags & BaseGameObject::eIsBaseAliveGameObject &&
            pBaseGameObject->field_B8_xpos.GetExponent() > currentScreenCoords.field_0_x && pBaseGameObject->field_B8_xpos.GetExponent() < currentScreenCoords.field_0_x + 350
            && pBaseGameObject->field_BC_ypos.GetExponent() > currentScreenCoords.field_2_y && pBaseGameObject->field_BC_ypos.GetExponent() < currentScreenCoords.field_2_y + 240)
        {
            return (int)pBaseGameObject;
        }
    }
    return 0;
#else
    NOT_IMPLEMENTED();
#endif
}

void Abe::Load_Basic_Resources_44D460()
{
    NOT_IMPLEMENTED();
}

void Abe::Free_Resources_44D420()
{
    NOT_IMPLEMENTED();
}

EXPORT BOOL Abe::sub_449D30()
{
    NOT_IMPLEMENTED();
    return 0;
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
    else if (state < eAbeStates::State_65_LedgeAscend_4548E0)
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
    else if (state < eAbeStates::State_91_RingRopePullEnd_4557B0)
    {
        mapped = ResourceIndices::eStone_8;
    }
    else if (state < eAbeStates::State_99_LeverUse_455AC0)
    {
        mapped = ResourceIndices::eEdge_13;
    }
    else if (state < eAbeStates::State_100_455B60)
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

void Abe::State_0_Idle_44EEB0()
{
    if (Input_IsChanting_45F260() && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit6)))
    {
        if (field_168 && field_16C)
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
                field_1A8 = pObj->field_8_object_id;
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
        BaseGameObject* pObj_field_110 = sObjectIds_5C1B70.Find_449CF0(field_110);
        if (pObj_field_110)
        {
            if (pObj_field_110->field_4_typeId == Types::eType_78)
            {
                const FP halfGrid = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((field_100_pCollisionLine->field_0_x1 + field_100_pCollisionLine->field_4_x2) / 2);
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
            // Must match our scale
            if (pHoist->field_16_scale == Path_Hoist::Scale::eHalf && field_D6_scale == 1)
            {
                return;
            }

            if (pHoist->field_16_scale == Path_Hoist::Scale::eHalf && field_D6_scale == 0)
            {
                return;
            }

            // Are we facing the same direction as the hoist edge?
            if ((pHoist->field_12_edge_type == Path_Hoist::EdgeType::eLeft || field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
             && (pHoist->field_12_edge_type == Path_Hoist::EdgeType::eRight || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))))
            {
                // Yeah go down
                field_106_current_state = eAbeStates::State_66_LedgeDescend_454970;
            }
            else
            {
                // Otherwise gotta turn around
                field_108_delayed_state = eAbeStates::State_66_LedgeDescend_454970;
                field_106_current_state = eAbeStates::State_2_StandingTurn_451830;
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

            sub_426C70(fartXPos, fartYPos, fartScale, 3, 32u, 128u, 32u);
        }

        field_106_current_state = eAbeStates::State_10_Fart_45B1A0;
        return;
    }

    bool handleDoActionOrThrow = false;
    if (pressed & sInputKey_Up_5550D8)
    {
        // Check for lift rope.. (going up)
        BaseGameObject* pObj_field_110_2 = sObjectIds_5C1B70.Find_449CF0(field_110);
        if (pObj_field_110_2)
        {
            if (pObj_field_110_2->field_4_typeId == Types::eType_78)
            {
                const FP halfGrid = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((field_100_pCollisionLine->field_0_x1 + field_100_pCollisionLine->field_4_x2) / 2);
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

                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3);
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

                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3);
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
                BaseAliveGameObject* pMachineButton = FindObjectOfType_425180(Types::eGrenadeMachine_66, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(25)));
                if (pMachineButton)
                {
                    pMachineButton->Vnull_408F70();
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
                    BaseAliveGameObject* pObj_148 = FindObjectOfType_425180(Types::eType_148, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)));
                    if (pObj_148)
                    {
                        field_164 = pObj_148->field_8_object_id;
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

        if (!sub_4569E0())
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
        else if (field_1A2_rock_or_bone_count > 0 || word_5C1BDE > 0)
        {
            field_158 = Make_Throwable_49AF30(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(40),
                FP_FromInteger(0))->field_8_object_id;

            if (!word_5C112C)
            {
                Class_544FE4* pThrowable = alive_new<Class_544FE4>();
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
            
            if (word_5C1BDE == 0)
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

        sub_408D10(TRUE);

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
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit3);
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
        case eFloor:
        case eBackGroundFloor:
        case 32u: // TODO: These type are never seen, internal only ??
        case 36u:
        {
            field_B8_xpos = hitX;
            field_BC_ypos = FP_NoFractional(hitY + FP_FromDouble(0.5));
            field_100_pCollisionLine = pPathLine;
            sub_408D10(TRUE);
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
                field_128.field_4 = sGnFrame_5C1B84 + 900;
            }

            field_F4 = 3;

            PSX_Point xy{ FP_GetExponent(field_B8_xpos - FP_FromInteger(10)), FP_GetExponent(field_B8_xpos + FP_FromInteger(10)) };
            PSX_Point wh{ FP_GetExponent(field_BC_ypos - FP_FromInteger(10)), FP_GetExponent(field_BC_ypos + FP_FromInteger(10)) };
            vsub_424EE0(
                xy,
                wh,
                ObjList_5C1B78,
                1,
                &BaseAliveGameObject::sub_408BA0);
        }
            break;

        case eWallLeft:
        case eWallRight: 
        case eBackGroundWallLeft:
        case eBackGroundWallRight: 
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            Knockback_44E700(1, 1);
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
        if (!pEdge->field_12_can_grab)
        {
            return;
        }

        // Edge scale must match
        if (pEdge->field_14_scale != Path_Edge::Scale::eFull && field_D6_scale != 0)
        {
            return;
        }
        else if (pEdge->field_14_scale != Path_Edge::Scale::eHalf && field_D6_scale != 1)
        {
            return;
        }

        if (pEdge->field_10_type == Path_Edge::Type::eLeft)
        {
            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                // Left but we are facing right
                return;
            }
        }
        else if (pEdge->field_10_type == Path_Edge::Type::eRight)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                // Right but we are facing left
                return;
            }
        }
        else if (pEdge->field_10_type != Path_Edge::Type::eBoth)
        {
            // Some unknown edge type
            return;
        }

        tryToHang = true;
    }

    // Didn't find and edge to grab so check if falling onto a hoist
    if (!tryToHang)
    {
        // Look down 20 for a hoist
        FP minus20 = field_CC_sprite_scale * FP_FromInteger(20);
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - minus20),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - minus20),
            Path_Hoist::kType));

        if (pHoist)
        {
            // Must match our scale
            if ((pHoist->field_12_edge_type == Path_Hoist::EdgeType::eRight && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
             || (pHoist->field_12_edge_type == Path_Hoist::EdgeType::eLeft && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))))
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

        sub_408D10(TRUE);

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
            (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y + 0xFFFF * field_FC_pPathTLV->field_8_top_left.field_2_y)) >= (field_CC_sprite_scale * FP_FromInteger(70)))
        {
            field_106_current_state = eAbeStates::State_69_LedgeHangWobble_454EF0;
            field_E0_176_ptr->field_14_flags |= 1u;
        }
        else
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
            field_F4 = 65;
            field_F6 = 12;
            field_E0_176_ptr->field_14_flags |= 1u;
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
            sub_408D10(TRUE);

            if (field_108_delayed_state == eAbeStates::State_28_HopMid_451C50)
            {
                field_108_delayed_state = eAbeStates::State_0_Idle_44EEB0;
                field_106_current_state = eAbeStates::State_27_HopBegin_4521C0;
                BaseGameObject* pObj = Vsub_408FD0(2);
                if (pObj)
                {
                    field_1A4 = 0;
                    field_1A8 = pObj->field_8_object_id;
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

void Abe::State_7_45B140()
{
    // TODO: Note jState_11_Speak_40388C omitted
    State_11_Speak_45B0A0();
}

void Abe::State_8_45B160()
{
    // TODO: Note jState_11_Speak_40388C omitted
    State_11_Speak_45B0A0();
}

void Abe::State_9_45B180()
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
    NOT_IMPLEMENTED();
}

void Abe::State_14_HoistIdle_452440()
{
    NOT_IMPLEMENTED();
}

void Abe::State_15_HoistLand_452BA0()
{
    NOT_IMPLEMENTED();
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
        if (field_F4 == 3)
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
        field_106_current_state = eAbeStates::jState_98_FallLedgeBegin_455AA0;
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
            sub_454090(gridSize + field_B8_xpos, FP_GetExponent(field_BC_ypos - FP_FromInteger(5)), 0);
        }
    }

    const DWORD held = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    
    // Crouching throw stuff
    if (sInputKey_ThrowItem_5550F4 & held
        && field_106_current_state == eAbeStates::State_17_CrouchIdle_456BC0
        && (field_1A2_rock_or_bone_count > 0 || word_5C1BDE))
    {
        field_158 = Make_Throwable_49AF30(field_B8_xpos, field_BC_ypos - FP_FromInteger(40), FP_FromInteger(0))->field_8_object_id;
        if (!word_5C112C)
        {
            auto pRockCountGraphic = alive_new<Class_544FE4>();
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
        
        if (!word_5C1BDE)
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
                sub_426C70(xpos, ypos, scale, 3, 32u, 128u, 32u);
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
        sub_454090(field_B8_xpos, FP_GetExponent(field_BC_ypos - FP_FromInteger(5)), 1);
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
        Knockback_44E700(1, 1);
        field_106_current_state = eAbeStates::State_74_455290;
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
        Knockback_44E700(1, 1);
        field_106_current_state = eAbeStates::State_74_455290;
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
                sub_408D10(TRUE);

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
    NOT_IMPLEMENTED();
}

void Abe::State_25_RunSlideStop_451330()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        Raycast_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        Knockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));

        if (field_106_current_state == eAbeStates::State_25_RunSlideStop_451330 && !sub_451060() && !TryEnterDoor_451220())
        {
            if (field_20_animation.field_92_current_frame != 15)
            {
                sub_408D10(FALSE);
            }

            const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (field_20_animation.field_92_current_frame >= 9)
            {
                if (field_20_animation.field_92_current_frame == 15)
                {
                    Abe_SFX_2_457A40(0, 0, 32767, this);
                    sub_408D10(TRUE);

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
                field_F6 = field_20_animation.field_92_current_frame;
            }
        }
    }
}

void Abe::State_26_RunTurn_451500()
{
    NOT_IMPLEMENTED();
}

void Abe::State_27_HopBegin_4521C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_28_HopMid_451C50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_29_HopLand_4523D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_30_RunJumpBegin_4532E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_31_RunJumpMid_452C10()
{
    NOT_IMPLEMENTED();
}

void Abe::State_32_RunJumpLand_453460()
{
    NOT_IMPLEMENTED();
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
        Knockback_44E700(1, 1);
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
            
            sub_408D10(TRUE);

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
                    field_1A8 = pObj->field_8_object_id;
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
                field_106_current_state = eAbeStates::jState_38_RollBegin_453A70;
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
                    Knockback_44E700(1, 1);
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
        sub_408D10(TRUE);

        if (field_118_prev_held & sInputKey_Hop_5550E0)
        {
            BaseGameObject* pObj = Vsub_408FD0(3);
            if (pObj)
            {
                field_1A4 = 0;
                field_1A8 = pObj->field_8_object_id;
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

void Abe::jState_38_RollBegin_453A70()
{
    NOT_IMPLEMENTED();
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
        sub_408D10(TRUE);
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
                sub_408D10(TRUE);
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
        sub_408D10(TRUE);
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
        sub_408D10(TRUE);
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
        field_F4 = 40;
        field_F6 = 10;
    }
}

void Abe::State_44_450500()
{
    State_42_4503D0();

    if (field_106_current_state == eAbeStates::State_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2);
        field_106_current_state = eAbeStates::State_44_450500;
        field_F4 = 1;
        field_F6 = 9;
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
        sub_408D10(TRUE);
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
        sub_408D10(TRUE);
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
        sub_408D10(TRUE);
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
        field_F4 = 33;
        field_F6 = 8;
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
        field_F4 = 1;
        field_F6 = 9;
    }
}

void Abe::State_52_451710()
{
    NOT_IMPLEMENTED();
}

void Abe::State_53_451800()
{
    NOT_IMPLEMENTED();
}

void Abe::State_54_RunJumpLandRun_4538F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_55_RunJumpLandWalk_453970()
{
    NOT_IMPLEMENTED();
}

void Abe::State_56_4591F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_57_Dead_4589A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_58_DeadPre_4593E0()
{
    NOT_IMPLEMENTED();
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
        Knockback_44E700(1, 1);
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

void Abe::State_65_LedgeAscend_4548E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_66_LedgeDescend_454970()
{
    NOT_IMPLEMENTED();
}

void Abe::State_67_LedgeHang_454E20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_68_454B80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_69_LedgeHangWobble_454EF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_70_RingRopePull_455AF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_71_Knockback_455090()
{
    NOT_IMPLEMENTED();
}

void Abe::State_72_KnockbackGetUp_455340()
{
    NOT_IMPLEMENTED();
}

void Abe::State_73_PushWall_4553A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_74_455290()
{
    NOT_IMPLEMENTED();
}

void Abe::State_75_Jump_Into_Well_45C7B0()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void Abe::State_79_WellInside_45CA60()
{
    NOT_IMPLEMENTED();
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

        field_E0_176_ptr->field_14_flags |= 2u;
    }

    if (field_106_current_state == eAbeStates::State_84_FallLandDie_45A420)
    {
        field_10C_health = FP_FromInteger(1);
        field_106_current_state = eAbeStates::State_16_LandSoft_45A360;
    }
}

void Abe::jState_81_WellBegin_45C7F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_82_45CC80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_83_45CF70()
{
    NOT_IMPLEMENTED();
}

void Abe::State_84_FallLandDie_45A420()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_85_Fall_455070()
{
    NOT_IMPLEMENTED();
}

void Abe::State_86_HandstoneBegin_45BD00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_87_HandstoneEnd_45C4F0()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void Abe::State_91_RingRopePullEnd_4557B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_92_455800()
{
    NOT_IMPLEMENTED();
}

void Abe::State_93_FallLedgeBegin_455970()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_94_FallLedgeBegin_4559A0()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_95_FallLedgeBegin_4559C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_96_4559E0()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_97_FallLedgeBegin_455A80()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_98_FallLedgeBegin_455AA0()
{
    NOT_IMPLEMENTED();
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

void Abe::State_100_455B60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_101_KnockForward_455420()
{
    NOT_IMPLEMENTED();
}

void Abe::State_102_455310()
{
    NOT_IMPLEMENTED();
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

void Abe::State_112_Chant_45B1C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_113_ChantEnd_45BBE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_114_DoorEnter_459470()
{
    NOT_IMPLEMENTED();
}

void Abe::State_115_DoorExit_459A40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_116_MineCarEnter_458780()
{
    NOT_IMPLEMENTED();
}

void Abe::State_117_4587C0()
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
    NOT_IMPLEMENTED();
}

void Abe::State_122_LiftGrabEnd_45A670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_123_LiftGrabIdle_45A6A0()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void Abe::State_127_TurnWheelLoop_456750()
{
    NOT_IMPLEMENTED();
}

void Abe::State_128_TurnWheelEnd_4569A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_129_PoisonGasDeath_4565C0()
{
    NOT_IMPLEMENTED();
}

void Abe::ToDie_4588D0()
{
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5);
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
    sub_408D10(TRUE);
}

__int16 Abe::sub_454090(FP /*fpX*/, int /*fpY*/, int /*a4*/)
{
    NOT_IMPLEMENTED();
    return 0;
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
    NOT_IMPLEMENTED();
}

void CC Abe::Create_Fart_421D20()
{
    NOT_IMPLEMENTED();
}

__int16 Abe::sub_4569E0()
{
    NOT_IMPLEMENTED();
}

int Abe::sub_44EE10()
{
    // Looks for a door object and checks something
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Abe::HandleDoAction_455BD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Abe::PushWall_44E890()
{
    field_C4_velx = FP_FromInteger(0);
    field_106_current_state = eAbeStates::State_73_PushWall_4553A0;
    Abe_SFX_2_457A40(9, 0, 32767, this);
}

void Abe::sub_44E9A0()
{
    NOT_IMPLEMENTED();
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

void Abe::MoveWithVelocity_450FA0(FP /*velocityX*/)
{
    NOT_IMPLEMENTED();
}

__int16 Abe::sub_451060()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Abe::TryEnterDoor_451220()
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
    sub_408D10(TRUE);
    return 1;
}

void Abe::ToDieFinal_458910()
{
    Class_545A60* pObj = static_cast<Class_545A60*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
    if (pObj)
    {
        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
        {
            pObj->sub_45FA50();
            field_178_invisible_effect_id = -1;
        }
    }

    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5);
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
        nextState = eAbeStates::State_7_45B140;
    }
    else if (input & sInputKey_GameSpeak3_555100)
    {
        Abe_SFX_457EC0(6u, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(12);
        nextState = eAbeStates::State_8_45B160;
    }
    else if (input & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(9);
        if (field_128.field_12_mood == 5 || field_128.field_12_mood == 6)
        {
            Abe_SFX_457EC0(19u, 0, 0, this);
            nextState = eAbeStates::State_9_45B180;
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
            nextState = eAbeStates::State_9_45B180;
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
        nextState = eAbeStates::State_9_45B180;
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
            BaseAliveGameObject* pObj = FindObjectOfType_425180(Types::eLockedSoul, field_B8_xpos + gridSize, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale));
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
                nextState = eAbeStates::State_8_45B160;
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

// TODO: Clean up
EXPORT void CC Abe_SFX_457EC0(unsigned __int8 idx, __int16 volume, int pitch, Abe* pHero)
{
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
        if (pHero == sActiveHero_5C1B68 && gMap_5C3030.sCurrentLevelId_5C3030 == 10)
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
                ((unsigned int)((unsigned __int64)(2863311532i64 * (signed __int16)volume) >> 32) >> 31)
                + ((unsigned __int64)(2863311532i64 * (signed __int16)volume) >> 32),
                2 * (signed __int16)volume / 9,
                static_cast<short>(pitch), static_cast<short>(pitch));
            break;
        case 4:
            // TODO: Deoptimise math
            SFX_SfxDefinition_Play_4CA700(
                &sAbeSFXList_555250[idx],
                ((unsigned int)((unsigned __int64)(1908874354i64 * (signed __int16)volume) >> 32) >> 31)
                + ((signed int)((unsigned __int64)(1908874354i64 * (signed __int16)volume) >> 32) >> 1),
                2 * (signed __int16)volume / 3,
                static_cast<short>(pitch), static_cast<short>(pitch));
            break;
        default:
            break;
        }
    }
}

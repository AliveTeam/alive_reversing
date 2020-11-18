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
#include "Switch.hpp"
#include "Throwable.hpp"
#include "LiftPoint.hpp"
#include "PullRingRope.hpp"
#include "FartMachine.hpp"
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

using TAbeStateFunction = decltype(&Abe::State_0_Idle_44EEB0);

#define MAKE_STRINGS(VAR) #VAR,
const char* const sAbeStateNames[] =
{
    ABE_STATES_ENUM(MAKE_STRINGS)
};

const TAbeStateFunction sAbeStateMachineTable_554910[] =
{
    &Abe::State_0_Idle_44EEB0,
    &Abe::State_1_WalkLoop_44FBA0,
    &Abe::State_2_StandingTurn_451830,
    &Abe::State_3_Fall_459B60,
    &Abe::State_4_WalkToIdle_44FFC0,
    &Abe::State_5_MidWalkToIdle_450080,
    &Abe::State_6_WalkBegin_44FEE0,
    &Abe::State_7_Speak_45B140,
    &Abe::State_8_Speak_45B160,
    &Abe::State_9_Speak_45B180,
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
    &Abe::State_20_CrouchSpeak_454550,
    &Abe::jState_21_ToCrouchSpeak_4545E0,
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
    &Abe::State_41_WalkToSneak_450250,
    &Abe::State_42_SneakToWalk_4503D0,
    &Abe::State_43_MidWalkToSneak_450380,
    &Abe::State_44_MidSneakToWalk_450500,
    &Abe::State_45_SneakBegin_4507A0,
    &Abe::State_46_SneakToIdle_450870,
    &Abe::jState_47_MidSneakToIdle_4508C0,
    &Abe::State_48_WalkToRun_4500A0,
    &Abe::State_49_MidWalkToRun_450200,
    &Abe::State_50_RunToWalk_450E20,
    &Abe::State_51_MidRunToWalk_450F50,
    &Abe::State_52_RunTurnToRun_451710,
    &Abe::State_53_RunTurnToWalk_451800,
    &Abe::State_54_RunJumpLandRun_4538F0,
    &Abe::State_55_RunJumpLandWalk_453970,
    &Abe::State_56_DeathDropFall_4591F0,
    &Abe::State_57_Dead_4589A0,
    &Abe::State_58_DeadPre_4593E0,
    &Abe::State_59_Null_459450,
    &Abe::State_60_4A3200,
    &Abe::State_61_TurnToRun_456530,
    &Abe::State_62_Punch_454750,
    &Abe::State_63_Sorry_454670,
    &Abe::State_64_AfterSorry_454730,
    &Abe::State_65_LedgeAscend_4548E0,
    &Abe::State_66_LedgeDescend_454970,
    &Abe::State_67_LedgeHang_454E20,
    &Abe::State_68_ToOffScreenHoist_454B80,
    &Abe::State_69_LedgeHangWobble_454EF0,
    &Abe::State_70_RingRopePullHang_455AF0,
    &Abe::State_71_Knockback_455090,
    &Abe::State_72_KnockbackGetUp_455340,
    &Abe::State_73_PushWall_4553A0,
    &Abe::State_74_RollingKnockback_455290,
    &Abe::State_75_JumpIntoWell_45C7B0,
    &Abe::State_76_ToInsideOfAWellLocal_45CA40,
    &Abe::State_77_ToWellShotOut_45D130,
    &Abe::State_78_WellBegin_45C810,
    &Abe::State_79_InsideWellLocal_45CA60,
    &Abe::State_80_WellShotOut_45D150,
    &Abe::jState_81_WellBegin_45C7F0,
    &Abe::State_82_InsideWellExpress_45CC80,
    &Abe::State_83_WellExpressShotOut_45CF70,
    &Abe::State_84_FallLandDie_45A420,
    &Abe::jState_85_Fall_455070,
    &Abe::State_86_HandstoneBegin_45BD00,
    &Abe::State_87_HandstoneEnd_45C4F0,
    &Abe::State_88_GrenadeMachineUse_45C510,
    &Abe::State_89_BrewMachineBegin_4584C0,
    &Abe::State_90_BrewMachineEnd_4585B0,
    &Abe::State_91_FallingFromGrab_4557B0,
    &Abe::State_92_ForceDownFromHoist_455800,
    &Abe::State_93_WalkOffEdge_455970,
    &Abe::State_94_RunOffEdge_4559A0,
    &Abe::State_95_SneakOffEdge_4559C0,
    &Abe::State_96_HopToFall_4559E0,
    &Abe::State_97_RunJumpToFall_455A80,
    &Abe::State_98_RollOffEdge_455AA0,
    &Abe::State_99_LeverUse_455AC0,
    &Abe::State_100_SlapBomb_455B60,
    &Abe::State_101_KnockForward_455420,
    &Abe::State_102_RollingKnockForward_455310,
    &Abe::jState_103_KnockForwardGetUp_455380,
    &Abe::State_104_RockThrowStandingHold_455DF0,
    &Abe::State_105_RockThrowStandingThrow_456460,
    &Abe::State_106_RockThrowStandingEnd_455F20,
    &Abe::State_107_RockThrowCrouchingHold_454410,
    &Abe::State_108_RockThrowCrouchingThrow_454500,
    &Abe::State_109_ZShotRolling_455550,
    &Abe::State_110_ZShot_455670,
    &Abe::State_111_PickupItem_4564A0,
    &Abe::State_112_Chant_45B1C0,
    &Abe::State_113_ChantEnd_45BBE0,
    &Abe::State_114_DoorEnter_459470,
    &Abe::State_115_DoorExit_459A40,
    &Abe::State_116_MineCarEnter_458780,
    &Abe::State_117_InMineCar_4587C0,
    &Abe::State_118_MineCarExit_458890,
    &Abe::State_119_ToShrykull_45A990,
    &Abe::State_120_EndShrykull_45AB00,
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

const AnimId sAbeFrameTables[130] =
{
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Walk,
    AnimId::Mudokon_Turn_Around,
    AnimId::Mudokon_Idle,
    AnimId::Mudokon_Unknown_B,
    AnimId::Mudokon_Unknown_C,
    AnimId::Mudokon_Unknown_A,
    AnimId::Mudokon_Talk_A,
    AnimId::Mudokon_Talk_C,
    AnimId::Mudokon_Talk_B,
    AnimId::Mudokon_Talk_D,
    AnimId::Abe_Unknown_A,
    AnimId::Abe_Unknown_B,
    AnimId::Mudokon_Hoist_Start,
    AnimId::Mudokon_Hoist,
    AnimId::Mudokon_Hoist_End,
    AnimId::Mudokon_Hoist_End,
    AnimId::Mudokon_Crouch,
    AnimId::Mudokon_Crouch_To_Stand,
    AnimId::Mudokon_Stand_To_Crouch,
    AnimId::Abe_Crouch_Talk_A,
    AnimId::Abe_Crouch_Talk_B,
    AnimId::Abe_Roll_Start,
    AnimId::Abe_Roll,
    AnimId::Abe_Roll_End,
    AnimId::Mudokon_Run_End_A,
    AnimId::Mudokon_Run_End_B,
    AnimId::Abe_Hop_Start,
    AnimId::Abe_Hop,
    AnimId::Abe_Hop_End,
    AnimId::Mudokon_Run_Jump_Start,
    AnimId::Mudokon_Run_Jump,
    AnimId::Abe_Unknown_C,
    AnimId::Mudokon_Run,
    AnimId::Mudokon_Shrug_Start,
    AnimId::Abe_Shrug_End,
    AnimId::Mudokon_Shrug_Start,
    AnimId::Mudokon_Crouch_Turn_Around,
    AnimId::Abe_Crouch_Start,
    AnimId::Mudokon_Unknown_R,
    AnimId::Mudokon_Sneak,
    AnimId::Mudokon_Sneak_Start,
    AnimId::Mudokon_Unknown_L,
    AnimId::Mudokon_Unknown_M,
    AnimId::Mudokon_Unknown_N,
    AnimId::Mudokon_Unknown_O,
    AnimId::Mudokon_Unknown_P,
    AnimId::Mudokon_Unknown_Q,
    AnimId::Mudokon_Unknown_G,
    AnimId::Mudokon_Unknown_H,
    AnimId::Mudokon_Unknown_I,
    AnimId::Mudokon_Unknown_J,
    AnimId::Mudokon_Unknown_K,
    AnimId::Abe_Unknown_D,
    AnimId::Abe_Unknown_E,
    AnimId::Abe_Unknown_F,
    AnimId::Mudokon_Crouch,
    AnimId::Mudokon_Crouch,
    AnimId::Mudokon_Crouch,
    AnimId::Mudokon_Crouch,
    AnimId::Mudokon_Crouch,
    AnimId::Abe_Unknown_G,
    AnimId::Mudokon_Slap,
    AnimId::Abe_Sorry_Start,
    AnimId::Abe_Sorry_End,
    AnimId::Abe_Hoist_Up,
    AnimId::Abe_Hoist_Down,
    AnimId::Abe_Hoist,
    AnimId::Abe_Hoist,
    AnimId::Abe_Hoist_Swing,
    AnimId::Abe_Hoist_High,
    AnimId::Mudokon_Knocked_Back_Face_Up,
    AnimId::Mudokon_Get_Up,
    AnimId::Abe_Push,
    AnimId::Abe_Knock_Back,
    AnimId::Abe_Well_Enter_Start,
    AnimId::Abe_Well,
    AnimId::Abe_Well,
    AnimId::Abe_Well_Enter,
    AnimId::Abe_Well,
    AnimId::Abe_Well,
    AnimId::Abe_Well_Enter,
    AnimId::Abe_Well,
    AnimId::Abe_Well,
    AnimId::Abe_Death_Fall,
    AnimId::Abe_Fall_A,
    AnimId::Abe_Handstone_Start,
    AnimId::Abe_Handstone_End,
    AnimId::Abe_Handstone_Short,
    AnimId::Abe_Handstone_Start,
    AnimId::Abe_Handstone_End,
    AnimId::Mudokon_Unknown_S,
    AnimId::Mudokon_Unknown_S,
    AnimId::Mudokon_Fall,
    AnimId::Abe_Fall_B,
    AnimId::Abe_Fall_C,
    AnimId::Abe_Unknown_H,
    AnimId::Abe_Unknown_I,
    AnimId::Abe_Unknown_J,
    AnimId::Mudokon_Lever_Pull,
    AnimId::Abe_Slap_Bomb,
    AnimId::Mudokon_Knocked_Back_Face_Down,
    AnimId::Abe_Knock_Forward,
    AnimId::Mudokon_Idle,
    AnimId::Abe_Rock,
    AnimId::Abe_Rock_Throw,
    AnimId::Abe_Rock_Cancel,
    AnimId::Abe_Rock_Crouch,
    AnimId::Abe_Rock_Crouch_Throw,
    AnimId::Abe_Death_Z_A,
    AnimId::Abe_Death_Z_B,
    AnimId::Abe_Item_Grab,
    AnimId::Mudokon_Chant,
    AnimId::Mudokon_Chant_End,
    AnimId::Abe_Door_Enter,
    AnimId::Abe_Door_Exit,
    AnimId::Abe_Mine_Car_Enter,
    AnimId::Abe_Mine_Car_Enter,
    AnimId::Abe_Mine_Car_Exit,
    AnimId::Abe_Shrykull_Start,
    AnimId::Abe_Shrykull_End,
    AnimId::Abe_Lift_Start,
    AnimId::Abe_Lift_End,
    AnimId::Abe_Lift,
    AnimId::Abe_Lift_Up,
    AnimId::Abe_Lift_Down,
    AnimId::Mudokon_Wheel_Start,
    AnimId::Mudokon_Wheel,
    AnimId::Mudokon_Wheel_End,
    AnimId::Abe_Death_Gas
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



EXPORT int CC Environment_SFX_457A40(EnvironmentSfx sfxId, int volume, int pitchMin, BaseAliveGameObject* pAliveObj)
{
    int sndVolume;
    short sndIndex = 0;

    switch (sfxId)
    {
    case EnvironmentSfx::eSlideStop_0:
        sndIndex = 11;
        sndVolume = 40;
        break;

    case EnvironmentSfx::eWalkingFootstep_1:
        sndVolume = volume;
        sndIndex = Math_RandomRange_496AB0(10, 12);
        if (volume)
        {
            break;
        }
        sndVolume = Math_RandomRange_496AB0(54, 58);
        if (pAliveObj)
        {
            if (pAliveObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
            {
                sndVolume *= 3;
            }
        }
        break;

    case EnvironmentSfx::eRunningFootstep_2:
        sndVolume = volume;
        sndIndex = Math_RandomRange_496AB0(13, 15);
        if (volume)
        {
            break;
        }

        sndVolume = Math_RandomRange_496AB0(66, 70);

        if (pAliveObj)
        {
            if (pAliveObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
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
        if (pAliveObj && pAliveObj->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            return SFX_Play_46FA90(SoundEffect::AbeGenericMovement_32, 20);
        }
        else
        {
            return SFX_Play_46FA90(SoundEffect::AbeGenericMovement_32, 35);
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
        if (gMap_5C3030.field_0_current_level == LevelIds::eMines_1
            || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8
            || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5
            || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6
            || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9)
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
        return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(sndVolume), static_cast<short>(pitchMin), 0x7FFF);
    }

    if (pAliveObj->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        sndVolume = 2 * sndVolume / 3;
    }

    if (pAliveObj != sActiveHero_5C1B68)
    {
        switch (gMap_5C3030.GetDirection_4811A0(
            pAliveObj->field_C2_lvl_number,
            pAliveObj->field_C0_path_number,
            pAliveObj->field_B8_xpos,
            pAliveObj->field_BC_ypos))
        {
        case CameraPos::eCamCurrent_0:
            return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(sndVolume), static_cast<short>(pitchMin), 0x7FFF);
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(2 * sndVolume / 3), static_cast<short>(pitchMin), 0x7FFF);
        case CameraPos::eCamLeft_3:
            return SFX_SfxDefinition_Play_4CA700(
                &sSFXList_555160[sndIndex],
                static_cast<short>(2 * sndVolume / 9),
                static_cast<short>(2 * sndVolume / 9),
                static_cast<short>(pitchMin),
                0x7FFF);
            break;
        case CameraPos::eCamRight_4:
            return SFX_SfxDefinition_Play_4CA700(
                &sSFXList_555160[sndIndex],
                static_cast<short>(2 * sndVolume / 3),
                static_cast<short>(2 * sndVolume / 3),
                static_cast<short>(pitchMin),
                0x7FFF);
            break;
        default:
            return 0;
        }
    }
    else
    {
        return SFX_SfxDefinition_Play_4CA420(&sSFXList_555160[sndIndex], static_cast<short>(sndVolume), static_cast<short>(pitchMin), 0x7FFF);
    }
}

const FP_Point sThrowVelocities_555118[9] =
{
    { FP_FromInteger(3),    FP_FromInteger(-14) },
    { FP_FromInteger(10),   FP_FromInteger(-10) },
    { FP_FromInteger(15),   FP_FromInteger(-8) },
    { FP_FromInteger(10),   FP_FromInteger(3) },
    { FP_FromInteger(10),   FP_FromInteger(-4) },
    { FP_FromInteger(4),    FP_FromInteger(-3) },
    { FP_FromInteger(0),    FP_FromInteger(0) },
    { FP_FromInteger(0),    FP_FromInteger(0) },
    { FP_FromInteger(0),    FP_FromInteger(0) }
};

int CC Animation_OnFrame_Abe_455F80(void* pPtr, signed __int16* pData)
{
    auto pAbe = static_cast<Abe*>(pPtr);
    auto pFramePos = reinterpret_cast<PSX_Point*>(pData);

    auto pThrowable = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(sActiveHero_5C1B68->field_158_throwable_id));

    auto tableX = sThrowVelocities_555118[pAbe->field_1A3_throw_direction].field_0_x * pAbe->field_CC_sprite_scale;
    const auto tableY = sThrowVelocities_555118[pAbe->field_1A3_throw_direction].field_4_y * pAbe->field_CC_sprite_scale;

    FP xOff = {};
    if (sActiveHero_5C1B68->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        tableX = -tableX;
        xOff = -(pAbe->field_CC_sprite_scale * FP_FromInteger(pFramePos->field_0_x));
    }
    else
    {
        xOff = pAbe->field_CC_sprite_scale * FP_FromInteger(pFramePos->field_0_x);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        pAbe->field_B8_xpos,
        pAbe->field_BC_ypos + FP_FromInteger(pFramePos->field_2_y),
        xOff + pAbe->field_B8_xpos,
        pAbe->field_BC_ypos + FP_FromInteger(pFramePos->field_2_y),
        &pLine,
        &hitX,
        &hitY,
        pAbe->field_D6_scale != 0 ? 6 : 0x60))
    {
        xOff = hitX - pAbe->field_B8_xpos;
        tableX = -tableX;
    }

    if (pThrowable)
    {
        pThrowable->field_B8_xpos = xOff + sActiveHero_5C1B68->field_B8_xpos;
        pThrowable->field_BC_ypos = (pAbe->field_CC_sprite_scale * FP_FromInteger(pFramePos->field_2_y)) + sActiveHero_5C1B68->field_BC_ypos;
        pThrowable->VThrow_49E460(tableX, tableY);
        pThrowable->field_CC_sprite_scale = pAbe->field_CC_sprite_scale;
        pThrowable->field_D6_scale = pAbe->field_D6_scale;
        sActiveHero_5C1B68->field_158_throwable_id = -1;
    }

    return 1;
}

enum AbeResources
{
    eAbeBSic = 0,
    eAbeBSic1 = 1,
};

EXPORT int CC XGrid_Index_To_XPos_4498F0(FP scale, int xGridIndex)
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

Abe* Abe::ctor_44AD10(int /*frameTableOffset*/, int /*r*/, int /*g*/, int /*b*/)
{
    const int kResourceArraySize = 28;

    ctor_408240(kResourceArraySize);

    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC

    field_4_typeId = Types::eAbe_69;

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    field_C_objectId = -65536;

    Init_GameStates_43BF40();

    // Zero out the resource array
    for (int i = 0; i < kResourceArraySize; i++)
    {
        field_10_resources_array.SetAt(i, nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC1.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEEDGE.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNFD.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEOMM.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeommResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNBK.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, TRUE, FALSE);
    }

    ResourceManager::LoadResourceFile_49C170("ABENOELM.BND", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kAbefallResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kAbesmashResID);

    ResourceManager::LoadResourceFile_49C170("OMMFLARE.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kOmmflareResID);

    ResourceManager::LoadResourceFile_49C170("SQBSMK.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kSquibSmokeResID);

    ResourceManager::LoadResourceFile_49C170("DUST.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kDustResID);

    ResourceManager::LoadResourceFile_49C170("BLOODROP.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kBloodropResID);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kRockShadowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kRockShadowResID);

    ResourceManager::LoadResourceFile_49C170("DEADFLR.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kDeathFlareResID);

    ResourceManager::LoadResourceFile_49C170("DOVBASIC.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kDovbasicResID);

    ResourceManager::LoadResourceFile_49C170("SPOTLITE.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID);

    field_128.field_10_resource_index = 1;

    const AnimRecord& rec = AnimRec(AnimId::Mudokon_Idle);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_1C_fn_ptr_array = kAbe_Anim_Frame_Fns_55EF98;

    PSX_Point point = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&point);

    field_B8_xpos = FP_FromInteger(point.field_0_x + XGrid_Index_To_XPos_4498F0(field_CC_sprite_scale, 4));
    field_BC_ypos = FP_FromInteger(point.field_2_y + 120);

    field_F8_LastLineYPos = field_BC_ypos;
    field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
    field_128.field_C_unused = 0;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_100_pCollisionLine = nullptr;
    field_106_current_motion = eAbeStates::State_3_Fall_459B60;
    field_122_knockdown_motion = -1;
    field_20_animation.field_C_render_layer = 32;
    field_198_has_evil_fart = 0;
    field_1A2_throwable_count = 0;
    field_158_throwable_id = -1;
    field_154_possesed_object_id = -1;
    field_150_OrbWhirlWind_id = -1;
    field_14C_circular_fade_id = -1;
    field_148_fade_obj_id = -1;
    field_1A8_portal_id = -1;
    field_164_wheel_id = -1;
    field_160_slapable_or_pick_item_id = -1;
    field_15C_pull_rope_id = -1;

    field_1AE_flags.Clear(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender);
    field_1AE_flags.Clear(Flags_1AE::e1AE_Bit2_do_quicksave);

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_1AC_flags.Raw().all = 0;

    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6_prevent_chanting);
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);

    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_shrivel);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_fall_to_well);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
    field_1AC_flags.Clear(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift);

    // Changes Abe's "default" colour depending on the level we are in
    SetTint_425600(&sTintTable_Abe_554D20[0], gMap_5C3030.field_0_current_level);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = 0;

    field_118_prev_held = 0;
    field_F6_anim_frame = 0;
    field_120_state = 0;
    field_168_ring_pulse_timer = 0;
    field_16E_bHaveInvisiblity = 0;
    field_170_invisible_timer = 0;
    field_174_unused = 0;
    field_176_invisibility_id = 0;
    field_178_invisible_effect_id = -1;
    field_124_timer = sGnFrame_5C1B84;
    field_FC_pPathTLV = nullptr;
    field_128.field_12_mood = Mud_Emotion::eNormal_0;
    field_128.field_18_say = MudSounds::eNone;
    field_144_auto_say_timer = 0;

    // Set Abe to be the current player controlled object
    sControlledCharacter_5C1B8C = this;

    // Create shadow
    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    
    // Animation test code
    //auto testAnim = ae_new<TestAnimation>();
    //testAnim->ctor();
    

    return this;
}

void Abe::dtor_44B380()
{
    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC

    BaseGameObject* pField_148 = sObjectIds_5C1B70.Find_449CF0(field_148_fade_obj_id);
    BaseGameObject* pField_14C = sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id);
    BaseGameObject* pField_150 = sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id);
    BaseGameObject* pField_154 = sObjectIds_5C1B70.Find_449CF0(field_154_possesed_object_id);
    BaseGameObject* pField_158 = sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id);
    BaseGameObject* pField_15C = sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id);
    BaseGameObject* pField_160 = sObjectIds_5C1B70.Find_449CF0(field_160_slapable_or_pick_item_id);
    BaseGameObject* pField_178 = sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id);

    SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);

    if (pField_148)
    {
        pField_148->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_148_fade_obj_id = -1;
    }

    if (pField_160)
    {
        field_160_slapable_or_pick_item_id = -1;
    }

    if (pField_15C)
    {
        pField_15C->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_15C_pull_rope_id = -1;
    }

    if (pField_14C)
    {
        pField_14C->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_14C_circular_fade_id = -1;
    }

    if (pField_150)
    {
        pField_150->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_150_OrbWhirlWind_id = -1;
    }

    if (pField_154)
    {
        field_154_possesed_object_id = -1;
    }

    if (pField_158)
    {
        pField_158->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_158_throwable_id = -1;
    }

    if (pField_178)
    {
        pField_178->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_178_invisible_effect_id = -1;
    }

    field_164_wheel_id = -1;

    sActiveHero_5C1B68 = spAbe_554D5C;

    dtor_4080B0();
}

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
    AEResourceID::kAbebasicResID,     // 10
    AEResourceID::kAbebsic1ResID,     // 55
    AEResourceID::kAbepullResID,      // 11
    AEResourceID::kAbepickResID,      // 12
    AEResourceID::kAbebombResID,      // 13
    AEResourceID::kAbethrowResID,     // 14
    AEResourceID::kAbesmashResID,     // 19
    AEResourceID::kAbefallResID,      // 20
    AEResourceID::kAbestoneResID,     // 21
    AEResourceID::kAbeknbkResID,      // 26
    AEResourceID::kAbeknfdResID,      // 27
    AEResourceID::kAbeknokzResID,     // 28
    AEResourceID::kAbehoistResID,     // 42
    AEResourceID::kAbeedgeResID,      // 43
    AEResourceID::kAbedoorResID,      // 45
    AEResourceID::kAbewellResID,      // 47
    AEResourceID::kAbeommResID,       // 48
    AEResourceID::kAbeliftResID,      // 53
    AEResourceID::kAbeCarResId,       // 113
    AEResourceID::kAbemorphResID,     // 117
    AEResourceID::kAbeworkResID,      // 515
    AEResourceID::kAbegasResID,       // 118
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
    const Abe_SaveState* pSaveState = reinterpret_cast<const Abe_SaveState*>(pData);

    Abe* pAbe = sActiveHero_5C1B68;
    if (sActiveHero_5C1B68 == spAbe_554D5C)
    {
        pAbe = ae_new<Abe>();
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

    sActiveHero_5C1B68->field_FC_pPathTLV = nullptr;
    sActiveHero_5C1B68->field_100_pCollisionLine = nullptr;
    sActiveHero_5C1B68->field_B8_xpos = pSaveState->field_4_xpos;
    sActiveHero_5C1B68->field_BC_ypos = pSaveState->field_8_ypos;
    sActiveHero_5C1B68->field_C4_velx = pSaveState->field_c_velx;
    sActiveHero_5C1B68->field_C8_vely = pSaveState->field_10_vely;
    sActiveHero_5C1B68->field_128.field_8_x_vel_slow_by = pSaveState->field_48_x_vel_slow_by;
    sActiveHero_5C1B68->field_128.field_C_unused = pSaveState->field_4C_unused;
    sActiveHero_5C1B68->field_C0_path_number = pSaveState->field_14_path_number;
    sActiveHero_5C1B68->field_C2_lvl_number = pSaveState->field_16_lvl_number;
    sActiveHero_5C1B68->field_CC_sprite_scale = pSaveState->field_18_sprite_scale;
    sActiveHero_5C1B68->field_D6_scale = pSaveState->field_1C_scale;

    sActiveHero_5C1B68->field_106_current_motion = pSaveState->current_motion;
    
    const AnimRecord& animRec = AnimRec(sAbeFrameTables[sActiveHero_5C1B68->field_106_current_motion]);
    BYTE** animFromState = sActiveHero_5C1B68->StateToAnimResource_44AAB0(sActiveHero_5C1B68->field_106_current_motion);
    if (!animFromState)
    {
        DWORD id = sAbeResourceIDTable_554D60[sActiveHero_5C1B68->field_128.field_10_resource_index];
        ResourceManager::LoadResourceFile_49C170(sAbe_ResNames_545830[sActiveHero_5C1B68->field_128.field_10_resource_index], 0);
        sActiveHero_5C1B68->field_10_resources_array.SetAt(sActiveHero_5C1B68->field_128.field_10_resource_index, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, id, TRUE, FALSE));
        animFromState = sActiveHero_5C1B68->field_10_resources_array.ItemAt(sActiveHero_5C1B68->field_128.field_10_resource_index);
    }

    sActiveHero_5C1B68->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, animFromState);
    //sActiveHero_5C1B68->field_20_animation.Set_Animation_Data_409C80(sAbeFrameTables[sActiveHero_5C1B68->field_106_current_motion], animFromState);
    
    sActiveHero_5C1B68->field_20_animation.field_92_current_frame = pSaveState->anim_current_frame;
    sActiveHero_5C1B68->field_20_animation.field_E_frame_change_counter = pSaveState->anim_frame_change_counter;

    sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pSaveState->bAnimFlipX & 1);
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pSaveState->bAnimRender & 1);
    sActiveHero_5C1B68->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pSaveState->bDrawable & 1);

    sActiveHero_5C1B68->field_20_animation.field_C_render_layer = pSaveState->anim_render_layer;

    if (IsLastFrame(&sActiveHero_5C1B68->field_20_animation))
    {
        sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    FrameInfoHeader* pFrameInfoHeader = sActiveHero_5C1B68->field_20_animation.Get_FrameHeader_40B730(-1);
    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*sActiveHero_5C1B68->field_20_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
    sActiveHero_5C1B68->field_20_animation.Load_Pal_40A530(sActiveHero_5C1B68->field_20_animation.field_20_ppBlock, pFrameHeader->field_0_clut_offset);

    sActiveHero_5C1B68->SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.field_0_current_level);
    sActiveHero_5C1B68->field_20_animation.field_B_render_mode = 0;
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    sActiveHero_5C1B68->field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    sActiveHero_5C1B68->field_10C_health = pSaveState->field_30_health;
    sActiveHero_5C1B68->field_106_current_motion = pSaveState->field_34_animation_num;
    sActiveHero_5C1B68->field_108_next_motion = pSaveState->next_motion;
    sActiveHero_5C1B68->field_F8_LastLineYPos = FP_FromInteger(pSaveState->last_line_ypos);
    sActiveHero_5C1B68->field_110_id = pSaveState->platform_obj_id;
    sActiveHero_5C1B68->field_120_state = static_cast<WORD>(pSaveState->field_50_state);
    sActiveHero_5C1B68->field_124_timer = pSaveState->field_54_timer;
    sActiveHero_5C1B68->field_128.field_0_gnFrame = pSaveState->field_58_gnFrame;
    sActiveHero_5C1B68->field_128.field_4_regen_health_timer = pSaveState->field_5C_regen_health_timer;
    sActiveHero_5C1B68->field_128.field_12_mood = pSaveState->mood;
    sActiveHero_5C1B68->field_128.field_18_say = pSaveState->say;
    sActiveHero_5C1B68->field_144_auto_say_timer = pSaveState->auto_say_timer;
    sActiveHero_5C1B68->field_1A2_throwable_count = pSaveState->field_6c_rock_bone_count;
    sActiveHero_5C1B68->field_168_ring_pulse_timer = pSaveState->ring_pulse_timer;
    sActiveHero_5C1B68->field_16C_bHaveShrykull = pSaveState->bHaveShrykull;

    if (sActiveHero_5C1B68->field_168_ring_pulse_timer && sActiveHero_5C1B68->field_16C_bHaveShrykull)
    {
        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(25))
        {
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("SHRYPORT.BND", nullptr);
            }
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, FALSE, FALSE))
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

    sActiveHero_5C1B68->field_16E_bHaveInvisiblity = pSaveState->bHaveInvisiblity;
    sActiveHero_5C1B68->field_104_collision_line_type = pSaveState->field_3a_collision_line_id;

    sActiveHero_5C1B68->field_118_prev_held = InputObject::Command_To_Raw_45EE40(pSaveState->prev_held);
    sActiveHero_5C1B68->field_11C_released_buttons = InputObject::Command_To_Raw_45EE40(pSaveState->released_buttons);
    sActiveHero_5C1B68->field_122_knockdown_motion = pSaveState->field_74_knockdown_motion;
    sActiveHero_5C1B68->field_128.field_14_rolling_motion_timer = sGnFrame_5C1B84 - pSaveState->field_78_rolling_motion_timer;
    sActiveHero_5C1B68->field_148_fade_obj_id = pSaveState->fade_obj_id;
    sActiveHero_5C1B68->field_14C_circular_fade_id = pSaveState->circular_fade_id;
    sActiveHero_5C1B68->field_150_OrbWhirlWind_id = pSaveState->orb_whirl_wind_id;
    sActiveHero_5C1B68->field_154_possesed_object_id = pSaveState->possesed_object_id;
    sActiveHero_5C1B68->field_158_throwable_id = pSaveState->throwabe_obj_id;
    sActiveHero_5C1B68->field_15C_pull_rope_id = pSaveState->pull_ring_rope_id;
    sActiveHero_5C1B68->field_160_slapable_or_pick_item_id = pSaveState->slapable_or_pickup_id;
    sActiveHero_5C1B68->field_164_wheel_id = pSaveState->wheel_id;
    sActiveHero_5C1B68->field_178_invisible_effect_id = -1;
    sActiveHero_5C1B68->field_170_invisible_timer = pSaveState->invisible_timer;
    sActiveHero_5C1B68->field_174_unused = pSaveState->field_A0_unused;
    sActiveHero_5C1B68->field_176_invisibility_id = pSaveState->field_A2_invisibility_id;

    sActiveHero_5C1B68->field_17C_cam_idx = pSaveState->field_A4_cam_idx;
    sActiveHero_5C1B68->field_180_hand_stone_type = pSaveState->hand_stone_type;
    sActiveHero_5C1B68->field_184_fmv_id = pSaveState->fmv_id;
    sActiveHero_5C1B68->field_186_to_camera_id[0] = pSaveState->cam_id_1;
    sActiveHero_5C1B68->field_186_to_camera_id[1] = pSaveState->cam_id_2;
    sActiveHero_5C1B68->field_186_to_camera_id[2] = pSaveState->cam_id_3;
    sActiveHero_5C1B68->field_18C_unused = pSaveState->field_B4_unused;

    sActiveHero_5C1B68->field_18E_unused = pSaveState->field_B6_unused;
    sActiveHero_5C1B68->field_190_unused = pSaveState->field_B8_unused;
    sActiveHero_5C1B68->field_192_unused = pSaveState->field_BA_unused;
    sActiveHero_5C1B68->field_194_unused = pSaveState->field_BC_unused;
    sActiveHero_5C1B68->field_196_unused = pSaveState->field_BE_unused;
    sActiveHero_5C1B68->field_198_has_evil_fart = pSaveState->bHaveEvilFart;
    sActiveHero_5C1B68->field_19A_to_level = pSaveState->to_level;
    sActiveHero_5C1B68->field_19C_to_path = pSaveState->to_path;
    sActiveHero_5C1B68->field_19E_to_camera = pSaveState->to_camera;
    sActiveHero_5C1B68->field_1A0_door_id = pSaveState->door_id;
    sActiveHero_5C1B68->field_1A3_throw_direction = pSaveState->field_ca_throw_direction;
    sActiveHero_5C1B68->field_1A4_portal_sub_state = pSaveState->field_CC_portal_sub_state;
    sActiveHero_5C1B68->field_1A8_portal_id = pSaveState->bird_portal_id;

    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted, pSaveState->bElectrocuted & 1);
    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit8_bInvisible, pSaveState->field_42_bInvisible & 1);
    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit10_Teleporting, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit13_teleporting));

    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit1_lift_point_dead_while_using_lift));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit2_return_to_previous_motion));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_fall_to_well, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit3_fall_to_well));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit4_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit4_unused));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel, pSaveState->bShrivel);
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit6_prevent_chanting, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit5_prevent_chanting));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit6_land_softly));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit8_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit7_unused));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_laugh_at_chant_end, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit8_laugh_at_chant_end));

    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_Bit12_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit9_unused));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit10_play_ledge_grab_sounds));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit14_unused, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit11_unused));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit15_have_healing, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_Bit12_have_healing));
    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit10_Teleporting, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit13_teleporting));
    sActiveHero_5C1B68->field_1AC_flags.Set(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit14_is_mudanchee_vault_ender));

    sActiveHero_5C1B68->field_1AE_flags.Set(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit15_is_mudomo_vault_ender));
    sActiveHero_5C1B68->field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit2_Enabled, pSaveState->field_D4_flags.Get(Abe_SaveState::eD4_eBit16_shadow_enabled));

    sActiveHero_5C1B68->field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom, pSaveState->field_D6_flags.Get(Abe_SaveState::eD6_Bit1_shadow_at_bottom));

    if (sActiveHero_5C1B68->field_198_has_evil_fart)
    {
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("EVILFART.BAN", nullptr);
        }

        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(22))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(22, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, TRUE, FALSE));
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
        }

        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(24))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(24, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, TRUE, FALSE));
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
        }

        if (!sActiveHero_5C1B68->field_10_resources_array.ItemAt(23))
        {
            sActiveHero_5C1B68->field_10_resources_array.SetAt(23, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, TRUE, FALSE));
        }
    }

    return sizeof(Abe_SaveState);
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

__int16 Abe::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_44BB50(pFrom);
}

void Abe::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_TLV_Collision_44B5D0(pTlv);
}

BirdPortal* Abe::VIntoBirdPortal_408FD0(__int16 gridBlocks)
{
    return vIntoBirdPortal_44E970(gridBlocks);
}

void Abe::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_45A570();
}

BaseGameObject* Abe::vdtor_44B350(signed int flags)
{
    dtor_44B380();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
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

ALIVE_VAR(1, 0x5c1bda, short, gAbeBulletProof_5C1BDA, 0);


void Abe::Update_449DC0()
{
    if (gAbeBulletProof_5C1BDA) // Some flag to reset HP?
    {
        field_114_flags.Clear(Flags_114::e114_Bit7_Electrocuted);
        field_10C_health = FP_FromDouble(1.0);
    }

    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (field_104_collision_line_type != -1)
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);

            field_104_collision_line_type = -1;
        }

        field_110_id = BaseGameObject::Find_Flags_4DC170(field_110_id);
        field_148_fade_obj_id = BaseGameObject::Find_Flags_4DC170(field_148_fade_obj_id);
        field_14C_circular_fade_id = BaseGameObject::Find_Flags_4DC170(field_14C_circular_fade_id);
        field_1A8_portal_id = BaseGameObject::Find_Flags_4DC170(field_1A8_portal_id);
        field_150_OrbWhirlWind_id = BaseGameObject::Find_Flags_4DC170(field_150_OrbWhirlWind_id);
        field_154_possesed_object_id = BaseGameObject::Find_Flags_4DC170(field_154_possesed_object_id);
        field_158_throwable_id = BaseGameObject::Find_Flags_4DC170(field_158_throwable_id);
        field_15C_pull_rope_id = BaseGameObject::Find_Flags_4DC170(field_15C_pull_rope_id);
        field_160_slapable_or_pick_item_id = BaseGameObject::Find_Flags_4DC170(field_160_slapable_or_pick_item_id);
        field_164_wheel_id = BaseGameObject::Find_Flags_4DC170(field_164_wheel_id);

        if (field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            if (!field_170_invisible_timer)
            {
                field_170_invisible_timer = sGnFrame_5C1B84 + 2;
            }

            auto pClass = ae_new<InvisibleEffect>();
            pClass->ctor_45F280(this);
            field_178_invisible_effect_id = pClass->field_8_object_id;
            pClass->InstantInvisibility_45FA00();
        }
    }

    if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit12_unused))
    {
        return;
    }

    if (gAbeBulletProof_5C1BDA)
    {
        field_10C_health = FP_FromDouble(1.0);
    }

    if (!Input_IsChanting_45F260())
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit6_prevent_chanting);
        field_174_unused = 1;
    }

    const int totalAliveSavedMuds = sRescuedMudokons_5C1BC2 - sKilledMudokons_5C1BC0;
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

        field_F8_LastLineYPos = field_BC_ypos;
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit5_shrivel);
        field_106_current_motion = eAbeStates::jState_85_Fall_455070;
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
                SetActiveCameraDelayedFromDir_408C40();
                return;
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
        }

        SetActiveCameraDelayedFromDir_408C40();
    }
    else
    {
        // Handle none DDCheat mode

        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

        short state_idx = field_106_current_motion;

        // Execute the current state
        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;
        (this->*(sAbeStateMachineTable_554910)[state_idx])();

        if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave) || field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel))
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

        if (field_114_flags.Get(Flags_114::e114_Bit1_bShot))
        {
            state_idx = field_122_knockdown_motion;
            ToKnockback_44E700(1, 1);
            if (state_idx != -1)
            {
                field_106_current_motion = state_idx;
            }

            field_108_next_motion = 0;
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
            field_122_knockdown_motion = eAbeStates::State_0_Idle_44EEB0;
            field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        }

        if (Event_Get_422C00(kEventScreenShake) && field_10C_health > FP_FromInteger(0))
        {
            if (IsStanding_449D30())
            {
                ToKnockback_44E700(1, 0);
            }
        }

        if (field_128.field_18_say != MudSounds::eNone && static_cast<int>(sGnFrame_5C1B84) >= field_144_auto_say_timer)
        {
            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0)
                || (field_106_current_motion == eAbeStates::State_112_Chant_45B1C0)
                || field_106_current_motion == eAbeStates::State_7_Speak_45B140
                || field_106_current_motion == eAbeStates::State_8_Speak_45B160
                || field_106_current_motion == eAbeStates::State_9_Speak_45B180
                || field_106_current_motion == eAbeStates::State_10_Fart_45B1A0)
            {
                // Prevents double laugh when blowing up a slig as we prevent saying anything while chanting or various other states
                // or when abe isn't in the active screen
                field_128.field_18_say = MudSounds::eNone;
            }
            else
            {
                if (field_106_current_motion == eAbeStates::State_0_Idle_44EEB0 || field_106_current_motion == eAbeStates::State_12_Null_4569C0)
                {
                    field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                    switch (field_128.field_18_say)
                    {
                    case MudSounds::eOops_14:
                        field_106_current_motion = eAbeStates::State_34_DunnoBegin_44ECF0;
                        break;
                    case MudSounds::eAnger_5:
                        field_106_current_motion = eAbeStates::State_10_Fart_45B1A0;
                        break;
                    case MudSounds::eSadUgh_28:
                        field_106_current_motion = eAbeStates::State_10_Fart_45B1A0;
                        break;
                    default:
                        field_106_current_motion = eAbeStates::State_9_Speak_45B180;
                        break;
                    }
                }

                if (field_128.field_18_say == MudSounds::eAnger_5)
                {
                    // Other evil muds laugh at the abe grr
                    Event_Broadcast_422BC0(kEventMudokonLaugh, sActiveHero_5C1B68);
                }

                if (field_128.field_18_say == MudSounds::eSadUgh_28)
                {
                    // This one has another volume for whatever reason
                    Mudokon_SFX_457EC0(field_128.field_18_say, 80, 0, this);
                }
                else
                {
                    Mudokon_SFX_457EC0(field_128.field_18_say, 0, 0, this);
                }

                field_128.field_18_say = MudSounds::eNone;
            }
        }

        if (state_idx != field_106_current_motion || field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2))
        {
            field_114_flags.Clear(Flags_114::e114_MotionChanged_Bit2);
            if (field_106_current_motion != eAbeStates::State_12_Null_4569C0 && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)))
            {
                const AnimRecord& animRec = AnimRec(sAbeFrameTables[field_106_current_motion]);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, StateToAnimResource_44AAB0(field_106_current_motion));
                //field_20_animation.Set_Animation_Data_409C80( sAbeFrameTables[field_106_current_motion], StateToAnimResource_44AAB0(field_106_current_motion));

                field_128.field_14_rolling_motion_timer = sGnFrame_5C1B84;

                if (state_idx == eAbeStates::State_12_Null_4569C0 || state_idx == eAbeStates::State_60_4A3200)
                {
                    field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                }
            }
        }

        if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit2_return_to_previous_motion))
        {
            field_106_current_motion = field_F4_previous_motion;
            
            const AnimRecord& animRec = AnimRec(sAbeFrameTables[field_106_current_motion]);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, StateToAnimResource_44AAB0(field_106_current_motion));
            //field_20_animation.Set_Animation_Data_409C80( sAbeFrameTables[field_106_current_motion], StateToAnimResource_44AAB0(field_106_current_motion));

            field_128.field_14_rolling_motion_timer = sGnFrame_5C1B84;
            field_20_animation.SetFrame_409D50(field_F6_anim_frame);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
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
                            else if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing))
                            {
                                ringType = RingTypes::eHealing_Pulse_14;
                            }

                            PSX_RECT rect = {};
                            vGetBoundingRect_424FD0(&rect, 1);
                            AbilityRing::Factory_482F80(
                                FP_FromInteger((rect.x + rect.w) / 2),
                                FP_FromInteger((rect.y + rect.h) / 2),
                                ringType, field_CC_sprite_scale);

                            SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 25, 2650);
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


        InvisibleEffect* pObj_field_178 = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find(field_178_invisible_effect_id, Types::eInvisibleEffect_75));
        if (pObj_field_178 && field_170_invisible_timer > 0)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_170_invisible_timer)
            {
                field_170_invisible_timer = 0;
                pObj_field_178->BecomeVisible_45FA30();
            }
        }

        // After the trials give Abe the healing power for the Necrum muds.
        if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender))
        {
            if (field_1AE_flags.Get(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender))
            {
                if (gMap_5C3030.field_0_current_level == LevelIds::eNecrum_2)
                {
                    field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
                    field_16C_bHaveShrykull = 0;
                    field_16E_bHaveInvisiblity = 0;
                    field_1AE_flags.Clear(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender);
                    field_1AC_flags.Clear(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender);
                    field_1AC_flags.Set(Flags_1AC::e1AC_eBit15_have_healing);
                }
            }
        }

        if (Event_Get_422C00(kEventMudokonDied))
        {
            field_128.field_18_say = MudSounds::eOops_14;
            field_144_auto_say_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);

            // Do the death jingle
            ae_new<MusicTrigger>()->ctor_47FF10(1, 0, 90, 0);
        }

        if (Event_Get_422C00(kEventMudokonComfort))
        {
            field_128.field_18_say = MudSounds::eGiggle_8;
            field_144_auto_say_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        }

        if (Event_Get_422C00(kEventMudokonComfort | kEventSpeaking))
        {
            field_128.field_18_say = MudSounds::eOops_14;
            field_144_auto_say_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
        }

        if (field_1AE_flags.Get(Flags_1AE::e1AE_Bit2_do_quicksave))
        {
            field_1AE_flags.Clear(Flags_1AE::e1AE_Bit2_do_quicksave);
            sActiveQuicksaveData_BAF7F8.field_204_world_info.field_A_save_num = field_1B0_save_num;
            Quicksave_SaveWorldInfo_4C9310(&sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info);
            vGetSaveState_457110(reinterpret_cast<BYTE*>(&sActiveQuicksaveData_BAF7F8.field_284_restart_path_abe_state));
            sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states = sSwitchStates_5C1A28;
            Quicksave_4C90D0();
        }
    }
}

BirdPortal* Abe::vIntoBirdPortal_44E970(__int16 gridBlocks)
{
    auto pPortal = BaseAliveGameObject::VIntoBirdPortal_408FD0(gridBlocks);
    if (pPortal && pPortal->field_24_portal_type == PortalType::eAbe_0)
    {
        return pPortal;
    }
    return nullptr;
}

void Abe::vOnTrapDoorOpen_45A570()
{
    // Handles falling when previously was on a platform, stop turning a wheel if we where turning one etc.
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find(field_164_wheel_id, Types::eWheel_148));
    if (pPlatform)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)))
        {
            VSetMotion_4081C0(eAbeStates::State_93_WalkOffEdge_455970);
        }

        pPlatform->VRemove(this);

        field_110_id = -1;
        field_F8_LastLineYPos = field_BC_ypos;

        if (pWheel)
        {
            pWheel->VStopTurning(FALSE);
        }
    }
}

void Abe::ToKnockback_44E700(__int16 bKnockbackSound, __int16 bDelayedAnger)
{
    OrbWhirlWind* pfield_150 = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));
    BaseThrowable* pfield_158 = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id));
    WorkWheel* pfield_164 = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find(field_164_wheel_id, Types::eWheel_148));
    if (sControlledCharacter_5C1B8C == this || field_10C_health <= FP_FromInteger(0))
    {
        // Chant music/orb kill ?
        SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);
        if (pfield_150)
        {
            pfield_150->ToStop_4E4050();
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

        // NOTE: This always seems to be set to true. This parameter might not be needed.
        if (bKnockbackSound)
        {
            Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, Math_RandomRange_496AB0(-127, 127), this);
            Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
        }

        field_106_current_motion = eAbeStates::State_71_Knockback_455090;

        if (bDelayedAnger)
        {
            field_128.field_18_say = MudSounds::eAnger_5; // anger in..
            field_144_auto_say_timer = sGnFrame_5C1B84 + 27; // 27 ticks
        }

        if (pfield_158)
        {
            pfield_158->VToDead_4114B0();
            field_158_throwable_id = -1;
            if (!gInfiniteGrenades_5C1BDE)
            {
                field_1A2_throwable_count++;
            }
        }
    }
}

void Abe::vRender_44B580(int** pOrderingTable)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)))
    {
        field_20_animation.field_14_scale = field_CC_sprite_scale;
    }

    if (field_106_current_motion != eAbeStates::State_79_InsideWellLocal_45CA60 && field_106_current_motion != eAbeStates::State_82_InsideWellExpress_45CC80 && field_106_current_motion != eAbeStates::State_76_ToInsideOfAWellLocal_45CA40)
    {
        Render_424B90(pOrderingTable);
    }
}

void Abe::vScreenChanged_44D240()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        field_C2_lvl_number = gMap_5C3030.field_A_level;
        field_C0_path_number = gMap_5C3030.field_C_path;
    }

    // Level has changed?
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level)
    {
        // Hack to make Abe say hello in the first screen of the mines
        if (gMap_5C3030.field_A_level == LevelIds::eMines_1 && !gAttract_5C1BA0)
        {
            field_128.field_18_say = MudSounds::eHelloNeutral_3;
            field_144_auto_say_timer = sGnFrame_5C1B84 + 35;
        }

        // Set the correct tint for this map
        SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.field_A_level);

        if (gMap_5C3030.field_0_current_level != LevelIds::eNone)
        {
            if (field_1A2_throwable_count > 0)
            {
                if (gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C->Remove_49AA00(field_1A2_throwable_count);
                }
            }

            field_1A2_throwable_count = 0;

            if (field_168_ring_pulse_timer > 0 && field_16C_bHaveShrykull)
            {
                Free_Shrykull_Resources_45AA90();
            }

            field_168_ring_pulse_timer = 0;
        }

        if (gMap_5C3030.field_A_level == LevelIds::eNecrum_2)
        {
            if (gMap_5C3030.field_0_current_level == LevelIds::eMudancheeVault_Ender_7)
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender);
            }

            if (gMap_5C3030.field_0_current_level == LevelIds::eMudomoVault_Ender_11)
            {
                field_1AE_flags.Set(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender);
            }
        }

        if (gMap_5C3030.field_A_level == LevelIds::eCredits_16 || gMap_5C3030.field_A_level == LevelIds::eMenu_0)
        {
            // Remove Abe for menu/credits levels?
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    // If level or path changed then kill rings and farts
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_168_ring_pulse_timer = 0;
        if (gMap_5C3030.field_0_current_level != LevelIds::eNone)
        {
            field_198_has_evil_fart = 0;
        }
    }

    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level && !(field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave)))
    {
        for (char& val : sSavedKilledMudsPerPath_5C1B50.mData)
        {
            val = 0;
        }
    }
}

int Abe::vGetSaveState_457110(BYTE* pSaveBuffer)
{
    Abe_SaveState* pSaveState = reinterpret_cast<Abe_SaveState*>(pSaveBuffer);

    pSaveState->field_0_id = Types::eAbe_69;
    pSaveState->field_4_xpos = field_B8_xpos;
    pSaveState->field_8_ypos = field_BC_ypos;
    pSaveState->field_c_velx = field_C4_velx;
    pSaveState->field_10_vely = field_C8_vely;
    pSaveState->field_48_x_vel_slow_by = field_128.field_8_x_vel_slow_by;
    pSaveState->field_4C_unused = field_128.field_C_unused;
    pSaveState->field_14_path_number = field_C0_path_number;
    pSaveState->field_16_lvl_number = field_C2_lvl_number;
    pSaveState->field_18_sprite_scale = field_CC_sprite_scale;
    pSaveState->field_1C_scale = field_D6_scale;
    pSaveState->field_1e_r = field_D0_r;
    pSaveState->field_20_g = field_D2_g;
    pSaveState->field_22_b = field_D4_b;

    if (field_114_flags.Get(Flags_114::e114_Bit11_Electrocuting))
    {
        for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_4_typeId == Types::eElectrocute_150)
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

    pSaveState->bAnimFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pSaveState->current_motion = field_106_current_motion;
    pSaveState->anim_current_frame = field_20_animation.field_92_current_frame;
    pSaveState->anim_frame_change_counter = field_20_animation.field_E_frame_change_counter;

    if (field_20_animation.field_E_frame_change_counter == 0)
    {
        pSaveState->anim_frame_change_counter = 1;
    }

    pSaveState->bDrawable = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pSaveState->bAnimRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pSaveState->anim_render_layer = static_cast<char>(field_20_animation.field_C_render_layer);
    pSaveState->field_30_health = field_10C_health;
    pSaveState->field_34_animation_num = field_106_current_motion;
    pSaveState->next_motion = field_108_next_motion;
    pSaveState->last_line_ypos = FP_GetExponent(field_F8_LastLineYPos);

    pSaveState->bElectrocuted = field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted);

    pSaveState->field_42_bInvisible = field_114_flags.Get(Flags_114::e114_Bit8_bInvisible);

    if (field_100_pCollisionLine)
    {
        pSaveState->field_3a_collision_line_id = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pSaveState->field_3a_collision_line_id = -1;
    }

    pSaveState->platform_obj_id = field_110_id;

    if (field_110_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj)
        {
            pSaveState->platform_obj_id = pObj->field_C_objectId;
        }
    }

    pSaveState->field_44_is_abe_controlled = (this == sControlledCharacter_5C1B8C);
    pSaveState->field_50_state = field_120_state;
    pSaveState->field_54_timer = field_124_timer;
    pSaveState->field_58_gnFrame = field_128.field_0_gnFrame;
    pSaveState->field_5C_regen_health_timer = field_128.field_4_regen_health_timer;
    pSaveState->mood = field_128.field_12_mood;
    pSaveState->say = field_128.field_18_say;
    pSaveState->auto_say_timer = field_144_auto_say_timer;
    pSaveState->ring_pulse_timer = field_168_ring_pulse_timer;
    pSaveState->field_6c_rock_bone_count = field_1A2_throwable_count;
    pSaveState->bHaveShrykull = static_cast<char>(field_16C_bHaveShrykull);
    pSaveState->bHaveInvisiblity = static_cast<char>(field_16E_bHaveInvisiblity);

    pSaveState->prev_held = InputObject::Raw_To_Command_45EF70(field_118_prev_held);
    pSaveState->released_buttons = InputObject::Raw_To_Command_45EF70(field_11C_released_buttons);

    pSaveState->field_74_knockdown_motion = field_122_knockdown_motion;
    pSaveState->field_78_rolling_motion_timer = sGnFrame_5C1B84 - field_128.field_14_rolling_motion_timer;
    pSaveState->fade_obj_id = field_148_fade_obj_id;

    if (field_148_fade_obj_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_148_fade_obj_id);
        if (pObj)
        {
            pSaveState->fade_obj_id = pObj->field_C_objectId;
        }
    }

    pSaveState->circular_fade_id = field_14C_circular_fade_id;

    if (field_14C_circular_fade_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id);
        if (pObj)
        {
            pSaveState->circular_fade_id = pObj->field_C_objectId;
        }
    }
    pSaveState->orb_whirl_wind_id = field_150_OrbWhirlWind_id;

    if (field_150_OrbWhirlWind_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id);
        if (pObj)
        {
            pSaveState->orb_whirl_wind_id = pObj->field_C_objectId;
        }
    }

    pSaveState->possesed_object_id = field_154_possesed_object_id;

    if (field_154_possesed_object_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_154_possesed_object_id);
        if (pObj)
        {
            pSaveState->possesed_object_id = pObj->field_C_objectId;
        }
    }

    pSaveState->throwabe_obj_id = field_158_throwable_id;

    if (field_158_throwable_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id);
        if (pObj)
        {
            pSaveState->throwabe_obj_id = pObj->field_C_objectId;
        }
    }

    pSaveState->pull_ring_rope_id = field_15C_pull_rope_id;

    if (field_15C_pull_rope_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id);
        if (pObj)
        {
            pSaveState->pull_ring_rope_id = pObj->field_C_objectId;
        }
    }

    pSaveState->slapable_or_pickup_id = field_160_slapable_or_pick_item_id;

    if (field_160_slapable_or_pick_item_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_160_slapable_or_pick_item_id);
        if (pObj)
        {
            pSaveState->slapable_or_pickup_id = pObj->field_C_objectId;
        }
    }

    pSaveState->wheel_id = field_164_wheel_id;

    if (field_164_wheel_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id);
        if (pObj)
        {
            pSaveState->wheel_id = pObj->field_C_objectId;
        }
    }

    pSaveState->bird_portal_id = field_1A8_portal_id;

    if (field_1A8_portal_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_1A8_portal_id);
        if (pObj)
        {
            pSaveState->bird_portal_id = pObj->field_C_objectId;
        }
    }

    pSaveState->invisible_timer = field_170_invisible_timer;
    pSaveState->field_A0_unused = field_174_unused;
    pSaveState->field_A2_invisibility_id = field_176_invisibility_id;
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
    pSaveState->to_level = field_19A_to_level;
    pSaveState->to_path = field_19C_to_path;
    pSaveState->to_camera = field_19E_to_camera;
    pSaveState->door_id = field_1A0_door_id;
    pSaveState->field_ca_throw_direction = field_1A3_throw_direction;
    pSaveState->field_CC_portal_sub_state = field_1A4_portal_sub_state;

    pSaveState->bElectrocuted = field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted);
    pSaveState->field_42_bInvisible = field_114_flags.Get(Flags_114::e114_Bit8_bInvisible);
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit13_teleporting, sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit10_Teleporting));

    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit1_lift_point_dead_while_using_lift, field_1AC_flags.Get(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit2_return_to_previous_motion, field_1AC_flags.Get(Flags_1AC::e1AC_Bit2_return_to_previous_motion));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_Bit3_fall_to_well, field_1AC_flags.Get(Flags_1AC::e1AC_Bit3_fall_to_well));
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
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit13_teleporting, field_114_flags.Get(Flags_114::e114_Bit10_Teleporting));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit14_is_mudanchee_vault_ender, field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender));

    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit15_is_mudomo_vault_ender, field_1AE_flags.Get(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender));
    pSaveState->field_D4_flags.Set(Abe_SaveState::eD4_eBit16_shadow_enabled, field_E0_pShadow->field_14_flags.Get(Shadow::Flags::eBit2_Enabled));

    pSaveState->field_D6_flags.Set(Abe_SaveState::eD6_Bit1_shadow_at_bottom, field_E0_pShadow->field_14_flags.Get(Shadow::Flags::eBit1_ShadowAtBottom));

    return sizeof(Abe_SaveState);
}

__int16 Abe::vTakeDamage_44BB50(BaseGameObject* pFrom)
{
    // Stop chant sound music.
    SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);

    const MudSounds oldSay = field_128.field_18_say;
    field_128.field_18_say = MudSounds::eNone;

    OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));
    if (pWhirlWind)
    {
        pWhirlWind->ToStop_4E4050();
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

    if (field_114_flags.Get(Flags_114::e114_Bit10_Teleporting))
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
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_10C_health = FP_FromInteger(0);
                field_106_current_motion = eAbeStates::State_129_PoisonGasDeath_4565C0;
                field_124_timer = 1;
            }
            else
            {
                if (field_106_current_motion != eAbeStates::State_71_Knockback_455090 &&
                    field_106_current_motion != eAbeStates::State_72_KnockbackGetUp_455340)
                {
                    ToKnockback_44E700(1, 1);
                    field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
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

        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_10C_health = FP_FromInteger(0);
        ToKnockback_44E700(1, 1);

        field_D4_b = 30;
        field_D2_g = 30;
        field_D0_r = 30;

        auto pGibs = ae_new<Gibs>();
        pGibs->ctor_40FB40(
            GibType::Abe_0,
            field_B8_xpos,
            field_BC_ypos,
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_CC_sprite_scale,
            0);

        // Note Check on word_5CC88C <= 3846 appeared always true, removed.
        auto pMoreGibs = ae_new<Gibs>();
        pMoreGibs->ctor_40FB40(
            GibType::Abe_0,
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
        Mudokon_SFX_457EC0(MudSounds::eOops_14, 0, 0, this);
        break;

    case Types::eBaseBomb_46:
    case Types::eMineCar_89:
    case Types::eExplosion_109:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_114_flags.Set(Flags_114::e114_Bit1_bShot);
                field_122_knockdown_motion = eAbeStates::State_71_Knockback_455090;
                field_108_next_motion = eAbeStates::State_71_Knockback_455090;
            }

            field_10C_health = FP_FromInteger(0);

            field_D4_b = 30;
            field_D2_g = 30;
            field_D0_r = 30;

            auto pGibs = ae_new<Gibs>();
            pGibs->ctor_40FB40(
                GibType::Abe_0,
                field_B8_xpos,
                field_BC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_E0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit2_Enabled);
        }
        break;

    case Types::eSecurityClaw_47:
    case Types::eSecurityOrb_83:
        field_128.field_18_say = MudSounds::eAnger_5;
        field_144_auto_say_timer = sGnFrame_5C1B84 + 27;
        if (field_106_current_motion != eAbeStates::State_123_LiftGrabIdle_45A6A0 &&
            field_106_current_motion != eAbeStates::State_124_LiftUseUp_45A780 &&
            field_106_current_motion != eAbeStates::State_125_LiftUseDown_45A7B0)
        {
            ToKnockback_44E700(1, 1);
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        }

        // The zap makes Abe drop his stuff everywhere
        for (int i = 0; i < field_1A2_throwable_count; i++)
        {
            BaseThrowable* pThrowable = Make_Throwable_49AF30(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(30),
                0);

            // Random explode time ?
            const FP rand1 = FP_FromRaw((Math_NextRandom() - 127) << 11); // TODO: Wat?
            const FP rand2 = (FP_FromDouble(0.03125) * FP_FromRaw(Math_NextRandom())) - FP_FromInteger(2);
            pThrowable->VThrow_49E460(rand1, rand2);

            pThrowable->field_CC_sprite_scale = field_CC_sprite_scale;
            pThrowable->field_D6_scale = field_D6_scale;
            pThrowable->VTimeToExplodeRandom_411490(); // Start count down ?
        }
        field_1A2_throwable_count = 0;
        break;

    case Types::eRockSpawner_48:
        if (field_10C_health > FP_FromInteger(0))
        {
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_10C_health = FP_FromInteger(0);

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }
            ToKnockback_44E700(1, 1);
            SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);
        }
        break;

    case Types::eFleech_50:
        if (field_10C_health > FP_FromInteger(0))
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

            field_10C_health -= FP_FromDouble(0.15075); // Yes it has to be this accurate to match

            if (field_10C_health < FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                Mudokon_SFX_457EC0(MudSounds::eLaugh_10, 0, 1000, this);
            }

            if (field_10C_health > FP_FromInteger(0))
            {
                // The damage sound from a Fleech keeps getting high and higher pitch till death
                const FP hpRandSoundRange = (FP_FromInteger(1) - sActiveHero_5C1B68->field_10C_health) / FP_FromDouble(0.15);
                const short pitchRand = Math_RandomRange_496AB0(
                    200 * (FP_GetExponent(hpRandSoundRange)),
                    40 * (5 * (FP_GetExponent(hpRandSoundRange)) + 5));

                Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, pitchRand, this);
                return 1;
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            auto pBlood = ae_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_40F0B0(
                    field_B8_xpos,
                    // Put YPos in the middle of who is getting damaged
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    FP_FromInteger((field_B8_xpos - pAliveObj->field_B8_xpos < FP_FromInteger(0)) ? -24 : 24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            ToKnockback_44E700(1, 1);
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);

            if (pAliveObj->field_B8_xpos < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
            }

            if (pAliveObj->field_B8_xpos > field_B8_xpos && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
            {
                field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
            }

            if (pAliveObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-7.8);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
            }

            SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);
            SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 90);
        }
        break;

    case Types::eGreeter_64:
        if (field_10C_health <= FP_FromInteger(0))
        {
            break;
        }

        Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, 0, this);
        Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
        field_114_flags.Set(Flags_114::e114_Bit1_bShot);
        field_122_knockdown_motion = eAbeStates::State_101_KnockForward_455420;
        field_108_next_motion = eAbeStates::State_101_KnockForward_455420;
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
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

            field_10C_health = FP_FromInteger(0);

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            auto pBlood = ae_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_40F0B0(
                    field_B8_xpos,
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    (pAliveObj->field_C4_velx <= FP_FromInteger(0)) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    50);
            }

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            ToKnockback_44E700(1, 1);
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);

            if (pAliveObj->field_B8_xpos < field_B8_xpos)
            {
                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                {
                    field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pAliveObj->field_B8_xpos > field_B8_xpos)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pAliveObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-7.8);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
            }

            SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);

            if (pFrom->field_4_typeId != Types::eParamite_96)
            {
                SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 90);
            }
        }
        break;

    case Types::eAbilityRing_104:
        return 0;

    case Types::eType_107:
        if (field_10C_health > FP_FromInteger(0))
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_10C_health = FP_FromInteger(0);

            if (ForceDownIfHoisting_44BA30())
            {
                return 1;
            }

            ToKnockback_44E700(1, 1);

            if (pAliveObj->field_B8_xpos < field_B8_xpos)
            {
                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                {
                    field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pAliveObj->field_B8_xpos > field_B8_xpos)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
                }
            }

            if (pAliveObj->field_C4_velx >= FP_FromInteger(0))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-7.8);
            }

            SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);
        }
        break;

    case Types::eMudokon_110:
        if (field_10C_health > FP_FromInteger(0) && field_106_current_motion != eAbeStates::State_71_Knockback_455090)
        {
            field_10C_health -= FP_FromDouble(0.07);
            if (field_10C_health <= FP_FromInteger(0) ||
                (field_106_current_motion != eAbeStates::State_123_LiftGrabIdle_45A6A0 &&
                field_106_current_motion != eAbeStates::State_124_LiftUseUp_45A780 &&
                field_106_current_motion != eAbeStates::State_125_LiftUseDown_45A7B0))
            {
                ToKnockback_44E700(1, 1);
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                if (field_10C_health <= FP_FromInteger(0))
                {
                    Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, 1000, this);
                    Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
                    field_10C_health = FP_FromInteger(0);
                }
            }
        }
        break;

    case Types::eSlamDoor_122:
        if (field_10C_health > FP_FromInteger(0))
        {
            ToKnockback_44E700(1, 1);
            VCheckCollisionLineStillValid_408A40(5);
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        }
        break;

    case Types::eElectrocute_150:
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        ToDieFinal_458910();
        break;

    default:
        if (pFrom->field_4_typeId != Types::eBullet_15)
        {
            LOG_ERROR("Expected default case to be bullets only but got: " << static_cast<int>(pFrom->field_4_typeId));
        }
        BulletDamage_44C980(static_cast<Bullet*>(pFrom));
        if (!field_114_flags.Get(Flags_114::e114_Bit1_bShot))
        {
            ret = 0;
            field_128.field_18_say = oldSay;
        }
        break;
    }

    if (sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed))
    {
        if (field_10C_health == FP_FromInteger(0))
        {
            sControlledCharacter_5C1B8C->VUnPosses_408F90();
            // We are the "active" player again
            GiveControlBackToMe_44BA10();
        }
    }

    return ret;
}

struct Path_ContinuePoint : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_save_file_id;
};

struct Path_Type_76 : public Path_TLV
{
    __int16 field_10_set_switches;
    __int16 field_12_start_id;
    __int16 field_14_end_id;
    __int16 field_16_skip_id;
    __int16 field_18_free_path_res;
    __int16 field_1A_path_to_free_id;
    __int16 field_1C_bEnabled;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Type_76, 0x20);

void Abe::vOn_TLV_Collision_44B5D0(Path_TLV* pTlv)
{
    for (; pTlv;  pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
        pTlv,
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos))
    {
        if (pTlv->field_4_type == TlvTypes::ContinuePoint_0)
        {
            auto pContinuePoint = static_cast<Path_ContinuePoint*>(pTlv);
            if (pContinuePoint->field_1_unknown == 0)
            {
                if ((pContinuePoint->field_10_scale != 1 || field_CC_sprite_scale == FP_FromInteger(1)) &&
                    (pContinuePoint->field_10_scale != 2 || field_CC_sprite_scale == FP_FromDouble(0.5))
                    && field_10C_health > FP_FromInteger(0) && !(field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted)))
                {
                    pContinuePoint->field_1_unknown = 1;
                    field_1AE_flags.Set(Flags_1AE::e1AE_Bit2_do_quicksave);
                    field_1B0_save_num = pContinuePoint->field_12_save_file_id;
                }
            }
        }
        else if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (sControlledCharacter_5C1B8C->field_4_typeId != Types::eMineCar_89 || gMap_5C3030.field_0_current_level != LevelIds::eMines_1)
            {
                Mudokon_SFX_457EC0(MudSounds::eDeathDropScream_15, 0, 0, this);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventLoudNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
                ToDie_4588D0();
            }
        }
        else if (pTlv->field_4_type == TlvTypes::Null_76)
        {
            auto pUnknown = static_cast<Path_Type_76*>(pTlv);
            if (pUnknown->field_1_unknown == 0 || pUnknown->field_1C_bEnabled)
            {
                pUnknown->field_1_unknown = 1;
                if (pUnknown->field_10_set_switches)
                {
                    for (short i = pUnknown->field_12_start_id; i <= pUnknown->field_14_end_id; i++)
                    {
                        if (i != pUnknown->field_16_skip_id && i > 1)
                        {
                            SwitchStates_Set_465FF0(i, 0);
                        }
                    }
                }
                if (pUnknown->field_18_free_path_res)
                {
                    Path::Reset_TLVs_4DBCF0(pUnknown->field_1A_path_to_free_id);
                }
            }

        }

    }

}

BaseAliveGameObject* Abe::FindObjectToPosses_44B7B0()
{
    BaseAliveGameObject* pTargetObj = nullptr;
    BaseAliveGameObject* pInRangeFart = nullptr;
    BaseAliveGameObject* pInRangeGlukkon = nullptr;

    short maxDistance = 32767;
    FP lastScale = {};
    for (int i = 0; i<gBaseAliveGameObjects_5C1B7C->Size(); i++)
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
                if (pObj->Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0 && pObj->field_10C_health > FP_FromInteger(0))
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
                if (pObj->Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
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
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, FALSE, FALSE))
    {
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("ABEBSIC.BAN", nullptr);
            ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, TRUE, FALSE);
        }
        field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbebasicResID, FALSE, FALSE));
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

EXPORT BOOL Abe::IsStanding_449D30()
{
    return
        field_106_current_motion == eAbeStates::State_0_Idle_44EEB0
        || field_106_current_motion == eAbeStates::State_2_StandingTurn_451830
        || field_106_current_motion == eAbeStates::State_34_DunnoBegin_44ECF0
        || field_106_current_motion == eAbeStates::State_35_DunnoEnd_44ED10
        || field_106_current_motion == eAbeStates::State_11_Speak_45B0A0
        || field_106_current_motion == eAbeStates::State_7_Speak_45B140
        || field_106_current_motion == eAbeStates::State_8_Speak_45B160
        || field_106_current_motion == eAbeStates::State_9_Speak_45B180
        || field_106_current_motion == eAbeStates::State_10_Fart_45B1A0
        || field_106_current_motion == eAbeStates::State_99_LeverUse_455AC0
        || field_106_current_motion == eAbeStates::State_105_RockThrowStandingThrow_456460
        || field_106_current_motion == eAbeStates::State_104_RockThrowStandingHold_455DF0
        || field_106_current_motion == eAbeStates::State_106_RockThrowStandingEnd_455F20
        || field_106_current_motion == eAbeStates::State_112_Chant_45B1C0
        || field_106_current_motion == eAbeStates::State_113_ChantEnd_45BBE0;

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
    else if (state < eAbeStates::State_75_JumpIntoWell_45C7B0)
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
    else if (state < eAbeStates::State_100_SlapBomb_455B60)
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
    else if (state < eAbeStates::State_109_ZShotRolling_455550)
    {
        mapped = ResourceIndices::eThrow_5;
    }
    else if (state < eAbeStates::State_111_PickupItem_4564A0)
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
    else if (state < eAbeStates::State_119_ToShrykull_45A990)
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
        field_10_resources_array.SetAt(mapped, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, sAbeResourceIDTable_554D60[mapped], TRUE, FALSE));
    }

    field_128.field_10_resource_index = mapped;
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

static bool isEdgeGrabbable(Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->field_10_type == Path_Edge::Type::eLeft && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return true;
    }
    else if (pEdge->field_10_type == Path_Edge::Type::eRight && !pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return true;
    }
    else if (pEdge->field_10_type == Path_Edge::Type::eBoth)
    {
        return true;
    }

    return false;
}

void Abe::State_0_Idle_44EEB0()
{
    if (Input_IsChanting_45F260() && !(field_1AC_flags.Get(Flags_1AC::e1AC_Bit6_prevent_chanting)))
    {
        if (field_168_ring_pulse_timer && field_16C_bHaveShrykull)
        {
            field_106_current_motion = eAbeStates::State_119_ToShrykull_45A990;
            field_120_state = 0;
        }
        else
        {
            // Go to chanting.
            field_124_timer = sGnFrame_5C1B84 + 90;
            field_106_current_motion = eAbeStates::State_112_Chant_45B1C0;
            field_120_state = 0;
            SND_SEQ_PlaySeq_4CA960(SeqId::MudokonChant1_10, 0, 1);
        }
        return;
    }

    // Go to game speak state.
    const DWORD held = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (held & (eChant | eGameSpeak8 | eGameSpeak7 | eGameSpeak6 | eGameSpeak5 | eGameSpeak4 | eGameSpeak3 | eGameSpeak2 | eGameSpeak1))
    {
        field_118_prev_held = held;
        field_106_current_motion = eAbeStates::State_11_Speak_45B0A0;
        return;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Hop_5550E0))
    {
        // Some strange alternative way of hoisting, hangover from PSX AO Demo?
        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            TryHoist_44ED30();
        }
        else
        {
            field_106_current_motion = eAbeStates::State_27_HopBegin_4521C0;

            BaseGameObject* pObj = VIntoBirdPortal_408FD0(2);
            if (pObj)
            {
                field_1A4_portal_sub_state = 0;
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

    if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
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
                    field_106_current_motion = eAbeStates::State_121_LiftGrabBegin_45A600;
                    return;
                }
            }
        }

        // Look below for a down hoist
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos) + 16,
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos) + 16,
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
                field_106_current_motion = eAbeStates::State_66_LedgeDescend_454970;
            }
            else
            {
                if (pHoist->field_12_edge_type == Path_Hoist::EdgeType::eBoth)
                {
                    // We can hoist down from any side
                    field_106_current_motion = eAbeStates::State_66_LedgeDescend_454970;
                }
                else
                {
                    // Otherwise gotta turn around
                    field_108_next_motion = eAbeStates::State_66_LedgeDescend_454970;
                    field_106_current_motion = eAbeStates::State_2_StandingTurn_451830;
                }
            }
        }
        else
        {
            // Isn't a hoist so just crouch
            field_106_current_motion = eAbeStates::State_19_StandToCrouch_453DC0;
        }
        return;
    }

    if (sInputObject_5BD4E0.isHeld(sInputKey_FartRoll_5550F0))
    {
        // Do the fart sound
        Mudokon_SFX_457EC0(MudSounds::eFart_7, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eFart_3);

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
                field_106_current_motion = eAbeStates::State_10_Fart_45B1A0;
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

            New_Smoke_Particles_426C70(fartXPos, fartYPos, fartScale, 3, 32u, 128u, 32u);
        }

        field_106_current_motion = eAbeStates::State_10_Fart_45B1A0;
        return;
    }

    bool handleDoActionOrThrow = false;
    if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
    {
        // Check for lift rope.. (going up)
        BaseGameObject* pObj_field_110_2 = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj_field_110_2)
        {
            if (pObj_field_110_2->field_4_typeId == Types::eLiftPoint_78)
            {
                const FP halfGrid = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((field_100_pCollisionLine->field_0_rect.x + field_100_pCollisionLine->field_0_rect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = FP_Abs(field_B8_xpos - liftPlatformXMidPoint);
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    field_106_current_motion = eAbeStates::State_121_LiftGrabBegin_45A600;
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
            case TlvTypes::Door_5:
                if (NearDoorIsOpen_44EE10() && !field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
                {
                    field_FC_pPathTLV = pTlv;
                    field_120_state = 0;
                    field_106_current_motion = eAbeStates::State_114_DoorEnter_459470;
                }
                else
                {
                    // Check for pressed + held so that Abe does a dunno only once, otherwise he will
                    // loop the dunno while up button is down.
                    if (sInputObject_5BD4E0.isHeld(sInputKey_Up_5550D8))
                    {
                        field_106_current_motion = eAbeStates::State_34_DunnoBegin_44ECF0;
                    }
                }
                return;

            case TlvTypes::LocalWell_8:
            {
                if (field_114_flags.Get(Flags_114::e114_Bit10_Teleporting))
                {
                    break;
                }

                // Bail if scale doesn't match
                Path_Well_Local* pWell = static_cast<Path_Well_Local*>(pTlv);
                if ((pWell->field_0_scale != 0 || field_CC_sprite_scale != FP_FromDouble(1.0)) && (pWell->field_0_scale != 1 || field_CC_sprite_scale != FP_FromDouble(0.5)))
                {
                    break;
                }

                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_fall_to_well);
                field_FC_pPathTLV = pTlv;
                field_106_current_motion = eAbeStates::State_78_WellBegin_45C810;
            }
            return;

            case TlvTypes::WellExpress_23:
            {
                if (field_114_flags.Get(Flags_114::e114_Bit10_Teleporting))
                {
                    break;
                }

                // Bail if scale doesn't match
                Path_Well_Base* pWell = static_cast<Path_Well_Base*>(pTlv);
                if ((pWell->field_0_scale != 0 || field_CC_sprite_scale != FP_FromDouble(1.0)) && (pWell->field_0_scale != 1 || field_CC_sprite_scale != FP_FromDouble(0.5)))
                {
                    break;
                }

                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_fall_to_well);
                field_FC_pPathTLV = pTlv;
                field_106_current_motion = eAbeStates::jState_81_WellBegin_45C7F0;
            }
            return;

            case TlvTypes::MovieHandStone_27:
            case TlvTypes::HandStone_61:
                field_FC_pPathTLV = pTlv;
                field_106_current_motion = eAbeStates::State_86_HandstoneBegin_45BD00;
                field_120_state = 0;
                return;

            case TlvTypes::GrenadeMachine_59:
            {
                auto pMachineButton = static_cast<BoomMachine*>(FindObjectOfType_425180(
                    Types::eGrenadeMachine_66,
                    field_B8_xpos,
                    field_BC_ypos - field_CC_sprite_scale * FP_FromInteger(25)
                ));
                if (pMachineButton)
                {
                    pMachineButton->Vsub_445F00();
                    field_106_current_motion = eAbeStates::State_88_GrenadeMachineUse_45C510;
                }
                else
                {
                    field_106_current_motion = eAbeStates::State_34_DunnoBegin_44ECF0;
                }
            }
            break;

            case TlvTypes::WorkWheel_79:
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
                    field_106_current_motion = eAbeStates::State_126_TurnWheelBegin_456700;
                    BaseGameObject* pObj_148 = FindObjectOfType_425180(Types::eWheel_148, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(50)));
                    if (pObj_148)
                    {
                        field_164_wheel_id = pObj_148->field_8_object_id;
                    }
                }
            }
            break;

            case TlvTypes::FartMachine_101:
                field_106_current_motion = eAbeStates::State_89_BrewMachineBegin_4584C0;
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
            if (field_106_current_motion == eAbeStates::State_0_Idle_44EEB0)
            {
                TryHoist_44ED30();
            }
            handleDoActionOrThrow = true;
        }
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8) || handleDoActionOrThrow)
    {
        if ((sInputKey_ThrowItem_5550F4 & held) &&
            field_106_current_motion == eAbeStates::State_0_Idle_44EEB0)
        {
            if (field_1A2_throwable_count > 0 || gInfiniteGrenades_5C1BDE)
            {
                field_158_throwable_id = Make_Throwable_49AF30(
                    field_B8_xpos,
                    field_BC_ypos - FP_FromInteger(40),
                    0)->field_8_object_id;

                if (!bThrowableIndicatorExists_5C112C)
                {
                    auto pThrowable = ae_new<ThrowableTotalIndicator>();
                    if (pThrowable)
                    {
                        const FP xOffSet = ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ?
                            FP_FromInteger(15) : FP_FromInteger(-15)) * field_CC_sprite_scale;

                        pThrowable->ctor_431CB0(
                            field_B8_xpos + xOffSet,
                            field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-50)),
                            field_20_animation.field_C_render_layer,
                            field_20_animation.field_14_scale,
                            field_1A2_throwable_count,
                            TRUE
                        );
                    }
                }

                field_106_current_motion = eAbeStates::State_104_RockThrowStandingHold_455DF0;

                if (!gInfiniteGrenades_5C1BDE)
                {
                    field_1A2_throwable_count--;
                }
            }
            else
            {
                field_106_current_motion = eAbeStates::State_34_DunnoBegin_44ECF0;
            }
        }
        else
        {
            if (sInputObject_5BD4E0.isHeld(sInputKey_DoAction_5550E4)) // not throwing, maybe pressing up and pressing action, so do action
            {
                field_106_current_motion = HandleDoAction_455BD0();
            }
        }

    }
}

void Abe::State_1_WalkLoop_44FBA0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    MoveForward_44E9A0();

    if (field_106_current_motion != eAbeStates::State_1_WalkLoop_44FBA0)
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

        Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);

        MapFollowMe_408D10(TRUE);

        if (sInputKey_Run_5550E8 & pressed)
        {
            // Walk to run
            if (field_20_animation.field_92_current_frame == 5)
            {
                field_106_current_motion = eAbeStates::State_49_MidWalkToRun_450200;
            }
            else
            {
                field_106_current_motion = eAbeStates::State_48_WalkToRun_4500A0;
            }
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            // Walk to sneak
            if (field_20_animation.field_92_current_frame != 5)
            {
                field_106_current_motion = eAbeStates::State_43_MidWalkToSneak_450380;
            }
            else
            {
                field_106_current_motion = eAbeStates::State_41_WalkToSneak_450250;
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
        field_108_next_motion = eAbeStates::State_28_HopMid_451C50;
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
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), offX) ||
                WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), offX))
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
            field_106_current_motion = eAbeStates::State_5_MidWalkToIdle_450080;
        }
        else
        {
            field_106_current_motion = eAbeStates::State_4_WalkToIdle_44FFC0;
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
        field_106_current_motion = eAbeStates::State_61_TurnToRun_456530;
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
            Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_108_next_motion)
            {
                field_106_current_motion = field_108_next_motion;
                field_108_next_motion = 0;
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
        field_C4_velx -= (field_CC_sprite_scale * field_128.field_8_x_vel_slow_by);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx += (field_CC_sprite_scale * field_128.field_8_x_vel_slow_by);
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    const int bCollision = InAirCollision_408810(&pPathLine, &hitX, &hitY, FP_FromDouble(1.80));
    SetActiveCameraDelayedFromDir_408C40();

    // Are we falling into a local well?
    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::LocalWell_8);

    if (!field_FC_pPathTLV)
    {
        // No, are we falling into an express well?
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::WellExpress_23);
    }

    // Handle falling into a well
    if (field_FC_pPathTLV)
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            if (field_FC_pPathTLV->field_4_type == TlvTypes::LocalWell_8 || field_FC_pPathTLV->field_4_type == TlvTypes::WellExpress_23)
            {
                // The well must be on the same scale/layer
                Path_Well_Base* pWellBase = static_cast<Path_Well_Base*>(field_FC_pPathTLV);
                if ((pWellBase->field_0_scale == 0 && field_CC_sprite_scale == FP_FromInteger(1))
                    || (pWellBase->field_0_scale == 1 && field_CC_sprite_scale == FP_FromDouble(0.5)))
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_fall_to_well);
                    field_106_current_motion = eAbeStates::State_75_JumpIntoWell_45C7B0;
                    return;
                }
            }
        }
    }

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
            field_124_timer = sGnFrame_5C1B84 + 30;

            // See if there is a soft landing at our feet (given we known we just hit the floor)
            Path_SoftLanding* pSoftLanding =
                static_cast<Path_SoftLanding*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    TlvTypes::SoftLanding_75));

            if (pSoftLanding)
            {
                if (!SwitchStates_Get_466020(static_cast<short>(pSoftLanding->field_10_id)))
                {
                    pSoftLanding = nullptr;
                }
            }

            if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit7_land_softly)
                || (pSoftLanding && field_10C_health > FP_FromInteger(0)) // If we are dead soft landing won't save us
                || ((field_BC_ypos - field_F8_LastLineYPos) < (field_CC_sprite_scale * FP_FromInteger(180)) // Check we didn't fall far enough to be killed
                && (field_10C_health > FP_FromInteger(0) || gAbeBulletProof_5C1BDA))) //TODO possibly OG bug: those conditions should probably be grouped the following way: ((A || B || C ) && D)
            {
                field_106_current_motion = eAbeStates::State_16_LandSoft_45A360;
            }
            else
            {
                // Slam into the floor and break all your bones
                field_10C_health = FP_FromInteger(0);
                field_106_current_motion = eAbeStates::State_84_FallLandDie_45A420;
                field_128.field_4_regen_health_timer = sGnFrame_5C1B84 + 900;
            }

            field_F4_previous_motion = eAbeStates::State_3_Fall_459B60;

            PSX_Point xy{ FP_GetExponent(field_B8_xpos - FP_FromInteger(10)), FP_GetExponent(field_BC_ypos - FP_FromInteger(10)) };
            PSX_Point wh{ FP_GetExponent(field_B8_xpos + FP_FromInteger(10)), FP_GetExponent(field_BC_ypos + FP_FromInteger(10)) };
            vOnCollisionWith_424EE0(
                xy,
                wh,
                ObjList_5C1B78,
                1,
                reinterpret_cast<TCollisionCallBack>(&BaseAliveGameObject::OnTrapDoorIntersection_408BA0)
            ); // Danger danger.. but will probably work.. can't see how else they would have got this to work
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
        TlvTypes::Edge_3
    ));

    bool tryToHang = false;
    if (pEdge)
    {
        if (pEdge->field_12_can_grab && IsSameScaleAsEdge(pEdge, this) &&
            (isEdgeGrabbable(pEdge, this)))
        {
            tryToHang = true;
        }
        field_FC_pPathTLV = pEdge;
    }
    else // Didn't find and edge to grab so check if falling onto a hoist
    {
        // Look down 20 for a hoist
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - field_CC_sprite_scale * FP_FromInteger(20)),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos - field_CC_sprite_scale * FP_FromInteger(20)),
            TlvTypes::Hoist_2));

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
            FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y -10), // TODO: Negative ??
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
        if (field_FC_pPathTLV->field_4_type != TlvTypes::Hoist_2 ||
            (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y)) >= (field_CC_sprite_scale * FP_FromInteger(70)))
        {
            field_106_current_motion = eAbeStates::State_69_LedgeHangWobble_454EF0;
            field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
        }
        else
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
            field_F4_previous_motion = eAbeStates::State_65_LedgeAscend_4548E0;
            field_F6_anim_frame = 12;
            field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
        }
    }
}

void Abe::State_4_WalkToIdle_44FFC0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    MoveForward_44E9A0();

    if (field_20_animation.field_92_current_frame)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_408D10(TRUE);

            if (field_108_next_motion == eAbeStates::State_28_HopMid_451C50)
            {
                field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
                field_106_current_motion = eAbeStates::State_27_HopBegin_4521C0;
                BaseGameObject* pObj = VIntoBirdPortal_408FD0(2);
                if (pObj)
                {
                    field_1A4_portal_sub_state = 0;
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

void Abe::State_5_MidWalkToIdle_450080()
{
    // TODO Jump func omitted jState_WalkEndLeftFoot_40459
    State_4_WalkToIdle_44FFC0();
}

void Abe::State_6_WalkBegin_44FEE0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    // They hear me walking, they hating
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_1_WalkLoop_44FBA0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
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

void Abe::State_9_Speak_45B180()
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
        field_106_current_motion = DoGameSpeak_45AB70(field_118_prev_held);
        if (field_106_current_motion == -1)
        {
            ToIdle_44E6B0();
            field_118_prev_held = 0;
        }
        else
        {
            if (field_106_current_motion != eAbeStates::State_62_Punch_454750)
            {
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }

            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
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
        field_F8_LastLineYPos = field_BC_ypos;
        const FP velY = field_CC_sprite_scale * FP_FromInteger(-8);
        field_C8_vely = velY;
        field_BC_ypos += velY;
        VOnTrapDoorOpen();
        field_106_current_motion = eAbeStates::State_14_HoistIdle_452440;
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
    const auto bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir_408C40();

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

                field_106_current_motion = eAbeStates::State_15_HoistLand_452BA0;
                field_F4_previous_motion = eAbeStates::State_14_HoistIdle_452440;

                vOnCollisionWith_424EE0(
                    { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos) },
                    { FP_GetExponent(field_B8_xpos), FP_GetExponent((field_BC_ypos + FP_FromInteger(5))) },
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0
                );
                break;
            }

        }
        return;

    }

    PullRingRope* pPullRope = GetPullRope_44D120();
    if (pPullRope)
    {
        if (pPullRope->VPull_49BBD0(this))
        {
            field_106_current_motion = eAbeStates::State_70_RingRopePullHang_455AF0;
            field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
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
        TlvTypes::Hoist_2));
    field_FC_pPathTLV = pHoist;

    if (field_C8_vely < FP_FromInteger(0))
    {
        // Going down and no hoist, can't do anything
        if (!pHoist)
        {
            return;
        }

        // Hoist is too far away
        if (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y + -field_FC_pPathTLV->field_8_top_left.field_2_y) >
            (field_CC_sprite_scale * FP_FromInteger(90)) || field_20_animation.field_92_current_frame)
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
                if (gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapTop_2, this, -1))
                {
                    PSX_Prevent_Rendering_4945B0();
                    field_106_current_motion = eAbeStates::State_68_ToOffScreenHoist_454B80;
                    return;
                }

                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);

                if (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y)
                    >= field_CC_sprite_scale * FP_FromInteger(70))
                {
                    field_106_current_motion = eAbeStates::State_67_LedgeHang_454E20;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                    field_F4_previous_motion = eAbeStates::State_65_LedgeAscend_4548E0;
                    field_F6_anim_frame = 12;
                }
                field_BC_ypos -= field_CC_sprite_scale * FP_FromInteger(75);
                field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
            }
            else
            {
                Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
                if (FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y) >=
                    field_CC_sprite_scale * FP_FromInteger(70))
                {
                    field_106_current_motion = eAbeStates::State_67_LedgeHang_454E20;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                    field_F4_previous_motion = eAbeStates::State_65_LedgeAscend_4548E0;
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
                field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
            }
            else
            {
                field_106_current_motion = eAbeStates::State_14_HoistIdle_452440;
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
        if (field_F4_previous_motion == eAbeStates::State_3_Fall_459B60)
        {
            Environment_SFX_457A40(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
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
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit7_land_softly)))
        {
            // Hitting the floor events.
            Event_Broadcast_422BC0(kEventNoise, this);
            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
        }

        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit7_land_softly);

        // Hitting the floor sounds.
        if (field_F4_previous_motion == eAbeStates::State_3_Fall_459B60)
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
            field_106_current_motion = eAbeStates::State_112_Chant_45B1C0;
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
        field_106_current_motion = eAbeStates::State_98_RollOffEdge_455AA0;
        return;
    }

    field_118_prev_held = 0;
    field_11C_released_buttons = 0;

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
        && field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0
        && (field_1A2_throwable_count > 0 || gInfiniteGrenades_5C1BDE))
    {
        field_158_throwable_id = Make_Throwable_49AF30(field_B8_xpos, field_BC_ypos - FP_FromInteger(40), 0)->field_8_object_id;
        if (!bThrowableIndicatorExists_5C112C)
        {
            auto pRockCountGraphic = ae_new<ThrowableTotalIndicator>();
            if (pRockCountGraphic)
            {
                const FP yOff = field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-30));
                const FP xOff = field_CC_sprite_scale * (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-10) : FP_FromInteger(10));
                pRockCountGraphic->ctor_431CB0(
                    field_B8_xpos + xOff,
                    yOff,
                    field_20_animation.field_C_render_layer,
                    field_20_animation.field_14_scale,
                    field_1A2_throwable_count,
                    1);
            }
        }

        field_106_current_motion = eAbeStates::State_107_RockThrowCrouchingHold_454410;

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
            field_106_current_motion = eAbeStates::State_18_CrouchToStand_454600;
            return;
        }

        // Crouching farts
        if (sInputKey_FartRoll_5550F0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eFart_3);

            Mudokon_SFX_457EC0(MudSounds::eFart_7, 0, 0, this);

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
                New_Smoke_Particles_426C70(xpos, ypos, scale, 3, 32u, 128u, 32u);
            }

            field_106_current_motion = eAbeStates::State_20_CrouchSpeak_454550;
        }
        else
        {
            // Crouching turns
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Right_5550D0)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_motion = eAbeStates::State_37_CrouchTurn_454390;
                }
                else
                {
                    field_106_current_motion = eAbeStates::State_22_RollBegin_4539A0;
                    field_11C_released_buttons = 0;
                }
            }

            if (sInputKey_Left_5550D4 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_106_current_motion = eAbeStates::State_22_RollBegin_4539A0;
                    field_11C_released_buttons = 0;
                }
                else
                {
                    field_106_current_motion = eAbeStates::State_37_CrouchTurn_454390;
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
        field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
    }
}

// TODO: End crouching game speak/fart?
void Abe::State_20_CrouchSpeak_454550()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;

        CrouchingGameSpeak_453E10();

        if (field_106_current_motion == eAbeStates::State_20_CrouchSpeak_454550 || field_106_current_motion == eAbeStates::jState_21_ToCrouchSpeak_4545E0)
        {
            Event_Broadcast_422BC0(kEventSpeaking, this);
        }
        field_118_prev_held = 0;
    }
}

void Abe::jState_21_ToCrouchSpeak_4545E0()
{
    // Note j_401389 omitted
    State_20_CrouchSpeak_454550();
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
    if (WallHit_408750(xpos, field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
        field_106_current_motion = eAbeStates::State_74_RollingKnockback_455290;
    }
    else
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eAbeStates::State_23_RollLoop_453A90;
        }
        MoveForward_44E9A0();
    }
}

void Abe::State_23_RollLoop_453A90()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    field_11C_released_buttons |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_10_released;

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
        field_106_current_motion = eAbeStates::State_74_RollingKnockback_455290;
    }
    else
    {
        MoveForward_44E9A0();

        if (field_106_current_motion == eAbeStates::State_23_RollLoop_453A90)
        {
            if (field_20_animation.field_92_current_frame == 1 || field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 9)
            {
                if (!sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8)
                    || sInputObject_5BD4E0.isPressed(sInputKey_FartRoll_5550F0)
                    || Is_Celling_Above_44E8D0()
                    || field_128.field_14_rolling_motion_timer + 9 >= static_cast<int>(sGnFrame_5C1B84))
                {
                    if (field_11C_released_buttons)
                    {
                        if (!Is_Celling_Above_44E8D0() && field_128.field_14_rolling_motion_timer + 9 < static_cast<int>(sGnFrame_5C1B84))
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
            else if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 8)
            {
                MapFollowMe_408D10(TRUE);

                if (field_C4_velx > FP_FromInteger(0) && !sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0) ||
                    field_C4_velx < FP_FromInteger(0) && !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                {
                    field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
                    field_C4_velx = FP_FromInteger(0);
                }
            }

            if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 6)
            {
                Environment_SFX_457A40(EnvironmentSfx::eRollingNoise_8, 0, 32767, this);
            }
        }
    }
}

//TODO: probably unused?
void Abe::State_24_453D00()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
        field_106_current_motion = eAbeStates::State_74_RollingKnockback_455290;
    }
    else
    {
        MoveForward_44E9A0();
        if (field_106_current_motion == eAbeStates::State_24_453D00)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(TRUE);
                field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Abe::State_25_RunSlideStop_451330()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));

        if (field_106_current_motion == eAbeStates::State_25_RunSlideStop_451330 && !RunTryEnterWell_451060() && !RunTryEnterDoor_451220())
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
                    Environment_SFX_457A40(EnvironmentSfx::eSlideStop_0, 0, 32767, this);
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
                field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                field_F4_previous_motion = eAbeStates::State_26_RunTurn_451500;
                field_F6_anim_frame = field_20_animation.field_92_current_frame;
            }
        }
    }
}

void Abe::State_26_RunTurn_451500()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));
        if (field_106_current_motion == eAbeStates::State_26_RunTurn_451500
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
                        field_106_current_motion = eAbeStates::State_52_RunTurnToRun_451710;
                    }
                    else
                    {
                        field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
                        field_106_current_motion = eAbeStates::State_53_RunTurnToWalk_451800;
                    }
                }
                else
                {
                    if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                    {
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                        field_106_current_motion = eAbeStates::State_52_RunTurnToRun_451710;
                    }
                    else
                    {
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                        field_106_current_motion = eAbeStates::State_53_RunTurnToWalk_451800;
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
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
                WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
            {
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                field_C4_velx = FP_FromInteger(0);
                ToKnockback_44E700(1, 1);
                return;
            }
        }
        field_B8_xpos += field_C4_velx;
        SetActiveCameraDelayedFromDir_408C40();
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F8_LastLineYPos = field_BC_ypos;
        const FP velX = field_CC_sprite_scale * (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        field_C4_velx = velX;
        field_B8_xpos += velX;

        const FP velY = field_CC_sprite_scale * FP_FromDouble(-2.7);
        field_C8_vely = velY;
        field_BC_ypos += velY;

        VOnTrapDoorOpen();

        field_106_current_motion = eAbeStates::State_28_HopMid_451C50;
        field_100_pCollisionLine = nullptr;

        if (field_1A8_portal_id == -1)
        {
            BaseGameObject* pObj = VIntoBirdPortal_408FD0(2);
            if (pObj)
            {
                field_1A4_portal_sub_state = 0;
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
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
        field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    // See if we've hit a path line floor type
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.80));
    SetActiveCameraDelayedFromDir_408C40();

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
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
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
            field_106_current_motion = eAbeStates::State_29_HopLand_4523D0;
            field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
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
            TlvTypes::Edge_3));

        field_FC_pPathTLV = pEdgeTlv;

        if (pEdgeTlv && pEdgeTlv->field_12_can_grab && IsSameScaleAsEdge(pEdgeTlv, this) &&
            ((isEdgeGrabbable(pEdgeTlv, this) && field_C4_velx != FP_FromInteger(0))))
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
                field_106_current_motion = eAbeStates::State_69_LedgeHangWobble_454EF0;
                field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
            }
        }
    }

    if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return;
    }

    if (field_106_current_motion != eAbeStates::State_28_HopMid_451C50)
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

    field_128.field_8_x_vel_slow_by = FP_FromDouble(0.55);
    field_106_current_motion = eAbeStates::State_96_HopToFall_4559E0;
    field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
}

void Abe::State_29_HopLand_4523D0()
{
    if (field_20_animation.field_92_current_frame == 2
        && sInputKey_Hop_5550E0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        field_F4_previous_motion = eAbeStates::State_27_HopBegin_4521C0;
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
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(FP_FromInteger(10), field_C4_velx)) // TODO: OG bug, why no scaling?
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        field_B8_xpos += field_C4_velx;

        if (field_20_animation.field_92_current_frame == 0)
        {
            Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_F8_LastLineYPos = field_BC_ypos;

            const FP velX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-7.6) : FP_FromDouble(7.6);
            field_C4_velx = field_CC_sprite_scale * velX;

            const FP velY = field_CC_sprite_scale * FP_FromDouble(-9.6);
            field_C8_vely = velY;
            field_BC_ypos += velY;

            VOnTrapDoorOpen();

            field_106_current_motion = eAbeStates::State_31_RunJumpMid_452C10;
            field_100_pCollisionLine = nullptr;
        }
    }

}

void Abe::State_31_RunJumpMid_452C10()
{
    BaseGameObject* pfield_110_id = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    if (field_1A8_portal_id != -1)
    {
        IntoPortalStates_451990();
        return;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(FP_FromInteger(10), field_C4_velx) ||
        Is_Celling_Above_44E8D0())
    {
        field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const auto bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.80));

    SetActiveCameraDelayedFromDir_408C40();

    if (bCollision)
    {
        switch (pLine->field_8_type)
        {
        case eLineTypes::eFloor_0:
        case eLineTypes::eBackGroundFloor_4:
        case 32u:
        case 36u:
            field_100_pCollisionLine = pLine;
            field_106_current_motion = eAbeStates::State_32_RunJumpLand_453460;
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
            field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
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
            TlvTypes::Hoist_2));

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
                TlvTypes::Edge_3));

            field_FC_pPathTLV = pEdgeTlv;

            if (pEdgeTlv && pEdgeTlv->field_12_can_grab)
            {
                if (IsSameScaleAsEdge(pEdgeTlv, this) && (isEdgeGrabbable(pEdgeTlv, this)))
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
                field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
                field_100_pCollisionLine = pLine;
                field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);

                if (field_FC_pPathTLV->field_4_type != TlvTypes::Hoist_2 ||
                    FP_FromInteger(field_FC_pPathTLV->field_C_bottom_right.field_2_y - 1 * field_FC_pPathTLV->field_8_top_left.field_2_y) >=
                    field_CC_sprite_scale * FP_FromInteger(70))
                {
                    field_106_current_motion = eAbeStates::State_69_LedgeHangWobble_454EF0;
                }
                else
                {
                    field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
                    field_F4_previous_motion = eAbeStates::State_65_LedgeAscend_4548E0;
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

    if (field_106_current_motion != eAbeStates::State_31_RunJumpMid_452C10)
    {
        return;
    }

    field_128.field_8_x_vel_slow_by = FP_FromDouble(0.75);
    field_106_current_motion = eAbeStates::State_97_RunJumpToFall_455A80;
    field_108_next_motion = eAbeStates::State_0_Idle_44EEB0;
}

void Abe::State_32_RunJumpLand_453460()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);

        MapFollowMe_408D10(TRUE);

        const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
        if (sInputKey_Left_5550D4 & pressed)
        {
            if (sInputKey_Hop_5550E0 & field_118_prev_held)
            {
                BaseGameObject* pPortal = VIntoBirdPortal_408FD0(3);
                if (pPortal)
                {
                    field_1A4_portal_sub_state = 0;
                    field_1A8_portal_id = pPortal->field_8_object_id;
                    field_106_current_motion = eAbeStates::State_30_RunJumpBegin_4532E0;
                    field_118_prev_held = 0;
                    return;
                }
                field_106_current_motion = eAbeStates::State_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (pressed & sInputKey_Run_5550E8)
                {
                    field_106_current_motion = eAbeStates::State_54_RunJumpLandRun_4538F0;
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                }
                else
                {
                    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), -ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                    {
                        field_106_current_motion = eAbeStates::State_25_RunSlideStop_451330;
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                    }
                    else
                    {
                        field_106_current_motion = eAbeStates::State_55_RunJumpLandWalk_453970;
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                    }
                }
            }
            else
            {
                field_106_current_motion = eAbeStates::State_26_RunTurn_451500;
                field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            }
        }
        else if (pressed & sInputKey_Right_5550D0)
        {
            if (sInputKey_Hop_5550E0 & field_118_prev_held)
            {
                BaseGameObject* pPortal = VIntoBirdPortal_408FD0(3);
                if (pPortal)
                {
                    field_1A4_portal_sub_state = 0;
                    field_1A8_portal_id = pPortal->field_8_object_id;
                }
                field_106_current_motion = eAbeStates::State_30_RunJumpBegin_4532E0;
                field_118_prev_held = 0;
                return;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_106_current_motion = eAbeStates::State_26_RunTurn_451500;
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
            }
            else if (pressed & sInputKey_Run_5550E8)
            {
                field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                field_106_current_motion = eAbeStates::State_54_RunJumpLandRun_4538F0;
            }
            else
            {
                if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                {
                    field_106_current_motion = eAbeStates::State_25_RunSlideStop_451330;
                    field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                }
                else
                {
                    field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9);
                    field_106_current_motion = eAbeStates::State_55_RunJumpLandWalk_453970;
                }
            }
        }
        else if (sInputKey_Hop_5550E0 & field_118_prev_held)
        {
            BaseGameObject* pPortal = VIntoBirdPortal_408FD0(2);
            if (pPortal)
            {
                field_1A4_portal_sub_state = 0;
                field_1A8_portal_id = pPortal->field_8_object_id;
            }
            field_106_current_motion = eAbeStates::State_27_HopBegin_4521C0;
            field_118_prev_held = 0;
        }
        else
        {
            field_106_current_motion = eAbeStates::State_25_RunSlideStop_451330;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
            }
            Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
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
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
        ToKnockback_44E700(1, 1);
        return;
    }

    MoveForward_44E9A0();

    if (field_106_current_motion != eAbeStates::State_33_RunLoop_4508E0)
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
        return;
    }

    if (field_20_animation.field_92_current_frame != 0  && field_20_animation.field_92_current_frame != 8)
    {
        if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 12)
        {
            Environment_SFX_457A40(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);

            MapFollowMe_408D10(TRUE);

            // Turning around?
            if (field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4) ||
                field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
                field_106_current_motion = eAbeStates::State_26_RunTurn_451500;
                Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                field_118_prev_held = 0;
                return;
            }

            if (!(field_118_prev_held & sInputKey_Hop_5550E0))
            {
                // Run to roll
                if (field_118_prev_held & sInputKey_FartRoll_5550F0)
                {
                    field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
                    field_106_current_motion = eAbeStates::jState_38_RunToRoll_453A70;
                    field_11C_released_buttons = 0;
                    field_118_prev_held = 0;
                    return;
                }

                if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4) ||
                    sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
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
                    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize) ||
                        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), gridSize))
                    {
                        ToKnockback_44E700(1, 1);
                    }
                    else
                    {
                        // Run to walk
                        if (field_20_animation.field_92_current_frame != 4)
                        {
                            field_106_current_motion = eAbeStates::State_50_RunToWalk_450E20;
                        }
                        else
                        {
                            field_106_current_motion = eAbeStates::State_51_MidRunToWalk_450F50;
                        }
                    }
                }
                else
                {
                    // No longer running or even moving, so slide stop
                    field_106_current_motion = eAbeStates::State_25_RunSlideStop_451330;
                    Environment_SFX_457A40(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
                }

                field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
                field_118_prev_held = 0;
            }
        }
    }
    else
    {
        MapFollowMe_408D10(TRUE);
    }

    // Run jump
    if (field_118_prev_held & sInputKey_Hop_5550E0)
    {
        BaseGameObject* pObj = VIntoBirdPortal_408FD0(3);
        if (pObj)
        {
            field_1A4_portal_sub_state = 0;
            field_1A8_portal_id = pObj->field_8_object_id;
        }

        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit14_unused);
        field_106_current_motion = eAbeStates::State_30_RunJumpBegin_4532E0;
        field_118_prev_held = 0;
    }
}

void Abe::State_34_DunnoBegin_44ECF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_35_DunnoEnd_44ED10;
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
            if (field_108_next_motion)
            {
                field_106_current_motion = field_108_next_motion;
                field_108_next_motion = 0;
            }
            else
            {
                field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
            }
        }
    }
    else
    {
        Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
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
        field_106_current_motion = eAbeStates::State_33_RunLoop_4508E0;
    }

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::State_40_SneakLoop_450550()
{
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        MoveForward_44E9A0();

        if (field_106_current_motion == eAbeStates::State_40_SneakLoop_450550)
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
                if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize) ||
                    WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), gridSize)
                    || (field_C4_velx > FP_FromInteger(0) && (sInputKey_Left_5550D4 & pressed))
                    || (field_C4_velx < FP_FromInteger(0) && (sInputKey_Right_5550D0 & pressed))
                    || !((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed))
                {
                    if (field_20_animation.field_92_current_frame != 3)
                    {
                        field_106_current_motion = eAbeStates::State_46_SneakToIdle_450870;
                    }
                    else
                    {
                        field_106_current_motion = eAbeStates::jState_47_MidSneakToIdle_4508C0;
                    }
                }
            }
            else if (field_20_animation.field_92_current_frame == 6 || field_20_animation.field_92_current_frame == 16)
            {
                Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe_408D10(TRUE);
                if ((sInputKey_Left_5550D4 | sInputKey_Right_5550D0) & pressed)
                {
                    // Sneak to walk
                    if (!(pressed & sInputKey_Sneak_5550EC))
                    {
                        if (field_20_animation.field_92_current_frame != 6)
                        {
                            field_106_current_motion = eAbeStates::State_44_MidSneakToWalk_450500;
                        }
                        else
                        {
                            field_106_current_motion = eAbeStates::State_42_SneakToWalk_4503D0;
                        }
                    }
                }
                field_118_prev_held = 0;
            }
        }
    }
}

void Abe::State_41_WalkToSneak_450250()
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
        field_106_current_motion = eAbeStates::State_40_SneakLoop_450550;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::State_42_SneakToWalk_4503D0()
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
        field_106_current_motion = eAbeStates::State_1_WalkLoop_44FBA0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::State_43_MidWalkToSneak_450380()
{
    State_41_WalkToSneak_450250();

    if (field_106_current_motion == eAbeStates::State_40_SneakLoop_450550)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        field_106_current_motion = eAbeStates::State_43_MidWalkToSneak_450380;
        field_F4_previous_motion = eAbeStates::State_40_SneakLoop_450550;
        field_F6_anim_frame = 10;
    }
}

void Abe::State_44_MidSneakToWalk_450500()
{
    State_42_SneakToWalk_4503D0();

    if (field_106_current_motion == eAbeStates::State_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        field_106_current_motion = eAbeStates::State_44_MidSneakToWalk_450500;
        field_F4_previous_motion = eAbeStates::State_1_WalkLoop_44FBA0;
        field_F6_anim_frame = 9;
    }
}

void Abe::State_45_SneakBegin_4507A0()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_40_SneakLoop_450550;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::State_46_SneakToIdle_450870()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
    }

    MoveForward_44E9A0();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_408D10(TRUE);
        ToIdle_44E6B0();
    }
}

void Abe::jState_47_MidSneakToIdle_4508C0()
{
    // TODO: Skipped jmp func jAbe::State_SneakEnd_40330F
    State_46_SneakToIdle_450870();
}

void Abe::State_48_WalkToRun_4500A0()
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
        field_106_current_motion = eAbeStates::State_33_RunLoop_4508E0;
        field_118_prev_held = 0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
        MapFollowMe_408D10(TRUE);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::State_49_MidWalkToRun_450200()
{
    State_48_WalkToRun_4500A0();

    if (field_106_current_motion == eAbeStates::State_33_RunLoop_4508E0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        field_106_current_motion = eAbeStates::State_49_MidWalkToRun_450200;
        field_F4_previous_motion = eAbeStates::State_33_RunLoop_4508E0;
        field_F6_anim_frame = 8;
    }
}

void Abe::State_50_RunToWalk_450E20()
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
        field_106_current_motion = eAbeStates::State_1_WalkLoop_44FBA0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::State_51_MidRunToWalk_450F50()
{
    State_50_RunToWalk_450E20();

    if (field_106_current_motion == eAbeStates::State_1_WalkLoop_44FBA0)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit2_return_to_previous_motion);
        field_106_current_motion = eAbeStates::State_51_MidRunToWalk_450F50;
        field_F4_previous_motion = eAbeStates::State_1_WalkLoop_44FBA0;
        field_F6_anim_frame = 9;
    }
}

void Abe::State_52_RunTurnToRun_451710()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eAbeStates::State_33_RunLoop_4508E0;
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Abe::State_53_RunTurnToWalk_451800()
{
    State_52_RunTurnToRun_451710();

    if (field_106_current_motion == eAbeStates::State_33_RunLoop_4508E0)
    {
        field_106_current_motion = eAbeStates::State_1_WalkLoop_44FBA0;
    }
}

void Abe::State_54_RunJumpLandRun_4538F0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eAbeStates::State_33_RunLoop_4508E0;
        }
    }
}

void Abe::State_55_RunJumpLandWalk_453970()
{
    State_54_RunJumpLandRun_4538F0();

    if (field_106_current_motion == eAbeStates::State_33_RunLoop_4508E0)
    {
        field_106_current_motion = eAbeStates::State_1_WalkLoop_44FBA0;
    }
}

void Abe::State_56_DeathDropFall_4591F0()
{
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    if (field_124_timer == 0)
    {
        field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
        field_128.field_C_unused = 0;
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 90;
        field_124_timer++;
    }
    else if (field_124_timer == 1)
    {
        if (static_cast<int>(sGnFrame_5C1B84) == field_128.field_0_gnFrame - 30)
        {
            SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        }
        else if (static_cast<int>(sGnFrame_5C1B84) == field_128.field_0_gnFrame - 24)
        {
            SFX_Play_46FA90(SoundEffect::KillEffect_64, 85);
            auto pShake = ae_new<ScreenShake>();
            if (pShake)
            {
                pShake->ctor_4ACF70(1, 0);
            }
        }
        else if (static_cast<int>(sGnFrame_5C1B84) >= field_128.field_0_gnFrame)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::State_57_Dead_4589A0()
{
    DeathFadeOut* pDeathFade_1 = static_cast<DeathFadeOut*>(sObjectIds_5C1B70.Find_449CF0(field_148_fade_obj_id));
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds_5C1B70.Find_449CF0(field_14C_circular_fade_id));

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);

    if (field_110_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (!pObj)
        {
            field_110_id = -1;
        }
        else if (pObj->field_4_typeId == Types::eLiftPoint_78)
        {
            static_cast<LiftPoint*>(pObj)->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        }
    }

    switch (field_124_timer)
    {
    case 0:
        Event_Broadcast_422BC0(kEventShooting | kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventHeroDying, this);
        field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
        field_128.field_C_unused = 0;
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        field_118_prev_held = 0;
        field_124_timer = field_124_timer + 1;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 30;
        if (field_FC_pPathTLV && field_FC_pPathTLV->field_4_type == TlvTypes::DeathDrop_4)
        {
            auto pBird = ae_new<DeathBirdParticle>();
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
            auto pBird = ae_new<DeathBirdParticle>();
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
            if (field_FC_pPathTLV && field_FC_pPathTLV->field_4_type == TlvTypes::DeathDrop_4)
            {
                auto pBird = ae_new<DeathBirdParticle>();
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
                auto pBird = ae_new<DeathBirdParticle>();
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

        if (static_cast<int>(sGnFrame_5C1B84) > field_128.field_0_gnFrame)
        {
            field_128.field_0_gnFrame = sGnFrame_5C1B84 + 60;

            if (field_FC_pPathTLV)
            {
                if (field_FC_pPathTLV->field_4_type == TlvTypes::DeathDrop_4)
                {
                    field_128.field_0_gnFrame = (sGnFrame_5C1B84 + 60) + 45;
                }
            }
            ++field_124_timer;
            MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
        }
        return;
    case 2:
        Event_Broadcast_422BC0(kEventHeroDying, this);
        if (static_cast<int>(sGnFrame_5C1B84) > field_128.field_0_gnFrame)
        {
            ++field_124_timer;
        }
        return;
    case 3:
    {
        Event_Broadcast_422BC0(kEventHeroDying, this);
        if (pDeathFade_1)
        {
            pDeathFade_1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_148_fade_obj_id = -1;
        }
        auto pDeathFade = ae_new<DeathFadeOut>();
        if (pDeathFade)
        {
            pDeathFade->ctor_427030(40, 1, 0, 8, 2);
            field_148_fade_obj_id = pDeathFade->field_8_object_id;
        }

        if (pCircularFade)
        {
            pCircularFade->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        ++field_124_timer;
    }
    return;

    case 4:
        Event_Broadcast_422BC0(kEventHeroDying, this);

        if (!pDeathFade_1->field_7E_bDone)
        {
            return;
        }

        if (field_10A_unused)
        {
            return;
        }

        VOnTrapDoorOpen();

        field_100_pCollisionLine = nullptr;
        field_128.field_0_gnFrame = sGnFrame_5C1B84 + 8;
        ++field_124_timer;
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
            sActiveQuicksaveData_BAF7F8.field_204_world_info.field_10_controlled_scale != 0 ? FP_FromDouble(1.0) : FP_FromDouble(0.5),
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
        field_106_current_motion = eAbeStates::State_57_Dead_4589A0;
        field_124_timer = 2;
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

//TODO: probably unused?
void Abe::State_60_4A3200()
{
    NOT_IMPLEMENTED();
}

void Abe::State_61_TurnToRun_456530()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        // Was going to run into something
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveForward_44E9A0();

        if (field_106_current_motion == eAbeStates::State_61_TurnToRun_456530)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_motion = eAbeStates::State_33_RunLoop_4508E0;
            }
        }
    }
}

void Abe::State_62_Punch_454750()
{
    if (field_20_animation.field_92_current_frame == 5)
    {
        FP gridSize = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
        }

        const FP kFP5 = FP_FromInteger(5);
        BaseGameObject* pSlapTarget = FindObjectOfType_425180(Types::eMudokon_110, gridSize, field_BC_ypos - kFP5);
        while (pSlapTarget)
        {
            // Is it in a state where we can slap it?
            const __int16 mud_state = static_cast<Mudokon*>(pSlapTarget)->field_106_current_motion;
            if (mud_state != 46 && mud_state != 47)
            {
                // Can slap, so exit
                break;
            }

            // Try to get the next "stacked" mud - e.g if we have like 20 muds on 1 grid block this will iterate through them
            pSlapTarget = GetStackedSlapTarget_425290(pSlapTarget->field_8_object_id, Types::eMudokon_110, gridSize, field_BC_ypos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType_425180(Types::eLockedSoul_61, gridSize, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale));
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType_425180(Types::eSlig_125, gridSize, field_BC_ypos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType_425180(Types::eSlig_125, field_B8_xpos, field_BC_ypos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType_425180(Types::eGlukkon_67, gridSize, field_BC_ypos - kFP5);
        }

        if (pSlapTarget)
        {
            static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage_408730(this);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_63_Sorry_454670()
{
    if (field_20_animation.field_92_current_frame == 4)
    {
        FP yOff = {};
        FP xOff = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            yOff = field_BC_ypos - FP_FromInteger(5);
            xOff = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            yOff = field_BC_ypos - FP_FromInteger(5);
            xOff = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
        }

        auto pMud = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eMudokon_110, xOff, yOff));
        if (pMud)
        {
            pMud->VTakeDamage_408730(this);
        }

        Mudokon_SFX_457EC0(MudSounds::eSorry_27, 0, 0, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_64_AfterSorry_454730;
    }
}

void Abe::State_64_AfterSorry_454730()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_65_LedgeAscend_4548E0()
{
    const __int16 curFrameNum = field_20_animation.field_92_current_frame;
    if (curFrameNum == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 32767, this);
    }
    else if (curFrameNum == 4)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        field_E0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Now the ascend is done go back to stand idle
        field_E0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
        MapFollowMe_408D10(TRUE);
        ToIdle_44E6B0();
    }
}

void Abe::State_66_LedgeDescend_454970()
{
    const __int16 curFrameNum = field_20_animation.field_92_current_frame;
    if (curFrameNum == 2)
    {
        Environment_SFX_457A40(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
    }
    else if (curFrameNum == 21)
    {
        Path_Hoist* pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos + FP_FromInteger(16)),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos + FP_FromInteger(16)),
            TlvTypes::Hoist_2));

        if (pHoist)
        {
            if (FP_FromInteger((pHoist->field_C_bottom_right.field_2_y - 1 * pHoist->field_8_top_left.field_2_y)) <
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
                    field_106_current_motion = eAbeStates::State_16_LandSoft_45A360;
                    field_E0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
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
        field_106_current_motion = eAbeStates::State_67_LedgeHang_454E20;
    }
}

void Abe::State_67_LedgeHang_454E20()
{
    field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);
    const int pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed || field_114_flags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        field_106_current_motion = eAbeStates::State_65_LedgeAscend_4548E0;
    }
    else if (pressed & sInputKey_Down_5550DC)
    {
        VOnTrapDoorOpen();
        field_100_pCollisionLine = nullptr;
        field_106_current_motion = eAbeStates::State_91_FallingFromGrab_4557B0;
        field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(75);
        field_E0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
        field_F8_LastLineYPos = field_BC_ypos;
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
        TlvTypes::Hoist_2);

    // Find the hoist we are "connecting" to
    field_FC_pPathTLV = pHoist;
    field_BC_ypos -= field_CC_sprite_scale * FP_FromInteger(75);
    field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit1_ShadowAtBottom);

    const FP ypos = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y) - (FP_FromInteger(40) * field_CC_sprite_scale);
    pHoist = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(ypos),
        TlvTypes::Hoist_2);

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
                { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos) },
                { FP_GetExponent(field_B8_xpos), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        field_106_current_motion = eAbeStates::State_67_LedgeHang_454E20;
    }
    else
    {
        field_106_current_motion = eAbeStates::State_14_HoistIdle_452440;
    }
}

void Abe::State_69_LedgeHangWobble_454EF0()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        if (!(field_1AC_flags.Get(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds)))
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
            Environment_SFX_457A40(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
        }
    }
    else
    {
        if (field_20_animation.field_92_current_frame == 2)
        {
            if (!(field_1AC_flags.Get(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds)))
            {
                field_1AC_flags.Set(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
                Mudokon_SFX_457EC0(MudSounds::eHurt1_16, 45, -200, this);
            }
        }
        else
        {
            field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        }
    }

    // Going up the ledge on wobble?
    const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (sInputKey_Up_5550D8 & pressed || field_114_flags.Get(Flags_114::e114_Bit10_Teleporting))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        field_106_current_motion = eAbeStates::State_65_LedgeAscend_4548E0;
    }
    // Going down the ledge wobble?
    else if (pressed & sInputKey_Down_5550DC)
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        VOnTrapDoorOpen();
        field_100_pCollisionLine = nullptr;
        field_106_current_motion = eAbeStates::State_91_FallingFromGrab_4557B0;
        field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(75);
        field_E0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit1_ShadowAtBottom);
        field_F8_LastLineYPos = field_BC_ypos;
    }
    // Now stabilized when wobble anim is done
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit13_play_ledge_grab_sounds);
        field_106_current_motion = eAbeStates::State_67_LedgeHang_454E20;
    }
}

void Abe::State_70_RingRopePullHang_455AF0()
{
    PullRingRope* pPullRing = static_cast<PullRingRope*>(sObjectIds_5C1B70.Find_449CF0(field_15C_pull_rope_id));
    if (pPullRing)
    {
        if (!pPullRing->VIsNotBeingPulled_49BC90())
        {
            return;
        }
        pPullRing->VMarkAsPulled_49B610();
    }

    field_15C_pull_rope_id = -1;
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eAbeStates::State_91_FallingFromGrab_4557B0;
}

void Abe::State_71_Knockback_455090()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        FallOnBombs_44EC10();
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_100_pCollisionLine)
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
            {
                field_C4_velx = FP_FromInteger(0);
            }

            MoveWithVelocity_450FA0(FP_FromDouble(0.67));

            if ((gMap_5C3030.field_0_current_level == LevelIds::eMines_1
                || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8
                || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5
                || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6
                || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9)
                && field_20_animation.field_92_current_frame == 7)
            {
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
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

            if (field_106_current_motion == eAbeStates::State_84_FallLandDie_45A420)
            {
                field_106_current_motion = eAbeStates::State_71_Knockback_455090;
                SFX_Play_46FA90(SoundEffect::KillEffect_64, 85);
                SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 95, 95);
            }
            else if (field_106_current_motion == eAbeStates::State_16_LandSoft_45A360)
            {
                field_106_current_motion = eAbeStates::State_71_Knockback_455090;
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!(field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2)) && (field_100_pCollisionLine || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))))
        {
            if (field_10C_health > FP_FromInteger(0) || gAbeBulletProof_5C1BDA || field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
            {
                field_106_current_motion = eAbeStates::State_72_KnockbackGetUp_455340;
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
            Environment_SFX_457A40(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 32767, this);
        }
    }
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_74_RollingKnockback_455290()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        FallOnBombs_44EC10();
    }

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!(field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2)))
        {
            if (field_10C_health > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)
            {
                field_106_current_motion = eAbeStates::State_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }

}

void Abe::State_75_JumpIntoWell_45C7B0()
{
    field_E0_pShadow->field_14_flags.Clear(Shadow::eBit2_Enabled);

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

void Abe::State_76_ToInsideOfAWellLocal_45CA40()
{
    State_79_InsideWellLocal_45CA60();
}

void Abe::State_77_ToWellShotOut_45D130()
{
    State_80_WellShotOut_45D150();
}

void Abe::State_78_WellBegin_45C810()
{
    if (field_20_animation.field_92_current_frame > 10)
    {
        field_E0_pShadow->field_14_flags.Clear(Shadow::eBit2_Enabled);

        // Get a local well
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::LocalWell_8);

        if (!field_FC_pPathTLV)
        {
            // No local well, must be an express well
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::WellExpress_23);
        }

        const short xpos = FP_GetExponent(field_B8_xpos);
        const short tlv_mid_x = (field_FC_pPathTLV->field_8_top_left.field_0_x + field_FC_pPathTLV->field_C_bottom_right.field_0_x) / 2;
        if (xpos > tlv_mid_x)
        {
            field_B8_xpos -= field_CC_sprite_scale;
        }
        else if (xpos < tlv_mid_x)
        {
            field_B8_xpos += field_CC_sprite_scale;
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
        field_124_timer = 15;

        SFX_Play_46FA90(SoundEffect::WellEnter_21, 0, field_CC_sprite_scale);

        if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::WellExpress_23))
        {
            field_106_current_motion = eAbeStates::State_82_InsideWellExpress_45CC80;
        }
        else
        {
            field_106_current_motion = eAbeStates::State_79_InsideWellLocal_45CA60;
        }
    }
}

void Abe::State_79_InsideWellLocal_45CA60()
{
    const int gnFrame = field_124_timer;
    field_124_timer = gnFrame - 1;
    if (!gnFrame)
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::LocalWell_8);

        if (!field_FC_pPathTLV)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::WellExpress_23);
        }

        field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
        field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_fall_to_well);

        Path_Well_Base* pBaseWell = static_cast<Path_Well_Base*>(field_FC_pPathTLV);
        if (pBaseWell->field_4_type == TlvTypes::LocalWell_8)
        {
            Path_Well_Local* pLocal = static_cast<Path_Well_Local*>(pBaseWell);
            if (SwitchStates_Get_466020(pBaseWell->field_2_trigger_id))
            {
                Calc_Well_Velocity_45C530(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    pLocal->field_1C_on_dx,
                    pLocal->field_1E_on_dy);
            }
            else
            {
                Calc_Well_Velocity_45C530(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    pLocal->field_18_off_dx,
                    pLocal->field_1A_off_dy);
            }
        }
        else if (pBaseWell->field_4_type == TlvTypes::WellExpress_23)
        {
            Path_Well_Express* pExpress = static_cast<Path_Well_Express*>(pBaseWell);
            Calc_Well_Velocity_45C530(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                pExpress->field_18_exit_x,
                pExpress->field_1A_exit_y);
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

        SFX_Play_46FA90(SoundEffect::WellExit_20, 0, field_CC_sprite_scale);

        ++field_106_current_motion;
        field_F8_LastLineYPos = field_BC_ypos;

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

        SetActiveCameraDelayedFromDir_408C40();

        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);

    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted) ||
        (field_106_current_motion != eAbeStates::State_80_WellShotOut_45D150 && field_106_current_motion != eAbeStates::State_77_ToWellShotOut_45D130))
    {
        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 13;
        }
        else
        {
            field_20_animation.field_C_render_layer = 32;
        }

        field_E0_pShadow->field_14_flags.Set(Shadow::Flags::eBit2_Enabled);
    }

    if (field_106_current_motion == eAbeStates::State_84_FallLandDie_45A420)
    {
        field_10C_health = FP_FromInteger(1);
        field_106_current_motion = eAbeStates::State_16_LandSoft_45A360;
    }
}

void Abe::jState_81_WellBegin_45C7F0()
{
    jState_81_WellBegin_4017F8();
}

void Abe::State_82_InsideWellExpress_45CC80()
{
    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::LocalWell_8);

    if (!field_FC_pPathTLV)
    {
        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::WellExpress_23);
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

    field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
    field_F8_LastLineYPos = field_BC_ypos;

    if (field_19A_to_level != gMap_5C3030.field_0_current_level ||
        field_19C_to_path != gMap_5C3030.field_2_current_path ||
        field_19E_to_camera != gMap_5C3030.field_4_current_camera)
    {
        field_124_timer = 1;

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
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
            field_106_current_motion = eAbeStates::jState_85_Fall_455070;
            field_20_animation.field_C_render_layer = 32;
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_106_current_motion = eAbeStates::State_83_WellExpressShotOut_45CF70;
        }
    }
    else
    {
        State_83_WellExpressShotOut_45CF70();
        field_BC_ypos -= field_C8_vely * field_CC_sprite_scale;
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_fall_to_well);
        field_106_current_motion = eAbeStates::State_79_InsideWellLocal_45CA60;
    }
}

void Abe::State_83_WellExpressShotOut_45CF70()
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
        if (pTlvIter->field_4_type == TlvTypes::LocalWell_8 || pTlvIter->field_4_type == TlvTypes::WellExpress_23)
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

    field_C2_lvl_number = gMap_5C3030.field_0_current_level;
    field_C0_path_number = gMap_5C3030.field_2_current_path;

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

        field_1AC_flags.Set(Flags_1AC::e1AC_Bit3_fall_to_well);
        field_106_current_motion = eAbeStates::State_79_InsideWellLocal_45CA60;
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
        SFX_Play_46FA90(SoundEffect::KillEffect_64, 85);
        SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 95, 95);
        auto pShake = ae_new<ScreenShake>();
        if (pShake)
        {
            pShake->ctor_4ACF70(1, 0);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<int>(sGnFrame_5C1B84) >= field_124_timer)
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
    DeathFadeOut* pFade = static_cast<DeathFadeOut*>(sObjectIds_5C1B70.Find_449CF0(field_148_fade_obj_id));

    switch (field_120_state)
    {
    case 0u:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            // Add ref
            ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, TRUE, 0);

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

            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);

            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::MovieHandStone_27);

            sHandstoneSoundChannels_5C2C68 = SFX_Play_46FBA0(SoundEffect::HandstoneTransition_12, 127, -300);

            int id = 0;
            Path_MovieStone* pMovieStoneTlv = static_cast<Path_MovieStone*>(field_FC_pPathTLV);
            if (!pMovieStoneTlv)
            {
                Path_HandStone* pHandStoneTlv = static_cast<Path_HandStone*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    TlvTypes::HandStone_61));

                field_FC_pPathTLV = pHandStoneTlv;

                if (pHandStoneTlv)
                {
                    id = pHandStoneTlv->field_18_trigger_id;

                    field_184_fmv_id = pHandStoneTlv->field_10_scale; // TODO: Never used for this type?
                    for (int i = 0; i < 3; i++)
                    {
                        field_186_to_camera_id[i] = pHandStoneTlv->field_12_camera_ids[i];
                    }
                    field_18C_unused = static_cast<short>(pHandStoneTlv->field_18_trigger_id); // TODO: Never used?
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

                field_180_hand_stone_type = field_FC_pPathTLV->field_4_type;
            }
            else
            {
                field_106_current_motion = eAbeStates::State_87_HandstoneEnd_45C4F0;
            }
        }
        break;

    case 1u:
        if (pCircularFade->VDone_4CE0B0())
        {
            if (field_180_hand_stone_type == TlvTypes::MovieHandStone_27)
            {
                pScreenManager_5BB5F4->field_40_flags |= 0x10000;

                FmvInfo* pFmvRec = Path_Get_FMV_Record_460F70(gMap_5C3030.field_0_current_level, field_184_fmv_id);
                DWORD pos = 0;

                Get_fmvs_sectors_494460(pFmvRec->field_0_pName, 0, 0, &pos, 0, 0);
                sLevelId_dword_5CA408 = static_cast<DWORD>(gMap_5C3030.field_0_current_level);

                Movie* pMovie = ae_new<Movie>();
                if (pMovie)
                {
                    pMovie->ctor_4DFDE0(pFmvRec->field_4_id, pos, pFmvRec->field_6_flags & 1, pFmvRec->field_8_flags, pFmvRec->field_A_volume);
                }
                field_120_state = 2;
            }
            else if (field_180_hand_stone_type == TlvTypes::HandStone_61)
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_17C_cam_idx = 1;
                field_120_state = 4;
                pCircularFade->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                field_14C_circular_fade_id = -1;
                DeathFadeOut* pFade33 = ae_new<DeathFadeOut>();
                if (pFade33)
                {
                    pFade33->ctor_427030(40, 0, 0, 8, 2);
                }

                field_148_fade_obj_id = pFade33->field_8_object_id;
                field_19E_to_camera = gMap_5C3030.field_4_current_camera;
                gMap_5C3030.SetActiveCam_480D30(field_C2_lvl_number, field_C0_path_number, field_186_to_camera_id[0], CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            }
        }
        break;

    case 2u:
        if (sMovie_ref_count_BB4AE4 == 0)
        {
            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60((unsigned __int16 **)gMap_5C3030.field_2C_camera_array[0]->field_C_pCamRes);
            pScreenManager_5BB5F4->field_40_flags |= 0x10000;
            pCircularFade->VFadeIn_4CE300(0, 0);
            field_120_state = 3;
        }
        break;

    case 3u:
        if (pCircularFade->VDone_4CE0B0())
        {
            pCircularFade->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_14C_circular_fade_id = -1;

            field_106_current_motion = eAbeStates::State_87_HandstoneEnd_45C4F0;

            if (sHandstoneSoundChannels_5C2C68)
            {
                SND_Stop_Channels_Mask_4CA810(sHandstoneSoundChannels_5C2C68);
                sHandstoneSoundChannels_5C2C68 = 0;
            }

            BYTE** ppResToFree = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, 0, 0);
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
                SFX_Play_46FA90(SoundEffect::IngameTransition_84, 90);
            }
        }
        break;

    case 5u:
        if (pFade->field_7E_bDone)
        {
            if (field_17C_cam_idx < 3 && field_186_to_camera_id[field_17C_cam_idx] != 0)
            {
                field_120_state = 4;

                pFade->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                pFade = ae_new<DeathFadeOut>();

                if (pFade)
                {
                    pFade->ctor_427030(40, 0, 0, 8, 2);
                }

                field_148_fade_obj_id = pFade->field_8_object_id;

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
        pFade->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_148_fade_obj_id = -1;

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
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_89_BrewMachineBegin_4584C0()
{
    if (field_120_state > 0)
    {
        if (field_120_state <= 36u)
        {
            if (field_120_state > 11u && !((field_120_state - 12) % 6))
            {
                SFX_Play_46FBA0(SoundEffect::BrewMachineUseEnd_119, 0, 32 * field_120_state);
            }
            field_120_state++;
        }
        else
        {
            field_106_current_motion = eAbeStates::State_90_BrewMachineEnd_4585B0;
        }
    }
    else if (field_20_animation.field_92_current_frame == 8)
    {
        if (GetEvilFart_4585F0(FALSE))
        {
            SFX_Play_46FA90(SoundEffect::BrewMachineUseStart_116, 0);
        }
        else
        {
            SFX_Play_46FA90(SoundEffect::BrewMachineUseEmpty_118, 0);
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (GetEvilFart_4585F0(TRUE))
        {
            SFX_Play_46FA90(SoundEffect::BrewMachineUseMid_117, 0);
            field_120_state = 1;
        }
        else
        {
            field_106_current_motion = eAbeStates::State_90_BrewMachineEnd_4585B0;
        }
    }

}

void Abe::State_90_BrewMachineEnd_4585B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_106_current_motion = eAbeStates::State_0_Idle_44EEB0;
        field_124_timer = 1;
    }
}

// Let go of an edge/hoist/pull ring
void Abe::State_91_FallingFromGrab_4557B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::jState_85_Fall_455070;
    }

    State_3_Fall_459B60();

    if (field_106_current_motion == eAbeStates::State_84_FallLandDie_45A420)
    {
        field_106_current_motion = eAbeStates::State_16_LandSoft_45A360;
    }

    field_F4_previous_motion = eAbeStates::State_91_FallingFromGrab_4557B0;
}

void Abe::State_92_ForceDownFromHoist_455800()
{
    if (!field_124_timer)
    {
        field_E0_pShadow->field_14_flags.Clear(Shadow::eBit1_ShadowAtBottom);
        VOnTrapDoorOpen();
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(75)),
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(10),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16) == 1)
        {
            field_BC_ypos = hitY;
            field_106_current_motion = eAbeStates::State_84_FallLandDie_45A420;
            field_128.field_4_regen_health_timer = sGnFrame_5C1B84 + 900;
            field_F4_previous_motion = 3;
            return;
        }
        field_BC_ypos += (field_CC_sprite_scale * FP_FromInteger(75));
        field_F8_LastLineYPos = field_BC_ypos;
        field_100_pCollisionLine = nullptr;
        field_124_timer = field_124_timer + 1;
    }
    State_3_Fall_459B60();
}

void Abe::State_93_WalkOffEdge_455970()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::jState_85_Fall_455070;
    }
    State_3_Fall_459B60();
}

void Abe::State_94_RunOffEdge_4559A0()
{
    State_93_WalkOffEdge_455970();
}

void Abe::State_95_SneakOffEdge_4559C0()
{
    State_93_WalkOffEdge_455970();
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
    State_93_WalkOffEdge_455970();
}

void Abe::State_97_RunJumpToFall_455A80()
{
    State_93_WalkOffEdge_455970();
}

void Abe::State_98_RollOffEdge_455AA0()
{
    State_93_WalkOffEdge_455970();
}

void Abe::State_99_LeverUse_455AC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_114_flags.Get(Flags_114::e114_Bit10_Teleporting))
        {
            field_106_current_motion = eAbeStates::State_34_DunnoBegin_44ECF0;
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::State_100_SlapBomb_455B60()
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
        field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
    }

}

void Abe::State_101_KnockForward_455420()
{
    if (field_20_animation.field_92_current_frame == 12)
    {
        FallOnBombs_44EC10();
    }

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_100_pCollisionLine)
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
            {
                field_C4_velx = FP_FromInteger(0);
            }
            MoveWithVelocity_450FA0(FP_FromDouble(0.7));
        }
        else
        {
            State_3_Fall_459B60();
            if (field_106_current_motion == eAbeStates::State_84_FallLandDie_45A420 || field_106_current_motion == eAbeStates::State_16_LandSoft_45A360)
            {
                field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2) && (field_100_pCollisionLine || !field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render)))
        {
            if (field_10C_health > FP_FromInteger(0) || gAbeBulletProof_5C1BDA)
            {
                field_106_current_motion = eAbeStates::jState_103_KnockForwardGetUp_455380;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }

}

void Abe::State_102_RollingKnockForward_455310()
{
    State_74_RollingKnockback_455290();
    if (field_106_current_motion == eAbeStates::State_72_KnockbackGetUp_455340)
    {
        field_106_current_motion = eAbeStates::jState_103_KnockForwardGetUp_455380;
    }
}

void Abe::jState_103_KnockForwardGetUp_455380()
{
    State_72_KnockbackGetUp_455340();
}

void Abe::State_104_RockThrowStandingHold_455DF0()
{
    auto pRock = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id));
    if (field_20_animation.field_92_current_frame >= 4)
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0 | sInputKey_Up_5550D8 | sInputKey_Down_5550DC))
        {
            if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_1A3_throw_direction = 0;
                }
                else
                {
                    field_1A3_throw_direction = 2;
                }
            }
            else if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_1A3_throw_direction = 2;
                }
                else
                {
                    field_1A3_throw_direction = 0;
                }
            }
            else if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
            {
                field_1A3_throw_direction = 1;
            }
            else
            {
                // Down
                field_1A3_throw_direction = 3;
            }
            field_106_current_motion = eAbeStates::State_105_RockThrowStandingThrow_456460;
        }
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4)) // ?? isn't released like in the crouching motion ??
    {
        pRock->VToDead_4114B0();
        field_158_throwable_id = -1;
        field_106_current_motion = eAbeStates::State_106_RockThrowStandingEnd_455F20;
        if (!gInfiniteGrenades_5C1BDE)
        {
            field_1A2_throwable_count++;
        }
    }
}

void Abe::State_105_RockThrowStandingThrow_456460()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play_46FA90(SoundEffect::AirStream_23, 0, field_CC_sprite_scale);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_106_RockThrowStandingEnd_455F20()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
}

void Abe::State_107_RockThrowCrouchingHold_454410()
{
    auto pRock = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(field_158_throwable_id));
    if (field_20_animation.field_92_current_frame >= 4)
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0 | sInputKey_Up_5550D8 | sInputKey_Down_5550DC))
        {
            field_1A3_throw_direction = 4;
            field_106_current_motion = eAbeStates::State_108_RockThrowCrouchingThrow_454500;
            if (pRock->field_4_typeId == Types::eMeat_84)
            {
                field_1A3_throw_direction = 5;
            }
        }
    }

    if (sInputObject_5BD4E0.IsReleased(sInputKey_ThrowItem_5550F4))
    {
        pRock->VToDead_4114B0();
        field_158_throwable_id = -1;
        field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
        if (!gInfiniteGrenades_5C1BDE)
        {
            field_1A2_throwable_count++;
        }
    }
}

void Abe::State_108_RockThrowCrouchingThrow_454500()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play_46FA90(SoundEffect::AirStream_23, 0, field_CC_sprite_scale);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
    }
}

void Abe::State_109_ZShotRolling_455550()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    State_3_Fall_459B60();
    
    if (field_106_current_motion != eAbeStates::State_109_ZShotRolling_455550 && !gAbeBulletProof_5C1BDA)
    {
        if (field_110_id != -1)
        {
            VOnTrapDoorOpen();
        }
        field_106_current_motion = eAbeStates::State_109_ZShotRolling_455550;
        field_100_pCollisionLine = nullptr;
        field_BC_ypos += (field_CC_sprite_scale * FP_FromInteger(4));
    }

    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            if (!field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2))
            {
                field_BC_ypos += FP_FromInteger(240);
                Mudokon_SFX_457EC0(MudSounds::eDeathDropScream_15, 0, 0, this);
                ToDie_4588D0();
            }
        }
    }
}

void Abe::State_110_ZShot_455670()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
    State_3_Fall_459B60();
    
    if (field_106_current_motion != eAbeStates::State_110_ZShot_455670 && !gAbeBulletProof_5C1BDA)
    {
        if (field_110_id != -1)
        {
            BaseGameObject* pLiftPoint = sObjectIds_5C1B70.Find_449CF0(field_110_id);
            if (pLiftPoint->field_4_typeId == Types::eLiftPoint_78)
            {
                static_cast<LiftPoint*>(pLiftPoint)->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
            }
            VOnTrapDoorOpen();
        }
        field_106_current_motion = eAbeStates::State_110_ZShot_455670;
        field_100_pCollisionLine = nullptr;
        field_BC_ypos += (field_CC_sprite_scale * FP_FromInteger(4));
    }

    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        field_BC_ypos += FP_FromInteger(240);
        Mudokon_SFX_457EC0(MudSounds::eDeathDropScream_15, 0, 0, this);
        ToDie_4588D0();
    }
}

void Abe::State_111_PickupItem_4564A0()
{
    auto pRock = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_160_slapable_or_pick_item_id));

    if (field_20_animation.field_92_current_frame == 7)
    {
        SFX_Play_46FA90(SoundEffect::PickupItem_28, 0, field_CC_sprite_scale);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
    }

    if (sActiveHero_5C1B68->field_20_animation.field_92_current_frame >= 5)
    {
        if (pRock)
        {
            pRock->VOnPickUpOrSlapped();
            field_160_slapable_or_pick_item_id = -1;
        }
    }
}

void Abe::State_112_Chant_45B1C0()
{
    BaseAliveGameObject* pPossesTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_154_possesed_object_id));
    OrbWhirlWind* pOrbWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds_5C1B70.Find_449CF0(field_150_OrbWhirlWind_id));

    if (field_120_state != 3 && field_120_state != 4)
    {
        SND_SEQ_PlaySeq_4CA960(SeqId::MudokonChant1_10, 0, 0);
    }

    if (!pOrbWhirlWind)
    {
        field_150_OrbWhirlWind_id = -1;
    }

    switch (field_120_state)
    {
    case 0u:
    {
        Event_Broadcast_422BC0(kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventAbeOhm, this);

        BaseAliveGameObject* pObj = FindObjectToPosses_44B7B0(); // Find a victim.

        if (field_168_ring_pulse_timer)
        {
            if (!field_16C_bHaveShrykull && !field_16E_bHaveInvisiblity && !(field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing)))
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
                    field_170_invisible_timer = field_170_invisible_timer + field_176_invisibility_id;
                }
                else
                {
                    field_170_invisible_timer = sGnFrame_5C1B84 + field_176_invisibility_id;
                }

                field_174_unused = 0;

                InvisibleEffect* pInvisible = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
                if (!pInvisible || pInvisible->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    pInvisible = ae_new<InvisibleEffect>();
                    if (pInvisible)
                    {
                        pInvisible->ctor_45F280(this);
                    }
                    field_178_invisible_effect_id = pInvisible->field_8_object_id;
                }
                pInvisible->BecomeInvisible_45F9E0();
            }

            if (field_168_ring_pulse_timer)
            {
                if (field_1AC_flags.Get(Flags_1AC::e1AC_eBit15_have_healing))
                {
                    bool bAliveMudIsInSameScreen = false;
                    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
                    {
                        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        if (pObjIter->field_4_typeId == Types::eMudokon_110)
                        {
                            if (pObjIter->field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed)) // TODO: Is sick flag ?
                            {
                                if (pObjIter->Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0 && pObjIter->field_10C_health > FP_FromInteger(0))
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

                        field_1AC_flags.Clear(Flags_1AC::e1AC_eBit15_have_healing);
                        field_168_ring_pulse_timer = 0;
                    }
                }
            }
        }

        // Stopped chanting?
        if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) || field_20_animation.field_92_current_frame == 3) && !Input_IsChanting_45F260())
        {
            field_106_current_motion = eAbeStates::State_113_ChantEnd_45BBE0;
            field_154_possesed_object_id = -1;
            if (pOrbWhirlWind)
            {
                pOrbWhirlWind->ToStop_4E4050();
                field_150_OrbWhirlWind_id = -1;
            }
            return;
        }

        if (!(sGnFrame_5C1B84 % 4))
        {
            New_RandomizedChant_Particle_45BC70(this);
        }

        if (static_cast<int>(sGnFrame_5C1B84) >= field_124_timer - 70)
        {
            if (pObj)
            {
                if (!pOrbWhirlWind)
                {
                    auto pWhirlWind = ae_new<OrbWhirlWind>();
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
                    pOrbWhirlWind = pWhirlWind;
                    field_150_OrbWhirlWind_id = pWhirlWind->field_8_object_id;
                }
            }
            else
            {
                field_124_timer = sGnFrame_5C1B84 + 70;
                if (pOrbWhirlWind)
                {
                    pOrbWhirlWind->ToStop_4E4050();
                    pOrbWhirlWind = nullptr;
                    field_150_OrbWhirlWind_id = -1;
                }
            }
        }


        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer)
        {
            return;
        }

        if (!pObj)
        {
            return;
        }

        field_154_possesed_object_id = pObj->field_8_object_id;
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 0, -600);
        field_120_state = 1;
        field_124_timer = sGnFrame_5C1B84 + 30;

        PSX_RECT bRect = {};
        pObj->vGetBoundingRect_424FD0(&bRect, 1);

        pOrbWhirlWind->ToSpin_4E3FD0(
            FP_FromInteger((bRect.w - bRect.x) / 2),
            FP_FromInteger((bRect.h - bRect.y) / 2),
            pObj->field_CC_sprite_scale,
            pObj);

        auto pClass_544FC8 = ae_new<PossessionFlicker>();
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

        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer)
        {
            if (!pPossesTarget ||
                pPossesTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3) ||
                pPossesTarget->field_10C_health <= FP_FromInteger(0) ||
                pPossesTarget->Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0)
            {
                field_106_current_motion = eAbeStates::State_113_ChantEnd_45BBE0;
                field_154_possesed_object_id = -1;
                if (pOrbWhirlWind)
                {
                    pOrbWhirlWind->ToStop_4E4050();
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

        if (!pPossesTarget)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_motion = eAbeStates::State_113_ChantEnd_45BBE0;
            }
            return;
        }

        if (pPossesTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_154_possesed_object_id = -1;
        }

        if (pPossesTarget->field_10C_health <= FP_FromInteger(0))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_motion = eAbeStates::State_113_ChantEnd_45BBE0;
            }
            return;
        }

        sControlledCharacter_5C1B8C = pPossesTarget;

        pPossesTarget->VPossessed_408F70();

        field_154_possesed_object_id = -1;

        if (sControlledCharacter_5C1B8C->field_4_typeId == Types::eSlig_125 ||
            sControlledCharacter_5C1B8C->field_4_typeId == Types::eFlyingSlig_54 ||
            sControlledCharacter_5C1B8C->field_4_typeId == Types::eCrawlingSlig_26 ||
            sControlledCharacter_5C1B8C->field_4_typeId == Types::eGlukkon_67)
        {
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_laugh_at_chant_end);
        }

        auto pFlicker = ae_new<PossessionFlicker>();
        if (pFlicker)
        {
            pFlicker->ctor_4319E0(sControlledCharacter_5C1B8C, 60, 128, 255, 255);
        }
        SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 70, 400);
        field_120_state = 3;
    }
    return;

    case 3u:
    {
        if (sControlledCharacter_5C1B8C != this)
        {
            return;
        }

        auto pFlicker = ae_new<PossessionFlicker>();
        if (pFlicker)
        {
            pFlicker->ctor_4319E0(sControlledCharacter_5C1B8C, 15, 128, 255, 255);
        }

        field_120_state = 4;
        field_124_timer = sGnFrame_5C1B84 + 15;
    }
    return;

    case 4u:
    {
        if (!(sGnFrame_5C1B84 % 4))
        {
            New_RandomizedChant_Particle_45BC70(this);
        }

        if (static_cast<int>(sGnFrame_5C1B84) <= field_124_timer)
        {
            return;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eAbeStates::State_113_ChantEnd_45BBE0;
        }
    }
    return;

    case 6u:
    {
        Event_Broadcast_422BC0(kEventSpeaking, this);
        Event_Broadcast_422BC0(kEventAbeOhm, this);
        if (!(sGnFrame_5C1B84 % 4))
        {
            New_RandomizedChant_Particle_45BC70(this);
        }
    }
    return;

    default:
        return;
    }
}

void Abe::State_113_ChantEnd_45BBE0()
{
    SND_SEQ_Stop_4CAE60(SeqId::MudokonChant1_10);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_1AC_flags.Get(Flags_1AC::e1AC_Bit9_laugh_at_chant_end))
        {
            field_106_current_motion = eAbeStates::State_9_Speak_45B180;
            Mudokon_SFX_457EC0(MudSounds::eGiggle_8, 0, 0, this);
            field_1AC_flags.Clear(Flags_1AC::e1AC_Bit9_laugh_at_chant_end);
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
            TlvTypes::Door_5));

        field_FC_pPathTLV = pDoorTlv;

        if (pDoorTlv->field_42_cancel_throwables)
        {
            if (field_1A2_throwable_count > 0 && gpThrowableArray_5D1E2C)
            {
                gpThrowableArray_5D1E2C->Remove_49AA00(field_1A2_throwable_count);
                field_1A2_throwable_count = 0;
            }
        }

        // An OWI hack.. when both mudomo and mundanchee are done force back to necrum mines
        bool hackChange = false;
        if (gMap_5C3030.field_0_current_level == LevelIds::eMudomoVault_Ender_11)
        {
            if (gMap_5C3030.field_2_current_path == 13 &&
                gMap_5C3030.field_4_current_camera == 14 &&
                field_1AC_flags.Get(Flags_1AC::e1AC_eBit16_is_mudanchee_vault_ender))
            {
                hackChange = true;
            }
        }
        else if (gMap_5C3030.field_0_current_level == LevelIds::eMudancheeVault_Ender_7)
        {
            if (gMap_5C3030.field_2_current_path == 11 &&
                gMap_5C3030.field_4_current_camera == 2 &&
                field_1AE_flags.Get(Flags_1AE::e1AE_Bit1_is_mudomo_vault_ender))
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
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_106_current_motion = eAbeStates::jState_85_Fall_455070;
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
        field_C2_lvl_number = gMap_5C3030.field_0_current_level;
        field_C0_path_number = gMap_5C3030.field_2_current_path;

        Path_Door* pDoorTlv2 = static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::Door_5, 0));
        field_FC_pPathTLV = pDoorTlv2;
        Path_Door* pTargetDoorTlv = pDoorTlv2;
        if (pTargetDoorTlv->field_18_door_number != field_1A0_door_id)
        {
            do
            {
                Path_Door* pDoorIter = static_cast<Path_Door*>(Path::TLV_Next_Of_Type_4DB720(field_FC_pPathTLV, TlvTypes::Door_5));
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
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
            field_BC_ypos = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y);
            field_F8_LastLineYPos = FP_FromInteger(field_FC_pPathTLV->field_8_top_left.field_2_y);
        }

        field_168_ring_pulse_timer = 0;
        InvisibleEffect* pInvisibleEffect = static_cast<InvisibleEffect*>(sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id));
        if (pInvisibleEffect)
        {
            if (!(pInvisibleEffect->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
            {
                pInvisibleEffect->ClearInvisibility_45FA50();
                field_114_flags.Clear(Flags_114::e114_Bit8_bInvisible);
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
            field_106_current_motion = eAbeStates::State_115_DoorExit_459A40;
        }
        else
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            // Abe opens the door and he falls through the floor
            field_106_current_motion = eAbeStates::jState_85_Fall_455070;
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
            TlvTypes::Door_5))->field_40_close_after_use)
        {
            // TODO: Ret ignored even in real ??
            FindObjectOfType_425180(
                Types::eDoor_33,
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

                if (pObj->field_4_typeId == Types::eDoor_33)
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
            field_106_current_motion = eAbeStates::State_117_InMineCar_4587C0;
        }
    }
}

void Abe::State_117_InMineCar_4587C0()
{
    if (sInputObject_5BD4E0.isPressed(sInputKey_DoAction_5550E4))
    {
        auto pMineCar = static_cast<MineCar*>(sControlledCharacter_5C1B8C);
        if (pMineCar->field_11C_state == MineCarStates::eState_1_ParkedWithAbe &&
            pMineCar->field_1BC_turn_direction == MineCarDirs::eUp_3)
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(2),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(2),
                &pLine,
                &hitX,
                &hitY,
                field_D6_scale != 0 ? 1 : 16))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

                field_106_current_motion = eAbeStates::State_118_MineCarExit_458890;
            }
        }
    }

}

void Abe::State_118_MineCarExit_458890()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
        field_106_current_motion = eAbeStates::State_0_Idle_44EEB0;
        field_124_timer = 1;
    }
}

void Abe::State_119_ToShrykull_45A990()
{
    if (field_120_state == 0)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);

            field_120_state = 1;
 
            auto pShryZapper = ae_new<Shrykull>();
            if (pShryZapper)
            {
                pShryZapper->ctor_4AEA20();
            }
        }
    }
}

void Abe::State_120_EndShrykull_45AB00()
{
    if (field_124_timer)
    {
        field_124_timer = field_124_timer - 1;
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_168_ring_pulse_timer == 0)
        {
            Free_Shrykull_Resources_45AA90();
        }

        // Abe finds transforming into a god rather funny
        field_106_current_motion = eAbeStates::State_9_Speak_45B180;
        Mudokon_SFX_457EC0(MudSounds::eGiggle_8, 0, 0, this);
    }
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
        field_106_current_motion = eAbeStates::State_123_LiftGrabIdle_45A6A0;
    }
}

void Abe::State_122_LiftGrabEnd_45A670()
{
    field_C8_vely = FP_FromInteger(0);
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44E6B0();
    }
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
            field_106_current_motion = eAbeStates::State_124_LiftUseUp_45A780;
        }
    }
    else if (pressed & sInputKey_Down_5550DC)
    {
        if (!pLiftPoint->vOnBottomFloor_4618F0())
        {
            field_106_current_motion = eAbeStates::State_125_LiftUseDown_45A7B0;
        }
    }
    else if (pLiftPoint->vOnAnyFloor_461920())
    {
        // You ain't letting go unless you are on a floor where you can walk off..
        field_106_current_motion = eAbeStates::State_122_LiftGrabEnd_45A670;
    }

}

void Abe::State_124_LiftUseUp_45A780()
{
    field_106_current_motion = MoveLiftUpOrDown_45A7E0(FP_FromInteger(-4));
}

void Abe::State_125_LiftUseDown_45A7B0()
{
    field_106_current_motion = MoveLiftUpOrDown_45A7E0(FP_FromInteger(4));
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
        field_106_current_motion = eAbeStates::State_127_TurnWheelLoop_456750;
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
        Path_LevelLoader* pLevelLoader = static_cast<Path_LevelLoader*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::LevelLoader_86, 0));
        if (pLevelLoader && SwitchStates_Get_466020(pLevelLoader->field_10_id))
        {
            field_120_state = eMapChanging;
            SND_SEQ_Play_4CAB10(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            auto pMusicTrigger = ae_new<MusicTrigger>();
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
            field_106_current_motion = eAbeStates::State_128_TurnWheelEnd_4569A0;
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
            auto pWorkWheel = static_cast<WorkWheel*>(sObjectIds_5C1B70.Find_449CF0(field_164_wheel_id));
            if (pWorkWheel) // Most likely always nullptr here, maybe the whole "stop wheel" was an inlined func
            {
                pWorkWheel->VStopTurning(1);
            }
            field_164_wheel_id = -1;

            auto pPathAbeStart = static_cast<Path_AbeStart*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::AbeStart_22, 0));
            field_B8_xpos = FP_FromInteger((pPathAbeStart->field_8_top_left.field_0_x + pPathAbeStart->field_C_bottom_right.field_0_x) / 2);
            field_BC_ypos = FP_FromInteger(pPathAbeStart->field_C_bottom_right.field_2_y);
            field_F8_LastLineYPos = FP_FromInteger(pPathAbeStart->field_C_bottom_right.field_2_y);
            // TODO: OG bug, scale not read from the TLV ??

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            field_106_current_motion = eAbeStates::jState_85_Fall_455070;
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit7_land_softly);
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
        SFX_Play_46FBA0(SoundEffect::Choke_81, 127, 128);
        break;
    case 9:
        SFX_Play_46FBA0(SoundEffect::Choke_81, 127, 384);
        break;
    case 28:
        SFX_Play_46FBA0(SoundEffect::Choke_81, 127, 640);
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

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        const int previousValue = field_124_timer;
        field_124_timer = previousValue - 1;
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

void Abe::FleechDeath_459350()
{
    BaseGameObject* pInvisibleEffect = sObjectIds_5C1B70.Find_449CF0(field_178_invisible_effect_id);
    if (pInvisibleEffect)
    {
        if (!pInvisibleEffect->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            static_cast<InvisibleEffect*>(pInvisibleEffect)->ClearInvisibility_45FA50();
            field_178_invisible_effect_id = -1;
        }
    }
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel);
    field_106_current_motion = eAbeStates::State_58_DeadPre_4593E0;
    field_120_state = 0;
    field_10C_health = FP_FromInteger(0);
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
    field_100_pCollisionLine = nullptr;
}

void Abe::ToDie_4588D0()
{
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel);
    field_106_current_motion = eAbeStates::State_56_DeathDropFall_4591F0;
    field_124_timer = 0;
    field_10C_health = FP_FromInteger(0);
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 1, 0);
}

void Abe::ToIdle_44E6B0()
{
    field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_124_timer = sGnFrame_5C1B84;
    field_106_current_motion = eAbeStates::State_0_Idle_44EEB0;
    field_118_prev_held = 0;
    field_11C_released_buttons = 0;
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

        if (pObj->field_6_flags.Get(BaseGameObject::eInteractive_Bit8))
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->field_E4_collection_rect.x && fpX <= pAliveObj->field_E4_collection_rect.w)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->field_E4_collection_rect.y && yPos <= pAliveObj->field_E4_collection_rect.h)
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
        case Types::eTimedMine_or_MovingBomb_10:
        case Types::eUXB_143:
            field_106_current_motion = eAbeStates::State_100_SlapBomb_455B60;
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
            field_106_current_motion = eAbeStates::State_111_PickupItem_4564A0;
            field_1A2_throwable_count += static_cast<char>(static_cast<BaseThrowable*>(pSlapableOrCollectable)->VGetCount_448080()); // TOOD: Check types are correct
            if (!bThrowableIndicatorExists_5C112C)
            {
                auto pThrowableIndicator = ae_new<ThrowableTotalIndicator>();
                if (pThrowableIndicator)
                {
                    FP yoff = (field_CC_sprite_scale * FP_FromInteger(-30)) + field_BC_ypos;
                    FP xoff = field_CC_sprite_scale * FP_FromInteger(0);
                    pThrowableIndicator->ctor_431CB0(
                        xoff + field_B8_xpos,
                        yoff,
                        field_20_animation.field_C_render_layer,
                        field_20_animation.field_14_scale,
                        field_1A2_throwable_count,
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
            if (field_106_current_motion == eAbeStates::State_111_PickupItem_4564A0)
            {
                if (bStandToCrouch)
                {
                    SFX_Play_46FA90(SoundEffect::PickupItem_28, 0, field_CC_sprite_scale);
                    pSlapableOrCollectable->VOnPickUpOrSlapped();
                    field_160_slapable_or_pick_item_id = -1;
                    field_106_current_motion = eAbeStates::State_17_CrouchIdle_456BC0;
                }
            }
        }
    }
}

void Abe::Get_Shrykull_Resources_45AA20()
{
    field_10_resources_array.SetAt(25, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID, TRUE, FALSE));
    field_10_resources_array.SetAt(26, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kShrmorphResID, TRUE, FALSE));
    field_10_resources_array.SetAt(27, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, TRUE, FALSE));
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
    const BOOL flipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);

    if ((flipX && (pressed & sInputKey_Right_5550D0)) || (!flipX && (pressed & sInputKey_Left_5550D4)))
    {
        field_106_current_motion = eAbeStates::State_2_StandingTurn_451830;
        return 1;
    }

    if ((pressed & sInputKey_Right_5550D0) || (pressed & sInputKey_Left_5550D4))
    {
        const FP directionX = FP_FromInteger((pressed & sInputKey_Right_5550D0) ? 1 : -1);

        if (pressed & sInputKey_Run_5550E8)
        {
            field_C4_velx = directionX * (gridSize / FP_FromInteger(4));
            field_106_current_motion = eAbeStates::State_39_StandingToRun_450D40;
        }
        else if (pressed & sInputKey_Sneak_5550EC)
        {
            field_C4_velx = directionX * (gridSize / FP_FromInteger(10));
            field_106_current_motion = eAbeStates::State_45_SneakBegin_4507A0;
        }
        else
        {
            field_C4_velx = directionX * (gridSize / FP_FromInteger(9));
            field_106_current_motion = eAbeStates::State_6_WalkBegin_44FEE0;
        }

        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), directionX * gridSize))
        {
            if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), directionX * gridSize))
            {
                return 1;
            }
        }

        // Walking into wall?
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), directionX * gridSize))
        {
            PushWall_44E890();
            return 0;
        }

        field_106_current_motion = eAbeStates::State_19_StandToCrouch_453DC0;
        return 1;
    }

    return 0;
}

void Abe::TryHoist_44ED30()
{
    field_106_current_motion = eAbeStates::State_13_HoistBegin_452B20;

    const FP xpos = field_B8_xpos;
    const FP ypos = field_BC_ypos - FP_FromInteger(10); // Look up 10 for a hoist

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
            if (!IsFacingSameDirectionAsHoist(pHoist, this) && pHoist->field_12_edge_type != Path_Hoist::EdgeType::eBoth)
            {
                // No so auto turn around to face it
                field_108_next_motion = field_106_current_motion;
                field_106_current_motion = eAbeStates::State_2_StandingTurn_451830;
            }
        }

        field_FC_pPathTLV = pHoist;
    }
}

void CC Abe::Create_Fart_421D20()
{
    auto pFart = ae_new<EvilFart>();
    pFart->ctor_422E30();
}

__int16 Abe::TryEnterMineCar_4569E0()
{
    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & sInputKey_Up_5550D8)
    {
        for (int idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                PSX_RECT abeRect = {};
                vGetBoundingRect_424FD0(&abeRect, 1);

                PSX_RECT mineCarRect = {};
                pObj->vGetBoundingRect_424FD0(&mineCarRect, 1);

                if (PSX_Rects_overlap_no_adjustment(&abeRect, &mineCarRect) &&
                    pObj->field_CC_sprite_scale == field_CC_sprite_scale &&
                    pObj->field_4_typeId == Types::eMineCar_89)
                {
                    const FP distanceCheck = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(0.5);
                    if (field_B8_xpos - pObj->field_B8_xpos < distanceCheck)
                    {
                        if (pObj->field_B8_xpos - field_B8_xpos < distanceCheck)
                        {
                            field_120_state = 0;
                            field_106_current_motion = eAbeStates::State_116_MineCarEnter_458780;
                            field_B8_xpos = FP_FromInteger((mineCarRect.x + mineCarRect.w) / 2);
                            MapFollowMe_408D10(TRUE);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int Abe::NearDoorIsOpen_44EE10()
{
    for (int i =0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eDoor_33)
        {
            auto pDoor = static_cast<Door*>(pObj);
            if (FP_Abs(field_B8_xpos - pDoor->field_B8_xpos) < FP_FromInteger(15) &&
                FP_Abs(field_BC_ypos - pDoor->field_BC_ypos) < FP_FromInteger(20))
            {
                return pDoor->vIsOpen_41EB00();
            }
        }
    }
    // We didn't find a door - so for some reason that makes no sense return that it is open...
    return TRUE;
}

__int16 Abe::HandleDoAction_455BD0()
{
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
        case TlvTypes::LocalWell_8:
            field_FC_pPathTLV = pTlv;
            return eAbeStates::State_78_WellBegin_45C810;

        case TlvTypes::Switch_17:
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

            Switch* pSwitch = static_cast<Switch*>(FindObjectOfType_425180(Types::eLever_139, ypos, xpos));
            if (!pSwitch || !(pSwitch->VPull_4D6050(field_B8_xpos < pSwitch->field_B8_xpos)))
            {
                return eAbeStates::State_34_DunnoBegin_44ECF0;
            }

            return eAbeStates::State_99_LeverUse_455AC0;
        }

        case TlvTypes::WellExpress_23:
            field_FC_pPathTLV = pTlv;
            return eAbeStates::State_78_WellBegin_45C810;

        case TlvTypes::GrenadeMachine_59:
        {
            auto pGrenadeMachine = static_cast<BoomMachine*>(FindObjectOfType_425180(Types::eGrenadeMachine_66, field_B8_xpos,  field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(25))));
            if (!pGrenadeMachine || !(pGrenadeMachine->Vsub_445DF0()))
            {
                return eAbeStates::State_34_DunnoBegin_44ECF0;
            }
            pGrenadeMachine->Vsub_445F00();
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
    field_106_current_motion = eAbeStates::State_73_PushWall_4553A0;
    Environment_SFX_457A40(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
}

void Abe::MoveForward_44E9A0()
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
    if (field_100_pCollisionLine && (field_D6_scale != 0 ? 1 : 16) & (1 << field_100_pCollisionLine->field_8_type))
    {
        // Handle trap door collision
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36) // TODO: Enum type
        {
            if (pfield_110)
            {
                pfield_110->VRemove(this);
                field_110_id = -1;
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            vOnCollisionWith_424EE0(
            { bRect.x, static_cast<short>(bRect.y + 5) },
            { static_cast<short>(bRect.w + 5), static_cast<short>(bRect.h + 5) }, // TODO: Is it really on both ??
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
        else if (pfield_110)
        {
            pfield_110->VRemove(this);
            field_110_id = -1;
        }
    }
    else
    {
        field_100_pCollisionLine = 0;
        if (pfield_110)
        {
            pfield_110->VRemove(this);
            field_110_id = -1;
        }

        field_118_prev_held = 0;

        switch (field_106_current_motion)
        {
        case eAbeStates::State_1_WalkLoop_44FBA0:
        case eAbeStates::State_6_WalkBegin_44FEE0:
        case eAbeStates::State_4_WalkToIdle_44FFC0:
        case eAbeStates::State_5_MidWalkToIdle_450080:
            field_106_current_motion = eAbeStates::State_93_WalkOffEdge_455970;
            break;
        case eAbeStates::State_33_RunLoop_4508E0:
        case eAbeStates::State_39_StandingToRun_450D40:
            field_106_current_motion = eAbeStates::State_94_RunOffEdge_4559A0;
            break;
        case eAbeStates::jState_38_RunToRoll_453A70:
        case eAbeStates::State_22_RollBegin_4539A0:
        case eAbeStates::State_23_RollLoop_453A90:
        case eAbeStates::State_24_453D00:
            field_106_current_motion = eAbeStates::State_98_RollOffEdge_455AA0;
            break;
        default:
            field_106_current_motion = eAbeStates::State_95_SneakOffEdge_4559C0;
            break;
        }

        field_128.field_8_x_vel_slow_by = FP_FromDouble(0.3); // TODO: Check
        field_B8_xpos = oldXPos + field_C4_velx;
        field_F8_LastLineYPos = field_BC_ypos;

        // TODO: OG bug, dead code due to switch default case ?
        if (field_106_current_motion == eAbeStates::State_71_Knockback_455090 || field_106_current_motion == eAbeStates::State_101_KnockForward_455420)
        {
            field_128.field_8_x_vel_slow_by = FP_FromDouble(0.67); // TODO: Check
        }
    }
}

__int16 Abe::CrouchingGameSpeak_453E10()
{
    field_118_prev_held |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (sInputKey_GameSpeak2_5550F8 & field_118_prev_held)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eFollowMe_10);
        Mudokon_SFX_457EC0(MudSounds::eFollowMe_4, 0, 0, this);
        field_106_current_motion = eAbeStates::State_20_CrouchSpeak_454550;
        return TRUE;
    }
    else if (sInputKey_GameSpeak3_555100 & field_118_prev_held)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eWait_12);
        Mudokon_SFX_457EC0(MudSounds::eWait_6, 0, 0, this);
        field_106_current_motion = eAbeStates::jState_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eHello_9);

        if (field_128.field_12_mood == Mud_Emotion::eHappy_5 || field_128.field_12_mood == Mud_Emotion::eWired_6)
        {
            Mudokon_SFX_457EC0(MudSounds::eHiHappy_19, 0, 0, this);
            field_106_current_motion = eAbeStates::State_20_CrouchSpeak_454550;
        }
        else
        {
            if (field_128.field_12_mood == Mud_Emotion::eSad_3)
            {
                Mudokon_SFX_457EC0(MudSounds::eHiSad_20, 0, 0, this);
            }
            else
            {
                Mudokon_SFX_457EC0(MudSounds::eHelloNeutral_3, 0, 0, this);
            }
            field_106_current_motion = eAbeStates::State_20_CrouchSpeak_454550;
        }
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak4_5550FC)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eWork_21);
        Mudokon_SFX_457EC0(MudSounds::eWork_25, 0, 0, this);
        field_106_current_motion = eAbeStates::jState_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak6_555108)
    {
        Mudokon_SFX_457EC0(MudSounds::eAllOYa_17, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eAllYa_23);
        field_106_current_motion = eAbeStates::jState_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak5_55510C)
    {
        Mudokon_SFX_457EC0(MudSounds::eAnger_5, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eAnger_11);
        field_106_current_motion = eAbeStates::State_20_CrouchSpeak_454550;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak8_555110)
    {
        Mudokon_SFX_457EC0(MudSounds::eStopIt_26, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eStopIt_22);
        field_106_current_motion = eAbeStates::jState_21_ToCrouchSpeak_4545E0;
        return TRUE;
    }
    else if (field_118_prev_held & sInputKey_GameSpeak7_555114)
    {
        Mudokon_SFX_457EC0(MudSounds::eSadUgh_28, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eSorry_24);
        field_106_current_motion = eAbeStates::State_20_CrouchSpeak_454550;
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
        MoveForward_44E9A0();
    }
}

__int16 Abe::RunTryEnterDoor_451220()
{
    // Can't be entering a door if we're not pressing up
    if (!sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
    {
        return 0;
    }

    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
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
        TlvTypes::Door_5);

    if (!pDoorTlv)
    {
        return 0;
    }

    if (!NearDoorIsOpen_44EE10())
    {
        return 0;
    }

    field_FC_pPathTLV = pDoorTlv;
    field_120_state = 0;
    field_106_current_motion = eAbeStates::State_114_DoorEnter_459470;
    field_B8_xpos = FP_FromInteger((pDoorTlv->field_8_top_left.field_0_x + pDoorTlv->field_C_bottom_right.field_0_x) / 2);
    MapFollowMe_408D10(TRUE);
    return 1;
}

__int16 Abe::RunTryEnterWell_451060()
{
    if (!sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8) ||
        field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted) ||
        field_20_animation.field_92_current_frame < 4)
    {
        return 0;
    }

    auto pWellLocal = static_cast<Path_Well_Local*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::LocalWell_8));
    if (pWellLocal)
    {
        if (!(field_114_flags.Get(Flags_114::e114_Bit10_Teleporting)))
        {
            if ((pWellLocal->field_0_scale == 0 && field_CC_sprite_scale == FP_FromInteger(1)) ||
                (pWellLocal->field_0_scale == 1 && field_CC_sprite_scale == FP_FromDouble(0.5)))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_fall_to_well);
                field_FC_pPathTLV = pWellLocal;
                field_106_current_motion = eAbeStates::State_78_WellBegin_45C810;
                return 1;
            }
        }
    }

    auto pWellExpress = static_cast<Path_Well_Express*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::WellExpress_23));
    if (pWellExpress)
    {
        if (!(field_114_flags.Get(Flags_114::e114_Bit10_Teleporting)))
        {
            if ((pWellExpress->field_0_scale == 0 && field_CC_sprite_scale == FP_FromInteger(1)) ||
                (pWellExpress->field_0_scale == 1 && field_CC_sprite_scale == FP_FromDouble(0.5)))
            {
                field_1AC_flags.Clear(Flags_1AC::e1AC_Bit3_fall_to_well);
                field_FC_pPathTLV = pWellExpress;
                field_106_current_motion = eAbeStates::jState_81_WellBegin_45C7F0;
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
        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
        {
            pObj->ClearInvisibility_45FA50();
            field_178_invisible_effect_id = -1;
        }
    }

    field_1AC_flags.Set(Flags_1AC::e1AC_Bit5_shrivel);
    field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
    field_106_current_motion = eAbeStates::State_57_Dead_4589A0;
    field_124_timer = 0;
    field_10C_health = FP_FromInteger(0);

    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
}

short Abe::DoGameSpeak_45AB70(int input)
{
    short nextState = -1;
    if (Input_IsChanting_45F260())
    {
        // Fixes an OG bug where abe doesn't transform into shrykull when you immediately chant after using gamespeak
        if (!(field_168_ring_pulse_timer && field_16C_bHaveShrykull))
        {
            field_124_timer = sGnFrame_5C1B84 + 90;
            SND_SEQ_PlaySeq_4CA960(SeqId::MudokonChant1_10, 0, 1);
            field_120_state = 0;
            nextState = eAbeStates::State_112_Chant_45B1C0;
        }
    }
    else if (input & sInputKey_GameSpeak2_5550F8)
    {
        Mudokon_SFX_457EC0(MudSounds::eFollowMe_4, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eFollowMe_10);
        nextState = eAbeStates::State_7_Speak_45B140;
    }
    else if (input & sInputKey_GameSpeak3_555100)
    {
        Mudokon_SFX_457EC0(MudSounds::eWait_6, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eWait_12);
        nextState = eAbeStates::State_8_Speak_45B160;
    }
    else if (input & sInputKey_GameSpeak1_555104)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eHello_9);
        if (field_128.field_12_mood == Mud_Emotion::eHappy_5 || field_128.field_12_mood == Mud_Emotion::eWired_6)
        {
            Mudokon_SFX_457EC0(MudSounds::eHiHappy_19, 0, 0, this);
            nextState = eAbeStates::State_9_Speak_45B180;
        }
        else
        {
            if (field_128.field_12_mood == Mud_Emotion::eSad_3)
            {
                Mudokon_SFX_457EC0(MudSounds::eHiSad_20, 0, 0, this);
            }
            else
            {
                Mudokon_SFX_457EC0(MudSounds::eHelloNeutral_3, 0, 0, this);
            }
            nextState = eAbeStates::State_9_Speak_45B180;
        }
    }
    else if (input & sInputKey_GameSpeak4_5550FC)
    {
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eWork_21);
        Mudokon_SFX_457EC0(MudSounds::eWork_25, 0, 0, this);
        nextState = eAbeStates::State_10_Fart_45B1A0;
    }
    else if (input & sInputKey_GameSpeak6_555108)
    {
        Mudokon_SFX_457EC0(MudSounds::eAllOYa_17, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eAllYa_23);
        nextState = eAbeStates::State_9_Speak_45B180;
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
                Mudokon_SFX_457EC0(MudSounds::eAnger_5, 0, 0, this);
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eAnger_11);
                nextState = eAbeStates::State_8_Speak_45B160;
            }
        }
    }
    else if (input & sInputKey_GameSpeak8_555110)
    {
        Mudokon_SFX_457EC0(MudSounds::eStopIt_26, 0, 0, this);
        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eStopIt_22);
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

        pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::eSorry_24);
        if (FindObjectOfType_425180(Types::eMudokon_110, field_B8_xpos + gridSize, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(40))))
        {
            nextState = eAbeStates::State_63_Sorry_454670;
        }
        else
        {
            Mudokon_SFX_457EC0(MudSounds::eSadUgh_28, 0, 0, this);
            nextState = eAbeStates::State_10_Fart_45B1A0; // TODO: Correct but isn't fart in this case ??
        }
    }

    return nextState;
}

__int16 Abe::CantBeDamaged_44BAB0()
{
    switch (field_106_current_motion)
    {
    case eAbeStates::State_75_JumpIntoWell_45C7B0:
    case eAbeStates::State_76_ToInsideOfAWellLocal_45CA40:
    case eAbeStates::State_77_ToWellShotOut_45D130:
    case eAbeStates::State_78_WellBegin_45C810:
    case eAbeStates::State_79_InsideWellLocal_45CA60:
    case eAbeStates::State_80_WellShotOut_45D150:
    case eAbeStates::jState_81_WellBegin_45C7F0:
    case eAbeStates::State_82_InsideWellExpress_45CC80:
    case eAbeStates::State_83_WellExpressShotOut_45CF70:
    case eAbeStates::State_114_DoorEnter_459470:
    case eAbeStates::State_115_DoorExit_459A40:
    case eAbeStates::State_119_ToShrykull_45A990:
    case eAbeStates::State_120_EndShrykull_45AB00:
        return TRUE;
    }

    // TODO: Unknown what this is checking, condition should probably be inverted
    if ((!(field_1AC_flags.Get(Flags_1AC::e1AC_Bit5_shrivel)) && field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render)) || field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
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
    PSX_RECT bOurRect = {};
    vGetBoundingRect_424FD0(&bOurRect, 1);
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eMine_88 || pObj->field_4_typeId == Types::eUXB_143)
        {
            PSX_RECT objRect = {};
            pObj->vGetBoundingRect_424FD0(&objRect, 1);

            if (bOurRect.x <= objRect.w &&
                bOurRect.w >= objRect.x &&
                bOurRect.h >= objRect.y &&
                bOurRect.y <= objRect.h)
            {
                pObj->VTakeDamage_408730(this);
            }
        }
    }
}

__int16 Abe::ForceDownIfHoisting_44BA30()
{
    if (field_106_current_motion != eAbeStates::State_65_LedgeAscend_4548E0 &&
        field_106_current_motion != eAbeStates::State_67_LedgeHang_454E20 &&
        field_106_current_motion != eAbeStates::State_69_LedgeHangWobble_454EF0 &&
        field_106_current_motion != eAbeStates::State_66_LedgeDescend_454970 &&
        field_106_current_motion != eAbeStates::State_68_ToOffScreenHoist_454B80)
    {
        return 0;
    }

    field_106_current_motion = eAbeStates::State_92_ForceDownFromHoist_455800;

    field_124_timer = 0;

    field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
    field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);

    return 1;
}

void Abe::BulletDamage_44C980(Bullet* pBullet)
{
    PSX_RECT rect = {};
    vGetBoundingRect_424FD0(&rect, 1);

    enum class ShootKind
    {
        eEverythingElse_0 = 0,
        eHanging_1 = 1,
        eRolling_2 = 2,
    };

    if (Is_In_Current_Camera_424A70() != CameraPos::eCamCurrent_0 ||
        field_10C_health <= FP_FromInteger(0))
    {
        return;
    }

    ShootKind shootKind = ShootKind::eEverythingElse_0;
    if (field_106_current_motion == eAbeStates::State_22_RollBegin_4539A0 ||
        field_106_current_motion == eAbeStates::State_23_RollLoop_453A90 ||
        field_106_current_motion == eAbeStates::State_24_453D00 ||
        field_106_current_motion == eAbeStates::State_37_CrouchTurn_454390 ||
        field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0)
    {
        shootKind = ShootKind::eRolling_2;
    }
    else if (field_106_current_motion == eAbeStates::State_65_LedgeAscend_4548E0 ||
        field_106_current_motion == eAbeStates::State_67_LedgeHang_454E20 ||
        field_106_current_motion == eAbeStates::State_69_LedgeHangWobble_454EF0 ||
        field_106_current_motion == eAbeStates::State_66_LedgeDescend_454970 ||
        field_106_current_motion == eAbeStates::State_68_ToOffScreenHoist_454B80)
    {
        shootKind = ShootKind::eHanging_1;
    }

    field_10C_health -= FP_FromDouble(0.34);
    if (field_10C_health > FP_FromInteger(0))
    {
        FP xoff = {};
        if (field_10C_health <= FP_FromDouble(0.5))
        {
            if (pBullet->field_30 > FP_FromInteger(0))
            {
                xoff = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                xoff = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
            }
        }
        else
        {
            if (pBullet->field_30 > FP_FromInteger(0))
            {
                xoff = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2));
            }
            else
            {
                xoff = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
            }
        }

        PathLine* pathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            xoff,
            field_BC_ypos - FP_FromInteger(5),
            xoff,
            field_BC_ypos + FP_FromInteger(5),
            &pathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16) == 1)
        {
            if (pBullet->field_20_type != BulletType::Type_1 && pBullet->field_20_type != BulletType::ZBullet_3)
            {
                auto pSpark = ae_new<Spark>();
                if (pSpark)
                {
                    pSpark->ctor_4CBBB0(hitX, hitY, field_CC_sprite_scale, 9, -31, 159, 0);
                }
                New_Smoke_Particles_426C70(hitX, hitY, field_CC_sprite_scale, 3, 128u, 128u, 128u);
            }
        }
        return;
    }

    field_114_flags.Set(Flags_114::e114_Bit1_bShot);

    switch (pBullet->field_20_type)
    {
    case BulletType::Type_0:
    case BulletType::Type_2:
    {
        FP bloodXOff = {};
        if (pBullet->field_30 <= FP_FromInteger(0))
        {
            bloodXOff = FP_FromInteger(-24);
        }
        else
        {
            bloodXOff = FP_FromInteger(24);
        }
        auto pBlood = ae_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(
                field_B8_xpos,
                pBullet->field_2C_ypos,
                bloodXOff,
                FP_FromInteger(0),
                field_CC_sprite_scale,
                50
            );
        }

        switch (shootKind)
        {
            case ShootKind::eEverythingElse_0:
            {
                ToKnockback_44E700(1, 1);
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != pBullet->field_30 > FP_FromInteger(0))
                {
                    field_106_current_motion = eAbeStates::State_101_KnockForward_455420;
                }
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(7.8);
                if (pBullet->field_30 < FP_FromInteger(0))
                {
                    field_C4_velx = -field_CC_sprite_scale;
                }
                break;
            }
            case ShootKind::eHanging_1:
            {
                field_106_current_motion = eAbeStates::State_92_ForceDownFromHoist_455800;
                field_124_timer = 0;
                field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
                field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
                break;
            }
            case ShootKind::eRolling_2:
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) == pBullet->field_30 > FP_FromInteger(0))
                {
                    field_108_next_motion = eAbeStates::State_74_RollingKnockback_455290;
                }
                else
                {
                    field_108_next_motion = eAbeStates::State_102_RollingKnockForward_455310;
                }
                break;
            }
            default:
                break;
        }
        break;
    }

    case BulletType::Type_1:
    case BulletType::ZBullet_3:
    {
        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_10C_health = FP_FromInteger(1);
            field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
            return;
        }

        const FP boundsY = FP_FromInteger(rect.y);
        if (Bullet::InZBulletCover(field_B8_xpos, boundsY, rect) ||
            !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, boundsY, 0))
        {
            field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
            field_10C_health = FP_FromInteger(1);
            return;
        }

        FP yOff = {};
        if (shootKind == ShootKind::eEverythingElse_0)
        {
            yOff = (FP_FromInteger(-45) * field_CC_sprite_scale);
            field_108_next_motion = eAbeStates::State_110_ZShot_455670;
        }
        else if (shootKind == ShootKind::eHanging_1)
        {
            yOff = (FP_FromInteger(45) * field_CC_sprite_scale);
            field_106_current_motion = eAbeStates::State_92_ForceDownFromHoist_455800;
            field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
            field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
            field_124_timer = 0;
        }
        else if (shootKind == ShootKind::eRolling_2)
        {
            yOff = (FP_FromInteger(-25) * field_CC_sprite_scale);
            field_108_next_motion = eAbeStates::State_109_ZShotRolling_455550;
        }

        auto pBlood = ae_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, yOff + field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 50);
        }
        break;
    }

    default:
        break;
    }

    if (field_114_flags.Get(Flags_114::e114_Bit1_bShot))
    {
        field_122_knockdown_motion = field_108_next_motion;
    }

    Environment_SFX_457A40(EnvironmentSfx::eElumHitWall_14, 0, 32767, this);
    Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 127, 0, this);
    Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
    SFX_Play_46FBA0(SoundEffect::Eating1_65, 0, -500, field_CC_sprite_scale);
    SFX_Play_46FA90(SoundEffect::KillEffect_64, 0, field_CC_sprite_scale);
}

void Abe::GiveControlBackToMe_44BA10()
{
    sControlledCharacter_5C1B8C = this;
    field_1AC_flags.Set(Flags_1AC::e1AC_Bit6_prevent_chanting);
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
        if (pObj->field_4_typeId == Types::ePullRope_103)
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
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds_5C1B70.Find_449CF0(field_1A8_portal_id));
    if (pBirdPortal)
    {
        PSX_RECT bRect = {};
        switch (field_1A4_portal_sub_state)
        {
        case 0:
            vGetBoundingRect_424FD0(&bRect, 1);
            if ((field_C4_velx > FP_FromInteger(0) && FP_FromInteger(bRect.x) > pBirdPortal->field_2C_xpos) ||
                (field_C4_velx < FP_FromInteger(0) && FP_FromInteger(bRect.w) < pBirdPortal->field_2C_xpos))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                pBirdPortal->Vsub_499610();
                pBirdPortal->VGiveShrukul_499680(TRUE);
                field_1A4_portal_sub_state = 1;
            }

            field_C8_vely += field_CC_sprite_scale * FP_FromDouble(1.8);
            field_B8_xpos += field_C4_velx;
            field_BC_ypos += field_C8_vely;
            return;

        case 1:
            if (pBirdPortal->VStateIs16_499850())
            {
                LevelIds level = {};
                WORD path = 0;
                WORD camera = 0;
                CameraSwapEffects screenChangeEffect = {};
                WORD movieId = 0;

                pBirdPortal->VGetMapChange_499AE0(&level, &path, &camera, &screenChangeEffect, &movieId);
                gMap_5C3030.SetActiveCam_480D30(level, path, camera, screenChangeEffect, movieId, FALSE);
                field_1A4_portal_sub_state = 4;
            }
            break;

        case 2:
            if (pBirdPortal->VIsState20_499A00())
            {
                pBirdPortal->Vsub_499430(0);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_106_current_motion = eAbeStates::State_27_HopBegin_4521C0;
                pBirdPortal->Vsub_499A20();
                field_1A8_portal_id = -1;
            }
            break;

        case 4:
            pBirdPortal->VExitPortal_499870();
            field_1A4_portal_sub_state = 2;

            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pBirdPortal->field_26_side == PortalSide::eLeft_1);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B8_xpos = ScaleToGridSize_4498B0(field_CC_sprite_scale) + pBirdPortal->field_34_exit_x;
            }
            else
            {
                field_B8_xpos = pBirdPortal->field_34_exit_x - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            field_BC_ypos = pBirdPortal->field_38_exit_y;
            field_F8_LastLineYPos = pBirdPortal->field_38_exit_y;
            field_C8_vely = FP_FromInteger(0);
            field_128.field_8_x_vel_slow_by = FP_FromInteger(0);
            break;

        default:
            return;
        }
    }
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
    const FP v20 = Math_SquareRoot_FP_496E90(((v18 * v17) + (gravity * gravity)));
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
        if (pLift->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            VOnTrapDoorOpen();
            field_1AC_flags.Set(Flags_1AC::e1AC_Bit1_lift_point_dead_while_using_lift);
        }
        SetActiveCameraDelayedFromDir_408C40();
    }
}

short Abe::MoveLiftUpOrDown_45A7E0(FP yVelocity)
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (!pLiftPoint)
    {
        return eAbeStates::State_123_LiftGrabIdle_45A6A0;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), yVelocity, 0);
    FollowLift_45A500();

    if (sControlledCharacter_5C1B8C == this &&
        !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)) &&
        field_20_animation.field_92_current_frame != 5)
    {
        return field_106_current_motion;
    }

    if (yVelocity >= FP_FromInteger(0))
    {
        if (yVelocity > FP_FromInteger(0))
        {
            if (pLiftPoint->vOnBottomFloor_4618F0())
            {
                return eAbeStates::State_123_LiftGrabIdle_45A6A0;
            }

            const DWORD pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (sInputKey_Down_5550DC & pressed)
            {
                return eAbeStates::State_125_LiftUseDown_45A7B0;
            }

            if (pressed & sInputKey_Up_5550D8)
            {
                return eAbeStates::State_124_LiftUseUp_45A780;
            }
        }
    }
    else
    {
        if (pLiftPoint->vOnTopFloor_461890())
        {
            return eAbeStates::State_123_LiftGrabIdle_45A6A0;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            return eAbeStates::State_124_LiftUseUp_45A780;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            return eAbeStates::State_125_LiftUseDown_45A7B0;
        }
    }

    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed &&
        pLiftPoint->vOnAnyFloor_461920() &&
        !(pLiftPoint->field_12C_bMoving & 1))
    {
        return eAbeStates::State_122_LiftGrabEnd_45A670;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eAbeStates::State_123_LiftGrabIdle_45A6A0;
}

__int16 Abe::GetEvilFart_4585F0(__int16 bDontLoad)
{
    PSX_RECT abeRect = {};
    vGetBoundingRect_424FD0(&abeRect, 1);

    FartMachine* pBrewMachine = nullptr;
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            return 0;
        }

        if (pObj->field_4_typeId == Types::eBrewMachine_13)
        {
            pBrewMachine = static_cast<FartMachine*>(pObj);

            PSX_RECT bRect = {};
            pBrewMachine->vGetBoundingRect_424FD0(&bRect, 1);

            if (RectsOverlap(abeRect, bRect) &&
                pBrewMachine->field_CC_sprite_scale == field_CC_sprite_scale &&
                pBrewMachine->field_144_total_brew_count > 0 &&
                field_198_has_evil_fart == FALSE)
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

    field_10_resources_array.SetAt(22, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, TRUE, FALSE));
    field_10_resources_array.SetAt(23, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(24, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, TRUE, FALSE));
    pBrewMachine->field_144_total_brew_count--;
    field_198_has_evil_fart = TRUE;

    return 1;
}

EXPORT void Abe::ChangeChantState_45BB90(__int16 bLaughAtChantEnd)
{
    if (bLaughAtChantEnd)
    {
        field_1AC_flags.Set(Flags_1AC::e1AC_Bit9_laugh_at_chant_end);
        field_120_state = 6; // Holds chant, then laughs.
    }
    else if (sControlledCharacter_5C1B8C == this)
    {
        field_120_state = 0; // Chants briefly, then stops.
    }
    else
    {
        field_120_state = 3; // Chants briefly with a possession flicker, then stops.
    }
}

void Abe::SetAsDead_459430()
{
    field_120_state = 1;
}

void Abe::ExitShrykull_45A9D0(__int16 bResetRingTimer)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);

    field_106_current_motion = eAbeStates::State_120_EndShrykull_45AB00;
    field_124_timer = 1;

    if (bResetRingTimer)
    {
        field_168_ring_pulse_timer = 0;
    }
}

static void playAbeSFX(MudSounds idx, __int16 volume, int pitch)
{
    SFX_SfxDefinition_Play_4CA420(
        &sAbeSFXList_555250[static_cast<unsigned __int8>(idx)],
        volume,
        static_cast<short>(pitch), static_cast<short>(pitch)
    );
}

EXPORT void CC Mudokon_SFX_457EC0(MudSounds idx, __int16 volume, int pitch, BaseAliveGameObject* pHero)
{
    switch (idx)
    {
        case MudSounds::eOops_14:
        {
            if (pHero && pHero->field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                SND_SEQ_Play_4CAB10(SeqId::AbeOops_16, 1, 90, 90);
            }
            else
            {
                SND_SEQ_Play_4CAB10(SeqId::AbeOops_16, 1, 127, 127);
            }
            break;
        }
        case MudSounds::eStopIt_26:
        {
            if (pHero && pHero->field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                SND_SEQ_Play_4CAB10(SeqId::AbeStopIt_18, 1, 80, 80);
            }
            else
            {
                SND_SEQ_Play_4CAB10(SeqId::AbeStopIt_18, 1, 115, 115);
            }
            break;
        }
        case MudSounds::eGiggle_8:
        {
            if (pHero == sActiveHero_5C1B68 && gMap_5C3030.field_0_current_level == LevelIds::eBrewery_Ender_10)
            {
                idx = MudSounds::eLaugh_10;
            }
        }
        // Fall through
        default:
        {
            auto idxToVal = static_cast<unsigned __int8>(idx);
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

            if (pHero->field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                volume = 2 * volume / 3;
            }

            if (pHero == sActiveHero_5C1B68)
            {
                playAbeSFX(idx, volume, pitch);
                return;
            }

            switch (gMap_5C3030.GetDirection_4811A0(
                pHero->field_C2_lvl_number,
                pHero->field_C0_path_number,
                pHero->field_B8_xpos,
                pHero->field_BC_ypos))
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
                    SFX_SfxDefinition_Play_4CA700(
                        &sAbeSFXList_555250[idxToVal],
                        2 * volume / 3,
                        2 * volume / 9,
                        static_cast<short>(pitch), static_cast<short>(pitch));
                    break;
                }
                case CameraPos::eCamRight_4:
                {
                    SFX_SfxDefinition_Play_4CA700(
                        &sAbeSFXList_555250[idxToVal],
                        2 * volume / 9,
                        2 * volume / 3,
                        static_cast<short>(pitch), static_cast<short>(pitch));
                    break;
                }
                default:
                    break;
            }
        }
    }
}

#include "stdafx_ao.h"
#include "Function.hpp"
#include "Abe.hpp"
#include "Blood.hpp"
#include "Bullet.hpp"
#include "ThrowableArray.hpp"
#include "Elum.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "CircularFade.hpp"
#include "DeathFadeOut.hpp"
#include "Throwable.hpp"
#include "OrbWhirlWind.hpp"
#include "PullRingRope.hpp"
#include "ScreenManager.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "LiftPoint.hpp"
#include "Well.hpp"
#include "Input.hpp"
#include "ScreenShake.hpp"
#include "Sfx.hpp"
#include "MusicController.hpp"
#include "AbilityRing.hpp"
#include "MusicTrigger.hpp"
#include "Math.hpp"
#include "Input.hpp"
#include "Grenade.hpp"
#include "Collisions.hpp"
#include "BirdPortal.hpp"
#include "Rope.hpp"
#include "BeeSwarm.hpp"
#include "Shrykull.hpp"
#include "Switch.hpp"
#include "GameSpeak.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x5076E4, short, gAbeInvunerableCheat_5076E4, 0);


// TODO: Move out to own file
ALIVE_ARY(1, 0x505668, BYTE, 8192, gSaveBuffer_505668, {});

class Demo
{
public:
    EXPORT static void CC Save_459490(BYTE* /*pBuffer*/)
    {
        // This is actually a game save
        NOT_IMPLEMENTED();
    }
};

using TAbeStateFunction = decltype(&Abe::State_0_Idle_423520);

#define MAKE_STRINGS(VAR) #VAR,
const char* const sAbeStateNames[] =
{
    ABE_STATES_ENUM(MAKE_STRINGS)
};

const TAbeStateFunction sAbeStateMachineTable_4C5F08[] =
{
    &Abe::State_0_Idle_423520,
    &Abe::State_1_WalkLoop_423F90,
    &Abe::State_2_StandingTurn_426040,
    &Abe::State_3_Fall_42E7F0,
    &Abe::State_4_WalkToIdle_4243C0,
    &Abe::State_5_MidWalkToIdle_424490,
    &Abe::State_6_WalkBegin_424300,
    &Abe::State_7_Speak_42F950,
    &Abe::State_8_Speak_42F9D0,
    &Abe::State_9_Speak_42FA50,
    &Abe::State_10_Speak_42FAD0,
    &Abe::State_11_Speak_42FB50,
    &Abe::State_12_Speak_42FBD0,
    &Abe::State_13_Speak_42FC50,
    &Abe::State_14_Speak_42FCD0,
    &Abe::State_15_Null_42A210,
    &Abe::State_16_HoistBegin_426E40,
    &Abe::State_17_HoistIdle_4269E0,
    &Abe::State_18_HoistLand_426EB0,
    &Abe::State_19_CrouchIdle_4284C0,
    &Abe::State_20_CrouchToStand_428AF0,
    &Abe::State_21_StandToCrouch_427F40,
    &Abe::State_22_CrouchSpeak_428A30,
    &Abe::State_23_CrouchSpeak_428A90,
    &Abe::State_24_RollBegin_427A20,
    &Abe::State_25_RollLoop_427BB0,
    &Abe::State_26_RollEnd_427EA0,
    &Abe::State_27_RunSlideStop_425B60,
    &Abe::State_28_RunTurn_425CE0,
    &Abe::State_29_HopBegin_4267B0,
    &Abe::State_30_HopMid_4264D0,
    &Abe::State_31_HopLand_426940,
    &Abe::State_32_RunJumpBegin_427440,
    &Abe::State_33_RunJumpMid_426FA0,
    &Abe::State_34_RunJumpLand_427560,
    &Abe::State_35_RunLoop_425060,
    &Abe::State_36_DunnoBegin_423260,
    &Abe::State_37_DunnoMid_4232C0,
    &Abe::State_38_DunnoEnd_423310,
    &Abe::State_39_CrouchTurn_4288C0,
    &Abe::State_40_RunToRoll_427AE0,
    &Abe::State_41_StandingToRun_425530,
    &Abe::State_42_SneakLoop_424BB0,
    &Abe::State_43_WalkToSneak_424790,
    &Abe::State_44_SneakToWalk_4249A0,
    &Abe::State_45_MidWalkToSneak_424890,
    &Abe::State_46_MidSneakToWalk_424AA0,
    &Abe::State_47_SneakBegin_424ED0,
    &Abe::State_48_SneakToIdle_424F80,
    &Abe::State_49_MidSneakToIdle_424FF0,
    &Abe::State_50_WalkToRun_424560,
    &Abe::State_51_MidWalkToRun_424670,
    &Abe::State_52_RunToWalk_4255E0,
    &Abe::State_53_MidRunToWalk_4256E0,
    &Abe::State_54_RunTurnToRun_425EA0,
    &Abe::State_55_RunTurnToWalk_425F70,
    &Abe::State_56_RunJumpLandRun_4278E0,
    &Abe::State_57_RunJumpLand_Walk_427980,
    &Abe::State_58_ToSpeak_42F8D0,
    &Abe::State_59_DeathDropFall_42CBE0,
    &Abe::State_60_Dead_42C4C0,
    &Abe::State_61_Respawn_42CD20,
    &Abe::State_62_LoadedSaveSpawn_45ADD0,
    &Abe::State_63_TurnToRun_42A0A0,
    &Abe::State_64_LedgeAscend_428B60,
    &Abe::State_65_LedgeDescend_428C00,
    &Abe::State_66_LedgeHang_428D90,
    &Abe::State_67_ToOffScreenHoist_428C50,
    &Abe::State_68_LedgeHangWobble_428E50,
    &Abe::State_69_RingRopePull_4299B0,
    &Abe::State_70_Knockback_428FB0,
    &Abe::State_71_KnockbackGetUp_429240,
    &Abe::State_72_PushWall_4292A0,
    &Abe::State_73_RollingKnockback_4291D0,
    &Abe::State_74_JumpIntoWell_430EC0,
    &Abe::State_75_ToInsideOfAWellLocal_431090,
    &Abe::State_76_ToWellShotOut_431720,
    &Abe::State_77_WellBegin_430F10,
    &Abe::State_78_InsideWellLocal_4310A0,
    &Abe::State_79_WellShotOut_431730,
    &Abe::State_80_430EF0,
    &Abe::State_81_InsideWellExpress_431320,
    &Abe::State_82_WellExpressShotOut_4315A0,
    &Abe::State_83_430F00,
    &Abe::State_84_431080,
    &Abe::State_85_431710,
    &Abe::State_86_FallLandDie_42EDD0,
    &Abe::State_87_428FA0,
    &Abe::State_88_HandstoneBegin_430590,
    &Abe::State_89_HandstoneEnd_430E80,
    &Abe::State_90_GrenadeMachineUse_430EA0,
    &Abe::State_91_FallingFromGrab_429780,
    &Abe::State_92_ForceDownFromHoist_4297C0,
    &Abe::State_93_WalkOffEdge_429840,
    &Abe::State_94_RunOffEdge_429860,
    &Abe::State_95_SneakOffEdge_429880,
    &Abe::State_96_HopToFall_4298A0,
    &Abe::State_97_RunJumpToFall_429930,
    &Abe::State_98_LandSoft_42ED40,
    &Abe::State_99_LedgeHoistUp_426DC0,
    &Abe::State_100_RollOffLedge_429950,
    &Abe::State_101_LeverUse_429970,
    &Abe::State_102_ElumWalkLoop_42DCA0,
    &Abe::State_103_ElumIdle_42DCD0,
    &Abe::State_104_ElumRunSlideStop_42DF00,
    &Abe::State_105_ElumRunTurn_42DF10,
    &Abe::State_106_42DF20,
    &Abe::State_107_ElumHopBegin_42DF30,
    &Abe::State_108_ElumHopMid_42DF40,
    &Abe::State_109_ElumHopLand_42DF50,
    &Abe::State_110_ElumRunJumpBegin_42DF60,
    &Abe::State_111_ElumRunJumpMid_42DF70,
    &Abe::State_112_ElumRunJumpLand_42DF80,
    &Abe::State_113_ElumTurn_42DF90,
    &Abe::State_114_ElumRunLoop_42DFA0,
    &Abe::State_115_ElumSpeak_4299F0,
    &Abe::State_116_42DFB0,
    &Abe::State_117_ElumWalkBegin_42DFC0,
    &Abe::State_118_ElumRunBegin_42DFD0,
    &Abe::State_119_42DFE0,
    &Abe::State_120_ElumRunToWalk_42DFF0,
    &Abe::State_121_ElumMidRunToWalk_42E000,
    &Abe::State_122_ElumRunTurnToRun_42E010,
    &Abe::State_123_ElumRunTurnToWalk_42E020,
    &Abe::State_124_ElumWalkEnd_42DCB0,
    &Abe::State_125_ElumMidWalkEnd_42DCC0,
    &Abe::State_126_ElumBeesStruggling_42E080,
    &Abe::State_127_SlapBomb_429A20,
    &Abe::State_128_KnockForward_429330,
    &Abe::State_129_RollingKnockForward_4294F0,
    &Abe::State_130_KnockForwardGetUp_429560,
    &Abe::State_131_LiftUseUp_42F150,
    &Abe::State_132_LiftUseDown_42F170,
    &Abe::State_133_LiftGrabBegin_42EF20,
    &Abe::State_134_LiftGrabEnd_42EFE0,
    &Abe::State_135_LiftGrabIdle_42F000,
    &Abe::State_136_ElumMountEnd_42E110,
    &Abe::State_137_ElumUnmountBegin_42E2B0,
    &Abe::State_138_ElumUnmountEnd_42E390,
    &Abe::State_139_ElumMountBegin_42E090,
    &Abe::State_140_BeesStruggling_423F30,
    &Abe::State_141_BeesStrugglingOnLift_42F390,
    &Abe::State_142_RockThrowStandingHold_429CE0,
    &Abe::State_143_RockThrowStandingThrow_429FD0,
    &Abe::State_144_RockThrowStandingEnd_429DE0,
    &Abe::State_145_RockThrowCrouchingHold_428930,
    &Abe::State_146_RockThrowCrouchingThrow_4289F0,
    &Abe::State_147_ShotRolling_4295C0,
    &Abe::State_148_Shot_4296A0,
    &Abe::State_149_PickupItem_42A030,
    &Abe::State_150_Chant_42FD50,
    &Abe::State_151_ChantEnd_430530,
    &Abe::State_152_ElumFallOffEdge_42E030,
    &Abe::State_153_ElumFall_42E040,
    &Abe::State_154_ElumLand_42E050,
    &Abe::State_155_ElumJumpToFall_42E060,
    &Abe::State_156_DoorEnter_42D370,
    &Abe::State_157_DoorExit_42D780,
    &Abe::State_158_ElumKnockback_42E070,
    &Abe::State_159_423360,
    &Abe::State_160_4233A0,
    &Abe::State_161_4233E0,
    &Abe::State_162_ToShrykull_42F410,
    &Abe::State_163_ShrykullEnd_42F520,
    &Abe::State_164_PoisonGasDeath_42A120
};

const int sAbeFrameOffsetTable_4C61A0[166] =
{
    55968,
    55888,
    56144,
    55968,
    55868,
    55868,
    55848,
    56108,
    56072,
    56036,
    56000,
    56108,
    56036,
    56072,
    56000,
    255776,
    255484,
    255532,
    255436,
    255600,
    255568,
    255384,
    256660,
    256700,
    255196,
    255216,
    255272,
    256424,
    255868,
    256584,
    256636,
    256544,
    256008,
    255312,
    256028,
    256312,
    255716,
    255688,
    256180,
    255968,
    256272,
    255788,
    256048,
    256252,
    256160,
    256404,
    256292,
    255828,
    255668,
    255848,
    255628,
    255808,
    256232,
    256384,
    255948,
    255648,
    255292,
    255364,
    256136,
    255600,
    255600,
    255600,
    255600,
    256524,
    54208,
    54348,
    54308,
    54308,
    54480,
    54448,
    48272,
    48332,
    48460,
    48404,
    19644,
    19624,
    19624,
    19476,
    19624,
    19624,
    19476,
    19624,
    19624,
    19476,
    19624,
    19624,
    7880,
    5560,
    15484,
    15428,
    15608,
    40652,
    40652,
    40680,
    40708,
    40752,
    40852,
    40836,
    40868,
    40772,
    40816,
    10988,
    144920,
    145132,
    145736,
    145804,
    145000,
    145104,
    145164,
    145196,
    145220,
    145688,
    145240,
    145456,
    145312,
    145512,
    145588,
    145608,
    145548,
    145416,
    145436,
    145568,
    145272,
    145292,
    145648,
    145668,
    145860,
    6360,
    16772,
    16712,
    55968,
    21744,
    21644,
    21696,
    21720,
    21612,
    23568,
    13716,
    34676,
    16256,
    21496,
    21496,
    30440,
    30328,
    30568,
    30512,
    30368,
    23996,
    24044,
    5828,
    9516,
    9564,
    21672,
    21700,
    21724,
    21780,
    17520,
    17568,
    11652,
    9860,
    13152,
    12392,
    8240,
    8280,
    27748,
    0
};


const InputCommands sInputKey_Right_4C6590 = eRight;
const InputCommands sInputKey_Left_4C6594 = eLeft;
const InputCommands sInputKey_Up_4C6598 = eUp;
const InputCommands sInputKey_Down_4C659C = eDown;
const InputCommands sInputKey_Hop_4C65A0 = eHop;
const InputCommands sInputKey_DoAction_4C65A4 = eDoAction;
const InputCommands sInputKey_Run_4C65A8 = eRun;
const InputCommands sInputKey_Sneak_4C65AC = eSneak;
const InputCommands sInputKey_FartRoll_4C65B0 = eFartOrRoll;
const InputCommands sInputKey_ThrowItem_4C65B4 = eThrowItem;

const InputCommands dword_4C65B8 = eGameSpeak6;
const InputCommands sInputKey_GameSpeak1_4C65C8 = eHop;
const InputCommands sInputKey_GameSpeak2_4C65BC = eDoAction;
const InputCommands sInputKey_GameSpeak3_4C65C0 = eThrowItem;
const InputCommands sInputKey_GameSpeak4_4C65C4 = eFartOrRoll;

const InputCommands dword_4C65DC = eGameSpeak3;
const InputCommands sInputKey_GameSpeak5_4C65EC = eFartOrRoll;
const InputCommands sInputKey_GameSpeak6_4C65E8 = eHop;
const InputCommands sInputKey_GameSpeak7_4C65E4 = eThrowItem;
const InputCommands sInputKey_GameSpeak8_4C65E0 = eDoAction;


ALIVE_VAR(1, 0x507678, Abe*, sActiveHero_507678, nullptr);
ALIVE_VAR(1, 0x50767C, BaseAliveGameObject*, sControlledCharacter_50767C, nullptr);

int CC Abe_SFX_2_42A220(unsigned __int8 /*sfxId*/, signed int /*volume*/, int /*pitchMin*/,
                        BaseAliveGameObject * /*pAliveObj*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int CC Abe_SFX_42A4D0(unsigned __int8 /*idx*/, int /*volume*/, int /*pitch*/, BaseAliveGameObject * /*pHero*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int CC XGrid_Index_To_XPos_41FA60(FP scale, int xGridIndex)
{
    if (scale == FP_FromDouble(0.5))
    {
        // 12.5 = half grid size
        return 13 * xGridIndex + 245;
    }

    if (scale == FP_FromInteger(1))
    {
        // 25 = full grid size
        return 25 * xGridIndex + 240;
    }

    // Default to middle of the screen
    return 440;
}

BaseGameObject* Abe::VDestructor(signed int flags)
{
    return vdtor_422A70(flags);
}

void Abe::VUpdate()
{
    vUpdate_41FDB0();
}

void Abe::VRender(int** pOrderingTable)
{
    vRender_420F30(pOrderingTable);
}

void Abe::VScreenChanged()
{
    vScreenChanged_422640();
}


void Abe::VOn_TLV_Collision(Path_TLV *pTlv)
{
    VOn_Tlv_Collision_421130(pTlv);
}


void Abe::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_42EED0();
}

void Abe::VOnTrapDoorOpen_42EED0()
{
    if (field_F8_pLiftPoint)
    {
        if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
        {
            VSetMotion(eAbeStates::State_93_WalkOffEdge_429840);
        }

        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
        field_E8_LastLineYPos = field_AC_ypos;
    }
}

const TintEntry sTintTable_Abe_4C6438[] =
{
    { 5, 25u, 25u, 25u },
    { 6, 25u, 25u, 25u },
    { 8, 125u, 125u, 95u },
    { 9, 120u, 120u, 90u },
    { -1, 102u, 102u, 102u }
};


Abe* Abe::ctor_420770(int frameTableOffset, int /*r*/, int /*g*/, int /*b*/)
{
    ctor_401090();

    SetVTable(this, 0x4BB158);

    field_4_typeId = Types::eAbe_43;

    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);

    Init_GameStates_41CEC0();
    
    // Zero out the resource array
    field_1A4_resources = {};

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEBSIC.BAN", nullptr, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0);
    }

    field_1A4_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 0, 0);

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEBSIC1.BAN", nullptr, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 1, 0);
    }

    field_1A4_resources.res[45] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 0, 0);

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 43, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEEDGE.BAN", nullptr, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 43, 1, 0);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 27, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEKNFD.BAN", nullptr, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 27, 1, 0);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEOMM.BAN", nullptr, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 1, 0);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 26, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEKNBK.BAN", nullptr, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 26, 1, 0);
    }

    ResourceManager::LoadResourceFile_455270("ABENOELM.BND", nullptr, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 20, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 19, 1, 0);
    ResourceManager::LoadResourceFile_455270("OMMFLARE.BAN", nullptr, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 312, 1, 0);
    ResourceManager::LoadResourceFile_455270("SQBSMK.BAN", nullptr, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 354, 1, 0);
    ResourceManager::LoadResourceFile_455270("BLOODROP.BAN", nullptr, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 366, 1, 0);
    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2035, 0, 0))
    {
        ResourceManager::LoadResourceFile_455270("SHADOW.BAN", nullptr, 0);
    }
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2035, 1, 0);
    ResourceManager::LoadResourceFile_455270("DEADFLR.BAN", nullptr, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 349, 1, 0);
    ResourceManager::LoadResourceFile_455270("DOVBASIC.BAN", nullptr, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 1, 0);
    
    field_128 = 45;
    Animation_Init_417FD0(frameTableOffset, 135, 80, field_1A4_resources.res[45], 1);

    field_10_anim.field_1C_fn_ptrs = kAbe_Anim_Frame_Fns_4CEBEC;

    PSX_Point pPoint = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&pPoint);
    field_A8_xpos = FP_FromInteger(pPoint.field_0_x + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 4));
    field_AC_ypos = FP_FromInteger(pPoint.field_2_y + 240);
    field_120 = FP_FromInteger(0);
    field_124 = 0;
    field_E8_LastLineYPos = field_AC_ypos;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_F4_pLine = nullptr;
    field_FC_current_motion = eAbeStates::State_3_Fall_42E7F0;

    field_112_prev_motion = -1;
    field_10_anim.field_C_layer = 32;
    field_15C_pThrowable = nullptr;
    field_19C_throwable_count = 0;
    field_198_pThrowable = nullptr;
    field_1A0_portal = 0;
    field_158_pDeathFadeout = nullptr;
    field_164_pCircularFade = nullptr;
    field_188_pOrbWhirlWind = nullptr;
    field_18C_pObjToPosses = nullptr;
    field_138_zone_top_left = {};
    field_13C_zone_bottom_right = {};
    field_140_saved_camera = -1;
    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    // TODO: Bitflags
    field_2AA_flags &= ~0x1Fu;

    field_14C_saved_sprite_scale = field_BC_sprite_scale;
    field_144_saved_level = gMap_507BA8.field_0_current_level;
    field_142_saved_path = -1;
    field_148_clear_from_id = 0;
    field_14A_clear_to_id = 0;
    field_146_zone_number = 0;
    field_150_saved_ring_timer = 0;
    field_154_bSavedHaveShrykull = 0;

    field_2A8_flags.Raw().all = 0;
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit8);

    // Changes Abe's "default" colour depending on the level we are in
    SetTint_418750(sTintTable_Abe_4C6438, gMap_507BA8.field_0_current_level);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);   
    field_10_anim.field_B_render_mode = 0;

    field_10C_prev_held = 0; // lowest to base class
    field_E6_last_anim_frame = 0;
    field_110_state = 0;
    field_168_ring_pulse_timer = 0;
    field_114_gnFrame = gnFrameCount_507670;
    field_F0_pTlv = nullptr;
    field_160_pRope = nullptr;
    field_F8_pLiftPoint = nullptr;
    field_130_say = -1;
    field_134_auto_say_timer = 0;
    field_EC = 1;
    field_12A = 161;

    // Set Abe to be the current player controlled object
    sControlledCharacter_50767C = this;

    // Create shadow
    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* Abe::dtor_420C80()
{
    SetVTable(this, 0x4BB158);
    
    SND_Seq_Stop_477A60(11u);

    BYTE** ppRes = nullptr;

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 0, 0);
    if (ppRes)
    {
        if (field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
        }
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 0, 0);
    if (ppRes)
    {
        if (field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
        }
    }

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 43, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 20, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 26, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 27, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 19, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 312, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 366, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2035, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 349, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 354, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    if (field_158_pDeathFadeout)
    {
        field_158_pDeathFadeout->field_C_refCount--;
        field_158_pDeathFadeout->field_6_flags.Set(Options::eDead_Bit3);
        field_158_pDeathFadeout = nullptr;
    }

    if (field_15C_pThrowable)
    {
        field_15C_pThrowable->field_C_refCount--;
        field_15C_pThrowable->field_6_flags.Set(Options::eDead_Bit3);
        field_15C_pThrowable = nullptr;
    }

    if (field_160_pRope)
    {
        field_160_pRope->field_C_refCount--;
        field_160_pRope->field_6_flags.Set(Options::eDead_Bit3);
        field_160_pRope = nullptr;
    }

    if (field_164_pCircularFade)
    {
        field_164_pCircularFade->field_C_refCount--;
        field_164_pCircularFade->field_6_flags.Set(Options::eDead_Bit3);
        field_164_pCircularFade = nullptr;
    }

    if (field_188_pOrbWhirlWind)
    {
        field_188_pOrbWhirlWind->field_C_refCount--;
        field_188_pOrbWhirlWind->field_6_flags.Set(Options::eDead_Bit3);
        field_188_pOrbWhirlWind = nullptr;
    }

    if (field_18C_pObjToPosses)
    {
        field_18C_pObjToPosses->field_C_refCount--;
        field_18C_pObjToPosses->field_6_flags.Set(Options::eDead_Bit3);
        field_18C_pObjToPosses = nullptr;
    }

    if (field_198_pThrowable)
    {
        field_198_pThrowable->field_C_refCount--;
        field_198_pThrowable->field_6_flags.Set(Options::eDead_Bit3);
        field_198_pThrowable = nullptr;
    }

    sActiveHero_507678 = nullptr;

    return dtor_401000();
}

BaseGameObject* Abe::vdtor_422A70(signed int flags)
{
    dtor_420C80();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

const unsigned int sAbe_xVel_table_4BB118[8] = { 262144, 262144, 0, 4294705152, 4294705152, 4294705152, 0, 262144 };
const unsigned int sAbe_yVel_table_4BB138[8] = { 0, 4294705152, 4294705152, 4294705152, 0, 262144, 262144, 262144 };



void Abe::vUpdate_41FDB0()
{
    if (!(field_2AA_flags & 1))
    {
        if (gAbeInvunerableCheat_5076E4)
        {
            field_100_health = FP_FromInteger(1);
        }
        
        if (!Input_IsChanting_4334C0())
        {
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit7);
        }

        if (sDDCheat_FlyingEnabled_50771C && sControlledCharacter_50767C == this)
        {
            VOnTrapDoorOpen();

            const FP old_y = field_AC_ypos;
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit6_bShrivel);
            field_E8_LastLineYPos = old_y;
            field_FC_current_motion = eAbeStates::State_87_428FA0;
            field_F4_pLine = nullptr;

            if (sInputObject_5009E8.isPressed(0xF000u))
            {
                const int dir = sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_2 >> 5;
                field_B4_velx = FP_FromRaw(sAbe_xVel_table_4BB118[dir] * 2);
                field_B8_vely = FP_FromRaw(sAbe_yVel_table_4BB138[dir]);

                if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
                {
                    field_B4_velx += FP_FromRaw(sAbe_xVel_table_4BB118[dir]);
                    field_B8_vely += FP_FromRaw(sAbe_yVel_table_4BB138[dir]);
                }

                field_A8_xpos += field_B4_velx;
                field_AC_ypos += field_B8_vely;

                PSX_Point mapSize = {};
                gMap_507BA8.Get_map_size_444870(&mapSize);

                if (field_A8_xpos < FP_FromInteger(0))
                {
                    field_A8_xpos = FP_FromInteger(0);
                }

                if (field_A8_xpos >= FP_FromInteger(mapSize.field_0_x))
                {
                    field_A8_xpos = FP_FromInteger(mapSize.field_0_x) - FP_FromInteger(1);
                }

                if (field_AC_ypos < FP_FromInteger(0))
                {
                    field_AC_ypos = FP_FromInteger(0);
                }

                if (field_AC_ypos >= FP_FromInteger(mapSize.field_2_y))
                {
                    field_AC_ypos = FP_FromInteger(mapSize.field_2_y) - FP_FromInteger(1);
                    SetActiveCameraDelayedFromDir_401C90();
                    return;
                }
            }
            else
            {
                field_B4_velx = FP_FromInteger(0);
                field_B8_vely = FP_FromInteger(0);
            }
            SetActiveCameraDelayedFromDir_401C90();
        }
        else
        {
            field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);


             // Execute the current state
            __int16 state_idx = field_FC_current_motion;
            const FP oldX = field_A8_xpos;
            const FP oldY = field_AC_ypos;
            (this->*sAbeStateMachineTable_4C5F08[state_idx])();

            if (field_F4_pLine)
            {
                // Snap to a whole number so we are "on" the line
                field_AC_ypos = FP_NoFractional(field_AC_ypos);
            }

            if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
            {
                // Did position change?
                if (oldX != field_A8_xpos ||
                    oldY != field_AC_ypos ||
                    sControlledCharacter_50767C == gElum_507680)
                {
                    // Get the TLV we are on
                    field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
                        nullptr,
                        field_A8_xpos,
                        field_AC_ypos,
                        field_A8_xpos,
                        field_AC_ypos);

                    VOn_TLV_Collision(field_F0_pTlv);
                }

                if (field_106_shot)
                {
                    state_idx = field_112_prev_motion;
                    ToKnockback_422D90(1, 0);
                    field_FC_current_motion = state_idx;
                    field_FE_next_state = 0;
                    field_112_prev_motion = 0;
                    field_108_bMotionChanged = 1;
                    field_106_shot = 0;
                }

                if (Event_Get_417250(kEventScreenShake_19))
                {
                    if (IsStanding_41FC10())
                    {
                        ToKnockback_422D90(1, 0);
                    }
                }

                if (state_idx != field_FC_current_motion || field_108_bMotionChanged)
                {
                    field_108_bMotionChanged = FALSE;

                    if (field_FC_current_motion != eAbeStates::State_15_Null_42A210 && !field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
                    {
                        field_10_anim.Set_Animation_Data_402A40(
                            sAbeFrameOffsetTable_4C61A0[field_FC_current_motion],
                            StateToAnimResource_4204F0(field_FC_current_motion));
                        
                        field_12C_timer = gnFrameCount_507670;

                        if (state_idx == eAbeStates::State_15_Null_42A210 ||
                            state_idx == eAbeStates::State_62_LoadedSaveSpawn_45ADD0)
                        {
                            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
                        }
                    }
                }
                else if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit2))
                {
                    field_FC_current_motion = field_E4;
                    field_10_anim.Set_Animation_Data_402A40(
                        sAbeFrameOffsetTable_4C61A0[field_FC_current_motion],
                        StateToAnimResource_4204F0(field_FC_current_motion));

                    field_12C_timer = gnFrameCount_507670;
                    field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
                    field_2A8_flags.Clear(Flags_2A8::e2A8_Bit2);
                }

                if (field_11C_regen_health_timer <= static_cast<int>(gnFrameCount_507670) &&
                    field_100_health > FP_FromInteger(0))
                {
                    field_100_health = FP_FromInteger(1);
                }

                if (field_168_ring_pulse_timer)
                {
                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
                    {
                        if (static_cast<int>(gnFrameCount_507670) <= field_168_ring_pulse_timer || field_16C_bHaveShrykull)
                        {
                            if (!(gnFrameCount_507670 % 32))
                            {
                                PSX_RECT bRect = {};
                                VGetBoundingRect(&bRect, 1);
                                AbilityRing::Factory_447590(
                                    FP_FromInteger((bRect.w + bRect.x) / 2),
                                    FP_FromInteger((bRect.h + bRect.y) / 2),
                                    field_16C_bHaveShrykull != 0 ? 4 : 0);
                                SFX_Play_43AE60(SoundEffect::PossessEffect_21, 25, 2650, 0);
                            }
                        }
                        else
                        {
                            field_168_ring_pulse_timer = 0;
                        }
                    }
                }

                if (Event_Get_417250(kEvent_15))
                {
                    field_130_say = 16;
                    field_134_auto_say_timer = gnFrameCount_507670 + Math_RandomRange_450F20(22, 30);
                    auto pMusicTrigger = ao_new<MusicTrigger>();
                    if (pMusicTrigger)
                    {
                        pMusicTrigger->ctor_443A60(1, 1, 0, 90);
                    }
                }

                if (field_130_say >= 0 && static_cast<int>(gnFrameCount_507670) >= field_134_auto_say_timer)
                {
                    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos,
                        0))
                    {
                        if (field_FC_current_motion != eAbeStates::State_150_Chant_42FD50
                            && field_FC_current_motion != eAbeStates::State_7_Speak_42F950
                            && field_FC_current_motion != eAbeStates::State_8_Speak_42F9D0
                            && field_FC_current_motion != eAbeStates::State_9_Speak_42FA50
                            && field_FC_current_motion != eAbeStates::State_10_Speak_42FAD0
                            && field_FC_current_motion != eAbeStates::State_11_Speak_42FB50
                            && field_FC_current_motion != eAbeStates::State_12_Speak_42FBD0
                            && field_FC_current_motion != eAbeStates::State_13_Speak_42FC50
                            && field_FC_current_motion != eAbeStates::State_14_Speak_42FCD0)
                        {
                            if (field_FC_current_motion == eAbeStates::State_0_Idle_423520 || field_FC_current_motion == eAbeStates::State_15_Null_42A210)
                            {
                                field_108_bMotionChanged = TRUE;
                                if (field_130_say == 5 || field_130_say == 16)
                                {
                                    field_FC_current_motion = eAbeStates::State_10_Speak_42FAD0;
                                }
                                else
                                {
                                    field_FC_current_motion = eAbeStates::State_9_Speak_42FA50;
                                }
                            }
                            if (field_130_say == 5)
                            {
                                Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
                            }
                            Abe_SFX_42A4D0(static_cast<unsigned char>(field_130_say), 0, 0, this);
                        }
                    }
                    field_130_say = -1;
                }

                if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone))
                {
                    if (field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabinaDone))
                    {
                        if (!field_2A8_flags.Get(Flags_2A8::e2A8_eBit15_bGiveShrykullFromBigFace) &&
                            gMap_507BA8.field_A_level == LevelIds::eLines_2)
                        {
                            field_2A8_flags.Set(Flags_2A8::e2A8_eBit15_bGiveShrykullFromBigFace);
                        }
                    }
                }

                if (field_2A8_flags.Get(Flags_2A8::e2A8_eBit15_bGiveShrykullFromBigFace))
                {
                    if (!field_2A8_flags.Get(Flags_2A8::e2A8_eBit14_bGotShrykullFromBigFace) &&
                        gMap_507BA8.field_0_current_level == LevelIds::eLines_2)
                    {
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 117, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("ABEMORPH.BAN", 0, 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 117, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("SHRMORPH.BAN", 0, 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("SPLINE.BAN", 0, 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 1, 0);
                        }
                        field_16C_bHaveShrykull = TRUE;
                        field_168_ring_pulse_timer = gnFrameCount_507670 + 32000;
                        field_2A8_flags.Set(Flags_2A8::e2A8_eBit14_bGotShrykullFromBigFace);
                    }
                }
            }
        }
    }
}

void Abe::vRender_420F30(int** pOrderingTable)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!(field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel)))
    {
        field_10_anim.field_14_scale = field_BC_sprite_scale;
    }

    if (field_FC_current_motion != eAbeStates::State_15_Null_42A210 &&
        field_FC_current_motion != eAbeStates::State_78_InsideWellLocal_4310A0 &&
        field_FC_current_motion != eAbeStates::State_81_InsideWellExpress_431320 &&
        field_FC_current_motion != eAbeStates::State_84_431080 &&
        field_FC_current_motion != eAbeStates::State_75_ToInsideOfAWellLocal_431090)
    {
        VRender_417DA0(pOrderingTable);
    }
}

void CC Abe::Free_Shrykull_Resources_42F4C0()
{
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 117, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 0, 0));
}

void Abe::FreeElumRes_420F80()
{
    if (field_FC_current_motion == eAbeStates::State_139_ElumMountBegin_42E090)
    {
        field_2AA_flags |= 4u;
    }
    else
    {
        field_2AA_flags &= ~4u;
    }

    if (field_FC_current_motion == eAbeStates::State_136_ElumMountEnd_42E110)
    {
        field_2AA_flags |= 8u;
    }
    else
    {
        field_2AA_flags &= ~8u;
    }

    if (field_FC_current_motion == eAbeStates::State_137_ElumUnmountBegin_42E2B0)
    {
        field_2AA_flags |= 0x10u;
    }
    else
    {
        field_2AA_flags &= ~0x10u;
    }

    ElumFree_4228F0();

    while (!ResourceManager::FreeResource_455550(field_1A4_resources.res[58]))
    {
        // Empty
    }
 
    field_1A4_resources.res[58] = nullptr;
    ResourceManager::FreeResource_455550(field_1A4_resources.res[46]);

    field_1A4_resources.res[46] = nullptr;
    if (gElum_507680)
    {
        gElum_507680->Vsub_411200();
    }

    if (field_FC_current_motion != eAbeStates::State_138_ElumUnmountEnd_42E390)
    {
        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile("ABEBSIC.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 0, 0))
        {
            field_104_pending_resource_count++;
            ResourceManager::LoadResourceFile("ANEPRMNT.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        }

        if (gElum_507680)
        {
            if (gElum_507680->field_144)
            {
                field_104_pending_resource_count++;
                ResourceManager::LoadResourceFile("ELMPRMNT.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);

                if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 230, 0, 0))
                {
                    field_104_pending_resource_count++;
                    ResourceManager::LoadResourceFile("ELMALONE.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
                }
            }
        }

        field_104_pending_resource_count++;
        ResourceManager::LoadResourceFile("ABENOELM.BND", BaseAliveGameObject::OnResourceLoaded_4019A0, this);

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 0, 0))
        {
            field_104_pending_resource_count++;
            ResourceManager::LoadResourceFile("ABEOMM.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        }
    }
}

void Abe::ToDeathDropFall_42C3D0()
{
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit6_bShrivel);
    field_FC_current_motion = eAbeStates::State_59_DeathDropFall_42CBE0;
    field_114_gnFrame = 0;
    field_100_health = FP_FromInteger(0);
    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 1, 0);
}

BOOL Abe::IsStanding_41FC10()
{
    return field_FC_current_motion == eAbeStates::State_0_Idle_423520
        || field_FC_current_motion == eAbeStates::State_1_WalkLoop_423F90
        || field_FC_current_motion == eAbeStates::State_27_RunSlideStop_425B60
        || field_FC_current_motion == eAbeStates::State_28_RunTurn_425CE0
        || field_FC_current_motion == eAbeStates::State_35_RunLoop_425060
        || field_FC_current_motion == eAbeStates::State_2_StandingTurn_426040
        || field_FC_current_motion == eAbeStates::State_36_DunnoBegin_423260
        || field_FC_current_motion == eAbeStates::State_37_DunnoMid_4232C0
        || field_FC_current_motion == eAbeStates::State_38_DunnoEnd_423310
        || field_FC_current_motion == eAbeStates::State_4_WalkToIdle_4243C0
        || field_FC_current_motion == eAbeStates::State_5_MidWalkToIdle_424490
        || field_FC_current_motion == eAbeStates::State_6_WalkBegin_424300
        || field_FC_current_motion == eAbeStates::State_41_StandingToRun_425530
        || field_FC_current_motion == eAbeStates::State_42_SneakLoop_424BB0
        || field_FC_current_motion == eAbeStates::State_43_WalkToSneak_424790
        || field_FC_current_motion == eAbeStates::State_44_SneakToWalk_4249A0
        || field_FC_current_motion == eAbeStates::State_45_MidWalkToSneak_424890
        || field_FC_current_motion == eAbeStates::State_46_MidSneakToWalk_424AA0
        || field_FC_current_motion == eAbeStates::State_47_SneakBegin_424ED0
        || field_FC_current_motion == eAbeStates::State_48_SneakToIdle_424F80
        || field_FC_current_motion == eAbeStates::State_49_MidSneakToIdle_424FF0
        || field_FC_current_motion == eAbeStates::State_50_WalkToRun_424560
        || field_FC_current_motion == eAbeStates::State_51_MidWalkToRun_424670
        || field_FC_current_motion == eAbeStates::State_52_RunToWalk_4255E0
        || field_FC_current_motion == eAbeStates::State_53_MidRunToWalk_4256E0
        || field_FC_current_motion == eAbeStates::State_54_RunTurnToRun_425EA0
        || field_FC_current_motion == eAbeStates::State_55_RunTurnToWalk_425F70
        || field_FC_current_motion == eAbeStates::State_58_ToSpeak_42F8D0
        || field_FC_current_motion == eAbeStates::State_7_Speak_42F950
        || field_FC_current_motion == eAbeStates::State_8_Speak_42F9D0
        || field_FC_current_motion == eAbeStates::State_9_Speak_42FA50
        || field_FC_current_motion == eAbeStates::State_10_Speak_42FAD0
        || field_FC_current_motion == eAbeStates::State_11_Speak_42FB50
        || field_FC_current_motion == eAbeStates::State_12_Speak_42FBD0
        || field_FC_current_motion == eAbeStates::State_13_Speak_42FC50
        || field_FC_current_motion == eAbeStates::State_14_Speak_42FCD0
        || field_FC_current_motion == eAbeStates::State_101_LeverUse_429970
        || field_FC_current_motion == eAbeStates::State_140_BeesStruggling_423F30
        || field_FC_current_motion == eAbeStates::State_143_RockThrowStandingThrow_429FD0
        || field_FC_current_motion == eAbeStates::State_142_RockThrowStandingHold_429CE0
        || field_FC_current_motion == eAbeStates::State_144_RockThrowStandingEnd_429DE0
        || field_FC_current_motion == eAbeStates::State_150_Chant_42FD50
        || field_FC_current_motion == eAbeStates::State_151_ChantEnd_430530
        || field_FC_current_motion == eAbeStates::State_159_423360
        || field_FC_current_motion == eAbeStates::State_160_4233A0
        || field_FC_current_motion == eAbeStates::State_161_4233E0
        || field_FC_current_motion == eAbeStates::State_164_PoisonGasDeath_42A120;
}

void Abe::FollowLift_42EE90()
{
    if (field_F8_pLiftPoint)
    {
        field_B8_vely = field_F8_pLiftPoint->field_B8_vely;
        if (field_F8_pLiftPoint->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_F8_pLiftPoint->VOnPickUpOrSlapped();
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit1);
        }
        SetActiveCameraDelayedFromDir_401C90();
    }
}

void Abe::ExitShrykull_42F440(__int16 bResetRingTimer)
{
    field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_FC_current_motion = eAbeStates::State_163_ShrykullEnd_42F520;

    field_108_bMotionChanged = 1;
    field_114_gnFrame = 1;

    if (bResetRingTimer)
    {
        field_168_ring_pulse_timer = 0;
    }
}

__int16 Abe::RunTryEnterWell_425880()
{
    if (!sInputObject_5009E8.isPressed(sInputKey_Up_4C6598) ||
        field_10_anim.field_92_current_frame < 4)
    {
        return 0;
    }

    auto pWellLocal = static_cast<Path_Well_Local*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::LocalWell_11));
    if (pWellLocal)
    {
        if (pWellLocal->field_18_scale == 0 && field_BC_sprite_scale == FP_FromInteger(1) ||
            pWellLocal->field_18_scale == 1 && field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_Fall_To_Well);
            field_F0_pTlv = pWellLocal;
            field_FC_current_motion = eAbeStates::State_77_WellBegin_430F10;
            return 1;
        }
    }

    auto pWellExpress = static_cast<Path_Well_Express*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::WellExpress_45));
    if (pWellExpress)
    {
        if (pWellExpress->field_18_scale == 0 && field_BC_sprite_scale == FP_FromInteger(1) ||
            pWellExpress->field_18_scale == 1 && field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_Fall_To_Well);
            field_F0_pTlv = pWellExpress;
            field_FC_current_motion = eAbeStates::State_80_430EF0;
        }
    }

    return 0;
}

void Abe::sub_430510(__int16 a2)
{
    if (a2)
    {
        field_110_state = 6;
    }
    else
    {
        field_110_state = 0;
    }
}

BaseAliveGameObject* CC Abe::FindObjectToPosses_421410()
{
    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            if (pObj->field_10A_flags.Get(Flags_10A::e10A_Bit1_Can_Be_Possessed))
            {
                if (pObj->field_4_typeId != Types::eSlig_88 ||
                    pObj->Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0 &&
                    pObj->field_100_health > FP_FromInteger(0))
                {
                    return pObj;
                }
            }
        }
    }
    return nullptr;
}

void CC Abe::Get_Shrykull_Resources_42F480()
{
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 117, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 1, 0);
}

void Abe::ToDieFinal_42C400()
{
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit6_bShrivel);
    field_FC_current_motion = eAbeStates::State_60_Dead_42C4C0;
    field_114_gnFrame = 0;
    field_100_health = FP_FromInteger(0);
    MusicController::sub_443810(MusicController::MusicTypes::eType14, this, 1, 0);
}

void Abe::ToKnockback_422D90(__int16 bUnknownSound, __int16 bDelayedAnger)
{
    if (sControlledCharacter_50767C->field_4_typeId != Types::eSlig_88 || field_100_health <= FP_FromInteger(0))
    {
        SND_Seq_Stop_477A60(11u);
        field_2AA_flags &= ~0x1Cu;

        if (field_188_pOrbWhirlWind)
        {
            field_188_pOrbWhirlWind->ToStop_48BBB0();
            field_188_pOrbWhirlWind = nullptr;
        }

        if (field_B4_velx != FP_FromInteger(0))
        {
            field_A8_xpos -= field_B4_velx;
        }

        MapFollowMe_401D30(1);

        field_B4_velx = FP_FromInteger(0);
        if (field_B8_vely < FP_FromInteger(0))
        {
            field_B8_vely = FP_FromInteger(0);
        }

        field_114_gnFrame = gnFrameCount_507670 + 10;

        if (bUnknownSound)
        {
            Abe_SFX_42A4D0(10u, 0,  Math_RandomRange_450F20(-127, 127), this);
            Abe_SFX_2_42A220(13u, 0, 0x7FFF, this);
        }

        if (!field_1A4_resources.res[0] && !field_104_pending_resource_count)
        {
            sControlledCharacter_50767C = this;
            field_E8_LastLineYPos = field_AC_ypos;
            field_F4_pLine = nullptr;
            field_AC_ypos -= (field_BC_sprite_scale * FP_FromInteger(20));
            VOnTrapDoorOpen();
            FreeElumRes_420F80();
        }

        field_FC_current_motion = eAbeStates::State_70_Knockback_428FB0;

        if (bDelayedAnger)
        {
            field_130_say = 5;
            field_134_auto_say_timer = gnFrameCount_507670 + 27;
        }

        if (field_198_pThrowable)
        {
            field_198_pThrowable->VToDead();
            field_198_pThrowable = nullptr;
            if (gInfiniteGrenades_5076EC == 0)
            {
                field_19C_throwable_count++;
            }
        }
    }
}

BYTE** Abe::StateToAnimResource_4204F0(short motion)
{
    short res_idx = 0;

    if (motion < eAbeStates::State_15_Null_42A210)
    {
        res_idx = 45;
    }
    else if (motion < eAbeStates::State_64_LedgeAscend_428B60)
    {
        res_idx = 0;
    }
    else if (motion < eAbeStates::State_70_Knockback_428FB0)
    {
        res_idx = 32;
    }
    else if (motion < eAbeStates::State_74_JumpIntoWell_430EC0)
    {
        res_idx = 16;
    }
    else if (motion < eAbeStates::State_86_FallLandDie_42EDD0)
    {
        res_idx = 37;
    }
    else if (motion < eAbeStates::State_87_428FA0)
    {
        res_idx = 9;
    }
    else if (motion < eAbeStates::State_88_HandstoneBegin_430590)
    {
        res_idx = 10;
    }
    else if (motion < eAbeStates::State_91_FallingFromGrab_429780)
    {
        res_idx = 11;
    }
    else if (motion < eAbeStates::State_101_LeverUse_429970)
    {
        res_idx = 33;
    }
    else if (motion < eAbeStates::State_102_ElumWalkLoop_42DCA0)
    {
        res_idx = 1;
    }
    else if (motion < eAbeStates::State_127_SlapBomb_429A20)
    {
        res_idx = 46;
    }
    else if (motion < eAbeStates::State_128_KnockForward_429330)
    {
        res_idx = 3;
    }
    else if (motion < eAbeStates::State_131_LiftUseUp_42F150)
    {
        res_idx = 17;
    }
    else if (motion < eAbeStates::State_136_ElumMountEnd_42E110)
    {
        res_idx = 43;
    }
    else if (motion < eAbeStates::State_137_ElumUnmountBegin_42E2B0)
    {
        res_idx = 56;
    }
    else if (motion < eAbeStates::State_138_ElumUnmountEnd_42E390)
    {
        res_idx = 58;
    }
    else if (motion < eAbeStates::State_139_ElumMountBegin_42E090)
    {
        res_idx = 59;
    }
    else if (motion < eAbeStates::State_140_BeesStruggling_423F30)
    {
        res_idx = 61;
    }
    else if (motion < eAbeStates::State_142_RockThrowStandingHold_429CE0)
    {
        res_idx = 6;
    }
    else if (motion < eAbeStates::State_147_ShotRolling_4295C0)
    {
        res_idx = 4;
    }
    else if (motion < eAbeStates::State_149_PickupItem_42A030)
    {
        res_idx = 18;
    }
    else if (motion < eAbeStates::State_150_Chant_42FD50)
    {
        res_idx = 2;
    }
    else if (motion < eAbeStates::State_152_ElumFallOffEdge_42E030)
    {
        res_idx = 38;
    }
    else if (motion < eAbeStates::State_156_DoorEnter_42D370)
    {
        res_idx = 54;
    }
    else if (motion < eAbeStates::State_158_ElumKnockback_42E070)
    {
        res_idx = 35;
    }
    else if (motion < eAbeStates::State_159_423360)
    {
        res_idx = 52;
    }
    else if (motion < eAbeStates::State_160_4233A0)
    {
        res_idx = 22;
    }
    else if (motion < eAbeStates::State_161_4233E0)
    {
        res_idx = 23;
    }
    else if (motion < eAbeStates::State_162_ToShrykull_42F410)
    {
        res_idx = 21;
    }
    else if (motion >= eAbeStates::State_164_PoisonGasDeath_42A120)
    {
        res_idx = 64;
        if (motion >= 165)
        {
            res_idx = motion;
        }
    }
    else
    {
        res_idx = 63;
    }


    if (res_idx == field_128)
    {
        return field_1A4_resources.res[field_128];
    }

    if (field_128)
    {
        if (field_128 != 45 && field_128 != 46)
        {
            ResourceManager::FreeResource_455550(field_1A4_resources.res[field_128]);
            field_1A4_resources.res[field_128] = nullptr;
        }
    }
    if (res_idx)
    {
        if (res_idx != 45 && res_idx != 46)
        {
            int v7 = res_idx + 10;
            if (res_idx >= 46)
            {
                v7 = res_idx + 54;
            }
            field_1A4_resources.res[res_idx] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, v7, 1, 0);
        }
    }
    field_128 = res_idx;
    return field_1A4_resources.res[res_idx];
}

void Abe::ToIdle_422D50()
{
    field_120 = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_114_gnFrame = gnFrameCount_507670;
    field_FC_current_motion = eAbeStates::State_0_Idle_423520;
    field_10C_prev_held = 0;
    field_10E_released_buttons = 0;
}

void Abe::sub_422FC0()
{
    FollowLift_42EE90();

    const FP old_x = field_A8_xpos;

    if (field_F4_pLine)
    {
        field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(
            &field_A8_xpos,
            &field_AC_ypos,
            field_B4_velx);
    }

    // TODO: Check mask is correct
    const int mask = field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10;
    if (field_F4_pLine && (mask & (1 << field_F4_pLine->field_8_type)))
    {
        if (field_F8_pLiftPoint)
        {
            if (field_F4_pLine->field_8_type != 32 && field_F4_pLine->field_8_type != 36)
            {
                field_F8_pLiftPoint->VRemove(this);
                field_F8_pLiftPoint->field_C_refCount--;
                field_F8_pLiftPoint = nullptr;
                
            }
        }
        else if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            bRect.y += 5;
            bRect.h += 5;
            bRect.w += 5; // TODO: Seems wrong - same in AE

            VOnCollisionWith(
                { bRect.x, bRect.y },
                { bRect.w, bRect.h },
                ObjListPlatforms_50766C,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }
    else
    {
        field_F4_pLine = nullptr;

        if (field_F8_pLiftPoint)
        {
            field_F8_pLiftPoint->VRemove(this);
            field_F8_pLiftPoint->field_C_refCount--;
            field_F8_pLiftPoint = nullptr;
        }

        field_10C_prev_held = 0;
        switch (field_FC_current_motion)
        {
        case eAbeStates::State_1_WalkLoop_423F90:
        case eAbeStates::State_6_WalkBegin_424300:
        case eAbeStates::State_4_WalkToIdle_4243C0:
        case eAbeStates::State_5_MidWalkToIdle_424490:
            field_FC_current_motion = eAbeStates::State_93_WalkOffEdge_429840;
            break;

        case eAbeStates::State_35_RunLoop_425060:
        case eAbeStates::State_41_StandingToRun_425530:
            field_FC_current_motion = eAbeStates::State_94_RunOffEdge_429860;
            break;

        case eAbeStates::State_40_RunToRoll_427AE0:
        case eAbeStates::State_24_RollBegin_427A20:
        case eAbeStates::State_25_RollLoop_427BB0:
        case eAbeStates::State_26_RollEnd_427EA0:
            field_FC_current_motion = eAbeStates::State_100_RollOffLedge_429950;
            break;

        default:
            field_FC_current_motion = eAbeStates::State_95_SneakOffEdge_429880;
            break;
        }

        field_A8_xpos = old_x + field_B4_velx;
        field_E8_LastLineYPos = field_AC_ypos;

        // TODO: OG bug, dead code due to switch default case ?
        if (field_FC_current_motion == eAbeStates::State_70_Knockback_428FB0 || field_FC_current_motion == eAbeStates::State_128_KnockForward_429330)
        {
            field_120 = FP_FromDouble(0.67); // TODO: Check
        }
    }
}

void Abe::ElumFree_4228F0()
{
    if ((field_2AA_flags >> 2) & 1)
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 110, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 223, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 222, 0, 0));
        
        if (gElum_507680->field_FC_current_motion != eElumStates::State_1_Idle_412990)
        {
            gElum_507680->Vsub_416120();
        }
    }

    if ((field_2AA_flags >> 3) & 1)
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 100, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 112, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 220, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 221, 1, 0));
    }

    if ((field_2AA_flags >> 4) & 1)
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 113, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 224, 1, 0));
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 221, 0, 0));

        if (gElum_507680->field_FC_current_motion != eElumStates::State_1_Idle_412990)
        {
            gElum_507680->Vsub_416120();
        }
    }
}


eAbeStates Abe::DoGameSpeak_42F5C0(unsigned __int16 /*input*/)
{
    NOT_IMPLEMENTED();
    return eAbeStates::State_0_Idle_423520;
}

void Abe::SyncToElum_42D850(__int16 elumMotion)
{
    if (field_FC_current_motion != eAbeStates::State_137_ElumUnmountBegin_42E2B0 && field_FC_current_motion != eAbeStates::State_138_ElumUnmountEnd_42E390)
    {
        switch (elumMotion)
        {
        case eElumStates::State_1_Idle_412990:
            field_FC_current_motion = eAbeStates::State_103_ElumIdle_42DCD0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = gElum_507680->field_F4_pLine;
            break;

        case eElumStates::State_3_WalkLoop_412C90:
            field_FC_current_motion = eAbeStates::State_102_ElumWalkLoop_42DCA0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = gElum_507680->field_F4_pLine;
            break;

        case eElumStates::State_4_Turn_4140F0:
            field_FC_current_motion = eAbeStates::State_113_ElumTurn_42DF90;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_5_WalkToIdle_4132D0:
            field_FC_current_motion = eAbeStates::State_124_ElumWalkEnd_42DCB0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_6_MidWalkToIdle_4133F0:
            field_FC_current_motion = eAbeStates::State_125_ElumMidWalkEnd_42DCC0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_7_Unknown_413200:
            field_FC_current_motion = eAbeStates::State_116_42DFB0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_8_IdleToWalk_413270:
            field_FC_current_motion = eAbeStates::State_117_ElumWalkBegin_42DFC0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_12_RunTurn_414520:
            field_FC_current_motion = eAbeStates::State_105_ElumRunTurn_42DF10;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_13_RunTurnToWalk_4147C0:
            field_FC_current_motion = eAbeStates::State_123_ElumRunTurnToWalk_42E020;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_21_Land_414A20:
            field_FC_current_motion = eAbeStates::State_153_ElumFall_42E040;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = nullptr;
            break;

        case eElumStates::State_22_RunOffEdge_415810:
            field_FC_current_motion = eAbeStates::State_154_ElumLand_42E050;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = gElum_507680->field_F4_pLine;
            break;

        case eElumStates::State_23_WalkOffEdge_415E90:
            field_FC_current_motion = eAbeStates::State_152_ElumFallOffEdge_42E030;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = nullptr;
            break;

        case eElumStates::State_24_JumpToFall_415ED0:
            field_FC_current_motion = eAbeStates::State_155_ElumJumpToFall_42E060;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = nullptr;
            break;

        case eElumStates::State_30_HopBegin_414E30:
            field_FC_current_motion = eAbeStates::State_107_ElumHopBegin_42DF30;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = nullptr;
            break;

        case eElumStates::State_31_HopMid_414C70:
            field_FC_current_motion = eAbeStates::State_108_ElumHopMid_42DF40;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_32_HopLand_415140:
            field_FC_current_motion = eAbeStates::State_109_ElumHopLand_42DF50;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = gElum_507680->field_F4_pLine;
            break;

        case eElumStates::State_33_RunJumpBegin_415400:
            field_FC_current_motion = eAbeStates::State_110_ElumRunJumpBegin_42DF60;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = nullptr;
            break;

        case eElumStates::State_34_RunJumpMid_415240:
            field_FC_current_motion = eAbeStates::State_111_ElumRunJumpMid_42DF70;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_35_RunJumpLand_415580:
            field_FC_current_motion = eAbeStates::State_112_ElumRunJumpLand_42DF80;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = gElum_507680->field_F4_pLine;
            break;

        case eElumStates::State_36_RunLoop_413720:
            field_FC_current_motion = eAbeStates::State_114_ElumRunLoop_42DFA0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            field_F4_pLine = gElum_507680->field_F4_pLine;
            break;

        case eElumStates::State_37_RunSlideStop_4142E0:
            field_FC_current_motion = eAbeStates::State_104_ElumRunSlideStop_42DF00;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_38_RunTurnToRun_414810:
            field_FC_current_motion = eAbeStates::State_122_ElumRunTurnToRun_42E010;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_39_IdleToRun_413B00:
            field_FC_current_motion = eAbeStates::State_118_ElumRunBegin_42DFD0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_40_WalkToRun_4134B0:
            field_FC_current_motion = eAbeStates::State_119_42DFE0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_41_MidWalkToRun_413560:
            field_FC_current_motion = eAbeStates::State_120_ElumRunToWalk_42DFF0;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_43_MidRunToWalk_413E20:
            field_FC_current_motion = eAbeStates::State_121_ElumMidRunToWalk_42E000;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        case eElumStates::State_50_Knockback_415DC0:
            field_FC_current_motion = eAbeStates::State_158_ElumKnockback_42E070;
            ToNewElumSyncMotion_422520(gElum_507680->field_10_anim.field_92_current_frame);
            break;

        default:
            break;
        }
    }
}

void Abe::PickUpThrowabe_Or_PressBomb_428260(FP fpX, int fpY, __int16 bStandToCrouch)
{
    LOG_INFO("fpX = " << FP_GetDouble(fpX) << " fpY = " << fpY << " bStandTo = " << bStandToCrouch);
    BaseAliveGameObject* pSlapableOrCollectable = nullptr;
    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eInteractive_Bit8))
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->field_D4_collection_rect.x &&
                fpX <= pAliveObj->field_D4_collection_rect.w)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->field_D4_collection_rect.y &&
                    yPos <= pAliveObj->field_D4_collection_rect.h)
                {
                    pSlapableOrCollectable = pAliveObj;
                    pSlapableOrCollectable->field_C_refCount++;
                    field_15C_pThrowable = static_cast<BaseThrowable*>(pSlapableOrCollectable);
                    break;
                }
            }
        }
    }


    if (pSlapableOrCollectable)
    {
        bool tryToSlapOrCollect = false;

        switch (pSlapableOrCollectable->field_4_typeId)
        {
        case Types::eTimedMine_8:
        case Types::eUXB_99:
            field_FC_current_motion = eAbeStates::State_127_SlapBomb_429A20;
            if (bStandToCrouch)
            {
                field_15C_pThrowable = nullptr;
                field_15C_pThrowable->field_C_refCount;
            }
            tryToSlapOrCollect = true;
            break;

        case Types::eGrenade_40:
        case Types::eMeat_54:
        case Types::eRock_70:
            field_FC_current_motion = eAbeStates::State_149_PickupItem_42A030;
            field_19C_throwable_count += static_cast<char>(field_15C_pThrowable->VGetCount());

            if (!bThrowableIndicatorExists_504C70)
            {
                auto pThrowableIndicator = ao_new<ThrowableTotalIndicator>();
                if (pThrowableIndicator)
                {
                    const FP v16 = (field_BC_sprite_scale * FP_FromInteger(-30)) + field_AC_ypos;
                    pThrowableIndicator->ctor_41B520(
                        (field_BC_sprite_scale * FP_FromInteger(0)) + field_A8_xpos,
                        v16,
                        field_10_anim.field_C_layer,
                        field_10_anim.field_14_scale,
                        field_19C_throwable_count,
                        1);
                }
            }
            tryToSlapOrCollect = true;
            break;

        case Types::eMine_57:
            field_15C_pThrowable = nullptr;
            field_15C_pThrowable->field_C_refCount;
            tryToSlapOrCollect = true;
            break;

        default:
            return;
        }

        if (tryToSlapOrCollect)
        {
            if (field_FC_current_motion == eAbeStates::State_149_PickupItem_42A030)
            {
                if (bStandToCrouch)
                {
                    SFX_Play_43AD70(SoundEffect::PickupItem_33, 0, this);
                    field_15C_pThrowable->field_C_refCount--;
                    field_15C_pThrowable->VOnPickUpOrSlapped();
                    field_15C_pThrowable = nullptr;
                    field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
                }
            }
        }
    }
}

void Abe::CrouchingGameSpeak_427F90()
{
    NOT_IMPLEMENTED();
}

void Abe::FallOnBombs_4231B0()
{
    PSX_RECT bOurRect = {};
    VGetBoundingRect(&bOurRect, 1);

    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eMine_57 || pObjIter->field_4_typeId == Types::eUXB_99)
        {
            PSX_RECT objRect = {};
            pObjIter->VGetBoundingRect(&objRect, 1);

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

__int16 Abe::ToLeftRightMovement_422AA0()
{
    field_B8_vely = FP_FromInteger(0);
    if (sControlledCharacter_50767C != this)
    {
        return 0;
    }

    const FP gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    const BOOL flipX = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);

    if (flipX && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590) ||
        !flipX && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
    {
        field_FC_current_motion = eAbeStates::State_2_StandingTurn_426040;
        return 1;
    }

    if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590) ||
        sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
    {
        const FP directionX = FP_FromInteger(sInputObject_5009E8.isPressed(sInputKey_Right_4C6590) ? 1 : -1);

        if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
        {
            field_B4_velx = directionX * (gridSize / FP_FromInteger(4));
            field_FC_current_motion = eAbeStates::State_41_StandingToRun_425530;
        }
        else if (sInputObject_5009E8.isPressed(sInputKey_Sneak_4C65AC))
        {
            field_B4_velx = directionX * (gridSize / FP_FromInteger(10));
            field_FC_current_motion = eAbeStates::State_47_SneakBegin_424ED0;
        }
        else
        {
            field_B4_velx = directionX * (gridSize / FP_FromInteger(9));
            field_FC_current_motion = eAbeStates::State_6_WalkBegin_424300;
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), directionX * gridSize))
        {
            if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), directionX * gridSize))
            {
                return 1;
            }
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), directionX * gridSize))
        {
            field_B4_velx = FP_FromInteger(0);
            field_FC_current_motion = eAbeStates::State_72_PushWall_4292A0;
            Abe_SFX_2_42A220(9u, 0, 0x7FFF, this);
            return 0;
        }

        field_FC_current_motion = eAbeStates::State_21_StandToCrouch_427F40;
        return 1;
    }

    return 0;
}


void Abe::MoveWithVelocity_4257F0(FP speed)
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx = field_B4_velx - (field_BC_sprite_scale * speed);
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx = (field_BC_sprite_scale * speed)  + field_B4_velx;
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    if (FP_GetExponent(field_B4_velx))
    {
        sub_422FC0();
    }
}

void Abe::ToNewElumSyncMotion_422520(short elum_frame)
{
    field_10_anim.Set_Animation_Data_402A40(sAbeFrameOffsetTable_4C61A0[field_FC_current_motion], StateToAnimResource_4204F0(field_FC_current_motion));
    field_12C_timer = gnFrameCount_507670;
    field_10_anim.SetFrame_402AC0(elum_frame + 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, gElum_507680->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));
}

void Abe::SetActiveControlledCharacter_421480()
{
    field_2A8_flags.Set(Flags_2A8::e2A8_Bit7);
    sControlledCharacter_50767C = this;
}

Rope* Abe::GetPullRope_422580()
{
    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::ePullRingRope_68)
        {
            Rope* pRope = static_cast<Rope*>(pObj);

            PSX_RECT bRect = {};
            pRope->VGetBoundingRect(&bRect, 1);

            if ((field_AC_ypos - pRope->field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(80))) <= FP_FromInteger(0))
            {
                if (field_A8_xpos > FP_FromInteger(bRect.x) && field_A8_xpos < FP_FromInteger(bRect.w))
                {
                    return pRope;
                }
            }
        }
    }
    return nullptr;
}

void Abe::Free_Resources_422870()
{
    if (field_1A4_resources.res[0])
    {
        ResourceManager::FreeResource_455550(field_1A4_resources.res[0]);
        field_1A4_resources.res[0] = nullptr;
    }
}

void CC Abe::Load_Basic_Resources_4228A0()
{
    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 0, 0) &&
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0))
    {
        ResourceManager::LoadResourceFile_455270("ABEBSIC.BAN", 0, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0);
    }
}

void Abe::LoadMountElumResources_42E690()
{
    ResourceManager::FreeResource_455550(field_1A4_resources.res[0]);
    field_1A4_resources.res[0] = nullptr;

    field_1A4_resources.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 20, 0, 0);
    ResourceManager::FreeResource_455550(field_1A4_resources.res[10]);
    field_1A4_resources.res[10] = nullptr;

    field_1A4_resources.res[38] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 0, 0);
    ResourceManager::FreeResource_455550(field_1A4_resources.res[38]);
    field_1A4_resources.res[38] = nullptr;

    field_1A4_resources.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 19, 0, 0);
    ResourceManager::FreeResource_455550(field_1A4_resources.res[9]);
    field_1A4_resources.res[9] = nullptr;

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 0, 0))
    {
        ResourceManager::LoadResourceFile_455270("ANEPRMNT.BAN", 0, 0);
        field_1A4_resources.res[61] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 1, 0);
    }

    field_104_pending_resource_count++;
    ResourceManager::LoadResourceFile("ANEMOUNT.BND", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
}

void Abe::ElumKnockForward_42E780(int /*not_used*/)
{
    ToKnockback_422D90(1, 1);
    field_FC_current_motion = eAbeStates::State_128_KnockForward_429330;
    field_FE_next_state = eAbeStates::State_0_Idle_423520;
    field_108_bMotionChanged = 1;
    field_10_anim.Set_Animation_Data_402A40(sAbeFrameOffsetTable_4C61A0[field_FC_current_motion], StateToAnimResource_4204F0(eAbeStates::State_128_KnockForward_429330));
    sControlledCharacter_50767C = sActiveHero_507678;
    gElum_507680->field_154 = 1;
}

__int16 Abe::TryMountElum_42E600()
{
    if (gElum_507680)
    {
        if (FP_Abs(field_A8_xpos - gElum_507680->field_A8_xpos) <= FP_FromInteger(2) && FP_Abs(field_AC_ypos - gElum_507680->field_AC_ypos) <= FP_FromInteger(2))
        {
            if (gElum_507680->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FE_next_state = eAbeStates::State_139_ElumMountBegin_42E090;
                return eAbeStates::State_2_StandingTurn_426040;
            }

            if (gElum_507680->field_FC_current_motion != eElumStates::State_1_Idle_412990
                || gElum_507680->field_128_brain_idx == 1
                || gElum_507680->field_170_flags & 1)
            {
                return eAbeStates::State_0_Idle_423520;
            }

            LoadMountElumResources_42E690();
            return eAbeStates::State_139_ElumMountBegin_42E090;
        }
    }
    return eAbeStates::State_0_Idle_423520;
}

void Abe::BulletDamage_4220B0(Bullet* pBullet)
{
    PSX_RECT rect = {};
    VGetBoundingRect_418120(&rect, 1);

    enum class ShootKind
    {
        eEverythingElse_0 = 0,
        eHanging_1 = 1,
        eRolling_2 = 2,
    };

    if (field_FC_current_motion != eAbeStates::State_137_ElumUnmountBegin_42E2B0
        && field_FC_current_motion != eAbeStates::State_136_ElumMountEnd_42E110
        && field_FC_current_motion != eAbeStates::State_138_ElumUnmountEnd_42E390
        && field_FC_current_motion != eAbeStates::State_139_ElumMountBegin_42E090
        && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        1)
        && field_100_health > FP_FromInteger(0))
    {
        ShootKind shootKind = ShootKind::eEverythingElse_0;
        if (field_FC_current_motion == eAbeStates::State_24_RollBegin_427A20
            || field_FC_current_motion == eAbeStates::State_25_RollLoop_427BB0
            || field_FC_current_motion == eAbeStates::State_26_RollEnd_427EA0
            || field_FC_current_motion == eAbeStates::State_39_CrouchTurn_4288C0
            || field_FC_current_motion == eAbeStates::State_19_CrouchIdle_4284C0)
        {
            shootKind = ShootKind::eRolling_2;
        }
        else if (field_FC_current_motion == eAbeStates::State_64_LedgeAscend_428B60
            || field_FC_current_motion == eAbeStates::State_66_LedgeHang_428D90
            || field_FC_current_motion == eAbeStates::State_68_LedgeHangWobble_428E50
            || field_FC_current_motion == eAbeStates::State_65_LedgeDescend_428C00
            || field_FC_current_motion == eAbeStates::State_67_ToOffScreenHoist_428C50)
        {
            shootKind = ShootKind::eHanging_1;
        }

        field_106_shot = 1;
        field_100_health = FP_FromInteger(0);

        switch (pBullet->field_10_type)
        {
            case BulletType::Type_0:
            case BulletType::Type_1:
            {
                auto yPos = FP_FromInteger(24);
                if (pBullet->field_20 > FP_FromInteger(0))
                {
                    yPos = -yPos;
                }

                auto pBlood = ao_new<Blood>();
                if (pBlood)
                {
                    pBlood->ctor_4072B0(
                        field_A8_xpos,
                        pBullet->field_1C_ypos,
                        yPos,
                        FP_FromInteger(0),
                        field_BC_sprite_scale,
                        50);
                }

                switch (shootKind)
                {
                    case ShootKind::eEverythingElse_0:
                    {
                        if (field_1A4_resources.res[0])
                        {
                            ToKnockback_422D90(1, 1);
                        }
                        else
                        {
                            ElumKnockForward_42E780(1);
                        }
                        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != pBullet->field_20 > FP_FromInteger(0))
                        {
                            field_FC_current_motion = eAbeStates::State_128_KnockForward_429330;
                        }
                        field_108_bMotionChanged = 1;
                        field_106_shot = 0;

                        field_B4_velx = field_BC_sprite_scale * FP_FromDouble(7.8);
                        if (pBullet->field_20 < FP_FromInteger(0))
                        {
                            field_B4_velx = -field_BC_sprite_scale * FP_FromDouble(7.8);
                        }
                        break;
                    }
                    case ShootKind::eHanging_1:
                    {
                        field_FC_current_motion = eAbeStates::State_92_ForceDownFromHoist_4297C0;
                        field_108_bMotionChanged = 1;
                        field_106_shot = 0;
                        field_114_gnFrame = 0;
                        break;
                    }
                    case ShootKind::eRolling_2:
                    {
                        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) == pBullet->field_20 > FP_FromInteger(0))
                        {
                            field_FE_next_state = eAbeStates::State_73_RollingKnockback_4291D0;
                        }
                        else
                        {
                            field_FE_next_state = eAbeStates::State_129_RollingKnockForward_4294F0;
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case BulletType::Type_2:
            {
                if (field_BC_sprite_scale == FP_FromDouble(0.5))
                {
                    field_106_shot = 0;
                    field_100_health = FP_FromInteger(1);
                    return;
                }

                Path_TLV *pPath = nullptr;
                while (true)
                {
                    pPath = gMap_507BA8.TLV_Get_At_446060(
                        pPath,
                        FP_FromInteger(rect.x),
                        FP_FromInteger(rect.y),
                        FP_FromInteger(rect.x),
                        FP_FromInteger(rect.y)
                    );
                    if (!pPath)
                        break;

                    if (pPath->field_4_type != 83)
                    {
                        continue;
                    }
                    if (rect.x >= pPath->field_10_top_left.field_0_x &&
                        rect.x <= pPath->field_14_bottom_right.field_0_x &&
                        rect.y >= pPath->field_10_top_left.field_2_y &&
                        rect.y <= pPath->field_14_bottom_right.field_2_y &&
                        rect.w >= pPath->field_10_top_left.field_0_x &&
                        rect.w <= pPath->field_14_bottom_right.field_0_x &&
                        rect.h >= pPath->field_10_top_left.field_2_y &&
                        rect.h <= pPath->field_14_bottom_right.field_2_y)
                    {
                        break;
                    }
                }
                if (!pPath)
                {
                    if (shootKind != ShootKind::eEverythingElse_0)
                    {
                        if (shootKind == ShootKind::eHanging_1)
                        {
                            field_FC_current_motion = 92;
                            field_108_bMotionChanged = 1;
                            field_106_shot = 0;
                            field_114_gnFrame = 0;
                        }
                        else if (shootKind == ShootKind::eRolling_2)
                        {
                            field_FE_next_state = 147;
                        }
                    }
                    if (field_FC_current_motion != eAbeStates::State_114_ElumRunLoop_42DFA0)
                    {
                        if (field_1A4_resources.res[0])
                        {
                            field_FE_next_state = eAbeStates::State_148_Shot_4296A0;
                        }
                        else
                        {
                            ElumKnockForward_42E780(1);
                            field_FC_current_motion = eAbeStates::State_148_Shot_4296A0;
                            field_108_bMotionChanged = 1;
                            field_106_shot = 0;
                        }
                    }

                    if (field_FC_current_motion != eAbeStates::State_114_ElumRunLoop_42DFA0 ||
                        shootKind != ShootKind::eEverythingElse_0)
                    {
                        auto pBlood = ao_new<Blood>();
                        if (pBlood)
                        {
                            pBlood->ctor_4072B0(
                                field_A8_xpos,
                                field_AC_ypos - FP_FromInteger(45),
                                FP_FromInteger(0),
                                FP_FromInteger(0),
                                FP_FromInteger(1),
                                50
                            );
                        }

                        break;
                    }
                }
                field_106_shot = 0;
                field_100_health = FP_FromInteger(1);
                return;
            }
            default:
                break;
        }

        if (field_106_shot)
        {
            field_112_prev_motion = field_FE_next_state;
        }

        Abe_SFX_2_42A220(14, 0, 0x7FFF, this);
        Abe_SFX_42A4D0(10, 127, 0, this);
        Abe_SFX_2_42A220(7, 0, 0x7FFF, this);
        SFX_Play_43AE60(SoundEffect::Unknown_79, 0, -500, this);
        SFX_Play_43AD70(SoundEffect::KillEffect_78, 0, this);
    }
}

__int16 Abe::RunTryEnterDoor_4259C0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Abe::MoveLiftUpOrDown_42F190(FP /*ySpeed*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void Abe::vScreenChanged_422640()
{
    if (sControlledCharacter_50767C == this || 
        sControlledCharacter_50767C == gElum_507680)
    {
        field_B2_lvl_number = gMap_507BA8.field_A_level;
        field_B0_path_number = gMap_507BA8.field_C_path;
    }

    // Level has changed?
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        // Set the correct tint for this map
        SetTint_418750(sTintTable_Abe_4C6438, gMap_507BA8.field_A_level);

        if (gMap_507BA8.field_0_current_level != LevelIds::eMenu_0)
        {
            if (field_19C_throwable_count > 0)
            {
                if (gpThrowableArray_50E26C)
                {
                    gpThrowableArray_50E26C->Remove_4540D0(field_19C_throwable_count);
                }
            }

            field_19C_throwable_count = 0;

            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::ResourceType::Resource_Palt, 350, 0, 0));

            if (field_168_ring_pulse_timer > 0 && field_16C_bHaveShrykull)
            {
                Free_Shrykull_Resources_42F4C0();
            }

            field_168_ring_pulse_timer = 0;
        }

        if (gMap_507BA8.field_A_level == LevelIds::eCredits_10 || gMap_507BA8.field_A_level == LevelIds::eMenu_0)
        {
            // Remove Abe for menu/credits levels?
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}



void Abe::VOn_Tlv_Collision_421130(Path_TLV *pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::ContinuePoint_0)
        {
            Path_ContinuePoint* pContinuePointTlv = static_cast<Path_ContinuePoint*>(pTlv);

            if ((pContinuePointTlv->field_18_zone_number != field_146_zone_number ||
                field_144_saved_level != gMap_507BA8.field_0_current_level) &&
                !field_10A_flags.Get(Flags_10A::e10A_Bit5_Electrocuted) &&
                field_FC_current_motion != eAbeStates::State_156_DoorEnter_42D370)
            {
                field_146_zone_number = pContinuePointTlv->field_18_zone_number;
                field_148_clear_from_id = pContinuePointTlv->field_1A_clear_from_id;
                field_14A_clear_to_id = pContinuePointTlv->field_1C_clear_to_id;

                field_138_zone_top_left = pContinuePointTlv->field_10_top_left;
                field_13C_zone_bottom_right = pContinuePointTlv->field_14_bottom_right;

                field_14C_saved_sprite_scale = field_BC_sprite_scale;

                field_2A8_flags.Set(Flags_2A8::e2A8_eBit16_AbeSpawnDir, pContinuePointTlv->field_20_abe_direction & 1);

                const auto bHaveShry = field_168_ring_pulse_timer - gnFrameCount_507670;
                field_150_saved_ring_timer = bHaveShry < 0 ? 0 : bHaveShry;
                field_154_bSavedHaveShrykull = field_16C_bHaveShrykull;

                field_144_saved_level = gMap_507BA8.field_0_current_level;
                field_142_saved_path = gMap_507BA8.field_2_current_path;
                field_140_saved_camera = gMap_507BA8.field_4_current_camera;

                if (gRestartRuptureFarmsSavedMuds_5076C8 == 0 &&
                    gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13 &&
                    gMap_507BA8.field_2_current_path == 19 &&
                    gMap_507BA8.field_4_current_camera == 3)
                {
                    gRestartRuptureFarmsKilledMuds_5076C4 = sKilledMudokons_5076BC;
                    gRestartRuptureFarmsSavedMuds_5076C8 = sRescuedMudokons_5076C0;
                }
                gOldKilledMuds_5076D0 = sKilledMudokons_5076BC;
                gOldSavedMuds_5076D4 = sRescuedMudokons_5076C0;

                Demo::Save_459490(gSaveBuffer_505668);

                const FP camXPos = FP_NoFractional(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));

                FP indicator_xpos = {};
                if (field_A8_xpos - camXPos >= FP_FromInteger(384 / 2)) // mid screen x
                {
                    indicator_xpos = field_A8_xpos - ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }
                else
                {
                    indicator_xpos = ScaleToGridSize_41FA30(field_BC_sprite_scale) + field_A8_xpos;
                }
                const FP indicator_ypos = field_AC_ypos + (field_BC_sprite_scale * FP_FromInteger(-50));

                auto pCheckpointIndicator = ao_new<ThrowableTotalIndicator>();
                if (pCheckpointIndicator)
                {
                    pCheckpointIndicator->ctor_41B520(indicator_xpos, indicator_ypos, field_10_anim.field_C_layer,
                        field_10_anim.field_14_scale, 11, 1);
                }
            }
        }
        else if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
        {
            Abe_SFX_42A4D0(17u, 0, 0, this);

            Event_Broadcast_417220(kEventNoise_0, this);
            Event_Broadcast_417220(kEvent_10, this);
            Event_Broadcast_417220(kEvent_14, this);
            Event_Broadcast_417220(kEventSpeaking_1, this);

            if (!field_1A4_resources.res[0] && sControlledCharacter_50767C != this)
            {
                sControlledCharacter_50767C = sActiveHero_507678;
                FreeElumRes_420F80();
            }
            ToDeathDropFall_42C3D0();
        }

        // To next TLV
        pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
    }
}

void Abe::State_0_Idle_423520()
{
    NOT_IMPLEMENTED();
}

void Abe::State_1_WalkLoop_423F90()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    sub_422FC0();

    if (field_FC_current_motion == eAbeStates::State_1_WalkLoop_423F90)
    {
        switch (field_10_anim.field_92_current_frame)
        {
        case 2:
        {
            if (field_B4_velx > FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594) ||
                field_B4_velx < FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590))
            {
                field_FC_current_motion = eAbeStates::State_5_MidWalkToIdle_424490;
                field_10C_prev_held = 0;
                return;
            }

            if (field_10C_prev_held & sInputKey_Hop_4C65A0)
            {
                field_FE_next_state = eAbeStates::State_30_HopMid_4264D0;
                field_FC_current_motion = eAbeStates::State_5_MidWalkToIdle_424490;
                field_10C_prev_held = 0;
                return;
            }

            if (!sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
            {
                field_FC_current_motion = eAbeStates::State_5_MidWalkToIdle_424490;
                field_10C_prev_held = 0;
            }
            else
            {
                FP directedScale = {};
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    directedScale = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }
                else
                {
                    directedScale = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }

                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), directedScale * FP_FromDouble(1.5)))
                {
                    field_FC_current_motion = eAbeStates::State_5_MidWalkToIdle_424490;
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
            if ((field_B4_velx <= FP_FromInteger(0) || !(sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))) &&
                (field_B4_velx >= FP_FromInteger(0) || !(sInputObject_5009E8.isPressed(sInputKey_Right_4C6590))))
            {
                if (field_10C_prev_held & sInputKey_Hop_4C65A0)
                {
                    field_FE_next_state = eAbeStates::State_30_HopMid_4264D0;
                }
                else if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
                {
                    FP directedScale = {};
                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedScale = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
                    }
                    else
                    {
                        directedScale = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                    }

                    if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), directedScale * FP_FromDouble(1.5)))
                    {
                        field_10C_prev_held = 0;
                        return;
                    }
                }
            }
            field_FC_current_motion = eAbeStates::State_4_WalkToIdle_4243C0;
            field_10C_prev_held = 0;
            return;

        case 5:
            Abe_SFX_2_42A220(1u, 0, 0x7FFF, this);

            if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit3))
            {
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit3);
                MapFollowMe_401D30(1);
            }

            if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
            {
                field_FC_current_motion = eAbeStates::State_51_MidWalkToRun_424670;
            }
            else if (sInputObject_5009E8.isPressed(sInputKey_Sneak_4C65AC))
            {
                field_FC_current_motion = eAbeStates::State_43_WalkToSneak_424790;
            }
            break;

        case 14:
            Abe_SFX_2_42A220(1u, 0, 0x7FFF, this);

            if (!field_2A8_flags.Get(Flags_2A8::e2A8_Bit3))
            {
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit3);
                MapFollowMe_401D30(1);
            }
            
            if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
            {
                field_FC_current_motion = eAbeStates::State_50_WalkToRun_424560;
            }
            else if (sInputObject_5009E8.isPressed(sInputKey_Sneak_4C65AC))
            {
                field_FC_current_motion = eAbeStates::State_45_MidWalkToSneak_424890;
            }
            break;

        default:
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit3);
            break;
        }
    }
}

void Abe::State_2_StandingTurn_426040()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 4)
    {
        if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8) && 
            sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
        {
            field_FC_current_motion = eAbeStates::State_63_TurnToRun_42A0A0;
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            return;
        }
    }

    if (!field_10_anim.field_92_current_frame)
    {
        Abe_SFX_2_42A220(9u, 0, 0x7FFF, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_FE_next_state)
        {
            // OG bug: this local variable allows you to "store" your next state if you face the opposite 
            // ledge direction and press down/up and then interrupt it by running away before you hoist.
            const short kNext_state = field_FE_next_state;
            if (field_FE_next_state != eAbeStates::State_139_ElumMountBegin_42E090)
            {
                if (field_FE_next_state == eAbeStates::State_101_LeverUse_429970)
                {
                    Switch* pSwitch;
                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pSwitch = static_cast<Switch*>(FindObjectOfType_418280(Types::eLever_97, field_A8_xpos - ScaleToGridSize_41FA30(field_BC_sprite_scale), field_AC_ypos - FP_FromInteger(5)));
                    }
                    else
                    {
                        pSwitch = static_cast<Switch*>(FindObjectOfType_418280(Types::eLever_97, field_A8_xpos + ScaleToGridSize_41FA30(field_BC_sprite_scale), field_AC_ypos - FP_FromInteger(5)));
                    }

                    if (pSwitch)
                    {
                        pSwitch->vPull_481640(field_A8_xpos < pSwitch->field_A8_xpos);
                    }
                }
                field_FE_next_state = eAbeStates::State_0_Idle_423520;
                field_FC_current_motion = kNext_state;
                return;
            }

            if (gElum_507680)
            {
                if (gElum_507680->field_FC_current_motion == eElumStates::State_1_Idle_412990 &&
                    !(gElum_507680->field_170_flags & 1))
                {
                    LoadMountElumResources_42E690();
                    field_FE_next_state = eAbeStates::State_0_Idle_423520;
                    field_FC_current_motion = kNext_state;
                    return;
                }
            }
            field_FE_next_state = eAbeStates::State_0_Idle_423520;
        }
        else
        {
            if (ToLeftRightMovement_422AA0())
            {
                field_10_anim.Set_Animation_Data_402A40(field_10_anim.field_18_frame_table_offset, 0);
                return;
            }
        }
        ToIdle_422D50();
    }
}

void Abe::State_3_Fall_42E7F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4_WalkToIdle_4243C0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    sub_422FC0();

    if (field_10_anim.field_92_current_frame != 0)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(1);

            if (field_FE_next_state == eAbeStates::State_30_HopMid_4264D0)
            {
                field_FE_next_state = eAbeStates::State_0_Idle_423520;
                field_FC_current_motion = eAbeStates::State_29_HopBegin_4267B0;

                field_1A0_portal = IntoBirdPortal_402350(2);
                if (field_1A0_portal)
                {
                    if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                    {
                        field_1A0_portal = nullptr;
                    }
                }

                if (field_1A0_portal)
                {
                    field_19E = 0;
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
        Abe_SFX_2_42A220(1u, 0, 0x7FFF, this);
    }
}

void Abe::State_5_MidWalkToIdle_424490()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    sub_422FC0();

    if (field_10_anim.field_92_current_frame)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(1);
            if (field_FE_next_state == eAbeStates::State_30_HopMid_4264D0)
            {
                field_FE_next_state = 0;
                field_FC_current_motion = eAbeStates::State_29_HopBegin_4267B0;
                field_1A0_portal = IntoBirdPortal_402350(2);
                if (field_1A0_portal)
                {
                    if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                    {
                        field_1A0_portal = nullptr;
                    }
                }

                if (field_1A0_portal)
                {
                    field_19E = 0;
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
        Abe_SFX_2_42A220(1u, 0, 0x7FFF, this);
    }
}

void Abe::State_6_WalkBegin_424300()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_1_WalkLoop_423F90;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_7_Speak_42F950()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_8_Speak_42F9D0()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_9_Speak_42FA50()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_10_Speak_42FAD0()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_11_Speak_42FB50()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_12_Speak_42FBD0()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_13_Speak_42FC50()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_14_Speak_42FCD0()
{
    State_58_ToSpeak_42F8D0();
}

void Abe::State_15_Null_42A210()
{
    NOT_IMPLEMENTED();
}

void Abe::State_16_HoistBegin_426E40()
{
    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E8_LastLineYPos = field_AC_ypos;
        const FP velY = field_BC_sprite_scale * FP_FromInteger(-8);
        field_B8_vely = velY;
        field_AC_ypos += velY;
        VOnTrapDoorOpen();
        field_FC_current_motion = eAbeStates::State_17_HoistIdle_4269E0;
        field_F4_pLine = nullptr;
    }
}

void Abe::State_17_HoistIdle_4269E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_18_HoistLand_426EB0()
{
    FollowLift_42EE90();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_92_current_frame == 2)
    {
        if (field_E4 == 3)
        {
            Abe_SFX_2_42A220(5u, 0, 0x7FFF, this);
        }
        else
        {
            Abe_SFX_2_42A220(6u, 0, 0x7FFF, this);
        }

        if (sInputObject_5009E8.isPressed(sInputKey_Hop_4C65A0))
        {
            if (sInputObject_5009E8.isPressed(sInputKey_Up_4C6598))
            {
                field_FC_current_motion = eAbeStates::State_16_HoistBegin_426E40;
            }
            else
            {
                field_FC_current_motion = eAbeStates::State_29_HopBegin_4267B0;
                field_1A0_portal = IntoBirdPortal_402350(2);

                if (field_1A0_portal)
                {
                    if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                    {
                        field_1A0_portal = nullptr;
                    }
                }

                if (field_1A0_portal)
                {
                    field_19E = 0;
                }
            }
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_19_CrouchIdle_4284C0()
{
    if (!field_F4_pLine)
    {
        field_FC_current_motion = eAbeStates::State_100_RollOffLedge_429950;
        return;
    }

    FollowLift_42EE90();

    if ((sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & 5) == 5 &&
        sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held & 5)
    {
        Abe_SFX_42A4D0(15u, 0, 0, this);
        field_FC_current_motion = eAbeStates::State_23_CrouchSpeak_428A90;
        return;
    }

    // Crouching game speak
    CrouchingGameSpeak_427F90();

    field_10C_prev_held = 0;
    field_10E_released_buttons = 0;

    if (field_FC_current_motion == eAbeStates::State_22_CrouchSpeak_428A30 || field_FC_current_motion == eAbeStates::State_23_CrouchSpeak_428A90)
    {
        Event_Broadcast_417220(kEventSpeaking_1, this);
        return;
    }


    // Hit bombs/pick up items ?
    if (sInputObject_5009E8.isHeld(sInputKey_DoAction_4C65A4))
    {
        if (!sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
        {
            FP gridSize = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }
            else
            {
                gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }

            PickUpThrowabe_Or_PressBomb_428260(
                gridSize + field_A8_xpos,
                FP_GetExponent(field_AC_ypos - FP_FromInteger(5)),
                0);
        }
    }


    // Crouching throw stuff
    if (sInputObject_5009E8.isHeld(sInputKey_ThrowItem_4C65B4))
    {
        if (field_FC_current_motion == eAbeStates::State_19_CrouchIdle_4284C0)
        {
            if (field_19C_throwable_count > 0 || gInfiniteGrenades_5076EC)
            {
                field_198_pThrowable = Make_Throwable_454560(
                    field_A8_xpos,
                    field_A8_xpos - FP_FromInteger(40),
                    0);

                if (!bThrowableIndicatorExists_504C70)
                {
                    auto pRockCountGraphic = ao_new<ThrowableTotalIndicator>();
                    if (pRockCountGraphic)
                    {
                        const FP yOff = field_AC_ypos + (field_BC_sprite_scale * FP_FromInteger(-30));
                        const FP xOff = field_BC_sprite_scale * (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-10) : FP_FromInteger(10));
                        pRockCountGraphic->ctor_41B520(
                            field_A8_xpos + xOff,
                            yOff,
                            field_10_anim.field_C_layer,
                            field_10_anim.field_14_scale,
                            field_19C_throwable_count,
                            1);
                    }
                }

                field_FC_current_motion = eAbeStates::State_145_RockThrowCrouchingHold_428930;

                if (!gInfiniteGrenades_5076EC)
                {
                    field_19C_throwable_count--;
                }

                return;
            }
        }

        if (!field_19C_throwable_count && !gInfiniteGrenades_5076EC)
        {
            Abe_SFX_42A4D0(15u, 0, 0, this);
            field_FC_current_motion = eAbeStates::State_23_CrouchSpeak_428A90;
            return;
        }
    }

    // Try to stand up
    if (sInputObject_5009E8.isHeld(sInputKey_Up_4C6598) || sInputObject_5009E8.isHeld(sInputKey_FartRoll_4C65B0))
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (!sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(60)),
            &pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 8 : 0x80))
        {
            field_FC_current_motion = eAbeStates::State_20_CrouchToStand_428AF0;
            return;
        }
    }

    // Crouching turns
    if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_FC_current_motion = eAbeStates::State_39_CrouchTurn_4288C0;
        }
        else
        {
            field_FC_current_motion = eAbeStates::State_24_RollBegin_427A20;
            field_10E_released_buttons = 0;
        }
    }

    if (sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_FC_current_motion = eAbeStates::State_24_RollBegin_427A20;
            field_10E_released_buttons = 0;
        }
        else
        {
            field_FC_current_motion = eAbeStates::State_39_CrouchTurn_4288C0;
        }
    }


}

void Abe::State_20_CrouchToStand_428AF0()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 3)
    {
        if (sInputObject_5009E8.isPressed(0xA000)) // TODO: Flags
        {
            ToLeftRightMovement_422AA0();
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_21_StandToCrouch_427F40()
{
    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PickUpThrowabe_Or_PressBomb_428260(
            field_A8_xpos,
            FP_GetExponent(field_AC_ypos - FP_FromInteger(5)),
            1);
        field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
    }
}

void Abe::State_22_CrouchSpeak_428A30()
{
    State_23_CrouchSpeak_428A90();
}

void Abe::State_23_CrouchSpeak_428A90()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
        
        CrouchingGameSpeak_427F90();

        if (field_FC_current_motion == eAbeStates::State_22_CrouchSpeak_428A30 || field_FC_current_motion == eAbeStates::State_23_CrouchSpeak_428A90)
        {
            Event_Broadcast_417220(kEventSpeaking_1, this);
        }

        field_10C_prev_held = 0;
    }
}

void Abe::State_24_RollBegin_427A20()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);

        field_FC_current_motion = eAbeStates::State_73_RollingKnockback_4291D0;
    }
    else
    {
        sub_422FC0();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_FC_current_motion == eAbeStates::State_24_RollBegin_427A20)
            {
                field_FC_current_motion = eAbeStates::State_25_RollLoop_427BB0;
            }
        }
    }
}

void Abe::State_25_RollLoop_427BB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_26_RollEnd_427EA0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);
        field_FC_current_motion = eAbeStates::State_73_RollingKnockback_4291D0;
    }
    else
    {
        sub_422FC0();

        if (field_FC_current_motion == eAbeStates::State_26_RollEnd_427EA0)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(1);
                field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
                field_B4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Abe::State_27_RunSlideStop_425B60()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        MoveWithVelocity_4257F0(FP_FromDouble(0.375));
        if (field_FC_current_motion == eAbeStates::State_27_RunSlideStop_425B60
            && !RunTryEnterWell_425880()
            && !RunTryEnterDoor_4259C0())
        {
            if (field_10_anim.field_92_current_frame >= 9)
            {
                if (field_10_anim.field_92_current_frame == 15)
                {
                    Abe_SFX_2_42A220(0, 0, 0x7FFF, this);
                    MapFollowMe_401D30(1);

                    if (!ToLeftRightMovement_422AA0())
                    {
                        ToIdle_422D50();
                    }
                }
            }
            else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590) ||
                    !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
            {
                field_2A8_flags.Set(Flags_2A8::e2A8_Bit2);
                field_E4 = eAbeStates::State_28_RunTurn_425CE0;
                field_E6_last_anim_frame = field_10_anim.field_92_current_frame;
            }
        }
    }
}

void Abe::State_28_RunTurn_425CE0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        MoveWithVelocity_4257F0(FP_FromDouble(0.375));
        if (field_FC_current_motion == eAbeStates::State_28_RunTurn_425CE0
            && !RunTryEnterWell_425880()
            && !RunTryEnterDoor_4259C0())
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(TRUE);

                const FP gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
                    {
                        field_B4_velx = gridSize / FP_FromInteger(4);
                        field_FC_current_motion = eAbeStates::State_54_RunTurnToRun_425EA0;
                    }
                    else
                    {
                        field_B4_velx = gridSize / FP_FromInteger(9);
                        field_FC_current_motion = eAbeStates::State_55_RunTurnToWalk_425F70;
                    }
                }
                else
                {
                    FP velX = {};
                    if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
                    {
                        velX = gridSize / FP_FromInteger(4);
                        field_FC_current_motion = eAbeStates::State_54_RunTurnToRun_425EA0;
                    }
                    else
                    {
                        velX = gridSize / FP_FromInteger(9);
                        field_FC_current_motion = eAbeStates::State_55_RunTurnToWalk_425F70;
                    }
                    field_B4_velx = -velX;
                }
            }
        }
    }
}

void Abe::State_29_HopBegin_4267B0()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 9)
    {
        const FP velX = field_BC_sprite_scale * (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(-17) : FP_FromInteger(17));
        field_B4_velx = velX;

        if (!field_1A0_portal)
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
            {
                Event_Broadcast_417220(kEventNoise_0, this);
                Event_Broadcast_417220(kEvent_10, this);
                field_B4_velx = FP_FromInteger(0);
                ToKnockback_422D90(1, 1);
                return;
            }
        }
        field_A8_xpos += field_B4_velx;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E8_LastLineYPos = field_AC_ypos;
        const FP velX = field_BC_sprite_scale * (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        field_B4_velx = velX;
        field_A8_xpos += velX;

        const FP velY = field_BC_sprite_scale * FP_FromDouble(-2.7);
        field_B8_vely = velY;
        field_AC_ypos += velY;

        VOnTrapDoorOpen();

        field_FC_current_motion = eAbeStates::State_30_HopMid_4264D0;
        field_F4_pLine = nullptr;

        if (!field_1A0_portal)
        {
            field_1A0_portal = IntoBirdPortal_402350(2);
            if (field_1A0_portal)
            {
                if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                {
                    field_1A0_portal = nullptr;
                }
            }

            if (field_1A0_portal)
            {
                field_19E = 0;
            }
        }
    }
}

void Abe::IntoPortalStates_4262A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_30_HopMid_4264D0()
{
    if (field_1A0_portal)
    {
        IntoPortalStates_4262A0();
    }
    else
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
        {
            Event_Broadcast_417220(kEventNoise_0, this);
            Event_Broadcast_417220(kEvent_10, this);
            if (gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_B4_velx + field_A8_xpos),
                FP_GetExponent(field_AC_ypos - field_BC_sprite_scale * FP_FromInteger(50)),
                FP_GetExponent(field_B4_velx + field_A8_xpos),
                FP_GetExponent(field_AC_ypos - field_BC_sprite_scale * FP_FromInteger(50)),
                38u))
            {
                SFX_Play_43AD70(SoundEffect::Unknown_9, 0, 0);
            }
            field_FE_next_state = eAbeStates::State_0_Idle_423520;
            ToKnockback_422D90(1, 1);
        }
        else
        {
            SetActiveCameraDelayedFromDir_401C90();

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.80)))
            {
                Event_Broadcast_417220(kEventNoise_0, this);
                Event_Broadcast_417220(kEvent_10, this);
                switch (pLine->field_8_type)
                {
                    case 0:
                    case 4:
                    case 32:
                    case 36:
                    {
                        Abe_SFX_2_42A220(6u, 0, 0x7FFF, this);
                        field_F4_pLine = pLine;
                        field_B8_vely = FP_FromInteger(0);
                        field_FC_current_motion = eAbeStates::State_31_HopLand_426940;

                        PSX_RECT rect = {};
                        VGetBoundingRect_418120(&rect, 1);
                        rect.y += 5;
                        rect.h += 5;

                        VOnCollisionWith_418080(
                            { rect.x, rect.y },
                            { rect.w, rect.h },
                            ObjListPlatforms_50766C,
                            1,
                            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                        field_A8_xpos = hitX;
                        field_AC_ypos = hitY;
                        field_B4_velx = FP_FromInteger(0);
                        MapFollowMe_401D30(1);
                        field_FE_next_state = eAbeStates::State_0_Idle_423520;
                        return;
                    }
                    default:
                        break;
                }
            }
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                if (field_FC_current_motion == eAbeStates::State_30_HopMid_4264D0)
                {
                    field_B4_velx = FP_FromRaw(field_B4_velx.fpValue / 2);
                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_A8_xpos += field_BC_sprite_scale * FP_FromInteger(5);
                    }
                    else
                    {
                        field_A8_xpos -= field_BC_sprite_scale * FP_FromInteger(5);
                    }
                    field_120 = FP_FromDouble(0.55);
                    field_AC_ypos += field_BC_sprite_scale * FP_FromInteger(2);
                    field_FC_current_motion = eAbeStates::State_96_HopToFall_4298A0;
                    field_FE_next_state = eAbeStates::State_0_Idle_423520;
                }
            }
        }
    }
}

void Abe::State_31_HopLand_426940()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 2  &&
        sInputObject_5009E8.isPressed(sInputKey_Hop_4C65A0))
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2);
        field_E4 = eAbeStates::State_29_HopBegin_4267B0;
        field_E6_last_anim_frame = 5;
    }
    else
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!ToLeftRightMovement_422AA0())
            {
                ToIdle_422D50();
            }
        }
    }
}

void Abe::State_32_RunJumpBegin_427440()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        FollowLift_42EE90();

        field_A8_xpos += field_B4_velx;
        if (field_10_anim.field_92_current_frame == 0)
        {
            Abe_SFX_2_42A220(11u, 0, 0x7FFF, this);
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_E8_LastLineYPos = field_AC_ypos;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(-7.6));
            }
            else
            {
                field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(7.6));
            }

            field_B8_vely = (field_BC_sprite_scale * FP_FromDouble(-9.6));
            field_AC_ypos += field_B8_vely;
            VOnTrapDoorOpen();
            field_FC_current_motion = eAbeStates::State_33_RunJumpMid_426FA0;
            field_F4_pLine = nullptr;
        }
    }
}

void Abe::State_33_RunJumpMid_426FA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_34_RunJumpLand_427560()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Abe_SFX_2_42A220(6u, 0, 0x7FFF, this);
        MapFollowMe_401D30(1);

        if (sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
        {
            if (!(sInputKey_Hop_4C65A0 & field_10C_prev_held))
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
                    {
                        field_FC_current_motion = eAbeStates::State_56_RunJumpLandRun_4278E0;
                        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    }
                    else
                    {
                        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
                        {
                            field_FC_current_motion = eAbeStates::State_27_RunSlideStop_425B60;
                            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                        }
                        else
                        {
                            field_FC_current_motion = eAbeStates::State_57_RunJumpLand_Walk_427980;
                            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                        }
                    }
                    return;
                }

                field_FC_current_motion = eAbeStates::State_28_RunTurn_425CE0;
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                Abe_SFX_2_42A220(4u, 0, 0x7FFF, this);
                return;
            }

            field_1A0_portal = IntoBirdPortal_402350(3);
            if (field_1A0_portal)
            {
                if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                {
                    field_1A0_portal = nullptr;
                }
            }

            if (field_1A0_portal)
            {
                field_19E = 0;
            }

            field_FC_current_motion = eAbeStates::State_32_RunJumpBegin_427440;
            field_10C_prev_held = 0;
            return;
        }

        if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590))
        {
            if (sInputKey_Hop_4C65A0 & field_10C_prev_held)
            {
                field_1A0_portal = IntoBirdPortal_402350(3);
                if (field_1A0_portal)
                {
                    if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                    {
                        field_1A0_portal = nullptr;
                    }
                }

                if (field_1A0_portal)
                {
                    field_19E = 0;
                }

                field_FC_current_motion = eAbeStates::State_32_RunJumpBegin_427440;
                field_10C_prev_held = 0;
                return;
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FC_current_motion = eAbeStates::State_28_RunTurn_425CE0;
                Abe_SFX_2_42A220(4u, 0, 0x7FFF, this);
                return;
            }

            if (sInputObject_5009E8.isPressed(sInputKey_Run_4C65A8))
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                field_FC_current_motion = eAbeStates::State_56_RunJumpLandRun_4278E0;
            }
            else
            {
                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
                {
                    field_FC_current_motion = eAbeStates::State_27_RunSlideStop_425B60;
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                }
                else
                {
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                    field_FC_current_motion = eAbeStates::State_57_RunJumpLand_Walk_427980;
                }
            }
        }
        else
        {
            if (!(sInputKey_Hop_4C65A0 & field_10C_prev_held))
            {
                field_FC_current_motion = eAbeStates::State_27_RunSlideStop_425B60;
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    Abe_SFX_2_42A220(4u, 0, 0x7FFF, this);
                }
                else
                {
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    Abe_SFX_2_42A220(4u, 0, 0x7FFF, this);
                }
                return;
            }

            field_1A0_portal = IntoBirdPortal_402350(3);
            if (field_1A0_portal)
            {
                if (field_1A0_portal->field_10_portal_type != PortalType::eAbe_0)
                {
                    field_1A0_portal = nullptr;
                }
            }

            if (field_1A0_portal)
            {
                field_19E = 0;
            }

            field_FC_current_motion = eAbeStates::State_29_HopBegin_4267B0;
            field_10C_prev_held = 0;
        }
    }
}

void Abe::State_35_RunLoop_425060()
{
    NOT_IMPLEMENTED();
}

void Abe::State_36_DunnoBegin_423260()
{
    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Abe_SFX_42A4D0(15u, 0, 0, this);

        if (sInputObject_5009E8.isPressed(sInputKey_DoAction_4C65A4 | sInputKey_ThrowItem_4C65B4))
        {
            field_FC_current_motion = eAbeStates::State_37_DunnoMid_4232C0;
        }
        else
        {
            field_FC_current_motion = eAbeStates::State_38_DunnoEnd_423310;
        }
    }
}

void Abe::State_37_DunnoMid_4232C0()
{
    FollowLift_42EE90();

    if (!sInputObject_5009E8.isPressed(sInputKey_DoAction_4C65A4 | sInputKey_ThrowItem_4C65B4) ||
        field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_38_DunnoEnd_423310;
    }
}

void Abe::State_38_DunnoEnd_423310()
{
    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_39_CrouchTurn_4288C0()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

            if (field_FE_next_state == eAbeStates::State_0_Idle_423520)
            {
                field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
            }
            else
            {
                field_FC_current_motion = field_FE_next_state;
                field_FE_next_state = 0;
            }
        }
    }
    else
    {
        Abe_SFX_2_42A220(9u, 0, 0x7FFF, this);
    }
}

void Abe::State_40_RunToRoll_427AE0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);
        field_FC_current_motion = eAbeStates::State_73_RollingKnockback_4291D0;
    }
    else
    {
        sub_422FC0();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_FC_current_motion == eAbeStates::State_40_RunToRoll_427AE0)
            {
                field_FC_current_motion = eAbeStates::State_25_RollLoop_427BB0;
            }
        }
    }
}

void Abe::State_41_StandingToRun_425530()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_35_RunLoop_425060;
    }

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_42_SneakLoop_424BB0()
{
    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
        return;
    }

    sub_422FC0();

    if (field_FC_current_motion == eAbeStates::State_42_SneakLoop_424BB0)
    {
        if (field_10_anim.field_92_current_frame == 3)
        {
            FP directedScale = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                directedScale = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }
            else
            {
                directedScale = ScaleToGridSize_41FA30(field_BC_sprite_scale);
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), directedScale) ||
                field_B4_velx > FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594) ||
                field_B4_velx < FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590) ||
                !sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
            {
                field_FC_current_motion = eAbeStates::State_48_SneakToIdle_424F80;
            }
            return;
        }

        if (field_10_anim.field_92_current_frame == 6)
        {
            Abe_SFX_2_42A220(3u, 0, 0x7FFF, this);
            MapFollowMe_401D30(1);

            if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594) &&
                !sInputObject_5009E8.isPressed(sInputKey_Sneak_4C65AC))
            {
                field_FC_current_motion = eAbeStates::State_44_SneakToWalk_4249A0;
                field_10C_prev_held = 0;
                return;
            }

            field_10C_prev_held = 0;
            return;
        }

        if (field_10_anim.field_92_current_frame != 13)
        {
            if (field_10_anim.field_92_current_frame != 16)
            {
                return;
            }

            Abe_SFX_2_42A220(3u, 0, 0x7FFF, this);
            MapFollowMe_401D30(1);

            if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
            {
                if (!sInputObject_5009E8.isPressed(sInputKey_Sneak_4C65AC))
                {
                    field_FC_current_motion = eAbeStates::State_46_MidSneakToWalk_424AA0;
                }
            }
            field_10C_prev_held = 0;
            return;
        }

        FP directedScale = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            directedScale = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            directedScale = ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), directedScale) || 
            field_B4_velx > FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Left_4C6594) ||
            field_B4_velx < FP_FromInteger(0) && sInputObject_5009E8.isPressed(sInputKey_Right_4C6590) ||
            !sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
        {
            field_FC_current_motion = eAbeStates::State_49_MidSneakToIdle_424FF0;
        }
    }
}

void Abe::State_43_WalkToSneak_424790()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_42_SneakLoop_424BB0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_44_SneakToWalk_4249A0()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_1_WalkLoop_423F90;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_45_MidWalkToSneak_424890()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(10));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2);
        field_E4 = 42;
        field_E6_last_anim_frame = 10;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_46_MidSneakToWalk_424AA0()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2);
        field_E4 = 1;
        field_E6_last_anim_frame = 9;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_47_SneakBegin_424ED0()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_42_SneakLoop_424BB0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_48_SneakToIdle_424F80()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Abe_SFX_2_42A220(3u, 0, 0x7FFF, this);
    }

    sub_422FC0();
    
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);
        ToIdle_422D50();
    }
}

void Abe::State_49_MidSneakToIdle_424FF0()
{
    if (!field_10_anim.field_92_current_frame)
    {
        Abe_SFX_2_42A220(3u, 0, 0x7FFF, this);
    }

    sub_422FC0();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        ToIdle_422D50();
    }
}

void Abe::State_50_WalkToRun_424560()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_35_RunLoop_425060;
        field_10C_prev_held = 0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_51_MidWalkToRun_424670()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2);
        field_E4 = 35;
        field_E6_last_anim_frame = 8;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
        MapFollowMe_401D30(1);
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_52_RunToWalk_4255E0()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_1_WalkLoop_423F90;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_53_MidRunToWalk_4256E0()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit2);
        field_E4 = 1;
        field_E6_last_anim_frame = 9;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();
    }
}

void Abe::State_54_RunTurnToRun_425EA0()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eAbeStates::State_35_RunLoop_425060;
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Abe::State_55_RunTurnToWalk_425F70()
{
    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed;
    
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);


    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eAbeStates::State_1_WalkLoop_423F90;
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
    }
}

void Abe::State_56_RunJumpLandRun_4278E0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eAbeStates::State_35_RunLoop_425060;
        }
    }
}

void Abe::State_57_RunJumpLand_Walk_427980()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToIdle_422D50();
    }
    else
    {
        sub_422FC0();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eAbeStates::State_1_WalkLoop_423F90;
        }
    }
}

void Abe::State_58_ToSpeak_42F8D0()
{
    FollowLift_42EE90();

    field_10C_prev_held |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = DoGameSpeak_42F5C0(field_10C_prev_held);
        if (field_FC_current_motion == -1)
        {
            ToIdle_422D50();
        }
        else
        {
            Event_Broadcast_417220(1, this);
        }
        field_10C_prev_held = 0;
    }
}

void Abe::State_59_DeathDropFall_42CBE0()
{
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);

    FollowLift_42EE90();

    if (field_114_gnFrame == 0)
    {
        field_120 = FP_FromInteger(0);
        field_124 = 0;
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_118 = gnFrameCount_507670 + 90;
        field_114_gnFrame = 1;
    }
    else if (field_114_gnFrame == 1)
    {
        if (static_cast<int>(gnFrameCount_507670) == field_118 - 30)
        {
            SND_SEQ_Play_477760(10u, 1, 65, 65);
        }
        else if (static_cast<int>(gnFrameCount_507670) == field_118 - 24)
        {
            Abe_SFX_2_42A220(15u, 0, 0x7FFF, this);
            
            auto pScreenShake = ao_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4624D0(1);
            }
        }
        else if (static_cast<int>(gnFrameCount_507670) >= field_118)
        {
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit6_bShrivel);
            field_FC_current_motion = eAbeStates::State_60_Dead_42C4C0;
            field_114_gnFrame = 0;
            field_100_health = FP_FromInteger(0);
            MusicController::sub_443810(MusicController::MusicTypes::eType14, this, 1, 0);
        }
    }
}

void Abe::State_60_Dead_42C4C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_61_Respawn_42CD20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_62_LoadedSaveSpawn_45ADD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_63_TurnToRun_42A0A0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
    {
        ToKnockback_422D90(1, 1);
    }
    else
    {
        sub_422FC0();

        if (field_FC_current_motion == eAbeStates::State_63_TurnToRun_42A0A0)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_motion = eAbeStates::State_35_RunLoop_425060;
            }
        }
    }
}

void Abe::State_64_LedgeAscend_428B60()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 0)
    {
        Abe_SFX_2_42A220(10u, 0, 0x7FFF, this);
    }
    if (field_10_anim.field_92_current_frame == 4)
    {
        Abe_SFX_2_42A220(11u, 0, 0x7FFF, this);
        field_D0_pShadow->field_14_flags.Clear(Shadow::eBit1_ShadowAtBottom);
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);
        ToIdle_422D50();
    }
}

void Abe::State_65_LedgeDescend_428C00()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 2)
    {
        Abe_SFX_2_42A220(11u, 0, 0x7FFF, this);
        field_D0_pShadow->field_14_flags.Toggle(Shadow::eBit1_ShadowAtBottom);
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_66_LedgeHang_428D90;
    }
}

void Abe::State_66_LedgeHang_428D90()
{
    FollowLift_42EE90();

    field_D0_pShadow->field_14_flags.Set(Shadow::eBit1_ShadowAtBottom);

    if (sInputObject_5009E8.isPressed(sInputKey_Up_4C6598))
    {
        field_FC_current_motion = eAbeStates::State_64_LedgeAscend_428B60;
    }
    else if (sInputObject_5009E8.isPressed(sInputKey_Down_4C659C))
    {
        VOnTrapDoorOpen();

        field_F4_pLine = nullptr;
        field_FC_current_motion = eAbeStates::State_91_FallingFromGrab_429780;
        field_AC_ypos += (field_BC_sprite_scale * FP_FromInteger(80)) + FP_FromInteger(3);
        field_E8_LastLineYPos = field_AC_ypos;
        field_D0_pShadow->field_14_flags.Clear(Shadow::eBit1_ShadowAtBottom);
    }
}

void Abe::State_67_ToOffScreenHoist_428C50()
{
    field_AC_ypos -= (field_BC_sprite_scale * FP_FromInteger(80));
    
    field_D0_pShadow->field_14_flags.Toggle(Shadow::eBit1_ShadowAtBottom);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(80)),
        field_A8_xpos,
        field_AC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10))
    {
        field_AC_ypos = hitY;
        field_F4_pLine = pLine;
        field_B8_vely = FP_FromInteger(0);
        if (!field_F8_pLiftPoint)
        {
            if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                bRect.y += 5;
                bRect.h += 5;

                VOnCollisionWith(
                    { bRect.x, bRect.y },
                    { bRect.w, bRect.h },
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
        }
        field_FC_current_motion = eAbeStates::State_66_LedgeHang_428D90;
    }
    else
    {
        field_FC_current_motion = eAbeStates::State_17_HoistIdle_4269E0;
    }
}

void Abe::State_68_LedgeHangWobble_428E50()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            Abe_SFX_2_42A220(1u, 0, 127, this);
        }
    }
    else if (field_10_anim.field_92_current_frame == 2)
    {
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            Abe_SFX_2_42A220(18u, 0, 127, this);
        }
    }
    else
    {
        field_2AA_flags &= ~2u;
    }

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    FollowLift_42EE90();

    if (sInputObject_5009E8.isPressed(sInputKey_Up_4C6598))
    {
        field_2AA_flags &= ~2u;
        field_FC_current_motion = eAbeStates::State_64_LedgeAscend_428B60;
    }
    else
    {
        if (sInputObject_5009E8.isPressed(sInputKey_Down_4C659C))
        {
            field_2AA_flags &= ~2u;

            VOnTrapDoorOpen();
            field_F4_pLine = nullptr;

            field_FC_current_motion = eAbeStates::State_91_FallingFromGrab_429780;
            field_AC_ypos += (field_BC_sprite_scale * FP_FromInteger(80)) + FP_FromInteger(3);
            field_D0_pShadow->field_14_flags.Clear(Shadow::eBit1_ShadowAtBottom);
            field_E8_LastLineYPos = field_AC_ypos;
        }
        else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_2AA_flags &= ~2u;
            field_FC_current_motion = eAbeStates::State_66_LedgeHang_428D90;
        }
    }
}

void Abe::State_69_RingRopePull_4299B0()
{
    if (field_160_pRope->sub_454D60())
    {
        field_FC_current_motion = eAbeStates::State_91_FallingFromGrab_429780;
        field_160_pRope->field_C_refCount--;
        field_160_pRope = nullptr;
        field_B8_vely = FP_FromInteger(0);
    }
}

void Abe::State_70_Knockback_428FB0()
{
    ElumFree_4228F0();

    if (field_1A0_portal)
    {
        field_1A0_portal->Vsub_453570();
        field_1A0_portal = nullptr;
    }

    if (field_10_anim.field_92_current_frame == 12)
    {
        FallOnBombs_4231B0();
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_F4_pLine)
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
            {
                field_B4_velx = FP_FromInteger(0);
            }

            MoveWithVelocity_4257F0(FP_FromDouble(0.7));

            if ((gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1
                || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13
                || gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12)
                && field_10_anim.field_92_current_frame == 7)
            {
                Abe_SFX_2_42A220(6u, 80, -200, this);
                Event_Broadcast_417220(kEventNoise_0, this);
                Event_Broadcast_417220(kEvent_10, this);
            }
        }
        else
        {
            State_3_Fall_42E7F0();

            if (field_FC_current_motion == eAbeStates::State_86_FallLandDie_42EDD0 || field_FC_current_motion == eAbeStates::State_98_LandSoft_42ED40)
            {
                field_114_gnFrame = gnFrameCount_507670 + 10;
                field_FC_current_motion = eAbeStates::State_70_Knockback_428FB0;
                Abe_SFX_2_42A220(6u, 80, -200, this);
            }
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if ((field_F4_pLine || !field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))  && !field_104_pending_resource_count)
        {
            if (!field_1A4_resources.res[0])
            {
                field_1A4_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0);
                field_1A4_resources.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 20, 1, 0);
                field_1A4_resources.res[38] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 1, 0);
                field_1A4_resources.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 19, 1, 0);

                if (!(field_2AA_flags & 4))
                {
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 1, 0);
                }
            }

            if (field_114_gnFrame <= static_cast<int>(gnFrameCount_507670))
            {
                if (field_100_health > FP_FromInteger(0) || gAbeInvunerableCheat_5076E4)
                {
                    field_FC_current_motion = eAbeStates::State_71_KnockbackGetUp_429240;
                }
                else
                {
                    ToDieFinal_42C400();
                }
            }
        }
    }
}

void Abe::State_71_KnockbackGetUp_429240()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_72_PushWall_4292A0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_92_current_frame == 10)
    {
        if (Math_NextRandom() <= 127u)
        {
            Abe_SFX_2_42A220(10u, 0, 0x7FFF, this);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_73_RollingKnockback_4291D0()
{
    if (field_10_anim.field_92_current_frame == 12)
    {
        FallOnBombs_4231B0();
    }

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_114_gnFrame <= static_cast<int>(gnFrameCount_507670))
        {
            if (field_100_health > FP_FromInteger(0) || gAbeInvunerableCheat_5076E4)
            {
                field_FC_current_motion = eAbeStates::State_71_KnockbackGetUp_429240;
            }
            else
            {
                ToDieFinal_42C400();
            }
        }
    }
}

void Abe::State_74_JumpIntoWell_430EC0()
{
    field_D0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit2_Enabled);

    if (field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        field_10_anim.field_C_layer = 3;
    }
    else
    {
        field_10_anim.field_C_layer = 22;
    }

    State_77_WellBegin_430F10();
}

void Abe::State_75_ToInsideOfAWellLocal_431090()
{
    State_78_InsideWellLocal_4310A0();
}

void Abe::State_76_ToWellShotOut_431720()
{
    State_79_WellShotOut_431730();
}

void Abe::State_77_WellBegin_430F10()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x4C73CC, FP, gPointlessWellScale_4C73CC, FP_FromDouble(1.8));

void Abe::State_78_InsideWellLocal_4310A0()
{
    const int v2 = field_114_gnFrame;
    field_114_gnFrame--;

    if (!v2)
    {
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            TlvTypes::LocalWell_11);

        if (!field_F0_pTlv)
        {
            field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                TlvTypes::WellExpress_45);
        }

        auto pWellBase = static_cast<Path_Well_Base*>(field_F0_pTlv);

        field_120 = FP_FromInteger(0);

        if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit4_Fall_To_Well))
        {
            field_B4_velx = (field_BC_sprite_scale * (FP_FromInteger(pWellBase->field_20_exit_x) / FP_FromInteger(100)));
            field_B8_vely = (field_BC_sprite_scale * (FP_FromInteger(pWellBase->field_22_exit_y) / FP_FromInteger(100)));
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit4_Fall_To_Well);
        }
        else
        {
            if (SwitchStates_Get(pWellBase->field_1A_trigger_id))
            {
                auto pLocalWell = static_cast<Path_Well_Local*>(field_F0_pTlv);
                field_B4_velx = (field_BC_sprite_scale * FP_FromInteger(pLocalWell->field_28_on_dx) / FP_FromInteger(100));
                field_B8_vely = (field_BC_sprite_scale * FP_FromInteger(pLocalWell->field_2A_on_dy) / FP_FromInteger(100));
            }
            else
            {
                field_B4_velx = (field_BC_sprite_scale * FP_FromInteger(pWellBase->field_24_off_level_or_dx) / FP_FromInteger(100));
                field_B8_vely = (field_BC_sprite_scale * FP_FromInteger(pWellBase->field_26_off_path_or_dy) / FP_FromInteger(100));
            }
        }

        MapFollowMe_401D30(1);

        field_AC_ypos += field_B8_vely;
        gPointlessWellScale_4C73CC = FP_FromDouble(1.8);

        if (field_B4_velx >= FP_FromInteger(0))
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }

        SFX_Play_43AD70(SoundEffect::Unknown_24, 0, this);

        field_FC_current_motion++; // ARGH todo figure out what the actual motion should be, 79 ??

        if (field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            field_10_anim.field_C_layer = 3;
        }
        else
        {
            field_10_anim.field_C_layer = 22;
        }

        field_F4_pLine = nullptr;
    }
}

void Abe::State_79_WellShotOut_431730()
{
    if (field_B8_vely >= FP_FromInteger(0))
    {
        State_3_Fall_42E7F0();
    }
    else
    {
        field_B8_vely += (field_BC_sprite_scale * gPointlessWellScale_4C73CC);

        field_A8_xpos += field_B4_velx;
        field_AC_ypos += field_B8_vely;

        SetActiveCameraDelayedFromDir_401C90();
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
            nullptr,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted)
        || field_FC_current_motion != eAbeStates::State_79_WellShotOut_431730
        && field_FC_current_motion != eAbeStates::State_85_431710
        && field_FC_current_motion != eAbeStates::State_76_ToWellShotOut_431720)
    {
        if (field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            field_10_anim.field_C_layer = 13;
        }
        else
        {
            field_10_anim.field_C_layer = 32;
        }

        field_D0_pShadow->field_14_flags.Set(Shadow::eBit2_Enabled);
    }

    if (field_FC_current_motion == eAbeStates::State_86_FallLandDie_42EDD0)
    {
        field_100_health = FP_FromInteger(1);
        field_FC_current_motion = eAbeStates::State_98_LandSoft_42ED40;
    }
}

void Abe::State_80_430EF0()
{
    State_77_WellBegin_430F10();
}

void Abe::State_81_InsideWellExpress_431320()
{
    NOT_IMPLEMENTED();
}

void Abe::State_82_WellExpressShotOut_4315A0()
{
    PSX_Point camCoords = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camCoords);
    Path_Well_Base* pWell = nullptr;
    Path_TLV* pTlvIter = nullptr;
    do
    {
        pTlvIter = gMap_507BA8.TLV_Get_At_446060(
            pTlvIter,
            FP_FromInteger(camCoords.field_0_x + 256),
            FP_FromInteger(camCoords.field_2_y + 120),
            FP_FromInteger(camCoords.field_0_x + 624),
            FP_FromInteger(camCoords.field_2_y + 360));
        if (!pTlvIter)
        {
            break;
        }

        if ((pTlvIter->field_4_type == TlvTypes::LocalWell_11 || pTlvIter->field_4_type == TlvTypes::WellExpress_45) &&
            static_cast<Path_Well_Base*>(pTlvIter)->field_1C_well_id == field_196_door_id)
        {
            pWell = static_cast<Path_Well_Base*>(pTlvIter);
            break;
        }
    } while (pTlvIter);


    field_B2_lvl_number = gMap_507BA8.field_0_current_level;
    field_B0_path_number = gMap_507BA8.field_2_current_path;

    if (pWell)
    {
        if (pWell->field_18_scale == 1)
        {
            field_BC_sprite_scale = FP_FromDouble(0.5);
            field_C6_scale = 0;
        }
        else
        {
            field_BC_sprite_scale = FP_FromInteger(1);
            field_C6_scale = 1;
        }

        field_FC_current_motion = eAbeStates::State_78_InsideWellLocal_4310A0;

        field_F0_pTlv = pWell;
        field_A8_xpos = FP_FromInteger((pWell->field_10_top_left.field_0_x + pWell->field_14_bottom_right.field_0_x) / 2);
        field_AC_ypos = FP_FromInteger(pWell->field_14_bottom_right.field_2_y);
        field_2A8_flags.Set(Flags_2A8::e2A8_Bit4_Fall_To_Well);
    }
    else
    {
        field_A8_xpos = FP_FromInteger(camCoords.field_0_x + 450);
        field_AC_ypos = FP_FromInteger(camCoords.field_2_y + 320);
        field_B4_velx = (field_BC_sprite_scale * -FP_FromDouble(2.7));
        field_B8_vely = (field_BC_sprite_scale * -FP_FromInteger(15));
    }
}

void Abe::State_83_430F00()
{
    State_77_WellBegin_430F10();
}

void Abe::State_84_431080()
{
    State_78_InsideWellLocal_4310A0();
}

void Abe::State_85_431710()
{
    State_79_WellShotOut_431730();
}

void Abe::State_86_FallLandDie_42EDD0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_92_current_frame == 0)
    {
        SFX_Play_43AD70(SoundEffect::KillEffect_78, 85, 0);
        SND_SEQ_Play_477760(10u, 1, 95, 95);
        auto pScreenShake = ao_new<ScreenShake>();
        if (pScreenShake)
        {
            pScreenShake->ctor_4624D0(1);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<int>(gnFrameCount_507670) >= field_114_gnFrame)
        {
            ToDieFinal_42C400();
        }
    }
}

void Abe::State_87_428FA0()
{
    State_3_Fall_42E7F0();
}

void Abe::State_88_HandstoneBegin_430590()
{
    NOT_IMPLEMENTED();
}

void Abe::State_89_HandstoneEnd_430E80()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eAbeStates::State_0_Idle_423520;
    }
}

void Abe::State_90_GrenadeMachineUse_430EA0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eAbeStates::State_0_Idle_423520;
    }
}

void Abe::State_91_FallingFromGrab_429780()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }
    
    State_3_Fall_42E7F0();

    if (field_FC_current_motion == eAbeStates::State_86_FallLandDie_42EDD0)
    {
        field_FC_current_motion = eAbeStates::State_98_LandSoft_42ED40;
    }

    field_E4 = eAbeStates::State_91_FallingFromGrab_429780;
}

void Abe::State_92_ForceDownFromHoist_4297C0()
{
    if (field_114_gnFrame == 0)
    {
        field_AC_ypos += (field_BC_sprite_scale * FP_FromInteger(80));
        field_D0_pShadow->field_14_flags.Clear(Shadow::eBit1_ShadowAtBottom);
        VOnTrapDoorOpen();
        field_F4_pLine = nullptr;
        field_E8_LastLineYPos = field_AC_ypos;
        field_114_gnFrame++;
    }

    State_3_Fall_42E7F0();
}

void Abe::State_93_WalkOffEdge_429840()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }
    State_3_Fall_42E7F0();
}

void Abe::State_94_RunOffEdge_429860()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }
    State_3_Fall_42E7F0();
}

void Abe::State_95_SneakOffEdge_429880()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }
    State_3_Fall_42E7F0();
}

void Abe::State_96_HopToFall_4298A0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_A8_xpos += (field_BC_sprite_scale * FP_FromInteger(5));
    }
    else
    {
        field_A8_xpos -= (field_BC_sprite_scale *  FP_FromInteger(5));
    }

    field_B8_vely += (field_BC_sprite_scale * FP_FromInteger(4));

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }

    State_3_Fall_42E7F0();
}

void Abe::State_97_RunJumpToFall_429930()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }
    State_3_Fall_42E7F0();
}

void Abe::State_98_LandSoft_42ED40()
{
    FollowLift_42EE90();

    if (field_10_anim.field_92_current_frame == 2)
    {
        if (!(field_2A8_flags.Get(Flags_2A8::e2A8_Bit8)))
        {
            Event_Broadcast_417220(kEventNoise_0, this);
            Event_Broadcast_417220(kEvent_10, this);
        }

        field_2A8_flags.Clear(Flags_2A8::e2A8_Bit8);

        if (field_E4 == eAbeStates::State_3_Fall_42E7F0)
        {
            Abe_SFX_2_42A220(5u, 0, 0x7FFF, this);
        }
        else
        {
            Abe_SFX_2_42A220(6u, 0, 0x7FFF, this);
        }

        if (sInputObject_5009E8.isPressed(0xA000u))
        {
            ToLeftRightMovement_422AA0();
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_99_LedgeHoistUp_426DC0()
{
    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E8_LastLineYPos = field_AC_ypos;
        field_B8_vely = (field_BC_sprite_scale * FP_FromInteger(-8)) * FP_FromDouble(1.3);
        field_AC_ypos += field_B8_vely;
        VOnTrapDoorOpen();
        field_FC_current_motion = eAbeStates::State_17_HoistIdle_4269E0;
        field_F4_pLine = nullptr;
    }
}

void Abe::State_100_RollOffLedge_429950()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_87_428FA0;
    }
    State_3_Fall_42E7F0();
}

void Abe::State_101_LeverUse_429970()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_102_ElumWalkLoop_42DCA0()
{
    // Empty
}

void Abe::State_103_ElumIdle_42DCD0()
{
    if (!sInputObject_5009E8.isPressed(dword_4C65B8 | dword_4C65DC) || !sInputObject_5009E8.isHeld(0xF0))
    {
        if (sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held == sInputKey_Down_4C659C &&
            !sDDCheat_FlyingEnabled_50771C)
        {
            ResourceManager::FreeResource_455550(field_1A4_resources.res[46]);
            field_104_pending_resource_count++;
            field_1A4_resources.res[46] = nullptr;
            ResourceManager::LoadResourceFile("ANEDSMNT.BND", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
            field_FC_current_motion = eAbeStates::State_137_ElumUnmountBegin_42E2B0;
        }
    }
    else
    {
        field_FC_current_motion = eAbeStates::State_115_ElumSpeak_4299F0;
        if (sInputObject_5009E8.isPressed(dword_4C65B8))
        {
            if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak2_4C65BC))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_10);
                Abe_SFX_42A4D0(4u, 0, 0, this);
            }
            else if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak4_4C65C4))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_12);
                Abe_SFX_42A4D0(6u, 0, 0, this);
            }
            else if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak1_4C65C8))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_9);
                Abe_SFX_42A4D0(3u, 0, 0, this);
            }
            else if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak3_4C65C0))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_11);
                Abe_SFX_42A4D0(5u, 0, 0, this);
            }
        }

        if (dword_4C65DC & sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed)
        {
            if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak6_4C65E8))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_1);
                Abe_SFX_42A4D0(1u, 0, 0, this);
            }
            else if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak5_4C65EC))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_2);
                Abe_SFX_42A4D0(2u, 0, 0, this);
            }
            else if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak8_4C65E0))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_4);
                Abe_SFX_42A4D0(8u, 0, 0, this);
            }
            else if (sInputObject_5009E8.isHeld(sInputKey_GameSpeak7_4C65E4))
            {
                pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_3);
                Abe_SFX_42A4D0(7u, 0, 0, this);
            }
        }
    }
}

void Abe::State_104_ElumRunSlideStop_42DF00()
{
    // Empty
}

void Abe::State_105_ElumRunTurn_42DF10()
{
    // Empty
}

void Abe::State_106_42DF20()
{
    // Empty
}

void Abe::State_107_ElumHopBegin_42DF30()
{
    // Empty
}

void Abe::State_108_ElumHopMid_42DF40()
{
    // Empty
}

void Abe::State_109_ElumHopLand_42DF50()
{
    // Empty
}

void Abe::State_110_ElumRunJumpBegin_42DF60()
{
    // Empty
}

void Abe::State_111_ElumRunJumpMid_42DF70()
{
    // Empty
}

void Abe::State_112_ElumRunJumpLand_42DF80()
{
    // Empty
}

void Abe::State_113_ElumTurn_42DF90()
{
    // Empty
}

void Abe::State_114_ElumRunLoop_42DFA0()
{
    // Empty
}

void Abe::State_115_ElumSpeak_4299F0()
{
    Event_Broadcast_417220(kEventSpeaking_1, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_103_ElumIdle_42DCD0;
    }
}

void Abe::State_116_42DFB0()
{
    // Empty
}

void Abe::State_117_ElumWalkBegin_42DFC0()
{
    // Empty
}

void Abe::State_118_ElumRunBegin_42DFD0()
{
    // Empty
}

void Abe::State_119_42DFE0()
{
    // Empty
}

void Abe::State_120_ElumRunToWalk_42DFF0()
{
    // Empty
}

void Abe::State_121_ElumMidRunToWalk_42E000()
{
    // Empty
}

void Abe::State_122_ElumRunTurnToRun_42E010()
{
    // Empty
}

void Abe::State_123_ElumRunTurnToWalk_42E020()
{
    // Empty
}

void Abe::State_124_ElumWalkEnd_42DCB0()
{
    // Empty
}

void Abe::State_125_ElumMidWalkEnd_42DCC0()
{
    // Empty
}

void Abe::State_126_ElumBeesStruggling_42E080()
{
    // Empty
}

void Abe::State_127_SlapBomb_429A20()
{
    if (sActiveHero_507678->field_10_anim.field_92_current_frame >= 6)
    {
        if (field_15C_pThrowable)
        {
            field_15C_pThrowable->field_C_refCount--;
            field_15C_pThrowable->VOnPickUpOrSlapped();
            field_15C_pThrowable = nullptr;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
    }
}

void Abe::State_128_KnockForward_429330()
{
    ElumFree_4228F0();

    if (field_10_anim.field_92_current_frame == 12)
    {
        FallOnBombs_4231B0();
    }

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (field_F4_pLine)
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(50), field_B4_velx))
            {
                field_B4_velx = FP_FromInteger(0);
            }
            MoveWithVelocity_4257F0(FP_FromDouble(0.7));
        }
        else
        {
            State_3_Fall_42E7F0();

            if (field_FC_current_motion == eAbeStates::State_86_FallLandDie_42EDD0 || field_FC_current_motion == eAbeStates::State_98_LandSoft_42ED40)
            {
                field_FC_current_motion = eAbeStates::State_128_KnockForward_429330;
                field_114_gnFrame = gnFrameCount_507670 + 10;
            }
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if ((field_F4_pLine || !field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render)) && !field_104_pending_resource_count)
        {
            if (!field_1A4_resources.res[0])
            {
                field_1A4_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0);
                field_1A4_resources.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 20, 1, 0);
                field_1A4_resources.res[38] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 1, 0);
                field_1A4_resources.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 19, 1, 0);

                if (!(field_2AA_flags & 4))
                {
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 1, 0);
                }
            }

            if (field_114_gnFrame <= static_cast<int>(gnFrameCount_507670))
            {
                if (field_100_health > FP_FromInteger(0) || gAbeInvunerableCheat_5076E4)
                {
                    field_FC_current_motion = eAbeStates::State_130_KnockForwardGetUp_429560;
                }
                else
                {
                    ToDieFinal_42C400();
                }
            }
        }
    }
}

void Abe::State_129_RollingKnockForward_4294F0()
{
    if (field_10_anim.field_92_current_frame == 12)
    {
        FallOnBombs_4231B0();
    }

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_114_gnFrame <= static_cast<int>(gnFrameCount_507670))
        {
            if (field_100_health > FP_FromInteger(0) || gAbeInvunerableCheat_5076E4)
            {
                field_FC_current_motion = eAbeStates::State_130_KnockForwardGetUp_429560;
            }
            else
            {
                ToDieFinal_42C400();
            }
        }
    }
}

void Abe::State_130_KnockForwardGetUp_429560()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_131_LiftUseUp_42F150()
{
    field_FC_current_motion = MoveLiftUpOrDown_42F190(FP_FromInteger(-4));
}

void Abe::State_132_LiftUseDown_42F170()
{
    field_FC_current_motion = MoveLiftUpOrDown_42F190(FP_FromInteger(4));
}

void Abe::State_133_LiftGrabBegin_42EF20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_134_LiftGrabEnd_42EFE0()
{
    field_B8_vely = FP_FromInteger(0);
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_135_LiftGrabIdle_42F000()
{
    NOT_IMPLEMENTED();
}

void Abe::State_136_ElumMountEnd_42E110()
{
    switch (field_10_anim.field_92_current_frame)
    {
    case 0:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            SFX_Play_43AD70(SoundEffect::Unknown_37, 0, this);
        }
        break;

    case 5:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            Abe_SFX_2_42A220(16u, 0, 0x7FFF, this);
        }
        break;

    case 12:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            Abe_SFX_2_42A220(17u, 0, 0x7FFF, this);
            SFX_Play_43AD70(SoundEffect::Unknown_38, 0, this);
        }
        break;

    case 15:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            Abe_SFX_2_42A220(18u, 0, 0x7FFF, this);
        }
        break;

    default:
        field_2AA_flags &= ~2u;
        break;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!field_104_pending_resource_count)
        {
            field_1A4_resources.res[46] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 100, 1, 0);
            field_1A4_resources.res[58] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 112, 1, 0);
            field_FC_current_motion = eAbeStates::State_103_ElumIdle_42DCD0;
            sControlledCharacter_50767C = gElum_507680;
            MusicController::sub_443810(MusicController::MusicTypes::eType1, 0, 0, 0);
            sActiveHero_507678->field_D0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit2_Enabled);
            Abe_SFX_2_42A220(19u, 0, 0x7FFF, this);
        }
    }
}

void Abe::State_137_ElumUnmountBegin_42E2B0()
{
    if (!field_104_pending_resource_count && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        while (!ResourceManager::FreeResource_455550(field_1A4_resources.res[58]))
        {
            // Empty
        }

        field_1A4_resources.res[58] = nullptr;
        field_104_pending_resource_count = 3;

        ResourceManager::LoadResourceFile("ABEBSIC.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        ResourceManager::LoadResourceFile("ANEPRMNT.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        ResourceManager::LoadResourceFile("ABENOELM.BND", BaseAliveGameObject::OnResourceLoaded_4019A0, this);

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, FALSE, FALSE))
        {
            field_104_pending_resource_count++;
            ResourceManager::LoadResourceFile("ABEOMM.BAN", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        }
        field_FC_current_motion = eAbeStates::State_138_ElumUnmountEnd_42E390;
    }
}

void Abe::State_138_ElumUnmountEnd_42E390()
{
    switch (field_10_anim.field_92_current_frame)
    {
    case 4:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            SFX_Play_43AD70(SoundEffect::Unknown_37, 17, this);
        }
        break;

    case 12:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;;
            Abe_SFX_2_42A220(16u, 0, 0x7FFF, this);
        }
        break;

    case 19:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            Abe_SFX_2_42A220(18u, 0, 0x7FFF, this);
        }
        break;

    case 20:
        if (!(field_2AA_flags & 2))
        {
            field_2AA_flags |= 2;
            SFX_Play_43AD70(SoundEffect::Unknown_38, 0, this);
        }
        break;

    default:
        field_2AA_flags &= ~2u;
        break;
    }

    if (field_10_anim.field_92_current_frame >= 28 && field_104_pending_resource_count == 0)
    {
        field_1A4_resources.res[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 1, 0);
        field_1A4_resources.res[61] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 1, 0);
        field_1A4_resources.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 20, 1, 0);
        field_1A4_resources.res[38] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 48, 1, 0);
        field_1A4_resources.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 19, 1, 0);
        
        field_F4_pLine = gElum_507680->field_F4_pLine;

        if (field_F8_pLiftPoint)
        {
            if (field_F4_pLine->field_8_type != 32 && field_F4_pLine->field_8_type != 36)
            {
                field_E4 = field_FC_current_motion;
                VOnTrapDoorOpen();
                field_FC_current_motion = field_E4;
            }
        }
        else
        {
            if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);
                bRect.y += 5;
                bRect.h += 5;
                VOnCollisionWith(
                    { bRect.x, bRect.y },
                    { bRect.w, bRect.h },
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
        }
        sControlledCharacter_50767C = sActiveHero_507678;
        MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);
        sActiveHero_507678->field_D0_pShadow->field_14_flags.Clear(Shadow::Flags::eBit2_Enabled);
        ToIdle_422D50();
    }
}

void Abe::State_139_ElumMountBegin_42E090()
{
    if (!field_104_pending_resource_count && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        while (!ResourceManager::FreeResource_455550(field_1A4_resources.res[61]))
        {
            // Empty
        }

        field_1A4_resources.res[61] = {};
        field_104_pending_resource_count = 1;

        ResourceManager::LoadResourceFile("ABEWELM.BND", BaseAliveGameObject::OnResourceLoaded_4019A0, this);
        VOnTrapDoorOpen();
        field_FC_current_motion = eAbeStates::State_136_ElumMountEnd_42E110;
    }
}

void Abe::State_140_BeesStruggling_423F30()
{
    State_0_Idle_423520();
    if ((!gBeeInstanceCount_5076B0 || !word_5076AC) && field_FC_current_motion == eAbeStates::State_140_BeesStruggling_423F30)
    {
        ToIdle_422D50();
    }
}

void Abe::State_141_BeesStrugglingOnLift_42F390()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);

    pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(12), 0);
    if (pLiftPoint)
    {
        field_B8_vely = pLiftPoint->field_B8_vely;
        if (pLiftPoint->field_6_flags.Get(Options::eDead_Bit3))
        {
            VOnTrapDoorOpen();
            field_2A8_flags.Set(Flags_2A8::e2A8_Bit1);
        }
        SetActiveCameraDelayedFromDir_401C90();
    }

    field_B8_vely = pLiftPoint->field_B8_vely;

    if (pLiftPoint->OnBottomFloor())
    {
        ToIdle_422D50();
    }
 }

void Abe::State_142_RockThrowStandingHold_429CE0()
{
    if (field_10_anim.field_92_current_frame >= 4)
    {
        if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594 | sInputKey_Up_4C6598 | sInputKey_Down_4C659C))
        {
            if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590))
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_19D_throw_direction = 0;
                }
                else
                {
                    field_19D_throw_direction = 2;
                }
            }
            else if (sInputObject_5009E8.isPressed(sInputKey_Left_4C6594))
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_19D_throw_direction = 2;
                }
                else
                {
                    field_19D_throw_direction = 0;
                }
            }
            else if (sInputObject_5009E8.isPressed(sInputKey_Up_4C6598))
            {
                field_19D_throw_direction = 1;
            }
            else
            {
                field_19D_throw_direction = 3;
            }
            field_FC_current_motion = eAbeStates::State_143_RockThrowStandingThrow_429FD0;
        }
    }

    if (sInputObject_5009E8.isReleased(sInputKey_ThrowItem_4C65B4))
    {
        field_198_pThrowable->VToDead();
        field_198_pThrowable = nullptr;
        field_FC_current_motion = eAbeStates::State_144_RockThrowStandingEnd_429DE0;
        if (!gInfiniteGrenades_5076EC)
        {
            field_19C_throwable_count++;
        }
    }
}

void Abe::State_143_RockThrowStandingThrow_429FD0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        SFX_Play_43AD70(SoundEffect::AirStream_28, 0, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        ToIdle_422D50();
    }
}

void Abe::State_144_RockThrowStandingEnd_429DE0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        ToIdle_422D50();
    }
}

void Abe::State_145_RockThrowCrouchingHold_428930()
{
    if (field_10_anim.field_92_current_frame >= 4)
    {
        if (sInputObject_5009E8.isPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594 | sInputKey_Up_4C6598 | sInputKey_Down_4C659C))
        {
            field_19D_throw_direction = 4;
            field_FC_current_motion = eAbeStates::State_146_RockThrowCrouchingThrow_4289F0;
            if (field_198_pThrowable->field_4_typeId == Types::eMeat_54)
            {
                field_19D_throw_direction = 5;
            }
        }
    }

    if (sInputObject_5009E8.isReleased(sInputKey_ThrowItem_4C65B4))
    {
        field_198_pThrowable->VToDead();
        field_198_pThrowable = nullptr;
        field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
        if (!gInfiniteGrenades_5076EC)
        {
            field_19C_throwable_count++;
        }
    }
}

void Abe::State_146_RockThrowCrouchingThrow_4289F0()
{
    FollowLift_42EE90();

    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_43AD70(SoundEffect::AirStream_28, 0, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
    }
}

void Abe::State_147_ShotRolling_4295C0()
{
    ElumFree_4228F0();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);

    State_3_Fall_42E7F0();

    if (field_FC_current_motion != eAbeStates::State_147_ShotRolling_4295C0 && !gAbeInvunerableCheat_5076E4)
    {
        field_FC_current_motion = eAbeStates::State_147_ShotRolling_4295C0;
        field_F4_pLine = nullptr;
        field_AC_ypos += (field_BC_sprite_scale * FP_FromInteger(4));
    }

    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_AC_ypos += FP_FromInteger(240);
            Abe_SFX_42A4D0(17u, 0, 0, this);
            ToDeathDropFall_42C3D0();
        }
    }
}

void Abe::State_148_Shot_4296A0()
{
    ElumFree_4228F0();

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEvent_10, this);
    
    State_3_Fall_42E7F0();

    if (field_FC_current_motion != eAbeStates::State_148_Shot_4296A0 && !gAbeInvunerableCheat_5076E4)
    {
        field_FC_current_motion = eAbeStates::State_148_Shot_4296A0;
        field_F4_pLine = nullptr;
        field_AC_ypos += (field_BC_sprite_scale * FP_FromInteger(4));
    }

    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_AC_ypos += FP_FromInteger(240);
            Abe_SFX_42A4D0(17u, 0, 0, this);
            ToDeathDropFall_42C3D0();
        }
    }
}

void Abe::State_149_PickupItem_42A030()
{
    if (field_10_anim.field_92_current_frame == 7)
    {
        SFX_Play_43AD70(SoundEffect::PickupItem_33, 0, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eAbeStates::State_19_CrouchIdle_4284C0;
    }

    if (sActiveHero_507678->field_10_anim.field_92_current_frame >= 5)
    {
        if (field_15C_pThrowable)
        {
            field_15C_pThrowable->field_C_refCount--;
            field_15C_pThrowable->VOnPickUpOrSlapped();
            field_15C_pThrowable = nullptr;
        }
    }
}

void Abe::State_150_Chant_42FD50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_151_ChantEnd_430530()
{
    SND_Seq_Stop_477A60(11u);

    FollowLift_42EE90();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_2A8_flags.Get(Flags_2A8::e2A8_Bit11_bLaughAtChantEnd))
        {
            field_FC_current_motion = eAbeStates::State_12_Speak_42FBD0;
            Abe_SFX_42A4D0(8u, 0, 0, this);
            field_2A8_flags.Clear(Flags_2A8::e2A8_Bit11_bLaughAtChantEnd);
        }
        else
        {
            ToIdle_422D50();
        }
    }
}

void Abe::State_152_ElumFallOffEdge_42E030()
{
    // Empty
}

void Abe::State_153_ElumFall_42E040()
{
    // Empty
}

void Abe::State_154_ElumLand_42E050()
{
    // Empty
}

void Abe::State_155_ElumJumpToFall_42E060()
{
    // Empty
}

void Abe::State_156_DoorEnter_42D370()
{
    NOT_IMPLEMENTED();
}

void Abe::State_157_DoorExit_42D780()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
            nullptr,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);

        VOn_TLV_Collision(field_F0_pTlv);

        ToIdle_422D50();
    }
}

void Abe::State_158_ElumKnockback_42E070()
{
    // Empty
}

void Abe::State_159_423360()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_422D50();
    }
}

void Abe::State_160_4233A0()
{
    State_159_423360();
}

void Abe::State_161_4233E0()
{
    State_159_423360();
}

void Abe::State_162_ToShrykull_42F410()
{
    if (field_110_state == 0)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

            field_110_state = 1;

            auto pShrykull = ao_new<Shrykull>();
            if (pShrykull)
            {
                pShrykull->ctor_463880();
            }
        }
    }
}

void Abe::State_163_ShrykullEnd_42F520()
{
    if (field_114_gnFrame)
    {
        field_114_gnFrame = field_114_gnFrame - 1;
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (!field_168_ring_pulse_timer)
        {
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 117, 0, 0));
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 0, 0));
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 0, 0));
        }

        field_FC_current_motion = eAbeStates::State_9_Speak_42FA50;
        Abe_SFX_42A4D0(8u, 0, 0, this);
    }
}

void Abe::State_164_PoisonGasDeath_42A120()
{
    switch (field_10_anim.field_92_current_frame)
    {
    case 0:
        SFX_Play_43AE60(SoundEffect::Choke_98, 127, 128, 0);
        break;
    case 9:
        SFX_Play_43AE60(SoundEffect::Choke_98, 127, 384, 0);
        break;
    case 28:
        SFX_Play_43AE60(SoundEffect::Choke_98, 127, 640, 0);
        break;
    case 32:
        Abe_SFX_2_42A220(6u, 80, 0, this);
        break;
    case 50:
        Abe_SFX_2_42A220(6u, 100, -200, this);
        break;
    case 53:
        Abe_SFX_2_42A220(6u, 50, -200, this);
        break;
    default:
        break;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        const int v2 = field_114_gnFrame;
        field_114_gnFrame--;
        if (v2 == 0)
        {
            ToDieFinal_42C400();
        }
    }
}

END_NS_AO;


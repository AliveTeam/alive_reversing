#include "stdafx_ao.h"
#include "Function.hpp"
#include "Abe.hpp"
#include "ThrowableArray.hpp"
#include "Elum.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "CircularFade.hpp"
#include "DeathFadeOut.hpp"
//#include "Grenade.hpp"
//#include "OrbWhirlWind.hpp"
#include "PullRingRope.hpp"
#include "ScreenManager.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"

START_NS_AO;

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

int CC GridXMidPos_41FA60(FP scale, int xpos)
{
    if (scale == FP_FromDouble(0.5))
    {
        return 13 * xpos + 245;
    }

    if (scale == FP_FromInteger(1))
    {
        return 25 * xpos + 240;
    }

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
    field_A8_xpos = FP_FromInteger(pPoint.field_0_x + GridXMidPos_41FA60(field_BC_sprite_scale, 4));
    field_AC_ypos = FP_FromInteger(pPoint.field_2_y + 240);
    field_120 = 0;
    field_124 = 0;
    field_E8_LastLineYPos = field_AC_ypos;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_F4_pLine = nullptr;
    field_FC_current_motion = eAbeStates::State_3_Fall_42E7F0;

    field_112 = -1;
    field_10_anim.field_C_layer = 32;
    field_15C_pUnknown = nullptr;
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
    field_10A_flags.Set(Flags_10A::e10A_Bit4);

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
    field_E6_last_state = 0;
    field_110_state = 0;
    field_168_ring_pulse_timer = 0;
    field_114_gnFrame = gnFrameCount_507670;
    field_F0_pTlv = nullptr;
    field_160_pRope = nullptr;
    field_F8_pLiftPoint = nullptr;
    field_130_say = -1;
    field_134_auto_say_timer = 0;
    field_EC_oldY = 1;
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

    if (field_15C_pUnknown)
    {
        field_15C_pUnknown->field_C_refCount--;
        field_15C_pUnknown->field_6_flags.Set(Options::eDead_Bit3);
        field_15C_pUnknown = nullptr;
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

void Abe::vUpdate_41FDB0()
{
    NOT_IMPLEMENTED();
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

void Abe::Free_Shrykull_Resources_42F4C0()
{
    NOT_IMPLEMENTED();
}

void Abe::FreeElumRes_420F80()
{
    NOT_IMPLEMENTED();
}

void Abe::ToDeathDropFall_42C3D0()
{
    NOT_IMPLEMENTED();
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
                !field_10A_flags.Get(Flags_10A::e10A_Bit5) &&
                field_FC_current_motion != eAbeStates::State_156_DoorEnter_42D370)
            {
                field_146_zone_number = pContinuePointTlv->field_18_zone_number;
                field_148_clear_from_id = pContinuePointTlv->field_1A_clear_from_id;
                field_14A_clear_to_id = pContinuePointTlv->field_1C_clear_to_id;

                field_138_zone_top_left = pContinuePointTlv->field_10_top_left;
                field_13C_zone_bottom_right = pContinuePointTlv->field_14_bottom_right;

                field_14C_saved_sprite_scale = field_BC_sprite_scale;

                field_2A8_flags.Set(Flags_2A8::e2A8_eBit16_abe_spawn_dir, pContinuePointTlv->field_20_abe_direction & 1);

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

            Event_Broadcast_417220(kEvent_0, this);
            Event_Broadcast_417220(kEvent_10, this);
            Event_Broadcast_417220(kEvent_14, this);
            Event_Broadcast_417220(kEvent_1, this);

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
    NOT_IMPLEMENTED();
}

void Abe::State_2_StandingTurn_426040()
{
    NOT_IMPLEMENTED();
}

void Abe::State_3_Fall_42E7F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4_WalkToIdle_4243C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_5_MidWalkToIdle_424490()
{
    NOT_IMPLEMENTED();
}

void Abe::State_6_WalkBegin_424300()
{
    NOT_IMPLEMENTED();
}

void Abe::State_7_Speak_42F950()
{
    NOT_IMPLEMENTED();
}

void Abe::State_8_Speak_42F9D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_9_Speak_42FA50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_10_Speak_42FAD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_11_Speak_42FB50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_12_Speak_42FBD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_13_Speak_42FC50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_14_Speak_42FCD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_15_Null_42A210()
{
    NOT_IMPLEMENTED();
}

void Abe::State_16_HoistBegin_426E40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_17_HoistIdle_4269E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_18_HoistLand_426EB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_19_CrouchIdle_4284C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_20_CrouchToStand_428AF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_21_StandToCrouch_427F40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_22_CrouchSpeak_428A30()
{
    NOT_IMPLEMENTED();
}

void Abe::State_23_CrouchSpeak_428A90()
{
    NOT_IMPLEMENTED();
}

void Abe::State_24_RollBegin_427A20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_25_RollLoop_427BB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_26_RollEnd_427EA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_27_RunSlideStop_425B60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_28_RunTurn_425CE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_29_HopBegin_4267B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_30_HopMid_4264D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_31_HopLand_426940()
{
    NOT_IMPLEMENTED();
}

void Abe::State_32_RunJumpBegin_427440()
{
    NOT_IMPLEMENTED();
}

void Abe::State_33_RunJumpMid_426FA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_34_RunJumpLand_427560()
{
    NOT_IMPLEMENTED();
}

void Abe::State_35_RunLoop_425060()
{
    NOT_IMPLEMENTED();
}

void Abe::State_36_DunnoBegin_423260()
{
    NOT_IMPLEMENTED();
}

void Abe::State_37_DunnoMid_4232C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_38_DunnoEnd_423310()
{
    NOT_IMPLEMENTED();
}

void Abe::State_39_CrouchTurn_4288C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_40_RunToRoll_427AE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_41_StandingToRun_425530()
{
    NOT_IMPLEMENTED();
}

void Abe::State_42_SneakLoop_424BB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_43_WalkToSneak_424790()
{
    NOT_IMPLEMENTED();
}

void Abe::State_44_SneakToWalk_4249A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45_MidWalkToSneak_424890()
{
    NOT_IMPLEMENTED();
}

void Abe::State_46_MidSneakToWalk_424AA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_47_SneakBegin_424ED0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_48_SneakToIdle_424F80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_49_MidSneakToIdle_424FF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_50_WalkToRun_424560()
{
    NOT_IMPLEMENTED();
}

void Abe::State_51_MidWalkToRun_424670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_52_RunToWalk_4255E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_53_MidRunToWalk_4256E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_54_RunTurnToRun_425EA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_55_RunTurnToWalk_425F70()
{
    NOT_IMPLEMENTED();
}

void Abe::State_56_RunJumpLandRun_4278E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_57_RunJumpLand_Walk_427980()
{
    NOT_IMPLEMENTED();
}

void Abe::State_58_ToSpeak_42F8D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_59_DeathDropFall_42CBE0()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void Abe::State_64_LedgeAscend_428B60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_65_LedgeDescend_428C00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_66_LedgeHang_428D90()
{
    NOT_IMPLEMENTED();
}

void Abe::State_67_ToOffScreenHoist_428C50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_68_LedgeHangWobble_428E50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_69_RingRopePull_4299B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_70_Knockback_428FB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_71_KnockbackGetUp_429240()
{
    NOT_IMPLEMENTED();
}

void Abe::State_72_PushWall_4292A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_73_RollingKnockback_4291D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_74_JumpIntoWell_430EC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_75_ToInsideOfAWellLocal_431090()
{
    NOT_IMPLEMENTED();
}

void Abe::State_76_ToWellShotOut_431720()
{
    NOT_IMPLEMENTED();
}

void Abe::State_77_WellBegin_430F10()
{
    NOT_IMPLEMENTED();
}

void Abe::State_78_InsideWellLocal_4310A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_79_WellShotOut_431730()
{
    NOT_IMPLEMENTED();
}

void Abe::State_80_430EF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_81_InsideWellExpress_431320()
{
    NOT_IMPLEMENTED();
}

void Abe::State_82_WellExpressShotOut_4315A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_83_430F00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_84_431080()
{
    NOT_IMPLEMENTED();
}

void Abe::State_85_431710()
{
    NOT_IMPLEMENTED();
}

void Abe::State_86_FallLandDie_42EDD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_87_428FA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_88_HandstoneBegin_430590()
{
    NOT_IMPLEMENTED();
}

void Abe::State_89_HandstoneEnd_430E80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_90_GrenadeMachineUse_430EA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_91_FallingFromGrab_429780()
{
    NOT_IMPLEMENTED();
}

void Abe::State_92_ForceDownFromHoist_4297C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_93_WalkOffEdge_429840()
{
    NOT_IMPLEMENTED();
}

void Abe::State_94_RunOffEdge_429860()
{
    NOT_IMPLEMENTED();
}

void Abe::State_95_SneakOffEdge_429880()
{
    NOT_IMPLEMENTED();
}

void Abe::State_96_HopToFall_4298A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_97_RunJumpToFall_429930()
{
    NOT_IMPLEMENTED();
}

void Abe::State_98_LandSoft_42ED40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_99_LedgeHoistUp_426DC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_100_RollOffLedge_429950()
{
    NOT_IMPLEMENTED();
}

void Abe::State_101_LeverUse_429970()
{
    NOT_IMPLEMENTED();
}

void Abe::State_102_ElumWalkLoop_42DCA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_103_ElumIdle_42DCD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_104_ElumRunSlideStop_42DF00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_105_ElumRunTurn_42DF10()
{
    NOT_IMPLEMENTED();
}

void Abe::State_106_42DF20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_107_ElumHopBegin_42DF30()
{
    NOT_IMPLEMENTED();
}

void Abe::State_108_ElumHopMid_42DF40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_109_ElumHopLand_42DF50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_110_ElumRunJumpBegin_42DF60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_111_ElumRunJumpMid_42DF70()
{
    NOT_IMPLEMENTED();
}

void Abe::State_112_ElumRunJumpLand_42DF80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_113_ElumTurn_42DF90()
{
    NOT_IMPLEMENTED();
}

void Abe::State_114_ElumRunLoop_42DFA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_115_ElumSpeak_4299F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_116_42DFB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_117_ElumWalkBegin_42DFC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_118_ElumRunBegin_42DFD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_119_42DFE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_120_ElumRunToWalk_42DFF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_121_ElumMidRunToWalk_42E000()
{
    NOT_IMPLEMENTED();
}

void Abe::State_122_ElumRunTurnToRun_42E010()
{
    NOT_IMPLEMENTED();
}

void Abe::State_123_ElumRunTurnToWalk_42E020()
{
    NOT_IMPLEMENTED();
}

void Abe::State_124_ElumWalkEnd_42DCB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_125_ElumMidWalkEnd_42DCC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_126_ElumBeesStruggling_42E080()
{
    NOT_IMPLEMENTED();
}

void Abe::State_127_SlapBomb_429A20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_128_KnockForward_429330()
{
    NOT_IMPLEMENTED();
}

void Abe::State_129_RollingKnockForward_4294F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_130_KnockForwardGetUp_429560()
{
    NOT_IMPLEMENTED();
}

void Abe::State_131_LiftUseUp_42F150()
{
    NOT_IMPLEMENTED();
}

void Abe::State_132_LiftUseDown_42F170()
{
    NOT_IMPLEMENTED();
}

void Abe::State_133_LiftGrabBegin_42EF20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_134_LiftGrabEnd_42EFE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_135_LiftGrabIdle_42F000()
{
    NOT_IMPLEMENTED();
}

void Abe::State_136_ElumMountEnd_42E110()
{
    NOT_IMPLEMENTED();
}

void Abe::State_137_ElumUnmountBegin_42E2B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_138_ElumUnmountEnd_42E390()
{
    NOT_IMPLEMENTED();
}

void Abe::State_139_ElumMountBegin_42E090()
{
    NOT_IMPLEMENTED();
}

void Abe::State_140_BeesStruggling_423F30()
{
    NOT_IMPLEMENTED();
}

void Abe::State_141_BeesStrugglingOnLift_42F390()
{
    NOT_IMPLEMENTED();
}

void Abe::State_142_RockThrowStandingHold_429CE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_143_RockThrowStandingThrow_429FD0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_144_RockThrowStandingEnd_429DE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_145_RockThrowCrouchingHold_428930()
{
    NOT_IMPLEMENTED();
}

void Abe::State_146_RockThrowCrouchingThrow_4289F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_147_ShotRolling_4295C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_148_Shot_4296A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_149_PickupItem_42A030()
{
    NOT_IMPLEMENTED();
}

void Abe::State_150_Chant_42FD50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_151_ChantEnd_430530()
{
    NOT_IMPLEMENTED();
}

void Abe::State_152_ElumFallOffEdge_42E030()
{
    NOT_IMPLEMENTED();
}

void Abe::State_153_ElumFall_42E040()
{
    NOT_IMPLEMENTED();
}

void Abe::State_154_ElumLand_42E050()
{
    NOT_IMPLEMENTED();
}

void Abe::State_155_ElumJumpToFall_42E060()
{
    NOT_IMPLEMENTED();
}

void Abe::State_156_DoorEnter_42D370()
{
    NOT_IMPLEMENTED();
}

void Abe::State_157_DoorExit_42D780()
{
    NOT_IMPLEMENTED();
}

void Abe::State_158_ElumKnockback_42E070()
{
    NOT_IMPLEMENTED();
}

void Abe::State_159_423360()
{
    NOT_IMPLEMENTED();
}

void Abe::State_160_4233A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_161_4233E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_162_ToShrykull_42F410()
{
    NOT_IMPLEMENTED();
}

void Abe::State_163_ShrykullEnd_42F520()
{
    NOT_IMPLEMENTED();
}

void Abe::State_164_PoisonGasDeath_42A120()
{
    NOT_IMPLEMENTED();
}

END_NS_AO;


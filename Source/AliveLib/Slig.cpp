#include "stdafx.h"
#include "Slig.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "DDCheat.hpp"
#include "BaseGameObject.hpp"
#include "NakedSlig.hpp" // TODO: SFX playing only
#include "SnoozeParticle.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Particle.hpp"
#include "Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "ScreenShake.hpp"
#include "LiftPoint.hpp"
#include "Switch.hpp"
#include "SwitchStates.hpp"

EXPORT void CC Start_Slig_sounds_4CB980(CameraPos /*a1*/, int /*kZero*/)
{
    // TODO: Probably a static function on the Path object
    NOT_IMPLEMENTED();
}

// This is a left bound, right bound and a persist.
struct Path_Slig_Bound : public Path_TLV
{
    __int16 field_10_slig_id;
    __int16 field_12_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig_Bound, 0x14);

using Path_Slig_LeftBound = Path_Slig_Bound;
using Path_Slig_RightBound = Path_Slig_Bound;
using Path_Slig_Persist = Path_Slig_Bound;


TintEntry stru_560570[15] =
{
    { 1u, 102u, 127u, 118u },
    { 2u, 102u, 127u, 118u },
    { 3u, 102u, 127u, 118u },
    { 4u, 102u, 127u, 118u },
    { 5u, 102u, 127u, 118u },
    { 6u, 102u, 127u, 118u },
    { 7u, 102u, 127u, 118u },
    { 8u, 102u, 127u, 118u },
    { 9u, 102u, 127u, 118u },
    { 10u, 102u, 127u, 118u },
    { 11u, 102u, 127u, 118u },
    { 12u, 102u, 127u, 118u },
    { 13u, 102u, 127u, 118u },
    { 14u, 102u, 127u, 118u },
    { -1, 102u, 127u, 118u }
};

ALIVE_VAR(1, 0xBAF7E4, int, dword_BAF7E4, 0);
ALIVE_VAR(1, 0xBAF7E8, short, word_BAF7E8, 0);

const int sSligFrameTables_547318[52] =
{
    135512,
    135804,
    135360,
    135824,
    135440,
    136088,
    135544,
    135512,
    135696,
    135576,
    135916,
    135632,
    135652,
    135768,
    135844,
    135876,
    135512,
    135788,
    135896,
    135936,
    136048,
    135976,
    136132,
    136012,
    136048,
    135976,
    136132,
    136012,
    136048,
    135976,
    136132,
    136012,
    33552,
    33448,
    33184,
    33228,
    33348,
    33348,
    30560,
    30592,
    30628,
    9204,
    9208,
    9260,
    13016,
    12612,
    23048,
    23072,
    23200,
    23148,
    23096,
    12660
};

const TSligMotionFn sSlig_motion_table_5604A0[52] =
{
    &Slig::M_StandIdle_0_4B4EC0,
    &Slig::M_StandToWalk_1_4B5F70,
    &Slig::M_Walking_2_4B5BC0,
    &Slig::M_StandToRun_3_4B62F0,
    &Slig::M_Running_4_4B6000,
    &Slig::M_TurnAroundStanding_5_4B6390,
    &Slig::M_Shoot_6_4B55A0,
    &Slig::M_Falling_7_4B42D0,
    &Slig::M_SlidingToStand_8_4B6520,
    &Slig::M_SlidingTurn_9_4B6680,
    &Slig::M_SlidingTurnToWalk_10_4B6800,
    &Slig::M_SlidingTurnToRun_11_4B6850,
    &Slig::M_ReloadGun_12_4B5530,
    &Slig::M_ShootToStand_13_4B5580,
    &Slig::M_SteppingToStand_14_4B8480,
    &Slig::M_StandingToStep_15_4B83B0,
    &Slig::M_DepossessingAbort_16_4B8250,
    &Slig::M_GameSpeak_17_4B5290,
    &Slig::M_WalkToStand_18_4B5FC0,
    &Slig::M_Recoil_19_4B8270,
    &Slig::M_SpeakHereBoy_20_4B5330,
    &Slig::M_SpeakHi_21_4B53D0,
    &Slig::M_SpeakFreeze_22_4B53F0,
    &Slig::M_SpeakGitIm_23_4B5410,
    &Slig::M_SpeakLaugh_24_4B5430,
    &Slig::M_SpeakBullShit1_25_4B5450,
    &Slig::M_SpeakLookOut_26_4B5470,
    &Slig::M_SpeakBullShit2_27_4B5490,
    &Slig::M_SpeakPanic_28_4B54B0,
    &Slig::M_SpeakWhat_29_4B54D0,
    &Slig::M_SpeakAIFreeze_30_4B54F0,
    &Slig::M_Blurgh_31_4B5510,
    &Slig::M_Sleeping_32_4B89A0,
    &Slig::M_SleepingToStand_33_4B8C50,
    &Slig::M_Knockback_34_4B68A0,
    &Slig::M_KnockbackToStand_35_4B6A30,
    &Slig::M_Depossessing_36_4B7F30,
    &Slig::M_Possess_37_4B72C0,
    &Slig::M_OutToFall_38_4B4570,
    &Slig::M_FallingInitiate_39_4B4640,
    &Slig::M_LandingSoft_40_4B4530,
    &Slig::M_LandingFatal_41_4B4680,
    &Slig::M_ShootZ_42_4B7560,
    &Slig::M_ShootZtoStand_43_4B77E0,
    &Slig::M_Smash_44_4B6B90,
    &Slig::M_PullLever_45_4B8950,
    &Slig::M_LiftGrip_46_4B3700,
    &Slig::M_LiftUngrip_47_4B3820,
    &Slig::M_LiftGripping_48_4B3850,
    &Slig::M_LiftUp_49_4B3930,
    &Slig::M_LiftDown_50_4B3960,
    &Slig::M_Beat_51_4B6C00
};

const TSligAIFn sSlig_ai_table_5605AC[36] =
{
    &Slig::AI_Death_0_4BBFB0,
    &Slig::AI_ReturnControlToAbeAndDie_1_4BC410,
    &Slig::AI_Possessed_2_4BBCF0,
    &Slig::AI_DeathDropDeath_3_4BC1E0,
    &Slig::AI_ListeningToGlukkon_4_4B9D20,
    &Slig::AI_Empty_5_4B3220,
    &Slig::AI_Empty_6_4B3420,
    &Slig::AI_SpottedEnemy_7_4B3240,
    &Slig::AI_Empty_8_4B3120,
    &Slig::AI_Empty_9_4B3440,
    &Slig::AI_AbeDead_10_4B3460,
    &Slig::AI_KilledAbe_11_4B35C0,
    &Slig::AI_PanicTurning_12_4BC490,
    &Slig::AI_PanicRunning_13_4BC780,
    &Slig::AI_PanicYelling_14_4BCA70,
    &Slig::AI_Idle_15_4BD800,
    &Slig::AI_StopChasing_16_4BCE30,
    &Slig::AI_Chasing2_17_4BCBD0,
    &Slig::AI_Chasing1_18_4BCEB0,
    &Slig::AI_Turning_19_4BDDD0,
    &Slig::AI_StoppingNextToMudokon_20_4BF1E0,
    &Slig::AI_Walking_21_4BE0C0,
    &Slig::AI_GetAlertedTurn_22_4BE990,
    &Slig::AI_GetAlerted_23_4BEC40,
    &Slig::AI_BeatingUp_24_4BF2B0,
    &Slig::AI_DiscussionWhat_25_4BF380,
    &Slig::AI_Empty_26_4BF620,
    &Slig::AI_Empty_27_4BF600,
    &Slig::AI_ShootingFromBackground_28_4BFA70,
    &Slig::AI_Shooting_29_4BF750,
    &Slig::AI_SpottedEnemyFromBackground_30_4BFA30,
    &Slig::AI_WakingUp_31_4B9390,
    &Slig::AI_Inactive_32_4B9430,
    &Slig::AI_Paused_33_4B8DD0,
    &Slig::AI_Sleeping_34_4B9170,
    &Slig::AI_GameEnder_35_4BF640
};

struct FnPair
{
    TSligAIFn mOurFn;
    DWORD mOriginal;
    const char* fnName;
};

static FnPair sAiFns[36] =
{
    { &Slig::AI_Death_0_4BBFB0,  0x40128F, "AI_Death_0", },
    { &Slig::AI_ReturnControlToAbeAndDie_1_4BC410,  0x40371F, "AI_ReturnControlToAbeAndDie_1", },
    { &Slig::AI_Possessed_2_4BBCF0,  0x40458E, "AI_Possessed_2", },
    { &Slig::AI_DeathDropDeath_3_4BC1E0,  0x401EC9, "AI_DeathDropDeath_3", },
    { &Slig::AI_ListeningToGlukkon_4_4B9D20,  0x4015A0, "AI_ListeningToGlukkon_4", },
    { &Slig::AI_Empty_5_4B3220,  0x4B3220, "AI_Empty_5", },
    { &Slig::AI_Empty_6_4B3420,  0x4B3420, "AI_Empty_6", },
    { &Slig::AI_SpottedEnemy_7_4B3240,  0x4032AB, "AI_SpottedEnemy_7", },
    { &Slig::AI_Empty_8_4B3120,  0x4B3120, "AI_Empty_8", },
    { &Slig::AI_Empty_9_4B3440,  0x4B3440, "AI_Empty_9", },
    { &Slig::AI_AbeDead_10_4B3460, 0x401E60, "AI_AbeDead_10", },
    { &Slig::AI_KilledAbe_11_4B35C0, 0x40120D, "AI_KilledAbe_11", },
    { &Slig::AI_PanicTurning_12_4BC490, 0x40479B, "AI_PanicTurning_12", },
    { &Slig::AI_PanicRunning_13_4BC780, 0x40144C, "AI_PanicRunning_13", },
    { &Slig::AI_PanicYelling_14_4BCA70, 0x403364, "AI_PanicYelling_14", },
    { &Slig::AI_Idle_15_4BD800, 0x403F85, "AI_Idle_15", },
    { &Slig::AI_StopChasing_16_4BCE30, 0x4021B2, "AI_StopChasing_16", },
    { &Slig::AI_Chasing2_17_4BCBD0, 0x403BCA, "AI_Chasing2_17", },
    { &Slig::AI_Chasing1_18_4BCEB0, 0x403E36, "AI_Chasing1_18", },
    { &Slig::AI_Turning_19_4BDDD0, 0x403ABC, "AI_Turning_19", },
    { &Slig::AI_StoppingNextToMudokon_20_4BF1E0, 0x4020C2, "AI_StoppingNextToMudokon_20", },
    { &Slig::AI_Walking_21_4BE0C0, 0x402A59, "AI_Walking_21", },
    { &Slig::AI_GetAlertedTurn_22_4BE990, 0x40266C, "AI_GetAlertedTurn_22", },
    { &Slig::AI_GetAlerted_23_4BEC40, 0x4020D1, "AI_GetAlerted_23", },
    { &Slig::AI_BeatingUp_24_4BF2B0, 0x403B25, "AI_BeatingUp_24", },
    { &Slig::AI_DiscussionWhat_25_4BF380, 0x403CE2, "AI_DiscussionWhat_25", },
    { &Slig::AI_Empty_26_4BF620, 0x4BF620, "AI_Empty_26", },
    { &Slig::AI_Empty_27_4BF600, 0x4BF600, "AI_Empty_27", },
    { &Slig::AI_ShootingFromBackground_28_4BFA70, 0x4025B3, "AI_ShootingFromBackground_28", },
    { &Slig::AI_Shooting_29_4BF750, 0x40308F, "AI_Shooting_29", },
    { &Slig::AI_SpottedEnemyFromBackground_30_4BFA30, 0x403215, "AI_SpottedEnemyFromBackground_30", },
    { &Slig::AI_WakingUp_31_4B9390, 0x403E0E, "AI_WakingUp_31", },
    { &Slig::AI_Inactive_32_4B9430, 0x4038CD, "AI_Inactive_32", },
    { &Slig::AI_Paused_33_4B8DD0, 0x40346D, "AI_Paused_33", },
    { &Slig::AI_Sleeping_34_4B9170, 0x402B17, "AI_Sleeping_34", },
    { &Slig::AI_GameEnder_35_4BF640, 0x4022B1, "AI_GameEnder_35" },
};

#if _WIN32 || !_WIN64
static const FnPair& GetOriginalFn(TSligAIFn fn)
{
    // If not running as standalone set the address to be
    // the address of the real function rather than the reimpl as the real
    // game code compares the function pointer addresses (see IsBrain(x)).
    for (const auto& addrPair : sAiFns)
    {
        if (addrPair.mOurFn == fn || memcmp(&addrPair.mOriginal, &fn, sizeof(DWORD)) == 0)
        {
            return addrPair;
        }
    }
    ALIVE_FATAL("No matching address!");
}
#endif

void Slig::SetBrain(TSligAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn).mOriginal;
        // Hack to overwrite the member function pointer bytes with arbitrary data
        memcpy(&field_154_brain_state, &actualAddressToUse, sizeof(DWORD));
        return;
    }
#endif
    field_154_brain_state = fn;
}

bool Slig::BrainIs(TSligAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn).mOriginal;
        TSligAIFn hack = nullptr;
        memcpy(&hack, &actualAddressToUse, sizeof(DWORD));
        return hack == field_154_brain_state;
    }
#endif
    return field_154_brain_state == fn;
}

Slig* Slig::ctor_4B1370(Path_Slig* pTlv, int tlvInfo)
{
    ctor_408240(17);

    field_160 = -1;
    field_176 = -1;
    field_174 = 0;
    
    SetVTable(this, 0x547460);

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 412, 1, 0));
    Animation_Init_424E10(135512, 160, 68, field_10_resources_array.ItemAt(0), 1, 1);

    field_4_typeId = Types::eSlig_125;

    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_292 &= ~1u;

    dword_BAF7E4 = 0;
    field_216 &= ~0xDu;


    field_120_timer = 0;
    field_11E = 45 * (Math_NextRandom() % 5 - 2);
    field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_124 = 0;
    field_150 = 0;
    field_14C = 0;
    field_FC_pPathTLV = pTlv;
    
    field_218_tlv_data = *pTlv;

    field_110_id = -1;
    
    field_106_current_motion = eSligMotions::M_Falling_7_4B42D0;

    field_124 = 0;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_130 = FP_FromInteger(0);
    field_118_tlvInfo = tlvInfo;
    field_128_input = 0;
    field_158 = 0;
    field_15A = 0;
    field_15C = 0;
    field_136 = -1;
    field_208 = 0;
    field_20C = -1;
    field_20E = 0;
    field_210 = 0;
    field_134_res_idx = 0;

    field_20_animation.field_1C_fn_ptr_array = kSlig_Anim_Frame_Fns_55EFAC;

    if (pTlv->field_10_scale)
    {
        if (pTlv->field_10_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 14;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 33;
        field_D6_scale = 1;
    }

    SetBaseAnimPaletteTint_425690(&stru_560570[0], gMap_5C3030.sCurrentLevelId_5C3030, 412);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
        field_BC_ypos = hitY;
    }

    MapFollowMe_408D10(TRUE);
    
    Init_4BB0D0();
    
    vStackOnObjectsOfType_425840(Types::eSlig_125);
    
    if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8 && gMap_5C3030.sCurrentPathId_5C3032 == 2 && gMap_5C3030.sCurrentCamId_5C3034 == 5)
    {
        field_DA_xOffset = 0;
    }

    field_DC_bApplyShadows |= 2u;
    field_142 = 0;
    field_140 = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* Slig::VDestructor(signed int flags)
{
    return vdtor_4B1790(flags);
}

void Slig::VUpdate()
{
    vUpdate_4B17C0();
}

void Slig::VScreenChanged()
{
    vScreenChanged_4B1E20();
}

void Slig::VPossessed_408F70()
{
    vPossessed_4B2F10();
}

void Slig::VUnPosses_408F90()
{
    vUnPosses_4B3050();
}

void Slig::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOnTlvCollision_4B2FB0(pTlv);
}

void Slig::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_4B3690();
}

void Slig::M_StandIdle_0_4B4EC0()
{
    FP xOff = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff = FP_FromInteger(10) * field_CC_sprite_scale;
    }
    else
    {
        xOff = FP_FromInteger(-10) * field_CC_sprite_scale;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        xOff + field_B8_xpos,
        field_BC_ypos - FP_FromInteger(10),
        xOff + field_B8_xpos,
        field_BC_ypos + FP_FromInteger(10),
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16))
    {
        VOnTrapDoorOpen();
        field_106_current_motion = eSligMotions::M_FallingInitiate_39_4B4640;
        field_F8_LastLineYPos = field_BC_ypos;
        field_130 = FP_FromDouble(0.3);
        return;
    }

    if (!MainMovement_4B4720())
    {
        if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
        {
            if (Input_IsChanting_45F260())
            {
                if (!(field_292 & 1))
                {
                    field_106_current_motion = eSligMotions::M_Depossessing_36_4B7F30;
                    field_12C = sGnFrame_5C1B84 + 30;
                    SFX_Play_46FA90(17, 0);
                    return;
                }
            }
            else
            {
                const auto inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
                const auto gameSpeakCmds =
                    InputCommands::eChant |
                    InputCommands::eGameSpeak8 |
                    InputCommands::eGameSpeak7 |
                    InputCommands::eGameSpeak6 |
                    InputCommands::eGameSpeak5 |
                    InputCommands::eGameSpeak4 |
                    InputCommands::eGameSpeak3 |
                    InputCommands::eGameSpeak2 |
                    InputCommands::eGameSpeak1;

                if (inputHeld & gameSpeakCmds)
                {
                    field_128_input = inputHeld;
                    field_106_current_motion = eSligMotions::M_GameSpeak_17_4B5290;
                    return;
                }
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) >= field_12C)
        {
            field_106_current_motion = eSligMotions::M_ReloadGun_12_4B5530;
        }
    }
}

void Slig::M_StandToWalk_1_4B5F70()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSligMotions::M_Walking_2_4B5BC0;
    }
    MoveOnLine_4B4C40();
}

void Slig::M_Walking_2_4B5BC0()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0 && MusicController::sub_47FDA0(0, 0, 0) <= 4)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }

    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx * FP_FromInteger(2)))
    {
        ToStand_4B4A20();
        return;
    }
    
    MoveOnLine_4B4C40();

    if (field_106_current_motion == eSligMotions::M_Walking_2_4B5BC0)
    {
        if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 14)
        {
            Slig_Sfx_4BFFE0(2, this);

            if (!field_126)
            {
                field_126 = 1;
                MapFollowMe_408D10(TRUE);
            }

            if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
            {
                if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    field_124 = 1;
                    field_F4 = 4;
                    field_F6_anim_frame = (field_20_animation.field_92_current_frame - 5) != 0 ? 13 : 5;
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_128_input = 0;
                        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                        return;
                    }
                    field_C4_velx = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4);
                }
                field_128_input = 0;
            }
        }
        else if (field_20_animation.field_92_current_frame == 11)
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

            if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), gridSize * FP_FromInteger(2)))
            {
                field_106_current_motion = eSligMotions::M_WalkToStand_18_4B5FC0;
            }
            else if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
            {
                PlayerControlStopWalkingIfRequired_4B8540();
            }
            else
            {
                if (field_108_next_motion == eSligMotions::M_Shoot_6_4B55A0)
                {
                    field_106_current_motion = eSligMotions::M_WalkToStand_18_4B5FC0;
                    field_12C = sGnFrame_5C1B84 + 60;
                }
                else if (field_108_next_motion != -1)
                {
                    field_106_current_motion = eSligMotions::M_WalkToStand_18_4B5FC0;
                }
            }
        }
        else
        {
            field_126 = 0;
        }
    }
}

void Slig::M_StandToRun_3_4B62F0()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSligMotions::M_Running_4_4B6000;
    }

    Event_Broadcast_422BC0(kEventNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();
    }
}

void Slig::M_Running_4_4B6000()
{
    NOT_IMPLEMENTED();
}

void Slig::M_TurnAroundStanding_5_4B6390()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0 && MusicController::sub_47FDA0(0, 0, 0) <= 4)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }

    Slig_Sfx_4BFFE0(1, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_4B4A20();
    }
}

void Slig::M_Shoot_6_4B55A0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Falling_7_4B42D0()
{
    if (field_C4_velx <= FP_FromInteger(0))
    {
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx += (field_CC_sprite_scale * field_130);
            if (field_C4_velx < (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4)))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }

            if (field_C4_velx > FP_FromInteger(0))
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
    else
    {
        field_C4_velx -= (field_CC_sprite_scale * field_130);
        if (field_C4_velx > (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4)))
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
        }

        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    PathLine* ppPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision_408810(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    sub_408C40();
    if (bCollision)
    {
        switch (ppPathLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_100_pCollisionLine = ppPathLine;
            field_BC_ypos = hitY;
            field_B8_xpos = hitX;
            MapFollowMe_408D10(TRUE);
            ToStand_4B4A20();
            PlatformCollide_4B4E00();
            field_106_current_motion = eSligMotions::M_LandingSoft_40_4B4530;
            break;
        case 1u:
        case 2u:
        case 5u:
        case 6u:
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            FallKnockBackOrSmash_4B4A90();
            break;
        default:
            return;
        }
    }
}

void Slig::M_SlidingToStand_8_4B6520()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SlidingTurn_9_4B6680()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        SlowDown_4B6450(FP_FromDouble(2.125));

        if (field_106_current_motion == eSligMotions::M_SlidingTurn_9_4B6680)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(TRUE);

                if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
                {
                    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
                    {
                        field_108_next_motion = eSligMotions::M_Running_4_4B6000;
                    }
                    else
                    {
                        field_108_next_motion = eSligMotions::M_Walking_2_4B5BC0;
                    }
                }

                if (field_108_next_motion == eSligMotions::M_Running_4_4B6000)
                {
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                    field_106_current_motion = eSligMotions::M_SlidingTurnToRun_11_4B6850;
                }
                else
                {
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                    field_106_current_motion = eSligMotions::M_SlidingTurnToWalk_10_4B6800;
                }

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = -field_C4_velx;
                }
            }
        }
    }

}

void Slig::M_SlidingTurnToWalk_10_4B6800()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    MoveOnLine_4B4C40();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        field_106_current_motion = eSligMotions::M_Walking_2_4B5BC0;
    }
}

void Slig::M_SlidingTurnToRun_11_4B6850()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    MoveOnLine_4B4C40();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        field_106_current_motion = eSligMotions::M_Running_4_4B6000;
    }
}

void Slig::M_ReloadGun_12_4B5530()
{
    if (field_20_animation.field_92_current_frame == 1)
    {
        Slig_Sfx_4BFFE0(6, this);
    }
    else if (field_20_animation.field_92_current_frame == 4)
    {
        Slig_Sfx_4BFFE0(7, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_ShootToStand_13_4B5580()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MainMovement_4B4720();
    }
}

void Slig::M_SteppingToStand_14_4B8480()
{
    if (!field_20_animation.field_92_current_frame)
    {
        Slig_Sfx_4BFFE0(2, this);
    }

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        FallKnockBackOrSmash_4B4A90();
    }
    else
    {
        MoveOnLine_4B4C40();
        if (field_106_current_motion == eSligMotions::M_SteppingToStand_14_4B8480)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(TRUE);
                ToStand_4B4A20();
            }
        }
    }
}

void Slig::M_StandingToStep_15_4B83B0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
        {
            field_106_current_motion = eSligMotions::M_StandToWalk_1_4B5F70;
        }
        else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_motion = eSligMotions::M_SteppingToStand_14_4B8480;
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
            field_106_current_motion = eSligMotions::M_SteppingToStand_14_4B8480;
        }
    }
}

void Slig::M_DepossessingAbort_16_4B8250()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_GameSpeak_17_4B5290()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = GetNextMotionIncGameSpeak_4B5080(field_128_input);
        if (field_106_current_motion == -1)
        {
            ToStand_4B4A20();
            field_128_input = 0;
        }
        else
        {
            if (!BrainIs(&Slig::AI_ListeningToGlukkon_4_4B9D20))
            {
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
            field_128_input = 0;
        }
    }
}

void Slig::M_WalkToStand_18_4B5FC0()
{
    MoveOnLine_4B4C40();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_408D10(TRUE);
        if (!Slig::MainMovement_4B4720())
        {
            ToStand_4B4A20();
        }
    }
}

void Slig::M_Recoil_19_4B8270()
{
    MoveOnLine_4B4C40();

    if (field_106_current_motion == eSligMotions::M_Recoil_19_4B8270)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_408D10(TRUE);
            if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
                {
                    field_C4_velx = FP_FromInteger(0);
                    field_F4 = 6;
                    field_12C = sGnFrame_5C1B84 + 60;
                    field_F6_anim_frame = 2;
                    field_124 = 1;
                }
                else
                {
                    SND_SEQ_PlaySeq_4CA960(9, 1, 1);
                    field_106_current_motion = eSligMotions::M_ShootToStand_13_4B5580;
                }
            }
            else if (field_108_next_motion != -1)
            {
                SND_SEQ_PlaySeq_4CA960(9, 1, 1);
                field_106_current_motion = eSligMotions::M_ShootToStand_13_4B5580;
                field_108_next_motion = -1;
            }
        }
    }
    else if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_46FA90(5u, 85);
    }
    else
    {
        SFX_Play_46FA90(5u, 0);
    }
}

void Slig::M_SpeakHereBoy_20_4B5330()
{
    field_128_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = GetNextMotionIncGameSpeak_4B5080(field_128_input);
        if (field_106_current_motion == -1)
        {
            ToStand_4B4A20();
            field_128_input = 0;
        }
        else
        {
            if (!BrainIs(&Slig::AI_ListeningToGlukkon_4_4B9D20))
            {
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
            field_128_input = 0;
        }
    }
}

void Slig::M_SpeakHi_21_4B53D0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakFreeze_22_4B53F0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakGitIm_23_4B5410()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakLaugh_24_4B5430()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakBullShit1_25_4B5450()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakLookOut_26_4B5470()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakBullShit2_27_4B5490()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakPanic_28_4B54B0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakWhat_29_4B54D0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_SpeakAIFreeze_30_4B54F0()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_Blurgh_31_4B5510()
{
    M_SpeakHereBoy_20_4B5330();
}

void Slig::M_Sleeping_32_4B89A0()
{
    if (field_108_next_motion == eSligMotions::M_SleepingToStand_33_4B8C50)
    {
        // Out of sleep
        field_106_current_motion = eSligMotions::M_SleepingToStand_33_4B8C50;
        field_108_next_motion = -1;
    }
    else if (static_cast<int>(sGnFrame_5C1B84 % 60))
    {
        if (!(static_cast<int>(sGnFrame_5C1B84 - 20) % 60))
        {
            Slig_Sfx_4BFFE0(5, this);
            if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
            {
                auto pSnoozeParticle = alive_new<SnoozeParticle>();
                if (pSnoozeParticle)
                {
                    FP xOff = {};
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        xOff = -(field_CC_sprite_scale * FP_FromInteger(-20));
                    }
                    else
                    {
                        xOff = (field_CC_sprite_scale * FP_FromInteger(-20));
                    }
                    pSnoozeParticle->ctor_4B06F0(
                        xOff + field_B8_xpos,
                        field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-10)),
                        field_20_animation.field_C_render_layer,
                        field_20_animation.field_14_scale);
                }
            }
        }
    }
    else
    {
        Slig_Sfx_4BFFE0(4, this);

        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            auto pSnoozeParticle = alive_new<SnoozeParticle>();
            if (pSnoozeParticle)
            {
                FP xOff = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    xOff = -(field_CC_sprite_scale * FP_FromInteger(-20));
                }
                else
                {
                    xOff = (field_CC_sprite_scale * FP_FromInteger(-20));
                }

                pSnoozeParticle->ctor_4B06F0(
                    xOff + field_B8_xpos,
                    field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-10)),
                    field_20_animation.field_C_render_layer,
                    field_20_animation.field_14_scale);
            }
        }
    }
}

void Slig::M_SleepingToStand_33_4B8C50()
{
    // OWI hack - kill all particles, even if they're not ours!
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_124)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = 2;
        }
    }

    if (field_20_animation.field_92_current_frame >= 2 && field_20_animation.field_92_current_frame <= 10)
    {
        Slig_Sfx_4BFFE0(0, this);
    }

    if (field_20_animation.field_92_current_frame == 9)
    {
        Slig_Sfx_4BFFE0(2, this);
    }

    switch (field_20_animation.field_92_current_frame)
    {
    case 9:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B8_xpos -= (field_CC_sprite_scale * FP_FromInteger(7));
            return;
        }
        else
        {
            field_B8_xpos += (field_CC_sprite_scale * FP_FromInteger(7));
            return;
        }
        break;

    case 10:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B8_xpos -= (field_CC_sprite_scale * FP_FromInteger(12));
            return;
        }
        else
        {
            field_B8_xpos += (field_CC_sprite_scale * FP_FromInteger(12));
            return;
        }
        break;

    case 11:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B8_xpos -= (field_CC_sprite_scale * FP_FromInteger(6));
            return;
        }
        else
        {
            field_B8_xpos += (field_CC_sprite_scale * FP_FromInteger(6));
            return;
        }
        break;

    default:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MusicController::sub_47FD60(4, this, 0, 0);
            MainMovement_4B4720();
        }
        break;
    }
}

void Slig::M_Knockback_34_4B68A0()
{
    if (field_100_pCollisionLine)
    {
        SlowDown_4B6450(FP_FromDouble(0.39));
    }
    else
    {
        M_Falling_7_4B42D0();

        if (field_106_current_motion == eSligMotions::M_LandingSoft_40_4B4530)
        {
            field_106_current_motion = eSligMotions::M_Knockback_34_4B68A0;
            field_12C = sGnFrame_5C1B84 + 10;
            if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(180)
                && !sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    FP_GetExponent(field_B8_xpos),
                    FP_GetExponent(field_BC_ypos),
                    TlvTypes::SoftLanding_75))
            {
                field_106_current_motion = eSligMotions::M_LandingFatal_41_4B4680;
                field_12C = sGnFrame_5C1B84 + 30;
                field_10C_health = FP_FromInteger(0);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
        }
    }

    Event_Broadcast_422BC0(kEventNoise, this);
    if (field_20_animation.field_92_current_frame == 4)
    {
        Abe_SFX_2_457A40(6, 80, -200, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_100_pCollisionLine)
        {
            field_C4_velx = FP_FromInteger(0);

            if (field_10C_health > FP_FromInteger(0) && 
                field_12C <= static_cast<int>(sGnFrame_5C1B84) &&
                sActiveHero_5C1B68->field_10C_health > FP_FromInteger(0))
            {
                field_106_current_motion = eSligMotions::M_KnockbackToStand_35_4B6A30;
            }
        }
    }
}

void Slig::M_KnockbackToStand_35_4B6A30()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Depossessing_36_4B7F30()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!Input_IsChanting_45F260())
            {
                field_106_current_motion = eSligMotions::M_DepossessingAbort_16_4B8250;
            }
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 4))
        {
            const FP xRand = FP_FromInteger(Math_RandomRange_496AB0(-20, 20));
            const FP yRand = FP_FromInteger(Math_RandomRange_496AB0(20, 50));
            New_Chant_Particle_426BE0(
                field_BC_ypos - (field_CC_sprite_scale * yRand),
                (field_CC_sprite_scale * xRand) + field_B8_xpos,
                field_CC_sprite_scale,
                0);
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_12C)
        {
            BlowUp_4B8020();
        }
    }
}

void Slig::M_Possess_37_4B72C0()
{
    if (!field_100_pCollisionLine)
    {
        M_Falling_7_4B42D0();
        if (field_106_current_motion != eSligMotions::M_Possess_37_4B72C0)
        {
            field_106_current_motion = eSligMotions::M_Possess_37_4B72C0;
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) >= field_150)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            FP xOff = (FP_FromInteger(20) * field_CC_sprite_scale);
            if (field_C4_velx < FP_FromInteger(0))
            {
                xOff = -xOff;
            }

            auto pGibs = alive_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_40FB40(
                    1,
                    field_B8_xpos,
                    field_BC_ypos,
                    xOff,
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    0);
            }
            New_Particles_426C70(
                field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                field_CC_sprite_scale,
                3,
                0x80u,
                0x80u,
                0x80u);

            if (field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                SFX_Play_46FA90(64u, 80);
                SFX_Play_46FA90(47u, 60);
            }
            else
            {
                SFX_Play_46FA90(64u, 127);
                SFX_Play_46FA90(47u, 90);
            }

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_10C_health = FP_FromInteger(0);
            MusicController::sub_47FD60(0, this, 0, 0);
            field_1C_update_delay = 40;
            field_120_timer = sGnFrame_5C1B84 + 60;
            SetBrain(&Slig::AI_Death_0_4BBFB0);
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) > field_14C)
    {
        if (field_C4_velx >= FP_FromInteger(0))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_106_current_motion = eSligMotions::M_Knockback_34_4B68A0;
            }
        }
        else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_motion = eSligMotions::M_Knockback_34_4B68A0;
        }
        else
        {
            field_106_current_motion = eSligMotions::M_Smash_44_4B6B90;
        }
    }
}

// When slig is "air walking" in a death fall
void Slig::M_OutToFall_38_4B4570()
{
    M_Falling_7_4B42D0();

    const FP fallDepth = field_BC_ypos - field_F8_LastLineYPos;
    if (fallDepth > FP_FromInteger(16 * 640))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_106_current_motion == eSligMotions::M_LandingSoft_40_4B4530 && fallDepth > FP_FromInteger(180) &&
        !sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos),
            TlvTypes::SoftLanding_75))
    {
        field_106_current_motion = eSligMotions::M_LandingFatal_41_4B4680;
        field_12C = sGnFrame_5C1B84 + 30;
        field_10C_health = FP_FromInteger(0);
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
    }
}

void Slig::M_FallingInitiate_39_4B4640()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eSligMotions::M_OutToFall_38_4B4570;
    }

    M_Falling_7_4B42D0();
}

void Slig::M_LandingSoft_40_4B4530()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Abe_SFX_2_457A40(6, 0, 0x7FFF, 0);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_LandingFatal_41_4B4680()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        SND_SEQ_Play_4CAB10(9u, 1, 65, 65);
        SFX_Play_46FA90(64u, 80);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<int>(sGnFrame_5C1B84) >= field_12C && !BrainIs(&Slig::AI_Death_0_4BBFB0))
        {
            field_10C_health = FP_FromInteger(0);
            SetBrain(&Slig::AI_Death_0_4BBFB0);
            field_120_timer = sGnFrame_5C1B84 + 60;
        }
    }
}

void Slig::M_ShootZ_42_4B7560()
{
    NOT_IMPLEMENTED();
}

void Slig::M_ShootZtoStand_43_4B77E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_Smash_44_4B6B90()
{
    if (field_100_pCollisionLine)
    {
        if (field_20_animation.field_92_current_frame == 4)
        {
            Abe_SFX_2_457A40(6, 80, -200, this);
        }
    }
    else
    {
        M_Falling_7_4B42D0();

        if (field_106_current_motion != eSligMotions::M_Smash_44_4B6B90)
        {
            field_106_current_motion = eSligMotions::M_Smash_44_4B6B90;
        }
    }
}

void Slig::M_PullLever_45_4B8950()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_114_flags.Get(Flags_114::e114_Bit10))
        {
            Sfx_Slig_4C04F0(11, 0, field_11E, this);
            field_106_current_motion = eSligMotions::M_Blurgh_31_4B5510;
        }
        else
        {
            ToStand_4B4A20();
        }
    }
}

void Slig::M_LiftGrip_46_4B3700()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pLiftPoint)
    {
        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        field_C8_vely = FP_FromInteger(0);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (sControlledCharacter_5C1B8C == this)
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
                {
                    if (pLiftPoint->vOnTopFloor_461890())
                    {
                        field_106_current_motion = eSligMotions::M_LiftGripping_48_4B3850;
                    }
                    else
                    {
                        field_106_current_motion = eSligMotions::M_LiftUp_49_4B3930;
                    }
                    return;
                }

                if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
                {
                    if (pLiftPoint->vOnBottomFloor_4618F0())
                    {
                        field_106_current_motion = eSligMotions::M_LiftGripping_48_4B3850;
                    }
                    else
                    {
                        field_106_current_motion = eSligMotions::M_LiftDown_50_4B3960;
                    }
                    return;
                }

                field_106_current_motion = eSligMotions::M_LiftGripping_48_4B3850;
            }
            else
            {
                if (pLiftPoint->vOnTopFloor_461890())
                {
                    field_106_current_motion = eSligMotions::M_LiftDown_50_4B3960;
                }

                if (pLiftPoint->vOnBottomFloor_4618F0())
                {
                    field_106_current_motion = eSligMotions::M_LiftUp_49_4B3930;
                }
            }
        }
    }
}

void Slig::M_LiftUngrip_47_4B3820()
{
    field_C8_vely = FP_FromInteger(0);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4B4A20();
    }
}

void Slig::M_LiftGripping_48_4B3850()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pLiftPoint)
    {
        CheckPlatformVanished_4B3640();

        pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
        field_C8_vely = FP_FromInteger(0);

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            if (!pLiftPoint->vOnTopFloor_461890())
            {
                field_106_current_motion = eSligMotions::M_LiftUp_49_4B3930;
            }
        }
        else if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            if (!pLiftPoint->vOnBottomFloor_4618F0())
            {
                field_106_current_motion = eSligMotions::M_LiftDown_50_4B3960;
            }
        }
        else if (pLiftPoint->vOnAnyFloor_461920())
        {
            field_106_current_motion = eSligMotions::M_LiftUngrip_47_4B3820;
        }
    }
}

void Slig::M_LiftUp_49_4B3930()
{
    field_106_current_motion = MoveLift_4B3990(FP_FromInteger(-4));
}

void Slig::M_LiftDown_50_4B3960()
{
    field_106_current_motion = MoveLift_4B3990(FP_FromInteger(4));
}

void Slig::M_Beat_51_4B6C00()
{
    NOT_IMPLEMENTED();
}

__int16 Slig::AI_Death_0_4BBFB0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render) && field_120_timer < (static_cast<int>(sGnFrame_5C1B84) + 30))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.01);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
        if (field_CC_sprite_scale >= FP_FromDouble(0.3) && !(static_cast<int>(sGnFrame_5C1B84) % 5))
        {
            const FP xOff = (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale);
            New_Particles_426C70(xOff + field_B8_xpos, (field_CC_sprite_scale / FP_FromInteger(2)), (field_CC_sprite_scale / FP_FromInteger(2)), 2, 128u, 128u, 128u);
            const FP sndPitch = (FP_FromInteger(2200) * field_CC_sprite_scale);
            SFX_Play_46FBA0(79u, 25, FP_GetExponent(sndPitch));
        }
    }
    else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_120_timer < static_cast<int>(sGnFrame_5C1B84))
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            MusicController::sub_47FD60(0, this, 0, 0);
            gMap_5C3030.SetActiveCam_480D30(field_146_level, field_148_path, field_14A_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }
    }

    if (sControlledCharacter_5C1B8C != this)
    {
        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 116;
}

__int16 Slig::AI_ReturnControlToAbeAndDie_1_4BC410()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        MusicController::sub_47FD60(0, this, 0, 0);
        gMap_5C3030.SetActiveCam_480D30(field_146_level, field_148_path, field_14A_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
    }

    field_6_flags.Set(BaseGameObject::eDead);
    return 117;
}

__int16 Slig::AI_Possessed_2_4BBCF0()
{
    switch (field_11C)
    {
    case 0:
        Sfx_Slig_4C04F0(10, 0, field_11E, this);
        field_11C = 1;
        field_10C_health = FP_FromInteger(0);
        field_106_current_motion = eSligMotions::M_Possess_37_4B72C0;
        field_120_timer = sGnFrame_5C1B84 + 30;
        field_150 = sGnFrame_5C1B84 + 1000;
        if (field_14C == 0)
        {
            field_14C = sGnFrame_5C1B84 + 1000;
        }
        return field_11C;

    case 1:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_120_timer)
        {
            field_11C = 2;
            field_120_timer = sGnFrame_5C1B84 + 20;
            field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
            return field_11C;
        }
        break;

    case 2:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_120_timer)
        {
            if (Math_NextRandom() & 1)
            {
                field_120_timer = sGnFrame_5C1B84 + 20;
                field_106_current_motion = eSligMotions::M_SpeakHi_21_4B53D0;
                Sfx_Slig_4C04F0(0, 0, field_11E, this);
            }
            else
            {
                field_120_timer = sGnFrame_5C1B84 + 20;
                field_106_current_motion = eSligMotions::M_SpeakLaugh_24_4B5430;
                Sfx_Slig_4C04F0(3, 0, field_11E, this);
            }
            field_11C = 3;
            return field_11C;
        }
        break;

    case 3:
        if (Event_Get_422C00(kEventDeathReset))
        {
            if (sControlledCharacter_5C1B8C != this)
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) >= field_120_timer)
        {
            field_11C = 4;
            field_10C_health = FP_FromInteger(1);
            return field_11C;
        }
        break;

    case 4:
        if (Event_Get_422C00(kEventDeathReset))
        {
            if (sControlledCharacter_5C1B8C != this)
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }
        if (field_10C_health <= FP_FromInteger(0))
        {
            SetBrain(&Slig::AI_Death_0_4BBFB0);
            field_120_timer = sGnFrame_5C1B84 + 60;
            return field_11C;
        }
        break;

    default:
        return field_11C;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_120_timer)
    {
        if (field_114_flags.Get(Flags_114::e114_Bit1_bShot))
        {
            BlowUp_4B8020();
        }
    }

    return field_11C;
}

__int16 Slig::AI_DeathDropDeath_3_4BC1E0()
{
    if (field_11C == 0)
    {
        Sfx_Slig_4C04F0(10, 0, field_11E, this);
        field_120_timer = sGnFrame_5C1B84 + 60;
        return 1;
    }

    // TODO: Detangle compiler mashed up switch/case
    const auto v3 = field_11C - 1;
    if (v3)
    {
        if (v3 != 1 || static_cast<int>(sGnFrame_5C1B84) <= field_120_timer)
        {
            return field_11C;
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            MusicController::sub_47FD60(0, this, 0, 0);
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            gMap_5C3030.SetActiveCam_480D30(field_146_level, field_148_path, field_14A_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }

        field_6_flags.Set(BaseGameObject::eDead);
        return field_11C;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_120_timer)
    {
        if (!((field_120_timer - sGnFrame_5C1B84) % 15))
        {
            Sfx_Slig_4C04F0(
                10,
                static_cast<short>(2 * ((field_120_timer & 0xFFFF) - sGnFrame_5C1B84)),
                field_11E,
                this);
        }

        if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 6)
        {
            SND_SEQ_Play_4CAB10(9u, 1, 65, 65);
        }

        return field_11C;
    }

    Abe_SFX_2_457A40(15, 0, 0x7FFF, this);

    auto pScreenShake = alive_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4ACF70(0, 0);
    }

    field_120_timer = sGnFrame_5C1B84 + 30;
    return 2;
}

__int16 Slig::AI_ListeningToGlukkon_4_4B9D20()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Slig::AI_Empty_5_4B3220()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Empty_6_4B3420()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_SpottedEnemy_7_4B3240()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Empty_8_4B3120()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Empty_9_4B3440()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_AbeDead_10_4B3460()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_KilledAbe_11_4B35C0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_PanicTurning_12_4BC490()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_PanicRunning_13_4BC780()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_PanicYelling_14_4BCA70()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Idle_15_4BD800()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_StopChasing_16_4BCE30()
{
    if (field_120_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        if (HandleEnemyStopper_4BBA00(4))
        {
            field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
            SetBrain(&Slig::AI_Idle_15_4BD800);
            field_120_timer = sGnFrame_5C1B84 + 1;
        }
    }
    else
    {
        field_15C = 0;
        ToTurn_4BE090();
    }
    return 119;

}

__int16 Slig::AI_Chasing2_17_4BCBD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Chasing1_18_4BCEB0()
{
    if (field_120_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0) && 
            !sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8) && 
            sControlledCharacter_5C1B8C->field_4_typeId != Types::eGlukkon_67)
        {
            field_15C = 0;
            ToShoot_4BF9A0();
        }
    }
    else
    {
        if (field_C0_path_number != gMap_5C3030.sCurrentPathId_5C3032 || field_C2_lvl_number != gMap_5C3030.sCurrentLevelId_5C3030)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }

        field_15C = 1;
        field_108_next_motion = eSligMotions::M_Running_4_4B6000;
        SetBrain(&Slig::AI_Chasing2_17_4BCBD0);
        field_120_timer = field_218_tlv_data.field_14_pause_time;
    }
    return 122;
}

__int16 Slig::AI_Turning_19_4BDDD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_StoppingNextToMudokon_20_4BF1E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Walking_21_4BE0C0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_GetAlertedTurn_22_4BE990()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_GetAlerted_23_4BEC40()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_BeatingUp_24_4BF2B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_DiscussionWhat_25_4BF380()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Empty_26_4BF620()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Empty_27_4BF600()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_ShootingFromBackground_28_4BFA70()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Shooting_29_4BF750()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_SpottedEnemyFromBackground_30_4BFA30()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_WakingUp_31_4B9390()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        WaitOrWalk_4BE870();
    }
    return 0;
}

__int16 Slig::AI_Inactive_32_4B9430()
{
    if (ListenToGlukkonCommands_4B95D0())
    {
        ToUnderGlukkonCommand_4B9660();
    }
    else if (field_120_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        if (sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0))
        {
            ToAbeDead_4B3580();
        }
        else if (vOnSameYLevel_425520(sControlledCharacter_5C1B8C) &&
                 vIsFacingMe_4254A0(sControlledCharacter_5C1B8C) &&
                 vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(1), sControlledCharacter_5C1B8C) &&
                 !IsInInvisibleZone_425710(sControlledCharacter_5C1B8C) && 
                 !sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8) &&
                 !IsWallBetween_4BB8B0(this, sControlledCharacter_5C1B8C) &&
                 !Event_Get_422C00(kEventResetting) && 
                 sControlledCharacter_5C1B8C->field_4_typeId != Types::eGlukkon_67)
        {
            ToShoot_4BF9A0();
        }
        else
        {
            ShouldStilBeAlive_4BBC00();
        }
    }
    else
    {
        WaitOrWalk_4BE870();
    }
    return 103;
}

__int16 Slig::AI_Paused_33_4B8DD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Sleeping_34_4B9170()
{
    auto pEvent = Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
    if (pEvent)
    {
        auto pNoise = Event_Is_Event_In_Range_422C30(kEventNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
        if (pNoise)
        {
            pEvent = pNoise;
        }

        const FP wakeUpDistance = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(field_218_tlv_data.field_4A_noise_wake_up_distance);
        if (vIsObjNearby_4253B0(wakeUpDistance, pEvent) &&
            field_120_timer <= static_cast<int>(sGnFrame_5C1B84) &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) &&
            !Event_Get_422C00(kEventResetting))
        {
            WakeUp_4B93B0();
            return 102;
        }
    }

    if (Event_Get_422C00(kEventSpeaking) || Event_Get_422C00(kEventLoudNoise))
    {
        if (pEvent != this  &&
            field_120_timer <= static_cast<int>(sGnFrame_5C1B84) &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) &&
            !Event_Get_422C00(kEventResetting))
        {
            WakeUp_4B93B0();
            return 102;
        }
    }

    if (Event_Get_422C00(kEventAlarm) && !Event_Get_422C00(kEventResetting))
    {
        WakeUp_4B93B0();
        return 102;
    }

    ShouldStilBeAlive_4BBC00();

    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        const CameraPos direction = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);
        Start_Slig_sounds_4CB980(direction, 0);
    }
    return 102;
}

__int16 Slig::AI_GameEnder_35_4BF640()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Slig::Init_4BB0D0()
{
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 319, 1, 0));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 423, 1, 0));
    field_10_resources_array.SetAt(12, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 360, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation,  414, 1, 0));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 354, 1, 0));
    
    if (!(field_218_tlv_data.field_48_disable_resources & 0x80))
    {
        field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 418, 1, 0));
    }

    if (!(field_218_tlv_data.field_48_disable_resources & 0x100))
    {
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 415, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 1))
    {
        field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 419, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 2))
    {
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 420, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 0x200))
    {
        field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 416, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 0x400))
    {
        field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 426, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 4))
    {
        field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 417, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 0x40))
    {
        field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 413, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 8))
    {
        field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 344, 1, 0));
    }
    
    if (!(field_218_tlv_data.field_48_disable_resources & 0x10))
    {
        field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 28, 1, 0));
    }

    field_15E = 0;
    field_120_timer = field_218_tlv_data.field_14_pause_time + sGnFrame_5C1B84;

    switch (field_218_tlv_data.field_12_start_state)
    {
    case Path_Slig::StartState::Paused_1:
        SetBrain(&Slig::AI_Inactive_32_4B9430);
        break;

    case Path_Slig::StartState::Sleeping_2:
        if (field_218_tlv_data.field_1_unknown && field_218_tlv_data.field_46_stay_awake)
        {
            SetBrain(&Slig::AI_Inactive_32_4B9430);
        }
        else
        {
            SetBrain(&Slig::AI_Sleeping_34_4B9170);
            field_106_current_motion = eSligMotions::M_Sleeping_32_4B89A0;
            vUpdateAnim_4B1320();
        }
        break;

    case Path_Slig::StartState::Chase_3:
        SetBrain(&Slig::AI_Chasing1_18_4BCEB0);
        field_120_timer = sGnFrame_5C1B84 + field_218_tlv_data.field_36_time_to_wait_before_chase;
        break;

    case Path_Slig::StartState::RunOffScreen_4:
        SetBrain(&Slig::AI_GameEnder_35_4BF640);
        field_120_timer = sGnFrame_5C1B84 + field_218_tlv_data.field_14_pause_time;
        break;

    case Path_Slig::StartState::GameEnder_5:
        break;

    case Path_Slig::StartState::Unknown_6:
        for (int i=0; i<gBaseGameObject_list_BB47C4->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_4_typeId == Types::eGlukkon_67)
            {
                auto pGlukkon = static_cast<BaseAliveGameObject*>(pObj);
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    pGlukkon->field_C2_lvl_number,
                    pGlukkon->field_C0_path_number,
                    pGlukkon->field_B8_xpos,
                    pGlukkon->field_BC_ypos,
                    0))
                {
                    field_208 = pGlukkon->field_8_object_id;
                    word_BAF7E8++;
                    field_216 |= 1u;
                    SetBrain(&Slig::AI_ListeningToGlukkon_4_4B9D20);
                    field_11C = 1;
                    break;
                }
            }
        }

        if (!field_208)
        {
            SetBrain(&Slig::AI_Inactive_32_4B9430);
        }
        break;

    default:
        SetBrain(&Slig::AI_Paused_33_4B8DD0);
        break;
    }

    if (!field_218_tlv_data.field_2C_start_direction)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
   
    field_290_points_count = 0;

    field_268_points[field_290_points_count].field_0_x = FP_GetExponent(field_B8_xpos);
    field_268_points[field_290_points_count].field_2_y = FP_GetExponent(field_BC_ypos);
    field_290_points_count++;

    for (short yCam = -3; yCam < 4; yCam++)
    {
        for (short xCam = -3; xCam < 4; xCam++)
        {
            Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(xCam, yCam);
            while (pTlvIter)
            {
                bool addPoint = false;
                if (pTlvIter->field_4_type == TlvTypes::SligBoundLeft_32)
                {
                    if (static_cast<Path_Slig_LeftBound*>(pTlvIter)->field_10_slig_id == field_218_tlv_data.field_38_slig_id)
                    {
                        field_138_zone_rect.x = pTlvIter->field_8_top_left.field_0_x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::SligBoundRight_45)
                {
                    if (static_cast<Path_Slig_RightBound*>(pTlvIter)->field_10_slig_id == field_218_tlv_data.field_38_slig_id)
                    {
                        field_138_zone_rect.w = pTlvIter->field_8_top_left.field_0_x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::SligPersist_46)
                {
                    if (static_cast<Path_Slig_Persist*>(pTlvIter)->field_10_slig_id == field_218_tlv_data.field_38_slig_id)
                    {
                        addPoint = true;
                    }
                }

                if (addPoint)
                {
                    if (field_290_points_count < ALIVE_COUNTOF(field_268_points))
                    {
                        field_268_points[field_290_points_count].field_0_x = pTlvIter->field_8_top_left.field_0_x;
                        field_268_points[field_290_points_count].field_2_y = pTlvIter->field_8_top_left.field_2_y;
                        field_290_points_count++;
                    }
                }

                pTlvIter = Path::Next_TLV_4DB6A0(pTlvIter);
            }
        }
    }
}

void Slig::dtor_4B1CF0()
{
    SetVTable(this, 0x547460);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        
        MusicController::sub_47FD60(0, this, 0, 0);

        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_146_level,
                field_148_path,
                field_14A_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_118_tlvInfo);
    if (pTlv)
    {
        if (pTlv->field_4_type != SligGetPants_104 && pTlv->field_4_type != SligSpawner_37)
        {
            if (field_10C_health <= FP_FromInteger(0))
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
                pTlv->field_1_unknown = 0;
            }
            else
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);
                pTlv->field_1_unknown = 0;
            }
        }
    }

    dtor_4080B0();
}

Slig* Slig::vdtor_4B1790(signed int flags)
{
    dtor_4B1CF0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

const FP dword_5473E8[8] =
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

// TODO: Size 8 ??
const FP dword_547408[22] =
{
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)
};

void Slig::vUpdate_4B17C0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
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

            if (field_100_pCollisionLine)
            {
                if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
                {
                    PSX_RECT bRect = {};
                    vGetBoundingRect_424FD0(&bRect, 1);
                    vOnCollisionWith_424EE0(
                        { bRect.x, static_cast<short>(bRect.y + 5) },
                        { bRect.w, static_cast<short>(bRect.h + 5) },
                        ObjList_5C1B78,
                        1,
                        (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
                }
            }
        }
        field_208 = BaseGameObject::Find_Flags_4DC170(field_208);
        if (BrainIs(&Slig::AI_ListeningToGlukkon_4_4B9D20))
        {
            word_BAF7E8++;
        }
        field_114_flags.Clear(Flags_114::e114_Bit9);
        field_104_collision_line_type = 0;
    }

    if (!Input_IsChanting_45F260())
    {
        field_292 &= ~1u;
    }

    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        MusicController::sub_47FD60(9, this, 0, 0);
    }

    if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
    {
        field_100_pCollisionLine = nullptr;

        // TODO: InputCommand constants
        if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0xF)
        {
            field_C4_velx = dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            field_C8_vely = dword_547408[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0x10)
            {
                field_C4_velx += dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                field_C4_velx += dword_5473E8[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                field_C8_vely += dword_547408[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            }

            field_B8_xpos += field_C4_velx;
            field_BC_ypos += field_C8_vely;

            // Keep in the map bounds
            PSX_Point mapBounds = {};
            gMap_5C3030.Get_map_size_480640(&mapBounds);

            if (field_B8_xpos < FP_FromInteger(0))
            {
                field_B8_xpos = FP_FromInteger(0);
            }

            if (field_B8_xpos >= FP_FromInteger(mapBounds.field_0_x))
            {
                field_B8_xpos = FP_FromInteger(mapBounds.field_0_x) - FP_FromInteger(1);
            }

            if (field_BC_ypos < FP_FromInteger(0))
            {
                field_BC_ypos = FP_FromInteger(0);
            }

            if (field_BC_ypos >= FP_FromInteger(mapBounds.field_2_y))
            {
                field_BC_ypos = FP_FromInteger(mapBounds.field_2_y) - FP_FromInteger(1);
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
        }

        sub_408C40();

        field_F8_LastLineYPos = field_BC_ypos;
    }
    else
    {
        const auto oldMotion = field_106_current_motion;
        const auto oldBrain = field_154_brain_state;
        field_11C = (this->*field_154_brain_state)();

        if (field_114_flags.Get(Flags_114::e114_Bit1_bShot))
        {
            vsub_4B2EA0();
        }

        if (sDDCheat_ShowAI_Info_5C1BD8)
        {
            DDCheat::DebugStr_4F5560("Slig %d %d %d %d\n", field_11C, field_120_timer, field_106_current_motion, field_108_next_motion);
        }

        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;

        (this->*sSlig_motion_table_5604A0[field_106_current_motion])();

        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        if (oldBrain != field_154_brain_state)
        {
            LOG_INFO("Slig: Old brain = " << GetOriginalFn(oldBrain).fnName << " new brain = " << GetOriginalFn(field_154_brain_state).fnName);
        }

        if (oldMotion != field_106_current_motion || field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2))
        {
            field_114_flags.Clear(Flags_114::e114_MotionChanged_Bit2);
            vUpdateAnim_4B1320();
        }
        else if (field_124)
        {
            field_106_current_motion = field_F4;
            vUpdateAnim_4B1320();
            field_20_animation.SetFrame_409D50(field_F6_anim_frame);
            field_124 = 0;
        }
    }
}

void Slig::vScreenChanged_4B1E20()
{
    if ((gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || 
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710() || 
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId) && this != sControlledCharacter_5C1B8C)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Slig::vPossessed_4B2F10()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_292 |= 1u;
    if (field_108_next_motion != eSligMotions::M_KnockbackToStand_35_4B6A30 && field_108_next_motion != eSligMotions::M_Knockback_34_4B68A0)
    {
        field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    }
    SetBrain(&Slig::AI_Possessed_2_4BBCF0);
    field_11C = 0;

    field_146_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_148_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_14A_camera = gMap_5C3030.sCurrentCamId_5C3034;

    MusicController::sub_47FD60(9, this, 1, 0);
}

void Slig::vUnPosses_4B3050()
{
    field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_120_timer = sGnFrame_5C1B84 + 180;
    MusicController::sub_47FD60(0, this, 0, 0);
}

void Slig::vOnTlvCollision_4B2FB0(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                field_11C = 0;
                SetBrain(&Slig::AI_DeathDropDeath_3_4BC1E0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                VSetMotion_4081C0(eSligMotions::M_Falling_7_4B42D0);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

void Slig::vsub_4B2EA0()
{
    NOT_IMPLEMENTED();
}

void Slig::vUpdateAnim_4B1320()
{
    BYTE** ppRes = ResForMotion_4B1E90(field_106_current_motion);
    if (!ppRes)
    {
        field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
        ppRes = ResForMotion_4B1E90(field_106_current_motion);
    }
    field_20_animation.Set_Animation_Data_409C80(sSligFrameTables_547318[field_106_current_motion], ppRes);
}

BOOL Slig::vUnderGlukkonCommand_4B1760()
{
    return BrainIs(&Slig::AI_ListeningToGlukkon_4_4B9D20);
}

void Slig::WakeUp_4B93B0()
{
    field_108_next_motion = eSligMotions::M_SleepingToStand_33_4B8C50;
    SetBrain(&Slig::AI_WakingUp_31_4B9390);

    MusicController::sub_47FD60(4, this, 0, 0);
    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        field_218_tlv_data.field_8_top_left.field_0_x,
        field_218_tlv_data.field_8_top_left.field_2_y,
        field_218_tlv_data.field_8_top_left.field_0_x,
        field_218_tlv_data.field_8_top_left.field_2_y,
        TlvTypes::Slig_15);
    if (pTlv)
    {
        pTlv->field_1_unknown = 1; // TODO: Keep track of these, 1 must keep slig awake ??
    }
}

void Slig::ShouldStilBeAlive_4BBC00()
{
    if (!field_15C)
    {
        // Check not falling and not in the current screen
        if (field_106_current_motion != eSligMotions::M_Falling_7_4B42D0
            && field_106_current_motion != eSligMotions::M_OutToFall_38_4B4570
            && !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0))
        {
            if (field_290_points_count <= 0)
            {
                // No patrol points, die
                field_6_flags.Set(BaseGameObject::eDead);
            }
            else
            {
                int i = 0;
                while (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    FP_FromInteger(field_268_points[i].field_0_x),
                    FP_FromInteger(field_268_points[i].field_2_y),
                    0))
                {
                    if (i >= field_290_points_count)
                    {
                        // No within any out our patrol points, die
                        field_6_flags.Set(BaseGameObject::eDead);
                        break;
                    }
                    i++;
                }
            }
        }
    }
}

BYTE** Slig::ResForMotion_4B1E90(__int16 motion)
{
    __int16 resIdx = 0;
    if (motion < eSligMotions::M_Sleeping_32_4B89A0)
    {
        resIdx = 0;
    }
    else if (motion < eSligMotions::M_Knockback_34_4B68A0)
    {
        resIdx = 1;
    }
    else if (motion < eSligMotions::M_OutToFall_38_4B4570)
    {
        resIdx = 2;
    }
    else if (motion < eSligMotions::M_LandingFatal_41_4B4680)
    {
        resIdx = 3;
    }
    else if (motion < eSligMotions::M_ShootZ_42_4B7560)
    {
        resIdx = 4;
    }
    else if (motion < eSligMotions::M_Smash_44_4B6B90)
    {
        resIdx = 5;
    }
    else if (motion < eSligMotions::M_PullLever_45_4B8950)
    {
        resIdx = 6;
    }
    else if (motion < eSligMotions::M_LiftGrip_46_4B3700)
    {
        resIdx = 7;
    }
    else if (motion < eSligMotions::M_Beat_51_4B6C00)
    {
        resIdx = eSligMotions::M_SlidingToStand_8_4B6520;
    }
    else
    {
        resIdx = motion >= 52 ? 0 : 9;
    }

    field_134_res_idx = resIdx;

    return field_10_resources_array.ItemAt(field_134_res_idx);
}

void Slig::ToTurn_4BE090()
{
    field_108_next_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    SetBrain(&Slig::AI_Turning_19_4BDDD0);
}

void Slig::RespondWithWhat_4BF400()
{
    field_120_timer = sGnFrame_5C1B84 + 20;
    if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
    {
        field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    }
    else
    {
        field_108_next_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    }
    SetBrain(&Slig::AI_DiscussionWhat_25_4BF380);
}

void Slig::ToShoot_4BF9A0()
{
    field_108_next_motion = eSligMotions::M_Shoot_6_4B55A0;
    SetBrain(&Slig::AI_Shooting_29_4BF750);
    field_158 = 0;
    field_15A = 0;
    MusicController::sub_47FD60(8, this, 0, 0);
}

void Slig::ToZShoot_4BF9E0()
{
    field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_120_timer = sGnFrame_5C1B84 + field_218_tlv_data.field_44_Z_shoot_delay;
    SetBrain(&Slig::AI_SpottedEnemyFromBackground_30_4BFA30);
    MusicController::sub_47FD60(8, this, 0, 0);
}

void Slig::PauseALittle_4BDD00()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_120_timer = field_218_tlv_data.field_16_pause_left_min + sGnFrame_5C1B84;
        if (field_218_tlv_data.field_18_pause_left_max > field_218_tlv_data.field_16_pause_left_min)
        {
            field_120_timer += Math_NextRandom() % (field_218_tlv_data.field_18_pause_left_max - field_218_tlv_data.field_16_pause_left_min);
        }
    }
    else
    {
        field_120_timer = field_218_tlv_data.field_1A_pause_right_min + sGnFrame_5C1B84;
        if (field_218_tlv_data.field_1C_pause_right_max > field_218_tlv_data.field_1A_pause_right_min)
        {
            field_120_timer += Math_NextRandom() % (field_218_tlv_data.field_1C_pause_right_max - field_218_tlv_data.field_1A_pause_right_min);
        }
    }
    field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    SetBrain(&Slig::AI_Idle_15_4BD800);
}

void Slig::ToStand_4B4A20()
{
    field_130 = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_128_input = 0;
    field_12C = Math_RandomRange_496AB0(0, 60) + sGnFrame_5C1B84 + 120;
    MapFollowMe_408D10(TRUE);
}

void Slig::BlowUp_4B8020()
{
    auto pGibs = alive_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(
            1,
            field_B8_xpos,
            field_BC_ypos,
            field_C4_velx,
            field_C8_vely,
            field_CC_sprite_scale,
            0);
    }

    auto pBlood = alive_new<Blood>();
    if (pBlood)
    {
        pBlood->ctor_40F0B0(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_CC_sprite_scale,
            20);
    }

    New_Particles_426C70(
        field_B8_xpos,
        field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
        field_CC_sprite_scale,
        3,
        128,
        128,
        128);

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_46FA90(64u, 80);
        SFX_Play_46FA90(47u, 60);
    }
    else
    {
        SFX_Play_46FA90(64u, 127);
        SFX_Play_46FA90(47u, 90);
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_C8_vely = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_10C_health = FP_FromInteger(0);
    MusicController::sub_47FD60(0, this, 0, 0);
    field_120_timer = sGnFrame_5C1B84 + 60;
    field_1C_update_delay = 40;
    SetBrain(&Slig::AI_Death_0_4BBFB0);
    Event_Broadcast_422BC0(kEventMudokonComfort, this);
}

__int16 Slig::MainMovement_4B4720()
{
    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        return HandlePlayerControlled_4B7800();
    }

    if (field_108_next_motion == eSligMotions::M_TurnAroundStanding_5_4B6390)
    {
        field_106_current_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        field_108_next_motion = -1;
        return 1;
    }

    if (field_108_next_motion != eSligMotions::M_Walking_2_4B5BC0)
    {
        if (field_108_next_motion == eSligMotions::M_Running_4_4B6000)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            field_106_current_motion = eSligMotions::M_StandToRun_3_4B62F0;
            field_108_next_motion = -1;
            return 1;
        }

        if (field_108_next_motion == eSligMotions::M_Shoot_6_4B55A0 || field_108_next_motion == eSligMotions::M_ShootZ_42_4B7560)
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
            field_12C = sGnFrame_5C1B84 + 60;
            field_130 = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
        }
        else
        {
            if (field_108_next_motion < eSligMotions::M_SpeakHereBoy_20_4B5330 || field_108_next_motion > eSligMotions::M_Blurgh_31_4B5510)
            {
                if (field_108_next_motion == eSligMotions::M_ReloadGun_12_4B5530 ||
                    field_108_next_motion == eSligMotions::M_PullLever_45_4B8950 ||
                    field_108_next_motion == eSligMotions::M_SteppingToStand_14_4B8480 ||
                    field_108_next_motion == eSligMotions::M_LiftGrip_46_4B3700 ||
                    field_108_next_motion == eSligMotions::M_Beat_51_4B6C00)
                {
                    field_106_current_motion = field_108_next_motion;
                    field_108_next_motion = -1;
                    return 1;
                }
                else
                {
                    ToStand_4B4A20();
                    return 0;
                }
            }

            field_106_current_motion = GetNextMotionIncGameSpeak_4B5080(0);
            if (field_106_current_motion == -1)
            {
                ToStand_4B4A20();
                return 0;
            }

            if (!BrainIs(&Slig::AI_ListeningToGlukkon_4_4B9D20))
            {
                Event_Broadcast_422BC0(kEventSpeaking, this);
                return 1;
            }
        }
        return 1;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), -ScaleToGridSize_4498B0(field_CC_sprite_scale)))
        {
            return 0;
        }
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), ScaleToGridSize_4498B0(field_CC_sprite_scale)))
        {
            return 0;
        }
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }

    field_106_current_motion = eSligMotions::M_StandToWalk_1_4B5F70;
    field_108_next_motion = -1;

    return 1;
}

__int16 Slig::HandlePlayerControlled_4B7800()
{
    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
        {
            // Run right
            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                field_106_current_motion = eSligMotions::M_StandToRun_3_4B62F0;
                return 1;
            }

            if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)))
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                field_106_current_motion = eSligMotions::M_StandingToStep_15_4B83B0;
                return 1;
            }

            if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), ScaleToGridSize_4498B0(field_CC_sprite_scale)))
            {
                return 0;
            }

            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
            field_106_current_motion = eSligMotions::M_SteppingToStand_14_4B8480;
            return 1;
        }
        else
        {
            field_106_current_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
            return 1;
        }
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            // Run left
            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                field_106_current_motion = eSligMotions::M_StandToRun_3_4B62F0;
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                return 1;
            }

            if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), -ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
                field_106_current_motion = eSligMotions::M_StandingToStep_15_4B83B0;
                return 1;
            }

            if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(45), -ScaleToGridSize_4498B0(field_CC_sprite_scale)))
            {
                return 0;
            }
            field_106_current_motion = eSligMotions::M_SteppingToStand_14_4B8480;
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
            return 1;
        }
        else
        {
            field_106_current_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
            return 1;
        }
    }

    if (sInputObject_5BD4E0.isHeld(sInputKey_DoAction_5550E4))
    {
        FP switchXPos = {};
        FP switchYPos = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            switchXPos = field_BC_ypos - FP_FromInteger(5);
            switchYPos = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            switchXPos = field_BC_ypos - FP_FromInteger(5);
            switchYPos = ScaleToGridSize_4498B0(field_CC_sprite_scale)  + field_B8_xpos;
        }

        auto pSwitch = static_cast<Switch*>(FindObjectOfType_425180(Types::eLever_139, switchYPos, switchXPos));
        if (pSwitch && !field_114_flags.Get(Flags_114::e114_Bit10))
        {
            pSwitch->Vsub_4D6050(field_B8_xpos < pSwitch->field_B8_xpos);
            field_106_current_motion = eSligMotions::M_PullLever_45_4B8950;
            return 1;
        }

        Sfx_Slig_4C04F0(9, 0, field_11E, this);
        field_106_current_motion = eSligMotions::M_SpeakWhat_29_4B54D0;
        return 1;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_FartRoll_5550F0))
        {
            field_106_current_motion = eSligMotions::M_Beat_51_4B6C00;
            field_12C = sGnFrame_5C1B84 + 60;
            return 1;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            auto pLiftPoint2 = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
            if (pLiftPoint2)
            {
                if (pLiftPoint2->field_4_typeId == Types::eLiftPoint_78)
                {
                    const FP halfGrid = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                    const FP midSwitchX = FP_FromInteger((field_100_pCollisionLine->field_0_rect.x + field_100_pCollisionLine->field_0_rect.w) / 2);
                    const FP distToSwitchMid = (field_B8_xpos - midSwitchX) >= FP_FromInteger(0) ? field_B8_xpos - midSwitchX : midSwitchX - field_B8_xpos;
                    if (distToSwitchMid < halfGrid)
                    {
                        field_106_current_motion = eSligMotions::M_LiftGrip_46_4B3700;
                        return 1;
                    }
                }
            }

            if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4) && 
                field_CC_sprite_scale == FP_FromDouble(0.5) && 
                !field_114_flags.Get(Flags_114::e114_Bit10))
            {
                field_106_current_motion = eSligMotions::M_ShootZ_42_4B7560;
                field_108_next_motion = -1;
                field_12C = sGnFrame_5C1B84 + 60;
                return 1;
            }
        }
        else if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
            if (pLiftPoint)
            {
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_78)
                {
                    const FP halfGrid = ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2);
                    const FP midSwitchX = FP_FromInteger((field_100_pCollisionLine->field_0_rect.x + field_100_pCollisionLine->field_0_rect.w) / 2);
                    const FP distToSwitchMid = (field_B8_xpos - midSwitchX) >= FP_FromInteger(0) ? field_B8_xpos - midSwitchX : midSwitchX - field_B8_xpos;
                    if (distToSwitchMid < halfGrid)
                    {
                        field_106_current_motion = eSligMotions::M_LiftGrip_46_4B3700;
                        return 1;
                    }
                }
            }

            if (sInputObject_5BD4E0.isHeld(sInputKey_Up_5550D8))
            {
                Sfx_Slig_4C04F0(9, 0, field_11E, this);
                field_106_current_motion = eSligMotions::M_SpeakWhat_29_4B54D0;
                return 1;
            }
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_FartRoll_5550F0 | sInputKey_Hop_5550E0))
        {
            Sfx_Slig_4C04F0(11, 0, field_11E, this);
            field_106_current_motion = eSligMotions::M_Blurgh_31_4B5510;
            return 1;
        }
        ToStand_4B4A20();
        return 0;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC)|| 
        field_CC_sprite_scale != FP_FromDouble(0.5) || 
        field_114_flags.Get(Flags_114::e114_Bit10))
    {
        field_106_current_motion = eSligMotions::M_Shoot_6_4B55A0;
    }
    else
    {
        field_106_current_motion = eSligMotions::M_ShootZ_42_4B7560;
    }

    field_108_next_motion = -1;
    field_12C = sGnFrame_5C1B84 + 60;
    return 1;
}

__int16 Slig::GetNextMotionIncGameSpeak_4B5080(int /*input*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void Slig::WaitOrWalk_4BE870()
{
    field_108_next_motion = eSligMotions::M_Walking_2_4B5BC0;
    SetBrain(&Slig::AI_Walking_21_4BE0C0);

    // Right rect bound
    if (!IsFacingEffectiveLeft_4BB780(this) && (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2)) + field_B8_xpos >= FP_FromInteger(field_138_zone_rect.w))
    {
        PauseALittle_4BDD00();
        MusicController::sub_47FD60(4, this, 0, 0);
        return;
    }

    // Left rect bound
    if (IsFacingEffectiveLeft_4BB780(this) && (field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))) <= FP_FromInteger(field_138_zone_rect.x))
    {
        PauseALittle_4BDD00();
        MusicController::sub_47FD60(4, this, 0, 0);
        return;
    }

    AI_Walking_21_4BE0C0();
    MusicController::sub_47FD60(4, this, 0, 0);
}

void Slig::ToAbeDead_4B3580()
{
    field_108_next_motion = eSligMotions::M_SpeakLaugh_24_4B5430;
    SetBrain(&Slig::AI_AbeDead_10_4B3460);
    field_120_timer = sGnFrame_5C1B84 + 45;
}

void Slig::ToUnderGlukkonCommand_4B9660()
{
    SetBrain(&Slig::AI_ListeningToGlukkon_4_4B9D20);
    field_11C = 0;
}

void Slig::ToKilledAbe_4B3600()
{
    field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    SetBrain(&Slig::AI_KilledAbe_11_4B35C0);
    field_120_timer = sGnFrame_5C1B84 + 15;
}

BOOL Slig::IsWallBetween_4BB8B0(BaseAliveGameObject* /*pLeft*/, BaseAliveGameObject* /*pRight*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::ListenToGlukkonCommands_4B95D0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Slig::PlatformCollide_4B4E00()
{
    PSX_Point xy;
    xy.field_0_x = FP_GetExponent(field_B8_xpos - FP_FromInteger(5));
    xy.field_2_y = FP_GetExponent(field_BC_ypos - FP_FromInteger(5));

    PSX_Point wh;
    wh.field_0_x = FP_GetExponent(field_B8_xpos + FP_FromInteger(5));
    wh.field_2_y = FP_GetExponent(field_BC_ypos + FP_FromInteger(5));

    vOnCollisionWith_424EE0(
        xy,
        wh,
        ObjList_5C1B78,
        1,
        (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
}

void Slig::FallKnockBackOrSmash_4B4A90()
{
    Abe_SFX_2_457A40(13, 0, 0x7FFF, this);

    if (field_C4_velx > FP_FromInteger(0))
    {
        field_B8_xpos -= field_C4_velx;
    }

    if (field_100_pCollisionLine)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!Raycast_408750((field_CC_sprite_scale * FP_FromInteger(45)), (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))))
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
        else
        {
            if (!Raycast_408750((field_CC_sprite_scale * FP_FromInteger(45)), (-ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2))))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }

    if (field_108_next_motion == eSligMotions::M_Smash_44_4B6B90)
    {
        field_C4_velx = -field_C4_velx;
    }

    MapFollowMe_408D10(TRUE);

    if (field_C8_vely < FP_FromInteger(0))
    {
        field_C8_vely = FP_FromInteger(0);
    }

    field_106_current_motion = eSligMotions::M_Knockback_34_4B68A0;
    field_12C = sGnFrame_5C1B84 + 10;
}

void Slig::TurnOrSayWhat_4BEBC0()
{
    if (Event_Get_422C00(kEventSpeaking) && 
        !IsInInvisibleZone_425710(sControlledCharacter_5C1B8C) && 
        !sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8))
    {
        GameSpeakResponse_4BF470();
        RespondWithWhat_4BF400();
    }
    else
    {
        field_108_next_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
        SetBrain(&Slig::AI_GetAlertedTurn_22_4BE990);
    }
}

void Slig::GameSpeakResponse_4BF470()
{
    NOT_IMPLEMENTED();
}

void Slig::GoAlertedOrSayWhat_4BF140()
{
    if (Event_Get_422C00(kEventSpeaking)
        && !IsInInvisibleZone_425710(sControlledCharacter_5C1B8C)
        && !sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8))
    {
        GameSpeakResponse_4BF470();
        RespondWithWhat_4BF400();
    }
    else
    {
        field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
        field_294 = 29;
        SetBrain(&Slig::AI_GetAlerted_23_4BEC40);
        field_120_timer = sGnFrame_5C1B84 + field_218_tlv_data.field_3A_listen_time;
    }
}

int CCSTD Slig::IsFacingEffectiveLeft_4BB780(Slig* pSlig)
{
    int flipX = pSlig->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    if (pSlig->field_106_current_motion == eSligMotions::M_TurnAroundStanding_5_4B6390 && pSlig->field_20_animation.field_92_current_frame > 4)
    {
        // Flip the result
        return flipX == 0;
    }
    else
    {
        return flipX;
    }
}

void Slig::MoveOnLine_4B4C40()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));

    const FP xpos = field_B8_xpos;
    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(
            &field_B8_xpos,
            &field_BC_ypos,
            field_C4_velx);
        if (field_100_pCollisionLine)
        {
            if (pPlatform)
            {
                if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
                {
                    const auto curMotion = field_106_current_motion;
                    VOnTrapDoorOpen();
                    field_106_current_motion = curMotion;
                }
            }
            else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PlatformCollide_4B4E00();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_B8_xpos = xpos + field_C4_velx;
            field_F8_LastLineYPos = field_BC_ypos;
            field_106_current_motion = eSligMotions::M_FallingInitiate_39_4B4640;
            field_130 = FP_FromDouble(0.3);

            if (field_C4_velx > (FP_FromInteger(6) * field_CC_sprite_scale))
            {
                field_C4_velx = (FP_FromInteger(6) * field_CC_sprite_scale);
            }

            if (field_C4_velx < (-FP_FromInteger(6) * field_CC_sprite_scale))
            {
                field_C4_velx = (-FP_FromInteger(6) * field_CC_sprite_scale);
            }
        }
    }
    else
    {
        VOnTrapDoorOpen();
        field_106_current_motion = eSligMotions::M_OutToFall_38_4B4570;
        field_F8_LastLineYPos = field_BC_ypos;
    }
}

void Slig::PlayerControlStopWalkingIfRequired_4B8540()
{
    // Pressing other direction to facing or not pressing a direction at all?
    if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) ||
        (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) ||
        !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        // Then go to standing
        field_106_current_motion = eSligMotions::M_WalkToStand_18_4B5FC0;
    }
    field_128_input = 0;
}

void Slig::CheckPlatformVanished_4B3640()
{
    BaseGameObject* pLiftPoint = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    if (pLiftPoint)
    {
        if (pLiftPoint->field_6_flags.Get(BaseGameObject::eDead))
        {
            // Platform is somehow gone, fall.
            const auto savedMotion = field_106_current_motion;
            VOnTrapDoorOpen();
            field_106_current_motion = savedMotion;
        }
        sub_408C40();
    }
}

void Slig::vOnTrapDoorOpen_4B3690()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        field_F8_LastLineYPos = field_BC_ypos;
        VSetMotion_4081C0(eSligMotions::M_OutToFall_38_4B4570);
    }
}

__int16 Slig::MoveLift_4B3990(FP ySpeed)
{
    auto pLiftPoint =  static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (!pLiftPoint)
    {
        return eSligMotions::M_LiftGripping_48_4B3850;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), ySpeed, 0);
    CheckPlatformVanished_4B3640();
    field_C8_vely = pLiftPoint->field_C8_vely;

    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) && field_20_animation.field_92_current_frame != 5)
    {
        return field_106_current_motion;
    }

    if (ySpeed < FP_FromInteger(0))
    {
        if (pLiftPoint->vOnTopFloor_461890())
        {
            return eSligMotions::M_LiftGripping_48_4B3850;
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            return field_106_current_motion;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            return eSligMotions::M_LiftUp_49_4B3930;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            return eSligMotions::M_LiftDown_50_4B3960;
        }
    }
    else if (ySpeed >  FP_FromInteger(0))
    {
        if (pLiftPoint->vOnBottomFloor_4618F0())
        {
            return eSligMotions::M_LiftGripping_48_4B3850;
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            return field_106_current_motion;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            return eSligMotions::M_LiftDown_50_4B3960;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            return eSligMotions::M_LiftUp_49_4B3930;
        }
    }

    // Strange how this isn't "if nothing pressed and on a floor then let go ??"
    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed && pLiftPoint->vOnAnyFloor_461920())
    {
        return eSligMotions::M_LiftUngrip_47_4B3820;
    }

    pLiftPoint->vMove_4626A0(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eSligMotions::M_LiftGripping_48_4B3850;
}

void Slig::SlowDown_4B6450(FP speed)
{
    if (FP_GetExponent(field_C4_velx))
    {
        MoveOnLine_4B4C40();

        if (field_C4_velx <= FP_FromInteger(0))
        {
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (field_CC_sprite_scale * speed) + field_C4_velx;
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_C4_velx = FP_FromInteger(0);
                }
            }
        }
        else
        {
            field_C4_velx = field_C4_velx - (field_CC_sprite_scale * speed);
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Slig::ToChase_4BCFF0()
{
    field_120_timer = sGnFrame_5C1B84 + field_218_tlv_data.field_36_time_to_wait_before_chase;

    if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    field_108_next_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    SetBrain(&Slig::AI_Chasing1_18_4BCEB0);
    MusicController::sub_47FD60(8, this, 0, 0);
}

__int16 Slig::HandleEnemyStopper_4BBA00(int gridBlocks)
{
    int directedGirdBlocks = gridBlocks;
    const auto bFacingLeft = IsFacingEffectiveLeft_4BB780(this);
    if (bFacingLeft)
    {
        directedGirdBlocks = -gridBlocks;
    }

    const FP width = ScaleToGridSize_4498B0(field_CC_sprite_scale * FP_FromInteger(directedGirdBlocks)) + field_B8_xpos;
    auto pTlv = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(width),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        TlvTypes::EnemyStopper_47));

    if (!pTlv)
    {
        return 0;
    }

    if (!SwitchStates_Get_466020(pTlv->field_12_id))
    {
        return 0;
    }

    if (pTlv->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
    {
        return 1;
    }

    if (bFacingLeft == 0 && pTlv->field_10_stop_direction == Path_EnemyStopper::StopDirection::Left_0)
    {
        return 1;
    }

    if (!bFacingLeft && pTlv->field_10_stop_direction == Path_EnemyStopper::StopDirection::Right_1)
    {
        return 1;
    }
    return 0;
}

void Slig::ToPanicYelling_4BCBA0()
{
    field_108_next_motion = eSligMotions::M_SpeakPanic_28_4B54B0;
    SetBrain(&Slig::AI_PanicYelling_14_4BCA70);
}

void Slig::ToPanicTurn_4BC750()
{
    field_108_next_motion = eSligMotions::M_TurnAroundStanding_5_4B6390;
    SetBrain(&Slig::AI_PanicTurning_12_4BC490);
}

ALIVE_VAR(1, 0xBAF7E8, short, sSligsUnderControlCount_BAF7E8, 0);
ALIVE_VAR(1, 0xBAF7EC, short, sDelayIdx_BAF7EC, 0);
const __int16 sDelayTable_56075C[6] = { 0, 6, 10, 8, 4, 12 };


__int16 CC Slig::SligStableDelay_4B99B0()
{
    // Similar to the mudokon delay stuff

    if (sSligsUnderControlCount_BAF7E8 <= 1)
    {
        return 0;
    }

    if (sDelayIdx_BAF7EC >= ALIVE_COUNTOF(sDelayTable_56075C))
    {
        sDelayIdx_BAF7EC = 0;
    }

    return sDelayTable_56075C[sDelayIdx_BAF7EC++];
}

void Slig::PlayerControlRunningSlideStopOrTurn1_4B8790()
{
    const FP curVelX = field_C4_velx;

    if (curVelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        field_C4_velx = field_CC_sprite_scale * FP_FromDouble(13.2);
        field_106_current_motion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        field_106_current_motion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            field_C4_velx = field_CC_sprite_scale * FP_FromDouble(13.2);
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        }
        field_106_current_motion = eSligMotions::M_SlidingToStand_8_4B6520;
        field_128_input = 0;
        return;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        field_128_input = 0;
        return;
    }

    field_F4 = 2;
    field_F6_anim_frame = 15;
    field_124 = 1;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_128_input = 0;
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
        field_128_input = 0;
    }
}

void Slig::PlayerControlRunningSlideStopOrTurn1_4B85D0()
{
    const FP curVelX = field_C4_velx;
    if (curVelX > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        field_C4_velx = (field_CC_sprite_scale * FP_FromDouble(13.2));
        field_106_current_motion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (curVelX < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        field_106_current_motion = eSligMotions::M_SlidingTurn_9_4B6680;
        field_128_input = 0;
        return;
    }

    if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        if (curVelX >= FP_FromInteger(0))
        {
            field_C4_velx = field_CC_sprite_scale * FP_FromDouble(13.2);
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * -FP_FromDouble(13.2));
        }
        field_106_current_motion = eSligMotions::M_SlidingToStand_8_4B6520;
        field_128_input = 0;
        return;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        field_128_input = 0;
        return;
    }

    field_F4 = 2;
    field_F6_anim_frame = 6;
    field_124 = 1;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_128_input = 0;
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_128_input = 0;
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(9));
    }
}

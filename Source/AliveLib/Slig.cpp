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
    field_130 = 0;
    field_118_tlvInfo = tlvInfo;
    field_128_input = 0;
    field_158 = 0;
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

void Slig::M_StandIdle_0_4B4EC0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_StandToWalk_1_4B5F70()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Walking_2_4B5BC0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_StandToRun_3_4B62F0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Running_4_4B6000()
{
    NOT_IMPLEMENTED();
}

void Slig::M_TurnAroundStanding_5_4B6390()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Shoot_6_4B55A0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Falling_7_4B42D0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SlidingToStand_8_4B6520()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SlidingTurn_9_4B6680()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SlidingTurnToWalk_10_4B6800()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SlidingTurnToRun_11_4B6850()
{
    NOT_IMPLEMENTED();
}

void Slig::M_ReloadGun_12_4B5530()
{
    NOT_IMPLEMENTED();
}

void Slig::M_ShootToStand_13_4B5580()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SteppingToStand_14_4B8480()
{
    NOT_IMPLEMENTED();
}

void Slig::M_StandingToStep_15_4B83B0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_DepossessingAbort_16_4B8250()
{
    NOT_IMPLEMENTED();
}

void Slig::M_GameSpeak_17_4B5290()
{
    NOT_IMPLEMENTED();
}

void Slig::M_WalkToStand_18_4B5FC0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Recoil_19_4B8270()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakHereBoy_20_4B5330()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakHi_21_4B53D0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakFreeze_22_4B53F0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakGitIm_23_4B5410()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakLaugh_24_4B5430()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakBullShit1_25_4B5450()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakLookOut_26_4B5470()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakBullShit2_27_4B5490()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakPanic_28_4B54B0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakWhat_29_4B54D0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_SpeakAIFreeze_30_4B54F0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Blurgh_31_4B5510()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void Slig::M_Knockback_34_4B68A0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_KnockbackToStand_35_4B6A30()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Depossessing_36_4B7F30()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Possess_37_4B72C0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_OutToFall_38_4B4570()
{
    NOT_IMPLEMENTED();
}

void Slig::M_FallingInitiate_39_4B4640()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LandingSoft_40_4B4530()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LandingFatal_41_4B4680()
{
    NOT_IMPLEMENTED();
}

void Slig::M_ShootZ_42_4B7560()
{
    NOT_IMPLEMENTED();
}

void Slig::M_ShootZtoStand_43_4B77E0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Smash_44_4B6B90()
{
    NOT_IMPLEMENTED();
}

void Slig::M_PullLever_45_4B8950()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LiftGrip_46_4B3700()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LiftUngrip_47_4B3820()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LiftGripping_48_4B3850()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LiftUp_49_4B3930()
{
    NOT_IMPLEMENTED();
}

void Slig::M_LiftDown_50_4B3960()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Beat_51_4B6C00()
{
    NOT_IMPLEMENTED();
}

__int16 Slig::AI_Death_0_4BBFB0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_ReturnControlToAbeAndDie_1_4BC410()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Possessed_2_4BBCF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_DeathDropDeath_3_4BC1E0()
{
    NOT_IMPLEMENTED(); 
    return 0;
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
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Chasing2_17_4BCBD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Chasing1_18_4BCEB0()
{
    NOT_IMPLEMENTED();
    return 0;
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
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_Inactive_32_4B9430()
{
    NOT_IMPLEMENTED();
    return 0;
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
                        field_138_rect.x = pTlvIter->field_8_top_left.field_0_x;
                        addPoint = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::SligBoundRight_45)
                {
                    if (static_cast<Path_Slig_RightBound*>(pTlvIter)->field_10_slig_id == field_218_tlv_data.field_38_slig_id)
                    {
                        field_138_rect.w = pTlvIter->field_8_top_left.field_0_x;
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
    field_130 = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eSligMotions::M_StandIdle_0_4B4EC0;
    field_128_input = 0;
    field_12C = Math_RandomRange_496AB0(0, 60) + sGnFrame_5C1B84 + 120;
    MapFollowMe_408D10(TRUE);
}

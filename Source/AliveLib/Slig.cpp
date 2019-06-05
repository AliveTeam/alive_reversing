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

const TSligMotionFn sSlig_motion_table_5604A0[52] =
{
    &Slig::M_StandIdle_0_4B4EC0,
    &Slig::M_StandToWalk_1_4B5F70,
    &Slig::M_Walking_2_4B5BC0,
    &Slig::M_StandToRun_3_4B62F0,
    &Slig::M_Running_4_4B6000,
    &Slig::M_TurnAroundStanding_5_4B6390,
    &Slig::M_Shoot_6_4B55A0,
    &Slig::M_FallToDeath_7_4B42D0,
    &Slig::M_SlidingToStand_8_4B6520,
    &Slig::M_SlidingTurn_9_4B6680,
    &Slig::M_SlidingTurnToWalk_10_4B6800,
    &Slig::M_SlidingTurnToRun_11_4B6850,
    &Slig::M_ReloadGun_12_4B5530,
    &Slig::M_ShootToStand_13_4B5580,
    &Slig::M_SteppingToStand_14_4B8480,
    &Slig::M_StandingToStep_15_4B83B0,
    &Slig::M_DepossessingAbort_16_4B8250,
    &Slig::M_UNKNOWNbutGetsCalledWhenUsingGameSpeak_17_4B5290,
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
    &Slig::M_UNKNONWN_31_4B5510,
    &Slig::M_Sleeping_32_4B89A0,
    &Slig::M_UNKNOWN_33_4B8C50,
    &Slig::M_Knockback_34_4B68A0,
    &Slig::M_KnockbackToStand_35_4B6A30,
    &Slig::M_Depossessing_36_4B7F30,
    &Slig::M_Possess_37_4B72C0,
    &Slig::M_Falling_38_4B4570,
    &Slig::M_FallingInitiate_39_4B4640,
    &Slig::M_LandingSoft_40_4B4530,
    &Slig::M_LandingFatal_41_4B4680,
    &Slig::M_ShootZ_42_4B7560,
    &Slig::M_ShootZtoStand_43_4B77E0,
    &Slig::M_UNKNOWN_44_4B6B90,
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
    &Slig::AI_0_4BBFB0,
    &Slig::AI_1_4BC410,
    &Slig::AI_2_4BBCF0,
    &Slig::AI_3_4BC1E0,
    &Slig::AI_4_4B9D20,
    &Slig::AI_5_4B3220,
    &Slig::AI_6_4B3420,
    &Slig::AI_7_4B3240,
    &Slig::AI_8_4B3120,
    &Slig::AI_9_4B3440,
    &Slig::AI_10_4B3460,
    &Slig::AI_11_4B35C0,
    &Slig::AI_12_4BC490,
    &Slig::AI_13_4BC780,
    &Slig::AI_14_4BCA70,
    &Slig::AI_15_4BD800,
    &Slig::AI_16_4BCE30,
    &Slig::AI_17_4BCBD0,
    &Slig::AI_18_4BCEB0,
    &Slig::AI_19_4BDDD0,
    &Slig::AI_20_4BF1E0,
    &Slig::AI_21_4BE0C0,
    &Slig::AI_22_4BE990,
    &Slig::AI_23_4BEC40,
    &Slig::AI_24_4BF2B0,
    &Slig::AI_25_4BF380,
    &Slig::AI_26_4BF620,
    &Slig::AI_27_4BF600,
    &Slig::AI_28_4BFA70,
    &Slig::AI_29_4BF750,
    &Slig::AI_30_4BFA30,
    &Slig::AI_31_4B9390,
    &Slig::AI_32_4B9430,
    &Slig::AI_33_4B8DD0,
    &Slig::AI_34_4B9170,
    &Slig::AI_35_4BF640
};

struct FnPair
{
    TSligAIFn mOurFn;
    DWORD mOriginal;
};

static FnPair sAiFns[36] =
{
    { &Slig::AI_0_4BBFB0,  0x40128F },
    { &Slig::AI_1_4BC410,  0x40371F },
    { &Slig::AI_2_4BBCF0,  0x40458E },
    { &Slig::AI_3_4BC1E0,  0x401EC9 },
    { &Slig::AI_4_4B9D20,  0x4015A0 },
    { &Slig::AI_5_4B3220,  0x4B3220 }, // No stub ??
    { &Slig::AI_6_4B3420,  0x4B3420 }, // No stub ??
    { &Slig::AI_7_4B3240,  0x4032AB },
    { &Slig::AI_8_4B3120,  0x4B3120 }, // No stub ??
    { &Slig::AI_9_4B3440,  0x4B3440 }, // No stub ??
    { &Slig::AI_10_4B3460, 0x401E60 },
    { &Slig::AI_11_4B35C0, 0x40120D },
    { &Slig::AI_12_4BC490, 0x40479B },
    { &Slig::AI_13_4BC780, 0x40144C },
    { &Slig::AI_14_4BCA70, 0x403364 },
    { &Slig::AI_15_4BD800, 0x403F85 },
    { &Slig::AI_16_4BCE30, 0x4021B2 },
    { &Slig::AI_17_4BCBD0, 0x403BCA },
    { &Slig::AI_18_4BCEB0, 0x403E36 },
    { &Slig::AI_19_4BDDD0, 0x403ABC },
    { &Slig::AI_20_4BF1E0, 0x4020C2 },
    { &Slig::AI_21_4BE0C0, 0x402A59 },
    { &Slig::AI_22_4BE990, 0x40266C },
    { &Slig::AI_23_4BEC40, 0x4020D1 },
    { &Slig::AI_24_4BF2B0, 0x403B25 },
    { &Slig::AI_25_4BF380, 0x403CE2 },
    { &Slig::AI_26_4BF620, 0x4BF620 }, // No stub ??
    { &Slig::AI_27_4BF600, 0x4BF600 }, // No stub ??
    { &Slig::AI_28_4BFA70, 0x4025B3 },
    { &Slig::AI_29_4BF750, 0x40308F },
    { &Slig::AI_30_4BFA30, 0x403215 },
    { &Slig::AI_31_4B9390, 0x403E0E },
    { &Slig::AI_32_4B9430, 0x4038CD },
    { &Slig::AI_33_4B8DD0, 0x40346D },
    { &Slig::AI_34_4B9170, 0x402B17 },
    { &Slig::AI_35_4BF640, 0x4022B1 },
};

#if _WIN32 || !_WIN64
static DWORD GetOriginalFn(TSligAIFn fn)
{
    // If not running as standalone set the address to be
    // the address of the real function rather than the reimpl as the real
    // game code compares the function pointer addresses (see IsBrain(x)).
    for (const auto& addrPair : sAiFns)
    {
        if (addrPair.mOurFn == fn)
        {
            const DWORD actualAddressToUse = addrPair.mOriginal;
            // Hack to overwrite the member function pointer bytes with arbitrary data
            return actualAddressToUse;
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
        const DWORD actualAddressToUse = GetOriginalFn(fn);
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
        const DWORD actualAddressToUse = GetOriginalFn(fn);
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
    field_108_next_motion = 0;
    field_124 = 0;
    field_150 = 0;
    field_14C = 0;
    field_FC_pPathTLV = pTlv;
    
    field_218_tlv_data = *pTlv;

    field_110_id = -1;
    
    field_106_current_motion = 7;

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
    field_134 = 0;

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
        field_106_current_motion = 0;
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

void Slig::M_FallToDeath_7_4B42D0()
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

void Slig::M_UNKNOWNbutGetsCalledWhenUsingGameSpeak_17_4B5290()
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

void Slig::SpeakAIFreeze_30_4B54F0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_UNKNOWN_31_4B5510()
{
    NOT_IMPLEMENTED();
}

void Slig::M_Sleeping_32_4B89A0()
{
    NOT_IMPLEMENTED();
}

void Slig::M_UNKNOWN_33_4B8C50()
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

void Slig::M_Falling_38_4B4570()
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

void Slig::M_UNKNOWN_44_4B6B90()
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

__int16 Slig::AI_0_4BBFB0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_1_4BC410()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_2_4BBCF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_3_4BC1E0()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Slig::AI_4_4B9D20()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Slig::AI_5_4B3220()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_6_4B3420()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_7_4B3240()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_8_4B3120()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_9_4B3440()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_10_4B3460()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_11_4B35C0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_12_4BC490()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_13_4BC780()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_14_4BCA70()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_15_4BD800()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_16_4BCE30()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_17_4BCBD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_18_4BCEB0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_19_4BDDD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_20_4BF1E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_21_4BE0C0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_22_4BE990()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_23_4BEC40()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_24_4BF2B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_25_4BF380()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_26_4BF620()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_27_4BF600()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_28_4BFA70()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_29_4BF750()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_30_4BFA30()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_31_4B9390()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_32_4B9430()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_33_4B8DD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_34_4B9170()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::AI_35_4BF640()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Slig::Init_4BB0D0()
{
    NOT_IMPLEMENTED();
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

ALIVE_VAR(1, 0xBAF7E8, short, word_BAF7E8, 0);

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
        if (BrainIs(&Slig::AI_4_4B9D20))
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

        if (oldMotion != field_106_current_motion)
        {
            LOG_INFO("Slig: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        if (oldMotion != field_106_current_motion || field_114_flags.Get(Flags_114::e114_MotionChanged_Bit2))
        {
            field_114_flags.Clear(Flags_114::e114_MotionChanged_Bit2);
            vsub_4B1320();
        }
        else if (field_124)
        {
            field_106_current_motion = field_F4;
            vsub_4B1320();
            field_20_animation.SetFrame_409D50(field_F6_anim_frame);
            field_124 = 0;
        }
    }
}

void Slig::vsub_4B2EA0()
{
    NOT_IMPLEMENTED();
}

void Slig::vsub_4B1320()
{
    NOT_IMPLEMENTED();
}

#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slig.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "MusicController.hpp"
#include "Game.hpp"

START_NS_AO

ALIVE_VAR(1, 0x9F11BC, int, dword_9F11BC, 0);
ALIVE_VAR(1, 0x9F11C0, int, dword_9F11C0, 0);

TintEntry stru_4CFB10[3] =
{
    { 5, 127u, 127u, 127u },
    { 6, 127u, 127u, 127u },
    { -1, 102u, 127u, 118u }
};

using TSligStateFunction = decltype(&Slig::State_0_StandIdle_467640);

const TSligStateFunction sSligMotionTable_4CF960[] =
{
    &Slig::State_0_StandIdle_467640,
    &Slig::State_1_StandToWalk_4695D0,
    &Slig::State_2_Walking_469130,
    &Slig::State_3_StandToRun_469C00,
    &Slig::State_4_Running_469690,
    &Slig::State_5_TurnAroundStanding_469C80,
    &Slig::State_6_Shoot_468820,
    &Slig::State_7_Falling_46A1A0,
    &Slig::State_8_Unknown_4673E0,
    &Slig::State_9_SlidingToStand_469DF0,
    &Slig::State_10_SlidingTurn_469F10,
    &Slig::State_11_SlidingTurnToWalk_46A120,
    &Slig::State_12_SlidingTurnToRun_46A160,
    &Slig::State_13_Reload_4687B0,
    &Slig::State_14_ShootToStand_468810,
    &Slig::State_15_SteppingToStand_469080,
    &Slig::State_16_StandingToStep_468FD0,
    &Slig::State_17_DepossessingAbort_468750,
    &Slig::State_18_GameSpeak_467B10,
    &Slig::State_19_WalkToStand_469610,
    &Slig::State_20_Recoil_468D30,
    &Slig::State_21_SpeakHereBoy_467BD0,
    &Slig::State_22_SpeakHi_467C90,
    &Slig::State_23_SpeakFreeze_467D50,
    &Slig::State_24_SpeakGitIm_467E10,
    &Slig::State_25_SpeakLaugh_467ED0,
    &Slig::State_26_SpeakBullshit1_467F90,
    &Slig::State_27_SpeakLookOut_468050,
    &Slig::State_28_SpeakBullshit2_468110,
    &Slig::State_29_SpeakPanic_4681D0,
    &Slig::State_30_SpeakWhat_468290,
    &Slig::State_31_SpeakAIFreeze_468350,
    &Slig::State_32_Blurgh_468410,
    &Slig::State_33_Sleeping_46A410,
    &Slig::State_34_SleepingToStand_46A5F0,
    &Slig::State_35_Knockback_46A720,
    &Slig::State_36_KnockbackToStand_46A7F0,
    &Slig::State_37_Depossessing_4684D0,
    &Slig::State_38_Possess_46B050,
    &Slig::State_39_OutToFall_46A9E0,
    &Slig::State_40_FallingInitiate_46AA60,
    &Slig::State_41_LandingSoft_46A390,
    &Slig::State_42_LandingFatal_46AFE0,
    &Slig::State_43_ShootZ_468E30,
    &Slig::State_44_ShootZtoStand_468F70,
    &Slig::State_45_Smash_46A990,
    &Slig::State_46_PullLever_46A590,
    &Slig::State_47_LiftUp_4665A0,
    &Slig::State_48_LiftDown_4665C0,
    &Slig::State_49_LiftGrip_4663A0,
    &Slig::State_50_LiftUngrip_466460,
    &Slig::State_51_LiftGrip_466480,
    &Slig::State_52_Beat_46AA90,
};


Slig* Slig::ctor_464D40(Path_Slig* pTlv, int tlvInfo)
{
    ctor_401090(); // Note: Empty base skipped

    field_15C = -1;
    field_172 = -1;
    field_170 = 0;

    SetVTable(this, 0x4BCA70);

    field_210_resources = {};

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 412, 1, 0);
    field_210_resources.res[0] = ppRes;
    Animation_Init_417FD0(
        132740,
        160,
        68,
        ppRes,
        1);

    field_10A_flags.Clear(Flags_10A::e10A_Bit2);
    field_10A_flags.Set(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_4_typeId = Types::eSlig_88;

    field_114 = 0;
    field_118 = 0;
    field_11C = -1;
    field_10C = 0;
    field_10E_brain_state = 0;
    field_FE_next_state = 0;
    field_EC = 3;
    field_158 = 0;
    field_154 = 0;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = 7;
    field_11E = 0;
    field_144 = 0;
    field_12C = 0;

    field_110 = 45 * ((Math_NextRandom() % 5) - 2);

    field_F0_pTlv = pTlv;
    field_174_tlv = *pTlv;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    field_134_tlvInfo = tlvInfo;
    field_254 &= ~7u;
    field_126 = 0;
    field_130 = 100;
    dword_9F11BC = 0;
    dword_9F11C0 = 0;
    field_200 = 0;
    field_204 = 0;
    field_208 = 0;
    field_20C = 0;
    field_13A = -1;
    field_138 = 0;

    field_10_anim.field_1C_fn_ptrs = kSlig_Anim_Frame_Fns_4CEBF0;

    if (pTlv->field_18_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 33;
        field_C6_scale = 1;
    }
    else
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 14;
        field_C6_scale = 0;
    }

    SetBaseAnimPaletteTint_4187C0(
        &stru_4CFB10[0],
        gMap_507BA8.field_0_current_level,
        412);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(24),
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10) == 1)
    {
        field_FC_current_motion = 0;
        field_AC_ypos = hitY;
    }

    MapFollowMe_401D30(TRUE);

    Init_46B890();
    
    VStackOnObjectsOfType_418930(Types::eSlig_88);

    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_146 = 0;
    field_14A = 0;
    field_148 = 0;
    field_122 = 0;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* Slig::dtor_465320()
{
    SetVTable(this, 0x4BCA70);

    if (sControlledCharacter_50767C == this)
    {
        if (field_14E_level != gMap_507BA8.field_0_current_level
            || field_150_path != gMap_507BA8.field_2_current_path
            || field_152_camera != gMap_507BA8.field_4_current_camera)
        {
            Event_Broadcast_417220(kEvent_7, this);
        }

        sControlledCharacter_50767C = sActiveHero_507678;
        MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);

        if (gMap_507BA8.field_A_level != LevelIds::eMenu_0 && gMap_507BA8.field_A_level != LevelIds::eNone)
            gMap_507BA8.SetActiveCam_444660(
                field_14E_level,
                field_150_path,
                field_152_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
    }

    auto pTlv = gMap_507BA8.TLV_Get_At_446260(
        field_174_tlv.field_C_sound_pos.field_0_x,
        field_174_tlv.field_C_sound_pos.field_2_y,
        field_174_tlv.field_C_sound_pos.field_0_x,
        field_174_tlv.field_C_sound_pos.field_2_y,
        static_cast<short>(field_174_tlv.field_4_type));

    if (field_100_health <= FP_FromInteger(0))
    {
        if (pTlv)
        {
            pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
            pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
        }
    }
    else if (pTlv)
    {
        pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);
    }

    for (BYTE**& ppRes : field_210_resources.res)
    {
        if (ppRes && field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
            ppRes = nullptr;
        }
    }

    return dtor_401000(); // Note: Empty dtor skipped
}

BaseGameObject* Slig::VDestructor(signed int flags)
{
    return Vdtor_465DC0(flags);
}

Slig* Slig::Vdtor_465DC0(signed int flags)
{
    dtor_465320();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Slig::Init_46B890()
{
    NOT_IMPLEMENTED();
}

void Slig::VUpdate_Real_465050()
{
    NOT_IMPLEMENTED();
}

void Slig::VUpdate_465050()
{
    const __int16 oldBrainState = field_10E_brain_state;
    const auto oldBrain = field_1F8_fn;

    VUpdate_Real_465050();
    
    if (oldBrain != field_1F8_fn)
    {
        LOG_INFO("oldBrain = " << oldBrain << " newBrain = " << field_1F8_fn);
    }

    if (oldBrainState != field_10E_brain_state)
    {
        LOG_INFO("oldBrainState = " << oldBrainState << " newBrainState = " << field_10E_brain_state);
    }
}

void Slig::State_0_StandIdle_467640()
{
    NOT_IMPLEMENTED();
}

void Slig::State_1_StandToWalk_4695D0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_2_Walking_469130()
{
    NOT_IMPLEMENTED();
}

void Slig::State_3_StandToRun_469C00()
{
    NOT_IMPLEMENTED();
}

void Slig::State_4_Running_469690()
{
    NOT_IMPLEMENTED();
}

void Slig::State_5_TurnAroundStanding_469C80()
{
    NOT_IMPLEMENTED();
}

void Slig::State_6_Shoot_468820()
{
    NOT_IMPLEMENTED();
}

void Slig::State_7_Falling_46A1A0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_8_Unknown_4673E0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_9_SlidingToStand_469DF0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_10_SlidingTurn_469F10()
{
    NOT_IMPLEMENTED();
}

void Slig::State_11_SlidingTurnToWalk_46A120()
{
    NOT_IMPLEMENTED();
}

void Slig::State_12_SlidingTurnToRun_46A160()
{
    NOT_IMPLEMENTED();
}

void Slig::State_13_Reload_4687B0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_14_ShootToStand_468810()
{
    NOT_IMPLEMENTED();
}

void Slig::State_15_SteppingToStand_469080()
{
    NOT_IMPLEMENTED();
}

void Slig::State_16_StandingToStep_468FD0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_17_DepossessingAbort_468750()
{
    NOT_IMPLEMENTED();
}

void Slig::State_18_GameSpeak_467B10()
{
    NOT_IMPLEMENTED();
}

void Slig::State_19_WalkToStand_469610()
{
    NOT_IMPLEMENTED();
}

void Slig::State_20_Recoil_468D30()
{
    NOT_IMPLEMENTED();
}

void Slig::State_21_SpeakHereBoy_467BD0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_22_SpeakHi_467C90()
{
    NOT_IMPLEMENTED();
}

void Slig::State_23_SpeakFreeze_467D50()
{
    NOT_IMPLEMENTED();
}

void Slig::State_24_SpeakGitIm_467E10()
{
    NOT_IMPLEMENTED();
}

void Slig::State_25_SpeakLaugh_467ED0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_26_SpeakBullshit1_467F90()
{
    NOT_IMPLEMENTED();
}

void Slig::State_27_SpeakLookOut_468050()
{
    NOT_IMPLEMENTED();
}

void Slig::State_28_SpeakBullshit2_468110()
{
    NOT_IMPLEMENTED();
}

void Slig::State_29_SpeakPanic_4681D0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_30_SpeakWhat_468290()
{
    NOT_IMPLEMENTED();
}

void Slig::State_31_SpeakAIFreeze_468350()
{
    NOT_IMPLEMENTED();
}

void Slig::State_32_Blurgh_468410()
{
    NOT_IMPLEMENTED();
}

void Slig::State_33_Sleeping_46A410()
{
    NOT_IMPLEMENTED();
}

void Slig::State_34_SleepingToStand_46A5F0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_35_Knockback_46A720()
{
    NOT_IMPLEMENTED();
}

void Slig::State_36_KnockbackToStand_46A7F0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_37_Depossessing_4684D0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_38_Possess_46B050()
{
    NOT_IMPLEMENTED();
}

void Slig::State_39_OutToFall_46A9E0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_40_FallingInitiate_46AA60()
{
    NOT_IMPLEMENTED();
}

void Slig::State_41_LandingSoft_46A390()
{
    NOT_IMPLEMENTED();
}

void Slig::State_42_LandingFatal_46AFE0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_43_ShootZ_468E30()
{
    NOT_IMPLEMENTED();
}

void Slig::State_44_ShootZtoStand_468F70()
{
    NOT_IMPLEMENTED();
}

void Slig::State_45_Smash_46A990()
{
    NOT_IMPLEMENTED();
}

void Slig::State_46_PullLever_46A590()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // TODO: ToIdle ?
        field_12C = 0;
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
        field_126 = 0;
        field_128_timer = Math_RandomRange_450F20(0, 60) + gnFrameCount_507670 + 120;
        MapFollowMe_401D30(1);
    }
}

void Slig::State_47_LiftUp_4665A0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_48_LiftDown_4665C0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_49_LiftGrip_4663A0()
{
    NOT_IMPLEMENTED();
}

void Slig::State_50_LiftUngrip_466460()
{
    NOT_IMPLEMENTED();
}

void Slig::State_51_LiftGrip_466480()
{
    NOT_IMPLEMENTED();
}

void Slig::State_52_Beat_46AA90()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

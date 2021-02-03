#include "stdafx_ao.h"
#include "AmbientSound.hpp"
#include "Function.hpp"
#include "Slig.hpp"
#include "Switch.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Abe.hpp"
#include "Elum.hpp"
#include "Events.hpp"
#include "MusicController.hpp"
#include "Game.hpp"
#include "DDCheat.hpp"
#include "Input.hpp"
#include "Particle.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Sfx.hpp"
#include "CameraSwapper.hpp"
#include "PlatformBase.hpp"
#include "LiftPoint.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "Particle.hpp"
#include "Midi.hpp"
#include "GameEnderController.hpp"
#include "SnoozeParticle.hpp"
#include "GameSpeak.hpp"
#include "ScreenShake.hpp"
#include "SwitchStates.hpp"
#include <algorithm>
#include "Psx_common.hpp"

//TODO fix
#undef max
#undef min

namespace AO {

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
    &Slig::State_24_SpeakGetHim_467E10,
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
    &Slig::State_46_ToIdle_46A590,
    &Slig::State_47_LiftUp_4665A0,
    &Slig::State_48_LiftDown_4665C0,
    &Slig::State_49_LiftGrip_4663A0,
    &Slig::State_50_LiftUngrip_466460,
    &Slig::State_51_LiftGripping_466480,
    &Slig::State_52_Beat_46AA90,
};

const int sSligFrameTables_4CFA38[] =
{
    132740,
    133032,
    132588,
    133052,
    132668,
    133316,
    132772,
    132740,
    132740,
    132924,
    132804,
    133144,
    132860,
    132880,
    132996,
    133072,
    133104,
    132740,
    133016,
    133124,
    133164,
    133276,
    133204,
    133360,
    133240,
    133276,
    133204,
    133360,
    133240,
    133276,
    133204,
    133360,
    133240,
    32848,
    32744,
    32240,
    32284,
    32388,
    32388,
    29708,
    29740,
    29776,
    9080,
    8844,
    8888,
    12752,
    12356,
    22728,
    22676,
    22628,
    22652,
    22780,
    12356,
    0
};

static AIFunctionData<Slig::TBrainFn> sSligAITable[]
{
    { &Slig::Brain_SpottedEnemy_465EB0, 0x465EB0, "Brain_SpottedEnemy_0" },
    { &Slig::Brain_Paused_466030, 0x466030, "Brain_Paused_1" },
    { &Slig::Brain_EnemyDead_466190, 0x466190, "Brain_EnemyDead_2" },
    { &Slig::Brain_KilledEnemy_4662A0, 0x4662A0, "Brain_KilledEnemy_3" },
    { &Slig::Brain_Unknown_46B250, 0x46B250, "Brain_Unknown_4" },
    { &Slig::Brain_Sleeping_46B4E0, 0x46B4E0, "Brain_Sleeping_5" },
    { &Slig::Brain_WakingUp_46B700, 0x46B700, "Brain_WakingUp_6" },
    { &Slig::Brain_Inactive_46B780, 0x46B780, "Brain_Inactive_7" },
    { &Slig::Brain_Possessed_46C190, 0x46C190, "Brain_Possessed_8" },
    { &Slig::Brain_Death_46C3A0, 0x46C3A0, "Brain_Death_9" },
    { &Slig::Brain_DeathDropDeath_46C5A0, 0x46C5A0, "Brain_DeathDropDeath_10" },
    { &Slig::Brain_ReturnControlToAbeAndDie_46C760, 0x46C760, "Brain_ReturnControlToAbeAndDie_11" },
    { &Slig::Brain_PanicTurning_46C7C0, 0x46C7C0, "Brain_PanicTurning_12" },
    { &Slig::Brain_PanicRunning_46CA20, 0x46CA20, "Brain_PanicRunning_13" },
    { &Slig::Brain_PanicYelling_46CC50, 0x46CC50, "Brain_PanicYelling_14" },
    { &Slig::Brain_Chasing_46CD60, 0x46CD60, "Brain_Chasing_15" },
    { &Slig::Brain_StopChasing_46CF20, 0x46CF20, "Brain_StopChasing_16" },
    { &Slig::Brain_StartChasing_46CF90, 0x46CF90, "Brain_StartChasing_17" },
    { &Slig::Brain_Idle_46D6E0, 0x46D6E0, "Brain_Idle_18" },
    { &Slig::Brain_Turning_46DC70, 0x46DC70, "Brain_Turning_19" },
    { &Slig::Brain_Walking_46DE90, 0x46DE90, "Brain_Walking_20" },
    { &Slig::Brain_GetAlertedTurn_46E520, 0x46E520, "Brain_GetAlertedTurn_21" },
    { &Slig::Brain_GetAlerted_46E800, 0x46E800, "Brain_GetAlerted_22" },
    { &Slig::Brain_StoppingNextToMudokon_46EBB0, 0x46EBB0, "Brain_StoppingNextToMudokon_23" },
    { &Slig::Brain_BeatingUp_46EC40, 0x46EC40, "Brain_BeatingUp_24" },
    { &Slig::Brain_Discussion_46ECE0, 0x46ECE0, "Brain_Discussion_25" },
    { &Slig::Brain_GameEnder_46EEE0, 0x46EEE0, "Brain_GameEnder_26" },
    { &Slig::Brain_Shooting_46EFD0, 0x46EFD0, "Brain_Shooting_27" },
    { &Slig::Brain_ZSpottedEnemy_46F260, 0x46F260, "Brain_ZSpottedEnemy_28" },
    { &Slig::Brain_ZShooting_46F290, 0x46F290, "Brain_ZShooting_29" },
};

SfxDefinition sSligSounds_4CFB30[] =
{
    { 0, 1, 58, 40, -256, -256, 0 },
    { 0, 1, 58, 50, 0, 0, 0 },
    { 0, 1, 59, 60, 0, 0, 0 },
    { 0, 1, 59, 60, 127, 127, 0 },
    { 0, 1, 60, 60, 0, 0, 0 },
    { 0, 1, 61, 60, 0, 0, 0 }
};

SfxDefinition sSligSounds2[] =
{
    { 0, 62, 60, 127, 0, 0, 0 },
    { 0, 62, 62, 127, 0, 0, 0 },
    { 0, 62, 61, 127, 0, 0, 0 },
    { 0, 62, 63, 127, 0, 0, 0 },
    { 0, 63, 60, 127, 0, 0, 0 },
    { 0, 63, 37, 127, 0, 0, 0 },
    { 0, 63, 61, 127, 0, 0, 0 },
    { 0, 63, 38, 127, 0, 0, 0 },
    { 0, 64, 61, 127, 0, 0, 0 },
    { 0, 64, 62, 127, 0, 0, 0 },
    { 0, 64, 63, 127, 0, 0, 0 },
    { 0, 64, 39, 127, 0, 0, 0 },
    { 0, 64, 36, 127, 0, 0, 0 }
};

void Slig::Slig_SoundEffect_46F310(SligSfx sfxIdx)
{
    PSX_RECT worldRect;
    int volRight = 0;
    int volLeft = 0;

    auto dir = gMap_507BA8.GetDirection_444A40(
        static_cast<int>(field_B2_lvl_number),
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos
    );

    auto sfxIdxInt = static_cast<int>(sfxIdx);
    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        volRight = sSligSounds_4CFB30[sfxIdxInt].field_C_default_volume;
    }
    else
    {
        volRight = sSligSounds_4CFB30[sfxIdxInt].field_C_default_volume / 2;
    }
    gMap_507BA8.Get_Camera_World_Rect_444C30(dir, &worldRect);
    switch (dir)
    {
        case CameraPos::eCamCurrent_0:
        {
            volLeft = volRight;
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volLeft = sSligSounds_4CFB30[sfxIdxInt].field_C_default_volume * 2 / 3;
            volRight = volLeft;
            break;
        }
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(worldRect.w) - field_A8_xpos) / FP_FromInteger(640);
            volLeft = volRight - FP_GetExponent(percentHowFar * FP_FromInteger(volRight - (volRight / 3)));
            volRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (field_A8_xpos - FP_FromInteger(worldRect.x)) / FP_FromInteger(640);
            volLeft = volRight - FP_GetExponent(percentHowFar * FP_FromInteger(volRight));
            volRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volRight - (volRight / 3)));
            break;
        }
        default:
            return;
    }
    if (field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        volLeft = volLeft * 2 / 3;
        volRight = volRight * 2 / 3;
    }
    auto pitch = Math_RandomRange_450F20(
        sSligSounds_4CFB30[sfxIdxInt].field_E_pitch_min,
        sSligSounds_4CFB30[sfxIdxInt].field_E_pitch_min
    );
    SFX_SfxDefinition_Play_477330(&sSligSounds_4CFB30[sfxIdxInt], static_cast<short>(volLeft), static_cast<short>(volRight), pitch, pitch);
}

Slig* Slig::ctor_464D40(Path_Slig* pTlv, int tlvInfo)
{
    ctor_401090(); // Note: Empty base skipped

    field_15C = -1;
    field_172 = -1;
    field_170 = 0;

    SetVTable(this, 0x4BCA70);

    field_210_resources = {};

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgbasicResID, 1, 0);
    field_210_resources.res[0] = ppRes;
    Animation_Init_417FD0(
        132740,
        160,
        68,
        ppRes,
        1);

    field_10A_flags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    field_10A_flags.Set(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_4_typeId = Types::eSlig_88;

    field_114_timer = 0;
    field_118 = 0;
    field_11C = -1;
    field_10C = 0;
    field_10E_brain_state = 0;
    field_FE_next_state = 0;
    field_EC = 3;
    field_158_explode_timer = 0;
    field_154_death_by_being_shot_timer = 0;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = eSligStates::State_7_Falling_46A1A0;
    field_11E = 0;
    field_144 = 0;
    field_12C_falling_velx_scale_factor = FP_FromInteger(0);

    field_110_pitch_min = 45 * ((Math_NextRandom() % 5) - 2);

    field_F0_pTlv = pTlv;
    field_174_tlv = *pTlv;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    field_134_tlvInfo = tlvInfo;
    field_254_prevent_depossession &= ~7u;
    field_126_input = 0;
    field_130 = 100;
    dword_9F11BC = 0;
    dword_9F11C0 = 0;
    field_200_num_times_to_shoot = 0;
    field_204 = 0;
    field_208 = 0;
    field_20C_force_alive_state = 0;
    field_13A = -1;
    field_138_res_idx = 0;

    field_10_anim.field_1C_fn_ptr_array = kSlig_Anim_Frame_Fns_4CEBF0;

    if (pTlv->field_18_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_33;
        field_C6_scale = 1;
    }
    else
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_14;
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
    
    VStackOnObjectsOfType(Types::eSlig_88);

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
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);

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

void Slig::VScreenChanged()
{
    VScreenChanged_465480();
}

void Slig::VScreenChanged_465480()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0()
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path && this != sControlledCharacter_50767C)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Slig::Init_46B890()
{
    field_210_resources.res[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBigflashResID, 1, 0);
    field_210_resources.res[11] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSligBlowResID, 1, 0);
    field_210_resources.res[12] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kShellResID, 1, 0);
    field_210_resources.res[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgknbkResID, 1, 0);
    field_210_resources.res[16] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSquibSmokeResID, 1, 0);

    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit8))
    {
        field_210_resources.res[6] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgknfdResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit9))
    {
        field_210_resources.res[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgedgeResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit1))
    {
        field_210_resources.res[7] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgleverResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit2))
    {
        field_210_resources.res[8] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgliftResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit10))
    {
        field_210_resources.res[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgsmashResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit11))
    {
        field_210_resources.res[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgbeatResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit3))
    {
        field_210_resources.res[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgzshotResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit7))
    {
        field_210_resources.res[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlgsleepResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit4))
    {
        field_210_resources.res[13] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kZflashResID, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit6))
    {
        field_210_resources.res[14] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kUnknownResID_333, 1, 0);
    }
    if (!field_174_tlv.field_50_disable_resources.Get(SligFlags_DisabledRes::eDisabledRes_Bit5))
    {
        field_210_resources.res[15] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeknokzResID, 1, 0);
    }
    field_11C = gMap_507BA8.field_4_current_camera;
    field_20E_spotted_possessed_slig = 0;
    field_114_timer = gnFrameCount_507670 + field_174_tlv.field_1C_pause_time;

    switch (field_174_tlv.field_1A_start_state)
    {
        case Path_Slig::StartState::Paused_1:
        {
            SetBrain(&Slig::Brain_Inactive_46B780);
            SetBrain2(&Slig::Brain_Inactive_46B780);
            break;
        }
        case Path_Slig::StartState::Sleeping_2:
        {
            if (field_174_tlv.field_1_unknown && field_174_tlv.field_4E_stay_awake)
            {
                SetBrain(&Slig::Brain_Inactive_46B780);
                SetBrain2(&Slig::Brain_Inactive_46B780);
            }
            else
            {
                SetBrain(&Slig::Brain_Sleeping_46B4E0);
                SetBrain2(&Slig::Brain_Sleeping_46B4E0);
                field_FC_current_motion = eAbeStates::State_33_RunJumpMid_426FA0;
                VUpdateAnimData_464D00();
            }
            break;
        }
        case Path_Slig::StartState::Chase_3:
        {
            SetBrain(&Slig::Brain_StartChasing_46CF90);
            SetBrain2(&Slig::Brain_StartChasing_46CF90);
            field_114_timer = gnFrameCount_507670 + field_174_tlv.field_3E_time_to_wait_before_chase;
            break;
        }
        case Path_Slig::StartState::GameEnder_4:
        {
            field_114_timer = gnFrameCount_507670 + field_174_tlv.field_1C_pause_time;
            SetBrain(&Slig::Brain_GameEnder_46EEE0);
            SetBrain2(&Slig::Brain_GameEnder_46EEE0);
            field_130 = field_174_tlv.field_1C_pause_time;
            break;
        }
        case Path_Slig::StartState::Paused_5:
        {
            SetBrain(&Slig::Brain_Paused_466030);
            SetBrain2(&Slig::Brain_Paused_466030);
            break;
        }
        default:
        {
            SetBrain(&Slig::Brain_Unknown_46B250);
            SetBrain2(&Slig::Brain_Unknown_46B250);
            break;
        }
    }

    if (field_174_tlv.field_34_start_direction == 0)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
    field_1F4_points_count = 0;

    field_1CC_points[field_1F4_points_count].field_0_x = FP_GetExponent(field_A8_xpos);
    field_1CC_points[field_1F4_points_count].field_2_y = FP_GetExponent(field_AC_ypos);
    field_1F4_points_count++;

    field_13C_zone_rect = {};
    bool zoneRectSet = false;

    for (short yCam = -2; yCam < 3; yCam++)
    {
        for (short xCam = -2; xCam < 3; xCam++)
        {
            auto pTlvIter = gMap_507BA8.Get_First_TLV_For_Offsetted_Camera_4463B0(xCam, yCam);
            while (pTlvIter)
            {
                bool addPoint = false;
                if (pTlvIter->field_4_type == TlvTypes::eSligBoundLeft_57)
                {
                    if (static_cast<Path_Slig_LeftBound*>(pTlvIter)->field_18_slig_id == field_174_tlv.field_40_slig_id)
                    {
                        field_13C_zone_rect.x = pTlvIter->field_C_sound_pos.field_0_x;
                        addPoint = true;
                        zoneRectSet = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::eSligBoundRight_76)
                {
                    if (static_cast<Path_Slig_RightBound*>(pTlvIter)->field_18_slig_id == field_174_tlv.field_40_slig_id)
                    {
                        field_13C_zone_rect.w = pTlvIter->field_C_sound_pos.field_0_x;
                        addPoint = true;
                        zoneRectSet = true;
                    }
                }
                else if (pTlvIter->field_4_type == TlvTypes::eSligPersist_77)
                {
                    if (static_cast<Path_Slig_Persist*>(pTlvIter)->field_18_slig_id == field_174_tlv.field_40_slig_id)
                    {
                        addPoint = true;
                    }
                }

                if (addPoint)
                {
                    if (field_1F4_points_count < ALIVE_COUNTOF(field_1CC_points))
                    {
                        field_1CC_points[field_1F4_points_count].field_0_x = pTlvIter->field_C_sound_pos.field_0_x;
                        field_1CC_points[field_1F4_points_count].field_2_y = pTlvIter->field_C_sound_pos.field_2_y;
                        field_1F4_points_count++;
                    }
                }
                
                pTlvIter = Path_TLV::Next_446460(pTlvIter);
            }
        }
    }

    // HACK FIX: Some slig spawners don't set a slig id that matches any left/right bound.
    // In the OG this leaves the left/right zone bounds set as random values, here we fix
    // to some of these seen random values that gives the slig some what predictable patrol zones.
    if (!zoneRectSet)
    {
        LOG_INFO("Hack fix the slig walking zone (if you see this in a custom map please give your spwaned slig a left/right bound that matches the spawn id!)");
        field_13C_zone_rect.x = 12809;
        field_13C_zone_rect.w = 6405;
    }
}

void Slig::VUpdate()
{
    VUpdate_465050();
}

const unsigned int sSligVelXTable_4BCA30[] = { 262144, 262144, 0, 4294705152, 4294705152, 4294705152, 0, 262144 };
const unsigned int sSligVelYTable_4BCA50[] = { 0, 4294705152, 4294705152, 4294705152, 0, 262144, 262144, 262144 };


void Slig::VUpdate_465050()
{
    if (!Input_IsChanting_4334C0())
    {
        field_254_prevent_depossession &= ~4u;
    }

    if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::ePossessed_6, this, 1, 0);
    }

    if (sDDCheat_FlyingEnabled_50771C && sControlledCharacter_50767C == this)
    {
        field_F4_pLine = nullptr;
        if (Input().IsAnyPressed(0xF000))
        {
            const int dir = Input().Dir();
            field_B4_velx = FP_FromRaw(sSligVelXTable_4BCA30[dir]);
            field_B8_vely = FP_FromRaw(sSligVelYTable_4BCA50[dir]);

            if (Input().IsAnyPressed(0x20))
            {
                const FP velX = FP_FromRaw(sSligVelXTable_4BCA30[dir]);
                const FP velY = FP_FromRaw(sSligVelYTable_4BCA50[dir]);
                field_B4_velx += velX;
                field_B4_velx += velX;
                field_B8_vely += velY;
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
            }

        }
        else
        {
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
        }
        
        SetActiveCameraDelayedFromDir_401C90();

        field_E8_LastLineYPos = field_AC_ypos;
    }
    else
    {
        const auto old_motion = field_FC_current_motion;
        
        const auto oldBrain = field_1F8_fn;

        field_10E_brain_state = (this->*field_1F8_fn)();

        if (field_1F8_fn != oldBrain)
        {
            //LOG_INFO("Brain changed from " << GetOriginalFn(oldBrain, sSligAITable).fnName << " to " << GetOriginalFn(field_1F8_fn, sSligAITable).fnName);
        }

        if (field_106_shot)
        {
            Vshot_465C30();
        }

        if (showDebugCreatureInfo_5076E0)
        {
            DDCheat::DebugStr_495990(
                "Slig %d %d %d %d\n",
                field_10E_brain_state,
                field_114_timer,
                field_FC_current_motion,
                field_FE_next_state);
        }

        const FP new_x = field_A8_xpos;
        const FP new_y = field_AC_ypos;
        
        (this->*sSligMotionTable_4CF960[field_FC_current_motion])();

        if (new_x != field_A8_xpos || new_y != field_AC_ypos)
        {
            field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
                nullptr,
                field_A8_xpos,
                field_AC_ypos,
                field_A8_xpos,
                field_AC_ypos);

            VOn_TLV_Collision(field_F0_pTlv);
        }

        if (old_motion != field_FC_current_motion || field_108_bMotionChanged)
        {
            field_108_bMotionChanged = FALSE;
            VUpdateAnimData_464D00();

            if (VIs8_465630(old_motion))
            {
                field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            }
        }
        else if (field_11E)
        {
            field_FC_current_motion = field_E4_previous_motion;
            
            VUpdateAnimData_464D00();

            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            field_11E = 0;
        }
    }
}

void Slig::VRender(PrimHeader** ppOt)
{
    VRender_465590(ppOt);
}

void Slig::VRender_465590(PrimHeader** ppOt)
{
    if (field_8_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Slig::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_466350();
}

void Slig::VOnTrapDoorOpen_466350()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
        field_E8_LastLineYPos = field_AC_ypos;
        VSetMotion(eSligStates::State_39_OutToFall_46A9E0);
    }
}

void Slig::VUnPosses()
{
    VUnPosses_465D80();
}

void Slig::VUnPosses_465D80()
{
    field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    field_11C = gMap_507BA8.field_4_current_camera;
    field_114_timer = gnFrameCount_507670 + 180;
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
}

void Slig::VPossessed()
{
    VPossessed_465C80();
}

void Slig::VPossessed_465C80()
{
    field_10A_flags.Set(Flags_10A::e10A_Bit2_bPossesed);
    field_254_prevent_depossession |= 4u;
    if (field_FE_next_state != eSligStates::State_36_KnockbackToStand_46A7F0 && field_FE_next_state != eSligStates::State_35_Knockback_46A720)
    {
        field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    }

    SetBrain(&Slig::Brain_Possessed_46C190);
    field_10E_brain_state = 0;

    field_14E_level = gMap_507BA8.field_0_current_level;
    field_150_path = gMap_507BA8.field_2_current_path;
    field_152_camera = gMap_507BA8.field_4_current_camera;

    MusicController::PlayMusic_443810(MusicController::MusicTypes::ePossessed_6, this, 1, 0);
}


__int16 Slig::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_465640(pFrom);
}

__int16 Slig::VTakeDamage_465640(BaseGameObject* pFrom)
{
    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_10:
    {
        auto pBullet = static_cast<Bullet*>(pFrom);
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                const FP yOff = FP_FromInteger(Math_NextRandom() % 16) - FP_FromInteger(8);
                const FP xOff = (pBullet->field_20 <= FP_FromInteger(0) ? FP_FromInteger(-1) : FP_FromInteger(1) *
                    FP_FromInteger(Math_NextRandom() & 15) + FP_FromInteger(16));
                const FP xPos = (field_BC_sprite_scale * pBullet->field_20 <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6));

                pBlood->ctor_4072B0(
                    xPos + field_A8_xpos,
                    pBullet->field_1C_ypos,
                    xOff,
                    yOff,
                    field_BC_sprite_scale,
                    12);
            }

            auto pBlood2 = ao_new<Blood>();
            if (pBlood2)
            {
                 const FP xPos = (field_BC_sprite_scale * pBullet->field_20 <= FP_FromInteger(0) ? FP_FromInteger(-12) : FP_FromInteger(12));

                const FP xOff = pBullet->field_20 <= FP_FromInteger(0) ? FP_FromInteger(-6) : FP_FromInteger(6);

                pBlood2->ctor_4072B0(
                    xOff + field_A8_xpos,
                    pBullet->field_1C_ypos,
                    xPos,
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    8);
            }
        }

        field_106_shot = TRUE;
        SetBrain(&Slig::Brain_Death_46C3A0);
        field_154_death_by_being_shot_timer = gnFrameCount_507670 + 5;

        if (field_FC_current_motion != eSligStates::State_38_Possess_46B050 &&
            field_FC_current_motion != eSligStates::State_45_Smash_46A990 &&
            field_FC_current_motion != eSligStates::State_35_Knockback_46A720)
        {
            field_158_explode_timer = gnFrameCount_507670 + 20;
            field_FE_next_state = eSligStates::State_38_Possess_46B050;
            field_13A = eSligStates::State_38_Possess_46B050;
            Vshot_465C30();
            field_108_bMotionChanged = TRUE;
            if (pBullet->field_20 >= FP_FromInteger(0))
            {
                field_B4_velx = FP_FromDouble(0.001);
            }
            else
            {
                field_B4_velx = FP_FromDouble(-0.001);
            }
        }

        if (field_100_health <= FP_FromInteger(0))
        {
            return field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render) ? 1 : 0;
        }

        if (field_FC_current_motion != eSligStates::State_45_Smash_46A990 &&
            field_FC_current_motion != eSligStates::State_35_Knockback_46A720)
        {
            field_FE_next_state = eSligStates::State_38_Possess_46B050;
            field_13A = eSligStates::State_38_Possess_46B050;
        }
        field_100_health = FP_FromInteger(0);
        Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
        return 1;
    }

    case Types::eElectricWall_25:
        Slig_GameSpeak_SFX_46F560(SligSpeak::eHelp_10, 0, field_110_pitch_min, this);
        return 1;

    case Types::eBaseBomb_30:
    case Types::eMeatSaw_56:
    case Types::eExplosion_74:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            auto pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    1,
                    field_A8_xpos,
                    field_AC_ypos,
                    field_B4_velx,
                    field_B8_vely,
                    field_BC_sprite_scale);
            }
            field_100_health = FP_FromInteger(0);
            SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 90);
            field_8_update_delay = 40;
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            SetBrain(&Slig::Brain_ReturnControlToAbeAndDie_46C760);
            field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
            VUpdateAnimData_464D00();
            Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
        }
        return 1;

    case Types::eAbilityRing_69:
        return 1;

    case Types::eSlog_89:
        if (field_100_health <= FP_FromInteger(0)
            && (field_FC_current_motion == eSligStates::State_35_Knockback_46A720 ||
                field_FC_current_motion == eSligStates::State_45_Smash_46A990))
        {
            return 1;
        }
        field_100_health = FP_FromInteger(0);
        SetBrain(&Slig::Brain_Death_46C3A0);
        field_106_shot = 1;
        Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
        if (VIsFacingMe(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            field_108_bMotionChanged = TRUE;
            field_128_timer = gnFrameCount_507670 + 10;
            field_FC_current_motion = eSligStates::State_35_Knockback_46A720;
            field_FE_next_state = eSligStates::State_35_Knockback_46A720;
            field_13A = eSligStates::State_35_Knockback_46A720;
        }
        else
        {
            field_FE_next_state = eSligStates::State_45_Smash_46A990;
            field_13A = eSligStates::State_45_Smash_46A990;
        }
        return 1;

    case Types::eBeeSwarm_95:
        if (field_100_health > FP_FromInteger(0))
        {
            field_100_health -= FP_FromDouble(0.34);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eHelp_10, 0, field_110_pitch_min, this);
            if (field_100_health <= FP_FromInteger(0))
            {
                field_100_health = FP_FromInteger(0);
                field_106_shot = 1;
                field_FE_next_state = eSligStates::State_35_Knockback_46A720;
                field_13A = eSligStates::State_35_Knockback_46A720;
            }
        }
        return 1;

    case Types::eElectrocute_103:
        if (field_100_health > FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_100_health = FP_FromInteger(0);
            Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
        }
        return 1;

    case Types::eBat_6:
        break;

    default:
        if (field_100_health > FP_FromInteger(0))
        {
            SFX_Play_43AD70(SoundEffect::KillEffect_78, 127);
            SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 90);
        }
        break;
    }

    if (field_100_health > FP_FromInteger(0))
    {
        field_100_health = FP_FromInteger(0);
        field_FE_next_state = eSligStates::State_45_Smash_46A990;
        field_13A = eSligStates::State_45_Smash_46A990;
        field_106_shot = 1;
    }
    return 1;
}

void Slig::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOn_TLV_Collision_465CF0(pTlv);
}

void Slig::VOn_TLV_Collision_465CF0(Path_TLV* pTlv)
{
    while(pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
        {
            if (field_100_health > FP_FromInteger(0))
            {
                field_100_health = FP_FromInteger(0);
                field_10E_brain_state = 0;
                SetBrain(&Slig::Brain_DeathDropDeath_46C5A0);
                field_B8_vely = FP_FromInteger(0);
                field_B4_velx = FP_FromInteger(0);
                VSetMotion(eSligStates::State_7_Falling_46A1A0);
                Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
            }
        }

        pTlv = gMap_507BA8.TLV_Get_At_446060(
            pTlv,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);
    }
}

__int16 Slig::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsFacingMe_4655B0(pOther);
}

__int16 Slig::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VOnSameYLevel_46BD00(pOther);
}

__int16 Slig::VOnSameYLevel_46BD00(BaseAnimatedWithPhysicsGameObject* pOther)
{
    PSX_RECT ourRect = {};
    PSX_RECT objRect = {};
    VGetBoundingRect(&ourRect, 1);
    pOther->VGetBoundingRect(&objRect, 1);
    return ((objRect.y + objRect.h) / 2) <= ourRect.h && objRect.h >= ourRect.y;
}

__int16 Slig::VIsFacingMe_4655B0(BaseAnimatedWithPhysicsGameObject* pWho)
{
    if (field_FC_current_motion != eSligStates::State_5_TurnAroundStanding_469C80
        || field_10_anim.field_92_current_frame < 6)
    {
        if (pWho->field_A8_xpos <= field_A8_xpos && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }

        if (pWho->field_A8_xpos >= field_A8_xpos && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }
    }
    else
    {
        if (pWho->field_A8_xpos <= field_A8_xpos && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }

        if (pWho->field_A8_xpos >= field_A8_xpos && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            return 1;
        }
    }
    return 0;
}

void Slig::VUpdateAnimData_464D00()
{
    BYTE** ppRes = ResBlockForMotion_4654D0(field_FC_current_motion);
    if (!ppRes)
    {
        field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
        ppRes = ResBlockForMotion_4654D0(field_FC_current_motion);
    }
    field_10_anim.Set_Animation_Data_402A40(sSligFrameTables_4CFA38[field_FC_current_motion], ppRes);
}

void Slig::Vshot_465C30()
{
    if (field_13A != -1)
    {
        field_FC_current_motion = field_13A;
    }

    field_FE_next_state = -1;
    field_13A = -1;
    field_106_shot = FALSE;
    field_114_timer = gnFrameCount_507670 + 60;
    SetBrain(&Slig::Brain_Death_46C3A0);
}

BYTE** Slig::ResBlockForMotion_4654D0(__int16 motion)
{
    short new_idx = 0;
    if (motion < eSligStates::State_33_Sleeping_46A410)
    {
        new_idx = 0;
    }
    else if (motion < eSligStates::State_35_Knockback_46A720)
    {
        new_idx = 1;
    }
    else if (motion < eSligStates::State_39_OutToFall_46A9E0)
    {
        new_idx = 2;
    }
    else if (motion < eSligStates::State_42_LandingFatal_46AFE0)
    {
        new_idx = 3;
    }
    else if (motion < eSligStates::State_43_ShootZ_468E30)
    {
        new_idx = 4;
    }
    else if (motion < eSligStates::State_45_Smash_46A990)
    {
        new_idx = 5;
    }
    else if (motion < eSligStates::State_46_PullLever_46A590)
    {
        new_idx = 6;
    }
    else if (motion < eSligStates::State_47_LiftUp_4665A0)
    {
        new_idx = 7;
    }
    else if (motion < eSligStates::State_52_Beat_46AA90)
    {
        new_idx = 8;
    }
    else
    {
        new_idx = motion >= 53 ? 0 : 9;
    }

    if (new_idx == field_138_res_idx)
    {
        return field_210_resources.res[field_138_res_idx];
    }

    field_138_res_idx = new_idx;
    return field_210_resources.res[field_138_res_idx];
}

void Slig::ToShoot_46F1D0()
{
    field_FE_next_state = eSligStates::State_6_Shoot_468820;
    SetBrain(&Slig::Brain_Shooting_46EFD0);
    field_200_num_times_to_shoot = 0;
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ToZShoot_46F200()
{
    field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    field_114_timer = gnFrameCount_507670 + field_174_tlv.field_4C_z_shoot_delay;
    SetBrain(&Slig::Brain_ZSpottedEnemy_46F260);
    field_118 = Math_RandomRange_450F20(3, 5);
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ShouldStilBeAlive_46C0D0()
{
    if (!field_20C_force_alive_state)
    {
        // Check not falling and not in the current screen
        if (field_FC_current_motion != eSligStates::State_7_Falling_46A1A0 && field_FC_current_motion != eSligStates::State_39_OutToFall_46A9E0)
        {
            if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                if (field_1F4_points_count <= 0)
                {
                    // No patrol points, die
                    field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
                else
                {
                    int i = 0;
                    while (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        FP_FromInteger(field_1CC_points[i].field_0_x),
                        FP_FromInteger(field_1CC_points[i].field_2_y),
                        0))
                    {
                        if (i >= field_1F4_points_count)
                        {
                            // No within any out our patrol points, die
                            field_6_flags.Set(BaseGameObject::eDead_Bit3);
                            break;
                        }
                        i++;
                    }
                }
            }
        }
    }
}

void Slig::ToAbeDead_466270()
{
    field_FE_next_state = eSligStates::State_25_SpeakLaugh_467ED0;
    SetBrain(&Slig::Brain_EnemyDead_466190);
    field_114_timer = gnFrameCount_507670 + 45;
}

void Slig::WaitOrWalk_46E440()
{
    field_FE_next_state = eSligStates::State_2_Walking_469130;
    SetBrain(&Slig::Brain_Walking_46DE90);

    // Right rect bound
    if (!IsFacingEffectiveLeft_46BD70(this) && (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2)) + field_A8_xpos >= FP_FromInteger(field_13C_zone_rect.w))
    {
        PauseALittle_46DBD0();
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
        return;
    }

    // Left rect bound
    if (IsFacingEffectiveLeft_46BD70(this) && (field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2))) <= FP_FromInteger(field_13C_zone_rect.x))
    {
        PauseALittle_46DBD0();
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
        return;
    }

    Brain_Walking_46DE90();
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
}

int Slig::IsFacingEffectiveLeft_46BD70(Slig* pSlig)
{
    int bFlipX = pSlig->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    if (pSlig->field_FC_current_motion == eSligStates::State_5_TurnAroundStanding_469C80
        && pSlig->field_10_anim.field_92_current_frame > 4)
    {
        // Flip the result
        return bFlipX == 0;
    }
    return bFlipX;
}

void Slig::PauseALittle_46DBD0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_114_timer = gnFrameCount_507670 + field_174_tlv.field_1E_pause_left_min;
        if (field_174_tlv.field_20_pause_left_max > field_174_tlv.field_1E_pause_left_min)
        {
            field_114_timer += Math_NextRandom() % (field_174_tlv.field_20_pause_left_max - field_174_tlv.field_1E_pause_left_min);
        }
    }
    else
    {
        field_114_timer = gnFrameCount_507670 + field_174_tlv.field_22_pause_right_min;
        if (field_174_tlv.field_24_pause_right_max > field_174_tlv.field_22_pause_right_min)
        {
            field_114_timer += Math_NextRandom() % (field_174_tlv.field_24_pause_right_max - field_174_tlv.field_22_pause_right_min);
        }
    }

    field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    SetBrain(&Slig::Brain_Idle_46D6E0);
}

void Slig::ToTurn_46DE70()
{
    field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
    SetBrain(&Slig::Brain_Turning_46DC70);
}

void Slig::ToPanicRunning_46CC20()
{
    field_FE_next_state = eSligStates::State_4_Running_469690;
    SetBrain(&Slig::Brain_PanicRunning_46CA20);
    Brain_PanicRunning_46CA20();
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ToPanic_46CD40()
{
    field_FE_next_state = eSligStates::State_29_SpeakPanic_4681D0;
    SetBrain(&Slig::Brain_PanicYelling_46CC50);
}

void Slig::ToChase_46D080()
{
    field_114_timer = gnFrameCount_507670 + field_174_tlv.field_3E_time_to_wait_before_chase;

    if (!VIsFacingMe(sControlledCharacter_50767C))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
    field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    SetBrain(&Slig::Brain_StartChasing_46CF90);
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
}

void Slig::ToKilledAbe_4662E0()
{
    field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    SetBrain(&Slig::Brain_KilledEnemy_4662A0);
    field_114_timer = gnFrameCount_507670 + 15;
}

inline PSX_RECT MakeRectFromFP(FP x, FP y, FP w, FP h)
{
    PSX_RECT r = {};
    r.x = FP_GetExponent(x);
    r.w = FP_GetExponent(w);
    r.y = FP_GetExponent(y);
    r.h = FP_GetExponent(h);
    return r;
}

inline PSX_RECT MakeMinMaxRect(FP x, FP y, FP w, FP h, bool flipToMaxMin = false)
{
    if (flipToMaxMin)
    {
        return MakeRectFromFP(
            std::max(x, w),
            std::max(y, h),
            std::min(x, w),
            std::min(y, h)
        );
    }
    else
    {
        return MakeRectFromFP(
            std::min(x, w),
            std::min(y, h),
            std::max(x, w),
            std::max(y, h)
        );
    }
}

__int16 Slig::FindBeatTarget_46D0E0(int /*typeToFind*/, int gridBlocks)
{
    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    const FP k2Scaled = FP_FromInteger(2) * kGridSize;
    const FP kGridBlocksScaled = FP_FromInteger(gridBlocks) * kGridSize;

    const FP xAndW = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? field_A8_xpos - kGridBlocksScaled : field_A8_xpos + kGridBlocksScaled;

    PSX_RECT hitRect = MakeMinMaxRect(
        xAndW,
        field_AC_ypos,
        xAndW,
        field_AC_ypos - k2Scaled,
        true
    );

    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        auto pTargetObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pTargetObj)
        {
            break;
        }
        if (pTargetObj != this && pTargetObj->field_4_typeId == Types::eMudokon_75)
        {
            PSX_RECT bRect = {};
            pTargetObj->VGetBoundingRect(
                &bRect,
                1
            );
            if (hitRect.w <= bRect.w &&
                hitRect.x >= bRect.x &&
                hitRect.y >= bRect.y &&
                hitRect.h <= bRect.h &&
                !Slig::IsInInvisibleZone_418870(pTargetObj))
            {
                return 1;
            }
        }
    }
    return 0;
}

__int16 Slig::HandleEnemyStopper_46BF30(int gridBlocks)
{
    int directedGirdBlocks = gridBlocks;
    auto bFacingLeft = IsFacingEffectiveLeft_46BD70(this);
    if (bFacingLeft)
    {
        directedGirdBlocks = -gridBlocks;
    }

    const auto dirScaled = ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(directedGirdBlocks) + field_A8_xpos;
    auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(dirScaled),
        FP_GetExponent(field_AC_ypos),
        EnemyStopper_79
    ));

    if (!pStopper)
    {
        return 0;
    }

    //TODO different from Exoddus (inverted) - watch out!
    if (SwitchStates_Get(pStopper->field_1A_id))
    {
        return 0;
    }

    if (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2)
    {
        return 1;
    }

    if (bFacingLeft && pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0)
    {
        return 1;
    }

    if (!bFacingLeft && pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1)
    {
        return 1;
    }

    return 0;
}

void Slig::RespondToEnemyOrPatrol_465DF0()
{
    field_14C_unused = 0;

    if (field_174_tlv.field_28_chal_time)
    {
        if (sControlledCharacter_50767C->field_4_typeId != Types::eSlig_88
            || field_174_tlv.field_26_chal_type == 1)
        {
            SetBrain(&Slig::Brain_SpottedEnemy_465EB0);
            field_FE_next_state = eSligStates::State_31_SpeakAIFreeze_468350;
            field_114_timer = field_174_tlv.field_28_chal_time + gnFrameCount_507670;
        }
        else
        {
            field_20E_spotted_possessed_slig = 1;
            TurnOrWalk_46D5B0(0);
        }
    }
    else if (VIsFacingMe(sControlledCharacter_50767C))
    {
        if (field_BC_sprite_scale == sControlledCharacter_50767C->field_BC_sprite_scale)
        {
            ToShoot_46F1D0();
        }
        else
        {
            ToZShoot_46F200();
        }
    }
    else
    {
        ToTurn_46DE70();
    }
}

void Slig::TurnOrWalk_46D5B0(int a2)
{
    if (a2 == 1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (field_A8_xpos > FP_FromInteger(field_13C_zone_rect.x) + (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)))
            {
                ToTurn_46DE70();
                return;
            }
        }
        else
        {
            if (field_A8_xpos < FP_FromInteger(field_13C_zone_rect.w) - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)))
            {
                ToTurn_46DE70();
                return;
            }
        }
    }

    if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (field_A8_xpos > FP_FromInteger(field_13C_zone_rect.w) - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)))
        {
            ToTurn_46DE70();
            return;
        }
    }
    else
    {
        if (field_A8_xpos < FP_FromInteger(field_13C_zone_rect.x) + (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)))
        {
            ToTurn_46DE70();
            return;
        }
    }

    WaitOrWalk_46E440();
}

void Slig::ToPanicTurn()
{
    field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
    SetBrain(&Slig::Brain_PanicTurning_46C7C0);
}

__int16 Slig::GetNextMotionIncGameSpeak_467700(unsigned __int16 input)
{
    if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
    {
        switch (field_FE_next_state)
        {
        case eSligStates::State_21_SpeakHereBoy_467BD0:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eHereBoy_1, 0, field_110_pitch_min, this);
            return eSligStates::State_21_SpeakHereBoy_467BD0;

        case eSligStates::State_22_SpeakHi_467C90:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eHi_0, 0, field_110_pitch_min, this);
            return eSligStates::State_22_SpeakHi_467C90;

        case eSligStates::State_23_SpeakFreeze_467D50:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eStay_3, 0, field_110_pitch_min, this);
            return eSligStates::State_23_SpeakFreeze_467D50;

        case eSligStates::State_24_SpeakGetHim_467E10:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eGetHim_2, 0, field_110_pitch_min, this);
            return eSligStates::State_24_SpeakGetHim_467E10;

        case eSligStates::State_25_SpeakLaugh_467ED0:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eLaugh_7, 0, field_110_pitch_min, this);
            return eSligStates::State_25_SpeakLaugh_467ED0;

        case eSligStates::State_26_SpeakBullshit1_467F90:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eBullshit_4, 0, field_110_pitch_min, this);
            return eSligStates::State_26_SpeakBullshit1_467F90;

        case eSligStates::State_27_SpeakLookOut_468050:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eLookOut_5, 0, field_110_pitch_min, this);
            return eSligStates::State_27_SpeakLookOut_468050;

        case eSligStates::State_28_SpeakBullshit2_468110:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eBullshit2_6, 0, field_110_pitch_min, this);
            return eSligStates::State_28_SpeakBullshit2_468110;

        case eSligStates::State_29_SpeakPanic_4681D0:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eHelp_10, 0, field_110_pitch_min, this);
            return eSligStates::State_29_SpeakPanic_4681D0;

        case eSligStates::State_30_SpeakWhat_468290:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eWhat_9, 0, field_110_pitch_min, this);
            return eSligStates::State_30_SpeakWhat_468290;

        case eSligStates::State_31_SpeakAIFreeze_468350:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eFreeze_8, 0, field_110_pitch_min, this);
            return eSligStates::State_31_SpeakAIFreeze_468350;

        case eSligStates::State_32_Blurgh_468410:
            field_FE_next_state = -1;
            Slig_GameSpeak_SFX_46F560(SligSpeak::eBlurgh_11, 0, field_110_pitch_min, this);
            return eSligStates::State_32_Blurgh_468410;

        default:
            return -1;
        }
    }

    if (Input_IsChanting_4334C0())
    {
        if (field_254_prevent_depossession & 4)
        {
            return -1;
        }

        field_128_timer = gnFrameCount_507670 + 30;
        SFX_Play_43AD70(SoundEffect::PossessEffect_21, 0, 0);
        return eSligStates::State_37_Depossessing_4684D0;
    }

    if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler_4C65B8))
    {
        if (input & sInputKey_GameSpeak2_4C65BC)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::Slig_HereBoy_24);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eHereBoy_1, 0, field_110_pitch_min, this);
            return eSligStates::State_21_SpeakHereBoy_467BD0;
        }
        if (input & sInputKey_GameSpeak1_4C65C8)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::Slig_Hi_23);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eHi_0, 0, field_110_pitch_min, this);
            return eSligStates::State_22_SpeakHi_467C90;
        }

        if (input & sInputKey_GameSpeak3_4C65C0)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_27);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eFreeze_8, 0, field_110_pitch_min, this);
            return eSligStates::State_23_SpeakFreeze_467D50;
        }

        if (input & sInputKey_GameSpeak4_4C65C4)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_25);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eGetHim_2, 0, field_110_pitch_min, this);
            return eSligStates::State_24_SpeakGetHim_467E10;
        }
        return -1;
    }
    else if (Input().IsAnyPressed(sInputKey_RightGameSpeakEnabler_4C65DC))
    {
        if (input & sInputKey_GameSpeak8_4C65E0)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_8);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eLaugh_7, 0, field_110_pitch_min, this);
            return eSligStates::State_25_SpeakLaugh_467ED0;
        }

        if (input & sInputKey_GameSpeak6_4C65E8)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_5);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eBullshit_4, 0, field_110_pitch_min, this);
            return eSligStates::State_26_SpeakBullshit1_467F90;
        }

        if (input & sInputKey_GameSpeak7_4C65E4)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::Slig_LookOut_6);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eLookOut_5, 0, field_110_pitch_min, this);
            return eSligStates::State_27_SpeakLookOut_468050;
        }

        if (input & sInputKey_GameSpeak5_4C65EC)
        {
            pEventSystem_4FF954->VPushEvent_40F9E0(GameSpeakEvents::eUnknown_7);
            Slig_GameSpeak_SFX_46F560(SligSpeak::eBullshit2_6, 0, field_110_pitch_min, this);
            return eSligStates::State_28_SpeakBullshit2_468110;
        }
    }
    return -1;
}

BOOL CCSTD Slig::RenderLayerIs_46C0A0(BaseAliveGameObject* pThis)
{
    return pThis->field_10_anim.field_C_layer == Layer::eLayer_3 || pThis->field_10_anim.field_C_layer == Layer::eLayer_22;
}

__int16 CCSTD Slig::IsAbeEnteringDoor_46BEE0(BaseAliveGameObject* pThis)
{
    if (((pThis->field_4_typeId == Types::eAbe_43) &&
        (pThis->field_FC_current_motion == eAbeStates::State_156_DoorEnter_42D370 &&
            pThis->field_10_anim.field_92_current_frame > 7)) ||
        (pThis->field_FC_current_motion == eAbeStates::State_157_DoorExit_42D780 &&
            pThis->field_10_anim.field_92_current_frame < 4))
    {
        return 1;
    }
    return 0;
}

__int16 CCSTD Slig::IsWallBetween_46BE60(Slig* pLeft, BaseAliveGameObject* pRight)
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_504C6C->RayCast_40C410(
        pLeft->field_A8_xpos,
        pLeft->field_AC_ypos - FP_FromInteger(25),
        pRight->field_A8_xpos,
        pRight->field_AC_ypos - FP_FromInteger(25),
        &pLine,
        &hitX,
        &hitY,
        pLeft->field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) == 1;
}

void CC Slig::Slig_GameSpeak_SFX_46F560(SligSpeak effectId, int defaultVol, int pitch_min, BaseAliveGameObject* pObj)
{
    int volume = defaultVol;
    if (defaultVol == 0)
    {
        volume = sSligSounds2[static_cast<int>(effectId)].field_C_default_volume;
    }
    if (pObj)
    {
        if (pObj->field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            volume = FP_GetExponent(FP_FromInteger(volume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_4770F0(&sSligSounds2[static_cast<int>(effectId)], volume, pitch_min, pitch_min);
}

__int16 CCSTD Slig::IsInInvisibleZone_418870(BaseAnimatedWithPhysicsGameObject* pObj)
{
    /* TODO: Not used in AE but might be possible to activate in AO
    if (gAbeInvisibleCheat_5076F8)
    {
        return TRUE;
    }
    */

    if (Event_Get_417250(kEventAbeOhm_8))
    {
        return FALSE;
    }

    PSX_RECT rect = {};
    pObj->VGetBoundingRect(&rect, 1);

    Path_TLV* pTlv = gMap_507BA8.TLV_Get_At_446260(rect.x, rect.y, rect.w, rect.h, TlvTypes::InvisibleZone_58);
    if (pTlv)
    {
        if (rect.x >= pTlv->field_10_top_left.field_0_x &&
            rect.x <= pTlv->field_14_bottom_right.field_0_x &&
            rect.y >= pTlv->field_10_top_left.field_2_y)
        {
            if (rect.y <= pTlv->field_14_bottom_right.field_2_y &&
                rect.w >= pTlv->field_10_top_left.field_0_x &&
                rect.w <= pTlv->field_14_bottom_right.field_0_x &&
                rect.h >= pTlv->field_10_top_left.field_2_y &&
                rect.h <= pTlv->field_14_bottom_right.field_2_y)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void Slig::ToStand()
{
    field_12C_falling_velx_scale_factor = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
    field_126_input = 0;
    field_128_timer = Math_RandomRange_450F20(0, 60) + gnFrameCount_507670 + 120;
    MapFollowMe_401D30(TRUE);
}

__int16 CCSTD Slig::IsInZCover_46BDA0(BaseAnimatedWithPhysicsGameObject* pObj)
{
    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    return Bullet::InZBulletCover(FP_FromInteger(bRect.x), FP_FromInteger(bRect.y), bRect);
}

void Slig::CheckPlatformVanished()
{
    if (field_F8_pLiftPoint)
    {
        if (field_F8_pLiftPoint->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            // Platform is somehow gone, fall.
            const auto oldMotion = field_FC_current_motion;
            VOnTrapDoorOpen();
            field_FC_current_motion = oldMotion;
        }
        SetActiveCameraDelayedFromDir_401C90();
    }
}


__int16 Slig::MoveLift_4665E0(FP ySpeed)
{
    auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);

    pLiftPoint->Move_435740(FP_FromInteger(0), ySpeed, 0);
    CheckPlatformVanished();
    field_B8_vely = pLiftPoint->field_B8_vely;

    if (sControlledCharacter_50767C == this
        && !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        && field_10_anim.field_92_current_frame != 5)
    {
        return field_FC_current_motion;
    }

    if (ySpeed < FP_FromInteger(0))
    {
        if (pLiftPoint->OnTopFloor())
        {
            return eSligStates::State_51_LiftGripping_466480;
        }

        if (Input().IsAnyPressed(sInputKey_Up_4C6598))
        {
            return eSligStates::State_47_LiftUp_4665A0;
        }

        if (Input().IsAnyPressed(sInputKey_Down_4C659C))
        {
            return eSligStates::State_48_LiftDown_4665C0;
        }
    }
    else if (ySpeed > FP_FromInteger(0))
    {
        if (pLiftPoint->OnBottomFloor())
        {
            return eSligStates::State_51_LiftGripping_466480;
        }

        if (Input().IsAnyPressed(sInputKey_Down_4C659C))
        {
            return eSligStates::State_48_LiftDown_4665C0;
        }

        if (Input().IsAnyPressed(sInputKey_Up_4C6598))
        {
            return eSligStates::State_47_LiftUp_4665A0;
        }
    }

    // Strange how this isn't "if nothing pressed and on a floor then let go ??"
    if (Input().Pressed() && pLiftPoint->OnAnyFloor())
    {
        return eSligStates::State_50_LiftUngrip_466460;
    }

    pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(0), 0);
    return eSligStates::State_51_LiftGripping_466480;
}

void Slig::GameSpeakResponse_46ED60()
{
    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    const int lastIdx = pEventSystem_4FF954->field_18_last_event_index;
    if (field_15C == lastIdx)
    {
        if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
        {
            speak = GameSpeakEvents::eNone_m1;
        }
        else
        {
              speak = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_15C = lastIdx;
        speak = pEventSystem_4FF954->field_10_last_event;
    }

    switch (speak)
    {
    case GameSpeakEvents::eWhistle1_1:
    case GameSpeakEvents::eWhistle2_2:
        if (!(Math_NextRandom() & 4))
        {
           field_258_next_gamespeak_motion = eSligStates::State_26_SpeakBullshit1_467F90;
        }
        else
        {
            field_258_next_gamespeak_motion = eSligStates::State_28_SpeakBullshit2_468110;
        }
        break;

    case GameSpeakEvents::eFart_3:
    case GameSpeakEvents::eLaugh_4:
    case GameSpeakEvents::eUnknown_8:
    case GameSpeakEvents::eUnknown_14:
    case GameSpeakEvents::eUnknown_15:
    case GameSpeakEvents::eUnknown_25:
    case GameSpeakEvents::eUnknown_30:
        field_258_next_gamespeak_motion = eSligStates::State_32_Blurgh_468410;
        return;

    case GameSpeakEvents::eUnknown_5:
        field_258_next_gamespeak_motion = eSligStates::State_26_SpeakBullshit1_467F90;
        break;

    case GameSpeakEvents::Slig_LookOut_6:
        if (!(Math_NextRandom() & 4))
        {
            field_258_next_gamespeak_motion = eSligStates::State_30_SpeakWhat_468290;
        }
        else
        {
            field_258_next_gamespeak_motion = eSligStates::State_29_SpeakPanic_4681D0;
        }
        break;

    case GameSpeakEvents::eUnknown_7:
        field_258_next_gamespeak_motion = eSligStates::State_28_SpeakBullshit2_468110;
        break;

    case GameSpeakEvents::eHello_9:
    case GameSpeakEvents::Slig_Hi_23:
        field_258_next_gamespeak_motion = eSligStates::State_22_SpeakHi_467C90;
        break;

    case GameSpeakEvents::eFollowMe_10:
    case GameSpeakEvents::eWait_12:
        if (Math_NextRandom() & 8)
        {
            field_258_next_gamespeak_motion = eSligStates::State_26_SpeakBullshit1_467F90;
        }
        else
        {
            field_258_next_gamespeak_motion = eSligStates::State_28_SpeakBullshit2_468110;
        }
        break;

    case GameSpeakEvents::eAnger_11:
    case GameSpeakEvents::eUnknown_13:
    case GameSpeakEvents::eUnknown_16:
    case GameSpeakEvents::eUnknown_17:
    case GameSpeakEvents::eUnknown_18:
    case GameSpeakEvents::eUnknown_19:
    case GameSpeakEvents::eUnknown_20:
    case GameSpeakEvents::eUnknown_21:
    case GameSpeakEvents::eUnknown_22:
    case GameSpeakEvents::eUnknown_26:
    case GameSpeakEvents::eUnknown_27:
    case GameSpeakEvents::eUnknown_28:
    case GameSpeakEvents::eUnknown_31:
        field_258_next_gamespeak_motion = eSligStates::State_25_SpeakLaugh_467ED0;
        break;

    default:
        field_258_next_gamespeak_motion = eSligStates::State_30_SpeakWhat_468290;
        break;
    }
}

BOOL Slig::VIs8_465630(short motion)
{
    return motion == eSligStates::State_8_Unknown_4673E0;
}

void Slig::MoveOnLine_467490()
{
    const FP oldX = field_A8_xpos;
    if (field_F4_pLine)
    {
        field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(
            &field_A8_xpos,
            &field_AC_ypos,
            field_B4_velx);
        if (field_F4_pLine)
        {
            if (field_F8_pLiftPoint)
            {
                if ( field_F4_pLine->field_8_type != 32 && field_F4_pLine->field_8_type != 36)
                {
                    const auto oldMotion = field_FC_current_motion;
                    VOnTrapDoorOpen();
                    field_FC_current_motion = oldMotion;
                }
            }
            else
            {
                if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
                {
                    // TODO: Inlined
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
        }
        else
        {
            VOnTrapDoorOpen();
            field_E8_LastLineYPos = field_AC_ypos;
            if (field_FC_current_motion == eSligStates::State_9_SlidingToStand_469DF0 || field_FC_current_motion == eSligStates::State_10_SlidingTurn_469F10)
            {
                field_B4_velx = (field_B4_velx * FP_FromDouble(0.5));
            }
            field_FC_current_motion = eSligStates::State_40_FallingInitiate_46AA60;
            field_A8_xpos = field_B4_velx + oldX;
        }
    }
    else
    {
        VOnTrapDoorOpen();
        field_FC_current_motion = eSligStates::State_39_OutToFall_46A9E0;
        field_E8_LastLineYPos = field_AC_ypos;
    }
}

void Slig::OperateLift()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_FC_current_motion = eSligStates::State_5_TurnAroundStanding_469C80;
        field_FE_next_state = eSligStates::State_49_LiftGrip_4663A0;

        const auto camPos = gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos);
        if ((camPos != CameraPos::eCamCurrent_0 && camPos != CameraPos::eCamInvalid_m1)
            && MusicController::GetAbmientAndMusicInfo_443840(nullptr, nullptr, nullptr) <= MusicController::MusicTypes::eChase_4)
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
        }
        Slig::Slig_SoundEffect_46F310(SligSfx::eStandingTurn_1);
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            ToStand();
        }
    }
    else
    {
        field_FC_current_motion = eSligStates::State_49_LiftGrip_4663A0;
    }
}

signed __int16 Slig::HandlePlayerControlled_4667B0()
{
    FP kScaleGrid = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    const auto pressed = Input().Pressed();
    if (pressed & sInputKey_Right_4C6590)
    {
        if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (pressed & sInputKey_Run_4C65A8)
            {
                field_B4_velx = kScaleGrid / FP_FromInteger(4);
                field_FC_current_motion = eSligStates::State_3_StandToRun_469C00;
                return 1;
            }

            if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), kScaleGrid * FP_FromInteger(2)))
            {
                field_B4_velx = kScaleGrid / FP_FromInteger(9);
                field_FC_current_motion = eSligStates::State_16_StandingToStep_468FD0;
                return 1;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), kScaleGrid))
            {
                return 0;
            }
            else
            {
                field_B4_velx = kScaleGrid / FP_FromInteger(6);
                field_FC_current_motion = eSligStates::State_15_SteppingToStand_469080;
                return 1;
            }
        }
        else
        {
            field_FC_current_motion = eSligStates::State_5_TurnAroundStanding_469C80;
            return 1;
        }
    }

    if (pressed & sInputKey_Left_4C6594)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (pressed & sInputKey_Run_4C65A8)
            {
                field_FC_current_motion = eSligStates::State_3_StandToRun_469C00;
                field_B4_velx = -(kScaleGrid / FP_FromInteger(4));
                return 1;
            }
            if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), -kScaleGrid * FP_FromInteger(2)))
            {
                field_FC_current_motion = eSligStates::State_16_StandingToStep_468FD0;
                field_B4_velx = -(kScaleGrid / FP_FromInteger(9));
                return 1;
            }
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), -kScaleGrid))
            {
                return 0;
            }
            else
            {
                field_FC_current_motion = eSligStates::State_15_SteppingToStand_469080;
                field_B4_velx = -(kScaleGrid / FP_FromInteger(6));
                return 1;
            }
        }
        else
        {
            field_FC_current_motion = eSligStates::State_5_TurnAroundStanding_469C80;
            return 1;
        }
    }

    if (Input().IsAnyHeld(sInputKey_DoAction_4C65A4)
        && !(pressed & (sInputKey_LeftGameSpeakEnabler_4C65B8 | sInputKey_RightGameSpeakEnabler_4C65DC)))
    {
        if (!Input_IsChanting_4334C0())
        {
            Path_Switch* pTlv = static_cast<Path_Switch*>(gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                TlvTypes::Switch_26
            ));
            field_F0_pTlv = pTlv;
            if (pTlv)
            {
                if (FP_FromInteger(FP_GetExponent(field_A8_xpos) - pTlv->field_10_top_left.field_0_x) < kScaleGrid)
                {
                    if (!(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        auto pSwitch = static_cast<Switch*>(FindObjectOfType_418280(
                            Types::eLever_97,
                            field_A8_xpos + kScaleGrid,
                            field_AC_ypos - FP_FromInteger(5)
                        ));
                        if (pSwitch)
                        {
                            pSwitch->VPull(
                                field_A8_xpos < pSwitch->field_A8_xpos
                            );
                            field_FC_current_motion = eSligStates::State_46_PullLever_46A590;
                            return 1;
                        }
                        field_FC_current_motion = eSligStates::State_46_PullLever_46A590;
                        return 1;
                    }
                    field_FE_next_state = eSligStates::State_46_PullLever_46A590;
                    field_FC_current_motion = eSligStates::State_5_TurnAroundStanding_469C80;
                    return 1;
                }
                if (FP_FromInteger(pTlv->field_14_bottom_right.field_0_x - FP_GetExponent(field_A8_xpos)) < kScaleGrid)
                {
                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        auto pSwitch = static_cast<Switch*>(FindObjectOfType_418280(Types::eLever_97, field_A8_xpos - kScaleGrid, field_AC_ypos - FP_FromInteger(5)));
                        if (pSwitch)
                        {
                            pSwitch->VPull(
                                field_A8_xpos < pSwitch->field_A8_xpos
                            );
                        }
                        field_FC_current_motion = eSligStates::State_46_PullLever_46A590;
                        return 1;
                    }
                    field_FE_next_state = eSligStates::State_46_PullLever_46A590;
                    field_FC_current_motion = eSligStates::State_5_TurnAroundStanding_469C80;
                    return 1;
                }
                Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eWhat_9, 0, field_110_pitch_min, this);
            }
            else
            {
                Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eWhat_9, 0, field_110_pitch_min, this);
            }
            field_FC_current_motion = eSligStates::State_30_SpeakWhat_468290;
            return 1;
        }
    }
    if (pressed & sInputKey_ThrowItem_4C65B4
        && !(pressed & (sInputKey_LeftGameSpeakEnabler_4C65B8 | sInputKey_RightGameSpeakEnabler_4C65DC)))
    {
        if (!Input_IsChanting_4334C0())
        {
            field_FC_current_motion = eSligStates::State_6_Shoot_468820;
            field_128_timer = gnFrameCount_507670 + 60;
            return 1;
        }
    }
    if (pressed & sInputKey_FartRoll_4C65B0
        && !(pressed & (sInputKey_LeftGameSpeakEnabler_4C65B8 | sInputKey_RightGameSpeakEnabler_4C65DC)))
    {
        if (!Input_IsChanting_4334C0())
        {
            field_FC_current_motion = eSligStates::State_52_Beat_46AA90;
            field_128_timer = gnFrameCount_507670 + 60;
            return 1;
        }
    }

    if (pressed & sInputKey_Down_4C659C)
    {
        if (field_F8_pLiftPoint)
        {
            if (field_F8_pLiftPoint && field_F8_pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
            {
                if (FP_Abs(field_A8_xpos - FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2)) < kScaleGrid / FP_FromInteger(2))
                {
                    OperateLift();
                    return 1;
                }
            }
        }
        else
        {
            if (Input().IsAnyHeld(sInputKey_Down_4C659C))
            {
                Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eWhat_9, 0, field_110_pitch_min, this);
                field_FC_current_motion = eSligStates::State_30_SpeakWhat_468290;
                return 1;
            }
            ToStand();
            return 0;
        }
    }
    if (pressed & sInputKey_Up_4C6598)
    {
        if (field_F8_pLiftPoint)
        {
            if (field_F8_pLiftPoint->field_10C == 1)
            {
                if (FP_Abs(field_A8_xpos - FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2)) < kScaleGrid / FP_FromInteger(2))
                {
                    OperateLift();
                    return 1;
                }
            }
        }
        else
        {
            if (Input().IsAnyHeld(sInputKey_Up_4C6598))
            {
                Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eWhat_9, 0, field_110_pitch_min, this);
                field_FC_current_motion = eSligStates::State_30_SpeakWhat_468290;
                return 1;
            }
            ToStand();
            return 0;
        }
    }

    if (Input().IsAnyHeld(sInputKey_Hop_4C65A0 | sInputKey_FartRoll_4C65B0) && !(Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler_4C65B8 | sInputKey_RightGameSpeakEnabler_4C65DC)))
    {
        Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eBlurgh_11, 0, field_110_pitch_min, this);
        field_FC_current_motion = eSligStates::State_32_Blurgh_468410;
        return 1;
    }
    ToStand();
    return 0;
}

void Slig::PlayerControlRunningSlideStopOrTurn(short last_anim_frame)
{
    if (field_B4_velx > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left_4C6594))
    {
        field_B4_velx = field_BC_sprite_scale * FP_FromDouble(13.2);
        field_FC_current_motion = eSligStates::State_10_SlidingTurn_469F10;
        field_126_input = 0;
        return;
    }

    if (field_B4_velx < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right_4C6590))
    {
        field_B4_velx = field_BC_sprite_scale * FP_FromDouble(-13.2);
        field_FC_current_motion = eSligStates::State_10_SlidingTurn_469F10;
        field_126_input = 0;
        return;
    }

    if (!Input().IsAnyPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594))
    {
        if (field_B4_velx >= FP_FromInteger(0))
        {
            field_B4_velx = field_BC_sprite_scale * FP_FromDouble(13.2);
        }
        else
        {
            field_B4_velx = field_BC_sprite_scale * FP_FromDouble(-13.2);
        }

        field_FC_current_motion = eSligStates::State_9_SlidingToStand_469DF0;
        field_126_input = 0;
        return;
    }

    if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
    {
        field_126_input = 0;
        return;
    }

    field_E4_previous_motion = 2;
    field_E6_last_anim_frame = last_anim_frame;
    field_11E = 1;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
    }
    else
    {
        field_B4_velx = ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9);
    }
    field_126_input = 0;
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame4_469900()
{
    PlayerControlRunningSlideStopOrTurn(6);
}

void Slig::PlayerControlRunningSlideStopOrTurnFrame12_469A80()
{
    PlayerControlRunningSlideStopOrTurn(15);
}

void Slig::SlowDown_469D50(FP speed)
{
    if (FP_GetExponent(field_B4_velx))
    {
        MoveOnLine_467490();

        if (field_B4_velx <= FP_FromInteger(0))
        {
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx += (field_BC_sprite_scale * speed);
                if (field_B4_velx > FP_FromInteger(0))
                {
                    field_B4_velx = FP_FromInteger(0);
                }
            }
        }
        else
        {
            field_B4_velx -= (field_BC_sprite_scale * speed);
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx = FP_FromInteger(0);
            }
        }
    }
}

signed __int16 Slig::MainMovement_467020()
{
    if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
    {
        return HandlePlayerControlled_4667B0();
    }

    switch (field_FE_next_state)
    {
    case eSligStates::State_5_TurnAroundStanding_469C80:

        field_FC_current_motion = eSligStates::State_5_TurnAroundStanding_469C80;
        field_FE_next_state = -1;
        break;
    case eSligStates::State_2_Walking_469130:

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }
        else
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }

        field_FC_current_motion = eSligStates::State_1_StandToWalk_4695D0;
        field_FE_next_state = -1;
        break;
    case eSligStates::State_4_Running_469690:

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }

        field_FC_current_motion = eSligStates::State_3_StandToRun_469C00;
        field_FE_next_state = -1;
        break;
    case eSligStates::State_6_Shoot_468820:
    case eSligStates::State_43_ShootZ_468E30:
        field_12C_falling_velx_scale_factor = FP_FromInteger(0);
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_128_timer = gnFrameCount_507670 + 60;
        field_FC_current_motion = field_FE_next_state;
        field_FE_next_state = -1;
        break;
    default:
        if (field_FE_next_state < eSligStates::State_21_SpeakHereBoy_467BD0 || field_FE_next_state > eSligStates::State_32_Blurgh_468410)
        {
            if (field_FE_next_state == eSligStates::State_13_Reload_4687B0 
                || field_FE_next_state == eSligStates::State_46_PullLever_46A590 
                || field_FE_next_state == eSligStates::State_52_Beat_46AA90
                )
            {
                field_FC_current_motion = field_FE_next_state;
                field_FE_next_state = -1;
                break;
            }
        }
        else
        {
            field_FC_current_motion = GetNextMotionIncGameSpeak_467700(0);
            if (field_FC_current_motion != -1)
            {
                Event_Broadcast_417220(kEventSpeaking_1, this);
                break;
            }
        }

        ToStand();
        return 0;
    }

    return 1;
}

void Slig::ToKnockBack_467300()
{
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 0, 0x7FFF, this);
    if (FP_GetExponent(field_B4_velx))
    {
        field_A8_xpos -= field_B4_velx;
    }

    if (field_F4_pLine)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
    }
    else
    {
        field_B4_velx = FP_FromInteger(0);
    }

    if (field_FE_next_state == eSligStates::State_45_Smash_46A990)
    {
        field_B4_velx = -field_B4_velx;
    }

    MapFollowMe_401D30(1);

    if (field_B8_vely < FP_FromInteger(0))
    {
        field_B8_vely = FP_FromInteger(0);
    }

    field_FC_current_motion = eSligStates::State_35_Knockback_46A720;
    field_128_timer = gnFrameCount_507670 + 10;
}

void Slig::State_0_StandIdle_467640()
{
    if (!MainMovement_467020())
    {
        if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
        {
            if (Input_IsChanting_4334C0())
            {
                if (!(field_254_prevent_depossession & 4))
                {
                    field_FC_current_motion = eSligStates::State_37_Depossessing_4684D0;
                    field_128_timer = gnFrameCount_507670 + 30;
                    SFX_Play_43AD70(SoundEffect::PossessEffect_21, 0, 0);
                    return;
                }
            }
            else if (Input().IsAnyPressed(0x04 | 0x01))
            {
                if (Input().IsAnyHeld(0xF0))
                {
                    field_126_input = Input().Held();
                    field_FC_current_motion = eSligStates::State_18_GameSpeak_467B10;
                    return;
                }
            }
        }

        if (static_cast<int>(gnFrameCount_507670) >= field_128_timer)
        {
            field_FC_current_motion = eSligStates::State_13_Reload_4687B0;
        }
    }
}

void Slig::State_1_StandToWalk_4695D0()
{
    field_126_input |= Input().Held();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSligStates::State_2_Walking_469130;
    }

    MoveOnLine_467490();
}

void Slig::State_2_Walking_469130()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0
        && MusicController::GetAbmientAndMusicInfo_443840(nullptr, nullptr, nullptr) <= MusicController::MusicTypes::eChase_4)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
    }

    field_126_input |= Input().Held();

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx * FP_FromInteger(2)))
    {
        State_46_ToIdle_46A590();
        return;
    }

    MoveOnLine_467490();

    if (field_FC_current_motion == eSligStates::State_2_Walking_469130)
    {
        FP v12 = {};
        switch (field_10_anim.field_92_current_frame)
        {
        case 5:

            Slig_SoundEffect_46F310(SligSfx::eWalkingStep_2);
            if (!field_120_checked_if_off_screen)
            {
                field_120_checked_if_off_screen = 1;
                MapFollowMe_401D30(1);
            }
            if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
            {
                if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
                {
                    field_E4_previous_motion = eSligStates::State_4_Running_469690;
                    field_E6_last_anim_frame = 5;
                    field_11E = 1;

                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_126_input = 0;
                        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                        return;
                    }

                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    field_126_input = 0;
                    return;
                }
                field_126_input = 0;
                return;
            }
            break;
        case 11:
            
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                v12 = -(ScaleToGridSize_41FA30(field_BC_sprite_scale));
            }
            else
            {
                v12 = (ScaleToGridSize_41FA30(field_BC_sprite_scale));
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), v12 * FP_FromDouble(2.5)))
            {
                field_FC_current_motion = eSligStates::State_19_WalkToStand_469610;
            }
            else if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
            {
                if (field_FE_next_state == eSligStates::State_6_Shoot_468820)
                {
                    field_FC_current_motion = eSligStates::State_19_WalkToStand_469610;
                    field_128_timer = gnFrameCount_507670 + 60;
                }
                else if (field_FE_next_state != -1)
                {
                    field_FC_current_motion = eSligStates::State_19_WalkToStand_469610;
                }
            }
            else
            {
                if (field_B4_velx > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left_4C6594)
                    || field_B4_velx < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right_4C6590)
                    || !(Input().IsAnyPressed(sInputKey_Right_4C6590 | sInputKey_Left_4C6594)))
                {
                    field_FC_current_motion = eSligStates::State_19_WalkToStand_469610;
                }
                field_126_input = 0;
            }
            break;
        case 14:

            Slig_SoundEffect_46F310(SligSfx::eWalkingStep_2);

            if (!field_120_checked_if_off_screen)
            {
                field_120_checked_if_off_screen = 1;
                MapFollowMe_401D30(1);
            }

            if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
            {
                if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
                {
                    field_E4_previous_motion = 4;
                    field_E6_last_anim_frame = 13;
                    field_11E = 1;

                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_126_input = 0;
                        field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                        return;
                    }

                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    field_126_input = 0;
                    return;
                }
                field_126_input = 0;
                return;
            }
            break;
        default:

            field_120_checked_if_off_screen = 0;
            break;
        }
    }
}

void Slig::State_3_StandToRun_469C00()
{
    field_126_input |= Input().Held();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSligStates::State_4_Running_469690;
    }

    Event_Broadcast_417220(kEventNoise_0, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx))
    {
        ToKnockBack_467300();
    }
    else
    {
        MoveOnLine_467490();
    }
}

void Slig::State_4_Running_469690()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0
        && MusicController::GetAbmientAndMusicInfo_443840(nullptr, nullptr, nullptr) != MusicController::MusicTypes::ePossessed_6)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
    }

    field_126_input |= Input().Held();

    Event_Broadcast_417220(kEventNoise_0, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx))
    {
        ToKnockBack_467300();
    }
    else
    {
        MoveOnLine_467490();

        if (field_FC_current_motion == eSligStates::State_4_Running_469690)
        {
            if (field_10_anim.field_92_current_frame == 4 || field_10_anim.field_92_current_frame == 12)
            {
                Slig_SoundEffect_46F310(SligSfx::eRunningStep_3);

                if (field_120_checked_if_off_screen == 0)
                {
                    field_120_checked_if_off_screen = 1;
                    MapFollowMe_401D30(TRUE);
                }

                if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
                {
                    if (field_10_anim.field_92_current_frame == 4)
                    {
                        PlayerControlRunningSlideStopOrTurnFrame4_469900();
                    }
                    else
                    {
                        PlayerControlRunningSlideStopOrTurnFrame12_469A80();
                    }
                }
                else
                {
                    if (field_FE_next_state == eSligStates::State_2_Walking_469130)
                    {
                        field_FC_current_motion = eSligStates::State_2_Walking_469130;
                        field_FE_next_state = -1;

                        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                        }
                        else
                        {
                            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
                        }
                    }
                    else if (field_FE_next_state == eSligStates::State_6_Shoot_468820)
                    {
                        field_FC_current_motion = eSligStates::State_6_Shoot_468820;
                        field_FE_next_state = -1;
                        field_128_timer = gnFrameCount_507670 + 60;
                    }
                    else if (field_FE_next_state != -1)
                    {
                        if (field_B4_velx >= FP_FromInteger(0))
                        {
                            field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(13.2));
                        }
                        else
                        {
                            field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(-13.2));
                        }

                        field_FC_current_motion = eSligStates::State_9_SlidingToStand_469DF0;
                    }
                }
            }
            else
            {
                field_120_checked_if_off_screen = 0;
            }
        }
    }
}

void Slig::State_5_TurnAroundStanding_469C80()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0
        && MusicController::GetAbmientAndMusicInfo_443840(nullptr, nullptr, nullptr) <= MusicController::MusicTypes::eChase_4)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
    }

    Slig_SoundEffect_46F310(SligSfx::eStandingTurn_1);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }
}

void Slig::State_6_Shoot_468820()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
        {
            if (Input().IsAnyPressed(sInputKey_ThrowItem_4C65B4))
            {
                const FP k35Scaled = field_BC_sprite_scale * FP_FromInteger(35);
                const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                const FP k8 = FP_FromInteger(8);

                // Recoil right
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Input().IsAnyPressed(sInputKey_Right_4C6590))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions_DArray_504C6C->RayCast_40C410(
                        field_A8_xpos,
                        field_AC_ypos,
                        field_A8_xpos + (k8 * (kGridSize / k8)),
                        field_AC_ypos,
                        &pLine,
                        &hitX,
                        &hitY,
                        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) ||
                        sCollisions_DArray_504C6C->RayCast_40C410(
                            field_A8_xpos,
                            field_AC_ypos - k35Scaled,
                            field_A8_xpos + (k8 * (kGridSize / k8)),
                            field_AC_ypos - k35Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60))
                    {
                        return;
                    }
                    field_B4_velx = (kGridSize / k8);
                    field_FC_current_motion = eSligStates::State_20_Recoil_468D30;
                    return;
                }

                // Recoil left
                if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Input().IsAnyPressed(sInputKey_Left_4C6594))
                {
                    PathLine* pLine = nullptr;
                    FP hitX = {};
                    FP hitY = {};
                    if (sCollisions_DArray_504C6C->RayCast_40C410(
                        field_A8_xpos,
                        field_AC_ypos,
                        field_A8_xpos - (k8 * (kGridSize / k8)),
                        field_AC_ypos,
                        &pLine,
                        &hitX,
                        &hitY,
                        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) ||
                        sCollisions_DArray_504C6C->RayCast_40C410(
                            field_A8_xpos,
                            field_AC_ypos - k35Scaled,
                            field_A8_xpos - (k8 * (kGridSize / k8)),
                            field_AC_ypos - k35Scaled,
                            &pLine,
                            &hitX,
                            &hitY,
                            field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60))
                    {
                        return;
                    }
                    field_B4_velx = -(kGridSize / k8);

                    field_FC_current_motion = eSligStates::State_20_Recoil_468D30;
                    return;
                }

                // General recoil
                // TODO: @ LABEL_27 is there a missing condition here, AE is checking
                // for the down key?
                if (field_128_timer > static_cast<int>(gnFrameCount_507670))
                {
                    return;
                }

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_B4_velx = (kGridSize / k8);
                }
                else
                {
                    field_B4_velx = -(kGridSize / k8);
                }

                PathLine* pLine = nullptr;
                FP hitX = {};
                FP hitY = {};
                if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_AC_ypos,
                    field_A8_xpos + (k8 * field_B4_velx),
                    field_AC_ypos,
                    &pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) ||
                    sCollisions_DArray_504C6C->RayCast_40C410(
                        field_A8_xpos,
                        field_AC_ypos - k35Scaled,
                        field_A8_xpos + (k8 * field_B4_velx),
                        field_AC_ypos - k35Scaled,
                        &pLine,
                        &hitX,
                        &hitY,
                        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60))
                {
                    field_B4_velx = FP_FromInteger(0);
                }
                else
                {
                    field_FC_current_motion = eSligStates::State_20_Recoil_468D30;
                }
                return;
            }
            else
            {
                SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
                field_FC_current_motion = eSligStates::State_14_ShootToStand_468810;
                return;
            }
        }

        if (field_FE_next_state == eSligStates::State_0_StandIdle_467640)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
            field_FC_current_motion = eSligStates::State_14_ShootToStand_468810;
            field_FE_next_state = -1;
            return;
        }

        if (field_FE_next_state != -1 && field_FE_next_state != eSligStates::State_6_Shoot_468820)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
            field_FC_current_motion = eSligStates::State_14_ShootToStand_468810;
            return;
        }
    }
}

void Slig::State_7_Falling_46A1A0()
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= field_BC_sprite_scale * field_12C_falling_velx_scale_factor;
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += field_BC_sprite_scale * field_12C_falling_velx_scale_factor;
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const __int16 bHit = InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir_401C90();
    if (bHit)
    {
        switch (pLine->field_8_type)
        {
        case 0:
        case 4:
        case 32:
        case 36:
        {
            ToStand();

            field_F4_pLine = pLine;

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

            field_A8_xpos = hitX;
            field_FC_current_motion = eSligStates::State_41_LandingSoft_46A390;
            field_AC_ypos = hitY;
            MapFollowMe_401D30(TRUE);
            break;
        }

        case 1:
        case 2:
        case 5:
        case 6:
            field_AC_ypos = hitY;
            field_A8_xpos = hitX;
            ToKnockBack_467300();
            break;

        default:
            break;
        }
    }
}

void Slig::State_8_Unknown_4673E0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
        {
            field_FC_current_motion = field_E4_previous_motion;
            if (field_F8_pLiftPoint)
            {
                field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
                field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
            }
        }
        else
        {
            field_FC_current_motion = field_E4_previous_motion;
            if (field_F8_pLiftPoint)
            {
                static_cast<LiftPoint*>(field_F8_pLiftPoint)->field_12C_bMoving |= 1u;
            }
        }

        field_B2_lvl_number = gMap_507BA8.field_0_current_level;
        field_B0_path_number = gMap_507BA8.field_2_current_path;
    }
}

void Slig::State_9_SlidingToStand_469DF0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx))
    {
        ToKnockBack_467300();
    }
    else
    {
        SlowDown_469D50(FP_FromDouble(2.125));
        if (field_FC_current_motion == eSligStates::State_9_SlidingToStand_469DF0)
        {
            if (field_10_anim.field_92_current_frame >= 6 || sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    Environment_SFX_42A220(EnvironmentSfx::eSlideStop_0, 0, 0x7FFF, this);
                    MapFollowMe_401D30(1);
                    MainMovement_467020();
                }
            }
            else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Input().IsAnyPressed(sInputKey_Right_4C6590)
                || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) && Input().IsAnyPressed(sInputKey_Left_4C6594))
            {
                field_E4_previous_motion = eSligStates::State_10_SlidingTurn_469F10;
                field_E6_last_anim_frame = field_10_anim.field_92_current_frame;
                field_11E = 1;
            }
        }
    }
}

void Slig::State_10_SlidingTurn_469F10()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx))
    {
        ToKnockBack_467300();
        return;
    }

    SlowDown_469D50(FP_FromDouble(2.125));

    if (field_FC_current_motion == eSligStates::State_10_SlidingTurn_469F10 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
            {
                if (field_FE_next_state == 4)
                {
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    field_FC_current_motion = eSligStates::State_12_SlidingTurnToRun_46A160;
                    return;
                }
            }
            else
            {
                if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
                {
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    field_FC_current_motion = eSligStates::State_12_SlidingTurnToRun_46A160;
                    return;
                }
            }
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
            field_FC_current_motion = eSligStates::State_11_SlidingTurnToWalk_46A120;
        }
        else
        {
            if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
            {
                if (field_FE_next_state == 4)
                {
                    field_FC_current_motion = eSligStates::State_12_SlidingTurnToRun_46A160;
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    return;
                }
            }
            else
            {
                if (Input().IsAnyPressed(sInputKey_Run_4C65A8))
                {
                    field_FC_current_motion = eSligStates::State_12_SlidingTurnToRun_46A160;
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
                    return;
                }
            }
            field_FC_current_motion = eSligStates::State_11_SlidingTurnToWalk_46A120;
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }
    }
}

void Slig::State_11_SlidingTurnToWalk_46A120()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    MoveOnLine_467490();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        field_FC_current_motion = eSligStates::State_2_Walking_469130;
    }
}

void Slig::State_12_SlidingTurnToRun_46A160()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    MoveOnLine_467490();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        field_FC_current_motion = eSligStates::State_4_Running_469690;
    }
}

void Slig::State_13_Reload_4687B0()
{
    State_46_ToIdle_46A590();
}

void Slig::State_14_ShootToStand_468810()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MainMovement_467020();
    }
}

void Slig::State_15_SteppingToStand_469080()
{
    if (!field_10_anim.field_92_current_frame)
    {
        Slig_SoundEffect_46F310(SligSfx::eWalkingStep_2);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx))
    {
        ToKnockBack_467300();
    }
    else
    {
        MoveOnLine_467490();
        if (field_FC_current_motion == eSligStates::State_15_SteppingToStand_469080)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(1);
                State_46_ToIdle_46A590();
            }
        }
    }
}

void Slig::State_16_StandingToStep_468FD0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Input().IsAnyPressed(sInputKey_Left_4C6594 | sInputKey_Right_4C6590))
        {
            field_FC_current_motion = eSligStates::State_1_StandToWalk_4695D0;
        }
        else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_FC_current_motion = eSligStates::State_15_SteppingToStand_469080;
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(6));
            field_FC_current_motion = eSligStates::State_15_SteppingToStand_469080;
        }
    }
}

void Slig::State_17_DepossessingAbort_468750()
{
    State_46_ToIdle_46A590();
}

void Slig::State_18_GameSpeak_467B10()
{
    field_126_input |= Input().Held();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = GetNextMotionIncGameSpeak_467700(field_126_input);
        if (field_FC_current_motion == -1)
        {
            ToStand();
        }
        else
        {
            Event_Broadcast_417220(kEventSpeaking_1, this);
        }
        field_126_input = 0;
    }
}

void Slig::State_19_WalkToStand_469610()
{
    MoveOnLine_467490();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(1);
        if (!MainMovement_467020())
        {
            State_46_ToIdle_46A590();
        }
    }
}

void Slig::State_20_Recoil_468D30()
{
    MoveOnLine_467490();

    if (field_FC_current_motion == eSligStates::State_20_Recoil_468D30)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(TRUE);

            if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
            {
                if (field_FE_next_state != -1)
                {
                    SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
                    field_FC_current_motion = eSligStates::State_14_ShootToStand_468810;
                    field_FE_next_state = -1;
                }
            }
            else if (Input().IsAnyPressed(sInputKey_ThrowItem_4C65B4))
            {
                field_B4_velx = FP_FromInteger(0);
                field_E4_previous_motion = eSligStates::State_6_Shoot_468820;
                field_128_timer = gnFrameCount_507670 + 60;
                field_E6_last_anim_frame = 2;
                field_11E = 1;
            }
            else
            {
                SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
                field_FC_current_motion = eSligStates::State_14_ShootToStand_468810;
            }
        }
    }
    else if (field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_43AD70(SoundEffect::SligShoot_6, 85, 0);
    }
    else
    {
        SFX_Play_43AD70(SoundEffect::SligShoot_6, 0, 0);
    }
}

void Slig::State_21_SpeakHereBoy_467BD0()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_22_SpeakHi_467C90()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_23_SpeakFreeze_467D50()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_24_SpeakGetHim_467E10()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_25_SpeakLaugh_467ED0()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_26_SpeakBullshit1_467F90()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_27_SpeakLookOut_468050()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_28_SpeakBullshit2_468110()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_29_SpeakPanic_4681D0()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_30_SpeakWhat_468290()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_31_SpeakAIFreeze_468350()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_32_Blurgh_468410()
{
    State_18_GameSpeak_467B10();
}

void Slig::State_33_Sleeping_46A410()
{
    if (field_FE_next_state == eSligStates::State_34_SleepingToStand_46A5F0)
    {
        field_FC_current_motion = eSligStates::State_34_SleepingToStand_46A5F0;
        field_FE_next_state = -1;
        return;
    }

    if (gnFrameCount_507670 % 60)
    {
        if (!((gnFrameCount_507670 - 20) % 60))
        {
            Slig_SoundEffect_46F310(SligSfx::eSnooze2_5);

            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                auto pSnoozeParticle = ao_new<SnoozeParticle>();
                if (pSnoozeParticle)
                {
                    pSnoozeParticle->ctor_464320(
                        field_A8_xpos
                        + ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(20) : FP_FromInteger(-20)),
                        field_AC_ypos - FP_FromInteger(10),
                        field_10_anim.field_C_layer,
                        field_10_anim.field_14_scale);
                    return;
                }
            }
        }
    }
    else
    {
        Slig_SoundEffect_46F310(SligSfx::eSnooze1_4);

        if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            auto pSnoozeParticle = ao_new<SnoozeParticle>();
            if (pSnoozeParticle)
            {
                pSnoozeParticle->ctor_464320(
                    field_A8_xpos
                    + ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(20) : FP_FromInteger(-20)),
                    field_AC_ypos - FP_FromInteger(10),
                    field_10_anim.field_C_layer,
                    field_10_anim.field_14_scale);
                return;
            }
        }
    }
}

void Slig::State_34_SleepingToStand_46A5F0()
{
    // OWI hack - kill all particles, even if they're not ours!
    for (int i =0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_87)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1D4_state = SnoozeParticle::SnoozeParticleState::BlowingUp_2;
        }
    }

    if (field_10_anim.field_92_current_frame >= 2 && field_10_anim.field_92_current_frame <= 10)
    {
        Slig_SoundEffect_46F310(SligSfx::eToStand_0);
    }
    
    if (field_10_anim.field_92_current_frame == 9)
    {
        Slig_SoundEffect_46F310(SligSfx::eWalkingStep_2);
    }

    switch (field_10_anim.field_92_current_frame)
    {
    case 9:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos -= (field_BC_sprite_scale * FP_FromInteger(7));
        }
        else
        {
            field_A8_xpos += (field_BC_sprite_scale * FP_FromInteger(7));
        }
        break;

    case 10:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos -= (field_BC_sprite_scale * FP_FromInteger(12));
        }
        else
        {
            field_A8_xpos += (field_BC_sprite_scale * FP_FromInteger(12));
        }
        break;

    case 11:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos -= (field_BC_sprite_scale * FP_FromInteger(6));
        }
        else
        {
            field_A8_xpos += (field_BC_sprite_scale * FP_FromInteger(6));
        }
        break;

    default:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
            MainMovement_467020();
        }
        break;
    }
}

void Slig::State_35_Knockback_46A720()
{
    if (field_F4_pLine)
    {
        SlowDown_469D50(FP_FromDouble(0.39));
    }
    else
    {
        State_7_Falling_46A1A0();

        if (field_FC_current_motion == eSligStates::State_41_LandingSoft_46A390)
        {
            field_FC_current_motion = eSligStates::State_35_Knockback_46A720;
            field_128_timer = gnFrameCount_507670 + 10;
        }
    }
    
    Event_Broadcast_417220(kEventNoise_0, this);

    if ((gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1
        || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13
        || gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12)
        && field_10_anim.field_92_current_frame == 4)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        if (field_F4_pLine)
        {
            field_B4_velx = FP_FromInteger(0);

            if (field_100_health > FP_FromInteger(0) &&
                field_128_timer <= static_cast<int>(gnFrameCount_507670) &&
                sActiveHero_507678->field_100_health > FP_FromInteger(0))
            {
                field_FC_current_motion = eSligStates::State_36_KnockbackToStand_46A7F0;
            }
        }
    }
}

void Slig::State_36_KnockbackToStand_46A7F0()
{
    Event_Broadcast_417220(kEventNoise_0, this);
    
    if (field_10_anim.field_92_current_frame >= 2 && field_10_anim.field_92_current_frame <= 10)
    {
        Slig_SoundEffect_46F310(SligSfx::eToStand_0);
    }

    if (field_10_anim.field_92_current_frame == 9)
    {
        Slig_SoundEffect_46F310(SligSfx::eWalkingStep_2);
    }

    FP gridSize = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }
    else
    {
        gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    }

    if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), gridSize))
    {
        switch (field_10_anim.field_92_current_frame)
        {
        case 9:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
               field_A8_xpos += (field_BC_sprite_scale * field_BC_sprite_scale) *  FP_FromInteger(-3);
            }
            else
            {
              field_A8_xpos += (field_BC_sprite_scale * field_BC_sprite_scale) *  FP_FromInteger(3);
            }
            break;

        case 10:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                 field_A8_xpos += (field_BC_sprite_scale * field_BC_sprite_scale) * FP_FromInteger(-13);
            }
            else
            {
                 field_A8_xpos += (field_BC_sprite_scale * field_BC_sprite_scale) * FP_FromInteger(13);
            }
           
            break;

        case 11:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_A8_xpos += ((field_BC_sprite_scale * field_BC_sprite_scale) * FP_FromInteger(-9));
            }
            else
            {
                field_A8_xpos += ((field_BC_sprite_scale * field_BC_sprite_scale) * FP_FromInteger(9));
            }
            break;

        default:
            break;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Slig::State_37_Depossessing_4684D0()
{
    if (sControlledCharacter_50767C == this)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!Input_IsChanting_4334C0())
            {
                field_FC_current_motion = eSligStates::State_17_DepossessingAbort_468750;
            }
        }

        if (!(gnFrameCount_507670 % 4))
        {
            New_Chant_Particle_4198E0(
                field_A8_xpos + (field_BC_sprite_scale * FP_FromInteger(Math_RandomRange_450F20(-20, 20))),
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(Math_RandomRange_450F20(20, 50))),
                field_BC_sprite_scale,
                Layer::eLayer_0);
        }

        if (static_cast<int>(gnFrameCount_507670) > field_128_timer)
        {
            BlowToGibs_4685A0();
        }

    }
}

void Slig::State_38_Possess_46B050()
{
    if (!field_F4_pLine)
    {
        State_7_Falling_46A1A0();
        if (field_FC_current_motion != eSligStates::State_38_Possess_46B050)
        {
            field_FC_current_motion = eSligStates::State_38_Possess_46B050;
        }
    }

    if (static_cast<int>(gnFrameCount_507670) >= field_158_explode_timer)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            FP xOff = (FP_FromInteger(20) * field_BC_sprite_scale);
            if (field_B4_velx < FP_FromInteger(0))
            {
                xOff = -xOff;
            }

            auto pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    1,
                    field_A8_xpos,
                    field_AC_ypos,
                    xOff,
                    FP_FromInteger(0),
                    field_BC_sprite_scale);
            }
            New_Smoke_Particles_419A80(
                field_A8_xpos,
                field_AC_ypos - (FP_FromInteger(30) * field_BC_sprite_scale),
                field_BC_sprite_scale,
                3,
                0);

            if (field_BC_sprite_scale == FP_FromDouble(0.5))
            {
                SFX_Play_43AD70(SoundEffect::KillEffect_78, 80);
                SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 60);
            }
            else
            {
                SFX_Play_43AD70(SoundEffect::KillEffect_78, 127);
                SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 90);
            }

            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
            field_B8_vely = FP_FromInteger(0);
            field_B4_velx = FP_FromInteger(0);
            field_100_health = FP_FromInteger(0);
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
            field_8_update_delay = 40;
            field_114_timer = gnFrameCount_507670 + 60;
            SetBrain(&Slig::Brain_Death_46C3A0);
        }
    }

    if (static_cast<int>(gnFrameCount_507670) > field_154_death_by_being_shot_timer)
    {
        if (field_B4_velx >= FP_FromInteger(0))
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_FC_current_motion = eSligStates::State_35_Knockback_46A720;
                return;
            }
        }
        else if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_FC_current_motion = eSligStates::State_35_Knockback_46A720;
            return;
        }
        field_FC_current_motion = eSligStates::State_45_Smash_46A990;
    }
}

void Slig::State_39_OutToFall_46A9E0()
{
    State_7_Falling_46A1A0();

    const FP fallDepth = field_AC_ypos - field_E8_LastLineYPos;
    if (fallDepth > FP_FromInteger(16 * 640))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_FC_current_motion == eSligStates::State_41_LandingSoft_46A390)
    {
        if (fallDepth > FP_FromInteger(240))
        {
            field_E8_LastLineYPos += FP_FromInteger(240);
        }

        if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(180))
        {
            field_FC_current_motion = eSligStates::State_42_LandingFatal_46AFE0;
            field_128_timer = gnFrameCount_507670 + 30;
            field_100_health = FP_FromInteger(0);
            Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
        }
    }
}

void Slig::State_40_FallingInitiate_46AA60()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSligStates::State_39_OutToFall_46A9E0;
    }

    State_7_Falling_46A1A0();
}

void Slig::State_41_LandingSoft_46A390()
{
    if (!field_10_anim.field_92_current_frame)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        State_46_ToIdle_46A590();
    }
}

void Slig::State_42_LandingFatal_46AFE0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
        SFX_Play_43AD70(SoundEffect::KillEffect_78, 80);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (static_cast<int>(gnFrameCount_507670) >= field_128_timer && !BrainIs(&Slig::Brain_Death_46C3A0))
        {
            field_100_health = FP_FromInteger(0);
            field_114_timer = gnFrameCount_507670 + 60;
            SetBrain(&Slig::Brain_Death_46C3A0);
        }
    }
}

void Slig::State_43_ShootZ_468E30()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_state != -1)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::eHitBottomOfDeathPit_10, 1, 1);
            field_FC_current_motion = eSligStates::State_44_ShootZtoStand_468F70;
            field_FE_next_state = -1;
        }
    }
    else if (field_10_anim.field_92_current_frame == 7)
    {
        auto pBullet = ao_new<Bullet>();
        if (pBullet)
        {
            pBullet->ctor_409380(
                this,
                BulletType::ZBullet_2,
                field_A8_xpos,
                field_AC_ypos - FP_FromInteger(12),
                FP_FromInteger(640),
                0,
                field_BC_sprite_scale,
                field_174_tlv.field_2A_number_of_times_to_shoot - field_200_num_times_to_shoot - 1);
        }

        New_ShootingZFire_Particle_419810(
            field_A8_xpos,
            field_AC_ypos - FP_FromInteger(12),
            field_BC_sprite_scale);

        if (field_BC_sprite_scale == FP_FromDouble(0.5))
        {
            SFX_Play_43AD70(SoundEffect::SligShoot_6, 85, 0);
        }
        else
        {
            SFX_Play_43AD70(SoundEffect::SligShoot_6, 0, 0);
        }

        // The doves don't like bullets
        Dove::All_FlyAway_40F390();

        Event_Broadcast_417220(kEvent_2, this);
        Event_Broadcast_417220(kEvent_14, this);
    }
}

void Slig::State_44_ShootZtoStand_468F70()
{
    State_46_ToIdle_46A590();
}

void Slig::State_45_Smash_46A990()
{
    if (field_F4_pLine)
    {
        if (field_10_anim.field_92_current_frame == 4)
        {
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
        }
    }
    else
    {
        State_7_Falling_46A1A0();

        if (field_FC_current_motion != eSligStates::State_45_Smash_46A990)
        {
            field_FC_current_motion = eSligStates::State_45_Smash_46A990;
        }
    }
}

void Slig::State_46_ToIdle_46A590()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Slig::State_47_LiftUp_4665A0()
{
    field_FC_current_motion = MoveLift_4665E0(FP_FromInteger(-4));
}

void Slig::State_48_LiftDown_4665C0()
{
    field_FC_current_motion = MoveLift_4665E0(FP_FromInteger(4));
}

void Slig::State_49_LiftGrip_4663A0()
{
    auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);

    pLiftPoint->Move_435740(FP_FromInteger(0), FP_FromInteger(0), 0);
    field_B8_vely = FP_FromInteger(0);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Input().IsAnyPressed(sInputKey_Up_4C6598))
        {
            if (pLiftPoint->OnTopFloor())
            {
                field_FC_current_motion = eSligStates::State_51_LiftGripping_466480;
            }
            else
            {
                field_FC_current_motion = eSligStates::State_47_LiftUp_4665A0;

            }
            return;
        }
        
        if (Input().IsAnyPressed(sInputKey_Down_4C659C))
        {
            if (pLiftPoint->OnBottomFloor())
            {
                field_FC_current_motion = eSligStates::State_51_LiftGripping_466480;
            }
            else
            {
                field_FC_current_motion = eSligStates::State_48_LiftDown_4665C0;
            }
            return;
        }

        field_FC_current_motion = eSligStates::State_51_LiftGripping_466480;
    }
}

void Slig::State_50_LiftUngrip_466460()
{
    field_B8_vely = FP_FromInteger(0);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Slig::State_51_LiftGripping_466480()
{
    CheckPlatformVanished();

    auto pLift = static_cast<LiftPoint*>(field_F8_pLiftPoint);

    pLift->Move_435740(FP_FromInteger(0), FP_FromInteger(0), 0);
    field_B8_vely = FP_FromInteger(0);

    if (Input().IsAnyPressed(sInputKey_Up_4C6598))
    {
        if (!pLift->OnTopFloor())
        {
            field_FC_current_motion = eSligStates::State_47_LiftUp_4665A0;
        }
    }
    else
    {
        if (Input().IsAnyPressed(sInputKey_Down_4C659C))
        {
            if (!pLift->OnBottomFloor())
            {
                field_FC_current_motion = eSligStates::State_48_LiftDown_4665C0;
            }
        }
        else if (pLift->OnAnyFloor())
        {
            field_FC_current_motion = eSligStates::State_50_LiftUngrip_466460;
        }
    }
}

void Slig::State_52_Beat_46AA90()
{
    if (field_10_anim.field_92_current_frame == 5)
    {
        SFX_Play_43AE60(SoundEffect::AirStream_28, 90, -300);
    }
    if (field_10_anim.field_92_current_frame == 8)
    {
        const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
        const FP k2Scaled = FP_FromInteger(2) * kGridSize;

        PSX_RECT hitRect = {};
        hitRect = MakeMinMaxRect(
            field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? field_A8_xpos - kGridSize : field_A8_xpos + kGridSize,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos - k2Scaled
        );

        for(int idx = 0; idx < gBaseAliveGameObjects_4FC8A0->Size(); idx++)
        {
            auto pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(idx);

            if (!pObjIter)
            {
                break;
            }
            if (pObjIter != this && pObjIter->field_4_typeId == Types::eMudokon_75)
            {
                PSX_RECT bRect = {};
                pObjIter->VGetBoundingRect(&bRect, 1);

                if (pObjIter->field_100_health > FP_FromInteger(0)
                    && PSX_Rects_overlap_no_adjustment(&hitRect, &bRect))
                {
                    pObjIter->VTakeDamage(this);
                    Event_Broadcast_417220(kEventNoise_0, this);
                    SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 60);
                    return;
                }
            }
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
    }
}

__int16 Slig::Brain_SpottedEnemy_465EB0()
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0)
        || !field_174_tlv.field_32_chase_abe)
    {
        if (VOnSameYLevel(sControlledCharacter_50767C)
            && VIsObj_GettingNear_On_X(sControlledCharacter_50767C)
            && VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_50767C) &&
            !Event_Get_417250(kEventResetting_6))
        {
            ToShoot_46F1D0();
        }
        else if (VOnSameYLevel(sControlledCharacter_50767C)
            && Event_Get_417250(kEventAbeOhm_8)
            && VIsFacingMe(sControlledCharacter_50767C))
        {
            ToShoot_46F1D0();
        }
        else if (field_114_timer > static_cast<int>(gnFrameCount_507670))
        {
            if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
            {
                ShouldStilBeAlive_46C0D0();
            }
            else
            {
                ToAbeDead_466270();
            }
        }
        else
        {
            if (VIsFacingMe(sControlledCharacter_50767C))
            {
                ToShoot_46F1D0();
            }
            else
            {
                ToTurn_46DE70();
            }
        }
    }
    else
    {
        ToChase_46D080();
    }
    return 302;
}

__int16 Slig::Brain_Paused_466030()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    switch (field_10E_brain_state)
    {
    case 0:
        break;

    case 1:
        if (VIsFacingMe(sActiveHero_507678))
        {
            if (field_FC_current_motion != eSligStates::State_0_StandIdle_467640)
            {
                return field_10E_brain_state;
            }
            field_FE_next_state = eSligStates::State_31_SpeakAIFreeze_468350;
            return 3;
        }
        else
        {
            field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
            return 2;
        }
        break;

    case 2:
        if (field_FC_current_motion != eSligStates::State_5_TurnAroundStanding_469C80
            || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_10E_brain_state;
        }
        return 1;

    case 3:
        if (field_FC_current_motion != eSligStates::State_31_SpeakAIFreeze_468350
            || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_10E_brain_state;
        }
        field_FE_next_state = eSligStates::State_4_Running_469690;
        return 4;

    case 4:
        if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
        return field_10E_brain_state;

    default:
        return field_10E_brain_state;
    }

    bool found = false;
    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eGameEnderController_37)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        GameEnderController::CreateGameEnderController_41C7D0();
    }

    field_FC_current_motion = eSligStates::State_7_Falling_46A1A0;
    field_FE_next_state = eSligStates::State_0_StandIdle_467640;
    return 1;
}

__int16 Slig::Brain_EnemyDead_466190()
{
    if (Event_Get_417250(4)
        && !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return 113;
    }

    if (field_114_timer >= static_cast<int>(gnFrameCount_507670))
    {
        ShouldStilBeAlive_46C0D0();
    }
    else
    {
        const auto rnd = Math_NextRandom();

        // Say this often
        if (rnd < 25u)
        {
            field_FE_next_state = eSligStates::State_26_SpeakBullshit1_467F90;
            field_114_timer = gnFrameCount_507670 + 15;
            return 113;
        }

        // Say this less often
        if (rnd < 50u)
        {
            field_FE_next_state = eSligStates::State_28_SpeakBullshit2_468110;
            field_114_timer = gnFrameCount_507670 + 15;
            return 113;
        }

        // And turn even less often
        if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
        {
            TurnOrWalk_46D5B0(0);
            return 113;
        }
    }
    return 113;
}

__int16 Slig::Brain_KilledEnemy_4662A0()
{
    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
    {
        ShouldStilBeAlive_46C0D0();
    }
    else
    {
        field_FE_next_state = eSligStates::State_25_SpeakLaugh_467ED0;
        field_114_timer =  gnFrameCount_507670 + 45;
        SetBrain(&Slig::Brain_EnemyDead_466190);
    }
    return 114;
}

__int16 Slig::Brain_Unknown_46B250()
{
    if (!VOnSameYLevel(sControlledCharacter_50767C)
        || !VIsFacingMe_4655B0(sControlledCharacter_50767C)
        || IsInInvisibleZone_418870(sControlledCharacter_50767C)
        || IsWallBetween_46BE60(this, sControlledCharacter_50767C)
        || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0)
        || field_20E_spotted_possessed_slig
        && sControlledCharacter_50767C->field_4_typeId == Types::eSlig_88
        || Event_Get_417250(kEventResetting_6))
    {
        if (!VOnSameYLevel(
            sControlledCharacter_50767C)
            || !VIsFacingMe_4655B0(sControlledCharacter_50767C)
            || IsInInvisibleZone_418870(sControlledCharacter_50767C)
            || IsWallBetween_46BE60(this, sControlledCharacter_50767C)
            || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0)
            || Event_Get_417250(kEventResetting_6))
        {
            if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
            {
                BaseAliveGameObject *pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSuspiciousNoise_10));
                if (!pEvent)
                {
                    pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSpeaking_1));
                }

                if (pEvent &&
                    pEvent->field_BC_sprite_scale == field_BC_sprite_scale &&
                    pEvent != this &&
                    field_114_timer <= static_cast<int>(gnFrameCount_507670) &&
                    !Event_Get_417250(kEventResetting_6))
                {
                    ToTurn_46DE70();
                }
                else if (sControlledCharacter_50767C->field_BC_sprite_scale <= field_BC_sprite_scale
                    || sControlledCharacter_50767C != sActiveHero_507678
                    && sControlledCharacter_50767C != gElum_507680
                    || !VIsFacingMe_4655B0(sControlledCharacter_50767C)
                    || IsInInvisibleZone_418870(sControlledCharacter_50767C)
                    || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0)
                    || IsInZCover_46BDA0(sControlledCharacter_50767C)
                    || IsInZCover_46BDA0(this)
                    || Event_Get_417250(kEventResetting_6))
                {
                    ShouldStilBeAlive_46C0D0();
                }
                else
                {
                    ToZShoot_46F200();
                }
            }
            else
            {
                ToAbeDead_466270();
            }
        }
        else
        {
            ToShoot_46F1D0();
        }
    }
    else
    {
        RespondToEnemyOrPatrol_465DF0();
    }
    return 101;
}

void Slig::WakeUp()
{
    field_FE_next_state = eSligStates::State_34_SleepingToStand_46A5F0;
    SetBrain(&Slig::Brain_WakingUp_46B700);
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eChase_4, this, 0, 0);
    auto pTlv = static_cast<Path_Slig*>(gMap_507BA8.TLV_Get_At_446260(
        field_174_tlv.field_C_sound_pos.field_0_x,
        field_174_tlv.field_C_sound_pos.field_2_y,
        field_174_tlv.field_C_sound_pos.field_0_x,
        field_174_tlv.field_C_sound_pos.field_2_y,
        TlvTypes::Slig_24
        ));
    if (pTlv)
    {
        pTlv->field_1_unknown = 1;
    }
}

void Start_Slig_Sounds_Helper(Sound_Ambiance_Array array, CameraPos camPos, unsigned __int8 ambianceId)
{
    if (!array.mArray[ambianceId].field_8_pScopedSeq)
    {
        array.mArray[ambianceId].field_8_pScopedSeq = ao_new<ScopedSeq>();
        if (array.mArray[ambianceId].field_8_pScopedSeq)
        {
            array.mArray[ambianceId].field_8_pScopedSeq->ctor_476400(ambianceId, camPos);
        }
    }
}

__int16 Slig::Brain_Sleeping_46B4E0()
{
    const auto pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventNoise_0));
    if (pEvent)
    {
        if (pEvent->field_BC_sprite_scale == field_BC_sprite_scale)
        {
            const auto kScaleGrid = ScaleToGridSize_41FA30(field_BC_sprite_scale);
            const auto wake_up_dist_scaled = kScaleGrid * FP_FromInteger(field_174_tlv.field_52_noise_wake_up_distance);
            if (VIsObjNearby(wake_up_dist_scaled, pEvent) &&
                field_114_timer <= static_cast<int>(gnFrameCount_507670) &&
                gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0) &&
                !Event_Get_417250(kEventResetting_6))
            {
                WakeUp();
                return 102;
            }
        }
    }

    if (Event_Get_417250(kEventSpeaking_1) || Event_Get_417250(kEvent_Alarm_17) || Event_Get_417250(kEvent_14))
    {
        if (pEvent != this &&
            field_114_timer <= static_cast<int>(gnFrameCount_507670) &&
            gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0) &&
            !Event_Get_417250(kEventResetting_6))
        {
            WakeUp();
            return 102;
        }
    }

    if (SwitchStates_Get(120) &&
        field_114_timer <= static_cast<int>(gnFrameCount_507670) &&
        gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0) &&
        !Event_Get_417250(kEventResetting_6))
    {
        WakeUp();
        return 102;
    }

    ShouldStilBeAlive_46C0D0();

    if (field_6_flags.Get(Options::eDead_Bit3))
    {
        Start_Slig_sounds_476960(gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos), 0);
    }
    return 102;
}

__int16 Slig::Brain_WakingUp_46B700()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        WaitOrWalk_46E440();
    }
    return 0;
}

__int16 Slig::Brain_Inactive_46B780()
{
    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
    {
        if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
        {
            if (!VOnSameYLevel(sControlledCharacter_50767C)
                || !VIsFacingMe(sControlledCharacter_50767C)
                || !VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(1), sControlledCharacter_50767C)
                || IsInInvisibleZone_418870(sControlledCharacter_50767C)
                || IsWallBetween_46BE60(this, sControlledCharacter_50767C)
                || Event_Get_417250(kEventResetting_6))
            {
                ShouldStilBeAlive_46C0D0();
            }
            else
            {
                ToShoot_46F1D0();
            }
        }
        else
        {
            ToAbeDead_466270();
        }
    }
    else
    {
        WaitOrWalk_46E440();
    }
    return 103;
}

__int16 Slig::Brain_Possessed_46C190()
{
    switch (field_10E_brain_state)
    {
    case 0:
        Slig_GameSpeak_SFX_46F560(SligSpeak::eHelp_10, 0, field_110_pitch_min, this);
        field_10E_brain_state = 1;
        field_100_health = FP_FromInteger(0);
        field_FC_current_motion = eSligStates::State_38_Possess_46B050;
        field_114_timer = gnFrameCount_507670 + 30;
        field_158_explode_timer = gnFrameCount_507670 + 1000;
        field_154_death_by_being_shot_timer = gnFrameCount_507670 + 1000;
        return field_10E_brain_state;

    case 1:
        if (static_cast<int>(gnFrameCount_507670) >= field_114_timer)
        {
            field_10E_brain_state = 2;
            field_114_timer = gnFrameCount_507670 + 20;
            field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
            return field_10E_brain_state;
        }
        break;

    case 2:
        if (static_cast<int>(gnFrameCount_507670) >= field_114_timer)
        {
            if (Math_NextRandom() & 1)
            {
                field_114_timer = gnFrameCount_507670 + 20;
                field_FC_current_motion = eSligStates::State_22_SpeakHi_467C90;
                Slig_GameSpeak_SFX_46F560(SligSpeak::eHi_0, 0, field_110_pitch_min, this);
            }
            else
            {
                field_114_timer = gnFrameCount_507670 + 20;
                field_FC_current_motion = eSligStates::State_25_SpeakLaugh_467ED0;
                Slig_GameSpeak_SFX_46F560(SligSpeak::eLaugh_7, 0, field_110_pitch_min, this);
            }
            field_10E_brain_state = 3;
            return field_10E_brain_state;
        }
        break;

    case 3:
        if (Event_Get_417250(kEventDeathReset_4))
        {
            if (sControlledCharacter_50767C != this)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }

        if (static_cast<int>(gnFrameCount_507670) >= field_114_timer)
        {
            field_10E_brain_state = 4;
            field_100_health = FP_FromInteger(1);
            return field_10E_brain_state;
        }
        break;

    case 4:
        if (Event_Get_417250(kEventDeathReset_4))
        {
            if (sControlledCharacter_50767C != this)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }

        if (field_100_health <= FP_FromInteger(0))
        {
            SetBrain(&Slig::Brain_Death_46C3A0);
            field_114_timer = gnFrameCount_507670 + 60;
            return field_10E_brain_state;
        }
        break;

    default:
        return field_10E_brain_state;
    }

    if (field_106_shot)
    {
        BlowToGibs_4685A0();
    }

    return field_10E_brain_state;
}

__int16 Slig::Brain_Death_46C3A0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render) && field_114_timer < static_cast<int>(gnFrameCount_507670) + 30)
    {
        field_BC_sprite_scale -= FP_FromDouble(0.01);
        field_C0_r -= 2;
        field_C2_g -= 2;
        field_C4_b -= 2;
        if (field_BC_sprite_scale >= FP_FromDouble(0.3) && !(static_cast<int>(gnFrameCount_507670) % 5))
        {
            New_Smoke_Particles_419A80(
                field_A8_xpos + (FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * field_BC_sprite_scale),
                field_AC_ypos - FP_FromInteger(6),
                field_BC_sprite_scale / FP_FromInteger(2),
                2,
                0);
            SFX_Play_43AE60(SoundEffect::Vaporize_96, 25, FP_GetExponent(FP_FromInteger(2200) * field_BC_sprite_scale));
        }
    }
    else if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (sControlledCharacter_50767C == this)
    {
        if (field_114_timer < static_cast<int>(gnFrameCount_507670))
        {
            if (field_14E_level != gMap_507BA8.field_0_current_level
                || field_150_path != gMap_507BA8.field_2_current_path
                || field_152_camera != gMap_507BA8.field_4_current_camera)
            {
                Event_Broadcast_417220(kEvent_7, this);
            }

            sControlledCharacter_50767C = sActiveHero_507678;
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
            gMap_507BA8.SetActiveCam_444660(
                field_14E_level,
                field_150_path,
                field_152_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }

    if (sControlledCharacter_50767C != this)
    {
        if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    if (field_BC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 116;
}

__int16 Slig::Brain_DeathDropDeath_46C5A0()
{
    switch (field_10E_brain_state)
    {
    case 0:
        Slig_GameSpeak_SFX_46F560(SligSpeak::eHelp_10, 0, field_110_pitch_min, this);
        field_114_timer = gnFrameCount_507670 + 60;
        return 1;

    case 1:
    {
        if (static_cast<int>(gnFrameCount_507670) < field_114_timer)
        {
            if (!((field_114_timer - gnFrameCount_507670) % 15))
            {
                Slig_GameSpeak_SFX_46F560(
                    SligSpeak::eHelp_10,
                    static_cast<short>(2 * ((field_114_timer & 0xFFFF) - gnFrameCount_507670)),
                    field_110_pitch_min,
                    this);
            }

            if (static_cast<int>(gnFrameCount_507670) == (field_114_timer - 6))
            {
                SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 65, 65);
            }
            return field_10E_brain_state;
        }

        Environment_SFX_42A220(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 32767, this);

        auto pScreenShake = ao_new<ScreenShake>();
        if (pScreenShake)
        {
            pScreenShake->ctor_4624D0(0);
        }
        field_114_timer = gnFrameCount_507670 + 30;
        return 2;
    }

    case 2:
        if (static_cast<int>(gnFrameCount_507670) > field_114_timer)
        {
            if (sControlledCharacter_50767C == this)
            {
                MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
                sControlledCharacter_50767C = sActiveHero_507678;
                gMap_507BA8.SetActiveCam_444660(field_14E_level, field_150_path, field_152_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        return field_10E_brain_state;

    default:
        return field_10E_brain_state;
    }
}

__int16 Slig::Brain_ReturnControlToAbeAndDie_46C760()
{
    if (sControlledCharacter_50767C == this)
    {
        sControlledCharacter_50767C = sActiveHero_507678;
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
        gMap_507BA8.SetActiveCam_444660(
            field_14E_level,
            field_150_path,
            field_152_camera,
            CameraSwapEffects::eEffect0_InstantChange,
            0,
            0);
    }
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    return 117;
}

__int16 Slig::Brain_PanicTurning_46C7C0()
{
    if (Event_Get_417250(kEventDeathReset_4)
        && !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return 107;
    }
    if (field_FC_current_motion != eSligStates::State_5_TurnAroundStanding_469C80
        || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
            return 107;
        }
        if (field_10_anim.field_92_current_frame != 4)
        {
            ShouldStilBeAlive_46C0D0();
            return 107;
        }
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (sControlledCharacter_50767C->field_B4_velx >= FP_FromInteger(0) &&
                (sControlledCharacter_50767C->field_B4_velx != FP_FromInteger(0) || sControlledCharacter_50767C->field_A8_xpos >= field_A8_xpos))
            {
                ShouldStilBeAlive_46C0D0();
                return 107;
            }
        }
        else
        {
            if (sControlledCharacter_50767C->field_B4_velx <= FP_FromInteger(0) &&
                (sControlledCharacter_50767C->field_B4_velx != FP_FromInteger(0) || sControlledCharacter_50767C->field_A8_xpos <= field_A8_xpos))
            {
                ShouldStilBeAlive_46C0D0();
                return 107;
            }
        }

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        PSX_RECT charRect = {};
        sControlledCharacter_50767C->VGetBoundingRect(&charRect, 1);

        if (sControlledCharacter_50767C->field_4_typeId != Types::eSlig_88 &&
            !IsInInvisibleZone_418870(sControlledCharacter_50767C))
        {
            if (charRect.x <= bRect.w &&
                charRect.w >= bRect.x &&
                charRect.h >= bRect.y &&
                charRect.y <= bRect.h)
            {
                field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                return 107;
            }
        }
        ShouldStilBeAlive_46C0D0();
        return 107;
    }

    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
    {
        field_FE_next_state = eSligStates::State_4_Running_469690;
        SetBrain(&Slig::Brain_PanicRunning_46CA20);
        Brain_PanicRunning_46CA20();
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
    }
    else
    {
        WaitOrWalk_46E440();
    }
    return 107;
}

__int16 Slig::Brain_PanicRunning_46CA20()
{
    if ((field_B4_velx > FP_FromInteger(4) &&
        (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)) + field_A8_xpos >
        FP_FromInteger(field_13C_zone_rect.w)) ||
        (field_B4_velx < FP_FromInteger(-4) &&
            (field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4))) < FP_FromInteger(field_13C_zone_rect.x))
        || HandleEnemyStopper_46BF30(4))
    {
        field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
        SetBrain(&Slig::Brain_PanicTurning_46C7C0);
    }
    else if (field_FC_current_motion || field_FE_next_state != -1)
    {
        if (VOnSameYLevel(sControlledCharacter_50767C)
            && VIsFacingMe(sControlledCharacter_50767C)
            && !IsInInvisibleZone_418870(sControlledCharacter_50767C)
            && !IsWallBetween_46BE60(this, sControlledCharacter_50767C)
            && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0)
            && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0)
            && !Event_Get_417250(kEventResetting_6))
        {
            ToShoot_46F1D0();
        }
        else if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
        {
            if (field_114_timer > static_cast<int>(gnFrameCount_507670))
            {
                ShouldStilBeAlive_46C0D0();
            }
            else
            {
                WaitOrWalk_46E440();
            }
        }
        else
        {
            ToAbeDead_466270();
        }
    }
    else
    {
        field_FE_next_state = eSligStates::State_4_Running_469690;
        SetBrain(&Slig::Brain_PanicRunning_46CA20);
        Brain_PanicRunning_46CA20();
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eSlogChase_5, this, 0, 0);
    }
    return 109;
}

__int16 Slig::Brain_PanicYelling_46CC50()
{
    if (field_FC_current_motion == eSligStates::State_29_SpeakPanic_4681D0
        && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Event_Broadcast_417220(kEvent_Alarm_17, this);

        const bool bFlipX = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
        field_114_timer = gnFrameCount_507670 + field_174_tlv.field_36_panic_timeout;
        if ((!bFlipX
            || field_A8_xpos >= FP_FromInteger((field_13C_zone_rect.x  + field_13C_zone_rect.w)
            / 2))
            && (bFlipX
                || field_A8_xpos <= FP_FromInteger((field_13C_zone_rect.x + field_13C_zone_rect.w)
                / 2)))
        {
            ToPanicRunning_46CC20();
        }
        else
        {
            ToPanicTurn();
        }
    }
    else
    {
        ShouldStilBeAlive_46C0D0();
    }
    return 115;
}

__int16 Slig::Brain_Chasing_46CD60()
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0)
        && VOnSameYLevel(sControlledCharacter_50767C)
        && VIsFacingMe(sControlledCharacter_50767C)
        && !IsInInvisibleZone_418870(sControlledCharacter_50767C)
        && !IsWallBetween_46BE60(this, sControlledCharacter_50767C)
        && !RenderLayerIs_46C0A0(sControlledCharacter_50767C)
        && !Event_Get_417250(kEventResetting_6))
    {
        field_20C_force_alive_state = 0;
        RespondToEnemyOrPatrol_465DF0();
        return 118;
    }

    if (HandleEnemyStopper_46BF30(4))
    {
        field_FE_next_state = eSligStates::State_0_StandIdle_467640;
        SetBrain(&Slig::Brain_Idle_46D6E0);
        field_114_timer = gnFrameCount_507670 + 1;
        return 118;
    }

    if (field_B0_path_number != gMap_507BA8.field_2_current_path
        || field_B2_lvl_number != gMap_507BA8.field_0_current_level
        || Event_Get_417250(kEventDeathReset_4)
        && !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        SetBrain(&Slig::Brain_StopChasing_46CF20);
        field_114_timer = gnFrameCount_507670 + field_174_tlv.field_3C_stop_chase_delay;
    }
    return 118;
}

__int16 Slig::Brain_StopChasing_46CF20()
{
    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
    {
        if (HandleEnemyStopper_46BF30(4))
        {
            field_FE_next_state = eSligStates::State_0_StandIdle_467640;
            SetBrain(&Slig::Brain_Idle_46D6E0);
            field_114_timer = gnFrameCount_507670 + 1;
        }
    }
    else
    {
        field_20C_force_alive_state = 0;
        field_11C = gMap_507BA8.field_4_current_camera;
        ToTurn_46DE70();
    }
    return 119;
}

__int16 Slig::Brain_StartChasing_46CF90()
{
    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
    {
        if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            field_20C_force_alive_state = 0;
            ToShoot_46F1D0();
        }
    }
    else
    {
        if (field_B0_path_number != gMap_507BA8.field_2_current_path
            || field_B2_lvl_number != gMap_507BA8.field_0_current_level)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        if (!VIsFacingMe(sControlledCharacter_50767C))
        {
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }

        field_20C_force_alive_state = 1;
        field_FE_next_state = eSligStates::State_4_Running_469690;
        SetBrain(&Slig::Brain_Chasing_46CD60);
        field_114_timer = field_174_tlv.field_1C_pause_time;
    }
    return 122;
}

__int16 Slig::Brain_Idle_46D6E0()
{
    if (Event_Get_417250(kEventAbeOhm_8) && !Event_Get_417250(kEventResetting_6))
    {
        ToPanic_46CD40();
        return 104;
    }

    if (VOnSameYLevel(sControlledCharacter_50767C)
        && VIsFacingMe(sControlledCharacter_50767C)
        && !IsInInvisibleZone_418870(sControlledCharacter_50767C)
        && !IsWallBetween_46BE60(this, sControlledCharacter_50767C)
        && (!field_20E_spotted_possessed_slig
        || sControlledCharacter_50767C->field_4_typeId != Types::eSlig_88)
        && !IsAbeEnteringDoor_46BEE0(sControlledCharacter_50767C)
        && !Event_Get_417250(kEventResetting_6)
        && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0
        ))
    {
        Slig::RespondToEnemyOrPatrol_465DF0();
        return 104;
    }
    if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
    {
        ToAbeDead_466270();
        return 104;
    }
    auto pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSuspiciousNoise_10));

    if(!pEvent)
    {
        pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSpeaking_1));
    }

    if (!pEvent ||
        pEvent->field_BC_sprite_scale != field_BC_sprite_scale ||
        pEvent == this ||
        !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0) ||
        Event_Get_417250(kEventResetting_6))
    {
        if (sControlledCharacter_50767C->field_BC_sprite_scale > field_BC_sprite_scale
            && (sControlledCharacter_50767C == sActiveHero_507678
            || sControlledCharacter_50767C == gElum_507680))
        {
            if (VIsFacingMe_4655B0(sControlledCharacter_50767C) &&
                !IsInInvisibleZone_418870(sControlledCharacter_50767C) &&
                gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0) &&
                !IsInZCover_46BDA0(static_cast<Slig*>(sControlledCharacter_50767C)) &&
                !IsInZCover_46BDA0(this) &&
                !Event_Get_417250(kEventResetting_6))
            {
                ToZShoot_46F200();
                return 104;
            }
        }
        if (sControlledCharacter_50767C->field_4_typeId == Types::eSlig_88)
        {
            if (field_FC_current_motion)
            {
                ShouldStilBeAlive_46C0D0();
                return 104;
            }
            if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eUnknown_29)
            {
                auto pTlv = static_cast<Path_Switch*>(gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::Switch_26
                ));
                if (pTlv)
                {
                    FP kScaleGrid = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                    if (FP_FromInteger(FP_GetExponent(field_A8_xpos) - pTlv->field_10_top_left.field_0_x) < kScaleGrid &&
                        !(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ||
                        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x - FP_GetExponent(field_A8_xpos)) < kScaleGrid &&
                        field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        auto pSwitch = static_cast<Switch*>(FindObjectOfType_418280(
                            Types::eLever_97,
                            field_A8_xpos + kScaleGrid,
                            field_AC_ypos - FP_FromInteger(5)
                        ));
                        if (pSwitch)
                        {
                            pSwitch->VPull(field_A8_xpos < pSwitch->field_A8_xpos ? TRUE : FALSE);
                        }
                        field_FE_next_state = eSligStates::State_46_PullLever_46A590;
                        return 104;
                    }
                }
            }
        }
        if (field_FC_current_motion == eSligStates::State_0_StandIdle_467640 &&
            field_114_timer <= static_cast<int>(gnFrameCount_507670))
        {
            field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
            SetBrain(&Slig::Brain_Turning_46DC70);
            return 104;
        }
        ShouldStilBeAlive_46C0D0();
        return 104;
    }
    if (pEvent != sControlledCharacter_50767C)
    {
        if (Math_NextRandom() >= 192u)
        {
            return 104;
        }
    }
    if (VIsFacingMe(sControlledCharacter_50767C))
    {
        if (!Event_Get_417250(kEventSpeaking_1) ||
            IsInInvisibleZone_418870(sControlledCharacter_50767C))
        {
            field_FE_next_state = eSligStates::State_0_StandIdle_467640;
            field_258_next_gamespeak_motion = eSligStates::State_30_SpeakWhat_468290;
            SetBrain(&Slig::Brain_GetAlerted_46E800);
            field_114_timer = static_cast<int>(gnFrameCount_507670) + field_174_tlv.field_42_listen_time;
            return 104;
        }
        GameSpeakResponse_46ED60();
        field_114_timer = static_cast<int>(gnFrameCount_507670) + 20;
    }
    else
    {
        if (!Event_Get_417250(kEventSpeaking_1) ||
            IsInInvisibleZone_418870(sControlledCharacter_50767C))
        {
            field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
            SetBrain(&Slig::Brain_GetAlertedTurn_46E520);
            return 104;
        }
        GameSpeakResponse_46ED60();
        field_114_timer = static_cast<int>(gnFrameCount_507670) + 20;
    }
    if (!VIsFacingMe(sControlledCharacter_50767C))
    {
        field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
    }
    SetBrain(&Slig::Brain_Discussion_46ECE0);
    return 104;
}

__int16 Slig::Brain_Turning_46DC70()
{
    if (Event_Get_417250(kEventDeathReset_4)
        && !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_6_flags.Set(Options::eDead_Bit3);
        return 106;
    }
    if (field_FC_current_motion == eSligStates::State_5_TurnAroundStanding_469C80
        && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        WaitOrWalk_46E440();
        return 106;
    }
    if (field_10_anim.field_92_current_frame == 4)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (sControlledCharacter_50767C->field_B4_velx >= FP_FromInteger(0) &&
                (sControlledCharacter_50767C->field_B4_velx != FP_FromInteger(0) || sControlledCharacter_50767C->field_A8_xpos >= field_A8_xpos))
            {
                ShouldStilBeAlive_46C0D0();
                return 106;
            }
        }
        else
        {
            if (sControlledCharacter_50767C->field_B4_velx <= FP_FromInteger(0) &&
                (sControlledCharacter_50767C->field_B4_velx != FP_FromInteger(0) || sControlledCharacter_50767C->field_A8_xpos <= field_A8_xpos))
            {
                ShouldStilBeAlive_46C0D0();
                return 106;
            }
        }

        PSX_RECT hitRect = {};
        VGetBoundingRect(&hitRect, 1);

        if (!Slig::IsInInvisibleZone_418870(sControlledCharacter_50767C)
            && !Slig::IsWallBetween_46BE60(this, sControlledCharacter_50767C))
        {
            PSX_RECT bRect = {};
            sControlledCharacter_50767C->VGetBoundingRect(&bRect, 1);

            if (sControlledCharacter_50767C->field_100_health > FP_FromInteger(0) &&
                PSX_Rects_overlap_no_adjustment(&hitRect, &bRect) &&
                sControlledCharacter_50767C->field_4_typeId != Types::eSlig_88)
            {
                field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                return 106;
            }
        }
    }

    ShouldStilBeAlive_46C0D0();
    return 106;
}

__int16 Slig::Brain_Walking_46DE90()
{
    if (dword_9F11BC)
    {
        ToPanicRunning_46CC20();
        return 108;
    }

    FP kScaleGrid = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    FP kScaleGrid2 = kScaleGrid * FP_FromInteger(2);
    if (field_B4_velx > FP_FromInteger(0))
    {
        if ((kScaleGrid2 + field_A8_xpos) >= FP_FromInteger(field_13C_zone_rect.w))
        {
            PauseALittle_46DBD0();
            return 108;
        }
    }
    if (field_B4_velx < FP_FromInteger(0))
    {
        if ((field_A8_xpos - kScaleGrid2) <= FP_FromInteger(field_13C_zone_rect.x))
        {
            PauseALittle_46DBD0();
            return 108;
        }
    }
    if (field_B4_velx == FP_FromInteger(0) &&
        field_FC_current_motion == eSligStates::State_0_StandIdle_467640 &&
        field_FE_next_state != eSligStates::State_2_Walking_469130)
    {
        PauseALittle_46DBD0();
        return 108;
    }
    if (HandleEnemyStopper_46BF30(2))
    {
        PauseALittle_46DBD0();
        return 108;
    }

    if (VOnSameYLevel(sControlledCharacter_50767C) &&
        VIsFacingMe(sControlledCharacter_50767C) &&
        !IsInInvisibleZone_418870(sControlledCharacter_50767C) &&
        !IsWallBetween_46BE60(this, sControlledCharacter_50767C) &&
        gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0
        ))
    {
        if (!field_20E_spotted_possessed_slig
            || sControlledCharacter_50767C->field_4_typeId != Types::eSlig_88)
        {
            if (!IsAbeEnteringDoor_46BEE0(sControlledCharacter_50767C) && !Event_Get_417250(kEventResetting_6))
            {
                RespondToEnemyOrPatrol_465DF0();
                return 108;
            }
        }
    }

    if (VOnSameYLevel(sControlledCharacter_50767C) &&
        VIsFacingMe(sControlledCharacter_50767C) &&
        !IsWallBetween_46BE60(this, sControlledCharacter_50767C) &&
        Event_Get_417250(kEventAbeOhm_8) &&
        gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0
        ) &&
        !Event_Get_417250(kEventResetting_6))
    {
        ToShoot_46F1D0();
        return 108;
    }
    if (Event_Get_417250(kEventAbeOhm_8) && !Event_Get_417250(kEventResetting_6))
    {
        ToPanic_46CD40();
        return 108;
    }
    if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
    {
        ToAbeDead_466270();
        return 108;
    }
    auto pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSuspiciousNoise_10));
    if (!pEvent)
    {
        pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSpeaking_1));
    }
    if (pEvent &&
        pEvent->field_BC_sprite_scale == field_BC_sprite_scale &&
        pEvent != this &&
        gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0
        ) &&
        !Event_Get_417250(kEventResetting_6))
    {
        if (VIsFacingMe(sControlledCharacter_50767C))
        {
            if (!Event_Get_417250(kEventSpeaking_1) || IsInInvisibleZone_418870(sControlledCharacter_50767C))
            {
                field_FE_next_state = eSligStates::State_0_StandIdle_467640;
                field_258_next_gamespeak_motion = eSligStates::State_30_SpeakWhat_468290;
                SetBrain(&Slig::Brain_GetAlerted_46E800);
                field_114_timer = static_cast<int>(gnFrameCount_507670) + field_174_tlv.field_42_listen_time;
                return 108;
            }
        }
        else
        {
            if (!Event_Get_417250(kEventSpeaking_1) || IsInInvisibleZone_418870(sControlledCharacter_50767C))
            {
                field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
                SetBrain(&Slig::Brain_GetAlertedTurn_46E520);
                return 108;
            }
        }
        GameSpeakResponse_46ED60();
        field_114_timer = gnFrameCount_507670 + 20;
        if (!VIsFacingMe(sControlledCharacter_50767C))
        {
            field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
        }
        SetBrain(&Slig::Brain_Discussion_46ECE0);
        return 108;
    }
    else if (sControlledCharacter_50767C->field_BC_sprite_scale <= field_BC_sprite_scale
        || sControlledCharacter_50767C != sActiveHero_507678
        && sControlledCharacter_50767C != gElum_507680
        || !VIsFacingMe(sControlledCharacter_50767C)
        || IsInInvisibleZone_418870(sControlledCharacter_50767C)
        || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0)
        || IsInZCover_46BDA0(sControlledCharacter_50767C)
        || IsInZCover_46BDA0(this)
        || Event_Get_417250(kEventResetting_6))
    {
        if (Math_NextRandom() < field_174_tlv.field_46_percent_beat_mud
            && FindBeatTarget_46D0E0(52, 2))
        {
            field_FE_next_state = eSligStates::State_0_StandIdle_467640;
            SetBrain(&Slig::Brain_StoppingNextToMudokon_46EBB0);
            field_114_timer = static_cast<int>(gnFrameCount_507670) + 30;
            return 108;
        }
        else
        {
            ShouldStilBeAlive_46C0D0();
            return 108;
        }
    }
    else
    {
        ToZShoot_46F200();
        return 108;
    }
}

__int16 Slig::Brain_GetAlertedTurn_46E520()
{
    if (field_FC_current_motion == eSligStates::State_5_TurnAroundStanding_469C80 &&
        field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!Event_Get_417250(kEventSpeaking_1) || IsInInvisibleZone_418870(sControlledCharacter_50767C))
        {
            field_FE_next_state = eSligStates::State_0_StandIdle_467640;
            field_258_next_gamespeak_motion = eSligStates::State_30_SpeakWhat_468290;
            SetBrain(&Slig::Brain_GetAlerted_46E800);
            field_114_timer = gnFrameCount_507670 + field_174_tlv.field_42_listen_time;
        }
        else
        {
            GameSpeakResponse_46ED60();
            field_114_timer = gnFrameCount_507670 + 20;

            if (!VIsFacingMe(sControlledCharacter_50767C))
            {
                field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
            }

            SetBrain(&Slig::Brain_Discussion_46ECE0);
        }

        return 123;
    }

    if (field_10_anim.field_92_current_frame == 4)
    {
        bool tryTurningToPlayer = true;
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (sControlledCharacter_50767C->field_B4_velx >= FP_FromInteger(0) &&
                (sControlledCharacter_50767C->field_B4_velx > FP_FromInteger(0) || sControlledCharacter_50767C->field_A8_xpos >= field_A8_xpos))
            {
                tryTurningToPlayer = false;
            }
        }
        else if (sControlledCharacter_50767C->field_B4_velx <= FP_FromInteger(0) &&
            (sControlledCharacter_50767C->field_B4_velx > FP_FromInteger(0) || sControlledCharacter_50767C->field_A8_xpos <= field_A8_xpos))
        {
            tryTurningToPlayer = false;
        }

        if (tryTurningToPlayer)
        {
            if (!IsInInvisibleZone_418870(sControlledCharacter_50767C))
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                PSX_RECT bRectChar = {};
                sControlledCharacter_50767C->VGetBoundingRect(&bRectChar, 1);

                if (PSX_Rects_overlap_no_adjustment(&bRectChar, &bRect))
                {
                    field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                    return 123;
                }
            }
        }
    }

    if (field_FC_current_motion != eSligStates::State_0_StandIdle_467640)
    {
        ShouldStilBeAlive_46C0D0();
    }
    else
    {
        field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
    }
    return 123;
}

__int16 Slig::Brain_GetAlerted_46E800()
{
    if (field_114_timer == field_174_tlv.field_42_listen_time + static_cast<int>(gnFrameCount_507670) - 2 &&
        Math_RandomRange_450F20(0, 100) < field_174_tlv.field_44_percent_say_what)
    {
        field_FE_next_state = eSligStates::State_30_SpeakWhat_468290;
    }

    if (!VOnSameYLevel(sControlledCharacter_50767C)
        || !VIsFacingMe(sControlledCharacter_50767C)
        || IsInInvisibleZone_418870(sControlledCharacter_50767C)
        || IsWallBetween_46BE60(this, sControlledCharacter_50767C)
        || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0)
        || sControlledCharacter_50767C->field_4_typeId == Types::eSlig_88 && field_20E_spotted_possessed_slig
        || IsAbeEnteringDoor_46BEE0(sControlledCharacter_50767C)
        || Event_Get_417250(kEventResetting_6))
    {
        if (!Event_Get_417250(kEventAbeOhm_8) || Event_Get_417250(kEventResetting_6))
        {
            BaseAliveGameObject* pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSuspiciousNoise_10));
            if (!pEvent)
            {
                pEvent = static_cast<BaseAliveGameObject*>(Event_Get_417250(kEventSpeaking_1));
            }

            if (pEvent && (pEvent == sControlledCharacter_50767C || pEvent->field_4_typeId == Types::eMudokon_75)
                && VOnSameYLevel(pEvent)
                && VIsFacingMe(pEvent)
                && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0)
                && !Event_Get_417250(kEventResetting_6))
            {
                ToShoot_46F1D0();
            }
            else
            {
                if (pEvent && (pEvent == sControlledCharacter_50767C || pEvent->field_4_typeId != Types::eSlig_88)
                    && !VIsFacingMe(pEvent)
                    && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0)
                    && !Event_Get_417250(kEventResetting_6))
                {
                    if (!Event_Get_417250(kEventSpeaking_1) || IsInInvisibleZone_418870(sControlledCharacter_50767C))
                    {
                        field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
                        SetBrain(&Slig::Brain_GetAlertedTurn_46E520);
                    }
                    else
                    {
                        GameSpeakResponse_46ED60();
                        field_114_timer = gnFrameCount_507670 + 20;
                        if (!VIsFacingMe(sControlledCharacter_50767C))
                        {
                            field_FE_next_state = eSligStates::State_5_TurnAroundStanding_469C80;
                        }

                        SetBrain(&Slig::Brain_Discussion_46ECE0);
                    }
                }
                else
                {
                    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
                    {
                        ShouldStilBeAlive_46C0D0();
                    }
                    else
                    {
                        WaitOrWalk_46E440();
                    }
                }
            }
        }
        else
        {
            ToPanic_46CD40();
        }
    }
    else
    {
        RespondToEnemyOrPatrol_465DF0();
    }
    return 124;
}

__int16 Slig::Brain_StoppingNextToMudokon_46EBB0()
{
    if (field_FC_current_motion != eSligStates::State_0_StandIdle_467640)
    {
        return 128;
    }

    //TODO OG BUG: Sligs beat up dead muds, fix from AE:
    //BaseAliveGameObject* pBeatTarget = FindBeatTarget_4BD070(static_cast<int>(Types::eMudokon2_81), 1);
    //if (!pBeatTarget || pBeatTarget->field_10C_health <= FP_FromInteger(0))
    if (!FindBeatTarget_46D0E0(static_cast<int>(Types::eMudokon_52), 1))
    {
        WaitOrWalk_46E440();
        return 128;
    }

    if (Math_NextRandom() >= 100u)
    {
        if (Math_NextRandom() & 1)
        {
            field_FE_next_state = eSligStates::State_26_SpeakBullshit1_467F90;
        }
        else
        {
            field_FE_next_state = eSligStates::State_28_SpeakBullshit2_468110;
        }
    }
    else
    {
        field_FE_next_state = eSligStates::State_52_Beat_46AA90;
        SetBrain(&Slig::Brain_BeatingUp_46EC40);
    }

    return 128;
}

__int16 Slig::Brain_BeatingUp_46EC40()
{
    if (field_FC_current_motion != eSligStates::State_0_StandIdle_467640 || field_114_timer >= static_cast<int>(gnFrameCount_507670))
    {
        return 129;
    }

    if (Math_NextRandom() < 100u)
    {
        field_FE_next_state = eSligStates::State_52_Beat_46AA90;
        return 129;
    }

    if (Math_NextRandom() < 64u)
    {
        field_FE_next_state = eSligStates::State_26_SpeakBullshit1_467F90;
        return 129;
    }

    if (Math_NextRandom() < 64u)
    {
        field_FE_next_state = eSligStates::State_28_SpeakBullshit2_468110;
        return 129;
    }
    
    WaitOrWalk_46E440();
    return 129;
}

__int16 Slig::Brain_Discussion_46ECE0()
{
    if (field_FC_current_motion == eSligStates::State_0_StandIdle_467640 && field_114_timer == static_cast<int>(gnFrameCount_507670))
    {
        field_FE_next_state = static_cast<short>(field_258_next_gamespeak_motion);
    }

    if (field_114_timer < static_cast<int>(gnFrameCount_507670) - 5)
    {
        WaitOrWalk_46E440();
    }

    return 128;
}

__int16 Slig::Brain_GameEnder_46EEE0()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_10E_brain_state == 0)
    {
        if (sNumCamSwappers_507668 > 0 || sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
        {
            return field_10E_brain_state;
        }
        field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
        field_114_timer = gnFrameCount_507670 + field_130;
        return 1;
    }
    else if (field_10E_brain_state == 1)
    {
        if (static_cast<int>(gnFrameCount_507670) < field_114_timer)
        {
             return field_10E_brain_state;
        }
        field_FE_next_state = eSligStates::State_4_Running_469690;
        return 2;
    }
    else
    {
        if (field_10E_brain_state == 2
            && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
        {
            return field_10E_brain_state;
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_100_health = FP_FromInteger(0);
        return field_10E_brain_state;
    }
}

__int16 Slig::Brain_Shooting_46EFD0()
{
    if (field_FC_current_motion == eSligStates::State_6_Shoot_468820
        && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_200_num_times_to_shoot++;

        if (field_200_num_times_to_shoot < field_174_tlv.field_2A_number_of_times_to_shoot
            || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_137_ElumUnmountBegin_42E2B0
            || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_139_ElumMountBegin_42E090
            || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_138_ElumUnmountEnd_42E390
            || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_136_ElumMountEnd_42E110)
        {
            field_FE_next_state = eSligStates::State_6_Shoot_468820;
            return 111;
        }

        if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
        {
            ToKilledAbe_4662E0();
            return 111;
        }

        if (sControlledCharacter_50767C->field_100_health <= FP_FromInteger(0))
        {
            ToKilledAbe_4662E0();
            return 111;
        }

        if (!VOnSameYLevel(sControlledCharacter_50767C)
            || !VIsFacingMe(sControlledCharacter_50767C)
            || IsInInvisibleZone_418870(sControlledCharacter_50767C)
            || IsWallBetween_46BE60(this, sControlledCharacter_50767C)
            || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0)
            || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0)
            || Event_Get_417250(kEventResetting_6))
        {
            PauseALittle_46DBD0();
            return 111;
        }

        if (!VIsFacingMe(sControlledCharacter_50767C))
        {
            ToTurn_46DE70();
            return 111;
        }

        if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0)
            && field_174_tlv.field_32_chase_abe)
        {
            ToChase_46D080();
            return 111;
        }
    }

    ShouldStilBeAlive_46C0D0();
    return 111;
}

__int16 Slig::Brain_ZSpottedEnemy_46F260()
{
    if (field_114_timer > static_cast<int>(gnFrameCount_507670))
    {
        return 126;
    }
    field_FE_next_state = eSligStates::State_43_ShootZ_468E30;
    SetBrain(&Slig::Brain_ZShooting_46F290);
    return 126;
}


__int16 Slig::Brain_ZShooting_46F290()
{
    if (field_FC_current_motion != eSligStates::State_43_ShootZ_468E30 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ShouldStilBeAlive_46C0D0();
        return 127;
    }

    field_200_num_times_to_shoot++;

    if (field_200_num_times_to_shoot < field_174_tlv.field_2A_number_of_times_to_shoot)
    {
        return 127;
    }

    if (sActiveHero_507678->field_FC_current_motion == eAbeStates::State_138_ElumUnmountEnd_42E390
        || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_136_ElumMountEnd_42E110
        || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_137_ElumUnmountBegin_42E2B0
        || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_139_ElumMountBegin_42E090)
    {
        return 127;
    }

    field_200_num_times_to_shoot = 0;

    if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
    {
        WaitOrWalk_46E440();
    }
    else
    {
        ToAbeDead_466270();
    }
    return 127;
}

void Slig::BlowToGibs_4685A0()
{
    auto pGibs = ao_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_407B20(
            1,
            field_A8_xpos,
            field_AC_ypos,
            field_B4_velx,
            field_B8_vely,
            field_BC_sprite_scale);
    }

    auto pBlood = ao_new<Blood>();
    if (pBlood)
    {
        pBlood->ctor_4072B0(
            field_A8_xpos,
            field_AC_ypos - (FP_FromInteger(30) * field_BC_sprite_scale),
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_BC_sprite_scale,
            20);
    }

    New_Smoke_Particles_419A80(
        field_A8_xpos,
        field_AC_ypos - (FP_FromInteger(30) * field_BC_sprite_scale),
        field_BC_sprite_scale,
        3,
        0);

    if (field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_43AD70(SoundEffect::KillEffect_78, 80, 0);
        SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 60, 0);
    }
    else
    {
        SFX_Play_43AD70(SoundEffect::KillEffect_78, 127, 0);
        SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 90, 0);
    }

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
    field_B8_vely = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_100_health = FP_FromInteger(0);
    MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
    field_114_timer = gnFrameCount_507670 + 60;
    field_8_update_delay = 40;
    SetBrain(&Slig::Brain_Death_46C3A0);
    Event_Broadcast_417220(kEventMudokonComfort_16, sActiveHero_507678);
}

void Slig::SetBrain(Slig::TBrainFn fn)
{
    ::SetBrain(fn, field_1F8_fn, sSligAITable);
}

void Slig::SetBrain2(Slig::TBrainFn fn)
{
    ::SetBrain(fn, field_1FC_fn2, sSligAITable);
}

bool Slig::BrainIs(Slig::TBrainFn fn)
{
    return ::BrainIs(fn, field_1F8_fn, sSligAITable);
}

}

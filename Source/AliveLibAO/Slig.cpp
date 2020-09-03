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
#include "DDCheat.hpp"
#include "Input.hpp"
#include "Particle.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Sfx.hpp"
#include "CameraSwapper.hpp"
#include "PlatformBase.hpp"
#include "LiftPoint.hpp"

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
    { &Slig::Brain_465EB0, 0x465EB0, "Brain_465EB0" },
    { &Slig::Brain_465EB0, 0x465EB0, "Brain_465EB0" },
    { &Slig::Brain_466030, 0x466030, "Brain_466030" },
    { &Slig::Brain_466190, 0x466190, "Brain_466190" },
    { &Slig::Brain_4662A0, 0x4662A0, "Brain_4662A0" },
    { &Slig::Brain_46B250, 0x46B250, "Brain_46B250" },
    { &Slig::Brain_46B4E0, 0x46B4E0, "Brain_46B4E0" },
    { &Slig::Brain_46B700, 0x46B700, "Brain_46B700" },
    { &Slig::Brain_46B780, 0x46B780, "Brain_46B780" },
    { &Slig::Brain_46C190, 0x46C190, "Brain_46C190" },
    { &Slig::Brain_46C3A0, 0x46C3A0, "Brain_46C3A0" },
    { &Slig::Brain_46C5A0, 0x46C5A0, "Brain_46C5A0" },
    { &Slig::Brain_46C760, 0x46C760, "Brain_46C760" },
    { &Slig::Brain_46C7C0, 0x46C7C0, "Brain_46C7C0" },
    { &Slig::Brain_46CA20, 0x46CA20, "Brain_46CA20" },
    { &Slig::Brain_46CC50, 0x46CC50, "Brain_46CC50" },
    { &Slig::Brain_46CD60, 0x46CD60, "Brain_46CD60" },
    { &Slig::Brain_46CF20, 0x46CF20, "Brain_46CF20" },
    { &Slig::Brain_46CF90, 0x46CF90, "Brain_46CF90" },
    { &Slig::Brain_46D6E0, 0x46D6E0, "Brain_46D6E0" },
    { &Slig::Brain_46DC70, 0x46DC70, "Brain_46DC70" },
    { &Slig::Brain_46DE90, 0x46DE90, "Brain_46DE90" },
    { &Slig::Brain_46E520, 0x46E520, "Brain_46E520" },
    { &Slig::Brain_46E800, 0x46E800, "Brain_46E800" },
    { &Slig::Brain_46EBB0, 0x46EBB0, "Brain_46EBB0" },
    { &Slig::Brain_46EC40, 0x46EC40, "Brain_46EC40" },
    { &Slig::Brain_46ECE0, 0x46ECE0, "Brain_46ECE0" },
    { &Slig::Brain_46EEE0, 0x46EEE0, "Brain_46EEE0" },
    { &Slig::Brain_46EFD0, 0x46EFD0, "Brain_46EFD0" },
    { &Slig::Brain_46F260, 0x46F260, "Brain_46F260" },
    { &Slig::Brain_46F290, 0x46F290, "Brain_46F290" },
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

    field_114_timer = 0;
    field_118 = 0;
    field_11C = -1;
    field_10C = 0;
    field_10E_brain_state = 0;
    field_FE_next_state = 0;
    field_EC = 3;
    field_158 = 0;
    field_154 = 0;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = eSligStates::State_7_Falling_46A1A0;
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
    field_138_res_idx = 0;

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
    NOT_IMPLEMENTED();
}

const unsigned int sSligVelXTable_4BCA30[] = { 262144, 262144, 0, 4294705152, 4294705152, 4294705152, 0, 262144 };
const unsigned int sSligVelYTable_4BCA50[] = { 0, 4294705152, 4294705152, 4294705152, 0, 262144, 262144, 262144 };


void Slig::VUpdate_465050()
{
    if (!Input_IsChanting_4334C0())
    {
        field_254 &= ~4u;
    }

    if (sControlledCharacter_50767C == this && field_100_health > FP_FromInteger(0))
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType6, this, 1, 0);
    }

    if (sDDCheat_FlyingEnabled_50771C && sControlledCharacter_50767C == this)
    {
        field_F4_pLine = nullptr;
        if (sInputObject_5009E8.isPressed(0xF000))
        {
            const int dir = sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_2 >> 5;
            field_B4_velx = FP_FromRaw(sSligVelXTable_4BCA30[dir]);
            field_B8_vely = FP_FromRaw(sSligVelYTable_4BCA50[dir]);

            if (sInputObject_5009E8.isPressed(0x20))
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
            LOG_INFO("Brain changed from " << GetOriginalFn(oldBrain, sSligAITable).fnName << " to " << GetOriginalFn(field_1F8_fn, sSligAITable).fnName);
        }

        if (field_106_shot)
        {
            Vsub_465C30();
        }

        if (word_5076E0)
        {
            DebugOut_495990(
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
            field_FC_current_motion = field_E4;
            
            VUpdateAnimData_464D00();

            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            field_11E = 0;
        }
    }
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

void Slig::Vsub_465C30()
{
    if (field_13A != -1)
    {
        field_FC_current_motion = field_13A;
    }

    field_FE_next_state = -1;
    field_13A = -1;
    field_106_shot = FALSE;
    field_114_timer = gnFrameCount_507670 + 60;
    SetBrain(&Slig::Brain_46C3A0);
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

BOOL Slig::VIs8_465630(short motion)
{
    return motion == eSligStates::State_8_Unknown_4673E0;
}

void Slig::CheckFloorGone_467490()
{
    NOT_IMPLEMENTED();
}

__int16 Slig::MainMovement_467020()
{
    NOT_IMPLEMENTED();
}

void Slig::ToKnockBack_467300()
{
    NOT_IMPLEMENTED();
}

void Slig::State_0_StandIdle_467640()
{
    NOT_IMPLEMENTED();
}

void Slig::State_1_StandToWalk_4695D0()
{
    field_126 |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSligStates::State_2_Walking_469130;
    }

    CheckFloorGone_467490();
}

void Slig::State_2_Walking_469130()
{
    NOT_IMPLEMENTED();
}

void Slig::State_3_StandToRun_469C00()
{
    field_126 |= sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSligStates::State_4_Running_469690;
    }

    Event_Broadcast_417220(kEvent_0, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(35), field_B4_velx))
    {
        ToKnockBack_467300();
    }
    else
    {
        CheckFloorGone_467490();
    }
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
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter_50767C != this || field_100_health <= FP_FromInteger(0))
        {
            field_FC_current_motion = field_E4;
            if (field_F8_pLiftPoint)
            {
                field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
                field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
            }
        }
        else
        {
            field_FC_current_motion = field_E4;
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
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_12C = 0;
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
        field_126 = 0;
        field_128_timer = Math_RandomRange_450F20(0, 60) + gnFrameCount_507670 + 120;
        MapFollowMe_401D30(1);
    }
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
            New_Particle_4198E0(
                field_A8_xpos + (field_BC_sprite_scale * FP_FromInteger(Math_RandomRange_450F20(-20, 20))),
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(Math_RandomRange_450F20(20, 50))),
                field_BC_sprite_scale,
                0);
        }

        if (static_cast<int>(gnFrameCount_507670) > field_128_timer)
        {
            BlowToGibs_4685A0();
        }

    }
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
    if (field_F4_pLine)
    {
        if (field_10_anim.field_92_current_frame == 4)
        {
            Abe_SFX_2_42A220(6u, 80, -200, this);
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


__int16 Slig::Brain_465EB0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_466030()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_466190()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_4662A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46B250()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46B4E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46B700()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46B780()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46C190()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46C3A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46C5A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46C760()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46C7C0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46CA20()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46CC50()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46CD60()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46CF20()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46CF90()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46D6E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46DC70()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46DE90()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46E520()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46E800()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46EBB0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46EC40()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46ECE0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46EEE0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46EFD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46F260()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Slig::Brain_46F290()
{
    NOT_IMPLEMENTED();
    return 0;
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

    New_Particles_419A80(
        field_A8_xpos,
        field_AC_ypos - (FP_FromInteger(30) * field_BC_sprite_scale),
        field_BC_sprite_scale,
        3,
        0);

    if (field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_43AD70(78u, 80, 0);
        SFX_Play_43AD70(53u, 60, 0);
    }
    else
    {
        SFX_Play_43AD70(78u, 127, 0);
        SFX_Play_43AD70(53u, 90, 0);
    }

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_FC_current_motion = eSligStates::State_0_StandIdle_467640;
    field_B8_vely = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_100_health = FP_FromInteger(0);
    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);
    field_114_timer = gnFrameCount_507670 + 60;
    field_8_update_delay = 40;
    SetBrain(&Slig::Brain_46C3A0);
    Event_Broadcast_417220(kEvent_16, sActiveHero_507678);
}

void Slig::SetBrain(Slig::TBrainFn fn)
{
    ::SetBrain(fn, field_1F8_fn, sSligAITable);
}

bool Slig::BrainIs(Slig::TBrainFn fn)
{
    return ::BrainIs(fn, field_1F8_fn, sSligAITable);
}

END_NS_AO

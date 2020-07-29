#include "stdafx_ao.h"
#include "Function.hpp"
#include "Paramite.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "ParamiteWeb.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "PlatformBase.hpp"

void Paramite_ForceLink() {}

START_NS_AO

using TParamiteStateFunction = decltype(&Paramite::State_0_Idle_44B900);

const TParamiteStateFunction sParamiteMotionTable_4CDCB0[] =
{
    &Paramite::State_0_Idle_44B900,
    &Paramite::State_1_WalkBegin_44BCA0,
    &Paramite::State_2_Walking_44B9E0,
    &Paramite::State_3_Running_44C070,
    &Paramite::State_4_Unknown_44B6C0,
    &Paramite::State_5_Turn_44C8E0,
    &Paramite::State_6_Hop_44CB20,
    &Paramite::State_7_Unknown_44BF10,
    &Paramite::State_8_WalkRunTransition_44C790,
    &Paramite::State_9_WalkEnd_44BDE0,
    &Paramite::State_10_RunBegin_44C4C0,
    &Paramite::State_11_RunEnd_44C620,
    &Paramite::State_12_Falling_44C960,
    &Paramite::State_13_GameSpeakBegin_44D050,
    &Paramite::State_14_PreHiss_44D170,
    &Paramite::State_15_Hiss_44D300,
    &Paramite::State_16_PostHiss_44D440,
    &Paramite::State_17_GameSpeakEnd_44D4F0,
    &Paramite::State_18_RunningAttack_44D5D0,
    &Paramite::State_19_Empty_44D990,
    &Paramite::State_20_SurpriseWeb_44D9A0,
    &Paramite::State_21_WebLeave_44DB00,
    &Paramite::State_22_Unknown_44D8F0,
    &Paramite::State_23_Eating_44B970,
    &Paramite::State_24_Struggle_44DB70,
    &Paramite::State_25_Death_44DB90,
};

Paramite* Paramite::ctor_44A7A0(Path_Paramite* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BBBE8);
    field_4_typeId = Types::eParamite_62;

    for (int i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        field_150_resources[i] = nullptr;
    }

    field_150_resources[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 600, 1, 0);
    field_150_resources[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 605, 1, 0);
    field_150_resources[14] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 614, 1, 0);
    field_150_resources[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 604, 1, 0);
    field_150_resources[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 609, 1, 0);
    field_150_resources[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 601, 1, 0);
    field_150_resources[15] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 615, 1, 0);
    field_150_resources[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 610, 1, 0);

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2034, 1, 0);

    Animation_Init_417FD0(
        57152,
        138,
        49,
        field_150_resources[0],
        1);

    field_12A = 0;


    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_114 = 0;
    field_10C_fn = &Paramite::Brain_447A10;
    field_110_state = 0;
    field_FE_next_state = 0;
    field_EC_oldY = 3;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
    field_140 = 0;

    field_148_pUnknown = nullptr;

    field_14C_pWeb = nullptr;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 8;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 27;
        field_C6_scale = 1;
    }

    if (pTlv->field_1A_entrance_type)
    {
        field_10C_fn = &Paramite::Brain_448D00;
    }

    field_11E_attack_delay = pTlv->field_1C_attack_delay;
    field_112_meat_eating_time = pTlv->field_1E_meat_eating_time;
    field_11C_attack_duration = pTlv->field_20_attack_duration;
    field_134_disabled_resources = pTlv->field_22_disabled_resources;
    field_13C_hiss_before_attack = pTlv->field_26_hiss_before_attack;
    field_13E_unload_when_far_away = pTlv->field_28_unload_when_far_away;

    field_144 = pTlv->field_2A_unknown;

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
        field_BC_sprite_scale != FP_FromInteger(0) ? 0x01 : 0x10) == 1)
    {
        field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
        field_AC_ypos = hitY;
    }

    field_C0_r = 105;
    field_C2_g = 105;
    field_C4_b = 105;

    field_128 = 0;
    field_12C = tlvInfo;

    if (!VIsFacingMe(sActiveHero_507678))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    VStackOnObjectsOfType_418930(Types::eParamite_62);

    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* Paramite::dtor_44AB00()
{
    SetVTable(this, 0x4BBBE8);

    if (field_14C_pWeb)
    {
        field_14C_pWeb->field_6_flags.Set(Options::eDead_Bit3);
        field_14C_pWeb->field_C_refCount--;
        field_14C_pWeb = nullptr;
    }

    if (field_148_pUnknown)
    {
        field_148_pUnknown->field_C_refCount--;
    }

    VOnTrapDoorOpen_44B8C0();

    for (int i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        if (field_10_anim.field_20_ppBlock != field_150_resources[i])
        {
            if (field_150_resources[i])
            {
                ResourceManager::FreeResource_455550(field_150_resources[i]);
            }
        }
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2034, 0, 0));
    if (field_100_health <= FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_12C, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_12C, -1, 0, 0);
    }

    SND_Seq_Stop_477A60(30u);
    
    MusicController::sub_443810(0, this, 0, 0);

    return dtor_401000();
}

BaseGameObject* Paramite::VDestructor(signed int flags)
{
    return Vdtor_44B300(flags);
}

Paramite* Paramite::Vdtor_44B300(signed int flags)
{
    dtor_44AB00();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Paramite::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_44B8C0();
}

void Paramite::VOnTrapDoorOpen_44B8C0()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
        field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
    }
}

void Paramite::VUpdate_Real_44A490()
{
    NOT_IMPLEMENTED();
}

void Paramite::VUpdate_44A490()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_44A490();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}


void Paramite::ToIdle_44B580()
{
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

    field_124 = 0;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    
    field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;

    MapFollowMe_401D30(1);
}

__int16 Paramite::sub_44B320()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::Brain_447A10()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::Brain_448D00()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Paramite::State_0_Idle_44B900()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_1_WalkBegin_44BCA0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_2_Walking_44B9E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_3_Running_44C070()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_4_Unknown_44B6C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_5_Turn_44C8E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_6_Hop_44CB20()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_7_Unknown_44BF10()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_8_WalkRunTransition_44C790()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_9_WalkEnd_44BDE0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_10_RunBegin_44C4C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_11_RunEnd_44C620()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_12_Falling_44C960()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_13_GameSpeakBegin_44D050()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_14_PreHiss_44D170()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_15_Hiss_44D300()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_16_PostHiss_44D440()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_17_GameSpeakEnd_44D4F0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_18_RunningAttack_44D5D0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_19_Empty_44D990()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_20_SurpriseWeb_44D9A0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_21_WebLeave_44DB00()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_22_Unknown_44D8F0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_23_Eating_44B970()
{
    if (field_10_anim.field_92_current_frame == 5)
    {
        SFX_Play_43AD70(static_cast<unsigned char>(Math_RandomRange_450F20(79, 80)), 0, 0);
    }

    if (field_FE_next_state != -1 && !sub_44B320())
    {
        ToIdle_44B580();
    }
}

void Paramite::State_24_Struggle_44DB70()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_25_Death_44DB90()
{
    NOT_IMPLEMENTED();
}

END_NS_AO


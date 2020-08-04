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
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "VRam.hpp"
#include "Meat.hpp"

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

const static int sParamiteFrameTables_4CDD18[] =
{
    57152,
    57132,
    57004,
    57068,
    57152,
    57304,
    57176,
    57340,
    57348,
    57368,
    57408,
    57388,
    57428,
    15860,
    15900,
    15916,
    15948,
    15980,
    19120,
    18084,
    18084,
    18116,
    10520,
    14728,
    11076,
    9132
};

static AIFunctionData<Paramite::TParamiteBrain> sParamiteAITable[]
{
    { &Paramite::Brain_447A10,  0x447A10, "Brain_447A10" },
    { &Paramite::Brain_448D00,  0x448D00, "Brain_448D00" },
    { &Paramite::Brain_44DD70,  0x44DD70, "Brain_44DD70" },
    { &Paramite::Brain_448BF0,  0x448BF0, "Brain_448BF0" },
    { &Paramite::Brain_449170,  0x449170, "Brain_449170" },
    { &Paramite::Brain_449CD0,  0x449CD0, "Brain_449CD0" },
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

    field_12A_res_idx = 0;


    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_114 = 0;
    SetBrain(&Paramite::Brain_447A10);
    field_110_state = 0;
    field_FE_next_state = 0;
    field_EC = 3;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
    field_140 = 0;

    field_148_pMeat = nullptr;

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
        SetBrain(&Paramite::Brain_448D00);
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

    if (field_148_pMeat)
    {
        field_148_pMeat->field_C_refCount--;
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
    
    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);

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

BYTE** Paramite::ResBlockForMotion_44AC10(__int16 motion)
{
    __int16 idx = 0;

    if (motion < eParamiteStates::State_13_GameSpeakBegin_44D050)
    {
        idx = 0;
    }
    else if (motion < eParamiteStates::State_18_RunningAttack_44D5D0)
    {
        idx = 5;
    }
    else if (motion < eParamiteStates::State_19_Empty_44D990)
    {
        idx = 14;
    }
    else if (motion < eParamiteStates::State_22_Unknown_44D8F0)
    {
        idx = 4;
    }
    else if (motion < eParamiteStates::State_23_Eating_44B970)
    {
        idx = 9;
    }
    else if (motion < eParamiteStates::State_24_Struggle_44DB70)
    {
        idx = 1;
    }
    else if (motion >= eParamiteStates::State_25_Death_44DB90)
    {
        idx = motion >= 26 ? 0 : 10;
    }
    else
    {
        idx = 15;
    }
    field_12A_res_idx = idx;
    return field_150_resources[idx];
}

void Paramite::VRender(int** pOrderingTable)
{
    VRender_44ACA0(pOrderingTable);
}

void Paramite::VRender_44ACA0(int** ppOt)
{
    if (field_8_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender_417DA0(ppOt);
    }
}

__int16 Paramite::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_44ACC0(pFrom);
}

__int16 Paramite::VTakeDamage_44ACC0(BaseGameObject* pFrom)
{
    if (field_100_health <= FP_FromInteger(0))
    {
        return 1;
    }

    if (field_148_pMeat)
    {
        field_148_pMeat->field_C_refCount--;
        field_148_pMeat = nullptr;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eBaseBomb_30:
    case Types::eExplosion_74:
    {
        auto pGibs = ao_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_407B20(
                2,
                field_A8_xpos,
                field_AC_ypos,
                field_B4_velx,
                field_B8_vely,
                field_BC_sprite_scale);
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_100_health = FP_FromInteger(0);
        return 1;
    }

    case Types::eAbilityRing_69:
        return 0;

    case Types::eBeeSwarm_95:
        field_100_health -= FP_FromDouble(0.2);
        if (field_100_health > FP_FromInteger(0))
        {
            if (!BrainIs(&Paramite::Brain_44DD70))
            {
                field_FE_next_state = eParamiteStates::State_25_Death_44DB90;
                SetBrain(&Paramite::Brain_44DD70);
                field_110_state = 0;
            }
        }
        else
        {
            field_100_health = FP_FromInteger(0);
            SetBrain(&Paramite::Brain_448BF0);
            field_128 = 2;
            field_114 = gnFrameCount_507670 + 90;
            field_FC_current_motion = eParamiteStates::State_24_Struggle_44DB70;
            VUpdateAnimData_44A460();
        }
        break;

    default:
    {
        field_100_health = FP_FromInteger(0);
        SetBrain(&Paramite::Brain_448BF0);
        field_128 = 2;
        field_114 = gnFrameCount_507670 + 90;
        field_FC_current_motion = eParamiteStates::State_24_Struggle_44DB70;
        VUpdateAnimData_44A460();

        auto pBlood = ao_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_4072B0(
                field_A8_xpos,
                field_AC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(5),
                field_BC_sprite_scale,
                50);
        }
        break;
    }
    }
    return 1;
}

void Paramite::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOn_Tlv_Collision_44AF30(pTlv);
}


void Paramite::VOn_Tlv_Collision_44AF30(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_100_health = FP_FromInteger(0);
            field_128 = 2;
        }
        pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
    }
}

void Paramite::VScreenChanged()
{
    VScreenChanged_44B2C0();
}

void Paramite::VScreenChanged_44B2C0()
{
    if (gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0()
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

__int16 Paramite::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VOnSameYLevel_44B240(pOther);
}

__int16 Paramite::VOnSameYLevel_44B240(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return FP_Abs(pOther->field_AC_ypos - field_AC_ypos) < (field_BC_sprite_scale * FP_FromInteger(40));
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

void Paramite::VUpdate()
{
    VUpdate_44A490();
}

void Paramite::VUpdate_44A490()
{
    if (field_100_health > FP_FromInteger(0)
        && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        SND_SEQ_PlaySeq_4775A0(30u, 1, 0);
    }
    else
    {
        bool bFound = false;
        for (int i=0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->field_4_typeId == Types::eParamite_62  && pObjIter != this)
            {
                Paramite* pOther = static_cast<Paramite*>(pObjIter);
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    pOther->field_B2_lvl_number,
                    pOther->field_B0_path_number,
                    pOther->field_A8_xpos,
                    pOther->field_AC_ypos,
                    0))
                {
                    bFound = true;
                    break;
                }
            }
        }

        if (!bFound)
        {
            SND_Seq_Stop_477A60(30u);
        }
    }

    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (FP_Abs(field_A8_xpos - sActiveHero_507678->field_A8_xpos) > FP_FromInteger(1536) || FP_Abs(field_AC_ypos - sActiveHero_507678->field_AC_ypos) > FP_FromInteger(480))
    {
        if (field_144)
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
        else
        {
            if (field_148_pMeat)
            {
                if (field_148_pMeat->VIsFalling() || field_148_pMeat->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    field_148_pMeat->field_C_refCount--;
                    field_148_pMeat = nullptr;
                    field_FE_next_state = 0;
                    SetBrain(&Paramite::Brain_447A10);
                    field_110_state = 0;
                }
            }
        }

        field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

        if (field_10_anim.field_84_vram_rect.w)
        {
            Vram_free_450CE0(
                { field_10_anim.field_84_vram_rect.x, field_10_anim.field_84_vram_rect.y },
                { field_10_anim.field_84_vram_rect.w, field_10_anim.field_84_vram_rect.h });
            field_10_anim.field_84_vram_rect.w = 0;
        }

    }
    else
    {
        if (field_10_anim.field_84_vram_rect.w == 0)
        {
            vram_alloc_450B20(138u, 49, 8u, &field_10_anim.field_84_vram_rect);
        }

        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

        const auto oldMotion = field_FC_current_motion;
        field_110_state = (this->*field_10C_fn)();

        if (word_5076E0)
        {
            DebugOut_495990(
                "Paramite %d %d %d %d\n",
                field_110_state,
                field_114,
                field_FC_current_motion,
                field_FE_next_state);
        }

        const FP oldx = field_A8_xpos;
        const FP oldy = field_AC_ypos;
        
        (this->*sParamiteMotionTable_4CDCB0[field_FC_current_motion])();
        
        if (oldx != field_A8_xpos || oldy != field_AC_ypos)
        {
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
            ToKnockBack_44B5B0();
            field_FE_next_state = -1;
            field_FC_current_motion = field_FE_next_state;
            field_106_shot = 0;
        }

        if (oldMotion == field_FC_current_motion)
        {
            if (field_140)
            {

                field_FC_current_motion = field_E4;
                VUpdateAnimData_44A460();
                field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
                field_140 = 0;
            }
        }
        else
        {
            VUpdateAnimData_44A460();
            if (oldMotion == eParamiteStates::State_4_Unknown_44B6C0)
            {
                field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            }
        }
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

void Paramite::VUpdateAnimData_44A460()
{
    field_10_anim.Set_Animation_Data_402A40(
        sParamiteFrameTables_4CDD18[field_FC_current_motion],
        ResBlockForMotion_44AC10(field_FC_current_motion));
}

__int16 Paramite::FindOtherParamite_44AF80()
{
    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eParamite_62 && pObjIter != this)
        {
            Paramite* pOther = static_cast<Paramite*>(pObjIter);
            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                pOther->field_B2_lvl_number,
                pOther->field_B0_path_number,
                pOther->field_A8_xpos,
                pOther->field_AC_ypos,
                0)
                && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                if (FP_Abs(pOther->field_AC_ypos - field_AC_ypos) < (field_BC_sprite_scale * FP_FromInteger(100)))
                {
                    if (pOther->BrainIs(&Paramite::Brain_447A10) || pOther->BrainIs(&Paramite::Brain_449170))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void Paramite::ToKnockBack_44B5B0()
{
    const FP nextX = field_A8_xpos - field_B4_velx;
    field_A8_xpos = FP_FromInteger((FP_GetExponent(nextX) & 0xFC00) + Grid_SnapX_41FAA0(field_BC_sprite_scale, FP_GetExponent(nextX) & 0x3FF));
    
    MapFollowMe_401D30(1);

    if (field_B8_vely < FP_FromInteger(0))
    {
        field_B8_vely = FP_FromInteger(0);
    }

    if (field_F4_pLine)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

        field_124 = 0;
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
        MapFollowMe_401D30(1);
    }
    else
    {
        field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
    }
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

__int16 Paramite::Brain_44DD70()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (IsBeeSwarmChasingMe_4022B0())
    {
        switch (field_110_state)
        {
        case 0:
            field_114 = gnFrameCount_507670 + 30;
            return 1;

        case 1:
            if (field_114 <= static_cast<int>(gnFrameCount_507670))
            {
                field_FC_current_motion = eParamiteStates::State_13_GameSpeakBegin_44D050;
                field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
                return 2;
            }
            break;

        case 2:
            if (field_FC_current_motion == eParamiteStates::State_15_Hiss_44D300)
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                    return 3;
                }
            }
            break;

        case 3:
            if (field_FC_current_motion == eParamiteStates::State_5_Turn_44C8E0)
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_FE_next_state = eParamiteStates::State_25_Death_44DB90;
                    field_114 = gnFrameCount_507670 + 30;
                    return 1;
                }
            }
            break;
        default:
            break;
        }
        return field_110_state;
    }
    else if (field_FC_current_motion == eParamiteStates::State_5_Turn_44C8E0)
    {
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        SetBrain(&Paramite::Brain_447A10);
        return 0;
    }
    else
    {
        field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
        field_FE_next_state = -1;
        SetBrain(&Paramite::Brain_447A10);
        return field_110_state;
    }
}

__int16 Paramite::Brain_448BF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::Brain_449170()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::Brain_449CD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Paramite::SetBrain(Paramite::TParamiteBrain fn)
{
    ::SetBrain(fn, field_10C_fn, sParamiteAITable);
}

bool Paramite::BrainIs(Paramite::TParamiteBrain fn)
{
    return ::BrainIs(fn, field_10C_fn, sParamiteAITable);
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


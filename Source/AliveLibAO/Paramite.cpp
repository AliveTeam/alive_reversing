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
#include "CameraSwapper.hpp"
#include "PlatformBase.hpp"
#include "LiftPoint.hpp"
#include "Particle.hpp"
#include "SwitchStates.hpp"

void Paramite_ForceLink() {}

namespace AO {

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

const static s32 sParamiteFrameTables_4CDD18[] =
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
    { &Paramite::Brain_Patrol_447A10,  0x447A10, "Brain_Patrol_0" },
    { &Paramite::Brain_SurpriseWeb_448D00,  0x448D00, "Brain_SurpriseWeb_1" },
    { &Paramite::Brain_Struggling_44DD70,  0x44DD70, "Brain_Struggling_2" },
    { &Paramite::Brain_Death_448BF0,  0x448BF0, "Brain_Death_3" },
    { &Paramite::Brain_ChasingAbe_449170,  0x449170, "Brain_ChasingAbe_4" },
    { &Paramite::Brain_SpottedMeat_449CD0,  0x449CD0, "Brain_SpottedMeat_5" },
};

Paramite* Paramite::ctor_44A7A0(Path_Paramite* pTlv, s32 tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BBBE8);
    field_4_typeId = Types::eParamite_62;

    for (s32 i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        field_150_resources[i] = nullptr;
    }

    field_150_resources[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjbasicResID, 1, 0);
    field_150_resources[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjpumpResID, 1, 0);
    field_150_resources[14] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjponceResID, 1, 0);
    field_150_resources[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjclimbResID, 1, 0);
    field_150_resources[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjscrchResID, 1, 0);
    field_150_resources[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjeatResID, 1, 0);
    field_150_resources[15] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjfalrkResID, 1, 0);
    field_150_resources[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArjwaspResID, 1, 0);

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kWebResID, 1, 0);

    Animation_Init_417FD0(
        57152,
        138,
        49,
        field_150_resources[0],
        1);

    field_12A_res_idx = 0;


    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_114_timer = 0;
    field_110_state = 0;
    field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
    field_EC = 3;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
    field_140_use_prev_motion = 0;

    field_148_pMeat = nullptr;

    field_14C_pWeb = nullptr;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_8;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_27;
        field_C6_scale = 1;
    }

    if (pTlv->field_1A_bEnter_from_web == Choice_short::eYes_1)
    {
        SetBrain(&Paramite::Brain_SurpriseWeb_448D00);
    }
    else
    {
        SetBrain(&Paramite::Brain_Patrol_447A10);
    }

    field_11E_attack_delay = pTlv->field_1C_attack_delay;
    field_112_drop_in_timer = pTlv->field_1E_drop_in_timer;
    field_11C_meat_eating_time = pTlv->field_20_meat_eating_time;
    field_134_attack_duration = pTlv->field_22_attack_duration;
    field_13C_id = pTlv->field_26_id;
    field_13E_hiss_before_attack = pTlv->field_28_hiss_before_attack;
    field_144_delete_when_far_away = pTlv->field_2A_delete_when_far_away;

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

    field_128_never_read = 0;
    field_12C_tlvInfo = tlvInfo;

    if (!VIsFacingMe(sActiveHero_507678))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    VStackOnObjectsOfType(Types::eParamite_62);

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

    for (s32 i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        if (field_10_anim.field_20_ppBlock != field_150_resources[i])
        {
            if (field_150_resources[i])
            {
                ResourceManager::FreeResource_455550(field_150_resources[i]);
            }
        }
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kWebResID, 0, 0));
    if (field_100_health <= FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_12C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_12C_tlvInfo, -1, 0, 0);
    }

    SND_Seq_Stop_477A60(SeqId::eParamiteNearby_30);

    MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);

    return dtor_401000();
}

BaseGameObject* Paramite::VDestructor(s32 flags)
{
    return Vdtor_44B300(flags);
}

Paramite* Paramite::Vdtor_44B300(s32 flags)
{
    dtor_44AB00();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BYTE** Paramite::ResBlockForMotion_44AC10(s16 motion)
{
    s16 idx = 0;

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

void Paramite::VRender(PrimHeader** ppOt)
{
    VRender_44ACA0(ppOt);
}

void Paramite::VRender_44ACA0(PrimHeader** ppOt)
{
    if (field_8_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

s16 Paramite::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_44ACC0(pFrom);
}

s16 Paramite::VTakeDamage_44ACC0(BaseGameObject* pFrom)
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
            if (!BrainIs(&Paramite::Brain_Struggling_44DD70))
            {
                field_FE_next_state = eParamiteStates::State_25_Death_44DB90;
                SetBrain(&Paramite::Brain_Struggling_44DD70);
                field_110_state = 0;
            }
        }
        else
        {
            field_100_health = FP_FromInteger(0);
            SetBrain(&Paramite::Brain_Death_448BF0);
            field_128_never_read = 2;
            field_114_timer = gnFrameCount_507670 + 90;
            field_FC_current_motion = eParamiteStates::State_24_Struggle_44DB70;
            VUpdateAnimData_44A460();
        }
        break;

    default:
    {
        field_100_health = FP_FromInteger(0);
        SetBrain(&Paramite::Brain_Death_448BF0);
        field_128_never_read = 2;
        field_114_timer = gnFrameCount_507670 + 90;
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
            field_128_never_read = 2;
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

s16 Paramite::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VOnSameYLevel_44B240(pOther);
}

s16 Paramite::VOnSameYLevel_44B240(BaseAnimatedWithPhysicsGameObject* pOther)
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
        SND_SEQ_PlaySeq_4775A0(SeqId::eParamiteNearby_30, 1, 0);
    }
    else
    {
        bool bFound = false;
        for (s32 i=0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
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
            SND_Seq_Stop_477A60(SeqId::eParamiteNearby_30);
        }
    }

    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (FP_Abs(field_A8_xpos - sActiveHero_507678->field_A8_xpos) > FP_FromInteger(1536) || FP_Abs(field_AC_ypos - sActiveHero_507678->field_AC_ypos) > FP_FromInteger(480))
    {
        if (field_144_delete_when_far_away == Choice_short::eYes_1)
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
                    field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                    SetBrain(&Paramite::Brain_Patrol_447A10);
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

        static auto oldBrain = field_10C_fn;
        if (field_10C_fn != oldBrain)
        {
            //LOG_INFO("Brain changed from " << GetOriginalFn(oldBrain, sParamiteAITable).fnName << " to " << GetOriginalFn(field_10C_fn, sParamiteAITable).fnName);
        }
        oldBrain = field_10C_fn;

        if (showDebugCreatureInfo_5076E0)
        {
            DDCheat::DebugStr_495990(
                "Paramite %d %d %d %d\n",
                field_110_state,
                field_114_timer,
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
            if (field_140_use_prev_motion)
            {

                field_FC_current_motion = field_E4_previous_motion;
                VUpdateAnimData_44A460();
                field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
                field_140_use_prev_motion = 0;
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

    field_124_XSpeed = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);

    field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;

    MapFollowMe_401D30(TRUE);
}

s16 Paramite::ToNextMotion_44B320()
{
    switch (field_FE_next_state)
    {
    case eParamiteStates::State_5_Turn_44C8E0:
    case eParamiteStates::State_23_Eating_44B970:
    case eParamiteStates::State_25_Death_44DB90:
    case eParamiteStates::State_13_GameSpeakBegin_44D050:
        field_FC_current_motion = field_FE_next_state;
        field_FE_next_state = -1;
        return 1;

    case eParamiteStates::State_3_Running_44C070:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromDouble(3.5));
            field_FC_current_motion = eParamiteStates::State_10_RunBegin_44C4C0;
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromDouble(3.5));
            field_FC_current_motion = eParamiteStates::State_10_RunBegin_44C4C0;
        }
        field_FE_next_state = -1;
        return 1;

    case eParamiteStates::State_2_Walking_44B9E0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(7);
            field_FC_current_motion = eParamiteStates::State_1_WalkBegin_44BCA0;
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(7));
            field_FC_current_motion = eParamiteStates::State_1_WalkBegin_44BCA0;
        }
        field_FE_next_state = -1;
        return 1;

    case eParamiteStates::State_15_Hiss_44D300:
        field_FC_current_motion = eParamiteStates::State_13_GameSpeakBegin_44D050;
        return 1;

    case eParamiteStates::State_18_RunningAttack_44D5D0:
        field_FC_current_motion = eParamiteStates::State_18_RunningAttack_44D5D0;
        field_FE_next_state = -1;
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
        return 1;

    case eParamiteStates::State_6_Hop_44CB20:
        VOnTrapDoorOpen();
        field_FC_current_motion = eParamiteStates::State_6_Hop_44CB20;
        field_FE_next_state = -1;
        field_E8_LastLineYPos = field_AC_ypos;
        field_B8_vely = FP_FromDouble(-7.2) * field_BC_sprite_scale;
        field_AC_ypos += field_B8_vely;
        field_F4_pLine = nullptr;
        return 1;

    default:
        return 0;
    }
}

void Paramite::VUpdateAnimData_44A460()
{
    field_10_anim.Set_Animation_Data_402A40(
        sParamiteFrameTables_4CDD18[field_FC_current_motion],
        ResBlockForMotion_44AC10(field_FC_current_motion));
}

s16 Paramite::AnotherParamiteNear_44AF80()
{
    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
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
                    if (pOther->BrainIs(&Paramite::Brain_Patrol_447A10) || pOther->BrainIs(&Paramite::Brain_ChasingAbe_449170))
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
    field_A8_xpos = FP_FromInteger((FP_GetExponent(nextX) & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, FP_GetExponent(nextX) & 0x3FF));

    MapFollowMe_401D30(1);

    if (field_B8_vely < FP_FromInteger(0))
    {
        field_B8_vely = FP_FromInteger(0);
    }

    if (field_F4_pLine)
    {
        ToIdle_44B580();
    }
    else
    {
        field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
    }
}

Meat* Paramite::FindMeat_44B160()
{
    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eMeat_54)
        {
            auto pMeat = static_cast<Meat*>(pObjIter);
            if (pMeat->VCanEatMe())
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    pMeat->field_B2_lvl_number,
                    pMeat->field_B0_path_number,
                    pMeat->field_A8_xpos,
                    pMeat->field_AC_ypos,
                    0)
                    && !WallHit_401930(field_AC_ypos, pMeat->field_A8_xpos - field_A8_xpos))
                {
                    if (!pMeat->field_124_pLine)
                    {
                        return pMeat;
                    }

                    if (FP_Abs(pMeat->field_AC_ypos - field_AC_ypos) < (field_BC_sprite_scale * FP_FromInteger(100)))
                    {
                        return pMeat;
                    }
                }
            }
        }
    }
    return nullptr;
}

void Paramite::MoveOnLine_44B740()
{
    const FP old_x = field_A8_xpos;
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
                if (field_F4_pLine->field_8_type != 32 && field_F4_pLine->field_8_type != 36)
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
                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);

                    VOnCollisionWith(
                        { bRect.x, static_cast<short>(bRect.y + 5) },
                        { bRect.w, static_cast<short>(bRect.h + 5) },
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
            field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
            field_A8_xpos = field_B4_velx + old_x;
        }
    }
    else
    {
        field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
        field_E8_LastLineYPos = field_AC_ypos;
    }
}

const SfxDefinition stru_4CDD98[9] =
{
    { 0, 37, 60, 90, -520, -520, 0 },
    { 0, 37, 61, 75, -520, -520, 0 },
    { 0, 37, 62, 50, -520, -520, 0 },
    { 0, 37, 64, 40, -520, -520, 0 },
    { 0, 37, 65, 40, -520, -520, 0 },
    { 0, 37, 66, 30, -520, -520, 0 },
    { 0, 37, 66, 50, 0, 0, 0 },
    { 0, 37, 67, 40, -520, -520, 0 },
    { 0, 37, 68, 30, -520, -520, 0 }
};

void Paramite::Sound_44DBB0(u8 idx)
{
    const CameraPos direction = gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos);

    short volRight = stru_4CDD98[idx].field_C_default_volume;
    short volLeft = 0;

    PSX_RECT rect = {};
    gMap_507BA8.Get_Camera_World_Rect_444C30(direction, &rect);

    switch (direction)
    {
    case CameraPos::eCamCurrent_0:
        volLeft = volRight;
        break;

    case CameraPos::eCamTop_1:
    case CameraPos::eCamBottom_2:
    {
        const FP tempVol = (FP_FromRaw(stru_4CDD98[idx].field_C_default_volume) / FP_FromInteger(3));
        volLeft = FP_GetExponent(tempVol);
        volRight = FP_GetExponent(tempVol);
        break;
    }

    case CameraPos::eCamLeft_3:
    {
        const FP numScreensAway = ((FP_FromInteger(rect.w) - field_A8_xpos) / FP_FromInteger(640));
        volLeft = volRight - FP_GetExponent(numScreensAway * FP_FromInteger(volRight - (volRight / 3)));
        volRight -= FP_GetExponent((numScreensAway * FP_FromInteger(volRight)));
        break;
    }

    case CameraPos::eCamRight_4:
    {
        const FP numScreensAway = ((field_A8_xpos - FP_FromInteger(rect.x)) / FP_FromInteger(640));
        volLeft = volRight - FP_GetExponent(numScreensAway * FP_FromInteger(volRight));
        volRight -= FP_GetExponent((numScreensAway * FP_FromInteger(volRight - (volRight / 3))));
        break;
    }

    default:
        return;
    }

    SFX_SfxDefinition_Play_477330(&stru_4CDD98[idx], volLeft, volRight, -520, -520);
}

void Paramite::SetMusic()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        if (BrainIs(&Paramite::Brain_ChasingAbe_449170))
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
        }
        else
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

void Paramite::ToHop_44B660()
{
    VOnTrapDoorOpen();

    field_FC_current_motion = eParamiteStates::State_6_Hop_44CB20;

    field_FE_next_state = -1;

    field_E8_LastLineYPos = field_AC_ypos;
    field_B8_vely = -FP_FromDouble(7.2) * field_BC_sprite_scale;
    field_AC_ypos += field_B8_vely;
    field_F4_pLine = nullptr;
}
enum AI_Patrol
{
    eState0_Inactive_0 = 0,
    eState0_IdleForAbe_1 = 1,
    eState0_FearingAbe_2 = 2,
    eState0_RunningFromAbe_3 = 3,
    eState0_ApproachingAbe_4 = 4,
    eState0_StopApproachingAbe_5 = 5,
    eState0_TurningForAbe_6 = 6,
    eState0_HittingAbe_7 = 7,
    eState0_StuckToWall_8 = 8,
    eState0_Attacking_9 = 9,

    eState0_Idle_12 = 12,
    eState0_IdleAnimation_13  = 13,
    eState0_Turning_14 = 14,
    eState0_Panic_15 = 15,
};

s16 Paramite::Brain_Patrol_447A10()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    switch (field_110_state)
    {
    case AI_Patrol::eState0_Inactive_0:
        if (sNumCamSwappers_507668 > 0)
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        field_128_never_read = 1;
        field_130_not_used = 0;
        return AI_Patrol::eState0_IdleForAbe_1;

    case AI_Patrol::eState0_IdleForAbe_1:
    {
        field_148_pMeat = FindMeat_44B160();
        if (field_148_pMeat)
        {
            SetBrain(&Paramite::Brain_SpottedMeat_449CD0);
            field_148_pMeat->field_C_refCount++;
            return AI_Patrol::eState0_Inactive_0;
        }

        if (!VOnSameYLevel(sActiveHero_507678)
            || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale
            || WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), sActiveHero_507678->field_A8_xpos - field_A8_xpos))
        {
            field_120_wait_timer = gnFrameCount_507670 + Math_RandomRange_450F20(45, 135);
            return AI_Patrol::eState0_Idle_12;
        }

        if (Event_Get_417250(kEventAbeOhm_8) && VIsFacingMe(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
            return AI_Patrol::eState0_Panic_15;
        }

        // TODO: Inlined HandleEnemyStopper but the directional logic isn't quite the same
        // so can't use it.
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            TlvTypes::EnemyStopper_79);
        if (field_F0_pTlv)
        {
            auto pStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);
            if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 && sActiveHero_507678->field_A8_xpos < field_A8_xpos) ||
                (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 && sActiveHero_507678->field_A8_xpos > field_A8_xpos))
            {
                if (!SwitchStates_Get(pStopper->field_1A_id))
                {
                    return AI_Patrol::eState0_IdleForAbe_1;
                }
            }
        }

        if ((AnotherParamiteNear_44AF80() ||
            sActiveHero_507678->field_100_health <= FP_FromInteger(0)) &&
            field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
        {
            Sound_44DBB0(5u);
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            SetBrain(&Paramite::Brain_ChasingAbe_449170);
            field_138_attack_timer = gnFrameCount_507670 + field_134_attack_duration;
            return AI_Patrol::eState0_Inactive_0;
        }

        if (!VIsObjNearby(kGridSize * FP_FromInteger(6), sActiveHero_507678))
        {
            if (!VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero_507678))
            {
                if (!VIsFacingMe(sActiveHero_507678))
                {
                    if ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_4021A0(0, 2)) ||
                        (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_4021A0(1, 2)))
                    {
                        field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                        return AI_Patrol::eState0_TurningForAbe_6;
                    }
                }
                else
                {
                    if ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_4021A0(1, 2)) ||
                        (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_4021A0(0, 2)))
                    {
                        field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                        return AI_Patrol::eState0_ApproachingAbe_4;
                    }
                }
            }

            if (!VIsFacingMe(sActiveHero_507678))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_TurningForAbe_6;
            }

            return field_110_state;
        }
        else if (VIsFacingMe(sActiveHero_507678))
        {
            if ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_4021A0(0, 2)) ||
                (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_4021A0(1, 2)))
            {
                field_FE_next_state = eParamiteStates::State_13_GameSpeakBegin_44D050;
                return AI_Patrol::eState0_StuckToWall_8;
            }

            Sound_44DBB0(5u);
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return AI_Patrol::eState0_TurningForAbe_6;
        }
        else if (!VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero_507678))
        {
            if ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_4021A0(1, 2)) ||
                (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_4021A0(0, 2)))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_HittingAbe_7;
            }

            field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
            field_128_never_read = 0;
            return AI_Patrol::eState0_FearingAbe_2;
        }
        else
        {
            if ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_4021A0(1, 2)) ||
                (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_4021A0(0, 2)))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_HittingAbe_7;
            }
            field_FE_next_state = eParamiteStates::State_3_Running_44C070;
            return AI_Patrol::eState0_RunningFromAbe_3;

        }
        break;
    }

    case AI_Patrol::eState0_FearingAbe_2:
        if (!VOnSameYLevel(sActiveHero_507678) || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale)
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (field_B4_velx < FP_FromInteger(0))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 2))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_HittingAbe_7;
            }
        }

        if (field_B4_velx > FP_FromInteger(0))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 2))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_HittingAbe_7;
            }
        }

        if (AnotherParamiteNear_44AF80())
        {
            Sound_44DBB0(5u);
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            SetBrain(&Paramite::Brain_ChasingAbe_449170);
            field_138_attack_timer = gnFrameCount_507670 + field_134_attack_duration;
            return 0;
        }

        if (VIsObj_GettingNear_On_X(sActiveHero_507678) && VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_3_Running_44C070;
            return AI_Patrol::eState0_RunningFromAbe_3;
        }

        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            return field_110_state;
        }

        if (VIsObjNearby(kGridSize * FP_FromInteger(7), sActiveHero_507678))
        {
            return field_110_state;
        }

        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        field_128_never_read = 1;
        return AI_Patrol::eState0_IdleForAbe_1;

    case AI_Patrol::eState0_RunningFromAbe_3:
        if (!VOnSameYLevel(sActiveHero_507678) || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale)
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (field_B4_velx < FP_FromInteger(0))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 3))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_HittingAbe_7;
            }
        }

        if (field_B4_velx > FP_FromInteger(0))
        {
            if ((WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 3)))
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_HittingAbe_7;
            }
        }

        if (AnotherParamiteNear_44AF80())
        {
            Sound_44DBB0(5u);
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            SetBrain(&Paramite::Brain_ChasingAbe_449170);
            field_138_attack_timer = gnFrameCount_507670 + field_134_attack_duration;
            return 0;
        }

        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            return field_110_state;
        }

        if (VIsObjNearby(kGridSize * FP_FromInteger(7), sActiveHero_507678))
        {
            return field_110_state;
        }

        field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
        return AI_Patrol::eState0_FearingAbe_2;

    case AI_Patrol::eState0_ApproachingAbe_4:
        if (!VOnSameYLevel(sActiveHero_507678) || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale)
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (AnotherParamiteNear_44AF80())
        {
            Sound_44DBB0(5u);
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            field_138_attack_timer = gnFrameCount_507670 + field_134_attack_duration;
            SetBrain(&Paramite::Brain_ChasingAbe_449170);
            return 0;
        }

        if (field_B4_velx < FP_FromInteger(0))
        {
            if (HandleEnemyStopper(-2, Path_EnemyStopper::StopDirection::Left_0))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 2))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }
        }

        if (field_B4_velx > FP_FromInteger(0))
        {
            if (HandleEnemyStopper(2, Path_EnemyStopper::StopDirection::Right_1))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 2))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }
        }

        if (VIsObj_GettingNear_On_X(sActiveHero_507678) || VIsObjNearby(kGridSize * FP_FromInteger(10), sActiveHero_507678))
        {
            if (!VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                return field_110_state;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero_507678))
            {
                return field_110_state;
            }
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
        field_FE_next_state = eParamiteStates::State_3_Running_44C070;
        return AI_Patrol::eState0_StopApproachingAbe_5;

    case AI_Patrol::eState0_StopApproachingAbe_5:
        if (!VOnSameYLevel(sActiveHero_507678) || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale)
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (AnotherParamiteNear_44AF80())
        {
            Sound_44DBB0(5u);
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            field_138_attack_timer = gnFrameCount_507670 + field_134_attack_duration;
            SetBrain(&Paramite::Brain_ChasingAbe_449170);
            return 0;
        }

        if (field_B4_velx < FP_FromInteger(0))
        {
            if (HandleEnemyStopper(-2, Path_EnemyStopper::StopDirection::Left_0))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 3))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }
        }

        if (field_B4_velx > FP_FromInteger(0))
        {
            if (HandleEnemyStopper(2, Path_EnemyStopper::StopDirection::Right_1))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 3))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_Patrol::eState0_IdleForAbe_1;
            }
        }

        if (!VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            return field_110_state;
        }

        if (VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (!VIsObjNearby(kGridSize * FP_FromInteger(10), sActiveHero_507678))
        {
            return field_110_state;
        }

        field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
        return AI_Patrol::eState0_ApproachingAbe_4;

    case AI_Patrol::eState0_TurningForAbe_6:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        return AI_Patrol::eState0_IdleForAbe_1;

    case AI_Patrol::eState0_HittingAbe_7:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0 || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_13_GameSpeakBegin_44D050;
        return AI_Patrol::eState0_StuckToWall_8;

    case AI_Patrol::eState0_StuckToWall_8:
        field_148_pMeat = FindMeat_44B160();
        if (field_148_pMeat)
        {
            SetBrain(&Paramite::Brain_SpottedMeat_449CD0);
            field_148_pMeat->field_C_refCount++;
            return 0;
        }

        if (!VOnSameYLevel(sActiveHero_507678) || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale)
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (VIsObjNearby(kGridSize * FP_FromInteger(3), sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_18_RunningAttack_44D5D0;
            return AI_Patrol::eState0_Attacking_9;
        }

        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
        }
        else
        {
            if (!VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero_507678))
            {
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_Patrol::eState0_ApproachingAbe_4;
            }
        }

        if (field_FC_current_motion != eParamiteStates::State_14_PreHiss_44D170)
        {
            return field_110_state;
        }

        if (Math_NextRandom() >= 6u)
        {
            return field_110_state;
        }

        field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
        return field_110_state;

    case AI_Patrol::eState0_Attacking_9:
        if (field_FC_current_motion != eParamiteStates::State_18_RunningAttack_44D5D0
            || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
        {
            return AI_Patrol::eState0_IdleForAbe_1;
        }
        SetBrain(&Paramite::Brain_ChasingAbe_449170);
        return 0;

    case AI_Patrol::eState0_Idle_12:
        field_148_pMeat = FindMeat_44B160();
        if (field_148_pMeat)
        {
            SetBrain(&Paramite::Brain_SpottedMeat_449CD0);
            field_148_pMeat->field_C_refCount++;
            return 0;
        }

        if (VOnSameYLevel(sActiveHero_507678)
            && field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale
            && !WallHit_401930(field_BC_sprite_scale * FP_FromInteger(5), sActiveHero_507678->field_A8_xpos - field_A8_xpos))
        {
           return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (Event_Get_417250(kEventAbeOhm_8))
        {
            field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
            return AI_Patrol::eState0_Panic_15;
        }

        if (field_120_wait_timer <= static_cast<s32>(gnFrameCount_507670))
        {
            if (Math_NextRandom() >= 6u)
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_Patrol::eState0_Turning_14;
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_13_GameSpeakBegin_44D050;
                return AI_Patrol::eState0_IdleAnimation_13;
            }
        }
        return field_110_state;

    case AI_Patrol::eState0_IdleAnimation_13:
        if (field_FC_current_motion != eParamiteStates::State_14_PreHiss_44D170
            || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        field_120_wait_timer = gnFrameCount_507670 + Math_RandomRange_450F20(45, 135);
        return AI_Patrol::eState0_Idle_12;

    case AI_Patrol::eState0_Turning_14:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0
            || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_110_state;
        }
        field_120_wait_timer = gnFrameCount_507670 + Math_RandomRange_450F20(45, 135);
        return AI_Patrol::eState0_Idle_12;

    case AI_Patrol::eState0_Panic_15:
        if (!Event_Get_417250(kEventAbeOhm_8))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_Patrol::eState0_Idle_12;
        }
        field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
        return field_110_state;

    default:
        return field_110_state;
    }
}

enum AI_SurpriseWeb
{
    eState3_Loading_0 = 0,
    eState3_AppearingRight_1 = 1,
    eState3_AppearingLeft_2 = 2,
    eState3_StartAnimation_3 = 3,
    eState3_StateLoop1_4 = 4,
    eState3_StateLoop2_5 = 5
};

s16 Paramite::Brain_SurpriseWeb_448D00()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_110_state)
    {
    case AI_SurpriseWeb::eState3_Loading_0:
    {
        if (sNumCamSwappers_507668 > 0)
        {
            return field_110_state;
        }

        field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;

        MapFollowMe_401D30(TRUE);
        auto pWeb = ao_new<ParamiteWeb>();
        if (pWeb)
        {
            pWeb->ctor_48A920(
                field_A8_xpos,
                FP_GetExponent(field_AC_ypos) - 20,
                FP_GetExponent(field_AC_ypos) - 10,
                field_BC_sprite_scale);
            pWeb->field_C_refCount++;
        }
        field_14C_pWeb = pWeb;
        if (sActiveHero_507678->field_A8_xpos >= field_A8_xpos)
        {
            return AI_SurpriseWeb::eState3_AppearingLeft_2;
        }
        return AI_SurpriseWeb::eState3_AppearingRight_1;
    }

    case AI_SurpriseWeb::eState3_AppearingRight_1:
        if (sActiveHero_507678->field_A8_xpos <= field_A8_xpos
            || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale
            || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                1))
        {
            if (!SwitchStates_Get(field_13C_id))
            {
                 return field_110_state;
            }
        }
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        field_114_timer = gnFrameCount_507670 + field_112_drop_in_timer;
        return AI_SurpriseWeb::eState3_StartAnimation_3;

    case AI_SurpriseWeb::eState3_AppearingLeft_2:
        if (sActiveHero_507678->field_A8_xpos >= field_A8_xpos
            || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale
            || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                1))
        {
            if (!SwitchStates_Get(field_13C_id))
            {
                 return field_110_state;
            }
        }
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_114_timer = gnFrameCount_507670 + field_112_drop_in_timer;
        return AI_SurpriseWeb::eState3_StartAnimation_3;

    case AI_SurpriseWeb::eState3_StartAnimation_3:
        if (field_114_timer > static_cast<s32>(gnFrameCount_507670))
        {
            return field_110_state;
        }
        field_B8_vely = FP_FromInteger(0);
        field_FC_current_motion = eParamiteStates::State_20_SurpriseWeb_44D9A0;
        return AI_SurpriseWeb::eState3_StateLoop1_4;

    case AI_SurpriseWeb::eState3_StateLoop1_4:
        field_14C_pWeb->field_EA_ttl_remainder = FP_GetExponent(FP_Abs(field_AC_ypos)) - 10;
        field_14C_pWeb->field_AC_ypos = FP_FromInteger(field_14C_pWeb->field_EA_ttl_remainder);
        if (field_FC_current_motion == eParamiteStates::State_0_Idle_44B900)
        {
            field_14C_pWeb->field_F0_bEnabled = TRUE;
            field_14C_pWeb->field_C_refCount--;
            field_14C_pWeb = nullptr;
            SetBrain(&Paramite::Brain_Patrol_447A10);
            return AI_Patrol::eState0_Inactive_0;
        }

        if (field_B8_vely < (field_BC_sprite_scale * FP_FromInteger(8)))
        {
            field_B8_vely += (field_BC_sprite_scale * FP_FromDouble(0.5));
            return field_110_state;

        }
        return AI_SurpriseWeb::eState3_StateLoop2_5;

    case AI_SurpriseWeb::eState3_StateLoop2_5:
        field_14C_pWeb->field_EA_ttl_remainder = FP_GetExponent(FP_Abs(field_AC_ypos)) - 10;
        field_14C_pWeb->field_AC_ypos = FP_FromInteger(field_14C_pWeb->field_EA_ttl_remainder);
        if (field_FC_current_motion != eParamiteStates::State_0_Idle_44B900)
        {
            if (field_B8_vely <= (field_BC_sprite_scale * FP_FromInteger(-1)))
            {
                return AI_SurpriseWeb::eState3_StateLoop1_4;
            }
            else
            {
                field_B8_vely -= (field_BC_sprite_scale * FP_FromInteger(1));
                return field_110_state;
            }
        }
        else
        {
            field_14C_pWeb->field_F0_bEnabled = TRUE;
            field_14C_pWeb->field_C_refCount--;
            field_14C_pWeb = nullptr;
            SetBrain(&Paramite::Brain_Patrol_447A10);
            return AI_Patrol::eState0_Inactive_0;
        }
        break;

    default:
        break;
    }

    return field_110_state;
}

s16 Paramite::Brain_Struggling_44DD70()
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
            field_114_timer = gnFrameCount_507670 + 30;
            return 1;

        case 1:
            if (field_114_timer <= static_cast<s32>(gnFrameCount_507670))
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
                    field_114_timer = gnFrameCount_507670 + 30;
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
        SetBrain(&Paramite::Brain_Patrol_447A10);
        return 0;
    }
    else
    {
        field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
        field_FE_next_state = -1;
        SetBrain(&Paramite::Brain_Patrol_447A10);
        return field_110_state;
    }
}

s16 Paramite::Brain_Death_448BF0()
{
    if (static_cast<s32>(gnFrameCount_507670) < field_114_timer && (field_114_timer < static_cast<s32>(gnFrameCount_507670) + 80))
    {
        field_BC_sprite_scale -= FP_FromDouble(0.008);
        field_C0_r -= 2;
        field_C2_g -= 2;
        field_C4_b -= 2;
        if (static_cast<s32>(gnFrameCount_507670) < field_114_timer - 24 && !(static_cast<s32>(gnFrameCount_507670) % 5))
        {
            New_Smoke_Particles_419A80(
                (FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * field_BC_sprite_scale) + field_A8_xpos,
                field_AC_ypos - FP_FromInteger(6),
                field_BC_sprite_scale / FP_FromInteger(2),
                2, 0);
            SFX_Play_43AE60(SoundEffect::Vaporize_96, 25, FP_GetExponent(FP_FromInteger(2200) * field_BC_sprite_scale), 0);
        }
    }

    if (field_114_timer < (s32) gnFrameCount_507670)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 100;
}

enum AI_ChasingAbe
{
    eState2_Inactive_0 = 0,
    eState2_Attacking_1 = 1,
    eState2_ToWarning_2 = 2,
    eState2_Warning_3 = 3,
    eState2_CloseAttack_4 = 4,
    eState2_ToChasing_5 = 5,
    eState2_QuickAttack_6 = 6,
    eState2_Chasing_7 = 7,
    eState2_Jumping_8 = 8,
    eState2_TurningWhileChasing_9 = 9,
    eState2_Turning_10 = 10,
    eState2_Walking_11 = 11,
    eState2_WalkingToHop_12 = 12,
    eState2_Eating_13 = 13,
    eState2_Idle_14 = 14,
    eState2_Panic_15 = 15
};

s16 Paramite::Brain_ChasingAbe_449170()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_138_attack_timer <= static_cast<s32>(gnFrameCount_507670)
        && (!VOnSameYLevel(sActiveHero_507678)
            || field_BC_sprite_scale != sActiveHero_507678->field_BC_sprite_scale)
        && sActiveHero_507678->field_100_health > FP_FromInteger(0))
    {
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        SetBrain(&Paramite::Brain_Patrol_447A10);
        return 0;
    }

    if (Event_Get_417250(kEventAbeOhm_8))
    {
        field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
        return AI_ChasingAbe::eState2_Panic_15;
    }

    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    switch (field_110_state)
    {
    case AI_ChasingAbe::eState2_Inactive_0:
        if (!VOnSameYLevel(sActiveHero_507678))
        {
            return AI_ChasingAbe::eState2_Inactive_0;
        }

        if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
        {
            if (VIsFacingMe(sActiveHero_507678))
            {
                if (field_13E_hiss_before_attack == Choice_short::eYes_1)
                {
                    field_114_timer = gnFrameCount_507670 + Math_RandomRange_450F20(0, 6);
                    return AI_ChasingAbe::eState2_ToWarning_2;
                }
                else
                {
                    field_114_timer = gnFrameCount_507670 + field_11E_attack_delay;
                    return AI_ChasingAbe::eState2_ToChasing_5;
                }
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_ChasingAbe::eState2_CloseAttack_4;
            }
        }
        else
        {
            if (VIsFacingMe(sActiveHero_507678))
            {
                if (VIsObjNearby(kGridSize, sActiveHero_507678))
                {
                    field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
                    field_114_timer = gnFrameCount_507670 + 28;
                    return AI_ChasingAbe::eState2_Eating_13;
                }

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (Check_IsOnEndOfLine_4021A0(1, 1))
                    {
                        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                        return AI_ChasingAbe::eState2_Jumping_8;
                    }
                }
                else
                {
                    if (Check_IsOnEndOfLine_4021A0(0, 1))
                    {
                        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                        return AI_ChasingAbe::eState2_Jumping_8;
                    }
                }
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_ChasingAbe::eState2_Walking_11;
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_ChasingAbe::eState2_Turning_10;
            }
        }
        break;

    case AI_ChasingAbe::eState2_Attacking_1:
        if (field_FC_current_motion != eParamiteStates::State_18_RunningAttack_44D5D0
            || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_110_state;
        }

        if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
        {
            if (VIsFacingMe(sActiveHero_507678))
            {
                if (VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero_507678))
                {
                    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), sActiveHero_507678->field_A8_xpos - field_A8_xpos))
                    {
                        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                        return AI_ChasingAbe::eState2_ToChasing_5;
                    }
                    else
                    {
                        field_FE_next_state = eParamiteStates::State_18_RunningAttack_44D5D0;
                        return AI_ChasingAbe::eState2_Attacking_1;
                    }
                }

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (Check_IsOnEndOfLine_4021A0(1, 1))
                    {
                        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                        return AI_ChasingAbe::eState2_Jumping_8;
                    }
                }
                else
                {
                    if (Check_IsOnEndOfLine_4021A0(0, 1))
                    {
                        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                        return AI_ChasingAbe::eState2_Jumping_8;
                    }
                }
                field_FE_next_state = eParamiteStates::State_3_Running_44C070;
                return AI_ChasingAbe::eState2_Chasing_7;
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_ChasingAbe::eState2_QuickAttack_6;
            }
        }
        else
        {
            if (VIsFacingMe(sActiveHero_507678))
            {
                if (VIsObjNearby(kGridSize, sActiveHero_507678))
                {
                    field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
                    field_114_timer = gnFrameCount_507670 + 28;
                    return AI_ChasingAbe::eState2_Eating_13;
                }

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (Check_IsOnEndOfLine_4021A0(1, 1))
                    {
                        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                        return AI_ChasingAbe::eState2_Jumping_8;
                    }
                }
                else
                {
                    if (Check_IsOnEndOfLine_4021A0(0, 1))
                    {
                        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                        return AI_ChasingAbe::eState2_Jumping_8;
                    }
                }
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_ChasingAbe::eState2_Walking_11;
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_ChasingAbe::eState2_Turning_10;
            }
        }
        break;

    case AI_ChasingAbe::eState2_ToWarning_2:
        if (field_114_timer > static_cast<s32>(gnFrameCount_507670))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
        field_114_timer = gnFrameCount_507670 + field_11E_attack_delay;
        return AI_ChasingAbe::eState2_Warning_3;

    case AI_ChasingAbe::eState2_Warning_3:
        if (field_FC_current_motion != eParamiteStates::State_15_Hiss_44D300)
        {
            return field_110_state;
        }
        return AI_ChasingAbe::eState2_ToChasing_5;

    case AI_ChasingAbe::eState2_CloseAttack_4:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0
            || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_110_state;
        }

        if (field_13E_hiss_before_attack == Choice_short::eYes_1)
        {
            field_114_timer = gnFrameCount_507670 + Math_RandomRange_450F20(0, 6);
            return AI_ChasingAbe::eState2_ToWarning_2;
        }
        else
        {
            field_114_timer = gnFrameCount_507670 + field_11E_attack_delay;
            return AI_ChasingAbe::eState2_ToChasing_5;
        }
        break;

    case AI_ChasingAbe::eState2_ToChasing_5:
        if (!VOnSameYLevel(sActiveHero_507678))
        {
            return AI_ChasingAbe::eState2_Inactive_0;
        }

        if (!VIsFacingMe(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return 6;
        }

        // TODO: Inlined HandleEnemyStopper but the directional logic isn't quite the same
        // so can't use it.
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            TlvTypes::EnemyStopper_79);
        if (field_F0_pTlv)
        {
            auto pStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);
            if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 && sActiveHero_507678->field_A8_xpos < field_A8_xpos) ||
                (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 && sActiveHero_507678->field_A8_xpos > field_A8_xpos))
            {
                if (!SwitchStates_Get(pStopper->field_1A_id))
                {
                    return AI_ChasingAbe::eState2_ToChasing_5;
                }
            }
        }

        if (VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero_507678))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), sActiveHero_507678->field_A8_xpos - field_A8_xpos))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_ChasingAbe::eState2_ToChasing_5;
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_18_RunningAttack_44D5D0;
                return AI_ChasingAbe::eState2_Attacking_1;
            }
        }

        if (field_114_timer > static_cast<s32>(gnFrameCount_507670) && field_13E_hiss_before_attack == Choice_short::eYes_1)
        {
            return field_110_state;
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (Check_IsOnEndOfLine_4021A0(1, 1))
            {
                field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                return AI_ChasingAbe::eState2_Jumping_8;
            }
        }
        else if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (Check_IsOnEndOfLine_4021A0(0, 1))
            {
                field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
                return AI_ChasingAbe::eState2_Jumping_8;
            }
        }

        field_FE_next_state = eParamiteStates::State_3_Running_44C070;
        return AI_ChasingAbe::eState2_Chasing_7;

    case AI_ChasingAbe::eState2_QuickAttack_6:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0
            || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_110_state;
        }

        if (field_13E_hiss_before_attack == Choice_short::eYes_1)
        {
            field_FE_next_state = eParamiteStates::State_15_Hiss_44D300;
        }
        return AI_ChasingAbe::eState2_ToChasing_5;

    case AI_ChasingAbe::eState2_Chasing_7:
    {
        if (!VOnSameYLevel(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_ChasingAbe::eState2_Inactive_0;
        }

        if (!VIsFacingMe(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return AI_ChasingAbe::eState2_TurningWhileChasing_9;
        }

        const short x_exp = FP_GetExponent(field_A8_xpos);
        const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
        if (field_B4_velx < FP_FromInteger(0))
        {
            if (HandleEnemyStopper(-2, Path_EnemyStopper::StopDirection::Left_0))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_ChasingAbe::eState2_ToChasing_5;
            }

            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
            {
                ToHop_44B660();
                return AI_ChasingAbe::eState2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }
        else if (field_B4_velx > FP_FromInteger(0))
        {
            if (HandleEnemyStopper(2, Path_EnemyStopper::StopDirection::Right_1))
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_ChasingAbe::eState2_ToChasing_5;
            }

            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
            {
                ToHop_44B660();
                return AI_ChasingAbe::eState2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }

        if (!VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero_507678))
        {
            return field_110_state;
        }

        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), sActiveHero_507678->field_A8_xpos - field_A8_xpos))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_ChasingAbe::eState2_ToChasing_5;
        }
        else
        {
            field_FE_next_state = eParamiteStates::State_18_RunningAttack_44D5D0;
            return AI_ChasingAbe::eState2_Attacking_1;
        }
    }
        break;

    case AI_ChasingAbe::eState2_Jumping_8:
        if (field_FC_current_motion != eParamiteStates::State_0_Idle_44B900)
        {
            return field_110_state;
        }
        return AI_ChasingAbe::eState2_ToChasing_5;

    case AI_ChasingAbe::eState2_TurningWhileChasing_9:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0
            || !(field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        return AI_ChasingAbe::eState2_ToChasing_5;

    case AI_ChasingAbe::eState2_Turning_10:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0
            || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_110_state;
        }

        if (VIsObjNearby(kGridSize, sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
            field_114_timer = gnFrameCount_507670 + 28;
            return AI_ChasingAbe::eState2_Eating_13;
        }

        if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!Check_IsOnEndOfLine_4021A0(0, 1))
            {
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_ChasingAbe::eState2_Walking_11;
            }
        }
        else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!Check_IsOnEndOfLine_4021A0(1, 1))
            {
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_ChasingAbe::eState2_Walking_11;
            }
        }
        field_FE_next_state = eParamiteStates::State_6_Hop_44CB20;
        return AI_ChasingAbe::eState2_Jumping_8;

    case AI_ChasingAbe::eState2_Walking_11:
    {
        if (!VIsFacingMe(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return AI_ChasingAbe::eState2_Turning_10;
        }

        if (!VOnSameYLevel(sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_ChasingAbe::eState2_Walking_11;
        }

        if (VIsObjNearby(kGridSize, sActiveHero_507678))
        {
            field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
            field_114_timer = gnFrameCount_507670 + 28;
            return AI_ChasingAbe::eState2_Eating_13;
        }

        const short x_exp = FP_GetExponent(field_A8_xpos);
        const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
        if (field_B4_velx > FP_FromInteger(0))
        {
            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
            {
                ToHop_44B660();
                return AI_ChasingAbe::eState2_WalkingToHop_12;
            }
        }
        else if (field_B4_velx < FP_FromInteger(0))
        {
            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
            {
                ToHop_44B660();
                return AI_ChasingAbe::eState2_WalkingToHop_12;
            }
        }
    }
        return field_110_state;

    case AI_ChasingAbe::eState2_WalkingToHop_12:
        if (field_FC_current_motion != eParamiteStates::State_0_Idle_44B900)
        {
            return field_110_state;
        }

        if (VIsFacingMe(sActiveHero_507678))
        {
            if (VIsObjNearby(kGridSize, sActiveHero_507678))
            {
                field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
                field_114_timer = gnFrameCount_507670 + 28;
                return AI_ChasingAbe::eState2_Eating_13;
            }
            else
            {
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_ChasingAbe::eState2_Walking_11;
            }
        }
        else
        {
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return AI_ChasingAbe::eState2_Turning_10;
        }
        break;

    case AI_ChasingAbe::eState2_Eating_13:
        if (field_114_timer > static_cast<s32>(gnFrameCount_507670))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        field_114_timer = gnFrameCount_507670 + 15;
        return AI_ChasingAbe::eState2_Idle_14;

    case AI_ChasingAbe::eState2_Idle_14:
        if (field_114_timer > static_cast<s32>(gnFrameCount_507670))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
        field_114_timer = gnFrameCount_507670 + 28;
        return AI_ChasingAbe::eState2_Eating_13;

    case AI_ChasingAbe::eState2_Panic_15:
        if (Event_Get_417250(kEventAbeOhm_8))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        return AI_ChasingAbe::eState2_Inactive_0;

    default:
        return field_110_state;
    }
}

enum AI_SpottedMeat
{
    eState5_Idle_0 = 0,
    eState5_Running_1 = 1,
    eState5_Walking_2 = 2,
    eState5_Jumping_3 = 3,
    eState5_Turning_4 = 4,
    eState5_AttentiveToMeat_5 = 5,
    eState5_Eating_6 = 6
};

s16 Paramite::Brain_SpottedMeat_449CD0()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_148_pMeat->VIsFalling() || field_148_pMeat->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        Sound_44DBB0(7u);
        SetBrain(&Paramite::Brain_Patrol_447A10);
        field_148_pMeat->field_C_refCount--;
        field_148_pMeat = nullptr;
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        return AI_Patrol::eState0_Inactive_0;
    }

    if (field_148_pMeat->field_124_pLine)
    {
        if (FP_Abs(field_148_pMeat->field_AC_ypos - field_AC_ypos) > FP_FromInteger(20))
        {
            field_148_pMeat->field_C_refCount--;
            field_148_pMeat = nullptr;
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            SetBrain(&Paramite::Brain_Patrol_447A10);
            return AI_Patrol::eState0_Inactive_0;
        }
    }

    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    switch (field_110_state)
    {
    case AI_SpottedMeat::eState5_Idle_0:
        if (!VIsFacingMe(field_148_pMeat))
        {
            if (FP_Abs(field_148_pMeat->field_A8_xpos - field_A8_xpos) > FP_FromInteger(5))
            {
                Sound_44DBB0(7u);
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_SpottedMeat::eState5_Turning_4;
            }
        }

        if (VIsObj_GettingNear_On_X(field_148_pMeat))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_SpottedMeat::eState5_AttentiveToMeat_5;
        }

        if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (Check_IsOnEndOfLine_4021A0(0, 1))
            {
                ToHop_44B660();
                return  AI_SpottedMeat::eState5_Jumping_3;
            }
        }
        else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (Check_IsOnEndOfLine_4021A0(1, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }

        if (!VIsObjNearby(kGridSize * FP_FromInteger(3), field_148_pMeat))
        {
            field_FE_next_state = eParamiteStates::State_3_Running_44C070;
            return AI_SpottedMeat::eState5_Running_1;
        }

        if (VIsObjNearby(field_BC_sprite_scale * FP_FromInteger(40), field_148_pMeat))
        {
            if (field_A8_xpos == field_148_pMeat->field_A8_xpos)
            {
                field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
                return AI_SpottedMeat::eState5_Walking_2;
            }

            if (!field_148_pMeat->field_124_pLine || !field_F4_pLine)
            {
                field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
                return AI_SpottedMeat::eState5_AttentiveToMeat_5;
            }

            field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
            field_114_timer = gnFrameCount_507670 + field_11C_meat_eating_time;
            return AI_SpottedMeat::eState5_Eating_6;
        }

        field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
        return AI_SpottedMeat::eState5_Walking_2;

    case AI_SpottedMeat::eState5_Running_1:
    {
        const short x_exp = FP_GetExponent(field_A8_xpos);
        const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
        if (field_B4_velx < FP_FromInteger(0))
        {
            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }

        if (field_B4_velx > FP_FromInteger(0))
        {
            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }

        if (!VIsFacingMe(field_148_pMeat))
        {
            if (FP_Abs(field_148_pMeat->field_A8_xpos - field_A8_xpos) > FP_FromInteger(5))
            {
                Sound_44DBB0(7u);
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_SpottedMeat::eState5_Turning_4;
            }
        }

        if (VIsObjNearby(field_BC_sprite_scale * FP_FromInteger(40), field_148_pMeat))
        {
            if (field_148_pMeat->field_124_pLine)
            {
                field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
                field_114_timer = gnFrameCount_507670 + field_11C_meat_eating_time;
                return AI_SpottedMeat::eState5_Eating_6;
            }
            return AI_SpottedMeat::eState5_Running_1;
        }

        if (!VIsObjNearby(kGridSize * FP_FromInteger(3), field_148_pMeat))
        {
            return field_110_state;
        }
        field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
        return AI_SpottedMeat::eState5_Walking_2;
    }

    case AI_SpottedMeat::eState5_Walking_2:
    {
        const short x_exp = FP_GetExponent(field_A8_xpos);
        const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
        if (field_B4_velx < FP_FromInteger(0))
        {
            if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }

        if (field_B4_velx > FP_FromInteger(0))
        {
            if (abs(xSnapped - x_exp) >= 6 && Check_IsOnEndOfLine_4021A0(0, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }

        if (!VIsFacingMe(field_148_pMeat))
        {
            if (FP_Abs(field_148_pMeat->field_A8_xpos - field_A8_xpos) > FP_FromInteger(5))
            {
                Sound_44DBB0(7u);
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_SpottedMeat::eState5_Turning_4;
            }
        }

        if (!VIsObjNearby(field_BC_sprite_scale * FP_FromInteger(40), field_148_pMeat))
        {
            return field_110_state;
        }

        if (field_148_pMeat->field_124_pLine)
        {
            field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
            field_114_timer = gnFrameCount_507670 + field_11C_meat_eating_time;
            return AI_SpottedMeat::eState5_Eating_6;
        }
        return AI_SpottedMeat::eState5_Walking_2;
    }

    case AI_SpottedMeat::eState5_Jumping_3:
        if (field_FC_current_motion != eParamiteStates::State_0_Idle_44B900)
        {
            return field_110_state;
        }
        return AI_SpottedMeat::eState5_Idle_0;

    case AI_SpottedMeat::eState5_Turning_4:
        if (field_FC_current_motion != eParamiteStates::State_5_Turn_44C8E0
            || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_110_state;
        }

        if (!VIsFacingMe(field_148_pMeat))
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_SpottedMeat::eState5_Idle_0;
        }

        if (FP_Abs(field_148_pMeat->field_A8_xpos - field_A8_xpos) > FP_FromInteger(5))
        {
            Sound_44DBB0(7u);
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return 4;
        }
        else
        {
            field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
            return AI_SpottedMeat::eState5_Idle_0;
        }
        break;

    case AI_SpottedMeat::eState5_AttentiveToMeat_5:
        if (!VIsFacingMe(field_148_pMeat))
        {
            Sound_44DBB0(7u);
            field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
            return AI_SpottedMeat::eState5_Turning_4;
        }

        if (VIsObj_GettingNear_On_X(field_148_pMeat))
        {
            return field_110_state;
        }

        if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (Check_IsOnEndOfLine_4021A0(0, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }
        else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (Check_IsOnEndOfLine_4021A0(1, 1))
            {
                ToHop_44B660();
                return AI_SpottedMeat::eState5_Jumping_3;
            }
        }

        if (!VIsObjNearby(kGridSize * FP_FromInteger(3), field_148_pMeat))
        {
            field_FE_next_state = eParamiteStates::State_3_Running_44C070;
            return AI_SpottedMeat::eState5_Running_1;
        }

        if (!VIsObjNearby(field_BC_sprite_scale * FP_FromInteger(40), field_148_pMeat))
        {
            field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
            return AI_SpottedMeat::eState5_Walking_2;
        }

        if (!field_148_pMeat->field_124_pLine || !field_F4_pLine)
        {
            return AI_SpottedMeat::eState5_AttentiveToMeat_5;
        }

        field_FE_next_state = eParamiteStates::State_23_Eating_44B970;
        field_114_timer = gnFrameCount_507670 + field_11C_meat_eating_time;
        return AI_SpottedMeat::eState5_Eating_6;

    case AI_SpottedMeat::eState5_Eating_6:
        if (!VIsObjNearby(field_BC_sprite_scale * FP_FromInteger(40), field_148_pMeat))
        {
            field_FE_next_state = eParamiteStates::State_2_Walking_44B9E0;
            return AI_SpottedMeat::eState5_Walking_2;
        }

        if (!VIsFacingMe(field_148_pMeat))
        {
            if (FP_Abs(field_148_pMeat->field_A8_xpos - field_A8_xpos) > FP_FromInteger(5))
            {
                Sound_44DBB0(7u);
                field_FE_next_state = eParamiteStates::State_5_Turn_44C8E0;
                return AI_SpottedMeat::eState5_Turning_4;
            }
        }

        if (field_114_timer > static_cast<s32>(gnFrameCount_507670))
        {
            return field_110_state;
        }

        field_148_pMeat->field_C_refCount--;
        field_148_pMeat->field_6_flags.Set(Options::eDead_Bit3);
        field_148_pMeat = nullptr;
        field_FE_next_state = eParamiteStates::State_0_Idle_44B900;
        SetBrain(&Paramite::Brain_Patrol_447A10);
        return AI_Patrol::eState0_Inactive_0;

    default:
        return field_110_state;
    }
}

void Paramite::SetBrain(Paramite::TParamiteBrain fn)
{
    ::SetBrain(fn, field_10C_fn, sParamiteAITable);
}

bool Paramite::BrainIs(Paramite::TParamiteBrain fn)
{
    return ::BrainIs(fn, field_10C_fn, sParamiteAITable);
}

s16 Paramite::HandleEnemyStopper(s16 numGridBlocks, Path_EnemyStopper::StopDirection dir)
{
    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    const FP numGridBlocksScaled = (kGridSize * FP_FromInteger(numGridBlocks));
    field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos + numGridBlocksScaled),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(field_A8_xpos + numGridBlocksScaled),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::EnemyStopper_79);

    if (field_F0_pTlv)
    {
        // No stopper or its disabled
        auto pEnemyStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);
        if (!pEnemyStopper || !SwitchStates_Get(pEnemyStopper->field_1A_id))
        {
            return 0;
        }

        // Matches the direction we want?
        if (pEnemyStopper->field_18_direction == dir)
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::State_0_Idle_44B900()
{
    if (!ToNextMotion_44B320()
        && gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        if (BrainIs(&Paramite::Brain_ChasingAbe_449170))
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
        }
        else
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sWalkBeginVelTable_4BBC88[3] =
{
    FP_FromInteger(0),
    FP_FromDouble(1.40),
    FP_FromDouble(9.02)
};

void Paramite::State_1_WalkBegin_44BCA0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = field_BC_sprite_scale * -sWalkBeginVelTable_4BBC88[field_10_anim.field_92_current_frame];
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * sWalkBeginVelTable_4BBC88[field_10_anim.field_92_current_frame];
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        MoveOnLine_44B740();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eParamiteStates::State_2_Walking_44B9E0;
        }
    }
}

const FP sWalkVelTable_4BBC50[14] =
{
    FP_FromDouble(2.03),
    FP_FromDouble(4.02),
    FP_FromDouble(4.46),
    FP_FromDouble(3.34),
    FP_FromDouble(3.18),
    FP_FromDouble(4.93),
    FP_FromDouble(3.12),
    FP_FromDouble(2.50),
    FP_FromDouble(4.44),
    FP_FromDouble(3.96),
    FP_FromDouble(2.90),
    FP_FromDouble(3.64),
    FP_FromDouble(4.70),
    FP_FromDouble(2.72)
};

void Paramite::State_2_Walking_44B9E0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = field_BC_sprite_scale * -sWalkVelTable_4BBC50[field_10_anim.field_92_current_frame];
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * sWalkVelTable_4BBC50[field_10_anim.field_92_current_frame];
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
        return;
    }

    MoveOnLine_44B740();

    if (field_FC_current_motion != eParamiteStates::State_2_Walking_44B9E0)
    {
        return;
    }

    switch (field_10_anim.field_92_current_frame)
    {
    case 0:
        [[fallthrough]];
    case 7:
        Sound_44DBB0(field_10_anim.field_92_current_frame == 7 ? 3 : 4);

        if (field_FE_next_state == eParamiteStates::State_0_Idle_44B900
            || field_FE_next_state == eParamiteStates::State_13_GameSpeakBegin_44D050
            || field_FE_next_state == eParamiteStates::State_5_Turn_44C8E0
            || field_FE_next_state == eParamiteStates::State_25_Death_44DB90
            || field_FE_next_state == eParamiteStates::State_15_Hiss_44D300
            || field_FE_next_state == eParamiteStates::State_23_Eating_44B970)
        {
            field_FC_current_motion = eParamiteStates::State_9_WalkEnd_44BDE0;
        }
        break;

    case 3:
        [[fallthrough]];
    case 10:
        if (field_FE_next_state == eParamiteStates::State_3_Running_44C070)
        {
            field_E4_previous_motion = eParamiteStates::State_3_Running_44C070;
            field_E6_last_anim_frame = 11;
            field_140_use_prev_motion = 1;
            field_FE_next_state = -1;
        }

        if (field_FE_next_state == eParamiteStates::State_18_RunningAttack_44D5D0)
        {
            field_FC_current_motion = eParamiteStates::State_18_RunningAttack_44D5D0;
            field_FE_next_state = -1;
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_B4_velx = ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4);
            }
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe_401D30(1);
            field_142_bSnapped = 1;
        }

        break;

    default:
        field_142_bSnapped = 0;
        break;
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        if (BrainIs(&Paramite::Brain_ChasingAbe_449170))
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
        }
        else
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunningTable_4BBCC0[14] =
{
  FP_FromDouble(5.16),
  FP_FromDouble(5.62),
  FP_FromDouble(3.32),
  FP_FromDouble(3.53),
  FP_FromDouble(5.26),
  FP_FromDouble(10.46),
  FP_FromDouble(9.87),
  FP_FromDouble(8.14),
  FP_FromDouble(8.29),
  FP_FromDouble(7.18),
  FP_FromDouble(6.79),
  FP_FromDouble(8.93),
  FP_FromDouble(11.46),
  FP_FromDouble(5.92)
};

void Paramite::State_3_Running_44C070()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    FP frameVelx = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelx = field_BC_sprite_scale * -sRunningTable_4BBCC0[field_10_anim.field_92_current_frame];
    }
    else
    {
        frameVelx = field_BC_sprite_scale * sRunningTable_4BBCC0[field_10_anim.field_92_current_frame];
    }

    field_B4_velx = frameVelx;

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
        return;
    }

    MoveOnLine_44B740();

    if (field_FC_current_motion != eParamiteStates::State_3_Running_44C070)
    {
        return;
    }

    if (field_10_anim.field_92_current_frame == 3)
    {
        SFX_Play_43AE60(SoundEffect::PickupItem_33, 45, -600, 0);
        if (field_FE_next_state == eParamiteStates::State_2_Walking_44B9E0)
        {
            field_FC_current_motion = eParamiteStates::State_8_WalkRunTransition_44C790;
            field_FE_next_state = -1;
        }

        if (!field_FE_next_state
            || field_FE_next_state == eParamiteStates::State_5_Turn_44C8E0
            || field_FE_next_state == eParamiteStates::State_23_Eating_44B970
            || field_FE_next_state == eParamiteStates::State_25_Death_44DB90
            || field_FE_next_state == eParamiteStates::State_15_Hiss_44D300)
        {
            field_FC_current_motion = eParamiteStates::State_11_RunEnd_44C620;
        }

        if (field_FE_next_state == eParamiteStates::State_18_RunningAttack_44D5D0)
        {
            field_FC_current_motion = eParamiteStates::State_18_RunningAttack_44D5D0;
            field_FE_next_state = -1;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
        }

        if (field_FE_next_state == eParamiteStates::State_6_Hop_44CB20)
        {
            VOnTrapDoorOpen();

            field_E8_LastLineYPos = field_AC_ypos;
            field_FC_current_motion = eParamiteStates::State_6_Hop_44CB20;
            field_FE_next_state = -1;
            field_B8_vely = -FP_FromDouble(7.2) * field_BC_sprite_scale;
            field_AC_ypos += field_B8_vely;
            field_F4_pLine = nullptr;

            if (field_142_bSnapped == 0)
            {
                MapFollowMe_401D30(1);
                field_142_bSnapped = 1;
            }

            SetMusic();
            return;
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe_401D30(1);
            field_142_bSnapped = 1;
        }

        SetMusic();
        return;
    }

    if (field_10_anim.field_92_current_frame == 10)
    {
        Sound_44DBB0(3u);
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);

        if (field_FE_next_state == eParamiteStates::State_2_Walking_44B9E0)
        {
            field_FC_current_motion = eParamiteStates::State_8_WalkRunTransition_44C790;
            field_FE_next_state = -1;
        }

        if (field_FE_next_state == eParamiteStates::State_0_Idle_44B900
            || field_FE_next_state == eParamiteStates::State_5_Turn_44C8E0
            || field_FE_next_state == eParamiteStates::State_23_Eating_44B970
            || field_FE_next_state == eParamiteStates::State_25_Death_44DB90
            || field_FE_next_state == eParamiteStates::State_15_Hiss_44D300)
        {
            field_FC_current_motion = eParamiteStates::State_11_RunEnd_44C620;
        }

        if (field_FE_next_state == eParamiteStates::State_18_RunningAttack_44D5D0)
        {
            field_FC_current_motion = eParamiteStates::State_18_RunningAttack_44D5D0;
            field_FE_next_state = -1;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
            }
        }

        if (field_FE_next_state == eParamiteStates::State_6_Hop_44CB20)
        {
            ToHop_44B660();
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe_401D30(1);
            field_142_bSnapped = 1;
        }

        SetMusic();
        return;
    }

    field_142_bSnapped = 0;
    SetMusic();
}

void Paramite::State_4_Unknown_44B6C0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter_50767C == this)
        {
            field_FC_current_motion = field_E4_previous_motion;
            if (field_F8_pLiftPoint)
            {
                // TODO: Correct type ??
                static_cast<LiftPoint*>(field_F8_pLiftPoint)->field_12C_bMoving |= 1u;
            }
        }
        else
        {
            field_FC_current_motion = field_E4_previous_motion;
            if (field_F8_pLiftPoint)
            {
                field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
                field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
            }
        }
    }
}

void Paramite::State_5_Turn_44C8E0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sound_44DBB0(4u);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (ToNextMotion_44B320())
        {
            field_10_anim.Set_Animation_Data_402A40(field_10_anim.field_18_frame_table_offset, nullptr);
        }
        else
        {
            ToIdle_44B580();
        }
    }
}

const FP sHopVelTable_4BBD28[16] =
{
    FP_FromDouble(5.33),
    FP_FromDouble(5.80),
    FP_FromDouble(3.43),
    FP_FromDouble(3.64),
    FP_FromDouble(5.43),
    FP_FromDouble(10.98),
    FP_FromDouble(10.02),
    FP_FromDouble(5.29),
    FP_FromDouble(5.31),
    FP_FromDouble(4.23),
    FP_FromDouble(4.77),
    FP_FromDouble(6.23),
    FP_FromDouble(5.56),
    FP_FromDouble(7.92),
    FP_FromDouble(10.22),
    FP_FromDouble(5.77)
};


void Paramite::State_6_Hop_44CB20()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    FP frameVelX = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = (field_BC_sprite_scale * -sHopVelTable_4BBD28[field_10_anim.field_92_current_frame]);
    }
    else
    {
        frameVelX = (field_BC_sprite_scale * sHopVelTable_4BBD28[field_10_anim.field_92_current_frame]);
    }

    field_B4_velx = frameVelX;

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(0.9)))
        {
            switch (pLine->field_8_type)
            {
            case 0:
            case 4:
            case 32:
            case 36:
            {
                ToIdle_44B580();

                field_F4_pLine = pLine;

                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);
                bRect.y += 5;
                bRect.h += 5;
                VOnCollisionWith(
                    { bRect.x, bRect.y },
                    { bRect.y, bRect.h },
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                field_A8_xpos = hitX;
                field_AC_ypos = hitY;
                return;
            }

            case 1:
            case 2:
                field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                return;

            default:
                break;
            }
        }

        if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(5))
        {
            field_124_XSpeed = FP_FromDouble(0.75);
            field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
        }

        if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos) >= CameraPos::eCamCurrent_0)
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
        }

    }
}

const FP State_7_Unknown_VelTable_4BBCA8[2] =
{
    FP_FromDouble(2.25),
    FP_FromDouble(5.63)
};

void Paramite::State_7_Unknown_44BF10()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = field_BC_sprite_scale * -State_7_Unknown_VelTable_4BBCA8[field_10_anim.field_92_current_frame];
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * State_7_Unknown_VelTable_4BBCA8[field_10_anim.field_92_current_frame];
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = 3;
        field_E6_last_anim_frame = 2;
        field_140_use_prev_motion = 1;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        MoveOnLine_44B740();
    }
}

const FP sWalkRunTransVelTable_4BBD18[3] =
{
    FP_FromDouble(5.39),
    FP_FromDouble(5.39),
    FP_FromDouble(5.39)
};

void Paramite::State_8_WalkRunTransition_44C790()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = field_BC_sprite_scale * -sWalkRunTransVelTable_4BBD18[field_10_anim.field_92_current_frame];
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * sWalkRunTransVelTable_4BBD18[field_10_anim.field_92_current_frame];
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eParamiteStates::State_2_Walking_44B9E0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        MoveOnLine_44B740();
    }
}

const FP sWalkEndVelTable_4BBC98[3] =
{
    FP_FromDouble(2.33),
    FP_FromDouble(5.03),
    FP_FromDouble(6.70)
};

void Paramite::State_9_WalkEnd_44BDE0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = field_BC_sprite_scale * -sWalkEndVelTable_4BBC98[field_10_anim.field_92_current_frame];
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * sWalkEndVelTable_4BBC98[field_10_anim.field_92_current_frame];
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        MoveOnLine_44B740();
        Event_Broadcast_417220(kEventNoise_0, this);

        if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }
    }

    ToIdle_44B580();
}

const FP sRunBeginVelTable_4BBCF8[3] =
{
    FP_FromDouble(1.87),
    FP_FromDouble(2.15),
    FP_FromDouble(3.33)
};

void Paramite::State_10_RunBegin_44C4C0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sRunBeginVelTable_4BBCF8[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * sRunBeginVelTable_4BBCF8[field_10_anim.field_92_current_frame];
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_E4_previous_motion = 3;
        field_E6_last_anim_frame = 2;
        field_140_use_prev_motion = 1;
    }

    Event_Broadcast_417220(kEventNoise_0, this);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        MoveOnLine_44B740();
    }
}

const FP sRunEndVelTable_4BBD08[3] =
{
    FP_FromDouble(3.11),
    FP_FromDouble(3.06),
    FP_FromDouble(3.43)
};

void Paramite::State_11_RunEnd_44C620()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = field_BC_sprite_scale * -sRunEndVelTable_4BBD08[field_10_anim.field_92_current_frame];
    }
    else
    {
        field_B4_velx = field_BC_sprite_scale * sRunEndVelTable_4BBD08[field_10_anim.field_92_current_frame];
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44B580();
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        ToKnockBack_44B5B0();
    }
    else
    {
        MoveOnLine_44B740();
    }
}

void Paramite::State_12_Falling_44C960()
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= (field_BC_sprite_scale * field_124_XSpeed);
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += (field_BC_sprite_scale * field_124_XSpeed);
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->field_8_type)
        {
        case 0:
        case 4:
        case 32:
        case 36:
        {
            ToIdle_44B580();

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
            field_AC_ypos = hitY;
            MapFollowMe_401D30(TRUE);
            break;
        }

        case 1:
        case 2:
            field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
            break;

        default:
            return;
        }
    }
}

void Paramite::State_13_GameSpeakBegin_44D050()
{
    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    if (abeRect.x <= ourRect.w
        && abeRect.w >= ourRect.x
        && abeRect.h >= ourRect.y
        && abeRect.y <= ourRect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            if (field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
            {
                if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
                {
                    SFX_Play_43AD70(SoundEffect::KillEffect_78, 0, 0);
                    Mudokon_SFX_42A4D0(MudSounds::eKnockbackOuch_10, 0, 0, sActiveHero_507678);
                }
                sActiveHero_507678->VTakeDamage(this);
            }
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eParamiteStates::State_14_PreHiss_44D170;
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) == CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Paramite::State_14_PreHiss_44D170()
{
    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            if (field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
            {
                sActiveHero_507678->VTakeDamage(this);
            }
        }
    }

    if (field_FE_next_state == eParamiteStates::State_18_RunningAttack_44D5D0)
    {
        field_FC_current_motion = eParamiteStates::State_18_RunningAttack_44D5D0;
        field_FE_next_state = -1;
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
    }

    if (field_FE_next_state == eParamiteStates::State_22_Unknown_44D8F0)
    {
        field_FC_current_motion = eParamiteStates::State_22_Unknown_44D8F0;
        field_FE_next_state = -1;
    }

    if (field_FE_next_state == eParamiteStates::State_15_Hiss_44D300)
    {
        field_FC_current_motion = eParamiteStates::State_15_Hiss_44D300;
        field_FE_next_state = -1;
    }

    if (field_FE_next_state != -1)
    {
        field_FC_current_motion = eParamiteStates::State_17_GameSpeakEnd_44D4F0;
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) == CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Paramite::State_15_Hiss_44D300()
{
    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if (field_10_anim.field_92_current_frame == 2)
    {
        Sound_44DBB0(1u);
    }

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            if (field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
            {
                sActiveHero_507678->VTakeDamage(this);
            }
        }
    }

    if (field_FE_next_state == eParamiteStates::State_18_RunningAttack_44D5D0)
    {
        field_FC_current_motion = eParamiteStates::State_18_RunningAttack_44D5D0;
        field_FE_next_state = -1;
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(4));
        }
    }
    else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eParamiteStates::State_16_PostHiss_44D440;
    }
}

void Paramite::State_16_PostHiss_44D440()
{
    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            if (field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
            {
                sActiveHero_507678->VTakeDamage(this);
            }
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eParamiteStates::State_14_PreHiss_44D170;
    }
}

void Paramite::State_17_GameSpeakEnd_44D4F0()
{
    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero_507678))
        {
            if (field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
            {
                sActiveHero_507678->VTakeDamage(this);
            }
        }
    }
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_44B320())
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);
            field_124_XSpeed = FP_FromInteger(0);
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;
            MapFollowMe_401D30(TRUE);
        }
    }
}

void Paramite::State_18_RunningAttack_44D5D0()
{
    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(10), field_B4_velx))
    {
        field_F4_pLine = nullptr;

        ToKnockBack_44B5B0();
        return;
    }

    field_A8_xpos += field_B4_velx;
    MapFollowMe_401D30(FALSE);

    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h
        && field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
    {
        sActiveHero_507678->VTakeDamage(this);
    }

    if (field_10_anim.field_92_current_frame == 3)
    {
        Sound_44DBB0(0);
    }

    if (field_10_anim.field_92_current_frame == 11)
    {
        FP gridSizeDirected = {};
        if (field_B4_velx >= FP_FromInteger(0))
        {
            gridSizeDirected = ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            gridSizeDirected = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos + gridSizeDirected,
            field_AC_ypos - FP_FromInteger(10),
            field_A8_xpos + gridSizeDirected,
            field_AC_ypos + FP_FromInteger(10),
            &pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
        {
            if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
            {
                PSX_RECT r = {};
                VGetBoundingRect(&r, 1);
                r.y += 5;
                r.h += 5;

                VOnCollisionWith(
                    { r.x, r.y },
                    { r.w, r.h },
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
            field_AC_ypos = hitY;
            field_F4_pLine = pLine;
        }
        else
        {
            field_F4_pLine = nullptr;
            field_FC_current_motion = eParamiteStates::State_12_Falling_44C960;
            field_124_XSpeed = FP_FromDouble(0.75);
            field_AC_ypos -= (field_BC_sprite_scale * FP_FromInteger(10));
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44B580();
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Paramite::State_19_Empty_44D990()
{
    // Empty
}

void Paramite::State_20_SurpriseWeb_44D9A0()
{
    FP hitX = {};
    FP hitY = {};

    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_B8_vely + field_AC_ypos,
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10) == 1)
    {
        field_AC_ypos = hitY;
        field_FC_current_motion = eParamiteStates::State_21_WebLeave_44DB00;

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
    else
    {
        field_AC_ypos += field_B8_vely;
    }

    if (field_10_anim.field_92_current_frame == 0 || field_10_anim.field_92_current_frame == 3)
    {
        Sound_44DBB0(6u);
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Paramite::State_21_WebLeave_44DB00()
{
    if (field_10_anim.field_92_current_frame == 2)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_44B580();
    }
}

void Paramite::State_22_Unknown_44D8F0()
{
    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h
        && field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
    {
        sActiveHero_507678->VTakeDamage(this);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eParamiteStates::State_14_PreHiss_44D170;
    }
}

void Paramite::State_23_Eating_44B970()
{
    if (field_10_anim.field_92_current_frame == 5)
    {
        SFX_Play_43AD70(static_cast<u8>(Math_RandomRange_450F20(79, 80)), 0, 0);
    }

    if (field_FE_next_state != -1 && !ToNextMotion_44B320())
    {
        ToIdle_44B580();
    }
}

void Paramite::State_24_Struggle_44DB70()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        SFX_Play_43AD70(SoundEffect::KillEffect_78, 0, 0);
    }
}

void Paramite::State_25_Death_44DB90()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sound_44DBB0(2u);
    }
}

}


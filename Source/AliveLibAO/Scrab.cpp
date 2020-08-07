#include "stdafx_ao.h"
#include "Scrab.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "VRam.hpp"
#include "DDCheat.hpp"
#include "Shadow.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "MusicController.hpp"
#include "PlatformBase.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "CameraSwapper.hpp"
#include "LiftPoint.hpp"

void Scrab_ForceLink() {}

START_NS_AO

using TScrabStateFunction = decltype(&Scrab::State_0_Empty_45E3D0);

const TScrabStateFunction sScrabMotionTable_4CF690[] =
{
    &Scrab::State_0_Empty_45E3D0,
    &Scrab::State_1_Stand_45E620,
    &Scrab::State_2_Walk_45E730,
    &Scrab::State_3_Run_45EAB0,
    &Scrab::State_4_Turn_45EF30,
    &Scrab::State_5_RunToStand_45ED90,
    &Scrab::State_6_HopBegin_45F3C0,
    &Scrab::State_7_HopMidair_45F1A0,
    &Scrab::State_8_Land_45F500,
    &Scrab::State_9_JumpToFall_45EFD0,
    &Scrab::State_10_StandToWalk_45E670,
    &Scrab::State_11_StandToRun_45E9F0,
    &Scrab::State_12_WalkToStand_45E930,
    &Scrab::State_13_RunJumpBegin_45F5D0,
    &Scrab::State_14_RunJumpEnd_45F850,
    &Scrab::State_15_ToFall_45F180,
    &Scrab::State_16_Stamp_45F920,
    &Scrab::State_17_Empty_45F9C0,
    &Scrab::State_18_GetEaten_45FF70,
    &Scrab::State_19_Unused_45F9D0,
    &Scrab::State_20_HowlBegin_45FA60,
    &Scrab::State_21_HowlEnd_45FAF0,
    &Scrab::State_22_Shriek_45FB00,
    &Scrab::State_23_ScrabBattleAnim_45FBA0,
    &Scrab::State_24_FeedToGulp_45FC30,
    &Scrab::State_25_ToFeed_45FCE0,
    &Scrab::State_26_Feed_45FDA0,
    &Scrab::State_27_AttackLunge_45FDF0,
    &Scrab::State_28_LegKick_45FF60,
    &Scrab::State_29_DeathBegin_45FFA0,
};

const int sScrabFrameTables_4CF708[30] =
{
    168644,
    168644,
    168548,
    168676,
    168740,
    168796,
    168844,
    168868,
    168908,
    168644,
    168932,
    168952,
    168972,
    168868,
    168908,
    168992,
    24136,
    1324,
    1324,
    11060,
    16944,
    17012,
    11380,
    30940,
    26664,
    19544,
    19600,
    8212,
    14228,
    12724
};

Scrab* Scrab::ctor_45B5F0(Path_Scrab* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BC710);

    field_4_typeId = Types::eScrab_77;
    
    for (int i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        field_150_resources[i] = nullptr;
    }

    field_150_resources[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 700, 1, 0);
    field_150_resources[11] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 711, 1, 0);
    field_150_resources[6] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 706, 1, 0);
    field_150_resources[8] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 708, 1, 0);
    field_150_resources[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 701, 1, 0);
    field_150_resources[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 710, 1, 0);
    field_150_resources[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 705, 1, 0);
    field_150_resources[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 702, 1, 0);
    field_150_resources[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 709, 1, 0);
    field_150_resources[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 703, 1, 0);
    field_150_resources[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 704, 1, 0);
    field_150_resources[13] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 713, 1, 0);
    
    Animation_Init_417FD0(
        168644,
        168,
        69,
        field_150_resources[0],
        1);

    
    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_132_res_block_idx = 0;
    field_118_timer = 0;
    
    field_10C_fn = &Scrab::Brain_460020;

    field_110_brain_ret = 0;
    field_FE_next_state = 0;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = 1;
    field_112 = 0;

    field_11C_pObj1 = nullptr;
    field_120_pObj2 = nullptr;

    field_140 = 0;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
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

    field_114_attack_delay = pTlv->field_1A_attack_delay;
    field_116_patrol_type = pTlv->field_1C_patrol_type;
    field_144_left_min_delay = pTlv->field_1E_left_min_delay;
    field_146_left_max_delay = pTlv->field_20_left_max_delay;
    field_148_right_min_delay = pTlv->field_22_right_min_delay;
    field_14A_right_max_delay = pTlv->field_24_right_max_delay;

    field_138_attack_duration = pTlv->field_26_attack_duration;

    field_188_flags = 32 * ( pTlv->field_2A_roar_randomly & 1) | (field_188_flags & ~0x11 | 4) & ~0x28;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(30),
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10) == 1)
    {
        field_AC_ypos = hitY;
        ToStand_45E310();

        if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
        {
            sub_45E580();
        }

        field_188_flags |= 8u;
    }

    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_C0_r = 127;
    field_C2_g = 127;
    field_C4_b = 127;

    field_130 = 0;

    field_134_tlvInfo = tlvInfo;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* Scrab::dtor_45BA50()
{
    SetVTable(this, 0x4BC710);

    if (field_11C_pObj1)
    {
        field_11C_pObj1->field_C_refCount--;
    }

    if (field_120_pObj2)
    {
        field_120_pObj2->field_C_refCount--;
    }

    VOnTrapDoorOpen_45E5E0();

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

    if (field_100_health <= FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_134_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_134_tlvInfo, -1, 0, 0);
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);

    return dtor_401000();
}

BaseGameObject* Scrab::VDestructor(signed int flags)
{
    return Vdtor_45C310(flags);
}

Scrab* Scrab::Vdtor_45C310(signed int flags)
{
    dtor_45BA50();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Scrab::VUpdate_45B360()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!(field_188_flags & 8))
    {
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos + FP_FromInteger(30),
            &field_F4_pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10) == 1)
        {
            field_AC_ypos = hitY;

            ToStand_45E310();

            if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
            {
                sub_45E580();
            }

            field_188_flags |= 8u;
        }
    }
    
    const FP hero_xd = FP_Abs(field_A8_xpos - sActiveHero_507678->field_A8_xpos);
    const FP hero_yd = FP_Abs(field_AC_ypos - sActiveHero_507678->field_AC_ypos);

    if (hero_xd > FP_FromInteger(2048) || hero_yd > FP_FromInteger(960))
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        if (field_10_anim.field_84_vram_rect.w)
        {
            Vram_free_450CE0(
                PSX_Point{ field_10_anim.field_84_vram_rect.x, field_10_anim.field_84_vram_rect.y },
                PSX_Point{ field_10_anim.field_84_vram_rect.w, field_10_anim.field_84_vram_rect.h });
            field_10_anim.field_84_vram_rect.w = 0;
        }
    }
    else
    {
        if (field_100_health > FP_FromInteger(0))
        {
            if (!field_10_anim.field_84_vram_rect.w)
            {
                vram_alloc_450B20(168, 69, 8u, &field_10_anim.field_84_vram_rect);
            }
            field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
        }
        
        const auto old_motion = field_FC_current_motion;

        field_110_brain_ret = (this->*field_10C_fn)();
     
        if (!word_5076E0)
        {
            DebugOut_495990(
                "Scrab %d %d %d %d\n",
                field_110_brain_ret,
                field_118_timer,
                field_FC_current_motion,
                field_FE_next_state);
        }


        const FP old_x = field_A8_xpos;
        const FP old_y = field_AC_ypos;

        (this->*sScrabMotionTable_4CF690[field_FC_current_motion])();

        if (old_x != field_A8_xpos || old_y != field_AC_ypos)
        {
            field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
                nullptr,
                field_A8_xpos,
                field_AC_ypos,
                field_A8_xpos,
                field_AC_ypos);
            VOn_TLV_Collision(field_F0_pTlv);
        }

        if (old_motion != field_FC_current_motion || field_188_flags & 0x10)
        {
            field_188_flags &= ~0x10u;
            VUpdateResBlock_45B330();

            if (old_motion != eScrabStates::State_0_Empty_45E3D0)
            {
                field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            }
        }
        else if (field_112)
        {
            field_FC_current_motion = field_E4;
            VUpdateResBlock_45B330();
            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            field_112 = 0;
        }
    }
}

void Scrab::ToStand_45E310()
{
    field_128 = 0;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
    MapFollowMe_401D30(1);
}

void Scrab::VUpdateResBlock_45B330()
{
    field_10_anim.Set_Animation_Data_402A40(
        sScrabFrameTables_4CF708[field_FC_current_motion],
        ResBlockForMotion_45BB30(field_FC_current_motion));
}

BYTE** Scrab::ResBlockForMotion_45BB30(__int16 motion)
{
    if (motion < eScrabStates::State_16_Stamp_45F920)
    {
        field_132_res_block_idx = 0;
    }
    else if (motion < eScrabStates::State_17_Empty_45F9C0)
    {
        field_132_res_block_idx = 6;
    }
    else if (motion < eScrabStates::State_19_Unused_45F9D0)
    {
        field_132_res_block_idx = 8;
    }
    else if (motion < eScrabStates::State_20_HowlBegin_45FA60)
    {
        field_132_res_block_idx = 10;
    }
    else if (motion < eScrabStates::State_22_Shriek_45FB00)
    {
        field_132_res_block_idx = 5;
    }
    else if (motion < eScrabStates::State_23_ScrabBattleAnim_45FBA0)
    {
        field_132_res_block_idx = 9;
    }
    else if (motion < eScrabStates::State_24_FeedToGulp_45FC30)
    {
        field_132_res_block_idx = 4;
    }
    else if (motion < eScrabStates::State_25_ToFeed_45FCE0)
    {
        field_132_res_block_idx = 11;
    }
    else if (motion < eScrabStates::State_27_AttackLunge_45FDF0)
    {
        field_132_res_block_idx = 1;
    }
    else if (motion < eScrabStates::State_28_LegKick_45FF60)
    {
        field_132_res_block_idx = 2;
    }
    else if (motion >= eScrabStates::State_29_DeathBegin_45FFA0)
    {
        field_132_res_block_idx = motion >= 30 ? 0 : 13;
    }
    else
    {
        field_132_res_block_idx = 3;
    }
    return field_150_resources[field_132_res_block_idx];
}

void Scrab::sub_45E580()
{
    NOT_IMPLEMENTED();
}


void Scrab::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_45E5E0();
}

void Scrab::VOnTrapDoorOpen_45E5E0()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;

        field_FC_current_motion = eScrabStates::State_15_ToFall_45F180;

        field_188_flags |= 0x10u;
    }
}

__int16 Scrab::sub_45DFB0()
{
    MapFollowMe_401D30(1);

    switch (field_FE_next_state)
    {
    case eScrabStates::State_4_Turn_45EF30:
    case eScrabStates::State_16_Stamp_45F920:
    case eScrabStates::State_19_Unused_45F9D0:
    case eScrabStates::State_20_HowlBegin_45FA60:
    case eScrabStates::State_22_Shriek_45FB00:
    case eScrabStates::State_23_ScrabBattleAnim_45FBA0:
    case eScrabStates::State_27_AttackLunge_45FDF0:
    case eScrabStates::State_28_LegKick_45FF60:
    case eScrabStates::State_25_ToFeed_45FCE0:
        field_FC_current_motion = field_FE_next_state;
        field_FE_next_state = -1;
        return 1;

    case eScrabStates::State_3_Run_45EAB0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            else
            {
                field_FC_current_motion = eScrabStates::State_11_StandToRun_45E9F0;
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromDouble(3.5));
                field_FE_next_state = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromDouble(3.5));
                field_FC_current_motion = eScrabStates::State_11_StandToRun_45E9F0;
                field_FE_next_state = -1;
                return 1;
            }
        }
        break;

    case eScrabStates::State_2_Walk_45E730:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            else
            {
                field_FC_current_motion = eScrabStates::State_10_StandToWalk_45E670;
                field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(7));
                field_FE_next_state = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
            else
            {
                field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(7));
                field_FC_current_motion = eScrabStates::State_10_StandToWalk_45E670;
                field_FE_next_state = -1;
                return 1;
            }
        }
        break;

    case eScrabStates::State_1_Stand_45E620:
        field_128 = 0;
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
        MapFollowMe_401D30(1);
        return 1;

    case eScrabStates::State_7_HopMidair_45F1A0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), -ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
        }
        else
        {
            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), ScaleToGridSize_41FA30(field_BC_sprite_scale)))
            {
                return 0;
            }
        }
        field_FC_current_motion = eScrabStates::State_6_HopBegin_45F3C0;
        field_FE_next_state = -1;
        return 1;

    default:
        return 0;
    }
}

int Scrab::Sfx_460B80(unsigned __int8 /*idx*/, int /*a3*/, int /*a4*/, __int16 /*a5*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void Scrab::ToJump_45E340()
{
    field_E8_LastLineYPos = field_AC_ypos;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(-5.7));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(5.7));
    }

    field_B8_vely = (field_BC_sprite_scale * FP_FromDouble(-9.6));
    field_AC_ypos += field_B8_vely;
    VOnTrapDoorOpen();
    field_FC_current_motion = eScrabStates::State_13_RunJumpBegin_45F5D0;
    field_F4_pLine = nullptr;
}

void Scrab::State_0_Empty_45E3D0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter_50767C == this)
        {
            field_FC_current_motion = field_E4;
            if (field_F8_pLiftPoint)
            {
                // TODO: Is type of field_F8_pLiftPoint too low?
                static_cast<LiftPoint*>(field_F8_pLiftPoint)->field_12C |= 1u;
            }
        }
        else
        {
            field_FC_current_motion = field_E4;
            if (field_F8_pLiftPoint)
            {
                field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
                field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
            }
        }
    }
}

void Scrab::State_1_Stand_45E620()
{
    sub_45DFB0();

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::State_2_Walk_45E730()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_3_Run_45EAB0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_4_Turn_45EF30()
{
    if (field_10_anim.field_92_current_frame == 5
        || field_10_anim.field_92_current_frame == 9
        || field_10_anim.field_92_current_frame == 11)
    {
        Sfx_460B80(6u, Math_RandomRange_450F20(40, 50), 0x7FFF, 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (sub_45DFB0())
        {
            field_10_anim.Set_Animation_Data_402A40(field_10_anim.field_18_frame_table_offset, nullptr);
        }
        else
        {
            field_128 = 0;
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
            MapFollowMe_401D30(1);
        }
    }
}

void Scrab::State_5_RunToStand_45ED90()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_6_HopBegin_45F3C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_7_HopMidair_45F1A0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_8_Land_45F500()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_9_JumpToFall_45EFD0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_10_StandToWalk_45E670()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_11_StandToRun_45E9F0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_12_WalkToStand_45E930()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_13_RunJumpBegin_45F5D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_14_RunJumpEnd_45F850()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_15_ToFall_45F180()
{
    State_9_JumpToFall_45EFD0();

    if (field_FC_current_motion == eScrabStates::State_1_Stand_45E620)
    {
        field_FC_current_motion = eScrabStates::State_8_Land_45F500;
    }
}

void Scrab::State_16_Stamp_45F920()
{
    if (field_10_anim.field_92_current_frame == 9)
    {
        Sfx_460B80(4u, 0, 0x7FFF, 1);
        SFX_Play_43AE60(78u, 60, Math_RandomRange_450F20(-255, 255), 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Sfx_460B80(0, 60, 511, 1);
        sub_45DFB0();
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::State_17_Empty_45F9C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_18_GetEaten_45FF70()
{
    if (!field_F4_pLine)
    {
        State_9_JumpToFall_45EFD0();

        if (field_FC_current_motion != eScrabStates::State_18_GetEaten_45FF70)
        {
            field_FC_current_motion = eScrabStates::State_18_GetEaten_45FF70;
        }
    }
}

void Scrab::State_19_Unused_45F9D0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!sub_45DFB0())
        {
            field_128 = 0;
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
            MapFollowMe_401D30(1);
        }
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::State_20_HowlBegin_45FA60()
{
    if (field_10_anim.field_92_current_frame == 2)
    {
        Sfx_460B80(8u, 0, Math_RandomRange_450F20(-1600, -900), 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_state != -1)
        {
            field_FC_current_motion = eScrabStates::State_21_HowlEnd_45FAF0;
        }
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::State_21_HowlEnd_45FAF0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        sub_45DFB0();
    }
}

void Scrab::State_22_Shriek_45FB00()
{
    if (field_10_anim.field_92_current_frame == 4)
    {
        Sfx_460B80(0, 0, 0x7FFF, 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!sub_45DFB0())
        {
            field_128 = 0;
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
            MapFollowMe_401D30(1);
        }
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::State_23_ScrabBattleAnim_45FBA0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        field_14C = Sfx_460B80(5u, 100, Math_RandomRange_450F20(-600, 200), 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        sub_45DFB0();
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::State_24_FeedToGulp_45FC30()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_FE_next_state == eScrabStates::State_25_ToFeed_45FCE0)
        {
            field_188_flags &= ~4u;
            field_FC_current_motion = eScrabStates::State_25_ToFeed_45FCE0;
            field_FE_next_state = -1;
        }
        else
        {
            field_128 = 0;
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_FC_current_motion = 1;
            MapFollowMe_401D30(1);
        }
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::State_25_ToFeed_45FCE0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_26_Feed_45FDA0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        if (Math_RandomRange_450F20(0, 100) >= 50)
        {
            SFX_Play_43AD70(80u, 0, 0);
        }
        else
        {
            SFX_Play_43AD70(79u, 0, 0);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eScrabStates::State_24_FeedToGulp_45FC30;
    }
}

void Scrab::State_27_AttackLunge_45FDF0()
{
    NOT_IMPLEMENTED();
}

void Scrab::State_28_LegKick_45FF60()
{
    State_27_AttackLunge_45FDF0();
}

void Scrab::State_29_DeathBegin_45FFA0()
{
    NOT_IMPLEMENTED();
}

__int16 Scrab::Brain_45C370()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Scrab::Brain_460020()
{
    NOT_IMPLEMENTED();
    return 0;
}

END_NS_AO


#include "stdafx_ao.h"
#include "Scrab.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Events.hpp"
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
#include "CameraSwapper.hpp"
#include "LiftPoint.hpp"
#include "Game.hpp"
#include "FixedPoint.hpp"
#include "Gibs.hpp"
#include "Particle.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"

void Scrab_ForceLink()
{ }

namespace AO {

using TScrabStateFunction = decltype(&Scrab::State_0_Empty_45E3D0);

const TScrabStateFunction sScrabMotionTable_4CF690[] = {
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

const s32 sScrabFrameTables_4CF708[30] = {
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
    12724};

static AIFunctionData<Scrab::TBrainType> sScrabAITable[]{
    {&Scrab::Brain_Fighting_45C370, 0x45C370, "Brain_Fighting_45C370"},
    {&Scrab::Brain_BatDeath_45CA60, 0x45CA60, "Brain_BatDeath_45CA60"},
    {&Scrab::Brain_Death_45CB80, 0x45CB80, "Brain_Death_45CB80"},
    {&Scrab::Brain_ChasingEnemy_45CC90, 0x45CC90, "Brain_ChasingEnemy_45CC90"},
    {&Scrab::Brain_Patrol_460020, 0x460020, "Brain_Patrol_460020"},
    {&Scrab::Brain_WalkAround_460D80, 0x460D80, "Brain_460D80"},
};

Scrab* Scrab::ctor_45B5F0(Path_Scrab* pTlv, s32 tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BC710);

    field_4_typeId = Types::eScrab_77;

    for (s32 i = 0; i < ALIVE_COUNTOF(field_150_resources); i++)
    {
        field_150_resources[i] = nullptr;
    }

    field_150_resources[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsbasicResID, 1, 0);
    field_150_resources[11] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArschewResID, 1, 0);
    field_150_resources[6] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsdanceResID, 1, 0);
    field_150_resources[8] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsdeadResID, 1, 0);
    field_150_resources[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArseatResID, 1, 0);
    field_150_resources[10] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsgrwlResID, 1, 0);
    field_150_resources[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArshowlResID, 1, 0);
    field_150_resources[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsprceResID, 1, 0);
    field_150_resources[9] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsroarResID, 1, 0);
    field_150_resources[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArsskwrResID, 1, 0);
    field_150_resources[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArswhirlResID, 1, 0);
    field_150_resources[13] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kArscrshResID, 1, 0);

    Animation_Init_417FD0(
        168644,
        168,
        69,
        field_150_resources[0],
        1);


    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_132_res_block_idx = 0;
    field_118_timer = 0;

    SetBrain(&Scrab::Brain_Patrol_460020);

    field_110_brain_ret = 0;
    field_FE_next_state = 0;
    field_F8_pLiftPoint = nullptr;
    field_FC_current_motion = 1;
    field_112 = 0;

    field_11C_pFight_target = nullptr;
    field_120_pTarget = nullptr;

    field_140_last_shriek_timer = 0;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == 1)
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

    field_114_attack_delay = pTlv->field_1A_attack_delay;
    field_116_patrol_type = pTlv->field_1C_patrol_type;
    field_144_left_min_delay = pTlv->field_1E_left_min_delay;
    field_146_left_max_delay = pTlv->field_20_left_max_delay;
    field_148_right_min_delay = pTlv->field_22_right_min_delay;
    field_14A_right_max_delay = pTlv->field_24_right_max_delay;

    field_138_attack_duration = pTlv->field_26_attack_duration;

    field_188_flags = 32 * (pTlv->field_2A_roar_randomly & 1) | (((field_188_flags & ~0x11) | 4) & ~0x28);

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
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10)
        == 1)
    {
        field_AC_ypos = hitY;
        ToStand_45E310();

        if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
        {
            PlatformCollide_45E580();
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

    if (field_11C_pFight_target)
    {
        field_11C_pFight_target->field_C_refCount--;
    }

    if (field_120_pTarget)
    {
        field_120_pTarget->field_C_refCount--;
    }

    VOnTrapDoorOpen_45E5E0();

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

    if (field_100_health <= FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_134_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_134_tlvInfo, -1, 0, 0);
    }

    MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);

    return dtor_401000();
}

BaseGameObject* Scrab::VDestructor(s32 flags)
{
    return Vdtor_45C310(flags);
}

void Scrab::VRender(PrimHeader** ppOt)
{
    VRender_45BBF0(ppOt);
}

void Scrab::VRender_45BBF0(PrimHeader** ppOt)
{
    if (field_8_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

Scrab* Scrab::Vdtor_45C310(s32 flags)
{
    dtor_45BA50();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Scrab::VUpdate()
{
    VUpdate_45B360();
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
                field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10)
            == 1)
        {
            field_AC_ypos = hitY;

            ToStand_45E310();

            if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
            {
                PlatformCollide_45E580();
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
                PSX_Point{field_10_anim.field_84_vram_rect.x, field_10_anim.field_84_vram_rect.y},
                PSX_Point{field_10_anim.field_84_vram_rect.w, field_10_anim.field_84_vram_rect.h});
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

        if (showDebugCreatureInfo_5076E0)
        {
            DDCheat::DebugStr_495990(
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
            vUpdateAnim_45B330();

            if (old_motion != eScrabStates::State_0_Empty_45E3D0)
            {
                field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            }
        }
        else if (field_112)
        {
            field_FC_current_motion = field_E4_previous_motion;
            vUpdateAnim_45B330();
            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            field_112 = 0;
        }
    }
}


s16 Scrab::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_45BC10(pFrom);
}

s16 Scrab::VTakeDamage_45BC10(BaseGameObject* pFrom)
{
    if (field_100_health > FP_FromInteger(0))
    {
        switch (pFrom->field_4_typeId)
        {
            case Types::eBat_6:
                if (BrainIs(&Scrab::Brain_BatDeath_45CA60))
                {
                    return 1;
                }

                field_100_health = FP_FromInteger(0);
                field_FE_next_state = 1;
                SetBrain(&Scrab::Brain_BatDeath_45CA60);
                field_110_brain_ret = 0;
                return 1;

            case Types::eBullet_10:
            case Types::eRollingBall_72:
                field_100_health = FP_FromInteger(0);
                SetBrain(&Scrab::Brain_Death_45CB80);
                field_130 = 2;
                field_118_timer = gnFrameCount_507670 + 90;
                field_FC_current_motion = eScrabStates::State_29_DeathBegin_45FFA0;
                vUpdateAnim_45B330();
                break;

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
                return 1;
            }

            case Types::eAbilityRing_69:
                return 0;

            default:
                SFX_Play_43AD70(SoundEffect::KillEffect_78, 127, 0);
                SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 90, 0);
                field_100_health = FP_FromInteger(0);
                SetBrain(&Scrab::Brain_Death_45CB80);
                field_130 = 2;
                field_118_timer = gnFrameCount_507670 + 90;
                field_FC_current_motion = eScrabStates::State_29_DeathBegin_45FFA0;
                vUpdateAnim_45B330();
                break;
        }
    }
    return 1;
}

void Scrab::VOn_TLV_Collision_45BDC0(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
        {
            Scrab_SFX_460B80(ScrabSounds::eYell_8, 127, -1000, 0);
            field_6_flags.Set(Options::eDead_Bit3);
            field_100_health = FP_FromInteger(0);
            field_130 = 2;
        }

        pTlv = gMap_507BA8.TLV_Get_At_446060(
            pTlv,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);
    }
}

void Scrab::VScreenChanged()
{
    VScreenChanged_45C290();
}

void Scrab::VScreenChanged_45C290()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        if (field_120_pTarget)
        {
            if (field_120_pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                field_120_pTarget->field_C_refCount--;
                field_120_pTarget = nullptr;
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                SetBrain(&Scrab::Brain_WalkAround_460D80);
                field_110_brain_ret = 0;
            }
        }
    }
}

void Scrab::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOn_TLV_Collision_45BDC0(pTlv);
}

void Scrab::ToStand_45E310()
{
    field_128 = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
    MapFollowMe_401D30(1);
}

void Scrab::vUpdateAnim_45B330()
{
    field_10_anim.Set_Animation_Data_402A40(
        sScrabFrameTables_4CF708[field_FC_current_motion],
        ResBlockForMotion_45BB30(field_FC_current_motion));
}

u8** Scrab::ResBlockForMotion_45BB30(s16 motion)
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

void Scrab::PlatformCollide_45E580()
{
    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    bRect.y += 5;
    bRect.h += 5;

    VOnCollisionWith(
        {bRect.x, bRect.y},
        {bRect.w, bRect.h},
        ObjListPlatforms_50766C,
        1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
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

s16 Scrab::ToNextMotion_45DFB0()
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
            ToStand();
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

void Scrab::ToStand()
{
    field_128 = FP_FromInteger(0);
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
    MapFollowMe_401D30(1);
}

SfxDefinition sScrabSfx_4CF798[9] = {
    {0, 39, 60, 55, 0, 0, 0},
    {0, 39, 61, 70, 0, 0, 0},
    {0, 39, 62, 80, 0, 0, 0},
    {0, 39, 63, 80, 0, 0, 0},
    {0, 39, 64, 60, -127, 127, 0},
    {0, 39, 66, 90, 0, 0, 0},
    {0, 39, 67, 50, -511, 0, 0},
    {0, 39, 67, 50, 0, 511, 0},
    {0, 39, 68, 110, -1791, -1791, 0}};

s32 Scrab::Scrab_SFX_460B80(ScrabSounds soundId, s32 /*vol*/, s32 pitch, s16 applyDirection)
{
    s32 volumeLeft = 0;
    s32 volumeRight = 0;

    auto defaultSndIdxVol = sScrabSfx_4CF798[static_cast<s32>(soundId)].field_C_default_volume;
    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos);
    PSX_RECT worldRect;
    gMap_507BA8.Get_Camera_World_Rect_444C30(direction, &worldRect);
    volumeLeft = volumeRight;
    if (applyDirection)
    {
        switch (direction)
        {
            case CameraPos::eCamCurrent_0:
            {
                break;
            }
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
            {
                volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
                volumeRight = volumeLeft;
            }
            break;
            case CameraPos::eCamLeft_3:
            {
                FP percentHowFar = (FP_FromInteger(worldRect.w) - field_A8_xpos) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                break;
            }
            case CameraPos::eCamRight_4:
            {
                FP percentHowFar = (field_A8_xpos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                break;
            }
            default:
                return 0;
        }
    }
    return SFX_SfxDefinition_Play_477330(&sScrabSfx_4CF798[static_cast<s32>(soundId)],
                                         static_cast<s16>(volumeLeft),
                                         static_cast<s16>(volumeRight),
                                         static_cast<s16>(pitch),
                                         static_cast<s16>(pitch));
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

void Scrab::MoveOnLine_45E450()
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
                    PlatformCollide_45E580();
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_E8_LastLineYPos = field_AC_ypos;
            field_FC_current_motion = eScrabStates::State_15_ToFall_45F180;
            field_A8_xpos = field_B4_velx + oldX;
            field_128 = FP_FromInteger(1);
        }
    }
    else
    {
        field_FC_current_motion = eScrabStates::State_9_JumpToFall_45EFD0;
        field_E8_LastLineYPos = field_AC_ypos;
    }
}

s16 Scrab::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VOnSameYLevel_45C180(pOther);
}


s16 Scrab::VOnSameYLevel_45C180(BaseAnimatedWithPhysicsGameObject* pObj)
{
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    PSX_RECT otherRect = {};
    pObj->VGetBoundingRect(&otherRect, 1);

    const FP k10Scaled = (FP_FromInteger(10) * field_BC_sprite_scale);
    if (FP_FromInteger(ourRect.y) <= (FP_FromInteger(otherRect.h) - k10Scaled) && ourRect.y >= otherRect.y)
    {
        return TRUE;
    }

    if (ourRect.h <= otherRect.h)
    {
        if (FP_FromInteger(ourRect.h) >= (k10Scaled + FP_FromInteger(otherRect.y)))
        {
            return TRUE;
        }
    }

    if (ourRect.y >= otherRect.y)
    {
        if (ourRect.h <= otherRect.h)
        {
            return TRUE;
        }

        if (ourRect.y > otherRect.y)
        {
            return FALSE;
        }
    }

    if (ourRect.h < otherRect.h)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

Scrab* Scrab::FindScrabToFight_45BE30()
{
    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eScrab_77)
        {
            Scrab* pOther = static_cast<Scrab*>(pObj);
            if (pOther != this
                && !pOther->BrainIs(&Scrab::Brain_Death_45CB80)
                && VOnSameYLevel(pOther)
                && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
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
                return pOther;
            }
        }
    }
    return nullptr;
}


s16 Scrab::FindAbeOrMud_45BEF0()
{
    if (CanSeeAbe_45C100(sActiveHero_507678) && sActiveHero_507678->field_100_health > FP_FromInteger(0) && sActiveHero_507678->field_BC_sprite_scale == field_BC_sprite_scale && !WallHit_401930(sActiveHero_507678->field_A8_xpos - field_A8_xpos, field_BC_sprite_scale * FP_FromInteger(35)))
    {
        field_120_pTarget = sActiveHero_507678;
        sActiveHero_507678->field_C_refCount++;
        return 1;
    }

    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            BaseAliveGameObject* pObj = static_cast<BaseAliveGameObject*>(pObjIter);

            if (pObj->field_4_typeId == Types::eMudokon_52 || pObj->field_4_typeId == Types::eMudokon_75 || pObj->field_4_typeId == Types::SlingMud_90)
            {
                if (CanSeeAbe_45C100(pObj) && pObj->field_100_health > FP_FromInteger(0) && pObj->field_BC_sprite_scale == field_BC_sprite_scale && !WallHit_401930(pObj->field_A8_xpos - field_A8_xpos, field_BC_sprite_scale * FP_FromInteger(35)))
                {
                    field_120_pTarget = pObj;
                    field_120_pTarget->field_C_refCount++;
                    return 1;
                }
            }
        }
    }
    return 0;
}

s16 Scrab::CanSeeAbe_45C100(BaseAliveGameObject* pObj)
{
    if (pObj->field_BC_sprite_scale != field_BC_sprite_scale)
    {
        return 0;
    }

    if (pObj == sActiveHero_507678)
    {
        if (sActiveHero_507678->field_FC_current_motion == eAbeStates::State_66_LedgeHang_428D90
            || sActiveHero_507678->field_FC_current_motion == eAbeStates::State_68_LedgeHangWobble_428E50)
        {
            return VOnSameYLevel(pObj);
        }
    }
    return pObj->field_AC_ypos > (field_AC_ypos - field_BC_sprite_scale * FP_FromInteger(35));
}

void Scrab::State_0_Empty_45E3D0()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter_50767C == this)
        {
            field_FC_current_motion = field_E4_previous_motion;
            if (field_F8_pLiftPoint)
            {
                // TODO: Is type of field_F8_pLiftPoint too low?
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

void Scrab::State_1_Stand_45E620()
{
    ToNextMotion_45DFB0();

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

const FP sWalkVelTable_4BC788[22] = {
    FP_FromDouble(1.57),
    FP_FromDouble(1.57),
    FP_FromDouble(1.52),
    FP_FromDouble(1.56),
    FP_FromDouble(1.59),
    FP_FromDouble(3.69),
    FP_FromDouble(3.95),
    FP_FromDouble(3.97),
    FP_FromDouble(3.30),
    FP_FromDouble(2.62),
    FP_FromDouble(2.16),
    FP_FromDouble(2.02),
    FP_FromDouble(1.80),
    FP_FromDouble(1.60),
    FP_FromDouble(2.64),
    FP_FromDouble(2.68),
    FP_FromDouble(2.67),
    FP_FromDouble(2.69),
    FP_FromDouble(1.39),
    FP_FromDouble(1.37),
    FP_FromDouble(1.37),
    FP_FromDouble(2.16)};

void Scrab::State_2_Walk_45E730()
{
    switch (field_10_anim.field_92_current_frame)
    {
        case 3:
        case 8:
        case 13:
        case 18:
            Scrab_SFX_460B80(ScrabSounds::eWalk1_6, Math_RandomRange_450F20(40, 50), 0x7FFF, 1);
            break;

        default:
            break;
    }

    FP vel = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        vel = -sWalkVelTable_4BC788[field_10_anim.field_92_current_frame];
    }
    else
    {
        vel = sWalkVelTable_4BC788[field_10_anim.field_92_current_frame];
    }

    field_B4_velx = (field_BC_sprite_scale * vel);

    const FP xOff = field_B4_velx + FP_FromRaw(field_B4_velx.fpValue / 2);
    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), xOff))
    {
        ToStand();
        return;
    }

    MoveOnLine_45E450();

    if (field_FC_current_motion == eScrabStates::State_2_Walk_45E730)
    {
        if (field_10_anim.field_92_current_frame == 5 || field_10_anim.field_92_current_frame == 15)
        {
            if (field_FE_next_state == eScrabStates::State_1_Stand_45E620
                || field_FE_next_state == eScrabStates::State_4_Turn_45EF30
                || field_FE_next_state == eScrabStates::State_16_Stamp_45F920
                || field_FE_next_state == eScrabStates::State_19_Unused_45F9D0
                || field_FE_next_state == eScrabStates::State_22_Shriek_45FB00
                || field_FE_next_state == eScrabStates::State_20_HowlBegin_45FA60
                || field_FE_next_state == eScrabStates::State_7_HopMidair_45F1A0
                || field_FE_next_state == eScrabStates::State_27_AttackLunge_45FDF0
                || field_FE_next_state == eScrabStates::State_28_LegKick_45FF60)
            {
                field_FC_current_motion = eScrabStates::State_12_WalkToStand_45E930;
            }
        }
        else if (field_10_anim.field_92_current_frame == 7 || field_10_anim.field_92_current_frame == 18)
        {
            if (field_FE_next_state == eScrabStates::State_3_Run_45EAB0)
            {
                field_FC_current_motion = eScrabStates::State_3_Run_45EAB0;
                field_FE_next_state = -1;
            }

            if (!(field_188_flags & 2))
            {
                MapFollowMe_401D30(1);
                field_188_flags |= 2u;
            }
        }
        else
        {
            field_188_flags &= ~2u;
        }

        if (gMap_507BA8.GetDirection(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos)
            >= CameraPos::eCamCurrent_0)
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunVelTable_4BC800[14] = {
    FP_FromDouble(9.49),
    FP_FromDouble(9.73),
    FP_FromDouble(9.58),
    FP_FromDouble(9.14),
    FP_FromDouble(9.09),
    FP_FromDouble(9.57),
    FP_FromDouble(9.32),
    FP_FromDouble(8.65),
    FP_FromDouble(5.29),
    FP_FromDouble(4.28),
    FP_FromDouble(3.44),
    FP_FromDouble(3.39),
    FP_FromDouble(3.81),
    FP_FromDouble(5.15)};


void Scrab::State_3_Run_45EAB0()
{
    switch (field_10_anim.field_92_current_frame)
    {
        case 0:
        case 5:
        case 7:
        case 12:
            Scrab_SFX_460B80(ScrabSounds::eWalk1_6, Math_RandomRange_450F20(40, 50), 0x7FFF, 1);
            break;
        default:
            break;
    }

    FP vel = {};
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        vel = -sRunVelTable_4BC800[field_10_anim.field_92_current_frame];
    }
    else
    {
        vel = sRunVelTable_4BC800[field_10_anim.field_92_current_frame];
    }
    field_B4_velx = (field_BC_sprite_scale * vel);

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
    }
    else
    {
        MoveOnLine_45E450();

        if (field_FC_current_motion == eScrabStates::State_3_Run_45EAB0)
        {
            if (field_120_pTarget == sActiveHero_507678)
            {
                PSX_RECT objRect = {};
                field_120_pTarget->VGetBoundingRect(&objRect, 1);

                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                if (bRect.x <= objRect.w
                    && bRect.w >= objRect.x
                    && bRect.h >= objRect.y
                    && bRect.y <= objRect.h
                    && field_120_pTarget->field_BC_sprite_scale == field_BC_sprite_scale)
                {
                    if (VIsFacingMe(field_120_pTarget))
                    {
                        if (field_120_pTarget->VTakeDamage(this))
                        {
                            SFX_Play_43AD70(SoundEffect::KillEffect_78, 0, 0);
                            Mudokon_SFX_42A4D0(MudSounds::eKnockbackOuch_10, 0, 0, field_120_pTarget);
                        }
                    }
                }
            }

            if (field_10_anim.field_92_current_frame != 3 && field_10_anim.field_92_current_frame != 10)
            {
                field_188_flags &= ~2u;
            }
            else
            {
                if (field_FE_next_state == eScrabStates::State_2_Walk_45E730)
                {
                    field_FC_current_motion = eScrabStates::State_2_Walk_45E730;
                    field_FE_next_state = -1;
                }

                if (field_FE_next_state == eScrabStates::State_1_Stand_45E620
                    || field_FE_next_state == eScrabStates::State_4_Turn_45EF30
                    || field_FE_next_state == eScrabStates::State_7_HopMidair_45F1A0
                    || field_FE_next_state == eScrabStates::State_27_AttackLunge_45FDF0
                    || field_FE_next_state == eScrabStates::State_19_Unused_45F9D0
                    || field_FE_next_state == eScrabStates::State_20_HowlBegin_45FA60)
                {
                    field_FC_current_motion = eScrabStates::State_5_RunToStand_45ED90;
                }

                if (field_FE_next_state == eScrabStates::State_23_ScrabBattleAnim_45FBA0)
                {
                    ToStand();
                    field_FC_current_motion = eScrabStates::State_23_ScrabBattleAnim_45FBA0;
                    field_FE_next_state = -1;
                }

                if (!(field_188_flags & 2))
                {
                    MapFollowMe_401D30(1);
                    field_188_flags |= 2;
                }
            }

            if (gMap_507BA8.GetDirection(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos)
                >= CameraPos::eCamCurrent_0)
            {
                MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
            }
        }
    }
}

void Scrab::State_4_Turn_45EF30()
{
    if (field_10_anim.field_92_current_frame == 5
        || field_10_anim.field_92_current_frame == 9
        || field_10_anim.field_92_current_frame == 11)
    {
        Scrab_SFX_460B80(ScrabSounds::eWalk1_6, Math_RandomRange_450F20(40, 50), 0x7FFF, 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (ToNextMotion_45DFB0())
        {
            field_10_anim.Set_Animation_Data_402A40(field_10_anim.field_18_frame_table_offset, nullptr);
        }
        else
        {
            ToStand();
        }
    }
}

const FP sRunToStandVelTable_4BC838[10] = {
    FP_FromDouble(7.49),
    FP_FromDouble(6.91),
    FP_FromDouble(3.88),
    FP_FromDouble(2.36),
    FP_FromDouble(0.76),
    FP_FromDouble(0.64),
    FP_FromDouble(0.70),
    FP_FromDouble(0.77),
    FP_FromDouble(0.72),
    FP_FromDouble(0.72)};

void Scrab::State_5_RunToStand_45ED90()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sRunToStandVelTable_4BC838[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sRunToStandVelTable_4BC838[field_10_anim.field_92_current_frame]);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
        return;
    }

    MoveOnLine_45E450();

    if (field_FC_current_motion == eScrabStates::State_5_RunToStand_45ED90)
    {
        if (field_120_pTarget)
        {
            PSX_RECT bObjRect = {};
            field_120_pTarget->VGetBoundingRect(&bObjRect, 1);

            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            if (bObjRect.x <= bRect.w
                && bObjRect.w >= bRect.x
                && bObjRect.h >= bRect.y
                && bObjRect.y <= bRect.h
                && field_120_pTarget->field_BC_sprite_scale == field_BC_sprite_scale)
            {
                if (VIsFacingMe(field_120_pTarget))
                {
                    if (field_120_pTarget->VTakeDamage(this))
                    {
                        SFX_Play_43AD70(SoundEffect::KillEffect_78, 0, 0);
                        Mudokon_SFX_42A4D0(MudSounds::eKnockbackOuch_10, 0, 0, field_120_pTarget);
                    }
                }
            }
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            MapFollowMe_401D30(1);

            if (!ToNextMotion_45DFB0())
            {
                ToStand();
            }
        }
    }
}

const FP sHopBeginVelTable_4BC860[4] = {
    FP_FromDouble(9.88),
    FP_FromDouble(10.10),
    FP_FromDouble(10.10),
    FP_FromDouble(9.88)};

void Scrab::State_6_HopBegin_45F3C0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sHopBeginVelTable_4BC860[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sHopBeginVelTable_4BC860[field_10_anim.field_92_current_frame]);
    }

    if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        field_A8_xpos += field_B4_velx;

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_43AE60(SoundEffect::PickupItem_33, 50, -800, 0);

            field_E8_LastLineYPos = field_AC_ypos;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_B4_velx = (field_BC_sprite_scale * -FP_FromDouble(5.7));
            }
            else
            {
                field_B4_velx = (field_BC_sprite_scale * FP_FromDouble(5.7));
            }

            field_B8_vely = (field_BC_sprite_scale * -FP_FromDouble(9.6));
            field_AC_ypos += field_B8_vely;
            VOnTrapDoorOpen();
            field_FC_current_motion = eScrabStates::State_7_HopMidair_45F1A0;
            field_F4_pLine = nullptr;
        }
    }
}

const FP sHopMidAirVelTable_4BC870[8] = {
    FP_FromDouble(9.45),
    FP_FromDouble(8.81),
    FP_FromDouble(8.11),
    FP_FromDouble(7.56),
    FP_FromDouble(7.09),
    FP_FromDouble(6.71),
    FP_FromDouble(6.42),
    FP_FromDouble(6.22)};

void Scrab::State_7_HopMidair_45F1A0()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sHopMidAirVelTable_4BC870[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sHopMidAirVelTable_4BC870[field_10_anim.field_92_current_frame]);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
        field_FC_current_motion = eScrabStates::State_9_JumpToFall_45EFD0;
    }
    else
    {
        if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
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
                    field_FC_current_motion = eScrabStates::State_8_Land_45F500;

                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

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
            field_128 = FP_FromDouble(1.25);
            field_FC_current_motion = eScrabStates::State_9_JumpToFall_45EFD0;
        }
    }
}

const FP sLandVelXTable_4BC890[4] = {
    FP_FromDouble(6.10),
    FP_FromDouble(6.07),
    FP_FromDouble(6.13),
    FP_FromDouble(6.28)};

void Scrab::State_8_Land_45F500()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Scrab_SFX_460B80(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sLandVelXTable_4BC890[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sLandVelXTable_4BC890[field_10_anim.field_92_current_frame]);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
        return;
    }

    MoveOnLine_45E450();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }
}

void Scrab::State_9_JumpToFall_45EFD0()
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= (field_BC_sprite_scale * field_128);
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }
    else if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += (field_BC_sprite_scale * field_128);
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
                ToStand();

                field_F4_pLine = pLine;

                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);
                bRect.y += 5;
                bRect.h += 5;

                VOnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.y, bRect.w},
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                field_A8_xpos = hitX;
                field_AC_ypos = hitY;
                MapFollowMe_401D30(1);
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

const FP sStandToWalkVelTable_4BC778[3] = {
    FP_FromDouble(1.11),
    FP_FromDouble(2.36),
    FP_FromDouble(3.20)};

void Scrab::State_10_StandToWalk_45E670()
{
    FP vel = sStandToWalkVelTable_4BC778[field_10_anim.field_92_current_frame];

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -vel);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * vel);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
    }
    else
    {
        MoveOnLine_45E450();

        if (field_FC_current_motion == eScrabStates::State_10_StandToWalk_45E670)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_motion = eScrabStates::State_2_Walk_45E730;
            }
        }
    }
}

const FP sStandToRunVel_4BC7F0[3] = {
    FP_FromDouble(2.29),
    FP_FromDouble(2.69),
    FP_FromDouble(3.87)};

void Scrab::State_11_StandToRun_45E9F0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sStandToRunVel_4BC7F0[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sStandToRunVel_4BC7F0[field_10_anim.field_92_current_frame]);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
    }
    else
    {
        MoveOnLine_45E450();

        if (field_FC_current_motion == eScrabStates::State_11_StandToRun_45E9F0)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_motion = eScrabStates::State_3_Run_45EAB0;
            }
        }
    }
}

const FP sWalkToStandVel_4BC7E0[3] = {
    FP_FromDouble(1.04),
    FP_FromDouble(3.29),
    FP_FromDouble(2.85)};

void Scrab::State_12_WalkToStand_45E930()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sWalkToStandVel_4BC7E0[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sWalkToStandVel_4BC7E0[field_10_anim.field_92_current_frame]);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
    }

    MoveOnLine_45E450();

    if (field_FC_current_motion == eScrabStates::State_12_WalkToStand_45E930 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) && !ToNextMotion_45DFB0())
    {
        ToStand();
    }
}

const FP sRunJumpBeginVelTable_4BC8A0[8] = {
    FP_FromDouble(13.13),
    FP_FromDouble(12.36),
    FP_FromDouble(11.54),
    FP_FromDouble(10.89),
    FP_FromDouble(9.35),
    FP_FromDouble(8.90),
    FP_FromDouble(8.56),
    FP_FromDouble(8.32)};

void Scrab::State_13_RunJumpBegin_45F5D0()
{
    if (field_10_anim.field_92_current_frame == 1)
    {
        SFX_Play_43AE60(SoundEffect::PickupItem_33, 50, -800, 0);
    }

    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sRunJumpBeginVelTable_4BC8A0[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sRunJumpBeginVelTable_4BC8A0[field_10_anim.field_92_current_frame]);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
        field_FC_current_motion = eScrabStates::State_9_JumpToFall_45EFD0;
    }
    else
    {
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
                    ToStand();
                    field_F4_pLine = pLine;
                    field_FC_current_motion = eScrabStates::State_14_RunJumpEnd_45F850;

                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

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
            field_128 = FP_FromDouble(1.25);
            field_FC_current_motion = eScrabStates::State_9_JumpToFall_45EFD0;
        }

        if (gMap_507BA8.GetDirection(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos)
            >= CameraPos::eCamCurrent_0)
        {
            MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunJumpEndVelTable_4BC8C0[4] = {
    FP_FromDouble(4.18),
    FP_FromDouble(4.14),
    FP_FromDouble(4.21),
    FP_FromDouble(4.38)};

void Scrab::State_14_RunJumpEnd_45F850()
{
    Event_Broadcast_417220(kEventNoise_0, this);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -sRunJumpEndVelTable_4BC8C0[field_10_anim.field_92_current_frame]);
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * sRunJumpEndVelTable_4BC8C0[field_10_anim.field_92_current_frame]);
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
    {
        ToStand();
        return;
    }

    MoveOnLine_45E450();

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Scrab_SFX_460B80(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        if (!ToNextMotion_45DFB0())
        {
            ToStand();
        }
    }
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
        Scrab_SFX_460B80(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        SFX_Play_43AE60(SoundEffect::KillEffect_78, 60, Math_RandomRange_450F20(-255, 255), 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Scrab_SFX_460B80(ScrabSounds::eHowl_0, 60, 511, 1);
        ToNextMotion_45DFB0();
    }

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::State_17_Empty_45F9C0()
{
    // Empty
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
        if (!ToNextMotion_45DFB0())
        {
            ToStand();
        }
    }

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::State_20_HowlBegin_45FA60()
{
    if (field_10_anim.field_92_current_frame == 2)
    {
        Scrab_SFX_460B80(ScrabSounds::eYell_8, 0, Math_RandomRange_450F20(-1600, -900), 1);
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
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::State_21_HowlEnd_45FAF0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToNextMotion_45DFB0();
    }
}

void Scrab::State_22_Shriek_45FB00()
{
    if (field_10_anim.field_92_current_frame == 4)
    {
        Scrab_SFX_460B80(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_45DFB0())
        {
            ToStand();
        }
    }

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::State_23_ScrabBattleAnim_45FBA0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        field_14C = Scrab_SFX_460B80(ScrabSounds::eShredding_5, 100, Math_RandomRange_450F20(-600, 200), 1);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToNextMotion_45DFB0();
    }

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
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
            ToStand();
        }
    }

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

const u32 sFeedVelTable_4BC8D0[12] = {
    380127,
    526666,
    388557,
    295550,
    135669,
    8971,
    4294918160,
    4294924422,
    4294964851,
    0,
    4294966370,
    4294966453};


void Scrab::State_25_ToFeed_45FCE0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -FP_FromRaw(sFeedVelTable_4BC8D0[field_10_anim.field_92_current_frame]));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(sFeedVelTable_4BC8D0[field_10_anim.field_92_current_frame]));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eScrabStates::State_26_Feed_45FDA0;
    }

    if ((field_188_flags >> 2) & 1)
    {
        if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_B4_velx))
        {
            ToStand();
        }
        else
        {
            MoveOnLine_45E450();
        }
    }
}

void Scrab::State_26_Feed_45FDA0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        if (Math_RandomRange_450F20(0, 100) >= 50)
        {
            SFX_Play_43AD70(SoundEffect::Eating2_80, 0, 0);
        }
        else
        {
            SFX_Play_43AD70(SoundEffect::Eating1_79, 0, 0);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eScrabStates::State_24_FeedToGulp_45FC30;
    }
}

void Scrab::State_27_AttackLunge_45FDF0()
{
    if (field_120_pTarget)
    {
        PSX_RECT objRect = {};
        field_120_pTarget->VGetBoundingRect(&objRect, 1);

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        if (objRect.x <= bRect.w
            && objRect.w >= bRect.x
            && objRect.h >= bRect.y
            && objRect.y <= bRect.h
            && field_120_pTarget->field_BC_sprite_scale == field_BC_sprite_scale)
        {
            if (VIsFacingMe(field_120_pTarget))
            {
                if (field_120_pTarget->VTakeDamage(this))
                {
                    SFX_Play_43AD70(SoundEffect::KillEffect_78, 0, 0);
                    Mudokon_SFX_42A4D0(MudSounds::eKnockbackOuch_10, 0, 0, field_120_pTarget);
                }

                field_120_pTarget->VTakeDamage(this);
            }
        }

        if (field_10_anim.field_92_current_frame == 4)
        {
            Scrab_SFX_460B80(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand();
    }

    if (gMap_507BA8.GetDirection(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::State_28_LegKick_45FF60()
{
    State_27_AttackLunge_45FDF0();
}

void Scrab::State_29_DeathBegin_45FFA0()
{
    if (!field_F4_pLine)
    {
        State_9_JumpToFall_45EFD0();

        if (field_FC_current_motion != eScrabStates::State_29_DeathBegin_45FFA0)
        {
            field_FC_current_motion = eScrabStates::State_29_DeathBegin_45FFA0;
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * -FP_FromRaw(sFeedVelTable_4BC8D0[field_10_anim.field_92_current_frame]));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(sFeedVelTable_4BC8D0[field_10_anim.field_92_current_frame]));
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eScrabStates::State_18_GetEaten_45FF70;
    }
}

s16 Scrab::Brain_Fighting_45C370()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    Scrab* pFighter = field_11C_pFight_target;
    if (pFighter && (pFighter->field_6_flags.Get(BaseGameObject::eDead_Bit3) || !VOnSameYLevel(field_11C_pFight_target)))
    {
        field_11C_pFight_target->field_C_refCount--;
        field_188_flags &= ~1u;
        field_11C_pFight_target = nullptr;
        field_FE_next_state = eScrabStates::State_1_Stand_45E620;
        SetBrain(&Scrab::Brain_WalkAround_460D80); // patrol ??
        return 0;
    }

    switch (field_110_brain_ret)
    {
        case 0:
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_ret;
            }

            if (VIsFacingMe(field_11C_pFight_target))
            {
                if (!VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(8), field_11C_pFight_target))
                {
                    if (field_11C_pFight_target->field_FC_current_motion == eScrabStates::State_20_HowlBegin_45FA60)
                    {
                        return field_110_brain_ret;
                    }
                    field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
                    return 7;
                }
                else
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 2;
                }
            }
            else
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 1;
            }
            break;

        case 1:
            if (field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            if (field_11C_pFight_target->field_A8_xpos != field_A8_xpos || (field_11C_pFight_target->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) || field_11C_pFight_target->field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                return 0;
            }
            else
            {
                field_FE_next_state = eScrabStates::State_2_Walk_45E730;
                return 3;
            }
            break;

        case 2:
            if (field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = eScrabStates::State_2_Walk_45E730;
            return 3;

        case 3:
        {
            FP xpos = {};
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), -(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2))))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }

                if (Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }

                if (!VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(10), field_11C_pFight_target))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }
                xpos = field_A8_xpos - FP_FromInteger(50);
            }
            else
            {
                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2)))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }

                if (Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }

                if (!VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(10), field_11C_pFight_target))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }
                xpos = field_A8_xpos + FP_FromInteger(50);
            }

            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    xpos,
                    field_AC_ypos,
                    0))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
            return 4;
        }

        case 4:
            if (field_FC_current_motion == eScrabStates::State_4_Turn_45EF30)
            {
                if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
                return 7;
            }
            else
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }
            break;

        case 5:
            if (field_FC_current_motion != eScrabStates::State_19_Unused_45F9D0
                || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            field_118_timer = gnFrameCount_507670 + 30;
            return 6;

        case 6:
            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
            return 7;

        case 7:
            if (field_FC_current_motion != eScrabStates::State_20_HowlBegin_45FA60
                || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            field_118_timer = gnFrameCount_507670 + 20;
            return 8;

        case 8:
            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
            return 9;

        case 9:
            if (field_FC_current_motion != eScrabStates::State_22_Shriek_45FB00
                || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }
            field_188_flags |= 1u;
            field_FE_next_state = 1;
            return 10;

        case 10:
            if (!(field_11C_pFight_target->field_188_flags & 1))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_3_Run_45EAB0;
            return 11;

        case 11:
            if (!VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale), field_11C_pFight_target)
                || !field_F4_pLine
                || !field_11C_pFight_target->field_F4_pLine)
            {
                return field_110_brain_ret;
            }

            MapFollowMe_401D30(TRUE);

            if (field_11C_pFight_target->field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_100_health = FP_FromInteger(0);
            }
            field_FC_current_motion = eScrabStates::State_23_ScrabBattleAnim_45FBA0;
            field_188_flags &= ~1u;
            field_118_timer = gnFrameCount_507670 + 40;
            return 12;

        case 12:
            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }

            SND_Stop_Channels_Mask_4774A0(field_14C);

            field_14C = 0;

            Scrab_SFX_460B80(ScrabSounds::eDeathHowl_1, 0, -1571, 1);
            Scrab_SFX_460B80(ScrabSounds::eYell_8, 0, -1571, 1);
            Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 0, -383, 0);
            field_11C_pFight_target->field_C_refCount--;
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                field_11C_pFight_target = nullptr;
                field_FC_current_motion = eScrabStates::State_1_Stand_45E620;
                field_118_timer = gnFrameCount_507670 + 20;
                return 13;
            }
            else
            {
                field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_A8_xpos = field_11C_pFight_target->field_A8_xpos;
                field_11C_pFight_target = nullptr;
                SetBrain(&Scrab::Brain_Death_45CB80);
                field_FC_current_motion = eScrabStates::State_29_DeathBegin_45FFA0;
                field_130 = 2;
                field_118_timer = gnFrameCount_507670 + 90;
                return 0;
            }
            break;

        case 13:
            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_16_Stamp_45F920;
            field_118_timer = gnFrameCount_507670 + 75;
            return 14;

        case 14:
            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
            field_118_timer = gnFrameCount_507670 + 45;
            return 15;

        case 15:
            if (field_118_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                field_FE_next_state = 1;
                SetBrain(&Scrab::Brain_WalkAround_460D80); // patrol ??
                return 0;
            }
            return field_110_brain_ret;

        default:
            return field_110_brain_ret;
    }
}

s16 Scrab::Brain_BatDeath_45CA60()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_110_brain_ret)
    {
        case 0:
            if (field_FC_current_motion == eScrabStates::State_1_Stand_45E620)
            {
                field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
                field_118_timer = gnFrameCount_507670 + 45;
                return 1;
            }
            break;

        case 1:
            if (field_118_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                field_FE_next_state = eScrabStates::State_2_Walk_45E730;
                return 2;
            }
            break;

        case 2:
            if (field_FC_current_motion == eScrabStates::State_2_Walk_45E730)
            {
                field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
                return 3;
            }
            break;

        case 3:
            if (field_FC_current_motion == eScrabStates::State_22_Shriek_45FB00
                && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 4;
            }
            break;

        case 4:
            if (field_FC_current_motion == eScrabStates::State_4_Turn_45EF30)
            {
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    SetBrain(&Scrab::Brain_Death_45CB80);
                    field_130 = 2;
                    field_118_timer = gnFrameCount_507670 + 90;
                    field_FC_current_motion = eScrabStates::State_29_DeathBegin_45FFA0;
                }
            }
            break;

        default:
            break;
    }

    return field_110_brain_ret;
}

s16 Scrab::Brain_Death_45CB80()
{
    if (field_118_timer > static_cast<s32>(gnFrameCount_507670) && field_118_timer < static_cast<s32>(gnFrameCount_507670) + 80)
    {
        field_BC_sprite_scale -= FP_FromDouble(0.008);
        field_C0_r -= 2;
        field_C2_g -= 2;
        field_C4_b -= 2;
    }

    if (static_cast<s32>(gnFrameCount_507670) < field_118_timer - 24 && !(gnFrameCount_507670 % 5))
    {
        New_Smoke_Particles_419A80((FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * field_BC_sprite_scale) + field_A8_xpos, field_AC_ypos - FP_FromInteger(6), field_BC_sprite_scale / FP_FromInteger(2), 2, 0);
        SFX_Play_43AE60(SoundEffect::Vaporize_96, 25, FP_GetExponent((FP_FromInteger(2200) * field_BC_sprite_scale)));
    }

    if (field_118_timer < static_cast<s32>(gnFrameCount_507670))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 100;
}

s16 Scrab::Brain_ChasingEnemy_Real_45CC90()
{
    NOT_IMPLEMENTED();
    return 0;
}

s16 Scrab::Brain_ChasingEnemy_45CC90()
{
    // 0 to 17
    if (field_110_brain_ret == 8)
    {
        //return Brain_ChasingEnemy_Real_45CC90();
    }

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    field_11C_pFight_target = FindScrabToFight_45BE30();
    if (field_11C_pFight_target)
    {
        field_120_pTarget->field_C_refCount--;
        field_120_pTarget = nullptr;
        field_11C_pFight_target->field_C_refCount++;
        SetBrain(&Scrab::Brain_Fighting_45C370);
        field_FE_next_state = eScrabStates::State_1_Stand_45E620;
        return 0;
    }

    if (field_120_pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3)
        || (field_13C_attack_timer <= static_cast<s32>(gnFrameCount_507670)
            && !CanSeeAbe_45C100(field_120_pTarget)
            && field_120_pTarget->field_100_health > FP_FromInteger(0)
            && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0)))
    {
        bool bCloseToEdge = false;
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (!Check_IsOnEndOfLine_4021A0(1, 2))
            {
                bCloseToEdge = true;
            }
        }
        else
        {
            if (!Check_IsOnEndOfLine_4021A0(0, 2))
            {
                bCloseToEdge = true;
            }
        }

        if (bCloseToEdge)
        {
            field_120_pTarget->field_C_refCount--;
            field_120_pTarget = nullptr;
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            SetBrain(&Scrab::Brain_WalkAround_460D80);
            return 0;
        }
    }

    if (Event_Get_417250(kEventAbeOhm_8))
    {
        field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
        return 18;
    }

    const FP kGridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    switch (field_110_brain_ret)
    {
        case 0:
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_ret;
            }
            field_13C_attack_timer = gnFrameCount_507670 + field_138_attack_duration;
            return 1;

        case 1:
        {
            if ((!CanSeeAbe_45C100(field_120_pTarget)
                 && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                     field_B2_lvl_number,
                     field_B0_path_number,
                     field_A8_xpos,
                     field_AC_ypos,
                     0))
                || WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_120_pTarget->field_A8_xpos - field_A8_xpos)
                || field_120_pTarget->field_100_health <= FP_FromInteger(0))
            {
                if (field_FC_current_motion == eScrabStates::State_1_Stand_45E620 && Math_NextRandom() < 26u && (field_188_flags & 0x20) && (gnFrameCount_507670 - field_140_last_shriek_timer) > 60)
                {
                    field_140_last_shriek_timer = gnFrameCount_507670;
                    field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
                    return 17;
                }

                return 1;
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos,
                        0))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                }
                else
                {
                    field_FC_current_motion = eScrabStates::State_4_Turn_45EF30;
                    field_FE_next_state = -1;
                    MapFollowMe_401D30(TRUE);
                }
                return 2;
            }

            if (field_F8_pLiftPoint)
            {
                if (field_F8_pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        return 4;
                    }
                }
            }

            if (VIsObjNearby(kGridSize, field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                field_FE_next_state = eScrabStates::State_28_LegKick_45FF60;
                return 10;
            }

            Path_TLV* pTlv = nullptr;
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos - kGridSize),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos - kGridSize),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79);
            }
            else
            {
                pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos + kGridSize),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos + kGridSize),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79);
            }

            if (!pTlv)
            {
                pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79);
            }

            auto pStopper = static_cast<Path_EnemyStopper*>(pTlv);
            field_F0_pTlv = pTlv;
            if (pStopper)
            {
                const bool bLeft = pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 && field_120_pTarget->field_A8_xpos < field_A8_xpos;
                const bool bRight = pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 && field_120_pTarget->field_A8_xpos > field_A8_xpos;
                const bool bBoth = pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2;
                const bool bSwitchOn = SwitchStates_Get(pStopper->field_1A_id) ? true : false;
                if ((bLeft || bRight || bBoth) && !bSwitchOn)
                {
                    if (field_FC_current_motion == eScrabStates::State_1_Stand_45E620 && Math_NextRandom() < 26u && (field_188_flags & 0x20) && (gnFrameCount_507670 - field_140_last_shriek_timer) > 60)
                    {
                        field_140_last_shriek_timer = gnFrameCount_507670;
                        field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
                        return 17;
                    }
                    return 1;
                }
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                field_FE_next_state = eScrabStates::State_27_AttackLunge_45FDF0;
                return 10;
            }

            field_118_timer = gnFrameCount_507670 + field_114_attack_delay;
            return 8;
        }

        case 2:
            if (field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            return 1;

        case 3:
        {
            if (field_B4_velx > FP_FromInteger(0))
            {
                const s16 x_exp = FP_GetExponent(field_A8_xpos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    if (field_120_pTarget->field_AC_ypos - field_AC_ypos < FP_FromInteger(5)
                        || gMap_507BA8.TLV_Get_At_446260(
                            FP_GetExponent(field_A8_xpos + kGridSize),
                            FP_GetExponent(field_AC_ypos + FP_FromInteger(10)),
                            FP_GetExponent(field_A8_xpos + kGridSize),
                            FP_GetExponent(field_AC_ypos + FP_FromInteger(10)),
                            TlvTypes::ScrabNoFall_93))
                    {
                        if (!Check_IsOnEndOfLine_4021A0(0, 4))
                        {
                            ToJump_45E340();
                            field_FE_next_state = -1;
                            return 7;
                        }
                    }
                }

                field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos + (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos + (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79);

                auto pStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);
                if (pStopper)
                {
                    if (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2)
                    {
                        if (!SwitchStates_Get(pStopper->field_1A_id))
                        {
                            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
            }
            else if (field_B4_velx < FP_FromInteger(0))
            {
                const s16 x_exp = FP_GetExponent(field_A8_xpos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    if ((field_120_pTarget->field_AC_ypos - field_AC_ypos < FP_FromInteger(5))
                        || gMap_507BA8.TLV_Get_At_446260(
                            FP_GetExponent(field_A8_xpos - kGridSize),
                            FP_GetExponent(field_AC_ypos + FP_FromInteger(10)),
                            FP_GetExponent(field_A8_xpos - kGridSize),
                            FP_GetExponent(field_AC_ypos + FP_FromInteger(10)),
                            TlvTypes::ScrabNoFall_93))
                    {
                        if (!Check_IsOnEndOfLine_4021A0(1, 4))
                        {
                            ToJump_45E340();
                            field_FE_next_state = -1;
                            return 7;
                        }
                    }
                }

                field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos - (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos - (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79);

                auto pStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);
                if (pStopper)
                {
                    if (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2)
                    {
                        if (!SwitchStates_Get(pStopper->field_1A_id))
                        {
                            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos,
                        0))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                }
                else
                {
                    field_FC_current_motion = eScrabStates::State_4_Turn_45EF30;
                    field_FE_next_state = -1;
                    MapFollowMe_401D30(TRUE);
                }
                return 2;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(3), field_120_pTarget)
                && field_FC_current_motion == eScrabStates::State_3_Run_45EAB0
                && VOnSameYLevel(field_120_pTarget))
            {
                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_120_pTarget->field_A8_xpos - field_A8_xpos))
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 1;
                }
                field_FE_next_state = eScrabStates::State_27_AttackLunge_45FDF0;
                return 10;
            }

            auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
            if (pLiftPoint && field_F8_pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                return 4;
            }

            if (field_F4_pLine)
            {
                return field_110_brain_ret;
            }
            return 5;
        }

        case 4:
            if (VIsObjNearby(kGridSize, field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                field_FE_next_state = eScrabStates::State_28_LegKick_45FF60;
                return 10;
            }

            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (!pLiftPoint->OnAnyFloor())
                {
                    return field_110_brain_ret;
                }
            }
            return 11;

        case 5:
        case 6:
            if (field_FC_current_motion != eScrabStates::State_1_Stand_45E620)
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = -1;
            return 8;

        case 7:
            if (field_FC_current_motion != eScrabStates::State_14_RunJumpEnd_45F850)
            {
                return field_110_brain_ret;
            }
            return 11;

        case 8:
        {
            if (!CanSeeAbe_45C100(field_120_pTarget)
                && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                return 1;
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 9;
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), field_120_pTarget) && VOnSameYLevel(field_120_pTarget))
            {
                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_120_pTarget->field_A8_xpos - field_A8_xpos))
                {
                    return 1;
                }

                if (!VIsObjNearby(kGridSize, field_120_pTarget))
                {
                    field_FE_next_state = eScrabStates::State_27_AttackLunge_45FDF0;
                    return 10;
                }

                field_FE_next_state = eScrabStates::State_28_LegKick_45FF60;
                return 10;
            }

            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                if (field_FC_current_motion != eScrabStates::State_1_Stand_45E620 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
                return field_110_brain_ret;
            }

            field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                TlvTypes::EnemyStopper_79);

            auto pStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);

            if (pStopper)
            {
                const bool bLeft = pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 && field_120_pTarget->field_A8_xpos < field_A8_xpos;
                const bool bRight = pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 && field_120_pTarget->field_A8_xpos > field_A8_xpos;
                const bool bBoth = pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2;
                const bool bSwitchOn = SwitchStates_Get(pStopper->field_1A_id) ? true : false;
                if ((bLeft || bRight || bBoth) && !bSwitchOn)
                {
                    return 1;
                }
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    field_FE_next_state = eScrabStates::State_7_HopMidair_45F1A0;
                    return 6;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    field_FE_next_state = eScrabStates::State_7_HopMidair_45F1A0;
                    return 6;
                }
            }

            field_FE_next_state = eScrabStates::State_3_Run_45EAB0;
            return 3;
        }

        case 9:
            if (field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            return 8;

        case 10:
            if ((field_FC_current_motion == eScrabStates::State_27_AttackLunge_45FDF0 || field_FC_current_motion == eScrabStates::State_28_LegKick_45FF60) && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                if (field_120_pTarget->field_100_health <= FP_FromInteger(0))
                {
                    if (!VIsFacingMe(field_120_pTarget))
                    {
                        field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                        return 14;
                    }

                    if (!VIsObjNearby(kGridSize, field_120_pTarget))
                    {
                        field_FE_next_state = eScrabStates::State_2_Walk_45E730;
                        return 13;
                    }

                    field_188_flags &= ~4u;
                    field_FE_next_state = eScrabStates::State_16_Stamp_45F920;
                    field_118_timer = gnFrameCount_507670 + 30;
                    return 15;
                }

                if (!VIsFacingMe(field_120_pTarget))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 12;
                }
                return HandleRunning();
            }
            return field_110_brain_ret;

        case 11:
            if (!CanSeeAbe_45C100(field_120_pTarget)
                && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                SetBrain(&Scrab::Brain_WalkAround_460D80);
                return 0;
            }

            if (field_120_pTarget->field_100_health <= FP_FromInteger(0))
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                return 1;
            }

            if (!VIsFacingMe(field_120_pTarget))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 12;
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), field_120_pTarget))
            {
                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), field_120_pTarget->field_A8_xpos - field_A8_xpos))
                {
                    return 1;
                }

                if (VIsObjNearby(kGridSize, field_120_pTarget))
                {
                    field_FE_next_state = eScrabStates::State_28_LegKick_45FF60;
                    return 10;
                }

                field_FE_next_state = eScrabStates::State_27_AttackLunge_45FDF0;
                return 10;
            }

            return HandleRunning();


        case 12:
            if (field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            return 11;

        case 13:
            if (VIsObjNearby(kGridSize, field_120_pTarget))
            {
                field_188_flags &= ~4u;
                field_FE_next_state = eScrabStates::State_16_Stamp_45F920;
                field_118_timer = gnFrameCount_507670 + 30;
                return 15;
            }
            return field_110_brain_ret;

        case 14:
            if (field_FC_current_motion != eScrabStates::State_4_Turn_45EF30)
            {
                if (field_FE_next_state == eScrabStates::State_4_Turn_45EF30 || field_100_health < FP_FromInteger(0))
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return field_110_brain_ret;
            }

            if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            if (!VIsObjNearby(kGridSize, field_120_pTarget))
            {
                field_FE_next_state = eScrabStates::State_2_Walk_45E730;
                return 13;
            }
            field_188_flags &= ~4u;
            field_FE_next_state = eScrabStates::State_16_Stamp_45F920;
            field_118_timer = gnFrameCount_507670 + 30;
            return 15;

        case 15:
            if (field_118_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                if (FP_Abs(field_120_pTarget->field_AC_ypos - field_AC_ypos) >= FP_FromInteger(5))
                {
                    field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
                    return 17;
                }

                field_FE_next_state = eScrabStates::State_25_ToFeed_45FCE0;
                return 16;
            }
            return field_110_brain_ret;

        case 16:
            if (field_FC_current_motion != eScrabStates::State_24_FeedToGulp_45FC30 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_25_ToFeed_45FCE0;
            return field_110_brain_ret;

        case 17:
        {
            if (field_FC_current_motion != eScrabStates::State_22_Shriek_45FB00 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = eScrabStates::State_1_Stand_45E620;

            if (field_F8_pLiftPoint && field_F8_pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (!pLiftPoint->OnAnyFloor())
                {
                    return 4;
                }
            }
            return 1;
        }

        case 18:
            if (Event_Get_417250(kEventAbeOhm_8))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            return 1;

        default:
            return field_110_brain_ret;
    }
}

s16 Scrab::Brain_Patrol_460020()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    field_11C_pFight_target = FindScrabToFight_45BE30();
    if (field_11C_pFight_target)
    {
        field_11C_pFight_target->field_C_refCount++;
        field_FE_next_state = eScrabStates::State_1_Stand_45E620;
        SetBrain(&Scrab::Brain_Fighting_45C370);
        return 0;
    }

    if (FindAbeOrMud_45BEF0())
    {
        if (CanSeeAbe_45C100(field_120_pTarget))
        {
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            SetBrain(&Scrab::Brain_ChasingEnemy_45CC90);
            return 0;
        }
        field_120_pTarget->field_C_refCount--;
        field_120_pTarget = nullptr;
    }

    if (Event_Get_417250(kEventAbeOhm_8))
    {
        field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
        return 9;
    }

    switch (field_110_brain_ret)
    {
        case 0:
        {
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_ret;
            }

            auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
            if (pLiftPoint)
            {
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
                {
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        SetBrain(&Scrab::Brain_WalkAround_460D80);
                        return 0;
                    }
                }
            }

            auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                TlvTypes::EnemyStopper_79));
            field_F0_pTlv = pStopper;

            if (pStopper)
            {
                if (!SwitchStates_Get(pStopper->field_1A_id))
                {
                    if (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        return 0;
                    }

                    if (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        return 0;
                    }

                    if (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2)
                    {
                        return 0;
                    }
                }
            }

            field_FE_next_state = GetMotionForPatrolType(field_116_patrol_type);

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::ScrabLeftBound_74);
                if (field_F0_pTlv)
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 2;
                }
                return 1;
            }
            else
            {
                field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::ScrabRightBound_75);
                if (field_F0_pTlv)
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 5;
                }
                return 4;
            }
            break;
        }

        case 1:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos - ScaleToGridSize_41FA30(field_BC_sprite_scale)),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos - ScaleToGridSize_41FA30(field_BC_sprite_scale)),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::ScrabLeftBound_74))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 2;
            }
            return field_110_brain_ret;

        case 2:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_FC_current_motion == eScrabStates::State_4_Turn_45EF30 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                field_118_timer = gnFrameCount_507670 + Math_RandomRange_450F20(field_144_left_min_delay, field_146_left_max_delay);
                return 3;
            }
            return field_110_brain_ret;

        case 3:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }

            if (Math_NextRandom() < 30u)
            {
                field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
                field_118_timer = gnFrameCount_507670 + 30;
                return 7;
            }

            field_FE_next_state = GetMotionForPatrolType(field_116_patrol_type);
            return 4;

        case 4:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(ScaleToGridSize_41FA30(field_BC_sprite_scale) + field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(ScaleToGridSize_41FA30(field_BC_sprite_scale) + field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::ScrabRightBound_75))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 5;
            }
            return field_110_brain_ret;

        case 5:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_FC_current_motion == eScrabStates::State_4_Turn_45EF30 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                field_118_timer = gnFrameCount_507670 + Math_RandomRange_450F20(field_148_right_min_delay, field_14A_right_max_delay);
                return 6;
            }
            return field_110_brain_ret;

        case 6:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }

            if (Math_NextRandom() < 30u)
            {
                field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
                field_118_timer = gnFrameCount_507670 + 30;
                return 7;
            }

            field_FE_next_state = GetMotionForPatrolType(field_116_patrol_type);
            return 1;

        case 7:
            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_110_brain_ret;
            }

            field_FE_next_state = GetMotionForPatrolType(field_116_patrol_type);

            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 8;
                }
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                return 1;
            }
            return 4;

        case 8:
            if (field_F8_pLiftPoint)
            {
                auto pLift = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (!pLift->OnAnyFloor())
                {
                    return field_110_brain_ret;
                }
            }
            return 0;

        case 9:
            if (!Event_Get_417250(kEventAbeOhm_8))
            {
                field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                return 0;
            }
            return field_110_brain_ret;

        default:
            return field_110_brain_ret;
    }
}

s16 Scrab::Brain_WalkAround_460D80()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    field_11C_pFight_target = FindScrabToFight_45BE30();
    if (field_11C_pFight_target)
    {
        field_11C_pFight_target->field_C_refCount++;
        SetBrain(&Scrab::Brain_Fighting_45C370);
        field_FE_next_state = eScrabStates::State_1_Stand_45E620;
        return 0;
    }

    if (FindAbeOrMud_45BEF0())
    {
        if (CanSeeAbe_45C100(field_120_pTarget))
        {
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            SetBrain(&Scrab::Brain_ChasingEnemy_45CC90);
            return 0;
        }
        field_120_pTarget->field_C_refCount--;
        field_120_pTarget = nullptr;
    }

    if (Event_Get_417250(kEventAbeOhm_8))
    {
        field_FE_next_state = eScrabStates::State_20_HowlBegin_45FA60;
        return 6;
    }

    switch (field_110_brain_ret)
    {
        case 0:
            if (sNumCamSwappers_507668 > 0)
            {
                return field_110_brain_ret;
            }
            field_12C = field_A8_xpos;
            return 1;

        case 1:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
                {
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        return 1;
                    }
                }
            }

            if (field_118_timer > static_cast<s32>(gnFrameCount_507670))
            {
                if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || gnFrameCount_507670 - field_140_last_shriek_timer <= 90)
                {
                    return field_110_brain_ret;
                }
                field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
                field_140_last_shriek_timer = gnFrameCount_507670;
                return 5;
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79));

                field_F0_pTlv = pStopper;

                if (pStopper)
                {
                    if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2) && !SwitchStates_Get(pStopper->field_1A_id))
                    {
                        field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                        return 4;
                    }
                }

                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), -ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromDouble(1.5)) || Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }
                field_FE_next_state = eScrabStates::State_2_Walk_45E730;
                return 2;
            }
            else
            {
                auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    FP_GetExponent(field_A8_xpos),
                    FP_GetExponent(field_AC_ypos),
                    TlvTypes::EnemyStopper_79));

                field_F0_pTlv = pStopper;

                if (pStopper)
                {
                    if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2) && !SwitchStates_Get(pStopper->field_1A_id))
                    {
                        field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                        return 4;
                    }
                }

                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromDouble(1.5))
                    || Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }
                field_FE_next_state = eScrabStates::State_2_Walk_45E730;
                return 3;
            }
            break;

        case 2:
        {
            auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                TlvTypes::EnemyStopper_79));

            field_F0_pTlv = pStopper;

            if (pStopper)
            {
                if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2) && !SwitchStates_Get(pStopper->field_1A_id))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }
            }

            if (field_12C - field_A8_xpos > FP_FromInteger(368))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 4;
            }

            if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), -ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromDouble(1.5))
                || Check_IsOnEndOfLine_4021A0(1, 2))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 4;
            }

            if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || gnFrameCount_507670 - field_140_last_shriek_timer <= 90)
            {
                if (field_F8_pLiftPoint)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                    if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
                    {
                        if (!pLiftPoint->OnAnyFloor())
                        {
                            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
                return field_110_brain_ret;
            }

            field_140_last_shriek_timer = gnFrameCount_507670;
            field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
            return 5;
        }

        case 3:
        {
            auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(field_AC_ypos),
                TlvTypes::EnemyStopper_79));

            field_F0_pTlv = pStopper;

            if (pStopper)
            {
                if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2) && !SwitchStates_Get(pStopper->field_1A_id))
                {
                    field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                    return 4;
                }
            }

            if (field_A8_xpos - field_12C > FP_FromInteger(368)
                || WallHit_401930(field_BC_sprite_scale * FP_FromInteger(30), ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromDouble(1.5))
                || Check_IsOnEndOfLine_4021A0(0, 2))
            {
                field_FE_next_state = eScrabStates::State_4_Turn_45EF30;
                return 4;
            }

            if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || gnFrameCount_507670 - field_140_last_shriek_timer <= 90)
            {
                if (field_F8_pLiftPoint)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                    if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51)
                    {
                        if (!pLiftPoint->OnAnyFloor())
                        {
                            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                            return 1;
                        }
                    }
                }
                return field_110_brain_ret;
            }

            field_140_last_shriek_timer = gnFrameCount_507670;
            field_FE_next_state = eScrabStates::State_22_Shriek_45FB00;
            return 5;
        }

        case 4:
            if (field_FC_current_motion != 4 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_118_timer = gnFrameCount_507670 + Math_RandomRange_450F20(field_144_left_min_delay, field_146_left_max_delay);
            }
            else
            {
                field_118_timer = gnFrameCount_507670 + Math_RandomRange_450F20(field_148_right_min_delay, field_14A_right_max_delay);
            }

            return 1;

        case 5:
            if (field_F8_pLiftPoint)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(field_F8_pLiftPoint);
                if (pLiftPoint->field_4_typeId == Types::eLiftPoint_51 && !pLiftPoint->OnAnyFloor())
                {
                    field_FE_next_state = eScrabStates::State_1_Stand_45E620;
                    return 1;
                }
            }

            if (field_FC_current_motion != eScrabStates::State_22_Shriek_45FB00 || !field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            return 1;

        case 6:
            if (Event_Get_417250(kEventAbeOhm_8))
            {
                return field_110_brain_ret;
            }
            field_FE_next_state = eScrabStates::State_1_Stand_45E620;
            return 1;

        default:
            return field_110_brain_ret;
    }
}

void Scrab::SetBrain(TBrainType fn)
{
    ::SetBrain(fn, field_10C_fn, sScrabAITable);
}

bool Scrab::BrainIs(TBrainType fn)
{
    return ::BrainIs(fn, field_10C_fn, sScrabAITable);
}

s16 Scrab::HandleRunning()
{
    field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::EnemyStopper_79);
    auto pStopper = static_cast<Path_EnemyStopper*>(field_F0_pTlv);

    if (pStopper)
    {
        if ((pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Left_0 && field_120_pTarget->field_A8_xpos < field_A8_xpos) || (pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Right_1 && field_120_pTarget->field_A8_xpos > field_A8_xpos) || pStopper->field_18_direction == Path_EnemyStopper::StopDirection::Both_2)
        {
            if (!SwitchStates_Get(pStopper->field_1A_id))
            {
                return 1;
            }
        }
    }

    if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_4021A0(0, 1))
        {
            field_FE_next_state = eScrabStates::State_7_HopMidair_45F1A0;
            return 6;
        }
    }
    else
    {
        if (Check_IsOnEndOfLine_4021A0(1, 1))
        {
            field_FE_next_state = eScrabStates::State_7_HopMidair_45F1A0;
            return 6;
        }
    }

    field_FE_next_state = eScrabStates::State_3_Run_45EAB0;
    return 3;
}

s16 Scrab::GetMotionForPatrolType(s16 patrolType)
{
    switch (patrolType)
    {
        case 1:
            if (Math_NextRandom() < 192u)
            {
                return eScrabStates::State_2_Walk_45E730;
            }
            else
            {
                return eScrabStates::State_3_Run_45EAB0;
            }
            break;

        case 2:
            if (Math_NextRandom() < 128u)
            {
                return eScrabStates::State_2_Walk_45E730;
            }
            else
            {
                return eScrabStates::State_3_Run_45EAB0;
            }
            break;

        case 3:
            if (Math_NextRandom() < 64u)
            {
                return eScrabStates::State_2_Walk_45E730;
            }
            else
            {
                return eScrabStates::State_3_Run_45EAB0;
            }
            break;

        case 4:
            return eScrabStates::State_3_Run_45EAB0;

        default:
            return eScrabStates::State_2_Walk_45E730;
    }
}

} // namespace AO

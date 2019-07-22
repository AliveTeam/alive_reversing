#include "stdafx.h"
#include "Scrab.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "Particle.hpp"
#include "Sfx.hpp"

TintEntry stru_560260[15] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u }
};

const TScrabMotionFn sScrab_motion_table_560120[40] =
{
    &Scrab::M_Stand_0_4A8220,
    &Scrab::M_Walk_1_4A84D0,
    &Scrab::M_Run_2_4A89C0,
    &Scrab::M_Turn_3_4A91A0,
    &Scrab::M_RunToStand_4_4A90C0,
    &Scrab::M_HopBegin_5_4A96C0,
    &Scrab::M_HopMidair_6_4A9490,
    &Scrab::M_HopLand_7_4A9890,
    &Scrab::M_JumpToFall_8_4A9220,
    &Scrab::M_StandToWalk_9_4A8450,
    &Scrab::M_StandToRun_10_4A8900,
    &Scrab::M_WalkToStand_11_4A8880,
    &Scrab::M_RunJumpBegin_12_4A99C0,
    &Scrab::M_RunJumpEnd_13_4A9BE0,
    &Scrab::M_WalkToFall_14_4A9460,
    &Scrab::M_RunToFall_15_4A9430,
    &Scrab::M_WalkToRun_16_4A8D60,
    &Scrab::M_RunToWalk_17_4A8D90,
    &Scrab::M_Knockback_18_4AA490,
    &Scrab::M_GetEaten_19_4AA3E0,
    &Scrab::M_Fall_20_4A93E0,
    &Scrab::M_Stamp_21_4A9CC0,
    &Scrab::M_GetPossessed_22_4AA420,
    &Scrab::M_Empty_23_4A9D80,
    &Scrab::M_DeathEnd_24_4AA140,
    &Scrab::M_Empty_25_4A34D0,
    &Scrab::M_HowlBegin_26_4A9DA0,
    &Scrab::M_HowlEnd_27_4A9E60,
    &Scrab::M_GetDepossessedBegin_28_4AA200,
    &Scrab::M_GetDepossessedEnd_29_4AA3C0,
    &Scrab::M_Shriek_30_4A9EA0,
    &Scrab::M_ScrabBattleAnim_31_4A9F30,
    &Scrab::M_AttackSpin_32_4A8DC0,
    &Scrab::M_FeedToGulp_33_4A9FA0,
    &Scrab::M_GulpToStand_34_4A9FF0,
    &Scrab::M_StandToFeed_35_4AA010,
    &Scrab::M_Feed_36_4AA030,
    &Scrab::M_AttackLunge_37_4AA0B0,
    &Scrab::M_LegKick_38_4AA120,
    &Scrab::M_DeathBegin_39_4AA190
};

const TScrabAIFn sScrab_ai_table_56029C[6] =
{
    &Scrab::AI_Patrol_0_4AA630,
    &Scrab::AI_ChasingEnemy_1_4A6470,
    &Scrab::AI_Fighting_2_4A5840,
    &Scrab::AI_Death_3_4A62B0,
    &Scrab::AI_ShrinkDeath_4_4A6420,
    &Scrab::AI_Possessed_5_4A6180
};


const static AIFunctionData<TScrabAIFn> sScrabAITable[6] =
{
    { &Scrab::AI_Patrol_0_4AA630,  0x404070, "AI_Patrol_0", },
    { &Scrab::AI_ChasingEnemy_1_4A6470,  0x403B98, "AI_ChasingEnemy_1", },
    { &Scrab::AI_Fighting_2_4A5840,  0x403E4F, "AI_Fighting_2", },
    { &Scrab::AI_Death_3_4A62B0,  0x4039C2, "AI_Death_3", },
    { &Scrab::AI_ShrinkDeath_4_4A6420,  0x4036B1, "AI_ShrinkDeath_4", },
    { &Scrab::AI_Possessed_5_4A6180,  0x4021F3, "AI_Possessed_5", },
};

void Scrab::SetBrain(TScrabAIFn fn)
{
    ::SetBrain(fn, field_118_brain_state, sScrabAITable);
}

bool Scrab::BrainIs(TScrabAIFn fn)
{
    return ::BrainIs(fn, field_118_brain_state, sScrabAITable);
}

Scrab* Scrab::ctor_4A3C40(Path_Scrab* pTlv, int tlvInfo, __int16 spawnedScale)
{
    ctor_408240(14);
    field_190 = 0;
    field_17C_last_event = -1;
    field_192 = -1;

    SetVTable(this, 0x546DD0);

    field_4_typeId = Types::eScrab_112;

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_124_fight_target_obj_id = -1;
    field_120_obj_id = -1;

    field_10_resources_array.SetAt(0,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 700, 1, 0));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 711, 1, 0));
    field_10_resources_array.SetAt(6,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 706, 1, 0));
    field_10_resources_array.SetAt(8,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 708, 1, 0));
    field_10_resources_array.SetAt(1,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 701, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 710, 1, 0));
    field_10_resources_array.SetAt(5,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 705, 1, 0));
    field_10_resources_array.SetAt(2,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 702, 1, 0));
    field_10_resources_array.SetAt(9,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 709, 1, 0));
    field_10_resources_array.SetAt(3,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 703, 1, 0));
    field_10_resources_array.SetAt(4,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 704, 1, 0));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 713, 1, 0));
    
    Animation_Init_424E10(224764, 156, 69, field_10_resources_array.ItemAt(0), 1, 1);

    field_140 = 0;
    field_12C = 0;
    field_108_next_motion = 0;
    field_110_id = -1;
    field_106_current_motion = 0;
    field_11E = 0;

    field_1AA_flags.Clear(Flags_1AA::eBit1);
    field_1AA_flags.Clear(Flags_1AA::eBit4);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_114_flags.Clear(Flags_114::e114_Bit9);

    field_164 = 0;
    field_16C = 0;
    field_170 = sGnFrame_5C1B84;
    field_1A2 = 0;
    field_1A4 = 0;
    field_1A6 = 25;
    field_178 = 0;
    field_154 = 0;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    if (spawnedScale == 1)
    {
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else if (spawnedScale == 2)
    {
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    field_128_attack_delay = pTlv->field_12_attack_delay;
    field_12A_patrol_type = pTlv->field_14_patrol_type;
    field_158_left_min_delay = pTlv->field_16_left_min_delay;
    field_15A_left_max_delay = pTlv->field_18_left_max_delay;
    field_15C = pTlv->field_1A_right_min_delay;
    field_15E_right_max_delay = pTlv->field_1C_right_max_delay;
    field_148_attack_duration = pTlv->field_1E_attack_duration;
    field_174_whirl_attack_duration = pTlv->field_26_whirl_attack_duration;
    field_176_whirl_attack_recharge = pTlv->field_28_whirl_attack_recharge;
    field_1A8_kill_close_fleech = pTlv->field_2A_kill_close_fleech;

    field_1A0 = 3;

    field_1AA_flags.Set(Flags_1AA::eBit5_roar_randomly, pTlv->field_22_roar_randomly & 1);
    field_1AA_flags.Set(Flags_1AA::eBit6_persistant, pTlv->field_24_persistant & 1);
    field_1AA_flags.Clear(Flags_1AA::eBit3);

    if (!sub_4A41E0())
    {
        field_106_current_motion = 8;
        vUpdateAnim_4A34F0();
        field_1C_update_delay = 30;
    }

    SetTint_425600(&stru_560260[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    field_DC_bApplyShadows |= 2u;

    field_14C = 0;
    field_150 = 0;
    field_144_tlvInfo = tlvInfo;
    field_11C_sub_state = 0;

    sub_4AA600();

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* Scrab::VDestructor(signed int flags)
{
    return vdtor_4A41B0(flags);
}

void Scrab::VUpdate()
{
    vUpdate_4A3530();
}

Scrab* Scrab::vdtor_4A41B0(signed int flags)
{
    dtor_4A42B0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Scrab::dtor_4A42B0()
{
    SetVTable(this, 0x546DD0);

    field_120_obj_id = -1;
    field_124_fight_target_obj_id = -1;

    vOnTrapDoorOpen_4A7ED0();

    if (field_10C_health <= FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_144_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_144_tlvInfo, -1, 0, 0);
    }

    MusicController::sub_47FD60(0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_166_level,
                field_168_path,
                field_16A_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }
    dtor_4080B0();
}

void Scrab::vOnTrapDoorOpen_4A7ED0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_1AA_flags.Set(Flags_1AA::eBit4);
        field_110_id = -1;
        field_106_current_motion = 15;
    }
}

void Scrab::vUpdateAnim_4A34F0()
{
    NOT_IMPLEMENTED();
}

__int16 Scrab::sub_4A41E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Scrab::sub_4AA600()
{
    NOT_IMPLEMENTED();
}

const FP dword_546D84[8] = 
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

const FP dword_546DA4[11] = 
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
    FP_FromInteger(0)
};


void Scrab::vUpdate_4A3530()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
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
        }
        field_104_collision_line_type = 0;
        field_120_obj_id = BaseGameObject::Find_Flags_4DC170(field_120_obj_id);
        field_124_fight_target_obj_id = BaseGameObject::Find_Flags_4DC170(field_124_fight_target_obj_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDelta = FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos);
    if (xDelta <= FP_FromInteger(750))
    {
        const FP yDelta = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);
        if (yDelta <= FP_FromInteger(520))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }

            if (!Input_IsChanting_45F260())
            {
                field_164 = 0;
            }

            if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
            {
                // Handle DDCheat mode

                // TODO: InputCommand constants
                field_100_pCollisionLine = nullptr;
                if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0xF)
                {
                    field_C4_velx = dword_546D84[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                    field_C8_vely = dword_546DA4[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];

                    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0x10)
                    {
                        field_C4_velx += dword_546D84[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                        field_C4_velx += dword_546D84[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                        field_C8_vely += dword_546DA4[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                    }

                    field_B8_xpos += field_C4_velx;
                    field_BC_ypos += field_C8_vely;

                    // Keep in map bounds
                    PSX_Point point = {};
                    gMap_5C3030.Get_map_size_480640(&point);
                    if (field_B8_xpos < FP_FromInteger(0))
                    {
                        field_B8_xpos = FP_FromInteger(0);
                    }

                    if (field_B8_xpos >= FP_FromInteger(point.field_0_x))
                    {
                        field_B8_xpos = FP_FromInteger(point.field_0_x) - FP_FromInteger(1);
                    }

                    if (field_BC_ypos < FP_FromInteger(0))
                    {
                        field_BC_ypos = FP_FromInteger(0);
                    }

                    if (field_BC_ypos >= FP_FromInteger(point.field_2_y))
                    {
                        field_BC_ypos = FP_FromInteger(point.field_2_y) - FP_FromInteger(1);
                    }
                }
                else
                {
                    field_C4_velx = FP_FromInteger(0);
                    field_C8_vely = FP_FromInteger(0);
                }
                
                sub_408C40();
                field_F8_LastLineYPos = field_BC_ypos;
                return;
            }

            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                1))
            {
                field_1A2 = 0;
            }

            const auto oldMotion = field_106_current_motion;

            const auto oldBrain = field_118_brain_state;

            field_11C_sub_state = (this->*field_118_brain_state)();

            if (sDDCheat_ShowAI_Info_5C1BD8)
            {
                DDCheat::DebugStr_4F5560(
                    "Scrab %d %d %d %d\n",
                    field_11C_sub_state,
                    field_12C,
                    field_106_current_motion,
                    field_108_next_motion);
            }

            field_19C = field_BC_ypos;
            field_198 = field_B8_xpos;

            (this->*sScrab_motion_table_560120[field_106_current_motion])();

            if (field_198 != field_B8_xpos || field_19C != field_BC_ypos)
            {
                field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                    nullptr,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_B8_xpos,
                    field_BC_ypos);
                VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
            }

            // TODO: This is extra debug logging to figure out the motion names
            if (oldBrain != field_118_brain_state)
            {
                LOG_INFO("Scrab: Old brain = " << GetOriginalFn(oldBrain, sScrabAITable).fnName << " new brain = " << GetOriginalFn(field_118_brain_state, sScrabAITable).fnName);

                //LOG_INFO("Scrab: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
            }

            if (oldMotion != field_106_current_motion || field_1AA_flags.Get(Flags_1AA::eBit4))
            {
                field_1AA_flags.Clear(Flags_1AA::eBit4);
                vUpdateAnim_4A34F0();
            }
            else if (field_11E)
            {
                field_106_current_motion = field_F4;
                vUpdateAnim_4A34F0();
                field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                field_11E = 0;
            }
            
            Update_Slurg_Step_Watch_Points_4A5780();
            
            if (field_178)
            {
                if (sControlledCharacter_5C1B8C != this)
                {
                    field_13C = field_BC_ypos;
                    return;
                }

                if (field_10C_health > FP_FromInteger(0) && !(static_cast<int>(sGnFrame_5C1B84) % 4))
                {
                    short v21 = Math_RandomRange_496AB0(40, 50);
                    const short v22 = Math_RandomRange_496AB0(45, 55);
                    FP v23 = (FP_FromInteger(-7) * field_CC_sprite_scale);
                    FP v24 = (FP_FromDouble(0.3) * field_CC_sprite_scale);
                    FP point2 = v24;

                    if (Math_NextRandom() & 1)
                    {
                        v23 = -v23;
                        v21 = -v21;
                    }

                    if (Math_NextRandom() & 1)
                    {
                        point2 = -v24;
                    }

                    const FP v26 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(v22));
                    const FP v27 = (field_CC_sprite_scale * FP_FromInteger(v21));

                    New_Particle_426AA0(
                        v27 + field_B8_xpos,
                        v26,
                        v23,
                        point2,
                        field_CC_sprite_scale,
                        0,
                        255,
                        0,
                        0);
                }
            }

            if (sControlledCharacter_5C1B8C == this && field_110_id != -1)
            {
                field_C8_vely = field_BC_ypos - field_13C;
                sub_408C40();
            }
            field_13C = field_BC_ypos;
            return;
        }
    }

    if (field_1AA_flags.Get(Flags_1AA::eBit6_persistant))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Scrab::Update_Slurg_Step_Watch_Points_4A5780()
{
    NOT_IMPLEMENTED();
}

__int16 Scrab::AI_Patrol_0_4AA630()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Scrab::AI_ChasingEnemy_1_4A6470()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Scrab::AI_Fighting_2_4A5840()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Scrab::AI_Death_3_4A62B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Scrab::AI_ShrinkDeath_4_4A6420()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Scrab::AI_Possessed_5_4A6180()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Scrab::M_Stand_0_4A8220()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Walk_1_4A84D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Run_2_4A89C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Turn_3_4A91A0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_RunToStand_4_4A90C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_HopBegin_5_4A96C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_HopMidair_6_4A9490()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_HopLand_7_4A9890()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_JumpToFall_8_4A9220()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_StandToWalk_9_4A8450()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_StandToRun_10_4A8900()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_WalkToStand_11_4A8880()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_RunJumpBegin_12_4A99C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_RunJumpEnd_13_4A9BE0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_WalkToFall_14_4A9460()
{
    M_JumpToFall_8_4A9220();
    if (field_106_current_motion == 0)
    {
        field_106_current_motion = 7;
    };
}

void Scrab::M_RunToFall_15_4A9430()
{
    M_JumpToFall_8_4A9220();
    if (field_106_current_motion == 0)
    {
        field_106_current_motion = 7;
    };
}

void Scrab::M_WalkToRun_16_4A8D60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = 2;
        field_108_next_motion = -1;
    }
}

void Scrab::M_RunToWalk_17_4A8D90()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = 1;
        field_108_next_motion = -1;
    }
}

void Scrab::M_Knockback_18_4AA490()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_GetEaten_19_4AA3E0()
{
    if (field_20_animation.field_92_current_frame == 3)
    {
        SFX_Play_46FBA0(28u, 120, -1000);
    }
}

void Scrab::M_Fall_20_4A93E0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Stamp_21_4A9CC0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_GetPossessed_22_4AA420()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Empty_23_4A9D80()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_DeathEnd_24_4AA140()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Empty_25_4A34D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_HowlBegin_26_4A9DA0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_HowlEnd_27_4A9E60()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_GetDepossessedBegin_28_4AA200()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_GetDepossessedEnd_29_4AA3C0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Shriek_30_4A9EA0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_ScrabBattleAnim_31_4A9F30()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_AttackSpin_32_4A8DC0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_FeedToGulp_33_4A9FA0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_GulpToStand_34_4A9FF0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_StandToFeed_35_4AA010()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_Feed_36_4AA030()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_AttackLunge_37_4AA0B0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_LegKick_38_4AA120()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_DeathBegin_39_4AA190()
{
    NOT_IMPLEMENTED();
}

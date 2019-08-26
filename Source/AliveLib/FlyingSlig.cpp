#include "stdafx.h"
#include "FlyingSlig.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "DDCheat.hpp"
#include "Slig.hpp"
#include "Grenade.hpp"
#include "Dove.hpp"
#include "NakedSlig.hpp"
#include "Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "GameSpeak.hpp"
#include "Explosion.hpp"
#include "Bullet.hpp"

ALIVE_ARY(1, 0x5523A0, TFlyingSligFn, 26, sFlyingSlig_motion_table_5523A0,
{
    &FlyingSlig::M_Idle_0_4385E0,
    &FlyingSlig::M_HorizontalMovement_1_4386A0,
    &FlyingSlig::M_IdleToTurn_2_4388B0,
    &FlyingSlig::M_DownMovement_3_438AA0,
    &FlyingSlig::M_DownMovementToTurn_4_438CC0,
    &FlyingSlig::M_UpMovement_5_438DD0,
    &FlyingSlig::M_UpMovementToTurn_6_439030,
    &FlyingSlig::M_LeverPull_7_439150,
    &FlyingSlig::M_GameSpeak_8_4391D0,
    &FlyingSlig::M_Possession_9_434290,
    &FlyingSlig::M_EndHorizontalMovement_10_4387D0,
    &FlyingSlig::M_BeginUpMovement_11_438E40,
    &FlyingSlig::M_HorizontalToDownMovement_12_438B10,
    &FlyingSlig::M_UpToHorizontalMovement_13_438F60,
    &FlyingSlig::M_DownToHorizontalMovement_14_438BF0,
    &FlyingSlig::M_QuickTurn_15_4387F0,
    &FlyingSlig::M_IdleToHorizontalMovement_16_438730,
    &FlyingSlig::M_BeginDownMovement_17_438B80,
    &FlyingSlig::M_EndDownMovement_18_438C90,
    &FlyingSlig::M_DownKnockback_19_4390D0,
    &FlyingSlig::M_UpKnockback_20_439110,
    &FlyingSlig::M_EndUpMovement_21_438EB0,
    &FlyingSlig::M_InstantUpXTurn_22_438EE0,
    &FlyingSlig::M_InstantDownXTurn_23_438F20,
    &FlyingSlig::M_HorizontalToUpMovement_24_438D60,
    &FlyingSlig::M_TurnToHorizontalMovement_25_4389E0,
});

ALIVE_ARY(1, 0x552350, TFlyingSligFn, 18, sFlyingSlig_AI_table_552350,
{
    &FlyingSlig::AI_Inactive_0_4355B0,
    &FlyingSlig::AI_Death_1_4364E0,
    &FlyingSlig::AI_Moving_2_4356D0,
    &FlyingSlig::AI_GetAlerted_3_435750,
    &FlyingSlig::AI_ChasingEnemy_4_435BC0,
    &FlyingSlig::AI_Idle_5_435820,
    &FlyingSlig::AI_GameSpeakToMoving_6_435940,
    &FlyingSlig::AI_PanicMoving_7_435990,
    &FlyingSlig::AI_PanicIdle_8_435AC0,
    &FlyingSlig::AI_SpottedEnemy_9_435E40,
    &FlyingSlig::AI_LaunchingGrenade_10_435F10,
    &FlyingSlig::AI_AbeDead_11_435FD0,
    &FlyingSlig::AI_Possessed_12_436040,
    &FlyingSlig::AI_Possession_13_4360F0,
    &FlyingSlig::AI_DePossession_14_436180,
    &FlyingSlig::AI_FlyingSligSpawn_15_4362C0,
    &FlyingSlig::AI_Death_1_4364E0,
    &FlyingSlig::AI_FromNakedSlig_17_4355E0,
});

const static AIFunctionData<TFlyingSligFn> sFlyingSligAITable[18] =
{
    { &FlyingSlig::AI_Inactive_0_4355B0, 0x401802 , "AI_Inactive_0" },
    { &FlyingSlig::AI_Death_1_4364E0, 0x402ABD , "AI_Death_1 (first)" },
    { &FlyingSlig::AI_Moving_2_4356D0, 0x4046BF, "AI_Moving_2" },
    { &FlyingSlig::AI_GetAlerted_3_435750, 0x402603, "AI_GetAlerted_3" },
    { &FlyingSlig::AI_ChasingEnemy_4_435BC0, 0x402086, "AI_ChasingEnemy_4" },
    { &FlyingSlig::AI_Idle_5_435820, 0x4028A6, "AI_Idle_5" },
    { &FlyingSlig::AI_GameSpeakToMoving_6_435940, 0x403E31, "AI_GameSpeakToMoving_6" },
    { &FlyingSlig::AI_PanicMoving_7_435990, 0x404453 , "AI_PanicMoving_7" },
    { &FlyingSlig::AI_PanicIdle_8_435AC0, 0x4013A2, "AI_PanicIdle_8" },
    { &FlyingSlig::AI_SpottedEnemy_9_435E40, 0x40394A, "AI_SpottedEnemy_9" },
    { &FlyingSlig::AI_LaunchingGrenade_10_435F10, 0x40253B, "AI_LaunchingGrenade_10" },
    { &FlyingSlig::AI_AbeDead_11_435FD0, 0x40469C, "AI_AbeDead_11" },
    { &FlyingSlig::AI_Possessed_12_436040, 0x404480, "AI_Possessed_12" },
    { &FlyingSlig::AI_Possession_13_4360F0, 0x40241E , "AI_Possession_13" },
    { &FlyingSlig::AI_DePossession_14_436180, 0x403DBE, "AI_DePossession_14" },
    { &FlyingSlig::AI_FlyingSligSpawn_15_4362C0, 0x40128A , "AI_FlyingSligSpawn_15" },
    { &FlyingSlig::AI_Death_1_4364E0, 0x402ABD, "AI_Death_1 (second)" },
    { &FlyingSlig::AI_FromNakedSlig_17_4355E0, 0x401749, "AI_FromNakedSlig_17" },
};

void FlyingSlig::SetBrain(TFlyingSligFn fn)
{
    ::SetBrain(fn, field_29C_brain_state, sFlyingSligAITable);
}

bool FlyingSlig::BrainIs(TFlyingSligFn fn)
{
    return ::BrainIs(fn, field_29C_brain_state, sFlyingSligAITable);
}

FlyingSlig* FlyingSlig::ctor_4342B0(Path_FlyingSlig* pTlv, int tlvInfo)
{
    //BaseCtor_4340B0(9); // Omitted for direct call
    ctor_408240(9);

    field_178 = 0;
    field_164 = -1;
    field_17A = -1;

    SetVTable(this, 0x545120);

    if (tlvInfo != -1)
    {
        field_C_objectId = tlvInfo;
    }

    field_4_typeId = Types::eFlyingSlig_54;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kFlySligResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSligBlowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSmallExplo2ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMetalResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kBigflashResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kVaporResID, TRUE, FALSE));
    field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, TRUE, FALSE));

    Animation_Init_424E10(
        116888,
        107,
        48u,
        field_10_resources_array.ItemAt(0),
        1,
        1u);

    field_15E = 0;

    field_20_animation.field_1C_fn_ptr_array = kFlyingSlig_Anim_Frames_Fns_55EFC4;

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_6_flags.Set(BaseGameObject::eCanExplode);

    field_14C_hi_pause_timer = 0;
    field_DC_bApplyShadows |= 2u;

    field_15C = 45 * ((Math_NextRandom() % 5) - 2);

    field_150_grenade_delay = 0;
    field_154 = 0;

    field_17E_flags.Clear(Flags_17E::eBit1);
    field_17E_flags.Clear(Flags_17E::eBit5);
    field_17E_flags.Clear(Flags_17E::eBit6);
    field_17E_flags.Clear(Flags_17E::eBit7);
    field_17E_flags.Clear(Flags_17E::eBit8);
    field_17E_flags.Clear(Flags_17E::eBit9);
    field_17E_flags.Clear(Flags_17E::eBit10);

    field_158_obj_id = -1;

    field_288 = 0;
    field_290 = 0;
    field_284 = 0;
    field_28C = 0;
    field_1E8 = 0;
    field_184 = 0;
    field_188_ySpeed = FP_FromInteger(0);
    field_190 = 0;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_data = *pTlv;

    field_148_tlvInfo = tlvInfo;

    SetBrain(&FlyingSlig::AI_Inactive_0_4355B0);

    field_106_current_motion = eFlyingSligMotions::M_Idle_0_4385E0;

    if (field_118_data.field_10_data.field_2_state == 1)
    {
        field_14C_hi_pause_timer = sGnFrame_5C1B84 + field_118_data.field_10_data.field_4_hi_pause_time;
    }
    else if (field_118_data.field_10_data.field_2_state == 0)
    {
        field_14C_hi_pause_timer = sGnFrame_5C1B84 + 1;
    }

    field_2A8 = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2AC = FP_FromInteger(-field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2B0 = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2B4 = FP_FromDouble(0.4) * field_CC_sprite_scale;
    field_2B8 = FP_FromDouble(0.4) * field_CC_sprite_scale;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_118_data.field_10_data.field_A_direction == 0);

    if (field_118_data.field_10_data.field_0_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 14;
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 33;
        field_D6_scale = 1;
    }

    field_17E_flags.Set(Flags_17E::eBit13_Persistant, field_118_data.field_10_data.field_1E_persistant & 1);

    field_17C_launch_id |= field_118_data.field_10_data.field_1C_launch_id;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    FP hitX = {};
    FP hitY = {};
    sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        0x100);

    if (field_100_pCollisionLine)
    {
        field_B8_xpos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.x);
        field_BC_ypos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.y);
        sub_4348A0();
        field_194 = FP_FromInteger(0);
    }

    field_1D8 = 0;
    field_1DC = 0;
    field_1E0 = 0;
    field_1E4 = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

void FlyingSlig::dtor_434990()
{
    SetVTable(this, 0x545120);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        MusicController::sub_47FD60(0, this, 0, 0);
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_2A0_abe_level,
                field_2A2_abe_path,
                field_2A4_abe_camera, 
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_148_tlvInfo);
    if (pTlv)
    {
        if (pTlv->field_4_type != TlvTypes::SligGetWings_105 && pTlv->field_4_type != TlvTypes::FlyingSligSpawner_92)
        {
            if (field_10C_health <= FP_FromInteger(0))
            {
                Path::TLV_Reset_4DB8E0(field_148_tlvInfo, -1, 0, 1);
            }
            else
            {
                Path::TLV_Reset_4DB8E0(field_148_tlvInfo, -1, 0, 0);
            }
        }
    }

    BaseAliveGameObject::dtor_4080B0();
}

FlyingSlig* FlyingSlig::vdtor_434870(signed int flags)
{
    dtor_434990();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void FlyingSlig::vScreenChanged_434C10()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710() ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId &&
        (this != sControlledCharacter_5C1B8C ||
        field_17E_flags.Get(Flags_17E::eBit13_Persistant)))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void FlyingSlig::vUpdate_434AD0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);

        if (!IsPossessed_436A90())
        {
            if (field_104_collision_line_type != -1)
            {
                const FP savedNextXPos = field_294_nextXPos;
                const FP savedNextYPos = field_298_nextYPos;
                field_100_pCollisionLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_104_collision_line_type);
                sub_4348A0();
                field_294_nextXPos = savedNextXPos;
                field_298_nextYPos = savedNextYPos;
            }
        }

        if (field_158_obj_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_158_obj_id)
                {
                    field_158_obj_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        field_B8_xpos = field_294_nextXPos;
        field_BC_ypos = field_298_nextYPos;

        const auto oldBrain = field_29C_brain_state;

        (this->*(field_29C_brain_state))();

        (this->*(sFlyingSlig_motion_table_5523A0)[field_106_current_motion])();

        if (oldBrain != field_29C_brain_state)
        {
            LOG_INFO("FlyingSlig: Old brain = " << GetOriginalFn(oldBrain, sFlyingSligAITable).fnName << " new brain = " << GetOriginalFn(field_29C_brain_state, sFlyingSligAITable).fnName);
            //LOG_INFO("FlyingSlig: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        sub_4396E0();
    }
}

void FlyingSlig::sub_4348A0()
{
    field_17E_flags.Clear(Flags_17E::eBit3);
    field_17E_flags.Clear(Flags_17E::eBit2);
    field_298_nextYPos = field_BC_ypos;
    field_294_nextXPos = field_B8_xpos;
    field_18C = 0;
    field_190 = 0;
    sub_437C70(field_100_pCollisionLine);
    const short v5 = FP_GetExponent(field_BC_ypos - field_1A8);
    const short v6 = FP_GetExponent(field_B8_xpos - field_1A4);
    field_194 = FP_FromInteger(Math_SquareRoot_Int_496E70(v5 * v5 + v6 * v6));
    field_17E_flags.Set(Flags_17E::eBit4, field_118_data.field_10_data.field_A_direction == 0);
}

void FlyingSlig::sub_4396E0()
{
    NOT_IMPLEMENTED();
}

__int16 FlyingSlig::vTakeDamage_434C90(BaseGameObject* pFrom)
{
    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
    {
        if (static_cast<Bullet*>(pFrom)->field_20_type == BulletType::Type_3)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            Path_TLV* pTlv = nullptr;
            do
            {
                pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
                    field_B8_xpos,
                    FP_FromInteger(bRect.y),
                    field_B8_xpos,
                    FP_FromInteger(bRect.y));

                if (pTlv->field_4_type == TlvTypes::ZSligCover_50)
                {
                    // Left/right in cover
                    if (bRect.x >= pTlv->field_8_top_left.field_0_x &&
                        bRect.x <= pTlv->field_C_bottom_right.field_0_x &&
                        bRect.y >= pTlv->field_8_top_left.field_2_y &&
                        bRect.y <= pTlv->field_C_bottom_right.field_2_y)
                    {
                        // Top/bottom in cover
                        if (bRect.w >= pTlv->field_8_top_left.field_0_x &&
                            bRect.w <= pTlv->field_C_bottom_right.field_0_x &&
                            bRect.h >= pTlv->field_8_top_left.field_2_y &&
                            bRect.h <= pTlv->field_C_bottom_right.field_2_y)
                        {
                            return 0;
                        }
                    }
                }
            } while (pTlv);
        }
        // Not in Z-Cover, fall through and be shot
    }
    case Types::eGrinder_30:
    case Types::eRockSpawner_48:
    case Types::eType_Abe_69:
    case Types::eType_86:
    case Types::eMineCar_89:
    case Types::eType_107:
    case Types::eSlog_126:
    {
        if (BrainIs(&FlyingSlig::AI_Death_1_4364E0))
        {
            return 1;
        }
        BlowUp_436510();
        auto pExplosion = alive_new<Explosion>();
        if (!pExplosion)
        {
            return 1;
        }
        pExplosion->ctor_4A1200(field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)), field_CC_sprite_scale, 1);
        return 1;
    }

    case Types::eElectricWall_39:
        Sfx_Slig_GameSpeak_4C04F0(SligSpeak::Help_10, 0, field_15C, this);
        break;

    case Types::eBaseBomb_46:
    case Types::eExplosion_109:
        if (BrainIs(&FlyingSlig::AI_Death_1_4364E0))
        {
            BlowUp_436510();
        }
        break;

    case Types::eElectrocute_150:
        if (BrainIs(&FlyingSlig::AI_Death_1_4364E0))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_10C_health = FP_FromInteger(0);
            SetBrain(&FlyingSlig::AI_Death_1_4364E0);
            field_14C_hi_pause_timer = sGnFrame_5C1B84;
        }
        break;

    default:
        return 1;
    }
    return 1;
}

void FlyingSlig::AI_Inactive_0_4355B0()
{
    if (static_cast<int>(sGnFrame_5C1B84) >= field_14C_hi_pause_timer)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::AI_Death_1_4364E0()
{
    if (static_cast<int>(sGnFrame_5C1B84) >= field_14C_hi_pause_timer)
    {
        field_6_flags.Set(BaseGameObject::eDead);
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
    }
}

void FlyingSlig::AI_Moving_2_4356D0()
{
    if (!sub_436730() && sub_4374A0(1) == 1)
    {
        field_17E_flags.Toggle(Flags_17E::eBit4);
        PatrolDelay_435860();
    }
}

void FlyingSlig::AI_GetAlerted_3_435750()
{
    if (field_106_current_motion != eFlyingSligMotions::M_Idle_0_4385E0)
    {
        field_14C_hi_pause_timer++;
    }
    else if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
    {
        if (!sub_436730() && static_cast<int>(sGnFrame_5C1B84) >= field_14C_hi_pause_timer)
        {
            ToMoving_435720();
        }
    }
    else
    {
        field_17E_flags.Set(Flags_17E::eBit6);
        field_14C_hi_pause_timer++;
    }
}

void FlyingSlig::AI_ChasingEnemy_4_435BC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Idle_5_435820()
{
    if (!sub_436730() && static_cast<int>(sGnFrame_5C1B84) >= field_14C_hi_pause_timer)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::AI_GameSpeakToMoving_6_435940()
{
    if (field_106_current_motion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::AI_PanicMoving_7_435990()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToChase_435E10();
        return;
    }

    if (!Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        ToMoving_435720();
        return;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_14C_hi_pause_timer)
    {
        if (sub_4374A0(0) != 1)
        {
            return;
        }
        field_17E_flags.Toggle(Flags_17E::eBit4);
    }

    ToPanicIdle_435B50();
}

void FlyingSlig::AI_PanicIdle_8_435AC0()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToChase_435E10();
    }
    else if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        if (static_cast<int>(sGnFrame_5C1B84) >= field_14C_hi_pause_timer && field_106_current_motion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
        {
            ToPanicMoving_435A50();
        }
    }
    else
    {
        ToMoving_435720();
    }
}

void FlyingSlig::AI_SpottedEnemy_9_435E40()
{
    if (static_cast<int>(sGnFrame_5C1B84) >= field_14C_hi_pause_timer)
    {
        ToChase_435E10();
    }
}

void FlyingSlig::AI_LaunchingGrenade_10_435F10()
{
    if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
    {
        field_17E_flags.Set(Flags_17E::eBit5);
    }
    ToChase_435E10();
}

void FlyingSlig::AI_AbeDead_11_435FD0()
{
    if (static_cast<int>(sGnFrame_5C1B84) == field_14C_hi_pause_timer - 45)
    {
        sub_436A50(SligSpeak ::GotYa_12, 0);
    }
    else if (static_cast<int>(sGnFrame_5C1B84) == field_14C_hi_pause_timer)
    {
        sub_436A50(SligSpeak::Laugh_3, 0);
    }
}

void FlyingSlig::AI_Possessed_12_436040()
{
    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        MusicController::sub_47FD60(9, this, 0, 0);
    }

    if (!field_17E_flags.Get(Flags_17E::eBit9))
    {
        if (Input_IsChanting_45F260())
        {
            ToChantShake_436270();
        }
    }

    if (!Input_IsChanting_45F260())
    {
        field_17E_flags.Clear(Flags_17E::eBit9);
    }

    HandlePlayerControls_439340();
}

void FlyingSlig::AI_Possession_13_4360F0()
{
    if (static_cast<int>(sGnFrame_5C1B84) > field_14C_hi_pause_timer)
    {
        sub_436A50(SligSpeak ::Hi_0, 0x1FF);
        ToPlayerControlled_4360C0();
    }
}

void FlyingSlig::AI_DePossession_14_436180()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_FlyingSligSpawn_15_4362C0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_FromNakedSlig_17_4355E0()
{
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos - FP_FromInteger(5),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        0x100) == 1)
    {
        field_B8_xpos = hitX;
        field_BC_ypos = hitY;
        sub_4348A0();
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_188_ySpeed = FP_FromInteger(0);
        field_184 = 0;
        PatrolDelay_435860();
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromDouble(-4.5);
        field_184 = 0;
        field_188_ySpeed = FP_FromDouble(-0.5);
    }

}

void FlyingSlig::M_Idle_0_4385E0()
{
    if (!field_28C)
    {
        field_28C = 2;
        field_290 = 0;
    }

    if (field_184)
    {
        if (sub_43A510())
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730);
            field_28C = 1;
            field_290 = 0;
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_IdleToTurn_2_4388B0);
        }
    }
    else
    {
        if (field_188_ySpeed > FP_FromInteger(0))
        {
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
            }
        }
    }
}

void FlyingSlig::M_HorizontalMovement_1_4386A0()
{
    if (field_188_ySpeed < FP_FromInteger(0))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
    }
    else if (field_188_ySpeed > FP_FromInteger(0))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
    }
    else if (!field_184)
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
    }
    else if (!sub_43A510())
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        field_28C = 3;
        field_290 = 0;
    }
}

void FlyingSlig::M_IdleToTurn_2_4388B0()
{
    if (field_20_animation.field_92_current_frame == 4)
    {
        if (field_184)
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                if (field_188_ySpeed <= FP_FromInteger(0))
                {
                    if (sub_43A510())
                    {
                        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                        VSetMotion_4081C0(eFlyingSligMotions::M_TurnToHorizontalMovement_25_4389E0);
                    }
                    else
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                    field_28C = 0;
                }
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
                field_28C = 0;
            }
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
        }
    }
}

void FlyingSlig::M_DownMovement_3_438AA0()
{
    if (!field_184 || sub_43A510())
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            if (field_184)
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            }
        }
    }
    else
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
    }
}

void FlyingSlig::M_DownMovementToTurn_4_438CC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (!field_184 || sub_43A510())
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
            }
            else if (field_184)
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
        }
    }
}

void FlyingSlig::M_UpMovement_5_438DD0()
{
    if (!field_184 || sub_43A510())
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_184)
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            }
        }
    }
    else
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
    }
}

void FlyingSlig::M_UpMovementToTurn_6_439030()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (!field_184 || sub_43A510())
        {
            if (field_188_ySpeed < FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
            }
            else if (field_184)
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
        }
    }
}

void FlyingSlig::M_LeverPull_7_439150()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_GameSpeak_8_4391D0()
{
    if (field_20_animation.field_92_current_frame == 1 && field_17E_flags.Get(Flags_17E::eBit1))
    {
        field_17E_flags.Clear(Flags_17E::eBit1);

        if (BrainIs(&FlyingSlig::AI_Possessed_12_436040))
        {
            switch (field_17D)
            {
            case SligSpeak::Hi_0:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_Hi_27);
                break;
            case SligSpeak::HereBoy_1:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_HereBoy_28);
                break;
            case SligSpeak::GetEm_2:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_GetEm_29);
                break;
            case SligSpeak::Laugh_3:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_Laugh_8);
                break;
            case SligSpeak::Bullshit_5:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_BS_5);
                break;
            case SligSpeak::LookOut_6:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_LookOut_6);
                break;
            case SligSpeak::Bullshit2_7:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_BS2_7);
                break;
            case SligSpeak::Freeze_8:
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_Freeze_31);
                break;
            default:
                break;
            }
        }
        Sfx_Slig_GameSpeak_4C04F0(field_17D, 0, field_160, this);
        Event_Broadcast_422BC0(kEventSpeaking, this);
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        this->VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_Possession_9_434290()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_EndHorizontalMovement_10_4387D0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_BeginUpMovement_11_438E40()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184)
        {
            if (!sub_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_HorizontalToDownMovement_12_438B10()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184)
        {
            if (!sub_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_UpToHorizontalMovement_13_438F60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
        }
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C = 0;
            }
            else if (field_188_ySpeed < FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
                field_28C = 0;
            }
            else if (!field_184)
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
            }
            else if (sub_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
            }
        }
    }
}

void FlyingSlig::M_DownToHorizontalMovement_14_438BF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184)
                {
                    if (sub_43A510())
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
                    }
                    else
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
                }
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C = 0;
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C = 0;
        }
    }
}

void FlyingSlig::M_QuickTurn_15_4387F0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_188_ySpeed < FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
            field_28C = 0;
        }
        else if (!field_184)
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
        }
        else if (!sub_43A510())
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
    }
}

void FlyingSlig::M_IdleToHorizontalMovement_16_438730()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed < FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
            field_28C = 0;
        }
        else if (!field_184)
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
        }
        else if (sub_43A510())
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        }
    }
}

void FlyingSlig::M_BeginDownMovement_17_438B80()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184)
        {
            if (!sub_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_EndDownMovement_18_438C90()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
        }
    }
}

void FlyingSlig::M_DownKnockback_19_4390D0()
{
    if (abs(field_184) > 6553 || field_188_ySpeed < FP_FromDouble(0.1))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
    }
}

void FlyingSlig::M_UpKnockback_20_439110()
{
    if (abs(field_184) > 6553 || field_188_ySpeed > FP_FromDouble(-0.1))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_EndUpMovement_21_438EB0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
        }
    }
}

void FlyingSlig::M_InstantUpXTurn_22_438EE0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_InstantDownXTurn_23_438F20()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_HorizontalToUpMovement_24_438D60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184)
        {
            if (!sub_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_TurnToHorizontalMovement_25_4389E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184)
                {
                    if (sub_43A510())
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
                    }
                    else
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
                }
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C = 0;
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C = 0;
        }
    }
}

signed __int16 FlyingSlig::sub_43A510()
{
    NOT_IMPLEMENTED();
    return ((field_184 > 0 && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
         || (field_184 < 0 &&   field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)));
}

void FlyingSlig::ToPlayerControlled_4360C0()
{
    VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    SetBrain(&FlyingSlig::AI_Possessed_12_436040);
}

void FlyingSlig::ToMoving_435720()
{
    MusicController::sub_47FD60(4, this, 0, 0);
    SetBrain(&FlyingSlig::AI_Moving_2_4356D0);
}

void FlyingSlig::ToPanicIdle_435B50()
{
    sub_436A50(SligSpeak::Help_10, 0);
    field_14C_hi_pause_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + field_118_data.field_10_data.field_C_panic_delay;
    SetBrain(&FlyingSlig::AI_PanicIdle_8_435AC0);
}

void FlyingSlig::ToChase_435E10()
{
    MusicController::sub_47FD60(8, this, 0, 0);
    SetBrain(&FlyingSlig::AI_ChasingEnemy_4_435BC0);
}

__int16 FlyingSlig::IsPossessed_436A90()
{
    return
        BrainIs(&FlyingSlig::AI_Possessed_12_436040) ||
        BrainIs(&FlyingSlig::AI_FlyingSligSpawn_15_4362C0) ||
        BrainIs(&FlyingSlig::AI_FromNakedSlig_17_4355E0) ||
        BrainIs(&FlyingSlig::AI_Possession_13_4360F0) ||
        BrainIs(&FlyingSlig::AI_DePossession_14_436180);
}

__int16 FlyingSlig::CanChase_436850(BaseAliveGameObject* pObj)
{
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) ||
        !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) ||
        Event_Get_422C00(kEventResetting) || 
        IsAbeEnteringDoor_43B030(pObj) ||
        sActiveHero_5C1B68->field_CC_sprite_scale != field_CC_sprite_scale ||
        !IsWallBetween_43A550(this, pObj))
    {
        return 0;
    }

    if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        return 1;
    }

    if (vIsFacingMe_4254A0(pObj) && !IsInInvisibleZone_425710(pObj) && !pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        return 1;
    }

    return 0;
}

void FlyingSlig::sub_436A50(SligSpeak  a2, __int16 a3)
{
    if (field_106_current_motion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
    {
        field_17E_flags.Set(Flags_17E::eBit1);
        field_17E_flags.Set(Flags_17E::eBit10);
        field_17D = a2;
        field_160 = a3;
    }
}

__int16 FlyingSlig::sub_4374A0(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 CCSTD FlyingSlig::IsAbeEnteringDoor_43B030(BaseAliveGameObject* pThis)
{
    return Slig::IsAbeEnteringDoor_4BB990(pThis);
}

BOOL CCSTD FlyingSlig::IsWallBetween_43A550(FlyingSlig *pThis, BaseAliveGameObject *pObj)
{
    // TODO: Duplicated like IsAbeEnteringDoor_4BB990 ??
    PSX_RECT bRect = {};
    pObj->vGetBoundingRect_424FD0(&bRect, 1);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast_417A60(
        pThis->field_B8_xpos,
        pThis->field_BC_ypos,
        pObj->field_B8_xpos,
        FP_FromInteger((bRect.y + bRect.h) / 2),
        &pLine,
        &hitX,
        &hitY,
        (pThis->field_D6_scale != 0 ? 1 : 16) | (pThis->field_D6_scale != 0 ? 6 : 96) | (pThis->field_D6_scale != 0 ? 8 : 0x80)) != 1;
}

void FlyingSlig::ThrowGrenade_43A1E0()
{
    FP grenadeXVel = (FP_FromInteger(Math_RandomRange_496AB0(50, 64)) / FP_FromInteger(10) * field_CC_sprite_scale);
    const FP grenadeYVel = (FP_FromInteger(-6) * field_CC_sprite_scale);

    FP grenadeXPos = (FP_FromInteger(0) * field_CC_sprite_scale);
    const FP grenadeYPos = (FP_FromInteger(-5) * field_CC_sprite_scale);

    const FP xpos = (FP_FromInteger(0) * field_CC_sprite_scale);
    const FP ypos = (FP_FromInteger(-20) * field_CC_sprite_scale);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        grenadeXPos = -grenadeXPos;
        grenadeXVel = -grenadeXVel;
    }

    auto pGrenade = alive_new<Grenade>();
    if (pGrenade)
    {
        pGrenade->ctor_447F70(grenadeXPos + field_B8_xpos, grenadeYPos + field_BC_ypos, 0, 1, 0, this);
    }

    pGrenade->field_CC_sprite_scale = field_CC_sprite_scale;
    pGrenade->field_D6_scale = field_D6_scale;
    pGrenade->VThrow_49E460(grenadeXVel, grenadeYVel);

    New_Particle_426890(xpos + field_B8_xpos, ypos + field_BC_ypos, field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), field_CC_sprite_scale);
    Slig_SoundEffect_4BFFE0(8, this);
    Event_Broadcast_422BC0(kEventShooting, this);
    Event_Broadcast_422BC0(kEventLoudNoise, this);

    Dove::All_FlyAway_41FA60(0);

    int randomisedGrenadeDelay = field_118_data.field_10_data.field_18_grenade_delay + (Math_NextRandom() & 7);
    if (randomisedGrenadeDelay < 20)
    {
        randomisedGrenadeDelay = 20;
    }
    field_150_grenade_delay = randomisedGrenadeDelay + sGnFrame_5C1B84;

    if (IsPossessed_436A90() == 0 && Math_NextRandom() < 168u)
    {
        Sfx_Slig_GameSpeak_4C04F0(SligSpeak::HereBoy_1, 0, field_15C, this);
    }
}

void FlyingSlig::BlowUp_436510()
{
    MusicController::sub_47FD60(0, this, 0, 0);
    auto pGibs = alive_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(1, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
    }

    auto pBlood = alive_new<Blood>();
    if (pBlood)
    {
        pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale), FP_FromInteger(0), FP_FromInteger(0), field_CC_sprite_scale, 20);
    }

    New_Particles_426C70(field_B8_xpos, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale), field_CC_sprite_scale, 3, 128u, 128u, 128u);
    SFX_Play_46FA90(64u, 128, field_CC_sprite_scale);
    SFX_Play_46FA90(47u, 90, field_CC_sprite_scale);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_18C = 0;
    field_C8_vely = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_190 = 0;
    field_188_ySpeed = FP_FromInteger(0);
    field_184 = 0;
    field_10C_health = FP_FromInteger(0);
    SetBrain(&FlyingSlig::AI_Death_1_4364E0);
    field_14C_hi_pause_timer = sGnFrame_5C1B84 + 40;
}

__int16 FlyingSlig::sub_436730()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToSpottedEnemy_435E70();
        return 1;
    }
    else if (CanHearAbe_4369C0() && !BrainIs(&FlyingSlig::AI_GetAlerted_3_435750))
    {
        ToAlerted_4357E0();
        return 1;
    }
    else if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        ToPanicMoving_435A50();
        return 1;
    }
    else
    {
        if (sActiveHero_5C1B68 &&
            sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0) &&
            FP_Abs(field_B8_xpos - sActiveHero_5C1B68->field_B8_xpos) < FP_FromInteger(640) &&
            FP_Abs(field_BC_ypos - sActiveHero_5C1B68->field_BC_ypos) < FP_FromInteger(240))
        {
            ToAbeDead_436010();
            return 1;
        }
        return 0;
    }

}

__int16 FlyingSlig::CanHearAbe_4369C0()
{
    return
        sActiveHero_5C1B68 == Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, field_D6_scale) ||
        sActiveHero_5C1B68 == Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale);
}

void FlyingSlig::ToSpottedEnemy_435E70()
{
    if (field_118_data.field_10_data.field_10_prechase_delay)
    {
        sub_436A50(SligSpeak ::Freeze_8, 0);
        field_14C_hi_pause_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + field_118_data.field_10_data.field_10_prechase_delay;
        SetBrain(&FlyingSlig::AI_SpottedEnemy_9_435E40);
    }
    else
    {
        Sfx_Slig_GameSpeak_4C04F0(SligSpeak::Freeze_8, 0, field_15C, this);
        ToChase_435E10();
    }
}

void FlyingSlig::ToAbeDead_436010()
{
    SetBrain(&FlyingSlig::AI_AbeDead_11_435FD0);
    field_14C_hi_pause_timer = sGnFrame_5C1B84 + 70;
}

void FlyingSlig::ToAlerted_4357E0()
{
    sub_436A50(SligSpeak ::What_9, 0);
    SetBrain(&FlyingSlig::AI_GetAlerted_3_435750);
    field_14C_hi_pause_timer = sGnFrame_5C1B84 + field_118_data.field_10_data.field_14_listen_time;
}

void FlyingSlig::ToPanicMoving_435A50()
{
    MusicController::sub_47FD60(8, this, 0, 0);
    field_14C_hi_pause_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + 12;
    SetBrain(&FlyingSlig::AI_PanicMoving_7_435990);
}

__int16 FlyingSlig::IsTurning_436AE0()
{
    return 
        field_106_current_motion == eFlyingSligMotions::M_IdleToTurn_2_4388B0 ||
        field_106_current_motion == eFlyingSligMotions::M_QuickTurn_15_4387F0 ||
        field_106_current_motion == eFlyingSligMotions::M_DownMovementToTurn_4_438CC0 ||
        field_106_current_motion == eFlyingSligMotions::M_UpMovementToTurn_6_439030;
}

BYTE** FlyingSlig::ResBlockForMotion_4350F0(int /*motion*/)
{
    if (field_15E == 0)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else
    {
        field_15E = 0;
        return field_10_resources_array.ItemAt(0);
    }
}

void FlyingSlig::ToChantShake_436270()
{
    SFX_Play_46FA90(17, 0);
    VSetMotion_4081C0(eFlyingSligMotions::M_Possession_9_434290);
    SetBrain(&FlyingSlig::AI_DePossession_14_436180);
    field_14C_hi_pause_timer = sGnFrame_5C1B84 + 45;
}

void FlyingSlig::ToPossesed_436130()
{
    Sfx_Slig_GameSpeak_4C04F0(SligSpeak::Help_10, 0, field_15C, this);
    VSetMotion_4081C0(eFlyingSligMotions::M_Possession_9_434290);
    SetBrain(&FlyingSlig::AI_Possession_13_4360F0);
    field_14C_hi_pause_timer = sGnFrame_5C1B84 + 35;
}

const int sFlyingSligFrameTables_552408[41] =
{
    116888,
    116912,
    117084,
    116988,
    117584,
    117012,
    117616,
    117188,
    117132,
    117524,
    117060,
    117316,
    117276,
    117444,
    117376,
    116936,
    117036,
    117336,
    117356,
    117396,
    117464,
    117424,
    117552,
    117492,
    117296,
    117752,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};


void FlyingSlig::vUpdateAnimRes_4350A0()
{
    BYTE** ppRes = ResBlockForMotion_4350F0(field_106_current_motion);
    if (!ppRes)
    {
        field_106_current_motion = eFlyingSligMotions::M_Idle_0_4385E0;
        ppRes = FlyingSlig::ResBlockForMotion_4350F0(field_106_current_motion);
    }
    field_20_animation.Set_Animation_Data_409C80(sFlyingSligFrameTables_552408[field_106_current_motion], ppRes);
}

void FlyingSlig::PatrolDelay_435860()
{
    if (BrainIs(&FlyingSlig::AI_ChasingEnemy_4_435BC0))
    {
        field_14C_hi_pause_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + field_118_data.field_10_data.field_E_give_up_chase_delay;
        SetBrain(&FlyingSlig::AI_Idle_5_435820);
        return;
    }

    field_14C_hi_pause_timer = field_118_data.field_10_data.field_6_patrol_pause_min + sGnFrame_5C1B84;
    if (field_118_data.field_10_data.field_6_patrol_pause_min <= field_118_data.field_10_data.field_8_patrol_pause_max)
    {
        SetBrain(&FlyingSlig::AI_Idle_5_435820);
        return;
    }

    field_14C_hi_pause_timer += Math_NextRandom() % (field_118_data.field_10_data.field_6_patrol_pause_min - field_118_data.field_10_data.field_8_patrol_pause_max);
    SetBrain(&FlyingSlig::AI_Idle_5_435820);
}

__int16 FlyingSlig::CanThrowGrenade_43A490()
{
    return 
        field_106_current_motion == eFlyingSligMotions::M_Idle_0_4385E0 ||
        field_106_current_motion == eFlyingSligMotions::M_HorizontalMovement_1_4386A0 ||
        field_106_current_motion == eFlyingSligMotions::M_DownMovement_3_438AA0 ||
        field_106_current_motion == eFlyingSligMotions::M_UpMovement_5_438DD0 ||
        field_106_current_motion == eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730 ||
        field_106_current_motion == eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0 ||
        field_106_current_motion == eFlyingSligMotions::M_BeginUpMovement_11_438E40 ||
        field_106_current_motion == eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60 ||
        field_106_current_motion == eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10 ||
        field_106_current_motion == eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0 ||
        field_106_current_motion == eFlyingSligMotions::M_EndDownMovement_18_438C90 ||
        field_106_current_motion == eFlyingSligMotions::M_BeginDownMovement_17_438B80;
}

void FlyingSlig::ToLaunchingGrenade_435F50()
{
    if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C) && !IsTurning_436AE0())
    {
        if (!field_18C)
        {
            field_17E_flags.Set(Flags_17E::eBit6);
        }
        field_17E_flags.Toggle(Flags_17E::eBit4);
    }
    SetBrain(&FlyingSlig::AI_LaunchingGrenade_10_435F10);
}

void FlyingSlig::HandlePlayerControls_439340()
{
    NOT_IMPLEMENTED();
}

__int16 FlyingSlig::sub_437C70(PathLine* pLine)
{
    field_100_pCollisionLine = pLine;
    if (!field_100_pCollisionLine)
    {
        return 0;
    }

    field_1A4 = FP_FromInteger(field_100_pCollisionLine->field_0_rect.x);
    field_1A8 = FP_FromInteger(field_100_pCollisionLine->field_0_rect.y);
    field_1AC = FP_FromInteger(field_100_pCollisionLine->field_0_rect.w);
    field_1B0 = FP_FromInteger(field_100_pCollisionLine->field_0_rect.h);

    field_1EC_pLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_100_pCollisionLine->field_C_next);
    field_1F0_pLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_100_pCollisionLine->field_A_previous);

    field_198 = FP_FromInteger(field_100_pCollisionLine->field_12_line_length);

    field_17E_flags.Set(Flags_17E::eBit11, field_1F0_pLine == nullptr);
    field_17E_flags.Set(Flags_17E::eBit12, field_1EC_pLine == nullptr);

    field_182 = FindLeftOrRightBound_43B0A0(field_1AC, field_1B0);
    field_180 = FindLeftOrRightBound_43B0A0(field_1A4, field_1A8);
    field_1BC = Math_496F70(field_1A8 - field_1B0, field_1AC - field_1A4);

    field_1C0 += FP_FromInteger(128);

    if (field_1C0 >= FP_FromInteger(256))
    {
        field_1C0 -= FP_FromInteger(256);
    }
    return 1;
}

__int16 FlyingSlig::FindLeftOrRightBound_43B0A0(FP xOrY, FP wOrH)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    const FP left = xOrY - kGridSize;
    const FP top = wOrH - kGridSize;
    const FP right =  xOrY + kGridSize;
    const FP bottom = wOrH + kGridSize;

    // TODO: Check left is really Abs'd
    short found_type = 0;
    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(FP_Abs(left)), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), TlvTypes::SligBoundLeft_32))
    {
        found_type = TlvTypes::SligBoundLeft_32;
    }
    else if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(left), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), TlvTypes::SligBoundRight_45))
    {
        found_type = TlvTypes::SligBoundRight_45;
    }
    return found_type;
}

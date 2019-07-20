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

    field_150 = 0;
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
    field_188 = 0;
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
        field_194 = 0;
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
    if ((gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710() ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId) &&
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
    NOT_IMPLEMENTED();
}

void FlyingSlig::sub_4396E0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Inactive_0_4355B0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Death_1_4364E0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Moving_2_4356D0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_GetAlerted_3_435750()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_ChasingEnemy_4_435BC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Idle_5_435820()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_GameSpeakToMoving_6_435940()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_PanicMoving_7_435990()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_PanicIdle_8_435AC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_SpottedEnemy_9_435E40()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_LaunchingGrenade_10_435F10()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_AbeDead_11_435FD0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Possessed_12_436040()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::AI_Possession_13_4360F0()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_Idle_0_4385E0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_HorizontalMovement_1_4386A0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_IdleToTurn_2_4388B0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_DownMovement_3_438AA0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_DownMovementToTurn_4_438CC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_UpMovement_5_438DD0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_UpMovementToTurn_6_439030()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_LeverPull_7_439150()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_GameSpeak_8_4391D0()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_HorizontalToDownMovement_12_438B10()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_UpToHorizontalMovement_13_438F60()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_DownToHorizontalMovement_14_438BF0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_QuickTurn_15_4387F0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_IdleToHorizontalMovement_16_438730()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_BeginDownMovement_17_438B80()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_EndDownMovement_18_438C90()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_DownKnockback_19_4390D0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_UpKnockback_20_439110()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_EndUpMovement_21_438EB0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188 <= 0)
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
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_TurnToHorizontalMovement_25_4389E0()
{
    NOT_IMPLEMENTED();
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

__int16 FlyingSlig::IsPossessed_436A90()
{
    NOT_IMPLEMENTED();
    return 0;
}

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

using TFlyingSligFn2 = TFlyingSligFn;

ALIVE_ARY(1, 0x552350, TFlyingSligFn2, 25, sFlyingSlig_fns1_552350,
{
    &FlyingSlig::state_0_0_4355B0,
    &FlyingSlig::state_0_1_Dead_4364E0,
    &FlyingSlig::state_0_2_4356D0,
    &FlyingSlig::state_0_3_435750,
    &FlyingSlig::state_0_4_435BC0,
    &FlyingSlig::state_0_5_435820,
    &FlyingSlig::state_0_6_435940,
    &FlyingSlig::state_0_7_435990,
    &FlyingSlig::state_0_8_435AC0,
    &FlyingSlig::state_0_9_435E40,
    &FlyingSlig::state_0_10_435F10,
    &FlyingSlig::state_0_11_435FD0,
    &FlyingSlig::state_0_12_PlayerControlled_436040,
    &FlyingSlig::state_0_13_4360F0,
    &FlyingSlig::state_0_14_ChantShake_436180,
    &FlyingSlig::state_0_15_4362C0,
    &FlyingSlig::state_0_1_Dead_4364E0,
    &FlyingSlig::state_0_16_4355E0,
});

ALIVE_ARY(1, 0x5523A0, TFlyingSligFn2, 26, sFlyingSlig_fns2_5523A0,
{
    &FlyingSlig::state_1_0_4385E0,
    &FlyingSlig::state_1_1_4386A0,
    &FlyingSlig::state_1_2_4388B0,
    &FlyingSlig::state_1_3_438AA0,
    &FlyingSlig::state_1_4_438CC0,
    &FlyingSlig::state_1_5_438DD0,
    &FlyingSlig::state_1_6_439030,
    &FlyingSlig::state_1_7_439150,
    &FlyingSlig::state_1_8_Speak_4391D0,
    &FlyingSlig::state_1_9_nullsub_33,
    &FlyingSlig::state_1_10_4387D0,
    &FlyingSlig::state_1_11_438E40,
    &FlyingSlig::state_1_12_438B10,
    &FlyingSlig::state_1_13_438F60,
    &FlyingSlig::state_1_14_438BF0,
    &FlyingSlig::state_1_15_4387F0,
    &FlyingSlig::state_1_16_438730,
    &FlyingSlig::state_1_17_438B80,
    &FlyingSlig::state_1_18_438C90,
    &FlyingSlig::state_1_19_4390D0,
    &FlyingSlig::state_1_20_439110,
    &FlyingSlig::state_1_21_438EB0,
    &FlyingSlig::state_1_22_438EE0,
    &FlyingSlig::state_1_23_438F20,
    &FlyingSlig::state_1_24_438D60,
    &FlyingSlig::state_1_25_4389E0,
});

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

    // TODO: Resource Ids
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 450, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 423, TRUE, FALSE));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 6005, TRUE, FALSE));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 372, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 365, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 25, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 319, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 305, TRUE, FALSE));
    field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 576, TRUE, FALSE));

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

    field_15C = 45 * (Math_NextRandom() % 5 - 2);

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

    field_29C_pTable1 = &FlyingSlig::state_0_0_4355B0;

    field_106_current_state = 0;

    if (field_118_data.field_10_data.field_2_state == 1)
    {
        field_14C_hi_pause_timer = sGnFrame_5C1B84 + field_118_data.field_10_data.field_4_hi_pause_time;
    }
    else
    {
        field_14C_hi_pause_timer = sGnFrame_5C1B84 + 1;
    }

    field_2A8 = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2AC = FP_FromInteger(-field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2B0 = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2B4 = FP_FromDouble(0.4) * field_CC_sprite_scale;
    field_2B8 = FP_FromDouble(0.4) * field_CC_sprite_scale;

    if (field_118_data.field_10_data.field_A_direction == 0)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }

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

    field_118_data.field_10_data.field_1E_persistant;

    field_17E_flags.Clear(Flags_17E::eBit13_Persistant);
    if (field_118_data.field_10_data.field_1E_persistant)
    {
        field_17E_flags.Set(Flags_17E::eBit13_Persistant);
    }

    field_17C |= field_118_data.field_10_data.field_1E_persistant;

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

    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
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

        if (!sub_436A90())
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

        // 0x4046bf = patrol + ends on FREEZE
        // 0x4028a6 = idle/still + ends on FREEZE
        // 0x403dbe = blow up on next chant of any duration
        // 0x402abd = dead, just gets destroyed after a timer
        // 0x404480 = under player control
        // 0x40241e = very fast rapid shake ??
        // 0x4013a2 = idle - music change on spot abe
        // 

        // static int oldPtrV = 0x4028a6;
        /*
        int ptrV = 0;
        memcpy(&ptrV, &field_29C_pTable1, sizeof(void*));
        if (oldPtrV != ptrV)
        {
            oldPtrV = ptrV;
            LOG_INFO("Addr = " << std::hex << ptrV);
        }
        */
        //memcpy(&field_29C_pTable1, &oldPtrV, sizeof(void*));

        (this->*(field_29C_pTable1))();
        /*
        0 = idle
        1 = flying
        2 = turn around
        3 = go down
        4 = rapid turn ??
        5 = go up
        6 = rapid turn ??
        7 = speak
        8 = ??
        9 = 
        10 = 
        */
        (this->*(sFlyingSlig_fns2_5523A0)[field_106_current_state])();
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

void FlyingSlig::state_0_0_4355B0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_1_Dead_4364E0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_2_4356D0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_3_435750()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_4_435BC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_5_435820()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_6_435940()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_7_435990()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_8_435AC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_9_435E40()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_10_435F10()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_11_435FD0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_12_PlayerControlled_436040()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_13_4360F0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_14_ChantShake_436180()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_15_4362C0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_0_16_4355E0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_0_4385E0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_1_4386A0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_2_4388B0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_3_438AA0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_4_438CC0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_5_438DD0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_6_439030()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_7_439150()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_8_Speak_4391D0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_9_nullsub_33()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_10_4387D0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_11_438E40()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_12_438B10()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_13_438F60()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_14_438BF0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_15_4387F0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_16_438730()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_17_438B80()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_18_438C90()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_19_4390D0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_20_439110()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_21_438EB0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_22_438EE0()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_23_438F20()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_24_438D60()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::state_1_25_4389E0()
{
    NOT_IMPLEMENTED();
}

__int16 FlyingSlig::sub_436A90()
{
    NOT_IMPLEMENTED();
    return 0;
}

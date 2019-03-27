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

FlyingSlig* FlyingSlig::ctor_4342B0(Path_FlyingSlig* pTlv, int tlvInfo)
{
    NOT_IMPLEMENTED();

    //BaseCtor_4340B0(9);
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
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 450, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 423, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 6005, 1, 0));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 372, 1, 0));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 365, 1, 0));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 25, 1, 0));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 319, 1, 0));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 305, 1, 0));
    field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 576, 1, 0));

    Animation_Init_424E10(
        116888,
        107,
        48u,
        field_10_resources_array.ItemAt(0),
        1,
        1u);

    field_15E = 0;
    
    // TODO
    //field_20_animation.field_1C_fn_ptr_array = (int(__cdecl **)(void *, __int16 *))kFlyingSlig_Anim_Frames_Fns_55EFC4;

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_6_flags.Set(BaseGameObject::eCanExplode);

    field_14C_hi_pause_timer = 0;
    field_DC_bApplyShadows |= 2u;

    field_15C = 45 * (Math_NextRandom() % 5 - 2);

    field_150 = 0;
    field_154 = 0;

    // TODO: Flags obj
    field_17E_flags &= ~0x3F1u;
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
    
    // TODO
    //field_29C_pTable1 = (int)j_FlyingSlig::sub_4355B0;

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

    // TODO: Check order is correct
    if (field_118_data.field_10_data.field_A_direction == 0)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
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

    /* TODO
    v16 = (field_118_data.field_10_data.field_1E_unknown & 1) << 12;
    v17 = field_17E_flags & ~0x1000;
    LOBYTE(field_17C) = field_118_data.field_10_data.field_1C_persistant;
    field_17E_flags = v17 | v16;
    */

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

void FlyingSlig::vUpdate_434AD0()
{
    NOT_IMPLEMENTED();

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

        // TODO
        //field_29C_pTable1();
        //sFlyingSlig_fns2_5523A0[field_106_current_state]();
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

__int16 FlyingSlig::sub_436A90()
{
    NOT_IMPLEMENTED();
    return 0;
}

FlyingSligSpawner* FlyingSligSpawner::ctor_433D50(Path_FlyingSligSpawner* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545090);

    field_4_typeId  = Types::eFlyingSligSpawner_55;

    if (tlvInfo != -1)
    {
        field_C_objectId = tlvInfo;
    }

    field_20_tlvInfo = tlvInfo;

    field_2C_tlv_header = *pTlv;

    field_40_bFirstUpdate &= ~2u;
    field_28_trigger_id = pTlv->field_10.field_16_trigger_id;
    field_3C_bSpawned = 0;
    field_24_spawned_slig_id = -1;

    return this;
}

BaseGameObject* FlyingSligSpawner::VDestructor(signed int flags)
{
    return vdtor_433DE0(flags);
}

void FlyingSligSpawner::VUpdate()
{
    return vUpdate_433E10();
}

int FlyingSligSpawner::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_43B730(reinterpret_cast<FlyingSligSpawner_State*>(pSaveBuffer));
}

signed int CC FlyingSligSpawner::CreateFromSaveState_43B690(const BYTE* pBuffer)
{
    const auto pState = reinterpret_cast<const FlyingSligSpawner_State*>(pBuffer);

    auto pTlv = static_cast<Path_FlyingSligSpawner*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pFlyingSligSpawner = alive_new<FlyingSligSpawner>();
    if (pFlyingSligSpawner)
    {
        pFlyingSligSpawner->ctor_433D50(pTlv, pState->field_4_tlvInfo);
    }

    pFlyingSligSpawner->field_3C_bSpawned = pState->field_8_bSpawned;
    pFlyingSligSpawner->field_40_bFirstUpdate |= 2u;
    pFlyingSligSpawner->field_24_spawned_slig_id = pState->field_C_spawned_slig_obj_id;
    return sizeof(FlyingSligSpawner_State);
}

void FlyingSligSpawner::dtor_434030()
{
    SetVTable(this, 0x545090);
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

FlyingSligSpawner* FlyingSligSpawner::vdtor_433DE0(signed int flags)
{
    dtor_434030();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void FlyingSligSpawner::vUpdate_433E10()
{
    if (field_40_bFirstUpdate & 2)
    {
        // Try to see if we already spawned a Slig on the first update
        field_40_bFirstUpdate &= ~2;
        if (field_24_spawned_slig_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId == Types::eFlyingSlig_54 && pObj->field_C_objectId == field_24_spawned_slig_id)
                {
                    field_24_spawned_slig_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    BaseGameObject* pCurrentSlig = sObjectIds_5C1B70.Find_449CF0(field_24_spawned_slig_id);
    if (!Event_Get_422C00(kEventDeathReset) && !field_6_flags.Get(BaseGameObject::eDead))
    {
        if (field_3C_bSpawned)
        {
            if (!pCurrentSlig || pCurrentSlig->field_6_flags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation_465F00(field_28_trigger_id, SwitchOp::eSetFalse_1);
                field_24_spawned_slig_id = -1;
                field_3C_bSpawned = FALSE;
            }
        }
        else if (SwitchStates_Get_466020(field_28_trigger_id))
        {
            auto pFlyingSligTlv = static_cast<Path_FlyingSlig*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                field_2C_tlv_header.field_8_top_left.field_0_x,
                field_2C_tlv_header.field_8_top_left.field_2_y,
                field_2C_tlv_header.field_8_top_left.field_0_x + 25,
                field_2C_tlv_header.field_8_top_left.field_2_y + 25,
                TlvTypes::FlyingSligSpawner_92));

            if (!pFlyingSligTlv)
            {
                field_6_flags.Set(BaseGameObject::eDead);
                return;
            }

            auto pNewSlig = alive_new<FlyingSlig>();
            if (!pNewSlig)
            {
                field_6_flags.Set(BaseGameObject::eDead);
                return;
            }

            pNewSlig->ctor_4342B0(pFlyingSligTlv, field_20_tlvInfo);
            field_24_spawned_slig_id = pNewSlig->field_8_object_id;
            field_3C_bSpawned = TRUE;
            SFX_Play_46FA90(113, 0);
        }
    }
}

signed int FlyingSligSpawner::vGetSaveState_43B730(FlyingSligSpawner_State* pSaveState)
{
    pSaveState->field_0_type = Types::eFlyingSligSpawner_55;
    pSaveState->field_4_tlvInfo = field_20_tlvInfo;
    pSaveState->field_8_bSpawned = field_3C_bSpawned;
    pSaveState->field_C_spawned_slig_obj_id = -1;
    if (field_24_spawned_slig_id == -1)
    {
        return sizeof(FlyingSligSpawner_State);
    }

    BaseGameObject* pSpawnedSlig = sObjectIds_5C1B70.Find_449CF0(field_24_spawned_slig_id);
    if (pSpawnedSlig)
    {
        pSaveState->field_C_spawned_slig_obj_id = pSpawnedSlig->field_C_objectId;
    }
    return sizeof(FlyingSligSpawner_State);
}

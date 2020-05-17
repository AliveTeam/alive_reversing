#include "stdafx.h"
#include "SligSpawner.hpp"
#include "Function.hpp"
#include "Slig.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"

SligSpawner* SligSpawner::ctor_409740(Path_Slig* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x544090);

    field_4_typeId = Types::eSligSpawner_2;

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_20_tlv_info = tlvInfo;
    field_28_tlv = *pTlv;

    field_26_flags |= 1u;

    field_24_slig_id = pTlv->field_4C_id;
    field_40_bFindSpawnedSlig = 0;
    
    // TODO: Flags
    field_26_flags = (field_26_flags & ~2) | 2 * (pTlv->field_4E_pad & 1);

    field_38_state = 0;
    field_3C_spawned_slig_obj_id = -1;
    return this;
}

void SligSpawner::dtor_409A70()
{
    SetVTable(this, 0x544090);
    if (field_26_flags & 1)
    {
        Path::TLV_Reset_4DB8E0(field_20_tlv_info, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_20_tlv_info, -1, 0, 1);
    }
    BaseGameObject_dtor_4DBEC0();
}

SligSpawner* SligSpawner::vdtor_409800(signed int flags)
{
    dtor_409A70();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void SligSpawner::vScreenChanged_409A30()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level ||
        gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path ||
        field_38_state == 0)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void SligSpawner::vUpdate_409830()
{
    if (field_40_bFindSpawnedSlig)
    {
        field_40_bFindSpawnedSlig = FALSE;

        if (field_3C_spawned_slig_obj_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId == Types::eSlig_125 && pObj->field_C_objectId == field_3C_spawned_slig_obj_id)
                {
                    // Seems redundant ?
                    field_3C_spawned_slig_obj_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    const auto pSpawnedSlig = static_cast<Slig*>(sObjectIds_5C1B70.Find_449CF0(field_3C_spawned_slig_obj_id));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_38_state == 1)
    {
        if (!pSpawnedSlig || pSpawnedSlig->field_6_flags.Get(BaseGameObject::eDead_Bit3) || pSpawnedSlig->field_10C_health <= FP_FromInteger(0))
        {
            SwitchStates_Set_465FF0(field_24_slig_id, 0);
            field_38_state = 0;
        }
    }
    else if (field_38_state == 0)
    {
        if (SwitchStates_Get_466020(field_24_slig_id))
        {
            Path_TLV* pSpawnerTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(field_28_tlv.field_8_top_left.field_0_x, field_28_tlv.field_8_top_left.field_2_y, field_28_tlv.field_8_top_left.field_0_x, field_28_tlv.field_8_top_left.field_2_y, TlvTypes::SligSpawner_37);
            if (pSpawnerTlv)
            {
                auto pSligMem = ae_new<Slig>();
                if (pSligMem)
                {
                    pSligMem->ctor_4B1370(static_cast<Path_Slig*>(pSpawnerTlv), field_20_tlv_info);
                }

                field_3C_spawned_slig_obj_id = pSligMem->field_8_object_id;
                field_38_state = 1;
                SFX_Play_46FA90(114u, 0);
            }

            if (!(field_26_flags & 2))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                field_26_flags = field_26_flags & ~1;
            }
        }
    }
}

int SligSpawner::vGetSaveState_409BB0(Slig_Spawner_State* pState)
{
    pState->field_0_type = Types::eSligSpawner_2;
    pState->field_4_tlvInfo = field_20_tlv_info;
    pState->field_8_state = field_38_state;
    pState->field_C_spawned_slig_obj_id = -1;
    if (field_3C_spawned_slig_obj_id == -1)
    {
        return sizeof(Slig_Spawner_State);
    }

    BaseGameObject* pSpawnedSlig = sObjectIds_5C1B70.Find_449CF0(field_3C_spawned_slig_obj_id);
    if (pSpawnedSlig)
    {
        pState->field_C_spawned_slig_obj_id = pSpawnedSlig->field_C_objectId;
    }
    return sizeof(Slig_Spawner_State);
}

int CC SligSpawner::CreateFromSaveState_409B10(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const Slig_Spawner_State*>(pBuffer);
    auto pTlv = static_cast<Path_Slig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pSpawner = ae_new<SligSpawner>();
    pSpawner->ctor_409740(pTlv, pState->field_4_tlvInfo);
    pSpawner->field_38_state = pState->field_8_state;
    pSpawner->field_3C_spawned_slig_obj_id = pState->field_C_spawned_slig_obj_id;
    pSpawner->field_40_bFindSpawnedSlig = TRUE;
    return sizeof(Slig_Spawner_State);
}

BaseGameObject* SligSpawner::VDestructor(signed int flags)
{
    return vdtor_409800(flags);
}

void SligSpawner::VUpdate()
{
    vUpdate_409830();
}

void SligSpawner::VScreenChanged()
{
    vScreenChanged_409A30();
}

int SligSpawner::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_409BB0(reinterpret_cast<Slig_Spawner_State*>(pSaveBuffer));
}

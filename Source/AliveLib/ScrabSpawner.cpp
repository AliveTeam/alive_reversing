#include "stdafx.h"
#include "ScrabSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"

ScrabSpawner* ScrabSpawner::ctor_4AB450(Path_Scrab_Spawner* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x546FF0);

    field_20_tlvInfo = tlvInfo;
    field_4_typeId = Types::eScrabSpawner_113;

    field_28_tlv_data.field_0_flags = pTlv->field_0_flags;
    field_28_tlv_data.field_1_unknown = pTlv->field_1_unknown;
    field_28_tlv_data.field_2_length = pTlv->field_2_length;
    field_28_tlv_data.field_4_type = pTlv->field_4_type;
    field_28_tlv_data.field_8_top_left = pTlv->field_8_top_left;
    field_28_tlv_data.field_C_bottom_right = pTlv->field_C_bottom_right;

    field_24_switch_id = pTlv->field_2C_switch_id;
    field_26_spawn_scale = pTlv->field_2E_scale;

    field_38_state = 0;

    field_3C_spawned_scrab_id = -1;
    field_40_bFindSpawnedScrab = 0;

    return this;
}

int CC ScrabSpawner::CreateFromSaveState_4ABEB0(const BYTE* pBuffer)
{
    const auto pState = reinterpret_cast<const ScrabSpawner_State*>(pBuffer);
    auto pTlv = static_cast<Path_Scrab_Spawner*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pScrabSpawner = alive_new<ScrabSpawner>();
    pScrabSpawner->ctor_4AB450(pTlv, pState->field_4_tlvInfo);
    pScrabSpawner->field_38_state = pState->field_8_state;
    pScrabSpawner->field_3C_spawned_scrab_id = pState->field_C_spawned_scrab_id;
    pScrabSpawner->field_40_bFindSpawnedScrab = 1;
    return sizeof(ScrabSpawner_State);
}

BaseGameObject* ScrabSpawner::VDestructor(signed int flags)
{
    return vdtor_4AB4E0(flags);
}

void ScrabSpawner::VUpdate()
{
    vUpdate_4AB510();
}

int ScrabSpawner::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_4ABF50(reinterpret_cast<ScrabSpawner_State*>(pSaveBuffer));
}

ScrabSpawner* ScrabSpawner::vdtor_4AB4E0(signed int flags)
{
    dtor_4AB720();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ScrabSpawner::dtor_4AB720()
{
    SetVTable(this, 0x546FF0);
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

int ScrabSpawner::vGetSaveState_4ABF50(ScrabSpawner_State* pSaveState)
{
    pSaveState->field_0_type = Types::eScrabSpawner_113;
    pSaveState->field_4_tlvInfo = field_20_tlvInfo;
    pSaveState->field_8_state = field_38_state;
    pSaveState->field_C_spawned_scrab_id = -1;

    if (field_3C_spawned_scrab_id != -1)
    {
        BaseGameObject* pSpawnedScrab = sObjectIds_5C1B70.Find_449CF0(field_3C_spawned_scrab_id);
        if (pSpawnedScrab)
        {
            pSaveState->field_C_spawned_scrab_id = pSpawnedScrab->field_C_objectId;
        }
    }
    return sizeof(ScrabSpawner_State);
}

void ScrabSpawner::vUpdate_4AB510()
{
    if (field_40_bFindSpawnedScrab)
    {
        field_40_bFindSpawnedScrab = FALSE;
        if (field_3C_spawned_scrab_id != -1)
        {
            for (int i=0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId == Types::eScrab_112 && pObj->field_C_objectId == field_3C_spawned_scrab_id)
                {
                    // Redundant ??
                    field_3C_spawned_scrab_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    auto pExistingSpawnedScrab = static_cast<Scrab*>(sObjectIds_5C1B70.Find_449CF0(field_3C_spawned_scrab_id));
    if (!Event_Get_422C00(kEventDeathReset) && !(field_6_flags.Get(BaseGameObject::eDead)))
    {
        if (field_38_state == 1)
        {
            if (!pExistingSpawnedScrab || pExistingSpawnedScrab->field_6_flags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation_465F00(field_24_switch_id, SwitchOp::eSetFalse_1);
                field_3C_spawned_scrab_id = -1;
                field_38_state = 0;
            }
        }
        else if (field_38_state == 0)
        {
            if (SwitchStates_Get_466020(field_24_switch_id))
            {
                auto pTlv = static_cast<Path_Scrab_Spawner*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    field_28_tlv_data.field_8_top_left.field_0_x,
                    field_28_tlv_data.field_8_top_left.field_2_y,
                    field_28_tlv_data.field_8_top_left.field_0_x,
                    field_28_tlv_data.field_8_top_left.field_2_y,
                    TlvTypes::ScrabSpawner_102));

                if (pTlv)
                {
                    auto pNewScrab = alive_new<Scrab>();
                    pNewScrab->ctor_4A3C40(pTlv, field_20_tlvInfo, field_26_spawn_scale);

                    SFX_Play_46FA90(111u, 0);

                    field_38_state = 1;
                    field_3C_spawned_scrab_id = pNewScrab->field_8_object_id;
                }
                else
                {
                    field_6_flags.Set(BaseGameObject::eDead);
                }
            }
        }
    }
}

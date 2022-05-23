#include "stdafx.h"
#include "ScrabSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"

ScrabSpawner* ScrabSpawner::ctor_4AB450(Path_ScrabSpawner* pTlv, s32 tlvInfo)
{
    BaseGameObject(TRUE, 0);
    SetVTable(this, 0x546FF0);

    field_20_tlvInfo = tlvInfo;
    SetType(AETypes::eScrabSpawner_113);

    field_28_tlv_data.field_0_flags = pTlv->field_0_flags;
    field_28_tlv_data.field_1_tlv_state = pTlv->field_1_tlv_state;
    field_28_tlv_data.field_2_length = pTlv->field_2_length;
    field_28_tlv_data.field_4_type = pTlv->field_4_type;
    field_28_tlv_data.field_8_top_left = pTlv->field_8_top_left;
    field_28_tlv_data.field_C_bottom_right = pTlv->field_C_bottom_right;

    field_24_spawner_switch_id = pTlv->field_2C_spawner_switch_id;
    field_26_spawn_direction = pTlv->field_2E_spawn_direction;

    field_38_state = ScrabSpawnerStates::eInactive_0;

    field_3C_spawned_scrab_id = -1;
    field_40_bFindSpawnedScrab = 0;

    return this;
}

s32 CC ScrabSpawner::CreateFromSaveState_4ABEB0(const u8* pBuffer)
{
    const auto pState = reinterpret_cast<const ScrabSpawner_State*>(pBuffer);
    auto pTlv = static_cast<Path_ScrabSpawner*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pScrabSpawner = ae_new<ScrabSpawner>();
    pScrabSpawner->ctor_4AB450(pTlv, pState->field_4_tlvInfo);
    pScrabSpawner->field_38_state = pState->field_8_state;
    pScrabSpawner->field_3C_spawned_scrab_id = pState->field_C_spawned_scrab_id;
    pScrabSpawner->field_40_bFindSpawnedScrab = 1;
    return sizeof(ScrabSpawner_State);
}

BaseGameObject* ScrabSpawner::VDestructor(s32 flags)
{
    return vdtor_4AB4E0(flags);
}

void ScrabSpawner::VUpdate()
{
    vUpdate_4AB510();
}

s32 ScrabSpawner::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_4ABF50(reinterpret_cast<ScrabSpawner_State*>(pSaveBuffer));
}

ScrabSpawner* ScrabSpawner::vdtor_4AB4E0(s32 flags)
{
    dtor_4AB720();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ScrabSpawner::dtor_4AB720()
{
    SetVTable(this, 0x546FF0);
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

s32 ScrabSpawner::vGetSaveState_4ABF50(ScrabSpawner_State* pSaveState)
{
    pSaveState->field_0_type = AETypes::eScrabSpawner_113;
    pSaveState->field_4_tlvInfo = field_20_tlvInfo;
    pSaveState->field_8_state = field_38_state;
    pSaveState->field_C_spawned_scrab_id = -1;

    if (field_3C_spawned_scrab_id != -1)
    {
        BaseGameObject* pSpawnedScrab = sObjectIds.Find_449CF0(field_3C_spawned_scrab_id);
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
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == AETypes::eScrab_112 && pObj->field_C_objectId == field_3C_spawned_scrab_id)
                {
                    // Redundant ??
                    field_3C_spawned_scrab_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    auto pExistingSpawnedScrab = static_cast<Scrab*>(sObjectIds.Find_449CF0(field_3C_spawned_scrab_id));
    if (!Event_Get_422C00(kEventDeathReset) && !(mFlags.Get(BaseGameObject::eDead)))
    {
        if (field_38_state == ScrabSpawnerStates::eScrabSpawned_1)
        {
            if (!pExistingSpawnedScrab || pExistingSpawnedScrab->mFlags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation_465F00(field_24_spawner_switch_id, SwitchOp::eSetFalse_1);
                field_3C_spawned_scrab_id = -1;
                field_38_state = ScrabSpawnerStates::eInactive_0;
            }
        }
        else if (field_38_state == ScrabSpawnerStates::eInactive_0)
        {
            if (SwitchStates_Get_466020(field_24_spawner_switch_id))
            {
                auto pTlv = static_cast<Path_ScrabSpawner*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    field_28_tlv_data.field_8_top_left.field_0_x,
                    field_28_tlv_data.field_8_top_left.field_2_y,
                    field_28_tlv_data.field_8_top_left.field_0_x,
                    field_28_tlv_data.field_8_top_left.field_2_y,
                    TlvTypes::ScrabSpawner_102));

                if (pTlv)
                {
                    auto pNewScrab = ae_new<Scrab>();
                    pNewScrab->ctor_4A3C40(pTlv, field_20_tlvInfo, field_26_spawn_direction);

                    SFX_Play_46FA90(SoundEffect::ScrabSpawn_111, 0);

                    field_38_state = ScrabSpawnerStates::eScrabSpawned_1;
                    field_3C_spawned_scrab_id = pNewScrab->field_8_object_id;
                }
                else
                {
                    mFlags.Set(BaseGameObject::eDead);
                }
            }
        }
    }
}

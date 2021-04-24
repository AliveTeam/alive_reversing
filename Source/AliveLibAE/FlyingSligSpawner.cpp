#include "stdafx.h"
#include "FlyingSligSpawner.hpp"
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
#include "FlyingSlig.hpp"

FlyingSligSpawner* FlyingSligSpawner::ctor_433D50(Path_FlyingSligSpawner* pTlv, s32 tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545090);

    field_4_typeId = AETypes::eFlyingSligSpawner_55;

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

BaseGameObject* FlyingSligSpawner::VDestructor(s32 flags)
{
    return vdtor_433DE0(flags);
}

void FlyingSligSpawner::VUpdate()
{
    return vUpdate_433E10();
}

s32 FlyingSligSpawner::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_43B730(reinterpret_cast<FlyingSligSpawner_State*>(pSaveBuffer));
}

s32 CC FlyingSligSpawner::CreateFromSaveState_43B690(const BYTE* pBuffer)
{
    const auto pState = reinterpret_cast<const FlyingSligSpawner_State*>(pBuffer);

    auto pTlv = static_cast<Path_FlyingSligSpawner*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4_tlvInfo));
    auto pFlyingSligSpawner = ae_new<FlyingSligSpawner>();
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

FlyingSligSpawner* FlyingSligSpawner::vdtor_433DE0(s32 flags)
{
    dtor_434030();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
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
            for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId == AETypes::eFlyingSlig_54 && pObj->field_C_objectId == field_24_spawned_slig_id)
                {
                    field_24_spawned_slig_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    BaseGameObject* pCurrentSlig = sObjectIds_5C1B70.Find_449CF0(field_24_spawned_slig_id);
    if (!Event_Get_422C00(kEventDeathReset) && !field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        if (field_3C_bSpawned)
        {
            if (!pCurrentSlig || pCurrentSlig->field_6_flags.Get(BaseGameObject::eDead_Bit3))
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
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            auto pNewSlig = ae_new<FlyingSlig>();
            if (!pNewSlig)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            pNewSlig->ctor_4342B0(pFlyingSligTlv, field_20_tlvInfo);
            field_24_spawned_slig_id = pNewSlig->field_8_object_id;
            field_3C_bSpawned = TRUE;
            SFX_Play_46FA90(SoundEffect::FlyingSligSpawn_113, 0);
        }
    }
}

s32 FlyingSligSpawner::vGetSaveState_43B730(FlyingSligSpawner_State* pSaveState)
{
    pSaveState->field_0_type = AETypes::eFlyingSligSpawner_55;
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

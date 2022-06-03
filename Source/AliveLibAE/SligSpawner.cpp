#include "stdafx.h"
#include "SligSpawner.hpp"
#include "Function.hpp"
#include "Slig.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"

SligSpawner::SligSpawner(Path_Slig* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(AETypes::eSligSpawner_2);

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_20_tlv_info = tlvInfo;
    field_28_tlv = *pTlv;

    field_26_flags.Set(SpawnerFlags::eBit1_DontDestroyTLV);

    field_24_slig_spawner_switch_id = pTlv->field_4C_slig_spawner_switch_id;
    field_40_bFindSpawnedSlig = 0;

    field_26_flags.Set(SpawnerFlags::eBit2_UnlimitedSpawns, pTlv->field_4E_unlimited_spawns == Choice_short::eYes_1);

    field_38_state = SpawnerStates::eInactive_0;
    field_3C_spawned_slig_obj_id = -1;
}

SligSpawner::~SligSpawner()
{
    if (field_26_flags.Get(SpawnerFlags::eBit1_DontDestroyTLV))
    {
        Path::TLV_Reset(field_20_tlv_info, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_20_tlv_info, -1, 0, 1);
    }
}

void SligSpawner::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || field_38_state == SpawnerStates::eInactive_0)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void SligSpawner::VUpdate()
{
    if (field_40_bFindSpawnedSlig)
    {
        field_40_bFindSpawnedSlig = FALSE;

        if (field_3C_spawned_slig_obj_id != -1)
        {
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                auto pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == AETypes::eSlig_125 && pObj->field_C_objectId == field_3C_spawned_slig_obj_id)
                {
                    // Seems redundant ?
                    field_3C_spawned_slig_obj_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    const auto pSpawnedSlig = static_cast<Slig*>(sObjectIds.Find_Impl(field_3C_spawned_slig_obj_id));

    if (Event_Get(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (field_38_state == SpawnerStates::eSligSpawned_1)
    {
        if (!pSpawnedSlig || pSpawnedSlig->mFlags.Get(BaseGameObject::eDead) || pSpawnedSlig->field_10C_health <= FP_FromInteger(0))
        {
            SwitchStates_Set(field_24_slig_spawner_switch_id, 0);
            field_38_state = SpawnerStates::eInactive_0;
        }
    }
    else if (field_38_state == SpawnerStates::eInactive_0)
    {
        if (SwitchStates_Get(field_24_slig_spawner_switch_id))
        {
            Path_TLV* pSpawnerTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(field_28_tlv.field_8_top_left.field_0_x, field_28_tlv.field_8_top_left.field_2_y, field_28_tlv.field_8_top_left.field_0_x, field_28_tlv.field_8_top_left.field_2_y, TlvTypes::SligSpawner_37);
            if (pSpawnerTlv)
            {
                auto pSligMem = ae_new<Slig>(static_cast<Path_Slig*>(pSpawnerTlv), field_20_tlv_info);
                if (pSligMem)
                {
                    field_3C_spawned_slig_obj_id = pSligMem->field_8_object_id;
                }

                field_38_state = SpawnerStates::eSligSpawned_1;
                SFX_Play_Mono(SoundEffect::SligSpawn_114, 0);
            }

            if (!field_26_flags.Get(SpawnerFlags::eBit2_UnlimitedSpawns))
            {
                mFlags.Set(BaseGameObject::eDead);
                field_26_flags.Clear(SpawnerFlags::eBit1_DontDestroyTLV);
            }
        }
    }
}

s32 SligSpawner::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Slig_Spawner_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eSligSpawner_2;
    pState->field_4_tlvInfo = field_20_tlv_info;
    pState->field_8_state = field_38_state;
    pState->field_C_spawned_slig_obj_id = -1;
    if (field_3C_spawned_slig_obj_id == -1)
    {
        return sizeof(Slig_Spawner_State);
    }

    BaseGameObject* pSpawnedSlig = sObjectIds.Find_Impl(field_3C_spawned_slig_obj_id);
    if (pSpawnedSlig)
    {
        pState->field_C_spawned_slig_obj_id = pSpawnedSlig->field_C_objectId;
    }
    return sizeof(Slig_Spawner_State);
}

s32 SligSpawner::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Slig_Spawner_State*>(pBuffer);
    auto pTlv = static_cast<Path_Slig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));
    auto pSpawner = ae_new<SligSpawner>(pTlv, pState->field_4_tlvInfo);
    if (pSpawner)
    {
        pSpawner->field_38_state = pState->field_8_state;
        pSpawner->field_3C_spawned_slig_obj_id = pState->field_C_spawned_slig_obj_id;
        pSpawner->field_40_bFindSpawnedSlig = TRUE;
    }

    return sizeof(Slig_Spawner_State);
}

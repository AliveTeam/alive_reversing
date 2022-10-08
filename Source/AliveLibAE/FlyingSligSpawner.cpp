#include "stdafx.h"
#include "FlyingSligSpawner.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "FlyingSlig.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

FlyingSligSpawner::FlyingSligSpawner(relive::Path_FlyingSligSpawner* pTlv, const Guid& tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eFlyingSligSpawner);

    if (tlvInfo != Guid{})
    {
        mBaseGameObjectTlvInfo = tlvInfo;
    }

    field_20_tlvInfo = tlvInfo;

    field_2C_tlv_header = pTlv;

    field_40_bFirstUpdate &= ~2u;
    field_28_spawner_switch_id = pTlv->mSpawnerSwitchId;
    field_3C_bSpawned = 0;
    field_24_spawned_slig_id = Guid{};
}

s32 FlyingSligSpawner::CreateFromSaveState(const u8* pBuffer)
{
    const auto pState = reinterpret_cast<const FlyingSligSpawner_State*>(pBuffer);

    auto pTlv = static_cast<relive::Path_FlyingSligSpawner*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));

    auto pFlyingSligSpawner = relive_new FlyingSligSpawner(pTlv, pState->field_4_tlvInfo);
    if (pFlyingSligSpawner)
    {
        pFlyingSligSpawner->field_3C_bSpawned = pState->field_8_bSpawned;
        pFlyingSligSpawner->field_40_bFirstUpdate |= 2u;
        pFlyingSligSpawner->field_24_spawned_slig_id = pState->field_C_spawned_slig_obj_id;
    }

    return sizeof(FlyingSligSpawner_State);
}

FlyingSligSpawner::~FlyingSligSpawner()
{
    Path::TLV_Reset(field_20_tlvInfo, -1, 0, 0);
}

void FlyingSligSpawner::VUpdate()
{
    if (field_40_bFirstUpdate & 2)
    {
        // Try to see if we already spawned a Slig on the first update
        field_40_bFirstUpdate &= ~2;
        if (field_24_spawned_slig_id != Guid{})
        {
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == ReliveTypes::eFlyingSlig && pObj->mBaseGameObjectTlvInfo == field_24_spawned_slig_id)
                {
                    field_24_spawned_slig_id = pObj->mBaseGameObjectId;
                    break;
                }
            }
        }
    }

    BaseGameObject* pCurrentSlig = sObjectIds.Find_Impl(field_24_spawned_slig_id);
    if (!EventGet(kEventDeathReset) && !mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (field_3C_bSpawned)
        {
            if (!pCurrentSlig || pCurrentSlig->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation(field_28_spawner_switch_id, relive::reliveSwitchOp::eSetFalse);
                field_24_spawned_slig_id = Guid{};
                field_3C_bSpawned = FALSE;
            }
        }
        else if (SwitchStates_Get(field_28_spawner_switch_id))
        {
            auto pFlyingSligTlv = static_cast<relive::Path_FlyingSlig*>(sPathInfo->TLV_Get_At_4DB4B0(
                field_2C_tlv_header->mTopLeftX,
                field_2C_tlv_header->mTopLeftY,
                field_2C_tlv_header->mTopLeftX + 25,
                field_2C_tlv_header->mTopLeftY + 25,
                ReliveTypes::eFlyingSligSpawner));

            if (!pFlyingSligTlv)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            auto pNewSlig = relive_new FlyingSlig(pFlyingSligTlv, field_20_tlvInfo);
            if (!pNewSlig)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            field_24_spawned_slig_id = pNewSlig->mBaseGameObjectId;
            field_3C_bSpawned = TRUE;
            SfxPlayMono(relive::SoundEffects::FlyingSligSpawn, 0);
        }
    }
}

s32 FlyingSligSpawner::VGetSaveState(u8* pSaveBuffer)
{
    auto pSaveState = reinterpret_cast<FlyingSligSpawner_State*>(pSaveBuffer);

    pSaveState->field_0_type = AETypes::eFlyingSligSpawner_55;
    pSaveState->field_4_tlvInfo = field_20_tlvInfo;
    pSaveState->field_8_bSpawned = field_3C_bSpawned;
    pSaveState->field_C_spawned_slig_obj_id = Guid{};
    if (field_24_spawned_slig_id == Guid{})
    {
        return sizeof(FlyingSligSpawner_State);
    }

    BaseGameObject* pSpawnedSlig = sObjectIds.Find_Impl(field_24_spawned_slig_id);
    if (pSpawnedSlig)
    {
        pSaveState->field_C_spawned_slig_obj_id = pSpawnedSlig->mBaseGameObjectTlvInfo;
    }
    return sizeof(FlyingSligSpawner_State);
}

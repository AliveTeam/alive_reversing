#include "stdafx.h"
#include "FlyingSligSpawner.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "FlyingSlig.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"

FlyingSligSpawner::FlyingSligSpawner(relive::Path_FlyingSligSpawner* pTlv, const Guid& tlvInfo)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eFlyingSligSpawner);

    if (tlvInfo != Guid{})
    {
        mBaseGameObjectTlvInfo = tlvInfo;
    }

    mTlvId = tlvInfo;

    field_2C_tlv_header = pTlv;

    field_40_bFirstUpdate &= ~2u;
    field_28_spawner_switch_id = pTlv->mSpawnerSwitchId;
    field_3C_bSpawned = 0;
    field_24_spawned_slig_id = Guid{};
}

void FlyingSligSpawner::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<FlyingSligSpawnerSaveState>();

    auto pTlv = static_cast<relive::Path_FlyingSligSpawner*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));

    auto pFlyingSligSpawner = relive_new FlyingSligSpawner(pTlv, pState->field_4_tlvInfo);
    if (pFlyingSligSpawner)
    {
        pFlyingSligSpawner->field_3C_bSpawned = pState->field_8_bSpawned;
        pFlyingSligSpawner->field_40_bFirstUpdate |= 2u;
        pFlyingSligSpawner->field_24_spawned_slig_id = pState->field_C_spawned_slig_obj_id;
    }
}

FlyingSligSpawner::~FlyingSligSpawner()
{
    Path::TLV_Reset(mTlvId);
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
    if (!EventGet(Event::kEventDeathReset) && !GetDead())
    {
        if (field_3C_bSpawned)
        {
            if (!pCurrentSlig || pCurrentSlig->GetDead())
            {
                SwitchStates_Do_Operation(field_28_spawner_switch_id, relive::reliveSwitchOp::eSetFalse);
                field_24_spawned_slig_id = Guid{};
                field_3C_bSpawned = false;
            }
        }
        else if (SwitchStates_Get(field_28_spawner_switch_id))
        {
            auto pFlyingSligTlv = static_cast<relive::Path_FlyingSlig*>(gPathInfo->VTLV_Get_At_Of_Type(
                field_2C_tlv_header->mTopLeftX,
                field_2C_tlv_header->mTopLeftY,
                field_2C_tlv_header->mTopLeftX + 25,
                field_2C_tlv_header->mTopLeftY + 25,
                ReliveTypes::eFlyingSligSpawner));

            if (!pFlyingSligTlv)
            {
                SetDead(true);
                return;
            }

            auto pNewSlig = relive_new FlyingSlig(pFlyingSligTlv, mTlvId);
            if (!pNewSlig)
            {
                SetDead(true);
                return;
            }

            field_24_spawned_slig_id = pNewSlig->mBaseGameObjectId;
            field_3C_bSpawned = true;
            SfxPlayMono(relive::SoundEffects::FlyingSligSpawn, 0);
        }
    }
}

void FlyingSligSpawner::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    FlyingSligSpawnerSaveState data = {};

    data.mType = ReliveTypes::eFlyingSligSpawner;
    data.field_4_tlvInfo = mTlvId;
    data.field_8_bSpawned = field_3C_bSpawned;
    data.field_C_spawned_slig_obj_id = Guid{};
    if (field_24_spawned_slig_id == Guid{})
    {
        pSaveBuffer.Write(data);
        return;
    }

    BaseGameObject* pSpawnedSlig = sObjectIds.Find_Impl(field_24_spawned_slig_id);
    if (pSpawnedSlig)
    {
        data.field_C_spawned_slig_obj_id = pSpawnedSlig->mBaseGameObjectTlvInfo;
    }
    pSaveBuffer.Write(data);
}

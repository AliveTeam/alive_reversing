#include "stdafx.h"
#include "FlyingSligSpawner.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "FlyingSlig.hpp"

FlyingSligSpawner::FlyingSligSpawner(Path_FlyingSligSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eFlyingSligSpawner);

    if (tlvInfo != -1)
    {
        mBaseGameObjectTlvInfo = tlvInfo;
    }

    field_20_tlvInfo = tlvInfo;

    field_2C_tlv_header = *pTlv;

    field_40_bFirstUpdate &= ~2u;
    field_28_spawner_switch_id = pTlv->mFlyingSligSpawnerData.mSpawnerSwitchId;
    field_3C_bSpawned = 0;
    field_24_spawned_slig_id = -1;
}

s32 FlyingSligSpawner::CreateFromSaveState(const u8* pBuffer)
{
    const auto pState = reinterpret_cast<const FlyingSligSpawner_State*>(pBuffer);

    auto pTlv = static_cast<Path_FlyingSligSpawner*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));

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
        if (field_24_spawned_slig_id != -1)
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
                SwitchStates_Do_Operation(field_28_spawner_switch_id, SwitchOp::eSetFalse_1);
                field_24_spawned_slig_id = -1;
                field_3C_bSpawned = FALSE;
            }
        }
        else if (SwitchStates_Get(field_28_spawner_switch_id))
        {
            auto pFlyingSligTlv = static_cast<Path_FlyingSlig*>(sPathInfo->TLV_Get_At_4DB4B0(
                field_2C_tlv_header.mTopLeft.x,
                field_2C_tlv_header.mTopLeft.y,
                field_2C_tlv_header.mTopLeft.x + 25,
                field_2C_tlv_header.mTopLeft.y + 25,
                TlvTypes::FlyingSligSpawner_92));

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
            SfxPlayMono(SoundEffect::FlyingSligSpawn_113, 0);
        }
    }
}

s32 FlyingSligSpawner::VGetSaveState(u8* pSaveBuffer)
{
    auto pSaveState = reinterpret_cast<FlyingSligSpawner_State*>(pSaveBuffer);

    pSaveState->field_0_type = AETypes::eFlyingSligSpawner_55;
    pSaveState->field_4_tlvInfo = field_20_tlvInfo;
    pSaveState->field_8_bSpawned = field_3C_bSpawned;
    pSaveState->field_C_spawned_slig_obj_id = -1;
    if (field_24_spawned_slig_id == -1)
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

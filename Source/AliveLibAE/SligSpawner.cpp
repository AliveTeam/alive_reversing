#include "stdafx.h"
#include "SligSpawner.hpp"
#include "Function.hpp"
#include "Slig.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Map.hpp"

SligSpawner::SligSpawner(relive::Path_Slig* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eSligSpawner);

    if (tlvId.IsValid())
    {
        mBaseGameObjectTlvInfo = tlvId;
    }

    mTlvInfo = tlvId;
    mPathTlv = *pTlv;

    mSpawnerFlags.Set(SpawnerFlags::eBit1_DontDestroyTLV);

    mSligSpawnerSwitchId = pTlv->mData.mSligSpawnerSwitchId;
    mFindSpawnedSlig = 0;

    mSpawnerFlags.Set(SpawnerFlags::eBit2_UnlimitedSpawns, pTlv->mData.mUnlimitedSpawns == relive::reliveChoice::eYes);

    mState = SpawnerStates::eInactive_0;
    mSpawnedSligId = Guid{};
}

SligSpawner::~SligSpawner()
{
    if (mSpawnerFlags.Get(SpawnerFlags::eBit1_DontDestroyTLV))
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }
}

void SligSpawner::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || mState == SpawnerStates::eInactive_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void SligSpawner::VUpdate()
{
    if (mFindSpawnedSlig)
    {
        mFindSpawnedSlig = false;

        if (mSpawnedSligId != Guid{})
        {
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                auto pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == ReliveTypes::eSlig && pObj->mBaseGameObjectTlvInfo == mSpawnedSligId)
                {
                    // Seems redundant ?
                    mSpawnedSligId = pObj->mBaseGameObjectId;
                    break;
                }
            }
        }
    }

    const auto pSpawnedSlig = static_cast<Slig*>(sObjectIds.Find_Impl(mSpawnedSligId));

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mState == SpawnerStates::eSligSpawned_1)
    {
        if (!pSpawnedSlig || pSpawnedSlig->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || pSpawnedSlig->mHealth <= FP_FromInteger(0))
        {
            SwitchStates_Set(mSligSpawnerSwitchId, 0);
            mState = SpawnerStates::eInactive_0;
        }
    }
    else if (mState == SpawnerStates::eInactive_0)
    {
        if (SwitchStates_Get(mSligSpawnerSwitchId))
        {
            relive::Path_TLV* pSpawnerTlv = sPathInfo->TLV_Get_At(mPathTlv.mTopLeftX, mPathTlv.mTopLeftY, mPathTlv.mTopLeftX, mPathTlv.mTopLeftY, ReliveTypes::eSligSpawner);
            if (pSpawnerTlv)
            {
                auto pSligMem = relive_new Slig(static_cast<relive::Path_Slig*>(pSpawnerTlv), mTlvInfo);
                if (pSligMem)
                {
                    mSpawnedSligId = pSligMem->mBaseGameObjectId;
                }

                mState = SpawnerStates::eSligSpawned_1;
                SfxPlayMono(relive::SoundEffects::SligSpawn, 0);
            }

            if (!mSpawnerFlags.Get(SpawnerFlags::eBit2_UnlimitedSpawns))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                mSpawnerFlags.Clear(SpawnerFlags::eBit1_DontDestroyTLV);
            }
        }
    }
}

s32 SligSpawner::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Slig_Spawner_State*>(pSaveBuffer);

    pState->mType = AETypes::eSligSpawner_2;
    pState->mTlvInfo = mTlvInfo;
    pState->mState = mState;
    pState->mSpawnedSligId = Guid{};
    if (mSpawnedSligId == Guid{})
    {
        return sizeof(Slig_Spawner_State);
    }

    BaseGameObject* pSpawnedSlig = sObjectIds.Find_Impl(mSpawnedSligId);
    if (pSpawnedSlig)
    {
        pState->mSpawnedSligId = pSpawnedSlig->mBaseGameObjectTlvInfo;
    }
    return sizeof(Slig_Spawner_State);
}

s32 SligSpawner::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Slig_Spawner_State*>(pBuffer);
    auto pTlv = static_cast<relive::Path_Slig*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));
    auto pSpawner = relive_new SligSpawner(pTlv, pState->mTlvInfo);
    if (pSpawner)
    {
        pSpawner->mState = pState->mState;
        pSpawner->mSpawnedSligId = pState->mSpawnedSligId;
        pSpawner->mFindSpawnedSlig = true;
    }

    return sizeof(Slig_Spawner_State);
}

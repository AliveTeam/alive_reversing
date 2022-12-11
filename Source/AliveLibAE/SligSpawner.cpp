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
#include "Path.hpp"

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

    mDontDestroyTLV = true;

    mSligSpawnerSwitchId = pTlv->mData.mSligSpawnerSwitchId;
    mFindSpawnedSlig = 0;

    mUnlimitedSpawns = pTlv->mData.mUnlimitedSpawns == relive::reliveChoice::eYes ? true : false;

    mState = SpawnerStates::eInactive_0;
    mSpawnedSligId = Guid{};
}

SligSpawner::~SligSpawner()
{
    if (mDontDestroyTLV)
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
        SetDead(true);
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
        SetDead(true);
    }

    if (mState == SpawnerStates::eSligSpawned_1)
    {
        if (!pSpawnedSlig || pSpawnedSlig->GetDead() || pSpawnedSlig->mHealth <= FP_FromInteger(0))
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

            if (!mUnlimitedSpawns)
            {
                SetDead(true);
                mDontDestroyTLV = false;
            }
        }
    }
}

s32 SligSpawner::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<SligSpawnerSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eSligSpawner;
    pState->mTlvId = mTlvInfo;
    pState->mState = mState;
    pState->mSpawnedSligId = Guid{};
    if (mSpawnedSligId == Guid{})
    {
        return sizeof(SligSpawnerSaveState);
    }

    BaseGameObject* pSpawnedSlig = sObjectIds.Find_Impl(mSpawnedSligId);
    if (pSpawnedSlig)
    {
        pState->mSpawnedSligId = pSpawnedSlig->mBaseGameObjectTlvInfo;
    }
    return sizeof(SligSpawnerSaveState);
}

s32 SligSpawner::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const SligSpawnerSaveState*>(pBuffer);
    auto pTlv = static_cast<relive::Path_Slig*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId));
    auto pSpawner = relive_new SligSpawner(pTlv, pState->mTlvId);
    if (pSpawner)
    {
        pSpawner->mState = pState->mState;
        pSpawner->mSpawnedSligId = pState->mSpawnedSligId;
        pSpawner->mFindSpawnedSlig = true;
    }

    return sizeof(SligSpawnerSaveState);
}

#include "stdafx.h"
#include "SligSpawner.hpp"
#include "Function.hpp"
#include "Slig.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "SwitchStates.hpp"

SligSpawner::SligSpawner(Path_Slig* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eSligSpawner);

    if (tlvInfo != 0xFFFF)
    {
        mBaseGameObjectTlvInfo = tlvInfo;
    }

    mTlvInfo = tlvInfo;
    mPathTlv = *pTlv;

    mSpawnerFlags.Set(SpawnerFlags::eBit1_DontDestroyTLV);

    mSligSpawnerSwitchId = pTlv->mSligSpawnerSwitchId;
    mFindSpawnedSlig = 0;

    mSpawnerFlags.Set(SpawnerFlags::eBit2_UnlimitedSpawns, pTlv->mUnlimitedSpawns == Choice_short::eYes_1);

    mState = SpawnerStates::eInactive_0;
    mSpawnedSligId = -1;
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
        mFindSpawnedSlig = FALSE;

        if (mSpawnedSligId != -1)
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
            Path_TLV* pSpawnerTlv = sPathInfo->TLV_Get_At_4DB4B0(mPathTlv.mTopLeft.x, mPathTlv.mTopLeft.y, mPathTlv.mTopLeft.x, mPathTlv.mTopLeft.y, TlvTypes::SligSpawner_37);
            if (pSpawnerTlv)
            {
                auto pSligMem = relive_new Slig(static_cast<Path_Slig*>(pSpawnerTlv), mTlvInfo);
                if (pSligMem)
                {
                    mSpawnedSligId = pSligMem->mBaseGameObjectId;
                }

                mState = SpawnerStates::eSligSpawned_1;
                SfxPlayMono(SoundEffect::SligSpawn_114, 0);
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
    pState->mSpawnedSligId = -1;
    if (mSpawnedSligId == -1)
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
    auto pTlv = static_cast<Path_Slig*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));
    auto pSpawner = relive_new SligSpawner(pTlv, pState->mTlvInfo);
    if (pSpawner)
    {
        pSpawner->mState = pState->mState;
        pSpawner->mSpawnedSligId = pState->mSpawnedSligId;
        pSpawner->mFindSpawnedSlig = TRUE;
    }

    return sizeof(Slig_Spawner_State);
}

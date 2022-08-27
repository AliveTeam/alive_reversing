#include "stdafx.h"
#include "SlurgSpawner.hpp"
#include "DDCheat.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"

SlurgSpawner::SlurgSpawner(relive::Path_SlurgSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eSlurgSpawner);
    mTlvInfo = tlvInfo;
    mSlurgTlv = pTlv;
    mSlurgPathData = pTlv->mSpawnerData;
    mSpawnCounter = 0;
    mSpawnedCount = pTlv->mTlvSpecificMeaning;
    if ((mSpawnedCount & 0x80u) != 0)
    {
        mSpawnedCount = 0;
    }
}

void SlurgSpawner::VUpdate()
{
    if (sDDCheat_ShowAI_Info)
    {
        DDCheat::DebugStr("SPAWNER EXISTS/n");
    }

    mSpawnCounter++;

    if (mSpawnCounter >= mSlurgPathData.mSpawnInterval &&
        mSpawnedCount < mSlurgPathData.mMaxSlurgs)
    {
        if (SwitchStates_Get(mSlurgPathData.mSwitchId))
        {
            mSpawnCounter = 0;
            mSpawnedCount++;
            relive_new Slurg(mSlurgTlv, 0xFFFFFFFF);
        }
    }
}

void SlurgSpawner::VScreenChanged()
{
    Path::TLV_Reset(mTlvInfo, mSpawnedCount, 0, 0);
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

#include "stdafx.h"
#include "SlurgSpawner.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"

static void SetData(const relive::Path_SlurgSpawner& tlv, Slurg_Spawner_Path_Data& data)
{
    data.mSpawnInterval = tlv.mSpawnInterval;
    data.mMaxSlurgs = tlv.mMaxSlurgs;
    data.mSwitchId = tlv.mSpawnerSwitchId;
    data.padding = 0;
}

SlurgSpawner::SlurgSpawner(relive::Path_SlurgSpawner* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eSlurgSpawner);
    mTlvInfo = tlvId;
    mSlurgTlv = pTlv;
    SetData(*pTlv, mSlurgPathData);
    mSpawnCounter = 0;
    mSpawnedCount = pTlv->mTlvSpecificMeaning;
    if ((mSpawnedCount & 0x80u) != 0)
    {
        mSpawnedCount = 0;
    }
}

void SlurgSpawner::VUpdate()
{
    if (gDDCheat_ShowAI_Info)
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
            relive_new Slurg(mSlurgTlv, Guid{});
        }
    }
}

void SlurgSpawner::VScreenChanged()
{
    Path::TLV_Reset(mTlvInfo, mSpawnedCount);
    SetDead(true);
}

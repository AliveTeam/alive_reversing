#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeNest.hpp"
#include "ResourceManager.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"

namespace AO {

BeeNest::BeeNest(Path_BeeNest* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eBeeNest;

    mTlvInfo = tlvInfo;

    mBeesAmount = pTlv->mBeesAmount;

    mSwitchId = pTlv->mSwitchId;

    mBeeSwarmX = FP_FromInteger(pTlv->mTopLeft.x);
    mBeeSwarmY = FP_FromInteger(pTlv->mTopLeft.y);

    mTotalChaseTime = pTlv->mTotalChaseTime;

    mSpeed = FP_FromRaw(pTlv->mSpeed << 8);

    mBeeSwarm = nullptr;

    mState = BeeNestStates::eWaitForTrigger_0;

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
    }

    // The "idle" swarm that hovers around the nest
    relive_new BeeSwarm(mBeeSwarmX, mBeeSwarmY, FP_FromInteger(0), pTlv->field_22_num_bees, 0);
}

BeeNest::~BeeNest()
{
    if (mBeeSwarm)
    {
        mBeeSwarm->mBaseGameObjectRefCount--;
    }
}

void BeeNest::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || !mBeeSwarm)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
        if (mBeeSwarm)
        {
            mBeeSwarm->mBaseGameObjectRefCount--;
            mBeeSwarm = nullptr;
        }
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BeeNest::VUpdate()
{
    switch (mState)
    {
        case BeeNestStates::eWaitForTrigger_0:
            if (SwitchStates_Get(mSwitchId))
            {
                mBeeSwarm = relive_new BeeSwarm(
                    mBeeSwarmX,
                    mBeeSwarmY,
                    mSpeed,
                    mBeesAmount,
                    mTotalChaseTime);
                if (mBeeSwarm)
                {
                    mBeeSwarm->mBaseGameObjectRefCount++;
                    mBeeSwarm->Chase(sActiveHero);
                    mState = BeeNestStates::eResetIfDead_1;
                }
            }
            break;

        case BeeNestStates::eResetIfDead_1:
            if (mBeeSwarm->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                mState = BeeNestStates::eWaitForTrigger_0;
                mBeeSwarm->mBaseGameObjectRefCount--;
                mBeeSwarm = nullptr;
                SwitchStates_Set(mSwitchId, 0);
            }
            break;

        default:
            break;
    }
}

} // namespace AO

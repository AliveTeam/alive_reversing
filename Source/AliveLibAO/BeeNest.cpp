#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeNest.hpp"
#include "ResourceManager.hpp"
#include "BeeSwarm.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"

namespace AO {

BeeNest::BeeNest(relive::Path_BeeNest* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eBeeNest);

    mTlvInfo = tlvId;

    mSwarmSize = pTlv->mSwarmSize;

    mSwitchId = pTlv->mSwitchId;

    mBeeSwarmX = FP_FromInteger(pTlv->mTopLeftX);
    mBeeSwarmY = FP_FromInteger(pTlv->mTopLeftY);

    mTotalChaseTime = pTlv->mChaseTime;
    mSpeed = FP_FromRaw(pTlv->mSpeed << 8);

    mBeeSwarm = nullptr;

    mState = BeeNestStates::eWaitForTrigger_0;

    // The "idle" swarm that hovers around the nest
    relive_new BeeSwarm(mBeeSwarmX, mBeeSwarmY, FP_FromInteger(0), pTlv->mBeesAmount, 0);
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
                    mSwarmSize,
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

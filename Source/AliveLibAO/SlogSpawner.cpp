#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SlogSpawner.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Slog.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "Path.hpp"

namespace AO {

void SlogSpawner::VScreenChanged()
{
    Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount, 0, 0);
    SetDead(true);
}

SlogSpawner::SlogSpawner(relive::Path_SlogSpawner* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvInfo = tlvId;
    mSpawnTimer = 0;

    mScale = pTlv->mScale;
    mMaxSlogs = pTlv->mMaxSlogs;
    mMaxSlogsAtATime = pTlv->mMaxSlogsAtATime;
    mStartDirection = pTlv->mStartDirection;
    mSlogSpawnInterval = pTlv->mSlogSpawnInterval;
    mSpawnerSwitchId = pTlv->mSpawnerSwitchId;

    mSpawnedSlogsCount = pTlv->mTlvSpecificMeaning;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
}

void SlogSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (static_cast<s32>(sGnFrame) > mSpawnTimer && sSlogCount < mMaxSlogsAtATime)
    {
        if (SwitchStates_Get(mSpawnerSwitchId))
        {
            mSpawnTimer = Math_NextRandom() % 8
                                 + sGnFrame
                                 + mSlogSpawnInterval;

            auto pSlog = relive_new Slog(
                    mXPos,
                    mYPos,
                    mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1));;
            if (pSlog)
            {
                pSlog->GetAnimation().SetFlipX(mStartDirection == relive::Path_SlogSpawner::StartDirection::eLeft);
            }

            mSpawnedSlogsCount++;

            if (mSpawnedSlogsCount >= mMaxSlogs)
            {
                Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount, 0, 1);
                SetDead(true);
            }
        }
    }
}

} // namespace AO

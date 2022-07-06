#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlogSpawner.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Slog.hpp"
#include "Events.hpp"
#include "Math.hpp"

namespace AO {

void SlogSpawner::VScreenChanged()
{
    Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount, 0, 0);
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

SlogSpawner::SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mTlvInfo = tlvInfo;
    mSpawnTimer = 0;

    mScale = pTlv->field_18_scale;
    mMaxSlogs = pTlv->mMaxSlogs;
    mMaxSlogsAtATime = pTlv->mMaxSlogsAtATime;
    mStartDirection = pTlv->mStartDirection;
    mSlogSpawnInterval = pTlv->mSlogSpawnInterval;
    mSpawnerSwitchId = pTlv->mSpawnerSwitchId;

    mSpawnedSlogsCount = pTlv->field_1_unknown;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);
}

void SlogSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (static_cast<s32>(sGnFrame) > mSpawnTimer && gNumSlogs_9F11C8 < mMaxSlogsAtATime)
    {
        if (SwitchStates_Get(mSpawnerSwitchId))
        {
            mSpawnTimer = Math_NextRandom() % 8
                                 + sGnFrame
                                 + mSlogSpawnInterval;

            auto pSlog = relive_new Slog(
                    mXPos,
                    mYPos,
                    mScale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));;
            if (pSlog)
            {
                pSlog->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, mStartDirection == StartDirection::eLeft_1);
            }

            mSpawnedSlogsCount++;

            if (mSpawnedSlogsCount >= mMaxSlogs)
            {
                Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount, 0, 1);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
    }
}

} // namespace AO

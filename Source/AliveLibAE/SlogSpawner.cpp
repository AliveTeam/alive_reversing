#include "stdafx.h"
#include "SlogSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"

SlogSpawner::SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mTlvInfo = tlvInfo;

    mScale = pTlv->mScale;
    mMaxSlogs = pTlv->mMaxSlogs;
    mMaxSlogsAtATime = pTlv->mMaxSlogsAtATime;
    mStartDirection = pTlv->mStartDirection;
    mSlogSpawnInterval = pTlv->mSlogSpawnInterval;
    mSpawnerSwitchId = pTlv->mSpawnerSwitchId;
    mListenToSligs = pTlv->mListenToSligs;
    mChaseDelay = pTlv->mChaseDelay;

    mSpawnedSlogsCount = pTlv->mTlvState;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mSpawnTimer = 0;
}

void SlogSpawner::VScreenChanged()
{
    Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount, 0, 0);
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void SlogSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (static_cast<s32>(sGnFrame) > mSpawnTimer && sSlogCount < mMaxSlogsAtATime)
    {
        if (SwitchStates_Get(mSpawnerSwitchId))
        {
            mSpawnTimer = (mSlogSpawnInterval + sGnFrame) + Math_NextRandom() % 8;
            auto pSlog = relive_new Slog(mXPos, mYPos, mScale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1), static_cast<s16>(mListenToSligs), mChaseDelay);
            if (pSlog)
            {
                pSlog->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, mStartDirection == StartDirection::eLeft_1);
            }

            ++mSpawnedSlogsCount;
            SfxPlayMono(SoundEffect::SlogSpawn_115, 0);

            if (mSpawnedSlogsCount >= mMaxSlogs)
            {
                Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount, 0, 1);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
    }
}

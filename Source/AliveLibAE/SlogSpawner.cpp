#include "stdafx.h"
#include "SlogSpawner.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"
#include "Path.hpp"
#include "Math.hpp"

SlogSpawner::SlogSpawner(relive::Path_SlogSpawner* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvInfo = tlvId;
    mScale = pTlv->mScale;
    mMaxSlogs = pTlv->mMaxSlogs;
    mMaxSlogsAtATime = pTlv->mMaxSlogsAtATime;
    mStartDirection = pTlv->mStartDirection;
    mSlogSpawnInterval = pTlv->mSlogSpawnInterval;
    mSpawnerSwitchId = pTlv->mSpawnerSwitchId;
    mListenToSligs = pTlv->mListenToSligs;
    mChaseDelay = pTlv->mChaseDelay;

    mSpawnedSlogsCount = pTlv->mTlvSpecificMeaning;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mSpawnTimer = 0;
}

void SlogSpawner::VScreenChanged()
{
    Path::TLV_Reset(mTlvInfo, mSpawnedSlogsCount);
    SetDead(true);
}

void SlogSpawner::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (static_cast<s32>(sGnFrame) > mSpawnTimer && sSlogCount < mMaxSlogsAtATime)
    {
        if (SwitchStates_Get(mSpawnerSwitchId))
        {
            mSpawnTimer = (sGnFrame + mSlogSpawnInterval) + Math_NextRandom() % 8;

            auto pSlog = relive_new Slog(mXPos, mYPos, mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1), static_cast<s16>(mListenToSligs), mChaseDelay);
            if (pSlog)
            {
                pSlog->GetAnimation().SetFlipX(mStartDirection == relive::Path_SlogSpawner::StartDirection::eLeft);
            }

            mSpawnedSlogsCount++;
            SfxPlayMono(relive::SoundEffects::SlogSpawn, 0);

            if (mSpawnedSlogsCount >= mMaxSlogs)
            {
                Path::TLV_Delete(mTlvInfo, mSpawnedSlogsCount);
                SetDead(true);
            }
        }
    }
}

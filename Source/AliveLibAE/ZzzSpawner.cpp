#include "stdafx.h"
#include "ZzzSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "SnoozeParticle.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

ZzzSpawner::ZzzSpawner(Path_ZzzSpawner* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);
    mTlvInfo = tlvInfo;

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else if (pTlv->mScale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
    }

    mSwitchId = pTlv->mSwitchId;
    mZzzInterval = pTlv->mZzzInterval;
    mTimer = 0;
}

ZzzSpawner::~ZzzSpawner()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void ZzzSpawner::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void ZzzSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!SwitchStates_Get(mSwitchId) && static_cast<s32>(sGnFrame) > mTimer)
    {
        relive_new SnoozeParticle(mXPos, mYPos, Layer::eLayer_Above_FG1_39, mSpriteScale);

        mTimer = sGnFrame + mZzzInterval;
    }
}

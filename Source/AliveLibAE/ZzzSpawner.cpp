#include "stdafx.h"
#include "ZzzSpawner.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "SnoozeParticle.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Path.hpp"

ZzzSpawner::ZzzSpawner(relive::Path_ZzzSpawner* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
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

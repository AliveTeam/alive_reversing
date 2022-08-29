#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZzzSpawner.hpp"
#include "stdlib.hpp"
#include "SnoozeParticle.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

namespace AO {

ZzzSpawner::~ZzzSpawner()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

ZzzSpawner::ZzzSpawner(relive::Path_ZzzSpawner* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
    }

    mSwitchId = pTlv->mSwitchId;
    mZzzInterval = pTlv->mZzzInterval;
    mTimer = 0;
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
        Layer snoozeLayer = Layer::eLayer_0;
        if (mSpriteScale != FP_FromInteger(1))
        {
            snoozeLayer = Layer::eLayer_Above_FG1_Half_20;
        }
        else
        {
            snoozeLayer = Layer::eLayer_Above_FG1_39;
        }
        relive_new SnoozeParticle(
            mXPos,
            mYPos,
            snoozeLayer,
            mSpriteScale);

        mTimer = sGnFrame + mZzzInterval;
    }
}

} // namespace AO

#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "ZzzSpawner.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "SnoozeParticle.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Path.hpp"

namespace AO {


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
    else
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
    SetDead(true);
}

void ZzzSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
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
        relive_new SnoozeParticle(mXPos, mYPos, snoozeLayer, mSpriteScale);

        mTimer = MakeTimer(mZzzInterval);
    }
}

} // namespace AO

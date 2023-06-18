#include "stdafx.h"
#include "ZzzSpawner.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "SnoozeParticle.hpp"
#include "Game.hpp"
#include "../relive_lib/SwitchStates.hpp"
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
        relive_new SnoozeParticle(mXPos, mYPos, Layer::eLayer_Above_FG1_39, mSpriteScale);

        mTimer = MakeTimer(mZzzInterval);
    }
}

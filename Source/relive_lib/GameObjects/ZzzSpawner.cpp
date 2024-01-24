#include "stdafx.h"
#include "../Function.hpp"
#include "ZzzSpawner.hpp"
#include "../../AliveLibAE/stdlib.hpp"
#include "SnoozeParticle.hpp"
#include "../Events.hpp"
#include "../SwitchStates.hpp"
#include "../GameType.hpp"

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
    GetMap().TLV_Reset(mTlvInfo);
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
        Layer snoozeLayer = Layer::eLayer_Above_FG1_39;
        // AO doesn't set a different layer for the bg
        if (GetGameType() == GameType::eAe)
        {
            if (mSpriteScale != FP_FromInteger(1))
            {
                snoozeLayer = Layer::eLayer_Above_FG1_Half_20;
            }
        }

        relive_new SnoozeParticle(mXPos, mYPos, snoozeLayer, mSpriteScale);

        mTimer = MakeTimer(mZzzInterval);
    }
}

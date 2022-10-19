#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimerTrigger.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Path.hpp"

namespace AO {

void TimerTrigger::VScreenChanged()
{
    if (mState == State::eWaitForEnabled_0 || mState == State::eCheckForStartAgain_2 || gMap.LevelChanged() || gMap.PathChanged())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

TimerTrigger::~TimerTrigger()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

TimerTrigger::TimerTrigger(relive::Path_TimerTrigger* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    mTlvInfo = tlvId;

    mActivationDelay = pTlv->mActivationDelay;

    mOutputSwitchIds[0] = pTlv->mOutputSwitchId1;
    mOutputSwitchIds[1] = pTlv->mOutputSwitchId2;
    mOutputSwitchIds[2] = pTlv->mOutputSwitchId3;
    mOutputSwitchIds[3] = pTlv->mOutputSwitchId4;

    mInputSwitchId = pTlv->mInputSwitchId;
    mState = State::eWaitForEnabled_0;

    if (mInputSwitchId)
    {
        mStartingSwitchState = SwitchStates_Get(mInputSwitchId);
    }
    else
    {
        mStartingSwitchState = 0;
    }
}

void TimerTrigger::ToggleAllIds()
{
    for (const auto& id : mOutputSwitchIds)
    {
        if (id > 1)
        {
            SwitchStates_Do_Operation(id, relive::reliveSwitchOp::eToggle);
        }
    }
}

void TimerTrigger::VUpdate()
{
    switch (mState)
    {
        case State::eWaitForEnabled_0:
            // If the value changes from what we first saw...
            if (SwitchStates_Get(mInputSwitchId) != mStartingSwitchState)
            {
                mState = State::eWaitForFirstTrigger_1;
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case State::eWaitForFirstTrigger_1:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                mState = State::eCheckForStartAgain_2;
            }
            break;

        case State::eCheckForStartAgain_2:
            if (SwitchStates_Get(mInputSwitchId) == mStartingSwitchState)
            {
                mState = State::eWaitForSecondTrigger_3;
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case State::eWaitForSecondTrigger_3:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                mState = State::eWaitForEnabled_0;
            }
            break;

        default:
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO

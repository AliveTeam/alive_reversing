#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimerTrigger.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

namespace AO {

void TimerTrigger::VScreenChanged()
{
    if (field_12_state == State::eWaitForEnabled_0 || field_12_state == State::eCheckForStartAgain_2 || gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

TimerTrigger::~TimerTrigger()
{
    Path::TLV_Reset(field_1C_tlvInfo, -1, 0, 0);
}

TimerTrigger::TimerTrigger(Path_TimerTrigger* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_1C_tlvInfo = tlvInfo;

    mActivationDelay = pTlv->mActivationDelay;

    mOutputSwitchIds[0] = pTlv->mOutputSwitchId1;
    mOutputSwitchIds[1] = pTlv->mOutputSwitchId2;
    mOutputSwitchIds[2] = pTlv->mOutputSwitchId3;
    mOutputSwitchIds[3] = pTlv->mOutputSwitchId4;

    mInputSwitchId = pTlv->mInputSwitchId;
    field_12_state = State::eWaitForEnabled_0;

    if (mInputSwitchId)
    {
        field_28_starting_switch_state = SwitchStates_Get(mInputSwitchId);
    }
    else
    {
        field_28_starting_switch_state = 0;
    }
}

void TimerTrigger::ToggleAllIds()
{
    for (const auto& id : mOutputSwitchIds)
    {
        if (id > 1)
        {
            SwitchStates_Do_Operation(id, SwitchOp::eToggle_2);
        }
    }
}

void TimerTrigger::VUpdate()
{
    switch (field_12_state)
    {
        case State::eWaitForEnabled_0:
            // If the value changes from what we first saw...
            if (SwitchStates_Get(mInputSwitchId) != field_28_starting_switch_state)
            {
                field_12_state = State::eWaitForFirstTrigger_1;
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case State::eWaitForFirstTrigger_1:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                field_12_state = State::eCheckForStartAgain_2;
            }
            break;

        case State::eCheckForStartAgain_2:
            if (SwitchStates_Get(mInputSwitchId) == field_28_starting_switch_state)
            {
                field_12_state = State::eWaitForSecondTrigger_3;
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case State::eWaitForSecondTrigger_3:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                field_12_state = State::eWaitForEnabled_0;
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

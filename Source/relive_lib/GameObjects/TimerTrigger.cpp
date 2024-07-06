#include "stdafx.h"
#include "TimerTrigger.hpp"
#include "../Function.hpp"
#include "../SwitchStates.hpp"
#include "../MapWrapper.hpp"
#include "../Events.hpp"
#include "../../AliveLibAE/QuikSave.hpp"

TimerTrigger::TimerTrigger(relive::Path_TimerTrigger* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvInfo = tlvId;
    SetType(ReliveTypes::eTimerTrigger);
    mActivationDelay = pTlv->mActivationDelay;

    mOutputSwitchIds[0] = pTlv->mOutputSwitchId1;
    mOutputSwitchIds[1] = pTlv->mOutputSwitchId2;
    mOutputSwitchIds[2] = pTlv->mOutputSwitchId3;
    mOutputSwitchIds[3] = pTlv->mOutputSwitchId4;

    mInputSwitchId = pTlv->mInputSwitchId;
    mState = TimerTriggerStates::eWaitForEnabled_0;
    mStartingSwitchState = SwitchStates_Get(mInputSwitchId);
}

void TimerTrigger::ToggleAllIds()
{
    for (const auto& id : mOutputSwitchIds)
    {
        if (id != 0)
        {
            SwitchStates_Do_Operation(id, relive::reliveSwitchOp::eToggle);
        }
    }
}

void TimerTrigger::VUpdate()
{
    switch (mState)
    {
        case TimerTriggerStates::eWaitForEnabled_0:
            // If the value changes from what we first saw...
            if (SwitchStates_Get(mInputSwitchId) != mStartingSwitchState)
            {
                mState = TimerTriggerStates::eWaitForFirstTrigger_1;
                mActivationDelayTimer = MakeTimer(mActivationDelay);
            }
            break;

        case TimerTriggerStates::eWaitForFirstTrigger_1:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                mState = TimerTriggerStates::eCheckForStartAgain_2;
            }
            break;

        case TimerTriggerStates::eCheckForStartAgain_2:
            if (SwitchStates_Get(mInputSwitchId) == mStartingSwitchState)
            {
                mState = TimerTriggerStates::eWaitForSecondTrigger_3;
                mActivationDelayTimer = MakeTimer(mActivationDelay);
            }
            break;

        case TimerTriggerStates::eWaitForSecondTrigger_3:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                mState = TimerTriggerStates::eWaitForEnabled_0;
            }
            break;

        default:
            break;
    }

    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }
}

void TimerTrigger::VScreenChanged()
{
    if (mState == TimerTriggerStates::eWaitForEnabled_0 || mState == TimerTriggerStates::eCheckForStartAgain_2 || GetMap().LevelChanged() || GetMap().PathChanged())
    {
        SetDead(true);
    }
}

TimerTrigger::~TimerTrigger()
{
    GetMap().TLV_Reset(mTlvInfo);
}

void TimerTrigger::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pState = pData.ReadTmpPtr<TimerTriggerSaveState>();

    relive::Path_TimerTrigger* pTlv = static_cast<relive::Path_TimerTrigger*>(GetMap().TLV_From_Offset_Lvl_Cam(pState->mTlvId));
    auto pTimerTrigger = relive_new TimerTrigger(pTlv, pState->mTlvId);
    if (pTimerTrigger)
    {
        pTimerTrigger->mState = pState->mState;
        pTimerTrigger->mActivationDelayTimer = sGnFrame + pState->mActivationDelayTimer;
        pTimerTrigger->mStartingSwitchState = pState->mStartingSwitchState;
    }
}

void TimerTrigger::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    TimerTriggerSaveState data = {};

    data.mType = ReliveTypes::eTimerTrigger;
    data.mTlvId = mTlvInfo;
    data.mState = mState;
    data.mActivationDelayTimer = mActivationDelayTimer - sGnFrame;
    data.mStartingSwitchState = mStartingSwitchState;
    pSaveBuffer.Write(data);
}

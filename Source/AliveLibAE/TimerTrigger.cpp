#include "stdafx.h"
#include "TimerTrigger.hpp"
#include "Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Path.hpp"

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
    mStartingSwitchState = static_cast<s16>(SwitchStates_Get(mInputSwitchId));
}

void TimerTrigger::ToggleAllIds()
{
    for (auto& id : mOutputSwitchIds)
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
                mActivationDelayTimer = sGnFrame + mActivationDelay;
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
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case TimerTriggerStates::eWaitForSecondTrigger_3:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                mState = TimerTriggerStates::eWaitForEnabled_0;
            }
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void TimerTrigger::VScreenChanged()
{
    if (mState == TimerTriggerStates::eWaitForEnabled_0 || mState == TimerTriggerStates::eCheckForStartAgain_2 || gMap.LevelChanged() || gMap.PathChanged())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

TimerTrigger::~TimerTrigger()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

s32 TimerTrigger::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const TimerTriggerSaveState*>(pData);

    relive::Path_TimerTrigger* pTlv = static_cast<relive::Path_TimerTrigger*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));
    auto pTimerTrigger = relive_new TimerTrigger(pTlv, pState->field_4_tlvInfo);
    if (pTimerTrigger)
    {
        pTimerTrigger->mState = pState->field_C_state;
        pTimerTrigger->mActivationDelayTimer = sGnFrame + pState->field_8_delay_timer_base;
        pTimerTrigger->mStartingSwitchState = pState->field_E_starting_switch_state;
    }
    return sizeof(TimerTriggerSaveState);
}

s32 TimerTrigger::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<TimerTriggerSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eTimerTrigger;
    pState->field_4_tlvInfo = mTlvInfo;
    pState->field_C_state = mState;
    pState->field_8_delay_timer_base = mActivationDelayTimer - sGnFrame;
    pState->field_E_starting_switch_state = mStartingSwitchState;
    return sizeof(TimerTriggerSaveState);
}

#include "stdafx.h"
#include "TimerTrigger.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"

TimerTrigger::TimerTrigger(Path_TimerTrigger* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_2C_tlvInfo = tlvInfo;
    SetType(ReliveTypes::eTimerTrigger);
    mInputSwitchId = pTlv->mInputSwitchId;
    mActivationDelay = pTlv->mActivationDelay;
    mOutputSwitchIds[0] = pTlv->mOutputSwitchId1;
    mOutputSwitchIds[1] = pTlv->mOutputSwitchId2;
    mOutputSwitchIds[2] = pTlv->mOutputSwitchId3;
    mOutputSwitchIds[3] = pTlv->mOutputSwitchId4;
    field_38_starting_switch_state = static_cast<s16>(SwitchStates_Get(mInputSwitchId));
    field_22_state = TimerTriggerStates::eWaitForEnabled_0;
}

s32 TimerTrigger::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const TimerTrigger_State*>(pData);

    Path_TimerTrigger* pTlv = static_cast<Path_TimerTrigger*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));
    auto pTimerTrigger = relive_new TimerTrigger(pTlv, pState->field_4_tlvInfo);
    if (pTimerTrigger)
    {
        pTimerTrigger->field_22_state = pState->field_C_state;
        pTimerTrigger->mActivationDelayTimer = sGnFrame + pState->field_8_delay_timer_base;
        pTimerTrigger->field_38_starting_switch_state = pState->field_E_starting_switch_state;
    }
    return sizeof(TimerTrigger_State);
}

TimerTrigger::~TimerTrigger()
{
    Path::TLV_Reset(field_2C_tlvInfo, -1, 0, 0);
}

void TimerTrigger::VUpdate()
{
    switch (field_22_state)
    {
        case TimerTriggerStates::eWaitForEnabled_0:
            if (SwitchStates_Get(mInputSwitchId) != field_38_starting_switch_state)
            {
                field_22_state = TimerTriggerStates::eWaitForFirstTrigger_1;
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case TimerTriggerStates::eWaitForFirstTrigger_1:
            if (mActivationDelayTimer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                field_22_state = TimerTriggerStates::eCheckForStartAgain_2;
            }
            break;

        case TimerTriggerStates::eCheckForStartAgain_2:
            if (SwitchStates_Get(mInputSwitchId) == field_38_starting_switch_state)
            {
                field_22_state = TimerTriggerStates::eWaitForSecondTrigger_3;
                mActivationDelayTimer = sGnFrame + mActivationDelay;
            }
            break;

        case TimerTriggerStates::eWaitForSecondTrigger_3:
            if (mActivationDelayTimer <= (s32) sGnFrame)
            {
                ToggleAllIds();
                field_22_state = TimerTriggerStates::eWaitForEnabled_0;
            }
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void TimerTrigger::ToggleAllIds()
{
    for (auto& id : mOutputSwitchIds)
    {
        if (id != 0)
        {
            SwitchStates_Do_Operation(id, SwitchOp::eToggle_2);
        }
    }
}

void TimerTrigger::VScreenChanged()
{
    if (field_22_state == TimerTriggerStates::eWaitForEnabled_0 || field_22_state == TimerTriggerStates::eCheckForStartAgain_2 || gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s32 TimerTrigger::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<TimerTrigger_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eTimerTrigger_136;
    pState->field_4_tlvInfo = field_2C_tlvInfo;
    pState->field_C_state = field_22_state;
    pState->field_8_delay_timer_base = mActivationDelayTimer - sGnFrame;
    pState->field_E_starting_switch_state = field_38_starting_switch_state;
    return sizeof(TimerTrigger_State);
}

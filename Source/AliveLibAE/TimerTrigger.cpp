#include "stdafx.h"
#include "TimerTrigger.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"

TimerTrigger::TimerTrigger(Path_TimerTrigger* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_2C_tlvInfo = tlvInfo;
    SetType(ReliveTypes::eTimerTrigger);
    field_20_input_switch_id = pTlv->field_10_input_switch_id;
    field_34_trigger_interval = pTlv->field_12_trigger_interval;
    field_24_output_switch_ids[0] = pTlv->field_14_output_switch_id1;
    field_24_output_switch_ids[1] = pTlv->field_16_output_switch_id2;
    field_24_output_switch_ids[2] = pTlv->field_18_output_switch_id3;
    field_24_output_switch_ids[3] = pTlv->field_1A_output_switch_id4;
    field_38_starting_switch_state = static_cast<s16>(SwitchStates_Get(field_20_input_switch_id));
    field_22_state = TimerTriggerStates::eWaitForEnabled_0;
}

s32 TimerTrigger::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const TimerTrigger_State*>(pData);

    Path_TimerTrigger* pTlv = static_cast<Path_TimerTrigger*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));
    auto pTimerTrigger = ae_new<TimerTrigger>(pTlv, pState->field_4_tlvInfo);
    if (pTimerTrigger)
    {
        pTimerTrigger->field_22_state = pState->field_C_state;
        pTimerTrigger->field_30_trigger_interval_timer = sGnFrame + pState->field_8_delay_timer_base;
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
            if (SwitchStates_Get(field_20_input_switch_id) != field_38_starting_switch_state)
            {
                field_22_state = TimerTriggerStates::eWaitForFirstTrigger_1;
                field_30_trigger_interval_timer = sGnFrame + field_34_trigger_interval;
            }
            break;

        case TimerTriggerStates::eWaitForFirstTrigger_1:
            if (field_30_trigger_interval_timer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                field_22_state = TimerTriggerStates::eCheckForStartAgain_2;
            }
            break;

        case TimerTriggerStates::eCheckForStartAgain_2:
            if (SwitchStates_Get(field_20_input_switch_id) == field_38_starting_switch_state)
            {
                field_22_state = TimerTriggerStates::eWaitForSecondTrigger_3;
                field_30_trigger_interval_timer = sGnFrame + field_34_trigger_interval;
            }
            break;

        case TimerTriggerStates::eWaitForSecondTrigger_3:
            if (field_30_trigger_interval_timer <= (s32) sGnFrame)
            {
                ToggleAllIds();
                field_22_state = TimerTriggerStates::eWaitForEnabled_0;
            }
            break;
    }

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void TimerTrigger::ToggleAllIds()
{
    for (auto& id : field_24_output_switch_ids)
    {
        if (id != 0)
        {
            SwitchStates_Do_Operation(id, SwitchOp::eToggle_2);
        }
    }
}

void TimerTrigger::VScreenChanged()
{
    if (field_22_state == TimerTriggerStates::eWaitForEnabled_0 || field_22_state == TimerTriggerStates::eCheckForStartAgain_2 || gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.mOverlayId != gMap.GetOverlayId())
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
    pState->field_8_delay_timer_base = field_30_trigger_interval_timer - sGnFrame;
    pState->field_E_starting_switch_state = field_38_starting_switch_state;
    return sizeof(TimerTrigger_State);
}

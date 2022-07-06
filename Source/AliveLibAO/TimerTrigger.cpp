#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimerTrigger.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
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

    field_24_trigger_interval = pTlv->field_1A_trigger_interval;

    field_14_ids[0] = pTlv->field_1C_id1;
    field_14_ids[1] = pTlv->field_1C_id2;
    field_14_ids[2] = pTlv->field_1C_id3;
    field_14_ids[3] = pTlv->field_1C_id4;

    field_10_source_switch_id = pTlv->field_18_source_switch_id;
    field_12_state = State::eWaitForEnabled_0;

    if (field_10_source_switch_id)
    {
        field_28_starting_switch_state = SwitchStates_Get(field_10_source_switch_id);
    }
    else
    {
        field_28_starting_switch_state = 0;
    }
}

void TimerTrigger::ToggleAllIds()
{
    for (const auto& id : field_14_ids)
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
            if (SwitchStates_Get(field_10_source_switch_id) != field_28_starting_switch_state)
            {
                field_12_state = State::eWaitForFirstTrigger_1;
                field_20_trigger_interval_timer = sGnFrame + field_24_trigger_interval;
            }
            break;

        case State::eWaitForFirstTrigger_1:
            if (field_20_trigger_interval_timer <= static_cast<s32>(sGnFrame))
            {
                ToggleAllIds();
                field_12_state = State::eCheckForStartAgain_2;
            }
            break;

        case State::eCheckForStartAgain_2:
            if (SwitchStates_Get(field_10_source_switch_id) == field_28_starting_switch_state)
            {
                field_12_state = State::eWaitForSecondTrigger_3;
                field_20_trigger_interval_timer = sGnFrame + field_24_trigger_interval;
            }
            break;

        case State::eWaitForSecondTrigger_3:
            if (field_20_trigger_interval_timer <= static_cast<s32>(sGnFrame))
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

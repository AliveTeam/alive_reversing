#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimerTrigger.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

namespace AO {

void TimerTrigger::VUpdate()
{
    VUpdate_479C40();
}

void TimerTrigger::VScreenChanged_479DB0()
{
    if (field_12_state == State::eWaitForEnabled_0 || field_12_state == State::eCheckForStartAgain_2 || gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void TimerTrigger::VScreenChanged()
{
    VScreenChanged_479DB0();
}

TimerTrigger* TimerTrigger::Vdtor_479E00(s32 flags)
{
    dtor_479BE0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* TimerTrigger::VDestructor(s32 flags)
{
    return Vdtor_479E00(flags);
}

BaseGameObject* TimerTrigger::dtor_479BE0()
{
    SetVTable(this, 0x4BCE20);
    gMap_507BA8.TLV_Reset_446870(field_1C_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

TimerTrigger* TimerTrigger::ctor_479B40(Path_TimerTrigger* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    field_1C_tlvInfo = tlvInfo;
    SetVTable(this, 0x4BCE20);

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

    return this;
}

void TimerTrigger::ToggleAllIds()
{
    for (const auto& id : field_14_ids)
    {
        if (id > 1)
        {
            SwitchStates_Do_Operation_436A10(id, SwitchOp::eToggle_2);
        }
    }
}

void TimerTrigger::VUpdate_479C40()
{
    switch (field_12_state)
    {
        case State::eWaitForEnabled_0:
            // If the value changes from what we first saw...
            if (SwitchStates_Get(field_10_source_switch_id) != field_28_starting_switch_state)
            {
                field_12_state = State::eWaitForFirstTrigger_1;
                field_20_trigger_interval_timer = gnFrameCount_507670 + field_24_trigger_interval;
            }
            break;

        case State::eWaitForFirstTrigger_1:
            if (field_20_trigger_interval_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                ToggleAllIds();
                field_12_state = State::eCheckForStartAgain_2;
            }
            break;

        case State::eCheckForStartAgain_2:
            if (SwitchStates_Get(field_10_source_switch_id) == field_28_starting_switch_state)
            {
                field_12_state = State::eWaitForSecondTrigger_3;
                field_20_trigger_interval_timer = gnFrameCount_507670 + field_24_trigger_interval;
            }
            break;

        case State::eWaitForSecondTrigger_3:
            if (field_20_trigger_interval_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                ToggleAllIds();
                field_12_state = State::eWaitForEnabled_0;
            }
            break;

        default:
            break;
    }

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

} // namespace AO

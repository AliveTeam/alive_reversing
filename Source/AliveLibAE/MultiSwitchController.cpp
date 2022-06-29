#include "stdafx.h"
#include "MultiSwitchController.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

MultiSwitchController::MultiSwitchController(Path_MultiSwitchController* pTlv, u32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_40_flags.Clear(Flags_40::eBit1_is_on);
    mBaseGameObjectTlvInfo = tlvInfo;
    field_30_tlvInfo = tlvInfo;

    field_34_last_switch_on_time = 0;
    field_38_all_switches_on_or_off_time = 0;

    field_2C_output_switch_id = pTlv->field_10_output_switch_id;

    field_2E_action = pTlv->field_12_action;

    // Time after all switches on/off to activate id switch
    field_3C_on_off_delay = pTlv->field_14_on_off_delay;

    field_20_input_switch_id1 = pTlv->field_16_input_switch_id1;
    field_22_input_switch_id2 = pTlv->field_18_input_switch_id2;
    field_24_input_switch_id3 = pTlv->field_1A_input_switch_id3;
    field_26_input_switch_id4 = pTlv->field_1C_input_switch_id4;
    field_28_input_switch_id5 = pTlv->field_1E_input_switch_id5;
    field_2A_input_switch_id6 = pTlv->field_20_input_switch_id6;
}

MultiSwitchController::~MultiSwitchController()
{
    Path::TLV_Reset(field_30_tlvInfo, -1, 0, 0);
}

void MultiSwitchController::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MultiSwitchController::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_40_flags.Get(Flags_40::eBit1_is_on))
    {
        WaitingForAllOff();
    }
    else
    {
        WaitingForAllOn();
    }
}

void MultiSwitchController::WaitingForAllOn()
{
    const s32 id1_value = SwitchStates_Get(field_20_input_switch_id1);
    if (id1_value)
    {
        if (field_34_last_switch_on_time == 0 && field_20_input_switch_id1 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    const s32 id2_value = SwitchStates_Get(field_22_input_switch_id2);
    if (id2_value)
    {
        if (field_34_last_switch_on_time == 0 && field_22_input_switch_id2 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    const s32 id3_value = SwitchStates_Get(field_24_input_switch_id3);
    if (id3_value)
    {
        if (field_34_last_switch_on_time == 0 && field_24_input_switch_id3 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    const s32 id4_value = SwitchStates_Get(field_26_input_switch_id4);
    if (id4_value)
    {
        if (field_34_last_switch_on_time == 0 && field_26_input_switch_id4 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    const s32 id5_value = SwitchStates_Get(field_28_input_switch_id5);
    if (id5_value)
    {
        if (field_34_last_switch_on_time == 0 && field_28_input_switch_id5 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    const s32 id6_value = SwitchStates_Get(field_2A_input_switch_id6);
    if (id6_value)
    {
        if (field_34_last_switch_on_time == 0 && field_2A_input_switch_id6 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    if ((!id1_value || field_20_input_switch_id1 == 1) && (!id2_value || field_22_input_switch_id2 == 1) && (!id3_value || field_24_input_switch_id3 == 1) && (!id4_value || field_26_input_switch_id4 == 1) && (!id5_value || field_28_input_switch_id5 == 1) && (!id6_value || field_2A_input_switch_id6 == 1))
    {
        field_34_last_switch_on_time = 0;
    }

    if (id1_value && id2_value && id3_value && id4_value && id5_value && id6_value && field_38_all_switches_on_or_off_time == 0)
    {
        field_38_all_switches_on_or_off_time = sGnFrame;
        if (field_38_all_switches_on_or_off_time - field_34_last_switch_on_time <= field_3C_on_off_delay)
        {
            SwitchStates_Do_Operation(field_2C_output_switch_id, field_2E_action);
            field_40_flags.Set(Flags_40::eBit1_is_on);
            field_34_last_switch_on_time = 0;
            field_38_all_switches_on_or_off_time = 0;
        }
    }
    else if (static_cast<s32>(sGnFrame) - field_34_last_switch_on_time > field_3C_on_off_delay)
    {
        if (field_20_input_switch_id1 > 1u)
        {
            SwitchStates_Set(field_20_input_switch_id1, 0);
        }

        if (field_22_input_switch_id2 > 1u)
        {
            SwitchStates_Set(field_22_input_switch_id2, 0);
        }

        if (field_24_input_switch_id3 > 1u)
        {
            SwitchStates_Set(field_24_input_switch_id3, 0);
        }

        if (field_26_input_switch_id4 > 1u)
        {
            SwitchStates_Set(field_26_input_switch_id4, 0);
        }

        if (field_28_input_switch_id5 > 1u)
        {
            SwitchStates_Set(field_28_input_switch_id5, 0);
        }

        if (field_2A_input_switch_id6 > 1u)
        {
            SwitchStates_Set(field_2A_input_switch_id6, 0);
        }

        field_34_last_switch_on_time = 0;
    }
}

void MultiSwitchController::WaitingForAllOff()
{
    s32 id1_value = 0;
    if (field_20_input_switch_id1 > 1u)
    {
        id1_value = SwitchStates_Get(field_20_input_switch_id1);
    }
    if (id1_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_20_input_switch_id1 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    s32 id2_value = 0;
    if (field_22_input_switch_id2 > 1u)
    {
        id2_value = SwitchStates_Get(field_22_input_switch_id2);
    }
    if (id2_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_22_input_switch_id2 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    s32 id3_value = 0;
    if (field_24_input_switch_id3 > 1u)
    {
        id3_value = SwitchStates_Get(field_24_input_switch_id3);
    }
    if (id3_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_24_input_switch_id3 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    s32 id4_value = 0;
    if (field_26_input_switch_id4 > 1u)
    {
        id4_value = SwitchStates_Get(field_26_input_switch_id4);
    }
    if (id4_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_26_input_switch_id4 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    s32 id5_value = 0;
    if (field_28_input_switch_id5 > 1u)
    {
        id5_value = SwitchStates_Get(field_28_input_switch_id5);
    }
    if (id5_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_28_input_switch_id5 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    s32 id6_value = 0;
    if (field_2A_input_switch_id6 > 1u)
    {
        id6_value = SwitchStates_Get(field_2A_input_switch_id6);
    }
    if (id6_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_2A_input_switch_id6 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame;
        }
    }

    if ((id1_value || field_20_input_switch_id1 == 1) && (id2_value || field_22_input_switch_id2 == 1) && (id3_value || field_24_input_switch_id3 == 1) && (id4_value || field_26_input_switch_id4 == 1) && (id5_value || field_28_input_switch_id5 == 1) && (id6_value || field_2A_input_switch_id6 == 1))
    {
        field_34_last_switch_on_time = 0;
    }

    if (!id1_value && !id2_value && !id3_value && !id4_value && !id5_value && !id6_value && field_38_all_switches_on_or_off_time == 0)
    {
        field_38_all_switches_on_or_off_time = sGnFrame;
        if (field_38_all_switches_on_or_off_time - field_34_last_switch_on_time <= field_3C_on_off_delay)
        {
            SwitchStates_Do_Operation(field_2C_output_switch_id, field_2E_action);
            field_40_flags.Clear(Flags_40::eBit1_is_on);
            field_34_last_switch_on_time = 0;
        }
        // TODO: OG bug, why is this always reset ??
        field_38_all_switches_on_or_off_time = 0;
    }
}

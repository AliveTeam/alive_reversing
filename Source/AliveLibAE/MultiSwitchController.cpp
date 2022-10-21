#include "stdafx.h"
#include "MultiSwitchController.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

BaseGameObject* MultiSwitchController::VDestructor(s32 flags)
{
    return vdtor_4D61A0(flags);
}

void MultiSwitchController::VUpdate()
{
    vUpdate_4D6250();
}

void MultiSwitchController::VScreenChanged()
{
    vScreenChanged_4D6860();
}

MultiSwitchController* MultiSwitchController::ctor_4D60E0(Path_MultiSwitchController* pTlv, u32 tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_40_flags.Clear(Flags_40::eBit1_is_on);
    field_C_objectId = tlvInfo;
    field_30_tlvInfo = tlvInfo;
    SetVTable(this, 0x547AA8); // vTbl_MultiSwitchController_547AA8

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

    return this;
}

MultiSwitchController* MultiSwitchController::vdtor_4D61A0(s32 flags)
{
    dtor_4D61D0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MultiSwitchController::dtor_4D61D0()
{
    SetVTable(this, 0x547AA8); // vTbl_MultiSwitchController_547AA8
    Path::TLV_Reset_4DB8E0(field_30_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

void MultiSwitchController::vScreenChanged_4D6860()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void MultiSwitchController::vUpdate_4D6250()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_40_flags.Get(Flags_40::eBit1_is_on))
    {
        WaitingForAllOff_4D65B0();
    }
    else
    {
        WaitingForAllOn_4D6290();
    }
}

EXPORT void MultiSwitchController::WaitingForAllOn_4D6290()
{
    const s32 id1_value = SwitchStates_Get_466020(field_20_input_switch_id1);
    if (id1_value)
    {
        if (field_34_last_switch_on_time == 0 && field_20_input_switch_id1 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const s32 id2_value = SwitchStates_Get_466020(field_22_input_switch_id2);
    if (id2_value)
    {
        if (field_34_last_switch_on_time == 0 && field_22_input_switch_id2 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const s32 id3_value = SwitchStates_Get_466020(field_24_input_switch_id3);
    if (id3_value)
    {
        if (field_34_last_switch_on_time == 0 && field_24_input_switch_id3 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const s32 id4_value = SwitchStates_Get_466020(field_26_input_switch_id4);
    if (id4_value)
    {
        if (field_34_last_switch_on_time == 0 && field_26_input_switch_id4 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const s32 id5_value = SwitchStates_Get_466020(field_28_input_switch_id5);
    if (id5_value)
    {
        if (field_34_last_switch_on_time == 0 && field_28_input_switch_id5 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const s32 id6_value = SwitchStates_Get_466020(field_2A_input_switch_id6);
    if (id6_value)
    {
        if (field_34_last_switch_on_time == 0 && field_2A_input_switch_id6 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    if ((!id1_value || field_20_input_switch_id1 == 1) && (!id2_value || field_22_input_switch_id2 == 1) && (!id3_value || field_24_input_switch_id3 == 1) && (!id4_value || field_26_input_switch_id4 == 1) && (!id5_value || field_28_input_switch_id5 == 1) && (!id6_value || field_2A_input_switch_id6 == 1))
    {
        field_34_last_switch_on_time = 0;
    }

    if (id1_value && id2_value && id3_value && id4_value && id5_value && id6_value && field_38_all_switches_on_or_off_time == 0)
    {
        field_38_all_switches_on_or_off_time = sGnFrame_5C1B84;
        if (field_38_all_switches_on_or_off_time - field_34_last_switch_on_time <= field_3C_on_off_delay)
        {
            SwitchStates_Do_Operation_465F00(field_2C_output_switch_id, field_2E_action);
            field_40_flags.Set(Flags_40::eBit1_is_on);
            field_34_last_switch_on_time = 0;
            field_38_all_switches_on_or_off_time = 0;
        }
    }
    else if (static_cast<s32>(sGnFrame_5C1B84) - field_34_last_switch_on_time > field_3C_on_off_delay)
    {
        if (field_20_input_switch_id1 > 1u)
        {
            SwitchStates_Set_465FF0(field_20_input_switch_id1, 0);
        }

        if (field_22_input_switch_id2 > 1u)
        {
            SwitchStates_Set_465FF0(field_22_input_switch_id2, 0);
        }

        if (field_24_input_switch_id3 > 1u)
        {
            SwitchStates_Set_465FF0(field_24_input_switch_id3, 0);
        }

        if (field_26_input_switch_id4 > 1u)
        {
            SwitchStates_Set_465FF0(field_26_input_switch_id4, 0);
        }

        if (field_28_input_switch_id5 > 1u)
        {
            SwitchStates_Set_465FF0(field_28_input_switch_id5, 0);
        }

        if (field_2A_input_switch_id6 > 1u)
        {
            SwitchStates_Set_465FF0(field_2A_input_switch_id6, 0);
        }

        field_34_last_switch_on_time = 0;
    }
}

EXPORT void MultiSwitchController::WaitingForAllOff_4D65B0()
{
    s32 id1_value = 0;
    if (field_20_input_switch_id1 > 1u)
    {
        id1_value = SwitchStates_Get_466020(field_20_input_switch_id1);
    }
    if (id1_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_20_input_switch_id1 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    s32 id2_value = 0;
    if (field_22_input_switch_id2 > 1u)
    {
        id2_value = SwitchStates_Get_466020(field_22_input_switch_id2);
    }
    if (id2_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_22_input_switch_id2 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    s32 id3_value = 0;
    if (field_24_input_switch_id3 > 1u)
    {
        id3_value = SwitchStates_Get_466020(field_24_input_switch_id3);
    }
    if (id3_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_24_input_switch_id3 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    s32 id4_value = 0;
    if (field_26_input_switch_id4 > 1u)
    {
        id4_value = SwitchStates_Get_466020(field_26_input_switch_id4);
    }
    if (id4_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_26_input_switch_id4 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    s32 id5_value = 0;
    if (field_28_input_switch_id5 > 1u)
    {
        id5_value = SwitchStates_Get_466020(field_28_input_switch_id5);
    }
    if (id5_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_28_input_switch_id5 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    s32 id6_value = 0;
    if (field_2A_input_switch_id6 > 1u)
    {
        id6_value = SwitchStates_Get_466020(field_2A_input_switch_id6);
    }
    if (id6_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_2A_input_switch_id6 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    if ((id1_value || field_20_input_switch_id1 == 1) && (id2_value || field_22_input_switch_id2 == 1) && (id3_value || field_24_input_switch_id3 == 1) && (id4_value || field_26_input_switch_id4 == 1) && (id5_value || field_28_input_switch_id5 == 1) && (id6_value || field_2A_input_switch_id6 == 1))
    {
        field_34_last_switch_on_time = 0;
    }

    if (!id1_value && !id2_value && !id3_value && !id4_value && !id5_value && !id6_value && field_38_all_switches_on_or_off_time == 0)
    {
        field_38_all_switches_on_or_off_time = sGnFrame_5C1B84;
        if (field_38_all_switches_on_or_off_time - field_34_last_switch_on_time <= field_3C_on_off_delay)
        {
            SwitchStates_Do_Operation_465F00(field_2C_output_switch_id, field_2E_action);
            field_40_flags.Clear(Flags_40::eBit1_is_on);
            field_34_last_switch_on_time = 0;
        }
        // TODO: OG bug, why is this always reset ??
        field_38_all_switches_on_or_off_time = 0;
    }
}

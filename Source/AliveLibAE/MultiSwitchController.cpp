#include "stdafx.h"
#include "MultiSwitchController.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

BaseGameObject* MultiSwitchController::VDestructor(signed int flags)
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

MultiSwitchController* MultiSwitchController::ctor_4D60E0(Path_MultiSwitchController* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_40_flags_is_on &= ~1u;
    field_C_objectId = tlvInfo;
    field_30_tlvInfo = tlvInfo;
    SetVTable(this, 0x547AA8); // vTbl_MultiSwitchController_547AA8

    field_34_last_switch_on_time = 0;
    field_38_all_switches_on_or_off_time = 0;

    field_2C_id = pTlv->field_10_id;

    field_2E_operation = pTlv->field_12_operation;

    // Time after all switches on/off to activate id switch
    field_3C_delay = pTlv->field_14_delay;

    field_20_id1 = pTlv->field_16_id1;
    field_22_id2 = pTlv->field_18_id2;
    field_24_id3 = pTlv->field_1A_id3;
    field_26_id4 = pTlv->field_1C_id4;
    field_28_id5 = pTlv->field_1E_id5;
    field_2A_id6 = pTlv->field_20_id6;

    return this;
}

MultiSwitchController* MultiSwitchController::vdtor_4D61A0(signed int flags)
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
    if (gMap_5C3030.field_22_overlayID != gMap_5C3030.GetOverlayId_480710())
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

    if (field_40_flags_is_on & 1)
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
    const int id1_value = SwitchStates_Get_466020(field_20_id1);
    if (id1_value)
    {
        if (field_34_last_switch_on_time == 0 && field_20_id1 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const int id2_value = SwitchStates_Get_466020(field_22_id2);
    if (id2_value)
    {
        if (field_34_last_switch_on_time == 0 && field_22_id2 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const int id3_value = SwitchStates_Get_466020(field_24_id3);
    if (id3_value)
    {
        if (field_34_last_switch_on_time == 0 && field_24_id3 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const int id4_value = SwitchStates_Get_466020(field_26_id4);
    if (id4_value)
    {
        if (field_34_last_switch_on_time == 0 && field_26_id4 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }
    
    const int id5_value = SwitchStates_Get_466020(field_28_id5);
    if (id5_value)
    {
        if (field_34_last_switch_on_time == 0 && field_28_id5 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    const int id6_value = SwitchStates_Get_466020(field_2A_id6);
    if (id6_value)
    {
        if (field_34_last_switch_on_time == 0 && field_2A_id6 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    if ((!id1_value || field_20_id1 == 1) &&
        (!id2_value || field_22_id2 == 1) &&
        (!id3_value || field_24_id3 == 1) &&
        (!id4_value || field_26_id4 == 1) &&
        (!id5_value || field_28_id5 == 1) &&
        (!id6_value || field_2A_id6 == 1))
    {
        field_34_last_switch_on_time = 0;
    }

    if (id1_value &&
        id2_value &&
        id3_value &&
        id4_value &&
        id5_value &&
        id6_value &&
        field_38_all_switches_on_or_off_time == 0)
    {
        field_38_all_switches_on_or_off_time = sGnFrame_5C1B84;
        if (field_38_all_switches_on_or_off_time - field_34_last_switch_on_time <= field_3C_delay)
        {
            SwitchStates_Do_Operation_465F00(field_2C_id, field_2E_operation);
            field_40_flags_is_on |= 1u;
            field_34_last_switch_on_time = 0;
            field_38_all_switches_on_or_off_time = 0;
        }
    }
    else if (static_cast<int>(sGnFrame_5C1B84) - field_34_last_switch_on_time > field_3C_delay)
    {
        if (field_20_id1 > 1u)
        {
            SwitchStates_Set_465FF0(field_20_id1, 0);
        }

        if (field_22_id2 > 1u)
        {
            SwitchStates_Set_465FF0(field_22_id2, 0);
        }

        if (field_24_id3 > 1u)
        {
            SwitchStates_Set_465FF0(field_24_id3, 0);
        }

        if (field_26_id4 > 1u)
        {
            SwitchStates_Set_465FF0(field_26_id4, 0);
        }

        if (field_28_id5 > 1u)
        {
            SwitchStates_Set_465FF0(field_28_id5, 0);
        }

        if (field_2A_id6 > 1u)
        {
            SwitchStates_Set_465FF0(field_2A_id6, 0);
        }

        field_34_last_switch_on_time = 0;
    }
}

EXPORT void MultiSwitchController::WaitingForAllOff_4D65B0()
{
    int id1_value = 0;
    if (field_20_id1 > 1u)
    {
        id1_value = SwitchStates_Get_466020(field_20_id1);
    }
    if (id1_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_20_id1 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    int id2_value = 0;
    if (field_22_id2 > 1u)
    {
        id2_value = SwitchStates_Get_466020(field_22_id2);
    }
    if (id2_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_22_id2 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    int id3_value = 0;
    if (field_24_id3 > 1u)
    {
        id3_value = SwitchStates_Get_466020(field_24_id3);
    }
    if (id3_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_24_id3 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    int id4_value = 0;
    if (field_26_id4 > 1u)
    {
        id4_value = SwitchStates_Get_466020(field_26_id4);
    }
    if (id4_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_26_id4 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    int id5_value = 0;
    if (field_28_id5 > 1u)
    {
        id5_value = SwitchStates_Get_466020(field_28_id5);
    }
    if (id5_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_28_id5 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    int id6_value = 0;
    if (field_2A_id6 > 1u)
    {
        id6_value = SwitchStates_Get_466020(field_2A_id6);
    }
    if (id6_value == 0)
    {
        if (field_34_last_switch_on_time == 0 && field_2A_id6 > 1u)
        {
            field_34_last_switch_on_time = sGnFrame_5C1B84;
        }
    }

    if ((id1_value || field_20_id1 == 1) &&
        (id2_value || field_22_id2 == 1) &&
        (id3_value || field_24_id3 == 1) &&
        (id4_value || field_26_id4 == 1) &&
        (id5_value || field_28_id5 == 1) &&
        (id6_value || field_2A_id6 == 1))
    {
        field_34_last_switch_on_time = 0;
    }

    if (!id1_value &&
        !id2_value &&
        !id3_value &&
        !id4_value &&
        !id5_value &&
        !id6_value &&
        field_38_all_switches_on_or_off_time == 0)
    {
        field_38_all_switches_on_or_off_time = sGnFrame_5C1B84;
        if (field_38_all_switches_on_or_off_time - field_34_last_switch_on_time <= field_3C_delay)
        {
            SwitchStates_Do_Operation_465F00(field_2C_id, field_2E_operation);
            field_40_flags_is_on &= ~1u;
            field_34_last_switch_on_time = 0;
        }
        // TODO: OG bug, why is this always reset ??
        field_38_all_switches_on_or_off_time = 0;
    }
}

#include "stdafx.h"
#include "WheelSyncer.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

BaseGameObject* WheelSyncer::VDestructor(signed int flags)
{
    return vdtor_466120(flags);
}

void WheelSyncer::VUpdate()
{
    vUpdate_4661D0();
}

void WheelSyncer::VScreenChanged()
{
    vScreenChanged_466310();
}

WheelSyncer* WheelSyncer::ctor_466090(Path_WheelSyncer* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x545F20); // vTbl_WheelSyncer_545F20

    field_30_tlvInfo = tlvInfo;
    field_20_id1 = pTlv->field_10_id1;
    field_22_id2 = pTlv->field_12_id2;
    field_24_trigger_id = pTlv->field_14_trigger_id;
    field_2E_action = pTlv->field_16_action;
    field_26_id3 = pTlv->field_18_id3;
    field_28_id4 = pTlv->field_1A_id4;
    field_2A_id5 = pTlv->field_1C_id5;
    field_2C_id6 = pTlv->field_1E_id6;
    return this;
}

void WheelSyncer::vUpdate_4661D0()
{
    const int state1 = SwitchStates_Get_466020(field_20_id1);
    const int state2 = SwitchStates_Get_466020(field_22_id2);
    const int state3 = SwitchStates_Get_466020(field_26_id3);
    const int state4 = SwitchStates_Get_466020(field_28_id4);
    const int state5 = SwitchStates_Get_466020(field_2A_id5);
    const int state6 = SwitchStates_Get_466020(field_2C_id6);

    int switchValue = 0;
    switch (field_2E_action)
    {
    case 0: // all on
        if (!state1 || !state2 || !state3 || !state4 || !state5 || !state6)
        {
            switchValue = 0;
        }
        else
        {
            switchValue = 1;
        }
        break;

    case 1: // off
        if (!state1 || state2)
        {
            switchValue = 0;
        }
        else
        {
            switchValue = 1;
        }
        break;

    case 2:  // toggle
        if (state1 || state2)
        {
            switchValue = 1;
        }
        else
        {
            switchValue = 0;
        }
        break;

    case 3:
        if (state1 || !state2)
        {
            switchValue = 1;
        }
        else
        {
            switchValue = 0;
        }
        break;

    default:
        switchValue = state5;
        break;
    }

    SwitchStates_Set_465FF0(field_24_trigger_id, static_cast<char>(switchValue));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

WheelSyncer* WheelSyncer::vdtor_466120(signed int flags)
{
    dtor_466150();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void WheelSyncer::dtor_466150()
{
    SetVTable(this, 0x545F20); // vTbl_WheelSyncer_545F20
    Path::TLV_Reset_4DB8E0(field_30_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

void WheelSyncer::vScreenChanged_466310()
{
    if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

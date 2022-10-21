#include "stdafx.h"
#include "WheelSyncer.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

BaseGameObject* WheelSyncer::VDestructor(s32 flags)
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

WheelSyncer* WheelSyncer::ctor_466090(Path_WheelSyncer* pTlv, u32 tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x545F20); // vTbl_WheelSyncer_545F20

    field_30_tlvInfo = tlvInfo;
    field_20_input_switch_id1 = pTlv->field_10_input_switch_id1;
    field_22_input_switch_id2 = pTlv->field_12_input_switch_id2;
    field_24_trigger_id = pTlv->field_14_output_switch_id;
    field_2E_output_requirement = pTlv->field_16_output_requirement;
    field_26_input_switch_id3 = pTlv->field_18_input_switch_id3;
    field_28_input_switch_id4 = pTlv->field_1A_input_switch_id4;
    field_2A_input_switch_id5 = pTlv->field_1C_input_switch_id5;
    field_2C_input_switch_id6 = pTlv->field_1E_input_switch_id6;
    return this;
}

void WheelSyncer::vUpdate_4661D0()
{
    const s32 state1 = SwitchStates_Get_466020(field_20_input_switch_id1);
    const s32 state2 = SwitchStates_Get_466020(field_22_input_switch_id2);
    const s32 state3 = SwitchStates_Get_466020(field_26_input_switch_id3);
    const s32 state4 = SwitchStates_Get_466020(field_28_input_switch_id4);
    const s32 state5 = SwitchStates_Get_466020(field_2A_input_switch_id5);
    const s32 state6 = SwitchStates_Get_466020(field_2C_input_switch_id6);

    s32 switchValue = 0;

    switch (field_2E_output_requirement)
    {
        case WheelSyncerOutputRequirement::eAllOn_0:
            if (!state1 || !state2 || !state3 || !state4 || !state5 || !state6)
            {
                switchValue = 0;
            }
            else
            {
                switchValue = 1;
            }
            break;

        case WheelSyncerOutputRequirement::e1OnAnd2Off_1:
            if (!state1 || state2)
            {
                switchValue = 0;
            }
            else
            {
                switchValue = 1;
            }
            break;

        case WheelSyncerOutputRequirement::e1Or2On_2:
            if (state1 || state2)
            {
                switchValue = 1;
            }
            else
            {
                switchValue = 0;
            }
            break;

        case WheelSyncerOutputRequirement::e1OnOr2Off_3:
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

    SwitchStates_Set_465FF0(field_24_trigger_id, static_cast<s8>(switchValue));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

WheelSyncer* WheelSyncer::vdtor_466120(s32 flags)
{
    dtor_466150();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
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
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

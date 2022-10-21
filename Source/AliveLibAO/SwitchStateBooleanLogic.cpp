#include "stdafx_ao.h"
#include "Function.hpp"
#include "SwitchStateBooleanLogic.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Events.hpp"

namespace AO {

void SwitchStateBooleanLogic::VUpdate()
{
    VUpdate_436B60();
}

BaseGameObject* SwitchStateBooleanLogic::Vdtor_436C60(s32 flags)
{
    dtor_436B00();

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }

    return this;
}

BaseGameObject* SwitchStateBooleanLogic::VDestructor(s32 flags)
{
    return Vdtor_436C60(flags);
}

void SwitchStateBooleanLogic::VScreenChanged_436C40()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void SwitchStateBooleanLogic::VScreenChanged()
{
    VScreenChanged_436C40();
}

BaseGameObject* SwitchStateBooleanLogic::dtor_436B00()
{
    SetVTable(this, 0x4BB7B8);
    gMap_507BA8.TLV_Reset_446870(field_18_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

SwitchStateBooleanLogic* SwitchStateBooleanLogic::ctor_436AB0(Path_SwitchStateBooleanLogic* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB7B8);

    field_18_tlvInfo = tlvInfo;

    field_10_input_1 = pTlv->field_18_input1;
    field_12_input_2 = pTlv->field_1A_input2;

    field_14_output = pTlv->field_1C_output;

    field_16_operator = pTlv->field_1E_operator;

    return this;
}


void SwitchStateBooleanLogic::VUpdate_436B60()
{
    const s16 id1_value = SwitchStates_Get(field_10_input_1);
    const s16 id2_value = SwitchStates_Get(field_12_input_2);

    s8 id_value = 0;
    switch (field_16_operator)
    {
        case BooleanOperatorType::eAllOn_0:
            if (id1_value && id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        case BooleanOperatorType::e1OnAnd2Off_1:
            if (id1_value && !id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        case BooleanOperatorType::e1Or2On_2:
            if (id1_value || id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        case BooleanOperatorType::e1OnOr2Off_3:
            if (id1_value || !id2_value)
            {
                id_value = 1;
            }
            else
            {
                id_value = 0;
            }
            break;

        default:
            id_value = 0;
            break;
    }

    SwitchStates_Set(field_14_output, id_value);

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

} // namespace AO

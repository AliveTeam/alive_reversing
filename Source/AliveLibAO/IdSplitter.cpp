#include "stdafx_ao.h"
#include "Function.hpp"
#include "IdSplitter.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

namespace AO {

void IdSplitter::VUpdate()
{
    VUpdate_479C40();
}

void IdSplitter::VScreenChanged_479DB0()
{
    if (field_12_state == State::eState_0 ||
        field_12_state == State::eState_2 ||
        gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void IdSplitter::VScreenChanged()
{
    VScreenChanged_479DB0();
}

IdSplitter* IdSplitter::Vdtor_479E00(signed int flags)
{
    dtor_479BE0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* IdSplitter::VDestructor(signed int flags)
{
    return Vdtor_479E00(flags);
}

BaseGameObject* IdSplitter::dtor_479BE0()
{
    SetVTable(this, 0x4BCE20);
    gMap_507BA8.TLV_Reset_446870(field_1C_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

IdSplitter* IdSplitter::ctor_479B40(Path_IdSplitter* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    field_1C_tlvInfo = tlvInfo;
    SetVTable(this, 0x4BCE20);

    field_24_delay = pTlv->field_1A_delay;

    field_14_ids[0] = pTlv->field_1C_ids[0];
    field_14_ids[1] = pTlv->field_1C_ids[1];
    field_14_ids[2] = pTlv->field_1C_ids[2];
    field_14_ids[3] = pTlv->field_1C_ids[3];

    field_10_source_id = pTlv->field_18_source_id;
    field_12_state = State::eState_0;

    if (field_10_source_id)
    {
        field_28_source_switch_value = SwitchStates_Get(field_10_source_id);
    }
    else
    {
        field_28_source_switch_value = 0;
    }

    return this;
}

void IdSplitter::VUpdate_479C40()
{
    switch (field_12_state)
    {
    case State::eState_0:
        // If the value changes from what we first saw...
        if (SwitchStates_Get(field_10_source_id) != field_28_source_switch_value)
        {
            field_12_state = State::eState_1;
            field_20_delay_timer = gnFrameCount_507670 + field_24_delay;
        }
        break;

    case State::eState_1:
        if (field_20_delay_timer <= static_cast<int>(gnFrameCount_507670))
        {
            for (const auto& id : field_14_ids)
            {
                if (id > 1)
                {
                    SwitchStates_Do_Operation_436A10(id, SwitchOp::eToggle_2);
                }
            }
            field_12_state = State::eState_2;
        }
        break;

    case State::eState_2:
        if (SwitchStates_Get(field_10_source_id) == field_28_source_switch_value)
        {
            field_12_state = State::eState_3;
            field_20_delay_timer = gnFrameCount_507670 + field_24_delay;
        }
        break;

    case State::eState_3:
        if (field_20_delay_timer <= static_cast<int>(gnFrameCount_507670))
        {
            for (const auto& id : field_14_ids)
            {
                if (id > 1)
                {
                    SwitchStates_Do_Operation_436A10(id, SwitchOp::eToggle_2);
                }
            }
            field_12_state = State::eState_0;
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

}


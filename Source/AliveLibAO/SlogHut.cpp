#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlogHut.hpp"
#include "stdlib.hpp"
#include "SnoozeParticle.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

namespace AO {

void SlogHut::VScreenChanged()
{
    VScreenChanged_472E20();
}

SlogHut* SlogHut::Vdtor_472E30(signed int flags)
{
    dtor_472CF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* SlogHut::VDestructor(signed int flags)
{
    return Vdtor_472E30(flags);
}

BaseGameObject* SlogHut::dtor_472CF0()
{
    SetVTable(this, 0x4BCBB0);
    gMap_507BA8.TLV_Reset_446870(field_1C_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

SlogHut* SlogHut::ctor_472C80(Path_SlogHut* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BCBB0);

    field_10_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_14_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_1C_tlvInfo = tlvInfo;

    if (pTlv->field_18_scale == 1)
    {
        field_18_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_18_scale = FP_FromInteger(1);
    }

    field_20_switch_id = pTlv->field_1A_switch_id;
    field_28_z_delay = pTlv->field_1C_z_delay;
    field_24_timer = 0;

    return this;
}

void SlogHut::VScreenChanged_472E20()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}


void SlogHut::VUpdate()
{
    VUpdate_472D50();
}

void SlogHut::VUpdate_472D50()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!SwitchStates_Get(field_20_switch_id) && static_cast<int>(gnFrameCount_507670) > field_24_timer)
    {
        auto pSnoozeParticle = ao_new<SnoozeParticle>();
        if (pSnoozeParticle)
        {
            pSnoozeParticle->ctor_464320(
                field_10_xpos,
                field_14_ypos,
                field_18_scale != FP_FromInteger(1) ? 20 : 39,
                field_18_scale);
        }
        field_24_timer = gnFrameCount_507670 + field_28_z_delay;
    }
}

}

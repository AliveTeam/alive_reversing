#include "stdafx.h"
#include "SlogHut.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SnoozeParticle.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

SlogHut* SlogHut::ctor_4C4070(Path_SlogHut* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54755C);

    field_20_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_24_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_2C_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_28_scale = FP_FromDouble(0.5);
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_28_scale = FP_FromInteger(1);
    }

    field_30_switch_id = pTlv->field_12_switch_id;
    field_38_z_delay = pTlv->field_14_z_delay;
    field_34_z_timer = 0;

    return this;
}

BaseGameObject* SlogHut::VDestructor(signed int flags)
{
    return vdtor_4C4100(flags);
}

void SlogHut::VUpdate()
{
    vUpdate_4C41B0();
}

void SlogHut::VScreenChanged()
{
    vScreenChanged_4C4280();
}

void SlogHut::dtor_4C4130()
{
    SetVTable(this, 0x54755C);
    Path::TLV_Reset_4DB8E0(field_2C_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

SlogHut* SlogHut::vdtor_4C4100(signed int flags)
{
    dtor_4C4130();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void SlogHut::vScreenChanged_4C4280()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

void SlogHut::vUpdate_4C41B0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (!SwitchStates_Get_466020(field_30_switch_id) && static_cast<int>(sGnFrame_5C1B84) > field_34_z_timer)
    {
        auto pSnoozeParticle = alive_new<SnoozeParticle>();
        if (pSnoozeParticle)
        {
            pSnoozeParticle->ctor_4B06F0(field_20_xpos, field_24_ypos, 39, field_28_scale);
        }

        field_34_z_timer = sGnFrame_5C1B84 + field_38_z_delay;
    }
}


#include "stdafx.h"
#include "SlogHut.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SnoozeParticle.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"

SlogHut* SlogHut::ctor_4C4070(Path_SlogHut* pTlv, s32 tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54755C);

    field_20_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_24_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_2C_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_28_scale = FP_FromDouble(0.5);
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        field_28_scale = FP_FromInteger(1);
    }

    field_30_switch_id = pTlv->field_12_switch_id;
    field_38_Zzz_delay = pTlv->field_14_Zzz_delay;
    field_34_Zzz_timer = 0;

    return this;
}

BaseGameObject* SlogHut::VDestructor(s32 flags)
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

SlogHut* SlogHut::vdtor_4C4100(s32 flags)
{
    dtor_4C4130();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void SlogHut::vScreenChanged_4C4280()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SlogHut::vUpdate_4C41B0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!SwitchStates_Get_466020(field_30_switch_id) && static_cast<s32>(sGnFrame_5C1B84) > field_34_Zzz_timer)
    {
        auto pSnoozeParticle = ae_new<SnoozeParticle>();
        if (pSnoozeParticle)
        {
            pSnoozeParticle->ctor_4B06F0(field_20_xpos, field_24_ypos, Layer::eLayer_Above_FG1_39, field_28_scale);
        }

        field_34_Zzz_timer = sGnFrame_5C1B84 + field_38_Zzz_delay;
    }
}

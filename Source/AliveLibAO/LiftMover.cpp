#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftMover.hpp"
#include "stdlib.hpp"

START_NS_AO

LiftMover* LiftMover::ctor_4054E0(Path_LiftMover* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA158);

    field_14_tlvInfo = tlvInfo;
    field_18_pLiftPoint = nullptr;
    field_4_typeId = Types::eLiftMover_7;

    field_10_enabled_by_switch_id = pTlv->field_18_switch_id;
    field_12_target_lift_point_id = pTlv->field_1A_lift_id;

    if (pTlv->field_1C_direction)
    {
        field_1C_speed = FP_FromInteger(-4);
    }
    else
    {
        field_1C_speed = FP_FromInteger(4);
    }

    field_20_state = 0;

    return this;
}

BaseGameObject* LiftMover::dtor_405550()
{
    SetVTable(this, 0x4BA158);
    if (field_18_pLiftPoint)
    {
        field_18_pLiftPoint->field_C_refCount--;
        field_18_pLiftPoint = nullptr;
    }
    gMap_507BA8.TLV_Reset_446870(field_14_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

BaseGameObject* LiftMover::VDestructor(signed int flags)
{
    return Vdtor_405A50(flags);
}

EXPORT LiftMover* LiftMover::Vdtor_405A50(signed int flags)
{
    dtor_405550();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

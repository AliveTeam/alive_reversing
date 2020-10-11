#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

START_NS_AO

LiftPoint* LiftPoint::ctor_434710(unsigned __int8* /*pTlv*/, Map* /*pPath*/, int /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
    return this;
}

void LiftPoint::Move_435740(FP xSpeed, FP ySpeed, int /*not_used*/)
{
    field_B4_velx = xSpeed;
    field_B8_vely = ySpeed;
    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
    }
}

bool LiftPoint::OnTopFloor() const
{
    return field_27A_flags.Get(Flags::eBit2_bTopFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnBottomFloor() const
{
    return field_27A_flags.Get(Flags::eBit4_bBottomFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnMiddleFloor() const
{
    return field_27A_flags.Get(Flags::eBit3_bMiddleFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnAnyFloor() const
{
    return OnBottomFloor() || OnTopFloor() || OnMiddleFloor();
}

void LiftPoint::VUpdate()
{
    VUpdate_434D10();
}

void LiftPoint::VUpdate_434D10()
{
    NOT_IMPLEMENTED();
}

void LiftPoint::VRender(int** pOrderingTable)
{
    VRender_435780(pOrderingTable);
}

void LiftPoint::VRender_435780(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void LiftPoint::VScreenChanged()
{
    VScreenChanged_435CC0();
}

void LiftPoint::VScreenChanged_435CC0()
{
    if (!field_27A_flags.Get(Flags::eBit5))
    {
        sub_435AE0(0, -1);
    }

    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void LiftPoint::sub_435AE0(int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
}

EXPORT BaseGameObject* AO::LiftPoint::dtor_4355E0()
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* LiftPoint::VDestructor(signed int flags)
{
    return Vdtor_435D10(flags);
}

LiftPoint* LiftPoint::Vdtor_435D10(signed int flags)
{
    dtor_4355E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

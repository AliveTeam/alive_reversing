#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftPoint.hpp"

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

END_NS_AO

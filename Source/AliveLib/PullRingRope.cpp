#include "stdafx.h"
#include "PullRingRope.hpp"
#include "Function.hpp"

BaseGameObject* PullRingRope::VDestructor(signed int flags)
{
    return vdtor_49B630(flags);
}

void PullRingRope::VUpdate()
{
    vUpdate_49B720();
}

void PullRingRope::VScreenChanged()
{
    vScreenChanged_49BCB0();
}

__int16 PullRingRope::VPull_49BBD0(BaseGameObject* a2)
{
    return vPull_49BBD0(a2);
}

BOOL PullRingRope::Vsub_49BC90()
{
    return vsub_49BC90();
}

void PullRingRope::Vsub_49B610()
{
    return vsub_49B610();
}

PullRingRope* PullRingRope::vdtor_49B630(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

void PullRingRope::vUpdate_49B720()
{
    NOT_IMPLEMENTED();
}

void PullRingRope::vScreenChanged_49BCB0()
{
    NOT_IMPLEMENTED();
}

__int16 PullRingRope::vPull_49BBD0(BaseGameObject* /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BOOL PullRingRope::vsub_49BC90()
{
    return field_100 != 1;
}

void PullRingRope::vsub_49B610()
{
    field_10C |= 1u;
}

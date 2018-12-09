#include "stdafx.h"
#include "Blood.hpp"
#include "Function.hpp"

BaseGameObject* Blood::VDestructor(signed int flags)
{
    return vDtor_40F5A0(flags);
}

void Blood::VUpdate()
{
    vUpdate_40F650();
}

void Blood::VRender(int** pOrderingTable)
{
    vRender_40F780(pOrderingTable);
}

void Blood::VScreenChanged()
{
    vScreenChanged_40FAD0();
}

Blood* Blood::ctor_40F0B0(FP /*xpos*/, FP /*ypos*/, FP /*xOff*/, FP /*yOff*/, FP /*scale*/, __int16 /*count*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* Blood::vDtor_40F5A0(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

void Blood::vUpdate_40F650()
{
    NOT_IMPLEMENTED();
}

void Blood::vRender_40F780(int** /*pOrderingTable*/)
{
    NOT_IMPLEMENTED();
}

void Blood::vScreenChanged_40FAD0()
{
    NOT_IMPLEMENTED();
}

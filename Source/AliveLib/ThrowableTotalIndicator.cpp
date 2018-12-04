#include "stdafx.h"
#include "ThrowableTotalIndicator.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

EXPORT ThrowableTotalIndicator* ThrowableTotalIndicator::ctor_431CB0(FP /*xpos*/, FP /*ypos*/, __int16 /*layer*/, FP /*scale*/, __int16 /*count*/, __int16 /*bUnknown*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* ThrowableTotalIndicator::VDestructor(signed int flags)
{
    return vdtor_431DE0(flags);
}

EXPORT void ThrowableTotalIndicator::dtor_431E10()
{
    NOT_IMPLEMENTED();
    BaseGameObject_dtor_4DBEC0();
}

EXPORT BaseGameObject* ThrowableTotalIndicator::vdtor_431DE0(signed int flags)
{
    dtor_431E10();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

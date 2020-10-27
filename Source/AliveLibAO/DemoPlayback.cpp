#include "stdafx_ao.h"
#include "Function.hpp"
#include "DemoPlayback.hpp"
#include "stdlib.hpp"

START_NS_AO

EXPORT DemoPlayback* DemoPlayback::ctor_4517B0(BYTE**, __int16)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

BaseGameObject* DemoPlayback::VDestructor(signed int flags)
{
    return Vdtor_451F50(flags);
}

BaseGameObject* DemoPlayback::dtor_4518D0()
{
    NOT_IMPLEMENTED();
    return this;
}

DemoPlayback* DemoPlayback::Vdtor_451F50(signed int flags)
{
    dtor_4518D0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

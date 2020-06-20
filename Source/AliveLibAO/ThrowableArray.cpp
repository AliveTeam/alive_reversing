#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableArray.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x50E26C, ThrowableArray*, gpThrowableArray_50E26C, nullptr);

BaseGameObject* ThrowableArray::VDestructor(signed int flags)
{
    return Vdtor_454690(flags);
}

void ThrowableArray::VUpdate()
{
    VUpdate_4542B0();
}

void ThrowableArray::VRender(int** /*pOrderingTable*/)
{
    //Nothing to do here
}

void ThrowableArray::VScreenChanged()
{
    vScreenChange_454300();
}

BaseGameObject* ThrowableArray::Vdtor_454690(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void ThrowableArray::VUpdate_4542B0()
{
    NOT_IMPLEMENTED();
}

void ThrowableArray::vScreenChange_454300()
{
    NOT_IMPLEMENTED();
}

void ThrowableArray::sub_4540D0(char)
{
    NOT_IMPLEMENTED();
}

END_NS_AO;

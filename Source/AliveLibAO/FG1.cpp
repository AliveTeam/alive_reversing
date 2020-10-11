#include "stdafx_ao.h"
#include "FG1.hpp"
#include "Function.hpp"

START_NS_AO

FG1* FG1::ctor_4539C0(unsigned __int8** /*ppRes*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* FG1::VDestructor(signed int flags)
{
    return Vdtor_453E90(flags);
}

void FG1::VUpdate()
{
    // Empty
}

void FG1::VScreenChanged()
{
    VScreenChanged_453DE0();
}

void FG1::VScreenChanged_453DE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void FG1::VRender(int** ppOt)
{
    VRender_453D50(ppOt);
}

void FG1::VRender_453D50(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

FG1* FG1::Vdtor_453E90(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

END_NS_AO

#pragma once

#include "Function.hpp"
#include "Abe.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x5C1B68, Abe*, sActiveHero_5C1B68, nullptr);

BaseGameObject* Abe::VDestructor(signed int flags)
{
    return vdtor_422A70(flags);
}

void Abe::VUpdate()
{
    vUpdate_41FDB0();
}

void Abe::VRender(int** pOrderingTable)
{
    vRender_420F30(pOrderingTable);
}

void Abe::VScreenChanged()
{
    vScreenChanged_422640();
}

Abe* Abe::ctor_420770(int /*frameTableOffset*/, int /*a3*/, int /*a4*/, int /*a5*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* Abe::vdtor_422A70(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
}

void Abe::vUpdate_41FDB0()
{
    NOT_IMPLEMENTED();
}

void Abe::vRender_420F30(int** /*pOrderingTable*/)
{
    NOT_IMPLEMENTED();
}

void Abe::vScreenChanged_422640()
{
    NOT_IMPLEMENTED();
}

END_NS_AO;

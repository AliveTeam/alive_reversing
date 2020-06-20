#include "stdafx_ao.h"
#include "Function.hpp"
#include "Elum.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x507680, Elum*, gElum_507680, nullptr);

BaseGameObject* Elum::VDestructor(signed int flags)
{
    return Vdtor_411710(flags);
}

void Elum::VUpdate()
{
    VUpdate_4102A0();
}

void Elum::VRender(int** pOrderingTable)
{
    VRender_410E40(pOrderingTable);
}

void Elum::VScreenChanged()
{
    vScreenChange_411340();
}

BaseGameObject* Elum::Vdtor_411710(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void Elum::VUpdate_4102A0()
{
    NOT_IMPLEMENTED();
}

void Elum::VRender_410E40(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

void Elum::vScreenChange_411340()
{
    NOT_IMPLEMENTED();
}

void Elum::Spawn_410E90(TlvItemInfoUnion)
{
    NOT_IMPLEMENTED();
}

END_NS_AO;

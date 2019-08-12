#include "stdafx.h"
#include "ParamiteWebLine.hpp"
#include "Function.hpp"

void ParamiteWebLine::Wobble_4E29D0(short /*ypos*/)
{
    NOT_IMPLEMENTED();
}

void ParamiteWebLine::VScreenChanged()
{
    return vScreenChanged_4E2BC0();
}

void ParamiteWebLine::vScreenChanged_4E2BC0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}


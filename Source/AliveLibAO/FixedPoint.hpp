#pragma once

#include "FixedPoint_common.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

EXPORT unsigned int CC Math_FixedPoint_Multiply_451040(signed int op1, signed int op2);

inline FixedPoint operator*(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = Math_FixedPoint_Multiply_451040(lhs.fpValue, rhs.fpValue);
    return f;
}

END_NS_AO

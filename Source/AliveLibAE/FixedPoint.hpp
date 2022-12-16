#pragma once

#include "Math.hpp"
#include "../relive_lib/FixedPoint_common.hpp"

inline FixedPoint& operator*=(FixedPoint& lhs, const FixedPoint& rhs)
{
    lhs.fpValue = Math_FixedPoint_Multiply(lhs.fpValue, rhs.fpValue);
    return lhs;
}

inline FixedPoint& operator/=(FixedPoint& lhs, const FixedPoint& rhs)
{
    lhs.fpValue = Math_FixedPoint_Divide(lhs.fpValue, rhs.fpValue);
    return lhs;
}

inline FixedPoint operator*(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = Math_FixedPoint_Multiply(lhs.fpValue, rhs.fpValue);
    return f;
}

inline FixedPoint operator/(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = Math_FixedPoint_Divide(lhs.fpValue, rhs.fpValue);
    return f;
}


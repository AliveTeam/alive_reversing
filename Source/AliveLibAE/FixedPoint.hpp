#pragma once

#include "FunctionFwd.hpp"
#include "Math.hpp"
#include "FixedPoint_common.hpp"

void FixedPoint_ForceLink();


inline FixedPoint& operator*=(FixedPoint& lhs, const FixedPoint& rhs)
{
    lhs.fpValue = Math_FixedPoint_Multiply_496C50(lhs.fpValue, rhs.fpValue);
    return lhs;
}

inline FixedPoint& operator/=(FixedPoint& lhs, const FixedPoint& rhs)
{
    lhs.fpValue = Math_FixedPoint_Divide_496B70(lhs.fpValue, rhs.fpValue);
    return lhs;
}

inline FixedPoint operator*(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = Math_FixedPoint_Multiply_496C50(lhs.fpValue, rhs.fpValue);
    return f;
}

inline FixedPoint operator/(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = Math_FixedPoint_Divide_496B70(lhs.fpValue, rhs.fpValue);
    return f;
}

EXPORT int CC Math_SquareRoot_Int_496E70(int value);
EXPORT FP CC Math_SquareRoot_FP_496E90(FP value);

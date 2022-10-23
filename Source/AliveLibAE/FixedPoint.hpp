#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Math.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

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

s32 Math_SquareRoot_Int_496E70(s32 value);
FP Math_SquareRoot_FP(FP value);

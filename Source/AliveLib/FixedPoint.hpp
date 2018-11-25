#pragma once

#include "FunctionFwd.hpp"
#include "stdafx.h"
#include "Math.hpp"

void FixedPoint_ForceLink();

struct FixedPoint
{
    int fpValue;
};
static_assert(std::is_pod<FixedPoint>::value, "FixedPoint must be a POD type, otherwise things are going to randomly break so STAP!");

inline FixedPoint& operator+=(FixedPoint& lhs, const FixedPoint& rhs)
{
    lhs.fpValue += rhs.fpValue;
    return lhs;
}

inline FixedPoint& operator-=(FixedPoint& lhs, const FixedPoint& rhs)
{
    lhs.fpValue -= rhs.fpValue;
    return lhs;
}

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

inline FixedPoint operator+(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = lhs.fpValue + rhs.fpValue;
    return f;
}

inline FixedPoint operator-(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f = {};
    f.fpValue = lhs.fpValue - rhs.fpValue;
    return f;
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

inline FixedPoint operator-(const FixedPoint& value)
{
    FixedPoint f = {};
    f.fpValue = -value.fpValue;
    return f;
}

inline bool operator < (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue < rhs.fpValue;
}

inline bool operator <= (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue <= rhs.fpValue;
}

inline bool operator > (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue > rhs.fpValue;
}

inline bool operator >= (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue >= rhs.fpValue;
}

inline bool operator == (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue == rhs.fpValue;
}

inline bool operator != (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue != rhs.fpValue;
}

inline FixedPoint FP_FromDouble(double v)
{
    FixedPoint f = {};
    f.fpValue = (static_cast<signed int>(v * 0x10000));
    return f;
}

inline FixedPoint FP_FromRaw(signed int v)
{
    FixedPoint f = {};
    f.fpValue = v;
    return f;
}

template<class T>
inline FixedPoint FP_FromInteger(T v)
{
    FixedPoint f = {};
    f.fpValue = v << 16;
    return f;
}

inline short FP_GetExponent(const FixedPoint& fp)
{
    return static_cast<short>(fp.fpValue / 0x10000);
}

inline double FP_GetDouble(FixedPoint& fp)
{
    return static_cast<double>(fp.fpValue) / 0x10000;
}

inline FixedPoint FP_NoFractional(const FixedPoint& fp)
{
    FixedPoint nfp = fp;
    nfp.fpValue &= 0xFFFF0000;
    return nfp;
}

using FP = FixedPoint;

ALIVE_ASSERT_SIZEOF(FP, 0x4);

struct FP_Point
{
    FP field_0_x;
    FP field_4_y;
};
ALIVE_ASSERT_SIZEOF(FP_Point, 8);

#pragma once

#include "FunctionFwd.hpp"
#include "stdafx.h"
#include "Math.hpp"

void FixedPoint_ForceLink();

struct FixedPoint
{
    FixedPoint();

    explicit FixedPoint(int v);

    explicit FixedPoint(double v);

    FixedPoint& operator+=(const FixedPoint& other);

    FixedPoint& operator-=(const FixedPoint& other);

    FixedPoint& operator*=(const FixedPoint& other);

    FixedPoint& operator/=(const FixedPoint& other);

    short GetExponent() const;
    double GetDouble() const;

    // Avoid using this. Directly writes to fp value
    void SetRaw(signed int rawFp);
public:
    int fpValue;
};

inline FixedPoint operator+(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f;
    f.fpValue = lhs.fpValue + rhs.fpValue;
    return f;
}

inline FixedPoint operator-(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f;
    f.fpValue = lhs.fpValue - rhs.fpValue;
    return f;
}

inline FixedPoint operator*(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f;
    f.fpValue = Math_FixedPoint_Multiply_496C50(lhs.fpValue, rhs.fpValue);
    return f;
}

inline FixedPoint operator/(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f;
    f.fpValue = Math_FixedPoint_Divide_496B70(lhs.fpValue, rhs.fpValue);
    return f;
}

inline bool operator < (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue < rhs.fpValue;
}

inline bool operator >= (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue >= rhs.fpValue;
}

inline FixedPoint FP_FromDouble(double v)
{
    FixedPoint f;
    f.fpValue = static_cast<int>(v * 0x10000);
    return f;
}

template<class T>
inline FixedPoint FP_FromInteger(T v)
{
    FixedPoint f;
    f.fpValue = v << 16;
    return f;
}

inline bool operator == (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue == rhs.fpValue;
}

inline bool operator != (const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue != rhs.fpValue;
}

using FP = FixedPoint;

ALIVE_ASSERT_SIZEOF(FP, 0x4);

struct FP_Point
{
    FP field_0_x;
    FP field_4_y;
};
ALIVE_ASSERT_SIZEOF(FP_Point, 8);

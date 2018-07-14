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

    int GetExponent() const;
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

inline FixedPoint operator*(const FixedPoint& lhs, const FixedPoint& rhs)
{
    FixedPoint f;
    f.fpValue = Math_FixedPoint_Multiply_496C50(lhs.fpValue, rhs.fpValue);
    return f;
}

inline FixedPoint FP_FromDouble(double v)
{
    FixedPoint f;
    f.fpValue = static_cast<int>(v * 0x10000);
    return f;
}

using FP = FixedPoint;

ALIVE_ASSERT_SIZEOF(FP, 0x4);

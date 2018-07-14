#pragma once

#include "FunctionFwd.hpp"
#include "stdafx.h"
#include "Math.hpp"

void FixedPoint_ForceLink();

template <typename T>
struct FixedPoint
{
    FixedPoint()
    {
        fpValue = 0;
    }

    FixedPoint(T v)
    {
        fpValue = v << (sizeof(T) * 4);
    }

    FixedPoint(double v)
    {
        fpValue = static_cast<T>(0x10000 * v);
    }

    inline FixedPoint& operator+=(const FixedPoint& other)
    {
        this->fpValue += other.fpValue;
        return *this;
    }
    inline FixedPoint& operator-=(const FixedPoint& other)
    {
        this->fpValue -= other.fpValue;
        return *this;
    }
    inline FixedPoint& operator*=(const FixedPoint& other)
    {
        this->fpValue = Math_FixedPoint_Multiply_496C50(this->fpValue, other.fpValue);
        return *this;
    }
    inline FixedPoint& operator/=(const FixedPoint& other)
    {
        this->fpValue = Math_FixedPoint_Divide_496B70(this->fpValue, other.fpValue);
        return *this;
    }

    // Type Conversions
    inline explicit operator T()
    {
        return fpValue >> (sizeof(T) * 4);
    }
    inline operator double()
    {
        return static_cast<double>(fpValue) / 0x10000;
    }

    // Avoid using this. Directly writes to fp value
    inline void SetRaw(signed int rawFp)
    {
        fpValue = rawFp;
    }
public:
    T fpValue;
};


typedef FixedPoint<signed int> FP;

ALIVE_ASSERT_SIZEOF(FP, 0x4);
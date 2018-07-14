#pragma once

#include "FunctionFwd.hpp"
#include "stdafx.h"
#include "Math.hpp"

void FixedPoint_ForceLink();

struct FixedPoint
{
    FixedPoint(signed int v)
    {
        fpValue = v << 16;
    }

    FixedPoint(double v)
    {
        fpValue = static_cast<signed int>(0x10000 * v);
    }

    inline FixedPoint& operator+=(const FixedPoint& other);
    inline FixedPoint& operator-=(const FixedPoint& other);
    inline FixedPoint& operator*=(const FixedPoint& other);
    inline FixedPoint& operator/=(const FixedPoint& other);

    inline int operator*(const FixedPoint& other)
    {
        return 0;
    }

    // Type Conversions
    inline explicit operator signed int();
    inline operator double();

    // Avoid using this. Directly writes to fp value
    inline void SetRaw(signed int rawFp);
public:
    signed int fpValue;
};
ALIVE_ASSERT_SIZEOF(FixedPoint, 0x4);

std::ostream& operator<<(std::ostream &strm, const FixedPoint &other);

typedef FixedPoint FP;
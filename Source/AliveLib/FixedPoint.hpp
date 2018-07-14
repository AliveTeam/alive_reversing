#pragma once

#include "FunctionFwd.hpp"
#include "stdafx.h"
#include "Math.hpp"

void FixedPoint_ForceLink();

struct FixedPoint
{
    FixedPoint(int v)
    {
        fpValue = v << 16;
    }

    FixedPoint(double v)
    {
        fpValue = static_cast<int>(0x10000 * v);
    }

    inline FixedPoint& operator+=(const FixedPoint& other);
    inline FixedPoint& operator-=(const FixedPoint& other);
    inline FixedPoint& operator*=(const FixedPoint& other);
    inline FixedPoint& operator/=(const FixedPoint& other);
public:
    int fpValue;
};

std::ostream& operator<<(std::ostream &strm, const FixedPoint &other);

typedef FixedPoint FP;
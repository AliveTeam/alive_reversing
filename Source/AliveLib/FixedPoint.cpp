#include "stdafx.h"
#include "FixedPoint.hpp"
#include <gtest/gtest.h>

void FixedPoint_ForceLink()
{
    FP fp(0);
    fp += FP(5);

    int f = fp.GetExponent();
    ASSERT_EQ(f, 5);

    FixedPoint one = FP_FromDouble(1.0);
    ASSERT_EQ(one.GetExponent(), 1);
    ASSERT_EQ(one.fpValue, 0x10000);

    FixedPoint point5 = FP_FromDouble(0.5);
    ASSERT_EQ(point5.GetExponent(), 0);
    ASSERT_EQ(point5.fpValue, 0x8000);

}

FixedPoint::FixedPoint()
{
    fpValue = 0;
}

FixedPoint::FixedPoint(int v)
{
    fpValue = 0x10000 * v;
}

FixedPoint::FixedPoint(double v)
{
    fpValue = static_cast<int>(0x10000 * v);
}


FixedPoint& FixedPoint::operator+=(const FixedPoint& other)
{
    fpValue += other.fpValue;
    return *this;
}

FixedPoint& FixedPoint::operator-=(const FixedPoint& other)
{
    fpValue -= other.fpValue;
    return *this;
}

FixedPoint& FixedPoint::operator/=(const FixedPoint& other)
{
    fpValue = Math_FixedPoint_Divide_496B70(fpValue, other.fpValue);
    return *this;
}


FixedPoint& FixedPoint::operator*=(const FixedPoint& other)
{
    fpValue = Math_FixedPoint_Multiply_496C50(fpValue, other.fpValue);
    return *this;
}

short FixedPoint::GetExponent() const
{
    return static_cast<short>(fpValue / 0x10000);
}

double FixedPoint::GetDouble() const
{
    return static_cast<double>(fpValue) / 0x10000;
}


void FixedPoint::RemoveFractional()
{
    // Leave only the whole number part
    fpValue &= 0xFFFF0000;
}

void FixedPoint::SetRaw(signed int rawFp)
{
    fpValue = rawFp;
}

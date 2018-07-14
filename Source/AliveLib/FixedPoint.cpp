#include "stdafx.h"
#include "FixedPoint.hpp"

void FixedPoint_ForceLink()
{
    FP fp = 0;
    fp += 2.0;
    fp -= 0.5;
    fp *= 2.0;
    fp /= 5.0;
    fp += 5;

    int f = fp;
    double d = fp;
}

inline FixedPoint& FixedPoint::operator+=(const FixedPoint& other)
{
    this->fpValue += other.fpValue;
    return *this;
}

inline FixedPoint& FixedPoint::operator-=(const FixedPoint& other)
{
    this->fpValue -= other.fpValue;
    return *this;
}

inline FixedPoint& FixedPoint::operator*=(const FixedPoint& other)
{
    this->fpValue = Math_FixedPoint_Multiply_496C50(this->fpValue, other.fpValue);
    return *this;
}

inline FixedPoint& FixedPoint::operator/=(const FixedPoint& other)
{
    this->fpValue = Math_FixedPoint_Divide_496B70(this->fpValue, other.fpValue);
    return *this;
}

inline void FixedPoint::SetRaw(signed int rawFp)
{
    fpValue = rawFp;
}

inline FixedPoint::operator signed int() {
    return fpValue >> 16;
}

inline FixedPoint::operator double() {
    return static_cast<double>(fpValue) / 0x10000;
}

std::ostream& operator<<(std::ostream &strm, const FixedPoint &other) {
    return strm << static_cast<double>((double)(other.fpValue) / 0x10000);
}
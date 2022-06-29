#pragma once

#include "../AliveLibCommon/Function.hpp"
#include <type_traits>

struct FixedPoint;
using FP = FixedPoint;

struct FixedPoint final
{
    s32 fpValue;
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

inline FixedPoint operator-(const FixedPoint& value)
{
    FixedPoint f = {};
    f.fpValue = -value.fpValue;
    return f;
}

inline bool operator<(const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue < rhs.fpValue;
}

inline bool operator<=(const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue <= rhs.fpValue;
}

inline bool operator>(const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue > rhs.fpValue;
}

inline bool operator>=(const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue >= rhs.fpValue;
}

inline bool operator==(const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue == rhs.fpValue;
}

inline bool operator!=(const FixedPoint& lhs, const FixedPoint& rhs)
{
    return lhs.fpValue != rhs.fpValue;
}

inline FixedPoint FP_FromDouble(f64 v)
{
    FixedPoint f = {};
    f.fpValue = (static_cast<s32>(v * 0x10000));
    return f;
}

inline FixedPoint FP_FromRaw(s32 v)
{
    FixedPoint f = {};
    f.fpValue = v;
    return f;
}

template <class T>
inline FixedPoint FP_FromInteger(T v)
{
    FixedPoint f = {};
    f.fpValue = v << 16;
    return f;
}

inline s16 FP_GetExponent(const FixedPoint& fp)
{
    return static_cast<s16>(fp.fpValue / 0x10000);
}

inline f64 FP_GetDouble(FixedPoint& fp)
{
    return static_cast<f64>(fp.fpValue) / 0x10000;
}

inline FixedPoint FP_NoFractional(const FixedPoint& fp)
{
    FixedPoint nfp = fp;
    nfp.fpValue &= 0xFFFF0000;
    return nfp;
}

inline FP FP_Abs(const FP& v)
{
    if (v < FP_FromInteger(0))
    {
        return FP_FromRaw(-v.fpValue);
    }
    else
    {
        return v;
    }
}

using FP = FixedPoint;

ALIVE_ASSERT_SIZEOF(FP, 0x4);

struct FP_Point final
{
    FP x, y;
};
ALIVE_ASSERT_SIZEOF(FP_Point, 8);

struct FP_Rect final
{
    FP x, y, w, h;
};

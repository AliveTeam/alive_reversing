#pragma once

#include "../AliveLibCommon/Types.hpp"
#include <limits>

template<class T> T Clamp(T val, T lowerBound, T upperBound)
{
    return val > upperBound ? upperBound : val < lowerBound ? lowerBound : val;
}

template<class T> T ClampedAdd(T val, s64 operand)
{
    if (operand == 0)
    {
        return val;
    }

    T newVal = val + static_cast<T>(operand);

    if (operand < 0 && newVal > val)
    {
        return std::numeric_limits<T>().min();
    }
    else if (operand > 0 && newVal < val)
    {
        return std::numeric_limits<T>().max();
    }

    return newVal;
}

template<class T> T ClampedAdd(T val, s64 operand, T lowerBound, T upperBound)
{
    return Clamp(ClampedAdd(val, operand), lowerBound, upperBound);
}

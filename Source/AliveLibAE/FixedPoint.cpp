#include "stdafx.h"
#include "FixedPoint.hpp"
#include <gtest/gtest.h>

s32 Math_SquareRoot_Shifted_496E20(u32 value, s16 iterations)
{
    u32 value_shifted = value;
    s32 ret = 0;
    s32 counter = (iterations / 2) + 15;
    u32 tmp = 0;
    for (s32 i = 0; i <= counter; i++)
    {
        ret *= 2;
        tmp = (value_shifted >> 30) | 4 * tmp; // Hm.. what?
        const u32 v6 = (2 * ret) + 1;
        value_shifted *= 4;
        if (tmp >= v6)
        {
            tmp -= v6;
            ret++;
        }
    }
    return ret;
}

s32 Math_SquareRoot_Int_496E70(s32 value)
{
    return Math_SquareRoot_Shifted_496E20(value, 0); // 15 iterations
}

FP Math_SquareRoot_FP(FP value)
{
    return FP_FromRaw(Math_SquareRoot_Shifted_496E20(value.fpValue, 16)); // 23 iterations (16/2+15)
}

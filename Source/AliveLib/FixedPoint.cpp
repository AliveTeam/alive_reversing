#include "stdafx.h"
#include "FixedPoint.hpp"
#include <gtest/gtest.h>

void FixedPoint_ForceLink()
{
    FP fp = FP_FromInteger(0);
    fp += FP_FromInteger(5);

    int f = FP_GetExponent(fp);
    ASSERT_EQ(f, 5);

    FixedPoint one = FP_FromDouble(1.0);
    ASSERT_EQ(FP_GetExponent(one), 1);
    ASSERT_EQ(one.fpValue, 0x10000);

    FixedPoint point5 = FP_FromDouble(0.5);
    ASSERT_EQ(FP_GetExponent(point5), 0);
    ASSERT_EQ(point5.fpValue, 0x8000);

    FixedPoint t1 = FP_NoFractional(FP_FromDouble(1.2));
    FixedPoint t2 = FP_NoFractional(FP_FromDouble(1.9));

    ASSERT_EQ(t1.fpValue, 0x10000);
    ASSERT_EQ(t2.fpValue, 0x10000);

    FixedPoint neg = {};
    neg.fpValue = 20;
    ASSERT_EQ(-20, (-neg).fpValue);
}

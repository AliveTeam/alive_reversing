#pragma once

#include "FunctionFwd.hpp"

struct FixedPoint;
using FP = FixedPoint;

void Math_ForceLink();

EXPORT u32 CC Math_FixedPoint_Multiply_496C50(s32 op1, s32 op2);
EXPORT u32 CC Math_FixedPoint_Divide_496B70(s32 op1, s32 op2);
EXPORT s16 CC Math_RandomRange_496AB0(s16 min, s16 max);
EXPORT u8 Math_NextRandom();

EXPORT FP CC Math_Cosine_496CD0(u8 v);
EXPORT FP CC Math_Sine_496DD0(u8 v);
EXPORT FP CC Math_Cosine_496D60(FP fp);
EXPORT FP CC Math_Sine_496DF0(FP fp);

EXPORT s32 CC Math_Distance_496EB0(s32 x1, s32 y1, s32 x2, s32 y2);

EXPORT FP CC Math_Tan_496F70(FP value1, FP value2);


namespace AETest::TestsMath {
void Math_Tests();
}

ALIVE_ARY_EXTERN(u8, 256, sRandomBytes_546744);

#pragma once

#include "../AliveLibCommon/Types.hpp"

struct FixedPoint;
using FP = FixedPoint;

u32 Math_FixedPoint_Multiply(s32 op1, s32 op2);
u32 Math_FixedPoint_Divide(s32 op1, s32 op2);
s16 Math_RandomRange(s16 min, s16 max);
u8 Math_NextRandom();

FP Math_Cosine_496CD0(u8 v);
FP Math_Sine_496DD0(u8 v);
FP Math_Cosine_496D60(FP fp);
FP Math_Sine_496DF0(FP fp);

s32 Math_Distance(s32 x1, s32 y1, s32 x2, s32 y2);

FP Math_Tan_496F70(FP value1, FP value2);

extern u8 sRandomBytes_546744[256];

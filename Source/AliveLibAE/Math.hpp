#pragma once

#include "../relive_lib/Types.hpp"

struct FixedPoint;
using FP = FixedPoint;

u32 Math_FixedPoint_Multiply(s32 op1, s32 op2);
u32 Math_FixedPoint_Divide(s32 op1, s32 op2);
s16 Math_RandomRange(s16 min, s16 max);
u8 Math_NextRandom();

FP Math_Cosine(u8 v);
FP Math_Sine(u8 v);
FP Math_Cosine(FP fp);
FP Math_Sine(FP fp);

s32 Math_Distance(s32 x1, s32 y1, s32 x2, s32 y2);

FP Math_Tan(FP value1, FP value2);
s32 Math_SquareRoot_Int(s32 value);
FP Math_SquareRoot_FP(FP value);


extern u8 gRandomBytes[256];

#pragma once

#include "../AliveLibCommon/FixedPoint_common.hpp"

namespace AO {

u8 Math_NextRandom();

s16 Math_RandomRange(s16 min, s16 max);

FP Math_Sine(u8 v);

FP Math_Cosine(u8 v);

s32 Math_Distance(s32 x1, s32 y1, s32 x2, s32 y2);

s32 Math_SquareRoot_Int(s32 v);

FP Math_SquareRoot_FP(FP v);

extern u8 gRandomBytes[256];

} // namespace AO

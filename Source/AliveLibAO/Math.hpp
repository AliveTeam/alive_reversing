#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

namespace AO {

u8 Math_NextRandom();

s16 Math_RandomRange(s16 min, s16 max);

FP Math_Sine_451110(u8 v);

FP Math_Cosine_4510A0(u8 v);

s32 Math_Distance(s32 x1, s32 y1, s32 x2, s32 y2);

s32 Math_SquareRoot_Int_4511B0(s32 a1);

FP Math_SquareRoot_FP(FP v);

ALIVE_ARY_EXTERN(u8, 256, sRandomBytes_4BBE30);

} // namespace AO

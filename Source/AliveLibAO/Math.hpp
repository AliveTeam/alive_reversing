#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

namespace AO {

BYTE Math_NextRandom();

EXPORT short CC Math_RandomRange_450F20(signed short min, signed short max);

EXPORT FP CC Math_Sine_451110(BYTE v);

EXPORT FP CC Math_Cosine_4510A0(BYTE v);

EXPORT s32 CC Math_Distance_451270(s32 x1, s32 y1, s32 x2, s32 y2);

EXPORT s32 CC Math_SquareRoot_Int_4511B0(s32 a1);

EXPORT FP CC Math_SquareRoot_FP_451210(FP v);

ALIVE_ARY_EXTERN(u8, 256, sRandomBytes_4BBE30);

}


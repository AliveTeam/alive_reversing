#pragma once

#include "FunctionFwd.hpp"

struct FixedPoint;
using FP = FixedPoint;

void Math_ForceLink();

EXPORT unsigned int  CC Math_FixedPoint_Multiply_496C50(signed int op1, signed int op2);
EXPORT unsigned int CC Math_FixedPoint_Divide_496B70(signed int op1, signed int op2);
EXPORT short CC Math_RandomRange_496AB0(signed short min, signed short max);
BYTE Math_NextRandom();

EXPORT FP CC Math_Cosine_496CD0(BYTE v);
EXPORT FP CC Math_Sine_496DD0(BYTE v);
EXPORT FP CC Math_Cosine_496D60(FP fp);
EXPORT FP CC Math_Sine_496DF0(FP fp);

EXPORT int CC Math_Distance_496EB0(int x1, int y1, int x2, int y2);

namespace Test
{
    void Math_Tests();
}

ALIVE_ARY_EXTERN(unsigned char, 256, sRandomBytes_546744);

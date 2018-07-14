#include "stdafx.h"
#include "FixedPoint.hpp"

void FixedPoint_ForceLink()
{
    FP fp = 0;
    fp += 2.0;
    fp -= 0.5;
    fp *= 2.0;
    fp /= 5.0;
    fp += 5;

    int f = fp;
    double d = fp;
}

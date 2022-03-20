#include "Grid.hpp"

EXPORT s32 CC SnapToXGrid_449930(FP scale, s32 x)
{
    if (scale == FP_FromDouble(0.5))
    {
        s32 v4 = (x % 375 - 6) % 13;
        if (v4 >= 7)
        {
            return x - v4 + 13;
        }
        else
        {
            return x - v4;
        }
    }
    else if (scale == FP_FromInteger(1))
    {
        s32 v3 = (x - 12) % 25;
        if (v3 >= 13)
        {
            return x - v3 + 25;
        }
        else
        {
            return x - v3;
        }
    }
    else
    {
        return x;
    }
}

FP CC ScaleToGridSize_4498B0(FP scaleFP)
{
    if (scaleFP == FP_FromDouble(0.5))
    {
        return FP_FromInteger(13); // 25/2
    }
    if (scaleFP == FP_FromDouble(1.0))
    {
        return FP_FromInteger(25);
    }
    return FP_FromInteger(0);
}


#include "Grid.hpp"

namespace AO {

s32 XGrid_Index_To_XPos(FP scale, s32 xGridIndex)
{
    if (scale == FP_FromDouble(0.5))
    {
        // 12.5 = half grid size
        return 13 * xGridIndex + 245;
    }

    if (scale == FP_FromInteger(1))
    {
        // 25 = full grid size
        return 25 * xGridIndex + 240;
    }

    // Default to middle of the screen
    return 440;
}

FP ScaleToGridSize(FP scale)
{
    if (scale == FP_FromDouble(0.5))
    {
        return FP_FromInteger(13);
    }

    if (scale == FP_FromInteger(1))
    {
        return FP_FromInteger(25);
    }
    return FP_FromInteger(0);
}

s32 SnapToXGrid(FP scale, s32 x)
{
    if (scale == FP_FromDouble(0.5))
    {
        s32 v4 = (x - 11) % 13;
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
        s32 v3 = (x - 15) % 25;
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
        return 440;
    }
}


FP CamX_VoidSkipper(FP xpos, FP xvel, s16 xMargin, u16* pResult)
{
    const FP v1 = xpos - FP_FromInteger(256);

    const s32 xDiv = FP_GetExponent(v1) / 512;
    const FP xMod = FP_FromInteger(FP_GetExponent(v1) % 512);

    const s32 xDivEven = xDiv % 2;

    FP result = {};
    if ((!xDivEven || xMod >= FP_FromInteger(512 - xMargin)) && (xDivEven || xMod <= FP_FromInteger(xMargin + 368)))
    {
        *pResult = 0;
        result = xpos;
    }
    else if (xvel <= FP_FromInteger(0))
    {
        *pResult = 1;
        result = FP_FromInteger((xDiv * 512) + xMargin + 112);
    }
    else
    {
        if (xDivEven)
        {
            *pResult = 2;
            result = FP_FromInteger((xDiv * 512) - xMargin + 768);
        }
        else
        {
            *pResult = 2;
            result = FP_FromInteger((xDiv * 512) - xMargin + 1280);
        }
    }
    return result;
}

FP CamY_VoidSkipper(FP ypos, FP yvel, s16 yMargin, u16* pResult)
{
    const s32 yVal = (FP_GetExponent(ypos) - 120);
    const s32 yIdx = yVal / 240;
    if (!(yIdx % 2))
    {
        *pResult = 0; // in camera
        return ypos;
    }

    const s32 blockNum = yVal % 240;
    if ((blockNum >= 240 - yMargin) || (blockNum <= yMargin))
    {
        return ypos;
    }

    s32 newY = 0;
    if (yvel <= FP_FromInteger(0))
    {
        *pResult = 3; // top
        newY = (240 * yIdx) + yMargin + 120;
    }
    else
    {
        *pResult = 4; // bottom
        newY = (240 * yIdx) - yMargin + 360;
    }
    return FP_FromInteger(newY);
}

} // namespace AO

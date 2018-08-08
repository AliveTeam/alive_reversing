#include "stdafx.h"
#include "VRam.hpp"
#include "Function.hpp"

EXPORT char CC Vram_calc_width_4955A0(int width, int depth)
{
    switch (depth)
    {
    case 0:
        return ((width + 7) >> 2) & 0xFE;
    case 1:
        return ((width + 3) >> 1) & 0xFE;
    case 2:
        return (width + 1) & 0xFE;
    }
    return 0;
}

signed __int16 CC Vram_alloc_4956C0(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT *pRect)
{
    NOT_IMPLEMENTED();
}

void CC Vram_free_495A60(int xy, int wh)
{
    NOT_IMPLEMENTED();
}

signed __int16 CC Pal_Allocate_483110(PSX_RECT *a1, unsigned int paletteColorCount)
{
    NOT_IMPLEMENTED();
}

void CC Pal_free_483390(PSX_Point xy, __int16 palDepth)
{
    NOT_IMPLEMENTED();
}

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

signed __int16 CC Vram_alloc_4956C0(unsigned __int16 /*width*/, __int16 /*height*/, unsigned __int16 /*colourDepth*/, PSX_RECT* /*pRect*/)
{
    NOT_IMPLEMENTED();
}

int __cdecl Vram_alloc_fixed_4955F0(__int16 /*a1*/, __int16 /*a2*/, __int16 /*a3*/, __int16 /*a4*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void CC Vram_free_495A60(int /*xy*/, int /*wh*/)
{
    NOT_IMPLEMENTED();
}

signed __int16 CC Pal_Allocate_483110(PSX_RECT* /*a1*/, unsigned int /*paletteColorCount*/)
{
    NOT_IMPLEMENTED();
}

void CC Pal_free_483390(PSX_Point /*xy*/, __int16 /*palDepth*/)
{
    NOT_IMPLEMENTED();
}

EXPORT BOOL CC Vram_rects_overlap_4959E0(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    const int x1 = pRect1->x;
    const int x2 = pRect2->x;
    if (x1 >= x2 + pRect2->w)
    {
        return 0;
    }

    const int y2 = pRect2->y;
    const int y1 = pRect1->y;
    if (y1 >= y2 + pRect2->h)
    {
        return 0;
    }

    if (x2 < x1 + pRect1->w)
    {
        return y2 < y1 + pRect1->h;
    }

    return 0;
}
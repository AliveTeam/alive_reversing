#include "stdafx_ao.h"
#include "Function.hpp"
#include "VRam.hpp"

namespace AO {

const int kMaxAllocs = 512;
ALIVE_ARY(1, 0x509018, PSX_RECT, kMaxAllocs, sVramAllocations_509018, {});
ALIVE_VAR(1, 0x50A018, int, sVram_Count_dword_50A018, 0);

EXPORT void CC Pal_Reset_4476C0(unsigned __int16 /*a1*/, unsigned __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}


EXPORT __int16 CC Pal_Allocate_4476F0(PSX_RECT* /*pRect*/, unsigned int /*paletteColorCount*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Pal_Free_447870(PSX_Point /*xy*/, __int16 /*palDepth*/)
{
    NOT_IMPLEMENTED();
}


void CC Pal_Copy_4479D0(PSX_Point point, __int16 w, WORD* pPalData, PSX_RECT* rect)
{
    rect->y = point.field_2_y;
    rect->x = point.field_0_x;
    rect->w = w;
    rect->h = 1;
    PSX_StoreImage_496320(rect, pPalData);
}

void CC Pal_Set_447990(PSX_Point xy, __int16 w, const BYTE* palData, PSX_RECT* rect)
{
    rect->y = xy.field_2_y;
    rect->x = xy.field_0_x;
    rect->w = w;
    rect->h = 1;
    PSX_LoadImage16_4962A0(rect, palData);
}

unsigned int CC Pal_Make_Colour_447950(BYTE /*r*/, BYTE /*g*/, BYTE /*b*/, __int16 /*bOpaque*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Vram_alloc_explicit_4507F0(__int16 x, __int16 y, __int16 w, __int16 h)
{
    if (sVram_Count_dword_50A018 < kMaxAllocs)
    {
        const int idx = sVram_Count_dword_50A018++;
        sVramAllocations_509018[idx].x = x;
        sVramAllocations_509018[idx].y = y;
        sVramAllocations_509018[idx].w = w - x;
        sVramAllocations_509018[idx].h = h - y;
    }
}

EXPORT void CC Vram_reset_450840()
{
    NOT_IMPLEMENTED();
}

void CC Vram_free_450CE0(PSX_Point /*xy*/, PSX_Point /*wh*/)
{
    NOT_IMPLEMENTED();
}

__int16 CC vram_alloc_450B20(unsigned __int16 /*width*/, __int16 /*height*/, unsigned __int16 /*colourDepth*/, PSX_RECT* /*pRect*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

}

#include "stdafx_ao.h"
#include "Function.hpp"
#include "VRam.hpp"

START_NS_AO

EXPORT signed __int16 CC Pal_Allocate_4476F0(PSX_RECT* /*pRect*/, unsigned int /*paletteColorCount*/)
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
}

END_NS_AO

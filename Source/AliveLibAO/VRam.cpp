#include "stdafx_ao.h"
#include "Function.hpp"
#include "VRam.hpp"
#include "../AliveLibAE/VRam.hpp"
#include "Renderer/IRenderer.hpp"

namespace AO {


EXPORT void CC Pal_Reset_4476C0(u16 /*a1*/, u16 /*a2*/)
{
    AE_IMPLEMENTED();
    Pal_Area_Init_483080(0, 240, 640, 32);
}


EXPORT s16 CC Pal_Allocate_4476F0(PSX_RECT* pRect, u32 paletteColorCount)
{
    AE_IMPLEMENTED();
    return Pal_Allocate_483110(pRect, paletteColorCount);
}

EXPORT void CC Pal_Free_447870(PSX_Point xy, s16 palDepth)
{
    AE_IMPLEMENTED();
    Pal_free_483390(xy, palDepth);
}


void CC Pal_Copy_4479D0(PSX_Point point, s16 w, WORD* pPalData, PSX_RECT* rect)
{
    rect->y = point.field_2_y;
    rect->x = point.field_0_x;
    rect->w = w;
    rect->h = 1;
    PSX_StoreImage_496320(rect, pPalData);
}

void CC Pal_Set_447990(PSX_Point xy, s16 w, const BYTE* palData, PSX_RECT* rect)
{
    rect->y = xy.field_2_y;
    rect->x = xy.field_0_x;
    rect->w = w;
    rect->h = 1;
    IRenderer::GetRenderer()->PalSetData(IRenderer::PalRecord{xy.field_0_x, xy.field_2_y, w}, palData);
}

u32 CC Pal_Make_Colour_447950(BYTE r, BYTE g, BYTE b, s16 bOpaque)
{
    AE_IMPLEMENTED();
    return Pal_Make_Colour_4834C0(r, g, b, bOpaque);
}

EXPORT void CC Vram_alloc_explicit_4507F0(s16 x, s16 y, s16 w, s16 h)
{
    AE_IMPLEMENTED();
    Vram_alloc_explicit_4955F0(x, y, w, h);
}

EXPORT void CC Vram_reset_450840()
{
    AE_IMPLEMENTED();
    Vram_init_495660();
}

void CC Vram_free_450CE0(PSX_Point xy, PSX_Point wh)
{
    AE_IMPLEMENTED();
    Vram_free_495A60(xy, wh);
}

s16 CC vram_alloc_450B20(u16 width, s16 height, u16 colourDepth, PSX_RECT* pRect)
{
    AE_IMPLEMENTED();
    return Vram_alloc_4956C0(width, height, colourDepth, pRect);
}

EXPORT s32 CC vram_alloc_450860(s16 width, s16 height, PSX_RECT* pRect)
{
    AE_IMPLEMENTED();
    return Vram_alloc_4956C0(width, height, 16, pRect);
}

}

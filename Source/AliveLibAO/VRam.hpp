#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

namespace AO {

EXPORT void CC Pal_Reset_4476C0(u16 a1, u16 a2);

EXPORT s16 CC Pal_Allocate_4476F0(PSX_RECT* pRect, u32 paletteColorCount);

EXPORT void CC Pal_Free_447870(PSX_Point xy, s16 palDepth);

EXPORT void CC Pal_Copy_4479D0(PSX_Point point, s16 w, WORD* pPalData, PSX_RECT* rect);

EXPORT void CC Pal_Set_447990(PSX_Point xy, s16 w, const u8* palData, PSX_RECT* rect);

EXPORT u32 CC Pal_Make_Colour_447950(u8 r, u8 g, u8 b, s16 bOpaque);

EXPORT void CC Vram_reset_450840();

EXPORT void CC Vram_free_450CE0(PSX_Point xy, PSX_Point wh);

EXPORT void CC Vram_alloc_explicit_4507F0(s16 x, s16 y, s16 w, s16 h);

EXPORT s16 CC vram_alloc_450B20(u16 width, s16 height, u16 colourDepth, PSX_RECT* pRect);


EXPORT s32 CC vram_alloc_450860(s16 width, s16 height, PSX_RECT* pRect);


}

#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

START_NS_AO

EXPORT void CC Pal_Reset_4476C0(unsigned __int16 a1, unsigned __int16 a2);

EXPORT __int16 CC Pal_Allocate_4476F0(PSX_RECT* pRect, unsigned int paletteColorCount);

EXPORT void CC Pal_Free_447870(PSX_Point xy, __int16 palDepth);

EXPORT void CC Pal_Copy_4479D0(PSX_Point point, __int16 w, WORD* pPalData, PSX_RECT* rect);

EXPORT void CC Pal_Set_447990(PSX_Point xy, __int16 w, const BYTE* palData, PSX_RECT* rect);

EXPORT unsigned int CC Pal_Make_Colour_447950(BYTE r, BYTE g, BYTE b, __int16 bOpaque);

EXPORT void CC Vram_reset_450840();

EXPORT void CC Vram_free_450CE0(PSX_Point xy, PSX_Point wh);

EXPORT void CC Vram_alloc_explicit_4507F0(__int16 x, __int16 y, __int16 w, __int16 h);

EXPORT __int16 CC vram_alloc_450B20(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT* pRect);

END_NS_AO

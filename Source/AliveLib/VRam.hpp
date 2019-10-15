#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

ALIVE_VAR_EXTERN(WORD, word_5CC88C);

EXPORT signed __int16 CC Vram_alloc_4956C0(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT *pRect);

EXPORT void CC Vram_init_495660();
EXPORT void CC Vram_alloc_explicit_4955F0(__int16 x, __int16 y, __int16 w, __int16 h);
EXPORT void CC Vram_free_495A60(PSX_Point xy, PSX_Point wh);

EXPORT signed __int16 CC Pal_Allocate_483110(PSX_RECT* a1, unsigned int paletteColorCount);
EXPORT void CC Pal_free_483390(PSX_Point xy, __int16 palDepth);
EXPORT BOOL CC Vram_rects_overlap_4959E0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);


EXPORT void CC Pal_Area_Init_483080(__int16 xpos, __int16 ypos, unsigned __int16 width, unsigned __int16 height);

EXPORT void CC Pal_Copy_483560(PSX_Point pPoint, __int16 w, WORD* pPalData, PSX_RECT* rect);


EXPORT unsigned int CC Pal_Make_Colour_4834C0(BYTE r, BYTE g, BYTE b, __int16 bOpaque);

EXPORT void CC Pal_Set_483510(PSX_Point xy, __int16 w, const BYTE* palData, PSX_RECT* rect);

namespace Test
{
    void VRamTests();
}

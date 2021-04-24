#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

ALIVE_VAR_EXTERN(WORD, unused_5CC88C);

EXPORT s16 CC Vram_alloc_4956C0(u16 width, s16 height, u16 colourDepth, PSX_RECT* pRect);

EXPORT void CC Vram_init_495660();
EXPORT void CC Vram_alloc_explicit_4955F0(s16 x, s16 y, s16 w, s16 h);
EXPORT void CC Vram_free_495A60(PSX_Point xy, PSX_Point wh);

EXPORT s16 CC Pal_Allocate_483110(PSX_RECT* pRect, u32 paletteColorCount);
EXPORT void CC Pal_free_483390(PSX_Point xy, s16 palDepth);
EXPORT BOOL CC Vram_rects_overlap_4959E0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);


EXPORT void CC Pal_Area_Init_483080(s16 xpos, s16 ypos, u16 width, u16 height);

EXPORT void CC Pal_Copy_483560(PSX_Point pPoint, s16 w, WORD* pPalData, PSX_RECT* rect);


EXPORT u32 CC Pal_Make_Colour_4834C0(u8 r, u8 g, u8 b, s16 bOpaque);

EXPORT void CC Pal_Set_483510(PSX_Point xy, s16 w, const u8* palData, PSX_RECT* rect);

namespace AETest::TestsVRam
{
    void VRamTests();
}

#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

EXPORT signed __int16 CC Vram_alloc_4956C0(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT *pRect);

EXPORT int __cdecl Vram_alloc_fixed_4955F0(__int16 /*a1*/, __int16 /*a2*/, __int16 /*a3*/, __int16 /*a4*/);

EXPORT void CC Vram_free_495A60(int xy, int wh);

EXPORT signed __int16 CC Pal_Allocate_483110(PSX_RECT *a1, unsigned int paletteColorCount);
EXPORT void CC Pal_free_483390(PSX_Point xy, __int16 palDepth);
EXPORT BOOL CC Vram_rects_overlap_4959E0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

namespace Test
{
    void VRamTests();
}
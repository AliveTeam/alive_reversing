#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

EXPORT signed __int16 CC Vram_alloc_4956C0(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT *pRect);
EXPORT void CC Vram_free_495A60(int xy, int wh);

EXPORT signed __int16 CC Pal_Allocate_483110(PSX_RECT *a1, unsigned int paletteColorCount);

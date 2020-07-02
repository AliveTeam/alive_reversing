#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"

START_NS_AO

EXPORT signed __int16 CC Pal_Allocate_4476F0(PSX_RECT* pRect, unsigned int paletteColorCount);

EXPORT void CC Pal_Free_447870(PSX_Point xy, __int16 palDepth);

END_NS_AO

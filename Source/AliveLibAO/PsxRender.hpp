#pragma once

#include "FunctionFwd.hpp"
#include "Psx_common.hpp"

namespace AO {


EXPORT void CC PSX_ClearOTag_496760(int** otBuffer, int otBufferSize);

EXPORT void CC PSX_DrawOTag_4969F0(int** pOT);

EXPORT signed int CC PSX_EMU_SetDispType_499E60(int dispType);

EXPORT int CC PSX_ClearImage_496020(const PSX_RECT* pRect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b);

}


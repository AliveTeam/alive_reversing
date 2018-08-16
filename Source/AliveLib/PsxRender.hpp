#pragma once

#include "FunctionFwd.hpp"

namespace Test
{
    void PsxRenderTests();
}

EXPORT void CC PSX_DrawOTag_4F6540(int** pOT);
EXPORT void CC PSX_OrderingTable_4F62C0(int** otBuffer, int otBufferSize);
EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize);

EXPORT void CC PSX_TPage_Change_4F6430(__int16 tPage);
EXPORT int CC PSX_EMU_SetDispType_4F9960(int dispType);
EXPORT void CC PSX_EMU_Render_51EF90(__int16 x, __int16 y, int minX, int minY, int r, int g, int b, __int16 w, __int16 h, int a10, int a11);

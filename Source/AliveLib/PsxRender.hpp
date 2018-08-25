#pragma once

#include "FunctionFwd.hpp"

struct PSX_RECT;

namespace Test
{
    void PsxRenderTests();
}

EXPORT void CC PSX_DrawOTag_4F6540(int** pOT);
EXPORT void CC PSX_OrderingTable_4F62C0(int** otBuffer, int otBufferSize);
EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize);

EXPORT void CC PSX_TPage_Change_4F6430(__int16 tPage);
EXPORT int CC PSX_EMU_SetDispType_4F9960(int dispType);

EXPORT void CC PSX_EMU_Render_SPRT_51EF90(__int16 x, __int16 y, int minX, int minY, BYTE r, BYTE g, BYTE b, __int16 w, __int16 h, WORD clut, int semiTrans);
EXPORT signed int CC PSX_ClearImage_4F5BD0(PSX_RECT* pRect, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b);

ALIVE_VAR_EXTERN(int, sScreenXOffSet_BD30E4);
ALIVE_VAR_EXTERN(int, sScreenYOffset_BD30A4);
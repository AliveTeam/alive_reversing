#pragma once

#include "FunctionFwd.hpp"

namespace Test
{
    void PsxRenderTests();
}

EXPORT void CC PSX_DrawOTag_4F6540(int** pOT);
EXPORT void CC PSX_OrderingTable_4F62C0(int** otBuffer, int otBufferSize);
EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize);

EXPORT void CC PSX_TPage_Change_4F6430(unsigned __int16 tPage);

#pragma once

#include "Function.hpp"

void Psx_ForceLink();

struct PSX_RECT
{
    short x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

EXPORT int CC PSX_VSync_4F6170(int mode);

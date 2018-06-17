#pragma once

#include "Function.hpp"

void Psx_ForceLink();

struct PSX_RECT
{
    short x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

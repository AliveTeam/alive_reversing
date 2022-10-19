#pragma once

#include "Function.hpp"

namespace AO {

s32 VGA_DisplaySet_490230(u16 width, u16 height, s8 bpp, u8 backbufferCount);

void VGA_Shutdown_4900E0();

} // namespace AO

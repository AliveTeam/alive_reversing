#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/bmp.hpp"

void VGA_Shutdown_4F3170();
void VGA_CopyToFront(RECT* pRect);
s32 VGA_DisplaySet_4F32C0(u16 width, u16 height, u8 bpp, u8 backbufferCount, TSurfaceType** ppSurface);

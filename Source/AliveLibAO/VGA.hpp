#pragma once

#include "Function.hpp"
#include "../AliveLibAE/bmp.hpp" // For TSurfaceType

namespace AO {

s32 VGA_FullScreenSet_490160(bool bFullScreen);

s32 VGA_DisplaySet_490230(u16 width, u16 height, s8 bpp, u8 backbufferCount, TSurfaceType** ppSurface);

s32 VGA_GetPixelFormat_490E60();

void VGA_Shutdown_4900E0();

ALIVE_VAR_EXTERN(Bitmap, sVGA_bmp_primary_A8A4C0);

} // namespace AO

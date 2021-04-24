#pragma once

#include "Function.hpp"
#include "../AliveLibAE/bmp.hpp" // For TSurfaceType

namespace AO {

EXPORT int CC VGA_FullScreenSet_490160(bool bFullScreen);

EXPORT s32 CC VGA_DisplaySet_490230(u16 width, u16 height, char bpp, u8 backbufferCount, TSurfaceType** ppSurface);

EXPORT int CC VGA_GetPixelFormat_490E60();

EXPORT void VGA_Shutdown_4900E0();

ALIVE_VAR_EXTERN(Bitmap, sVGA_bmp_primary_A8A4C0);

}

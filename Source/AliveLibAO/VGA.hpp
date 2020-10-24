#pragma once

#include "Function.hpp"
#include "../AliveLibAE/bmp.hpp" // For TSurfaceType

START_NS_AO

EXPORT int CC VGA_FullScreenSet_490160(bool bFullScreen);

EXPORT signed int CC VGA_DisplaySet_490230(unsigned __int16 width, unsigned __int16 height, char bpp, unsigned __int8 backbufferCount, TSurfaceType** ppSurface);

EXPORT int CC VGA_GetPixelFormat_490E60();

EXPORT void VGA_Shutdown_4900E0();

ALIVE_VAR_EXTERN(Bitmap, sVGA_bmp_primary_A8A4C0);

END_NS_AO

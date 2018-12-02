#pragma once

#include "FunctionFwd.hpp"
#include "bmp.hpp"

void VGA_ForceLink();

EXPORT void CC VGA_Shutdown_4F3170();
EXPORT void CC VGA_CopyToFront_4F3710(Bitmap* pBmp, RECT* pRect);
EXPORT void CC VGA_CopyToFront_4F3EB0(Bitmap* pBmp, RECT* pRect, unsigned __int8 screenMode);
EXPORT bool VGA_IsWindowMode_4F31E0();
EXPORT signed int CC VGA_FullScreenSet_4F31F0(bool bFullScreen);
EXPORT signed int CC VGA_DisplaySet_4F32C0(unsigned __int16 width, unsigned __int16 height, unsigned __int8 bpp, unsigned __int8 a4, TSurfaceType** ppSurface);
EXPORT int VGA_GetPixelFormat_4F3EE0();

ALIVE_VAR_EXTERN(Bitmap, sVGA_bmp_primary_BD2A20);

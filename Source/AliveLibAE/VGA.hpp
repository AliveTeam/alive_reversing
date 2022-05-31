#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "bmp.hpp"

void VGA_ForceLink();

void VGA_Shutdown_4F3170();
void VGA_CopyToFront_4F3710(Bitmap* pBmp, RECT* pRect);
void VGA_CopyToFront_4F3EB0(Bitmap* pBmp, RECT* pRect, u8 screenMode);
bool VGA_IsWindowMode_4F31E0();
s32 VGA_FullScreenSet_4F31F0(bool bFullScreen);
s32 VGA_DisplaySet_4F32C0(u16 width, u16 height, u8 bpp, u8 backbufferCount, TSurfaceType** ppSurface);
s32 VGA_GetPixelFormat_4F3EE0();

ALIVE_VAR_EXTERN(Bitmap, sVGA_bmp_primary_BD2A20);

extern bool s_VGA_KeepAspectRatio;
extern bool s_VGA_FilterScreen;
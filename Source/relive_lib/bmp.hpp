#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/relive_config.h"

#ifdef _WIN32
    #if !USE_SDL2
        #define DIRECTINPUT_VERSION 0x700
        #include <dinput.h>
        #define DIRECTDRAW_VERSION 0x700
        #include <ddraw.h>
        #define DIRECT3D_VERSION 0x700
        #include "d3d.h"
    #endif
#endif

#if USE_SDL2
    #include "SDL.h"
using TSurfaceType = SDL_Surface;
#else
using TSurfaceType = IDirectDrawSurface;
#endif

struct Bitmap final
{
    TSurfaceType* field_0_pSurface;
    void* field_4_pLockedPixels;
    s32 field_8_width;
    s32 field_C_height;
    s32 field_10_locked_pitch;
    s8 field_14_bpp;
    u8 field_15_pixel_format;
    s16 field_16_pad;
    s32 field_18_create_flags;
};
ALIVE_ASSERT_SIZEOF(Bitmap, 0x1C);

s32 BMP_Blt_4F1E50(Bitmap* pDstBmp, s32 xPos, s32 yPos, Bitmap* pSrcBmp, LPRECT pRect, s32 flags);
void Bmp_Free_4F1950(Bitmap* pBmp);
s32 Bmp_Convert_Colour_4F17D0(Bitmap* pBmp, s32 r, s32 g, s32 b);
s32 BMP_ClearRect(Bitmap* pBmp, const RECT* pRect, u32 fillColour);
void BMP_unlock_4F2100(Bitmap* pBmp);
void BMP_Release_DC_4F21A0(Bitmap* pBmp, HDC hdc);
LPVOID BMP_Lock_4F1FF0(Bitmap* pBitmap);
s32 BMP_New_4F1990(Bitmap* pBitmap, s32 width, s32 height, s32 pixelFormat, s32 createFlags);
void BMP_Draw_String_4F2230(Bitmap* pBmp, s32 x, s32 y, u32 fgColour, s32 bgColour, LPCSTR lpString);
LONG BMP_Get_Font_Height_4F21F0(Bitmap* pBmp);

ALIVE_VAR_EXTERN(u8, gVGA_force_sys_memory_surfaces_BC0BB4);

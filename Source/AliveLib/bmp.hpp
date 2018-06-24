#pragma once

#include "FunctionFwd.hpp"

#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"

namespace Test
{
    void BmpTests();
}

struct Bitmap
{
    IDirectDrawSurface* field_0_pSurface;
    void* field_4_pLockedPixels;
    int field_8_width;
    int field_C_height;
    int field_10_locked_pitch;
    char field_14_bpp;
    unsigned char field_15_pixel_format;
    __int16 field_16_pad;
    int field_18_create_flags;
};
ALIVE_ASSERT_SIZEOF(Bitmap, 0x1C);

EXPORT signed int CC BMP_Blt_4F1E50(Bitmap* pDstBmp, int xPos, int yPos, Bitmap* pSrcBmp, LPRECT pRect, int flags);
EXPORT void CC Bmp_Free_4F1950(Bitmap* pBmp);
EXPORT int CC Bmp_Convert_Colour_4F17D0(Bitmap* pBmp, int r, int g, int b);
EXPORT signed int CC BMP_ClearRect_4F1EE0(Bitmap* pBmp, const RECT* pRect, DWORD fillColour);
EXPORT void CC BMP_unlock_4F2100(Bitmap* pBmp);
EXPORT void CC BMP_Release_DC_4F21A0(Bitmap* pBmp, HDC hdc);
EXPORT LPVOID CC BMP_Lock_4F1FF0(Bitmap* pBitmap);
EXPORT signed int CC BMP_New_4F1990(Bitmap* pBitmap, int width, int height, int pixelFormat, int createFlags);
EXPORT void CC BMP_Draw_String_4F2230(Bitmap* pBmp, int x, int y, unsigned int fgColour, int bgColour, LPCSTR lpString);
EXPORT LONG CC BMP_Get_Font_Height_4F21F0(Bitmap* pBmp);

ALIVE_VAR_EXTERN(unsigned __int8, gVGA_force_sys_memory_surfaces_BC0BB4);

#pragma once

START_NS_AO

#if USE_SDL2
#include "SDL.h"
using TSurfaceType = ::SDL_Surface;
#else
using TSurfaceType = IDirectDrawSurface;
#endif

struct Bitmap
{
    TSurfaceType* field_0_pSurface;
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

END_NS_AO

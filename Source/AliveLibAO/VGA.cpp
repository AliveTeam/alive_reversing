#include "stdafx.h"
#include "VGA.hpp"
#include "Function.hpp"
#include "../AliveLibAE/VGA.hpp"

START_NS_AO

EXPORT int CC VGA_FullScreenSet_490160(bool bFullScreen)
{
    NOT_IMPLEMENTED();
    AE_IMPLEMENTED();
    return VGA_FullScreenSet_4F31F0(bFullScreen);
}

EXPORT signed int CC VGA_DisplaySet_490230(unsigned __int16 width, unsigned __int16 height, char bpp, unsigned __int8 backbufferCount, TSurfaceType** ppSurface)
{
    NOT_IMPLEMENTED();
    AE_IMPLEMENTED();
    return VGA_DisplaySet_4F32C0(width, height, bpp, backbufferCount, ppSurface);
}

EXPORT int CC VGA_GetPixelFormat_490E60()
{
    NOT_IMPLEMENTED();
    AE_IMPLEMENTED();
    return VGA_GetPixelFormat_4F3EE0();
}

END_NS_AO

#pragma once

#include "../AliveLibCommon/Function.hpp"

#if _WIN32
    #if !USE_SDL2
        #define DIRECTINPUT_VERSION 0x700
        #include <dinput.h>
        #define DIRECTDRAW_VERSION 0x700
        #include <ddraw.h>
        #define DIRECT3D_VERSION 0x700
        #include "d3d.h"

RECT ClientToScreenConvert(HWND hwnd);
const s8* DX_HR_To_String_4F4EC0(HRESULT hr);
s32 DD_Shutdown_4F0790(s32 bDestroyDD);
s32 DD_Init_4F0840(s32 a1);
s32 DD_Init_4F02D0(HWND hwnd, bool bFullScreen, s32 forceSoftwareSurfaces);
s32 DD_Enable_4F0380(HWND hwnd, s32 width, s32 height, s32 bpp, s32 flipMode, s32 a6);
void DD_render_back_buffer_4F0D90(IDirectDrawSurface* pSurf, RECT* pRect, s32 screenMode);
void DD_Flip_4F15D0();

ALIVE_VAR_EXTERN(LPDIRECTDRAW, sDDraw_BBC3D4);
ALIVE_VAR_EXTERN(LPDIRECTDRAWSURFACE, sDD_primary_surface_BBC3C8);
ALIVE_VAR_EXTERN(LPDIRECTDRAWSURFACE, sDD_surface_backbuffer_BBC3CC);
ALIVE_VAR_EXTERN(LPDIRECTDRAWPALETTE, sDD_Pal_BBC3D8);
    #endif
#endif

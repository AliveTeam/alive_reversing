#pragma once

#include "FunctionFwd.hpp"

#if _WIN32
#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"
#endif

RECT ClientToScreenConvert(HWND hwnd);
EXPORT const char* CC DX_HR_To_String_4F4EC0(HRESULT hr);
EXPORT signed int CC DD_Shutdown_4F0790(int bDestroyDD);
EXPORT signed int CC DD_Init_4F0840(signed int a1);
EXPORT signed int CC DD_Init_4F02D0(HWND hwnd, bool bFullScreen, int forceSoftwareSurfaces);
EXPORT signed int CC DD_Enable_4F0380(HWND hwnd, int width, int height, int bpp, int flipMode, int a6);
EXPORT void CC DD_render_back_buffer_4F0D90(IDirectDrawSurface* pSurf, RECT* pRect, int screenMode);
EXPORT void CC DD_Flip_4F15D0();

#if _WIN32
ALIVE_VAR_EXTERN(LPDIRECTDRAW, sDDraw_BBC3D4);
ALIVE_VAR_EXTERN(LPDIRECTDRAWSURFACE, sDD_primary_surface_BBC3C8);
ALIVE_VAR_EXTERN(LPDIRECTDRAWSURFACE, sDD_surface_backbuffer_BBC3CC);
ALIVE_VAR_EXTERN(LPDIRECTDRAWPALETTE, sDD_Pal_BBC3D8);
#endif

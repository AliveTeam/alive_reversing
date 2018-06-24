#pragma once

#include "FunctionFwd.hpp"

#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"

EXPORT const char* CC DX_HR_To_String_4F4EC0(HRESULT hr);
EXPORT signed int CC DD_Shutdown_4F0790(int bDestroyDD);

ALIVE_VAR_EXTERN(LPDIRECTDRAW, sDDraw_BBC3D4);
ALIVE_VAR_EXTERN(LPDIRECTDRAWSURFACE, sDD_Surface1_BBC3C8);
ALIVE_VAR_EXTERN(LPDIRECTDRAWSURFACE, sDD_Surface2_BBC3CC);
ALIVE_VAR_EXTERN(LPDIRECTDRAWPALETTE, sDD_Pal_BBC3D8);

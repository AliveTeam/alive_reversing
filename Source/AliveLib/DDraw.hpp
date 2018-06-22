#pragma once

#include "FunctionFwd.hpp"

#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"

EXPORT const char* CC DX_HR_To_String_4F4EC0(HRESULT hr);

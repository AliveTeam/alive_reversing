// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

    // TODO: reference additional headers your program requires here
    #include <windows.h>


    #define DIRECTINPUT_VERSION 0x700
    #include <dinput.h>
    #define DIRECTDRAW_VERSION 0x700
    #include <ddraw.h>
    #define DIRECT3D_VERSION 0x700
    #include "d3d.h"
#else
using PVOID = void*;
using LPVOID = void*;
using HINSTANCE = void*;
using DWORD = unsigned int;
using PCHAR = char*;
using BOOL = long;
using LONG = long;
using LPSTR = char*;
using LPCSTR = const char*;

struct POINT
{
    LONG x;
    LONG y;
};

#define LRESULT

#endif

#include <iostream>
#include <memory>
#include <map>
#include <gmock/gmock.h>
#include "config.h"
#include "logger.hpp"
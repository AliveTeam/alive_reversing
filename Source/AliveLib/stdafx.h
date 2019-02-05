// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
    #define NOMINMAX
    
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
using BYTE = unsigned char;
using WORD = unsigned short int;
using HDC = void*;
using UINT = unsigned int;

#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8  char

#define TRUE 1
#define FALSE 0

struct RECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};

using LPRECT = RECT*;
struct POINT
{
    LONG x;
    LONG y;
};

#define LRESULT

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69

#define VK_F2             0x71
#define VK_ESCAPE         0x1B
#define VK_TAB 0x09
#define VK_LEFT 0x25
#define VK_RIGHT 0x27
#define VK_UP 0x26
#define VK_DOWN 0x28
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_BACK 0x08
#define VK_INSERT 0x2D
#define VK_HOME 0x24
#define VK_DELETE 0x2E
#define VK_END 0x23
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_SPACE 0x20
#define VK_SEPARATOR 0x6C
#define VK_DECIMAL 0x6E
#define VK_RETURN 0x0D

#endif

#include <iostream>
#include <memory>
#include <map>
#include <gmock/gmock.h>
#include "config.h"
#include "logger.hpp"
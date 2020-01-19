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
using LONG = int;
using LPSTR = char*;
using LPCSTR = const char*;
using BYTE = unsigned char;
using WORD = unsigned short int;
using HDC = void*;
using UINT = unsigned int;
using byte = unsigned char;
using HANDLE = void*;
using CHAR = char;

#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8 char

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

#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B

#define VK_OEM_3 0xC0 
#define VK_OEM_COMMA 0xBC 
#define VK_OEM_PERIOD 0xBE 
#define VK_OEM_102 0xE2 
#define VK_CAPITAL 0x14
#define VK_PRINT 0x2A
#define VK_SCROLL 0x91
#define VK_PAUSE 0x13
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C

#define VK_ESCAPE 0x1B
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
#define VK_LSHIFT 0xA0
#define VK_LBUTTON 0x01
#define VK_LCONTROL 0xA2
#define VK_SUBTRACT 0x6D

#define WINAPI
#define CALLBACK
#define DWORD_PTR DWORD
#define LOBYTE(w) ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))

#endif

#include <iostream>
#include <memory>
#include <map>
#include <gmock/gmock.h>
#include "config.h"
#include "logger.hpp"
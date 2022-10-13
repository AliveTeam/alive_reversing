#include "stdafx_ao.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "../AliveLibAE/Sys.hpp"

namespace AO {

static LPSTR sCommandLine = 0;
static HINSTANCE sInstance = 0;
static s32 sCmdShow = 0;

//static u32 sIsAKeyDown = 0;

s32 Sys_IsAnyKeyDown()
{
    // AE impl
    return ::Sys_IsAnyKeyDown();

    // AO impl
    //return sIsAKeyDown;
}

void AO_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    sInstance = hInstance;
    sCmdShow = nShowCmd;
    sCommandLine = lpCmdLine;
    Sys_Main_Common();
}

LPSTR Sys_GetCommandLine()
{
    return sCommandLine;
}

TWindowHandleType Sys_GetWindowHandle()
{
    return ::Sys_GetWindowHandle();
}

void Sys_Set_Hwnd(TWindowHandleType hwnd)
{
    ::Sys_Set_Hwnd(hwnd);
}

s32 Sys_WindowClass_Register(LPCSTR lpClassName, LPCSTR lpWindowName, s32 X, s32 Y, s32 nWidth, s32 nHeight)
{
    return ::Sys_WindowClass_Register(lpClassName, lpWindowName, X, Y, nWidth, nHeight);
}

void SYS_EventsPump()
{
    ::SYS_EventsPump();
}

} // namespace AO

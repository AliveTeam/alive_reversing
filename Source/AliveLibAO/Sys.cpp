#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "Input.hpp"
#include "../AliveLibAE/Sys.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F7718, LPSTR, sCommandLine_9F7718, 0);
ALIVE_VAR(1, 0x9F771C, HINSTANCE, sInstance_9F771C, 0);
ALIVE_VAR(1, 0x9F772C, s32, sCmdShow_9F772C, 0);
ALIVE_VAR(1, 0xA8A600, u32, sIsAKeyDown_A8A600, 0);

s32 Sys_IsAnyKeyDown_48E6C0()
{
    // AE impl
    return ::Sys_IsAnyKeyDown_4EDDF0();

    // AO impl
    //return sIsAKeyDown_A8A600;
}

void AO_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    sInstance_9F771C = hInstance;
    sCmdShow_9F772C = nShowCmd;
    sCommandLine_9F7718 = lpCmdLine;
    Sys_Main_Common();
}

LPSTR Sys_GetCommandLine_48E920()
{
    return sCommandLine_9F7718;
}

TWindowHandleType Sys_GetWindowHandle_48E930()
{
    return Sys_GetWindowHandle_4EE180();
}

void Sys_Set_Hwnd_48E340(TWindowHandleType hwnd)
{
    Sys_Set_Hwnd_4F2C50(hwnd);
}

s32 Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, s32 X, s32 Y, s32 nWidth, s32 nHeight)
{
    return Sys_WindowClass_Register_4EE22F(lpClassName, lpWindowName, X, Y, nWidth, nHeight);
}

void SYS_EventsPump_44FF90()
{
    SYS_EventsPump_494580();
}

} // namespace AO

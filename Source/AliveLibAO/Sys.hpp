#pragma once

#include "FunctionFwd.hpp"
#include "Sys_common.hpp"

namespace AO {

EXPORT TWindowHandleType CC Sys_GetWindowHandle_48E930();

EXPORT void CC Sys_Set_Hwnd_48E340(TWindowHandleType hwnd);

EXPORT int CC Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, int X, int Y, int nWidth, int nHeight);

EXPORT void SYS_EventsPump_44FF90();

EXPORT LPSTR CC Sys_GetCommandLine_48E920();

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd);

EXPORT int Sys_IsAnyKeyDown_48E6C0();

ALIVE_VAR_EXTERN(DWORD, sIsAKeyDown_A8A600);

}

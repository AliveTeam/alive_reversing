#pragma once

#include "FunctionFwd.hpp"
#include "Sys_common.hpp"

namespace AO {

EXPORT TWindowHandleType CC Sys_GetWindowHandle_48E930();

EXPORT void CC Sys_Set_Hwnd_48E340(TWindowHandleType hwnd);

EXPORT s32 CC Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, s32 X, s32 Y, s32 nWidth, s32 nHeight);

EXPORT void SYS_EventsPump_44FF90();

EXPORT LPSTR CC Sys_GetCommandLine_48E920();

void AO_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd);

EXPORT s32 Sys_IsAnyKeyDown_48E6C0();

ALIVE_VAR_EXTERN(u32, sIsAKeyDown_A8A600);

} // namespace AO

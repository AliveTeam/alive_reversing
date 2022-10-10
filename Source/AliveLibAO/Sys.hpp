#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Sys_common.hpp"

namespace AO {

TWindowHandleType Sys_GetWindowHandle_48E930();

void Sys_Set_Hwnd_48E340(TWindowHandleType hwnd);

s32 Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, s32 X, s32 Y, s32 nWidth, s32 nHeight);

void SYS_EventsPump_44FF90();

LPSTR Sys_GetCommandLine_48E920();

void AO_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd);

s32 Sys_IsAnyKeyDown_48E6C0();

extern u32 sIsAKeyDown_A8A600;

} // namespace AO

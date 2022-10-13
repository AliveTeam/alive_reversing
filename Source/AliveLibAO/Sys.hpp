#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Sys_common.hpp"

namespace AO {

TWindowHandleType Sys_GetWindowHandle();

void Sys_Set_Hwnd(TWindowHandleType hwnd);

s32 Sys_WindowClass_Register(LPCSTR lpClassName, LPCSTR lpWindowName, s32 X, s32 Y, s32 nWidth, s32 nHeight);

void SYS_EventsPump();

LPSTR Sys_GetCommandLine();

void AO_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd);

s32 Sys_IsAnyKeyDown();

//extern u32 sIsAKeyDown;

} // namespace AO

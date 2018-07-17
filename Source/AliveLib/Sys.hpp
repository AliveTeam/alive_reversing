#pragma once

#include "FunctionFwd.hpp"

typedef LRESULT(CC* TWindowProcFilter)(HWND, UINT, WPARAM, LPARAM);

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd);
EXPORT LPSTR CC Sys_GetCommandLine_4EE176();
EXPORT void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter);
EXPORT void CC Sys_Set_Hwnd_4F2C50(HWND hwnd);
EXPORT HWND CC Sys_GetHWnd_4F2C70();
EXPORT int CC Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight);
EXPORT HWND CC Sys_GetWindowHandle_4EE180();
EXPORT BOOL CC Sys_IsAnyKeyDown_4EDDF0();
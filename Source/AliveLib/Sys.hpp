#pragma once

#include "FunctionFwd.hpp"

typedef LRESULT(CC* TWindowProcFilter)(HWND, UINT, WPARAM, LPARAM);

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd);
EXPORT LPSTR CC Sys_GetCommandLine_4EE176();
EXPORT void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter);

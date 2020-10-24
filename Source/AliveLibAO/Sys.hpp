#pragma once

#include "FunctionFwd.hpp"
#include "Sys_common.hpp"

START_NS_AO

EXPORT TWindowHandleType CC Sys_GetWindowHandle_48E930();

EXPORT void CC Sys_Set_Hwnd_48E340(TWindowHandleType hwnd);

EXPORT int CC Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, int X, int Y, int nWidth, int nHeight);

END_NS_AO

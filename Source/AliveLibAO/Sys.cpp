#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "Input.hpp"
#include "../AliveLibAE/Sys.hpp"

START_NS_AO

EXPORT TWindowHandleType CC Sys_GetWindowHandle_48E930()
{
    AE_IMPLEMENTED();
    return Sys_GetWindowHandle_4EE180();
}

EXPORT void CC Sys_Set_Hwnd_48E340(TWindowHandleType hwnd)
{
    AE_IMPLEMENTED();
    Sys_Set_Hwnd_4F2C50(hwnd);
}

EXPORT int CC Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, int X, int Y, int nWidth, int nHeight)
{
    AE_IMPLEMENTED();
    return Sys_WindowClass_Register_4EE22F(lpClassName, lpWindowName, X, Y, nWidth, nHeight);
}

END_NS_AO

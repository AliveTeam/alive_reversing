#pragma once

#include "FunctionFwd.hpp"
#include "Sys_common.hpp"

void Sys_SetWindowText(TWindowHandleType windowHandle, const s8* title);
POINT Sys_GetScreenMousePos();
enum class MouseButtons
{
    eLeft,
    eRight
};
bool Sys_IsMouseButtonDown(MouseButtons button);
EXPORT void CC SYS_EventsPump_494580();
void AE_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd);
EXPORT LPSTR CC Sys_GetCommandLine_4EE176();
#if _WIN32
EXPORT void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter);
#endif
EXPORT void CC Sys_Set_Hwnd_4F2C50(TWindowHandleType hwnd);
EXPORT TWindowHandleType CC Sys_GetHWnd_4F2C70();
EXPORT s32 CC Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, s32 x, s32 y, s32 nWidth, s32 nHeight);
EXPORT TWindowHandleType CC Sys_GetWindowHandle_4EE180();
EXPORT BOOL CC Sys_IsAnyKeyDown_4EDDF0();
EXPORT BOOL CC Sys_IsAppActive_4EDF30();
EXPORT s8 CC Sys_PumpMessages_4EE4F4();
#ifdef _WIN32
EXPORT LRESULT CC Sys_WindowMessageHandler_494A40(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
#if ORIGINAL_PS1_BEHAVIOR
EXPORT void setSaveMenuOpen(bool val);
#endif

struct SoundEntry;

ALIVE_VAR_EXTERN(SoundEntry*, sMovieSoundEntry_5CA230);

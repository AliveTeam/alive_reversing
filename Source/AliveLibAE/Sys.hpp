#pragma once

#include "FunctionFwd.hpp"
#include "Sys_common.hpp"

void Sys_SetWindowText(TWindowHandleType windowHandle, const char* title);
POINT Sys_GetScreenMousePos();
enum class MouseButtons
{
    eLeft,
    eRight
};
bool Sys_IsMouseButtonDown(MouseButtons button);
EXPORT void CC SYS_EventsPump_494580();
void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd);
EXPORT LPSTR CC Sys_GetCommandLine_4EE176();
#if _WIN32
EXPORT void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter);
#endif
EXPORT void CC Sys_Set_Hwnd_4F2C50(TWindowHandleType hwnd);
EXPORT TWindowHandleType CC Sys_GetHWnd_4F2C70();
EXPORT int CC Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight);
EXPORT TWindowHandleType CC Sys_GetWindowHandle_4EE180();
EXPORT BOOL CC Sys_IsAnyKeyDown_4EDDF0();
EXPORT BOOL CC Sys_IsAppActive_4EDF30();
EXPORT char CC Sys_PumpMessages_4EE4F4();
#ifdef _WIN32
EXPORT LRESULT CC Sys_WindowMessageHandler_494A40(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

struct SoundEntry;

ALIVE_VAR_EXTERN(SoundEntry*, sMovieSoundEntry_5CA230);

#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Sys_common.hpp"

void Sys_SetWindowText(TWindowHandleType windowHandle, const char_type* title);
POINT Sys_GetScreenMousePos();
enum class MouseButtons
{
    eLeft,
    eRight
};
bool Sys_IsMouseButtonDown(MouseButtons button);
void SYS_EventsPump_494580();
void AE_Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd);
LPSTR Sys_GetCommandLine_4EE176();
#if _WIN32
void Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter);
#endif
void Sys_Set_Hwnd_4F2C50(TWindowHandleType hwnd);
TWindowHandleType Sys_GetHWnd_4F2C70();
s32 Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, s32 x, s32 y, s32 nWidth, s32 nHeight);
TWindowHandleType Sys_GetWindowHandle_4EE180();
Bool32 Sys_IsAnyKeyDown_4EDDF0();
Bool32 Sys_IsAppActive_4EDF30();
s8 Sys_PumpMessages_4EE4F4();
#ifdef _WIN32
LRESULT Sys_WindowMessageHandler_494A40(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
void setSaveMenuOpen(bool val);
#endif

struct SoundEntry;

ALIVE_VAR_EXTERN(SoundEntry*, sMovieSoundEntry_5CA230);

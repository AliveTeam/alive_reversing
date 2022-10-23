#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Sys_common.hpp"

void Sys_SetWindowText(TWindowHandleType windowHandle, const char_type* title);
POINT Sys_GetScreenMousePos();
enum class MouseButtons
{
    eLeft,
    eRight
};
bool Sys_IsMouseButtonDown(MouseButtons button);
void SYS_EventsPump();
void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nShowCmd);
LPSTR Sys_GetCommandLine();
void Sys_Set_Hwnd(TWindowHandleType hwnd);
TWindowHandleType Sys_GetHWnd();
s32 Sys_WindowClass_Register(LPCSTR lpWindowName, s32 x, s32 y, s32 nWidth, s32 nHeight);
TWindowHandleType Sys_GetWindowHandle();
bool Sys_IsAnyKeyDown();
bool Sys_IsAppActive_4EDF30();
s8 Sys_PumpMessages_4EE4F4();

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
void setSaveMenuOpen(bool val);
#endif

struct SoundEntry;

extern SoundEntry* sMovieSoundEntry_5CA230;

#pragma once

#include <string>

struct SDL_Window;
using TWindowHandleType = SDL_Window*;

enum class MessageBoxType
{
    eStandard,
    eError,
    eQuestion,
};

enum class MessageBoxButton
{
    eOK,
    eNo,
    eYes,
};

MessageBoxButton Sys_MessageBox(TWindowHandleType windowHandle, const char_type* message, const char_type* title, MessageBoxType type = MessageBoxType::eStandard);


std::string BuildString();
std::string BuildAndBitnesString();
std::string WindowTitleAO();
std::string WindowTitleAE();
void Alive_Show_ErrorMsg(const char_type* fmt, ...);
u32 SYS_GetTicks();

void Sys_SetWindowText(TWindowHandleType windowHandle, const char_type* title);
enum class MouseButtons
{
    eLeft,
    eRight
};
bool Sys_IsMouseButtonDown(MouseButtons button);
void SYS_EventsPump();
TWindowHandleType Sys_GetHWnd();
void Sys_DestroyWindow();
bool Sys_WindowClass_Register(const char_type* lpWindowName, s32 x, s32 y, s32 nWidth, s32 nHeight, s32 extraAttributes);
TWindowHandleType Sys_GetWindowHandle();
bool Sys_IsAnyKeyDown();
bool Sys_IsAppActive();
s8 Sys_PumpMessages();

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
void setSaveMenuOpen(bool val);
#endif

struct SoundEntry;

extern SoundEntry* gMovieSoundEntry;

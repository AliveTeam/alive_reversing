#pragma once

#include "FunctionFwd.hpp"
#include "config.h"

#if _WIN32
using TWindowProcFilter = LRESULT(CC*)(HWND, UINT, WPARAM, LPARAM);
#endif

#if USE_SDL2
struct SDL_Window;
using TWindowHandleType = SDL_Window*;
#if _WIN32
HWND Sys_Win32FromSDLWindow(TWindowHandleType windowHandle);
#endif
#else
using TWindowHandleType = HWND;
#endif

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

MessageBoxButton CC Sys_MessageBox(TWindowHandleType windowHandle, const char* message, const char* title, MessageBoxType type = MessageBoxType::eStandard);

[[noreturn]] void ALIVE_FATAL(const char* errMsg);

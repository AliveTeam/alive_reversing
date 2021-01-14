#pragma once

#include "FunctionFwd.hpp"
#include "config.h"

#if _WIN32
using TWindowProcFilter = LRESULT(CC*)(HWND, UINT, WPARAM, LPARAM);
#endif

#if USE_SDL2
struct SDL_Window;
using TWindowHandleType = SDL_Window*;
#include "SDL.h"
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
void Sys_Main_Common();

inline void Alive_Show_ErrorMsg(const char* msg)
{
#ifdef BUILD_NUMBER
    // Automated AppVeyor build title
#define TITLE_STR "R.E.L.I.V.E. (AV Build: " BUILD_NUMBER ")"
#else
#define TITLE_STR "R.E.L.I.V.E."
#endif
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITLE_STR, msg, nullptr);
}

[[noreturn]] void ALIVE_FATAL(const char* errMsg);

DWORD SYS_GetTicks();

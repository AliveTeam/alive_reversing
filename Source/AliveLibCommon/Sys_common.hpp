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

inline const char* BuildString()
{
#ifdef BUILD_NUMBER
    // Automated AppVeyor build title
    return "("CI_PROVIDER " Build: " BUILD_NUMBER ")";
#else
    return "";
#endif
}

inline std::string BuildAndBitnesString()
{
    std::string buildAndBitness;
    std::string buildStr = BuildString();
    if (!buildStr.empty())
    {
        buildAndBitness += " ";
        buildAndBitness += buildStr;
    }

    std::string kBitness = sizeof(void*) == 4 ? " (32 bit)" : " (64 bit)";
    buildAndBitness += kBitness;
    return buildAndBitness;
}

inline std::string WindowTitleAO()
{
    return "R.E.L.I.V.E. Oddworld Abe's Oddysee" + BuildAndBitnesString();
}

inline std::string WindowTitleAE()
{
    return "R.E.L.I.V.E. Oddworld Abe's Exoddus" + BuildAndBitnesString();
}

inline void Alive_Show_ErrorMsg(const char* msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (std::string("R.E.L.I.V.E. ") + BuildString()).c_str(), msg, nullptr);
}

[[noreturn]] void ALIVE_FATAL(const char* errMsg);

DWORD SYS_GetTicks();

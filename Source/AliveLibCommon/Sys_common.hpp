#pragma once

#include "relive_config.h"

#if _WIN32
#include <windows.h>
using TWindowProcFilter = LRESULT(__cdecl*)(HWND, UINT, WPARAM, LPARAM);
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

MessageBoxButton Sys_MessageBox(TWindowHandleType windowHandle, const char_type* message, const char_type* title, MessageBoxType type = MessageBoxType::eStandard);
void Sys_Main_Common();

#include <string>

inline std::string BuildString()
{
#ifdef BUILD_NUMBER
    // Automated AppVeyor build title
    return std::string("(") + CI_PROVIDER + " Build: " + std::to_string(BUILD_NUMBER) + ")";
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

void Alive_Show_ErrorMsg(const char_type* msg);

u32 SYS_GetTicks();

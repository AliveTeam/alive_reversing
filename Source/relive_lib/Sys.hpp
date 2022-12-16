#pragma once
#include "relive_config.h"

#if USE_SDL2
struct SDL_Window;
using TWindowHandleType = SDL_Window*;
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

#include "stdafx_common.h"
#include "Sys_common.hpp"

#if USE_SDL2
    #include "SDL.h"
#else
    #include <windows.h>
#endif

[[noreturn]] void ALIVE_FATAL(const char_type* errMsg)
{
    Sys_MessageBox(nullptr, errMsg, "ALIVE Hook fatal error.");
    abort();
}

SYS_time_point_t SYS_GetTime()
{
    return std::chrono::steady_clock::now();
}

u32 SYS_AsU32Ms(SYS_time_point_t t)
{
    return (u32) std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
}

u32 SYS_GetTicks()
{
    return SYS_AsU32Ms(SYS_GetTime());
}

MessageBoxButton CC Sys_MessageBox(TWindowHandleType windowHandle, const char_type* message, const char_type* title, MessageBoxType type)
{
#if USE_SDL2
    SDL_MessageBoxData data = {};
    data.title = title;
    data.message = message;

    if (type == MessageBoxType::eQuestion)
    {
        const static SDL_MessageBoxButtonData buttons[] = {
            {0, 0, "No"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes"},
        };

        data.numbuttons = SDL_arraysize(buttons);
        data.buttons = buttons;
    }
    else
    {
        const static SDL_MessageBoxButtonData buttons[] = {
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK"},
        };

        data.numbuttons = SDL_arraysize(buttons);
        data.buttons = buttons;
    }

    data.window = windowHandle;

    switch (type)
    {
        case MessageBoxType::eStandard:
            data.flags = SDL_MESSAGEBOX_ERROR;
            break;
        case MessageBoxType::eError:
            data.flags = SDL_MESSAGEBOX_INFORMATION;
            break;
        case MessageBoxType::eQuestion:
            data.flags = SDL_MESSAGEBOX_WARNING;
            break;
        default:
            data.flags = SDL_MESSAGEBOX_ERROR;
    }

    s32 button = 0;
    if (SDL_ShowMessageBox(&data, &button) == 0)
    {
        if (type == MessageBoxType::eQuestion)
        {
            if (button == 1)
            {
                return MessageBoxButton::eYes;
            }
            return MessageBoxButton::eNo;
        }
        else
        {
            return MessageBoxButton::eOK;
        }
    }
    return MessageBoxButton::eOK;
#else
    u32 w32type = MB_OK;
    switch (type)
    {
        case MessageBoxType::eStandard:
            w32type = MB_OK;
            break;
        case MessageBoxType::eError:
            w32type = MB_OK | MB_ICONERROR;
            break;
        case MessageBoxType::eQuestion:
            w32type = MB_YESNO | MB_ICONQUESTION;
            break;
    }
    const s32 button = ::MessageBoxA(windowHandle, message, title, w32type);
    switch (button)
    {
        case IDNO:
            return MessageBoxButton::eNo;
        case IDYES:
            return MessageBoxButton::eYes;
        case IDOK:
        default:
            return MessageBoxButton::eOK;
    }
#endif
}

void Sys_Main_Common()
{
#if USE_SDL2
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        LOG_ERROR(SDL_GetError());
        ALIVE_FATAL(SDL_GetError());
    }
#endif
}

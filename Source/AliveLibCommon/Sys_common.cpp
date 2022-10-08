#include "stdafx_common.h"
#include "Sys_common.hpp"
#include "FatalError.hpp"

#if USE_SDL2
    #include "SDL.h"
#else
    #include <windows.h>
#endif


u32 SYS_GetTicks()
{
#if USE_SDL2
    // Using this instead of SDL_GetTicks resolves a weird x64 issue on windows where
    // the tick returned is a lot faster on some machines.
    return static_cast<u32>(SDL_GetPerformanceCounter() / (SDL_GetPerformanceFrequency() / 1000));
#else
    return timeGetTime();
#endif
}

MessageBoxButton Sys_MessageBox(TWindowHandleType windowHandle, const char_type* message, const char_type* title, MessageBoxType type)
{
#if USE_SDL2
    SDL_MessageBoxData data = {};
    data.title = title;
    data.message = message;

    if (type == MessageBoxType::eQuestion)
    {
        const static SDL_MessageBoxButtonData buttons[] = {
            {0, 1, "No"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yes"},
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

    SDL_ShowMessageBox(&data, &button);

    if (type == MessageBoxType::eQuestion)
    {
        if (button == 1)
        {
            return MessageBoxButton::eNo;
        }

        return MessageBoxButton::eYes;
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

#if USE_SDL2
static void PrintSDL2Versions()
{
    SDL_version compiled = {};
    SDL_version linked = {};

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    LOG_INFO("Compiled with SDL2 ver " << static_cast<int>(compiled.major) << "." << static_cast<int>(compiled.minor) << "." << static_cast<int>(compiled.patch));
    LOG_INFO("Runtime SDL2 ver " << static_cast<int>(linked.major) << "." << static_cast<int>(linked.minor) << "." << static_cast<int>(linked.patch));
}
#endif

void Sys_Main_Common()
{
#if USE_SDL2
    PrintSDL2Versions(); // Ok to call before init

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
    {
        LOG_ERROR(SDL_GetError());
        ALIVE_FATAL(SDL_GetError());
    }
#endif
}

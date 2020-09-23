#include "stdafx_common.h"
#include "Sys_common.hpp"

#if USE_SDL2
#include "SDL.h"
#else
#include <windows.h>
#endif

[[noreturn]] void ALIVE_FATAL(const char* errMsg)
{
    Sys_MessageBox(nullptr, errMsg, "ALIVE Hook fatal error.");
    abort();
}

MessageBoxButton CC Sys_MessageBox(TWindowHandleType windowHandle, const char* message, const char* title, MessageBoxType type)
{
#if USE_SDL2
    SDL_MessageBoxData data = {};
    data.title = title;
    data.message = message;

    if (type == MessageBoxType::eQuestion)
    {
        const static SDL_MessageBoxButtonData buttons[] =
        {
            { 0, 0, "No" },
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
        };

        data.numbuttons = SDL_arraysize(buttons);
        data.buttons = buttons;
    }
    else
    {
        const static SDL_MessageBoxButtonData buttons[] =
        {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
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

    int button = 0;
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
    DWORD w32type = MB_OK;
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
    const int button = ::MessageBoxA(windowHandle, message, title, w32type);
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

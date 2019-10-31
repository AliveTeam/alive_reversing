#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "Input.hpp"
#include <assert.h>
#include "../AliveExe/resource.h"

#include "PsxRender.hpp"
#include "Sound.hpp"
#include "DDraw.hpp"
#include "Midi.hpp"
#include "PauseMenu.hpp"

#if USE_SDL2
#include "SDL.h"
#include "SDL_syswm.h"
#include "VGA.hpp"
#endif

#if _WIN32
#include <timeapi.h>
#endif

ALIVE_VAR(1, 0xBBBA00, BOOL, sAppIsActivated_BBBA00, FALSE);
ALIVE_VAR(1, 0xBBB9F4, TWindowHandleType, sHwnd_BBB9F4, nullptr);
#if _WIN32
ALIVE_VAR(1, 0xBBB9F8, TWindowProcFilter, sWindowProcFilter_BBB9F8, nullptr);
#endif
ALIVE_VAR(1, 0xBBB9E8, LPSTR, sCommandLine_BBB9E8, nullptr);
ALIVE_VAR(1, 0xBBB9EC, HINSTANCE, sInstance_BBB9EC, nullptr);
ALIVE_VAR(1, 0xBBB9FC, int, sCmdShow_BBB9FC, 0);
ALIVE_VAR(1, 0xBBFB04, TWindowHandleType, hWnd_BBFB04, nullptr);

EXPORT void CC Sys_Set_Hwnd_4F2C50(TWindowHandleType hwnd)
{
    hWnd_BBFB04 = hwnd;
    // Note: Not setting byte BBE6F8
}

EXPORT TWindowHandleType CC Sys_GetHWnd_4F2C70()
{
    return hWnd_BBFB04;
}

EXPORT BOOL CC Sys_IsAnyKeyDown_4EDDF0()
{
    return sIsAKeyDown_BD309C;
}

#if _WIN32
EXPORT LRESULT CALLBACK Sys_WindowProc_4EE32D(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#if BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    switch (msg)
    {
    case WM_ERASEBKGND:
    {
        RECT rcWin;
        HDC hDC = GetDC(hWnd);
        GetClipBox((HDC)wParam, &rcWin);
        FillRect(hDC, &rcWin, GetSysColorBrush(COLOR_DESKTOP));  // hBrush can be obtained by calling GetWindowLong()
    }
    return TRUE;

    case WM_GETICON:
    case WM_MOUSEACTIVATE:
    case WM_NCLBUTTONDOWN:
    case WM_NCMOUSELEAVE:
    case WM_KILLFOCUS:
    case WM_SETFOCUS:
    case WM_NCHITTEST:
    case WM_ACTIVATE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_NCCALCSIZE:
    case WM_MOVE:
    case WM_WINDOWPOSCHANGED:
    case WM_WINDOWPOSCHANGING:
    case WM_NCMOUSEMOVE:
    case WM_MOUSEMOVE:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    case WM_SETCURSOR:
    {
        // Set the cursor so the resize cursor or whatever doesn't "stick"
        // when we move the mouse over the game window.
        static HCURSOR cur = LoadCursor(0, IDC_ARROW);
        if (cur)
        {
            SetCursor(cur);
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);

    }
    return FALSE;
    }
#endif

    if (sWindowProcFilter_BBB9F8)
    {
        const LRESULT filterRet = sWindowProcFilter_BBB9F8(hWnd, msg, wParam, lParam);
        if (filterRet > 0)
        {
            return ::DefWindowProcA(hWnd, msg, wParam, lParam);
        }
        else if (filterRet < 0)
        {
            return -1 - filterRet;
        }
    }

    switch (msg)
    {
    case WM_SETCURSOR:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT paint = {};
            BeginPaint(hWnd, &paint);
            EndPaint(hWnd, &paint);
        }
        break;

    case WM_ACTIVATEAPP:
        sAppIsActivated_BBBA00 = static_cast<BOOL>(wParam);
        break;

    case WM_KEYDOWN:
        if (!Input_GetInputEnabled_4EDDE0())
        {
            // Store the ASCII of a single key press. Used for typing in text for save names etc.
            sIsAKeyDown_BD309C = TRUE;
            BYTE KeyState[256] = {};
            ::GetKeyboardState(KeyState);

            const UINT vKey = static_cast<UINT>(wParam);
            const UINT scanCode = HIWORD(lParam);
            char translated[4] = {};
            // TODO: can be negative but is never checked
            const int numBytesWritten = ::ToAscii(vKey, scanCode, KeyState, reinterpret_cast<WORD*>(&translated), 0);
            translated[numBytesWritten] = 0;
            ::CharToOemA(translated, translated);
            sLastPressedKey_BD30A0 = translated[0];
        }
        break;

    case WM_KEYUP:
        sIsAKeyDown_BD309C = 0;
        sLastPressedKey_BD30A0 = 0;
        break;
    }
    return ::DefWindowProcA(hWnd, msg, wParam, lParam);
}
#endif

#if USE_SDL2
#if _WIN32
HWND Sys_Win32FromSDLWindow(TWindowHandleType windowHandle)
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(windowHandle, &wmInfo);
    return wmInfo.info.win.window;
}
#endif
#endif

MessageBoxButton Sys_MessageBox(TWindowHandleType windowHandle, const char* message, const char* title, MessageBoxType type)
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

void Sys_SetWindowText(TWindowHandleType windowHandle, const char* title)
{
#if USE_SDL2
    SDL_SetWindowTitle(windowHandle, title);
#else
    ::SetWindowText(windowHandle, title);
#endif
}

POINT Sys_GetScreenMousePos()
{
#if USE_SDL2
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&x, &y);
    return {x ,y};
#else
    HWND windowHandle = Sys_GetWindowHandle_4EE180();
    POINT mousePos;
    RECT r;
    GetClientRect(windowHandle, &r);
    GetCursorPos(&mousePos);
    ScreenToClient(windowHandle, &mousePos);
    return mousePos;
#endif
}

bool Sys_IsMouseButtonDown(MouseButtons button)
{
    if (button == MouseButtons::eRight)
    {
        return !!(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT));
    }
    return !!(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT));
}

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
    sInstance_BBB9EC = hInstance;
    sCmdShow_BBB9FC = nShowCmd;
    sCommandLine_BBB9E8 = lpCmdLine;
}

#if _WIN32
EXPORT void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter)
{
    sWindowProcFilter_BBB9F8 = pFilter;
}
#endif

ALIVE_VAR(1, 0x5CA230, SoundEntry*, sSoundEntry_5CA230, nullptr);

#if _WIN32
EXPORT LRESULT CC Sys_WindowMessageHandler_494A40(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;

    switch (msg)
    {
    case WM_PAINT:
    {
        RECT rect = {};
        PAINTSTRUCT paint = {};
        BeginPaint(hWnd, &paint);
        GetClientRect(hWnd, &rect);
        PatBlt(paint.hdc, 0, 0, rect.right, rect.bottom, 0x42u); // TODO: Constants
        EndPaint(hWnd, &paint);
        Add_Dirty_Area_4ED970(0, 0, 640, 240);
    }
    return 1;

    case WM_CLOSE:
        sDDraw_BBC3D4->FlipToGDISurface();

#if !USE_SDL2_SOUND
        if (sSoundEntry_5CA230)
        {
            LPDIRECTSOUNDBUFFER pDSoundBuffer = sSoundEntry_5CA230->field_4_pDSoundBuffer;
            if (pDSoundBuffer)
            {
                pDSoundBuffer->Stop();
            }
        }
#endif

        if (SND_Seq_Table_Valid_4CAFE0())
        {
            SND_StopAll_4CB060();
        }

        ret = -(MessageBoxA(hWnd, "Do you really want to quit?", "Abe's Exoddus 1.0", 0x124u) != 6); // TODO: Constants, refactor negation


        if (SND_Seq_Table_Valid_4CAFE0())
        {
            SND_Restart_4CB0E0();
        }

        if (!sSoundEntry_5CA230)
        {
            return ret;
        }

        if (!sSoundEntry_5CA230->field_4_pDSoundBuffer)
        {
            return ret;
        }

#if !USE_SDL2_SOUND
        sSoundEntry_5CA230->field_4_pDSoundBuffer->Play(0, 0, 1);
#endif
        return ret;

    case WM_SETCURSOR:
        SetCursor(0);
        return -1;

    case WM_NCLBUTTONDOWN:
        return -1;

    case WM_KEYFIRST:
        if (wParam == VK_F5)
        {
            sQuicksave_SaveNextFrame_5CA4D8 = 1;
        }
        else if (wParam == VK_F6)
        {
            sQuicksave_LoadNextFrame_5CA4D9 = 1;
            Input_SetKeyState_4EDD80(117, 1);
            return 0;
        }
        Input_SetKeyState_4EDD80(static_cast<int>(wParam), 1);
        return 0;

    case WM_KEYUP:
        Input_SetKeyState_4EDD80(static_cast<int>(wParam), 0);
        return 1;

    case WM_ACTIVATE:
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
    case WM_ENTERMENULOOP:
    case WM_EXITMENULOOP:
    case WM_ENTERSIZEMOVE:
    case WM_EXITSIZEMOVE:
        Input_InitKeyStateArray_4EDD60();
        break;

    case WM_INITMENUPOPUP:
        // TODO: Constants for wParam
        if ((unsigned int)lParam >> 16)
        {
            return -1;
        }
        return ret;

    case WM_SYSKEYDOWN:
        // TODO: Constants for wParam
        if (wParam == 18 || wParam == 32)
        {
            ret = -1;
        }
        Input_SetKeyState_4EDD80(static_cast<int>(wParam), 1);
        break;

    case WM_SYSKEYUP:
        // TODO: Constants for wParam
        if (wParam == 18 || wParam == 32)
        {
            ret = -1;
        }
        Input_SetKeyState_4EDD80(static_cast<int>(wParam), 0);
        break;

    case WM_TIMER:
        return 1;

    default:
        return ret;
    }
    return ret;
}
#endif

#if USE_SDL2
static int sdl_key_to_win32_vkey(SDL_Keycode key)
{
    switch (key)
    {
    default:                      return 0;
    case SDLK_UNKNOWN:            return 0;
    case SDLK_a:                  return 0x41;
    case SDLK_b:                  return 0x42;
    case SDLK_c:                  return 0x43;
    case SDLK_d:                  return 0x44;
    case SDLK_e:                  return 0x45;
    case SDLK_f:                  return 0x46;
    case SDLK_g:                  return 0x47;
    case SDLK_h:                  return 0x48;
    case SDLK_i:                  return 0x49;
    case SDLK_j:                  return 0x4A;
    case SDLK_k:                  return 0x4B;
    case SDLK_l:                  return 0x4C;
    case SDLK_m:                  return 0x4D;
    case SDLK_n:                  return 0x4E;
    case SDLK_o:                  return 0x4F;
    case SDLK_p:                  return 0x50;
    case SDLK_q:                  return 0x51;
    case SDLK_r:                  return 0x52;
    case SDLK_s:                  return 0x53;
    case SDLK_t:                  return 0x54;
    case SDLK_u:                  return 0x55;
    case SDLK_v:                  return 0x56;
    case SDLK_w:                  return 0x57;
    case SDLK_x:                  return 0x58;
    case SDLK_y:                  return 0x59;
    case SDLK_z:                  return 0x5A;
    case SDLK_1:                  return VK_NUMPAD1;
    case SDLK_2:                  return VK_NUMPAD2;
    case SDLK_3:                  return VK_NUMPAD3;
    case SDLK_4:                  return VK_NUMPAD4;
    case SDLK_5:                  return VK_NUMPAD5;
    case SDLK_6:                  return VK_NUMPAD6;
    case SDLK_7:                  return VK_NUMPAD7;
    case SDLK_8:                  return VK_NUMPAD8;
    case SDLK_9:                  return VK_NUMPAD9;
    case SDLK_0:                  return VK_NUMPAD0;
    case SDLK_RETURN:             return VK_RETURN;
    case SDLK_ESCAPE:             return VK_ESCAPE;
    case SDLK_BACKSPACE:          return VK_BACK;
    case SDLK_TAB:                return VK_TAB;
    case SDLK_SPACE:              return VK_SPACE;
    case SDLK_MINUS:              return 0;
    case SDLK_EQUALS:             return 0;
    case SDLK_LEFTBRACKET:        return 0;
    case SDLK_RIGHTBRACKET:       return 0;
    case SDLK_BACKSLASH:          return 0;
    case SDLK_SEMICOLON:          return 0;
    case SDLK_QUOTE:              return 0;
    case SDLK_BACKQUOTE:          return VK_OEM_3; // Console key
    case SDLK_COMMA:              return VK_OEM_COMMA;
    case SDLK_PERIOD:             return VK_OEM_PERIOD;
    case SDLK_SLASH:              return VK_OEM_102;
    case SDLK_CAPSLOCK:           return VK_CAPITAL;
    case SDLK_F1:                 return VK_F1;
    case SDLK_F2:                 return VK_F2;
    case SDLK_F3:                 return VK_F3;
    case SDLK_F4:                 return VK_F4;
    case SDLK_F5:                 return VK_F5;
    case SDLK_F6:                 return VK_F6;
    case SDLK_F7:                 return VK_F7;
    case SDLK_F8:                 return VK_F8;
    case SDLK_F9:                 return VK_F9;
    case SDLK_F10:                return VK_F10;
    case SDLK_F11:                return VK_F11;
    case SDLK_F12:                return VK_F12;
    case SDLK_PRINTSCREEN:        return VK_PRINT;
    case SDLK_SCROLLLOCK:         return VK_SCROLL;
    case SDLK_PAUSE:              return VK_PAUSE;
    case SDLK_INSERT:             return VK_INSERT;
    case SDLK_HOME:               return VK_HOME;
    case SDLK_PAGEUP:             return VK_PRIOR;
    case SDLK_DELETE:             return VK_DELETE;
    case SDLK_END:                return VK_END;
    case SDLK_PAGEDOWN:           return VK_NEXT;
    case SDLK_RIGHT:              return VK_RIGHT;
    case SDLK_LEFT:               return VK_LEFT;
    case SDLK_DOWN:               return VK_DOWN;
    case SDLK_UP:                 return VK_UP;
    case SDLK_NUMLOCKCLEAR:       return 0;
    case SDLK_KP_DIVIDE:          return 0;
    case SDLK_KP_MULTIPLY:        return 0;
    case SDLK_KP_MINUS:           return 0;
    case SDLK_KP_PLUS:            return 0;
    case SDLK_KP_ENTER:           return VK_RETURN;
    case SDLK_KP_1:               return VK_NUMPAD1;
    case SDLK_KP_2:               return VK_NUMPAD2;
    case SDLK_KP_3:               return VK_NUMPAD3;
    case SDLK_KP_4:               return VK_NUMPAD4;
    case SDLK_KP_5:               return VK_NUMPAD5;
    case SDLK_KP_6:               return VK_NUMPAD6;
    case SDLK_KP_7:               return VK_NUMPAD7;
    case SDLK_KP_8:               return VK_NUMPAD8;
    case SDLK_KP_9:               return VK_NUMPAD9;
    case SDLK_KP_0:               return VK_NUMPAD0;
    case SDLK_KP_PERIOD:          return 0;
    case SDLK_APPLICATION:        return 0;
    case SDLK_POWER:              return 0;
    case SDLK_KP_EQUALS:          return 0;
    case SDLK_F13:                return 0;
    case SDLK_F14:                return 0;
    case SDLK_F15:                return 0;
    case SDLK_F16:                return 0;
    case SDLK_F17:                return 0;
    case SDLK_F18:                return 0;
    case SDLK_F19:                return 0;
    case SDLK_F20:                return 0;
    case SDLK_F21:                return 0;
    case SDLK_F22:                return 0;
    case SDLK_F23:                return 0;
    case SDLK_F24:                return 0;
    case SDLK_EXECUTE:            return 0;
    case SDLK_HELP:               return 0;
    case SDLK_MENU:               return 0;
    case SDLK_SELECT:             return 0;
    case SDLK_STOP:               return 0;
    case SDLK_AGAIN:              return 0;
    case SDLK_UNDO:               return 0;
    case SDLK_CUT:                return 0;
    case SDLK_COPY:               return 0;
    case SDLK_PASTE:              return 0;
    case SDLK_FIND:               return 0;
    case SDLK_MUTE:               return 0;
    case SDLK_VOLUMEUP:           return 0;
    case SDLK_VOLUMEDOWN:         return 0;
    case SDLK_KP_COMMA:           return 0;
    case SDLK_KP_EQUALSAS400:     return 0;
    case SDLK_ALTERASE:           return 0;
    case SDLK_SYSREQ:             return 0;
    case SDLK_CANCEL:             return 0;
    case SDLK_CLEAR:              return 0;
    case SDLK_PRIOR:              return 0;
    case SDLK_RETURN2:            return 0;
    case SDLK_SEPARATOR:          return 0;
    case SDLK_OUT:                return 0;
    case SDLK_OPER:               return 0;
    case SDLK_CLEARAGAIN:         return 0;
    case SDLK_CRSEL:              return 0;
    case SDLK_EXSEL:              return 0;
    case SDLK_KP_00:              return 0;
    case SDLK_KP_000:             return 0;
    case SDLK_THOUSANDSSEPARATOR: return 0;
    case SDLK_DECIMALSEPARATOR:   return 0;
    case SDLK_CURRENCYUNIT:       return 0;
    case SDLK_CURRENCYSUBUNIT:    return 0;
    case SDLK_KP_LEFTPAREN:       return 0;
    case SDLK_KP_RIGHTPAREN:      return 0;
    case SDLK_KP_LEFTBRACE:       return 0;
    case SDLK_KP_RIGHTBRACE:      return 0;
    case SDLK_KP_TAB:             return 0;
    case SDLK_KP_BACKSPACE:       return 0;
    case SDLK_KP_A:               return 0;
    case SDLK_KP_B:               return 0;
    case SDLK_KP_C:               return 0;
    case SDLK_KP_D:               return 0;
    case SDLK_KP_E:               return 0;
    case SDLK_KP_F:               return 0;
    case SDLK_KP_XOR:             return 0;
    case SDLK_KP_POWER:           return 0;
    case SDLK_KP_PERCENT:         return 0;
    case SDLK_KP_LESS:            return 0;
    case SDLK_KP_GREATER:         return 0;
    case SDLK_KP_AMPERSAND:       return 0;
    case SDLK_KP_DBLAMPERSAND:    return 0;
    case SDLK_KP_VERTICALBAR:     return 0;
    case SDLK_KP_DBLVERTICALBAR:  return 0;
    case SDLK_KP_COLON:           return 0;
    case SDLK_KP_HASH:            return 0;
    case SDLK_KP_SPACE:           return 0;
    case SDLK_KP_AT:              return 0;
    case SDLK_KP_EXCLAM:          return 0;
    case SDLK_KP_MEMSTORE:        return 0;
    case SDLK_KP_MEMRECALL:       return 0;
    case SDLK_KP_MEMCLEAR:        return 0;
    case SDLK_KP_MEMADD:          return 0;
    case SDLK_KP_MEMSUBTRACT:     return 0;
    case SDLK_KP_MEMMULTIPLY:     return 0;
    case SDLK_KP_MEMDIVIDE:       return 0;
    case SDLK_KP_PLUSMINUS:       return 0;
    case SDLK_KP_CLEAR:           return 0;
    case SDLK_KP_CLEARENTRY:      return 0;
    case SDLK_KP_BINARY:          return 0;
    case SDLK_KP_OCTAL:           return 0;
    case SDLK_KP_DECIMAL:         return 0;
    case SDLK_KP_HEXADECIMAL:     return 0;
    case SDLK_LCTRL:              return VK_CONTROL;
    case SDLK_LSHIFT:             return VK_SHIFT;
    case SDLK_LALT:               return VK_MENU;
    case SDLK_LGUI:               return VK_LWIN;
    case SDLK_RCTRL:              return VK_CONTROL;
    case SDLK_RSHIFT:             return VK_SHIFT;
    case SDLK_RALT:               return VK_MENU;
    case SDLK_RGUI:               return VK_RWIN;
    case SDLK_MODE:               return 0;
    case SDLK_AUDIONEXT:          return 0;
    case SDLK_AUDIOPREV:          return 0;
    case SDLK_AUDIOSTOP:          return 0;
    case SDLK_AUDIOPLAY:          return 0;
    case SDLK_AUDIOMUTE:          return 0;
    case SDLK_MEDIASELECT:        return 0;
    case SDLK_WWW:                return 0;
    case SDLK_MAIL:               return 0;
    case SDLK_CALCULATOR:         return 0;
    case SDLK_COMPUTER:           return 0;
    case SDLK_AC_SEARCH:          return 0;
    case SDLK_AC_HOME:            return 0;
    case SDLK_AC_BACK:            return 0;
    case SDLK_AC_FORWARD:         return 0;
    case SDLK_AC_STOP:            return 0;
    case SDLK_AC_REFRESH:         return 0;
    case SDLK_AC_BOOKMARKS:       return 0;
    case SDLK_BRIGHTNESSDOWN:     return 0;
    case SDLK_BRIGHTNESSUP:       return 0;
    case SDLK_DISPLAYSWITCH:      return 0;
    case SDLK_KBDILLUMTOGGLE:     return 0;
    case SDLK_KBDILLUMDOWN:       return 0;
    case SDLK_KBDILLUMUP:         return 0;
    case SDLK_EJECT:              return 0;
    case SDLK_SLEEP:              return 0;
    case SDL_NUM_SCANCODES:       return 0;
    }
}

// This is a combination of the window proc and the window proc "filter"

static bool bNeedToQuit = false;

static int Sys_EventFilter(void* /*userData*/, SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (!Input_GetInputEnabled_4EDDE0())
        {
            // "Typing" input
            sLastPressedKey_BD30A0 = sdl_key_to_win32_vkey(event->key.keysym.sym);

            // Between A-Z
            if (sLastPressedKey_BD30A0 >= 0x41 && sLastPressedKey_BD30A0 <= 0x5A)
            {
                sLastPressedKey_BD30A0 -= 0x41;

                if (SDL_GetModState() & (KMOD_SHIFT | KMOD_CAPS))
                {
                    sLastPressedKey_BD30A0 += 'A';
                }
                else
                {
                    sLastPressedKey_BD30A0 += 'a';
                }
            }
            // Between 0-9
            else if (sLastPressedKey_BD30A0 >= VK_NUMPAD0 && sLastPressedKey_BD30A0 <= VK_NUMPAD9)
            {
                sLastPressedKey_BD30A0 -= VK_NUMPAD0;
                if (SDL_GetModState() & (KMOD_SHIFT | KMOD_CAPS))
                {
                    sLastPressedKey_BD30A0 += '!';
                }
                else
                {
                    sLastPressedKey_BD30A0 += '0';
                }
            }

            sIsAKeyDown_BD309C = TRUE;
        }
        else
        {
            // "Game button" input
            const int vk = sdl_key_to_win32_vkey(event->key.keysym.sym);
            Input_SetKeyState_4EDD80(vk, 1);

            if (Input_IsVKPressed_4EDD40(VK_F5))
            {
                sQuicksave_SaveNextFrame_5CA4D8 = 1;
            }
            else if (Input_IsVKPressed_4EDD40(VK_F6))
            {
                sQuicksave_LoadNextFrame_5CA4D9 = 1;
            }
            else if (Input_IsVKPressed_4EDD40(VK_F12))
            {
                const Uint32 flags = SDL_GetWindowFlags(Sys_GetWindowHandle_4EE180());
                if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
                {
                    SDL_SetWindowFullscreen(Sys_GetWindowHandle_4EE180(), 0);
                }
                else
                {
                    SDL_SetWindowFullscreen(Sys_GetWindowHandle_4EE180(), SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
        }
    }
    else if (event->type == SDL_KEYUP)
    {
        const int vk = sdl_key_to_win32_vkey(event->key.keysym.sym);
        Input_SetKeyState_4EDD80(vk, 0);
        sIsAKeyDown_BD309C = FALSE;
        sLastPressedKey_BD30A0 = 0;
    }
    else if (event->type == SDL_WINDOWEVENT)
    {
        if (event->window.type == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
            sAppIsActivated_BBBA00 = TRUE;
        }
        else if (event->window.type == SDL_WINDOWEVENT_FOCUS_LOST)
        {
            sAppIsActivated_BBBA00 = FALSE;
        }
        else if (event->window.type == SDL_WINDOWEVENT_EXPOSED)
        {
            Add_Dirty_Area_4ED970(0, 0, 640, 240);
        }
    }
    else if (event->type == SDL_QUIT)
    {
        if (sSoundEntry_5CA230)
        {
#if !USE_SDL2_SOUND
            LPDIRECTSOUNDBUFFER pDSoundBuffer = sSoundEntry_5CA230->field_4_pDSoundBuffer;
            if (pDSoundBuffer)
            {
                pDSoundBuffer->Stop();
            }
#endif
        }
        if (SND_Seq_Table_Valid_4CAFE0())
        {
            SND_StopAll_4CB060();
        }

        // Full screen message boxes act really strange.. so force window mode before we show it
        const Uint32 flags = SDL_GetWindowFlags(Sys_GetWindowHandle_4EE180());
        bool forcedWindowMode = false;
        if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
        {
            forcedWindowMode = true;
            SDL_SetWindowFullscreen(Sys_GetWindowHandle_4EE180(), 0);
            if (sPsxEMU_show_vram_BD1465)
            {
                VGA_CopyToFront_4F3710(&sPsxVram_C1D160, nullptr);
            }
            else
            {
                RECT rect = { 0,0, 640, 240 };
                VGA_CopyToFront_4F3710(&sPsxVram_C1D160, &rect);
            }
        }

        const MessageBoxButton button = Sys_MessageBox(Sys_GetWindowHandle_4EE180(), "Do you really want to quit?", "Abe's Exoddus 1.0", MessageBoxType::eQuestion);
        if (SND_Seq_Table_Valid_4CAFE0())
        {
            SND_Restart_4CB0E0();
        }

#if !USE_SDL2_SOUND
        if (sSoundEntry_5CA230 && sSoundEntry_5CA230->field_4_pDSoundBuffer)
        {
            sSoundEntry_5CA230->field_4_pDSoundBuffer->Play(0, 0, 1);
        }
#endif

        if (button == MessageBoxButton::eYes)
        {
            // So Sys_PumpMessages_4EE4F4 thinks we got an quit
            bNeedToQuit = true;
        }
        else
        {
            if (forcedWindowMode)
            {
                SDL_SetWindowFullscreen(Sys_GetWindowHandle_4EE180(), SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }
    }
    return 0;
}
#endif

EXPORT BOOL CC Sys_IsAppActive_4EDF30()
{
    return sAppIsActivated_BBBA00;
}

EXPORT char CC Sys_PumpMessages_4EE4F4()
{
#if USE_SDL2
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        // Pump events
    }

    if (bNeedToQuit)
    {
        return 1;
    }

    return 0;
#else
    MSG msg = {};
    unsigned int paintMessageCount = 0;
    while (::PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return 1;
        }

        // I guess this stops the game hanging from paint request spam, seems like a hack.
        if (msg.message == WM_PAINT && ++paintMessageCount >= 10)
        {
            break;
        }

        if (msg.message != WM_SYSKEYDOWN || msg.wParam != 32)
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
            if (msg.message == WM_QUIT)
            {
                return 1;
            }
        }
    }
    return 0;
#endif
}

EXPORT TWindowHandleType CC Sys_GetWindowHandle_4EE180()
{
    return sHwnd_BBB9F4;
}

EXPORT LPSTR CC Sys_GetCommandLine_4EE176()
{
    return sCommandLine_BBB9E8;
}

EXPORT void CC Sys_SetWindowPos_4EE1B1(int width, int height)
{
#if USE_SDL2
    SDL_SetWindowSize(Sys_GetWindowHandle_4EE180(), width, height);
    SDL_SetWindowPosition(Sys_GetWindowHandle_4EE180(), 0, 0);
#else
    RECT clientRect = {};
    ::SetWindowPos(Sys_GetWindowHandle_4EE180(), HWND_TOPMOST, 0, 0, width, height, SWP_NOREPOSITION | SWP_NOZORDER);
    ::GetClientRect(Sys_GetWindowHandle_4EE180(), &clientRect);
    if (width != clientRect.right || height != clientRect.bottom)
    {
        ::SetWindowPos(Sys_GetWindowHandle_4EE180(), HWND_TOPMOST, 0, 0, width - clientRect.right + width, height - clientRect.bottom + height, SWP_NOREPOSITION | SWP_NOZORDER);
    }
#endif
}

#if USE_SDL2
static int CC Sys_WindowClass_Register_SDL(LPCSTR /*lpClassName*/, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight)
{
#if __ANDROID__
    SDL_Rect gScreenRect = { 0, 0, 640, 480 };
    SDL_DisplayMode displayMode;
    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
    {
        gScreenRect.w = displayMode.w;
        gScreenRect.h = displayMode.h;
    }

    sHwnd_BBB9F4 = SDL_CreateWindow(lpWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
#else
    sHwnd_BBB9F4 = SDL_CreateWindow(lpWindowName, x, y, nWidth, nHeight, SDL_WINDOW_RESIZABLE);
#endif

    Input_InitKeyStateArray_4EDD60();

    SDL_AddEventWatch(Sys_EventFilter, nullptr);

    SDL_ShowCursor(SDL_DISABLE);

    // Bring to front and give input focus
    SDL_RaiseWindow(sHwnd_BBB9F4);

    // SDL will not send a window focused message on start up, so default to activated
    sAppIsActivated_BBBA00 = TRUE;

    return 0;
}
#else
static int CC Sys_WindowClass_Register_Win32(LPCSTR lpClassName, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight)
{
    WNDCLASSA windowClass = {};
    windowClass.style = CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc = Sys_WindowProc_4EE32D;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = sInstance_BBB9EC;
    if (IsAlive())
    {
        windowClass.hIcon = ::LoadIconA(sInstance_BBB9EC, IDI_APPLICATION);
    }
    else
    {
        windowClass.hIcon = ::LoadIconA(sInstance_BBB9EC, MAKEINTRESOURCE(IDI_MAIN_ICON));
    }
    windowClass.hCursor = ::LoadCursorA(sInstance_BBB9EC, IDC_WAIT);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = lpClassName;
    windowClass.lpszClassName = lpClassName;
    ::RegisterClassA(&windowClass);

    DWORD style = WS_CAPTION | WS_VISIBLE;
#if BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    style |= WS_OVERLAPPEDWINDOW;
#endif

    const HWND hWnd = ::CreateWindowExA(
        0, // ExStyle
        lpClassName,
        lpWindowName,
        style,
        x,
        y,
        nWidth,
        nHeight,
        0, // hWndParent
        0, // hMenu
        sInstance_BBB9EC,
        0); // lpParam

    if (!hWnd)
    {
        return -1;
    }

    sHwnd_BBB9F4 = hWnd;

#if BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
    Sys_SetWindowPos_4EE1B1(nWidth, nHeight);
#else
    RECT rc;
    SetRect(&rc, 0, 0, nWidth, nHeight);
    AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW | WS_VISIBLE, TRUE, 0);
    SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
#endif

    ::ShowWindow(hWnd, sCmdShow_BBB9FC);
    ::UpdateWindow(hWnd);
    ::ShowCursor(TRUE);
    return 0;
}
#endif

EXPORT int CC Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight)
{
#if USE_SDL2
    return Sys_WindowClass_Register_SDL(lpClassName, lpWindowName, x, y, nWidth, nHeight);
#else
    return Sys_WindowClass_Register_Win32(lpClassName, lpWindowName, x, y, nWidth, nHeight);
#endif
}

DWORD SYS_GetTicks()
{
#if USE_SDL2
    // Using this instead of SDL_GetTicks resolves a weird x64 issue on windows where
    // the tick returned is a lot faster on some machines.
    return static_cast<DWORD>(SDL_GetPerformanceCounter() / (SDL_GetPerformanceFrequency() / 1000));
#else
    return timeGetTime();
#endif
}

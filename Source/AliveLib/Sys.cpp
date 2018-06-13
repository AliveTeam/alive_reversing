#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "Input.hpp"
#include <assert.h>

#define SYS_IMPL true

typedef LRESULT(CC* TWindowProcFilter)(HWND, UINT, WPARAM, LPARAM);

ALIVE_VAR(1, 0xBBBA00, BOOL, sAppIsActivated_BBBA00, FALSE);
ALIVE_VAR(1, 0xBBB9F4, HWND, sHwnd_BBB9F4, nullptr);
ALIVE_VAR(1, 0xBBB9F8, TWindowProcFilter, sWindowProcFilter_BBB9F8, nullptr);
ALIVE_VAR(1, 0xBBB9E8, LPSTR, sCommandLine_BBB9E8, nullptr);
ALIVE_VAR(1, 0xBBB9EC, HINSTANCE, sInstance_BBB9EC, nullptr);
ALIVE_VAR(1, 0xBBB9FC, int, sCmdShow_BBB9FC, 0);
ALIVE_VAR(1, 0xBD309C, BOOL, sIsAKeyDown_BD309C, FALSE);
ALIVE_VAR(1, 0xBD30A0, int, sLastPressedKey_BD30A0, 0);

BOOL CC Sys_IsAnyKeyDown_4EDDF0()
{
    return sIsAKeyDown_BD309C;
}
ALIVE_FUNC_IMPLEX(0x4EDDF0, Sys_IsAnyKeyDown_4EDDF0, SYS_IMPL);

LRESULT CALLBACK Sys_WindowProc_4EE32D(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
        sAppIsActivated_BBBA00 = wParam;
        break;

    case WM_KEYDOWN:
        if (!Input_GetInputEnabled_4EDDE0())
        {
            // Store the ASCII of a single key press. Used for typing in text for save names etc.
            sIsAKeyDown_BD309C = TRUE;
            BYTE KeyState[256] = {};
            ::GetKeyboardState(KeyState);

            const UINT vKey = wParam;
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
ALIVE_FUNC_IMPLEX(0x4EE32D, Sys_WindowProc_4EE32D, SYS_IMPL);

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
    sInstance_BBB9EC = hInstance;
    sCmdShow_BBB9FC = nShowCmd;
    sCommandLine_BBB9E8 = lpCmdLine;
}

void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter)
{
    sWindowProcFilter_BBB9F8 = pFilter;
}
ALIVE_FUNC_IMPLEX(0x4EE197, Sys_SetWindowProc_Filter_4EE197, SYS_IMPL);

BOOL CC SYS_IsAppActive_4EDF30()
{
    return sAppIsActivated_BBBA00;
}
ALIVE_FUNC_IMPLEX(0x4EDF30, SYS_IsAppActive_4EDF30, SYS_IMPL);

HWND CC Sys_GetWindowHandle_4EE180()
{
    return sHwnd_BBB9F4;
}
ALIVE_FUNC_IMPLEX(0x4EE180, Sys_GetWindowHandle_4EE180, SYS_IMPL);

LPSTR CC Sys_GetCommandLine_4EE176()
{
    return sCommandLine_BBB9E8;
}
ALIVE_FUNC_IMPLEX(0x4EE176, Sys_GetCommandLine_4EE176, SYS_IMPL);

void CC Sys_SetWindowPos_4EE1B1(int width, int height)
{
    RECT clientRect = {};
    ::SetWindowPos(Sys_GetWindowHandle_4EE180(), HWND_TOPMOST, 0, 0, width, height, SWP_NOREPOSITION | SWP_NOZORDER);
    ::GetClientRect(Sys_GetWindowHandle_4EE180(), &clientRect);
    if (width != clientRect.right || height != clientRect.bottom)
    {
        ::SetWindowPos(Sys_GetWindowHandle_4EE180(), HWND_TOPMOST, 0, 0, width - clientRect.right + width, height - clientRect.bottom + height, SWP_NOREPOSITION | SWP_NOZORDER);
    }
}
ALIVE_FUNC_IMPLEX(0x4EE1B1, Sys_SetWindowPos_4EE1B1, SYS_IMPL);

int CC Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight)
{
    WNDCLASSA windowClass = {};
    windowClass.style = CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc = Sys_WindowProc_4EE32D;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = sInstance_BBB9EC;
    windowClass.hIcon = ::LoadIconA(sInstance_BBB9EC, IDI_APPLICATION);
    windowClass.hCursor = ::LoadCursorA(sInstance_BBB9EC, IDC_WAIT);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = lpClassName;
    windowClass.lpszClassName = lpClassName;
    ::RegisterClassA(&windowClass);

    //const DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    const DWORD style = WS_CAPTION | WS_VISIBLE;

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
    Sys_SetWindowPos_4EE1B1(nWidth, nHeight);
    ::ShowWindow(hWnd, sCmdShow_BBB9FC);
    ::UpdateWindow(hWnd);
    ::ShowCursor(TRUE);
    return 0;
}
ALIVE_FUNC_IMPLEX(0x4EE22F, Sys_WindowClass_Register_4EE22F, SYS_IMPL);

#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "Input.hpp"
#include <assert.h>
#include "../AliveExe/resource.h"

ALIVE_VAR(1, 0xBBBA00, BOOL, sAppIsActivated_BBBA00, FALSE);
ALIVE_VAR(1, 0xBBB9F4, HWND, sHwnd_BBB9F4, nullptr);
ALIVE_VAR(1, 0xBBB9F8, TWindowProcFilter, sWindowProcFilter_BBB9F8, nullptr);
ALIVE_VAR(1, 0xBBB9E8, LPSTR, sCommandLine_BBB9E8, nullptr);
ALIVE_VAR(1, 0xBBB9EC, HINSTANCE, sInstance_BBB9EC, nullptr);
ALIVE_VAR(1, 0xBBB9FC, int, sCmdShow_BBB9FC, 0);
ALIVE_VAR(1, 0xBBFB04, HWND, hWnd_BBFB04, nullptr);

EXPORT void CC Sys_Set_Hwnd_4F2C50(HWND hwnd)
{
    hWnd_BBFB04 = hwnd;
    // Note: Not setting byte BBE6F8
}

EXPORT HWND CC Sys_GetHWnd_4F2C70()
{
    return hWnd_BBFB04;
}

EXPORT BOOL CC Sys_IsAnyKeyDown_4EDDF0()
{
    return sIsAKeyDown_BD309C;
}

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

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
    sInstance_BBB9EC = hInstance;
    sCmdShow_BBB9FC = nShowCmd;
    sCommandLine_BBB9E8 = lpCmdLine;
}

EXPORT void CC Sys_SetWindowProc_Filter_4EE197(TWindowProcFilter pFilter)
{
    sWindowProcFilter_BBB9F8 = pFilter;
}

EXPORT BOOL CC SYS_IsAppActive_4EDF30()
{
    return sAppIsActivated_BBBA00;
}

EXPORT HWND CC Sys_GetWindowHandle_4EE180()
{
    return sHwnd_BBB9F4;
}

EXPORT LPSTR CC Sys_GetCommandLine_4EE176()
{
    return sCommandLine_BBB9E8;
}

EXPORT void CC Sys_SetWindowPos_4EE1B1(int width, int height)
{
    RECT clientRect = {};
    ::SetWindowPos(Sys_GetWindowHandle_4EE180(), HWND_TOPMOST, 0, 0, width, height, SWP_NOREPOSITION | SWP_NOZORDER);
    ::GetClientRect(Sys_GetWindowHandle_4EE180(), &clientRect);
    if (width != clientRect.right || height != clientRect.bottom)
    {
        ::SetWindowPos(Sys_GetWindowHandle_4EE180(), HWND_TOPMOST, 0, 0, width - clientRect.right + width, height - clientRect.bottom + height, SWP_NOREPOSITION | SWP_NOZORDER);
    }
}

EXPORT int CC Sys_WindowClass_Register_4EE22F(LPCSTR lpClassName, LPCSTR lpWindowName, int x, int y, int nWidth, int nHeight)
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

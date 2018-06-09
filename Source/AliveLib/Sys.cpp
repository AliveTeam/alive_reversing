#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"

#define SYS_IMPL true

ALIVE_VAR(1, 0xBBBA00, BOOL, sAppIsActivated_BBBA00, FALSE);
ALIVE_VAR(1, 0xBBB9F4, HWND, sHwnd_BBB9F4, nullptr);
ALIVE_VAR(1, 0xBBB9F8, WNDPROC, sWindowProcFilter_BBB9F8, nullptr);
ALIVE_VAR(1, 0xBBB9E8, LPSTR, sCommandLine_BBB9E8, nullptr);
ALIVE_VAR(1, 0xBBB9EC, HINSTANCE, sInstance_BBB9EC, nullptr);
ALIVE_VAR(1, 0xBBB9FC, int, sCmdShow_BBB9FC, 0);

ALIVE_FUNC_NOT_IMPL(0x4EE32D, LRESULT CALLBACK(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam), Sys_WindowProc_4EE32D); // TODO

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
    sInstance_BBB9EC = hInstance;
    sCmdShow_BBB9FC = nShowCmd;
    sCommandLine_BBB9E8 = lpCmdLine;
}

void CC Sys_SetWindowProc_Filter_4EE197(WNDPROC pFilter)
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
    windowClass.lpfnWndProc = Sys_WindowProc_4EE32D.Ptr();
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = sInstance_BBB9EC;
    windowClass.hIcon = ::LoadIconA(sInstance_BBB9EC, IDI_APPLICATION);
    windowClass.hCursor = ::LoadCursorA(sInstance_BBB9EC, IDC_WAIT);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = lpClassName;
    windowClass.lpszClassName = lpClassName;
    ::RegisterClassA(&windowClass);

    const HWND hWnd = ::CreateWindowExA(
        0, // ExStyle
        lpClassName,
        lpWindowName,
        WS_CAPTION | WS_VISIBLE,
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

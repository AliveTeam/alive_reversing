#include "stdafx.h"
#include "WinMain.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP;

#define WINMAIN_IMPL true

ALIVE_VAR(1, 0xBBBA04, int, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBB9EC, HINSTANCE, sInstance_BBB9EC, nullptr);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);
ALIVE_VAR(1, 0xBBB9E8, LPSTR, sCommandLine_BBB9E8, nullptr);
ALIVE_VAR(1, 0xBBB9FC, int, sCmdShow_BBB9FC, 0);
ALIVE_VAR(1, 0xBBB9F4, HWND, sHwnd_BBB9F4, nullptr);
ALIVE_VAR(1, 0xBBB9F8, WNDPROC, sWindowProcFilter_BBB9F8, nullptr);

void CC Sys_SetWindowProc_Filter_4EE197(WNDPROC pFilter)
{
    sWindowProcFilter_BBB9F8 = pFilter;
}
ALIVE_FUNC_IMPLEX(0x4EE197, Sys_SetWindowProc_Filter_4EE197, WINMAIN_IMPL);

ALIVE_FUNC_NOT_IMPL(0x4949F0, void CC(), Main_4949F0); // TODO
ALIVE_FUNC_NOT_IMPL(0x4EE32D, LRESULT CALLBACK(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam), Sys_WindowProc_4EE32D); // TODO

void CC Sys_SetWindowPos_4EE1B1(int width, int height)
{
    RECT clientRect = {};
    ::SetWindowPos(sHwnd_BBB9F4, HWND_TOPMOST, 0, 0, width, height, SWP_NOREPOSITION | SWP_NOZORDER);
    ::GetClientRect(sHwnd_BBB9F4, &clientRect);
    if (width != clientRect.right || height != clientRect.bottom)
    {
        ::SetWindowPos(sHwnd_BBB9F4, HWND_TOPMOST, 0, 0, width - clientRect.right + width, height - clientRect.bottom + height, SWP_NOREPOSITION | SWP_NOZORDER);
    }
}
ALIVE_FUNC_IMPLEX(0x4EE1B1, Sys_SetWindowPos_4EE1B1, WINMAIN_IMPL);

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
ALIVE_FUNC_IMPLEX(0x4EE22F, Sys_WindowClass_Register_4EE22F, WINMAIN_IMPL);

int CALLBACK WinMain_4EE631(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LOG_INFO("WinMain_4EE631");
    sInstance_BBB9EC = hInstance;
    sPrevInstance_BBBA54 = hPrevInstance;
    sCmdShow_BBB9FC = nShowCmd;
    sCommandLine_BBB9E8 = lpCmdLine;
    Main_4949F0();
    return sExitCode_BBBA04;
}
ALIVE_FUNC_IMPLEX(0x4EE631, WinMain_4EE631, WINMAIN_IMPL);

#include "stdafx.h"
#include "Game.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "bmp.hpp"
#include "Input.hpp"
#include "Error.hpp"
#include <timeapi.h>

#define GAME_IMPL true

using TExitGameCallBack = std::add_pointer<void CC()>::type;

ALIVE_VAR(1, 0xBBFB00, TExitGameCallBack, sGame_OnExitCallback_BBFB00, nullptr);

// Timer
ALIVE_VAR(1, 0xBBB9D4, DWORD, sTimer_period_BBB9D4, 0);

// I/O
ALIVE_VAR(1, 0xBD2A5C, BOOL, sIOSyncReads_BD2A5C, FALSE);
ALIVE_VAR(1, 0xBBC558, DWORD, sIoThreadId_BBC558, 0);
ALIVE_VAR(1, 0xBBC55C, HANDLE, sIoThreadHandle_BBC55C, nullptr);

ALIVE_FUNC_NOT_IMPL(0x4F25A0, DWORD WINAPI(LPVOID lpThreadParameter), FS_IOThread_4F25A0);
ALIVE_FUNC_NOT_IMPL(0x494EA0, void CC(const char *pCmdLineNotUsed, const char *pCommandLine), Main_ParseCommandLineArguments_494EA0);
ALIVE_FUNC_NOT_IMPL(0x494A40, LRESULT CC(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam), Sys_WindowMessageHandler_494A40);
ALIVE_FUNC_NOT_IMPL(0x466D40, void CC(), Game_Run_466D40);
ALIVE_FUNC_NOT_IMPL(0x4FA010, void CC(), PSX_EMU_VideoDeAlloc_4FA010);
ALIVE_FUNC_NOT_IMPL(0x4EDEC0, int __cdecl (UINT uDelay, void* callBack), CreateTimer_4EDEC0);
ALIVE_FUNC_NOT_IMPL(0x4EFD50, void (), SND_Close_4EFD50);


void Game_ForceLink() { }

void CC Game_SetExitCallBack_4F2BA0(TExitGameCallBack callBack)
{
    sGame_OnExitCallback_BBFB00 = callBack;
}
ALIVE_FUNC_IMPLEX(0x4F2BA0, Game_SetExitCallBack_4F2BA0, GAME_IMPL);

void CC Game_ExitGame_4954B0()
{
    PSX_EMU_VideoDeAlloc_4FA010();
}
ALIVE_FUNC_IMPLEX(0x4954B0, Game_ExitGame_4954B0, GAME_IMPL);

void CC IO_Stop_ASync_IO_Thread_4F26B0()
{
    if (sIoThreadHandle_BBC55C)
    {
        ::CloseHandle(sIoThreadHandle_BBC55C);
        sIoThreadHandle_BBC55C = nullptr;
    }
}
ALIVE_FUNC_IMPLEX(0x4F26B0, IO_Stop_ASync_IO_Thread_4F26B0, GAME_IMPL);

void CC Game_Shutdown_4F2C30()
{
    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    CreateTimer_4EDEC0(0, nullptr); // Creates a timer that calls a call back which is always null, therefore seems like dead code?
    Input_DisableInput_4EDDC0();
    //SND_MCI_Close_4F0060(nullptr); // Seems like more dead code because the mci is never set?
    SND_Close_4EFD50(); // TODO: This appears to terminate the game, re-impl function to figure out why
    IO_Stop_ASync_IO_Thread_4F26B0();
    VGA_Shutdown_4F3170();
}
ALIVE_FUNC_IMPLEX(0x4F2C30, Game_Shutdown_4F2C30, GAME_IMPL);

signed int TMR_Init_4EDE20()
{
    struct timecaps_tag ptc = {};
    if (::timeGetDevCaps(&ptc, sizeof(timecaps_tag)))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\TMR.C", 25, 0, "TMR_Init: timeGetDevCaps() failed !");
        return -1;
    }

    sTimer_period_BBB9D4 = ptc.wPeriodMin;
    // This makes timers as accurate as possible increasing cpu/power usage as a trade off
    ::timeBeginPeriod(ptc.wPeriodMin);
    return 0;
}
ALIVE_FUNC_IMPLEX(0x4EDE20, TMR_Init_4EDE20, GAME_IMPL);

signed int CC Init_Input_Timer_And_IO_4F2BF0(bool forceSystemMemorySurfaces)
{
    static bool sbGameShutdownSet_BBC560 = false;
    if (!sbGameShutdownSet_BBC560)
    {
        atexit(Game_Shutdown_4F2C30);
        sbGameShutdownSet_BBC560 = 1;
        gVGA_force_sys_memory_surfaces_BC0BB4 = forceSystemMemorySurfaces;
    }

    Input_EnableInput_4EDDD0();
    Input_InitKeyStateArray_4EDD60();
    TMR_Init_4EDE20();

    if (!sIoThreadHandle_BBC55C)
    {
        sIoThreadHandle_BBC55C = ::CreateThread(
            0,
            0x4000u,
            FS_IOThread_4F25A0.Ptr(),
            0,
            0,
            &sIoThreadId_BBC558);
        if (!sIoThreadHandle_BBC55C)
        {
            return -1;
        }
    }

    if (strstr(Sys_GetCommandLine_4EE176(), "-syncread"))
    {
        sIOSyncReads_BD2A5C = TRUE;
    }
    else
    {
        sIOSyncReads_BD2A5C = FALSE;
    }
    return 0;
}
ALIVE_FUNC_IMPLEX(0x4F2BF0, Init_Input_Timer_And_IO_4F2BF0, GAME_IMPL);

void CC Game_Main_4949F0()
{
    // Inits
    Init_Input_Timer_And_IO_4F2BF0(false);
    
    Main_ParseCommandLineArguments_494EA0(Sys_GetCommandLine_4EE176(), Sys_GetCommandLine_4EE176());
    Game_SetExitCallBack_4F2BA0(Game_ExitGame_4954B0);
    Sys_SetWindowProc_Filter_4EE197(Sys_WindowMessageHandler_494A40.Ptr());
    
    // Only returns once the engine is shutting down
    Game_Run_466D40();

    if (sGame_OnExitCallback_BBFB00)
    {
        sGame_OnExitCallback_BBFB00();
        sGame_OnExitCallback_BBFB00 = nullptr;
    }

    Game_Shutdown_4F2C30();
}
ALIVE_FUNC_IMPLEX(0x4949F0, Game_Main_4949F0, GAME_IMPL);

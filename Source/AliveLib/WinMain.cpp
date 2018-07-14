#include "stdafx.h"
#include "WinMain.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"
#include "Primitives.hpp"
#include "bmp.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "Psx.hpp"
#include "Midi.hpp"
#include "GameSpeak.hpp"
#include "Quicksave.hpp"
#include "Math.hpp"
#include "DDCheat.hpp"
#include "DynamicArray.hpp"
#include "Game.hpp"
#include "PauseMenu.hpp"
#include "Map.hpp"
#include "PsxDisplay.hpp"
#include "easylogging++.h"
#include <gmock/gmock.h>
#include "Function.hpp"
#include "CreditsController.hpp"
#include "Font.hpp"
#include "FixedPoint.hpp"

INITIALIZE_EASYLOGGINGPP;

static void RunTests()
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    int argCount = 0;
    LPSTR cmdLine = Sys_GetCommandLine_4EE176();
    ::testing::InitGoogleMock(&argCount, &cmdLine);

    Test::BmpTests();
    Test::PsxDisplayTests();
    Test::PsxTests();
    Test::GameTests();
}

static void InitOtherHooksAndRunTests()
{
    RunTests();

    VGA_ForceLink();
    Psx_ForceLink();
    DynamicArray_ForceLink();
    Math_ForceLink();
    Midi_ForceLink();
    Quicksave_ForceLink();
    Game_ForceLink();
    GameSpeak_ForceLink();
    DDCheat_ForceLink();
    Map_ForceLink();
    CreditsController_ForceLink();
    PauseMenu_ForceLink();
    Font_ForceLink();
    Primitives_ForceLink();
    FixedPoint_ForceLink();
}

ALIVE_VAR(1, 0xBBBA04, int, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);

EXPORT int CALLBACK WinMain_4EE631(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LOG_INFO("Main enter");
    Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_BBBA54 = hPrevInstance;
    InitOtherHooksAndRunTests();
    Game_Main_4949F0();
    LOG_INFO("Main exit");
    return sExitCode_BBBA04;
}

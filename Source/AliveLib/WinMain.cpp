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
#include "QuikSave.hpp"
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
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "StringFormatters.hpp"
#include "MainMenu.hpp"
#include "ScreenManager.hpp"
#include "LCDScreen.hpp"
#include "UXB.hpp"
#include "Mine.hpp"
#include "StatsSign.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "LvlArchive.hpp"
#include "ObjectIds.hpp"
#include "PsxRender.hpp"
#include "VRam.hpp"
#include "Compression.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

INITIALIZE_EASYLOGGINGPP;

static void RunTests()
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    int argCount = 0;
    LPSTR cmdLine = Sys_GetCommandLine_4EE176();
    ::testing::InitGoogleMock(&argCount, &cmdLine);

    Test::VRamTests();
    Test::AnimationTests();
    Test::BmpTests();
    Test::PsxDisplayTests();
    Test::PsxTests();
    Test::GameTests();
    Test::EventTests();
    Test::MidiTests();
    Test::ScreenManagerTests();
    Test::ObjectIdsTests();
    Test::PsxRenderTests();
    Test::BaseAnimatedWithPhysicsGameObjectTests();
}

static void InitOtherHooksAndRunTests()
{
    RunTests();

    VGA_ForceLink();
    Psx_ForceLink();
    DynamicArray_ForceLink();
    Math_ForceLink();
    Midi_ForceLink();
    Game_ForceLink();
    GameSpeak_ForceLink();
    DDCheat_ForceLink();
    Map_ForceLink();
    CreditsController_ForceLink();
    PauseMenu_ForceLink();
    Font_ForceLink();
    Primitives_ForceLink();
    FixedPoint_ForceLink();
    SwitchStates_ForceLink();
    MainMenu_ForceLink();
    StringFormatters_ForceLink();
    LCDScreen_ForceLink();
    Compression_ForceLink();
    StatsSign_ForceLink();
    UXB_ForceLink();
    Mine_ForceLink();
}

ALIVE_VAR(1, 0xBBBA04, int, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);

ALIVE_VAR(1, 0x5BC100, DynamicArray, stru_5BC100, {}); // Dove particles? Will become more apparent when those objects are reversed
ALIVE_VAR(1, 0x5BC110, BYTE, byte_5BC110, 0);

EXPORT void CC Static_ctor_dynamic_array_41F3C0()
{
    stru_5BC100.ctor_40CA60(3);
}

EXPORT void CC Static_dtor_dynamic_array_41F400()
{
    // Flag to guard against multiple destruction - although seems impossible given its in the table once
    if (!(byte_5BC110 & 1))
    {
        byte_5BC110 |= 1u;
        stru_5BC100.dtor_40CAD0();
    }
}

EXPORT void CC Static_init_dynamic_array_41F3A0()
{
    Static_ctor_dynamic_array_41F3C0();
    atexit(Static_dtor_dynamic_array_41F400);
}

// Manual calls to construct global static objects when not running as dll/injected into the real game.
// There are only a very small amount of global static constructors to call.
EXPORT void Static_Inits()
{
    static_ObjectIds_init_43EC30();
    Static_init_dynamic_array_41F3A0();
    LvlArchive_Static_init_432E00();
    static_map_init_4802D0();
    static_lvl_init_480350();
    static_init_set_default_hero_43EC10();
    static_font1context_init_433300();
    static_font2context_init_433380();
}


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

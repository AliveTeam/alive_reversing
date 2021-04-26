#include "stdafx.h"
#include "WinMain.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include "Primitives.hpp"
#include "bmp.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "Psx.hpp"
#include "Sound/Midi.hpp"
#include "GameSpeak.hpp"
#include "QuikSave.hpp"
#include "Math.hpp"
#include "DDCheat.hpp"
#include "DynamicArray.hpp"
#include "Game.hpp"
#include "PauseMenu.hpp"
#include "Map.hpp"
#include "PsxDisplay.hpp"
#include "Function.hpp"
#include "Font.hpp"
#include "FixedPoint.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "MainMenu.hpp"
#include "ScreenManager.hpp"
#include "LCDScreen.hpp"
#include "UXB.hpp"
#include "Mine.hpp"
#include "LCDStatusBoard.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "LvlArchive.hpp"
#include "ObjectIds.hpp"
#include "PsxRender.hpp"
#include "VRam.hpp"
#include "Compression.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Dove.hpp"
#include "SlamDoor.hpp"
#include "QuikSave.hpp"

INITIALIZE_EASYLOGGINGPP

static void RunTests()
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    s32 argCount = 0;
    LPSTR cmdLine = Sys_GetCommandLine_4EE176();
    ::testing::InitGoogleMock(&argCount, &cmdLine);

    AETest::TestsCollision::CollisionTests();
    AETest::TestsVRam::VRamTests();
    AETest::TestsAnimation::AnimationTests();
    AETest::TestsBmp::BmpTests();
    AETest::TestsPsxDisplay::PsxDisplayTests();
    AETest::TestsPsx::PsxTests();
    AETest::TestsGame::GameTests();
    AETest::TestsEvent::EventTests();
    AETest::TestsScreenManager::ScreenManagerTests();
    AETest::TestsObjectIds::ObjectIdsTests();
    AETest::TestsPsxRender::PsxRenderTests();
    AETest::TestsBaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObjectTests();
    AETest::TestsMath::Math_Tests();
    AETest::TestsQuikSave::QuikSave_Tests();
}

static void InitOtherHooksAndRunTests()
{
    RunTests();

    VGA_ForceLink();
    Psx_ForceLink();
    DynamicArray_ForceLink();
    Math_ForceLink();
    Game_ForceLink();
    GameSpeak_ForceLink();
    DDCheat_ForceLink();
    Map_ForceLink();
    PauseMenu_ForceLink();
    Font_ForceLink();
    Primitives_ForceLink();
    FixedPoint_ForceLink();
    SwitchStates_ForceLink();
    MainMenu_ForceLink();
    Compression_ForceLink();
    StatsSign_ForceLink();
    UXB_ForceLink();
    Mine_ForceLink();
    SlamDoor_ForceLink();
}

ALIVE_VAR(1, 0xBBBA04, s32, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);

void Winmain_ForceLink()
{

}

// Manual calls to construct global static objects when not running as dll/injected into the real game.
// There are only a very small amount of global static constructors to call.
EXPORT void Static_Inits_AE()
{
    static_ObjectIds_init_43EC30();
    Dove_static_ctor_41F3A0();
    LvlArchive_Static_init_432E00();
    static_map_init_4802D0();
    static_lvl_init_480350();
    static_init_set_default_hero_43EC10();
    static_font1context_init_433300();
    static_font2context_init_433380();
}


EXPORT s32 CALLBACK WinMain_4EE631(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("Main enter");
    AE_Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_BBBA54 = hPrevInstance;
    InitOtherHooksAndRunTests();
    Game_Main_4949F0();
    LOG_INFO("Main exit");
    return sExitCode_BBBA04;
}

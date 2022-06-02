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
#include <gmock/gmock.h>

INITIALIZE_EASYLOGGINGPP

ALIVE_VAR(1, 0xBBBA04, s32, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);


// Manual calls to construct global static objects when not running as dll/injected into the real game.
// There are only a very small amount of global static constructors to call.
void Static_Inits_AE()
{
    static_font1context_init_433300();
    static_font2context_init_433380();
}


s32 CALLBACK WinMain_4EE631(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("Main enter");
    AE_Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_BBBA54 = hPrevInstance;
    Game_Main_4949F0();
    LOG_INFO("Main exit");
    return sExitCode_BBBA04;
}

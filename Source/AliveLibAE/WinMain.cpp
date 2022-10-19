#include "stdafx.h"
#include "WinMain.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "Psx.hpp"
#include "Sound/Midi.hpp"
#include "GameSpeak.hpp"
#include "QuikSave.hpp"
#include "Math.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "Game.hpp"
#include "PauseMenu.hpp"
#include "Map.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Function.hpp"
#include "Font.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "LCDScreen.hpp"
#include "UXB.hpp"
#include "Mine.hpp"
#include "LCDStatusBoard.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PsxRender.hpp"
#include "../relive_lib/VRam.hpp"
#include "Compression.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Math.hpp"
#include "Dove.hpp"
#include "SlamDoor.hpp"
#include "QuikSave.hpp"
#include <gmock/gmock.h>

INITIALIZE_EASYLOGGINGPP

s32 sExitCode_BBBA04 = 0;
HINSTANCE sPrevInstance_BBBA54 = nullptr;


s32 CALLBACK WinMain_4EE631(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("Main enter");
    AE_Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_BBBA54 = hPrevInstance;
    Game_Main();
    LOG_INFO("Main exit");
    return sExitCode_BBBA04;
}

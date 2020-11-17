#include "stdafx_ao.h"
#include "WinMain.hpp"
#include "Function.hpp"
#include "bmp.hpp"
#include "Psx_common.hpp"
#include "stdlib.hpp"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Animation.hpp"
#include "FixedPoint_common.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Psx.hpp"
#include "Alarm.hpp"
#include "BaseAliveGameObject.hpp"
#include "PsxDisplay.hpp"
#include "AmbientSound.hpp"
#include "MusicController.hpp"
#include "GameSpeak.hpp"
#include "Input.hpp"
#include "DDCheat.hpp"
#include "CheatController.hpp"
#include "RollingBallShaker.hpp"
#include "Blood.hpp"
#include "Paramite.hpp"
#include "Scrab.hpp"
#include "Slog.hpp"
#include "Mudokon.hpp"
#include "SlingMudokon.hpp"
#include "CameraSwapper.hpp"
#include "HintFly.hpp"
#include "ZapLine.hpp"
#include "ScreenWave.hpp"
#include "Dove.hpp"
#include "LvlArchive.hpp"
#include "Font.hpp"
#include "GameEnderController.hpp"
#include "Spark.hpp"
#include "Events.hpp"
#include "../AliveLibCommon/config.h"
#include "Sfx.hpp"
#include "ShadowZone.hpp"
#include "PauseMenu.hpp"
#include "SwitchStates.hpp"
#include "Midi.hpp"
#include "Error.hpp"
#include "VGA.hpp"
#include "Sys.hpp"
#include "PsxRender.hpp"
#include "Psx.hpp"

namespace AO {

void Static_Inits_AO()
{
    Dove_static_ctor_40EE10();
    LvlArchive::ctor_static_41BBA0();
    FontContext::static_ctor_41C010();
    Map::ctor_static_443E10();
    LvlArchive::ctor_static_443E70();
}

void WinMain_ForceLink()
{

}

void ForceLinks()
{
    RollingBallShaker_ForceLink();
    Blood_ForceLink();
    Paramite_ForceLink();
    Scrab_ForceLink();
    Slog_ForceLink();
    Mud_ForceLink();
    SlingMud_ForceLink();
    HintFly_ForceLink();
    ZapLine_ForceLink();
    Forcelink_ScreenWave();
    GameEnderController_ForceLink();
    Spark_ForceLink();
    Alarm_ForceLink();
}


ALIVE_VAR(1, 0x9F7734, int, sExitCode_9F7734, 0);
ALIVE_VAR(1, 0x9F7784, HINSTANCE, sPrevInstance_9F7784, 0);

EXPORT int CALLBACK WinMain_48EF50(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LOG_INFO("Main enter");
    ForceLinks();
    Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_9F7784 = hPrevInstance;
    Game_Main_450050();
    LOG_INFO("Main exit");
    return sExitCode_9F7734;
}

}

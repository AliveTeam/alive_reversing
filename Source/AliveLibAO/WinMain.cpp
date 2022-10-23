#include "stdafx_ao.h"
#include "WinMain.hpp"
#include "Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "Alarm.hpp"
#include "RollingBallShaker.hpp"
#include "Blood.hpp"
#include "Paramite.hpp"
#include "Scrab.hpp"
#include "Slog.hpp"
#include "Mudokon.hpp"
#include "SlingMudokon.hpp"
#include "HintFly.hpp"
#include "ZapLine.hpp"
#include "ScreenWave.hpp"
#include "GameEnderController.hpp"
#include "Spark.hpp"
#include "Sys.hpp"

namespace AO {

s32 sExitCode_9F7734 = 0;
HINSTANCE sPrevInstance_9F7784 = 0;

s32 CALLBACK WinMain_48EF50(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("Main enter");
    AO_Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_9F7784 = hPrevInstance;
    Game_Main();
    LOG_INFO("Main exit");
    return sExitCode_9F7734;
}

} // namespace AO

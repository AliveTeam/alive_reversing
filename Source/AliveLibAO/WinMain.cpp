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
#include "../AliveLibAE/Sys.hpp"

namespace AO {

s32 CALLBACK WinMain_48EF50(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("Main enter");
    Sys_Main(hInstance, lpCmdLine, nShowCmd);
    Game_Main();
    LOG_INFO("Main exit");
    return 0;
}

} // namespace AO

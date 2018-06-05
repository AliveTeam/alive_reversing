#include "stdafx.h"
#include "WinMain.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP;

ALIVE_VAR(1, 0xBBBA04, int, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBB9EC, HINSTANCE, sInstance_BBB9EC, nullptr);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);
ALIVE_VAR(1, 0xBBB9E8, LPSTR, sCommandLine_BBB9E8, nullptr);
ALIVE_VAR(1, 0xBBB9FC, int, sCmdShow_BBB9FC, 0);

ALIVE_FUNC_NOT_IMPL(0x4949F0, void CC(), Main_4949F0);

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
ALIVE_FUNC_IMPLEX(0x4EE631, WinMain_4EE631, true);

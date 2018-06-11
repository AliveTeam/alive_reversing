#include "stdafx.h"
#include "WinMain.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"
#include "bmp.hpp"
#include "Sys.hpp"
#include "VGA.hpp"
#include "Psx.hpp"
#include "Midi.hpp"
#include "Quicksave.hpp"
#include "Math.hpp"
#include "DynamicArray.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP;

#define WINMAIN_IMPL true

static void RunTests()
{
    BmpTests();
}

static void ReplaceStdLib()
{
    if (!IsAlive())
    {
        return;
    }

    ALIVE_REDIRECT(0x5212C0, malloc);
    ALIVE_REDIRECT(0x495540, free);
    ALIVE_REDIRECT(0x522335, realloc);
    ALIVE_REDIRECT(0x528DD7, calloc);
}

static void InitOtherHooksAndRunTests()
{
    // TODO: Fix memory API's
    //ReplaceStdLib();
    //RunTests();

    VGA_ForceLink();
    Psx_ForceLink();
    DynamicArray_ForceLink();
	Math_ForceLink();
	Midi_ForceLink();
	Quicksave_ForceLink();
}

ALIVE_VAR(1, 0xBBBA04, int, sExitCode_BBBA04, 0);
ALIVE_VAR(1, 0xBBBA54, HINSTANCE, sPrevInstance_BBBA54, nullptr);

ALIVE_FUNC_NOT_IMPL(0x4949F0, void CC(), Main_4949F0); // TODO

int CALLBACK WinMain_4EE631(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LOG_INFO("WinMain_4EE631");
    Sys_Main(hInstance, lpCmdLine, nShowCmd);
    sPrevInstance_BBBA54 = hPrevInstance;
    InitOtherHooksAndRunTests();
    Main_4949F0();
    return sExitCode_BBBA04;
}
ALIVE_FUNC_IMPLEX(0x4EE631, WinMain_4EE631, WINMAIN_IMPL);

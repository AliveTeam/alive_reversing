#include "stdafx.h"
#include <iostream>

#if _WIN32
#include <windows.h>
#include "ExportHooker.hpp"
#include "WinMain.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
    ExportHooker hooker(hInstance);
    hooker.Apply();

    // In the real game these are called before main, but shouldn't really matter in this case
    Static_Inits();

    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
#else

#include "WinMain.hpp"

#if __ANDROID__
extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char** argv)
#else
int main()
#endif
{
    // In the real game these are called before main, but shouldn't really matter in this case
    Static_Inits();

    // TOOD: Convert command line arguments to what WinMain_4EE631 expects
    char cmdArgsHack[2] = "";
    return WinMain_4EE631(0, 0, cmdArgsHack, 1);
}
#endif

bool IsAlive()
{
    return false;
}

#include "stdafx.h"
#include <iostream>
#include <string>

#if _WIN32
#include <windows.h>
#include "ExportHooker.hpp"
#endif

#include "../AliveLibAE/WinMain.hpp"
#include "../AliveLibAO/WinMain.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);

    ExportHooker hooker(hInstance);
    hooker.Apply();
#endif

#ifdef AE_EXE
    LOG_INFO("AE standalone starting...");
    // In the real game these are called before main, but shouldn't really matter in this case
    Static_Inits_AE();
    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#elif AO_EXE
    LOG_INFO("AO standalone starting...");
    Static_Inits_AO();
    return WinMain_48EF50(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#else
    // Default to AE but allow switching to AO with a command line, if AO is anywhere in the command line then assume we want to run AO
    if (strstr(lpCmdLine, "AO"))
    {
        LOG_INFO("AO standalone starting...");
        Static_Inits_AO();
        return WinMain_48EF50(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
    else
    {
        LOG_INFO("AE standalone starting...");
        // In the real game these are called before main, but shouldn't really matter in this case
        Static_Inits_AE();
        return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
    return 0;
#endif
}

#if __ANDROID__
extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    std::string args;
    for (int i = 0; i < argc; i++)
    {
        args += argv[i] + std::string(" ");
    }
    return WinMain(0, 0, const_cast<LPSTR>(args.c_str()), 1);
}

bool RunningAsInjectedDll()
{
    return false;
}

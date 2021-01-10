#include "stdafx.h"
#include <iostream>
#include <string>

#if _WIN32
#include <windows.h>
#include "ExportHooker.hpp"
#endif

#include "../AliveLibAE/WinMain.hpp"
#include "../AliveLibAO/WinMain.hpp"
#include "W32CrashHandler.hpp"
#include "SDL.h"
#include "Sys_common.hpp"

enum class GameType
{
    eAo,
    eAe,
};

static bool FileExists(const char* fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f)
    {
        fclose(f);
        return true;
    }
    return false;
}



static bool CheckRequiredGameFilesExist(GameType gameType)
{
    if (gameType == GameType::eAe)
    {
        if (!FileExists("st.lvl") || !FileExists("mi.lvl"))
        {
            SDL_Init(SDL_INIT_EVENTS);
            Alive_Show_ErrorMsg("Abes Exoddus cant start because st.lvl or mi.lvl was not found in the working directory. Copy relive files to the root game directory to fix this.");
            return false;
        }
        return true;
    }
    else if (gameType == GameType::eAo)
    {
        if (!FileExists("s1.lvl") || !FileExists("r1.lvl"))
        {
            SDL_Init(SDL_INIT_EVENTS);
            Alive_Show_ErrorMsg("Abes Oddysee cant start because s1.lvl or r1.lvl was not found in the working directory. Copy relive files to the root game directory to fix this.");
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if _WIN32
    ::SetUnhandledExceptionFilter(unhandled_handler);

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
    if (!CheckRequiredGameFilesExist(GameType::eAe))
    {
        return 1;
    }
    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#elif AO_EXE
    LOG_INFO("AO standalone starting...");
    AO::Static_Inits_AO();
    if (!CheckRequiredGameFilesExist(GameType::eAo))
    {
        return 1;
    }
    return AO::WinMain_48EF50(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#else
    // Default to AE but allow switching to AO with a command line, if AO is anywhere in the command line then assume we want to run AO
    if (strstr(lpCmdLine, "AO"))
    {
        LOG_INFO("AO standalone starting...");
        AO::Static_Inits_AO();
        if (!CheckRequiredGameFilesExist(GameType::eAo))
        {
            return 1;
        }
        return AO::WinMain_48EF50(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
    else
    {
        LOG_INFO("AE standalone starting...");
        // In the real game these are called before main, but shouldn't really matter in this case
        Static_Inits_AE();
        if (!CheckRequiredGameFilesExist(GameType::eAe))
        {
            return 1;
        }
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

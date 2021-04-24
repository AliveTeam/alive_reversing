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

#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAE/Font.hpp"
#include "../AliveLibAE/Abe.hpp"

#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Font.hpp"
#include "../AliveLibAO/Game.hpp"
#include "../AliveLibAO/SwitchStates.hpp"

enum class GameType : int
{
    eAo = 0,
    eAe = 1,
};

namespace AutoSplitterData
{
    struct GuidStr
    {
        const char guid[39];
    };

    struct AOGameInfo
    {
        GuidStr guid;
        GameType* gameType;         // 0
        AO::LevelIds* levelId;      // 1
        s16* pathId;            // 2
        s16* camId;             // 3
        u32* gnFrame;      // 4
        AO::Abe** pAbe;             // 5
        int abeYOffSet;             // 6
        short* isGameRunning;       // 7
        char* isGameBeaten;         // 8
    };

    struct AEGameInfo
    {
        GuidStr guid;
        // 1 byte padding/null
        GameType* gameType;         // 0
        LevelIds* levelId;          // 1
        s16* pathId;                // 2
        s16* camId;                 // 3
        u16* fmvId;                 // 4
        u32* gnFrame;               // 5
        Abe** pAbe;                 // 6
        int abeYOffSet;             // 7
        char* isPaused;             // 8
    };

    extern "C"
    {
        GameType gameType = GameType::eAo;

        // Auto splitter looks for this guid, if it exists then it assumes its the 64bit relive
#ifdef _WIN64
        EXPORT constexpr GuidStr k64BitGuid = {"{069DDB51-609D-49AB-B69D-5CC6D13E73EE}"};
        
        // If not exported the var will get optimized away
        EXPORT const void* Get64BitInfo()
        {
            return &k64BitGuid;
        }
#endif

#ifdef _WIN32
        EXPORT constexpr AEGameInfo kAeInfo =
        {
            "{DBC2AE1C-A5DE-465F-A89A-C385BE1DEFCC}",
            // 2 byte padding (32bit)
            &gameType,
            &LocalVar_gMap_5C3030.field_0_current_level,
            &LocalVar_gMap_5C3030.field_2_current_path,
            &LocalVar_gMap_5C3030.field_4_current_camera,
            &LocalVar_gMap_5C3030.field_12_fmv_base_id,
            &LocalVar_sGnFrame_5C1B84,
            &LocalVar_spAbe_554D5C,
            offsetof(Abe, field_BC_ypos),
            &LocalVar_sDisableFontFlicker_5C9304
        };

        EXPORT const void* GetAeInfo()
        {
            return &kAeInfo;
        }

        EXPORT constexpr AOGameInfo kAoInfo =
        {
            "{1D2E2B5A-19EE-4776-A0EE-98F49F781370}",
            // 2 byte padding (32bit)
            &gameType,
            &AO::LocalVar_gMap_507BA8.field_0_current_level,
            &AO::LocalVar_gMap_507BA8.field_2_current_path,
            &AO::LocalVar_gMap_507BA8.field_4_current_camera,
            &AO::LocalVar_gnFrameCount_507670,
            &AO::LocalVar_sActiveHero_507678,
            offsetof(AO::Abe, field_AC_ypos) + sizeof(short), // +2 for exp only
            &AO::LocalVar_sDisableFontFlicker_5080E4,
            &AO::LocalVar_sSwitchStates_505568.mData[70]
        };

        EXPORT const void* GetAoInfo()
        {
            return &kAoInfo;
        }
#endif
    }
}

void PopulateAutoSplitterVars(GameType gameType)
{
    AutoSplitterData::gameType = gameType;
}

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



static bool CheckRequiredGameFilesExist(GameType gameType, bool showError)
{
    if (gameType == GameType::eAe)
    {
        if (!FileExists("st.lvl") || !FileExists("mi.lvl"))
        {
            if (showError)
            {
                SDL_Init(SDL_INIT_EVENTS);
                Alive_Show_ErrorMsg("Abes Exoddus/Abes Oddysee cant start because st.lvl or mi.lvl was not found in the working directory. Copy relive files to the root game directory to fix this.");
            }
            return false;
        }
        return true;
    }
    else if (gameType == GameType::eAo)
    {
        if (!FileExists("s1.lvl") || !FileExists("r1.lvl"))
        {
            if (showError)
            {
                SDL_Init(SDL_INIT_EVENTS);
                Alive_Show_ErrorMsg("Abes Oddysee/Abes Exoddus cant start because s1.lvl or r1.lvl was not found in the working directory. Copy relive files to the root game directory to fix this.");
            }
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

static int AOMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LOG_INFO("AO standalone starting...");
    AO::Static_Inits_AO();
    PopulateAutoSplitterVars(GameType::eAo);
    return AO::WinMain_48EF50(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

static int AEMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LOG_INFO("AE standalone starting...");
    // In the real game these are called before main, but shouldn't really matter in this case
    Static_Inits_AE();
    PopulateAutoSplitterVars(GameType::eAe);
    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
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

    // Default to AE but allow switching to AO with a command line, if AO is anywhere in the command line then assume we want to run AO
    GameType gameToRun = strstr(lpCmdLine, "AO") ? GameType::eAo : GameType::eAe;
    if (gameToRun == GameType::eAo)
    {
        LOG_INFO("Checking AO files are present...");
        if (!CheckRequiredGameFilesExist(GameType::eAo, false))
        {
            gameToRun = GameType::eAe;
            LOG_INFO("No AO files found, switch to AE");
        }
    }
    else
    {
        LOG_INFO("Checking AE files are present...");
        if (!CheckRequiredGameFilesExist(GameType::eAe, false))
        {
            gameToRun = GameType::eAo;
            LOG_INFO("No AO files found, switch to AO");
        }
    }

    if (!CheckRequiredGameFilesExist(gameToRun, true))
    {
        return 1;
    }

    if (gameToRun == GameType::eAo)
    {
        return AOMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
    else
    {
        return AEMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
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

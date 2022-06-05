#include "stdafx.h"
#include <iostream>
#include <string>

#if _WIN32
    #include <windows.h>
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

#include "../AliveLibAE/GameAutoPlayer.hpp"
#include "../AliveLibAO/GameAutoPlayer.hpp"

#include "../relive_lib/GameType.hpp"

#include "Io.hpp"

namespace AutoSplitterData {
struct GuidStr
{
    const s8 guid[39];
};

struct AOGameInfo
{
    GuidStr guid;
    GameType* gameType;    // 0
    AO::LevelIds* levelId; // 1
    s16* pathId;           // 2
    s16* camId;            // 3
    u32* gnFrame;          // 4
    AO::Abe** pAbe;        // 5
    s32 abeYOffSet;        // 6
    s16* isGameRunning;    // 7
    s8* isGameBeaten;      // 8
};

struct AEGameInfo
{
    GuidStr guid;
    // 1 byte padding/null
    GameType* gameType; // 0
    LevelIds* levelId;  // 1
    s16* pathId;        // 2
    s16* camId;         // 3
    u16* fmvId;         // 4
    u32* gnFrame;       // 5
    Abe** pAbe;         // 6
    s32 abeYOffSet;     // 7
    s8* isPaused;       // 8
};

extern "C"
{
    static GameType gameType = GameType::eAo;

// Auto splitter looks for this guid, if it exists then it assumes its the 64bit relive
#ifdef _WIN64
    constexpr GuidStr k64BitGuid = {"{069DDB51-609D-49AB-B69D-5CC6D13E73EE}"};

    // If not exported the var will get optimized away
    const void* Get64BitInfo()
    {
        return &k64BitGuid;
    }
#endif

#ifdef _WIN32
    constexpr AEGameInfo kAeInfo = {
        "{DBC2AE1C-A5DE-465F-A89A-C385BE1DEFCC}",
        // 2 byte padding (32bit)
        &gameType,
        &gMap.mCurrentLevel,
        &gMap.mCurrentPath,
        &gMap.mCurrentCamera,
        &gMap.field_12_fmv_base_id,
        &sGnFrame,
        &sActiveHero,
        offsetof(Abe, mBaseAnimatedWithPhysicsGameObject_YPos),
        & sDisableFontFlicker_5C9304};

    const void* GetAeInfo()
    {
        return &kAeInfo;
    }

    constexpr AOGameInfo kAoInfo = {
        "{1D2E2B5A-19EE-4776-A0EE-98F49F781370}",
        // 2 byte padding (32bit)
        &gameType,
        &AO::gMap.mCurrentLevel,
        &AO::gMap.mCurrentPath,
        &AO::gMap.mCurrentCamera,
        &AO::sGnFrame,
        &AO::sActiveHero_507678,
        offsetof(AO::Abe, mBaseAnimatedWithPhysicsGameObject_YPos) + sizeof(s16), // +2 for exp only
        &AO::sDisableFontFlicker_5080E4,
        &AO::sSwitchStates_505568.mData[70]};

    const void* GetAoInfo()
    {
        return &kAoInfo;
    }
#endif
}
} // namespace AutoSplitterData

void PopulateAutoSplitterVars(GameType gameType)
{
    AutoSplitterData::gameType = gameType;
}

static bool FileExists(const char_type* fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f)
    {
        fclose(f);
        return true;
    }
    return false;
}

// Only used on Windows for logging to help when people have issues launching the game
static void ShowCwd()
{
#ifdef _WIN32
    // Note: Not using SDL_GetBasePath as that returns the executable dir which isn't
    // always the same as the cwd.
    char_type buffer[2048] = {};
    if (::GetCurrentDirectoryA(sizeof(buffer), buffer))
    {
        LOG_INFO("Win32 cwd is: " << buffer);
    }
    else
    {
        LOG_INFO("Failed to get Win32 cwd: " << ::GetLastError());
    }
#endif
}

static void GameDirListing()
{
    IO_EnumerateDirectory("*.*", [](const char_type* fileName, u32)
                          { LOG_INFO(fileName); });
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
                GameDirListing();
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
                GameDirListing();
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

static BaseGameAutoPlayer& GetGameAutoPlayerAO()
{
    static AO::GameAutoPlayer autoPlayer;
    return autoPlayer;
}

static BaseGameAutoPlayer& GetGameAutoPlayerAE()
{
    static GameAutoPlayer autoPlayer;
    return autoPlayer;
}

static BaseGameAutoPlayer* pAutoPlayer = nullptr;

BaseGameAutoPlayer& GetGameAutoPlayer()
{
    return *pAutoPlayer;
}

static s32 AOMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("AO standalone starting...");
    PopulateAutoSplitterVars(GameType::eAo);
    return AO::WinMain_48EF50(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

static s32 AEMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    LOG_INFO("AE standalone starting...");
    // In the real game these are called before main, but shouldn't really matter in this case
    Static_Inits_AE();
    PopulateAutoSplitterVars(GameType::eAe);
    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    Install_Crash_Handler();
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);

#endif
    LOG_INFO("Relive: " << BuildAndBitnesString());

    // Default to AE but allow switching to AO with a command line, if AO is anywhere in the command line then assume we want to run AO
    GameType gameToRun = strstr(lpCmdLine, "AO") ? GameType::eAo : GameType::eAe;
    ShowCwd();

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
            LOG_INFO("No AE files found, switch to AO");
        }
    }

    if (!CheckRequiredGameFilesExist(gameToRun, true))
    {
        return 1;
    }

    SetGameType(gameToRun);

    if (gameToRun == GameType::eAo)
    {
        pAutoPlayer = &GetGameAutoPlayerAO();
        return AOMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
    else
    {
        pAutoPlayer = &GetGameAutoPlayerAE();
        return AEMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    }
}

#if __ANDROID__
extern "C" __attribute__((visibility("default"))) s32 SDL_main(s32 argc, s8** argv)
#else
s32 main(s32 argc, char_type** argv)
#endif
{
    std::string args;
    for (s32 i = 0; i < argc; i++)
    {
        args += argv[i] + std::string(" ");
    }
    return WinMain(0, 0, const_cast<LPSTR>(args.c_str()), 1);
}

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <unistd.h>

static void __attribute__((constructor)) FixCWD()
{
    char* imagePath = strdup(_dyld_get_image_name(0));
    if (!imagePath)
    {
        return;
    }
    
    for (u32 i = strlen(imagePath); --i;)
    {
        if (imagePath[i] == '/')
        {
            imagePath[i] = 0;
            chdir(imagePath);
            break;
        }
    }
    free(imagePath);
}
#endif

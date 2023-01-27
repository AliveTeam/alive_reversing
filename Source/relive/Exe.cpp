#include "stdafx.h"

#if _WIN32
    #include <windows.h>
#endif

#include "W32CrashHandler.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/SwitchStates.hpp"

#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAE/Abe.hpp"

#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/Abe.hpp"

#include "../AliveLibAE/GameAutoPlayer.hpp"
#include "../AliveLibAO/GameAutoPlayer.hpp"

#include "../relive_lib/GameType.hpp"
#include "../relive_lib/Engine.hpp"
#include "../relive_lib/SwitchStates.hpp"

#include "../AliveLibAE/Io.hpp"
#include "../AliveLibAE/Font.hpp"

#include "../relive_lib/data_conversion/file_system.hpp"
#include "../relive_lib/GameType.hpp"

namespace AutoSplitterData {
struct GuidStr
{
    const s8 guid[39];
};

struct AOGameInfo
{
    GuidStr guid;
    GameType* gameType;    // 0
    // TODO: Auto splitter fix - we've changed all the level numbers :)
    EReliveLevelIds* levelId; // 1
    s16* pathId;           // 2
    s16* camId;            // 3
    u32* gnFrame;          // 4
    AO::Abe** pAbe;        // 5
    s32 abeYOffSet;        // 6
    bool* isGameRunning;     // 7
    s8* isGameBeaten;      // 8
};

struct AEGameInfo
{
    GuidStr guid;
    // 1 byte padding/null
    GameType* gameType; // 0
    EReliveLevelIds* levelId;  // 1
    s16* pathId;        // 2
    s16* camId;         // 3
    u16* fmvId;         // 4
    u32* gnFrame;       // 5
    Abe** pAbe;         // 6
    s32 abeYOffSet;     // 7
    bool* isPaused;       // 8
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
        &gMap.mFmvBaseId,
        &sGnFrame,
        &sActiveHero,
        offsetof(Abe, mYPos),
        &gDisableFontFlicker};

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
        &::sGnFrame,
        &AO::sActiveHero,
        offsetof(AO::Abe, mYPos) + sizeof(s16), // +2 for exp only
        &gDisableFontFlicker,
        &gSwitchStates.mData[70]};

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

#ifndef _WIN32
static std::string GetPrefPath()
{
    char* pPath = SDL_GetPrefPath("relive_team", "relive");
    std::string ret(pPath);
    SDL_free(pPath);
    return ret;
}
#endif

static std::string GetCwd()
{
#ifdef _WIN32
    // Note: Not using SDL_GetBasePath as that returns the executable dir which isn't
    // always the same as the cwd.
    char_type buffer[2048] = {};
    if (!::GetCurrentDirectoryA(sizeof(buffer), buffer))
    {
        LOG_INFO("Failed to get Win32 cwd: %d", ::GetLastError());
    }
    return buffer;
#else
    char buffer[4096] = {};
    const char* answer = getcwd(buffer, sizeof(buffer));
    if (answer)
    {
        char* pBasePath = SDL_GetBasePath();
        LOG_INFO("Mac/Linux cwd is %s SDL_GetBasePath is %s SDL_GetPrefPath is %s", answer, pBasePath, GetPrefPath().c_str());
        std::string tmp(pBasePath);
        SDL_free(pBasePath);
        return tmp;
    }
    else
    {
        LOG_ERROR("Failed to get CWD");
    }
    return {};
#endif
}

// Only used on Windows for logging to help when people have issues launching the game
static void ShowCwd()
{
    LOG_INFO("Cwd is: %s", GetCwd().c_str());
}

static void PrintSDL2Versions()
{
    SDL_version compiled = {};
    SDL_version linked = {};

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    LOG_INFO("Compiled with SDL2 ver %d.%d.%d", static_cast<int>(compiled.major), static_cast<int>(compiled.minor), static_cast<int>(compiled.patch));
    LOG_INFO("Runtime SDL2 ver %d.%d.%d", static_cast<int>(linked.major), static_cast<int>(linked.minor), static_cast<int>(linked.patch));
}

static void SDL2_Init()
{
    PrintSDL2Versions(); // Ok to call before init

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
    {
        LOG_ERROR(SDL_GetError());
        ALIVE_FATAL(SDL_GetError());
    }
}

static void GameDirListing()
{
    IO_EnumerateDirectory("*.*", [](const char_type* fileName, u32)
                          { LOG_INFO(fileName); });
}

static bool CheckRequiredGameFilesExist(GameType gameType, bool showError)
{
    FileSystem fs;
    if (gameType == GameType::eAe)
    {
        if (!fs.FileExists("st.lvl") || !fs.FileExists("mi.lvl"))
        {
            if (showError)
            {
                SDL_Init(SDL_INIT_EVENTS);
                GameDirListing();
                Alive_Show_ErrorMsg("Abes Exoddus/Abes Oddysee cant start because st.lvl or mi.lvl was not found in the working directory (%s). Copy relive files to the root game directory to fix this.", GetCwd().c_str());
            }
            return false;
        }
        return true;
    }
    else if (gameType == GameType::eAo)
    {
        if (!fs.FileExists("s1.lvl") || !fs.FileExists("r1.lvl"))
        {
            if (showError)
            {
                SDL_Init(SDL_INIT_EVENTS);
                GameDirListing();
                Alive_Show_ErrorMsg("Abes Oddysee/Abes Exoddus cant start because s1.lvl or r1.lvl was not found in the working directory (%s). Copy relive files to the root game directory to fix this.", GetCwd().c_str());
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


#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <unistd.h>

static void __attribute__((constructor)) FixCWD()
{
    chdir(GetPrefPath().c_str());
}
#endif


s32 WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, s32 /*nShowCmd*/)
{
    Install_Crash_Handler();
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);

#endif
    LOG_INFO("Relive: %s", BuildAndBitnesString().c_str());

    SDL2_Init();

    // Default to AE but allow switching to AO with a command line, if AO is anywhere in the command line then assume we want to run AO
    GameType gameToRun = strstr(lpCmdLine, "AO") ? GameType::eAo : GameType::eAe;

#ifdef __APPLE__
    FixCWD();
#endif
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

    pAutoPlayer = gameToRun == GameType::eAo ? &GetGameAutoPlayerAO() : &GetGameAutoPlayerAE();

    PopulateAutoSplitterVars(gameToRun);

    Engine e(gameToRun, lpCmdLine);
    e.Run();

    return 0;
}

s32 main(s32 argc, char_type** argv)
{
    std::string args;
    for (s32 i = 0; i < argc; i++)
    {
        args += argv[i] + std::string(" ");
    }
    return WinMain(0, 0, const_cast<LPSTR>(args.c_str()), 1);
}


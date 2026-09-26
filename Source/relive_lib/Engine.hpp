#pragma once

#include "GameType.hpp"
#include "Ipc/Ipc.hpp"
#include "ResourceManagerWrapper.hpp"
#include "Factory.hpp"

class FileSystem;
class CommandLineParser;
class BaseMap;
enum class EReliveLevelIds : s16;

extern u32 sGnFrame;
extern bool gDDCheatOn;
extern u16 gAttract;
extern bool gSkipGameObjectUpdates;
extern s16 gNumCamSwappers;
extern bool gBreakGameLoop;

void DestroyObjects(ResourceManagerWrapper& resMan);

class Engine final
{
public:
    Engine(GameType gameType, FileSystem& fs, CommandLineParser& clp);
    ~Engine();
    void Run();

    // Called from Run() once the game's map exists (e.g. so the exe can publish its address for auto-splitters)
    using TMapCreatedCb = void (*)(BaseMap& map);
    void SetMapCreatedCallback(TMapCreatedCb cb)
    {
        mMapCreatedCb = cb;
    }
    static void Init_GameStates();
private:
    void CmdLineRenderInit(const std::string& activeModName);

    void Game_Run(EReliveLevelIds startLevel, s32 startPath, s32 startCamera);
    void Game_Main(EReliveLevelIds startLevel, s32 startPath, s32 startCamera);

    void Init_Sound_DynamicArrays_And_Others();

    GameType mGameType = GameType::eAe;
    FileSystem& mFs;
    CommandLineParser& mClp;
    std::unique_ptr<relive::IIpcInterface> mIpcInterface;
    std::unique_ptr<ResourceManagerWrapper> mResMan;
    std::unique_ptr<BaseMap> mMap;
    relive::Factory mFactory;
    TMapCreatedCb mMapCreatedCb = nullptr;
};

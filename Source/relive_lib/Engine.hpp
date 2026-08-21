#pragma once

#include "GameType.hpp"
#include "Ipc/Ipc.hpp"

class FileSystem;
class CommandLineParser;

class Engine final
{
public:
    Engine(GameType gameType, FileSystem& fs, CommandLineParser& clp);
    ~Engine();
    void Run();

private:
    void CmdLineRenderInit();

    GameType mGameType = GameType::eAe;
    FileSystem& mFs;
    CommandLineParser& mClp;
    std::unique_ptr<relive::IIpcInterface> mIpcInterface; 
};

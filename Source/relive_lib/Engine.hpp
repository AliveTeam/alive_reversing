#pragma once

#include "GameType.hpp"

class FileSystem;
class CommandLineParser;

class Engine final
{
public:
    Engine(GameType gameType, FileSystem& fs, CommandLineParser& clp);
    void Run();

private:
    void CmdLineRenderInit();

    GameType mGameType = GameType::eAe;
    FileSystem& mFs;
    CommandLineParser& mClp;
};

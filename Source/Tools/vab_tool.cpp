#include "config.h"
#include "logger.hpp"
#include "FunctionFwd.hpp"
#include "SDL_main.h"
#include "Io.hpp"
#include "Sys.hpp"
#define NO_WAVE
#include "../AliveLibAE/Sound/PsxSpuApi.hpp"
#include "../AliveLibAE/LvlArchive.hpp"

bool CC RunningAsInjectedDll()
{
    return false;
}

static std::vector<unsigned char> ReadLvlFile(LvlArchive& archive, const char* fileName)
{
    LvlFileRecord* pFileRec = archive.Find_File_Record_433160(fileName);
    if (!pFileRec)
    {
        ALIVE_FATAL("Failed find file in lvl");
    }
    std::vector<unsigned char> fileContent;
    fileContent.resize(pFileRec->field_10_num_sectors * 2048);
    sLvlArchive_5BC520.Read_File_4330A0(pFileRec, fileContent.data());
    fileContent.resize(pFileRec->field_14_file_size);
    return fileContent;
}

static void PCToPsxVab(bool isAe, const char* lvlName, const char* vhName, const char* vbName)
{
    ResourceManager::Init_49BCE0();

    LvlArchive archive;

    if (!archive.Open_Archive_432E80(lvlName))
    {
        ALIVE_FATAL("Failed to open lvl");
    }

    auto vhData = ReadLvlFile(archive, vhName);
    auto pHeader = reinterpret_cast<VabHeader*>(vhData.data());

    auto vbData = ReadLvlFile(archive, vbName);
    auto pBody = reinterpret_cast<VabBodyRecord*>(vbData.data());

    if (isAe)
    {
        auto hDat = IO_Open("sounds.dat", "rb");
        if (!hDat)
        {
            ALIVE_FATAL("Failed to open sounds.dat");
        }
        IO_Close(hDat);
    }
    
    archive.Free_433130();
}

int main(int argc, char* argv[])
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
#endif

    LOG_INFO("Test");

    PCToPsxVab(true, "MI.LVL", "MINES.VH", "MINES.VB");

    return 0;
}

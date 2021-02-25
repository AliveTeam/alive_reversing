#include "alive_api.hpp"
#include "SDL.h"
#include "logger.hpp"

int main(int argc, char* argv[])
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
#endif

    auto ret = AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Oddysee\\R6.LVL", 6);
    //auto ret = AliveAPI::EnumeratePaths("C:\\GOG Games\\Abes Oddysee\\R1.LVL");

    return ret.errorCode;
}

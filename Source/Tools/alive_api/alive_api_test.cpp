#include "alive_api.hpp"
#include "SDL.h"

int main(int argc, char* argv[])
{
    //auto ret = AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Oddysee\\R6.LVL", 6);
    auto ret = AliveAPI::EnumeratePaths("C:\\GOG Games\\Abes Oddysee\\R1.LVL");

    return ret.errorCode;
}

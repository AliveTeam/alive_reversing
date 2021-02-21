#include "alive_api.hpp"
#include "SDL.h"

int main(int argc, char* argv[])
{
    auto ret = AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Oddysee\\R1.LVL", 15);
    return ret.errorCode;
}

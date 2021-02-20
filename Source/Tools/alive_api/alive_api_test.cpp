#include "alive_api.hpp"
#include "SDL.h"

int main(int argc, char* argv[])
{
    auto ret = AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Exoddus\\MI.LVL", 1);
    return ret.errorCode;
}

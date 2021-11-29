#pragma once

#include "JsonModelTypes.hpp"
#include <fstream>

namespace ReliveAPI {
// Reads the root fields to read the version/game type (we need to know this so we can create a game specific reader/do an upgrade of the json).
class JsonMapRootInfoReader final
{
public:
    void Read(const std::string& fileName);
    MapRootInfo mMapRootInfo;
};

void readFileContentsIntoString(std::string& target, std::ifstream& ifs);

std::string& getStaticStringBuffer();
} // namespace ReliveAPI

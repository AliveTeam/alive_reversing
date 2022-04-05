#pragma once

#include "JsonModelTypes.hpp"

namespace ReliveAPI {
class IFileIO;
class IFile;

    // Reads the root fields to read the version/game type (we need to know this so we can create a game specific reader/do an upgrade of the json).
class JsonMapRootInfoReader final
{
public:
    void Read(IFileIO& fileIO, const std::string& fileName);
    MapRootInfo mMapRootInfo;
};

void readFileContentsIntoString(std::string& target, IFile& file);

std::string& getStaticStringBuffer();
} // namespace ReliveAPI

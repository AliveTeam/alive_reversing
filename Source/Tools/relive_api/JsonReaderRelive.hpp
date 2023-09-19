#pragma once

#include "JsonReaderBase.hpp"

namespace ReliveAPI {
class Context;
struct LoadedJsonRelive final
{
    std::vector<CameraNameAndTlvBlob> mPerCamData;
    std::set<AnimId> mResourcesRequiredInLvl;
    std::vector<::PathLine> mCollisions;
};

class JsonReaderRelive final : public JsonReaderBase
{
public:
    LoadedJsonRelive Load(IFileIO& fileIO, const std::string& fileName, Context& context);
};
} // namespace ReliveAPI

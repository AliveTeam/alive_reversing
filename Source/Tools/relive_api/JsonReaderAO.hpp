#pragma once

#include "JsonReaderBase.hpp"

namespace ReliveAPI {
class Context;
struct LoadedJsonAO final
{
    std::vector<CameraNameAndTlvBlob> mPerCamData;
    std::set<AnimId> mResourcesRequiredInLvl;
    std::vector<AO::PathLine> mCollisions;
};

class JsonReaderAO final : public JsonReaderBase
{
public:
    LoadedJsonAO Load(IFileIO& fileIO, const std::string& fileName, Context& context);
};
} // namespace ReliveAPI

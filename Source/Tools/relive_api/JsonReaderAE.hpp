#pragma once

#include "JsonReaderBase.hpp"

namespace ReliveAPI {
class Context;
struct LoadedJsonAE final
{
    std::vector<CameraNameAndTlvBlob> mPerCamData;
    std::set<AnimId> mResourcesRequiredInLvl;
    std::vector<::PathLineAE> mCollisions;
};

class JsonReaderAE final : public JsonReaderBase
{
public:
    LoadedJsonAE Load(IFileIO& fileIO, const std::string& fileName, Context& context);
};
} // namespace ReliveAPI

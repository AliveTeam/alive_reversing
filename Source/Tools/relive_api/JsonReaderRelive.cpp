#include "JsonReaderRelive.hpp"
#include "TypesCollectionRelive.hpp"
#include "JsonReadUtils.hpp"

namespace ReliveAPI {
LoadedJsonRelive JsonReaderRelive::Load(IFileIO& fileIO, const std::string& fileName, Context& context)
{
    TypesCollectionRelive globalTypes;
    LoadedJsonBase loadedJsonBase = JsonReaderBase::Load(globalTypes, fileIO, fileName, context);

    const nlohmann::json& collisionsObject = ReadObject(loadedJsonBase.mMapJson, "collisions");
    const nlohmann::json& collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<::PathLine> lines = ReadReliveLines(globalTypes, collisionsArray, context);

    LoadedJsonRelive ret;
    ret.mResourcesRequiredInLvl = std::move(loadedJsonBase.mResourcesRequiredInLvl);
    ret.mPerCamData = std::move(loadedJsonBase.mPerCamData);
    ret.mCollisions = std::move(lines);

    return ret;
}
} // namespace ReliveAPI

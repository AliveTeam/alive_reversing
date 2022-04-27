#include "JsonReaderAO.hpp"
#include "TypesCollectionAO.hpp"
#include "JsonReadUtils.hpp"

namespace ReliveAPI {
LoadedJsonAO JsonReaderAO::Load(IFileIO& fileIO, const std::string& fileName, Context& context)
{
    TypesCollectionAO globalTypes;
    LoadedJsonBase loadedJsonBase = JsonReaderBase::Load(globalTypes, fileIO, fileName, context);

    const jsonxx::Object& collisionsObject = ReadObject(loadedJsonBase.mMapJson, "collisions");
    const jsonxx::Array& collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<AO::PathLine> lines = ReadAOLines(globalTypes, collisionsArray, context);

    LoadedJsonAO ret;
    ret.mResourcesRequiredInLvl = std::move(loadedJsonBase.mResourcesRequiredInLvl);
    ret.mPerCamData = std::move(loadedJsonBase.mPerCamData);
    ret.mCollisions = std::move(lines);
    return ret;
}
} // namespace ReliveAPI

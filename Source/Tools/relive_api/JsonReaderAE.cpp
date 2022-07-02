#include "JsonReaderAE.hpp"
#include "TypesCollectionAE.hpp"
#include "JsonReadUtils.hpp"

namespace ReliveAPI {
LoadedJsonAE JsonReaderAE::Load(IFileIO& fileIO, const std::string& fileName, Context& context)
{
    TypesCollectionAE globalTypes;
    LoadedJsonBase loadedJsonBase = JsonReaderBase::Load(globalTypes, fileIO, fileName, context);

    const jsonxx::Object& collisionsObject = ReadObject(loadedJsonBase.mMapJson, "collisions");
    const jsonxx::Array& collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<::PathLineAE> lines = ReadAELines(globalTypes, collisionsArray, context);

    LoadedJsonAE ret;
    ret.mResourcesRequiredInLvl = std::move(loadedJsonBase.mResourcesRequiredInLvl);
    ret.mPerCamData = std::move(loadedJsonBase.mPerCamData);
    ret.mCollisions = std::move(lines);

    return ret;
}
} // namespace ReliveAPI

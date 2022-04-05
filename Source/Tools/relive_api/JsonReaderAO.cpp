#include "JsonReaderAO.hpp"
#include "TypesCollectionAO.hpp"
#include "JsonReadUtils.hpp"

namespace ReliveAPI {
std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> JsonReaderAO::Load(IFileIO& fileIO, const std::string& fileName)
{
    TypesCollectionAO globalTypes;
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(globalTypes, fileIO, fileName);

    const jsonxx::Object& collisionsObject = ReadObject(mapJsonObject, "collisions");
    const jsonxx::Array& collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<AO::PathLine> lines = ReadAOLines(globalTypes, collisionsArray);

    return {std::move(mapData), std::move(lines)};
}
} // namespace ReliveAPI

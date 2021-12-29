#include "JsonReaderAE.hpp"
#include "TypesCollectionAE.hpp"
#include "JsonReadUtils.hpp"

namespace ReliveAPI {
std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<::PathLine>> JsonReaderAE::Load(const std::string& fileName)
{
    TypesCollectionAE globalTypes;
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(globalTypes, fileName);

    const jsonxx::Object& collisionsObject = ReadObject(mapJsonObject, "collisions");
    const jsonxx::Array& collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<::PathLine> lines = ReadAELines(globalTypes, collisionsArray);

    return {std::move(mapData), std::move(lines)};
}
} // namespace ReliveAPI

#pragma once

#include "JsonWriterBase.hpp"
#include "../../relive_lib/ObjectTypes.hpp"

#include <memory>
#include <unordered_map>

namespace ReliveAPI {
class IFileIO;
class TypesCollectionRelive;
class JsonWriterRelive final : public JsonWriterBase
{
private:
    JsonWriterRelive(Game gameType, std::unique_ptr<TypesCollectionRelive>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info);

public:
    JsonWriterRelive(Game gameType, s32 pathId, const std::string& pathBndName, const PathInfo& info);
    ~JsonWriterRelive();

private:
    void ResetTypeCounterMap() override;
    [[nodiscard]] nlohmann::json ReadCollisionStream(u8* ptr, s32 numItems, Context& context) override;
    [[nodiscard]] nlohmann::json ReadTlvStream(u8* ptr, Context& context) override;
    [[nodiscard]] nlohmann::json AddCollisionLineStructureJson() override;

    std::unordered_map<ReliveTypes, s32> mTypeCounterMap;
    std::unique_ptr<TypesCollectionRelive> mTypesCollection;
};
} // namespace ReliveAPI

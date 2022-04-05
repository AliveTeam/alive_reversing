#pragma once

#include "JsonWriterBase.hpp"
#include <memory>
#include <unordered_map>

class TypesCollectionAO;

namespace ReliveAPI {
class IFileIO;
class JsonWriterAO final : public JsonWriterBase
{
private:
    JsonWriterAO(std::unique_ptr<TypesCollectionAO>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info);

public:
    JsonWriterAO(s32 pathId, const std::string& pathBndName, const PathInfo& info);
    ~JsonWriterAO();

    void DebugDumpTlvs(IFileIO& fileIo, const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource) override;

private:
    void ResetTypeCounterMap() override;
    [[nodiscard]] jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems) override;
    [[nodiscard]] jsonxx::Array ReadTlvStream(u8* ptr) override;
    [[nodiscard]] jsonxx::Array AddCollisionLineStructureJson() override;

    std::unordered_map<AO::TlvTypes, s32> mTypeCounterMap;
    std::unique_ptr<TypesCollectionAO> mTypesCollection;
};
} // namespace ReliveAPI

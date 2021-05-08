#pragma once

#include "JsonWriterBase.hpp"

class JsonWriterAE final : public JsonWriterBase
{
private:
    JsonWriterAE(std::unique_ptr<TypesCollectionAE>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info);

public:
    JsonWriterAE(s32 pathId, const std::string& pathBndName, const PathInfo& info);
    ~JsonWriterAE();

    void DebugDumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource) override;

private:
    void ResetTypeCounterMap() override;
    [[nodiscard]] jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems) override;
    [[nodiscard]] jsonxx::Array ReadTlvStream(u8* ptr) override;
    [[nodiscard]] jsonxx::Array AddCollisionLineStructureJson() override;

    std::unordered_map<::TlvTypes, s32> mTypeCounterMap;
    std::unique_ptr<TypesCollectionAE> mTypesCollection;
};

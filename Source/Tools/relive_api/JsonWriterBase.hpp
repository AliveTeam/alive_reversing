#pragma once

#include "JsonModelTypes.hpp"

class JsonWriterBase
{
public:
    JsonWriterBase(TypesCollectionBase& types, s32 pathId, const std::string& pathBndName, const PathInfo& info);
    virtual ~JsonWriterBase();

    void Save(const PathInfo& info, std::vector<u8>& pathResource, const std::string& fileName);
    virtual void DebugDumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource) = 0;

    virtual jsonxx::Array ReadTlvStream(u8* ptr) = 0;
    virtual jsonxx::Array AddCollisionLineStructureJson() = 0;

protected:
    template <typename T>
    static void DebugDumpTlv(const std::string& prefix, s32 idx, const T& tlv)
    {
        const std::string fileName = prefix + "_" + std::to_string(static_cast<s32>(tlv.field_4_type.mType)) + "_" + std::to_string(idx) + ".dat";
        FILE* hFile = ::fopen(fileName.c_str(), "wb");
        if (!hFile)
        {
            throw ReliveAPI::IOWriteException(fileName.c_str());
        }
        ::fwrite(&tlv, tlv.field_2_length, 1, hFile);
        ::fclose(hFile);
    }

    virtual jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems) = 0;
    virtual void ResetTypeCounterMap() = 0;

    MapRootInfo mMapRootInfo;
    MapInfo mMapInfo;
    TypesCollectionBase& mBaseTypesCollection;
};

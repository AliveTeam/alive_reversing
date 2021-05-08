#pragma once

#include "JsonModelTypes.hpp"

struct Path_TLV;

namespace AO {
    struct Path_TLV;
}

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
    static void DebugDumpTlv(const std::string& prefix, s32 idx, const Path_TLV& tlv);
    static void DebugDumpTlv(const std::string& prefix, s32 idx, const AO::Path_TLV& tlv);

    virtual jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems) = 0;
    virtual void ResetTypeCounterMap() = 0;

    MapRootInfo mMapRootInfo;
    MapInfo mMapInfo;
    TypesCollectionBase& mBaseTypesCollection;
};

class PathCamerasEnumerator final
{
public:
    PathCamerasEnumerator(const PathInfo& pathInfo, const std::vector<u8>& pathResource);

    using TFnOnCamera = std::function<void(CameraObject& cam)>;
    void Enumerate(TFnOnCamera onCamera);

private:
    const PathInfo& mPathInfo;
    const std::vector<u8>& mPathResource;
};

#pragma once

#include "JsonModelTypes.hpp"
#include <functional>

struct Path_TLV;

namespace AO {
    struct Path_TLV;
}

namespace ReliveAPI {

inline s32 To1dIndex(s32 width, s32 x, s32 y)
{
    return x + (y * width);
}

class LvlReader;
class IFileIO;
class Context;

class JsonWriterBase
{
public:
    JsonWriterBase(TypesCollectionBase& types, s32 pathId, const std::string& pathBndName, const PathInfo& info);
    virtual ~JsonWriterBase();

    void Save(std::vector<u8>& fileDataBuffer, LvlReader& lvlReader, const PathInfo& info, std::vector<u8>& pathResource, IFileIO& fileIO, const std::string& fileName, Context& context);
    virtual void DebugDumpTlvs(IFileIO& fileIo, const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource) = 0;

    virtual jsonxx::Array ReadTlvStream(u8* ptr, Context& context) = 0;
    virtual jsonxx::Array AddCollisionLineStructureJson() = 0;

protected:
    void ProcessCamera(std::vector<u8>& fileDataBuffer, LvlReader& lvlReader, const PathInfo& info, const s32* indexTable, const CameraObject& tmpCamera, jsonxx::Array& cameraArray, u8* pPathData, Context& context);

    static void DebugDumpTlv(IFileIO& fileIo, const std::string& prefix, s32 idx, const Path_TLV& tlv);
    static void DebugDumpTlv(IFileIO& fileIo, const std::string& prefix, s32 idx, const AO::Path_TLV& tlv);

    virtual jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems, Context& context) = 0;
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
} // namespace ReliveAPI

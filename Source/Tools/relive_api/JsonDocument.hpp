#pragma once

#include <jsonxx/jsonxx.h>

#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <utility>
#include <unordered_map>

enum class Game;
class TypesCollectionBase;
class TypesCollectionAO;
class TypesCollectionAE;

class PathLine;
enum class TlvTypes : s16;

namespace AO
{
    class PathLine;
    enum class TlvTypes : s16;
}

inline s32 To1dIndex(s32 width, s32 x, s32 y)
{
    return x + (y * width);
}

struct CameraObject
{
    std::string mName;
    s32 mId = 0;
    s32 mX = 0;
    s32 mY = 0;

    [[nodiscard]] jsonxx::Object ToJsonObject(jsonxx::Array mapObjectsArray) const;
};

struct PathInfo
{
    s32 mGridWidth = 0;
    s32 mWidth = 0;
    s32 mGridHeight = 0;
    s32 mHeight = 0;
    u32 mObjectOffset = 0;
    u32 mIndexTableOffset = 0;

    s32 mNumCollisionItems = 0;
    s32 mCollisionOffset = 0;
};

struct CameraNameAndTlvBlob
{
    s32 mId = 0;
    s32 x = 0;
    s32 y = 0;
    std::string mName;
    std::vector<std::vector<u8>> mTlvBlobs;

    [[nodiscard]] std::size_t TotalTlvSize() const;
};

struct MapRootInfo
{
    s32 mVersion = 0;
    std::string mGame;
};

struct MapInfo
{
    std::string mPathBnd;
    s32 mPathId = 0;

    s32 mXGridSize = 0;
    s32 mXSize = 0;

    s32 mYGridSize = 0;
    s32 mYSize = 0;
};

// Reads the root fields to read the version/game type (we need to know this so we can create a game specific reader/do an upgrade of the json).
class JsonMapRootInfoReader final
{
public:
    void Read(const std::string& fileName);
    MapRootInfo mMapRootInfo;
};

class JsonReaderBase
{
public:
    MapInfo mRootInfo;

protected:
    std::pair<std::vector<CameraNameAndTlvBlob>, jsonxx::Object> Load(TypesCollectionBase& types, const std::string& fileName);

    std::vector<AO::PathLine> ReadAOLines(TypesCollectionBase& types, jsonxx::Array& collisionsArray);
    std::vector<::PathLine> ReadAELines(TypesCollectionBase& types, jsonxx::Array& collisionsArray);
};

class JsonReaderAO : public JsonReaderBase
{
public:
    std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> Load(const std::string& fileName);
};

class JsonReaderAE : public JsonReaderBase
{
public:
    std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<::PathLine>> Load(const std::string& fileName);
};

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
    virtual jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems) = 0;
    virtual void ResetTypeCounterMap() = 0;

    MapRootInfo mMapRootInfo;
    MapInfo mMapInfo;
    TypesCollectionBase& mBaseTypesCollection;
};

class JsonWriterAO : public JsonWriterBase
{
private:
    JsonWriterAO(std::unique_ptr<TypesCollectionAO>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info);

public:
    JsonWriterAO(s32 pathId, const std::string& pathBndName, const PathInfo& info);
    ~JsonWriterAO();

    void DebugDumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource) override;

private:
    void ResetTypeCounterMap() override;
    [[nodiscard]] jsonxx::Array ReadCollisionStream(u8* ptr, s32 numItems) override;
    [[nodiscard]] jsonxx::Array ReadTlvStream(u8* ptr) override;
    [[nodiscard]] jsonxx::Array AddCollisionLineStructureJson() override;

    std::unordered_map<AO::TlvTypes, s32> mTypeCounterMap;
    std::unique_ptr<TypesCollectionAO> mTypesCollection;
};

class JsonWriterAE : public JsonWriterBase
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

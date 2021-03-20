#pragma once

#include <string>
#include <vector>
#include <jsonxx/jsonxx.h>
#include "../AliveLibAO/Collisions.hpp"
#include "../AliveLibAE/Collisions.hpp"

enum class TlvTypes : short;

namespace AO
{
    enum class TlvTypes : short;
}

inline int To1dIndex(int width, int x, int y)
{
    return x + (y * width);
}

struct CameraObject
{
    std::string mName;
    int mId = 0;
    int mX = 0;
    int mY = 0;

    jsonxx::Object ToJsonObject(jsonxx::Array mapObjectsArray) const
    {
        jsonxx::Object obj;

        obj << "name" << mName;
        obj << "x" << mX;
        obj << "y" << mY;
        obj << "id" << mId;
        obj << "map_objects" << mapObjectsArray;
        
        return obj;
    }
};


struct PathInfo
{
    int mGridWidth = 0;
    int mWidth = 0;
    int mGridHeight = 0;
    int mHeight = 0;
    unsigned int mObjectOffset = 0;
    unsigned int mIndexTableOffset = 0;

    int mNumCollisionItems = 0;
    int mCollisionOffset = 0;
};

struct CameraNameAndTlvBlob
{
    int mId = 0;
    int x = 0;
    int y = 0;
    std::string mName;
    std::vector<std::vector<BYTE>> mTlvBlobs;

    std::size_t TotalTlvSize() const
    {
        std::size_t allTlvsLen = 0;
        for (const auto& tlv : mTlvBlobs)
        {
            allTlvsLen += tlv.size();
        }
        return allTlvsLen;
    }
};

struct MapRootInfo
{
    int mVersion = 0;
    std::string mGame;
};

struct MapInfo
{
    std::string mPathBnd;
    int mPathId = 0;

    int mXGridSize = 0;
    int mXSize = 0;

    int mYGridSize = 0;
    int mYSize = 0;
};

// Reads the root fields to read the version/game type (we need to know this so we can create a game specific reader/do an upgrade of the json).
class JsonMapRootInfoReader
{
public:
    bool Read(const std::string& fileName);
    MapRootInfo mMapRootInfo;
};

enum class Game;

class JsonReaderBase
{
public:
    MapInfo mRootInfo;
protected:
    std::pair<std::vector<CameraNameAndTlvBlob>,jsonxx::Object> Load(Game gameType, const std::string& fileName);

    std::vector<AO::PathLine> ReadAOLines(jsonxx::Array& collisionsArray);
    std::vector<::PathLine> ReadAELines(jsonxx::Array& collisionsArray);
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

class TypesCollection;

class JsonWriterBase
{
public:
    virtual ~JsonWriterBase() { }
    JsonWriterBase(int pathId, const std::string& pathBndName, const PathInfo& info);
    void Save(const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName);
    virtual void DumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<BYTE>& pathResource) = 0;
protected:
    virtual jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems) = 0;
    virtual jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr) = 0;
    virtual std::unique_ptr<TypesCollection> MakeTypesCollection() const = 0;
    virtual void ResetTypeCounterMap() = 0;
protected:
    MapRootInfo mMapRootInfo;
    MapInfo mMapInfo;
};

class JsonWriterAO : public JsonWriterBase
{
public:
    JsonWriterAO(int pathId, const std::string& pathBndName, const PathInfo& info);
    void DumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<BYTE>& pathResource) override;
private:
    void ResetTypeCounterMap() override;
    jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems) override;
    jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr) override;
    std::unique_ptr<TypesCollection> MakeTypesCollection() const override;
    std::map<AO::TlvTypes, int> mTypeCounterMap;
};

class JsonWriterAE : public JsonWriterBase
{
public:
    JsonWriterAE(int pathId, const std::string& pathBndName, const PathInfo& info);
    void DumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<BYTE>& pathResource) override;
private:
    void ResetTypeCounterMap() override;
    jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems) override;
    jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr) override;
    std::unique_ptr<TypesCollection> MakeTypesCollection() const override;
    std::map<::TlvTypes, int> mTypeCounterMap;

};

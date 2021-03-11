#pragma once

#include <string>
#include <vector>
#include <jsonxx/jsonxx.h>
#include "../AliveLibAO/Collisions.hpp"

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
};

struct MapRootInfo
{
    int mVersion = 0;
    std::string mGame;

    std::string mPathBnd;
    int mPathId = 0;

    int mXGridSize = 0;
    int mXSize = 0;

    int mYGridSize = 0;
    int mYSize = 0;
};

class JsonReaderAO
{
public:
    std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> Load(const std::string& fileName);

    MapRootInfo mRootInfo;
};

class TypesCollection;

class JsonWriterBase
{
public:
    virtual ~JsonWriterBase() { }
    JsonWriterBase(int pathId, const std::string& pathBndName, const PathInfo& info);
    void Save(const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName);
protected:
    virtual jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems) = 0;
    virtual jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr) = 0;
protected:
    MapRootInfo mRootInfo;
};

class JsonWriterAO : public JsonWriterBase
{
public:
    JsonWriterAO(int pathId, const std::string& pathBndName, const PathInfo& info);
private:
    jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems) override;
    jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr) override;
};

class JsonWriterAE : public JsonWriterBase
{
public:
    JsonWriterAE(int pathId, const std::string& pathBndName, const PathInfo& info);
private:
    jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems) override;
    jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr) override;
};

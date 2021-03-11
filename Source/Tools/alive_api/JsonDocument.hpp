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

class JsonWriterAO
{
public:
    JsonWriterAO(const std::string& pathBndName, const PathInfo& info);
    void SaveAO(int pathId, const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName);
private:
    jsonxx::Array ReadCollisionStream(BYTE* ptr, int numItems);
    jsonxx::Array ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr);

    MapRootInfo mRootInfo;
};

class JsonWriterAE
{
public:

    void SaveAE(int pathId, const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName);
};

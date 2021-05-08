#include "JsonWriterBase.hpp"
#include "TypesCollectionBase.hpp"
#include "relive_api.hpp"
#include <jsonxx/jsonxx.h>
#include "../AliveLibAO/Map.hpp"

inline s32 To1dIndex(s32 width, s32 x, s32 y)
{
    return x + (y * width);
}

JsonWriterBase::~JsonWriterBase() = default;

JsonWriterBase::JsonWriterBase(TypesCollectionBase& types, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : mBaseTypesCollection(types)
{
    mMapInfo.mPathId = pathId;
    mMapInfo.mPathBnd = pathBndName;
    mMapInfo.mXGridSize = info.mGridWidth;
    mMapInfo.mYGridSize = info.mGridHeight;

    mMapInfo.mXSize = info.mWidth;
    mMapInfo.mYSize = info.mHeight;

    mMapRootInfo.mVersion = ReliveAPI::GetApiVersion();
}

void JsonWriterBase::Save(const PathInfo& info, std::vector<u8>& pathResource, const std::string& fileName)
{
    ResetTypeCounterMap();

    jsonxx::Object rootObject;

    rootObject << "api_version" << mMapRootInfo.mVersion;

    rootObject << "game" << mMapRootInfo.mGame;

    jsonxx::Object rootMapObject;
    rootMapObject << "path_bnd" << mMapInfo.mPathBnd;
    rootMapObject << "path_id" << mMapInfo.mPathId;

    rootMapObject << "x_grid_size" << mMapInfo.mXGridSize;
    rootMapObject << "x_size" << mMapInfo.mXSize;

    rootMapObject << "y_grid_size" << mMapInfo.mYGridSize;
    rootMapObject << "y_size" << mMapInfo.mYSize;

    u8* pPathData = pathResource.data();


    u8* pLineIter = pPathData + info.mCollisionOffset;
    jsonxx::Array collisionsArray = ReadCollisionStream(pLineIter, info.mNumCollisionItems);
    jsonxx::Object colllisionObject;
    colllisionObject << "structure" << AddCollisionLineStructureJson();
    colllisionObject << "items" << collisionsArray;

    rootMapObject << "collisions" << colllisionObject;

    const s32* indexTable = reinterpret_cast<const s32*>(pPathData + info.mIndexTableOffset);


    jsonxx::Array cameraArray;
    for (s32 y = 0; y < info.mHeight; y++)
    {
        for (s32 x = 0; x < info.mWidth; x++)
        {
            auto pCamName = reinterpret_cast<const AO::CameraName*>(&pPathData[To1dIndex(info.mWidth, x, y) * sizeof(AO::CameraName)]);
            CameraObject tmpCamera;
            tmpCamera.mX = x;
            tmpCamera.mY = y;
            if (pCamName->name[0])
            {
                tmpCamera.mName = std::string(pCamName->name, 8);
                tmpCamera.mId = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0') + 100 * (pCamName->name[4] - '0') + 1000 * (pCamName->name[3] - '0');
            }

            const s32 indexTableEntryOffset = indexTable[To1dIndex(info.mWidth, x, y)];
            if (indexTableEntryOffset == -1 || indexTableEntryOffset >= 0x100000)
            {
                if (pCamName->name[0])
                {
                    // LOG_INFO("Add camera with no objects " << tmpCamera.mName);
                    cameraArray << tmpCamera.ToJsonObject({});
                }
            }
            else
            {
                // Can have objects that do not live in a camera, as strange as it seems (R1P15)
                // "blank" cameras just do not have a name set.

                u8* ptr = pPathData + indexTableEntryOffset + info.mObjectOffset;
                jsonxx::Array mapObjects = ReadTlvStream(ptr);
                // LOG_INFO("Add camera " << tmpCamera.mName);
                cameraArray << tmpCamera.ToJsonObject(mapObjects);
            }
        }
    }

    rootMapObject << "cameras" << cameraArray;

    jsonxx::Object schemaObject;
    schemaObject << "object_structure_property_basic_types" << mBaseTypesCollection.BasicTypesToJson();
    schemaObject << "object_structure_property_enums" << mBaseTypesCollection.EnumsToJson();

    jsonxx::Array objectStructuresArray;
    mBaseTypesCollection.AddTlvsToJsonArray(objectStructuresArray);
    schemaObject << "object_structures" << objectStructuresArray;

    rootObject << "map" << rootMapObject;
    rootObject << "schema" << schemaObject;

    std::ofstream s(fileName.c_str());
    if (s)
    {
        s << rootObject.json();
    }
    else
    {
        throw ReliveAPI::IOWriteException(fileName.c_str());
    }
}

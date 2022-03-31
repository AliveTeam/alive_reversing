#include "JsonWriterBase.hpp"
#include "TypesCollectionBase.hpp"
#include "relive_api.hpp"
#include <jsonxx/jsonxx.h>
#include "../../AliveLibAO/Map.hpp"
#include "LvlReaderWriter.hpp"
#include "CamConverter.hpp"

namespace ReliveAPI {

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

    mMapInfo.mAbeStartXPos = info.mAbeStartXPos;
    mMapInfo.mAbeStartYPos = info.mAbeStartYPos;

    mMapInfo.mNumMudsInPath = info.mNumMudsInPath;

    mMapInfo.mTotalMuds = info.mTotalMuds;
    mMapInfo.mBadEndingMuds = info.mBadEndingMuds;
    mMapInfo.mGoodEndingMuds = info.mGoodEndingMuds;

    mMapInfo.mLCDScreenMessages = info.mLCDScreenMessages;
    mMapInfo.mHintFlyMessages = info.mHintFlyMessages;

    mMapRootInfo.mVersion = ReliveAPI::GetApiVersion();
}

void JsonWriterBase::ProcessCamera(std::vector<u8>& fileDataBuffer, LvlReader& lvlReader, const PathInfo& info, const s32* indexTable, const CameraObject& tmpCamera, jsonxx::Array& cameraArray, u8* pPathData)
{
    bool addCameraToJsonArray = false;
    const s32 indexTableEntryOffset = indexTable[To1dIndex(info.mWidth, tmpCamera.mX, tmpCamera.mY)];
    jsonxx::Array mapObjects;
    if (indexTableEntryOffset == -1)
    {
        if (!tmpCamera.mName.empty())
        {
            // LOG_INFO("Add camera with no objects " << tmpCamera.mName);
            addCameraToJsonArray = true;
        }
    }
    else
    {
        // Can have objects that do not live in a camera, as strange as it seems (R1P15)
        // "blank" cameras just do not have a name set.

        u8* ptr = pPathData + indexTableEntryOffset + info.mObjectOffset;
        mapObjects = ReadTlvStream(ptr);
        addCameraToJsonArray = true;
        // LOG_INFO("Add camera " << tmpCamera.mName);
    }

    CameraImageAndLayers cameraImageAndLayers;
    if (!tmpCamera.mName.empty())
    {
        const std::string cameraName = tmpCamera.mName + ".CAM";
        if (lvlReader.ReadFileInto(fileDataBuffer, cameraName.c_str()))
        {
            ChunkedLvlFile camFile(fileDataBuffer);
            CamConverter converter(camFile, cameraImageAndLayers);
        }
        else
        {
            LOG_WARNING("Camera " << tmpCamera.mName << " not found in the LVL");
        }
    }

    if (addCameraToJsonArray)
    {
        cameraArray << tmpCamera.ToJsonObject(mapObjects, cameraImageAndLayers);
    }
}

void JsonWriterBase::Save(std::vector<u8>& fileDataBuffer, LvlReader& lvlReader, const PathInfo& info, std::vector<u8>& pathResource, const std::string& fileName)
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

    rootMapObject << "abe_start_xpos" << mMapInfo.mAbeStartXPos;
    rootMapObject << "abe_start_ypos" << mMapInfo.mAbeStartYPos;

    rootMapObject << "num_muds_in_path" << mMapInfo.mNumMudsInPath;

    rootMapObject << "total_muds" << mMapInfo.mTotalMuds;
    rootMapObject << "num_muds_for_bad_ending" << mMapInfo.mBadEndingMuds;
    rootMapObject << "num_muds_for_good_ending" << mMapInfo.mGoodEndingMuds;


    jsonxx::Array LCDScreenMessagesArray;
    for (const auto& msg : mMapInfo.mLCDScreenMessages)
    {
        LCDScreenMessagesArray << msg;
    }
    rootMapObject << "lcdscreen_messages" << LCDScreenMessagesArray;

    jsonxx::Array hintFlyMessagesArray;
    for (const auto& msg : mMapInfo.mHintFlyMessages)
    {
        hintFlyMessagesArray << msg;
    }
    rootMapObject << "hintfly_messages" << hintFlyMessagesArray;

    u8* pPathData = pathResource.data();

    u8* pLineIter = pPathData + info.mCollisionOffset;
    jsonxx::Array collisionsArray = ReadCollisionStream(pLineIter, info.mNumCollisionItems);
    jsonxx::Object colllisionObject;
    colllisionObject << "structure" << AddCollisionLineStructureJson();
    colllisionObject << "items" << collisionsArray;

    rootMapObject << "collisions" << colllisionObject;

    const s32* indexTable = reinterpret_cast<const s32*>(pPathData + info.mIndexTableOffset);

    jsonxx::Array cameraArray;
    PathCamerasEnumerator cameraEnumerator(info, pathResource);
    cameraEnumerator.Enumerate([&](const CameraObject& tmpCamera)
        { 
            ProcessCamera(fileDataBuffer, lvlReader, info, indexTable, tmpCamera, cameraArray, pPathData);
        });

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

void JsonWriterBase::DebugDumpTlv(const std::string& prefix, s32 idx, const Path_TLV& tlv)
{
    ReliveAPI::DebugDumpTlv(prefix, idx, tlv);
}

void JsonWriterBase::DebugDumpTlv(const std::string& prefix, s32 idx, const AO::Path_TLV& tlv)
{
    ReliveAPI::DebugDumpTlv(prefix, idx, tlv);
}

PathCamerasEnumerator::PathCamerasEnumerator(const PathInfo& pathInfo, const std::vector<u8>& pathResource)
    : mPathInfo(pathInfo)
    , mPathResource(pathResource)
{
}

void PathCamerasEnumerator::Enumerate(TFnOnCamera onCamera)
{
    const u8* pPathData = mPathResource.data();
    jsonxx::Array cameraArray;
    for (s32 y = 0; y < mPathInfo.mHeight; y++)
    {
        for (s32 x = 0; x < mPathInfo.mWidth; x++)
        {
            // AO::CameraName is the same as AE structure
            auto pCamName = reinterpret_cast<const AO::CameraName*>(&pPathData[To1dIndex(mPathInfo.mWidth, x, y) * sizeof(AO::CameraName)]);
            CameraObject tmpCamera;
            tmpCamera.mX = x;
            tmpCamera.mY = y;
            if (pCamName->name[0])
            {
                tmpCamera.mName = std::string(pCamName->name, 8);
                tmpCamera.mId = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0') + 100 * (pCamName->name[4] - '0') + 1000 * (pCamName->name[3] - '0');
            }

            onCamera(tmpCamera);
        }
    }
}
} // namespace ReliveAPI

#include "JsonWriterBase.hpp"
#include "TypesCollectionBase.hpp"
#include "relive_api.hpp"
#include <jsonxx/jsonxx.h>
#include "../../AliveLibAO/Map.hpp"
#include "LvlReaderWriter.hpp"
#include "CamConverter.hpp"
#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"
#include "../../relive_lib/data_conversion/PathTlvsAO.hpp"

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

void JsonWriterBase::ProcessCamera(std::vector<u8>& fileDataBuffer, LvlReader& lvlReader, const PathInfo& info, const s32* indexTable, const CameraObject& tmpCamera, jsonxx::Array& cameraArray, u8* pPathData, Context& context)
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
        mapObjects = ReadTlvStream(ptr, context);
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
            LOG_WARNING("Camera %s not found in the LVL", tmpCamera.mName.c_str());
        }
    }

    if (addCameraToJsonArray)
    {
        cameraArray << tmpCamera.ToJsonObject(mapObjects, cameraImageAndLayers);
    }
}

void JsonWriterBase::Save(std::vector<u8>& fileDataBuffer, LvlReader& lvlReader, const PathInfo& info, std::vector<u8>& pathResource, IFileIO& fileIO, const std::string& fileName, Context& context)
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
    jsonxx::Array collisionsArray = ReadCollisionStream(pLineIter, info.mNumCollisionItems, context);
    jsonxx::Object colllisionObject;
    colllisionObject << "structure" << AddCollisionLineStructureJson();
    colllisionObject << "items" << collisionsArray;

    rootMapObject << "collisions" << colllisionObject;

    const s32* indexTable = reinterpret_cast<const s32*>(pPathData + info.mIndexTableOffset);

    jsonxx::Array cameraArray;
    PathCamerasEnumerator cameraEnumerator(info.mWidth, info.mHeight, pathResource);
    cameraEnumerator.Enumerate([&](const CameraEntry& tmpCamera)
        { 
            CameraObject camObj;
            camObj.mId = tmpCamera.mId;
            camObj.mName = tmpCamera.mName;
            camObj.mX = tmpCamera.mX;
            camObj.mY = tmpCamera.mY;
            ProcessCamera(fileDataBuffer, lvlReader, info, indexTable, camObj, cameraArray, pPathData, context);
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

    auto s = fileIO.Open(fileName, IFileIO::Mode::Write);
    if (s->IsOpen())
    {
        s->Write(rootObject.json());
    }
    else
    {
        throw ReliveAPI::IOWriteException(fileName.c_str());
    }
}

template <typename T>
static void DebugDumpTlv(IFileIO& fileIo, const std::string& prefix, s32 idx, const T& tlv)
{
    const std::string fileName = prefix + "_" + std::to_string(static_cast<s32>(tlv.mTlvType32.mType)) + "_" + std::to_string(idx) + ".dat";
    auto hFile = fileIo.Open(fileName, IFileIO::Mode::WriteBinary);
    if (!hFile->IsOpen())
    {
        throw ReliveAPI::IOWriteException(fileName.c_str());
    }
    hFile->Write(reinterpret_cast<const u8*>(&tlv), tlv.mLength);
}

void JsonWriterBase::DebugDumpTlv(IFileIO& fileIo, const std::string& prefix, s32 idx, const Path_TLV& tlv)
{
    ReliveAPI::DebugDumpTlv(fileIo, prefix, idx, tlv);
}

void JsonWriterBase::DebugDumpTlv(IFileIO& fileIo, const std::string& prefix, s32 idx, const AO::Path_TLV& tlv)
{
    ReliveAPI::DebugDumpTlv(fileIo, prefix, idx, tlv);
}

} // namespace ReliveAPI

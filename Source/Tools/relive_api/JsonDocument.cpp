#include "JsonDocument.hpp"

#include "relive_api.hpp"
#include "TlvObjectBase.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "TypesCollectionAO.hpp"
#include "TypesCollectionAE.hpp"

#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Collisions.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"

#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAE/Collisions.hpp"

#include "../AliveLibCommon/stdafx_common.h"

#include <magic_enum/include/magic_enum.hpp>
#include <jsonxx/jsonxx.h>

#include <cstddef>
#include <fstream>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

[[nodiscard]] jsonxx::Object CameraObject::ToJsonObject(jsonxx::Array mapObjectsArray) const
{
    jsonxx::Object obj;

    obj << "name" << mName;
    obj << "x" << mX;
    obj << "y" << mY;
    obj << "id" << mId;
    obj << "map_objects" << mapObjectsArray;

    return obj;
}

[[nodiscard]] std::size_t CameraNameAndTlvBlob::TotalTlvSize() const
{
    std::size_t allTlvsLen = 0;

    for (const auto& tlv : mTlvBlobs)
    {
        allTlvsLen += tlv.size();
    }

    return allTlvsLen;
}

class AOLine final : public PropertyCollection
{
public:
    AOLine(TypesCollectionBase& globalTypes, const AO::PathLine* line = nullptr)
    {
        if (line)
        {
            mLine = *line;
        }

        ADD("x1", mLine.field_0_rect.x);
        ADD("y1", mLine.field_0_rect.y);

        ADD("x2", mLine.field_0_rect.w);
        ADD("y2", mLine.field_0_rect.h);

        ADD("type", mLine.field_8_type);

        ADD("next", mLine.field_10_next);
        ADD("previous", mLine.field_C_previous);
    }

    AO::PathLine mLine = {};
};

class AELine final : public PropertyCollection
{
public:
    AELine(TypesCollectionBase& globalTypes, const PathLine* line = nullptr)
    {
        if (line)
        {
            mLine = *line;
        }

        ADD("x1", mLine.field_0_rect.x);
        ADD("y1", mLine.field_0_rect.y);

        ADD("x2", mLine.field_0_rect.w);
        ADD("y2", mLine.field_0_rect.h);

        ADD("type", mLine.field_8_type);

        ADD("next", mLine.field_C_next);
        ADD("previous", mLine.field_A_previous);

        ADD("next2", mLine.field_10_next2);
        ADD("previous2", mLine.field_E_previous2);

        ADD("length", mLine.field_12_line_length);
    }

    PathLine mLine = {};
};

static jsonxx::Array ReadArray(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Array>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::Array>(key);
}

static jsonxx::Object ReadObject(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Object>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::Object>(key);
}

static s32 ReadNumber(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Number>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }
    return static_cast<s32>(o.get<jsonxx::Number>(key));
}

static std::string ReadString(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::String>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::String>(key);
}

std::vector<AO::PathLine> JsonReaderBase::ReadAOLines(TypesCollectionBase& types, jsonxx::Array& collisionsArray)
{
    std::vector<AO::PathLine> lines;
    for (auto i = 0u; i < collisionsArray.values().size(); i++)
    {
        jsonxx::Object collision = collisionsArray.get<jsonxx::Object>(i);
        AOLine tmpLine(types);
        tmpLine.PropertiesFromJson(types, collision);
        lines.emplace_back(tmpLine.mLine);
    }
    return lines;
}

std::vector<::PathLine> JsonReaderBase::ReadAELines(TypesCollectionBase& types, jsonxx::Array& collisionsArray)
{
    std::vector<::PathLine> lines;
    for (auto i = 0u; i < collisionsArray.values().size(); i++)
    {
        jsonxx::Object collision = collisionsArray.get<jsonxx::Object>(i);
        AELine tmpLine(types);
        tmpLine.PropertiesFromJson(types, collision);
        lines.emplace_back(tmpLine.mLine);
    }
    return lines;
}

std::pair<std::vector<CameraNameAndTlvBlob>,jsonxx::Object> JsonReaderBase::Load(TypesCollectionBase& types, const std::string& fileName)
{
    std::ifstream inputFileStream(fileName.c_str());
    if (!inputFileStream.is_open())
    {
        throw ReliveAPI::IOReadException();
    }

    std::string jsonStr((std::istreambuf_iterator<s8>(inputFileStream)), std::istreambuf_iterator<s8>());
    jsonxx::Object rootObj;
    if (!rootObj.parse(jsonStr))
    {
        throw ReliveAPI::InvalidJsonException();
    }

    jsonxx::Object map = ReadObject(rootObj, "map");
    mRootInfo.mPathBnd = ReadString(map, "path_bnd");

    mRootInfo.mPathId = ReadNumber(map, "path_id");

    mRootInfo.mXSize = ReadNumber(map, "x_size");
    mRootInfo.mYSize = ReadNumber(map, "y_size");

    mRootInfo.mXGridSize = ReadNumber(map, "x_grid_size");
    mRootInfo.mYGridSize = ReadNumber(map, "y_grid_size");

    std::vector<CameraNameAndTlvBlob> mapData;

    jsonxx::Array camerasArray = ReadArray(map, "cameras");
    for (auto i = 0u; i < camerasArray.values().size(); i++)
    {
        jsonxx::Object camera = camerasArray.get<jsonxx::Object>(i);

        const s32 x = ReadNumber(camera, "x");
        const s32 y = ReadNumber(camera, "y");
        if (x > mRootInfo.mXSize || y > mRootInfo.mYSize)
        {
            throw ReliveAPI::CameraOutOfBoundsException();
        }

        CameraNameAndTlvBlob cameraNameBlob;
        cameraNameBlob.mId = ReadNumber(camera, "id");
        cameraNameBlob.mName = ReadString(camera, "name");
        cameraNameBlob.x = x;
        cameraNameBlob.y = y;

        jsonxx::Array mapObjectsArray = ReadArray(camera, "map_objects");
        for (auto j = 0u; j < mapObjectsArray.values().size(); j++)
        {
            jsonxx::Object mapObject = mapObjectsArray.get<jsonxx::Object>(j);
            std::string structureType = ReadString(mapObject, "object_structures_type");
            std::unique_ptr<TlvObjectBase> tlv = types.MakeTlvFromString(structureType);
            if (!tlv)
            {
                throw ReliveAPI::UnknownStructureTypeException(structureType.c_str());
            }

            tlv->InstanceFromJson(types, mapObject);
            cameraNameBlob.mTlvBlobs.emplace_back(tlv->GetTlvData(j == mapObjectsArray.values().size() - 1));
        }
        mapData.push_back(cameraNameBlob);
    }
    return { mapData, map };
}

std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> JsonReaderAO::Load(const std::string& fileName)
{
    TypesCollectionAO globalTypes;
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(globalTypes, fileName);

    jsonxx::Object collisionsObject = ReadObject(mapJsonObject, "collisions");
    jsonxx::Array collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<AO::PathLine> lines = ReadAOLines(globalTypes, collisionsArray);

    return { mapData, lines };
}

std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<::PathLine>> JsonReaderAE::Load(const std::string& fileName)
{
    TypesCollectionAE globalTypes;
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(globalTypes, fileName);

    jsonxx::Object collisionsObject = ReadObject(mapJsonObject, "collisions");
    jsonxx::Array collisionsArray = ReadArray(collisionsObject, "items");
    std::vector<::PathLine> lines = ReadAELines(globalTypes, collisionsArray);

    return { mapData, lines };
}

JsonWriterAO::JsonWriterAO(std::unique_ptr<TypesCollectionAO>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(*typesCollection, pathId, pathBndName, info), mTypesCollection{std::move(typesCollection)}
{
    mMapRootInfo.mGame = "AO";
}

JsonWriterAO::JsonWriterAO(s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterAO{std::make_unique<TypesCollectionAO>(), pathId, pathBndName, info}
{
}

JsonWriterAO::~JsonWriterAO() = default;

template<typename T>
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

void JsonWriterAO::DebugDumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource)
{
    u8* pStart = pathResource.data() + info.mObjectOffset;
    u8* pEnd = pathResource.data() + info.mIndexTableOffset;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(pStart);
    pPathTLV->RangeCheck();
    s32 idx = 0;
    if (static_cast<s32>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            idx++;
            DebugDumpTlv(prefix, idx, *pPathTLV);
            pPathTLV = AO::Path_TLV::Next_NoCheck(pPathTLV);
            if (pPathTLV)
            {
                pPathTLV->RangeCheck();
                if (pPathTLV->field_2_length == 0)
                {
                    // Dont get stuck in a loop
                    break;
                }

                if (reinterpret_cast<u8*>(pPathTLV) == pEnd)
                {
                    break;
                }
            }
        }
    }
}

void JsonWriterAE::DebugDumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource)
{
    u8* pData = pathResource.data();
    u8* pStart = pData + info.mObjectOffset;
    u8* pEnd = pData + info.mIndexTableOffset;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(pStart);
    s32 idx = 0;
    while (pPathTLV && reinterpret_cast<u8*>(pPathTLV) < pEnd)
    {
        idx++;
        DebugDumpTlv(prefix, idx, *pPathTLV);

        // Skip length bytes to get to the start of the next TLV
        u8* ptr = reinterpret_cast<u8*>(pPathTLV);
        u8* pNext = ptr + pPathTLV->field_2_length;
        pPathTLV = reinterpret_cast<Path_TLV*>(pNext);
    }
}

[[nodiscard]] jsonxx::Array JsonWriterAO::ReadTlvStream(u8* ptr)
{
    jsonxx::Array mapObjects;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
    pPathTLV->RangeCheck();

    if (static_cast<s32>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            mTypeCounterMap[pPathTLV->field_4_type.mType]++;

            auto obj = mTypesCollection->MakeTlvAO(pPathTLV->field_4_type.mType, pPathTLV, mTypeCounterMap[pPathTLV->field_4_type.mType]);
            if (obj)
            {
                if (pPathTLV->field_2_length != obj->TlvLen())
                {
                    LOG_ERROR(magic_enum::enum_name(pPathTLV->field_4_type.mType) << " size should be " << pPathTLV->field_2_length << " but got " << obj->TlvLen());
                    throw ReliveAPI::WrongTLVLengthException();
                }
                mapObjects << obj->InstanceToJson(*mTypesCollection);
            }
            else
            {
                LOG_WARNING("Ignoring type: " << magic_enum::enum_name(pPathTLV->field_4_type.mType));
            }

            pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
            if (pPathTLV)
            {
                pPathTLV->RangeCheck();
            }
        }
    }

    return mapObjects;
}


[[nodiscard]] jsonxx::Array JsonWriterAO::AddCollisionLineStructureJson()
{
    AOLine tmpLine(*mTypesCollection);
    return tmpLine.PropertiesToJson();
}

void JsonWriterAO::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

[[nodiscard]] jsonxx::Array JsonWriterAO::ReadCollisionStream(u8* ptr, s32 numItems)
{
    jsonxx::Array collisionsArray;
    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(ptr);
    TypesCollectionAO types;

    for (s32 i = 0; i < numItems; i++)
    {
        AOLine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties);

        collisionsArray << properties;
    }
    return collisionsArray;
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
                tmpCamera.mId =
                    1 * (pCamName->name[7] - '0') +
                    10 * (pCamName->name[6] - '0') +
                    100 * (pCamName->name[4] - '0') +
                    1000 * (pCamName->name[3] - '0');
            }

            const s32 indexTableEntryOffset = indexTable[To1dIndex(info.mWidth, x, y)];
            if (indexTableEntryOffset == -1 || indexTableEntryOffset >= 0x100000)
            {
                if (pCamName->name[0])
                {
                    LOG_INFO("Add camera with no objects " << tmpCamera.mName);
                    cameraArray << tmpCamera.ToJsonObject({});
                }
            }
            else
            {
                // Can have objects that do not live in a camera, as strange as it seems (R1P15)
                // "blank" cameras just do not have a name set.

                u8* ptr = pPathData + indexTableEntryOffset + info.mObjectOffset;
                jsonxx::Array mapObjects = ReadTlvStream(ptr);
                LOG_INFO("Add camera " << tmpCamera.mName);
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

JsonWriterAE::JsonWriterAE(std::unique_ptr<TypesCollectionAE>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(*typesCollection, pathId, pathBndName, info), mTypesCollection{std::move(typesCollection)}
{
    mMapRootInfo.mGame = "AE";
}

JsonWriterAE::JsonWriterAE(s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterAE{std::make_unique<TypesCollectionAE>(), pathId, pathBndName, info}
{
}

JsonWriterAE::~JsonWriterAE() = default;

void JsonWriterAE::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

[[nodiscard]] jsonxx::Array JsonWriterAE::ReadCollisionStream(u8* ptr, s32 numItems)
{
    jsonxx::Array collisionsArray;
    PathLine* pLineIter = reinterpret_cast<PathLine*>(ptr);
    TypesCollectionAE types;

    for (s32 i = 0; i < numItems; i++)
    {
        AELine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties);

        collisionsArray << properties;
    }
    return collisionsArray;
}

[[nodiscard]] jsonxx::Array JsonWriterAE::ReadTlvStream(u8* ptr)
{
    jsonxx::Array mapObjects;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        mTypeCounterMap[pPathTLV->field_4_type.mType]++;
        auto obj = mTypesCollection->MakeTlvAE(pPathTLV->field_4_type.mType, pPathTLV, mTypeCounterMap[pPathTLV->field_4_type.mType]);
        if (obj)
        {
            if (pPathTLV->field_2_length != obj->TlvLen())
            {
                LOG_ERROR(magic_enum::enum_name(pPathTLV->field_4_type.mType) << " size should be " << pPathTLV->field_2_length << " but got " << obj->TlvLen());
                throw ReliveAPI::WrongTLVLengthException();
            }

            mapObjects << obj->InstanceToJson(*mTypesCollection);
        }
        else
        {
            LOG_WARNING("Ignoring type: " << pPathTLV->field_4_type.mType);
        }

        pPathTLV = Path::Next_TLV_4DB6A0(pPathTLV); // TODO: Will skip the last entry ??
    }

    return mapObjects;
}

[[nodiscard]] jsonxx::Array JsonWriterAE::AddCollisionLineStructureJson()
{
    AELine tmpLine(*mTypesCollection);
    return tmpLine.PropertiesToJson();
}

void JsonMapRootInfoReader::Read(const std::string& fileName)
{
    std::ifstream inputFileStream(fileName.c_str());
    if (!inputFileStream.is_open())
    {
        throw ReliveAPI::IOReadException(fileName.c_str());
    }

    std::string jsonStr((std::istreambuf_iterator<s8>(inputFileStream)), std::istreambuf_iterator<s8>());

    jsonxx::Object rootObj;
    if (!rootObj.parse(jsonStr))
    {
        throw ReliveAPI::InvalidJsonException();
    }

    mMapRootInfo.mVersion = ReadNumber(rootObj, "api_version");
    mMapRootInfo.mGame = ReadString(rootObj, "game");

    if (mMapRootInfo.mGame == "AO")
    {
        return;
    }

    if (mMapRootInfo.mGame == "AE")
    {
        return;
    }

    /*
    if (mMapRootInfo.mVersion != AliveAPI::GetApiVersion())
    {
        // TODO: Upgrade
    }*/


    throw ReliveAPI::InvalidGameException(mMapRootInfo.mGame.c_str());
}

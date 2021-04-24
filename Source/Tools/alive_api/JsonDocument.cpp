#include "../AliveLibCommon/stdafx_common.h"
#include "JsonDocument.hpp"
#include "alive_api.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Collisions.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAE/Collisions.hpp"
#include "AOTlvs.hpp"
#include <fstream>
#include <streambuf>
#include <magic_enum/include/magic_enum.hpp>

class AOLine : public PropertyCollection
{
public:
    AOLine(TypesCollection& globalTypes, const AO::PathLine* line = nullptr)
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

class AELine : public PropertyCollection
{
public:
    AELine(TypesCollection& globalTypes, const PathLine* line = nullptr)
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


std::vector<AO::PathLine> JsonReaderBase::ReadAOLines(TypesCollection& types, jsonxx::Array& collisionsArray)
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

std::vector<::PathLine> JsonReaderBase::ReadAELines(TypesCollection& types, jsonxx::Array& collisionsArray)
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

std::pair<std::vector<CameraNameAndTlvBlob>,jsonxx::Object> JsonReaderBase::Load(Game gameType, TypesCollection& types, const std::string& fileName)
{
    std::ifstream inputFileStream(fileName.c_str());
    std::string jsonStr((std::istreambuf_iterator<char>(inputFileStream)), std::istreambuf_iterator<char>());

    jsonxx::Object rootObj;
    if (!rootObj.parse(jsonStr))
    {
        abort();
    }

    if (!rootObj.has<jsonxx::Object>("map"))
    {
        abort();
    }

    jsonxx::Object map = rootObj.get<jsonxx::Object>("map");

    if (!map.has<jsonxx::String>("path_bnd"))
    {
        abort();
    }
    mRootInfo.mPathBnd = map.get<jsonxx::String>("path_bnd");

    if (!map.has<jsonxx::Number>("path_id"))
    {
        abort();
    }

    mRootInfo.mPathId = static_cast<int>(map.get<jsonxx::Number>("path_id"));

    mRootInfo.mXSize = static_cast<int>(map.get<jsonxx::Number>("x_size"));
    mRootInfo.mYSize = static_cast<int>(map.get<jsonxx::Number>("y_size"));

    mRootInfo.mXGridSize = static_cast<int>(map.get<jsonxx::Number>("x_grid_size"));
    mRootInfo.mYGridSize = static_cast<int>(map.get<jsonxx::Number>("y_grid_size"));

    if (!map.has<jsonxx::Array>("cameras"))
    {
        abort();
    }

    std::vector<CameraNameAndTlvBlob> mapData;

    jsonxx::Array camerasArray = map.get<jsonxx::Array>("cameras");
    for (auto i = 0u; i < camerasArray.values().size(); i++)
    {
        jsonxx::Object camera = camerasArray.get<jsonxx::Object>(i);
        if (!camera.has<jsonxx::Array>("map_objects"))
        {
            abort();
        }

        const int x = static_cast<int>(camera.get<jsonxx::Number>("x"));
        const int y = static_cast<int>(camera.get<jsonxx::Number>("y"));
        if (x > mRootInfo.mXSize || y > mRootInfo.mYSize)
        {
            abort();
        }

        CameraNameAndTlvBlob cameraNameBlob;
        cameraNameBlob.mId = static_cast<int>(camera.get<jsonxx::Number>("id"));
        cameraNameBlob.mName = camera.get<jsonxx::String>("name");
        cameraNameBlob.x = x;
        cameraNameBlob.y = y;

        jsonxx::Array mapObjectsArray = camera.get<jsonxx::Array>("map_objects");
        for (auto j = 0u; j < mapObjectsArray.values().size(); j++)
        {
            jsonxx::Object mapObject = mapObjectsArray.get<jsonxx::Object>(j);
            if (!mapObject.has<jsonxx::String>("object_structures_type"))
            {
                abort();
            }
            std::string structureType = mapObject.get<jsonxx::String>("object_structures_type");
            std::unique_ptr<TlvObjectBase> tlv = gameType == Game::AO ? types.MakeTlvAO(structureType, nullptr) : types.MakeTlvAE(structureType, nullptr);
            if (!tlv)
            {
                abort();
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
    TypesCollection globalTypes(Game::AO);
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(Game::AO, globalTypes, fileName);
  
    if (!mapJsonObject.has<jsonxx::Object>("collisions"))
    {
        abort();
    }
    
    jsonxx::Object collisionsObject = mapJsonObject.get<jsonxx::Object>("collisions");
    jsonxx::Array collisionsArray = collisionsObject.get<jsonxx::Array>("items");
    std::vector<AO::PathLine> lines = ReadAOLines(globalTypes, collisionsArray);

    return { mapData, lines };
}

std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<::PathLine>> JsonReaderAE::Load(const std::string& fileName)
{
    TypesCollection globalTypes(Game::AE);
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(Game::AE, globalTypes, fileName);

    if (!mapJsonObject.has<jsonxx::Object>("collisions"))
    {
        abort();
    }
    jsonxx::Object collisionsObject = mapJsonObject.get<jsonxx::Object>("collisions");
    jsonxx::Array collisionsArray = collisionsObject.get<jsonxx::Array>("items");
    std::vector<::PathLine> lines = ReadAELines(globalTypes, collisionsArray);

    return { mapData, lines };
}

JsonWriterAO::JsonWriterAO(int pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(pathId, pathBndName, info)
{
    mMapRootInfo.mGame = "AO";
}

template<typename T>
static void DumpTlv(const std::string& prefix, int idx, const T& tlv)
{
    const std::string fileName = prefix + "_" + std::to_string(static_cast<int>(tlv.field_4_type.mType)) + "_" + std::to_string(idx) + ".dat";
    FILE* hFile = ::fopen(fileName.c_str(), "wb");
    if (!hFile)
    {
        abort();
    }
    ::fwrite(&tlv, tlv.field_2_length, 1, hFile);
    ::fclose(hFile);
}

void JsonWriterAO::DumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<BYTE>& pathResource)
{
    BYTE* pStart = pathResource.data() + info.mObjectOffset;
    BYTE* pEnd = pathResource.data() + info.mIndexTableOffset;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(pStart);
    pPathTLV->RangeCheck();
    int idx = 0;
    if (static_cast<int>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            idx++;
            DumpTlv(prefix, idx, *pPathTLV);
            pPathTLV = AO::Path_TLV::Next_NoCheck(pPathTLV);
            if (pPathTLV)
            {
                pPathTLV->RangeCheck();
                if (pPathTLV->field_2_length == 0)
                {
                    // Dont get stuck in a loop
                    break;
                }

                if (reinterpret_cast<BYTE*>(pPathTLV) == pEnd)
                {
                    break;
                }
            }
        }
    }
}

void JsonWriterAE::DumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<BYTE>& pathResource)
{
    BYTE* pData = pathResource.data();
    BYTE* pStart = pData + info.mObjectOffset;
    BYTE* pEnd = pData + info.mIndexTableOffset;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(pStart);
    int idx = 0;
    while (pPathTLV && reinterpret_cast<BYTE*>(pPathTLV) < pEnd)
    {
        idx++;
        DumpTlv(prefix, idx, *pPathTLV);

        // Skip length bytes to get to the start of the next TLV
        BYTE* ptr = reinterpret_cast<BYTE*>(pPathTLV);
        BYTE* pNext = ptr + pPathTLV->field_2_length;
        pPathTLV = reinterpret_cast<Path_TLV*>(pNext);
    }
}

jsonxx::Array JsonWriterAO::ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr)
{
    jsonxx::Array mapObjects;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
    pPathTLV->RangeCheck();

    if (static_cast<int>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            mTypeCounterMap[pPathTLV->field_4_type.mType]++;

            auto obj = globalTypes.MakeTlvAO(pPathTLV->field_4_type.mType, pPathTLV, mTypeCounterMap[pPathTLV->field_4_type.mType]);
            if (obj)
            {
                if (pPathTLV->field_2_length != obj->TlvLen())
                {
                    LOG_ERROR(magic_enum::enum_name(pPathTLV->field_4_type.mType) << " size should be " << pPathTLV->field_2_length << " but got " << obj->TlvLen());
                    abort();
                }
                mapObjects << obj->InstanceToJson(globalTypes);
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

std::unique_ptr<TypesCollection> JsonWriterAO::MakeTypesCollection() const
{
    return std::make_unique<TypesCollection>(Game::AO);
}

void JsonWriterAO::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

jsonxx::Array JsonWriterAO::ReadCollisionStream(BYTE* ptr, int numItems)
{
    jsonxx::Array collisionsArray;
    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(ptr);
    TypesCollection types(Game::AO);

    for (int i = 0; i < numItems; i++)
    {
        AOLine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties);

        collisionsArray << properties;
    }
    return collisionsArray;
}


JsonWriterBase::JsonWriterBase(int pathId, const std::string& pathBndName, const PathInfo& info)
{
    mMapInfo.mPathId = pathId;
    mMapInfo.mPathBnd = pathBndName;
    mMapInfo.mXGridSize = info.mGridWidth;
    mMapInfo.mYGridSize = info.mGridHeight;

    mMapInfo.mXSize = info.mWidth;
    mMapInfo.mYSize = info.mHeight;

    mMapRootInfo.mVersion = AliveAPI::GetApiVersion();
}

void JsonWriterBase::Save(Game gameType, const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName)
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

    BYTE* pPathData = pathResource.data();


    BYTE* pLineIter = pPathData + info.mCollisionOffset;
    jsonxx::Array collisionsArray = ReadCollisionStream(pLineIter, info.mNumCollisionItems);
    jsonxx::Object colllisionObject;

    std::unique_ptr<TypesCollection> globalTypes = MakeTypesCollection();
    if (gameType == Game::AO)
    {
        AOLine tmpLine(*globalTypes);
        colllisionObject << "structure" << tmpLine.PropertiesToJson();
    }
    else
    {
        AELine tmpLine(*globalTypes);
        colllisionObject << "structure" << tmpLine.PropertiesToJson();
    }

    colllisionObject << "items" << collisionsArray;
    rootMapObject << "collisions" << colllisionObject;

    const int* indexTable = reinterpret_cast<const int*>(pPathData + info.mIndexTableOffset);


    jsonxx::Array cameraArray;
    for (int y = 0; y < info.mHeight; y++)
    {
        for (int x = 0; x < info.mWidth; x++)
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

            const int indexTableEntryOffset = indexTable[To1dIndex(info.mWidth, x, y)];
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

                BYTE* ptr = pPathData + indexTableEntryOffset + info.mObjectOffset;
                jsonxx::Array mapObjects = ReadTlvStream(*globalTypes, ptr);
                LOG_INFO("Add camera " << tmpCamera.mName);
                cameraArray << tmpCamera.ToJsonObject(mapObjects);
            }
        }
    }

    rootMapObject << "cameras" << cameraArray;

    jsonxx::Object schemaObject;
    schemaObject << "object_structure_property_basic_types" << globalTypes->BasicTypesToJson();

    schemaObject << "object_structure_property_enums" << globalTypes->EnumsToJson();

    jsonxx::Array objectStructuresArray;
    globalTypes->AddTlvsToJsonArray(objectStructuresArray);
    schemaObject << "object_structures" << objectStructuresArray;

    // TODO: Collision structure(s)

    rootObject << "map" << rootMapObject;
    rootObject << "schema" << schemaObject;

    std::ofstream s(fileName.c_str());
    if (s)
    {
        s << rootObject.json();
    }
}

JsonWriterAE::JsonWriterAE(int pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(pathId, pathBndName, info)
{
    mMapRootInfo.mGame = "AE";
}

void JsonWriterAE::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

jsonxx::Array JsonWriterAE::ReadCollisionStream(BYTE* ptr, int numItems)
{
    jsonxx::Array collisionsArray;
    PathLine* pLineIter = reinterpret_cast<PathLine*>(ptr);
    TypesCollection types(Game::AE);

    for (int i = 0; i < numItems; i++)
    {
        AELine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties);

        collisionsArray << properties;
    }
    return collisionsArray;
}

jsonxx::Array JsonWriterAE::ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr)
{
    jsonxx::Array mapObjects;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        mTypeCounterMap[pPathTLV->field_4_type.mType]++;
        auto obj = globalTypes.MakeTlvAE(pPathTLV->field_4_type.mType, pPathTLV, mTypeCounterMap[pPathTLV->field_4_type.mType]);
        if (obj)
        {
            if (pPathTLV->field_2_length != obj->TlvLen())
            {
                LOG_ERROR(magic_enum::enum_name(pPathTLV->field_4_type.mType) << " size should be " << pPathTLV->field_2_length << " but got " << obj->TlvLen());
                abort();
            }

            mapObjects << obj->InstanceToJson(globalTypes);
        }
        else
        {
            LOG_WARNING("Ignoring type: " << pPathTLV->field_4_type.mType);
        }

        pPathTLV = Path::Next_TLV_4DB6A0(pPathTLV); // TODO: Will skip the last entry ?? 
    }

    return mapObjects;
}

std::unique_ptr<TypesCollection> JsonWriterAE::MakeTypesCollection() const
{
    return std::make_unique<TypesCollection>(Game::AE);
}

bool JsonMapRootInfoReader::Read(const std::string& fileName)
{
    std::ifstream inputFileStream(fileName.c_str());
    if (!inputFileStream.good())
    {
        abort();
    }

    std::string jsonStr((std::istreambuf_iterator<char>(inputFileStream)), std::istreambuf_iterator<char>());

    jsonxx::Object rootObj;
    if (!rootObj.parse(jsonStr))
    {
        abort();
    }

    if (!rootObj.has<jsonxx::Number>("api_version"))
    {
        abort();
    }
    mMapRootInfo.mVersion = static_cast<int>(rootObj.get<jsonxx::Number>("api_version"));

    if (!rootObj.has<jsonxx::String>("game"))
    {
        abort();
    }
    mMapRootInfo.mGame = rootObj.get<jsonxx::String>("game");

    if (mMapRootInfo.mGame == "AO")
    {
        return true;
    }

    if (mMapRootInfo.mGame == "AE")
    {
        return true;
    }

    /*
    if (mMapRootInfo.mVersion != AliveAPI::GetApiVersion())
    {
        // TODO: Upgrade
        abort();
    }*/


    abort();
}

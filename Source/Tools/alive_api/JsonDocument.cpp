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


static jsonxx::Object AOLineToJsonObject(const AO::PathLine& line)
{
    jsonxx::Object obj;

    obj << "x1" << static_cast<int>(line.field_0_rect.x);
    obj << "y1" << static_cast<int>(line.field_0_rect.y);

    obj << "x2" << static_cast<int>(line.field_0_rect.w);
    obj << "y2" << static_cast<int>(line.field_0_rect.h);

    obj << "type" << static_cast<int>(line.field_8_type);

    obj << "next" << line.field_10_next;
    obj << "previous" << line.field_C_previous;

    return obj;
}

static AO::PathLine JsonObjectToAOLine(jsonxx::Object& collision)
{
    AO::PathLine col = {};
    col.field_0_rect.x = collision.get<jsonxx::Number>("x1");
    col.field_0_rect.y = collision.get<jsonxx::Number>("y1");

    col.field_0_rect.w = collision.get<jsonxx::Number>("x2");
    col.field_0_rect.h = collision.get<jsonxx::Number>("y2");

    col.field_8_type = collision.get<jsonxx::Number>("type");
    col.field_10_next = collision.get<jsonxx::Number>("next");
    col.field_C_previous = collision.get<jsonxx::Number>("previous");

    return col;
}

static jsonxx::Object AELineToJsonObject(const PathLine& line)
{
    jsonxx::Object obj;

    obj << "x1" << static_cast<int>(line.field_0_rect.x);
    obj << "y1" << static_cast<int>(line.field_0_rect.y);

    obj << "x2" << static_cast<int>(line.field_0_rect.w);
    obj << "y2" << static_cast<int>(line.field_0_rect.h);

    obj << "type" << static_cast<int>(line.field_8_type);

    obj << "previous" << static_cast<int>(line.field_A_previous);
    obj << "next" << static_cast<int>(line.field_C_next);

    obj << "previous2" << static_cast<int>(line.field_E_previous2);
    obj << "next2" << static_cast<int>(line.field_10_next2);

    obj << "length" << static_cast<int>(line.field_12_line_length);

    return obj;
}

static ::PathLine JsonObjectToAELine(jsonxx::Object& collision)
{
    ::PathLine col = {};
    col.field_0_rect.x = collision.get<jsonxx::Number>("x1");
    col.field_0_rect.y = collision.get<jsonxx::Number>("y1");

    col.field_0_rect.w = collision.get<jsonxx::Number>("x2");
    col.field_0_rect.h = collision.get<jsonxx::Number>("y2");

    col.field_8_type = collision.get<jsonxx::Number>("type");

    col.field_A_previous = collision.get<jsonxx::Number>("previous");
    col.field_C_next = collision.get<jsonxx::Number>("next");
    
    col.field_E_previous2 = collision.get<jsonxx::Number>("previous2");
    col.field_10_next2 = collision.get<jsonxx::Number>("next2");

    col.field_12_line_length = collision.get<jsonxx::Number>("length");

    return col;
}


std::vector<AO::PathLine> JsonReaderBase::ReadAOLines(jsonxx::Array& collisionsArray)
{
    std::vector<AO::PathLine> lines;
    for (int i = 0; i < collisionsArray.values().size(); i++)
    {
        jsonxx::Object collision = collisionsArray.get<jsonxx::Object>(i);
        lines.emplace_back(JsonObjectToAOLine(collision));
    }
    return lines;
}

std::vector<::PathLine> JsonReaderBase::ReadAELines(jsonxx::Array& collisionsArray)
{
    std::vector<::PathLine> lines;
    for (int i = 0; i < collisionsArray.values().size(); i++)
    {
        jsonxx::Object collision = collisionsArray.get<jsonxx::Object>(i);
        lines.emplace_back(JsonObjectToAELine(collision));
    }
    return lines;
}

std::pair<std::vector<CameraNameAndTlvBlob>,jsonxx::Object> JsonReaderBase::Load(Game gameType, const std::string& fileName)
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

    mRootInfo.mPathId = map.get<jsonxx::Number>("path_id");

    mRootInfo.mXSize = map.get<jsonxx::Number>("x_size");
    mRootInfo.mYSize = map.get<jsonxx::Number>("y_size");

    mRootInfo.mXGridSize = map.get<jsonxx::Number>("x_grid_size");
    mRootInfo.mYGridSize = map.get<jsonxx::Number>("y_grid_size");

    if (!map.has<jsonxx::Array>("cameras"))
    {
        abort();
    }

    TypesCollection globalTypes(gameType);
    std::vector<CameraNameAndTlvBlob> mapData(mRootInfo.mXSize * mRootInfo.mYSize);

    jsonxx::Array camerasArray = map.get<jsonxx::Array>("cameras");
    for (int i = 0; i < camerasArray.values().size(); i++)
    {
        jsonxx::Object camera = camerasArray.get<jsonxx::Object>(i);
        if (!camera.has<jsonxx::Array>("map_objects"))
        {
            abort();
        }

        const int x = camera.get<jsonxx::Number>("x");
        const int y = camera.get<jsonxx::Number>("y");
        if (x > mRootInfo.mXSize || y > mRootInfo.mYSize)
        {
            abort();
        }

        CameraNameAndTlvBlob& cameraNameBlob = mapData[To1dIndex(mRootInfo.mXSize, x, y)];
        cameraNameBlob.mId = camera.get<jsonxx::Number>("id");
        cameraNameBlob.mName = camera.get<jsonxx::String>("name");
        cameraNameBlob.x = x;
        cameraNameBlob.y = y;

        jsonxx::Array mapObjectsArray = camera.get<jsonxx::Array>("map_objects");
        for (int j = 0; j < mapObjectsArray.values().size(); j++)
        {
            jsonxx::Object mapObject = mapObjectsArray.get<jsonxx::Object>(j);
            if (!mapObject.has<jsonxx::String>("object_structures_type"))
            {
                abort();
            }
            std::string structureType = mapObject.get<jsonxx::String>("object_structures_type");
            std::unique_ptr<TlvObjectBase> tlv = gameType == Game::AO ? globalTypes.MakeTlvAO(structureType, nullptr) : globalTypes.MakeTlvAE(structureType, nullptr);
            if (!tlv)
            {
                abort();
            }

            tlv->InstanceFromJson(globalTypes, mapObject);
            cameraNameBlob.mTlvBlobs.emplace_back(tlv->GetTlvData(j == mapObjectsArray.values().size() - 1));
        }
    }
    return { mapData, map };
}

std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> JsonReaderAO::Load(const std::string& fileName)
{
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(Game::AO, fileName);
  
    if (!mapJsonObject.has<jsonxx::Array>("collisions"))
    {
        abort();
    }
    jsonxx::Array collisionsArray = mapJsonObject.get<jsonxx::Array>("collisions");
    std::vector<AO::PathLine> lines = ReadAOLines(collisionsArray);

    return { mapData, lines };
}

std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<::PathLine>> JsonReaderAE::Load(const std::string& fileName)
{
    auto [mapData, mapJsonObject] = JsonReaderBase::Load(Game::AE, fileName);

    if (!mapJsonObject.has<jsonxx::Array>("collisions"))
    {
        abort();
    }
    jsonxx::Array collisionsArray = mapJsonObject.get<jsonxx::Array>("collisions");
    std::vector<::PathLine> lines = ReadAELines(collisionsArray);

    return { mapData, lines };
}

JsonWriterAO::JsonWriterAO(int pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(pathId, pathBndName, info)
{
    mMapRootInfo.mGame = "AO";
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
            auto obj = globalTypes.MakeTlvAO(pPathTLV->field_4_type.mType, pPathTLV);
            if (obj)
            {
                mapObjects << obj->InstanceToJson(globalTypes);
            }
            else
            {
                switch (pPathTLV->field_4_type.mType)
                {
                default:
                    LOG_WARNING("Ignoring type: " << magic_enum::enum_name(pPathTLV->field_4_type.mType));
                    break;
                }
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

jsonxx::Array JsonWriterAO::ReadCollisionStream(BYTE* ptr, int numItems)
{
    jsonxx::Array collisionsArray;
    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(ptr);
    for (int i = 0; i < numItems; i++)
    {
        collisionsArray << AOLineToJsonObject(pLineIter[i]);
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

void JsonWriterBase::Save(const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName)
{
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
    rootMapObject << "collisions" << collisionsArray;

    const int* indexTable = reinterpret_cast<const int*>(pPathData + info.mIndexTableOffset);

    std::unique_ptr<TypesCollection> globalTypes = MakeTypesCollection();

    jsonxx::Array cameraArray;
    for (int x = 0; x < info.mWidth; x++)
    {
        for (int y = 0; y < info.mHeight; y++)
        {
            auto pCamName = reinterpret_cast<const AO::CameraName*>(&pPathData[To1dIndex(info.mWidth, x, y) * sizeof(AO::CameraName)]);
            CameraObject tmpCamera;
            if (pCamName->name[0])
            {
                tmpCamera.mName = std::string(pCamName->name, 8);
                tmpCamera.mX = x;
                tmpCamera.mY = y;
                tmpCamera.mId = 
                    1 * (pCamName->name[7] - '0') +
                    10 * (pCamName->name[6] - '0') +
                    100 * (pCamName->name[4] - '0') +
                    1000 * (pCamName->name[3] - '0');
            }

            const int indexTableOffset = indexTable[To1dIndex(info.mWidth, x, y)];
            if (indexTableOffset == -1 || indexTableOffset >= 0x100000)
            {
                if (pCamName->name[0])
                {
                    LOG_INFO("Add camera with no objects " << pCamName->name);
                    cameraArray << tmpCamera.ToJsonObject({});
                }
            }
            else
            {
                // Can have objects that do not live in a camera, as strange as it seems (R1P15)
                // "blank" cameras just do not have a name set.

                BYTE* ptr = pPathData + indexTableOffset + info.mObjectOffset;
                jsonxx::Array mapObjects = ReadTlvStream(*globalTypes, ptr);
                LOG_INFO("Add camera " << tmpCamera.mName);
                cameraArray << tmpCamera.ToJsonObject(mapObjects);
            }
        }
    }

    rootMapObject << "cameras" << cameraArray;

    rootMapObject << "object_structure_property_basic_types" << globalTypes->BasicTypesToJson();

    rootMapObject << "object_structure_property_enums" << globalTypes->EnumsToJson();

    jsonxx::Array objectStructuresArray;
    globalTypes->AddTlvsToJsonArray(objectStructuresArray);
    rootMapObject << "object_structures" << objectStructuresArray;

    rootObject << "map" << rootMapObject;

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

jsonxx::Array JsonWriterAE::ReadCollisionStream(BYTE* ptr, int numItems)
{
    jsonxx::Array collisionsArray;
    PathLine* pLineIter = reinterpret_cast<PathLine*>(ptr);
    for (int i = 0; i < numItems; i++)
    {
        collisionsArray << AELineToJsonObject(pLineIter[i]);
    }
    return collisionsArray;
}

jsonxx::Array JsonWriterAE::ReadTlvStream(TypesCollection& globalTypes, BYTE* ptr)
{
    jsonxx::Array mapObjects;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        auto obj = globalTypes.MakeTlvAE(pPathTLV->field_4_type.mType, pPathTLV);
        if (obj)
        {
            mapObjects << obj->InstanceToJson(globalTypes);
        }
        else
        {
            switch (pPathTLV->field_4_type.mType)
            {
            default:
                LOG_WARNING("Ignoring type: " << pPathTLV->field_4_type.mType);
                break;
            }
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

    mMapRootInfo.mVersion = rootObj.get<jsonxx::Number>("api_version");
    if (mMapRootInfo.mVersion != AliveAPI::GetApiVersion())
    {
        // TODO: Upgrade
        abort();
    }

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

    abort();
}

#include "../AliveLibCommon/stdafx_common.h"
#include "JsonDocument.hpp"
#include "alive_api.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Collisions.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "AOTlvs.hpp"
#include <fstream>
#include <streambuf>

std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> JsonReaderAO::Load(const std::string& fileName)
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

    mRootInfo.mVersion = rootObj.get<jsonxx::Number>("api_version");
    if (mRootInfo.mVersion != AliveAPI::GetApiVersion())
    {
        // TODO: Upgrade
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

    if (!map.has<jsonxx::Array>("collisions"))
    {
        abort();
    }

    std::vector<AO::PathLine> lines;
    jsonxx::Array collisionsArray = map.get<jsonxx::Array>("collisions");
    for (int i = 0; i < collisionsArray.values().size(); i++)
    {
        jsonxx::Object collision = collisionsArray.get<jsonxx::Object>(i);

        AO::PathLine col = {};
        col.field_0_rect.x = collision.get<jsonxx::Number>("x1");
        col.field_0_rect.y = collision.get<jsonxx::Number>("y1");
        
        col.field_0_rect.w = collision.get<jsonxx::Number>("x2");
        col.field_0_rect.h = collision.get<jsonxx::Number>("y2");

        col.field_8_type = collision.get<jsonxx::Number>("type");
        col.field_10_next = collision.get<jsonxx::Number>("next");
        col.field_C_previous = collision.get<jsonxx::Number>("previous");

        lines.emplace_back(col);
    }

    if (!map.has<jsonxx::Array>("cameras"))
    {
        abort();
    }
    
    TypesCollection globalTypes;
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
            auto tlv = globalTypes.MakeTlvAO(structureType, nullptr);
            if (!tlv)
            {
                abort();
            }

            tlv->InstanceFromJson(globalTypes, mapObject);
            cameraNameBlob.mTlvBlobs.emplace_back(tlv->GetTlvData(j == mapObjectsArray.values().size() - 1));
        }
    }
    return { mapData, lines };
}

JsonWriterAO::JsonWriterAO(const std::string& pathBndName, const PathInfo& info)
{
    mRootInfo.mPathBnd = pathBndName;
    mRootInfo.mXGridSize = info.mGridWidth;
    mRootInfo.mYGridSize = info.mGridHeight;

    mRootInfo.mXSize = info.mWidth;
    mRootInfo.mYSize = info.mHeight;

    mRootInfo.mGame = "AO";

    mRootInfo.mVersion = AliveAPI::GetApiVersion();
}

static jsonxx::Object ToJsonObject(const AO::PathLine& line)
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

void JsonWriterAO::SaveAO(int pathId, const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName)
{
    mRootInfo.mPathId = pathId;

    jsonxx::Object rootObject;

    rootObject << "api_version" << mRootInfo.mVersion;

    rootObject << "game" << mRootInfo.mGame;

    jsonxx::Object rootMapObject;
    rootMapObject << "path_bnd" << mRootInfo.mPathBnd;
    rootMapObject << "path_id" << mRootInfo.mPathId;

    rootMapObject << "x_grid_size" << mRootInfo.mXGridSize;
    rootMapObject << "x_size" << mRootInfo.mXSize;

    rootMapObject << "y_grid_size" << mRootInfo.mYGridSize;
    rootMapObject << "y_size" << mRootInfo.mYSize;

    BYTE* pPathData = pathResource.data();

    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(pPathData + info.mCollisionOffset);
    jsonxx::Array collisionsArray;
    for (int i = 0; i < info.mNumCollisionItems; i++)
    {
        collisionsArray << ToJsonObject(pLineIter[i]);
    }

    rootMapObject << "collisions" << collisionsArray;

    const int* indexTable = reinterpret_cast<const int*>(pPathData + info.mIndexTableOffset);

    TypesCollection globalTypes;
    std::set<AO::TlvTypes> usedTypes;

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
                continue;
            }

            // Can have objects that do not live in a camera, as strange as it seems (R1P15)
            // "blank" cameras just do not have a name set.

            jsonxx::Array mapObjects;

            BYTE* ptr = pPathData + indexTableOffset + info.mObjectOffset;
            AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
            pPathTLV->RangeCheck();
            if (pPathTLV->field_4_type <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
            {
                while(pPathTLV)
                {
                    usedTypes.insert(static_cast<AO::TlvTypes>(pPathTLV->field_4_type));

                    auto obj = globalTypes.MakeTlvAO(static_cast<AO::TlvTypes>(pPathTLV->field_4_type), pPathTLV);
                    if (obj)
                    {
                        mapObjects << obj->InstanceToJson(globalTypes);
                    }
                    else
                    {
                        switch (pPathTLV->field_4_type)
                        {
                        case 37:
                            LOG_WARNING("Unused abe start ignored");
                            break;
                        /*
                        case 2:
                            LOG_WARNING("Unused continue zone ignored");
                            break;
                        */

                        default:
                            LOG_WARNING("Ignoring type: " << pPathTLV->field_4_type);
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
            LOG_INFO("Add camera " << tmpCamera.mName);
            cameraArray << tmpCamera.ToJsonObject(mapObjects);
        }
    }

    rootMapObject << "cameras" << cameraArray;

    rootMapObject << "object_structure_property_basic_types" << globalTypes.BasicTypesToJson();

    rootMapObject << "object_structure_property_enums" << globalTypes.EnumsToJson();

    jsonxx::Array objectStructuresArray;
    globalTypes.AddTlvsToJsonArray(objectStructuresArray);
    rootMapObject << "object_structures" << objectStructuresArray;

    rootObject << "map" << rootMapObject;

    std::ofstream s(fileName.c_str());
    if (s)
    {
        s << rootObject.json();
    }
}

void JsonWriterAE::SaveAE(int /*pathId*/, const PathInfo& /*info*/, std::vector<BYTE>& /*pathResource*/, const std::string& /*fileName*/)
{

}

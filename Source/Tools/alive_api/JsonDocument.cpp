#include "../AliveLibCommon/stdafx_common.h"
#include "JsonDocument.hpp"
#include "alive_api.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Collisions.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "AOTlvs.hpp"

void JsonDocument::SetPathInfo(const std::string& pathBndName, const PathInfo& info)
{
    mPathBnd = pathBndName;
    mXGridSize = info.mGridWidth;
    mYGridSize = info.mGridHeight;

    mXSize = info.mWidth;
    mYSize = info.mHeight;

    mVersion = AliveAPI::GetApiVersion();

}

void JsonDocument::SaveAO(int pathId, const PathInfo& info, std::vector<BYTE>& pathResource, const std::string& fileName)
{
    mPathId = pathId;

    jsonxx::Object rootObject;

    rootObject << "api_version" << mVersion;

    rootObject << "game" << mGame;

    jsonxx::Object rootMapObject;
    rootMapObject << "path_bnd" << mPathBnd;
    rootMapObject << "path_id" << mPathId;

    rootMapObject << "x_grid_size" << mXGridSize;
    rootMapObject << "x_size" << mXSize;

    rootMapObject << "y_grid_size" << mYGridSize;
    rootMapObject << "y_size" << mYSize;

    BYTE* pPathData = pathResource.data() + sizeof(::ResourceManager::Header);


    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(pPathData + info.mCollisionOffset);
    for (int i = 0; i < info.mNumCollisionItems; i++)
    {
        CollisionObject tmpCol;
        tmpCol.mX1 = pLineIter[i].field_0_rect.x;
        tmpCol.mY1 = pLineIter[i].field_0_rect.y;
        tmpCol.mX2 = pLineIter[i].field_0_rect.w;
        tmpCol.mY2 = pLineIter[i].field_0_rect.h;


        mCollisions.push_back(tmpCol);
    }

    jsonxx::Array collisionsArray;
    for (const auto& item : mCollisions)
    {
        collisionsArray << item.ToJsonObject();
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
            auto pCamName = reinterpret_cast<const AO::CameraName*>(&pPathData[(x + (y * info.mWidth)) * sizeof(AO::CameraName)]);
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

            const int indexTableOffset = indexTable[(x + (y * info.mWidth))];
            if (indexTableOffset == -1 || indexTableOffset >= 0x100000)
            {
                if (pCamName->name[0])
                {
                    cameraArray << tmpCamera.ToJsonObject({});
                }
                continue;
            }

            if (!pCamName->name[0])
            {
                // Cant have objects that dont live in a camera
                abort();
            }

            jsonxx::Array mapObjects;

            BYTE* ptr = pPathData + indexTableOffset + info.mObjectOffset;
            AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
            pPathTLV->RangeCheck();
            if (pPathTLV->field_4_type <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
            {
                for (;;)
                {
                    // End of TLV list for current camera
                    if (pPathTLV->field_0_flags.Get(AO::TLV_Flags::eBit3_End_TLV_List))
                    {
                        break;
                    }

                    usedTypes.insert(static_cast<AO::TlvTypes>(pPathTLV->field_4_type));

                    auto obj = globalTypes.MakeTlv(static_cast<AO::TlvTypes>(pPathTLV->field_4_type), pPathTLV);
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

                        case 2:
                            LOG_WARNING("Unused continue zone ignored");
                            break;

                        default:
                            LOG_WARNING("Ignoring type: " << pPathTLV->field_4_type);
                            break;
                        }
                    }

                    pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
                    pPathTLV->RangeCheck();

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

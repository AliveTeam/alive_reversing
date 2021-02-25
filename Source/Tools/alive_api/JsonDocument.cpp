#include "../AliveLibCommon/stdafx_common.h"
#include "JsonDocument.hpp"
#include "alive_api.hpp"
#include "../AliveLibAO/PathData.hpp"
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

    jsonxx::Array collisionsArray;
    for (const auto& item : mCollisions)
    {
        collisionsArray << item.ToJsonObject();
    }
    rootMapObject << "collisions" << collisionsArray;

    jsonxx::Array cameraArray;
    for (const auto& camera : mCameras)
    {
        cameraArray << camera.ToJsonObject();
    }
    rootMapObject << "cameras" << cameraArray;

    BYTE* pPathData = pathResource.data() + sizeof(::ResourceManager::Header);

    const int* indexTable = reinterpret_cast<const int*>(pPathData + info.mIndexTableOffset);

    TypesCollection globalTypes;
    std::set<AO::TlvTypes> usedTypes;

    for (int x = 0; x < info.mWidth; x++)
    {
        for (int y = 0; y < info.mHeight; y++)
        {
            const int objectTableIdx = indexTable[(x + (y * info.mWidth))];
            if (objectTableIdx == -1 || objectTableIdx >= 0x100000)
            {
                continue;
            }

            BYTE* ptr = pPathData + objectTableIdx + info.mObjectOffset;
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
                    switch (pPathTLV->field_4_type)
                    {
                    case AO::TlvTypes::ContinuePoint_0:
                    {
                        AOTlvs::Path_ContinuePoint obj(globalTypes, pPathTLV);
                        obj.InstanceToJson(globalTypes);
                    }
                    break;

                    case AO::TlvTypes::Hoist_3:
                    {
                        AOTlvs::Path_Hoist obj(globalTypes, pPathTLV);
                        obj.InstanceToJson(globalTypes);
                    }
                    break;

                    case AO::TlvTypes::Door_6:
                    {
                        AOTlvs::Path_Door obj(globalTypes, pPathTLV);
                        obj.InstanceToJson(globalTypes);
                    }
                    break;

                    default:
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
                        break;
                    }

                    pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
                    pPathTLV->RangeCheck();
                }
            }
        }
    }


    /*
    jsonxx::Object objectStructurePropertiesObject;
    for (const auto& propertyEnum : mStructureAndTypes.mEnums)
    {
        propertyEnum.AddJsonArray(objectStructurePropertiesObject);
    }
    rootMapObject << "object_structure_property_enums" << objectStructurePropertiesObject;

    jsonxx::Array objectStructureBasicTypes;
    for (const auto& basicType : mStructureAndTypes.mBasicTypes)
    {
        basicType.AddToJsonArray(objectStructureBasicTypes);
    }
    rootMapObject << "object_structure_property_basic_types" << objectStructureBasicTypes;

    jsonxx::Array objectStructures;
    for (const auto& objectStructure : mStructureAndTypes.mObjectStructures)
    {
        objectStructure.AddToJsonArray(objectStructures);
    }
    rootMapObject << "object_structures" << objectStructures;
    */

    rootObject << "map" << rootMapObject;

    std::ofstream s(fileName.c_str());
    if (s)
    {
        s << rootObject.json();
    }
}

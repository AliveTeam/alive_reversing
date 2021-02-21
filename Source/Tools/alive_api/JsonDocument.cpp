#include "../AliveLibCommon/stdafx_common.h"
#include "JsonDocument.hpp"

void JsonDocument::SaveAO(int pathId, const std::vector<BYTE>& pathResource, const PathInfo& info, const std::string& fileName) const
{
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

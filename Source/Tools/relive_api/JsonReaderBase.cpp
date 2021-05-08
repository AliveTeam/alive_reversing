#include "JsonReaderBase.hpp"
#include "JsonMapRootInfoReader.hpp"
#include "JsonReadUtils.hpp"
#include "TlvObjectBase.hpp"

std::vector<AO::PathLine> JsonReaderBase::ReadAOLines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray)
{
    std::vector<AO::PathLine> lines;
    for (auto i = 0u; i < collisionsArray.values().size(); i++)
    {
        const jsonxx::Object& collision = collisionsArray.get<jsonxx::Object>(i);
        AOLine tmpLine(types);
        tmpLine.PropertiesFromJson(types, collision);
        lines.emplace_back(tmpLine.mLine);
    }
    return lines;
}

std::vector<::PathLine> JsonReaderBase::ReadAELines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray)
{
    std::vector<::PathLine> lines;
    for (auto i = 0u; i < collisionsArray.values().size(); i++)
    {
        const jsonxx::Object& collision = collisionsArray.get<jsonxx::Object>(i);
        AELine tmpLine(types);
        tmpLine.PropertiesFromJson(types, collision);
        lines.emplace_back(tmpLine.mLine);
    }
    return lines;
}

std::pair<std::vector<CameraNameAndTlvBlob>, jsonxx::Object> JsonReaderBase::Load(TypesCollectionBase& types, const std::string& fileName)
{
    std::ifstream inputFileStream(fileName.c_str());
    if (!inputFileStream.is_open())
    {
        throw ReliveAPI::IOReadException();
    }

    std::string& jsonStr = getStaticStringBuffer();
    readFileContentsIntoString(jsonStr, inputFileStream);

    jsonxx::Object rootObj;
    if (!rootObj.parse(jsonStr))
    {
        throw ReliveAPI::InvalidJsonException();
    }

    const jsonxx::Object& map = ReadObject(rootObj, "map");
    mRootInfo.mPathBnd = ReadString(map, "path_bnd");

    mRootInfo.mPathId = ReadNumber(map, "path_id");

    mRootInfo.mXSize = ReadNumber(map, "x_size");
    mRootInfo.mYSize = ReadNumber(map, "y_size");

    mRootInfo.mXGridSize = ReadNumber(map, "x_grid_size");
    mRootInfo.mYGridSize = ReadNumber(map, "y_grid_size");

    std::vector<CameraNameAndTlvBlob> mapData;

    const jsonxx::Array& camerasArray = ReadArray(map, "cameras");
    for (auto i = 0u; i < camerasArray.values().size(); i++)
    {
        const jsonxx::Object& camera = camerasArray.get<jsonxx::Object>(i);

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

        const jsonxx::Array& mapObjectsArray = ReadArray(camera, "map_objects");
        for (auto j = 0u; j < mapObjectsArray.values().size(); j++)
        {
            const jsonxx::Object& mapObject = mapObjectsArray.get<jsonxx::Object>(j);
            const std::string& structureType = ReadString(mapObject, "object_structures_type");
            std::unique_ptr<TlvObjectBase> tlv = types.MakeTlvFromString(structureType);
            if (!tlv)
            {
                throw ReliveAPI::UnknownStructureTypeException(structureType.c_str());
            }

            tlv->InstanceFromJson(types, mapObject);
            cameraNameBlob.mTlvBlobs.emplace_back(tlv->GetTlvData(j == mapObjectsArray.values().size() - 1));
        }

        mapData.push_back(std::move(cameraNameBlob));
    }

    return { std::move(mapData), std::move(map) };
}

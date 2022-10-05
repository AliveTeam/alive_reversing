#include "JsonReaderBase.hpp"
#include "JsonMapRootInfoReader.hpp"
#include "JsonReadUtils.hpp"
#include "TlvObjectBase.hpp"
#include "file_api.hpp"
#include <set>

namespace ReliveAPI {
std::vector<AO::PathLine> JsonReaderBase::ReadAOLines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray, Context& context)
{
    std::vector<AO::PathLine> lines;
    for (auto i = 0u; i < collisionsArray.values().size(); i++)
    {
        const jsonxx::Object& collision = collisionsArray.get<jsonxx::Object>(i);
        AOLine tmpLine(types);
        tmpLine.PropertiesFromJson(types, collision, context);
        lines.emplace_back(tmpLine.mLine);
    }
    return lines;
}

std::vector<::PathLine> JsonReaderBase::ReadAELines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray, Context& context)
{
    std::vector<::PathLine> lines;
    for (auto i = 0u; i < collisionsArray.values().size(); i++)
    {
        const jsonxx::Object& collision = collisionsArray.get<jsonxx::Object>(i);
        AELine tmpLine(types);
        tmpLine.PropertiesFromJson(types, collision, context);
        lines.emplace_back(tmpLine.mLine);
    }
    return lines;
}

LoadedJsonBase JsonReaderBase::Load(TypesCollectionBase& types, IFileIO& fileIO, const std::string& fileName, Context& context)
{
    auto inputFileStream = fileIO.Open(fileName, IFileIO::Mode::Read);
    if (!inputFileStream->IsOpen())
    {
        throw ReliveAPI::IOReadException(fileName);
    }

    std::string& jsonStr = getStaticStringBuffer();
    readFileContentsIntoString(jsonStr, *inputFileStream);

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

    mRootInfo.mAbeStartXPos = ReadNumber(map, "abe_start_xpos");
    mRootInfo.mAbeStartYPos = ReadNumber(map, "abe_start_ypos");

    mRootInfo.mNumMudsInPath = ReadNumber(map, "num_muds_in_path");

    mRootInfo.mTotalMuds = ReadNumber(map, "total_muds");
    mRootInfo.mBadEndingMuds = ReadNumber(map, "num_muds_for_bad_ending");
    mRootInfo.mGoodEndingMuds = ReadNumber(map, "num_muds_for_good_ending");

    const jsonxx::Array& LCDScreenMessagesArray = ReadArray(map, "lcdscreen_messages");
    for (auto i = 0u; i < LCDScreenMessagesArray.values().size(); i++)
    {
        mRootInfo.mLCDScreenMessages.emplace_back(LCDScreenMessagesArray.get<std::string>(i));
    }

    const jsonxx::Array& hintFlyMessagesArray = ReadArray(map, "hintfly_messages");
    for (auto i = 0u; i < hintFlyMessagesArray.values().size(); i++)
    {
        mRootInfo.mHintFlyMessages.emplace_back(hintFlyMessagesArray.get<std::string>(i));
    }

    LoadedJsonBase ret;

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

        cameraNameBlob.mCameraAndLayers.mCameraImage = ReadOptionalString(camera, "image");
        cameraNameBlob.mCameraAndLayers.mForegroundLayer = ReadOptionalString(camera, "foreground_layer");
        cameraNameBlob.mCameraAndLayers.mForegroundWellLayer = ReadOptionalString(camera, "foreground_well_layer");
        cameraNameBlob.mCameraAndLayers.mBackgroundLayer = ReadOptionalString(camera, "background_layer");
        cameraNameBlob.mCameraAndLayers.mBackgroundWellLayer = ReadOptionalString(camera, "background_well_layer");

        const jsonxx::Array& mapObjectsArray = ReadArray(camera, "map_objects");
        for (auto j = 0u; j < mapObjectsArray.values().size(); j++)
        {
            const jsonxx::Object& mapObject = mapObjectsArray.get<jsonxx::Object>(j);
            const std::string& structureType = ReadString(mapObject, "object_structures_type");
            std::unique_ptr<TlvObjectBase> tlv = types.MakeTlvFromString(structureType);
            if (!tlv)
            {
                throw ReliveAPI::UnknownStructureTypeException(structureType);
            }

            for (const auto& resource : tlv->Resources())
            {
                if (resource.mAddMeTo == AddResourceTo::CameraBlock)
                {
                    cameraNameBlob.mRequiredResources.insert(resource.mResource);
                }
                else if (resource.mAddMeTo == AddResourceTo::File)
                {
                    ret.mResourcesRequiredInLvl.insert(resource.mResource);
                }
            }

            tlv->InstanceFromJson(types, mapObject, context);
            cameraNameBlob.mTlvBlobs.emplace_back(tlv->GetTlvData(j == mapObjectsArray.values().size() - 1));
        }

        ret.mPerCamData.push_back(std::move(cameraNameBlob));
    }

    ret.mMapJson = map;

    return ret;
}
} // namespace ReliveAPI

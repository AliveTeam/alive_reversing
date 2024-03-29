#include "JsonModelTypes.hpp"
#include <jsonxx/jsonxx.h>
#include "CamConverter.hpp"
#include "Base64.hpp"

namespace ReliveAPI {

[[nodiscard]] jsonxx::Object CameraObject::ToJsonObject(jsonxx::Array mapObjectsArray, const CameraImageAndLayers& cameraImageAndLayers) const
{
    jsonxx::Object obj;

    obj << "name" << mName;
    obj << "x" << mX;
    obj << "y" << mY;
    obj << "id" << mId;
    obj << "map_objects" << mapObjectsArray;

    if (!cameraImageAndLayers.mCameraImage.empty())
    {
        obj << "image" << cameraImageAndLayers.mCameraImage;
    }

    if (!cameraImageAndLayers.mForegroundLayer.empty())
    {
        obj << "foreground_layer" << cameraImageAndLayers.mForegroundLayer;
    }

    if (!cameraImageAndLayers.mForegroundWellLayer.empty())
    {
        obj << "foreground_well_layer" << cameraImageAndLayers.mForegroundWellLayer;
    }

    if (!cameraImageAndLayers.mBackgroundLayer.empty())
    {
        obj << "background_layer" << cameraImageAndLayers.mBackgroundLayer;
    }

    if (!cameraImageAndLayers.mBackgroundWellLayer.empty())
    {
        obj << "background_well_layer" << cameraImageAndLayers.mBackgroundWellLayer;
    }

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
} // namespace ReliveAPI

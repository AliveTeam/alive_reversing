#include "JsonModelTypes.hpp"
#include <nlohmann/json.hpp>
#include "CamConverter.hpp"
#include "Base64.hpp"

namespace ReliveAPI {

[[nodiscard]] nlohmann::json CameraObject::ToJsonObject(nlohmann::json mapObjectsArray, const CameraImageAndLayers& cameraImageAndLayers) const
{
    nlohmann::json obj = nlohmann::json::object();

    obj["name"] = mName;
    obj["x"] = mX;
    obj["y"] = mY;
    obj["id"] = mId;
    obj["map_objects"] = mapObjectsArray;

    if (!cameraImageAndLayers.mCameraImage.empty())
    {
        obj["image"] = cameraImageAndLayers.mCameraImage;
    }

    if (!cameraImageAndLayers.mForegroundLayer.empty())
    {
        obj["foreground_layer"] = cameraImageAndLayers.mForegroundLayer; 
    }

    if (!cameraImageAndLayers.mForegroundWellLayer.empty())
    {
        obj["foreground_well_layer"] = cameraImageAndLayers.mForegroundWellLayer;
    }

    if (!cameraImageAndLayers.mBackgroundLayer.empty())
    {
        obj["background_layer"] = cameraImageAndLayers.mBackgroundLayer;
    }

    if (!cameraImageAndLayers.mBackgroundWellLayer.empty())
    {
        obj["background_well_layer"] = cameraImageAndLayers.mBackgroundWellLayer;
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

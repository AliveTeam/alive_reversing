#include "JsonModelTypes.hpp"
#include <jsonxx/jsonxx.h>
#include "CamConverter.hpp"

namespace ReliveAPI {

static const unsigned char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64_encode(const u8* src, size_t len)
{
    unsigned char *out, *pos;
    const unsigned char *end, *in;

    size_t olen = 4 * ((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len)
    {
        return std::string(); /* integer overflow */
    }

    std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*) &outStr[0];

    end = src + len;
    in = src;
    pos = out;
    while (end - in >= 3)
    {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
    }

    if (end - in)
    {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1)
        {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else
        {
            *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return outStr;
}

static std::string ToBase64(const std::vector<u8>& vec)
{
    return base64_encode(vec.data(), vec.size());
}

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
        obj << "image" << ToBase64(cameraImageAndLayers.mCameraImage);
    }

    if (!cameraImageAndLayers.mForegroundLayer.empty())
    {
        obj << "foreground_layer" << ToBase64(cameraImageAndLayers.mForegroundLayer);
    }

    if (!cameraImageAndLayers.mForegroundWellLayer.empty())
    {
        obj << "foreground_well_layer" << ToBase64(cameraImageAndLayers.mForegroundWellLayer);
    }

    if (!cameraImageAndLayers.mBackgroundLayer.empty())
    {
        obj << "background_layer" << ToBase64(cameraImageAndLayers.mBackgroundLayer);
    }

    if (!cameraImageAndLayers.mBackgroundWellLayer.empty())
    {
        obj << "background_well_layer" << ToBase64(cameraImageAndLayers.mBackgroundWellLayer);
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

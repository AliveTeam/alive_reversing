#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <string>
#include <vector>

namespace ReliveAPI {
class ChunkedLvlFile;

class CameraImageAndLayers
{
public:
    std::vector<u8> mCameraImage;
    std::vector<u8> mForegroundLayer;
    std::vector<u8> mBackgroundLayer;
    std::vector<u8> mForegroundWellLayer;
    std::vector<u8> mBackgroundWellLayer;
};

class CamConverterAO final
{
public:
    CamConverterAO(const ChunkedLvlFile& camFile, CameraImageAndLayers& outData);
};

class CamConverterAE final
{
public:
    CamConverterAE(const ChunkedLvlFile& camFile, CameraImageAndLayers& outData);
};
} // namespace ReliveAPI

#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <string>
#include <vector>

namespace ReliveAPI {
class ChunkedLvlFile;
class CameraImageAndLayers;

class CamConverter final
{
public:
    CamConverter(const ChunkedLvlFile& camFile, CameraImageAndLayers& outData);

    static u32 CamBitsIdFromName(const std::string& pCamName);
};

std::string RGB565ToBase64PngString(const u16* pRgb565Buffer);

} // namespace ReliveAPI

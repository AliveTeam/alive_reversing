#pragma once

#include "../../relive_lib/Types.hpp"
#include <string>
#include <vector>
#include <memory>

namespace ReliveAPI {
class ChunkedLvlFile;
class CameraImageAndLayers;
class ApiFG1Reader;

class CamConverter final
{
public:
    std::pair<std::unique_ptr<ApiFG1Reader>, u32> Convert(const ChunkedLvlFile& camFile, const std::string& baseName);

    CamConverter() = default;
    CamConverter(const ChunkedLvlFile& camFile, CameraImageAndLayers& outData);

    static u32 CamBitsIdFromName(const std::string& pCamName);
};

std::string RGB565ToBase64PngString(const u16* pRgb565Buffer);
void RGB565ToPngBuffer(const u16* camBuffer, std::vector<u8>& outPngData);

} // namespace ReliveAPI

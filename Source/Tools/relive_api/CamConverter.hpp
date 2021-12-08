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
};

} // namespace ReliveAPI

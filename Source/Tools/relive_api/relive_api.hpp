#pragma once

#include "../../relive_lib/Types.hpp"
#include "ApiContext.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#define API_EXPORT

namespace ReliveAPI {

class LvlWriter;
class CameraImageAndLayers;
class IFileIO;

struct EnumeratePathsResult final
{
    std::string pathBndName;
    std::vector<s32> paths;
};

using TAliveFatalCb = void(*)(const char_type*);

API_EXPORT void SetAliveFatalCallBack(TAliveFatalCb callBack);
API_EXPORT [[nodiscard]] s32 GetApiVersion();

// TODO: Remove (data conversion already made them all json)
API_EXPORT void ExportPathBinaryToJson(IFileIO& fileIO, const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId, Context& context);

API_EXPORT [[nodiscard]] std::string UpgradePathJson(IFileIO& fileIO, const std::string& jsonFile);

// TODO: change to look at path jsons on disc
API_EXPORT [[nodiscard]] EnumeratePathsResult EnumeratePaths(IFileIO& fileIO, const std::string& inputLvlFile);

class ChunkedLvlFile;
namespace Detail {

// TODO: Remove (everything is json, no need to open binary paths)
API_EXPORT void ExportPathBinaryToJson(std::vector<u8>& fileDataBuffer, IFileIO& fileIO, const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId, Context& context);

// TODO: change to look at path jsons on disc
API_EXPORT [[nodiscard]] EnumeratePathsResult EnumeratePaths(std::vector<u8>& fileDataBuffer, IFileIO& fileIO, const std::string& inputLvlFile);

// TODO: Remove (everything is json, no need to open binary paths)
void ImportCameraAndFG1(std::vector<u8>& fileDataBuffer, LvlWriter& inputLvl, const std::string& camName, const CameraImageAndLayers& imageAndLayers, bool allowFullFG1Blocks);

// TODO: Remove (everything is json, no need to open binary paths)
[[nodiscard]] std::unique_ptr<ChunkedLvlFile> OpenPathBnd(IFileIO& fileIo, const std::string& inputLvlFile, std::vector<u8>& fileDataBuffer);
}

} // namespace ReliveAPI

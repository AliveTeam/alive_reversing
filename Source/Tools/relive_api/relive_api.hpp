#pragma once

#include "../AliveLibCommon/Types.hpp"
#include "relive_api_exceptions.hpp"

#include <string>
#include <vector>
#include <stdexcept>

#define API_EXPORT

namespace ReliveAPI {

struct EnumeratePathsResult final
{
    std::string pathBndName;
    std::vector<s32> paths;
};

void DebugDumpTlvs(const std::string& prefix, const std::string& lvlFile, s32 pathId);

API_EXPORT [[nodiscard]] s32 GetApiVersion();

API_EXPORT void ExportPathBinaryToJson(std::vector<u8>& fileDataBuffer, const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId);
API_EXPORT void ExportPathBinaryToJson(const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId);

API_EXPORT void UpgradePathJson(const std::string& jsonFile);

API_EXPORT void ImportPathJsonToBinary(std::vector<u8>& fileDataBuffer, const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& lvlResourceSources);
API_EXPORT void ImportPathJsonToBinary(const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& lvlResourceSources);

API_EXPORT [[nodiscard]] EnumeratePathsResult EnumeratePaths(std::vector<u8>& fileDataBuffer, const std::string& inputLvlFile);
API_EXPORT [[nodiscard]] EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile);

} // namespace ReliveAPI

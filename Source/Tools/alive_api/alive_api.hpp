#pragma once

#include <string>
#include <vector>

#define API_EXPORT

namespace AliveAPI
{
    enum class Error
    {
        None,
        LvlFileReadError,
        JsonFileWriteError,
        JsonFileNeedsUpgrading,
        JsonFileReadError,
        JsonNotValid,
        PathResourceNotFound,
        RequiredResourceNotFoundInAnyLvl
    };

    struct Result
    {
        Error mResult = Error::None;
        s32 errorCode = 0;
    };

    struct EnumeratePathsResult : public Result
    {
        std::string pathBndName;
        std::vector<s32> paths;
    };

    enum class UpgradeError
    {
        None,
    };

    struct JsonUpgradeResult
    {
         UpgradeError mResult = UpgradeError::None;
    };

    void DebugDumpTlvs(const std::string& prefix, const std::string& lvlFile, s32 pathId);

    API_EXPORT [[nodiscard]] s32 GetApiVersion();
    API_EXPORT [[nodiscard]] Result ExportPathBinaryToJson(const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId);
    API_EXPORT [[nodiscard]] JsonUpgradeResult UpgradePathJson(const std::string& jsonFile);
    API_EXPORT [[nodiscard]] Result ImportPathJsonToBinary(const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& lvlResourceSources);
    API_EXPORT [[nodiscard]] EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile);

    // TODO: Camera in/exporting
}

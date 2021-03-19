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
        int errorCode = 0;
    };

    struct EnumeratePathsResult : public Result
    {
        std::string pathBndName;
        std::vector<int> paths;
    };

    void DebugDumpTlvs(const std::string& prefix, const std::string& lvlFile, int pathId);

    API_EXPORT [[nodiscard]] int GetApiVersion();
    API_EXPORT [[nodiscard]] Result ExportPathBinaryToJson(const std::string& jsonOutputFile, const std::string& inputLvlFile, int pathResourceId);
    API_EXPORT [[nodiscard]] Result UpgradePathJson(const std::string& jsonFile);
    API_EXPORT [[nodiscard]] Result ImportPathJsonToBinary(const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& lvlResourceSources);
    API_EXPORT [[nodiscard]] EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile);

    // TODO: Camera in/exporting
}

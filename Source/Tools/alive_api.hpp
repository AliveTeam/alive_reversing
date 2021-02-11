#pragma once

#include <string>
#include <vector>

#define API_EXPORT

namespace AliveAPI
{
    enum class Error
    {
        None,
        LvlFileReadEror,
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
        int numPaths = 0;
    };

    API_EXPORT int GetApiVersion();
    API_EXPORT Result ExportPathBinaryToJson(const std::string& jsonOutputFile, const std::string& outputLvlFile, int pathResourceId);
    API_EXPORT Result UpgradePathJson(const std::string& jsonFile);
    API_EXPORT Result ImportPathJsonToBinary(const std::string& jsonInputFile, const std::string& inputLvlFile, int pathResourceId, const std::vector<std::string>& lvlResourceSources);
    API_EXPORT EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile);
}

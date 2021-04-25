#pragma once

#include <string>
#include <vector>

#define API_EXPORT

namespace AliveAPI
{
    class Exception
    {
    public:
        virtual ~Exception() { }
        Exception() = default;
        explicit Exception(const std::string& what) : mWhat(what) { }
        const std::string& what() const
        {
            return mWhat;
        }
    protected:
        std::string mWhat;
    };

    // Error opening json file on disk
    class IOReadException final : public Exception { public: using Exception::Exception; };
    class IOWriteException final : public Exception { public: using Exception::Exception; };

    class InvalidGameException final : public Exception { public: using Exception::Exception; };

    // Json data failed to parse
    class InvalidJsonException final : public Exception {};

    // The x,y of a camera in the json it outside of the map size (e.g camera 2,2 but the map size is 1,1)
    class CameraOutOfBoundsException final : public Exception{};

    // The TLV couldn't be created for this structure type because it isn't known
    class UnknownStructureTypeException final : public Exception { public: using Exception::Exception; };

    // When reading binary path data the length field of the TLV is not the fixed size we expected it to be
    class WrongTLVLengthException final : public Exception {};

    // Expected to read "key" in the json but it either didn't exist or was the wrong type
    class JsonKeyNotFoundException final : public Exception
    {
    public:
        explicit JsonKeyNotFoundException(const std::string& key)
            : Exception(key), mKey(key)
        {

        }

        const std::string& Key() const { return mKey; }

    private:
        std::string mKey;
    };

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

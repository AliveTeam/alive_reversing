#include "JsonMapRootInfoReader.hpp"
#include "relive_api_exceptions.hpp"
#include "JsonMapRootInfoReader.hpp"
#include <jsonxx/jsonxx.h>
#include "JsonReadUtils.hpp"
#include "file_api.hpp"

namespace ReliveAPI {
void JsonMapRootInfoReader::Read(IFileIO& fileIO, const std::string& fileName)
{
    auto inputFileStream = fileIO.Open(fileName, IFileIO::Mode::Read);
    if (!inputFileStream->IsOpen())
    {
        throw ReliveAPI::IOReadException(fileName.c_str());
    }

    std::string& jsonStr = getStaticStringBuffer();
    readFileContentsIntoString(jsonStr, *inputFileStream);

    jsonxx::Object rootObj;
    if (!rootObj.parse(jsonStr))
    {
        throw ReliveAPI::InvalidJsonException();
    }

    mMapRootInfo.mVersion = ReadNumber(rootObj, "api_version");
    mMapRootInfo.mGame = ReadString(rootObj, "game");

    if (mMapRootInfo.mGame == "AO")
    {
        return;
    }

    if (mMapRootInfo.mGame == "AE")
    {
        return;
    }

    /*
    if (mMapRootInfo.mVersion != AliveAPI::GetApiVersion())
    {
        // TODO: auto upgrade
    }*/


    throw ReliveAPI::InvalidGameException(mMapRootInfo.mGame);
}

void readFileContentsIntoString(std::string& target, IFile& file)
{
    file.ReadInto(target);
}

std::string& getStaticStringBuffer()
{
    static std::string result;
    return result;
}
} // namespace ReliveAPI

#include "JsonMapRootInfoReader.hpp"
#include "relive_api_exceptions.hpp"
#include "JsonMapRootInfoReader.hpp"
#include <jsonxx/jsonxx.h>
#include "JsonReadUtils.hpp"

namespace ReliveAPI {
void JsonMapRootInfoReader::Read(const std::string& fileName)
{
    std::ifstream inputFileStream(fileName.c_str());
    if (!inputFileStream.is_open())
    {
        throw ReliveAPI::IOReadException(fileName.c_str());
    }

    std::string& jsonStr = getStaticStringBuffer();
    readFileContentsIntoString(jsonStr, inputFileStream);

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
        // TODO: Upgrade
    }*/


    throw ReliveAPI::InvalidGameException(mMapRootInfo.mGame.c_str());
}

void readFileContentsIntoString(std::string& target, std::ifstream& ifs)
{
    ifs.seekg(0, std::ios::end);
    target.resize(static_cast<std::size_t>(ifs.tellg()));
    ifs.seekg(0);
    ifs.read(target.data(), target.size());
}

std::string& getStaticStringBuffer()
{
    static std::string result;
    return result;
}
} // namespace ReliveAPI

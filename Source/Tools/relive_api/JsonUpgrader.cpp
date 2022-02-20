#include "JsonUpgrader.hpp"
#include "relive_api.hpp"
#include "JsonMapRootInfoReader.hpp"
#include "JsonReadUtils.hpp"
#include "nlohmann/json.hpp"

namespace ReliveAPI {
void JsonUpgraderBase::RenameMapLevelItem(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName)
{
    auto map = rootObj["map"];
    if (map.contains(oldName))
    {
        auto tmp = map[oldName];
        map.erase(oldName);
        map[newName] = tmp;
    }
    rootObj["map"] = map;
}

std::string JsonUpgraderBase::Upgrade(const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion)
{
    std::ifstream inputFileStream(jsonFile.c_str());
    if (!inputFileStream.is_open())
    {
        throw ReliveAPI::IOReadException();
    }

    std::string& jsonStr = getStaticStringBuffer();
    readFileContentsIntoString(jsonStr, inputFileStream);

    UpgradeTargetIsValid(currentJsonVersion, targetApiVersion);

    AddUpgraders();

    s32 currentVersion = currentJsonVersion;
    while (currentVersion != targetApiVersion)
    {
        auto rootObj = nlohmann::json::parse(jsonStr, nullptr, false);
        if (rootObj.is_discarded())
        {
            throw ReliveAPI::InvalidJsonException();
        }

        jsonStr = mUpgraders[currentJsonVersion]()->Upgrade(*this, rootObj);
        currentVersion++;
    }

    // TODO: Replace/inject new schema

    return jsonStr;
}

void JsonUpgraderBase::UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion)
{
    if (currentJsonVersion > targetApiVersion)
    {
        // json is newer than what we support (probably created in a new editor and opened in an old editor).
        throw ReliveAPI::JsonVersionTooNew();
    }

    if (currentJsonVersion < 1)
    {
        // json is older than anything ever released, probably someone manually edited the json
        throw ReliveAPI::JsonVersionTooOld();
    }
}

} // namespace ReliveAPI

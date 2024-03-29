#include "JsonUpgrader.hpp"
#include "relive_api.hpp"
#include "JsonMapRootInfoReader.hpp"
#include "JsonReadUtils.hpp"
#include "TypesCollectionBase.hpp"
#include "file_api.hpp"

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

void JsonUpgraderBase::RenameMapObjectStructure(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName)
{
    bool anyCameraChanged = false;
    auto cameras = rootObj["map"]["cameras"];
    for (auto i = 0u; i < cameras.size(); i++)
    {
        auto mapObjects = cameras[i]["map_objects"];
        bool mapObjectsChanged = false;
        for (auto j = 0u; j < mapObjects.size(); j++)
        {
            auto mapObject = mapObjects[j];
            if (mapObject["object_structures_type"] == oldName)
            {
                mapObject["object_structures_type"] = newName;
                mapObjects[j] = mapObject;
                mapObjectsChanged = true;
            }
        }
        if (mapObjectsChanged)
        {
            cameras[i]["map_objects"] = mapObjects;
            anyCameraChanged = true;
        }
    }

    if (anyCameraChanged)
    {
        rootObj["map"]["cameras"] = cameras;
    }
}

void JsonUpgraderBase::RenameMapObjectProperty(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& oldName, const std::string& newName)
{
    auto cameras = rootObj["map"]["cameras"];
    bool anyCameraChanged = false;
    for (auto i = 0u; i < cameras.size(); i++)
    {
        auto mapObjects = cameras[i]["map_objects"];
        bool mapObjectsChanged = false;
        for (auto j = 0u; j < mapObjects.size(); j++)
        {
            auto mapObject = mapObjects[j];
            if (mapObject["object_structures_type"] == structureName)
            {
                auto properties = mapObject["properties"];
                if (properties.contains(oldName))
                {
                    auto oldValue = properties[oldName];
                    properties.erase(oldName);
                    properties[newName] = oldValue;

                    mapObject["properties"] = properties;
                    mapObjects[j] = mapObject;
                    mapObjectsChanged = true;
                }
            }
        }

        if (mapObjectsChanged)
        {
            cameras[i]["map_objects"] = mapObjects;
            anyCameraChanged = true;
        }
    }

    if (anyCameraChanged)
    {
        rootObj["map"]["cameras"] = cameras;
    }
}

std::string JsonUpgraderBase::Upgrade(TypesCollectionBase& baseTypesCollection, IFileIO& fileIO, const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion)
{
    auto inputFileStream = fileIO.Open(jsonFile, IFileIO::Mode::Read);
    if (!inputFileStream->IsOpen())
    {
        throw ReliveAPI::IOReadException(jsonFile);
    }

    std::string& jsonStr = getStaticStringBuffer();
    readFileContentsIntoString(jsonStr, *inputFileStream);

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

        // Remove the schemea now to avoid constantly re-parsing it
        rootObj.erase("schema");

        auto upgrader = mUpgraders.find(currentJsonVersion);
        if (upgrader != std::end(mUpgraders))
        {
            jsonStr = upgrader->second()->Upgrade(*this, rootObj);
        }

        currentVersion++;
    }

    // Replace/inject new schema - HACK using the old json lib for now and then parse the json string it creates with the new lib to add it
    {
        jsonxx::Object schemaObject;
        schemaObject << "object_structure_property_basic_types" << baseTypesCollection.BasicTypesToJson();
        schemaObject << "object_structure_property_enums" << baseTypesCollection.EnumsToJson();

        jsonxx::Array objectStructuresArray;
        baseTypesCollection.AddTlvsToJsonArray(objectStructuresArray);
        schemaObject << "object_structures" << objectStructuresArray;

        auto newSchema = nlohmann::json::parse(schemaObject.json());
        if (newSchema.is_discarded())
        {
            throw ReliveAPI::InvalidJsonException();
        }

        auto rootObj = nlohmann::json::parse(jsonStr, nullptr, false);
        if (rootObj.is_discarded())
        {
            throw ReliveAPI::InvalidJsonException();
        }
        rootObj["schema"] = newSchema;

        // Make sure we are now set to be the latest version
        rootObj["api_version"] = GetApiVersion();

        jsonStr = rootObj.dump(4);
    }

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

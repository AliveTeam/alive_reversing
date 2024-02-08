#include "JsonUpgrader.hpp"
#include "relive_api.hpp"
#include "JsonMapRootInfoReader.hpp"
#include "JsonReadUtils.hpp"
#include "TypesCollectionBase.hpp"
#include "file_api.hpp"
#include "nlohmann/json.hpp"

namespace ReliveAPI {
void JsonUpgraderBase::RenameMapLevelItem(nlohmann::json& rootObj, const std::string& oldName, const std::string& newName)
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

void JsonUpgraderBase::RenameMapObjectStructure(nlohmann::json& rootObj, const std::string& oldName, const std::string& newName)
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

void JsonUpgraderBase::RenameMapObjectProperty(nlohmann::json& rootObj, const std::string& structureName, const std::string& oldName, const std::string& newName)
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
        nlohmann::json schemaObject = nlohmann::json::object();
        schemaObject["object_structure_property_basic_types"] = baseTypesCollection.BasicTypesToJson();
        schemaObject["object_structure_property_enums"] = baseTypesCollection.EnumsToJson();

        nlohmann::json objectStructuresArray = nlohmann::json::array();
        baseTypesCollection.AddTlvsToJsonArray(objectStructuresArray);
        schemaObject["object_structures"] = objectStructuresArray;

        nlohmann::json newSchema = nlohmann::json::parse(schemaObject.dump(4), nullptr, false);
        if (newSchema.is_discarded())
        {
            throw ReliveAPI::InvalidJsonException();
        }

        nlohmann::json rootObj = nlohmann::json::parse(jsonStr, nullptr, false);
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

template <typename MapType, typename DefaultValueType>
void JsonUpgraderBase::RemapMapObjectPropertyValuesImpl(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const MapType& remapValues, DefaultValueType defaultVal)
{
    bool camerasChanged = false;
    auto cameras = rootObj["map"]["cameras"];
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
                if (properties.contains(propertyName))
                {
                    auto newVal = remapValues.find(properties[propertyName]);
                    properties[propertyName] = newVal == std::end(remapValues) ? defaultVal : newVal->second;

                    mapObject["properties"] = properties;
                    mapObjects[j] = mapObject;
                    mapObjectsChanged = true;
                }
            }
        }

        if (mapObjectsChanged)
        {
            cameras[i]["map_objects"] = mapObjects;
            camerasChanged = true;
        }
    }

    if (camerasChanged)
    {
        rootObj["map"]["cameras"] = cameras;
    }
}

void JsonUpgraderBase::RemapMapObjectPropertyValues(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const RemapEnums& remapValues, typename RemapEnums::mapped_type defaultVal)
{
    // Map from A -> A_unique, B -> B_unique
    // and then map back from A_unique -> A and B_unique -> B after all replacements are done
    // this handles the case of swapping enum values A and B without all of them turning into A or B due to
    // the lack of a temp value.
    const char uniqueValue[] = "_unique";
    RemapEnums toUniqueNames;
    for (const auto& [oldVal, newVal] : remapValues)
    {
        toUniqueNames[oldVal] = newVal + uniqueValue;
    }

    RemapEnums fromUniqueNames;
    for (const auto& [oldVal, newVal] : remapValues)
    {
        fromUniqueNames[newVal + uniqueValue] = newVal;
    }

    RemapMapObjectPropertyValuesImpl(rootObj, structureName, propertyName, toUniqueNames, defaultVal);
    RemapMapObjectPropertyValuesImpl(rootObj, structureName, propertyName, fromUniqueNames, defaultVal);
}

void JsonUpgraderBase::RemapMapObjectPropertyValues(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const RemapNumbers& remapValues, typename RemapNumbers::mapped_type defaultVal)
{
    // Map from A -> A_unique, B -> B_unique
    // and then map back from A_unique -> A and B_unique -> B after all replacements are done
    // this handles the case of swapping enum values A and B without all of them turning into A or B due to
    // the lack of a temp value.
    const int uniqueValue = 123456;
    RemapNumbers toUniqueNames;
    for (const auto& [oldVal, newVal] : remapValues)
    {
        toUniqueNames[oldVal] = newVal + uniqueValue;
    }

    RemapNumbers fromUniqueNames;
    for (const auto& [oldVal, newVal] : remapValues)
    {
        fromUniqueNames[newVal + uniqueValue] = newVal;
    }

    RemapMapObjectPropertyValuesImpl(rootObj, structureName, propertyName, toUniqueNames, defaultVal);
    RemapMapObjectPropertyValuesImpl(rootObj, structureName, propertyName, fromUniqueNames, defaultVal);
}

void JsonUpgraderBase::RemapMapObjectPropertyValues(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const RemapNumberToEnum& remapValues, typename RemapNumberToEnum::mapped_type defaultVal)
{
    // Map from A -> A_unique, B -> B_unique
    // and then map back from A_unique -> A and B_unique -> B after all replacements are done
    // this handles the case of swapping enum values A and B without all of them turning into A or B due to
    // the lack of a temp value.
    const char uniqueValue[] = "_unique";
    RemapNumberToEnum toUniqueNames;
    for (const auto& [oldKey, newVal] : remapValues)
    {
        toUniqueNames[oldKey] = newVal + uniqueValue; // Replace the number with a unique string
    }

    RemapEnums fromUniqueNames;
    for (const auto& [oldKey, newVal] : remapValues)
    {
        fromUniqueNames[oldKey + uniqueValue] = newVal; // Now we are mapping the new temp enum value to the enum value we originally wanted
    }

    RemapMapObjectPropertyValuesImpl(rootObj, structureName, propertyName, toUniqueNames, defaultVal);
    RemapMapObjectPropertyValuesImpl(rootObj, structureName, propertyName, fromUniqueNames, defaultVal);
}

} // namespace ReliveAPI

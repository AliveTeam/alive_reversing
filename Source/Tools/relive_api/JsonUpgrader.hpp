#pragma once

#include "../../AliveLibCommon/AddPointer.hpp"
#include "../../AliveLibCommon/Types.hpp"

#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include "nlohmann/json.hpp"

namespace ReliveAPI {
class JsonUpgraderBase;

class IJsonUpgrader
{
public:
    virtual ~IJsonUpgrader()
    { }
    virtual std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) = 0;
};

using TUpgradeFactoryFn = AddPointer_t<std::unique_ptr<IJsonUpgrader>()>;

using RemapNumbers = std::map<int, int>;
using RemapEnums = std::map<std::string, std::string>;
using RemapNumberToEnum = std::map<int, std::string>;

class TypesCollectionBase;
class IFileIO;

class JsonUpgraderBase
{
public:
    virtual ~JsonUpgraderBase()
    { }
    virtual void AddUpgraders() = 0;

    std::string Upgrade(TypesCollectionBase& baseTypesCollection, IFileIO& fileIO, const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion);

private:
    void UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion);

public:
    // Rename an item at the top level under "map : { }"
    void RenameMapLevelItem(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName);

    void RenameMapObjectStructure(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName);
    void RenameMapObjectProperty(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& oldName, const std::string& newName);

    template <typename MapType, typename DefaultValueType>
    void RemapMapObjectPropertyValuesImpl(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& propertyName, const MapType& remapValues, DefaultValueType defaultVal = {})
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

    void RemapMapObjectPropertyValues(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& propertyName, const RemapEnums& remapValues, typename RemapEnums::mapped_type defaultVal = {})
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

    void RemapMapObjectPropertyValues(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& propertyName, const RemapNumbers& remapValues, typename RemapNumbers::mapped_type defaultVal = {})
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

    void RemapMapObjectPropertyValues(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& propertyName, const RemapNumberToEnum& remapValues, typename RemapNumberToEnum::mapped_type defaultVal = {})
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

protected:
    std::unordered_map<s32, TUpgradeFactoryFn> mUpgraders;
};
} // namespace ReliveAPI

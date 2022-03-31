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

class JsonUpgraderBase
{
public:
    virtual ~JsonUpgraderBase()
    { }
    virtual void AddUpgraders() = 0;

    std::string Upgrade(TypesCollectionBase& baseTypesCollection, const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion);

private:
    void UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion);

public:
    // Rename an item at the top level under "map : { }"
    void RenameMapLevelItem(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName);

    void RenameMapObjectStructure(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName);
    void RenameMapObjectProperty(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& oldName, const std::string& newName);

    template <typename MapType>
    void RemapMapObjectPropertyValues(nlohmann::basic_json<>& rootObj, const std::string& structureName, const std::string& propertyName, const MapType& remapValues)
    {
        auto cameras = rootObj["map"]["cameras"];
        for (auto i = 0u; i < cameras.size(); i++)
        {
            auto mapObjects = cameras[i]["map_objects"];
            bool mapObjectsChanged = false;
            for (auto j = 0u; j < mapObjects.size(); j++)
            {
                auto mapObject = mapObjects[i];
                if (mapObject["object_structures_type"] == structureName)
                {
                    bool propertiesChanged = false;
                    auto properties = mapObject["properties"];
                    for (auto k = 0u; k < properties.size(); k++)
                    {
                        if (properties[k].contains(propertyName))
                        {
                            auto oldValue = properties[k][propertyName];
                            auto newVal = remapValues.find(oldValue);
                            if (newVal == std::end(remapValues))
                            {
                                // No remapped value, use the default value
                                properties[k][propertyName] = {};
                            }
                            else
                            {
                                properties[k][propertyName] = newVal->second;
                            }

                            propertiesChanged = true;
                        }
                    }

                    if (propertiesChanged)
                    {
                        mapObject["properties"] = properties;
                        mapObjects[i] = mapObject;
                        mapObjectsChanged = true;
                    }
                }
            }

            if (mapObjectsChanged)
            {
                cameras[i]["map_objects"] = mapObjects;
            }
        }
    }

protected:
    std::unordered_map<s32, TUpgradeFactoryFn> mUpgraders;
};
} // namespace ReliveAPI

#pragma once

#include "../../relive_lib/AddPointer.hpp"
#include "../../relive_lib/Types.hpp"

#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include "nlohmann/json_fwd.hpp"

namespace ReliveAPI {
class JsonUpgraderBase;

class IJsonUpgrader
{
public:
    virtual ~IJsonUpgrader()
    { }
    virtual std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::json& rootObj) = 0;
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
    void RenameMapLevelItem(nlohmann::json& rootObj, const std::string& oldName, const std::string& newName);

    void RenameMapObjectStructure(nlohmann::json& rootObj, const std::string& oldName, const std::string& newName);
    void RenameMapObjectProperty(nlohmann::json& rootObj, const std::string& structureName, const std::string& oldName, const std::string& newName);

    template <typename MapType, typename DefaultValueType>
    void RemapMapObjectPropertyValuesImpl(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const MapType& remapValues, DefaultValueType defaultVal = {});

    void RemapMapObjectPropertyValues(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const RemapEnums& remapValues, typename RemapEnums::mapped_type defaultVal = {});
    void RemapMapObjectPropertyValues(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const RemapNumbers& remapValues, typename RemapNumbers::mapped_type defaultVal = {});
    void RemapMapObjectPropertyValues(nlohmann::json& rootObj, const std::string& structureName, const std::string& propertyName, const RemapNumberToEnum& remapValues, typename RemapNumberToEnum::mapped_type defaultVal = {});

protected:
    std::unordered_map<s32, TUpgradeFactoryFn> mUpgraders;
};
} // namespace ReliveAPI

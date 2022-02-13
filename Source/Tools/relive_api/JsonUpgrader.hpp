#pragma once

#include "../../AliveLibCommon/AddPointer.hpp"
#include "../../AliveLibCommon/Types.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include "nlohmann\json.hpp"

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

class JsonUpgraderBase
{
public:
    virtual ~JsonUpgraderBase()
    { }
    virtual void AddUpgraders() = 0;

    std::string Upgrade(const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion);

private:
    void UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion);

public:
    // Rename an item at the top level under "map : { }"
    void RenameMapLevelItem(nlohmann::basic_json<>& rootObj, const std::string& oldName, const std::string& newName);

protected:
    std::unordered_map<s32, TUpgradeFactoryFn> mUpgraders;
};
} // namespace ReliveAPI

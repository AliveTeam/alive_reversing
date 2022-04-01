#include "JsonUpgraderAO.hpp"

#include "JsonUpgraderMacros.hpp"

namespace ReliveAPI {


class ExampleUpgrader final : public IJsonUpgrader
{
public:
    std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) override
    {
        upgrader.RenameMapLevelItem(rootObj, "hintfly_messages", "mouzedrift_messages");

        upgrader.RenameMapObjectStructure(rootObj, "WellExpress", "NotVeryWellExpress");
        upgrader.RenameMapObjectProperty(rootObj, "NotVeryWellExpress", "Animation ID (Unused?)", "NuclearLaunchCode");

        const RemapNumbers oldToNewNumbers = {{0, 1}, {2, 3}};
        upgrader.RemapMapObjectPropertyValues(rootObj, "NotVeryWellExpress", "NuclearLaunchCode", oldToNewNumbers);

        const RemapEnums oldToNewEnums = {{"Yes", "Go on then my son"}};
        upgrader.RemapMapObjectPropertyValues(rootObj, "NotVeryWellExpress", "Emit Leaves", oldToNewEnums);

        const RemapNumberToEnum oldNumbersToEnums = {{0, "Better meaning of 0"}};
        upgrader.RemapMapObjectPropertyValues(rootObj, "NotVeryWellExpress", "Exit X", oldNumbersToEnums);

        return rootObj.dump(4);
    }
};

void JsonUpgraderAO::AddUpgraders()
{
    ADD_UPGRADE_STEP_FROM(1, ExampleUpgrader);
}
} // namespace ReliveAPI

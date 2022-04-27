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

class Upgrader3To4 final : public IJsonUpgrader
{
public:
    std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) override
    {
        const RemapEnums swapLeftRight =
        {
             {"Left", "Right_temp"},
             {"Right", "Left"},
             {"Right_temp", "Right"},
        };
        upgrader.RemapMapObjectPropertyValues(rootObj, "ContinuePoint", "Abe Spawn Direction", swapLeftRight);
        upgrader.RenameMapObjectProperty(rootObj, "MeatSaw", "Switch Max TIme Off", "Switch Max Time Off");
        upgrader.RenameMapObjectProperty(rootObj, "LiftPoint", "Point ID", "Lift Point ID");
        upgrader.RenameMapObjectProperty(rootObj, "LiftMover", "Switch ID", "Lift Mover Switch ID");
        upgrader.RenameMapObjectProperty(rootObj, "LiftMover", "Lift ID", "Target Lift Point ID");
        upgrader.RenameMapObjectStructure(rootObj, "SlogHut", "ZzzSpawner");
        upgrader.RemapMapObjectPropertyValues(rootObj, "LiftMudokon", "Start Direction", swapLeftRight);
        upgrader.RemapMapObjectPropertyValues(rootObj, "BoomMachine", "Nozzle Side", swapLeftRight);
        upgrader.RenameMapObjectProperty(rootObj, "SlogSpawner", "Slog Spawn Delay", "Slog Spawn Interval");
        return rootObj.dump(4);
    }
};

void JsonUpgraderAO::AddUpgraders()
{
    ADD_UPGRADE_STEP_FROM(3, Upgrader3To4);
}
} // namespace ReliveAPI

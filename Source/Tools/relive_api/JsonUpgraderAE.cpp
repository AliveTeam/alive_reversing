#include "JsonUpgraderAE.hpp"
#include "JsonUpgraderMacros.hpp"

namespace ReliveAPI {
class TestUpgrader final : public IJsonUpgrader
{
public:
    std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) override
    {
        upgrader.RenameMapLevelItem(rootObj, "hintfly_messages", "mouzedrift_messages");

        return rootObj.dump(4);
    }
};

class UpgraderAE3 final : public IJsonUpgrader
{
public:
    std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) override
    {
        upgrader.RenameMapObjectProperty(rootObj, "LiftMover", "Lift Point Switch ID", "Target Lift Point ID");
        upgrader.RenameMapObjectProperty(rootObj, "LiftPoint", "Lift Point Switch ID", "Lift Point ID");
        upgrader.RenameMapObjectStructure(rootObj, "SlogHut", "ZzzSpawner");
        upgrader.RenameMapObjectProperty(rootObj, "SlapLock", "Invisibility Power-up ID", "Invisibility Duration");
        upgrader.RenameMapObjectProperty(rootObj, "ExplosionSet", "Explosion Interval", "Asset Interval");
        upgrader.RenameMapObjectProperty(rootObj, "ExplosionSet", "Big Rocks", "Spawn Assets");
        const RemapEnums swapLeftRight = 
        {
             {"Left", "Right"},
             {"Right", "Left"}
        };
        upgrader.RemapMapObjectPropertyValues(rootObj, "BoomMachine", "Nozzle Side", swapLeftRight);
        upgrader.RemapMapObjectPropertyValues(rootObj, "SlogSpawner", "Start Direction", swapLeftRight);
        return rootObj.dump(4);
    }
};

class UpgraderAE4 final : public IJsonUpgrader
{
public:
    std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) override
    {
        const RemapEnums directionToChoice =
        {
             {"Left", "No"},
             {"Right", "Yes"}
        };
        upgrader.RemapMapObjectPropertyValues(rootObj, "SligSpawner", "Chase Abe When Spotted", directionToChoice);
        return rootObj.dump(4);
    }
};

void JsonUpgraderAE::AddUpgraders()
{
    ADD_UPGRADE_STEP_FROM(3, UpgraderAE3);
    ADD_UPGRADE_STEP_FROM(4, UpgraderAE4);
}
} // namespace ReliveAPI

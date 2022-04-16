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

class Upgrader3To4 final : public IJsonUpgrader
{
public:
    std::string Upgrade(JsonUpgraderBase& upgrader, nlohmann::basic_json<>& rootObj) override
    {
        upgrader.RenameMapObjectProperty(rootObj, "LiftMover", "Lift Point Switch ID", "Target Lift Point ID");
        upgrader.RenameMapObjectProperty(rootObj, "LiftPoint", "Lift Point Switch ID", "Lift Point ID");
        return rootObj.dump(4);
    }
};

void JsonUpgraderAE::AddUpgraders()
{
    ADD_UPGRADE_STEP_FROM(1, TestUpgrader);
    ADD_UPGRADE_STEP_FROM(3, Upgrader3To4);
}
} // namespace ReliveAPI

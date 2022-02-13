#include "JsonUpgraderAO.hpp"

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

void JsonUpgraderAO::AddUpgraders()
{
    ADD_UPGRADE_STEP(1, TestUpgrader);
}
} // namespace ReliveAPI

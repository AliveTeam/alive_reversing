#include "JsonUpgraderAO.hpp"

#include "JsonUpgraderMacros.hpp"

namespace ReliveAPI {
void JsonUpgraderAO::AddUpgraders()
{
    ADD_UPGRADE_STEP(2, DoNothingUpgrader);
}
} // namespace ReliveAPI

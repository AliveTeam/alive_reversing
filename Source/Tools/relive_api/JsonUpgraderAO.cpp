#include "JsonUpgraderAO.hpp"

#include "JsonUpgraderMacros.hpp"

void JsonUpgraderAO::AddUpgraders()
{
    ADD_UPGRADE_STEP(2, DoNothingUpgrader);
}

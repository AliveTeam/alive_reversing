#include "AOJsonUpgrader.hpp"

#include "JsonUpgraderMacros.hpp"

void AOJsonUpgrader::AddUpgraders()
{
    ADD_UPGRADE_STEP(2, DoNothingUpgrader);
}

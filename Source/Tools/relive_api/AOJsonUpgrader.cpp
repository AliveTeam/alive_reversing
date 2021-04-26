#include "AOJsonUpgrader.hpp"

#include "JsonUpgrader.hpp"

void AOJsonUpgrader::AddUpgraders()
{
    ADD_UPGRADE_STEP(2, DoNothingUpgrader);
}

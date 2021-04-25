#include "stdafx.h"
#include "AOJsonUpgrader.hpp"

void AOJsonUpgrader::AddUpgraders()
{
    ADD_UPGRADE_STEP(2, DoNothingUpgrader);
}

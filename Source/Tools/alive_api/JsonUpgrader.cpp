#include "stdafx.h"
#include "JsonUpgrader.hpp"
#include "alive_api.hpp"

AliveAPI::UpgradeError BaseJsonUpgrader::Upgrade(const std::string& /*jsonFile*/, int currentJsonVersion, int targetApiVersion)
{
    auto ret = UpgradeTargetIsValid(currentJsonVersion, targetApiVersion);
    if (ret != AliveAPI::UpgradeError::None)
    {
        return ret;
    }

    int currentVersion = currentJsonVersion;
    while (currentVersion != targetApiVersion)
    {
        ret = mUpgraders[currentJsonVersion]()->Upgrade();
        if (ret != AliveAPI::UpgradeError::None)
        {
            return ret;
        }
        currentVersion++;
    }

    return ret;
}

AliveAPI::UpgradeError BaseJsonUpgrader::UpgradeTargetIsValid(int currentJsonVersion, int targetApiVersion)
{
    if (currentJsonVersion > targetApiVersion)
    {
        // json is newer than what we support (probably created in a new editor and opened in an old editor).
        abort();
    }

    if (currentJsonVersion < 1)
    {
        // json is older than anything ever released, probably someone manually edited the json
        abort();
    }

    return AliveAPI::UpgradeError::None;
}

AliveAPI::UpgradeError DoNothingUpgrader::Upgrade()
{
    return AliveAPI::UpgradeError::None;
}

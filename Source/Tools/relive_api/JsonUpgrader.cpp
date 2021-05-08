#include "stdafx.h"
#include "JsonUpgrader.hpp"
#include "relive_api.hpp"

void JsonUpgraderBase::Upgrade(const std::string& /*jsonFile*/, s32 currentJsonVersion, s32 targetApiVersion)
{
    UpgradeTargetIsValid(currentJsonVersion, targetApiVersion);

    s32 currentVersion = currentJsonVersion;
    while (currentVersion != targetApiVersion)
    {
        mUpgraders[currentJsonVersion]()->Upgrade();
        currentVersion++;
    }
}

void JsonUpgraderBase::UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion)
{
    if (currentJsonVersion > targetApiVersion)
    {
        // json is newer than what we support (probably created in a new editor and opened in an old editor).
        throw ReliveAPI::JsonVersionTooNew();
    }

    if (currentJsonVersion < 1)
    {
        // json is older than anything ever released, probably someone manually edited the json
        throw ReliveAPI::JsonVersionTooOld();
    }
}

void DoNothingUpgrader::Upgrade()
{
}

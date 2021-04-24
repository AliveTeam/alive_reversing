#pragma once

#include "AddPointer.hpp"

namespace AliveAPI
{
    enum class UpgradeError;
}

class IJsonUpgrader
{
public:
    virtual ~IJsonUpgrader() { }
    virtual AliveAPI::UpgradeError Upgrade() = 0;
};

class DoNothingUpgrader : public IJsonUpgrader
{
public:
    AliveAPI::UpgradeError Upgrade() override;
};

using TUpgradeFactoryFn = AddPointer_t<std::unique_ptr<IJsonUpgrader>()>;

#define ADD_UPGRADE_STEP(version, upgradeObjType)  mUpgraders[version] = []() -> std::unique_ptr<IJsonUpgrader> { return std::make_unique<upgradeObjType>(); }

class BaseJsonUpgrader
{
public:
    virtual ~BaseJsonUpgrader() { }
    virtual void AddUpgraders() = 0;

    AliveAPI::UpgradeError Upgrade(const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion);

private:
    AliveAPI::UpgradeError UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion);

protected:
    std::map<s32, TUpgradeFactoryFn> mUpgraders;
};

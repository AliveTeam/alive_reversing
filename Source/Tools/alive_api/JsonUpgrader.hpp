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

    AliveAPI::UpgradeError Upgrade(const std::string& jsonFile, int currentJsonVersion, int targetApiVersion);

private:
    AliveAPI::UpgradeError UpgradeTargetIsValid(int currentJsonVersion, int targetApiVersion);

protected:
    std::map<int, TUpgradeFactoryFn> mUpgraders;
};

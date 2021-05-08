#pragma once

#include "../AliveLibCommon/AddPointer.hpp"
#include "../AliveLibCommon/Types.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class IJsonUpgrader
{
public:
    virtual ~IJsonUpgrader()
    { }
    virtual void Upgrade() = 0;
};

class DoNothingUpgrader final : public IJsonUpgrader
{
public:
    void Upgrade() override;
};

using TUpgradeFactoryFn = AddPointer_t<std::unique_ptr<IJsonUpgrader>()>;

class JsonUpgraderBase
{
public:
    virtual ~JsonUpgraderBase()
    { }
    virtual void AddUpgraders() = 0;

    void Upgrade(const std::string& jsonFile, s32 currentJsonVersion, s32 targetApiVersion);

private:
    void UpgradeTargetIsValid(s32 currentJsonVersion, s32 targetApiVersion);

protected:
    std::unordered_map<s32, TUpgradeFactoryFn> mUpgraders;
};

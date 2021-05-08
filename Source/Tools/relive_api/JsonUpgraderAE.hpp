#pragma once

#include "JsonUpgrader.hpp"

class JsonUpgraderAE : public JsonUpgraderBase
{
public:
    virtual void AddUpgraders() override;
};

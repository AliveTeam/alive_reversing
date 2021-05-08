#pragma once

#include "JsonUpgrader.hpp"

class JsonUpgraderAO : public JsonUpgraderBase
{
public:
    virtual void AddUpgraders() override;
};

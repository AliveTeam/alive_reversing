#pragma once

#include "JsonUpgrader.hpp"

class AOJsonUpgrader : public BaseJsonUpgrader
{
public:
    virtual void AddUpgraders() override;
};

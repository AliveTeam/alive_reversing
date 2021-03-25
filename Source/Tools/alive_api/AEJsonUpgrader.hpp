#pragma once

#include "JsonUpgrader.hpp"

class AEJsonUpgrader : public BaseJsonUpgrader
{
public:
    virtual void AddUpgraders() override;
};

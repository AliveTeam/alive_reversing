#pragma once

#include "JsonUpgrader.hpp"

namespace ReliveAPI {
class JsonUpgraderAE : public JsonUpgraderBase
{
public:
    virtual void AddUpgraders() override;
};
} // namespace ReliveAPI

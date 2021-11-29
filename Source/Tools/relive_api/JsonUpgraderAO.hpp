#pragma once

#include "JsonUpgrader.hpp"

namespace ReliveAPI {
class JsonUpgraderAO : public JsonUpgraderBase
{
public:
    virtual void AddUpgraders() override;
};
} // namespace ReliveAPI

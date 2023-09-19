#pragma once

#include "JsonUpgrader.hpp"

namespace ReliveAPI {
class JsonUpgraderRelive : public JsonUpgraderBase
{
public:
    virtual void AddUpgraders() override;
};
} // namespace ReliveAPI

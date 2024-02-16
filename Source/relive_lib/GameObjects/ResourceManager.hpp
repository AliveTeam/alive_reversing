#pragma once

#include "BaseGameObject.hpp"

class ResourceManager final : public BaseGameObject
{
public:
    ResourceManager();

    void VUpdate() override;
    void VScreenChanged() override;
};

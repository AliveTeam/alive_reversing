#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

class ResourceManager final : public BaseGameObject
{
public:
    ResourceManager();

    void VUpdate() override;
    void VScreenChanged() override;
};

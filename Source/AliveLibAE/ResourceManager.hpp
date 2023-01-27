#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

void Game_ShowLoadingIcon();

class ResourceManager final : public BaseGameObject
{
public:
    ResourceManager();

    void VUpdate() override;
    void VScreenChanged() override;
};

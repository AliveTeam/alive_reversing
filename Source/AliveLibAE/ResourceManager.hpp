#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/AddPointer.hpp"
#include "../relive_lib/Psx.hpp"

class Camera;

void Game_ShowLoadingIcon();

class ResourceManager final : public BaseGameObject
{
public:
    ResourceManager();
    ~ResourceManager();

    void LoadingLoop(s16 bShowLoadingIcon);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:

};

extern ResourceManager* pResourceManager;
extern s16 bHideLoadingIcon;
extern s32 loading_ticks;

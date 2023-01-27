#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"

class Camera;

namespace AO {

class BaseAliveGameObject;
class LoadingFile;

void Game_ShowLoadingIcon();

class ResourceManager final
{
public:
    static void LoadingLoop(s16 bShowLoadingIcon);
};

extern s16 bHideLoadingIcon;
extern s32 loading_ticks;

} // namespace AO

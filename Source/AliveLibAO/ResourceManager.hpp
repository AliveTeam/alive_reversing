#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"

class Camera;

namespace AO {

class BaseAliveGameObject;
class LoadingFile;

void Game_ShowLoadingIcon_445EB0();

class ResourceManager final
{
public:
    static s32 SEQ_HashName_454EA0(const char_type* seqFileName);
    static void LoadingLoop(s16 bShowLoadingIcon);
};

extern s16 bHideLoadingIcon_5076A0;
extern s32 loading_ticks_5076A4;

} // namespace AO

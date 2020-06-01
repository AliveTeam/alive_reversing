#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

void Game_ForceLink();

START_NS_AO

class BaseGameObject;

ALIVE_VAR_EXTERN(int, gnFrameCount_507670);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618);

END_NS_AO

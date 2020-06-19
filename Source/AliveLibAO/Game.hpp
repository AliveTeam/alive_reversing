#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

void Game_ForceLink();

START_NS_AO

class BaseGameObject;

ALIVE_VAR_EXTERN(int, gnFrameCount_507670);
ALIVE_VAR_EXTERN(short, gAttract_507698);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618);

EXPORT void SYS_EventsPump_44FF90();


END_NS_AO

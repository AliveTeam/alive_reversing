#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class BaseAliveGameObject;

class MusicController
{
public:
    EXPORT static int CC Create_47FC40();
    EXPORT static void CC Shutdown_47FD20();
    EXPORT static void CC sub_47FD60(__int16 a1, BaseGameObject* a2, __int16 a3, char a4);
};

ALIVE_VAR_EXTERN(MusicController*, pMusicController_5C3020);

#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseAnimatedWithPhysicsGameObject;
class BaseAliveGameObject;

EXPORT int CC SFX_Play_43AE60(unsigned __int8 sfxId, int volume, int pitch, BaseAnimatedWithPhysicsGameObject* pObj);

EXPORT int CC SFX_Play_43AD70(unsigned __int8 sfxId, int a2, int a3);

EXPORT int CC SFX_Play_43ADE0(unsigned __int8 sfxId, int leftVol, int rightVol, BaseAliveGameObject* pObj);

END_NS_AO

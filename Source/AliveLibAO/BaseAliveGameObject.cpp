#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Function.hpp"

START_NS_AO

void BaseAliveGameObject_ForceLink() {}


ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

void BaseAliveGameObject::VCheckCollisionLineStillValid_401A90(int /*distance*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO

#include "stdafx_ao.h"
#include "Collisions.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

START_NS_AO

ALIVE_VAR(1, 0x504C6C, Collisions*, sCollisions_DArray_504C6C, nullptr);

EXPORT void Collisions::dtor_40CFB0()
{
    ao_delete_free_447540(field_0);
}

EXPORT Collisions* Collisions::ctor_40CF30(const CollisionInfo* /*pCollisionInfo*/, const BYTE* /*ppPathData*/)
{
    NOT_IMPLEMENTED();
    return this;
}

END_NS_AO

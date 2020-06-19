#pragma once

#include "FunctionFwd.hpp"

struct PSX_RECT;

START_NS_AO

struct CollisionInfo;

class Collisions
{
public:
    EXPORT void dtor_40CFB0();

    EXPORT Collisions* ctor_40CF30(const CollisionInfo* pCollisionInfo, const BYTE* ppPathData);

    void* field_0;
    int field_4;
    int field_8;
    int field_C;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

EXPORT PSX_RECT* CCSTD Rect_Clear_40C920(PSX_RECT* pRect);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_504C6C);


END_NS_AO

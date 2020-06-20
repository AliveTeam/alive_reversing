#pragma once

#include "Psx.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

struct PathLine
{
    PSX_RECT field_0_rect;
    char field_8_type;
    char field_9;
    char field_A;
    char field_B;
    int field_C;
    int field_10;
};
ALIVE_ASSERT_SIZEOF(PathLine, 20);

struct CollisionInfo;

class Collisions
{
public:
    EXPORT void dtor_40CFB0();

    EXPORT Collisions* ctor_40CF30(const CollisionInfo* pCollisionInfo, const BYTE* ppPathData);

    EXPORT PathLine* Add_Dynamic_Collision_Line_40C8A0( __int16 x1, __int16 y1, __int16 x2, __int16 y2, char mode);

    void* field_0;
    int field_4;
    int field_8;
    int field_C;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

EXPORT PSX_RECT* CCSTD Rect_Clear_40C920(PSX_RECT* pRect);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_504C6C);


END_NS_AO

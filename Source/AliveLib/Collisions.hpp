#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

struct PathLine
{
    __int16 field_0_x1;
    __int16 field_2_y1;
    __int16 field_4_x2;
    __int16 field_6_y2;
    __int8 field_8_mode;
    __int8 field_9_padding; // Mode is 1 byte, but compiler aligned it to 2
    __int16 field_A_unknown1;
    __int16 field_C_unknown2;
    __int16 field_E_unknown3;
    __int16 field_10_unknown4;
    __int16 field_12_unknown5;
};
ALIVE_ASSERT_SIZEOF(PathLine, 0x14);

struct CollisionInfo;

class Collisions
{
public:
    EXPORT Collisions* ctor_418930(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes);
    EXPORT void dtor_4189F0();
    EXPORT static void CC Factory_4188A0(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes);

    EXPORT signed __int16 Raycast_417A60(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP * hitX, FP * hitY, unsigned int modeMask);
public:
    PathLine* field_0_pArray;
    WORD field_4_current_item_count;
    int field_8_item_count;
    int field_C_max_count;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

using TCollisionsFactory = decltype(&Collisions::Factory_4188A0);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_5C1128);

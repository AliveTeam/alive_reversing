#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

namespace Test
{
    void CollisionTests();
}

enum eLineTypes : BYTE
{
    eFloor = 0,
    eWallLeft = 1,
    eWallRight = 2,
    eCeiling = 3,
    eBackGroundFloor = 4,
    eBackGroundWallLeft = 5,
    eBackGroundWallRight = 6,
    eBackGroundCeiling = 7,
    eTrackLine = 8,
    eArt = 9,
    eBulletWall = 10,
    eMineCarFloor = 11,
    eMineCarWall = 12,
    eMineCarCeiling = 13,
    eFlyingSligCeiling = 17,
};

struct PathLine
{
    __int16 field_0_x1;
    __int16 field_2_y1;
    __int16 field_4_x2;
    __int16 field_6_y2;
    BYTE field_8_type;
    //__int8 field_9_padding; // Mode is 1 byte, but compiler aligned it to 2
    __int16 field_A_previous;
    __int16 field_C_next;
    __int16 field_E_previous2;
    __int16 field_10_next2;
    __int16 field_12_line_length;
};
ALIVE_ASSERT_SIZEOF(PathLine, 0x14);

struct CollisionInfo;

class Collisions
{
public:
    EXPORT Collisions* ctor_418930(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes);
    EXPORT void dtor_4189F0();
    EXPORT static void CC Factory_4188A0(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes);
    EXPORT signed __int16 Raycast_Real_417A60(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP * hitX, FP * hitY, unsigned int modeMask);
    EXPORT signed __int16 Raycast_417A60(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP * hitX, FP * hitY, unsigned int modeMask);
    EXPORT PathLine* Add_Dynamic_Collision_Line_417FA0(__int16 x1, __int16 y1, __int16 x2, __int16 y2, char mode);
    EXPORT PathLine* Get_Line_At_Idx_418070(__int16 idx);
    EXPORT PathLine* PreviousLine_4180A0(PathLine* pLine);
    EXPORT PathLine* NextLine_418180(PathLine* pLine);
    signed __int16 Raycast_Impl(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP * hitX, FP * hitY, unsigned int modeMask);
public:
    PathLine* field_0_pArray;
    WORD field_4_current_item_count;
    int field_8_item_count;
    int field_C_max_count;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

struct PSX_RECT;

EXPORT PSX_RECT* __stdcall Rect_Clear_418040(PSX_RECT* pRect);

using TCollisionsFactory = decltype(&Collisions::Factory_4188A0);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_5C1128);

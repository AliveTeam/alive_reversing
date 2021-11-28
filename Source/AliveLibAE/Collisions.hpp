#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

namespace AETest::TestsCollision {
void CollisionTests();
}

enum eLineTypes : u8
{
    eFloor_0 = 0,
    eWallLeft_1 = 1,
    eWallRight_2 = 2,
    eCeiling_3 = 3,
    eBackGroundFloor_4 = 4,
    eBackGroundWallLeft_5 = 5,
    eBackGroundWallRight_6 = 6,
    eBackGroundCeiling_7 = 7,
    eTrackLine_8 = 8,
    eArt_9 = 9,
    eBulletWall_10 = 10,
    eMineCarFloor_11 = 11,
    eMineCarWall_12 = 12,
    eMineCarCeiling_13 = 13,
    eFlyingSligCeiling_17 = 17,
};

class PathLine final
{
public:
    PSX_RECT field_0_rect;
    u8 field_8_type;
    s8 field_9_padding; // Mode is 1 byte, but compiler aligned it to 2
    s16 field_A_previous;
    s16 field_C_next;
    s16 field_E_previous2; // Never used
    s16 field_10_next2;    // And... also never used
    s16 field_12_line_length;

    EXPORT PathLine* MoveOnLine_418260(FP* xpos, FP* ypos, FP velX);
};
ALIVE_ASSERT_SIZEOF(PathLine, 0x14);

struct CollisionInfo;

class Collisions final
{
public:
    EXPORT Collisions* ctor_418930(const CollisionInfo* pCollisionInfo, const u8* pPathRes);
    EXPORT void dtor_4189F0();
    EXPORT static void CC Factory_4188A0(const CollisionInfo* pCollisionInfo, const u8* pPathRes);
    EXPORT s16 Raycast_Real_417A60(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP* hitX, FP* hitY, u32 modeMask);
    EXPORT BOOL Raycast_417A60(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP* hitX, FP* hitY, u32 modeMask);
    EXPORT PathLine* Add_Dynamic_Collision_Line_417FA0(s16 x1, s16 y1, s16 x2, s16 y2, s8 mode);
    EXPORT PathLine* Get_Line_At_Idx_418070(s16 idx);
    EXPORT PathLine* PreviousLine_4180A0(PathLine* pLine);
    EXPORT PathLine* NextLine_418180(PathLine* pLine);
    s16 Raycast_Impl(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP* hitX, FP* hitY, u32 modeMask);

public:
    PathLine* field_0_pArray;
    u16 field_4_current_item_count;
    s32 field_8_item_count;
    s32 field_C_max_count;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

struct PSX_RECT;

EXPORT PSX_RECT* CCSTD Rect_Clear_418040(PSX_RECT* pRect);

using TCollisionsFactory = decltype(&Collisions::Factory_4188A0);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_5C1128);

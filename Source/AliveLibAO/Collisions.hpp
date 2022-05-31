#pragma once

#include "Psx.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

namespace AO {

enum eLineTypes : u8
{
    eFloor_0 = 0,
    eWallLeft_1 = 1,
    eWallRight_2 = 2,
    eCeiling_3 = 3,
    eBackgroundFloor_4 = 4,
    eBackgroundWallLeft_5 = 5,
    eBackgroundWallRight_6 = 6,
    eBackgroundCeiling_7 = 7,
    eTrackLine_8 = 8,
    eArt_9 = 9,
    eBulletWall_10 = 10,

    eUnknown_32 = 32, // lift point and trap door but not sure which of them is which
    eUnknown_36 = 36,
};

class PathLine final
{
public:
    PSX_RECT field_0_rect;
    eLineTypes field_8_type;
    s8 field_9_pad;
    s8 field_A_pad;
    s8 field_B_pad;
    s32 field_C_previous;
    s32 field_10_next;

    PathLine* MoveOnLine(FP* pXPos, FP* pYPos, const FP distToMove);
};
ALIVE_ASSERT_SIZEOF(PathLine, 20);

struct CollisionInfo;

class Collisions final
{
public:
    static void Factory(const CollisionInfo* pCollisionInfo, const u8* pPathData);

    void dtor_40CFB0();

    Collisions* ctor_40CF30(const CollisionInfo* pCollisionInfo, const u8* pPathData);

    PathLine* Add_Dynamic_Collision_Line(s16 x1, s16 y1, s16 x2, s16 y2, eLineTypes mode);

    s16 RayCast(FP X1_16_16, FP Y1_16_16, FP X2_16_16, FP Y2_16_16, PathLine** ppLine, FP* hitX, FP* hitY, u32 modeMask);

    PathLine* PreviousLine(PathLine* pLine);

    PathLine* NextLine(PathLine* pLine);

    PathLine* field_0_pArray;
    u16 field_4_current_item_count;
    s32 field_8_item_count;
    s32 field_C_max_count;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

PSX_RECT* Rect_Clear(PSX_RECT* pRect);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_504C6C);


} // namespace AO

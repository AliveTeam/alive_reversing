#pragma once

#include "Psx.hpp"
#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

START_NS_AO

enum eLineTypes : BYTE
{
    eFloor_0 = 0,
    eWallLeft_1 = 1,
    eWallRight_2 = 2,
    eCeiling_3 = 3,
    eBackGroundFloor_4 = 4,
    eBackGroundWallLeft_5 = 5,
    eBackGroundWallRight_6 = 6
};

class PathLine
{
public:
    PSX_RECT field_0_rect;
    BYTE field_8_type;
    char field_9_pad;
    char field_A_pad;
    char field_B_pad;
    int field_C_previous;
    int field_10_next;

    EXPORT PathLine* MoveOnLine_40CA20(FP* pXPos, FP* pYPos, const FP distToMove);
};
ALIVE_ASSERT_SIZEOF(PathLine, 20);

struct CollisionInfo;

class Collisions
{
public:
    EXPORT static void CC Factory_40CEC0(const CollisionInfo* pCollisionInfo, const BYTE* pPathData);

    EXPORT void dtor_40CFB0();

    EXPORT Collisions* ctor_40CF30(const CollisionInfo* pCollisionInfo, const BYTE* pPathData);

    EXPORT PathLine* Add_Dynamic_Collision_Line_40C8A0( __int16 x1, __int16 y1, __int16 x2, __int16 y2, char mode);

    EXPORT signed __int16 RayCast_40C410(FP X1_16_16, FP Y1_16_16, FP X2_16_16, FP Y2_16_16, PathLine** ppLine, FP* hitX, FP* hitY, unsigned int modeMask);

    EXPORT PathLine* PreviousLine_40C940(PathLine* pLine);

    EXPORT PathLine* NextLine_40C9B0(PathLine* pLine);

    PathLine* field_0_pArray;
    WORD field_4_current_item_count;
    int field_8_item_count;
    int field_C_max_count;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

EXPORT PSX_RECT* CCSTD Rect_Clear_40C920(PSX_RECT* pRect);

ALIVE_VAR_EXTERN(Collisions*, sCollisions_DArray_504C6C);


END_NS_AO

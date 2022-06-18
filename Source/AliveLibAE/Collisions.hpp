#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

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
    eMineCarFloor_11 = 11,
    eMineCarWall_12 = 12,
    eMineCarCeiling_13 = 13,
    eBackgroundMineCarFloor_14 = 14,
    eBackgroundMineCarWall_15 = 15,
    eBackgroundMineCarCeiling_16 = 16,
    eFlyingSligCeiling_17 = 17,
    eBackgroundFlyingSligCeiling_18 = 18,

    eDynamicCollision_32 = 32,
    eBackgroundDynamicCollision_36 = 36,
};


class CollisionMask final
{
public:
    CollisionMask()
    {

    }

    constexpr explicit CollisionMask(eLineTypes lineType)
    {
        Add(lineType);
    }

    constexpr CollisionMask(eLineTypes line1, eLineTypes line2)
    {
        Add(line1);
        Add(line2);
    }

    constexpr CollisionMask(eLineTypes line1, eLineTypes line2, eLineTypes line3)
    {
        Add(line1);
        Add(line2);
        Add(line3);
    }

    constexpr CollisionMask(eLineTypes line1, eLineTypes line2, eLineTypes line3, eLineTypes line4)
    {
        Add(line1);
        Add(line2);
        Add(line3);
        Add(line4);
    }

    constexpr CollisionMask(eLineTypes line1, eLineTypes line2, eLineTypes line3, eLineTypes line4, eLineTypes line5)
    {
        Add(line1);
        Add(line2);
        Add(line3);
        Add(line4);
        Add(line5);
    }

    constexpr void Add(eLineTypes lineType)
    {
        mMask |= (1 << (static_cast<u32>(lineType) % 32));
    }

    constexpr u32 Mask() const noexcept
    {
        return mMask;
    }

public:
    u32 mMask = 0;
};

inline constexpr CollisionMask kFgWalls(eWallLeft_1, eWallRight_2);
inline constexpr CollisionMask kBgWalls(eBackgroundWallLeft_5, eBackgroundWallRight_6);

inline constexpr CollisionMask kFgFloor(eFloor_0, eDynamicCollision_32);
inline constexpr CollisionMask kBgFloor(eBackgroundFloor_4, eBackgroundDynamicCollision_36);

inline constexpr CollisionMask kFgOrBgFloor(eFloor_0, eDynamicCollision_32, eBackgroundFloor_4, eBackgroundDynamicCollision_36);

inline constexpr CollisionMask kFgFloorCeilingOrWalls(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32);
inline constexpr CollisionMask kBgFloorCeilingOrWalls(eBackgroundCeiling_7, eBackgroundWallRight_6, eBackgroundWallLeft_5, eBackgroundFloor_4, eBackgroundDynamicCollision_36);

inline constexpr CollisionMask kFgCeiling(eCeiling_3);
inline constexpr CollisionMask kBgCeiling(eBackgroundCeiling_7);

inline constexpr CollisionMask kFgFloorOrCeiling(eFloor_0, eDynamicCollision_32, eCeiling_3);
inline constexpr CollisionMask kBgFloorOrCeiling(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundCeiling_7);


inline constexpr CollisionMask kFgFloorWallOrCeiling(eFloor_0, eDynamicCollision_32, eWallLeft_1, eWallRight_2, eCeiling_3);
inline constexpr CollisionMask kBgFloorWallOrCeiling(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundCeiling_7);

class PathLine final
{
public:
    PSX_RECT field_0_rect;
    eLineTypes field_8_type;
    s8 field_9_pad;
    s16 field_A_previous;
    s16 field_C_next;
    s16 field_E_previous2; // Never used
    s16 field_10_next2;    // And... also never used
    s16 field_12_line_length;

    PathLine* MoveOnLine(FP* xpos, FP* ypos, FP velX);
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PathLine, 0x14);

struct CollisionInfo;

class Collisions final
{
public:
    ~Collisions();
    Collisions(const CollisionInfo* pCollisionInfo, const u8* pPathRes);

    PathLine* Add_Dynamic_Collision_Line(s16 x1, s16 y1, s16 x2, s16 y2, s8 mode);
    PathLine* Get_Line_At_Idx(s16 idx);
    Bool32 Raycast(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP* hitX, FP* hitY, const CollisionMask& modeMask);
    PathLine* PreviousLine(PathLine* pLine);
    PathLine* NextLine(PathLine* pLine);

public:
    PathLine* field_0_pArray = nullptr;
    u16 field_4_current_item_count = 0;
    s32 field_8_item_count = 0;
    s32 field_C_max_count = 0;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

struct PSX_RECT;

PSX_RECT* Rect_Clear(PSX_RECT* pRect);

ALIVE_VAR_EXTERN(Collisions*, sCollisions);

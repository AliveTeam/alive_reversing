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

inline constexpr CollisionMask kFgWallsOrFloor(eFloor_0, eDynamicCollision_32, eWallLeft_1, eWallRight_2);
inline constexpr CollisionMask kBgWallsOrFloor(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundWallLeft_5, eBackgroundWallRight_6);

class PathLine final
{
public:
    PSX_RECT mRect;
    eLineTypes mLineType;
    s8 field_9_pad;
    s8 field_A_pad;
    s8 field_B_pad;
    s32 field_C_previous;
    s32 field_10_next;

    PathLine* MoveOnLine(FP* pXPos, FP* pYPos, const FP distToMove);
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PathLine, 20);

struct CollisionInfo;

class Collisions final
{
public:
    ~Collisions();
    Collisions(const CollisionInfo* pCollisionInfo, const u8* pPathData);

    PathLine* Add_Dynamic_Collision_Line(s16 x1, s16 y1, s16 x2, s16 y2, eLineTypes mode);

    s16 Raycast(FP X1_16_16, FP Y1_16_16, FP X2_16_16, FP Y2_16_16, PathLine** ppLine, FP* hitX, FP* hitY, const CollisionMask& modeMask);

    PathLine* PreviousLine(PathLine* pLine);

    PathLine* NextLine(PathLine* pLine);

    PathLine* field_0_pArray = nullptr;
    u16 field_4_current_item_count = 0;
    s32 field_8_item_count = 0;
    s32 field_C_max_count = 0;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

PSX_RECT* Rect_Clear(PSX_RECT* pRect);

ALIVE_VAR_EXTERN(Collisions*, sCollisions);


} // namespace AO

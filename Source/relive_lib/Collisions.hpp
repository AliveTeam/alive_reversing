#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "../AliveLibCommon/Psx_common.hpp"

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

inline constexpr CollisionMask kFgWallsOrFloor(eFloor_0, eDynamicCollision_32, eWallLeft_1, eWallRight_2);
inline constexpr CollisionMask kBgWallsOrFloor(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundWallLeft_5, eBackgroundWallRight_6);

// Represents the AO line structure on disk, don't change the layout
class PathLineAO final
{
public:
    PSX_RECT mRect;
    eLineTypes mLineType;
    s8 field_9_pad;
    s8 field_A_pad;
    s8 field_B_pad;
    s32 field_C_previous;
    s32 field_10_next;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PathLineAO, 20);

// Represents the AE line structure on disk, don't change the layout
class PathLineAE final
{
public:
    PSX_RECT mRect;
    eLineTypes mLineType;
    s8 field_9_pad;
    s16 field_A_previous;
    s16 field_C_next;
    s16 field_E_previous2; // Never used
    s16 field_10_next2;    // And... also never used
    s16 field_12_line_length;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PathLineAE, 0x14);

// Another on disk structure that is identical between games
struct CollisionInfo final
{
    s16 field_4_left;
    s16 field_6_right;
    s16 field_8_top;
    s16 field_A_bottom;
    u32 field_C_collision_offset;
    u32 field_10_num_collision_items;
    u32 field_14_grid_width;
    u32 field_18_grid_height;
};
ALIVE_ASSERT_SIZEOF(CollisionInfo, 0x1C);

// The in memory representation of the line structure, layout etc doesn't matter
class PathLine final
{
public:
    PSX_RECT mRect;
    eLineTypes mLineType;
    s16 field_A_previous;
    s16 field_C_next;
    s16 field_12_line_length;
    PathLine* MoveOnLine(FP* xpos, FP* ypos, FP velX);
};



class Collisions final
{
public:
    ~Collisions();
    enum class LineFormat
    {
        Ao,
        Ae
    };
    Collisions(LineFormat lineFormat, const CollisionInfo* pCollisionInfo, const u8* pPathRes);

    PathLine* Add_Dynamic_Collision_Line(s16 x1, s16 y1, s16 x2, s16 y2, eLineTypes mode);
    PathLine* Get_Line_At_Idx(s16 idx);
    Bool32 Raycast(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP* hitX, FP* hitY, const CollisionMask& modeMask);
    PathLine* PreviousLine(PathLine* pLine);
    PathLine* NextLine(PathLine* pLine);

private:
    static void ToPathLine(PathLine& dst, const PathLineAO& src)
    {
        dst.mRect = src.mRect;
        dst.mLineType = src.mLineType;
        dst.field_A_previous = static_cast<s16>(src.field_C_previous);
        dst.field_C_next = static_cast<s16>(src.field_10_next);
        dst.field_12_line_length = 0; // TODO: Calculate for AO in the future
    }

    static void ToPathLine(PathLine& dst, const PathLineAE& src)
    {
        dst.mRect = src.mRect;
        dst.mLineType = src.mLineType;
        dst.field_A_previous = src.field_A_previous;
        dst.field_C_next = src.field_C_next;
        dst.field_12_line_length = src.field_12_line_length;
    }

    template <typename LineClassType>
    void ConvertLineTypes(const CollisionInfo* pCollisionInfo, const u8* pPathRes)
    {
        auto pLines = reinterpret_cast<const LineClassType>(&pPathRes[pCollisionInfo->field_C_collision_offset]);
        for (s32 i = 0; i < field_C_max_count; i++)
        {
            if (i < field_4_current_item_count)
            {
                ToPathLine(field_0_pArray[i], pLines[i]);
            }
            else
            {
                // Zero init the "free" dynamic items
                field_0_pArray[i] = {};
            }
        }
    }


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

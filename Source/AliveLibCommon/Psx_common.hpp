#pragma once

#include "FunctionFwd.hpp"

struct PSX_RECT
{
    short x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

struct PSX_Point
{
    short field_0_x;
    short field_2_y;
};
ALIVE_ASSERT_SIZEOF(PSX_Point, 4);

inline bool operator == (const PSX_RECT& lhs, const PSX_RECT& rhs)
{
    return
        (lhs.x == rhs.x) &&
        (lhs.y == rhs.y) &&
        (lhs.w == rhs.w) &&
        (lhs.h == rhs.h);
}

inline bool RectsOverlap(const PSX_RECT& r1, const PSX_RECT& r2)
{
    return !(r2.x > r1.w ||
        r2.w < r1.x ||
        r2.y > r1.h ||
        r2.h < r1.y);
}

struct PSX_Pos16
{
    short x, y;
};
ALIVE_ASSERT_SIZEOF(PSX_Pos16, 0x4);

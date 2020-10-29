#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint_common.hpp"

struct CdlLOC
{
    unsigned __int8 field_0_minute;
    unsigned __int8 field_1_second;
    unsigned __int8 field_2_sector;
    char field_3_track;
};
ALIVE_ASSERT_SIZEOF(CdlLOC, 0x4);


struct CdlFILE
{
    CdlLOC field_0_loc;
    int field_4_pos;
    char field_8_name[16];
};
ALIVE_ASSERT_SIZEOF(CdlFILE, 24);


struct PSX_RECT
{
    short x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

struct FP_RECT
{
    FP x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(FP_RECT, 4 * 4);

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

struct PSX_DR_ENV
{
    int field_0_tag;
    int field_4_code[15];
};
// TODO: Size

struct PSX_DRAWENV
{
    PSX_RECT field_0_clip;
    __int16 field_8_ofs[2];
    PSX_RECT field_C_tw;
    unsigned __int16 field_14_tpage;
    char field_16_dtd;
    char field_17_dfe;
    char field_18_isbg;
    char field_19_r0;
    char field_1A_g0;
    char field_1B_b0;
    PSX_DR_ENV field_1C_dr_env;
};
// TODO: Size

struct PSX_DISPENV
{
    PSX_RECT disp;
    PSX_RECT screen;
    char isinter;
    char isrgb24;
    char pad0;
    char pad1;
};
// TODO: Size

inline BOOL CC PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    return (pRect1->x <= pRect2->w && pRect1->w >= pRect2->x &&
        pRect1->y <= pRect2->h && pRect1->h >= pRect2->y);
}

#pragma once

#include "../AliveLibCommon/Types.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint_common.hpp"

struct CdlLOC final
{
    u8 field_0_minute;
    u8 field_1_second;
    u8 field_2_sector;
    s8 field_3_track;
};
ALIVE_ASSERT_SIZEOF(CdlLOC, 0x4);


struct CdlFILE final
{
    CdlLOC field_0_loc;
    s32 field_4_pos;
    s8 field_8_name[16];
};
ALIVE_ASSERT_SIZEOF(CdlFILE, 24);


struct PSX_RECT final
{
    s16 x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

struct FP_RECT final
{
    FP x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(FP_RECT, 4 * 4);

struct PSX_Point final
{
    s16 x, y;
};
ALIVE_ASSERT_SIZEOF(PSX_Point, 4);

inline bool operator==(const PSX_RECT& lhs, const PSX_RECT& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.w == rhs.w) && (lhs.h == rhs.h);
}

inline bool RectsOverlap(const PSX_RECT& r1, const PSX_RECT& r2)
{
    return !(r2.x > r1.w || r2.w < r1.x || r2.y > r1.h || r2.h < r1.y);
}

struct PSX_Pos16 final
{
    s16 x, y;
};
ALIVE_ASSERT_SIZEOF(PSX_Pos16, 0x4);

struct PSX_DR_ENV final
{
    s32 field_0_tag;
    s32 field_4_code[15];
};
// TODO: Size

struct PSX_DRAWENV final
{
    PSX_RECT field_0_clip;
    s16 field_8_ofs[2];
    PSX_RECT field_C_tw;
    u8 field_14_tpage;
    s8 field_16_dtd;
    s8 field_17_dfe;
    s8 field_18_isbg;
    s8 field_19_r0;
    s8 field_1A_g0;
    s8 field_1B_b0;
    PSX_DR_ENV field_1C_dr_env;
};
// TODO: Size

struct PSX_DISPENV final
{
    PSX_RECT disp;
    PSX_RECT screen;
    s8 isinter;
    s8 isrgb24;
    s8 pad0;
    s8 pad1;
};
// TODO: Size

inline Bool32 PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    return (pRect1->x <= pRect2->w && pRect1->w >= pRect2->x && pRect1->y <= pRect2->h && pRect1->h >= pRect2->y);
}

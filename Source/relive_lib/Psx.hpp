#pragma once

#include "Types.hpp"
#include "AddPointer.hpp"
#include "Function.hpp"
#include "FixedPoint.hpp"

struct PSX_DISPENV;
struct PSX_RECT;

using TPsxEmuCallBack = AddPointer_t<s32(u32)>;

enum class VSyncMode
{
    UncappedFps,
    LimitTo30Fps,
};

void PSX_VSync(VSyncMode mode);
void PSX_PutDispEnv_4F5890();
void PSX_PutDispEnv_4F58E0();
void PSX_EMU_SetCallBack_4F9430(TPsxEmuCallBack fnPtr);

bool PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

bool PSX_Rects_overlap_4FA0B0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);

void PSX_Prevent_Rendering();

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


inline bool PSX_Rects_overlap_no_adjustment(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    return (pRect1->x <= pRect2->w && pRect1->w >= pRect2->x && pRect1->y <= pRect2->h && pRect1->h >= pRect2->y);
}

extern bool gTurnOffRendering;

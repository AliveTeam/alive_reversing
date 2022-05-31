#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"

namespace AO {

void Pal_Reset_4476C0(u16 a1, u16 a2);

s16 Pal_Allocate_4476F0(PSX_RECT* pRect, u32 paletteColorCount);

void Pal_Free_447870(PSX_Point xy, s16 palDepth);

void Pal_Copy_4479D0(PSX_Point point, s16 w, u16* pPalData, PSX_RECT* rect);

void Pal_Set_447990(PSX_Point xy, s16 w, const u8* palData, PSX_RECT* rect);

u32 Pal_Make_Colour_447950(u8 r, u8 g, u8 b, s16 bOpaque);

void Vram_reset_450840();

void Vram_free_450CE0(PSX_Point xy, PSX_Point wh);

void Vram_alloc_explicit_4507F0(s16 x, s16 y, s16 w, s16 h);

s16 vram_alloc_450B20(u16 width, s16 height, u16 colourDepth, PSX_RECT* pRect);


s32 vram_alloc_450860(s16 width, s16 height, PSX_RECT* pRect);


} // namespace AO

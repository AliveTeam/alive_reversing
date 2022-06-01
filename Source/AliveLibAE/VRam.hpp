#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"

ALIVE_VAR_EXTERN(u16, unused_5CC88C);

s16 Vram_alloc_4956C0(u16 width, s16 height, u16 colourDepth, PSX_RECT* pRect);

void Vram_init_495660();
void Vram_alloc_explicit_4955F0(s16 x, s16 y, s16 w, s16 h);
void Vram_free_495A60(PSX_Point xy, PSX_Point wh);

s16 Pal_Allocate_483110(PSX_RECT* pRect, u32 paletteColorCount);
void Pal_free_483390(PSX_Point xy, s16 palDepth);
Bool32 Vram_rects_overlap_4959E0(const PSX_RECT* pRect1, const PSX_RECT* pRect2);


void Pal_Area_Init_483080(s16 xpos, s16 ypos, u16 width, u16 height);

void Pal_Copy_483560(PSX_Point pPoint, s16 w, u16* pPalData, PSX_RECT* rect);


u32 Pal_Make_Colour_4834C0(u8 r, u8 g, u8 b, s16 bOpaque);

void Pal_Set_483510(PSX_Point xy, s16 w, const u8* palData, PSX_RECT* rect);

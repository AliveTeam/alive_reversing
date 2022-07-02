#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibAE/Psx.hpp"

ALIVE_VAR_EXTERN(u16, unused_5CC88C);

s16 Vram_alloc(u16 width, s16 height, u16 colourDepth, PSX_RECT* pRect);

void Vram_init();
void Vram_alloc(s16 x, s16 y, s16 w, s16 h);
void Vram_free(PSX_Point xy, PSX_Point wh);

s16 Pal_Allocate(PSX_RECT* pRect, u32 paletteColorCount);
void Pal_free(PSX_Point xy, s16 palDepth);
bool Vram_rects_overlap(const PSX_RECT* pRect1, const PSX_RECT* pRect2);


void Pal_Area_Init(s16 xpos, s16 ypos, u16 width, u16 height);

void Pal_Copy(PSX_Point pPoint, s16 w, u16* pPalData, PSX_RECT* rect);


u32 Pal_Make_Colour(u8 r, u8 g, u8 b, s16 bOpaque);

void Pal_Set(PSX_Point xy, s16 w, const u8* palData, PSX_RECT* rect);

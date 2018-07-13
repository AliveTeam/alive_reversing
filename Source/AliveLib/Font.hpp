#pragma once

#include "stdlib.hpp"
#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Game.hpp"
#include "Psx.hpp"
#include "Math.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PsxDisplay.hpp"

void Font_ForceLink();

struct Font_AtlasEntry
{
    unsigned __int8 field_0_x;
    unsigned __int8 field_1_y;
    unsigned __int8 field_2_width;
    unsigned __int8 field_3_height;
};
ALIVE_ASSERT_SIZEOF(Font_AtlasEntry, 0x4);

struct Font_Context
{
    PSX_RECT field_0_rect;
    Font_AtlasEntry *field_8_atlas_array;
    __int16 field_C_resource_id;
};
ALIVE_ASSERT_SIZEOF(Font_Context, 0x10);


struct Font
{
public:
    Font();
    Font(int maxCharLength, BYTE *palette, Font_Context *fontContext);

    EXPORT void ctor_433590(int maxCharLength, BYTE *palette, Font_Context *fontContext);
    EXPORT int DrawString_4337D0(int **ot, char *text, int x, __int16 y, char abr, int bSemiTrans, int a2, int otLayer, char r, char g, char b, int polyOffset, signed int scale, int a15, __int16 colorRandomRange);
    EXPORT int MeasureWidth_433700(char * text);
    EXPORT int MeasureWidth_4336C0(char *text, signed int fp_scale);
public:
    byte gap0[32];
    BYTE **field_20_fnt_poly_block_ptr;
    Poly_FT4 *field_24_fnt_poly_array;
    PSX_RECT field_28_palette_rect;
    int field_30_poly_count;
    Font_Context *field_34_font_context;
};
ALIVE_ASSERT_SIZEOF(Font, 0x38);

ALIVE_VAR_EXTERN(Font_Context, sFont1Context_5BC5C8);
ALIVE_VAR_EXTERN(Font_Context, sFont2Context_5BC5D8);

ALIVE_VAR_EXTERN(char, sDisableFontFlicker_5C9304);
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
#include "Primitives.hpp"
#include "ScreenManager.hpp"
#include "FixedPoint.hpp"

void Font_ForceLink();

EXPORT void CC static_font1context_init_433300();
EXPORT void CC static_font2context_init_433380();


struct File_Font
{
    __int16 field_0_width;
    __int16 field_2_height;
    __int16 field_4_color_depth;
    __int16 field_6_palette_size;
    byte field_8_palette[32];
    BYTE field_28_pixel_buffer[];
};
ALIVE_ASSERT_SIZEOF(File_Font, 0x28);
// Can vary in size

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

    EXPORT void LoadFontType_433400(int resourceID);
    EXPORT void dtor_433510();
    // Custom Functions to load fonts directly from raw files
    bool LoadFontTypeFromFile(const char * fontPath, const char * atlasPath, char * pPaletteOut);
    void LoadFontTypeCustom(File_Font * fontFile, Font_AtlasEntry * fontAtlas, char * pPaletteOut);
};
ALIVE_ASSERT_SIZEOF(Font_Context, 0x10);


struct Font
{
public:
    Font();
    Font(int maxCharLength, BYTE *palette, Font_Context *fontContext);

    EXPORT void ctor_433590(int maxCharLength, BYTE *palette, Font_Context *fontContext);
    EXPORT void dtor_433540();
    EXPORT int DrawString_4337D0(int **ot, const char *text, int x, __int16 y, char abr, int bSemiTrans, int a2, int otLayer, char r, char g, char b, int polyOffset, FP scale, int a15, __int16 colorRandomRange);
    EXPORT int MeasureWidth_433700(const char * text);
    EXPORT int MeasureWidth_4336C0(const char *text, FP scale);
    EXPORT int MeasureWidth_433630(unsigned char character);
    EXPORT const char * SliceText_433BD0(const char *text, int left, FP scale, int right);
public:
    byte gap0[32];
    BYTE **field_20_fnt_poly_block_ptr;
    Poly_FT4 *field_24_fnt_poly_array;
    PSX_RECT field_28_palette_rect;
    int field_30_poly_count;
    Font_Context *field_34_font_context;
};
ALIVE_ASSERT_SIZEOF(Font, 0x38);

ALIVE_VAR_EXTERN(__int16, sFontType2LoadCount_5BC5E8);
ALIVE_VAR_EXTERN(byte, sFontDrawScreenSpace_5CA4B4);

ALIVE_VAR_EXTERN(Font_Context, sFont1Context_5BC5C8);
ALIVE_VAR_EXTERN(Font_Context, sFont2Context_5BC5D8);

ALIVE_VAR_EXTERN(char, sDisableFontFlicker_5C9304);

ALIVE_ARY_EXTERN(Font_AtlasEntry, 169, sFont1Atlas_551D34);
ALIVE_ARY_EXTERN(Font_AtlasEntry, 169, sFont2Atlas_551FD8);
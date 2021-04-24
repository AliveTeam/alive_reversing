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
    s16 field_0_width;
    s16 field_2_height;
    s16 field_4_color_depth;
    s16 field_6_palette_size;
    BYTE field_8_palette[32];
    BYTE field_28_pixel_buffer[1];
};
//ALIVE_ASSERT_SIZEOF(File_Font, 0x28);
// Can vary in size

struct Font_AtlasEntry
{
    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_width;
    u8 field_3_height;
};
ALIVE_ASSERT_SIZEOF(Font_AtlasEntry, 0x4);

struct Font_Context
{
    PSX_RECT field_0_rect;
    Font_AtlasEntry *field_8_atlas_array;
    s16 field_C_resource_id;

    EXPORT void LoadFontType_433400(short resourceID);
    EXPORT void dtor_433510();
    // Custom Functions to load fonts directly from raw files
    bool LoadFontTypeFromFile(const char * fontPath, const char * atlasPath, char * pPaletteOut);
    void LoadFontTypeCustom(File_Font * fontFile, Font_AtlasEntry * fontAtlas, char * pPaletteOut);
    bool LoadFontTypeFromOddFont(const char * fontPath, char * pPaletteOut);
    bool LoadFontTypeFromOddFontMem(BYTE * data, char * pPaletteOut);
};
ALIVE_ASSERT_SIZEOF(Font_Context, 0x10);

// In a space space otherwise it conflicts with the Font structure in x11.h on Linux brought in by SDL2's header
namespace Alive
{
    struct Font
    {
    public:
        Font();
        Font(int maxCharLength, const BYTE *palette, Font_Context *fontContext);

        EXPORT void ctor_433590(int maxCharLength, const BYTE *palette, Font_Context *fontContext);
        EXPORT void dtor_433540();
        EXPORT int DrawString_4337D0(PrimHeader** ppOt, const char *text, int x, s16 y, TPageAbr abr, int bSemiTrans, int a2, Layer layer, BYTE r, BYTE g, BYTE b, int polyOffset, FP scale, int a15, s16 colorRandomRange);
        EXPORT int MeasureWidth_433700(const char * text);
        EXPORT int MeasureWidth_4336C0(const char *text, FP scale);
        EXPORT int MeasureWidth_433630(u8 character);
        EXPORT const char * SliceText_433BD0(const char *text, int left, FP scale, int right);
    public:
        BYTE gap0[32];
        BYTE **field_20_fnt_poly_block_ptr;
        Poly_FT4 *field_24_fnt_poly_array;
        PSX_RECT field_28_palette_rect;
        int field_30_poly_count;
        Font_Context *field_34_font_context;
    };
    ALIVE_ASSERT_SIZEOF(Font, 0x38);
}

ALIVE_VAR_EXTERN(s16, sFontType2LoadCount_5BC5E8);
ALIVE_VAR_EXTERN(BYTE, sFontDrawScreenSpace_5CA4B4);

ALIVE_VAR_EXTERN(Font_Context, sFont1Context_5BC5C8);
ALIVE_VAR_EXTERN(Font_Context, sFont2Context_5BC5D8);

ALIVE_VAR_EXTERN(char, sDisableFontFlicker_5C9304);

ALIVE_ARY_EXTERN(Font_AtlasEntry, 169, sFont1Atlas_551D34);
ALIVE_ARY_EXTERN(Font_AtlasEntry, 169, sFont2Atlas_551FD8);
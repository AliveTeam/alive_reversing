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


struct File_Font final
{
    s16 field_0_width;
    s16 field_2_height;
    s16 field_4_color_depth;
    s16 field_6_palette_size;
    u8 field_8_palette[32];
    u8 field_28_pixel_buffer[1];
};
//ALIVE_ASSERT_SIZEOF(File_Font, 0x28);
// Can vary in size

struct Font_AtlasEntry final
{
    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_width;
    u8 field_3_height;
};
ALIVE_ASSERT_SIZEOF(Font_AtlasEntry, 0x4);

struct Font_Context final
{
    PSX_RECT field_0_rect;
    Font_AtlasEntry* field_8_atlas_array;
    s16 field_C_resource_id;

    EXPORT void LoadFontType_433400(s16 resourceID);
    EXPORT void dtor_433510();
    // Custom Functions to load fonts directly from raw files
    bool LoadFontTypeFromFile(const char_type* fontPath, const char_type* atlasPath, s8* pPaletteOut);
    void LoadFontTypeCustom(File_Font* fontFile, Font_AtlasEntry* fontAtlas, s8* pPaletteOut);
    bool LoadFontTypeFromOddFont(const char_type* fontPath, s8* pPaletteOut);
    bool LoadFontTypeFromOddFontMem(u8* data, s8* pPaletteOut);
};
ALIVE_ASSERT_SIZEOF(Font_Context, 0x10);

// In a space space otherwise it conflicts with the Font structure in x11.h on Linux brought in by SDL2's header
namespace Alive {
struct Font final
{
public:
    Font();
    Font(s32 maxCharLength, const u8* palette, Font_Context* fontContext);

    EXPORT void ctor_433590(s32 maxCharLength, const u8* palette, Font_Context* fontContext);
    EXPORT void dtor_433540();
    EXPORT s32 DrawString_4337D0(PrimHeader** ppOt, const char_type* text, s32 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 a2, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 a15, s16 colorRandomRange);
    EXPORT s32 MeasureWidth_433700(const char_type* text);
    EXPORT s32 MeasureWidth_4336C0(const char_type* text, FP scale);
    EXPORT s32 MeasureWidth_433630(char_type character);
    EXPORT const char_type* SliceText_433BD0(const char_type* text, s32 left, FP scale, s32 right);

public:
    u8 gap0[32];
    u8** field_20_fnt_poly_block_ptr;
    Poly_FT4* field_24_fnt_poly_array;
    PSX_RECT field_28_palette_rect;
    s32 field_30_poly_count;
    Font_Context* field_34_font_context;
};
ALIVE_ASSERT_SIZEOF(Font, 0x38);
} // namespace Alive

ALIVE_VAR_EXTERN(s16, sFontType2LoadCount_5BC5E8);
ALIVE_VAR_EXTERN(u8, sFontDrawScreenSpace_5CA4B4);

ALIVE_VAR_EXTERN(Font_Context, sFont1Context_5BC5C8);
ALIVE_VAR_EXTERN(Font_Context, sFont2Context_5BC5D8);

ALIVE_VAR_EXTERN(s8, sDisableFontFlicker_5C9304);

ALIVE_ARY_EXTERN(Font_AtlasEntry, 169, sFont1Atlas_551D34);
ALIVE_ARY_EXTERN(Font_AtlasEntry, 169, sFont2Atlas_551FD8);
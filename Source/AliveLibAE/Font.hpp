#pragma once

#include "stdlib.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Animation.hpp"
#include "Game.hpp"
#include "Psx.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "FixedPoint.hpp"

void static_font1context_init_433300();
void static_font2context_init_433380();


struct File_Font final
{
    s16 mWidth;
    s16 mHeight;
    s16 field_4_color_depth;
    s16 field_6_palette_size;
    u8 field_8_palette[32];
    u8 field_28_pixel_buffer[1];
};
//ALIVE_ASSERT_SIZEOF(File_Font, 0x28);
// Can vary in size

struct Font_AtlasEntry final
{
    u8 x;
    u8 field_1_y;
    u8 field_2_width;
    u8 field_3_height;
};
ALIVE_ASSERT_SIZEOF(Font_AtlasEntry, 0x4);

struct Font_Context final
{
    PSX_RECT mRect;
    Font_AtlasEntry* field_8_atlas_array;
    s16 field_C_resource_id;

    void LoadFontType_433400(s16 resourceID);
    void dtor_433510();
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

    void ctor_433590(s32 maxCharLength, const u8* palette, Font_Context* fontContext);
    void dtor_433540();
    s32 DrawString_4337D0(PrimHeader** ppOt, const char_type* text, s32 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 a2, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 a15, s16 colorRandomRange);
    s32 MeasureTextWidth(const char_type* text);
    s32 MeasureScaledTextWidth(const char_type* text, FP scale);
    s32 MeasureCharacterWidth(char_type character);
    const char_type* SliceText(const char_type* text, s32 left, FP scale, s32 right);

public:
    u8 gap0[32] = {};
    u8** field_20_fnt_poly_block_ptr = nullptr;
    Poly_FT4* field_24_fnt_poly_array = nullptr;
    PSX_RECT field_28_palette_rect = {};
    s32 field_30_poly_count = 0;
    Font_Context* field_34_font_context = nullptr;
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
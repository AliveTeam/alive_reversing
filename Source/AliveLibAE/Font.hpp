#pragma once

#include "stdlib.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "Game.hpp"
#include "Psx.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "FixedPoint.hpp"

struct File_Font final
{
    s16 mWidth;
    s16 mHeight;
    s16 field_4_color_depth;
    s16 field_6_palette_size;
    u16 field_8_palette[32/2];
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
    Font_AtlasEntry* field_8_atlas_array;
    FontResource field_C_resource_id;

    void LoadFontType_433400(FontType resourceID);

};
ALIVE_ASSERT_SIZEOF(Font_Context, 0x10);

// In a space space otherwise it conflicts with the Font structure in x11.h on Linux brought in by SDL2's header
namespace Alive {
struct Font final
{
public:
    Font();
    Font(s32 maxCharLength, const PalResource& pal, Font_Context* fontContext);

    void ctor_433590(s32 maxCharLength, const PalResource& pal, Font_Context* fontContext);
    void dtor_433540();
    s32 DrawString_4337D0(PrimHeader** ppOt, const char_type* text, s32 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 a2, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 a15, s16 colorRandomRange);
    s32 MeasureTextWidth(const char_type* text);
    s32 MeasureScaledTextWidth(const char_type* text, FP scale);
    s32 MeasureCharacterWidth(char_type character);
    const char_type* SliceText(const char_type* text, s32 left, FP scale, s32 right);

public:
    Poly_FT4* field_24_fnt_poly_array = nullptr;
    s32 field_30_poly_count = 0;
    Font_Context* field_34_font_context = nullptr;
};
ALIVE_ASSERT_SIZEOF(Font, 0x38);
} // namespace Alive

extern u8 sFontDrawScreenSpace_5CA4B4;

extern s8 sDisableFontFlicker_5C9304;

extern Font_AtlasEntry sFont1Atlas_551D34[169];
extern Font_AtlasEntry sFont2Atlas_551FD8[169];
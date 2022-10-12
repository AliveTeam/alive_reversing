#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "Psx.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"

struct PrimHeader;

namespace AO {


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
    std::string mCharName;
};

class FontContext
{
public:
    void LoadFontType(FontType resourceID);


    const Font_AtlasEntry* field_8_atlas_array = nullptr;
    FontResource field_C_resource_id;
};


class AliveFont
{
public:
    void Load(s32 maxCharLength, const PalResource& pal, FontContext* fontContext);
    ~AliveFont();

    u32 MeasureTextWidth(const char_type* text);
    s32 MeasureCharacterWidth(char_type character);
    s32 MeasureScaledTextWidth(const char_type* text, FP scale);

    s32 DrawString(PrimHeader** ppOt, const char_type* text, s16 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 a2, Layer otLayer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 a15, s32 colorRandomRange);

    const char_type* SliceText(const char_type* text, s32 left, FP scale, s32 right);

    Poly_FT4* field_24_fnt_poly_array = nullptr;
    s32 field_30_poly_count = 0;
    FontContext* field_34_font_context = nullptr;
};

extern s16 sDisableFontFlicker_5080E4;
extern u8 sFontDrawScreenSpace_508BF4;

} // namespace AO

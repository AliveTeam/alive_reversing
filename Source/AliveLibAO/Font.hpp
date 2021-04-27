#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint_common.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"

struct PrimHeader;

namespace AO {


struct File_Font
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

struct Font_AtlasEntry
{
    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_width;
    u8 field_3_height;
};
ALIVE_ASSERT_SIZEOF(Font_AtlasEntry, 0x4);

class FontContext
{
public:
    EXPORT static void CC static_ctor_41C010();

    EXPORT static void CC static_dtor_41C020();

    EXPORT void LoadFontType_41C040(s16 resourceID);


    EXPORT void dtor_41C110();


    PSX_RECT field_0_rect;
    const Font_AtlasEntry* field_8_atlas_array;
    s16 field_C_resource_id;
    s16 field_E_pad;
};


class AliveFont
{
public:
    EXPORT AliveFont* ctor_41C170(s32 maxCharLength, const u8* palette, FontContext* fontContext);

    EXPORT u32 MeasureWidth_41C2B0(const s8* character);

    EXPORT s32 MeasureWidth_41C200(s8 character);

    EXPORT s32 MeasureWidth_41C280(const s8* text, FP scale);

    EXPORT s32 DrawString_41C360(PrimHeader** ppOt, const s8* text, s16 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 a2, Layer otLayer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 a15, s32 colorRandomRange);

    EXPORT void dtor_41C130();

    EXPORT const s8* SliceText_41C6C0(const s8* text, s32 left, FP scale, s32 right);

    u8 gap0[32];
    u8** field_20_fnt_poly_block_ptr;
    Poly_FT4* field_24_fnt_poly_array;
    PSX_RECT field_28_palette_rect;
    s32 field_30_poly_count;
    FontContext* field_34_font_context;
};
ALIVE_ASSERT_SIZEOF(AliveFont, 0x38);

ALIVE_VAR_EXTERN(FontContext, sFontContext_4FFD68);
ALIVE_VAR_EXTERN(s16, sDisableFontFlicker_5080E4);
ALIVE_VAR_EXTERN(u8, sFontDrawScreenSpace_508BF4);

} // namespace AO

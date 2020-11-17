#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint_common.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"

namespace AO {


struct File_Font
{
    __int16 field_0_width;
    __int16 field_2_height;
    __int16 field_4_color_depth;
    __int16 field_6_palette_size;
    byte field_8_palette[32];
    BYTE field_28_pixel_buffer[1];
};
//ALIVE_ASSERT_SIZEOF(File_Font, 0x28);
// Can vary in size

struct Font_AtlasEntry
{
    unsigned __int8 field_0_x;
    unsigned __int8 field_1_y;
    unsigned __int8 field_2_width;
    unsigned __int8 field_3_height;
};
ALIVE_ASSERT_SIZEOF(Font_AtlasEntry, 0x4);

class FontContext
{
public:
    EXPORT static void CC static_ctor_41C010();

    EXPORT static void CC static_dtor_41C020();

    EXPORT void LoadFontType_41C040(short resourceID);


    EXPORT void dtor_41C110();


    PSX_RECT field_0_rect;
    const Font_AtlasEntry* field_8_atlas_array;
    __int16 field_C_resource_id;
    __int16 field_E_pad;
};


class AliveFont
{
public:
    EXPORT AliveFont* ctor_41C170(int maxCharLength, const BYTE* palette, FontContext* fontContext);

    EXPORT unsigned int MeasureWidth_41C2B0(const char* /*pStr*/);

    EXPORT int MeasureWidth_41C200(char a2);

    EXPORT int MeasureWidth_41C280(const char* text, FP scale);

    EXPORT int DrawString_41C360(int** ot, const char* text, __int16 x, __int16 y, char abr, int bSemiTrans, int a2, int otLayer, unsigned char r, unsigned char g, unsigned char b, int polyOffset, FP scale, int a15, int colorRandomRange);

    EXPORT void dtor_41C130();

    EXPORT const char* SliceText_41C6C0(const char* text, int left, FP scale, int right);

    byte gap0[32];
    BYTE** field_20_fnt_poly_block_ptr;
    Poly_FT4* field_24_fnt_poly_array;
    PSX_RECT field_28_palette_rect;
    int field_30_poly_count;
    FontContext* field_34_font_context;
};
ALIVE_ASSERT_SIZEOF(AliveFont, 0x38);

ALIVE_VAR_EXTERN(FontContext, sFontContext_4FFD68);
ALIVE_VAR_EXTERN(short, sDisableFontFlicker_5080E4);
ALIVE_VAR_EXTERN(BYTE, sFontDrawScreenSpace_508BF4);

}

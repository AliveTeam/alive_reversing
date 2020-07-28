#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint_common.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"

START_NS_AO

class FontContext
{
public:
    EXPORT __int16 LoadFontType_41C040(int);


    EXPORT void dtor_41C110();


    __int16 field_0;
    __int16 field_2;
    int field_4;
    int field_8;
    __int16 field_C_resource_id;
    __int16 field_E;
};


class AliveFont
{
public:
    EXPORT AliveFont* ctor_41C170(int maxCharLength, const BYTE* palette, FontContext* fontContext);

    EXPORT unsigned int MeasureWidth_41C2B0(const char* /*pStr*/);

    EXPORT int MeasureWidth_41C200(char a2);

    EXPORT int DrawString_41C360(int**, const char*, __int16, __int16, int, int, int, int, unsigned char, unsigned char, unsigned char, int, FP, int, int);

    EXPORT void dtor_41C130();

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

END_NS_AO

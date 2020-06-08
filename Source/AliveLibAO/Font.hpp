#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint_common.hpp"

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
    __int16 field_C;
    __int16 field_E;
};


class AliveFont
{
public:
    EXPORT AliveFont* ctor_41C170(int /*a2*/, BYTE* /*pPal*/, FontContext* /*pContext*/);

    EXPORT unsigned int MeasureWidth_41C2B0(const char* /*pStr*/);

    EXPORT int DrawString_41C360(int**, const char*, __int16, __int16, int, int, int, int, unsigned char, unsigned char, unsigned char, int, FP, int, int);

    EXPORT void dtor_41C130();


    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    __int16 field_28;
    __int16 field_2A;
    int field_2C;
    int field_30;
    FontContext* field_34_pFontContext;
};

END_NS_AO

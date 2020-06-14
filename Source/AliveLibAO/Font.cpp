#include "stdafx_ao.h"
#include "Font.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FFD68, FontContext, sFontContext_4FFD68, {});

EXPORT __int16 FontContext::LoadFontType_41C040(int)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void FontContext::dtor_41C110()
{
    NOT_IMPLEMENTED();
}

EXPORT AliveFont* AliveFont::ctor_41C170(int /*a2*/, BYTE* /*pPal*/, FontContext* /*pContext*/)
{
    NOT_IMPLEMENTED();
    return this;
}

EXPORT unsigned int AliveFont::MeasureWidth_41C2B0(const char* /*pStr*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int AliveFont::DrawString_41C360(int**, const char*, __int16, __int16, int, int, int, int, unsigned char, unsigned char, unsigned char, int, FP, int, int)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void AliveFont::dtor_41C130()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

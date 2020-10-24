#include "stdafx_ao.h"
#include "Font.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "VRam.hpp"
#include "FixedPoint.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FFD68, FontContext, sFontContext_4FFD68, {});

ALIVE_VAR(1, 0x5080E4, short, sDisableFontFlicker_5080E4, 0);
ALIVE_VAR(1, 0x508BF4, BYTE, sFontDrawScreenSpace_508BF4, 0);


void CC FontContext::static_ctor_41C010()
{
    atexit(static_dtor_41C020);
}

void CC FontContext::static_dtor_41C020()
{
    sFontContext_4FFD68.dtor_41C110();
}

EXPORT __int16 FontContext::LoadFontType_41C040(int /*resourceID*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void FontContext::dtor_41C110()
{
    if (field_0_rect.x > 0)
    {
        Vram_free_450CE0(
            { field_0_rect.x, field_0_rect.y },
            { field_0_rect.w, field_0_rect.h });
    }
}

AliveFont* AliveFont::ctor_41C170(int maxCharLength, const BYTE* palette, FontContext* fontContext)
{
    field_34_font_context = fontContext;
    Pal_Allocate_4476F0(&field_28_palette_rect, 16u);
    PSX_RECT rect = { field_28_palette_rect.x, field_28_palette_rect.y, 16, 1 };
    PSX_LoadImage16_4962A0(&rect, palette);
    field_30_poly_count = maxCharLength;
    field_20_fnt_poly_block_ptr = ResourceManager::Allocate_New_Locked_Resource_454F80(
        ResourceManager::Resource_FntP,
        fontContext->field_C_resource_id,
        sizeof(Poly_FT4) * 2 * maxCharLength);
    field_24_fnt_poly_array = reinterpret_cast<Poly_FT4*>(*field_20_fnt_poly_block_ptr);
    return this;
}

EXPORT unsigned int AliveFont::MeasureWidth_41C2B0(const char* /*pStr*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int AliveFont::MeasureWidth_41C200(char /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


int AliveFont::MeasureWidth_41C280(const char* text, FP scale)
{
    const FP width = FP_FromInteger(MeasureWidth_41C2B0(text));
    return FP_GetExponent((width * scale) + FP_FromDouble(0.5));
}

EXPORT int AliveFont::DrawString_41C360(int**, const char*, __int16, __int16, int, int, int, int, unsigned char, unsigned char, unsigned char, int, FP, int, int)
{
    NOT_IMPLEMENTED();
    return 0;
}

void AliveFont::dtor_41C130()
{
    PSX_Point palPoint = { field_28_palette_rect.x, field_28_palette_rect.y };
    Pal_Free_447870(palPoint, field_28_palette_rect.w);
    field_28_palette_rect.x = 0;

    ResourceManager::FreeResource_455550(field_20_fnt_poly_block_ptr);
}

const char* AliveFont::SliceText_41C6C0(const char* /*text*/, int /*left*/, FP /*scale*/, int /*right*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

END_NS_AO

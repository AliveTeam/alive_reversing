#include "stdafx_ao.h"
#include "Function.hpp"
#include "ScreenWave.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Primitives.hpp"
#include "Math.hpp"

void Forcelink_ScreenWave()
{ }

namespace AO {

static constexpr s32 kMaxUVCount = 32;
static constexpr s32 kMaxPolygons = 4;

struct ScreenWave_Data final
{
    FP_Point field_0_uv1[kMaxUVCount][kMaxPolygons + 1];
    FP_Point field_500_uv2[kMaxUVCount][kMaxPolygons + 1];
    FP_Point field_A00_xy[kMaxUVCount];
    Poly_FT4 field_B00_poly[2][kMaxUVCount][kMaxPolygons];
};
ALIVE_ASSERT_SIZEOF(ScreenWave_Data, 0x3700);

#undef min
#undef max

ScreenWave* ScreenWave::ctor_462A70(FP xpos, FP ypos, Layer layer, FP width, FP speed, s16 radius)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC9A0);
    field_4_typeId = Types::eScreenWave_83;
    gObjList_drawables_504618->Push_Back(this);

    // TODO: Using frame counter as an ID seems extremely dangerous due to id collision risk!
    field_14_ppRes = reinterpret_cast<ScreenWave_Data**>(ResourceManager::Allocate_New_Locked_Resource_454F80('evaW', gnFrameCount_507670, sizeof(ScreenWave_Data)));
    if (!field_14_ppRes)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return this;
    }

    field_10_layer = layer;
    field_1C_ypos = ypos;
    field_30_speed = speed;

    field_3C_path = gMap_507BA8.field_2_current_path;
    field_3A_level = gMap_507BA8.field_0_current_level;

    field_6_flags.Set(Options::eDrawable_Bit4);
    field_18_xpos = xpos;

    field_2C = FP_FromInteger(0);

    field_36_screen_xpos = FP_GetExponent(xpos) - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
    field_38_screen_ypos = FP_GetExponent(ypos) - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos));

    // TODO: This needs to be checked, my brain melted halfway
    const s16 v1 = static_cast<s16>(std::abs(field_36_screen_xpos) + std::abs(field_38_screen_ypos));
    const s16 v2 = static_cast<s16>(std::abs(field_36_screen_xpos - gPsxDisplay_504C78.field_0_width) + std::abs(field_38_screen_ypos));

    const s16 v3 = static_cast<s16>(std::abs(field_36_screen_xpos) + std::abs(field_38_screen_ypos - gPsxDisplay_504C78.field_2_height));
    const s16 v4 = static_cast<s16>(std::abs(field_36_screen_xpos - gPsxDisplay_504C78.field_0_width) + std::abs(field_38_screen_ypos - gPsxDisplay_504C78.field_2_height));

    field_34_max_radius = static_cast<s16>(std::max(std::max(v1, v2), std::max(v3, v4)));

    if (radius > 0)
    {
        field_34_max_radius = std::min(field_34_max_radius, radius);
    }

    field_20_fp1 = FP_FromInteger(1);
    field_24_fp1 = FP_FromInteger(1);

    const FP uv1_off = (width / FP_FromInteger(4));
    const FP uv2_off = (width / FP_FromInteger(2));

    ScreenWave_Data* pData = *field_14_ppRes;

    u8 ang2 = 0;
    for (s32 i = 0; i < 32; i++)
    {
        u8 ang = 128;
        for (s32 j = 0; j < 5; j++)
        {
            pData->field_0_uv1[i][j].field_0_x = ((FP_FromInteger(j) * uv1_off) * Math_Sine_451110(ang2));
            pData->field_0_uv1[i][j].field_4_y = ((FP_FromInteger(j) * uv1_off) * Math_Cosine_4510A0(ang2));

            pData->field_500_uv2[i][j].field_0_x = (((Math_Sine_451110(ang) * uv2_off) + uv2_off) * Math_Sine_451110(ang2));
            pData->field_500_uv2[i][j].field_4_y = (((Math_Sine_451110(ang) * uv2_off) + uv2_off) * Math_Cosine_4510A0(ang2));

            ang -= 32;
        }

        pData->field_A00_xy[i].field_0_x = (Math_Sine_451110(ang2) * speed);
        pData->field_A00_xy[i].field_4_y = (Math_Cosine_4510A0(ang2) * speed);
        ang2 += 8;
    }

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < 32; j++)
        {
            for (s32 k = 0; k < 4; k++)
            {
                PolyFT4_Init(&pData->field_B00_poly[i][j][k]);
                Poly_Set_SemiTrans_498A40(&pData->field_B00_poly[i][j][k].mBase.header, 0);
            }
        }
    }

    return this;
}

BaseGameObject* ScreenWave::dtor_463010()
{
    SetVTable(this, 0x4BC9A0);
    gObjList_drawables_504618->Remove_Item(this);
    ResourceManager::FreeResource_455550(reinterpret_cast<u8**>(field_14_ppRes));
    return dtor_487DF0();
}

BaseGameObject* ScreenWave::VDestructor(s32 flags)
{
    return Vdtor_463860(flags);
}

ScreenWave* ScreenWave::Vdtor_463860(s32 flags)
{
    dtor_463010();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void ScreenWave::VScreenChanged()
{
    VScreenChanged_463110();
}

void ScreenWave::VScreenChanged_463110()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ScreenWave::VUpdate()
{
    VUpdate_463080();
}

void ScreenWave::VUpdate_463080()
{
    if (FP_GetExponent(field_2C) <= field_34_max_radius)
    {
        field_2C += field_30_speed;

        auto pData = reinterpret_cast<ScreenWave_Data*>(*field_14_ppRes);

        for (s32 i = 0; i < 32; i++)
        {
            for (s32 j = 0; j < 5; j++)
            {
                pData->field_0_uv1[i][j].field_0_x += pData->field_A00_xy[i].field_0_x;
                pData->field_0_uv1[i][j].field_4_y += pData->field_A00_xy[i].field_4_y;

                pData->field_500_uv2[i][j].field_0_x += pData->field_A00_xy[i].field_0_x;
                pData->field_500_uv2[i][j].field_4_y += pData->field_A00_xy[i].field_4_y;
            }
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ScreenWave::VRender(PrimHeader** ppOt)
{
    VRender_463130(ppOt);
}

void ScreenWave::VRender_463130(PrimHeader** ppOt)
{
    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_3A_level,
            field_3C_path,
            field_18_xpos,
            field_1C_ypos,
            0))
    {
        return;
    }

    ScreenWave_Data* pScreenWaveData = *field_14_ppRes;

    const PSX_Point displaySize = {
        static_cast<s16>(gPsxDisplay_504C78.field_0_width),
        static_cast<s16>(gPsxDisplay_504C78.field_2_height)};

    PSX_RECT clearRectSize = {};

    clearRectSize.x = 0;
    clearRectSize.y = 0;
    clearRectSize.w = displaySize.field_0_x;
    clearRectSize.h = displaySize.field_2_y;

    for (s32 i = 0; i < kMaxUVCount; i++)
    {
        const s32 i_inc = i + 1;
        for (s32 j = 0; j < kMaxPolygons; j++)
        {
            const s16 x0 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j].field_0_x);
            const s16 y0 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j].field_4_y);
            const s16 x1 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j + 1].field_0_x);
            const s16 y1 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j + 1].field_4_y);

            const s32 next_i = i_inc == kMaxUVCount ? 0 : i_inc;

            const s16 x2 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j].field_0_x);
            const s16 y2 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j].field_4_y);
            const s16 x3 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j + 1].field_0_x);
            const s16 y3 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j + 1].field_4_y);

            const s16 minX = static_cast<s16>(std::min(
                std::min(x0, x1),
                std::min(x2, x3)));

            const s16 minY = static_cast<s16>(std::min(
                std::min(y0, y1),
                std::min(y2, y3)));

            const s16 maxX = static_cast<s16>(std::max(
                std::max(x0, x1),
                std::max(x2, x3)));

            const s16 maxY = static_cast<s16>(std::max(
                std::max(y0, y1),
                std::max(y2, y3)));

            if (
                maxX >= 0 && maxY >= 0 && minX < displaySize.field_0_x && minY < displaySize.field_2_y)
            {
                Poly_FT4* pPoly = &pScreenWaveData->field_B00_poly[gPsxDisplay_504C78.field_A_buffer_index][i][j];

                SetXY0(pPoly,
                       static_cast<s16>(PsxToPCX(x0, 11)),
                       static_cast<s16>(y0));
                SetXY1(pPoly,
                       static_cast<s16>(PsxToPCX(x1, 11)),
                       static_cast<s16>(y1));
                SetXY2(pPoly,
                       static_cast<s16>(PsxToPCX(x2, 11)),
                       static_cast<s16>(y2));
                SetXY3(pPoly,
                       static_cast<s16>(PsxToPCX(x3, 11)),
                       static_cast<s16>(y3));

                s16 u0 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j].field_0_x), 11));
                const s16 v0 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j].field_4_y);

                s16 u1 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j + 1].field_0_x), 11));
                const s16 v1 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j + 1].field_4_y);

                s16 u2 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j].field_0_x), 11));
                const s16 v2 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j].field_4_y);

                s16 u3 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j + 1].field_0_x), 11));
                const s16 v3 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j + 1].field_4_y);

                const s16 minU = std::min(
                    std::min(u1, u0),
                    std::min(u2, u3));
                const s16 minU_capped = minU & 0xffC0;

                u0 -= minU_capped;
                u1 -= minU_capped;
                u2 -= minU_capped;
                u3 -= minU_capped;

                SetTPage(pPoly, static_cast<s16>(PSX_getTPage_4965D0(
                                    TPageMode::e16Bit_2,
                                    TPageAbr::eBlend_0,
                                    static_cast<s16>(minU_capped),
                                    0)));

                SetUV0(pPoly,
                       static_cast<u8>(u0),
                       static_cast<u8>(v0));
                SetUV1(pPoly,
                       static_cast<u8>(u1),
                       static_cast<u8>(v1));
                SetUV2(pPoly,
                       static_cast<u8>(u2),
                       static_cast<u8>(v2));
                SetUV3(pPoly,
                       static_cast<u8>(u3),
                       static_cast<u8>(v3));

                Poly_Set_SemiTrans_498A40(&pPoly->mBase.header, 0);
                Poly_Set_Blending_498A00(&pPoly->mBase.header, 1);

                SetPrimExtraPointerHack(pPoly, nullptr);

                OrderingTable_Add_498A80(OtLayer(ppOt, field_10_layer), &pPoly->mBase.header);

                clearRectSize.x = std::min(clearRectSize.x, minX);
                clearRectSize.y = std::min(clearRectSize.y, minY);
                clearRectSize.w = std::max(clearRectSize.w, maxX);
                clearRectSize.h = std::max(clearRectSize.h, maxY);
            }
        }
        if (clearRectSize.y <= clearRectSize.h && clearRectSize.x <= clearRectSize.w)
        {
            pScreenManager_4FF7C8->InvalidateRect_406E40(
                clearRectSize.x,
                clearRectSize.y,
                clearRectSize.w,
                clearRectSize.h,
                pScreenManager_4FF7C8->field_2E_idx);
        }
    }
}

} // namespace AO

#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "ScreenWave.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Math.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

#undef min
#undef max

#include <algorithm>

namespace AO {

static constexpr s32 kMaxUVCount = 32;
static constexpr s32 kMaxPolygons = 4;

struct ScreenWave_Data final
{
    FP_Point field_0_uv1[kMaxUVCount][kMaxPolygons + 1];
    FP_Point field_500_uv2[kMaxUVCount][kMaxPolygons + 1];
    FP_Point field_A00_xy[kMaxUVCount];
    Poly_FT4 field_B00_poly[kMaxUVCount][kMaxPolygons];
};
ALIVE_ASSERT_SIZEOF(ScreenWave_Data, 0x3700);

ScreenWave::ScreenWave(FP xpos, FP ypos, Layer layer, FP width, FP speed, s32 radius)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eScreenWave);
    gObjListDrawables->Push_Back(this);

    // TODO: Using frame counter as an ID seems extremely dangerous due to id collision risk!
    field_14_ppRes = relive_new ScreenWave_Data();
    if (!field_14_ppRes)
    {
        SetDead(true);
        return;
    }

    field_10_layer = layer;
    field_1C_ypos = ypos;
    field_30_speed = speed;

    field_3C_path = gMap.mCurrentPath;
    field_3A_level = gMap.mCurrentLevel;

    SetDrawable(true);
    field_18_xpos = xpos;

    field_2C = FP_FromInteger(0);

    field_36_screen_xpos = FP_GetExponent(xpos) - FP_GetExponent(gScreenManager->CamXPos());
    field_38_screen_ypos = FP_GetExponent(ypos) - FP_GetExponent(gScreenManager->CamYPos());

    // TODO: This needs to be checked, my brain melted halfway
    const s16 v1 = static_cast<s16>(std::abs(field_36_screen_xpos) + std::abs(field_38_screen_ypos));
    const s16 v2 = static_cast<s16>(std::abs(field_36_screen_xpos - gPsxDisplay.mWidth) + std::abs(field_38_screen_ypos));

    const s16 v3 = static_cast<s16>(std::abs(field_36_screen_xpos) + std::abs(field_38_screen_ypos - gPsxDisplay.mHeight));
    const s16 v4 = static_cast<s16>(std::abs(field_36_screen_xpos - gPsxDisplay.mWidth) + std::abs(field_38_screen_ypos - gPsxDisplay.mHeight));

    field_34_max_radius = static_cast<s16>(std::max(std::max(v1, v2), std::max(v3, v4)));

    if (radius > 0)
    {
        field_34_max_radius = std::min(field_34_max_radius, static_cast<s16>(radius));
    }

    field_20_fp1 = FP_FromInteger(1);
    field_24_fp1 = FP_FromInteger(1);

    const FP uv1_off = (width / FP_FromInteger(4));
    const FP uv2_off = (width / FP_FromInteger(2));

    ScreenWave_Data* pData = field_14_ppRes;

    u8 ang2 = 0;
    for (s32 i = 0; i < 32; i++)
    {
        u8 ang = 128;
        for (s32 j = 0; j < 5; j++)
        {
            pData->field_0_uv1[i][j].x = ((FP_FromInteger(j) * uv1_off) * Math_Sine(ang2));
            pData->field_0_uv1[i][j].y = ((FP_FromInteger(j) * uv1_off) * Math_Cosine(ang2));

            pData->field_500_uv2[i][j].x = (((Math_Sine(ang) * uv2_off) + uv2_off) * Math_Sine(ang2));
            pData->field_500_uv2[i][j].y = (((Math_Sine(ang) * uv2_off) + uv2_off) * Math_Cosine(ang2));

            ang -= 32;
        }

        pData->field_A00_xy[i].x = (Math_Sine(ang2) * speed);
        pData->field_A00_xy[i].y = (Math_Cosine(ang2) * speed);
        ang2 += 8;
    }

    for (s32 j = 0; j < 32; j++)
    {
        for (s32 k = 0; k < 4; k++)
        {
            pData->field_B00_poly[j][k].SetSemiTransparent(false);
        }
    }
}

ScreenWave::~ScreenWave()
{
    gObjListDrawables->Remove_Item(this);
    relive_delete field_14_ppRes;
}

void ScreenWave::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void ScreenWave::VUpdate()
{
    if (FP_GetExponent(field_2C) <= field_34_max_radius)
    {
        field_2C += field_30_speed;

        auto pData = field_14_ppRes;

        for (s32 i = 0; i < 32; i++)
        {
            for (s32 j = 0; j < 5; j++)
            {
                pData->field_0_uv1[i][j].x += pData->field_A00_xy[i].x;
                pData->field_0_uv1[i][j].y += pData->field_A00_xy[i].y;

                pData->field_500_uv2[i][j].x += pData->field_A00_xy[i].x;
                pData->field_500_uv2[i][j].y += pData->field_A00_xy[i].y;
            }
        }
    }
    else
    {
        SetDead(true);
    }
}

void ScreenWave::VRender(OrderingTable& ot)
{
    if (!gMap.Is_Point_In_Current_Camera(
            field_3A_level,
            field_3C_path,
            field_18_xpos,
            field_1C_ypos,
            0))
    {
        return;
    }

    ScreenWave_Data* pScreenWaveData = field_14_ppRes;

    const PSX_Point displaySize = {
        static_cast<s16>(gPsxDisplay.mWidth),
        static_cast<s16>(gPsxDisplay.mHeight)};

    for (s32 i = 0; i < kMaxUVCount; i++)
    {
        const s32 i_inc = i + 1;
        for (s32 j = 0; j < kMaxPolygons; j++)
        {
            const s16 x0 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j].x);
            const s16 y0 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j].y);
            const s16 x1 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j + 1].x);
            const s16 y1 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[i][j + 1].y);

            const s32 next_i = i_inc == kMaxUVCount ? 0 : i_inc;

            const s16 x2 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j].x);
            const s16 y2 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j].y);
            const s16 x3 = field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j + 1].x);
            const s16 y3 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_500_uv2[next_i][j + 1].y);

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
                maxX >= 0 && maxY >= 0 && minX < displaySize.x && minY < displaySize.y)
            {
                Poly_FT4* pPoly = &pScreenWaveData->field_B00_poly[i][j];

                pPoly->SetXY0(
                       static_cast<s16>(PsxToPCX(x0, 11)),
                       static_cast<s16>(y0));
                pPoly->SetXY1(
                       static_cast<s16>(PsxToPCX(x1, 11)),
                       static_cast<s16>(y1));
                pPoly->SetXY2(
                       static_cast<s16>(PsxToPCX(x2, 11)),
                       static_cast<s16>(y2));
                pPoly->SetXY3(
                       static_cast<s16>(PsxToPCX(x3, 11)),
                       static_cast<s16>(y3));

                s16 u0 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j].x), 11));
                const s16 v0 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j].y);

                s16 u1 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j + 1].x), 11));
                const s16 v1 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[i][j + 1].y);

                s16 u2 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j].x), 11));
                const s16 v2 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j].y);

                s16 u3 = static_cast<s16>(PsxToPCX(field_36_screen_xpos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j + 1].x), 11));
                const s16 v3 = field_38_screen_ypos + FP_GetExponent(pScreenWaveData->field_0_uv1[next_i][j + 1].y);

                const s16 minU = std::min(
                    std::min(u1, u0),
                    std::min(u2, u3));
                const s16 minU_capped = minU & 0xffC0;

                u0 -= minU_capped;
                u1 -= minU_capped;
                u2 -= minU_capped;
                u3 -= minU_capped;

                pPoly->SetBlendMode(relive::TBlendModes::eBlend_0);

                pPoly->uBase = static_cast<s16>(minU_capped);
                pPoly->vBase = 0.0f;

                pPoly->SetUV0(
                       static_cast<u8>(u0),
                       static_cast<u8>(v0));
                pPoly->SetUV1(
                       static_cast<u8>(u1),
                       static_cast<u8>(v1));
                pPoly->SetUV2(
                       static_cast<u8>(u2),
                       static_cast<u8>(v2));
                pPoly->SetUV3(
                       static_cast<u8>(u3),
                       static_cast<u8>(v3));

                pPoly->SetSemiTransparent(false);
                pPoly->SetShadeTex(true);

                ot.Add(field_10_layer, pPoly);
            }
        }
    }
}

} // namespace AO

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

void Forcelink_ScreenWave() {}

START_NS_AO

struct ScreenWave_Data
{
    FP_Point field_0_uv1[32][5];
    FP_Point field_500_uv2[32][5];
    FP_Point field_A00_xy[32];
    Poly_FT4 field_B00_poly[2][32][4];
};
ALIVE_ASSERT_SIZEOF(ScreenWave_Data, 0x3700);

#undef min
#undef max

ScreenWave* ScreenWave::ctor_462A70(FP xpos, FP ypos, __int16 layer, FP width, FP speed, __int16 radius)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC9A0);
    field_4_typeId = Types::eScreenWave_83;
    gObjList_drawables_504618->Push_Back(this);

    // TODO: Using frame counter as an ID seems extremely dangerous due to id collision risk!
    field_14_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80('evaW', gnFrameCount_507670, sizeof(ScreenWave_Data));
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
    const __int16 v1 = static_cast<__int16>(std::abs(field_36_screen_xpos) + std::abs(field_38_screen_ypos));
    const __int16 v2 = static_cast<__int16>(std::abs(field_36_screen_xpos - gPsxDisplay_504C78.field_0_width) + std::abs(field_38_screen_ypos));

    const __int16 v3 = static_cast<__int16>(std::abs(field_36_screen_xpos) + std::abs(field_38_screen_ypos - gPsxDisplay_504C78.field_2_height));
    const __int16 v4 = static_cast<__int16>(std::abs(field_36_screen_xpos - gPsxDisplay_504C78.field_0_width) + std::abs(field_38_screen_ypos - gPsxDisplay_504C78.field_2_height));

    field_34_max_radius = static_cast<__int16>(std::max(std::max(v1, v2), std::max(v3, v4)));

    if (radius > 0)
    {
        field_34_max_radius = std::min(field_34_max_radius, radius);
    }

    field_20_fp1 = FP_FromInteger(1);
    field_24_fp1 = FP_FromInteger(1);

    const FP uv1_off = (width / FP_FromInteger(4));
    const FP uv2_off = (width / FP_FromInteger(2));

    auto pData = reinterpret_cast<ScreenWave_Data*>(*field_14_ppRes);

    BYTE ang2 = 0;
    for (int i = 0; i < 32; i++)
    {
        BYTE ang = 128;
        for (int j = 0; j < 5; j++)
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

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            for (int k = 0; k < 4; k++)
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
    ResourceManager::FreeResource_455550(field_14_ppRes);
    return dtor_487DF0();
}

BaseGameObject* ScreenWave::VDestructor(signed int flags)
{
    return Vdtor_463860(flags);
}

ScreenWave* ScreenWave::Vdtor_463860(signed int flags)
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
    if (gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
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

        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 5; j++)
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

void ScreenWave::VRender(int** ppOt)
{
    VRender_463130(ppOt);
}

void ScreenWave::VRender_463130(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO

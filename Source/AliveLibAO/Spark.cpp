#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "Events.hpp"
#include "Function.hpp"
#include "Spark.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "Animation.hpp"
#include "Particle.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

#undef min
#undef max

void Spark_ForceLink() { }

namespace AO {

Spark* Spark::ctor_477B70(FP xpos, FP ypos, FP scale, unsigned __int8 count, __int16 min, __int16 max)
{
    ctor_487E10(1);
    field_6_flags.Set(Options::eDrawable_Bit4);

    SetVTable(this, 0x4BCD48);
    field_4_typeId = Types::eNone_0;
    
    gObjList_drawables_504618->Push_Back(this);

    field_30_xpos = xpos;
    field_34_ypos = ypos;
    field_38_scale = scale;

    if (scale == FP_FromDouble(0.5))
    {
        field_42_layer = 17;
    }
    else
    {
        field_42_layer = 36;
    }

    field_40_b = 127;
    field_3C_r = 31;
    field_3E_g = 31;

    field_4C_count = count;

    field_44_ppSprxRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (field_44_ppSprxRes)
    {
        field_48_pRes = reinterpret_cast<SparkRes*>(*field_44_ppSprxRes);
        for (int idx = 0; idx < field_4C_count; idx++)
        {
            SparkRes* pSparkIter = &field_48_pRes[idx];
            int randAng = 0;
            if (min >= 0)
            {
                randAng = Math_RandomRange_450F20(min, max);
            }
            else
            {
                randAng = min + Math_RandomRange_450F20(0, max - min);
            }
            pSparkIter->field_10_ang = static_cast<BYTE>(randAng);
            pSparkIter->field_14_radius = FP_FromInteger(0);
            pSparkIter->field_18_len = FP_FromInteger(Math_RandomRange_450F20(2, 4));
        }

        field_50_timer = gnFrameCount_507670 + 3;

        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID, 1, 0);
        auto pParticle = ao_new<Particle>();
        if (pParticle)
        {
            pParticle->ctor_478880(xpos, ypos - FP_FromInteger(4), 1532, 38, 21, ppRes);

            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);

            pParticle->field_10_anim.field_B_render_mode = 1;
            pParticle->field_10_anim.field_8_r = 128;
            pParticle->field_10_anim.field_9_g = 128;
            pParticle->field_10_anim.field_A_b = 128;

            if (scale == FP_FromInteger(1))
            {
                pParticle->field_10_anim.field_C_layer = 36;
            }
            else
            {
                pParticle->field_10_anim.field_C_layer = 17;
            }

            pParticle->field_BC_sprite_scale = scale;
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

BaseGameObject* Spark::dtor_477D40()
{
    SetVTable(this, 0x4BCD48);

    if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_504618->Remove_Item(this);
    }

    if (field_44_ppSprxRes)
    {
        ResourceManager::FreeResource_455550(field_44_ppSprxRes);
    }

    return dtor_487DF0();
}

BaseGameObject* Spark::VDestructor(signed int flags)
{
    return Vdtor_478290(flags);
}

Spark* Spark::Vdtor_478290(signed int flags)
{
    dtor_477D40();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Spark::VScreenChanged()
{
    VScreenChanged_478280();
}

void Spark::VScreenChanged_478280()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Spark::VUpdate()
{
    VUpdate_477DB0();
}

void Spark::VUpdate_477DB0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
    if (!sNumCamSwappers_507668)
    {
        if (static_cast<int>(gnFrameCount_507670) < field_50_timer)
        {
            if (static_cast<int>(gnFrameCount_507670) == field_50_timer - 1)
            {
                field_4C_count /= 3;
            }
            for (int idx = 0; idx < field_4C_count; idx++)
            {
                field_48_pRes[idx].field_0_x0 = field_48_pRes[idx].field_14_radius * Math_Sine_451110(field_48_pRes[idx].field_10_ang);
                field_48_pRes[idx].field_4_y0 = field_48_pRes[idx].field_14_radius * Math_Cosine_4510A0(field_48_pRes[idx].field_10_ang);
                field_48_pRes[idx].field_8_x1 = (field_48_pRes[idx].field_18_len + field_48_pRes[idx].field_14_radius) * Math_Sine_451110(field_48_pRes[idx].field_10_ang);
                field_48_pRes[idx].field_C_y1 = (field_48_pRes[idx].field_18_len + field_48_pRes[idx].field_14_radius) * Math_Cosine_4510A0(field_48_pRes[idx].field_10_ang);
                field_48_pRes[idx].field_14_radius = field_48_pRes[idx].field_18_len + FP_FromInteger(Math_RandomRange_450F20(2, 5));
                field_48_pRes[idx].field_18_len = field_48_pRes[idx].field_18_len + FP_FromInteger(2);
            }
        }
        else
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
    }
}

void Spark::VRender(int** ppOt)
{
    VRender_477ED0(ppOt);
}

void Spark::VRender_477ED0(int** ppOt)
{
    PSX_RECT rect = {};
    rect.x = 32767;
    rect.y = 32767;
    rect.w = -32767;
    rect.h = -32767;

    const FP_Point* pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;

    const short xOrg = FP_GetExponent(field_30_xpos) - FP_GetExponent(pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
    const short yOrg = FP_GetExponent(field_34_ypos) - FP_GetExponent(pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos));

    for (int i = 0; i < field_4C_count; i++)
    {
        SparkRes* pSpark = &field_48_pRes[i];

        Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay_504C78.field_A_buffer_index];
        LineG2_Init(pPrim);

        const int y0 = yOrg + FP_GetExponent(pSpark->field_4_y0 * field_38_scale);
        const int y1 = yOrg + FP_GetExponent(pSpark->field_C_y1 * field_38_scale);

        const int x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_0_x0 * field_38_scale), 11);
        const int x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_8_x1 * field_38_scale), 11);

        SetXY0(pPrim, static_cast<short>(x0), static_cast<short>(y0));
        SetXY1(pPrim, static_cast<short>(x1), static_cast<short>(y1));

        SetRGB0(pPrim,
            static_cast<BYTE>(field_3C_r / 2),
            static_cast<BYTE>(field_3E_g / 2),
            static_cast<BYTE>(field_40_b / 2));

        SetRGB1(pPrim,
            static_cast<BYTE>(field_3C_r),
            static_cast<BYTE>(field_3E_g),
            static_cast<BYTE>(field_40_b));

        Poly_Set_SemiTrans_498A40(&pPrim->mBase.header, TRUE);
        OrderingTable_Add_498A80(&ppOt[field_42_layer], &pPrim->mBase.header);
        
        rect.x = std::min(rect.x, std::min(static_cast<short>(x0), static_cast<short>(x1)));
        rect.w = std::max(rect.w, std::max(static_cast<short>(x0), static_cast<short>(x1)));

        rect.y = std::min(rect.y, std::min(static_cast<short>(y0), static_cast<short>(y1)));
        rect.h = std::max(rect.h, std::max(static_cast<short>(y0), static_cast<short>(y1)));
    }

    Prim_SetTPage* pTPage = &field_10_tPage[gPsxDisplay_504C78.field_A_buffer_index];
    Init_SetTPage_495FB0(pTPage, 1, 0, PSX_getTPage_4965D0(0, 1, 0, 0));
    OrderingTable_Add_498A80(&ppOt[field_42_layer], &pTPage->mBase);
    pScreenManager_4FF7C8->InvalidateRect_406E40(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_4FF7C8->field_2E_idx);
}

}

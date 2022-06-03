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

void Spark_ForceLink()
{ }

namespace AO {

Spark::Spark(FP xpos, FP ypos, FP scale, s32 count, s32 min, s32 max)
    : BaseGameObject(1)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    mBaseGameObjectTypeId = Types::eNone_0;

    gObjList_drawables_504618->Push_Back(this);

    field_30_xpos = xpos;
    field_34_ypos = ypos;
    field_38_scale = scale;

    if (scale == FP_FromDouble(0.5))
    {
        field_42_layer = Layer::eLayer_Foreground_Half_17;
    }
    else
    {
        field_42_layer = Layer::eLayer_Foreground_36;
    }

    field_40_b = 127;
    field_3C_r = 31;
    field_3E_g = 31;

    field_4C_count = static_cast<s16>(count);

    field_44_ppSprxRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (field_44_ppSprxRes)
    {
        field_48_pRes = reinterpret_cast<SparkRes*>(*field_44_ppSprxRes);
        for (s32 idx = 0; idx < field_4C_count; idx++)
        {
            SparkRes* pSparkIter = &field_48_pRes[idx];
            s32 randAng = 0;
            if (min >= 0)
            {
                randAng = Math_RandomRange_450F20(static_cast<s16>(min), static_cast<s16>(max));
            }
            else
            {
                randAng = min + Math_RandomRange_450F20(0, static_cast<s16>(max - min));
            }
            pSparkIter->field_10_ang = static_cast<u8>(randAng);
            pSparkIter->field_14_radius = FP_FromInteger(0);
            pSparkIter->field_18_len = FP_FromInteger(Math_RandomRange_450F20(2, 4));
        }

        field_50_timer = sGnFrame + 3;

        const AnimRecord& rec = AO::AnimRec(AnimId::Zap_Sparks);
        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        auto pParticle = ao_new<Particle>(xpos, ypos - FP_FromInteger(4), rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
        if (pParticle)
        {
            pParticle->field_10_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_10_anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

            pParticle->field_10_anim.mRenderMode = TPageAbr::eBlend_1;
            pParticle->field_10_anim.mRed = 128;
            pParticle->field_10_anim.mGreen = 128;
            pParticle->field_10_anim.mBlue = 128;

            if (scale == FP_FromInteger(1))
            {
                pParticle->field_10_anim.mRenderLayer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->field_10_anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->field_BC_sprite_scale = scale;
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Spark::~Spark()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_504618->Remove_Item(this);
    }

    if (field_44_ppSprxRes)
    {
        ResourceManager::FreeResource_455550(field_44_ppSprxRes);
    }
}

void Spark::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Spark::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
    if (!sNumCamSwappers_507668)
    {
        if (static_cast<s32>(sGnFrame) < field_50_timer)
        {
            if (static_cast<s32>(sGnFrame) == field_50_timer - 1)
            {
                field_4C_count /= 3;
            }
            for (s32 idx = 0; idx < field_4C_count; idx++)
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
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

void Spark::VRender(PrimHeader** ppOt)
{
    PSX_RECT rect = {};
    rect.x = 32767;
    rect.y = 32767;
    rect.w = -32767;
    rect.h = -32767;

    const FP_Point* pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;

    const s16 xOrg = FP_GetExponent(field_30_xpos) - FP_GetExponent(pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
    const s16 yOrg = FP_GetExponent(field_34_ypos) - FP_GetExponent(pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos));

    for (s32 i = 0; i < field_4C_count; i++)
    {
        SparkRes* pSpark = &field_48_pRes[i];

        Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay_504C78.field_A_buffer_index];
        LineG2_Init(pPrim);

        const s32 y0 = yOrg + FP_GetExponent(pSpark->field_4_y0 * field_38_scale);
        const s32 y1 = yOrg + FP_GetExponent(pSpark->field_C_y1 * field_38_scale);

        const s32 x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_0_x0 * field_38_scale), 11);
        const s32 x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_8_x1 * field_38_scale), 11);

        SetXY0(pPrim, static_cast<s16>(x0), static_cast<s16>(y0));
        SetXY1(pPrim, static_cast<s16>(x1), static_cast<s16>(y1));

        SetRGB0(pPrim,
                static_cast<u8>(field_3C_r / 2),
                static_cast<u8>(field_3E_g / 2),
                static_cast<u8>(field_40_b / 2));

        SetRGB1(pPrim,
                static_cast<u8>(field_3C_r),
                static_cast<u8>(field_3E_g),
                static_cast<u8>(field_40_b));

        Poly_Set_SemiTrans_498A40(&pPrim->mBase.header, TRUE);
        OrderingTable_Add_498A80(OtLayer(ppOt, field_42_layer), &pPrim->mBase.header);

        rect.x = std::min(rect.x, std::min(static_cast<s16>(x0), static_cast<s16>(x1)));
        rect.w = std::max(rect.w, std::max(static_cast<s16>(x0), static_cast<s16>(x1)));

        rect.y = std::min(rect.y, std::min(static_cast<s16>(y0), static_cast<s16>(y1)));
        rect.h = std::max(rect.h, std::max(static_cast<s16>(y0), static_cast<s16>(y1)));
    }

    Prim_SetTPage* pTPage = &field_10_tPage[gPsxDisplay_504C78.field_A_buffer_index];
    Init_SetTPage_495FB0(pTPage, 1, 0, PSX_getTPage_4965D0(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
    OrderingTable_Add_498A80(OtLayer(ppOt, field_42_layer), &pTPage->mBase);
    pScreenManager_4FF7C8->InvalidateRect(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_4FF7C8->field_2E_idx);
}

} // namespace AO

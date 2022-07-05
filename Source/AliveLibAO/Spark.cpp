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
#include "../relive_lib/Primitives.hpp"

#undef min
#undef max

void Spark_ForceLink()
{ }

namespace AO {

Spark::Spark(FP xpos, FP ypos, FP scale, s32 count, s32 min, s32 max)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    mBaseGameObjectTypeId = ReliveTypes::eNone;

    gObjListDrawables->Push_Back(this);

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

    field_44_ppSprxRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (field_44_ppSprxRes)
    {
        field_48_pRes = reinterpret_cast<SparkRes*>(*field_44_ppSprxRes);
        for (s32 idx = 0; idx < field_4C_count; idx++)
        {
            SparkRes* pSparkIter = &field_48_pRes[idx];
            s32 randAng = 0;
            if (min >= 0)
            {
                randAng = Math_RandomRange(static_cast<s16>(min), static_cast<s16>(max));
            }
            else
            {
                randAng = min + Math_RandomRange(0, static_cast<s16>(max - min));
            }
            pSparkIter->field_10_ang = static_cast<u8>(randAng);
            pSparkIter->field_14_radius = FP_FromInteger(0);
            pSparkIter->field_18_len = FP_FromInteger(Math_RandomRange(2, 4));
        }

        field_50_timer = sGnFrame + 3;

        const AnimRecord& rec = AO::AnimRec(AnimId::Zap_Sparks);
        u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        auto pParticle = relive_new Particle(xpos, ypos - FP_FromInteger(4), AnimId::Zap_Sparks, ppRes);
        if (pParticle)
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 128;
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 128;
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 128;

            if (scale == FP_FromInteger(1))
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
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
        gObjListDrawables->Remove_Item(this);
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
    if (EventGet(kEventDeathReset))
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
                field_48_pRes[idx].field_14_radius = field_48_pRes[idx].field_18_len + FP_FromInteger(Math_RandomRange(2, 5));
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

    const FP_Point* pCamPos = pScreenManager->mCamPos;

    const s16 xOrg = FP_GetExponent(field_30_xpos) - FP_GetExponent(pCamPos->x - FP_FromInteger(pScreenManager->mCamXOff));
    const s16 yOrg = FP_GetExponent(field_34_ypos) - FP_GetExponent(pCamPos->y - FP_FromInteger(pScreenManager->mCamYOff));

    for (s32 i = 0; i < field_4C_count; i++)
    {
        SparkRes* pSpark = &field_48_pRes[i];

        Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay.mBufferIndex];
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

        Poly_Set_SemiTrans(&pPrim->mBase.header, TRUE);
        OrderingTable_Add(OtLayer(ppOt, field_42_layer), &pPrim->mBase.header);

        rect.x = std::min(rect.x, std::min(static_cast<s16>(x0), static_cast<s16>(x1)));
        rect.w = std::max(rect.w, std::max(static_cast<s16>(x0), static_cast<s16>(x1)));

        rect.y = std::min(rect.y, std::min(static_cast<s16>(y0), static_cast<s16>(y1)));
        rect.h = std::max(rect.h, std::max(static_cast<s16>(y0), static_cast<s16>(y1)));
    }

    Prim_SetTPage* pTPage = &field_10_tPage[gPsxDisplay.mBufferIndex];
    Init_SetTPage(pTPage, 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
    OrderingTable_Add(OtLayer(ppOt, field_42_layer), &pTPage->mBase);
    pScreenManager->InvalidateRectCurrentIdx(
        rect.x,
        rect.y,
        rect.w,
        rect.h);
}

} // namespace AO

#include "stdafx.h"
#include "Spark.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"

Spark::Spark(FP xpos, FP ypos, FP scale, s32 count, s32 minAngle, s32 maxAngle, SparkType type)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetType(ReliveTypes::eNone);

    gObjListDrawables->Push_Back(this);

    field_64_type = type;
    field_40_xpos = xpos;
    field_44_ypos = ypos;
    field_48_scale = scale;

    if (scale == FP_FromDouble(0.5))
    {
        field_52_layer = Layer::eLayer_Foreground_Half_17;
    }
    else
    {
        field_52_layer = Layer::eLayer_Foreground_36;
    }

    field_50_b = 127;
    field_4C_r = 31;
    field_4E_g = 31;

    field_5C_count = static_cast<s16>(count);

    field_54_ppSprxRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (field_54_ppSprxRes)
    {
        field_58_pRes = reinterpret_cast<SparkRes*>(*field_54_ppSprxRes);
        for (s32 idx = 0; idx < field_5C_count; idx++)
        {
            SparkRes* pSparkIter = &field_58_pRes[idx];
            s32 randAng = 0;
            if (minAngle >= 0)
            {
                randAng = Math_RandomRange(static_cast<s16>(minAngle), static_cast<s16>(maxAngle));
            }
            else
            {
                randAng = minAngle + Math_RandomRange(0, static_cast<s16>(maxAngle - minAngle));
            }
            pSparkIter->field_10_ang = static_cast<u8>(randAng);
            pSparkIter->field_14_radius = FP_FromInteger(0);
            pSparkIter->field_18_len = FP_FromInteger(Math_RandomRange(2, 4));
        }

        field_60_timer = sGnFrame + 3;

        if (field_64_type == SparkType::eBigChantParticle_1)
        {
            New_TintChant_Particle(field_40_xpos, field_44_ypos - FP_FromInteger(4), scale, Layer::eLayer_0);
        }
        else
        {
            // Normal drill type sparks
            const AnimRecord& rec = AnimRec(AnimId::Zap_Sparks);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            auto pParticle = relive_new Particle(
                field_40_xpos,
                field_44_ypos,
                rec.mFrameTableOffset,
                rec.mMaxW,
                rec.mMaxH,
                ppRes);
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
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Spark::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sNum_CamSwappers_5C1B66 == 0)
    {
        if (static_cast<s32>(sGnFrame) < field_60_timer)
        {
            if (static_cast<s32>(sGnFrame) == field_60_timer - 1)
            {
                // Reduce spark count as time passes
                field_5C_count /= 3;
            }

            for (s32 idx = 0; idx < field_5C_count; idx++)
            {
                SparkRes* pSpark = &field_58_pRes[idx];
                pSpark->field_0_x0 = pSpark->field_14_radius * Math_Sine_496DD0(pSpark->field_10_ang);
                pSpark->field_4_y0 = pSpark->field_14_radius * Math_Cosine_496CD0(pSpark->field_10_ang);
                pSpark->field_8_x1 = (pSpark->field_14_radius + pSpark->field_18_len) * Math_Sine_496DD0(pSpark->field_10_ang);
                pSpark->field_C_y1 = (pSpark->field_14_radius + pSpark->field_18_len) * Math_Cosine_496CD0(pSpark->field_10_ang);
                pSpark->field_14_radius = pSpark->field_18_len + FP_FromInteger(Math_RandomRange(2, 5));
                pSpark->field_18_len = pSpark->field_18_len + FP_FromInteger(2);
            }
        }
        else
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void Spark::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4810D0(
            sActiveHero->mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            sActiveHero->mBaseAnimatedWithPhysicsGameObject_PathNumber,
            field_40_xpos,
            field_44_ypos,
            0))
    {
        PSX_Point xy = {32767, 32767};
        PSX_Point wh = {-32767, -32767};

        const s32 xOrg = FP_GetExponent(field_40_xpos) - FP_GetExponent(pScreenManager->mCamPos->field_0_x);
        const s32 yOrg = FP_GetExponent(field_44_ypos) - FP_GetExponent(pScreenManager->mCamPos->field_4_y);

        for (s32 i = 0; i < field_5C_count; i++)
        {
            SparkRes* pSpark = &field_58_pRes[i];

            Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay_5C1130.field_C_buffer_index];
            LineG2_Init(pPrim);

            const s32 y0 = yOrg + FP_GetExponent(pSpark->field_4_y0 * field_48_scale);
            const s32 y1 = yOrg + FP_GetExponent(pSpark->field_C_y1 * field_48_scale);
            const s32 x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_0_x0 * field_48_scale));
            const s32 x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_8_x1 * field_48_scale));

            SetXY0(pPrim, static_cast<s16>(x0), static_cast<s16>(y0));
            SetXY1(pPrim, static_cast<s16>(x1), static_cast<s16>(y1));

            SetRGB0(pPrim,
                    static_cast<u8>(field_4C_r / 2),
                    static_cast<u8>(field_4E_g / 2),
                    static_cast<u8>(field_50_b / 2));

            SetRGB1(pPrim,
                    static_cast<u8>(field_4C_r),
                    static_cast<u8>(field_4E_g),
                    static_cast<u8>(field_50_b));

            Poly_Set_SemiTrans_4F8A60(&pPrim->mBase.header, TRUE);
            OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_52_layer), &pPrim->mBase.header);

            // TODO: Can be refactored much further - looks like min/max stuff
            s16 x1Short = static_cast<s16>(x1);
            s16 maxX = xy.field_0_x;

            if (x1Short <= xy.field_0_x)
            {
                maxX = x1Short;
            }

            if (x0 <= maxX)
            {
                xy.field_0_x = static_cast<s16>(x0);
            }
            else if (x1Short <= xy.field_0_x)
            {
                xy.field_0_x = x1Short;
            }

            s16 x1Short2 = x1Short;
            if (x1Short <= wh.field_0_x)
            {
                x1Short2 = wh.field_0_x;
            }

            if (x0 <= x1Short2)
            {
                if (x1Short > wh.field_0_x)
                {
                    wh.field_0_x = x1Short;
                }
            }
            else
            {
                wh.field_0_x = static_cast<s16>(x0);
            }

            s16 yPoint = xy.field_2_y;
            if (y1 <= xy.field_2_y)
            {
                yPoint = static_cast<s16>(y1);
            }
            if (y0 <= yPoint)
            {
                xy.field_2_y = static_cast<s16>(y0);
            }
            else if (y1 <= xy.field_2_y)
            {
                xy.field_2_y = static_cast<s16>(y1);
            }

            s16 y1Short = static_cast<s16>(y1);
            if (y1 <= wh.field_2_y)
            {
                y1Short = wh.field_2_y;
            }

            if (y0 <= y1Short)
            {
                if (y1 > wh.field_2_y)
                {
                    wh.field_2_y = static_cast<s16>(y1);
                }
            }
            else
            {
                wh.field_2_y = static_cast<s16>(y0);
            }
        }

        Prim_SetTPage* pTPage = &field_20_tPage[gPsxDisplay_5C1130.field_C_buffer_index];
        Init_SetTPage(pTPage, 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_52_layer), &pTPage->mBase);
        pScreenManager->InvalidateRect(
            xy.field_0_x,
            xy.field_2_y,
            wh.field_0_x,
            wh.field_2_y,
            pScreenManager->mIdx);
    }
}

void Spark::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

Spark::~Spark()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjListDrawables->Remove_Item(this);
    }

    if (field_54_ppSprxRes)
    {
        ResourceManager::FreeResource_49C330(field_54_ppSprxRes);
    }
}

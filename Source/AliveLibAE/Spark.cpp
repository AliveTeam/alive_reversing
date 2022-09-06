#include "stdafx.h"
#include "Spark.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/Particle.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

Spark::Spark(FP xpos, FP ypos, FP scale, s32 count, s32 minAngle, s32 maxAngle, SparkType type)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetType(ReliveTypes::eNone);

    gObjListDrawables->Push_Back(this);

    mSparkType = type;
    mXPos = xpos;
    mYPos = ypos;
    mSpriteScale = scale;

    if (scale == FP_FromDouble(0.5))
    {
        mLayer = Layer::eLayer_Foreground_Half_17;
    }
    else
    {
        mLayer = Layer::eLayer_Foreground_36;
    }

    mBlue = 127;
    mRed = 31;
    mGreen = 31;

    mSparkCount = static_cast<s16>(count);

    mSprxRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (mSprxRes)
    {
        mSparkRes = reinterpret_cast<SparkRes*>(*mSprxRes);
        for (s32 idx = 0; idx < mSparkCount; idx++)
        {
            SparkRes* pSparkIter = &mSparkRes[idx];
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

        mTimer = sGnFrame + 3;

        if (mSparkType == SparkType::eBigChantParticle_1)
        {
            New_TintChant_Particle(mXPos, mYPos - FP_FromInteger(4), scale, Layer::eLayer_0);
        }
        else
        {
            // Normal drill type sparks
            const AnimRecord& rec = AnimRec(AnimId::ChantOrb_Particle_Small);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            auto pParticle = relive_new Particle(
                mXPos,
                mYPos,
                AnimId::ChantOrb_Particle_Small,
                ppRes);
            if (pParticle)
            {
                pParticle->mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
                pParticle->mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

                pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mAnim.mRed = 128;
                pParticle->mAnim.mGreen = 128;
                pParticle->mAnim.mBlue = 128;

                if (scale == FP_FromInteger(1))
                {
                    pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
                }
                else
                {
                    pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
                }

                pParticle->mSpriteScale = scale;
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
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sNum_CamSwappers_5C1B66 == 0)
    {
        if (static_cast<s32>(sGnFrame) < mTimer)
        {
            if (static_cast<s32>(sGnFrame) == mTimer - 1)
            {
                // Reduce spark count as time passes
                mSparkCount /= 3;
            }

            for (s32 idx = 0; idx < mSparkCount; idx++)
            {
                SparkRes* pSpark = &mSparkRes[idx];
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
    if (gMap.Is_Point_In_Current_Camera(
            sActiveHero->mCurrentLevel,
            sActiveHero->mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        PSX_Point xy = {32767, 32767};
        PSX_Point wh = {-32767, -32767};

        const s32 xOrg = FP_GetExponent(mXPos) - FP_GetExponent(pScreenManager->CamXPos());
        const s32 yOrg = FP_GetExponent(mYPos) - FP_GetExponent(pScreenManager->CamYPos());

        for (s32 i = 0; i < mSparkCount; i++)
        {
            SparkRes* pSpark = &mSparkRes[i];

            Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay.mBufferIndex];
            LineG2_Init(pPrim);

            const s32 y0 = yOrg + FP_GetExponent(pSpark->field_4_y0 * mSpriteScale);
            const s32 y1 = yOrg + FP_GetExponent(pSpark->field_C_y1 * mSpriteScale);
            const s32 x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_0_x0 * mSpriteScale));
            const s32 x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_8_x1 * mSpriteScale));

            SetXY0(pPrim, static_cast<s16>(x0), static_cast<s16>(y0));
            SetXY1(pPrim, static_cast<s16>(x1), static_cast<s16>(y1));

            SetRGB0(pPrim,
                    static_cast<u8>(mRed / 2),
                    static_cast<u8>(mGreen / 2),
                    static_cast<u8>(mBlue / 2));

            SetRGB1(pPrim,
                    static_cast<u8>(mRed),
                    static_cast<u8>(mGreen),
                    static_cast<u8>(mBlue));

            Poly_Set_SemiTrans(&pPrim->mBase.header, TRUE);
            OrderingTable_Add(OtLayer(ppOt, mLayer), &pPrim->mBase.header);

            // TODO: Can be refactored much further - looks like min/max stuff
            s16 x1Short = static_cast<s16>(x1);
            s16 maxX = xy.x;

            if (x1Short <= xy.x)
            {
                maxX = x1Short;
            }

            if (x0 <= maxX)
            {
                xy.x = static_cast<s16>(x0);
            }
            else if (x1Short <= xy.x)
            {
                xy.x = x1Short;
            }

            s16 x1Short2 = x1Short;
            if (x1Short <= wh.x)
            {
                x1Short2 = wh.x;
            }

            if (x0 <= x1Short2)
            {
                if (x1Short > wh.x)
                {
                    wh.x = x1Short;
                }
            }
            else
            {
                wh.x = static_cast<s16>(x0);
            }

            s16 yPoint = xy.y;
            if (y1 <= xy.y)
            {
                yPoint = static_cast<s16>(y1);
            }
            if (y0 <= yPoint)
            {
                xy.y = static_cast<s16>(y0);
            }
            else if (y1 <= xy.y)
            {
                xy.y = static_cast<s16>(y1);
            }

            s16 y1Short = static_cast<s16>(y1);
            if (y1 <= wh.y)
            {
                y1Short = wh.y;
            }

            if (y0 <= y1Short)
            {
                if (y1 > wh.y)
                {
                    wh.y = static_cast<s16>(y1);
                }
            }
            else
            {
                wh.y = static_cast<s16>(y0);
            }
        }

        Prim_SetTPage* pTPage = &mTPage[gPsxDisplay.mBufferIndex];
        Init_SetTPage(pTPage, 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
        OrderingTable_Add(OtLayer(ppOt, mLayer), &pTPage->mBase);
        pScreenManager->InvalidateRectCurrentIdx(
            xy.x,
            xy.y,
            wh.x,
            wh.y);
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

    if (mSprxRes)
    {
        ResourceManager::FreeResource_49C330(mSprxRes);
    }
}

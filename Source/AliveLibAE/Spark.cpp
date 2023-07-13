#include "stdafx.h"
#include "Spark.hpp"
#include "../relive_lib/Function.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Map.hpp"

Spark::Spark(FP xpos, FP ypos, FP scale, s32 count, s32 minAngle, s32 maxAngle, SparkType type)
    : BaseGameObject(true, 0)
{
    SetDrawable(true);

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
    mSparkRes = relive_new SparkRes[mSparkCount];
    if (mSparkRes)
    {
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
            pSparkIter->mAng = static_cast<u8>(randAng);
            pSparkIter->mRadius = FP_FromInteger(0);
            pSparkIter->mLen = FP_FromInteger(Math_RandomRange(2, 4));
        }

        mTimer = MakeTimer(3);

        if (mSparkType == SparkType::eBigChantParticle_1)
        {
            New_TintChant_Particle(mXPos, mYPos - FP_FromInteger(4), scale, Layer::eLayer_0);
        }
        else
        {
            // Normal drill type sparks
            AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle_Small);
            auto pParticle = relive_new Particle(
                mXPos,
                mYPos,
                ppRes);
            if (pParticle)
            {
                pParticle->GetAnimation().SetSemiTrans(true);
                pParticle->GetAnimation().SetBlending(true);

                pParticle->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

                pParticle->GetAnimation().SetRGB(128, 128, 128);

                if (scale == FP_FromInteger(1))
                {
                    pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
                }
                else
                {
                    pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
                }

                pParticle->SetSpriteScale(scale);
            }
        }
    }
    else
    {
        SetDead(true);
    }
}

void Spark::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (gNumCamSwappers == 0)
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
                pSpark->mX0 = pSpark->mRadius * Math_Sine(pSpark->mAng);
                pSpark->mY0 = pSpark->mRadius * Math_Cosine(pSpark->mAng);
                pSpark->mX1 = (pSpark->mRadius + pSpark->mLen) * Math_Sine(pSpark->mAng);
                pSpark->mY1 = (pSpark->mRadius + pSpark->mLen) * Math_Cosine(pSpark->mAng);
                pSpark->mRadius = pSpark->mLen + FP_FromInteger(Math_RandomRange(2, 5));
                pSpark->mLen = pSpark->mLen + FP_FromInteger(2);
            }
        }
        else
        {
            SetDead(true);
        }
    }
}

void Spark::VRender(OrderingTable& ot)
{
    if (gMap.Is_Point_In_Current_Camera(
            sActiveHero->mCurrentLevel,
            sActiveHero->mCurrentPath,
            mXPos,
            mYPos,
            0))
    {

        const s32 xOrg = FP_GetExponent(mXPos) - FP_GetExponent(gScreenManager->CamXPos());
        const s32 yOrg = FP_GetExponent(mYPos) - FP_GetExponent(gScreenManager->CamYPos());

        for (s32 i = 0; i < mSparkCount; i++)
        {
            SparkRes* pSpark = &mSparkRes[i];

            Line_G2* pPrim = &pSpark->mLineG2s[gPsxDisplay.mBufferIndex];

            const s32 y0 = yOrg + FP_GetExponent(pSpark->mY0 * mSpriteScale);
            const s32 y1 = yOrg + FP_GetExponent(pSpark->mY1 * mSpriteScale);

            const s32 x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->mX0 * mSpriteScale));
            const s32 x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->mX1 * mSpriteScale));

            pPrim->SetXY0(static_cast<s16>(x0), static_cast<s16>(y0));
            pPrim->SetXY1(static_cast<s16>(x1), static_cast<s16>(y1));

            pPrim->SetRGB0(
                    static_cast<u8>(mRed / 2),
                    static_cast<u8>(mGreen / 2),
                    static_cast<u8>(mBlue / 2));

            pPrim->SetRGB1(
                    static_cast<u8>(mRed),
                    static_cast<u8>(mGreen),
                    static_cast<u8>(mBlue));

            pPrim->SetSemiTransparent(true);
            pPrim->SetBlendMode(relive::TBlendModes::eBlend_1);

            OrderingTable_Add(OtLayer(ppOt, mLayer), pPrim);
        }
    }
}

void Spark::VScreenChanged()
{
    SetDead(true);
}

Spark::~Spark()
{
    if (GetDrawable())
    {
        gObjListDrawables->Remove_Item(this);
    }

    relive_delete[] mSparkRes;
}

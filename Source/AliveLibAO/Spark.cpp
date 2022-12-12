#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "Function.hpp"
#include "Spark.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Primitives.hpp"

#undef min
#undef max

#include <algorithm>

namespace AO {

Spark::Spark(FP xpos, FP ypos, FP scale, s32 count, s32 min, s32 max)
    : BaseGameObject(true, 0)
{
    SetDrawable(true);

    SetType(ReliveTypes::eNone);

    gObjListDrawables->Push_Back(this);

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
            if (min >= 0)
            {
                randAng = Math_RandomRange(static_cast<s16>(min), static_cast<s16>(max));
            }
            else
            {
                randAng = min + Math_RandomRange(0, static_cast<s16>(max - min));
            }
            pSparkIter->mAng = static_cast<u8>(randAng);
            pSparkIter->mRadius = FP_FromInteger(0);
            pSparkIter->mLen = FP_FromInteger(Math_RandomRange(2, 4));
        }

        mTimer = sGnFrame + 3;

        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle_Small));
        auto pParticle = relive_new Particle(xpos, ypos - FP_FromInteger(4), GetAnimRes(AnimId::ChantOrb_Particle_Small));
        if (pParticle)
        {
            pParticle->GetAnimation().SetSemiTrans(true);
            pParticle->GetAnimation().SetBlending(true);

            pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
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
    else
    {
        SetDead(true);
    }
}

Spark::~Spark()
{
    if (GetDrawable())
    {
        gObjListDrawables->Remove_Item(this);
    }

    relive_delete[] mSparkRes;
}

void Spark::VScreenChanged()
{
    SetDead(true);
}

void Spark::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    if (!gNumCamSwappers)
    {
        if (static_cast<s32>(sGnFrame) < mTimer)
        {
            if (static_cast<s32>(sGnFrame) == mTimer - 1)
            {
                mSparkCount /= 3;
            }
            for (s32 idx = 0; idx < mSparkCount; idx++)
            {
                mSparkRes[idx].mX0 = mSparkRes[idx].mRadius * Math_Sine(mSparkRes[idx].mAng);
                mSparkRes[idx].mY0 = mSparkRes[idx].mRadius * Math_Cosine(mSparkRes[idx].mAng);
                mSparkRes[idx].mX1 = (mSparkRes[idx].mLen + mSparkRes[idx].mRadius) * Math_Sine(mSparkRes[idx].mAng);
                mSparkRes[idx].mY1 = (mSparkRes[idx].mLen + mSparkRes[idx].mRadius) * Math_Cosine(mSparkRes[idx].mAng);
                mSparkRes[idx].mRadius = mSparkRes[idx].mLen + FP_FromInteger(Math_RandomRange(2, 5));
                mSparkRes[idx].mLen = mSparkRes[idx].mLen + FP_FromInteger(2);
            }
        }
        else
        {
            SetDead(true);
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

    const FP_Point* pCamPos = gScreenManager->mCamPos;

    const s16 xOrg = FP_GetExponent(mXPos) - FP_GetExponent(pCamPos->x - FP_FromInteger(gScreenManager->mCamXOff));
    const s16 yOrg = FP_GetExponent(mYPos) - FP_GetExponent(pCamPos->y - FP_FromInteger(gScreenManager->mCamYOff));

    for (s32 i = 0; i < mSparkCount; i++)
    {
        SparkRes* pSpark = &mSparkRes[i];

        Line_G2* pPrim = &pSpark->mLineG2s[gPsxDisplay.mBufferIndex];
        LineG2_Init(pPrim);

        const s32 y0 = yOrg + FP_GetExponent(pSpark->mY0 * mSpriteScale);
        const s32 y1 = yOrg + FP_GetExponent(pSpark->mY1 * mSpriteScale);

        const s32 x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->mX0 * mSpriteScale), 11);
        const s32 x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->mX1 * mSpriteScale), 11);

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

        Poly_Set_SemiTrans(&pPrim->mBase.header, true);
        OrderingTable_Add(OtLayer(ppOt, mLayer), &pPrim->mBase.header);

        rect.x = std::min(rect.x, std::min(static_cast<s16>(x0), static_cast<s16>(x1)));
        rect.w = std::max(rect.w, std::max(static_cast<s16>(x0), static_cast<s16>(x1)));

        rect.y = std::min(rect.y, std::min(static_cast<s16>(y0), static_cast<s16>(y1)));
        rect.h = std::max(rect.h, std::max(static_cast<s16>(y0), static_cast<s16>(y1)));
    }

    Prim_SetTPage* pTPage = &mTPage[gPsxDisplay.mBufferIndex];
    Init_SetTPage(pTPage, PSX_getTPage(TPageAbr::eBlend_1));
    OrderingTable_Add(OtLayer(ppOt, mLayer), &pTPage->mBase);
}

} // namespace AO

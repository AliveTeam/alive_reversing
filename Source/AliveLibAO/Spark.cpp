#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "Function.hpp"
#include "Spark.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
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

        mTimer = sGnFrame + 3;

        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle_Small));
        auto pParticle = relive_new Particle(xpos, ypos - FP_FromInteger(4), GetAnimRes(AnimId::ChantOrb_Particle_Small));
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

    if (mSprxRes)
    {
        ResourceManager::FreeResource_455550(mSprxRes);
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
        if (static_cast<s32>(sGnFrame) < mTimer)
        {
            if (static_cast<s32>(sGnFrame) == mTimer - 1)
            {
                mSparkCount /= 3;
            }
            for (s32 idx = 0; idx < mSparkCount; idx++)
            {
                mSparkRes[idx].field_0_x0 = mSparkRes[idx].field_14_radius * Math_Sine_451110(mSparkRes[idx].field_10_ang);
                mSparkRes[idx].field_4_y0 = mSparkRes[idx].field_14_radius * Math_Cosine_4510A0(mSparkRes[idx].field_10_ang);
                mSparkRes[idx].field_8_x1 = (mSparkRes[idx].field_18_len + mSparkRes[idx].field_14_radius) * Math_Sine_451110(mSparkRes[idx].field_10_ang);
                mSparkRes[idx].field_C_y1 = (mSparkRes[idx].field_18_len + mSparkRes[idx].field_14_radius) * Math_Cosine_4510A0(mSparkRes[idx].field_10_ang);
                mSparkRes[idx].field_14_radius = mSparkRes[idx].field_18_len + FP_FromInteger(Math_RandomRange(2, 5));
                mSparkRes[idx].field_18_len = mSparkRes[idx].field_18_len + FP_FromInteger(2);
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

    const s16 xOrg = FP_GetExponent(mXPos) - FP_GetExponent(pCamPos->x - FP_FromInteger(pScreenManager->mCamXOff));
    const s16 yOrg = FP_GetExponent(mYPos) - FP_GetExponent(pCamPos->y - FP_FromInteger(pScreenManager->mCamYOff));

    for (s32 i = 0; i < mSparkCount; i++)
    {
        SparkRes* pSpark = &mSparkRes[i];

        Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay.mBufferIndex];
        LineG2_Init(pPrim);

        const s32 y0 = yOrg + FP_GetExponent(pSpark->field_4_y0 * mSpriteScale);
        const s32 y1 = yOrg + FP_GetExponent(pSpark->field_C_y1 * mSpriteScale);

        const s32 x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_0_x0 * mSpriteScale), 11);
        const s32 x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_8_x1 * mSpriteScale), 11);

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

        rect.x = std::min(rect.x, std::min(static_cast<s16>(x0), static_cast<s16>(x1)));
        rect.w = std::max(rect.w, std::max(static_cast<s16>(x0), static_cast<s16>(x1)));

        rect.y = std::min(rect.y, std::min(static_cast<s16>(y0), static_cast<s16>(y1)));
        rect.h = std::max(rect.h, std::max(static_cast<s16>(y0), static_cast<s16>(y1)));
    }

    Prim_SetTPage* pTPage = &mTPage[gPsxDisplay.mBufferIndex];
    Init_SetTPage(pTPage, 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
    OrderingTable_Add(OtLayer(ppOt, mLayer), &pTPage->mBase);
    pScreenManager->InvalidateRectCurrentIdx(
        rect.x,
        rect.y,
        rect.w,
        rect.h);
}

} // namespace AO

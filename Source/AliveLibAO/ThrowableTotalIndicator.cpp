#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Math.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

u16 bThrowableIndicatorExists_504C70 = 0;

ThrowableTotalIndicator::~ThrowableTotalIndicator()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjListDrawables->Remove_Item(this);
    }

    if (mFade)
    {
        bThrowableIndicatorExists_504C70--;
    }
}

void ThrowableTotalIndicator::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void ThrowableTotalIndicator::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    switch (mState)
    {
        case ThrowableTotalIndicatorState::eCreated:
        {
            mXPos = mStartXPos - (FP_FromInteger(12) * Math_Sine_451110(static_cast<u8>(2 * sGnFrame)));
            mYPos = (FP_FromInteger(12) * Math_Cosine_4510A0(static_cast<u8>(2 * sGnFrame))) + mStartYPos;

            const s16 rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine_451110(static_cast<u8>(3 * sGnFrame))) + 80;

            mRGB.SetRGB(rgb, rgb, rgb);
        }
        break;

        case ThrowableTotalIndicatorState::eFading:
            if (mYPos >= (mStartYPos - FP_FromInteger(20)))
            {
                if (mRGB.r < 70 && mRGB.g < 90 && mRGB.b < 20)
                {
                    mRGB.r += 14;
                    mRGB.g += 18;
                    mRGB.b += 4;
                }

                mXPos += mSpeedX;
                mYPos += mSpeedY;
            }
            else
            {
                mState = ThrowableTotalIndicatorState::eVanishing;
            }
            break;

        case ThrowableTotalIndicatorState::eVanishing:
            if (mRGB.r < 7 && mRGB.g < 7 && mRGB.b < 7)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            mRGB.r -= 7;
            mRGB.g -= 9;
            mRGB.b -= 2;

            mXPos += mSpeedX;
            mYPos += mSpeedY;
            break;
    }
}

const s16 kNum_0[17] = {
    4,
    -3, -4, 3, -4,
    3, -3, 3, 3,
    3, 4, -3, 4,
    -3, 3, -3, -3};

const s16 kNum_1[5] = {
    1,
    2, -4, 2, 4};

const s16 kNum_2[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, -1,
    5, 0, -5, 0,
    -5, 1, -5, 3,
    -5, 4, 5, 4};

const s16 kNum_3[17] = {
    4,
    -5,
    -4,
    5,
    -4,
    5,
    -3,
    5,
    3,
    5,
    4,
    -5,
    4,
    -4,
    0,
    4,
    0,
};

const s16 kNum_4[13] = {
    3,
    -5, -4, -5, -1,
    -5, 0, 4, 0,
    5, -4, 5, 4};

const s16 kNum_5[21] = {
    5,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 5, 0,
    5, 1, 5, 3,
    5, 4, -5, 4};

const s16 kNum_6[21] = {
    5,
    5, -4, -5, -4, -5,
    -3, -5, 3, -5, 4,
    5, 4, 5, 3, 5,
    1, 5, 0, -4, 0};

const s16 kNum_7[9] = {
    2,
    -5, -4, 5, -4,
    5, -3, 0, 4};

const s16 kNum_8[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -5, 3, -5, -3,
    -4, 0, 4, 0};

const s16 kNum_9[17] = {
    4,
    5, 4, 5, -3,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 4, 0};

const s16 kInfinity[25] = {
    6,
    -3, -2, -5, 0,
    -5, 1, -3, 3,
    -2, 3, 2, -2,
    3, -2, 5, 0,
    5, 1, 3, 3,
    2, 3, -2, -2};

const s16 kCheckpoint[36] = {
    8, 0, -6, 1,
    -6,
    12,
    0,
    13,
    0,
    0,
    6,
    1,
    6,
    -11,
    0,
    -12,
    0,
    2,
    -5,
    11,
    -1,
    11,
    1,
    2,
    5,
    -1,
    5,
    -10,
    1,
    -10,
    -1,
    -1,
    -5,
    0,
    0,
    0};

const s16* kNumbersArray[12] = {
    kNum_0,
    kNum_1,
    kNum_2,
    kNum_3,
    kNum_4,
    kNum_5,
    kNum_6,
    kNum_7,
    kNum_8,
    kNum_9,
    kInfinity,
    kCheckpoint};

void ThrowableTotalIndicator::VRender(PrimHeader** ppOt)
{
    if (*kNumbersArray[mNumToShow] <= 0)
    {
        return;
    }

    const FP_Point* camPos = pScreenManager->mCamPos;
    const FP camX = FP_FromInteger(FP_GetExponent(camPos->x - FP_FromInteger(pScreenManager->mCamXOff)));
    const FP camY = FP_FromInteger(FP_GetExponent(camPos->y - FP_FromInteger(pScreenManager->mCamYOff)));

    s16 xpos = 0;
    s16 ypos = 0;

    for (s16 counter = 0; counter < kNumbersArray[mNumToShow][0]; counter++)
    {
        xpos = FP_GetExponent(mXPos - camX);
        ypos = FP_GetExponent(mYPos - camY);

        const FP x0 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 1]) * mSpriteScale;
        const FP y0 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 2]) * mSpriteScale;
        const FP x1 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 3]) * mSpriteScale;
        const FP y1 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 4]) * mSpriteScale;

        s16 primBaseX = 0;
        s16 primVertX = 0;
        if (mNumToShow == 11)
        {
            primBaseX = PsxToPCX(xpos);
            primVertX = PsxToPCX(xpos);
        }
        else
        {
            primBaseX = PsxToPCX(xpos, 11);
            primVertX = PsxToPCX(xpos, 11);
        }

        Line_F2* pLine = &mLines[gPsxDisplay.mBufferIndex][counter];
        Line_F2_Init(pLine);

        SetXY0(pLine, primBaseX + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, primVertX + FP_GetExponent(x1), ypos + FP_GetExponent(y1));
        SetRGB0(pLine,
                static_cast<u8>(mRGB.r),
                static_cast<u8>(mRGB.g),
                static_cast<u8>(mRGB.b));
        Poly_Set_SemiTrans(&pLine->mBase.header, 1);
        OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pLine->mBase.header);
    }

    Init_SetTPage(&mTPage[gPsxDisplay.mBufferIndex], 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));
    OrderingTable_Add(OtLayer(ppOt, mOtLayer), &mTPage->mBase);
}

ThrowableTotalIndicator::ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP /*scale*/, s32 count, bool bFade)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    SetType(ReliveTypes::eThrowableTotalIndicator);

    gObjListDrawables->Push_Back(this);

    mStartXPos = xpos;
    mStartYPos = ypos;

    mXPos = xpos;
    mYPos = ypos;

    mSpeedX = FP_FromInteger(0);

    mFade = bFade;

    if (bFade)
    {
        mSpeedY = FP_FromDouble(-0.7);
    }
    else
    {
        mSpeedY = FP_FromInteger(0);
    }

    mSpriteScale = FP_FromInteger(1); // OG bug - should be using scale ??
    mOtLayer = layer;

    mRGB.SetRGB(0, 0, 0);

    if (bFade)
    {
        mState = ThrowableTotalIndicatorState::eFading;
    }
    else
    {
        mState = ThrowableTotalIndicatorState::eCreated;
    }

    if (count == -1)
    {
        mNumToShow = 10;
    }
    else
    {
        mNumToShow = static_cast<s16>(count);
    }

    if (bFade)
    {
        bThrowableIndicatorExists_504C70++;
    }
}

} // namespace AO

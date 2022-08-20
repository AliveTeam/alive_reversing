#include "stdafx.h"
#include "ThrowableTotalIndicator.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5c112c, u16, bThrowableIndicatorExists_5C112C, 0);

const s16 kNum_0_551994[17] = {
    4,
    -3, -4, 3, -4,
    3, -3, 3, 3,
    3, 4, -3, 4,
    -3, 3, -3, -3};

const s16 kNum_1_5519B8[5] = {
    1,
    2, -4, 2, 4};

const s16 kNum_2_5519C4[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, -1,
    5, 0, -5, 0,
    -5, 1, -5, 3,
    -5, 4, 5, 4};

const s16 kNum_3_5519F0[17] = {
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

const s16 kNum_4_551A14[13] = {
    3,
    -5, -4, -5, -1,
    -5, 0, 4, 0,
    5, -4, 5, 4};

const s16 kNum_5_551A30[21] = {
    5,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 5, 0,
    5, 1, 5, 3,
    5, 4, -5, 4};

const s16 kNum_6_551A5C[21] = {
    5,
    5, -4, -5, -4, -5,
    -3, -5, 3, -5, 4,
    5, 4, 5, 3, 5,
    1, 5, 0, -4, 0};

const s16 kNum_7_551A88[9] = {
    2,
    -5, -4, 5, -4,
    5, -3, 0, 4};

const s16 kNum_8_551A9C[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -5, 3, -5, -3,
    -4, 0, 4, 0};

const s16 kNum_9_551AC8[17] = {
    4,
    5, 4, 5, -3,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 4, 0};

const s16 kInfinity_551AEC[25] = {
    6,
    -3, -2, -5, 0,
    -5, 1, -3, 3,
    -2, 3, 2, -2,
    3, -2, 5, 0,
    5, 1, 3, 3,
    2, 3, -2, -2};

const s16* kNumbersArray[11] = {
    kNum_0_551994,
    kNum_1_5519B8,
    kNum_2_5519C4,
    kNum_3_5519F0,
    kNum_4_551A14,
    kNum_5_551A30,
    kNum_6_551A5C,
    kNum_7_551A88,
    kNum_8_551A9C,
    kNum_9_551AC8,
    kInfinity_551AEC};

ThrowableTotalIndicator::ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP /*scale*/, s32 count, bool bFade)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    SetType(ReliveTypes::eThrowableTotalIndicator);

    gObjListDrawables->Push_Back(this);

    mStartXPos = xpos;
    mStartYPos = ypos;

    mXPos = xpos;
    mYPos = ypos;

    mSpeedX = FP_FromInteger(0);

    mFade = bFade;

    if (mFade)
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

    if (mFade)
    {
        mState = ThrowableTotalIndicatorState::eFading;
    }
    else
    {
        mState = ThrowableTotalIndicatorState::eCreated;
    }

    if (count < 0 || count > 9)
    {
        mNumToShow = 10;
    }
    else
    {
        mNumToShow = static_cast<s16>(count);
    }

    if (bFade)
    {
        bThrowableIndicatorExists_5C112C++;
    }
}

ThrowableTotalIndicator::~ThrowableTotalIndicator()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjListDrawables->Remove_Item(this);
    }

    if (mFade)
    {
        bThrowableIndicatorExists_5C112C--;
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

    if (sNum_CamSwappers_5C1B66 != 0)
    {
        return;
    }

    switch (mState)
    {
        case ThrowableTotalIndicatorState::eCreated:
        {
            mXPos = mStartXPos - (FP_FromInteger(12) * Math_Sine_496DD0(static_cast<u8>(2 * sGnFrame)));
            mYPos = (FP_FromInteger(12) * Math_Cosine_496CD0(static_cast<u8>(2 * sGnFrame))) + mStartYPos;

            const s16 rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine_496DD0(static_cast<u8>(3 * sGnFrame))) + 80;

            mRGB.SetRGB(rgb, rgb, rgb);
        }
        break;

        case ThrowableTotalIndicatorState::eFading:
            if (mYPos >= mStartYPos - FP_FromInteger(20))
            {
                if (mRGB.r < 70 && mRGB.g < 90 && mRGB.b < 20)
                {
                    mRGB.r += 14;
                    mRGB.g += 18;
                    mRGB.b += 4;
                }

                mXPos += mSpeedX;
                mYPos += mSpeedY;
                return;
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

void ThrowableTotalIndicator::VRender(PrimHeader** ppOt)
{
    if (*kNumbersArray[mNumToShow] <= 0)
    {
        return;
    }

    const FP camX = FP_NoFractional(pScreenManager->CamXPos());
    const FP camY = FP_NoFractional(pScreenManager->CamYPos());

    s16 xpos = 0;
    s16 ypos = 0;

    for (s16 counter = 0; counter < kNumbersArray[mNumToShow][0]; counter++)
    {
        // return static_cast<T>(((40 * x) + 11) / 23);
        // Can't use PsxToPCX as this deals with FP's and it adds 11 before the divide ??

        xpos = FP_GetExponent(((mXPos - camX) * FP_FromInteger(40) + FP_FromInteger(11)) / FP_FromInteger(23));
        ypos = FP_GetExponent(mYPos - camY);
        const FP x0 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 1]) * mSpriteScale;
        const FP y0 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 2]) * mSpriteScale;
        const FP x1 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 3]) * mSpriteScale;
        const FP y1 = FP_FromInteger(kNumbersArray[mNumToShow][(4 * counter) + 4]) * mSpriteScale;
        Line_G2* pLine = &mLines[gPsxDisplay.mBufferIndex][counter];

        LineG2_Init(pLine);

        SetXY0(pLine, xpos + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, xpos + FP_GetExponent(x1), ypos + FP_GetExponent(y1));

        SetRGB0(pLine, mRGB.r & 0xFF, mRGB.g & 0xFF, mRGB.b & 0xFF);
        SetRGB1(pLine, mRGB.r & 0xFF, mRGB.g & 0xFF, mRGB.b & 0xFF);

        Poly_Set_SemiTrans(&pLine->mBase.header, TRUE);
        OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pLine->mBase.header);
    }

    Prim_SetTPage* pTPage = &mTPage[gPsxDisplay.mBufferIndex];
    Init_SetTPage(pTPage, 1, 0, PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0));

    OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pTPage->mBase);

    pScreenManager->InvalidateRectCurrentIdx(
        xpos - 8,
        ypos - 8,
        xpos + 8,
        ypos + 8);
}

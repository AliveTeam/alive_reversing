#include "stdafx_ao.h"
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

u16 gThrowableIndicatorExists = 0;
static const s16 kNumber_0[17] = {
    4,
    -3, -4, 3, -4,
    3, -3, 3, 3,
    3, 4, -3, 4,
    -3, 3, -3, -3};

static const s16 kNumber_1[5] = {
    1,
    2, -4, 2, 4};

static const s16 kNumber_2[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, -1,
    5, 0, -5, 0,
    -5, 1, -5, 3,
    -5, 4, 5, 4};

static const s16 kNumber_3[17] = {
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

static const s16 kNumber_4[13] = {
    3,
    -5, -4, -5, -1,
    -5, 0, 4, 0,
    5, -4, 5, 4};

static const s16 kNumber_5[21] = {
    5,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 5, 0,
    5, 1, 5, 3,
    5, 4, -5, 4};

static const s16 kNumber_6[21] = {
    5,
    5, -4, -5, -4, -5,
    -3, -5, 3, -5, 4,
    5, 4, 5, 3, 5,
    1, 5, 0, -4, 0};

static const s16 kNumber_7[9] = {
    2,
    -5, -4, 5, -4,
    5, -3, 0, 4};

static const s16 kNumber_8[21] = {
    5,
    -5, -4, 5, -4,
    5, -3, 5, 3,
    5, 4, -5, 4,
    -5, 3, -5, -3,
    -4, 0, 4, 0};

static const s16 kNumber_9[17] = {
    4,
    5, 4, 5, -3,
    5, -4, -5, -4,
    -5, -3, -5, -1,
    -5, 0, 4, 0};

static const s16 kInfinity[25] = {
    6,
    -3, -2, -5, 0,
    -5, 1, -3, 3,
    -2, 3, 2, -2,
    3, -2, 5, 0,
    5, 1, 3, 3,
    2, 3, -2, -2};

static const s16 kCheckpoint[36] = {
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

static const s16* kNumbersArray[12] = {
    kNumber_0,
    kNumber_1,
    kNumber_2,
    kNumber_3,
    kNumber_4,
    kNumber_5,
    kNumber_6,
    kNumber_7,
    kNumber_8,
    kNumber_9,
    kInfinity,
    kCheckpoint};

ThrowableTotalIndicator::ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP /*scale*/, s32 count, bool bFade)
    : BaseGameObject(true, 0)
{
    SetDrawable(true);
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
        gThrowableIndicatorExists++;
    }
}

ThrowableTotalIndicator::~ThrowableTotalIndicator()
{
    if (GetDrawable())
    {
        gObjListDrawables->Remove_Item(this);
    }

    if (mFade)
    {
        gThrowableIndicatorExists--;
    }
}

void ThrowableTotalIndicator::VScreenChanged()
{
    SetDead(true);
}

void ThrowableTotalIndicator::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (gNumCamSwappers != 0)
    {
        return;
    }

    switch (mState)
    {
        case ThrowableTotalIndicatorState::eCreated:
        {
            mXPos = mStartXPos - (FP_FromInteger(12) * Math_Sine(static_cast<u8>(2 * sGnFrame)));
            mYPos = (FP_FromInteger(12) * Math_Cosine(static_cast<u8>(2 * sGnFrame))) + mStartYPos;

            const s16 rgb = FP_GetExponent(FP_FromInteger(48) * Math_Sine(static_cast<u8>(3 * sGnFrame))) + 80;

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
                SetDead(true);
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

    const FP_Point* camPos = gScreenManager->mCamPos;
    const FP camX = FP_FromInteger(FP_GetExponent(camPos->x - FP_FromInteger(gScreenManager->mCamXOff)));
    const FP camY = FP_FromInteger(FP_GetExponent(camPos->y - FP_FromInteger(gScreenManager->mCamYOff)));

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

        Line_G2* pLine = &mLines[gPsxDisplay.mBufferIndex][counter];
        LineG2_Init(pLine);

        SetXY0(pLine, primBaseX + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        SetXY1(pLine, primVertX + FP_GetExponent(x1), ypos + FP_GetExponent(y1));
        SetRGB0(pLine, static_cast<u8>(mRGB.r), static_cast<u8>(mRGB.g), static_cast<u8>(mRGB.b));
        SetRGB1(pLine, static_cast<u8>(mRGB.r), static_cast<u8>(mRGB.g), static_cast<u8>(mRGB.b));
        SetRGB2(pLine, static_cast<u8>(mRGB.r), static_cast<u8>(mRGB.g), static_cast<u8>(mRGB.b));


        Poly_Set_SemiTrans(&pLine->mBase.header, 1);
        OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pLine->mBase.header);
    }

    Init_SetTPage(&mTPage[gPsxDisplay.mBufferIndex], PSX_getTPage(TPageAbr::eBlend_1));
    OrderingTable_Add(OtLayer(ppOt, mOtLayer), &mTPage->mBase);
}


} // namespace AO

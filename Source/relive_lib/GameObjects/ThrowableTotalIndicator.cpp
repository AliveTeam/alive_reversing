#include "stdafx.h"
#include "ThrowableTotalIndicator.hpp"
#include "../../AliveLibAE/Game.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameType.hpp"

u16 gThrowableIndicatorExists = 0;

struct LinePoint final
{
    s32 mX1 = 0;
    s32 mY1 = 0;
    s32 mX2 = 0;
    s32 mY2 = 0;
};

struct Glyph final
{
    u32 mPointsCount = 0;
    const LinePoint* mPoints = nullptr;
};

static const LinePoint kNumber_0[] = 
{
    { -3, -4,3, -4 },
    { 3, -3 ,3, 3 },
    { 3, 4,-3, 4 },
    { -3, 3,-3, -3}
};

static const LinePoint kNumber_1[] = 
{
    {2, -4, 2, 4}
};

static const LinePoint kNumber_2[] = 
{
    {-5, -4, 5, -4},
    {5, -3, 5, -1},
    {5, 0, -5, 0},
    {-5, 1, -5, 3},
    {-5, 4, 5, 4}
};

static const LinePoint kNumber_3[] = 
{
    {-5, -4, 5, -4},
    {5, -3, 5, 3},
    {5, 4, -5, 4},
    {-4, 0, 4, 0},
};

static const LinePoint kNumber_4[] = 
{
    {-5, -4, -5, -1},
    {-5, 0, 4, 0},
    {5, -4, 5, 4}
};

static const LinePoint kNumber_5[] = {
    {5, -4, -5, -4},
    {-5, -3, -5, -1},
    {-5, 0, 5, 0},
    {5, 1, 5, 3},
    {5, 4, -5, 4}
};

static const LinePoint kNumber_6[] = 
{
    {5, -4, -5, -4},
    {-5, -3, -5, 3},
    {-5, 4, 5, 4},
    {5, 3, 5, 1},
    {5, 0, -4, 0}
};

static const LinePoint kNumber_7[] = 
{
    {-5, -4, 5, -4},
    {5, -3, 0, 4}
};

static const LinePoint kNumber_8[] = 
{
    {-5, -4, 5, -4},
    {5, -3, 5, 3},
    {5, 4, -5, 4},
    {-5, 3, -5, -3},
    {-4, 0, 4, 0}
};

static const LinePoint kNumber_9[] = 
{
    {5, 4, 5, -3},
    {5, -4, -5, -4},
    {-5, -3, -5, -1},
    {-5, 0, 4, 0}
};

static const LinePoint kInfinity[] = 
{
    {-3, -2, -5, 0},
    {-5, 1, -3, 3},
    {-2, 3, 2, -2},
    {3, -2, 5, 0},
    {5, 1, 3, 3},
    {2, 3, -2, -2}
};

static const LinePoint kCheckpoint[] = 
{
    {0, -6, 1, -6},
    {12, 0, 13, 0},
    {0, 6, 1, 6},
    {-11, 0, -12, 0},
    {2, -5, 11, -1},
    {11, 1, 2, 5},
    {-1, 5, -10, 1},
    {-10, -1, -1, -5}
};

static const Glyph sGlyphs[] = {
    {ALIVE_COUNTOF(kNumber_0), kNumber_0},
    {ALIVE_COUNTOF(kNumber_1), kNumber_1},
    {ALIVE_COUNTOF(kNumber_2), kNumber_2},
    {ALIVE_COUNTOF(kNumber_3), kNumber_3},
    {ALIVE_COUNTOF(kNumber_4), kNumber_4},
    {ALIVE_COUNTOF(kNumber_5), kNumber_5},
    {ALIVE_COUNTOF(kNumber_6), kNumber_6},
    {ALIVE_COUNTOF(kNumber_7), kNumber_7},
    {ALIVE_COUNTOF(kNumber_8), kNumber_8},
    {ALIVE_COUNTOF(kNumber_9), kNumber_9},
    {ALIVE_COUNTOF(kInfinity), kInfinity},
    {ALIVE_COUNTOF(kCheckpoint), kCheckpoint},
};


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

    if ((GetGameType() == GameType::eAo && count == -1) ||
        (GetGameType() == GameType::eAe && (count < 0 || count > 9)))
    {
        mNumToShow = 10;
    }
    else
    {
        mNumToShow = count;
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
    if (EventGet(Event::kEventDeathReset))
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

void ThrowableTotalIndicator::VRender(OrderingTable& ot)
{
    FP camX = FP_NoFractional(gScreenManager->CamXPos());
    FP camY = FP_NoFractional(gScreenManager->CamYPos());

    if (GetGameType() == GameType::eAe)
    {
        camX = FP_NoFractional(camX);
        camY = FP_NoFractional(camY);
    }

    s16 xpos = 0;
    s16 ypos = 0;

    const Glyph& pointData = sGlyphs[mNumToShow];
    for (u32 counter = 0; counter < pointData.mPointsCount; counter++)
    {
        xpos = FP_GetExponent(mXPos - camX);
        ypos = FP_GetExponent(mYPos - camY);

        const FP x0 = FP_FromInteger(pointData.mPoints[counter].mX1) * mSpriteScale;
        const FP y0 = FP_FromInteger(pointData.mPoints[counter].mY1) * mSpriteScale;
        const FP x1 = FP_FromInteger(pointData.mPoints[counter].mX2) * mSpriteScale;
        const FP y1 = FP_FromInteger(pointData.mPoints[counter].mY2) * mSpriteScale;

        s16 BasePrimitiveX = 0;
        if (mNumToShow == 11) // CheckPoint
        {
            BasePrimitiveX = PsxToPCX(xpos);
        }
        else
        {
            BasePrimitiveX = PsxToPCX(xpos, 11);
        }

        Line_G2* pLine = &mLines[counter];

        pLine->SetXY0(BasePrimitiveX + FP_GetExponent(x0), ypos + FP_GetExponent(y0));
        pLine->SetXY1(BasePrimitiveX + FP_GetExponent(x1), ypos + FP_GetExponent(y1));

        pLine->SetRGB0(static_cast<u8>(mRGB.r), static_cast<u8>(mRGB.g), static_cast<u8>(mRGB.b));
        pLine->SetRGB1(static_cast<u8>(mRGB.r), static_cast<u8>(mRGB.g), static_cast<u8>(mRGB.b));
        pLine->SetRGB2(static_cast<u8>(mRGB.r), static_cast<u8>(mRGB.g), static_cast<u8>(mRGB.b));

        pLine->SetSemiTransparent(true);
        pLine->SetBlendMode(relive::TBlendModes::eBlend_1);
        ot.Add(mOtLayer, pLine);
    }
}

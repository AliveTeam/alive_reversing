#include "stdafx.h"
#include "ColourfulMeter.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "MinesAlarm.hpp"
#include "Path.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"

u8 gTotalMeterBars = 0;
bool gbDrawMeterCountDown = false;

ColourfulMeter::ColourfulMeter(relive::Path_ColourfulMeter* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0),
    mTlvInfo(tlvId),
    mTlvX(pTlv->mTopLeftX),
    mTlvY(pTlv->mTopLeftY),
    mPal(ResourceManagerWrapper::LoadPal(PalId::LedFont_ColourfulMeter)),
    mSwitchId(pTlv->mSwitchId),
    mNumberOfMeterBars(pTlv->mNumberOfMeterBars)
{
    SetType(ReliveTypes::eColourfulMeter);

    mFontContext.LoadFontType(FontType::LcdFont);
    mFont.Load(5, mPal, &mFontContext);
    SetDrawable(true);
    gObjListDrawables->Push_Back(this);

    mTextX = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftX)) - gScreenManager->CamXPos());
    mTextY = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftY)) - gScreenManager->CamYPos());

    mBarCount = kMeterBarsXCount / mNumberOfMeterBars + 1;

    if (mNumberOfMeterBars == 4)
    {
        mBarCount = 5;
    }
    else if (mNumberOfMeterBars == 5)
    {
        mBarCount = 4;
    }

    mStartingSwitchState = static_cast<s16>(SwitchStates_Get(mSwitchId));
    mStartFilled = pTlv->mStartFilled;
    mMinesAlarmCountdown = pTlv->mMinesAlarmCountdown;

    if (mStartFilled)
    {
        if (mStartingSwitchState)
        {
            mPolysToRenderCount = 0;
        }
        else
        {
            mPolysToRenderCount = kMeterBarsXCount;
        }
    }
    else
    {
        mPolysToRenderCount = mBarCount * gTotalMeterBars;
    }
}

ColourfulMeter::~ColourfulMeter()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvInfo);
}

void ColourfulMeter::VScreenChanged()
{
    SetDead(true);

    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        gTotalMeterBars = 0;
    }
}

void ColourfulMeter::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
        gTotalMeterBars = 0;
    }

    if (EventGet(kEventDeathResetEnd))
    {
        gbDrawMeterCountDown = false;
    }

    if (mStartFilled)
    {
        gbDrawMeterCountDown = false;

        if (SwitchStates_Get(mSwitchId))
        {
            if (mPolysToRenderCount > 0)
            {
                if (mCount == 0)
                {
                    mCount = 15;
                    mPolysToRenderCount--;
                }
                mCount--;
            }
        }
    }
    else
    {
        if (!mStartingSwitchState)
        {
            if (SwitchStates_Get(mSwitchId))
            {
                gTotalMeterBars++;
                mStartingSwitchState = 1;
            }
        }

        if (mPolysToRenderCount < gTotalMeterBars * mBarCount)
        {
            if (mCount == 0)
            {
                mCount = 15;
                mPolysToRenderCount++;
            }
            mCount--;
        }

        if (mNumberOfMeterBars > gTotalMeterBars)
        {
            gbDrawMeterCountDown = false;
        }
        else if (!gbDrawMeterCountDown)
        {
            gbDrawMeterCountDown = true;
            MinesAlarm::Create(30 * mMinesAlarmCountdown);
        }
    }
}

static const PSX_Point sMeterBarsInfo[kMeterBarsXCount] = {
    {0, 9},
    {2, 7},
    {4, 5},
    {6, 4},
    {8, 3},
    {10, 2},
    {13, 1},
    {15, 1},
    {17, 0},
    {20, 0},
    {22, 0},
    {25, 0},
    {27, 1},
    {29, 1},
    {32, 2},
    {34, 3},
    {36, 4},
    {38, 5},
    {40, 7},
    {42, 9},
};

void ColourfulMeter::VRender(OrderingTable& ot)
{
    const s16 screenXOff = FP_GetExponent(gScreenManager->CamXPos() + FP_FromInteger(4));
    const s16 screenYOff = FP_GetExponent(gScreenManager->CamYPos() + FP_FromInteger(4));

    for (s16 poly_idx = 0; poly_idx < mPolysToRenderCount && poly_idx < kMeterBarsXCount - 1; poly_idx++)
    {
        Poly_G4* pPolyG4 = &mPolyG4s[poly_idx];

        const s16 x0 = mTlvX + (FP_GetExponent(FP_FromInteger(sMeterBarsInfo[poly_idx].x))) - screenXOff;
        const s16 y0 = FP_GetExponent(FP_FromInteger(sMeterBarsInfo[poly_idx].y)) - screenYOff + mTlvY - 20;

        const s16 x1 = mTlvX + ((poly_idx + 1) * 2) - screenXOff;
        const s16 y1 = mTlvY - screenYOff - 5;

        const s16 x2 = mTlvX + FP_GetExponent(FP_FromInteger(sMeterBarsInfo[poly_idx + 1].x)) - screenXOff;
        const s16 y2 = FP_GetExponent(FP_FromInteger(sMeterBarsInfo[poly_idx + 1].y)) - screenYOff + mTlvY - 20;

        pPolyG4->SetXY0(static_cast<s16>(PsxToPCX(x0)), y0);
        pPolyG4->SetXY1(static_cast<s16>(PsxToPCX(x1)), y1);
        pPolyG4->SetXY2(static_cast<s16>(PsxToPCX(x2)), y2);
        pPolyG4->SetXY3(static_cast<s16>(PsxToPCX(x1 + 2)), y1);

        if (((poly_idx + 1)) >= (kMeterBarsXCount / 2) + 1)
        {
            // 1st half
            pPolyG4->SetRGB0(127, static_cast<u8>(240 - 12 * poly_idx), 0);
            pPolyG4->SetRGB1(127, static_cast<u8>(240 - 12 * poly_idx), 0);
            pPolyG4->SetRGB2(127, static_cast<u8>(228 - 12 * poly_idx), 0);
            pPolyG4->SetRGB3(127, static_cast<u8>(228 - 12 * poly_idx), 0);
        }
        else
        {
            // 2nd half
            pPolyG4->SetRGB0(static_cast<u8>(12 * poly_idx), 127, 0);
            pPolyG4->SetRGB1(static_cast<u8>(12 * poly_idx), 127, 0);
            pPolyG4->SetRGB2(static_cast<u8>(12 * (poly_idx + 1)), 127, 0);
            pPolyG4->SetRGB3(static_cast<u8>(12 * (poly_idx + 1)), 127, 0);
        }

        pPolyG4->SetSemiTransparent(false);
        ot.Add(Layer::eLayer_Well_23, pPolyG4);
    }

    if (gbDrawMeterCountDown)
    {
        char_type text[12] = {};
        sprintf(text, "%01d:%02d", gExplosionTimer / 1800u, gExplosionTimer / 30u % 60);
        const s32 textWidth = mFont.MeasureTextWidth(text);

        s16 colourRand = 50;
        if (gDisableFontFlicker)
        {
            colourRand = 0;
        }

        mFont.DrawString(
            ot,
            text,
            mTextX + 1,
            mTextY - 5,
            relive::TBlendModes::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127, 127, 127,
            0,
            FP_FromInteger(1),
            mTextX + textWidth,
            colourRand);
    }
}

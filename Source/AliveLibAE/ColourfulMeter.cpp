#include "stdafx.h"
#include "ColourfulMeter.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "MinesAlarm.hpp"

const u8 byte_5543D0[32] = {
    0u, 0u, 1u, 128u, 1u, 132u, 32u, 132u, 33u,
    128u, 32u, 132u, 33u, 132u, 101u, 206u, 101u, 140u,
    140u, 177u, 19u, 148u, 100u, 206u, 101u, 206u, 215u,
    152u, 20u, 161u, 24u, 216u};

ALIVE_VAR(1, 0x5c1bfa, u8, gTotalMeterBars_5C1BFA, 0);
ALIVE_VAR(1, 0x5c1bf8, u16, gbDrawMeterCountDown_5C1BF8, 0);

ColourfulMeter::ColourfulMeter(relive::Path_ColourfulMeter* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eColourfulMeter);
    mTlvInfo = tlvInfo;

    mTlvX = pTlv->mTopLeftX;
    mTlvY = pTlv->mTopLeftY;

    field_20_font_context.LoadFontType_433400(2);
    field_30_font.ctor_433590(5, byte_5543D0, &field_20_font_context);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    mTextX = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftX)) - pScreenManager->CamXPos());
    mTextY = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftY)) - pScreenManager->CamYPos());

    mSwitchId = pTlv->mSwitchId;
    field_80 = 0;
    mNumberOfMeterBars = pTlv->mNumberOfMeterBars;
    field_82_bar_count = kMeterBarsXCount / mNumberOfMeterBars + 1;

    if (mNumberOfMeterBars == 4)
    {
        field_82_bar_count = 5;
    }
    else if (mNumberOfMeterBars == 5)
    {
        field_82_bar_count = 4;
    }

    field_78_count = 15;

    mStartingSwitchState = static_cast<s16>(SwitchStates_Get(mSwitchId));
    mStartFilled = pTlv->mStartFilled;
    mMinesAlarmCountdown = pTlv->mMinesAlarmCountdown;

    if (mStartFilled == relive::reliveChoice::eYes)
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
        mPolysToRenderCount = field_82_bar_count * gTotalMeterBars_5C1BFA;
    }
}

ColourfulMeter::~ColourfulMeter()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    field_30_font.dtor_433540();
    field_20_font_context.dtor_433510();
}

void ColourfulMeter::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        gTotalMeterBars_5C1BFA = 0;
    }
}

void ColourfulMeter::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        gTotalMeterBars_5C1BFA = 0;
    }

    if (EventGet(kEventDeathResetEnd))
    {
        gbDrawMeterCountDown_5C1BF8 = 0;
    }

    if (mStartFilled == relive::reliveChoice::eYes)
    {
        gbDrawMeterCountDown_5C1BF8 = 0;

        if (SwitchStates_Get(mSwitchId))
        {
            if (mPolysToRenderCount > 0)
            {
                if (field_78_count == 0)
                {
                    field_78_count = 15;
                    mPolysToRenderCount--;
                }
                field_78_count--;
            }
        }
    }
    else
    {
        if (!mStartingSwitchState)
        {
            if (SwitchStates_Get(mSwitchId))
            {
                gTotalMeterBars_5C1BFA++;
                field_80 = 1;
                mStartingSwitchState = 1;
            }
        }

        if (mPolysToRenderCount < gTotalMeterBars_5C1BFA * field_82_bar_count)
        {
            if (field_78_count == 0)
            {
                field_78_count = 15;
                mPolysToRenderCount++;
            }
            field_78_count--;
        }

        if (mNumberOfMeterBars > gTotalMeterBars_5C1BFA)
        {
            gbDrawMeterCountDown_5C1BF8 = 0;
        }
        else if (!gbDrawMeterCountDown_5C1BF8)
        {
            gbDrawMeterCountDown_5C1BF8 = 1;
            MinesAlarm::Create(30 * mMinesAlarmCountdown);
        }
    }
}

const PSX_Point stru_5543F0[kMeterBarsXCount] = {
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

void ColourfulMeter::VRender(PrimHeader** ppOt)
{
    const s16 screenXOff = FP_GetExponent(pScreenManager->CamXPos() + FP_FromInteger(4));
    const s16 screenYOff = FP_GetExponent(pScreenManager->CamYPos() + FP_FromInteger(4));

    for (s16 poly_idx = 0; poly_idx < mPolysToRenderCount && poly_idx < kMeterBarsXCount - 1; poly_idx++)
    {
        Poly_G4* pPolyG4 = &field_88_polyG4s[gPsxDisplay.mBufferIndex][poly_idx];
        PolyG4_Init(pPolyG4);

        const s16 x0 = mTlvX + (FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx].x))) - screenXOff;
        const s16 y0 = FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx].y)) - screenYOff + mTlvY - 20;

        const s16 x1 = mTlvX + ((poly_idx + 1) * 2) - screenXOff;
        const s16 y1 = mTlvY - screenYOff - 5;

        const s16 x2 = mTlvX + FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx + 1].x)) - screenXOff;
        const s16 y2 = FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx + 1].y)) - screenYOff + mTlvY - 20;

        SetXY0(pPolyG4, static_cast<s16>(PsxToPCX(x0)), y0);
        SetXY1(pPolyG4, static_cast<s16>(PsxToPCX(x1)), y1);
        SetXY2(pPolyG4, static_cast<s16>(PsxToPCX(x2)), y2);
        SetXY3(pPolyG4, static_cast<s16>(PsxToPCX(x1 + 2)), y1);

        if (((poly_idx + 1)) >= (kMeterBarsXCount / 2) + 1)
        {
            // 1st half
            SetRGB0(pPolyG4, 127, static_cast<u8>(240 - 12 * poly_idx), 0);
            SetRGB1(pPolyG4, 127, static_cast<u8>(240 - 12 * poly_idx), 0);
            SetRGB2(pPolyG4, 127, static_cast<u8>(228 - 12 * poly_idx), 0);
            SetRGB3(pPolyG4, 127, static_cast<u8>(228 - 12 * poly_idx), 0);
        }
        else
        {
            // 2nd half
            SetRGB0(pPolyG4, static_cast<u8>(12 * poly_idx), 127, 0);
            SetRGB1(pPolyG4, static_cast<u8>(12 * poly_idx), 127, 0);
            SetRGB2(pPolyG4, static_cast<u8>(12 * (poly_idx + 1)), 127, 0);
            SetRGB3(pPolyG4, static_cast<u8>(12 * (poly_idx + 1)), 127, 0);
        }

        Poly_Set_SemiTrans(&pPolyG4->mBase.header, FALSE);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Well_23), &pPolyG4->mBase.header);
    }

    if (gbDrawMeterCountDown_5C1BF8)
    {
        char_type text[12] = {};
        sprintf(text, "%01d:%02d", sTimerValue_5C1BFC / 1800u, sTimerValue_5C1BFC / 30u % 60);
        const s32 textWidth = field_30_font.MeasureTextWidth(text);

        s16 colourRand = 50;
        if (sDisableFontFlicker_5C9304)
        {
            colourRand = 0;
        }

        field_30_font.DrawString_4337D0(
            ppOt,
            text,
            mTextX + 1,
            mTextY - 5,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127, 127, 127,
            0,
            FP_FromInteger(1),
            mTextX + textWidth,
            colourRand);
    }

    pScreenManager->InvalidateRectCurrentIdx(
        PsxToPCX(mTextX - 50),
        mTextY - 30,
        PsxToPCX(mTextX + 500),
        mTextY);
}

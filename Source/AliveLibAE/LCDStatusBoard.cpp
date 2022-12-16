#include "stdafx.h"
#include "LCDStatusBoard.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"
#include "DDCheat.hpp" // only mud info
#include "Path.hpp"
#include "../relive_lib/FatalError.hpp"
#include <algorithm>
#include "../relive_lib/ScreenManager.hpp"

s16 gMudokonsInArea = 0;
s8 gZulagNumber = 0;

LCDStatusBoard::LCDStatusBoard(relive::Path_LCDStatusBoard* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvId = tlvId;

    mFontContext.LoadFontType(FontType::LcdFont);

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);

    mKilledMudsFont.Load(3, mPal, &mFontContext);
    mRescuedMudsFont.Load(3, mPal, &mFontContext);
    mMudsInLevelFont.Load(3, mPal, &mFontContext);
    mMudsInAreaFont.Load(3, mPal, &mFontContext);

    SetDrawable(true);
    gObjListDrawables->Push_Back(this);
    mXPos = FP_GetExponent(FP_FromInteger(static_cast<s32>(pTlv->mTopLeftX)) - gScreenManager->CamXPos());
    mYPos = FP_GetExponent(FP_FromInteger(static_cast<s32>(pTlv->mTopLeftY)) - gScreenManager->CamYPos());
    gMudokonsInArea = pTlv->mNumberOfMuds;
    mHideBoard = static_cast<s16>(pTlv->mHideBoard);
    gZulagNumber = static_cast<s8>(pTlv->mZulagNumber);
    if (gZulagNumber > ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData))
    {
        ALIVE_FATAL("sZulagNumber_5C1A20 out of bounds %d max is 20. Don't set your zulag number to > 20", gZulagNumber);
    }
}

LCDStatusBoard::~LCDStatusBoard()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void LCDStatusBoard::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

// Todo: clean up
void LCDStatusBoard::VRender(PrimHeader** ppOt)
{
    if (!mHideBoard)
    {
        char_type text[12] = {};
        sprintf(text, "%3d", Path_GetMudsInLevel(gMap.mCurrentLevel, gMap.mCurrentPath));
        s32 maxWidth = mMudsInLevelFont.MeasureTextWidth(text);

        s16 flickerAmount = 50; // ax
        if (gDisableFontFlicker)
        {
            flickerAmount = 0;
        }

        // Muds In This Level
        mMudsInLevelFont.DrawString(
            ppOt,
            text,
            mXPos - maxWidth + 33,
            mYPos,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            mXPos + maxWidth,
            flickerAmount);
        const s16 mudsLeftInArea = gMudokonsInArea - sSavedKilledMudsPerZulag_5C1B50.mData[gZulagNumber];
        mMudsLeftInArea = mudsLeftInArea;

        // Muds in this Area
        sprintf(text, "%3d", mudsLeftInArea);
        const s32 font4Width = mMudsInAreaFont.MeasureTextWidth(text);
        mMudsInAreaFont.DrawString(
            ppOt,
            text,
            mXPos - font4Width + 33,
            mYPos + 16,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            mXPos + font4Width,
            flickerAmount);
        maxWidth = std::max(font4Width, maxWidth);

        // Rescued Mudokons
        sprintf(text, "%3d", gRescuedMudokons);
        const s32 font2Width = mRescuedMudsFont.MeasureTextWidth(text);
        mRescuedMudsFont.DrawString(
            ppOt,
            text,
            mXPos - font2Width + 33,
            mYPos + 32,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            mXPos + font2Width,
            flickerAmount);
        maxWidth = std::max(font2Width, maxWidth);

        // Killed mudokons
        sprintf(text, "%3d", gKilledMudokons);
        const s32 font1Width = mKilledMudsFont.MeasureTextWidth(text);
        mKilledMudsFont.DrawString(
            ppOt,
            text,
            mXPos - font1Width + 33,
            mYPos + 48,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            mXPos + font1Width,
            flickerAmount);
        maxWidth = std::max(font1Width, maxWidth);
    }
}

void LCDStatusBoard::VScreenChanged()
{
    SetDead(true);
}

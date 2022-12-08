#include "stdafx_ao.h"
#include "LCDStatusBoard.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "DDCheat.hpp"
#include "PathDataExtensions.hpp"
#include "Path.hpp"
#include "Map.hpp"

namespace AO {

void LCDStatusBoard::VScreenChanged()
{
    SetDead(true);
}

LCDStatusBoard::~LCDStatusBoard()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

LCDStatusBoard::LCDStatusBoard(relive::Path_LCDStatusBoard* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvId = tlvId;
    mFontContext.LoadFontType(FontType::LcdFont);

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);

    mKilledMudsFont.Load(3, mPal, &mFontContext);
    mRescuedMudsFont.Load(3, mPal, &mFontContext);
    mEmployeesFont.Load(3, mPal, &mFontContext);

    SetDrawable(true);
    gObjListDrawables->Push_Back(this);

    mXPos = (pScreenManager->mCamXOff + pTlv->mTopLeftX) - FP_GetExponent(pScreenManager->mCamPos->x);
    mYPos = (pScreenManager->mCamYOff + pTlv->mTopLeftY) - FP_GetExponent(pScreenManager->mCamPos->y);
}

void LCDStatusBoard::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

void LCDStatusBoard::VRender(PrimHeader** ppOt)
{
    char_type text[12] = {};
    sprintf(text, "%02d", Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath) - sRescuedMudokons - sKilledMudokons);

    const s16 w1 = static_cast<s16>(mEmployeesFont.MeasureTextWidth(text));
    const s16 colourRange = sDisableFontFlicker ? 0 : 50;
    mEmployeesFont.DrawString(
        ppOt,
        text,
        mXPos - w1 + 22,
        mYPos,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w1 + mXPos,
        colourRange);

    sprintf(text, "%02d", sKilledMudokons);
    const s16 w2 = static_cast<s16>(mKilledMudsFont.MeasureTextWidth(text));
    mKilledMudsFont.DrawString(
        ppOt,
        text,
        mXPos - w2 + 22,
        mYPos + 16,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w2 + mXPos,
        colourRange);

    sprintf(text, "%02d", sRescuedMudokons);
    s16 w3 = static_cast<s16>(mRescuedMudsFont.MeasureTextWidth(text));
    mRescuedMudsFont.DrawString(
        ppOt,
        text,
        mXPos - w3 + 22,
        mYPos + 32,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w3 + mXPos,
        colourRange);

    s32 biggestW = w1;
    if (w2 > biggestW)
    {
        biggestW = w2;
    }
    if (w3 > biggestW)
    {
        biggestW = w3;
    }
}

} // namespace AO

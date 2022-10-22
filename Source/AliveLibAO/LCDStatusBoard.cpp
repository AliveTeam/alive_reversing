#include "stdafx_ao.h"
#include "Function.hpp"
#include "LCDStatusBoard.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp" // mud info
#include "DDCheat.hpp"
#include "PathDataExtensions.hpp"
#include "Path.hpp"

namespace AO {

void LCDStatusBoard::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

LCDStatusBoard::~LCDStatusBoard()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_C8_tlv, -1, 0, 0);
}

LCDStatusBoard::LCDStatusBoard(relive::Path_LCDStatusBoard* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    field_C8_tlv = tlvId;
    field_10_fontContext.LoadFontType(FontType::LcdFont);

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);

    field_20_font1.Load(3, mPal, &field_10_fontContext);
    field_58_font2.Load(3, mPal, &field_10_fontContext);
    field_90_font3.Load(3, mPal, &field_10_fontContext);

    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    field_CC_xpos = (pScreenManager->mCamXOff + pTlv->mTopLeftX) - FP_GetExponent(pScreenManager->mCamPos->x);
    field_CE_ypos = (pScreenManager->mCamYOff + pTlv->mTopLeftY) - FP_GetExponent(pScreenManager->mCamPos->y);
}

void LCDStatusBoard::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void LCDStatusBoard::VRender(PrimHeader** ppOt)
{
    char_type text[12] = {};
    sprintf(text, "%02d", Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath) - sRescuedMudokons_5076C0 - sKilledMudokons_5076BC);

    const s16 w1 = static_cast<s16>(field_90_font3.MeasureTextWidth(text));
    const s16 colourRange = sDisableFontFlicker ? 0 : 50;

    field_90_font3.DrawString(
        ppOt,
        text,
        field_CC_xpos - w1 + 22,
        field_CE_ypos,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w1 + field_CC_xpos,
        colourRange);

    sprintf(text, "%02d", sKilledMudokons_5076BC);
    const s16 w2 = static_cast<s16>(field_20_font1.MeasureTextWidth(text));
    field_20_font1.DrawString(
        ppOt,
        text,
        field_CC_xpos - w2 + 22,
        field_CE_ypos + 16,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w2 + field_CC_xpos,
        colourRange);

    sprintf(text, "%02d", sRescuedMudokons_5076C0);
    s16 w3 = static_cast<s16>(field_58_font2.MeasureTextWidth(text));
    field_58_font2.DrawString(
        ppOt,
        text,
        field_CC_xpos - w3 + 22,
        field_CE_ypos + 32,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w3 + field_CC_xpos,
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

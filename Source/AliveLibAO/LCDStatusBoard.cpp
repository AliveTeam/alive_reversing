#include "stdafx_ao.h"
#include "Function.hpp"
#include "LCDStatusBoard.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "DDCheat.hpp"
#include "PathDataExtensions.hpp"

namespace AO {

const u8 sStatsSignFontPalette_4CD570[32] = {
    0u,
    0u,
    1u,
    128u,
    1u,
    132u,
    32u,
    132u,
    33u,
    128u,
    32u,
    132u,
    33u,
    132u,
    101u,
    206u,
    101u,
    140u,
    140u,
    177u,
    19u,
    148u,
    100u,
    206u,
    101u,
    206u,
    215u,
    152u,
    20u,
    161u,
    24u,
    216u};

void LCDStatusBoard::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

LCDStatusBoard::~LCDStatusBoard()
{
    gObjList_drawables_504618->Remove_Item(this);
    gMap.TLV_Reset(field_C8_tlv, -1, 0, 0);
}

LCDStatusBoard::LCDStatusBoard(Path_LCDStatusBoard* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    field_C8_tlv = tlvInfo;
    field_10_fontContext.LoadFontType(2);
    field_20_font1.Load(3, sStatsSignFontPalette_4CD570, &field_10_fontContext);
    field_58_font2.Load(3, sStatsSignFontPalette_4CD570, &field_10_fontContext);
    field_90_font3.Load(3, sStatsSignFontPalette_4CD570, &field_10_fontContext);

    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    gObjList_drawables_504618->Push_Back(this);

    field_CC_xpos = (pScreenManager_4FF7C8->field_14_xpos + pTlv->field_10_top_left.field_0_x) - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);
    field_CE_ypos = (pScreenManager_4FF7C8->field_16_ypos + pTlv->field_10_top_left.field_2_y) - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);
}

void LCDStatusBoard::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void LCDStatusBoard::VRender(PrimHeader** ppOt)
{
    char_type text[12] = {};
    sprintf(text, "%02d", Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath) - sRescuedMudokons_5076C0 - sKilledMudokons_5076BC);

    const s16 w1 = static_cast<s16>(field_90_font3.MeasureTextWidth(text));
    const s16 colourRange = sDisableFontFlicker_5080E4 ? 0 : 50;

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

    pScreenManager_4FF7C8->InvalidateRect(
        field_CC_xpos,
        field_CE_ypos,
        biggestW,
        48,
        pScreenManager_4FF7C8->field_2E_idx);
}

} // namespace AO

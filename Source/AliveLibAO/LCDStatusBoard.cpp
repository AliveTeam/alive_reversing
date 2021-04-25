#include "stdafx_ao.h"
#include "Function.hpp"
#include "LCDStatusBoard.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "DDCheat.hpp"

namespace AO {

const u8 sStatsSignFontPalette_4CD570[32] =
{
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
    216u
};

void LCDStatusBoard::VUpdate()
{
    VUpdate_441A90();
}

void LCDStatusBoard::VScreenChanged_441C70()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void LCDStatusBoard::VScreenChanged()
{
    VScreenChanged_441C70();
}

LCDStatusBoard* LCDStatusBoard::Vdtor_441C80(s32 flags)
{
    dtor_4419E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* LCDStatusBoard::VDestructor(s32 flags)
{
    return Vdtor_441C80(flags);
}

BaseGameObject* LCDStatusBoard::dtor_4419E0()
{
    SetVTable(this, 0x4BBB90);
    gObjList_drawables_504618->Remove_Item(this);
    gMap_507BA8.TLV_Reset_446870(field_C8_tlv, -1, 0, 0);
    field_90_font3.dtor_41C130();
    field_58_font2.dtor_41C130();
    field_20_font1.dtor_41C130();
    field_10_fontContext.dtor_41C110();
    return dtor_487DF0();
}

LCDStatusBoard* LCDStatusBoard::ctor_4418E0(Path_LCDStatusBoard* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBB90);
    field_C8_tlv = tlvInfo;
    field_10_fontContext.LoadFontType_41C040(2);
    field_20_font1.ctor_41C170(3, sStatsSignFontPalette_4CD570, &field_10_fontContext);
    field_58_font2.ctor_41C170(3, sStatsSignFontPalette_4CD570, &field_10_fontContext);
    field_90_font3.ctor_41C170(3, sStatsSignFontPalette_4CD570, &field_10_fontContext);

    field_6_flags.Set(Options::eDrawable_Bit4);
    gObjList_drawables_504618->Push_Back(this);

    field_CC_xpos = (pScreenManager_4FF7C8->field_14_xpos + pTlv->field_10_top_left.field_0_x) - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);
    field_CE_ypos = (pScreenManager_4FF7C8->field_16_ypos + pTlv->field_10_top_left.field_2_y) - FP_GetExponent
        (pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);
    return this;
}

void LCDStatusBoard::VUpdate_441A90()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void LCDStatusBoard::VRender(PrimHeader** ppOt)
{
    VRender_441AB0(ppOt);
}

void LCDStatusBoard::VRender_441AB0(PrimHeader** ppOt)
{
    s8 text[12] = {};
    sprintf(text, "%02d", 99 - sRescuedMudokons_5076C0 - sKilledMudokons_5076BC);

    const s16 w1 = static_cast<s16>(field_90_font3.MeasureWidth_41C2B0(text));
    const s16 colourRange = sDisableFontFlicker_5080E4 ? 0 : 50;

    field_90_font3.DrawString_41C360(
        ppOt,
        text,
        field_CC_xpos - w1 + 22,
        field_CE_ypos,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w1 + field_CC_xpos,
        colourRange);

    sprintf(text, "%02d", sKilledMudokons_5076BC);
    const s16 w2 = static_cast<s16>(field_20_font1.MeasureWidth_41C2B0(text));
    field_20_font1.DrawString_41C360(
        ppOt,
        text,
        field_CC_xpos - w2 + 22,
        field_CE_ypos + 16,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        w2 + field_CC_xpos,
        colourRange);

    sprintf(text, "%02d", sRescuedMudokons_5076C0);
    s16 w3 = static_cast<s16>(field_58_font2.MeasureWidth_41C2B0(text));
    field_58_font2.DrawString_41C360(
        ppOt,
        text,
        field_CC_xpos - w3 + 22,
        field_CE_ypos + 32,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_22,
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

    pScreenManager_4FF7C8->InvalidateRect_406E40(
        field_CC_xpos,
        field_CE_ypos,
        biggestW,
        48,
        pScreenManager_4FF7C8->field_2E_idx);
}

}

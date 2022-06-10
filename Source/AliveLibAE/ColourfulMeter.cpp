#include "stdafx.h"
#include "ColourfulMeter.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "MinesAlarm.hpp"

const u8 byte_5543D0[32] = {
    0u, 0u, 1u, 128u, 1u, 132u, 32u, 132u, 33u,
    128u, 32u, 132u, 33u, 132u, 101u, 206u, 101u, 140u,
    140u, 177u, 19u, 148u, 100u, 206u, 101u, 206u, 215u,
    152u, 20u, 161u, 24u, 216u};

ALIVE_VAR(1, 0x5c1bfa, u8, gTotalMeterBars_5C1BFA, 0);
ALIVE_VAR(1, 0x5c1bf8, u16, gbDrawMeterCountDown_5C1BF8, 0);

ColourfulMeter::ColourfulMeter(Path_ColourfulMeter* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eColourfulMeter);
    field_68_tlvInfo = tlvInfo;

    field_74_tlv_x = pTlv->field_8_top_left.field_0_x;
    field_76_tlv_y = pTlv->field_8_top_left.field_2_y;

    field_20_font_context.LoadFontType_433400(2);
    field_30_font.ctor_433590(5, byte_5543D0, &field_20_font_context);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    field_6C_text_x = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x)) - pScreenManager->mCamPos->field_0_x);
    field_6E_text_y = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y)) - pScreenManager->mCamPos->field_4_y);

    field_72_switch_id = pTlv->field_10_switch_id;
    field_80 = 0;
    field_7A_number_of_meter_bars = pTlv->field_12_number_of_meter_bars;
    field_82_bar_count = kMeterBarsXCount / field_7A_number_of_meter_bars + 1;

    if (field_7A_number_of_meter_bars == 4)
    {
        field_82_bar_count = 5;
    }
    else if (field_7A_number_of_meter_bars == 5)
    {
        field_82_bar_count = 4;
    }

    field_78_count = 15;

    field_7E_starting_switch_state = static_cast<s16>(SwitchStates_Get(field_72_switch_id));
    field_84_bStartFilled = pTlv->field_16_bStartFilled;
    field_7C_mines_alarm_countdown = pTlv->field_14_mines_alarm_countdown;

    if (field_84_bStartFilled == Choice_short::eYes_1)
    {
        if (field_7E_starting_switch_state)
        {
            field_70_polys_to_render_count = 0;
        }
        else
        {
            field_70_polys_to_render_count = kMeterBarsXCount;
        }
    }
    else
    {
        field_70_polys_to_render_count = field_82_bar_count * gTotalMeterBars_5C1BFA;
    }
}

ColourfulMeter::~ColourfulMeter()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_68_tlvInfo, -1, 0, 0);
    field_30_font.dtor_433540();
    field_20_font_context.dtor_433510();
}

void ColourfulMeter::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        gTotalMeterBars_5C1BFA = 0;
    }
}

void ColourfulMeter::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        gTotalMeterBars_5C1BFA = 0;
    }

    if (Event_Get(kEventDeathResetEnd))
    {
        gbDrawMeterCountDown_5C1BF8 = 0;
    }

    if (field_84_bStartFilled == Choice_short::eYes_1)
    {
        gbDrawMeterCountDown_5C1BF8 = 0;

        if (SwitchStates_Get(field_72_switch_id))
        {
            if (field_70_polys_to_render_count > 0)
            {
                if (field_78_count == 0)
                {
                    field_78_count = 15;
                    field_70_polys_to_render_count--;
                }
                field_78_count--;
            }
        }
    }
    else
    {
        if (!field_7E_starting_switch_state)
        {
            if (SwitchStates_Get(field_72_switch_id))
            {
                gTotalMeterBars_5C1BFA++;
                field_80 = 1;
                field_7E_starting_switch_state = 1;
            }
        }

        if (field_70_polys_to_render_count < gTotalMeterBars_5C1BFA * field_82_bar_count)
        {
            if (field_78_count == 0)
            {
                field_78_count = 15;
                field_70_polys_to_render_count++;
            }
            field_78_count--;
        }

        if (field_7A_number_of_meter_bars > gTotalMeterBars_5C1BFA)
        {
            gbDrawMeterCountDown_5C1BF8 = 0;
        }
        else if (!gbDrawMeterCountDown_5C1BF8)
        {
            gbDrawMeterCountDown_5C1BF8 = 1;
            MinesAlarm::Create(30 * field_7C_mines_alarm_countdown);
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
    const s16 screenXOff = FP_GetExponent(pScreenManager->mCamPos->field_0_x + FP_FromInteger(4));
    const s16 screenYOff = FP_GetExponent(pScreenManager->mCamPos->field_4_y + FP_FromInteger(4));

    for (s16 poly_idx = 0; poly_idx < field_70_polys_to_render_count && poly_idx < kMeterBarsXCount - 1; poly_idx++)
    {
        Poly_G4* pPolyG4 = &field_88_polyG4s[gPsxDisplay_5C1130.field_C_buffer_index][poly_idx];
        PolyG4_Init_4F88B0(pPolyG4);

        const s16 x0 = field_74_tlv_x + (FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx].field_0_x))) - screenXOff;
        const s16 y0 = FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx].field_2_y)) - screenYOff + field_76_tlv_y - 20;

        const s16 x1 = field_74_tlv_x + ((poly_idx + 1) * 2) - screenXOff;
        const s16 y1 = field_76_tlv_y - screenYOff - 5;

        const s16 x2 = field_74_tlv_x + FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx + 1].field_0_x)) - screenXOff;
        const s16 y2 = FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx + 1].field_2_y)) - screenYOff + field_76_tlv_y - 20;

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

        Poly_Set_SemiTrans_4F8A60(&pPolyG4->mBase.header, FALSE);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, Layer::eLayer_Well_23), &pPolyG4->mBase.header);
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
            field_6C_text_x + 1,
            field_6E_text_y - 5,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127, 127, 127,
            0,
            FP_FromInteger(1),
            field_6C_text_x + textWidth,
            colourRand);
    }

    pScreenManager->InvalidateRect(
        PsxToPCX(field_6C_text_x - 50),
        field_6E_text_y - 30,
        PsxToPCX(field_6C_text_x + 500),
        field_6E_text_y,
        pScreenManager->mIdx);
}

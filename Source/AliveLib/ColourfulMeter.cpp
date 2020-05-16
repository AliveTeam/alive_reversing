#include "stdafx.h"
#include "ColourfulMeter.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "MinesAlarm.hpp"

const BYTE byte_5543D0[32] =
{
    0u,    0u,    1u,    128u,    1u,    132u,    32u,    132u,    33u,
    128u,  32u,   132u,  33u,     132u,  101u,    206u,   101u,    140u,
    140u,  177u,  19u,   148u,    100u,  206u,    101u,   206u,    215u,
    152u,  20u,   161u,  24u,     216u
};

ALIVE_VAR(1, 0x5c1bfa, BYTE, gTotalMeterBars_5C1BFA, 0);
ALIVE_VAR(1, 0x5c1bf8, WORD, gbDrawMeterCountDown_5C1BF8, 0);

ColourfulMeter* ColourfulMeter::ctor_43CE40(Path_ColourfulMeter* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5451EC);

    field_4_typeId = Types::eColourfulMeter_59;
    field_68_tlvInfo = tlvInfo;

    field_74_tlv_x = pTlv->field_8_top_left.field_0_x;
    field_76_tlv_y = pTlv->field_8_top_left.field_2_y;

    field_20_font_context.LoadFontType_433400(2);
    field_30_font.ctor_433590(5, byte_5543D0, &field_20_font_context);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    gObjList_drawables_5C1124->Push_Back(this);

    field_6C_text_x = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x)) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_6E_text_y = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y)) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    field_72_switch_id = pTlv->field_10_id;
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

    field_7E_starting_switch_state = static_cast<short>(SwitchStates_Get_466020(field_72_switch_id));
    field_84_bStartsFull = pTlv->field_16_bStartsFull;
    field_7C_timer = pTlv->field_14_timer;

    if (field_84_bStartsFull)
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
    return this;
}

BaseGameObject* ColourfulMeter::VDestructor(signed int flags)
{
    return vdtor_43D050(flags);
}

void ColourfulMeter::VScreenChanged()
{
    vScreenChanged_43D7A0();
}

void ColourfulMeter::VUpdate()
{
    vUpdate_43D140();
}

void ColourfulMeter::VRender(int** pOrderingTable)
{
    vRender_43D2B0(pOrderingTable);
}

ColourfulMeter* ColourfulMeter::vdtor_43D050(signed int flags)
{
    dtor_43D080();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void ColourfulMeter::dtor_43D080()
{
    SetVTable(this, 0x5451EC);
    gObjList_drawables_5C1124->Remove_Item(this);
    Path::TLV_Reset_4DB8E0(field_68_tlvInfo, -1, 0, 0);
    field_30_font.dtor_433540();
    field_20_font_context.dtor_433510();
    BaseGameObject_dtor_4DBEC0();
}

void ColourfulMeter::vScreenChanged_43D7A0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);

    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level ||
        gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        gTotalMeterBars_5C1BFA = 0;
    }
}

void ColourfulMeter::vUpdate_43D140()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        gTotalMeterBars_5C1BFA = 0;
    }

    if (Event_Get_422C00(kEventDeathResetEnd))
    {
        gbDrawMeterCountDown_5C1BF8 = 0;
    }

    if (field_84_bStartsFull)
    {
        gbDrawMeterCountDown_5C1BF8 = 0;

        if (SwitchStates_Get_466020(field_72_switch_id))
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
            if (SwitchStates_Get_466020(field_72_switch_id))
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
            MinesAlarm::Create_4177F0(30 * field_7C_timer);
        }
    }
}

const PSX_Point stru_5543F0[kMeterBarsXCount] =
{
    { 0, 9 },
    { 2, 7 },
    { 4, 5 },
    { 6, 4 },
    { 8, 3 },
    { 10, 2 },
    { 13, 1 },
    { 15, 1 },
    { 17, 0 },
    { 20, 0 },
    { 22, 0 },
    { 25, 0 },
    { 27, 1 },
    { 29, 1 },
    { 32, 2 },
    { 34, 3 },
    { 36, 4 },
    { 38, 5 },
    { 40, 7 },
    { 42, 9 },
};

void ColourfulMeter::vRender_43D2B0(int** pOt)
{
    const short screenXOff = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(4));
    const short screenYOff = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(4));

    for (short poly_idx = 0; poly_idx < field_70_polys_to_render_count && poly_idx < kMeterBarsXCount -1; poly_idx++)
    {
        Poly_G4* pPolyG4 = &field_88_polyG4s[gPsxDisplay_5C1130.field_C_buffer_index][poly_idx];
        PolyG4_Init_4F88B0(pPolyG4);

        const short x0 = field_74_tlv_x + (FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx].field_0_x))) - screenXOff;
        const short y0 = FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx].field_2_y)) - screenYOff + field_76_tlv_y - 20;

        const short x1 = field_74_tlv_x + ((poly_idx + 1) * 2) - screenXOff;
        const short y1 = field_76_tlv_y - screenYOff - 5;

        const short x2 = field_74_tlv_x + FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx + 1].field_0_x)) - screenXOff;
        const short y2 = FP_GetExponent(FP_FromInteger(stru_5543F0[poly_idx + 1].field_2_y)) - screenYOff + field_76_tlv_y - 20;

        SetXY0(pPolyG4, static_cast<short>(PsxToPCX(x0)), y0);
        SetXY1(pPolyG4, static_cast<short>(PsxToPCX(x1)), y1);
        SetXY2(pPolyG4, static_cast<short>(PsxToPCX(x2)), y2);
        SetXY3(pPolyG4, static_cast<short>(PsxToPCX(x1 + 2)), y1);

        if (((poly_idx + 1)) >= (kMeterBarsXCount/2)+1)
        {
            // 1st half
            SetRGB0(pPolyG4, 127, static_cast<BYTE>(240 - 12 * poly_idx), 0);
            SetRGB1(pPolyG4, 127, static_cast<BYTE>(240 - 12 * poly_idx), 0);
            SetRGB2(pPolyG4, 127, static_cast<BYTE>(228 - 12 * poly_idx), 0);
            SetRGB3(pPolyG4, 127, static_cast<BYTE>(228 - 12 * poly_idx), 0);
        }
        else
        {
            // 2nd half
            SetRGB0(pPolyG4, static_cast<BYTE>(12 * poly_idx), 127, 0);
            SetRGB1(pPolyG4, static_cast<BYTE>(12 * poly_idx), 127, 0);
            SetRGB2(pPolyG4, static_cast<BYTE>(12 * (poly_idx + 1)), 127, 0);
            SetRGB3(pPolyG4, static_cast<BYTE>(12 * (poly_idx + 1)), 127, 0);
        }

        Poly_Set_SemiTrans_4F8A60(&pPolyG4->mBase.header, FALSE);
        OrderingTable_Add_4F8AA0(&pOt[23], &pPolyG4->mBase.header);
    }

    if (gbDrawMeterCountDown_5C1BF8)
    {
        char text[12] = {};
        sprintf(text, "%01d:%02d", sTimerValue_5C1BFC / 1800u, sTimerValue_5C1BFC / 30u % 60);
        const int textWidth = field_30_font.MeasureWidth_433700(text);
        
        short colourRand = 50;
        if (sDisableFontFlicker_5C9304)
        {
            colourRand = 0;
        }

        field_30_font.DrawString_4337D0(
            pOt,
            text,
            field_6C_text_x + 1,
            field_6E_text_y - 5,
            1, 1, 0, 22,
            127, 127, 127,
            0,
            FP_FromInteger(1),
            field_6C_text_x + textWidth,
            colourRand);
    }
    
    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        PsxToPCX(field_6C_text_x - 50),
        field_6E_text_y - 30,
        PsxToPCX(field_6C_text_x + 500),
        field_6E_text_y,
        pScreenManager_5BB5F4->field_3A_idx);
}

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
ALIVE_VAR(1, 0x5c1bf8, WORD, word_5C1BF8, 0);

ColourfulMeter* ColourfulMeter::ctor_43CE40(Path_ColourfulMeter* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5451EC);

    field_4_typeId = Types::eColourfulMeter_59;
    field_68_tlvInfo = tlvInfo;

    field_74 = pTlv->field_8_top_left.field_0_x;
    field_76 = pTlv->field_8_top_left.field_2_y;

    field_20_font_context.LoadFontType_433400(2);
    field_30_font.ctor_433590(5, byte_5543D0, &field_20_font_context);
    field_6_flags.Set(BaseGameObject::eDrawable);
    gObjList_drawables_5C1124->Push_Back(this);

    field_6C = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x)) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_6E = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y)) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    field_72_switch_id = pTlv->field_10_id;
    field_80 = 0;
    field_7A_number_of_meter_bars = pTlv->field_12_number_of_meter_bars;;
    field_82_bar_count = 20 / field_7A_number_of_meter_bars + 1;

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
            field_70_polys_to_render_count = 20;
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

ColourfulMeter* ColourfulMeter::vdtor_43D050(signed int flags)
{
    dtor_43D080();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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
    field_6_flags.Set(BaseGameObject::eDead);

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        gTotalMeterBars_5C1BFA = 0;
    }
}

void ColourfulMeter::vUpdate_43D140()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        gTotalMeterBars_5C1BFA = 0;
    }

    if (Event_Get_422C00(kEventDeathResetEnd))
    {
        word_5C1BF8 = 0;
    }

    if (field_84_bStartsFull)
    {
        word_5C1BF8 = 0;

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
            word_5C1BF8 = 0;
        }
        else if (!word_5C1BF8)
        {
            word_5C1BF8 = 1;
            MinesAlarm::Create_4177F0(30 * field_7C_timer);
        }
    }
}

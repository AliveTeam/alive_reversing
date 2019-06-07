#include "stdafx.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "Map.hpp"

const BYTE byte_5513D4[40] =
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
    216u,
    0u,
    0u,
    0u,
    0u,
    0u,
    0u,
    0u,
    0u
};

ALIVE_VAR(1, 0x5c1be8, int, dword_5C1BE8, 0); // Gas counter?
ALIVE_VAR(1, 0x5C1C00, short, gGasOn_5C1C00, 0);

GasCountDown* GasCountDown::ctor_417010(Path_GasCountDown* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5445E0);

    field_4_typeId = Types::eGasClock_23;
    field_68_tlvInfo = tlvInfo;

    field_20_font_context.LoadFontType_433400(2);
    field_30_font.ctor_433590(5, byte_5513D4, &field_20_font_context);
    field_6_flags.Set(BaseGameObject::eDrawable);
    gObjList_drawables_5C1124->Push_Back(this);

    field_6C_xpos = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_6E_ypos = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    gGasOn_5C1C00 = 0;

    field_70_start_trigger_id = pTlv->field_10_start_trigger_id;
    field_76_time = pTlv->field_12_time;
    field_72_stop_trigger_id = pTlv->field_14_stop_trigger_id;

    if (dword_5C1BE8)
    {
        field_74_time_left = static_cast<short>((field_76_time - (sGnFrame_5C1B84 - dword_5C1BE8)) / 30);
        auto pAlarm = alive_new<Alarm>();
        if (pAlarm)
        {
            pAlarm->ctor_4091F0(field_76_time, 0, 0, 39);
            return this;
        }
    }
    else
    {
        field_74_time_left = field_76_time / 30;
    }

    return this;
}

BaseGameObject* GasCountDown::VDestructor(signed int flags)
{
    return vdtor_4171F0(flags);
}

void GasCountDown::VScreenChanged()
{
    vScreenChanged_417700();
}

void GasCountDown::dtor_417220()
{
    SetVTable(this, 0x5445E0);
    gObjList_drawables_5C1124->Remove_Item(this);
    Path::TLV_Reset_4DB8E0(field_68_tlvInfo, -1, 0, 0);
    field_30_font.dtor_433540();
    field_20_font_context.dtor_433510();
    BaseGameObject_dtor_4DBEC0();
}

GasCountDown* GasCountDown::vdtor_4171F0(signed int flags)
{
    dtor_417220();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void GasCountDown::vScreenChanged_417700()
{
    field_6_flags.Set(BaseGameObject::eDead);
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        dword_5C1BE8 = 0;
    }
}

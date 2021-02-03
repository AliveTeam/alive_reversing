#include "stdafx_ao.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"
#include "Alarm.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"

namespace AO {

const BYTE byte_4C5080[32] =
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

ALIVE_VAR(1, 0x507700, int, sGasTimer_507700, 0);
ALIVE_VAR(1, 0x4FF888, short, gGasOn_4FF888, 0);

GasCountDown* GasCountDown::ctor_40BF60(Path_GasCountDown* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA440);
    field_4_typeId = Types::eGasClock_16;
    field_58_tlvInfo = tlvInfo;
    field_10_font_context.LoadFontType_41C040(2);
    field_20_font.ctor_41C170(5, byte_4C5080, &field_10_font_context);
    field_6_flags.Set(Options::eDrawable_Bit4);
    gObjList_drawables_504618->Push_Back(this);

    gGasOn_4FF888 = 0;

    field_62_time_left = 120;

    field_5C_xpos = FP_GetExponent((FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos + pTlv->field_C_sound_pos.field_0_x) -  pScreenManager_4FF7C8->field_10_pCamPos->field_0_x));

    field_5E_ypos = FP_GetExponent((FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + pTlv->field_C_sound_pos.field_2_y)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);

    field_60_switch_id = pTlv->field_18_start_id;
    return this;
}

BaseGameObject* GasCountDown::dtor_40C050()
{
    SetVTable(this, 0x4BA440);
    gObjList_drawables_504618->Remove_Item(this);
    gMap_507BA8.TLV_Reset_446870(field_58_tlvInfo, -1, 0, 0);
    field_20_font.dtor_41C130();
    field_10_font_context.dtor_41C110();
    return dtor_487DF0();
}

BaseGameObject* GasCountDown::Vdtor_40C3F0(int flags)
{
    dtor_40C050();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}


void GasCountDown::VScreenChanged()
{
    VScreenChanged_40C3C0();
}

void GasCountDown::VScreenChanged_40C3C0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        sGasTimer_507700 = 0;
    }
}

void GasCountDown::VUpdate()
{
    VUpdate_40C0E0();
}

void GasCountDown::VUpdate_40C0E0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (Event_Get_417250(kEvent_5))
    {
        sGasTimer_507700 = 0;
        gGasOn_4FF888 = 0;
    }

    // Enable
    if (!sGasTimer_507700 && 
        SwitchStates_Get(field_60_switch_id) &&
        !SwitchStates_Get(70))
    {
        sGasTimer_507700 = gnFrameCount_507670;
        auto pAlarm = ao_new<Alarm>();
        if (pAlarm)
        {
            pAlarm->ctor_402570(3600, 0, 0, Layer::eLayer_39);
        }

    }

    if (!sGasTimer_507700)
    {
        // Off/idle
        field_62_time_left = 120;
    }
    else
    {
        // Running
        if (SwitchStates_Get(70))
        {
            sGasTimer_507700 = 0;
            return;
        }

        if (Event_Get_417250(kEventResetting_6))
        {
            sGasTimer_507700++;
        }

        const int oldTimer = field_62_time_left;
        const int newTimer = 120 - (static_cast<int>(gnFrameCount_507670) - sGasTimer_507700) / 30;
        field_62_time_left = static_cast<__int16>(newTimer);
        if (oldTimer != field_62_time_left && field_62_time_left > 0)
        {
            SFX_Play_43AE60(SoundEffect::RedTick_4, 55, -1000, 0);
        }
    }

    DealDamage();
}

void GasCountDown::DealDamage()
{
    if (field_62_time_left < 0)
    {
        if (-field_62_time_left > 2)
        {
            sActiveHero_507678->VTakeDamage(this);
            for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
            {
                BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId == Types::eMudokon_75)
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
        field_62_time_left = 0;
    }

    if (!gGasOn_4FF888 && field_62_time_left <= 0)
    {
        gGasOn_4FF888 = TRUE;
        auto pDeathGas = ao_new<DeathGas>();
        if (pDeathGas)
        {
            pDeathGas->ctor_41CF40(Layer::eLayer_39, 2);
        }
    }
}

void GasCountDown::VRender(PrimHeader** ppOt)
{
    VRender_40C2F0(ppOt);
}

void GasCountDown::VRender_40C2F0(PrimHeader** ppOt)
{
    char text[128] = {}; 
    sprintf(text, "%02d:%02d", field_62_time_left / 60, field_62_time_left % 60);
    const auto textWidth = field_20_font.MeasureWidth_41C2B0(text);

    field_20_font.DrawString_41C360(
        ppOt,
        text,
        field_5C_xpos,
        field_5E_ypos,
        1,
        1,
        0,
        22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        textWidth + field_5C_xpos,
        sDisableFontFlicker_5080E4 ? 0 : 50);

    pScreenManager_4FF7C8->InvalidateRect_406E40(
        field_5C_xpos,
        field_5E_ypos,
        field_5C_xpos + textWidth,
        field_5E_ypos + 16,
        pScreenManager_4FF7C8->field_2E_idx);
}

BaseGameObject* GasCountDown::VDestructor(int flags)
{
    return Vdtor_40C3F0(flags);
}

}

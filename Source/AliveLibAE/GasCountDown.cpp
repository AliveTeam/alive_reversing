#include "stdafx.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "Map.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"

const u8 byte_5513D4[40] = {
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
    0u};

ALIVE_VAR(1, 0x5c1be8, s32, sGasTimer_5C1BE8, 0);
ALIVE_VAR(1, 0x5C1C00, s16, gGasOn_5C1C00, 0);

GasCountDown::GasCountDown(Path_GasCountDown* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eGasClock);
    field_68_tlvInfo = tlvInfo;

    field_20_font_context.LoadFontType_433400(2);
    field_30_font.ctor_433590(5, byte_5513D4, &field_20_font_context);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    field_6C_xpos = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager->mCamPos->field_0_x);
    field_6E_ypos = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager->mCamPos->field_4_y);

    gGasOn_5C1C00 = 0;

    field_70_start_timer_switch_id = pTlv->field_10_start_timer_switch_id;
    field_76_gas_countdown_timer = pTlv->field_12_gas_countdown_timer;
    field_72_stop_timer_switch_id = pTlv->field_14_stop_timer_switch_id;

    if (sGasTimer_5C1BE8)
    {
        field_74_time_left = static_cast<s16>((field_76_gas_countdown_timer - (sGnFrame - sGasTimer_5C1BE8)) / 30);
        if (field_74_time_left < 0)
        {
            field_74_time_left = 0;
        }

        relive_new Alarm(field_76_gas_countdown_timer, 0, 0, Layer::eLayer_Above_FG1_39);
    }
    else
    {
        field_74_time_left = field_76_gas_countdown_timer / 30;
    }
}

GasCountDown::~GasCountDown()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_68_tlvInfo, -1, 0, 0);
    field_30_font.dtor_433540();
    field_20_font_context.dtor_433510();
}

void GasCountDown::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        sGasTimer_5C1BE8 = 0;
    }
}

void GasCountDown::VRender(PrimHeader** ppOt)
{
    char_type text[128] = {}; // Bigger buffer to handle large numbers or negative numbers causing a buffer overflow/crash.
    sprintf(text, "%02d:%02d", field_74_time_left / 60, field_74_time_left % 60);

    const auto textWidth = field_30_font.MeasureTextWidth(text);
    field_30_font.DrawString_4337D0(
        ppOt,
        text,
        field_6C_xpos,
        field_6E_ypos,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        field_6C_xpos + textWidth,
        sDisableFontFlicker_5C9304 ? 0 : 50);

    pScreenManager->InvalidateRect_40EC90(
        PsxToPCX(field_6C_xpos),
        field_6E_ypos,
        PsxToPCX(field_6C_xpos + textWidth),
        field_6E_ypos + 16,
        pScreenManager->mIdx);
}

void GasCountDown::DealDamage()
{
    if (field_74_time_left < 0)
    {
        if (-field_74_time_left > 2)
        {
            sActiveHero->VTakeDamage(this);
            for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                if (pObj->Type() == ReliveTypes::eMudokon)
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
        field_74_time_left = 0;
    }

    if (!gGasOn_5C1C00 && field_74_time_left <= 0)
    {
        gGasOn_5C1C00 = TRUE;
        if (!gDeathGasCount_5BD24C)
        {
            relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
        }
    }
}

void GasCountDown::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (Event_Get(kEventDeathResetEnd))
    {
        sGasTimer_5C1BE8 = 0;
        gGasOn_5C1C00 = FALSE;
    }

    // Enable
    if (!sGasTimer_5C1BE8 && SwitchStates_Get(field_70_start_timer_switch_id) && !SwitchStates_Get(field_72_stop_timer_switch_id))
    {
        sGasTimer_5C1BE8 = sGnFrame;
        relive_new Alarm(field_76_gas_countdown_timer, 0, 0, Layer::eLayer_Above_FG1_39);
    }


    if (!sGasTimer_5C1BE8)
    {
        // Off/idle
        field_74_time_left = field_76_gas_countdown_timer / 30;
    }
    else
    {
        // Running
        if (SwitchStates_Get(field_72_stop_timer_switch_id))
        {
            sGasTimer_5C1BE8 = 0;
            return;
        }

        if (Event_Get(kEventResetting))
        {
            sGasTimer_5C1BE8++;
        }

        const s32 old_timer = field_74_time_left;
        const s32 new_timer = (field_76_gas_countdown_timer - static_cast<s32>(sGnFrame - sGasTimer_5C1BE8)) / 30;
        field_74_time_left = static_cast<s16>(new_timer);
        if (old_timer != field_74_time_left && field_74_time_left > 0)
        {
            SFX_Play_Pitch(SoundEffect::RedTick_3, 55, -1000);
        }
    }

    DealDamage();
}

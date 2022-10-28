#include "stdafx.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "Map.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"
#include "Path.hpp"

s32 sGasTimer = 0;
s16 gGasOn = 0;

GasCountDown::GasCountDown(relive::Path_GasCountDown* pTlv, const Guid& tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eGasCountDown);
    field_68_tlvInfo = tlvInfo;

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);
    field_20_FontContext.LoadFontType(FontType::LcdFont);
    field_30_font.Load(5, mPal, &field_20_FontContext);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    field_6C_xpos = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - pScreenManager->CamXPos());
    field_6E_ypos = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - pScreenManager->CamYPos());

    gGasOn = 0;

    field_70_start_timer_switch_id = pTlv->mStartTimerSwitchId;
    field_76_gas_countdown_timer = pTlv->mGasCountdownTimer;
    field_72_stop_timer_switch_id = pTlv->mStopTimerSwitchId;

    if (sGasTimer)
    {
        field_74_time_left = static_cast<s16>((field_76_gas_countdown_timer - (sGnFrame - sGasTimer)) / 30);
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
}

void GasCountDown::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        sGasTimer = 0;
    }
}

void GasCountDown::VRender(PrimHeader** ppOt)
{
    char_type text[128] = {}; // Bigger buffer to handle large numbers or negative numbers causing a buffer overflow/crash.
    sprintf(text, "%02d:%02d", field_74_time_left / 60, field_74_time_left % 60);

    const auto textWidth = field_30_font.MeasureTextWidth(text);
    field_30_font.DrawString(
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
        sDisableFontFlicker ? 0 : 50);
}

void GasCountDown::DealDamage()
{
    if (field_74_time_left < 0)
    {
        if (-field_74_time_left > 2)
        {
            sActiveHero->VTakeDamage(this);
            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
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

    if (!gGasOn && field_74_time_left <= 0)
    {
        gGasOn = TRUE;
        if (!gDeathGasCount_5BD24C)
        {
            relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
        }
    }
}

void GasCountDown::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (EventGet(kEventDeathResetEnd))
    {
        sGasTimer = 0;
        gGasOn = FALSE;
    }

    // Enable
    if (!sGasTimer && SwitchStates_Get(field_70_start_timer_switch_id) && !SwitchStates_Get(field_72_stop_timer_switch_id))
    {
        sGasTimer = sGnFrame;
        relive_new Alarm(field_76_gas_countdown_timer, 0, 0, Layer::eLayer_Above_FG1_39);
    }


    if (!sGasTimer)
    {
        // Off/idle
        field_74_time_left = field_76_gas_countdown_timer / 30;
    }
    else
    {
        // Running
        if (SwitchStates_Get(field_72_stop_timer_switch_id))
        {
            sGasTimer = 0;
            return;
        }

        if (EventGet(kEventResetting))
        {
            sGasTimer++;
        }

        const s32 old_timer = field_74_time_left;
        const s32 new_timer = (field_76_gas_countdown_timer - static_cast<s32>(sGnFrame - sGasTimer)) / 30;
        field_74_time_left = static_cast<s16>(new_timer);
        if (old_timer != field_74_time_left && field_74_time_left > 0)
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 55, -1000);
        }
    }

    DealDamage();
}

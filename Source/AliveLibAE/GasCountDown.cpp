#include "stdafx.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "Map.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"
#include "Path.hpp"

s32 gGasTimer = 0;
s16 gGasOn = 0;

GasCountDown::GasCountDown(relive::Path_GasCountDown* pTlv, const Guid& tlvInfo)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eGasCountDown);
    mTlvId = tlvInfo;

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);
    mFontContext.LoadFontType(FontType::LcdFont);
    mFont.Load(5, mPal, &mFontContext);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    mGasXPos = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - pScreenManager->CamXPos());
    mGasYPos = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - pScreenManager->CamYPos());

    gGasOn = 0;

    mStartTimerSwitchId = pTlv->mStartTimerSwitchId;
    field_76_gas_countdown_timer = pTlv->mGasCountdownTimer;
    field_72_stop_timer_switch_id = pTlv->mStopTimerSwitchId;

    if (gGasTimer)
    {
        mGasTimeLeft = static_cast<s16>((field_76_gas_countdown_timer - (sGnFrame - gGasTimer)) / 30);
        if (mGasTimeLeft < 0)
        {
            mGasTimeLeft = 0;
        }

        relive_new Alarm(field_76_gas_countdown_timer, 0, 0, Layer::eLayer_Above_FG1_39);
    }
    else
    {
        mGasTimeLeft = field_76_gas_countdown_timer / 30;
    }
}

GasCountDown::~GasCountDown()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void GasCountDown::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        gGasTimer = 0;
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
        gGasTimer = 0;
        gGasOn = false;
    }

    // Enable
    if (!gGasTimer && SwitchStates_Get(mStartTimerSwitchId) && !SwitchStates_Get(field_72_stop_timer_switch_id))
    {
        gGasTimer = sGnFrame;
        relive_new Alarm(field_76_gas_countdown_timer, 0, 0, Layer::eLayer_Above_FG1_39);
    }

    if (!gGasTimer)
    {
        // Off/idle
        mGasTimeLeft = field_76_gas_countdown_timer / 30;
    }
    else
    {
        // Running
        if (SwitchStates_Get(field_72_stop_timer_switch_id))
        {
            gGasTimer = 0;
            return;
        }

        if (EventGet(kEventResetting))
        {
            gGasTimer++;
        }

        const s32 oldTimer = mGasTimeLeft;
        const s32 newTimer = (field_76_gas_countdown_timer - static_cast<s32>(sGnFrame - gGasTimer)) / 30;
        mGasTimeLeft = static_cast<s16>(newTimer);
        if (oldTimer != mGasTimeLeft && mGasTimeLeft > 0)
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 55, -1000);
        }
    }

    DealDamage();
}

void GasCountDown::DealDamage()
{
    if (mGasTimeLeft < 0)
    {
        if (-mGasTimeLeft > 2)
        {
            sActiveHero->VTakeDamage(this);
            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
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
        mGasTimeLeft = 0;
    }

    if (!gGasOn && mGasTimeLeft <= 0)
    {
        gGasOn = true;
        if (!gDeathGasCount_5BD24C)
        {
            relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
        }
    }
}

void GasCountDown::VRender(PrimHeader** ppOt)
{
    char_type text[128] = {}; // Bigger buffer to handle large numbers or negative numbers causing a buffer overflow/crash.
    sprintf(text, "%02d:%02d", mGasTimeLeft / 60, mGasTimeLeft % 60);
    const auto textWidth = mFont.MeasureTextWidth(text);

    mFont.DrawString(
        ppOt,
        text,
        mGasXPos,
        mGasYPos,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        textWidth + mGasXPos,
        sDisableFontFlicker ? 0 : 50);
}

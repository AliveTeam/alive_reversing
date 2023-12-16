#include "stdafx.h"
#include "GasCountDown.hpp"
#include "../relive_lib/Function.hpp"
#include "Alarm.hpp"
#include "Map.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"
#include "Path.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"

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
    SetDrawable(true);
    gObjListDrawables->Push_Back(this);

    mGasXPos = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - gScreenManager->CamXPos());
    mGasYPos = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - gScreenManager->CamYPos());

    gGasOn = 0;

    mStartTimerSwitchId = pTlv->mStartTimerSwitchId;
    mGasCountdownTimer = pTlv->mGasCountdownTimer;
    mStopTimerSwitchId = pTlv->mStopTimerSwitchId;

    if (gGasTimer)
    {
        mGasTimeLeftSecs = static_cast<s16>((mGasCountdownTimer - (sGnFrame - gGasTimer)) / 30);
        if (mGasTimeLeftSecs < 0)
        {
            mGasTimeLeftSecs = 0;
        }

        relive_new Alarm(mGasCountdownTimer, 0, 0, Layer::eLayer_Above_FG1_39);
    }
    else
    {
        mGasTimeLeftSecs = mGasCountdownTimer / 30;
    }
}

GasCountDown::~GasCountDown()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void GasCountDown::VScreenChanged()
{
    SetDead(true);
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        gGasTimer = 0;
    }
}

void GasCountDown::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (EventGet(kEventDeathResetEnd))
    {
        gGasTimer = 0;
        gGasOn = false;
    }

    // Enable
    if (!gGasTimer && SwitchStates_Get(mStartTimerSwitchId) && !SwitchStates_Get(mStopTimerSwitchId))
    {
        gGasTimer = sGnFrame;
        relive_new Alarm(mGasCountdownTimer, 0, 0, Layer::eLayer_Above_FG1_39);
    }

    if (!gGasTimer)
    {
        // Off/idle
        mGasTimeLeftSecs = mGasCountdownTimer / 30;
    }
    else
    {
        // Running
        if (SwitchStates_Get(mStopTimerSwitchId))
        {
            gGasTimer = 0;
            return;
        }

        if (EventGet(kEventResetting))
        {
            gGasTimer++;
        }

        const s32 oldTimer = mGasTimeLeftSecs;
        const s32 newTimer = (mGasCountdownTimer - static_cast<s32>(sGnFrame - gGasTimer)) / 30;
        mGasTimeLeftSecs = static_cast<s16>(newTimer);
        if (oldTimer != mGasTimeLeftSecs && mGasTimeLeftSecs > 0)
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 55, -1000);
        }
    }

    DealDamage();
}

void GasCountDown::DealDamage()
{
    if (mGasTimeLeftSecs < 0)
    {
        if (-mGasTimeLeftSecs > 2)
        {
            gAbe->VTakeDamage(this);
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
        mGasTimeLeftSecs = 0;
    }

    if (!gGasOn && mGasTimeLeftSecs <= 0)
    {
        gGasOn = true;
        if (!gDeathGasCount)
        {
            relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
        }
    }
}

void GasCountDown::VRender(OrderingTable& ot)
{
    char_type text[128] = {}; // Bigger buffer to handle large numbers or negative numbers causing a buffer overflow/crash.
    sprintf(text, "%02d:%02d", mGasTimeLeftSecs / 60, mGasTimeLeftSecs % 60);
    const auto textWidth = mFont.MeasureTextWidth(text);

    mFont.DrawString(
        ot,
        text,
        mGasXPos,
        mGasYPos,
        relive::TBlendModes::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        textWidth + mGasXPos,
        gDisableFontFlicker ? 0 : 50);
}

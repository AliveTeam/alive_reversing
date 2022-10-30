#include "stdafx_ao.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"
#include "Alarm.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"

namespace AO {

s32 sGasTimer = 0;
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

    mGasXPos = FP_GetExponent((FP_FromInteger(pScreenManager->mCamXOff + pTlv->mTopLeftX) - pScreenManager->mCamPos->x));
    mGasYPos = FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + pTlv->mTopLeftY)) - pScreenManager->mCamPos->y);

    gGasOn = 0;

    mGasTimeLeft = 120;

   
    mStartTimerSwitchId = pTlv->mStartTimerSwitchId;
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
        sGasTimer = 0;
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
        gGasOn = false;
    }

    // Enable
    if (!sGasTimer && SwitchStates_Get(mStartTimerSwitchId) && !SwitchStates_Get(70))
    {
        sGasTimer = sGnFrame;
        relive_new Alarm(3600, 0, 0, Layer::eLayer_Above_FG1_39);
    }

    if (!sGasTimer)
    {
        // Off/idle
        mGasTimeLeft = 120;
    }
    else
    {
        // Running
        if (SwitchStates_Get(70))
        {
            sGasTimer = 0;
            return;
        }

        if (EventGet(kEventResetting))
        {
            sGasTimer++;
        }

        const s32 oldTimer = mGasTimeLeft;
        const s32 newTimer = 120 - (static_cast<s32>(sGnFrame) - sGasTimer) / 30;
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
        relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
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

} // namespace AO

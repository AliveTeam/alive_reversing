#include "stdafx_ao.h"
#include "GasCountDown.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Abe.hpp"
#include "Alarm.hpp"
#include "Sfx.hpp"
#include "DeathGas.hpp"
#include "Path.hpp"

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
    SetDrawable(true);
    gObjListDrawables->Push_Back(this);

    mGasXPos = FP_GetExponent((FP_FromInteger(gScreenManager->mCamXOff + pTlv->mTopLeftX) - gScreenManager->mCamPos->x));
    mGasYPos = FP_GetExponent((FP_FromInteger(gScreenManager->mCamYOff + pTlv->mTopLeftY)) - gScreenManager->mCamPos->y);

    gGasOn = 0;

    mGasTimeLeftSecs = 120;

   
    mStartTimerSwitchId = pTlv->mStartTimerSwitchId;
}

GasCountDown::~GasCountDown()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void GasCountDown::VScreenChanged()
{
    SetDead(true);
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        sGasTimer = 0;
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
        mGasTimeLeftSecs = 120;
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

        const s32 oldTimer = mGasTimeLeftSecs;
        const s32 newTimer = 120 - (static_cast<s32>(sGnFrame) - sGasTimer) / 30;
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
        mGasTimeLeftSecs = 0;
    }

    if (!gGasOn && mGasTimeLeftSecs <= 0)
    {
        gGasOn = true;
        relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
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

} // namespace AO

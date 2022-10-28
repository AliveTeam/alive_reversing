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

GasCountDown::GasCountDown(relive::Path_GasCountDown* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eGasCountDown);
    field_58_tlvInfo = tlvId;

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);
    field_10_FontContext.LoadFontType(FontType::LcdFont);
    field_20_font.Load(5, mPal, &field_10_FontContext);
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);

    gGasOn = 0;

    field_62_time_left = 120;

    field_5C_xpos = FP_GetExponent((FP_FromInteger(pScreenManager->mCamXOff + pTlv->mTopLeftX) - pScreenManager->mCamPos->x));

    field_5E_ypos = FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + pTlv->mTopLeftY)) - pScreenManager->mCamPos->y);

    field_60_start_switch_id = pTlv->mStartTimerSwitchId;
}

GasCountDown::~GasCountDown()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_58_tlvInfo, -1, 0, 0);
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
        gGasOn = 0;
    }

    // Enable
    if (!sGasTimer && SwitchStates_Get(field_60_start_switch_id) && !SwitchStates_Get(70))
    {
        sGasTimer = sGnFrame;
        relive_new Alarm(3600, 0, 0, Layer::eLayer_Above_FG1_39);
    }

    if (!sGasTimer)
    {
        // Off/idle
        field_62_time_left = 120;
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

        const s32 oldTimer = field_62_time_left;
        const s32 newTimer = 120 - (static_cast<s32>(sGnFrame) - sGasTimer) / 30;
        field_62_time_left = static_cast<s16>(newTimer);
        if (oldTimer != field_62_time_left && field_62_time_left > 0)
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 55, -1000);
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
        field_62_time_left = 0;
    }

    if (!gGasOn && field_62_time_left <= 0)
    {
        gGasOn = TRUE;
        relive_new DeathGas(Layer::eLayer_Above_FG1_39, 2);
    }
}

void GasCountDown::VRender(PrimHeader** ppOt)
{
    char_type text[128] = {};
    sprintf(text, "%02d:%02d", field_62_time_left / 60, field_62_time_left % 60);
    const auto textWidth = field_20_font.MeasureTextWidth(text);

    field_20_font.DrawString(
        ppOt,
        text,
        field_5C_xpos,
        field_5E_ypos,
        TPageAbr::eBlend_1,
        1,
        0,
        Layer::eLayer_BeforeWell_22,
        127,
        127,
        127,
        0,
        FP_FromInteger(1),
        textWidth + field_5C_xpos,
        sDisableFontFlicker ? 0 : 50);
}

} // namespace AO

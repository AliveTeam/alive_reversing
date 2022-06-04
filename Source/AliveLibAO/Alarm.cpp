#include "stdafx_ao.h"
#include "Alarm.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "DynamicArray.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

namespace AO {

void Alarm_ForceLink()
{ }

ALIVE_VAR(1, 0x5076A8, s16, alarmInstanceCount_5076A8, 0);

Alarm::Alarm(s32 duration_timer, s32 switchId, s32 timer, Layer layer)
    : EffectBase(layer, TPageAbr::eBlend_1)
{
    mAlarmPauseTimer = timer + sGnFrame;
    mAlarmSwitchId = static_cast<s16>(switchId);
    mBaseGameObjectTypeId = ReliveTypes::eAlarm;
    mAlarmRed = 0;
    mAlarmState = States::eAfterConstructed_0;
    mAlarmDurationTimer = mAlarmPauseTimer + duration_timer;

    alarmInstanceCount_5076A8++;
    if (alarmInstanceCount_5076A8 > 1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mEffectBaseRed = 0;
    mEffectBaseGreen = 0;
    mEffectBaseBlue = 0;

    // Disable red screen flashing in the stock yards
    if (gMap.mCurrentLevel == LevelIds::eStockYards_5 || gMap.mCurrentLevel == LevelIds::eStockYardsReturn_6)
    {
        gObjListDrawables->Remove_Item(this);
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
    }
}

Alarm::~Alarm()
{
    alarmInstanceCount_5076A8--;
    SwitchStates_Set(mAlarmSwitchId, 0);
}

void Alarm::VRender(PrimHeader** ppOt)
{
    if (!sNumCamSwappers_507668)
    {
        EffectBase::VRender(ppOt);
    }
}

void Alarm::VUpdate()
{
    Event_Broadcast(kEvent_Alarm_17, this);

    if (mEffectBasePathId != gMap.mCurrentPath || mEffectBaseLevelId != gMap.mCurrentLevel || static_cast<s32>(sGnFrame) > mAlarmDurationTimer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    switch (mAlarmState)
    {
        case States::eAfterConstructed_0:
            if (Event_Get(kEventHeroDying_3))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
            {
                mAlarmState = States::eEnabling_1;

                SFX_Play_Mono(SoundEffect::Alarm_45, 0, 0);

                if (mAlarmSwitchId)
                {
                    SwitchStates_Set(mAlarmSwitchId, 1);
                }
            }
            break;

        case States::eEnabling_1:
            mAlarmRed += 25;

            if (mAlarmRed >= 100)
            {
                mAlarmRed = 100;
                mAlarmPauseTimer = sGnFrame + 15;
                mAlarmState = States::eOnFlash_2;
                SFX_Play_Mono(SoundEffect::Alarm_45, 0, 0);
            }
            break;

        case States::eOnFlash_2:
            if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
            {
                mAlarmState = States::eDisabling_3;
            }
            break;

        case States::eDisabling_3:
            mAlarmRed -= 25;

            if (mAlarmRed <= 0)
            {
                mAlarmRed = 0;
                mAlarmPauseTimer = sGnFrame + 15;
                mAlarmState = States::eDisabled_4;
            }
            break;

        case States::eDisabled_4:
            if (Event_Get(kEventHeroDying_3))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
            {
                mAlarmState = States::eEnabling_1;
                SFX_Play_Mono(SoundEffect::Alarm_45, 0, 0);
            }
            break;

        default:
            break;
    }

    mEffectBaseRed = mAlarmRed;
}

void Alarm::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO

#include "stdafx_ao.h"
#include "Alarm.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"

namespace AO {

s16 gAlarmInstanceCount = 0;

Alarm::Alarm(s32 duration_timer, u16 switchId, s32 timer, Layer layer)
    : EffectBase(layer, relive::TBlendModes::eBlend_1),
    mAlarmState(States::eAfterConstructed_0),
    mAlarmSwitchId(switchId)
{
    SetType(ReliveTypes::eAlarm);

    mAlarmPauseTimer = timer + sGnFrame;
    mAlarmDurationTimer = mAlarmPauseTimer + duration_timer;

    gAlarmInstanceCount++;
    if (gAlarmInstanceCount > 1)
    {
        // More than one instance, kill self
        SetDead(true);
    }

    // Disable red screen flashing in the stock yards
    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        gObjListDrawables->Remove_Item(this);
        SetDrawable(false);
    }
}

Alarm::~Alarm()
{
    gAlarmInstanceCount--;
    SwitchStates_Set(mAlarmSwitchId, 0);
}

void Alarm::VRender(OrderingTable& ot)
{
    if (gNumCamSwappers == 0)
    {
        EffectBase::VRender(ot);
    }
}

void Alarm::VUpdate()
{
    EventBroadcast(kEventAlarm, this);

    if (mEffectBasePathId != gMap.mCurrentPath || mEffectBaseLevelId != gMap.mCurrentLevel || static_cast<s32>(sGnFrame) > mAlarmDurationTimer)
    {
        SetDead(true);
        return;
    }

    switch (mAlarmState)
    {
        case States::eAfterConstructed_0:
            if (EventGet(kEventHeroDying))
            {
                SetDead(true);
                return;
            }

            if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
            {
                mAlarmState = States::eEnabling_1;

                SfxPlayMono(relive::SoundEffects::Alarm, 0);

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
                mAlarmPauseTimer = MakeTimer(15);
                mAlarmState = States::eOnFlash_2;
                SfxPlayMono(relive::SoundEffects::Alarm, 0);
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
                mAlarmPauseTimer = MakeTimer(15);
                mAlarmState = States::eDisabled_4;
            }
            break;

        case States::eDisabled_4:
            if (EventGet(kEventHeroDying))
            {
                SetDead(true);
                return;
            }

            if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
            {
                mAlarmState = States::eEnabling_1;
                SfxPlayMono(relive::SoundEffects::Alarm, 0);
            }
            break;

        default:
            break;
    }

    mEffectBaseRed = mAlarmRed;
}

void Alarm::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

} // namespace AO

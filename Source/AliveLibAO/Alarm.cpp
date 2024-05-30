#include "stdafx_ao.h"
#include "Alarm.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../relive_lib/GameType.hpp"

namespace AO {

s16 gAlarmInstanceCount = 0;
Guid gAlarmObjId = Guid{}; // Only used in AE to kill the alarm in the last level

Alarm::Alarm(relive::Path_Alarm* pTlv, const Guid& tlvId)
    : EffectBase(Layer::eLayer_Above_FG1_39, relive::TBlendModes::eBlend_1)
    , mAlarmTlvInfo(tlvId)
    , mAlarmState(States::eWaitForSwitchEnable)
    , mAlarmSwitchId(pTlv->mSwitchId) // This won't count as an alarm instance till this id is enabled
    , mAlarmDuration(pTlv->mAlarmDuration)
{
    SetType(ReliveTypes::eAlarm);
}

Alarm::Alarm(s32 durationTimer, u16 switchId, s32 pauseTimer, Layer layer)
    : EffectBase(layer, relive::TBlendModes::eBlend_1),
    mAlarmTlvInfo(Guid{}),
    mAlarmState(States::eAfterConstructed),
    mAlarmSwitchId(switchId)
{
    SetType(ReliveTypes::eAlarm);

    mAlarmPauseTimer = MakeTimer(pauseTimer);
    mAlarmDurationTimer = mAlarmPauseTimer + durationTimer;

    gAlarmInstanceCount++;

    if (gAlarmInstanceCount > 1)
    {
        // More than one instance, kill self
        SetDead(true);
    }
    else
    {
        gAlarmObjId = mBaseGameObjectId;
    }

    // Disable red screen flashing in the stock yards
    if (GetMap().mCurrentLevel == EReliveLevelIds::eStockYards || GetMap().mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        gObjListDrawables->Remove_Item(this);
        SetDrawable(false);
    }
}

Alarm::~Alarm()
{
    if (mAlarmState != States::eWaitForSwitchEnable)
    {
        gAlarmInstanceCount--;
    }

    if (gAlarmObjId == mBaseGameObjectId)
    {
        gAlarmObjId = Guid{};
    }

    if (mAlarmTlvInfo == Guid{})
    {
        if (mAlarmSwitchId)
        {
            SwitchStates_Set(mAlarmSwitchId, 0);
        }
    }
    else
    {
        GetMap().TLV_Reset(mAlarmTlvInfo);
    }
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
    if (mAlarmState != States::eWaitForSwitchEnable)
    {
        EventBroadcast(Event::kEventAlarm, this);
        if (static_cast<s32>(sGnFrame) > mAlarmDurationTimer)
        {
            SetDead(true);
            return;
        }

        // TODO: check if this can be removed
        if (GetGameType() == GameType::eAo)
        {
            if (mEffectBasePathId != GetMap().mCurrentPath || mEffectBaseLevelId != GetMap().mCurrentLevel)
            {
                SetDead(true);
                return;
            }
        }
    }

    const auto alarmSound = GetGameType() == GameType::eAo ? relive::SoundEffects::Alarm : relive::SoundEffects::SecurityDoorDeny;
    switch (mAlarmState)
    {
        case States::eWaitForSwitchEnable:
            if (EventGet(Event::kEventDeathReset))
            {
                SetDead(true);
            }

            if (!SwitchStates_Get(mAlarmSwitchId))
            {
                mEffectBaseRed = mAlarmRed;
                return;
            }

            gAlarmInstanceCount++;
            if (gAlarmInstanceCount > 1)
            {
                SetDead(true);
            }
            else
            {
                gAlarmObjId = mBaseGameObjectId;
            }

            mAlarmState = States::eEnabling;
            SfxPlayMono(alarmSound, 0);
            mAlarmDurationTimer = MakeTimer(mAlarmDuration);
            mEffectBaseRed = mAlarmRed;
            break;

        case States::eAfterConstructed: // When not created by a map TLV
            if (EventGet(Event::kEventHeroDying))
            {
                SetDead(true);
                return;
            }

            mEffectBaseRed = mAlarmRed;
            if (static_cast<s32>(sGnFrame) <= mAlarmPauseTimer)
            {
                return;
            }

            mAlarmState = States::eEnabling;
            SfxPlayMono(alarmSound, 0);

            if (!mAlarmSwitchId)
            {
                return;
            }

            SwitchStates_Set(mAlarmSwitchId, 1);
            break;

        case States::eEnabling:
            mAlarmRed += 25;

            mEffectBaseRed = mAlarmRed;
            if (mAlarmRed >= 100)
            {
                mAlarmRed = 100;
                mAlarmPauseTimer = MakeTimer(15);
                mAlarmState = States::eOnFlash;
                SfxPlayMono(alarmSound, 0);
            }
            break;

        case States::eOnFlash:
            mEffectBaseRed = mAlarmRed;
            if (static_cast<s32>(sGnFrame) <= mAlarmPauseTimer)
            {
                return;
            }

            mAlarmState = States::eDisabling;
            break;

        case States::eDisabling:
            mAlarmRed -= 25;

            if (mAlarmRed > 0)
            {
                mEffectBaseRed = mAlarmRed;
                return;
            }

            mAlarmRed = 0;
            mEffectBaseRed = 0;
            mAlarmPauseTimer = MakeTimer(15);
            mAlarmState = States::eDisabled;
            break;

        case States::eDisabled:
            if (EventGet(Event::kEventHeroDying))
            {
                SetDead(true);
                return;
            }

            if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
            {
                mAlarmState = States::eEnabling;
                SfxPlayMono(alarmSound, 0);
            }
            break;

        default:
            mEffectBaseRed = mAlarmRed;
            break;
    }
}

} // namespace AO

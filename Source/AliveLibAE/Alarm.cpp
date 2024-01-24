#include "stdafx.h"
#include "Alarm.hpp"
#include "Game.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Path.hpp"

s16 gAlarmInstanceCount = 0;
Guid gAlarmObjId = Guid{};

Alarm::Alarm(relive::Path_Alarm* pTlv, const Guid& tlvId)
    : EffectBase(Layer::eLayer_Above_FG1_39, relive::TBlendModes::eBlend_3),
    mAlarmTlvInfo(tlvId),
    mAlarmState(States::eWaitForSwitchEnable_0),
    mAlarmSwitchId(pTlv->mSwitchId), // This won't count as an alarm instance till this id is enabled
    mAlarmDuration(pTlv->mAlarmDuration)
{
    SetType(ReliveTypes::eAlarm);
}

Alarm::Alarm(s32 durationOffset, u16 switchId, s32 timerOffset, Layer layer)
    : EffectBase(layer, relive::TBlendModes::eBlend_3),
    mAlarmTlvInfo(Guid{}),
    mAlarmState(States::eAfterConstructed_1),
    mAlarmSwitchId(switchId)
{
    SetType(ReliveTypes::eAlarm);

    mAlarmPauseTimer = MakeTimer(timerOffset);
    mAlarmDurationTimer = mAlarmPauseTimer + durationOffset;

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
}

Alarm::~Alarm()
{
    if (mAlarmState != States::eWaitForSwitchEnable_0)
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
        Path::TLV_Reset(mAlarmTlvInfo);
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
    if (mAlarmState != States::eWaitForSwitchEnable_0)
    {
        EventBroadcast(kEventAlarm, this);
        if (static_cast<s32>(sGnFrame) > mAlarmDurationTimer)
        {
            SetDead(true);
            return;
        }
    }

    switch (mAlarmState)
    {
        case States::eWaitForSwitchEnable_0:
            if (EventGet(kEventDeathReset))
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

            mAlarmState = States::eEnabling_2;
            SfxPlayMono(relive::SoundEffects::SecurityDoorDeny, 0);
            mAlarmDurationTimer = MakeTimer(mAlarmDuration);
            mEffectBaseRed = mAlarmRed;
            break;

        case States::eAfterConstructed_1: // When not created by a map TLV
            if (EventGet(kEventHeroDying))
            {
                SetDead(true);
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= mAlarmPauseTimer)
                {
                    mEffectBaseRed = mAlarmRed;
                    return;
                }

                mAlarmState = States::eEnabling_2;
                SfxPlayMono(relive::SoundEffects::SecurityDoorDeny, 0);
                
                if (!mAlarmSwitchId)
                {
                    mEffectBaseRed = mAlarmRed;
                    return;
                }

                SwitchStates_Set(mAlarmSwitchId, 1);
                mEffectBaseRed = mAlarmRed;
            }
            break;

        case States::eEnabling_2:
            mAlarmRed += 25;

            if (mAlarmRed < 100)
            {
                mEffectBaseRed = mAlarmRed;
                return;
            }

            mAlarmRed = 100;
            mAlarmState = States::eOnFlash_3;
            mAlarmPauseTimer = MakeTimer(15);
            SfxPlayMono(relive::SoundEffects::SecurityDoorDeny, 0);
            mEffectBaseRed = mAlarmRed;
            break;

        case States::eOnFlash_3:
            if (static_cast<s32>(sGnFrame) <= mAlarmPauseTimer)
            {
                mEffectBaseRed = mAlarmRed;
                return;
            }

            mAlarmState = States::eDisabling_4;
            mEffectBaseRed = mAlarmRed;
            break;

        case States::eDisabling_4:
            mAlarmRed -= 25;

            if (mAlarmRed > 0)
            {
                mEffectBaseRed = mAlarmRed;
                return;
            }

            mAlarmRed = 0;
            mAlarmPauseTimer = MakeTimer(15);
            mAlarmState = States::eDisabled_5;
            mEffectBaseRed = mAlarmRed;
            break;

        case States::eDisabled_5:
            if (EventGet(kEventHeroDying))
            {
                SetDead(true);
            }
            else
            {
                if (static_cast<s32>(sGnFrame) > mAlarmPauseTimer)
                {
                    mAlarmState = States::eEnabling_2;
                    SfxPlayMono(relive::SoundEffects::SecurityDoorDeny, 0);
                }
                mEffectBaseRed = mAlarmRed;
            }
            break;

        default:
            mEffectBaseRed = mAlarmRed;
            break;
    }
}

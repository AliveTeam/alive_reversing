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
    : EffectBase(Layer::eLayer_Above_FG1_39, TPageAbr::eBlend_3)
{
    mAlarmTlvInfo = tlvId;

    SetType(ReliveTypes::eAlarm);

    mAlarmRed = 0;
    mAlarmState = States::eWaitForSwitchEnable_0;

    // This won't count as an alarm instance till this id is enabled
    mAlarmSwitchId = pTlv->mSwitchId;

    mEffectBaseRed = 0;
    mEffectBaseGreen = 0;
    mEffectBaseBlue = 0;

    mAlarmDuration = pTlv->mAlarmDuration;
}

Alarm::Alarm(s32 durationOffset, u16 switchId, s32 timerOffset, Layer layer)
    : EffectBase(layer, TPageAbr::eBlend_3)
{
    SetType(ReliveTypes::eAlarm);

    mAlarmRed = 0;
    mAlarmState = States::eAfterConstructed_1;
    mAlarmTlvInfo = Guid{};
    mAlarmPauseTimer = sGnFrame + timerOffset;
    mAlarmDurationTimer = mAlarmPauseTimer + durationOffset;
    mAlarmSwitchId = switchId;

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

    mEffectBaseRed = 0;
    mEffectBaseGreen = 0;
    mEffectBaseBlue = 0;
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
        Path::TLV_Reset(mAlarmTlvInfo, -1, 0, 0);
    }
}

void Alarm::VRender(PrimHeader** ppOt)
{
    if (gNumCamSwappers == 0)
    {
        EffectBase::VRender(ppOt);
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
            mAlarmDurationTimer = sGnFrame + mAlarmDuration;
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
            mAlarmPauseTimer = sGnFrame + 15;
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
            mAlarmPauseTimer = sGnFrame + 15;
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

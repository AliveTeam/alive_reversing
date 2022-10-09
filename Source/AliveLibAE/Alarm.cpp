#include "stdafx.h"
#include "Alarm.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Path.hpp"

ALIVE_VAR(1, 0x5c1bb4, s16, alarmInstanceCount_5C1BB4, 0);
ALIVE_VAR(1, 0x550d70, Guid, sAlarmObjId_550D70, Guid{});

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

Alarm::Alarm(s32 durationOffset, s32 switchId, s32 timerOffset, Layer layer)
    : EffectBase(layer, TPageAbr::eBlend_3)
{
    SetType(ReliveTypes::eAlarm);

    mAlarmRed = 0;
    mAlarmState = States::eAfterConstructed_1;
    mAlarmTlvInfo = Guid{};
    mAlarmPauseTimer = sGnFrame + timerOffset;
    mAlarmDurationTimer = mAlarmPauseTimer + durationOffset;
    mAlarmSwitchId = static_cast<s16>(switchId);

    alarmInstanceCount_5C1BB4++;

    if (alarmInstanceCount_5C1BB4 > 1)
    {
        // More than one instance, kill self
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        sAlarmObjId_550D70 = mBaseGameObjectId;
    }

    mEffectBaseRed = 0;
    mEffectBaseGreen = 0;
    mEffectBaseBlue = 0;
}

Alarm::~Alarm()
{
    if (mAlarmState != States::eWaitForSwitchEnable_0)
    {
        alarmInstanceCount_5C1BB4--;
    }

    if (sAlarmObjId_550D70 == mBaseGameObjectId)
    {
        sAlarmObjId_550D70 = Guid{};
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
    if (sNum_CamSwappers_5C1B66 == 0)
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
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }
    }

    switch (mAlarmState)
    {
        case States::eWaitForSwitchEnable_0:
            if (EventGet(kEventDeathReset))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            if (!SwitchStates_Get(mAlarmSwitchId))
            {
                mEffectBaseRed = mAlarmRed;
                return;
            }

            alarmInstanceCount_5C1BB4++;
            if (alarmInstanceCount_5C1BB4 > 1)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                sAlarmObjId_550D70 = mBaseGameObjectId;
            }

            mAlarmState = States::eEnabling_2;
            SfxPlayMono(relive::SoundEffects::SecurityDoorDeny, 0);
            mAlarmDurationTimer = sGnFrame + mAlarmDuration;
            mEffectBaseRed = mAlarmRed;
            break;

        case States::eAfterConstructed_1: // When not created by a map TLV
            if (EventGet(kEventHeroDying))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

#include "stdafx.h"
#include "MinesAlarm.hpp"
#include "Function.hpp"
#include "ExplosionSet.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"

s32 bCreated_5BC030 = false;
s32 sTimerValue_5C1BFC = 0;

void MinesAlarm::Create(s32 timer)
{
    if (!bCreated_5BC030)
    {
        relive_new MinesAlarm(timer);
    }
}

s32 MinesAlarm::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MinesAlarmSaveState*>(pBuffer);
    relive_new MinesAlarm(pState->field_4_timer);
    return sizeof(MinesAlarmSaveState);
}

MinesAlarm::MinesAlarm(s32 timer)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eMinesAlarm);
    sTimerValue_5C1BFC = timer;
}

s32 MinesAlarm::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<MinesAlarmSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eMinesAlarm;
    pState->field_4_timer = sTimerValue_5C1BFC;
    return sizeof(MinesAlarmSaveState);
}

MinesAlarm::~MinesAlarm()
{
    sTimerValue_5C1BFC = 0;
    bCreated_5BC030 = false;
}

void MinesAlarm::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MinesAlarm::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sTimerValue_5C1BFC > 0)
    {
        if (!(--sTimerValue_5C1BFC % 30u))
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 55, -1000);
        }
    }
    else
    {
        if (!pExplosionSet_5BBF68)
        {
            relive_new ExplosionSet();
        }
        pExplosionSet_5BBF68->Start();
        mBaseGameObjectFlags.Clear(BaseGameObject::eUpdatable_Bit2);
    }
}

#include "stdafx.h"
#include "MinesAlarm.hpp"
#include "../relive_lib/Function.hpp"
#include "ExplosionSet.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"

static bool sAlarmExists = false;
s32 gExplosionTimer = 0;

void MinesAlarm::Create(s32 timer)
{
    if (!sAlarmExists)
    {
        relive_new MinesAlarm(timer);
    }
}

s32 MinesAlarm::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MinesAlarmSaveState*>(pBuffer);
    relive_new MinesAlarm(pState->mExplosionTimer);
    return sizeof(MinesAlarmSaveState);
}

MinesAlarm::MinesAlarm(s32 timer)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eMinesAlarm);
    gExplosionTimer = timer;
}

s32 MinesAlarm::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<MinesAlarmSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eMinesAlarm;
    pState->mExplosionTimer = gExplosionTimer;
    return sizeof(MinesAlarmSaveState);
}

MinesAlarm::~MinesAlarm()
{
    gExplosionTimer = 0;
    sAlarmExists = false;
}

void MinesAlarm::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        SetDead(true);
    }
}

void MinesAlarm::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (gExplosionTimer > 0)
    {
        if (!(--gExplosionTimer % 30u))
        {
            SFX_Play_Pitch(relive::SoundEffects::RedTick, 55, -1000);
        }
    }
    else
    {
        if (!gExplosionSet)
        {
            relive_new ExplosionSet();
        }
        gExplosionSet->Start();
        SetUpdatable(false);
    }
}

#include "stdafx.h"
#include "MinesAlarm.hpp"
#include "../relive_lib/Function.hpp"
#include "ExplosionSet.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "QuikSave.hpp"

static bool sAlarmExists = false;
s32 gExplosionTimer = 0;

void MinesAlarm::Create(s32 timer)
{
    if (!sAlarmExists)
    {
        relive_new MinesAlarm(timer);
    }
}

void MinesAlarm::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<MinesAlarmSaveState>();
    relive_new MinesAlarm(pState->mExplosionTimer);
}

MinesAlarm::MinesAlarm(s32 timer)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eMinesAlarm);
    gExplosionTimer = timer;
}

void MinesAlarm::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    MinesAlarmSaveState data = {};

    data.mType = ReliveTypes::eMinesAlarm;
    data.mExplosionTimer = gExplosionTimer;
    pSaveBuffer.Write(data);
}

MinesAlarm::~MinesAlarm()
{
    gExplosionTimer = 0;
    sAlarmExists = false;
}

void MinesAlarm::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
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

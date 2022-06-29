#include "stdafx.h"
#include "MinesAlarm.hpp"
#include "Function.hpp"
#include "ExplosionSet.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

ALIVE_VAR(1, 0x5BC030, s32, bCreated_5BC030, FALSE);
ALIVE_VAR(1, 0x5C1BFC, s32, sTimerValue_5C1BFC, 0);

void MinesAlarm::Create(s32 timer)
{
    if (!bCreated_5BC030)
    {
        relive_new MinesAlarm(timer);
    }
}

s32 MinesAlarm::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MinesAlarm_State*>(pBuffer);
    relive_new MinesAlarm(pState->field_4_timer);
    return sizeof(MinesAlarm_State);
}

MinesAlarm::MinesAlarm(s32 timer)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eMinesAlarm);
    sTimerValue_5C1BFC = timer;
}

s32 MinesAlarm::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<MinesAlarm_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eMinesAlarm_25;
    pState->field_4_timer = sTimerValue_5C1BFC;
    return sizeof(MinesAlarm_State);
}

MinesAlarm::~MinesAlarm()
{
    sTimerValue_5C1BFC = 0;
    bCreated_5BC030 = FALSE;
}

void MinesAlarm::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
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
            SFX_Play_Pitch(SoundEffect::RedTick_3, 55, -1000);
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

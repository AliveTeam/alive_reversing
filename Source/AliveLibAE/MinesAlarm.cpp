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

void CC MinesAlarm::Create_4177F0(s32 timer)
{
    if (!bCreated_5BC030)
    {
        ae_new<MinesAlarm>(timer);
    }
}

s32 CC MinesAlarm::CreateFromSaveState_417740(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MinesAlarm_State*>(pBuffer);
    ae_new<MinesAlarm>(pState->field_4_timer);
    return sizeof(MinesAlarm_State);
}

MinesAlarm::MinesAlarm(s32 timer)
    : BaseGameObject(TRUE, 0)
{
    SetType(AETypes::eMinesAlarm_25);
    sTimerValue_5C1BFC = timer;
}

void MinesAlarm::VScreenChanged()
{
    vScreenChanged_417A20();
}

void MinesAlarm::VUpdate()
{
    vUpdate_417910();
}

s32 MinesAlarm::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_4177C0(reinterpret_cast<MinesAlarm_State*>(pSaveBuffer));
}

s32 MinesAlarm::vGetSaveState_4177C0(MinesAlarm_State* pState)
{
    pState->field_0_type = AETypes::eMinesAlarm_25;
    pState->field_4_timer = sTimerValue_5C1BFC;
    return sizeof(MinesAlarm_State);
}

MinesAlarm::~MinesAlarm()
{
    sTimerValue_5C1BFC = 0;
    bCreated_5BC030 = FALSE;
}

void MinesAlarm::vScreenChanged_417A20()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void MinesAlarm::vUpdate_417910()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (sTimerValue_5C1BFC > 0)
    {
        if (!(--sTimerValue_5C1BFC % 30u))
        {
            SFX_Play(SoundEffect::RedTick_3, 55, -1000);
        }
    }
    else
    {
        if (!pExplosionSet_5BBF68)
        {
            ae_new<ExplosionSet>();
        }
        pExplosionSet_5BBF68->Start_415300();
        mFlags.Clear(BaseGameObject::eUpdatable_Bit2);
    }
}

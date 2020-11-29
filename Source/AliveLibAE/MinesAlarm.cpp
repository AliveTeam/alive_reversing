#include "stdafx.h"
#include "MinesAlarm.hpp"
#include "Function.hpp"
#include "ExplosionSet.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

ALIVE_VAR(1, 0x5BC030, int, bCreated_5BC030, FALSE);
ALIVE_VAR(1, 0x5C1BFC, int, sTimerValue_5C1BFC, 0);

void CC MinesAlarm::Create_4177F0(int timer)
{
    if (!bCreated_5BC030)
    {
        auto pMinesAlarm = ae_new<MinesAlarm>();
        if (pMinesAlarm)
        {
            pMinesAlarm->ctor_417870(timer);
        }
    }
}

int CC MinesAlarm::CreateFromSaveState_417740(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const MinesAlarm_State*>(pBuffer);
    auto pMinesAlarm = ae_new<MinesAlarm>();
    if (pMinesAlarm)
    {
        pMinesAlarm->ctor_417870(pState->field_4_timer);
    }
    return sizeof(MinesAlarm_State);
}

BaseGameObject* MinesAlarm::ctor_417870(int timer)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5445FC);
    field_4_typeId = Types::eMinesAlarm_25;
    sTimerValue_5C1BFC = timer;
    return this;
}

BaseGameObject* MinesAlarm::VDestructor(signed int flags)
{
    return vdtor_4178B0(flags);
}

void MinesAlarm::VScreenChanged()
{
    vScreenChanged_417A20();
}

void MinesAlarm::VUpdate()
{
    vUpdate_417910();
}

int MinesAlarm::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_4177C0(reinterpret_cast<MinesAlarm_State*>(pSaveBuffer));
}

int MinesAlarm::vGetSaveState_4177C0(MinesAlarm_State* pState)
{
    pState->field_0_type = Types::eMinesAlarm_25;
    pState->field_4_timer = sTimerValue_5C1BFC;
    return sizeof(MinesAlarm_State);
}

void MinesAlarm::dtor_4178E0()
{
    SetVTable(this, 0x5445FC);
    sTimerValue_5C1BFC = 0;
    bCreated_5BC030 = FALSE;
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* MinesAlarm::vdtor_4178B0(signed int flags)
{
    dtor_4178E0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MinesAlarm::vScreenChanged_417A20()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level ||
        gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void MinesAlarm::vUpdate_417910()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (sTimerValue_5C1BFC > 0)
    {
        if (!(--sTimerValue_5C1BFC % 30u))
        {
            SFX_Play_46FBA0(SoundEffect::RedTick_3, 55, -1000);
        }
    }
    else
    {
        if (!pExplosionSet_5BBF68)
        {
            auto pExplosionSet = ae_new<ExplosionSet>();
            if (pExplosionSet)
            {
                pExplosionSet->ctor_414CA0();
            }
        }
        pExplosionSet_5BBF68->Start_415300();
        field_6_flags.Clear(BaseGameObject::eUpdatable_Bit2);
    }
}

#include "stdafx.h"
#include "MusicTrigger.hpp"
#include "Function.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Game.hpp"

EXPORT MusicTrigger* MusicTrigger::ctor_47FF10(__int16 type, __int16 a3, int /*a4*/, __int16 delay)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5463DC);
    Init_47FFB0(type, a3, delay);
    field_2C = 0;
    field_30 = 0;
    field_2E = 0;
    field_32 = 0;
    field_20 = -1;
    return this;
}

EXPORT void MusicTrigger::Init_47FFB0(__int16 type, __int16 a3, __int16 delay)
{
    field_24_flags &= ~7u;
    field_4_typeId = BaseGameObject::Types::eMusicTrigger_94;
    field_28_counter = 0;

    switch (type)
    {
    case 0:
        field_26 = 3;
        field_28_counter = 400;
        break;
    case 1:
        field_26 = 10;
        field_28_counter = 30;
        break;
    case 2:
        field_26 = 13;
        field_28_counter = 30;
        break;
    case 3:
        field_26 = 8;
        field_24_flags |= 4;
        break;
    case 4:
        field_26 = 7;
        field_24_flags |= 4;
        break;
    case 5:
        field_26 = 2;
        field_28_counter = delay;
        break;
    case 6:
        field_26 = 12;
        field_28_counter = 30;
        break;
    default:
        break;
    }

    if (a3)
    {
        if (a3 == 1)
        {
            field_24_flags |= 1u;
        }
    }
    else
    {
        field_1C_update_delay = delay;
    }
}

EXPORT BaseGameObject* MusicTrigger::vdtor_47FEE0(signed int flags)
{
    dtor_4800C0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

EXPORT void MusicTrigger::dtor_4800C0()
{
    SetVTable(this, 0x5463DC);
    if (field_24_flags & 4)
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }
    BaseGameObject_dtor_4DBEC0();
}

EXPORT void MusicTrigger::vScreenChange_4802A0()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

EXPORT void MusicTrigger::vUpdate_480140()
{
    if (Event_Get_422C00(kEventHeroDying))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_24_flags & 1)
    {
        FP xpos = sControlledCharacter_5C1B8C->field_B8_xpos;
        FP ypos = sControlledCharacter_5C1B8C->field_BC_ypos;

        if (xpos >= FP_FromInteger(field_2C) && xpos <= FP_FromInteger(field_30) &&
            (ypos >= FP_FromInteger(field_2E) && ypos <= FP_FromInteger(field_32)))
        {
            field_24_flags &= ~1;
            MusicController::sub_47FD60(field_26, this, (field_24_flags >> 2) & 1, 1);
            field_24_flags |= 2u;
            if (field_28_counter >= 0)
            {
                field_28_counter += sGnFrame_5C1B84;
            }
        }
    }
    else if (field_24_flags & 2)
    {
        if (field_28_counter < 0 || static_cast<int>(sGnFrame_5C1B84) < field_28_counter)
        {
            MusicController::sub_47FD60(field_26, this, (field_24_flags >> 2) & 1, 0);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        MusicController::sub_47FD60(field_26, this, (field_24_flags >> 2) & 1, 1);
        field_24_flags |= 2u;
        field_28_counter += sGnFrame_5C1B84;
    }
}

BaseGameObject* MusicTrigger::VDestructor(signed int flags)
{
    return vdtor_47FEE0(flags);
}

void MusicTrigger::VUpdate()
{
    vUpdate_480140();
}

void MusicTrigger::VScreenChanged()
{
    vScreenChange_4802A0();
}

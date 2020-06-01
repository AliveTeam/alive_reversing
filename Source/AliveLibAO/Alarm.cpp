#include "stdafx_ao.h"
#include "Alarm.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "DynamicArray.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5c1bb4, short, alarmInstanceCount_5076A8, 0);

EXPORT Alarm * Alarm::ctor_402570(__int16 a2, __int16 switchId, __int16 a4, __int16 layer)
{
    ctor_461550(layer, 1);

    SetVTable(this, 0x4BA060);
    field_6C_15_timer = a4 + gnFrameCount_507670;
    field_74_switch_id = switchId;
    field_4_typeId = Types::eAlarm_1;
    field_68_r_value = 0;
    field_6A_state = 0;
    field_70_duration_timer = field_6C_15_timer + a2;

    alarmInstanceCount_5076A8++;
    if (alarmInstanceCount_5076A8 > 1)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_5E_r = 0;
    field_60_g = 0;
    field_62_b = 0;

    if (gMap_507BA8.field_0_current_level != 5 && gMap_507BA8.field_0_current_level != 6)
    {
        return this;
    }
    else
    {
        gObjList_drawables_504618->Remove_Item(this);
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
    }
    return this;
}

END_NS_AO

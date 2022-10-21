#include "stdafx_ao.h"
#include "Alarm.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "DynamicArray.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

namespace AO {

void Alarm_ForceLink()
{ }

ALIVE_VAR(1, 0x5076A8, s16, alarmInstanceCount_5076A8, 0);

EXPORT Alarm* Alarm::ctor_402570(s16 duration_timer, s16 switchId, s16 timer, Layer layer)
{
    ctor_461550(layer, TPageAbr::eBlend_1);

    SetVTable(this, 0x4BA060);
    field_6C_15_timer = timer + gnFrameCount_507670;
    field_74_switch_id = switchId;
    field_4_typeId = Types::eAlarm_1;
    field_68_r_value = 0;
    field_6A_state = States::eAfterConstructed_0;
    field_70_duration_timer = field_6C_15_timer + duration_timer;

    alarmInstanceCount_5076A8++;
    if (alarmInstanceCount_5076A8 > 1)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_5E_r = 0;
    field_60_g = 0;
    field_62_b = 0;

    // Disable red screen flashing in the stock yards
    if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
    {
        gObjList_drawables_504618->Remove_Item(this);
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
    }
    return this;
}

BaseGameObject* Alarm::dtor_402630()
{
    SetVTable(this, 0x4BA060);
    alarmInstanceCount_5076A8--;
    SwitchStates_Set(field_74_switch_id, 0);
    return dtor_461630();
}

BaseGameObject* Alarm::VDestructor(s32 flags)
{
    return Vdtor_402830(flags);
}

Alarm* Alarm::Vdtor_402830(s32 flags)
{
    dtor_402630();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Alarm::VScreenChanged_402810()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Alarm::VRender_4027F0(PrimHeader** ppOt)
{
    if (!sNumCamSwappers_507668)
    {
        EffectBase::VRender_461690(ppOt);
    }
}


void Alarm::VUpdate()
{
    VUpdate_402660();
}

void Alarm::VUpdate_402660()
{
    Event_Broadcast_417220(kEvent_Alarm_17, this);

    if (field_10_path_id != gMap_507BA8.field_2_current_path || field_12_level_id != gMap_507BA8.field_0_current_level || static_cast<s32>(gnFrameCount_507670) > field_70_duration_timer)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    switch (field_6A_state)
    {
        case States::eAfterConstructed_0:
            if (Event_Get_417250(kEventHeroDying_3))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_6C_15_timer)
            {
                field_6A_state = States::eEnabling_1;

                SFX_Play_43AD70(SoundEffect::Alarm_45, 0, 0);

                if (field_74_switch_id)
                {
                    SwitchStates_Set(field_74_switch_id, 1);
                }
            }
            break;

        case States::eEnabling_1:
            field_68_r_value += 25;

            if (field_68_r_value >= 100)
            {
                field_68_r_value = 100;
                field_6C_15_timer = gnFrameCount_507670 + 15;
                field_6A_state = States::eOnFlash_2;
                SFX_Play_43AD70(SoundEffect::Alarm_45, 0, 0);
            }
            break;

        case States::eOnFlash_2:
            if (static_cast<s32>(gnFrameCount_507670) > field_6C_15_timer)
            {
                field_6A_state = States::eDisabling_3;
            }
            break;

        case States::eDisabling_3:
            field_68_r_value -= 25;

            if (field_68_r_value <= 0)
            {
                field_68_r_value = 0;
                field_6C_15_timer = gnFrameCount_507670 + 15;
                field_6A_state = States::eDisabled_4;
            }
            break;

        case States::eDisabled_4:
            if (Event_Get_417250(kEventHeroDying_3))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_6C_15_timer)
            {
                field_6A_state = States::eEnabling_1;
                SFX_Play_43AD70(SoundEffect::Alarm_45, 0, 0);
            }
            break;

        default:
            break;
    }

    field_5E_r = field_68_r_value;
}

void Alarm::VRender(PrimHeader** ppOt)
{
    VRender_4027F0(ppOt);
}

void Alarm::VScreenChanged()
{
    VScreenChanged_402810();
}

} // namespace AO

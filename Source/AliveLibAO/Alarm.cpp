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

Alarm::Alarm(s16 duration_timer, s16 switchId, s16 timer, Layer layer)
    : EffectBase(layer, TPageAbr::eBlend_1)
{
    field_6C_15_timer = timer + gnFrameCount_507670;
    field_74_switch_id = switchId;
    field_4_typeId = Types::eAlarm_1;
    field_68_r_value = 0;
    field_6A_state = States::eAfterConstructed_0;
    field_70_duration_timer = field_6C_15_timer + duration_timer;

    alarmInstanceCount_5076A8++;
    if (alarmInstanceCount_5076A8 > 1)
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    field_5E_r = 0;
    field_60_g = 0;
    field_62_b = 0;

    // Disable red screen flashing in the stock yards
    if (gMap.mCurrentLevel == LevelIds::eStockYards_5 || gMap.mCurrentLevel == LevelIds::eStockYardsReturn_6)
    {
        gObjList_drawables_504618->Remove_Item(this);
        mFlags.Clear(BaseGameObject::eDrawable_Bit4);
    }
}

Alarm::~Alarm()
{
    alarmInstanceCount_5076A8--;
    SwitchStates_Set(field_74_switch_id, 0);
}

void Alarm::VRender(PrimHeader** ppOt)
{
    if (!sNumCamSwappers_507668)
    {
        EffectBase::VRender_461690(ppOt);
    }
}

void Alarm::VUpdate()
{
    Event_Broadcast_417220(kEvent_Alarm_17, this);

    if (field_10_path_id != gMap.mCurrentPath || field_12_level_id != gMap.mCurrentLevel || static_cast<s32>(gnFrameCount_507670) > field_70_duration_timer)
    {
        mFlags.Set(BaseGameObject::eDead);
        return;
    }

    switch (field_6A_state)
    {
        case States::eAfterConstructed_0:
            if (Event_Get_417250(kEventHeroDying_3))
            {
                mFlags.Set(BaseGameObject::eDead);
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
                mFlags.Set(BaseGameObject::eDead);
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

void Alarm::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO

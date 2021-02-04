#include "stdafx.h"
#include "Alarm.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

ALIVE_VAR(1, 0x5c1bb4, short, alarmInstanceCount_5C1BB4, 0);
ALIVE_VAR(1, 0x550d70, int, sAlarmObjId_550D70, -1);

BaseGameObject* Alarm::VDestructor(signed int flags)
{
    return vdtor_4092D0(flags);
}

void Alarm::VUpdate()
{
    vUpdate_409460();
}

void Alarm::VRender(PrimHeader** ppOt)
{
    vRender_409710(ppOt);
}

Alarm* Alarm::ctor_409300(Path_Alarm* pTlv, int tlvInfo)
{
    ctor_4AB7A0(Layer::eLayer_39, 3);
    field_84_tlvOffsetLevelPathCamId = tlvInfo;

    SetVTable(this, 0x544074); // vTbl_GlukkonPanic_544074
    field_4_typeId = Types::eAlarm_1;

    field_78_r_value = 0;
    field_90_state = eState_0_WaitForSwitchEnable;

    // This won't count as an alarm instance till this id is enabled
    field_88_switch_id = pTlv->field_10_id;

    field_6E_r = 0;
    field_70_g = 0;
    field_72_b = 0;

    field_8A_duration = pTlv->field_12_duration;

    return this;
}

Alarm* Alarm::ctor_4091F0(__int16 durationOffset, __int16 switchId, __int16 timerOffset, Layer layer)
{
    ctor_4AB7A0(layer, 3);

    SetVTable(this, 0x544074);
    field_4_typeId = Types::eAlarm_1;

    field_78_r_value = 0;
    field_90_state = eState_1_AfterConstructed;
    field_84_tlvOffsetLevelPathCamId = 0xFFFF;
    field_7C_15_timer = sGnFrame_5C1B84 + timerOffset;
    field_80_duration_timer = field_7C_15_timer + durationOffset;
    field_88_switch_id = switchId;

    alarmInstanceCount_5C1BB4++;

    if (alarmInstanceCount_5C1BB4 > 1)
    {
        // More than one instance, kill self
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        sAlarmObjId_550D70 = field_8_object_id;
    }

    field_6E_r = 0;
    field_70_g = 0;
    field_72_b = 0;

    return this;
}

void Alarm::dtor_409380()
{
    SetVTable(this, 0x544074); // vTbl_GlukkonPanic_544074

    if (field_90_state != eState_0_WaitForSwitchEnable)
    {
        --alarmInstanceCount_5C1BB4;
    }

    if (sAlarmObjId_550D70 == field_8_object_id)
    {
        sAlarmObjId_550D70 = -1;
    }

    if (field_84_tlvOffsetLevelPathCamId == 0xFFFF)
    {
        if (field_88_switch_id)
        {
            SwitchStates_Set_465FF0(field_88_switch_id, 0);
        }
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_84_tlvOffsetLevelPathCamId, -1, 0, 0);
    }

    dtor_4AB8F0();
}

Alarm* Alarm::vdtor_4092D0(signed int flags)
{
    dtor_409380();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Alarm::vRender_409710(PrimHeader** ppOt)
{
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        EffectBase::VRender(ppOt);
    }
}

void Alarm::vUpdate_409460()
{
    if (field_90_state != eState_0_WaitForSwitchEnable)
    {
        Event_Broadcast_422BC0(kEventAlarm, this);
        if (static_cast<int>(sGnFrame_5C1B84) > field_80_duration_timer)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }
    }

    switch (field_90_state)
    {
        case eState_0_WaitForSwitchEnable:
            if (Event_Get_422C00(kEventDeathReset))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }

            if (!SwitchStates_Get_466020(field_88_switch_id))
            {
                field_6E_r = field_78_r_value;
                return;
            }

            alarmInstanceCount_5C1BB4++;
            if (alarmInstanceCount_5C1BB4 > 1)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            else
            {
                sAlarmObjId_550D70 = field_8_object_id;
            }

            field_90_state = eState_2_Enabling;
            SFX_Play_46FA90(SoundEffect::SecurityDoorDeny_38, 0);
            field_80_duration_timer = sGnFrame_5C1B84 + field_8A_duration;
            field_6E_r = field_78_r_value;
            break;

        case eState_1_AfterConstructed: // When not created by a map TLV
            if (Event_Get_422C00(kEventHeroDying))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            else
            {
                if (static_cast<int>(sGnFrame_5C1B84) <= field_7C_15_timer)
                {
                    field_6E_r = field_78_r_value;
                    return;
                }

                field_90_state = eState_2_Enabling;
                SFX_Play_46FA90(SoundEffect::SecurityDoorDeny_38, 0);

                if (!field_88_switch_id)
                {
                    field_6E_r = field_78_r_value;
                    return;
                }

                SwitchStates_Set_465FF0(field_88_switch_id, 1);
                field_6E_r = field_78_r_value;
            }
            break;

        case eState_2_Enabling:
            field_78_r_value += 25;

            if (field_78_r_value < 100)
            {
                field_6E_r = field_78_r_value;
                return;
            }

            field_78_r_value = 100;
            field_90_state = eState_3_OnFlash;
            field_7C_15_timer = sGnFrame_5C1B84 + 15;
            SFX_Play_46FA90(SoundEffect::SecurityDoorDeny_38, 0);
            field_6E_r = field_78_r_value;
            break;

        case eState_3_OnFlash:
            if (static_cast<int>(sGnFrame_5C1B84) <= field_7C_15_timer)
            {
                field_6E_r = field_78_r_value;
                return;
            }

            field_90_state = eState_4_Disabling;
            field_6E_r = field_78_r_value;
            break;

        case eState_4_Disabling:
            field_78_r_value -= 25;

            if (field_78_r_value > 0)
            {
                field_6E_r = field_78_r_value;
                return;
            }

            field_78_r_value = 0;
            field_7C_15_timer = sGnFrame_5C1B84 + 15;
            field_90_state = eState_5_Disabled;
            field_6E_r = field_78_r_value;
            break;

        case eState_5_Disabled:
            if (Event_Get_422C00(kEventHeroDying))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            else
            {
                if (static_cast<int>(sGnFrame_5C1B84) > field_7C_15_timer)
                {
                    field_90_state = eState_2_Enabling;
                    SFX_Play_46FA90(SoundEffect::SecurityDoorDeny_38, 0);
                }
                field_6E_r = field_78_r_value;
            }
            break;

        default:
            field_6E_r = field_78_r_value;
            break;
    }
}

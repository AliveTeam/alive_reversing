#include "stdafx.h"
#include "InvisibleSwitch.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"

BaseGameObject* InvisibleSwitch::VDestructor(signed int flags)
{
    return vdtor_45FAF0(flags);
}

void InvisibleSwitch::VUpdate()
{
    vUpdate_45FBA0();
}

void InvisibleSwitch::VScreenChanged()
{
    vScreenChanged_45FD80();
}

InvisibleSwitch* InvisibleSwitch::ctor_45FA70(Path_InvisibleSwitch* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545A7C); // vTbl_InvisibleSwitch_545A7C
    field_24_tlvInfo = tlvInfo;
    field_20_id = pTlv->field_10_id;
    field_22_action = pTlv->field_12_action;
    field_38_state = States::eState_WaitForTrigger_0;
    field_2C_delay = pTlv->field_14_delay;
    field_30_top_left = pTlv->field_8_top_left;
    field_34_bottom_right = pTlv->field_C_bottom_right;
    field_3A_set_off_alarm = pTlv->field_16_set_off_alarm;
    field_3C_scale = pTlv->field_18_scale;
    return this;
}

void InvisibleSwitch::dtor_45FB20()
{
    SetVTable(this, 0x545A7C); // vTbl_InvisibleSwitch_545A7C
    Path::TLV_Reset_4DB8E0(field_24_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

InvisibleSwitch* InvisibleSwitch::vdtor_45FAF0(signed int flags)
{
    dtor_45FB20();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void InvisibleSwitch::vUpdate_45FBA0()
{
    if (field_38_state == States::eState_WaitForDelayTimer_1)
    {
        if (field_28_delay_timer <= static_cast<int>(sGnFrame_5C1B84))
        {
            // Timer expired, do the operation
            SwitchStates_Do_Operation_465F00(field_20_id, field_22_action);

            // Fire alarm if set
            if (field_3A_set_off_alarm)
            {
                auto pAlarm = ae_new<Alarm>();
                if (pAlarm)
                {
                    pAlarm->ctor_4091F0(150, 0, 30, Layer::eLayer_39);
                }
            }

            // Go back to waiting for trigger
            field_38_state = States::eState_WaitForTrigger_0;
        }
    }
    else if (field_38_state == States::eState_WaitForTrigger_0)
    {
        // If not trying to turn off the target and the state is set
        if (field_22_action != SwitchOp::eSetFalse_1 || SwitchStates_Get_466020(field_20_id))
        {
            // Within X bounds?
            const FP charXPos = sControlledCharacter_5C1B8C->field_B8_xpos;
            if (charXPos >= FP_FromInteger(field_30_top_left.field_0_x) && charXPos <= FP_FromInteger(field_34_bottom_right.field_0_x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter_5C1B8C->field_BC_ypos;
                if (charYPos >= FP_FromInteger(field_30_top_left.field_2_y) && charYPos <= FP_FromInteger(field_34_bottom_right.field_2_y))
                {
                    // TODO: ???
                    if (sControlledCharacter_5C1B8C != sActiveHero_5C1B68
                        || (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_115_DoorExit_459A40
                        && sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_114_DoorEnter_459470))
                    {
                        // Scale matches ?
                        if (field_3C_scale == 2
                            || (field_3C_scale == 0 && sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(0.5))
                            || (field_3C_scale == 1 && sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(1.0)))
                        {
                            field_38_state = States::eState_WaitForDelayTimer_1;
                            field_28_delay_timer = sGnFrame_5C1B84 + field_2C_delay;
                        }
                    }
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

EXPORT void InvisibleSwitch::vScreenChanged_45FD80()
{
    BaseGameObject::VScreenChanged();
    if (field_38_state != States::eState_WaitForDelayTimer_1)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

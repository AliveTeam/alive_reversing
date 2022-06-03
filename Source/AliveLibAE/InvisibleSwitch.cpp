#include "stdafx.h"
#include "InvisibleSwitch.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"

InvisibleSwitch::InvisibleSwitch(Path_InvisibleSwitch* pTlv, u32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_24_tlvInfo = tlvInfo;
    field_20_switch_id = pTlv->field_10_switch_id;
    field_22_action = pTlv->field_12_action;
    field_38_state = States::eWaitForTrigger_0;
    field_2C_delay = pTlv->field_14_delay;
    field_30_top_left = pTlv->field_8_top_left;
    field_34_bottom_right = pTlv->field_C_bottom_right;
    field_3A_set_off_alarm = pTlv->field_16_set_off_alarm;
    field_3C_scale = pTlv->field_18_scale;
}

InvisibleSwitch::~InvisibleSwitch()
{
    Path::TLV_Reset(field_24_tlvInfo, -1, 0, 0);
}

void InvisibleSwitch::VUpdate()
{
    if (field_38_state == States::eWaitForDelayTimer_1)
    {
        if (field_28_delay_timer <= static_cast<s32>(sGnFrame_5C1B84))
        {
            // Timer expired, do the operation
            SwitchStates_Do_Operation(field_20_switch_id, field_22_action);

            // Fire alarm if set
            if (field_3A_set_off_alarm == Choice_short::eYes_1)
            {
                ae_new<Alarm>(150, 0, 30, Layer::eLayer_Above_FG1_39);
            }

            // Go back to waiting for trigger
            field_38_state = States::eWaitForTrigger_0;
        }
    }
    else if (field_38_state == States::eWaitForTrigger_0)
    {
        // If not trying to turn off the target and the state is set
        if (field_22_action != SwitchOp::eSetFalse_1 || SwitchStates_Get(field_20_switch_id))
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
                        || (sActiveHero_5C1B68->field_106_current_motion != eAbeMotions::Motion_115_DoorExit_459A40
                            && sActiveHero_5C1B68->field_106_current_motion != eAbeMotions::Motion_114_DoorEnter_459470))
                    {
                        // Scale matches ?
                        if (field_3C_scale == InvisibleSwitchScale::eAny_2
                            || (field_3C_scale == InvisibleSwitchScale::eHalf_0 && sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(0.5))
                            || (field_3C_scale == InvisibleSwitchScale::eFull_1 && sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(1.0)))
                        {
                            field_38_state = States::eWaitForDelayTimer_1;
                            field_28_delay_timer = sGnFrame_5C1B84 + field_2C_delay;
                        }
                    }
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void InvisibleSwitch::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    if (field_38_state != States::eWaitForDelayTimer_1)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

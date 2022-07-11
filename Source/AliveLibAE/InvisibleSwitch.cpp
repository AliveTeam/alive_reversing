#include "stdafx.h"
#include "InvisibleSwitch.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "../relive_lib/Events.hpp"
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
    field_30_top_left = pTlv->mTopLeft;
    field_34_bottom_right = pTlv->mBottomRight;
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
        if (field_28_delay_timer <= static_cast<s32>(sGnFrame))
        {
            // Timer expired, do the operation
            SwitchStates_Do_Operation(field_20_switch_id, field_22_action);

            // Fire alarm if set
            if (field_3A_set_off_alarm == Choice_short::eYes_1)
            {
                relive_new Alarm(150, 0, 30, Layer::eLayer_Above_FG1_39);
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
            const FP charXPos = sControlledCharacter->mXPos;
            if (charXPos >= FP_FromInteger(field_30_top_left.x) && charXPos <= FP_FromInteger(field_34_bottom_right.x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter->mYPos;
                if (charYPos >= FP_FromInteger(field_30_top_left.y) && charYPos <= FP_FromInteger(field_34_bottom_right.y))
                {
                    // TODO: ???
                    if (!IsActiveHero(sActiveHero)
                        || (sActiveHero->mCurrentMotion != eAbeMotions::Motion_115_DoorExit_459A40
                            && sActiveHero->mCurrentMotion != eAbeMotions::Motion_114_DoorEnter_459470))
                    {
                        // Scale matches ?
                        if (field_3C_scale == InvisibleSwitchScale::eAny_2
                            || (field_3C_scale == InvisibleSwitchScale::eHalf_0 && sControlledCharacter->mSpriteScale == FP_FromDouble(0.5))
                            || (field_3C_scale == InvisibleSwitchScale::eFull_1 && sControlledCharacter->mSpriteScale == FP_FromDouble(1.0)))
                        {
                            field_38_state = States::eWaitForDelayTimer_1;
                            field_28_delay_timer = sGnFrame + field_2C_delay;
                        }
                    }
                }
            }
        }
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void InvisibleSwitch::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    if (field_38_state != States::eWaitForDelayTimer_1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

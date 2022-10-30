#include "stdafx.h"
#include "InvisibleSwitch.hpp"
#include "Function.hpp"
#include "Alarm.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Abe.hpp"
#include "Path.hpp"

InvisibleSwitch::InvisibleSwitch(relive::Path_InvisibleSwitch* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    field_24_tlvInfo = tlvId;
    field_20_switch_id = pTlv->mSwitchId;
    field_22_action = pTlv->mAction;
    field_38_state = States::eWaitForTrigger_0;
    field_2C_delay = pTlv->mActivationDelay;
    field_30_top_left.x = pTlv->mTopLeftX;
    field_30_top_left.y = pTlv->mTopLeftY;
    field_34_bottom_right.x = pTlv->mBottomRightX;
    field_34_bottom_right.y = pTlv->mBottomRightY;
    field_3A_set_off_alarm = pTlv->mSetOffAlarm;
    field_3C_scale = pTlv->mScale;
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
            if (field_3A_set_off_alarm == relive::reliveChoice::eYes)
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
        if (field_22_action != relive::reliveSwitchOp::eSetFalse || SwitchStates_Get(field_20_switch_id))
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
                        || (sActiveHero->mCurrentMotion != eAbeMotions::Motion_115_DoorExit
                            && sActiveHero->mCurrentMotion != eAbeMotions::Motion_114_DoorEnter))
                    {
                        // Scale matches ?
                        if (field_3C_scale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny
                            || (field_3C_scale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf && sControlledCharacter->GetSpriteScale() == FP_FromDouble(0.5))
                            || (field_3C_scale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull && sControlledCharacter->GetSpriteScale() == FP_FromDouble(1.0)))
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

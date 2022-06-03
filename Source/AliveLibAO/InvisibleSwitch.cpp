#include "stdafx_ao.h"
#include "Function.hpp"
#include "InvisibleSwitch.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "Alarm.hpp"
#include "Events.hpp"
#include "Game.hpp"

namespace AO {

InvisibleSwitch::~InvisibleSwitch()
{
    gMap.TLV_Reset(field_14_tlvInfo, -1, 0, 0);
}

InvisibleSwitch::InvisibleSwitch(Path_InvisibleSwitch* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    field_14_tlvInfo = tlvInfo;
    field_28_state = States::eWaitForTrigger_0;
    field_10_switch_id = pTlv->field_18_switch_id;
    field_12_action = pTlv->field_1A_action;
    field_1C_delay = pTlv->field_1C_delay;
    field_2C_scale = pTlv->field_20_scale;
    field_20_top_left = pTlv->field_10_top_left;
    field_24_bottom_right = pTlv->field_14_bottom_right;
    field_2A_set_off_alarm = pTlv->field_1E_set_off_alarm;
}

void InvisibleSwitch::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mGameObjectFlags.Set(Options::eDead);
    }

    if (field_28_state != States::eWaitForDelayTimer_1)
    {
        mGameObjectFlags.Set(Options::eDead);
    }
}

void InvisibleSwitch::VUpdate()
{
    switch (field_28_state)
    {
        case States::eWaitForTrigger_0:
        {
            // sControlledCharacter can be nullptr during the game ender
            // Within X bounds?
            const FP charXPos = sControlledCharacter->mXPos;
            if (sControlledCharacter && charXPos >= FP_FromInteger(field_20_top_left.field_0_x) && charXPos <= FP_FromInteger(field_24_bottom_right.field_0_x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter->mYPos;
                if (charYPos >= FP_FromInteger(field_20_top_left.field_2_y) && charYPos <= FP_FromInteger(field_24_bottom_right.field_2_y))
                {
                    // TODO: ???
                    if (sControlledCharacter != sActiveHero
                        || (sActiveHero->mCurrentMotion != eAbeMotions::Motion_157_DoorExit_42D780
                            && sActiveHero->mCurrentMotion != eAbeMotions::Motion_156_DoorEnter_42D370))
                    {
                        // Scale matches ?
                        if (field_2C_scale == InvisibleSwitchScale::eAny_2
                            || (field_2C_scale == InvisibleSwitchScale::eHalf_0 && sControlledCharacter->mSpriteScale == FP_FromDouble(0.5))
                            || (field_2C_scale == InvisibleSwitchScale::eFull_1 && sControlledCharacter->mSpriteScale == FP_FromInteger(1)))
                        {
                            field_28_state = States::eWaitForDelayTimer_1;
                            field_18_delay_timer = gnFrameCount_507670 + field_1C_delay;
                        }
                    }
                }
            }
            break;
        }

        case States::eWaitForDelayTimer_1:
            if (field_18_delay_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                SwitchStates_Do_Operation(field_10_switch_id, field_12_action);
                if (field_2A_set_off_alarm == Choice_short::eYes_1)
                {
                    ao_new<Alarm>(150, 0, 30, Layer::eLayer_Above_FG1_39);
                }
                field_28_state = States::eWaitForTrigger_0;
            }
            break;
    }

    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO

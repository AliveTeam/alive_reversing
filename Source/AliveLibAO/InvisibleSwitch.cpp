#include "stdafx_ao.h"
#include "Function.hpp"
#include "InvisibleSwitch.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Alarm.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "Path.hpp"

namespace AO {


InvisibleSwitch::InvisibleSwitch(relive::Path_InvisibleSwitch* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    field_14_tlvInfo = tlvId;
    field_10_switch_id = pTlv->mSwitchId;
    field_12_action = pTlv->mAction;
    field_28_state = States::eWaitForTrigger_0;
    field_1C_delay = pTlv->mActivationDelay;
    field_20_top_left.x = pTlv->mTopLeftX;
    field_20_top_left.y = pTlv->mTopLeftY;
    field_24_bottom_right.x = pTlv->mBottomRightX;
    field_24_bottom_right.y = pTlv->mBottomRightY;
    mSetOffAlarm = pTlv->mSetOffAlarm;
    field_2C_scale = pTlv->mScale;
}

InvisibleSwitch::~InvisibleSwitch()
{
    Path::TLV_Reset(field_14_tlvInfo, -1, 0, 0);
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
            if (sControlledCharacter && charXPos >= FP_FromInteger(field_20_top_left.x) && charXPos <= FP_FromInteger(field_24_bottom_right.x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter->mYPos;
                if (charYPos >= FP_FromInteger(field_20_top_left.y) && charYPos <= FP_FromInteger(field_24_bottom_right.y))
                {
                    // TODO: ???
                    if (sControlledCharacter != sActiveHero
                        || (sActiveHero->mCurrentMotion != eAbeMotions::Motion_157_DoorExit
                            && sActiveHero->mCurrentMotion != eAbeMotions::Motion_156_DoorEnter))
                    {
                        // Scale matches ?
                        if (field_2C_scale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny
                            || (field_2C_scale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf && sControlledCharacter->GetSpriteScale() == FP_FromDouble(0.5))
                            || (field_2C_scale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull && sControlledCharacter->GetSpriteScale() == FP_FromInteger(1)))
                        {
                            field_28_state = States::eWaitForDelayTimer_1;
                            field_18_delay_timer = sGnFrame + field_1C_delay;
                        }
                    }
                }
            }
            break;
        }

        case States::eWaitForDelayTimer_1:
            if (field_18_delay_timer <= static_cast<s32>(sGnFrame))
            {
                SwitchStates_Do_Operation(field_10_switch_id, field_12_action);
                if (mSetOffAlarm == relive::reliveChoice::eYes)
                {
                    relive_new Alarm(150, 0, 30, Layer::eLayer_Above_FG1_39);
                }
                field_28_state = States::eWaitForTrigger_0;
            }
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void InvisibleSwitch::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_28_state != States::eWaitForDelayTimer_1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}


} // namespace AO

#include "stdafx.h"
#include "InvisibleSwitch.hpp"
#include "../relive_lib/Function.hpp"
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
    mTlvId = tlvId;
    mSwitchId = pTlv->mSwitchId;
    mAction = pTlv->mAction;
    mState = States::eWaitForTrigger_0;
    mActivationDelay = pTlv->mActivationDelay;
    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;
    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;
    mSetOffAlarm = pTlv->mSetOffAlarm;
    mScale = pTlv->mScale;
}

InvisibleSwitch::~InvisibleSwitch()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void InvisibleSwitch::VUpdate()
{
    if (mState == States::eWaitForDelayTimer_1)
    {
        if (mDelayTimer <= static_cast<s32>(sGnFrame))
        {
            // Timer expired, do the operation
            SwitchStates_Do_Operation(mSwitchId, mAction);

            // Fire alarm if set
            if (mSetOffAlarm == relive::reliveChoice::eYes)
            {
                relive_new Alarm(150, 0, 30, Layer::eLayer_Above_FG1_39);
            }

            // Go back to waiting for trigger
            mState = States::eWaitForTrigger_0;
        }
    }
    else if (mState == States::eWaitForTrigger_0)
    {
        // If not trying to turn off the target and the state is set
        if (mAction != relive::reliveSwitchOp::eSetFalse || SwitchStates_Get(mSwitchId))
        {
            // Within X bounds?
            const FP charXPos = sControlledCharacter->mXPos;
            if (charXPos >= FP_FromInteger(mTlvTopLeft.x) && charXPos <= FP_FromInteger(mTlvBottomRight.x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter->mYPos;
                if (charYPos >= FP_FromInteger(mTlvTopLeft.y) && charYPos <= FP_FromInteger(mTlvBottomRight.y))
                {
                    // TODO: ???
                    if (!IsActiveHero(sActiveHero)
                        || (sActiveHero->mCurrentMotion != eAbeMotions::Motion_115_DoorExit
                            && sActiveHero->mCurrentMotion != eAbeMotions::Motion_114_DoorEnter))
                    {
                        // Scale matches ?
                        if (mScale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny
                            || (mScale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf && sControlledCharacter->GetSpriteScale() == FP_FromDouble(0.5))
                            || (mScale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull && sControlledCharacter->GetSpriteScale() == FP_FromDouble(1.0)))
                        {
                            mState = States::eWaitForDelayTimer_1;
                            mDelayTimer = MakeTimer(mActivationDelay);
                        }
                    }
                }
            }
        }
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

void InvisibleSwitch::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    if (mState != States::eWaitForDelayTimer_1)
    {
        SetDead(true);
    }
}

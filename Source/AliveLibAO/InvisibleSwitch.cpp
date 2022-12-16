#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
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
    switch (mState)
    {
        case States::eWaitForTrigger_0:
        {
            // sControlledCharacter can be nullptr during the game ender
            // Within X bounds?
            const FP charXPos = sControlledCharacter->mXPos;
            if (sControlledCharacter && charXPos >= FP_FromInteger(mTlvTopLeft.x) && charXPos <= FP_FromInteger(mTlvBottomRight.x))
            {
                // Within Y bounds?
                const FP charYPos = sControlledCharacter->mYPos;
                if (charYPos >= FP_FromInteger(mTlvTopLeft.y) && charYPos <= FP_FromInteger(mTlvBottomRight.y))
                {
                    // TODO: ???
                    if (sControlledCharacter != sActiveHero
                        || (sActiveHero->mCurrentMotion != eAbeMotions::Motion_157_DoorExit
                            && sActiveHero->mCurrentMotion != eAbeMotions::Motion_156_DoorEnter))
                    {
                        // Scale matches ?
                        if (mScale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny
                            || (mScale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf && sControlledCharacter->GetSpriteScale() == FP_FromDouble(0.5))
                            || (mScale == relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull && sControlledCharacter->GetSpriteScale() == FP_FromInteger(1)))
                        {
                            mState = States::eWaitForDelayTimer_1;
                            mDelayTimer = sGnFrame + mActivationDelay;
                        }
                    }
                }
            }
            break;
        }

        case States::eWaitForDelayTimer_1:
            if (mDelayTimer <= static_cast<s32>(sGnFrame))
            {
                SwitchStates_Do_Operation(mSwitchId, mAction);
                if (mSetOffAlarm == relive::reliveChoice::eYes)
                {
                    relive_new Alarm(150, 0, 30, Layer::eLayer_Above_FG1_39);
                }
                mState = States::eWaitForTrigger_0;
            }
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

void InvisibleSwitch::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        SetDead(true);
    }

    if (mState != States::eWaitForDelayTimer_1)
    {
        SetDead(true);
    }
}


} // namespace AO

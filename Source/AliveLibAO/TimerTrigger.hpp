#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace relive
{
    struct Path_TimerTrigger;
}

namespace AO {

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(relive::Path_TimerTrigger* pTlv, const Guid& tlvId);
    ~TimerTrigger();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void ToggleAllIds();

private:
    u16 mInputSwitchId = 0;

    enum class TimerTriggerStates : s16
    {
        eWaitForEnabled_0 = 0,
        eWaitForFirstTrigger_1 = 1,
        eCheckForStartAgain_2 = 2,
        eWaitForSecondTrigger_3 = 3,
    };
    TimerTriggerStates mState = TimerTriggerStates::eWaitForEnabled_0;
    s16 mOutputSwitchIds[4] = {};
    Guid mTlvInfo;
    s32 mActivationDelayTimer = 0;
    s32 mActivationDelay = 0;
    s32 mStartingSwitchState = 0;
};


} // namespace AO

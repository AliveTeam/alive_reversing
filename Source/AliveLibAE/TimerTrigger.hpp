#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace relive
{
    struct Path_TimerTrigger;
}

enum class TimerTriggerStates : s16
{
    eWaitForEnabled_0 = 0,
    eWaitForFirstTrigger_1 = 1,
    eCheckForStartAgain_2 = 2,
    eWaitForSecondTrigger_3 = 3,
};

struct TimerTriggerSaveState final
{
    ReliveTypes field_0_type;
    Guid field_4_tlvInfo;
    s32 field_8_delay_timer_base;
    TimerTriggerStates field_C_state;
    s16 field_E_starting_switch_state;
};

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(relive::Path_TimerTrigger* pTlv, const Guid& tlvId);
    ~TimerTrigger();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    static s32 CreateFromSaveState(const u8* pData);

private:
    void ToggleAllIds();

private:
    s16 mInputSwitchId = 0;
    TimerTriggerStates mState = TimerTriggerStates::eWaitForEnabled_0;
    s16 mOutputSwitchIds[4] = {};
    Guid mTlvInfo;
    s32 mActivationDelayTimer = 0;
    s32 mActivationDelay = 0;
    s16 mStartingSwitchState = 0;
};

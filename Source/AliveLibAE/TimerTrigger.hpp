#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_TimerTrigger final : public Path_TLV
{
    s16 mInputSwitchId;
    u16 mActivationDelay;
    s16 mOutputSwitchId1;
    s16 mOutputSwitchId2;
    s16 mOutputSwitchId3;
    s16 mOutputSwitchId4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x1C);

enum class TimerTriggerStates : s16
{
    eWaitForEnabled_0 = 0,
    eWaitForFirstTrigger_1 = 1,
    eCheckForStartAgain_2 = 2,
    eWaitForSecondTrigger_3 = 3,
};

struct TimerTrigger_State final
{
    AETypes field_0_type;
    s32 field_4_tlvInfo;
    s32 field_8_delay_timer_base;
    TimerTriggerStates field_C_state;
    s16 field_E_starting_switch_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TimerTrigger_State, 0x10);

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(relive::Path_TimerTrigger* pTlv, s32 tlvInfo);
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
    s32 mTlvInfo = 0;
    s32 mActivationDelayTimer = 0;
    s32 mActivationDelay = 0;
    s16 mStartingSwitchState = 0;
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x3C);

#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

struct Path_TimerTrigger final : public Path_TLV
{
    s16 mInputSwitchId;
    u16 mActivationDelay;
    s16 mOutputSwitchId1;
    s16 mOutputSwitchId2;
    s16 mOutputSwitchId3;
    s16 mOutputSwitchId4;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimerTrigger, 0x24);

class TimerTrigger final : public BaseGameObject
{
public:
    TimerTrigger(relive::Path_TimerTrigger* pTlv, const Guid& tlvId);
    ~TimerTrigger();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void ToggleAllIds();

    u16 mInputSwitchId = 0;

    enum class State : s16
    {
        eWaitForEnabled_0 = 0,
        eWaitForFirstTrigger_1 = 1,
        eCheckForStartAgain_2 = 2,
        eWaitForSecondTrigger_3 = 3,
    };
    State mState = State::eWaitForEnabled_0;
    s16 mOutputSwitchIds[4] = {};
    Guid mTlvInfo;
    s32 mActivationDelayTimer = 0;
    s32 mActivationDelay = 0;
    s16 mStartingSwitchState = 0;
};
ALIVE_ASSERT_SIZEOF(TimerTrigger, 0x2C);


} // namespace AO

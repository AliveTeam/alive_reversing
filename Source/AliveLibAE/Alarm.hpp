#pragma once

#include "Function.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

struct Path_Alarm;

class Alarm final : public EffectBase
{
public:
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Alarm(Path_Alarm* pTlv, s32 tlvInfo);
    Alarm(s32 durationOffset, s32 switchId, s32 timerOffset, Layer layer);
    ~Alarm();

private:
    s16 mAlarmRed;
    s32 mAlarmPauseTimer;
    s32 mAlarmDurationTimer;
    s32 mAlarmTlvInfo;
    s16 mAlarmSwitchId;
    u16 mAlarmDuration;
    enum class States : s16
    {
        eWaitForSwitchEnable_0 = 0,
        eAfterConstructed_1 = 1,
        eEnabling_2 = 2,
        eOnFlash_3 = 3,
        eDisabling_4 = 4,
        eDisabled_5 = 5,
    };
    States mAlarmState;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x94);

ALIVE_VAR_EXTERN(s16, alarmInstanceCount_5C1BB4);
ALIVE_VAR_EXTERN(s32, sAlarmObjId_550D70);

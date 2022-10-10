#pragma once

#include "Function.hpp"
#include "EffectBase.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

struct Path_Alarm;

class Alarm final : public EffectBase
{
public:
    
    Alarm(relive::Path_Alarm* pTlv, const Guid& tlvId);
    Alarm(s32 durationOffset, s32 switchId, s32 timerOffset, Layer layer);
    ~Alarm();

private:

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s16 mAlarmRed = 0;
    s32 mAlarmPauseTimer = 0;
    s32 mAlarmDurationTimer = 0;
    Guid mAlarmTlvInfo;
    s16 mAlarmSwitchId = 0;
    u16 mAlarmDuration = 0;
    enum class States : s16
    {
        eWaitForSwitchEnable_0 = 0,
        eAfterConstructed_1 = 1,
        eEnabling_2 = 2,
        eOnFlash_3 = 3,
        eDisabling_4 = 4,
        eDisabled_5 = 5,
    };
    States mAlarmState = States::eWaitForSwitchEnable_0;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x94);

extern s16 alarmInstanceCount_5C1BB4;
extern Guid sAlarmObjId_550D70;

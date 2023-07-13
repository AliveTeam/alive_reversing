#pragma once

#include "../relive_lib/GameObjects/EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

namespace relive
{
    struct Path_Alarm;
}

class Alarm final : public EffectBase
{
public:
    
    Alarm(relive::Path_Alarm* pTlv, const Guid& tlvId);
    Alarm(s32 durationOffset, u16 switchId, s32 timerOffset, Layer layer);
    ~Alarm();

private:

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

    s16 mAlarmRed = 0;
    s32 mAlarmPauseTimer = 0;
    s32 mAlarmDurationTimer = 0;
    Guid mAlarmTlvInfo;
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
    u16 mAlarmSwitchId = 0;
    u16 mAlarmDuration = 0;
};

extern s16 gAlarmInstanceCount;
extern Guid gAlarmObjId;

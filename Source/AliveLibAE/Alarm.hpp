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
    Alarm(s32 durationTimer, u16 switchId, s32 pauseTimer, Layer layer);
    ~Alarm();

private:

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

    s16 mAlarmRed = 0;
    s32 mAlarmPauseTimer = 0;
    s32 mAlarmDurationTimer = 0;
    Guid mAlarmTlvInfo;
    enum class States
    {
        eWaitForSwitchEnable,
        eAfterConstructed,
        eEnabling,
        eOnFlash,
        eDisabling,
        eDisabled,
    };
    States mAlarmState = States::eWaitForSwitchEnable;
    u16 mAlarmSwitchId = 0;
    u16 mAlarmDuration = 0;
};

extern s16 gAlarmInstanceCount;
extern Guid gAlarmObjId;

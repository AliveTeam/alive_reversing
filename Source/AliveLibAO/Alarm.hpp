#pragma once

#include "../relive_lib/GameObjects/EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

class Alarm final : public EffectBase
{
public:
    Alarm(relive::Path_Alarm* pTlv, const Guid& tlvId);
    Alarm(s32 durationTimer, u16 switchId, s32 pauseTimer, Layer layer);
    ~Alarm();

    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;

private:
    s16 mAlarmRed = 0;
    Guid mAlarmTlvInfo;
    enum class States
    {
        eWaitForSwitchEnable,
        eAfterConstructed,
        eEnabling,
        eOnFlash,
        eDisabling,
        eDisabled
    };
    States mAlarmState = States::eAfterConstructed;
    s32 mAlarmPauseTimer = 0;
    s32 mAlarmDurationTimer = 0;
    u16 mAlarmSwitchId = 0;
    u16 mAlarmDuration = 0;
};

extern s16 gAlarmInstanceCount;

} // namespace AO

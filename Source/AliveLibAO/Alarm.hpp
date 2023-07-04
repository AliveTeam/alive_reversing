#pragma once

#include "../relive_lib/GameObjects/EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

class Alarm final : public EffectBase
{
public:
    Alarm(s32 durationOffset, u16 switchId, s32 timer, Layer layer);
    ~Alarm();

    virtual void VScreenChanged() override;
    virtual void VRender(BasePrimitive** ppOt) override;
    virtual void VUpdate() override;

private:
    s16 mAlarmRed = 0;
    enum class States : s16
    {
        eAfterConstructed_0 = 0,
        eEnabling_1 = 1,
        eOnFlash_2 = 2,
        eDisabling_3 = 3,
        eDisabled_4 = 4
    };
    States mAlarmState = States::eAfterConstructed_0;
    s32 mAlarmPauseTimer = 0;
    s32 mAlarmDurationTimer = 0;
    u16 mAlarmSwitchId = 0;
};

extern s16 gAlarmInstanceCount;

} // namespace AO

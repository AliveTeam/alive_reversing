#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

void Alarm_ForceLink();


class Alarm final : public EffectBase
{
public:
    Alarm(s32 a2, s32 switchId, s32 a4, Layer layer);
    ~Alarm();

    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

private:
    s16 mAlarmRed;
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
    s16 mAlarmSwitchId = 0;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x7C);

ALIVE_VAR_EXTERN(s16, alarmInstanceCount_5076A8);

} // namespace AO

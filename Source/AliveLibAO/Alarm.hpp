#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

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
    s16 field_68_r_value;
    enum class States : s16
    {
        eAfterConstructed_0 = 0,
        eEnabling_1 = 1,
        eOnFlash_2 = 2,
        eDisabling_3 = 3,
        eDisabled_4 = 4
    };
    States field_6A_state;
    s32 field_6C_15_timer;
    s32 field_70_duration_timer;
    s16 field_74_switch_id;
    s16 field_76_padding;
    s32 field_78_padding;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x7C);

ALIVE_VAR_EXTERN(s16, alarmInstanceCount_5076A8);

} // namespace AO

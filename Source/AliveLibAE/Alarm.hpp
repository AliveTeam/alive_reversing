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
    s16 field_78_r_value;
    s16 field_7A_padding;
    s32 field_7C_15_timer;
    s32 field_80_duration_timer;
    s32 field_84_tlvOffsetLevelPathCamId;
    s16 field_88_switch_id;
    u16 field_8A_duration;
    s32 field_8C_padding;
    enum class States : s16
    {
        eWaitForSwitchEnable_0 = 0,
        eAfterConstructed_1 = 1,
        eEnabling_2 = 2,
        eOnFlash_3 = 3,
        eDisabling_4 = 4,
        eDisabled_5 = 5,
    };
    States field_90_state;
    s16 field_92_padding;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x94);

ALIVE_VAR_EXTERN(s16, alarmInstanceCount_5C1BB4);
ALIVE_VAR_EXTERN(s32, sAlarmObjId_550D70);

#pragma once

#include "Function.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

struct Path_Alarm;

class Alarm : public EffectBase
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT Alarm* ctor_409300(Path_Alarm* pTlv, s32 tlvInfo);
    EXPORT Alarm* ctor_4091F0(s16 durationOffset, s16 switchId, s16 timerOffset, Layer layer);
private:
    EXPORT void dtor_409380();
    EXPORT Alarm* vdtor_4092D0(s32 flags);
    EXPORT void vRender_409710(PrimHeader** ppOt);
    EXPORT void vUpdate_409460();
private:
    s16 field_78_r_value;
    s16 field_7A_padding;
    s32 field_7C_15_timer;
    s32 field_80_duration_timer;
    s32 field_84_tlvOffsetLevelPathCamId;
    s16 field_88_switch_id;
    WORD field_8A_duration;
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

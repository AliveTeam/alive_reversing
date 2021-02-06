#pragma once

#include "Function.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

struct Path_Alarm;

class Alarm : public EffectBase
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT Alarm* ctor_409300(Path_Alarm* pTlv, int tlvInfo);
    EXPORT Alarm* ctor_4091F0(__int16 durationOffset, __int16 switchId, __int16 timerOffset, Layer layer);
private:
    EXPORT void dtor_409380();
    EXPORT Alarm* vdtor_4092D0(signed int flags);
    EXPORT void vRender_409710(PrimHeader** ppOt);
    EXPORT void vUpdate_409460();
private:
    __int16 field_78_r_value;
    __int16 field_7A_padding;
    int field_7C_15_timer;
    int field_80_duration_timer;
    int field_84_tlvOffsetLevelPathCamId;
    __int16 field_88_switch_id;
    WORD field_8A_duration;
    int field_8C_padding;
    enum class States : __int16
    {
        eWaitForSwitchEnable_0 = 0,
        eAfterConstructed_1 = 1,
        eEnabling_2 = 2,
        eOnFlash_3 = 3,
        eDisabling_4 = 4,
        eDisabled_5 = 5,
    };
    States field_90_state;
    __int16 field_92_padding;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x94);

ALIVE_VAR_EXTERN(short, alarmInstanceCount_5C1BB4);
ALIVE_VAR_EXTERN(int, sAlarmObjId_550D70);

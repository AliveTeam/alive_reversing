#pragma once

#include "Function.hpp"
#include "EffectBase.hpp"

struct Path_Alarm;

class Alarm : public EffectBase
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;

    EXPORT Alarm* ctor_409300(Path_Alarm* pTlv, int tlvInfo);
    EXPORT Alarm* ctor_4091F0(__int16 a2, __int16 switchId, __int16 a4, __int16 layer);
private:
    EXPORT void dtor_409380();
    EXPORT Alarm* vdtor_4092D0(signed int flags);
    EXPORT void vRender_409710(int **pOt);
    EXPORT void vUpdate_409460();
private:
    __int16 field_78_b_value;
    __int16 field_7A;
    int field_7C_15_timer;
    int field_80_duration_timer;
    int field_84_tlvOffsetLevelPathCamId;
    __int16 field_88_switch_id;
    WORD field_8A_duration;
    int field_8C;
    enum States : __int16
    {
        eState_0_WaitForSwitchEnable = 0,
        eState_1_AfterConstructed = 1,
        eState_2_Enabling = 2,
        eState_3_OnFlash = 3,
        eState_4_Disabling = 4,
        eState_5_Disabled = 5,
    };
    States field_90_state;
    __int16 field_92;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x94);

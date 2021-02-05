#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

namespace AO {

void Alarm_ForceLink();


class Alarm : public EffectBase
{
public:
    EXPORT Alarm* ctor_402570(__int16 a2, __int16 switchId, __int16 a4, Layer layer);

    EXPORT BaseGameObject* dtor_402630();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Alarm* Vdtor_402830(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_402810();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_4027F0(PrimHeader** ppOt);

    virtual void VUpdate() override;

    EXPORT void VUpdate_402660();
private:
    __int16 field_68_r_value;
    enum class States : __int16
    {
        eAfterConstructed_0 = 0,
        eEnabling_1 = 1,
        eOnFlash_2 = 2,
        eDisabling_3 = 3,
        eDisabled_4 = 4
    };
    States field_6A_state;
    int field_6C_15_timer;
    int field_70_duration_timer;
    __int16 field_74_switch_id;
    __int16 field_76_padding;
    int field_78_padding;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x7C);

ALIVE_VAR_EXTERN(short, alarmInstanceCount_5076A8);

}

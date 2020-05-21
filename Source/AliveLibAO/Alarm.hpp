#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"

START_NS_AO

class Alarm : public EffectBase
{
public:
    EXPORT Alarm* ctor_402570(__int16 a2, __int16 switchId, __int16 a4, __int16 layer);
private:
    __int16 field_68_r_value;
    __int16 field_6A_state;
    int field_6C_15_timer;
    int field_70_duration_timer;
    __int16 field_74_switch_id;
    __int16 field_76;
    int field_78;
};
ALIVE_ASSERT_SIZEOF(Alarm, 0x7C);

END_NS_AO

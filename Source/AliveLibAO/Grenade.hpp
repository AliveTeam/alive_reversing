#pragma once

#include "FunctionFwd.hpp"
#include "Throwable.hpp"

START_NS_AO


class Grenade : public BaseThrowable
{
public:
    EXPORT Grenade* ctor_41EBD0(FP xpos, FP ypos, __int16 numGrenades);

    EXPORT BaseGameObject* dtor_41ECD0();

    __int16 field_110_state;
    __int16 field_112_explode_timer;
    int field_114;
    __int16 field_118;
    __int16 field_11A;
    BaseGameObject* field_11C;
    FP field_120;
    FP field_124;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x128);

END_NS_AO


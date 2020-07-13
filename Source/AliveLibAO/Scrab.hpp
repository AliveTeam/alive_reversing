#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void Scrab_ForceLink();

START_NS_AO

class Scrab : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_45B360();

    EXPORT void VUpdate_45B360();

    int field_10C_fn;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114;
    __int16 field_116;
    int field_118;
    BaseGameObject* field_11C;
    BaseAliveGameObject* field_120;
    int field_124;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    int field_14C;
    int field_150_resources[14];
    __int16 field_188;
    __int16 field_18A;
};
ALIVE_ASSERT_SIZEOF(Scrab, 0x18C);

END_NS_AO


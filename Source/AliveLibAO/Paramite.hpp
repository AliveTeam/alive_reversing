#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void Paramite_ForceLink();

START_NS_AO

class ParamiteWeb;

class Paramite : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_44A490();

    EXPORT void VUpdate_44A490();

    int field_10C_fn;
    __int16 field_110_state;
    __int16 field_112;
    int field_114;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    int field_120;
    int field_124;
    __int16 field_128;
    __int16 field_12A;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    int field_144;
    int field_148;
    ParamiteWeb* field_14C_pWeb;
    int field_150_resources[16];
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x190);

END_NS_AO


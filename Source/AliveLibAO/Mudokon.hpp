#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void Mud_ForceLink();

START_NS_AO

class Mudokon : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_43F560();

    EXPORT void VUpdate_43F560();

    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    __int16 field_124;
    __int16 field_126;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    __int16 field_13C;
    __int16 field_13E;
    int field_140;
    __int16 field_144;
    __int16 field_146;
    int field_148[15];
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    int field_18C;
    int field_190;
    int field_194;
    __int16 field_198;
    __int16 field_19A;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    int field_1A4;
    __int16 field_1A8;
    __int16 field_1AA;
    int field_1AC;
    __int16 field_1B0;
    __int16 field_1B2;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8;
    __int16 field_1BA;
    __int16 field_1BC;
    __int16 field_1BE;
    int field_1C0;
    __int16 field_1C4;
    __int16 field_1C6;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x1C8);

END_NS_AO


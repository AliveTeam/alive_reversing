#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void Slog_ForceLink();

START_NS_AO

class Slog : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_4739C0();

    EXPORT void VUpdate_4739C0();

    BaseAliveGameObject* field_10C;
    int field_110;
    __int16 field_114;
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    int field_11C;
    int field_120;
    __int16 field_124;
    __int16 field_126;
    int field_128;
    int field_12C;
    int field_130;
    __int16 field_134;
    __int16 field_136;
    int field_138;
    __int16 field_13C;
    __int16 field_13E;
    int field_140;
    int field_144;
    int field_148;
    BaseAliveGameObject* field_14C;
    int field_150;
    __int16 field_154;
    __int16 field_156;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    int field_160;
    int field_164;
    __int16 field_168;
    __int16 field_16A;
    int field_16C;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A;
    __int16 field_17C;
    __int16 field_17E;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
};
ALIVE_ASSERT_SIZEOF(Slog, 0x198);

END_NS_AO


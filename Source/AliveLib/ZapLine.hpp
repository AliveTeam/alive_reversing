#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class ZapLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZapLine* ctor_4CC690(FP xpos, FP ypos, FP a4, FP a5, __int16 a6, signed __int16 type, __int16 layer);

private:
    __int16 field_E4;
    __int16 field_E6;
    __int16 field_E8;
    __int16 field_EA;
    __int16 field_EC;
    __int16 field_EE;
    __int16 field_F0;
    __int16 field_F2;
    __int16 field_F4_state;
    __int16 field_F6;
    int field_F8_ppRes;
    __int16 field_FC_tPage_p8;
    __int16 field_FE;
    __int16 field_100;
    __int16 field_102;
    __int16 field_104;
    __int16 field_106;
    __int16 field_108;
    __int16 field_10A;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114;
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    __int16 field_128;
    __int16 field_12A_type;
    __int16 field_12C;
    __int16 field_12E_count;
    __int16 field_130_count;
    __int16 field_132_total_count;
    void *field_134_pSprts;
    int field_138_buf1;
    int field_13C_buf2;
    int field_140_buf3;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150;
    __int16 field_152;
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x154);

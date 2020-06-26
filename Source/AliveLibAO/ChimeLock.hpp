#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

START_NS_AO

class Bells;

struct Path_ChimeLock : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_solve_id;
    __int16 field_1C_code1;
    __int16 field_1E_code2;
    __int16 field_20_id;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ChimeLock, 0x24);


class ChimeLock : public BaseAliveGameObject
{
public:
    EXPORT ChimeLock* ctor_40AB20(Path_ChimeLock* pTlv, signed int tlvInfo);

    EXPORT BaseGameObject* dtor_40AE60();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ChimeLock* Vdtor_40BD40(signed int flags);

    int field_10C_tlvInfo;
    __int16 field_110;
    __int16 field_112;
    Bells* field_114_bells;
    Bells* field_118_bells;
    Bells* field_11C_bells;
    unsigned __int16 field_120;
    __int16 field_122;
    int field_124_code1;
    __int16 field_128;
    __int16 field_12A;
    int field_12C;
    __int16 field_130;
    __int16 field_132_solve_id;
    __int16 field_134;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    FP field_13C;
    FP field_140_ypos;
    int field_144;
    int field_148;
    FP field_14C;
    int field_150;
    int field_154;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    __int16 field_160;
    __int16 field_162;
    __int16 field_164;
    __int16 field_166;
};
ALIVE_ASSERT_SIZEOF(ChimeLock, 0x168);

END_NS_AO


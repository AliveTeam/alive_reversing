#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

struct SparkRes
{
    FP field_0_x0;
    FP field_4_y0;
    FP field_8_x1;
    FP field_C_y1;
    BYTE field_10_ang;
    // 3 byte pad
    FP field_14_radius;
    FP field_18_len;
    Line_G2 field_1C_pLineG2s[2];
};
ALIVE_ASSERT_SIZEOF(SparkRes, 0x4C);

class Spark : public BaseGameObject
{
public:
    EXPORT Spark* ctor_477B70(FP xpos, FP ypos, FP scale, unsigned __int8 count, __int16 min, __int16 max);

    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    FP field_30_xpos;
    FP field_34_ypos;
    FP field_38_scale;
    __int16 field_3C_r;
    __int16 field_3E_g;
    __int16 field_40_b;
    __int16 field_42_layer;
    BYTE** field_44_ppSprxRes;
    SparkRes* field_48_pRes;
    __int16 field_4C_count;
    __int16 field_4E;
    int field_50_timer;
};


END_NS_AO

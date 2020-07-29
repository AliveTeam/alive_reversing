#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

void ZapLine_ForceLink();

START_NS_AO

struct ZapLineSprites
{
    Prim_Sprt field_0_sprts[2];
};
ALIVE_ASSERT_SIZEOF(ZapLineSprites, 0x30);

struct ZapPoint
{
    FP field_0;
    FP field_4;
    FP field_8;
};
ALIVE_ASSERT_SIZEOF(ZapPoint, 0xC);

class ZapLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZapLine* ctor_4789A0(FP x1, FP y1, FP x2, FP y2, __int16 aliveTime, signed __int16 type, __int16 layer);

    EXPORT BaseGameObject* dtor_478E90();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ZapLine* Vdtor_479B20(signed int flags);

    EXPORT void sub_478CF0(FP x1, FP y1, FP x2, FP y2);

    int field_D4[4];

    __int16 field_E4;
    __int16 field_E6;
    BYTE** field_E8_ppRes;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114_tPageMode;
    __int16 field_116;
    __int16 field_118_max_alive_time;
    __int16 field_11A_type;
    __int16 field_11C_tPageAbr;
    __int16 field_11E_part_count;
    __int16 field_120_count_per_part;
    __int16 field_122_pSprts_count;
    ZapLineSprites* field_124_pSprts;
    PSX_Point* field_128_buf1;
    ZapPoint* field_12C_buf2;
    FP_Point* field_130_buf3;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x144);

END_NS_AO


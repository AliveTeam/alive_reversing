#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

struct ZapLineSprites
{
    Prim_Sprt field_0_sprts[2];
};
ALIVE_ASSERT_SIZEOF(ZapLineSprites, 0x30);

class ZapLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZapLine* ctor_4CC690(FP x1, FP y1, FP x2, FP y2, __int16 aliveTime, signed __int16 type, __int16 layer);

private:
    EXPORT void sub_4CCAD0(FP x1, FP y1, FP x2, FP y2);

    EXPORT void dtor_4CCCB0();

    EXPORT void vScreenChanged_4CDBE0();

private:
    int field_E4_not_used[4];
    __int16 field_F4_state;
    __int16 field_F6_pad;
    BYTE** field_F8_ppRes;
    Prim_SetTPage field_FC_tPage_p8[2];
    __int16 field_11C_x1;
    __int16 field_11E_y1;
    __int16 field_120_x2;
    __int16 field_122_y2;
    __int16 field_124_tPageMode;
    __int16 field_126_alive_timer;
    __int16 field_128_max_alive_time;
    __int16 field_12A_type;
    __int16 field_12C;
    __int16 field_12E_part_count;
    __int16 field_130_count_per_part;
    __int16 field_132_pSprts_count;
    ZapLineSprites* field_134_pSprts;
    void* field_138_buf1;
    void* field_13C_buf2;
    void* field_140_buf3;
    PSX_RECT field_144_rects[2];
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x154);

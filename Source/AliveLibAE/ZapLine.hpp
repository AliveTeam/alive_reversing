#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

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
    EXPORT ZapLine* ctor_4CC690(FP x1, FP y1, FP x2, FP y2, __int16 aliveTime, signed __int16 type, __int16 layer);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
private:
    EXPORT ZapLine* vdtor_4CCAA0(signed int flags);
public:
    EXPORT void sub_4CCAD0(FP x1, FP y1, FP x2, FP y2);
private:
    EXPORT void dtor_4CCCB0();
    EXPORT void vScreenChanged_4CDBE0();
    EXPORT void State_0_2_4_4CCD50();
    EXPORT void State_0_2_4_4CD110();
    EXPORT void State_0_4CD340();
    EXPORT void sub_4CD400(int idx1, int idx2, int idx3, __int16 idx4);
    EXPORT void State_2_4_4CD650();
    EXPORT void State_1_3_4CD5D0();
    EXPORT void vUpdate_4CD790();
    EXPORT void vRender_4CD8C0(int** pOt);

private:
    enum class State : __int16
    {
        State_0 = 0,
        State_1 = 1,
        State_2 = 2,
        State_3 = 3,
        State_4 = 4,
    };

    State field_F4_state;
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
    __int16 field_12C_tPageAbr;
    __int16 field_12E_part_count;
    __int16 field_130_count_per_part;
    __int16 field_132_pSprts_count;
    ZapLineSprites* field_134_pSprts;
    PSX_Point* field_138_buf1;
    ZapPoint* field_13C_buf2;
    FP_Point* field_140_buf3;
    PSX_RECT field_144_rects[2];
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x154);

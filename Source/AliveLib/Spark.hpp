#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"

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
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    EXPORT Spark* ctor_4CBBB0(FP xpos, FP ypos, FP scale, unsigned __int8 count, __int16 min, __int16 max, __int16 type);
private:
    EXPORT void vUpdate_4CBEF0();
    EXPORT void vRender_4CC050(int** ppOt);
    EXPORT void vScreenChange_4CC4A0();
    EXPORT Spark* vdtor_4CBE30(signed int flags);
    EXPORT void dtor_4CBE60();
private:
    Prim_SetTPage field_20_tPage[2];
    FP field_40_xpos;
    FP field_44_ypos;
    FP field_48_scale;
    __int16 field_4C_r;
    __int16 field_4E_g;
    __int16 field_50_b;
    __int16 field_52_layer;
    BYTE **field_54_ppSprxRes;
    SparkRes *field_58_pRes;
    __int16 field_5C_count;
    //__int16 field_5E_pad;
    int field_60_timer;
    __int16 field_64_type;
    //__int16 field_66_pad;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x68);

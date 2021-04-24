#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "BaseGameObject.hpp"

void Spark_ForceLink(); 

namespace AO {

struct SparkRes
{
    FP field_0_x0;
    FP field_4_y0;
    FP field_8_x1;
    FP field_C_y1;
    u8 field_10_ang;
    // 3 byte pad
    FP field_14_radius;
    FP field_18_len;
    Line_G2 field_1C_pLineG2s[2];
};
ALIVE_ASSERT_SIZEOF(SparkRes, 0x4C);

class Spark : public BaseGameObject
{
public:
    EXPORT Spark* ctor_477B70(FP xpos, FP ypos, FP scale, u8 count, s16 min, s16 max);

    EXPORT BaseGameObject* dtor_477D40();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Spark* Vdtor_478290(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_478280();

    virtual void VUpdate() override;

    EXPORT void VUpdate_477DB0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_477ED0(PrimHeader** ppOt);

    Prim_SetTPage field_10_tPage[2];
    FP field_30_xpos;
    FP field_34_ypos;
    FP field_38_scale;
    s16 field_3C_r;
    s16 field_3E_g;
    s16 field_40_b;
    Layer field_42_layer;
    u8** field_44_ppSprxRes;
    SparkRes* field_48_pRes;
    s16 field_4C_count;
    s16 field_4E;
    s32 field_50_timer;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x54);


}

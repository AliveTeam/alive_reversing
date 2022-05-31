#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "BaseGameObject.hpp"

void Spark_ForceLink();

namespace AO {

struct SparkRes final
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

class Spark final : public BaseGameObject
{
public:
    Spark(FP xpos, FP ypos, FP scale, s32 count, s32 min, s32 max);
    ~Spark();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

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


} // namespace AO

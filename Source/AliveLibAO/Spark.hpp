#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/BaseGameObject.hpp"

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

    Prim_SetTPage field_10_tPage[2] = {};
    FP field_30_xpos = {};
    FP field_34_ypos = {};
    FP field_38_scale = {};
    s16 field_3C_r = 0;
    s16 field_3E_g = 0;
    s16 field_40_b = 0;
    Layer field_42_layer = Layer::eLayer_0;
    u8** field_44_ppSprxRes = nullptr;
    SparkRes* field_48_pRes = nullptr;
    s16 field_4C_count = 0;
    s16 field_4E = 0;
    s32 field_50_timer = 0;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x54);


} // namespace AO

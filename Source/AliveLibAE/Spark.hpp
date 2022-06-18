#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

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

enum class SparkType : s16
{
    eSmallChantParticle_0 = 0,
    eBigChantParticle_1 = 1
};

class Spark final : public BaseGameObject
{
public:
    Spark(FP xpos, FP ypos, FP scale, s32 count, s32 minAngle, s32 maxAngle, SparkType type);
    ~Spark();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

private:
    Prim_SetTPage field_20_tPage[2] = {};
    FP field_40_xpos = {};
    FP field_44_ypos = {};
    FP field_48_scale = {};
    s16 field_4C_r = 0;
    s16 field_4E_g = 0;
    s16 field_50_b = 0;
    Layer field_52_layer = Layer::eLayer_0;
    u8** field_54_ppSprxRes = nullptr;
    SparkRes* field_58_pRes = nullptr;
    s16 field_5C_count = 0;
    s32 field_60_timer = 0;
    SparkType field_64_type = SparkType::eSmallChantParticle_0;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x68);

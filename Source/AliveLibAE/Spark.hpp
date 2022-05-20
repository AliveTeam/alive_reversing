#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
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
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    Spark(FP xpos, FP ypos, FP scale, s32 count, s32 minAngle, s32 maxAngle, SparkType type);
    ~Spark();

private:
    Prim_SetTPage field_20_tPage[2];
    FP field_40_xpos;
    FP field_44_ypos;
    FP field_48_scale;
    s16 field_4C_r;
    s16 field_4E_g;
    s16 field_50_b;
    Layer field_52_layer;
    u8** field_54_ppSprxRes;
    SparkRes* field_58_pRes;
    s16 field_5C_count;
    s32 field_60_timer;
    SparkType field_64_type;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x68);

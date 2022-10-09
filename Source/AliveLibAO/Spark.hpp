#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Primitives.hpp"
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

class Spark final : public ::BaseGameObject
{
public:
    Spark(FP xpos, FP ypos, FP scale, s32 count, s32 min, s32 max);
    ~Spark();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Prim_SetTPage mTPage[2] = {};
    FP mXPos = {};
    FP mYPos = {};
    FP mSpriteScale = {};
    s16 mRed = 0;
    s16 mGreen = 0;
    s16 mBlue = 0;
    Layer mLayer = Layer::eLayer_0;
    SparkRes* mSparkRes = nullptr;
    s16 mSparkCount = 0;
    s32 mTimer = 0;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x54);


} // namespace AO

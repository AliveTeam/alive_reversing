#pragma once

#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

struct SparkRes final
{
    FP mX0;
    FP mY0;
    FP mX1;
    FP mY1;
    u8 mAng;
    // 3 byte pad
    FP mRadius;
    FP mLen;
    Line_G2 mLineG2s[2];
};

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
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;

private:
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
    SparkType mSparkType = SparkType::eSmallChantParticle_0;
};

} // namespace AO

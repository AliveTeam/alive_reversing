#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

struct BloodParticle final
{
    FP x;
    FP y;
    FP mOffX;
    FP mOffY;
    Poly_FT4 mPoly;
};
ALIVE_ASSERT_SIZEOF(BloodParticle, 0x40);

class Blood final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Blood(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s32 count);
    ~Blood();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;

private:
    s16 mBloodXPos = 0;
    s16 mBloodYPos = 0;
    u8 mRandSeed = 0;
    s32 mTotalBloodCount = 0;
    s32 mCurrentBloodCount = 0;
    BloodParticle* mBloodParticle = nullptr;
    s32 mUpdateCalls = 0;
    Layer mOtLayer = Layer::eLayer_0;
};

} // namespace AO

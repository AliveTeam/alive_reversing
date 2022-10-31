#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

enum class ThrowableTotalIndicatorState : s16
{
    eCreated = 0,
    eFading = 1,
    eVanishing = 2
};

class ThrowableTotalIndicator final : public BaseGameObject
{
public:
    ThrowableTotalIndicator(FP xpos, FP ypos, Layer layer, FP scale, s32 count, bool bFade);
    ~ThrowableTotalIndicator();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    FP mStartXPos = {};
    FP mStartYPos = {};
    FP mXPos = {};
    FP mYPos = {};
    FP mSpeedX = {};
    FP mSpeedY = {};
    FP mSpriteScale = {};
    Layer mOtLayer = Layer::eLayer_0;
    RGB16 mRGB;
    s16 mNumToShow = 0;

    Line_F2 mLines[2][8] = {};
    Prim_SetTPage mTPage[2] = {};
    ThrowableTotalIndicatorState mState = ThrowableTotalIndicatorState::eCreated;
    s16 mFade = 0;
};

extern u16 bThrowableIndicatorExists_504C70;

} // namespace AO

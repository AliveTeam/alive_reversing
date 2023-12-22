#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/Primitives.hpp"

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
    virtual void VRender(OrderingTable& ot) override;

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
    s32 mNumToShow = 0;

    Line_G2 mLines[8] = {};
    ThrowableTotalIndicatorState mState = ThrowableTotalIndicatorState::eCreated;
    s16 mFade = 0;
};

extern u16 gThrowableIndicatorExists;

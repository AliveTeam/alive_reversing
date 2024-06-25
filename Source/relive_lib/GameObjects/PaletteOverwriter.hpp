#pragma once

#include "BaseGameObject.hpp"
#include "../data_conversion/AnimationConverter.hpp"

class PalleteOverwriter final : public BaseGameObject
{
public:
    PalleteOverwriter(AnimationPal& pal, s16 colour);
    ~PalleteOverwriter();

    virtual void VScreenChanged() override;
    virtual void VRender(OrderingTable& /*ot*/) override;
    virtual void VUpdate() override;

private:
    AnimationPal mPal;
    s16 mPalBuffer[8] = {};
    s16 mPalXIndex = 0;
    s16 mPalW = 0;
    bool mFirstUpdate = false;

public:
    bool mDone = false;
};
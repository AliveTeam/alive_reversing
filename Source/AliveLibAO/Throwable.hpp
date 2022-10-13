#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

extern const AOTypes gThrowableFromOverlayId[];

class BaseThrowable;

BaseThrowable* Make_Throwable(FP xpos, FP ypos, s16 count);

class BaseThrowable : public BaseAliveGameObject
{
public:
    s16 mThrowableCount = 0;
    s16 mIsDead = 0;

    virtual void VThrow(FP velX, FP velY) = 0;
    virtual s16 VCanThrow() = 0;
    virtual s16 VIsFalling() = 0;
    virtual void VTimeToExplodeRandom() = 0;

    virtual s16 VGetCount();
    virtual void VToDead();
    virtual void VOnPickUpOrSlapped() override;

    void BaseAddToPlatform();
};

} // namespace AO

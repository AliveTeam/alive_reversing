#pragma once

#include "BaseAliveGameObject.hpp"

namespace AO {

extern const AOTypes gThrowableFromOverlayId[];

class BaseThrowable;

BaseThrowable* Make_Throwable(FP xpos, FP ypos, s16 count);

// NOTE: This base type must exist but seems to have been decimated by the compiler, so this contains pure virtuals for
// non common virtuals, and virtuals for common virtuals.
class BaseThrowable : public BaseAliveGameObject
{
public:
    BaseThrowable();

    // New virtuals for throwables
    virtual void VThrow(FP velX, FP velY) = 0;
    virtual bool VCanThrow() = 0;
    virtual bool VIsFalling() = 0;
    virtual void VTimeToExplodeRandom() = 0;

    virtual s16 VGetCount();
    virtual void VToDead();
    virtual void VOnAbeInteraction() override;

    void BaseAddToPlatform();
protected:
    s16 mBaseThrowableCount = 0;
    s16 mBaseThrowableDead = 0;
};

} // namespace AO

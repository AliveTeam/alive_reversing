#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

extern const AOTypes word_4CF158[];

class BaseThrowable;

BaseThrowable* Make_Throwable(FP xpos, FP ypos, s16 count);

class BaseThrowable : public BaseAliveGameObject
{
public:
    s16 field_10C_count = 0;
    s16 field_10E_bDead = 0;

    virtual void VThrow(FP velX, FP velY) = 0;
    virtual s16 VCanThrow() = 0;
    virtual s16 VIsFalling() = 0;
    virtual void VTimeToExplodeRandom() = 0;

    virtual s16 VGetCount();
    virtual void VToDead();
    virtual void VOnPickUpOrSlapped() override;

    void BaseAddToPlatform();
};
ALIVE_ASSERT_SIZEOF(BaseThrowable, 0x110);

} // namespace AO

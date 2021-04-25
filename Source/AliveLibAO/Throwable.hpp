#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

extern const Types word_4CF158[];

class BaseThrowable;

EXPORT BaseThrowable* CCSTD Make_Throwable_454560(FP xpos, FP ypos, s16 count);

class BaseThrowable : public BaseAliveGameObject
{
public:
    s16 field_10C_count;
    s16 field_10E_bDead;

    virtual void VThrow(FP velX, FP velY) = 0;

    virtual s16 VCanThrow() = 0;

    virtual s16 VIsFalling() = 0;

    virtual void VTimeToExplodeRandom() = 0;

    virtual s16 VGetCount();

    EXPORT s16 VGetCount_41F980();

    virtual void VToDead();

    EXPORT void VToDead_41F990();

    virtual void VOnPickUpOrSlapped() override;
};
ALIVE_ASSERT_SIZEOF(BaseThrowable, 0x110);

}


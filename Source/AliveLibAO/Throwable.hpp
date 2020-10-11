#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

extern const Types word_4CF158[];

class BaseThrowable;

EXPORT BaseThrowable* CCSTD Make_Throwable_454560(FP xpos, FP ypos, __int16 count);

class BaseThrowable : public BaseAliveGameObject
{
public:
    __int16 field_10C_count;
    __int16 field_10E_bDead;

    virtual void VThrow(FP velX, FP velY) = 0;

    virtual __int16 VCanThrow() = 0;

    virtual __int16 VIsFalling() = 0;

    virtual void VTimeToExplodeRandom() = 0;

    virtual __int16 VGetCount();

    EXPORT __int16 VGetCount_41F980();

    virtual void VToDead();

    EXPORT void VToDead_41F990();

    virtual void VOnPickUpOrSlapped() override;
};
ALIVE_ASSERT_SIZEOF(BaseThrowable, 0x110);

END_NS_AO


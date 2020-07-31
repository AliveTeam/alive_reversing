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

    // TODO: Isn't pure, call stubs in here
    virtual void VOnTrapDoorOpen() override
    {

    }

    virtual void VThrow(FP /*velX*/, FP /*velY*/)
    {

    }

    virtual __int16 VCanThrow()
    {
        return 0;
    }

    virtual __int16 VIsFalling()
    {
        return 0;
    }

    virtual void VTimeToExplodeRandom()
    {

    }

    virtual __int16 VGetCount()
    {
        return 0;
    }

    virtual void VToDead()
    {

    }

};
ALIVE_ASSERT_SIZEOF(BaseThrowable, 0x110);

END_NS_AO


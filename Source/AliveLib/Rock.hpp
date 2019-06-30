#pragma once

#include "Throwable.hpp"

class Rock : public BaseThrowable
{
public:
    EXPORT Rock* ctor_49E150(FP xpos, FP ypos, __int16 count);

    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;


    virtual void VThrow_49E460(FP /*velX*/, FP /*velY*/) override
    {
        // TODO
    }

    virtual BOOL VCanThrow_49E350() override
    {
        // TODO
        return 0;
    }

    virtual BOOL VIsFalling_49E330() override
    {
        // TOOD
        return 0;
    }

    virtual void VTimeToExplodeRandom_411490() override
    {
        // TODO
    }

private:

    EXPORT void vScreenChanged_49F030();
    EXPORT int vGetSaveState_49F9A0(BYTE* /*pSaveBuffer*/);

    /*
    BaseAliveGameObject__vnull_4081F0 vOnTrapDoorOpen
    Rock__vThrow_49E460
    Rock__vCanThrow_49E350
    Rock__vIsFalling_49E330
    Rock__vTimeToExplodeRandom_411490
    GrenadeBase__vGetCount_448080
    Rock__vToDead_4114B0
    */

private:
    __int16 field_11C_state;
    __int16 field_11E;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128_timer;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x12C);

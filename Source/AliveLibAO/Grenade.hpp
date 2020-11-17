#pragma once

#include "FunctionFwd.hpp"
#include "Throwable.hpp"

namespace AO {

enum class GrenadeStates : short
{
    eState_0_FallingToBeCollected = 0,
    eState_1_WaitToBeCollected = 1,
    eState_2 = 2,
    eState_3_CountingDown = 3,
    eState_4_Falling = 4,
    eState_5_HitGround = 5,
    eState_6_WaitForExplodeEnd = 6,
    eState_7_Exploded = 7
};

class Grenade : public BaseThrowable
{
public:
    EXPORT void AddToPlatform_41F7C0();

    virtual __int16 VCanThrow() override;

    EXPORT __int16 VCanThrow_453EC0();

    virtual __int16 VIsFalling() override;

    EXPORT __int16 VIsFalling_453ED0();

    EXPORT signed __int16 BlowUp_41EDD0();

    EXPORT signed __int16 OnCollision_BounceOff_41F650(BaseGameObject* pHit);

    EXPORT signed __int16 InTheAir_41EF10();

    virtual void VTimeToExplodeRandom() override;

    EXPORT void VTimeToExplodeRandom_41F9B0();

    virtual void VThrow(FP velX, FP velY) override;

    EXPORT void VThrow_41ED90(FP velX, FP velY);

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_41F920();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41F240();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41F720();

    EXPORT Grenade* ctor_41EBD0(FP xpos, FP ypos, __int16 numGrenades);

    EXPORT BaseGameObject* dtor_41ECD0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Grenade* Vdtor_41F9F0(signed int flags);

    GrenadeStates field_110_state;
    __int16 field_112_explode_timer;
    PathLine* field_114_pCollisionLine;
    __int16 field_118;
    __int16 field_11A;
    BaseGameObject* field_11C;
    FP field_120_xpos;
    FP field_124_ypos;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x128);

ALIVE_VAR_EXTERN(short, gInfiniteGrenades_5076EC);

}


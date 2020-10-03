#pragma once

#include "FunctionFwd.hpp"
#include "Throwable.hpp"

START_NS_AO


class Grenade : public BaseThrowable
{
public:
    EXPORT void AddToPlatform_41F7C0();

    EXPORT __int16 VCanThrow_453EC0();

    EXPORT __int16 VIsFalling_453ED0();

    EXPORT signed __int16 BlowUp_41EDD0();

    EXPORT signed __int16 OnCollision_BounceOff_41F650(BYTE* a2);

    EXPORT signed __int16 InTheAir_41EF10();

    EXPORT void VTimeToExplodeRandom_41F9B0();

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

    __int16 field_110_state;
    __int16 field_112_explode_timer;
    int field_114;
    __int16 field_118;
    __int16 field_11A;
    BaseGameObject* field_11C;
    FP field_120;
    FP field_124;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x128);

ALIVE_VAR_EXTERN(short, gInfiniteGrenades_5076EC);

END_NS_AO


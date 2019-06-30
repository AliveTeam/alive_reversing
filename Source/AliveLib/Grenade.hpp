#pragma once

#include "Throwable.hpp"

class Grenade : public BaseThrowable
{
public:
    EXPORT Grenade* ctor_447F70(FP xpos, FP ypos, __int16 numGrenades, __int16 a5, __int16 a6, BaseGameObject* pOwner);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VThrow_49E460(FP velX, FP velY) override;

    virtual BOOL VCanThrow_49E350() override;

    virtual BOOL VIsFalling_49E330() override;

    virtual void VTimeToExplodeRandom_411490() override;


    // Next 2 virtuals are base

private:

    EXPORT void vScreenChanged_449140();


    EXPORT void Init_448110(FP xpos, FP ypos);

    EXPORT void vOnTrapDoorOpen_449390();


    EXPORT void vThrow_4482E0(FP velX, FP velY);

    EXPORT BOOL vCanThrow_49A5F0();

    EXPORT BOOL vIsFalling_49A610();

    EXPORT void vTimeToExplodeRandom_4480A0();

    EXPORT __int16 vGetCount_448080();

    EXPORT void BlowUp_4483C0(__int16 bSmallExplosion);

    EXPORT void dtor_448220();

    EXPORT Grenade* vdtor_4480E0(signed int flags);

    EXPORT signed __int16 TimeToBlowUp_448350();

    EXPORT void vUpdate_4489C0();

    EXPORT __int16 InTheAir_4484F0(__int16 blowUpOnFloorTouch);

    EXPORT void AddToPlatform_449210();

    EXPORT __int16 OnCollision_BounceOff_448F90(BaseGameObject* pHit);

    EXPORT __int16 OnCollision_InstantExplode_4490D0(BaseGameObject* pHit);


    /*
    Grenade__vOnTrapDoorOpen_449390
    Grenade__vThrow_4482E0
    Grenade__vCanThrow_49A5F0 // Always 0 ??
    Grenade__vIsFalling_49A610 // Always 0 ??
    Grenade__vTimeToExplodeRandom_4480A0
    Rock__vGetCount_448080
    BaseThrowable__vToDead_4114B0
    */

private:
    int field_11C_explosion_id;
    enum class States : __int16
    {
        eState_0_FallingToBeCollected = 0,
        eState_1_WaitToBeCollected = 1,
        eState_2 = 2,
        eState_3_CountingDown = 3,
        eState_4_Falling = 4,
        eState_5_HitGround = 5,
        eState_6_WaitForExplodeEnd = 6,
        eState_7_Exploded = 7,
        eState_8 = 8,
        eState_9_FallingBlowUpOnGround = 9,
    };
    States field_120_state;
    __int16 field_122_explode_timer;
    __int16 field_124;
    __int16 field_126;
    FP field_128_xpos;
    FP field_12C_ypos;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134_bExplodeNow;
    __int16 field_136;
    BaseGameObject* field_138_pOwner;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x13C);
#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

class BaseThrowable;

EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP /*xpos*/, FP /*ypos*/, FP /*scale*/);

// NOTE: This base type must exist but seems to have been decimated by the compiler, so this contains pure virtuals for
// non common virtuals, and virtuals for common virtuals.
class BaseThrowable : public BaseAliveGameObject
{
public:
    virtual void VOnPickUpOrSlapped() override;

    // New virtuals for throwables
    virtual void VThrow_49E460(FP velX, FP velY) = 0;
    virtual BOOL VCanThrow_49E350() = 0;
    virtual BOOL VIsFalling_49E330() = 0;
    virtual void VTimeToExplodeRandom_411490() = 0;
    virtual __int16 VGetCount_448080() = 0;

    virtual void VToDead_4114B0();

    EXPORT void vToDead_4114B0();
    EXPORT void vOnPickUpOrSlapped_4114D0();
};

class Rock : public BaseThrowable
{
public:
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

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
    // TODO: Don't know yet which of these are part of base (if any)
    __int16 field_116;
    __int16 field_118_count;
    __int16 field_11A_bDead;
    __int16 field_11C_state;
    __int16 field_11E;
    int field_120_xpos;
    int field_124_ypos;
    int field_128_timer;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x12C);

class Grenade : public BaseThrowable
{
public:
    EXPORT Grenade* ctor_447F70(FP xpos, FP ypos, __int16 numGrenades, __int16 a5, __int16 a6, int a7);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override
    {
        vUpdate_4489C0();
    }

    virtual void VOnTrapDoorOpen() override;

    virtual void VThrow_49E460(FP velX, FP velY) override;

    virtual BOOL VCanThrow_49E350() override;

    virtual BOOL VIsFalling_49E330() override;

    virtual void VTimeToExplodeRandom_411490() override;

    virtual __int16 VGetCount_448080() override;

    // Next 2 virtuals are base

private:
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

    EXPORT __int16 InTheAir_4484F0(__int16 a1);

    EXPORT void AddToPlatform_449210();

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
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    int field_11C_explosion_id;
    __int16 field_120_state;
    __int16 field_122_explode_timer;
    __int16 field_124;
    __int16 field_126;
    FP field_128_xpos;
    FP field_12C_ypos;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134;
    __int16 field_136;
    int field_138;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x13C);

class Bone : public BaseThrowable
{
public:
    EXPORT Bone* ctor_4112C0(FP xpos, FP ypos, __int16 countId);


private:
    EXPORT Bone* vdtor_411580(signed int flags);

    EXPORT void dtor_4115B0();

    /*
    Bone__vOnTrapDoorOpen_412490
    Bone__vThrow_411670
    Bone__vCanThrow_411530
    Bone__vIsFalling_411510
    Rock__vTimeToExplodeRandom_411490
    Bone__vGetCount_412500
    BaseThrowable__vToDead_4114B0
    Bone__vsub_411560 // State is 4, eaten?
    */
private:
    __int16 field_116;
    __int16 field_118_count_id;
    __int16 field_11A;
    __int16 field_11C_state;
    __int16 field_11E;
    FP field_120;
    FP field_124;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
};
ALIVE_ASSERT_SIZEOF(Bone, 0x134);

struct Path_BoneBag : public Path_TLV
{
    __int16 field_10_side;
    unsigned __int16 field_12_x_vel;
    unsigned __int16 field_14_y_vel;
    __int16 field_16_scale;
    __int16 field_18_num_bones;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoneBag, 0x1C);

class BoneBag : public BaseAliveGameObject
{
public:
    EXPORT BoneBag* ctor_4125C0(Path_BoneBag* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4127C0(flags);
    }

    virtual void VUpdate() override
    {
        // TODO
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_412BF0();
    }

private:
    EXPORT void vScreenChanged_412BF0();
    EXPORT BoneBag* vdtor_4127C0(signed int flags);
    EXPORT void dtor_4127F0();

private:
    __int16 field_116;
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_count;
    __int16 field_120;
    __int16 field_122;
    int field_124;
    int field_128;
};
ALIVE_ASSERT_SIZEOF(BoneBag, 0x12C);

class Meat : public BaseThrowable
{
public:
    EXPORT Meat* ctor_4694A0(FP xpos, FP ypos, __int16 a4);
    /*
    Meat__vOnTrapDoorOpen_46A2E0
    Meat__vThrow_469790
    Meat__vCanThrow_469680
    Meat__vIsFalling_469660
    Rock__vTimeToExplodeRandom_411490
    Meat__vGetCount_46A350
    BaseThrowable__vToDead_4114B0
    Meat__vsub_4696A0 // State not 0, eaten?
    */
private:
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    __int16 field_11C;
    __int16 field_11E;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128;
    int field_12C;
    PathLine* field_130_pLine;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x134);

ALIVE_VAR_EXTERN(WORD, gInfiniteGrenades_5C1BDE);

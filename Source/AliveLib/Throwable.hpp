#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include <type_traits>

class BaseThrowable;

EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP xpos, FP ypos, short count);

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

    virtual __int16 VGetCount_448080();

    virtual void VToDead_4114B0();

    using FnTypeMatcher = std::add_pointer<bool(Types type)>::type;
    void BaseAddToPlatform(FnTypeMatcher cb);
private:
    EXPORT void vToDead_4114B0();
    EXPORT void vOnPickUpOrSlapped_4114D0();

    __int16 vGetCount_448080();
protected:
    __int16 field_118_count;
    __int16 field_11A_bDead;
};

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

class Bone : public BaseThrowable
{
public:
    EXPORT Bone* ctor_4112C0(FP xpos, FP ypos, __int16 countId);

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
    EXPORT Bone* vdtor_411580(signed int flags);

    EXPORT void dtor_4115B0();

    EXPORT void AddToPlatform_412310();


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
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_count;
    __int16 field_120;
    __int16 field_122;
    int field_124;
    int field_128;
};
ALIVE_ASSERT_SIZEOF(BoneBag, 0x12C);

struct Path_MeatSack : public Path_TLV
{
    __int16 field_10_side;
    unsigned __int16 field_12_xVel;
    unsigned __int16 field_14_yVel;
    __int16 field_16_scale;
    __int16 field_18_num_items;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x1C);

class MeatSack : public BaseAliveGameObject
{
public:
    EXPORT MeatSack* ctor_46A410(Path_MeatSack* pTlv, int tlvInfo);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
private:
    EXPORT MeatSack* vdtor_46A5E0(signed int flags);
    EXPORT void dtor_46A610();
    EXPORT void vScreenChanged_46A9C0();
    EXPORT void vUpdate_46A6A0();

private:
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_num_items;
    __int16 field_120;
    __int16 field_122;
    FP field_124_velX;
    FP field_128_velY;
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x12C);

class Meat : public BaseThrowable
{
public:
    EXPORT Meat* ctor_4694A0(FP xpos, FP ypos, __int16 a4);

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4696C0(flags);
    }

    virtual void VScreenChanged() override;

    virtual void VOnTrapDoorOpen() override
    {
        vOnTrapDoorOpen_46A2E0();
    }

    virtual void VThrow_49E460(FP velX, FP velY) override
    {
        vThrow_469790(velX, velY);
    }

    virtual BOOL VCanThrow_49E350() override
    {
        return vCanThrow_469680();
    }

    virtual BOOL VIsFalling_49E330() override
    {
        return vIsFalling_469660();
    }

    virtual void VTimeToExplodeRandom_411490() override
    {
        // TODO
    }


    virtual __int16 VGetCount_448080() override
    {
        return vGetCount_46A350();
    }

private:
    EXPORT void vScreenChanged_46A130();

    EXPORT void AddToPlatform_46A170();

    EXPORT void vOnTrapDoorOpen_46A2E0();

    EXPORT BOOL vIsFalling_469660();

    EXPORT BOOL vCanThrow_469680();

    EXPORT void dtor_4696F0();

    EXPORT Meat* vdtor_4696C0(signed int flags);

    EXPORT void vThrow_469790(FP velX, FP velY);

    EXPORT __int16 vGetCount_46A350();

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
    __int16 field_11C_state;
    __int16 field_11E;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128;
    int field_12C;
    PathLine* field_130_pLine;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x134);

ALIVE_VAR_EXTERN(WORD, gInfiniteGrenades_5C1BDE);
extern const Types word_55FAFC[252];

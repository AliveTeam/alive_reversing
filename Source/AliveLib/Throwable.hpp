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
    virtual int Vsub_49E460(FP velX, FP velY) = 0;
    virtual BOOL Vsub_49E350() = 0;
    virtual BOOL Vsub_49E330() = 0;
    virtual void Vnull_411490() = 0;
    virtual __int16 Vsub_448080() = 0;

    virtual void Vsub_4114B0();

    EXPORT void vsub_4114B0();
    EXPORT void vOnPickUpOrSlapped_4114D0();
};

class Rock : public BaseThrowable
{
public:
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT void vScreenChanged_49F030();
    EXPORT int vGetSaveState_49F9A0(BYTE* /*pSaveBuffer*/);

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

// TODO
class Grenade : public BaseThrowable
{
public:
};

// TODO
class Bone : public BaseThrowable
{
public:
};

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

// TODO
class Meat : public BaseThrowable
{
public:
};

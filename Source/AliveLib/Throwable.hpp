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
    virtual __int16 Vnull_411490() = 0;
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

class Grenade : public BaseThrowable
{
public:
    EXPORT Grenade* ctor_447F70(FP xpos, FP ypos, __int16 numGrenades, __int16 a5, __int16 a6, int a7);

    virtual int Vsub_49E460(FP velX, FP velY) override;

    virtual BOOL Vsub_49E350() override;

    virtual BOOL Vsub_49E330() override;

    virtual __int16 Vnull_411490() override;

    virtual __int16 Vsub_448080() override;

    // Next 2 virtuals are base

private:
    EXPORT void Init_448110(FP xpos, FP ypos);

    EXPORT int Vsub_449390(FP velX, FP velY);

    EXPORT BOOL Vsub_4482E0();

    EXPORT BOOL Vsub_49A5F0();

    EXPORT __int16 Vnull_49A610();

    EXPORT __int16 Vsub_4480A0();

private:
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    int field_11C;
    __int16 field_120;
    __int16 field_122;
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

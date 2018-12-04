#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

class BaseThrowable;

EXPORT BaseThrowable* __stdcall Make_Throwable_49AF30(FP /*xpos*/, FP /*ypos*/, FP /*scale*/);

// NOTE: This base type must exist but seems to have been decimated by the compiler, so this contains pure virtuals for
// non common virtuals, and virtuals for common virtuals.
class BaseThrowable : public BaseAliveGameObject
{
public:
    virtual void vnull_408180() override;

    // New virtuals for throwables
    virtual int Vsub_49E460(FP velX, FP velY) = 0;
    virtual BOOL Vsub_49E350() = 0;
    virtual BOOL Vsub_49E330() = 0;
    virtual void Vnull_411490() = 0;
    virtual __int16 Vsub_448080() = 0;

    virtual void Vsub_4114B0();

    EXPORT void vsub_4114B0();
    EXPORT void vsub_4114D0();
};

class Rock : public BaseThrowable
{
public:
    virtual void VScreenChanged() override;
    virtual int GetSaveState_4DC110(BYTE* pSaveBuffer) override;

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

// TODO
class Meat : public BaseThrowable
{
public:
};

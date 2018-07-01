#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "Function.hpp"

void DDCheat_ForceLink();

struct DDCheatProperty
{
    const char * Name;
    int Unknown;
    int * ValuePtr;
};
ALIVE_ASSERT_SIZEOF(DDCheatProperty, 12);

class DDCheat : public BaseGameObject
{
public:
    virtual void VDestructor(signed int flags);
    virtual void VUpdate() override;

    EXPORT DDCheat* ctor_4153C0();
    EXPORT void dtor_415530();
    EXPORT void dtor_415500(signed int flags);

    EXPORT void AddPropertyEntry_004162C0(const char *text, int unknown, int *valuePtr);
    static EXPORT void CC sub_415390();

private:
    EXPORT void Update_415780();
private:
    int field_20;
    __int16 field_24_fn_idx;
    __int16 field_26;
    int field_28;
    int field_2C;
    __int16 field_30;
    __int16 field_32;
    int field_34;
    int field_38_input_pressed;
    __int16 field_3C_flags;
    __int16 field_3E;
};
ALIVE_ASSERT_SIZEOF(DDCheat, 0x40);

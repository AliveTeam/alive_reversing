#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"

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

    EXPORT static void AddPropertyEntry_004162C0(const char *text, int unknown, int *valuePtr);
    static EXPORT void CC sub_415390();
    static EXPORT void DebugStr_4F5560(const char *pFormatStr, ...);

    EXPORT void Menu_Teleport_415E20();
    EXPORT void Menu_Movies_416000();

private:
    EXPORT void Update_415780();
private:
    int field_20;
    __int16 field_24_fn_idx;
    __int16 field_26_next_fn_idx;
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

struct Abe_1BC_20_sub_object
{
    int field_0;
    char field_4;
    char field_5;
    __int16 field_6;
    char field_8;
    char field_9;
    char field_A;
    char field_B;
    __int16 field_C;
    __int16 field_E;
    int field_10_mood;
    int field_14;
    __int16 field_18;
    __int16 field_1A;
};
ALIVE_ASSERT_SIZEOF(Abe_1BC_20_sub_object, 0x1C);

class Abe : public BaseAliveGameObject
{
    // TODO: Vtables
public:
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    Abe_1BC_20_sub_object field_128;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    __int16 field_16C;
    __int16 field_16E;
    int field_170;
    __int16 field_174;
    __int16 field_176;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    __int16 field_198;
    __int16 field_19A;
    int field_19C;
    __int16 field_1A0;
    char field_1A2_rock_or_bone_count;
    char field_1A3_throw_direction;
    int field_1A4;
    int field_1A8;
    __int16 field_1AC;
    __int16 field_1AE;
    int field_1B0_save_num;
    int field_1B4;
    int field_1B8;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x1BC);
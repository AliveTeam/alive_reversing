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
    enum Flags_3C
    {
        e3C_Bit1 = 0x1,
        e3C_Bit2 = 0x2,
        eOnTeleport_Bit3 = 0x4,
        e3C_Bit4 = 0x8,
        e3C_Bit5 = 0x10,
        e3C_Bit6 = 0x20,
        e3C_Bit7 = 0x40,
        e3C_Bit8 = 0x80,
        e3C_Bit9 = 0x100,
        e3C_Bit10 = 0x200,
        e3C_Bit11 = 0x400,
        e3C_Bit12 = 0x800,
        e3C_eBit13 = 0x1000,
        e3C_eBit14 = 0x2000,
        e3C_eBit15 = 0x4000,
        e3C_eBit16 = 0x8000
    };

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override { }

    DDCheat();
    EXPORT DDCheat* ctor_4153C0();
    EXPORT void dtor_415530();
    EXPORT BaseGameObject* vdtor_415500(signed int flags);

    EXPORT static void AddPropertyEntry_004162C0(const char *text, int unknown, int *valuePtr);
    static EXPORT void CC ClearProperties_415390();
    static EXPORT void DebugStr_4F5560(const char *pFormatStr, ...);

    EXPORT void Menu_Teleport_415E20();
    EXPORT void Menu_Movies_416000();

public:
    EXPORT void Update_415780();
public:
    int field_20;
    __int16 field_24_fn_idx;
    __int16 field_26_next_fn_idx;
    int field_28;
    int field_2C;
    __int16 field_30;
    __int16 field_32;
    int field_34;
    int field_38_input_pressed;
    BitField16<Flags_3C> field_3C_flags;
    __int16 field_3E;
};

ALIVE_ASSERT_SIZEOF(DDCheat, 0x40);

ALIVE_VAR_EXTERN(bool, sDDCheat_FlyingEnabled_5C2C08);

ALIVE_VAR_EXTERN(__int16, sRescuedMudokons_5C1BC2);
ALIVE_VAR_EXTERN(__int16, sKilledMudokons_5C1BC0);

ALIVE_VAR_EXTERN(int, sTweakX_5C1BD0);
ALIVE_VAR_EXTERN(int, sTweakY_5C1BD4);

ALIVE_VAR_EXTERN(__int16, sVisitedBonewerks_5C1C02);
ALIVE_VAR_EXTERN(__int16, sVisitedBarracks_5C1C04);
ALIVE_VAR_EXTERN(__int16, sVisitedFeecoEnder_5C1C06);
ALIVE_VAR_EXTERN(bool, sDDCheat_ShowAI_Info_5C1BD8);

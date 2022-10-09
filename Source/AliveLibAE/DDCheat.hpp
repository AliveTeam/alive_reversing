#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "FixedPoint.hpp"

enum class DDCheatValueType : s32
{
    eShort_1 = 1,
    eInt_2 = 2,
    eShort_4 = 4,
    eInt_6 = 6,
};

union DDCheatValue
{
    s32* pInt;
    s16* pShort;
    FP* pFixedPoint;

    DDCheatValue()
    {
        pInt = nullptr;
    }

    DDCheatValue(s32* v)
    {
        pInt = v;
    }

    DDCheatValue(s16* v)
    {
        pShort = v;
    }

    DDCheatValue(FP* v)
    {
        pFixedPoint = v;
    }
};

struct DDCheatProperty final
{
    const char_type* Name;
    DDCheatValueType ValueType;
    DDCheatValue ValuePtr;
};
ALIVE_ASSERT_SIZEOF(DDCheatProperty, 12);

class DDCheat final : public BaseGameObject
{
public:
    DDCheat();

    void Menu_Teleport();
    void Menu_Movies();
    static void ClearProperties();
    static void DebugStr(const char_type* pFormatStr, ...);

private:
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

    virtual void VUpdate() override;
    virtual void VScreenChanged() override
    { }

    static void AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr);

    s32 field_20 = 0;
    s16 field_24_fn_idx = 0;
    s16 field_26_next_fn_idx = 0;
    s32 field_38_input_pressed = 0;
    BitField16<Flags_3C> field_3C_flags = {};
};

ALIVE_ASSERT_SIZEOF(DDCheat, 0x40);

ALIVE_VAR_EXTERN(bool, sDDCheat_FlyingEnabled_5C2C08);

ALIVE_VAR_EXTERN(s16, sRescuedMudokons_5C1BC2);
ALIVE_VAR_EXTERN(s16, sKilledMudokons_5C1BC0);

ALIVE_VAR_EXTERN(s32, sTweakX_5C1BD0);
ALIVE_VAR_EXTERN(s32, sTweakY_5C1BD4);

ALIVE_VAR_EXTERN(s16, sVisitedBonewerks_5C1C02);
ALIVE_VAR_EXTERN(s16, sVisitedBarracks_5C1C04);
ALIVE_VAR_EXTERN(s16, sVisitedFeecoEnder_5C1C06);
ALIVE_VAR_EXTERN(bool, sDDCheat_ShowAI_Info);

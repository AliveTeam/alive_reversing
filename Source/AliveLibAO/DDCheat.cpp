#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

START_NS_AO

struct DDCheatProperties
{
    DDCheatProperty props[10];
};
ALIVE_VAR(1, 0x4FF7D8, DDCheatProperties, DDCheatProperties_4FF7D8, {});

ALIVE_VAR(1, 0x5076FC, int, gDoorsOpen_5076FC, 0);
ALIVE_VAR(1, 0x5076D8, int, gTweak_X_5076D8, 0);
ALIVE_VAR(1, 0x5076DC, int, gTweak_Y_5076DC, 0);

ALIVE_VAR(1, 0x5076C0, short, sRescuedMudokons_5076C0, 0);
ALIVE_VAR(1, 0x5076BC, short, sKilledMudokons_5076BC, 0);



DDCheat* DDCheat::ctor_4095D0()
{
    ctor_487E10(1);
    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);
    field_6_flags.Set(Options::eUpdateDuringCamSwap_Bit10);

    SetVTable(this, 0x4BA340);
    field_4_typeId = Types::eDDCheat_12;
    field_10 = 0;
    field_14 = 0;
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;

    ClearProperties_4095B0();

    AddPropertyEntry("Doors Open ", DDCheatValueType::eShort_4, &gDoorsOpen_5076FC);
    AddPropertyEntry("Tweak X ", DDCheatValueType::eInt_6, &gTweak_X_5076D8);
    AddPropertyEntry("Tweak Y ", DDCheatValueType::eInt_6, &gTweak_Y_5076DC);
    AddPropertyEntry("RescuedMudokons ", DDCheatValueType::eShort_4, &sRescuedMudokons_5076C0);
    return this;
}

void DDCheat::ClearProperties_4095B0()
{
    DDCheatProperties_4FF7D8 = {};
}

BaseGameObject* DDCheat::dtor_409710()
{
    SetVTable(this, 0x4BA340);
    return dtor_487DF0();
}

BaseGameObject* DDCheat::VDestructor(signed int flags)
{
    return Vdtor_40A380(flags);
}

DDCheat* DDCheat::Vdtor_40A380(signed int flags)
{
    dtor_409710();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void DDCheat::AddPropertyEntry(const char* text, DDCheatValueType valueType, DDCheatValue valuePtr)
{
    for (auto& prop : DDCheatProperties_4FF7D8.props)
    {
        if (prop.Name == nullptr)
        {
            prop.Name = text;
            prop.ValueType = valueType;
            prop.ValuePtr = valuePtr;
            return;
        }
    }
}

END_NS_AO

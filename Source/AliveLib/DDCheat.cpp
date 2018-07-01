#include "stdafx.h"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

void DDCheat_ForceLink() { }

ALIVE_VAR(1, 0x5c1be6, int, sDoorsOpen_5C1BE6, 0);
ALIVE_VAR(1, 0x5c1bd0, int, sTweakX_5C1BD0, 0);
ALIVE_VAR(1, 0x5c1bd4, int, sTweakY_5C1BD4, 0);
ALIVE_VAR(1, 0x5c1bc2, __int16, sRescuedMudokons_5C1BC2, 0);
ALIVE_VAR(1, 0x5c1c02, __int16, sVisitedBonewerks_5C1C02, 0);
ALIVE_VAR(1, 0x5c1c04, __int16, sVisitedBarracks_5C1C04, 0);
ALIVE_VAR(1, 0x5c1c06, __int16, sVisitedFeecoEnder_5C1C06, 0);

ALIVE_VAR(1, 0x5bc008, __int16, sScreenshotOnNextFrame_5BC008, 0);

EXPORT void DDCheat_SaveScreenshot_415550() { NOT_IMPLEMENTED(); }

#define DDCHEAT_PROPERTY_LIST_SIZE 10
ALIVE_ARY(1, 0x5BBF78, DDCheatProperty, DDCHEAT_PROPERTY_LIST_SIZE, DDCheatProperties_5BBF78, {});

DDCheat* DDCheat::ctor_4153C0()
{
    BaseGameObject::ctor_4DBFA0(1, 0);
    SetVTable(this, 0x544518);
    field_6_flags |= 0x300;

    field_3C_flags &= 0xFFF8u;
    field_4_typeId = 19;
    field_20 = 0;
    field_24_fn_idx = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;

    memset(DDCheatProperties_5BBF78, 0, sizeof(DDCheatProperty) * DDCHEAT_PROPERTY_LIST_SIZE);

    // The code below allows changing of certain variables in memory using in game controls.
    // There's no code using any of this in the retail final build as the compiler occluded it.
    // But, the Exoddus Demo does in fact have the code, so it's possible to reimplement it
    // in the future.
    AddPropertyEntry_004162C0("Doors Open ", 4, (int *)&sDoorsOpen_5C1BE6);
    AddPropertyEntry_004162C0("Tweak X ", 6, &sTweakX_5C1BD0);
    AddPropertyEntry_004162C0("Tweak Y ", 6, &sTweakY_5C1BD4);
    AddPropertyEntry_004162C0("RescuedMudokons ", 4, (int *)&sRescuedMudokons_5C1BC2);
    AddPropertyEntry_004162C0("Visited Bonewerks ", 1, (int *)&sVisitedBonewerks_5C1C02);
    AddPropertyEntry_004162C0("Visited Barracks ", 1, (int *)&sVisitedBarracks_5C1C04);
    AddPropertyEntry_004162C0("Visited Feeco Ender ", 1, (int *)&sVisitedFeecoEnder_5C1C06);

    return this;
}


void DDCheat::dtor_415530()
{
    dtor_4DBEC0();
}


void DDCheat::dtor_415500(signed int flags)
{
    dtor_415530();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void DDCheat::AddPropertyEntry_004162C0(const char * text, int unknown, int * valuePtr)
{
    for (int i = 0; i < DDCHEAT_PROPERTY_LIST_SIZE; i++)
    {
        if (DDCheatProperties_5BBF78[i].Name == nullptr)
        {
            DDCheatProperties_5BBF78[i].Name = text;
            DDCheatProperties_5BBF78[i].Unknown = unknown;
            DDCheatProperties_5BBF78[i].ValuePtr = valuePtr;
            break;
        }
    }
}

void CC DDCheat::sub_415390()
{
    NOT_IMPLEMENTED();
}

void DDCheat::Update_415780()
{
    NOT_IMPLEMENTED();
}

void DDCheat::VDestructor(signed int flags)
{
    dtor_415500(flags);
}

void DDCheat::VUpdate()
{
    Update_415780();
}
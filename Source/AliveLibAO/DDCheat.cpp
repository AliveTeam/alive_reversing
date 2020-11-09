#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "Slig.hpp"
#include "Input.hpp"
#include "Elum.hpp"
#include "Game.hpp"
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

ALIVE_VAR(1, 0x5076E0, short, showDebugCreatureInfo_5076E0, 0);
ALIVE_VAR(1, 0x50771C, short, sDDCheat_FlyingEnabled_50771C, 0);

using TDDCheatFn = decltype(&DDCheat::Teleport_409CE0);

const TDDCheatFn off_4C3150[] = 
{
    &DDCheat::Teleport_409CE0,
    &DDCheat::Misc_409E90
};

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


void DDCheat::VScreenChanged()
{
    // Empty
}

void DDCheat::VUpdate()
{
    VUpdate_4098C0();
}

ALIVE_VAR(1, 0x4FF868, WORD, word_4FF868, 0);

ALIVE_VAR(1, 0x4C315C, DWORD, level_4C315C, 3);
ALIVE_VAR(1, 0x4C3160, DWORD, path_4C3160, 1);
ALIVE_VAR(1, 0x4FF864, DWORD, gVox_4FF864, 0);
ALIVE_VAR(1, 0x4FF860, WORD, doNothing_4FF860, 0);
ALIVE_VAR_EXTERN(char, gDDCheatMode_508BF8);

ALIVE_VAR(1, 0x9F0E40, DWORD, dword_9F0E40, 0);
ALIVE_VAR(1, 0x9F0E44, DWORD, dword_9F0E44, 1);

ALIVE_VAR(1, 0x4FF854, DWORD, currentlyPressedButtons_4FF854, 0);
ALIVE_VAR(1, 0x4C31A8, DWORD, dword_4C31A8, 10);

void DDCheat::ScreenShot_409720()
{
    NOT_IMPLEMENTED();
}

EXPORT int CC sub_49AD50(int /*a1*/)
{
    return 0;
}

void DDCheat::VUpdate_4098C0()
{
    if (gDDCheatMode_508BF8)
    {
        const int otherControler = sCurrentControllerIndex_5076B8 == 0 ? 1 : 0;
        Abe* pObj = sActiveHero_507678;
        int cheat_enabled = 0;

        if (word_4FF868)
        {
            ScreenShot_409720();
        }
        field_10 = 0;

        if (field_20 == 0)
        {
            cheat_enabled = sDDCheat_FlyingEnabled_50771C;
        }
        else
        {
            field_20 = 0;
            if (pObj)
            {
                PSX_Point point = {};
                gMap_507BA8.GetCurrentCamCoords_444890(&point);
                pObj = sActiveHero_507678;
                cheat_enabled = 1;
                pObj->field_A8_xpos = FP_FromInteger(point.field_0_x + 448);
                pObj->field_AC_ypos = FP_FromInteger(point.field_2_y + 180);
                pObj->field_FC_current_motion = 3;
                pObj->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8);
                pObj->field_B2_lvl_number = static_cast<LevelIds>(level_4C315C);
                pObj->field_B0_path_number = static_cast<short>(path_4C3160);
                sDDCheat_FlyingEnabled_50771C = 1;
                field_18 = 0;
            }
        }

        if (gMap_507BA8.field_0_current_level != LevelIds::eMenu_0)
        {
            if (pObj)
            {
                if (sInputObject_5009E8.isHeld(InputCommands::eGameSpeak4))
                {
                    sDDCheat_FlyingEnabled_50771C = cheat_enabled == 0;
                    if (sDDCheat_FlyingEnabled_50771C)
                    {
                        pObj->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8);
                        showDebugCreatureInfo_5076E0 = 0;
                        sControlledCharacter_50767C->field_F4_pLine = 0;
                        sControlledCharacter_50767C->field_E8_LastLineYPos = sControlledCharacter_50767C->field_AC_ypos;
                        switch (sControlledCharacter_50767C->field_4_typeId)
                        {
                            case Types::eElum_26:
                            {
                                sControlledCharacter_50767C->field_FC_current_motion = eElumStates::State_21_Land_414A20;
                                break;
                            }
                            case Types::eAbe_43:
                            {
                                sControlledCharacter_50767C->field_FC_current_motion = eAbeStates::State_3_Fall_42E7F0;
                                break;
                            }
                            case Types::eSlig_88:
                            {
                                sControlledCharacter_50767C->field_FC_current_motion = eSligStates::State_7_Falling_46A1A0;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }
        }

        const auto screenshotCombination = InputCommands::eDoAction | InputCommands::eSneak | InputCommands::eRun;
        if ((sInputObject_5009E8.field_0_pads[otherControler].field_0_pressed & screenshotCombination) == screenshotCombination)
        {
            ScreenShot_409720();
        }
        else
        {
            if (cheat_enabled)
            {
                if (pObj)
                {
                    pObj->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8);
                }
                sControlledCharacter_50767C->field_F4_pLine = 0;
                sControlledCharacter_50767C->field_E8_LastLineYPos = sControlledCharacter_50767C->field_AC_ypos;
            }
            if (!(gnFrameCount_507670 % 10))
            {
                gVox_4FF864 = 0;
                auto counter = 0;
                while (counter < 24)
                {
                    if (sub_49AD50(1 << counter))
                    {
                        gVox_4FF864++;
                    }
                    counter++;
                }
                cheat_enabled = sDDCheat_FlyingEnabled_50771C;
            }
            if (cheat_enabled || showDebugCreatureInfo_5076E0 || doNothing_4FF860)
            {
                DebugOut_495990(
                    "\n%sP%dC%d %6d",
                    gMapData_4CAB58.paths[static_cast<int>(gMap_507BA8.field_0_current_level)].field_14_lvl_name,
                    gMap_507BA8.field_2_current_path,
                    gMap_507BA8.field_4_current_camera,
                    gnFrameCount_507670);
                DebugOut_495990(
                    " %5d %5d  ",
                    (sManagedMemoryUsedSize_9F0E48 + 999) / 1000,
                    (sPeakedManagedMemUsage_9F0E4C + 999) / 1000);
                DebugOut_495990(" Vox %d\n", gVox_4FF864);
                cheat_enabled = sDDCheat_FlyingEnabled_50771C;
                field_10 = 6;
            }
            if (cheat_enabled)
            {
                auto isHeld = sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_6_held;
                if (isHeld & InputCommands::eDoAction)
                {
                    showDebugCreatureInfo_5076E0 = showDebugCreatureInfo_5076E0 == 0;
                }
                if (isHeld & InputCommands::eThrowItem)
                {
                    sPeakedManagedMemUsage_9F0E4C = sManagedMemoryUsedSize_9F0E48;
                    dword_9F0E44 = dword_9F0E40;
                }
                if (isHeld & InputCommands::eHop)
                {
                    doNothing_4FF860 = doNothing_4FF860 == 0;
                }
            }
            field_24_input = sInputObject_5009E8.field_0_pads[otherControler].field_6_held;
            auto isPressed = sInputObject_5009E8.field_0_pads[otherControler].field_0_pressed;
            if (currentlyPressedButtons_4FF854 == isPressed && currentlyPressedButtons_4FF854)
            {
                dword_4C31A8--;
                if (dword_4C31A8 == 0)
                {
                    field_24_input = currentlyPressedButtons_4FF854;
                    dword_4C31A8 = 2;
                }
            }
            else
            {
                currentlyPressedButtons_4FF854 = isPressed;
                dword_4C31A8 = 10;
            }
            if (field_24_input & InputCommands::eGameSpeak2)
            {
                field_18 = field_18 == 0;
            }
            if (field_18)
            {
                if (isPressed & InputCommands::eSneak &&
                    isPressed & InputCommands::eGameSpeak4)
                {
                    field_14 = 0;
                }
                else if (field_24_input & InputCommands::eGameSpeak4)
                {
                    field_14++;
                    if (field_14 >= 2)
                    {
                        field_14 = 0;
                    }
                }
                (this->*off_4C3150[field_14])();
            }
            if (field_10)
            {
                pScreenManager_4FF7C8->InvalidateRect_406CC0(0, 0, 640, 240);
            }
        }
    }
}

void DDCheat::Teleport_409CE0()
{
    NOT_IMPLEMENTED();
}

void DDCheat::Misc_409E90()
{
    NOT_IMPLEMENTED();
}

int DebugOut_495990(const char* /*pStr*/, ...)
{
    NOT_IMPLEMENTED();
    return 0;
}

END_NS_AO

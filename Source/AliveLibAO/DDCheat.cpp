#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "PsxDisplay.hpp"
#include "Psx.hpp"
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

namespace AO {

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
ALIVE_VAR(1, 0x4C3160, WORD, path_4C3160, 1);
ALIVE_VAR(1, 0x4FF864, DWORD, gVox_4FF864, 0);
ALIVE_VAR(1, 0x4FF860, WORD, doNothing_4FF860, 0);
ALIVE_VAR_EXTERN(char, gDDCheatMode_508BF8);

ALIVE_VAR(1, 0x9F0E40, DWORD, dword_9F0E40, 0);
ALIVE_VAR(1, 0x9F0E44, DWORD, dword_9F0E44, 1);

ALIVE_VAR(1, 0x4FF854, DWORD, currentlyPressedButtons_4FF854, 0);
ALIVE_VAR(1, 0x4C31A8, DWORD, dword_4C31A8, 10);


template <class T>
static void writeHeaderElement(const T& element, FILE* f)
{
    fwrite(&element, 1, sizeof(T), f);
}

void DDCheat::ScreenShot_409720()
{
    auto pixelBuffer = reinterpret_cast<WORD*>(alloc_450740(640 * sizeof(WORD) * gPsxDisplay_504C78.field_2_height));
    if (pixelBuffer)
    {
        char fileNameBuffer[16] = {};
        sprintf(fileNameBuffer, "SD%06ld.TGA", gnFrameCount_507670 % 1000000);
        const auto fileHandle = fopen(fileNameBuffer, "wb");
        if (!fileHandle)
        {
            return;
        }
        const PSX_RECT rect = { 0, 0, 640, static_cast<short>(gPsxDisplay_504C78.field_2_height) };
        PSX_StoreImage_496320(&rect, pixelBuffer);
        PSX_DrawSync_496750(0);

        for (int i = 0; i < 640 * gPsxDisplay_504C78.field_2_height; i++)
        {
            const WORD pixel = pixelBuffer[i];
            pixelBuffer[i] = ((pixel >> 10) & 0x1F) + (32 * (32 * (pixel & 0x1F) + ((pixel >> 5) & 0x1F)));
        }

        struct {
            char  idlength = 0;
            char  colourmaptype = 0;
            char  datatypecode = 2;
            short int colourmaporigin = 0;
            short int colourmaplength = 0;
            char  colourmapdepth = 0;
            short int x_origin = 0;
            short int y_origin = 0;
            short width = 640;
            short height = 480;
            char  bitsperpixel = 16;
            char  imagedescriptor = 0;
        } headerTGA;

        writeHeaderElement(headerTGA.idlength, fileHandle);
        writeHeaderElement(headerTGA.colourmaptype, fileHandle);
        writeHeaderElement(headerTGA.datatypecode, fileHandle);
        writeHeaderElement(headerTGA.colourmaporigin, fileHandle);
        writeHeaderElement(headerTGA.colourmaplength, fileHandle);
        writeHeaderElement(headerTGA.colourmapdepth, fileHandle);
        writeHeaderElement(headerTGA.x_origin, fileHandle);
        writeHeaderElement(headerTGA.y_origin, fileHandle);
        writeHeaderElement(headerTGA.width, fileHandle);
        writeHeaderElement(headerTGA.height, fileHandle);
        writeHeaderElement(headerTGA.bitsperpixel, fileHandle);
        writeHeaderElement(headerTGA.bitsperpixel, fileHandle);

        unsigned char *rowOfPixels = reinterpret_cast<unsigned char*>(pixelBuffer + 640 * 239);
        const int rowNum = 240;
        for(int i = 0; i < rowNum; i++)
        {
            fwrite(rowOfPixels, 1, 640 * sizeof(WORD), fileHandle);
            fwrite(rowOfPixels, 1, 640 * sizeof(WORD), fileHandle);
            rowOfPixels -= 640 * sizeof(WORD);
        }
        fclose(fileHandle);
        ao_delete_free_450770(pixelBuffer);
    }
}

EXPORT int CC sub_49AD50(int /*a1*/)
{
    return 0;
}

void DDCheat::VUpdate_4098C0()
{
    if (gDDCheatMode_508BF8)
    {
        const int otherController = sCurrentControllerIndex_5076B8 == 0 ? 1 : 0;
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
                if (sInputObject_5009E8.IsHeld(InputCommands::eGameSpeak4))
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
        if ((sInputObject_5009E8.field_0_pads[otherController].field_0_pressed & screenshotCombination) == screenshotCombination)
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
            field_24_input = sInputObject_5009E8.field_0_pads[otherController].field_6_held;
            auto isPressed = sInputObject_5009E8.field_0_pads[otherController].field_0_pressed;
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

const char *lvl_names_4C3168[16] =
{
    "Start screen",
    "Rupture 1",
    "Lines 1",
    "Forest Outside",
    "Forest Inside",
    "Escape 1",
    "Escape 2",
    "(removed)",
    "Desert Outside",
    "Desert Inside",
    "Credits",
    "(removed)",
    "Game Ender",
    "Rupture 2",
    "Forest Level Ender",
    "Desert Level Ender"
};

ALIVE_VAR(1, 0x4C3164, short, camera_4C3164, 1);

void DDCheat::Teleport_409CE0()
{
    DebugOut_495990("\n[Teleport]\n");
    DebugOut_495990("Level    (L,R):      %s \n", lvl_names_4C3168[level_4C315C]);
    DebugOut_495990("Path    (Up/Down):   %d \n", path_4C3160);
    DebugOut_495990("Camera (Left/Right): %d \n", static_cast<unsigned __int16>(camera_4C3164));
    DebugOut_495990("Teleport = [] Reset = O\n"); //TODO don't display PSX buttons
    int input = field_24_input;
    field_10 = 6;
    if (input &  InputCommands::eSneak)
    {
        if (level_4C315C)
        {
            --level_4C315C;
        }
    }
    else if (input & InputCommands::eGameSpeak4)
    {
        if (level_4C315C < 15u)
        {
            ++level_4C315C;
        }
    }
    else if (input & InputCommands::eUp)
    {
        ++path_4C3160;
    }
    else if (input & InputCommands::eDown)
    {
        if (path_4C3160 > 1u)
        {
            --path_4C3160;
        }
    }
    else if (input & InputCommands::eLeft)
    {
        if (camera_4C3164 > 1)
        {
            --camera_4C3164;
        }
    }
    else if (input & InputCommands::eRight)
    {
        ++camera_4C3164;
    }
    else if (input & InputCommands::eThrowItem)
    {
        path_4C3160 = gMap_507BA8.field_2_current_path;
        level_4C315C = static_cast<DWORD>(gMap_507BA8.field_0_current_level);
        camera_4C3164 = gMap_507BA8.field_4_current_camera;
    }
    else if (input & InputCommands::eDoAction)
    {
        if (path_4C3160 <= 21u)
        {
            const auto pPathRec = Path_Get_Bly_Record_434650(static_cast<LevelIds>(level_4C315C), path_4C3160);
            if (pPathRec &&
                pPathRec->field_0_blyName &&
                pPathRec->field_4_pPathData &&
                pPathRec->field_8_pCollisionData)
            {
                if (camera_4C3164 <= 21)
                {
                    sDDCheat_FlyingEnabled_50771C = 1;
                    gMap_507BA8.SetActiveCam_444660(
                        static_cast<LevelIds>(level_4C315C),
                        path_4C3160,
                        camera_4C3164,
                        CameraSwapEffects::eEffect0_InstantChange,
                        0,
                        0
                    );
                    field_20 = 1;
                }
            }
        }
    }
}

ALIVE_VAR(1, 0x4C3158, DWORD, gScale_4C3158, 100);

void DDCheat::Misc_409E90()
{
    if (field_24_input & InputCommands::eLeft)
    {
        gScale_4C3158 = 100;
        sControlledCharacter_50767C->field_BC_sprite_scale = FP_FromInteger(1);
        sControlledCharacter_50767C->field_C6_scale = 1;
        sControlledCharacter_50767C->field_10_anim.field_C_layer = 32;
    }
    else if (field_24_input & InputCommands::eRight)
    {
        sControlledCharacter_50767C = sControlledCharacter_50767C;
        gScale_4C3158 = 50;
        sControlledCharacter_50767C->field_BC_sprite_scale = FP_FromDouble(0.5);
        sControlledCharacter_50767C->field_C6_scale = 0;
        sControlledCharacter_50767C->field_10_anim.field_C_layer = 13;
    }
    else if (field_24_input & InputCommands::eDown)
    {
        gScale_4C3158 -= 5;
        sControlledCharacter_50767C->field_BC_sprite_scale = FP_FromInteger(gScale_4C3158) * FP_FromDouble(0.01);
    }
    else if (field_24_input & InputCommands::eUp)
    {
        gScale_4C3158 += 5;
        sControlledCharacter_50767C->field_BC_sprite_scale = FP_FromInteger(gScale_4C3158) * FP_FromDouble(0.01);
    }
    else if (field_24_input & InputCommands::eHop)
    {
        gAbeInvulnerableCheat_5076E4 = gAbeInvulnerableCheat_5076E4 == 0;
    }
    else if (field_24_input & InputCommands::eFartOrRoll)
    {
        gAbeInvisibleCheat_5076F8 = gAbeInvisibleCheat_5076F8 == 0;
    }
    DebugOut_495990("\nScale: up=+5 down=-5 left=100 right=50\n");
    DebugOut_495990("Scale: %d\n\n", gScale_4C3158);

    const char *invulnerableDisplayText = "on";
    if (!gAbeInvulnerableCheat_5076E4)
    {
        invulnerableDisplayText = "off";
    }
    DebugOut_495990("triangle=invulnerable (%s)\n", invulnerableDisplayText);

    char *invisibleDisplayText = "on";
    if (!gAbeInvisibleCheat_5076F8)
    {
        invisibleDisplayText = "off";
    }
    DebugOut_495990("cross = invisible (%s)\n", invisibleDisplayText);

    field_10 = 9;
    if (!gElum_507680)
    {
        if (sControlledCharacter_50767C != gElum_507680)
        {
            return;
        }
    }
    else if (sControlledCharacter_50767C != gElum_507680)
    {
        gElum_507680->field_BC_sprite_scale = sControlledCharacter_50767C->field_BC_sprite_scale;
        gElum_507680->field_C6_scale = sControlledCharacter_50767C->field_C6_scale;
        if (sControlledCharacter_50767C != gElum_507680)
        {
            return;
        }
    }
    sActiveHero_507678->field_BC_sprite_scale = sControlledCharacter_50767C->field_BC_sprite_scale;
    sActiveHero_507678->field_C6_scale = sControlledCharacter_50767C->field_C6_scale;
}

struct TextRecords
{
    char field_0_src_txt[1024];
    char field_400_dst_txt[1027];
};
ALIVE_ASSERT_SIZEOF(TextRecords, 0x803);

struct DebugTexts
{
    BYTE field_0_xMargin;
    BYTE field_1_yMargin;
    BYTE field_2_displayWidth;
    BYTE field_3_displayHeight;
    DWORD field_4_max_len;
    BYTE field_8_bgColour;
    TextRecords field_9_text;
};
ALIVE_ASSERT_SIZEOF(DebugTexts, 0x80C);

ALIVE_ARY(1, 0xAD0900, DebugTexts, 4, sTexts_AD0900, {});

int DDCheat::sub_498B40(int idx, const char* formatStr, ...)
{
    va_list va;
    va_start(va, formatStr);
    if (idx < 0 || idx > 3)
    {
        return -1;
    }

    char buffer[1024] = {};
    vsprintf(buffer, formatStr, va);
    strncat(sTexts_AD0900[idx].field_9_text.field_0_src_txt, buffer, sTexts_AD0900[idx].field_4_max_len);
    return static_cast<int>(strlen(sTexts_AD0900[idx].field_9_text.field_0_src_txt));
}

int DDCheat::DebugOut_495990(const char* pStr, ...)
{
    va_list va;
    va_start(va, pStr);

    char strBuffer[1024];
    vsprintf(strBuffer, pStr, va);
    DDCheat::sub_498B40(0, strBuffer);
    return 0;
}

}

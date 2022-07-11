#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Psx.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "Slig.hpp"
#include "Input.hpp"
#include "Elum.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/PsxDisplay.hpp"

namespace AO {

struct DDCheatProperties final
{
    DDCheatProperty props[10];
};
ALIVE_VAR(1, 0x4FF7D8, DDCheatProperties, DDCheatProperties_4FF7D8, {});

ALIVE_VAR(1, 0x5076FC, s32, gDoorsOpen_5076FC, 0);
ALIVE_VAR(1, 0x5076D8, s32, gTweak_X_5076D8, 0);
ALIVE_VAR(1, 0x5076DC, s32, gTweak_Y_5076DC, 0);

ALIVE_VAR(1, 0x5076C0, s16, sRescuedMudokons_5076C0, 0);
ALIVE_VAR(1, 0x5076BC, s16, sKilledMudokons_5076BC, 0);

ALIVE_VAR(1, 0x5076E0, s16, showDebugCreatureInfo_5076E0, 0);
ALIVE_VAR(1, 0x50771C, s16, sDDCheat_FlyingEnabled_50771C, 0);

using TDDCheatFn = decltype(&DDCheat::Teleport);

const TDDCheatFn CheatsFn_4C3150[] = {
    &DDCheat::Teleport,
    &DDCheat::Misc};

DDCheat::DDCheat()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    mBaseGameObjectTypeId = ReliveTypes::eDDCheat;
    field_10_bInvalidateRect = 0;
    field_14_SelectedCheat = 0;
    field_18_backInputPressed = 0;
    field_1C_unused = 0;
    field_20_bTeleportCheatActive = 0;

    ClearProperties();

    AddPropertyEntry("Doors Open ", DDCheatValueType::eShort_4, &gDoorsOpen_5076FC);
    AddPropertyEntry("Tweak X ", DDCheatValueType::eInt_6, &gTweak_X_5076D8);
    AddPropertyEntry("Tweak Y ", DDCheatValueType::eInt_6, &gTweak_Y_5076DC);
    AddPropertyEntry("RescuedMudokons ", DDCheatValueType::eShort_4, &sRescuedMudokons_5076C0);
}

void DDCheat::ClearProperties()
{
    DDCheatProperties_4FF7D8 = {};
}

void DDCheat::AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr)
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

ALIVE_VAR(1, 0x4FF868, u16, unused_4FF868, 0);

ALIVE_VAR(1, 0x4C315C, s32, level_4C315C, 3);
ALIVE_VAR(1, 0x4C3160, u16, path_4C3160, 1);
ALIVE_VAR(1, 0x4FF864, u32, gVox_4FF864, 0);
ALIVE_VAR(1, 0x4FF860, u16, doNothing_4FF860, 0);
ALIVE_VAR_EXTERN(s8, gDDCheatMode_508BF8);

ALIVE_VAR(1, 0x9F0E40, u32, dword_9F0E40, 0);
ALIVE_VAR(1, 0x9F0E44, u32, dword_9F0E44, 1);

ALIVE_VAR(1, 0x4FF854, u32, currentlyPressedButtons_4FF854, 0);
ALIVE_VAR(1, 0x4C31A8, u32, dword_4C31A8, 10);


template <typename T>
static void writeHeaderElement(const T& element, FILE* f)
{
    ::fwrite(&element, 1, sizeof(T), f);
}

void DDCheat::ScreenShot()
{
    auto pixelBuffer = relive_new u16[640 * gPsxDisplay.mHeight];
    if (pixelBuffer)
    {
        char_type fileNameBuffer[16] = {};
        ::sprintf(fileNameBuffer, "SD%06ld.TGA", static_cast<long int>(sGnFrame % 1000000));
        const auto fileHandle = ::fopen(fileNameBuffer, "wb");
        if (!fileHandle)
        {
            return;
        }
        const PSX_RECT rect = {0, 0, 640, static_cast<s16>(gPsxDisplay.mHeight)};
        PSX_StoreImage_496320(&rect, pixelBuffer);
        PSX_DrawSync_496750(0);

        for (s32 i = 0; i < 640 * gPsxDisplay.mHeight; i++)
        {
            const u16 pixel = pixelBuffer[i];
            pixelBuffer[i] = ((pixel >> 10) & 0x1F) + (32 * (32 * (pixel & 0x1F) + ((pixel >> 5) & 0x1F)));
        }

        struct
        {
            s8 idlength = 0;
            s8 colourmaptype = 0;
            s8 datatypecode = 2;
            s16 colourmaporigin = 0;
            s16 colourmaplength = 0;
            s8 colourmapdepth = 0;
            s16 x_origin = 0;
            s16 y_origin = 0;
            s16 width = 640;
            s16 height = 480;
            s8 bitsperpixel = 16;
            s8 imagedescriptor = 0;
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

        u8* rowOfPixels = reinterpret_cast<u8*>(pixelBuffer + 640 * 239);
        const s32 rowNum = 240;
        for (s32 i = 0; i < rowNum; i++)
        {
            fwrite(rowOfPixels, 1, 640 * sizeof(u16), fileHandle);
            fwrite(rowOfPixels, 1, 640 * sizeof(u16), fileHandle);
            rowOfPixels -= 640 * sizeof(u16);
        }
        ::fclose(fileHandle);
        relive_delete[] pixelBuffer;
    }
}

s32 sub_49AD50(s32 /*a1*/)
{
    return 0;
}

void DDCheat::VUpdate()
{
    if (gDDCheatMode_508BF8)
    {
        const InputObject::PadIndex otherController = Input().CurrentController() == InputObject::PadIndex::First ? InputObject::PadIndex::Second : InputObject::PadIndex::First;
        Abe* pAbe = sActiveHero;
        s32 cheat_enabled = 0;

        if (unused_4FF868)
        {
            ScreenShot();
        }
        field_10_bInvalidateRect = 0;

        if (field_20_bTeleportCheatActive == 0)
        {
            cheat_enabled = sDDCheat_FlyingEnabled_50771C;
        }
        else
        {
            field_20_bTeleportCheatActive = 0;
            if (pAbe)
            {
                PSX_Point point = {};
                gMap.GetCurrentCamCoords(&point);
                pAbe = sActiveHero;
                cheat_enabled = 1;
                pAbe->mXPos = FP_FromInteger(point.x + 448);
                pAbe->mYPos = FP_FromInteger(point.y + 180);
                pAbe->mCurrentMotion = eAbeMotions::Motion_3_Fall_42E7F0;
                pAbe->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);
                pAbe->mCurrentLevel = MapWrapper::FromAO(static_cast<LevelIds>(level_4C315C));
                pAbe->mCurrentPath = static_cast<s16>(path_4C3160);
                sDDCheat_FlyingEnabled_50771C = 1;
                field_18_backInputPressed = 0;
            }
        }

        if (gMap.mCurrentLevel != EReliveLevelIds::eMenu)
        {
            if (pAbe)
            {
                if (Input().IsAnyHeld(InputCommands::eCheatMode))
                {
                    sDDCheat_FlyingEnabled_50771C = cheat_enabled == 0;
                    if (sDDCheat_FlyingEnabled_50771C)
                    {
                        pAbe->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);
                        showDebugCreatureInfo_5076E0 = 0;
                        sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                        sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
                        switch (sControlledCharacter->mBaseGameObjectTypeId)
                        {
                            case ReliveTypes::eElum:
                            {
                                sControlledCharacter->mCurrentMotion = eElumMotions::Motion_21_Land_414A20;
                                break;
                            }
                            case ReliveTypes::eAbe:
                            {
                                sControlledCharacter->mCurrentMotion = eAbeMotions::Motion_3_Fall_42E7F0;
                                break;
                            }
                            case ReliveTypes::eSlig:
                            {
                                sControlledCharacter->mCurrentMotion = eSligMotions::Motion_7_Falling_46A1A0;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }
        }

        /*const auto screenshotCombination = InputCommands::eDoAction | InputCommands::eSneak | InputCommands::eRun;
        if (Input().IsAllPressed(otherController, screenshotCombination))
        {
            // will always be true in the dll version so we disable it because
            // it would take a bunch of screenshots every frame
            //ScreenShot();
        }
        else*/
        {
            if (cheat_enabled)
            {
                if (pAbe)
                {
                    pAbe->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);
                }

                if (sControlledCharacter)
                {
                    sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                    sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
                }
            }

            if (!(sGnFrame % 10))
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
                DebugStr(
                    "\n%sP%dC%d %6d",
                    Path_Get_Lvl_Name(gMap.mCurrentLevel),
                    gMap.mCurrentPath,
                    gMap.mCurrentCamera,
                    sGnFrame);
                DebugStr(
                    " mem used %5d mem peak %5d",
                    (sManagedMemoryUsedSize_9F0E48 + 999) / 1000,
                    (sPeakedManagedMemUsage_9F0E4C + 999) / 1000);
                //DebugStr(" Vox %d\n", gVox_4FF864);

                if (sActiveHero)
                {
                    DebugStr(
                        "\nheroxy=%4d,%4d\n",
                        FP_GetExponent(sActiveHero->mXPos),
                        FP_GetExponent(sActiveHero->mYPos));
                }

                cheat_enabled = sDDCheat_FlyingEnabled_50771C;
                field_10_bInvalidateRect = 6;
            }

            if (cheat_enabled)
            {
                auto isHeld = Input().Held();
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
            field_24_input = Input().Held(otherController);
            auto isPressed = Input().Pressed(otherController);
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

            if (field_24_input & InputCommands::eBack)
            {
                field_18_backInputPressed = field_18_backInputPressed == 0;
            }

            if (field_18_backInputPressed)
            {
                if (isPressed & InputCommands::eSneak && isPressed & InputCommands::eCheatMode)
                {
                    field_14_SelectedCheat = 0;
                }
                else if (field_24_input & InputCommands::eCheatMode)
                {
                    field_14_SelectedCheat++;
                    if (field_14_SelectedCheat >= 2)
                    {
                        field_14_SelectedCheat = 0;
                    }
                }
                (this->*CheatsFn_4C3150[field_14_SelectedCheat])();
            }

            if (field_10_bInvalidateRect)
            {
                pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, 240);
            }
        }
    }
}

const char_type* lvl_names_4C3168[16] = {
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
    "Desert Level Ender"};

ALIVE_VAR(1, 0x4C3164, s16, camera_4C3164, 1);

void DDCheat::Teleport()
{
    DebugStr("\n[Teleport]\n");
    DebugStr("Level    (L,R):      %s \n", lvl_names_4C3168[level_4C315C]);
    DebugStr("Path    (Up/Down):   %d \n", path_4C3160);
    DebugStr("Camera (Left/Right): %d \n", static_cast<u16>(camera_4C3164));
    DebugStr("Teleport = [] Reset = O\n"); //TODO don't display PSX buttons
    s32 input = field_24_input;
    field_10_bInvalidateRect = 6;
    if (input & InputCommands::eSneak)
    {
        if (level_4C315C)
        {
            --level_4C315C;
        }
    }
    else if (input & InputCommands::eCheatMode)
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
        path_4C3160 = gMap.mCurrentPath;
        level_4C315C = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        camera_4C3164 = gMap.mCurrentCamera;
    }
    else if (input & InputCommands::eDoAction)
    {
        if (path_4C3160 <= 21u)
        {
            const auto pPathRec = Path_Get_Bly_Record_434650(MapWrapper::FromAO(static_cast<LevelIds>(level_4C315C)), path_4C3160);
            if (pPathRec && pPathRec->field_0_blyName && pPathRec->field_4_pPathData && pPathRec->field_8_pCollisionData)
            {
                if (camera_4C3164 <= 21)
                {
                    sDDCheat_FlyingEnabled_50771C = 1;
                    gMap.SetActiveCam(
                        MapWrapper::FromAO(static_cast<LevelIds>(level_4C315C)),
                        path_4C3160,
                        camera_4C3164,
                        CameraSwapEffects::eInstantChange_0,
                        0,
                        0);
                    field_20_bTeleportCheatActive = 1;
                }
            }
        }
    }
}

ALIVE_VAR(1, 0x4C3158, u32, gScale_4C3158, 100);

void DDCheat::Misc()
{
    if (field_24_input & InputCommands::eLeft)
    {
        gScale_4C3158 = 100;
        sControlledCharacter->mSpriteScale = FP_FromInteger(1);
        sControlledCharacter->mScale = Scale::Fg;
        sControlledCharacter->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    }
    else if (field_24_input & InputCommands::eRight)
    {
        sControlledCharacter = sControlledCharacter;
        gScale_4C3158 = 50;
        sControlledCharacter->mSpriteScale = FP_FromDouble(0.5);
        sControlledCharacter->mScale = Scale::Bg;
        sControlledCharacter->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
    }
    else if (field_24_input & InputCommands::eDown)
    {
        gScale_4C3158 -= 5;
        sControlledCharacter->mSpriteScale = FP_FromInteger(gScale_4C3158) * FP_FromDouble(0.01);
    }
    else if (field_24_input & InputCommands::eUp)
    {
        gScale_4C3158 += 5;
        sControlledCharacter->mSpriteScale = FP_FromInteger(gScale_4C3158) * FP_FromDouble(0.01);
    }
    else if (field_24_input & InputCommands::eHop)
    {
        gAbeInvulnerableCheat_5076E4 = gAbeInvulnerableCheat_5076E4 == 0;
    }
    else if (field_24_input & InputCommands::eCrouchOrRoll)
    {
        gAbeInvisibleCheat_5076F8 = gAbeInvisibleCheat_5076F8 == 0;
    }
    DebugStr("\nScale: up=+5 down=-5 left=100 right=50\n");
    DebugStr("Scale: %d\n\n", gScale_4C3158);

    const char_type* invulnerableDisplayText = "on";
    if (!gAbeInvulnerableCheat_5076E4)
    {
        invulnerableDisplayText = "off";
    }
    DebugStr("triangle=invulnerable (%s)\n", invulnerableDisplayText);

    const char_type* invisibleDisplayText = "on";
    if (!gAbeInvisibleCheat_5076F8)
    {
        invisibleDisplayText = "off";
    }
    DebugStr("cross = invisible (%s)\n", invisibleDisplayText);

    field_10_bInvalidateRect = 9;
    if (!gElum)
    {
        if (sControlledCharacter != gElum)
        {
            return;
        }
    }
    else if (sControlledCharacter != gElum)
    {
        gElum->mSpriteScale = sControlledCharacter->mSpriteScale;
        gElum->mScale = sControlledCharacter->mScale;
        if (sControlledCharacter != gElum)
        {
            return;
        }
    }
    sActiveHero->mSpriteScale = sControlledCharacter->mSpriteScale;
    sActiveHero->mScale = sControlledCharacter->mScale;
}



s32 DDCheat::DebugFont_Printf(s32 idx, const char_type* formatStr, ...)
{
    // AE_IMPLEMENTED();

    va_list va;
    va_start(va, formatStr);
    if (idx < 0 || idx > 3)
    {
        return -1;
    }

    char_type buffer[1024] = {};
    vsprintf(buffer, formatStr, va);

    return ::DebugFont_Printf(idx, buffer);
}

s32 DDCheat::DebugStr(const char_type* pStr, ...)
{
    va_list va;
    va_start(va, pStr);

    char_type strBuffer[1024];
    vsprintf(strBuffer, pStr, va);
    DDCheat::DebugFont_Printf(0, strBuffer);
    return 0;
}

} // namespace AO

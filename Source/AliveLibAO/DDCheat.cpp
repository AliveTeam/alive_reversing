#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "Slig.hpp"
#include "Input.hpp"
#include "Elum.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"

namespace AO {

struct DDCheatProperties final
{
    DDCheatProperty props[10];
};
DDCheatProperties DDCheatProperties_4FF7D8 = {};

s32 gDoorsOpen_5076FC = 0;
s32 gTweakX = 0;
s32 gTweakY = 0;

s16 sRescuedMudokons = 0;
s16 sKilledMudokons = 0;

s16 showDebugCreatureInfo_5076E0 = 0;
bool gDDCheat_FlyingEnabled = false;

using TDDCheatFn = decltype(&DDCheat::Teleport);

const TDDCheatFn CheatsFn_4C3150[] = {
    &DDCheat::Teleport,
    &DDCheat::Misc};

DDCheat::DDCheat()
    : BaseGameObject(true, 0)
{
    mBaseGameObjectFlags.Set(Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    SetType(ReliveTypes::eDDCheat);
    field_14_SelectedCheat = 0;
    field_18_backInputPressed = 0;
    field_1C_unused = 0;
    field_20_bTeleportCheatActive = 0;

    ClearProperties();

    AddPropertyEntry("Doors Open ", DDCheatValueType::eShort_4, &gDoorsOpen_5076FC);
    AddPropertyEntry("Tweak X ", DDCheatValueType::eInt_6, &gTweakX);
    AddPropertyEntry("Tweak Y ", DDCheatValueType::eInt_6, &gTweakY);
    AddPropertyEntry("RescuedMudokons ", DDCheatValueType::eShort_4, &sRescuedMudokons);
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

s32 level_4C315C = 3;
u16 path_4C3160 = 1;
u32 gVox_4FF864 = 0;
u16 doNothing_4FF860 = 0;
extern s8 gDDCheatOn;

u32 dword_9F0E40 = 0;
u32 dword_9F0E44 = 1;

u32 currentlyPressedButtons_4FF854 = 0;
u32 dword_4C31A8 = 10;


template <typename T>
static void writeHeaderElement(const T& element, FILE* f)
{
    ::fwrite(&element, 1, sizeof(T), f);
}

s32 sub_49AD50(s32 /*a1*/)
{
    return 0;
}

void DDCheat::VUpdate()
{
    if (gDDCheatOn)
    {
        const InputObject::PadIndex otherController = Input().CurrentController() == InputObject::PadIndex::First ? InputObject::PadIndex::Second : InputObject::PadIndex::First;
        Abe* pAbe = sActiveHero;
        bool cheat_enabled = false;

        if (field_20_bTeleportCheatActive == 0)
        {
            cheat_enabled = gDDCheat_FlyingEnabled;
        }
        else
        {
            field_20_bTeleportCheatActive = 0;
            if (pAbe)
            {
                PSX_Point point = {};
                gMap.GetCurrentCamCoords(&point);
                pAbe = sActiveHero;
                cheat_enabled = true;
                pAbe->mXPos = FP_FromInteger(point.x + 448);
                pAbe->mYPos = FP_FromInteger(point.y + 180);
                pAbe->mCurrentMotion = eAbeMotions::Motion_3_Fall;
                pAbe->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);
                pAbe->mCurrentLevel = MapWrapper::FromAO(static_cast<LevelIds>(level_4C315C));
                pAbe->mCurrentPath = static_cast<s16>(path_4C3160);
                gDDCheat_FlyingEnabled = true;
                field_18_backInputPressed = 0;
            }
        }

        if (gMap.mCurrentLevel != EReliveLevelIds::eMenu)
        {
            if (pAbe)
            {
                if (Input().IsAnyHeld(InputCommands::eCheatMode))
                {
                    gDDCheat_FlyingEnabled = cheat_enabled == false;
                    if (gDDCheat_FlyingEnabled)
                    {
                        pAbe->field_2A8_flags.Set(Flags_2A8::e2A8_Bit8_bLandSoft);
                        showDebugCreatureInfo_5076E0 = 0;
                        sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                        sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
                        switch (sControlledCharacter->Type())
                        {
                            case ReliveTypes::eElum:
                            {
                                static_cast<Elum*>(sControlledCharacter)->SetCurrentMotion(eElumMotions::Motion_21_Land);
                                break;
                            }
                            case ReliveTypes::eAbe:
                            {
                                sControlledCharacter->mCurrentMotion = eAbeMotions::Motion_3_Fall;
                                break;
                            }
                            case ReliveTypes::eSlig:
                            {
                                sControlledCharacter->mCurrentMotion = eSligMotions::Motion_7_Falling;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }
        }


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
            cheat_enabled = gDDCheat_FlyingEnabled;
        }

        if (cheat_enabled || showDebugCreatureInfo_5076E0 || doNothing_4FF860)
        {
            DebugStr(
                "\n%sP%dC%d %6d",
                Path_Get_Lvl_Name(gMap.mCurrentLevel),
                gMap.mCurrentPath,
                gMap.mCurrentCamera,
                sGnFrame);

            if (sActiveHero)
            {
                DebugStr(
                    "\nheroxy=%4d,%4d\n",
                    FP_GetExponent(sActiveHero->mXPos),
                    FP_GetExponent(sActiveHero->mYPos));
            }

            cheat_enabled = gDDCheat_FlyingEnabled;
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

s16 camera_4C3164 = 1;

void DDCheat::Teleport()
{
    DebugStr("\n[Teleport]\n");
    DebugStr("Level    (L,R):      %s \n", lvl_names_4C3168[level_4C315C]);
    DebugStr("Path    (Up/Down):   %d \n", path_4C3160);
    DebugStr("Camera (Left/Right): %d \n", static_cast<u16>(camera_4C3164));
    DebugStr("Teleport = [] Reset = O\n"); //TODO don't display PSX buttons
    s32 input = field_24_input;
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
                    gDDCheat_FlyingEnabled = true;
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

u32 gScale_4C3158 = 100;

void DDCheat::Misc()
{
    if (field_24_input & InputCommands::eLeft)
    {
        gScale_4C3158 = 100;
        sControlledCharacter->SetSpriteScale(FP_FromInteger(1));
        sControlledCharacter->SetScale(Scale::Fg);
        sControlledCharacter->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
    }
    else if (field_24_input & InputCommands::eRight)
    {
        sControlledCharacter = sControlledCharacter; // TODO: rev bug? check in IDA
        gScale_4C3158 = 50;
        sControlledCharacter->SetSpriteScale(FP_FromDouble(0.5));
        sControlledCharacter->SetScale(Scale::Bg);
        sControlledCharacter->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
    }
    else if (field_24_input & InputCommands::eDown)
    {
        gScale_4C3158 -= 5;
        sControlledCharacter->SetSpriteScale(FP_FromInteger(gScale_4C3158) * FP_FromDouble(0.01));
    }
    else if (field_24_input & InputCommands::eUp)
    {
        gScale_4C3158 += 5;
        sControlledCharacter->SetSpriteScale(FP_FromInteger(gScale_4C3158) * FP_FromDouble(0.01));
    }
    else if (field_24_input & InputCommands::eHop)
    {
        gAbeInvulnerableCheat = gAbeInvulnerableCheat == 0;
    }
    else if (field_24_input & InputCommands::eCrouchOrRoll)
    {
        gAbeInvisibleCheat = gAbeInvisibleCheat == 0;
    }
    DebugStr("\nScale: up=+5 down=-5 left=100 right=50\n");
    DebugStr("Scale: %d\n\n", gScale_4C3158);

    const char_type* invulnerableDisplayText = "on";
    if (!gAbeInvulnerableCheat)
    {
        invulnerableDisplayText = "off";
    }
    DebugStr("triangle=invulnerable (%s)\n", invulnerableDisplayText);

    const char_type* invisibleDisplayText = "on";
    if (!gAbeInvisibleCheat)
    {
        invisibleDisplayText = "off";
    }
    DebugStr("cross = invisible (%s)\n", invisibleDisplayText);

    if (!gElum)
    {
        if (sControlledCharacter != gElum)
        {
            return;
        }
    }
    else if (sControlledCharacter != gElum)
    {
        gElum->SetSpriteScale(sControlledCharacter->GetSpriteScale());
        gElum->SetScale(sControlledCharacter->GetScale());
        if (sControlledCharacter != gElum)
        {
            return;
        }
    }
    sActiveHero->SetSpriteScale(sControlledCharacter->GetSpriteScale());
    sActiveHero->SetScale(sControlledCharacter->GetScale());
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

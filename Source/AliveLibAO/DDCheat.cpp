#include "stdafx_ao.h"
#include "DDCheat.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "Slig.hpp"
#include "Input.hpp"
#include "Elum.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"

extern bool gDDCheatOn;

namespace AO {

static s32 sDoorsOpen = 0;
s16 gRescuedMudokons = 0;
s16 gKilledMudokons = 0;

static u16 sTeleport_Level = 3;
static u16 sTeleport_Path = 1;
static u16 sTeleport_Cam = 1;

bool gDDCheat_FlyingEnabled = false;
bool gDDCheat_ShowAI_Info = false;

using TDDCheatFn = decltype(&DDCheat::Teleport);

#define DDCHEAT_FN_COUNT 2
const TDDCheatFn sDDCheat_FnCheatsTable[DDCHEAT_FN_COUNT] = {
    &DDCheat::Teleport,
    &DDCheat::Misc};

struct DDCheatProperties final
{
    DDCheatProperty props[10];
};
static DDCheatProperties sDDCheatProperties = {};

static const char_type* sTeleportLevelNameTable[16] = {
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

DDCheat::DDCheat()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eDDCheat);

    ClearProperties();

    AddPropertyEntry("Doors Open ", DDCheatValueType::eShort_4, &sDoorsOpen);
    AddPropertyEntry("RescuedMudokons ", DDCheatValueType::eShort_4, &gRescuedMudokons);
}

void DDCheat::ClearProperties()
{
    sDDCheatProperties = {};
}

void DDCheat::AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr)
{
    for (auto& prop : sDDCheatProperties.props)
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
    if (!gDDCheatOn || !gAbe)
    {
        return;
    }

    const InputObject::PadIndex otherController = Input().CurrentController() == InputObject::PadIndex::First ? InputObject::PadIndex::Second : InputObject::PadIndex::First;
    bool cheat_enabled = false;

    if (mTeleporting == 0)
    {
        cheat_enabled = gDDCheat_FlyingEnabled;
    }
    else
    {
        mTeleporting = 0;
        PSX_Point point = {};
        gMap.GetCurrentCamCoords(&point);
        cheat_enabled = true;
        gAbe->mXPos = FP_FromInteger(point.x + 448);
        gAbe->mYPos = FP_FromInteger(point.y + 180);
        gAbe->mCurrentMotion = eAbeMotions::Motion_3_Fall;
        gAbe->mLandSoft = true;
        gAbe->mCurrentLevel = MapWrapper::FromAO(static_cast<LevelIds>(sTeleport_Level));
        gAbe->mCurrentPath = static_cast<s16>(sTeleport_Path);
        gDDCheat_FlyingEnabled = true;
        field_18_backInputPressed = 0;
    }

    if (gMap.mCurrentLevel != EReliveLevelIds::eMenu)
    {
        if (Input().IsAnyPressed(InputCommands::eCheatMode))
        {
            gDDCheat_FlyingEnabled = !cheat_enabled;
            if (gDDCheat_FlyingEnabled)
            {
                gAbe->mLandSoft = true;
                gDDCheat_ShowAI_Info = 0;
                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
                switch (sControlledCharacter->Type())
                {
                    case ReliveTypes::eElum:
                    {
                        static_cast<Elum*>(sControlledCharacter)->mCurrentMotion = eElumMotions::Motion_21_Land;
                        break;
                    }
                    case ReliveTypes::eAbe:
                    {
                        static_cast<Abe*>(sControlledCharacter)->mCurrentMotion = eAbeMotions::Motion_3_Fall;
                        break;
                    }
                    case ReliveTypes::eSlig:
                    {
                        static_cast<Slig*>(sControlledCharacter)->mCurrentMotion = eSligMotions::Motion_7_Falling;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }


    if (cheat_enabled)
    {
        gAbe->mLandSoft = true;

        if (sControlledCharacter)
        {
            sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
            sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
        }
    }

    if (!(sGnFrame % 10))
    {
        cheat_enabled = gDDCheat_FlyingEnabled;
    }

    if (cheat_enabled || gDDCheat_ShowAI_Info)
    {
        DebugStr(
            "\n%sP%dC%d %6d",
            AO::Path_Get_Lvl_Name(gMap.mCurrentLevel),
            gMap.mCurrentPath,
            gMap.mCurrentCamera,
            sGnFrame);

        DebugStr(
            "\nheroxy=%4d,%4d\n",
            FP_GetExponent(gAbe->mXPos),
            FP_GetExponent(gAbe->mYPos));

        cheat_enabled = gDDCheat_FlyingEnabled;
    }

    if (cheat_enabled)
    {
        auto isHeld = Input().GetPressed();
        if (isHeld & InputCommands::eDoAction)
        {
            gDDCheat_ShowAI_Info = gDDCheat_ShowAI_Info == 0;
        }
        if (isHeld & InputCommands::eThrowItem)
        {
            dword_9F0E44 = 0;
        }
    }
    field_24_input = Input().GetPressed(otherController);
    auto isPressed = Input().GetHeld(otherController);
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
            mCheatFnIdx = 0;
        }
        else if (field_24_input & InputCommands::eCheatMode)
        {
            mCheatFnIdx++;
            if (mCheatFnIdx >= 2)
            {
                mCheatFnIdx = 0;
            }
        }
        ::BaseAliveGameObject::InvokeMemberFunction(this, sDDCheat_FnCheatsTable, mCheatFnIdx);
    }
}

void DDCheat::Teleport()
{
    DebugStr("\n[Teleport]\n");
    DebugStr("Level    (L,R):      %s \n", sTeleportLevelNameTable[sTeleport_Level]);
    DebugStr("Path    (Up/Down):   %d \n", sTeleport_Path);
    DebugStr("Camera (Left/Right): %d \n", sTeleport_Cam);
    DebugStr("Teleport = [] Reset = O\n"); //TODO don't display PSX buttons
    s32 input = field_24_input;
    if (input & InputCommands::eSneak)
    {
        if (sTeleport_Level)
        {
            --sTeleport_Level;
        }
    }
    else if (input & InputCommands::eCheatMode)
    {
        if (sTeleport_Level < 15u)
        {
            ++sTeleport_Level;
        }
    }
    else if (input & InputCommands::eUp)
    {
        ++sTeleport_Path;
    }
    else if (input & InputCommands::eDown)
    {
        if (sTeleport_Path > 1u)
        {
            --sTeleport_Path;
        }
    }
    else if (input & InputCommands::eLeft)
    {
        if (sTeleport_Cam > 1)
        {
            --sTeleport_Cam;
        }
    }
    else if (input & InputCommands::eRight)
    {
        ++sTeleport_Cam;
    }
    else if (input & InputCommands::eThrowItem)
    {
        sTeleport_Path = gMap.mCurrentPath;
        sTeleport_Level = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        sTeleport_Cam = gMap.mCurrentCamera;
    }
    else if (input & InputCommands::eDoAction)
    {
        if (sTeleport_Path <= 21u)
        {
            const auto pPathRec = Path_Get_Bly_Record(MapWrapper::FromAO(static_cast<LevelIds>(sTeleport_Level)), sTeleport_Path);
            if (pPathRec && pPathRec->field_0_blyName && pPathRec->field_4_pPathData && pPathRec->field_8_pCollisionData)
            {
                if (sTeleport_Cam <= 21)
                {
                    gDDCheat_FlyingEnabled = true;
                    gMap.SetActiveCam(
                        MapWrapper::FromAO(static_cast<LevelIds>(sTeleport_Level)),
                        sTeleport_Path,
                        sTeleport_Cam,
                        CameraSwapEffects::eInstantChange_0,
                        0,
                        0);
                    mTeleporting = true;
                }
            }
        }
    }
}

static u32 sScaleMultiplicator = 100;

void DDCheat::Misc()
{
    if (field_24_input & InputCommands::eLeft)
    {
        sScaleMultiplicator = 100;
        sControlledCharacter->SetSpriteScale(FP_FromInteger(1));
        sControlledCharacter->SetScale(Scale::Fg);
        sControlledCharacter->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
    }
    else if (field_24_input & InputCommands::eRight)
    {
        sScaleMultiplicator = 50;
        sControlledCharacter->SetSpriteScale(FP_FromDouble(0.5));
        sControlledCharacter->SetScale(Scale::Bg);
        sControlledCharacter->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
    }
    else if (field_24_input & InputCommands::eDown)
    {
        sScaleMultiplicator -= 5;
        sControlledCharacter->SetSpriteScale(FP_FromInteger(sScaleMultiplicator) * FP_FromDouble(0.01));
    }
    else if (field_24_input & InputCommands::eUp)
    {
        sScaleMultiplicator += 5;
        sControlledCharacter->SetSpriteScale(FP_FromInteger(sScaleMultiplicator) * FP_FromDouble(0.01));
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
    DebugStr("Scale: %d\n\n", sScaleMultiplicator);

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
    gAbe->SetSpriteScale(sControlledCharacter->GetSpriteScale());
    gAbe->SetScale(sControlledCharacter->GetScale());
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

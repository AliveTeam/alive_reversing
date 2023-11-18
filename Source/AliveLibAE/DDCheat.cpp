#include "stdafx.h"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "Movie.hpp"
#include "PathData.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "Abe.hpp"
#include "Input.hpp"
#include "Game.hpp"

static s16 sDoorsOpen = 0;
s16 gRescuedMudokons = 0;
s16 gKilledMudokons = 0;

bool gVisitedBonewerkz = false;
bool gVisitedBarracks = false;
bool gVisitedFeecoEnder = false;

static u16 sTeleport_Level = 0;
static u16 sTeleport_Path = 0;
static u16 sTeleport_Cam = 0;

bool gDDCheat_FlyingEnabled = false;
bool gDDCheat_ShowAI_Info = false;
static bool sDDCheat_AlwaysShow = false;
static s32 sDDCheat_Unknown_5BC004 = 0;
static u32 sDDCheat_PrevDebugInput = 0;
static s32 sDDCheat_DebugInputDelay = 0;

using TDDCheatMenu = decltype(&DDCheat::Menu_Teleport);

#define DDCHEAT_MENU_COUNT 2
static TDDCheatMenu sDDCheat_FnTable[DDCHEAT_MENU_COUNT] = {
    &DDCheat::Menu_Teleport,
    &DDCheat::Menu_Movies,
};

static s16 sDDCheat_MovieSelectIdx = 0;
extern u32 sLevelId_dword_5CA408;

struct DDCheatProperties final
{
    DDCheatProperty props[10];
};


DDCheatProperties DDCheatProperties_5BBF78 = {};

const char_type* sTeleportLevelNameTable_550F64[17] = {
    "Start screen",
    "Mines",
    "Necrum",
    "Paramite Vault",
    "Scrab Vault",
    "Feeco Depot",
    "Slig Barracks",
    "Scrab Ender",
    "Bonewerks",
    "Soulstorm Brewery",
    "Brewery Ender",
    "Paramite Ender",
    "Feeco Ender",
    "Barracks Ender",
    "Bonewerks Ender",
    "Test Level",
    "Credits",
};


void DDCheat::Menu_Teleport()
{
    DebugStr("\n[Teleport]\n");
    DebugStr("Level (1,2):         %s\n", sTeleportLevelNameTable_550F64[sTeleport_Level]);
    DebugStr("Path    (Up/Down):   %d\n", sTeleport_Path);
    DebugStr("Camera (Left/Right): %d\n", sTeleport_Cam);
    DebugStr("Teleport = Enter Reset = Alt\n");

    if (mInputPressed & InputCommands::eGameSpeak1)
    {
        if (sTeleport_Level)
            --sTeleport_Level;
    }
    else if (mInputPressed & InputCommands::eGameSpeak2)
    {
        if (sTeleport_Level < static_cast<s32>(LevelIds::eCredits_16))
        {
            ++sTeleport_Level;
        }
    }
    else if (mInputPressed & InputCommands::eUp)
    {
        ++sTeleport_Path;
    }
    else if (mInputPressed & InputCommands::eDown)
    {
        if (sTeleport_Path > 1)
        {
            --sTeleport_Path;
        }
    }
    else if (mInputPressed & InputCommands::eLeft)
    {
        if (sTeleport_Cam > 1)
        {
            --sTeleport_Cam;
        }
    }
    else if (mInputPressed & InputCommands::eRight)
    {
        ++sTeleport_Cam;
    }
    else if (mInputPressed & InputCommands::eSneak)
    {
        sTeleport_Level = static_cast<s32>(gMap.mCurrentLevel);
        sTeleport_Path = gMap.mCurrentPath;
        sTeleport_Cam = gMap.mCurrentCamera;
    }
    else if (mInputPressed & InputCommands::eUnPause_OrConfirm)
    {
        gDDCheat_FlyingEnabled = true;

        gMap.SetActiveCam(MapWrapper::FromAE(static_cast<LevelIds>(sTeleport_Level)), sTeleport_Path, sTeleport_Cam, CameraSwapEffects::eInstantChange_0, 0, 0);
        mTeleporting = true;
    }
}

void DDCheat::Menu_Movies()
{
    if (mInputPressed & InputCommands::eLeft)
    {
        sDDCheat_MovieSelectIdx--;
    }
    else if (mInputPressed & InputCommands::eRight)
    {
        sDDCheat_MovieSelectIdx++;
    }

    if (Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx)->field_4_id <= 0)
    {
        sDDCheat_MovieSelectIdx = 1;
    }

    if (mInputPressed & InputCommands::eDown)
    {
        Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx)->field_4_id--;
    }
    if (mInputPressed & InputCommands::eUp)
    {
        FmvInfo* movieToPlayInfo = Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx);
        sLevelId_dword_5CA408 = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));
        relive_new Movie(movieToPlayInfo->field_0_pName);
    }

    const FmvInfo* fmvInfo = Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx);
    DDCheat::DebugStr("\n<- Movie -> %d %d %s \n", sDDCheat_MovieSelectIdx, fmvInfo->field_4_id, fmvInfo->field_0_pName);
}

DDCheat::DDCheat()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);
    SetType(ReliveTypes::eDDCheat);

    ClearProperties();

    // The code below allows changing of certain variables in memory using in game controls.
    // There's no code using any of this in the retail final build as the compiler occluded it.
    // But, the Exoddus Demo does in fact have the code, so it's possible to reimplement it
    // in the future.
    AddPropertyEntry("Doors Open ", DDCheatValueType::eShort_4, &sDoorsOpen);
    AddPropertyEntry("RescuedMudokons ", DDCheatValueType::eShort_4, &gRescuedMudokons);
    //AddPropertyEntry("Visited Bonewerks ", DDCheatValueType::eShort_1, &gVisitedBonewerkz);
    //AddPropertyEntry("Visited Barracks ", DDCheatValueType::eShort_1, &gVisitedBarracks);
    //AddPropertyEntry("Visited Feeco Ender ", DDCheatValueType::eShort_1, &gVisitedFeecoEnder);

#if FORCE_DDCHEAT
    gDDCheatOn = true;
#endif
}

void DDCheat::AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr)
{
    for (auto& prop : DDCheatProperties_5BBF78.props)
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

void DDCheat::ClearProperties()
{
    DDCheatProperties_5BBF78 = {};
}

void DDCheat::DebugStr(const char_type* pFormatStr, ...)
{
    char_type buffer[1024] = {};
    va_list va;

    va_start(va, pFormatStr);
    vsprintf(buffer, pFormatStr, va);
    DebugFont_Printf(0, buffer);
}

void DDCheat::VUpdate()
{
    if (!sActiveHero)
    {
        return;
    }

    auto activePadPressed = Input().mPads[sCurrentControllerIndex].mPressed;

    if (gDDCheatOn)
    {
        if (mTeleporting)
        {
            mTeleporting = false;
            if (sActiveHero)
            {
                PSX_Point pos;
                gMap.GetCurrentCamCoords(&pos);
                sActiveHero->mXPos = FP_FromInteger(pos.x + 184);
                sActiveHero->mYPos = FP_FromInteger(pos.y + 60);
                sActiveHero->mCurrentMotion = 3;
                sActiveHero->mLandSoftly = true;
                sActiveHero->mCurrentLevel = MapWrapper::FromAE(static_cast<LevelIds>(sTeleport_Level));
                sActiveHero->mCurrentPath = sTeleport_Path;
                gDDCheat_FlyingEnabled = false;
                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
                mUnknown1 = false;
            }
        }

        if ((gMap.mCurrentLevel != EReliveLevelIds::eMenu && gMap.mCurrentLevel != EReliveLevelIds::eNone) && sActiveHero && activePadPressed & InputCommands::eCheatMode)
        {
            switch (sControlledCharacter->Type())
            {
                case ReliveTypes::eSlig:
                    sControlledCharacter->mCurrentMotion = 7;
                    break;
                case ReliveTypes::eAbe:
                    sControlledCharacter->mCurrentMotion = eAbeMotions::Motion_3_Fall_459B60;
                    break;
                case ReliveTypes::eScrab:
                    sControlledCharacter->mCurrentMotion = 8;
                    break;
                default:
                    LOG_INFO("ddcheat for this controlled character not implemented");
                    return;
            }

            gDDCheat_FlyingEnabled = !gDDCheat_FlyingEnabled;
            if (!gDDCheat_FlyingEnabled)
            {
                if (IsActiveHero(sControlledCharacter))
                {
                    sActiveHero->mLandSoftly = true;
                }
                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
            }

            gDDCheat_ShowAI_Info = false;
        }

        if (gDDCheat_FlyingEnabled || gDDCheat_ShowAI_Info || sDDCheat_AlwaysShow)
        {
            DebugStr(
                "\n%sP%dC%d gnframe=%5d",
                Path_Get_Lvl_Name(gMap.mCurrentLevel),
                gMap.mCurrentPath,
                gMap.mCurrentCamera,
                sGnFrame);

            DebugStr(
                "\nheroxy=%4d,%4d",
                FP_GetExponent(sActiveHero->mXPos),
                FP_GetExponent(sActiveHero->mYPos));

            if (gDDCheat_FlyingEnabled)
            {
                if (activePadPressed & InputCommands::eDoAction)
                {
                    gDDCheat_ShowAI_Info = !gDDCheat_ShowAI_Info;
                }

                if (activePadPressed & InputCommands::eHop)
                {
                    sDDCheat_AlwaysShow = !sDDCheat_AlwaysShow;
                }

                if (IsActiveHero(sControlledCharacter))
                {
                    sActiveHero->mLandSoftly = true;
                }

                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
            }

            /*DebugStr_4F5560("\n[Memory]");
            DebugStr_4F5560("\nUsed: %ikb", sManagedMemoryUsedSize_AB4A04 / 1024);
            DebugStr_4F5560("\nPeak: %ikb", sPeakedManagedMemUsage_AB4A08 / 1024);*/
        }

        mInputPressed = Input().mPads[sCurrentControllerIndex == 0].mPressed;
        if (sDDCheat_PrevDebugInput == Input().mPads[sCurrentControllerIndex == 0].mRawInput
            && sDDCheat_PrevDebugInput)
        {
            if (!--sDDCheat_DebugInputDelay)
            {
                mInputPressed = sDDCheat_PrevDebugInput;
                sDDCheat_DebugInputDelay = 2;
            }
        }
        else
        {
            sDDCheat_PrevDebugInput = Input().mPads[sCurrentControllerIndex == 0].mRawInput;
            sDDCheat_DebugInputDelay = 10;
        }

        if (mInputPressed & InputCommands::ePause)
        {
            mUnknown1 = !mUnknown1;
        }

        if (mUnknown1)
        {
            if (Input().mPads[sCurrentControllerIndex == 0].mRawInput & InputCommands::eCheatMode)
            {
                mFnIdx = 0;
            }
            else if (mInputPressed & InputCommands::eCheatMode)
            {
                mNextFnIdx = mFnIdx;
            }

            // Using hop instead looks like the only way to actually change the menu properly
            if (Input().mPads[sCurrentControllerIndex == 0].mRawInput & InputCommands::eHop /*field_3C_flags & 2*/)
            {
                if (mInputPressed & InputCommands::eDown)
                {
                    mNextFnIdx++;
                }
                else if (mInputPressed & InputCommands::eUp)
                {
                    mNextFnIdx--;
                }

                if (mInputPressed & InputCommands::eUnPause_OrConfirm)
                {
                    //mFnIdx = mNextFnIdx; TODO
                }

                if (mNextFnIdx < 0)
                {
                    mNextFnIdx = DDCHEAT_MENU_COUNT - 1;
                }

                if (mNextFnIdx >= DDCHEAT_MENU_COUNT)
                {
                    mNextFnIdx = 0;
                }

                mFnIdx = mNextFnIdx; // Always set new func index
            }
            else
            {
                (*this.*(sDDCheat_FnTable)[mFnIdx])();
            }
        }
    }
}

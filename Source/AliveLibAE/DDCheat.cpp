#include "stdafx.h"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Movie.hpp"
#include "PathData.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "Abe.hpp"

s16 sDoorsOpen_5C1BE6 = 0;
s32 sTweakX_5C1BD0 = 0;
s32 sTweakY_5C1BD4 = 0;
s16 sRescuedMudokons_5C1BC2 = 0;
s16 sKilledMudokons_5C1BC0 = 0;

s16 sVisitedBonewerks_5C1C02 = 0;
s16 sVisitedBarracks_5C1C04 = 0;
s16 sVisitedFeecoEnder_5C1C06 = 0;

u16 sTeleport_Level_550F5C = 0;
u16 sTeleport_Path_550F5E = 0;
u16 sTeleport_Cam_550F60 = 0;

bool sDDCheat_FlyingEnabled_5C2C08 = false;
bool sDDCheat_ShowAI_Info = false;
bool sDDCheat_AlwaysShow_5BC000 = false;
s32 sDDCheat_Unknown_5BC004 = 0;
u32 sDDCheat_PrevDebugInput_5BBFF4 = 0;
s32 sDDCheat_DebugInputDelay_550FA8 = 0;

using TDDCheatMenu = decltype(&DDCheat::Menu_Teleport);

#define DDCHEAT_MENU_COUNT 2
TDDCheatMenu sDDCheat_FnTable_550F50[DDCHEAT_MENU_COUNT] = {
    &DDCheat::Menu_Teleport,
    &DDCheat::Menu_Movies,
};

s32 sDDCheat_Unused2_AB49FC = 0;
s32 sDDCheat_Unused1_AB4A00 = 0;

s16 sDDCheat_MovieSelectIdx_5BBFF0 = 0;
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
    DebugStr("Level (1,2):         %s\n", sTeleportLevelNameTable_550F64[sTeleport_Level_550F5C]);
    DebugStr("Path    (Up/Down):   %d\n", sTeleport_Path_550F5E);
    DebugStr("Camera (Left/Right): %d\n", sTeleport_Cam_550F60);
    DebugStr("Teleport = Enter Reset = Alt\n");

    field_20 += 6;

    if (field_38_input_pressed & InputCommands::Enum::eGameSpeak1)
    {
        if (sTeleport_Level_550F5C)
            --sTeleport_Level_550F5C;
    }
    else if (field_38_input_pressed & InputCommands::Enum::eGameSpeak2)
    {
        if (sTeleport_Level_550F5C < static_cast<s32>(LevelIds::eCredits_16))
        {
            ++sTeleport_Level_550F5C;
        }
    }
    else if (field_38_input_pressed & InputCommands::Enum::eUp)
    {
        ++sTeleport_Path_550F5E;
    }
    else if (field_38_input_pressed & InputCommands::Enum::eDown)
    {
        if (sTeleport_Path_550F5E > 1)
        {
            --sTeleport_Path_550F5E;
        }
    }
    else if (field_38_input_pressed & InputCommands::Enum::eLeft)
    {
        if (sTeleport_Cam_550F60 > 1)
        {
            --sTeleport_Cam_550F60;
        }
    }
    else if (field_38_input_pressed & InputCommands::Enum::eRight)
    {
        ++sTeleport_Cam_550F60;
    }
    else if (field_38_input_pressed & InputCommands::Enum::eSneak)
    {
        sTeleport_Level_550F5C = static_cast<s32>(gMap.mCurrentLevel);
        sTeleport_Path_550F5E = gMap.mCurrentPath;
        sTeleport_Cam_550F60 = gMap.mCurrentCamera;
    }
    else if (field_38_input_pressed & InputCommands::Enum::eUnPause_OrConfirm)
    {
        sDDCheat_FlyingEnabled_5C2C08 = true;

        gMap.SetActiveCam(MapWrapper::FromAE(static_cast<LevelIds>(sTeleport_Level_550F5C)), sTeleport_Path_550F5E, sTeleport_Cam_550F60, CameraSwapEffects::eInstantChange_0, 0, 0);
        field_3C_flags.Set(DDCheat::Flags_3C::eOnTeleport_Bit3);
    }
}

void DDCheat::Menu_Movies()
{
    if (field_38_input_pressed & InputCommands::Enum::eLeft)
    {
        sDDCheat_MovieSelectIdx_5BBFF0--;
    }
    else if (field_38_input_pressed & InputCommands::Enum::eRight)
    {
        sDDCheat_MovieSelectIdx_5BBFF0++;
    }

    if (Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx_5BBFF0)->field_4_id <= 0)
    {
        sDDCheat_MovieSelectIdx_5BBFF0 = 1;
    }

    if (field_38_input_pressed & InputCommands::Enum::eDown)
    {
        Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx_5BBFF0)->field_4_id--;
    }
    if (field_38_input_pressed & InputCommands::Enum::eUp)
    {
        FmvInfo* movieToPlayInfo = Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx_5BBFF0);
        u32 pos = 0;
        Get_fmvs_sectors(movieToPlayInfo->field_0_pName, 0, 0, &pos, 0, 0);
        sLevelId_dword_5CA408 = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));
        relive_new Movie(movieToPlayInfo->field_4_id,
                                         pos,
                                         movieToPlayInfo->field_6_flags & 1,
                                         movieToPlayInfo->field_8_flags,
                                         movieToPlayInfo->field_A_volume);
    }

    const FmvInfo* fmvInfo = Path_Get_FMV_Record(gMap.mCurrentLevel, sDDCheat_MovieSelectIdx_5BBFF0);
    DDCheat::DebugStr("\n<- Movie -> %d %d %s \n", sDDCheat_MovieSelectIdx_5BBFF0, fmvInfo->field_4_id, fmvInfo->field_0_pName);
    field_20 += 6;
}

DDCheat::DDCheat()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);
    field_3C_flags.Clear(DDCheat::Flags_3C::e3C_Bit4);
    SetType(ReliveTypes::eDDCheat);
    field_20 = 0;
    field_24_fn_idx = 0;

    ClearProperties();

    // The code below allows changing of certain variables in memory using in game controls.
    // There's no code using any of this in the retail final build as the compiler occluded it.
    // But, the Exoddus Demo does in fact have the code, so it's possible to reimplement it
    // in the future.
    AddPropertyEntry("Doors Open ", DDCheatValueType::eShort_4, &sDoorsOpen_5C1BE6);
    AddPropertyEntry("Tweak X ", DDCheatValueType::eInt_6, &sTweakX_5C1BD0);
    AddPropertyEntry("Tweak Y ", DDCheatValueType::eInt_6, &sTweakY_5C1BD4);
    AddPropertyEntry("RescuedMudokons ", DDCheatValueType::eShort_4, &sRescuedMudokons_5C1BC2);
    AddPropertyEntry("Visited Bonewerks ", DDCheatValueType::eShort_1, &sVisitedBonewerks_5C1C02);
    AddPropertyEntry("Visited Barracks ", DDCheatValueType::eShort_1, &sVisitedBarracks_5C1C04);
    AddPropertyEntry("Visited Feeco Ender ", DDCheatValueType::eShort_1, &sVisitedFeecoEnder_5C1C06);

#if FORCE_DDCHEAT
    sCommandLine_DDCheatEnabled_5CA4B5 = true;
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

    field_20 = 0;

    auto activePadPressed = Input().mPads[sCurrentControllerIndex].mHeld;

    if (sCommandLine_DDCheatEnabled_5CA4B5)
    {
        if (field_3C_flags.Get(DDCheat::Flags_3C::eOnTeleport_Bit3))
        {
            field_3C_flags.Clear(DDCheat::Flags_3C::eOnTeleport_Bit3);
            if (sActiveHero)
            {
                PSX_Point pos;
                gMap.GetCurrentCamCoords(&pos);
                sActiveHero->mXPos = FP_FromInteger(pos.x + 184);
                sActiveHero->mYPos = FP_FromInteger(pos.y + 60);
                sActiveHero->mCurrentMotion = 3;
                sActiveHero->field_1AC_flags.Set(Abe::e1AC_Bit7_land_softly);
                sActiveHero->mCurrentLevel = MapWrapper::FromAE(static_cast<LevelIds>(sTeleport_Level_550F5C));
                sActiveHero->mCurrentPath = sTeleport_Path_550F5E;
                sDDCheat_FlyingEnabled_5C2C08 = false;
                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
                field_3C_flags.Clear(DDCheat::Flags_3C::e3C_Bit1);
            }
        }

        if ((gMap.mCurrentLevel != EReliveLevelIds::eMenu && gMap.mCurrentLevel != EReliveLevelIds::eNone) && sActiveHero && activePadPressed & InputCommands::Enum::eCheatMode)
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

            sDDCheat_FlyingEnabled_5C2C08 = !sDDCheat_FlyingEnabled_5C2C08;
            if (!sDDCheat_FlyingEnabled_5C2C08)
            {
                if (IsActiveHero(sControlledCharacter))
                {
                    sActiveHero->field_1AC_flags.Set(Abe::e1AC_Bit7_land_softly);
                }
                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
            }

            sDDCheat_ShowAI_Info = false;
        }

        // Unused
        //if (!(sGnFrame % 10))
        //{
        //    sDDCheat_Unknown_5BC004 = 0;
        //    for (s32 i = 0; i < 24; i++)
        //    {
        //        if (sub_4FA7C0()) // Always returns 0
        //            ++sDDCheat_Unknown_5BC004;
        //    }
        //}

        if (sDDCheat_FlyingEnabled_5C2C08 || sDDCheat_ShowAI_Info || sDDCheat_AlwaysShow_5BC000)
        {
            DebugStr(
                "\n%sP%dC%d gnframe=%5d",
                Path_Get_Lvl_Name(gMap.mCurrentLevel),
                gMap.mCurrentPath,
                gMap.mCurrentCamera,
                sGnFrame);



#if DEVELOPER_MODE
            if (sActiveHero && gMap.mCurrentLevel != LevelIds::eMenu_0)
            {
                // HACK When quitting sControlledCharacter becomes a dangling pointer
                // probably this should be removed as there is no sane way to check this pointer is still valid
                DebugStr(
                    "\n[obj %i] xy=%.3f,%.3f flags=%x",
                    sControlledCharacter->Type(),
                    FP_GetDouble(sControlledCharacter->mXPos),
                    FP_GetDouble(sControlledCharacter->mYPos),
                    sControlledCharacter->mFlags);

                DebugStr("\nLine=%X\nState=%i", sControlledCharacter->field_100_pCollisionLine, sControlledCharacter->field_106_current_motion);

                if (sControlledCharacter->Type() == AETypes::eAbe_69)
                {
                    DebugStr("\nStateName=%s", sAbeMotionNames[sControlledCharacter->field_106_current_motion]);
                }
            }
#else
            DebugStr(
                "\nheroxy=%4d,%4d",
                FP_GetExponent(sActiveHero->mXPos),
                FP_GetExponent(sActiveHero->mYPos));
#endif

            field_20 = 6;

            if (sDDCheat_FlyingEnabled_5C2C08)
            {
                if (activePadPressed & InputCommands::Enum::eDoAction)
                {
                    sDDCheat_ShowAI_Info = !sDDCheat_ShowAI_Info;
                }

                if ((activePadPressed & InputCommands::Enum::eThrowItem) != 0)
                {
                    sPeakedManagedMemUsage_AB4A08 = sManagedMemoryUsedSize_AB4A04;
                    sDDCheat_Unused1_AB4A00 = sDDCheat_Unused2_AB49FC;
                }

                if (activePadPressed & InputCommands::Enum::eHop)
                {
                    sDDCheat_AlwaysShow_5BC000 = !sDDCheat_AlwaysShow_5BC000;
                }

                if (IsActiveHero(sControlledCharacter))
                {
                    sActiveHero->field_1AC_flags.Set(Abe::e1AC_Bit7_land_softly);
                }

                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
            }

            /*DebugStr_4F5560("\n[Memory]");
            DebugStr_4F5560("\nUsed: %ikb", sManagedMemoryUsedSize_AB4A04 / 1024);
            DebugStr_4F5560("\nPeak: %ikb", sPeakedManagedMemUsage_AB4A08 / 1024);*/
        }

        field_38_input_pressed = Input().mPads[sCurrentControllerIndex == 0].mHeld;
        if (sDDCheat_PrevDebugInput_5BBFF4 == Input().mPads[sCurrentControllerIndex == 0].mPressed
            && sDDCheat_PrevDebugInput_5BBFF4)
        {
            if (!--sDDCheat_DebugInputDelay_550FA8)
            {
                field_38_input_pressed = sDDCheat_PrevDebugInput_5BBFF4;
                sDDCheat_DebugInputDelay_550FA8 = 2;
            }
        }
        else
        {
            sDDCheat_PrevDebugInput_5BBFF4 = Input().mPads[sCurrentControllerIndex == 0].mPressed;
            sDDCheat_DebugInputDelay_550FA8 = 10;
        }

        if (field_38_input_pressed & InputCommands::Enum::ePause)
        {
            field_3C_flags.Toggle(DDCheat::Flags_3C::e3C_Bit1);
        }

        if (field_3C_flags.Get(DDCheat::Flags_3C::e3C_Bit1))
        {
            if (Input().mPads[sCurrentControllerIndex == 0].mPressed & InputCommands::Enum::eCheatMode)
            {
                field_24_fn_idx = 0;
            }
            else if (field_38_input_pressed & InputCommands::Enum::eCheatMode)
            {
                field_3C_flags.Toggle(DDCheat::Flags_3C::e3C_Bit2);
                field_26_next_fn_idx = field_24_fn_idx;
            }

            // Using hop instead looks like the only way to actually change the menu properly
            if (Input().mPads[sCurrentControllerIndex == 0].mPressed & InputCommands::Enum::eHop /*field_3C_flags & 2*/)
            {
                if (field_38_input_pressed & InputCommands::Enum::eDown)
                {
                    field_26_next_fn_idx++;
                }
                else if (field_38_input_pressed & InputCommands::Enum::eUp)
                {
                    field_26_next_fn_idx--;
                }

                if (field_38_input_pressed & InputCommands::Enum::eUnPause_OrConfirm)
                {
                    //field_24_fn_idx = field_26_next_fn_idx; TODO
                    field_3C_flags.Clear(DDCheat::Flags_3C::e3C_Bit2);
                }

                if (field_26_next_fn_idx < 0)
                {
                    field_26_next_fn_idx = DDCHEAT_MENU_COUNT - 1;
                }

                if (field_26_next_fn_idx >= DDCHEAT_MENU_COUNT)
                {
                    field_26_next_fn_idx = 0;
                }

                field_24_fn_idx = field_26_next_fn_idx; // Always set new func index
                field_20 += 4;
            }
            else
            {
                (*this.*(sDDCheat_FnTable_550F50)[field_24_fn_idx])();
            }
        }
    }
}

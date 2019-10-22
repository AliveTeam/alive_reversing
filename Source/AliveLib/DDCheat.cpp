#include "stdafx.h"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Movie.hpp"
#include "PathData.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Abe.hpp"

void DDCheat_ForceLink() { }

ALIVE_VAR(1, 0x5c1be6, int, sDoorsOpen_5C1BE6, 0);
ALIVE_VAR(1, 0x5c1bd0, int, sTweakX_5C1BD0, 0);
ALIVE_VAR(1, 0x5c1bd4, int, sTweakY_5C1BD4, 0);
ALIVE_VAR(1, 0x5c1bc2, __int16, sRescuedMudokons_5C1BC2, 0);
ALIVE_VAR(1, 0x5c1bc0, __int16, sKilledMudokons_5C1BC0, 0);

ALIVE_VAR(1, 0x5c1c02, __int16, sVisitedBonewerks_5C1C02, 0);
ALIVE_VAR(1, 0x5c1c04, __int16, sVisitedBarracks_5C1C04, 0);
ALIVE_VAR(1, 0x5c1c06, __int16, sVisitedFeecoEnder_5C1C06, 0);

ALIVE_VAR(1, 0x550f5c, unsigned __int16, sTeleport_Level_550F5C, 0);
ALIVE_VAR(1, 0x550f5e, unsigned __int16, sTeleport_Path_550F5E, 0);
ALIVE_VAR(1, 0x550f60, unsigned __int16, sTeleport_Cam_550F60, 0);

ALIVE_VAR(1, 0x5c2c08, bool, sDDCheat_FlyingEnabled_5C2C08, false);
ALIVE_VAR(1, 0x5c1bd8, bool, sDDCheat_ShowAI_Info_5C1BD8, false);
ALIVE_VAR(1, 0x5bc000, bool, sDDCheat_AlwaysShow_5BC000, false);
ALIVE_VAR(1, 0x5bc004, int, sDDCheat_Unknown_5BC004, 0);
ALIVE_VAR(1, 0x5bbff4, DWORD, sDDCheat_PrevDebugInput_5BBFF4, 0);
ALIVE_VAR(1, 0x550fa8, int, sDDCheat_DebugInputDelay_550FA8, 0);

using TDDCheatMenu = decltype(&DDCheat::Menu_Teleport_415E20);

#define DDCHEAT_MENU_COUNT 2
ALIVE_ARY(1, 0x550f50, TDDCheatMenu, DDCHEAT_MENU_COUNT, sDDCheat_FnTable_550F50, {
    &DDCheat::Menu_Teleport_415E20,
    &DDCheat::Menu_Movies_416000,
});

ALIVE_VAR(1, 0x5bc008, __int16, sScreenshotOnNextFrame_5BC008, 0);
ALIVE_VAR(1, 0xab49fc, int, sDDCheat_Unused2_AB49FC, 0);
ALIVE_VAR(1, 0xab4a00, int, sDDCheat_Unused1_AB4A00, 0);

ALIVE_VAR(1, 0x5BBFF0, __int16, sDDCheat_MovieSelectIdx_5BBFF0, 0);
ALIVE_VAR_EXTERN(DWORD, sLevelId_dword_5CA408);

EXPORT void DDCheat_SaveScreenshot_415550() { NOT_IMPLEMENTED(); }

#define DDCHEAT_PROPERTY_LIST_SIZE 10
ALIVE_ARY(1, 0x5BBF78, DDCheatProperty, DDCHEAT_PROPERTY_LIST_SIZE, DDCheatProperties_5BBF78, {});

ALIVE_ARY(1, 0x550f64, const char*, 17, sTeleportLevelNameTable_550F64,{
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
});


void DDCheat::Menu_Teleport_415E20()
{
    DebugStr_4F5560("\n[Teleport]\n");
    DebugStr_4F5560("Level (1,2):         %s\n", sTeleportLevelNameTable_550F64[sTeleport_Level_550F5C]);
    DebugStr_4F5560("Path    (Up/Down):   %d\n", sTeleport_Path_550F5E);
    DebugStr_4F5560("Camera (Left/Right): %d\n", sTeleport_Cam_550F60);
    DebugStr_4F5560("Teleport = Enter Reset = Alt\n");

    field_20 += 6;

    if (field_38_input_pressed & InputCommands::eGameSpeak1)
    {
        if (sTeleport_Level_550F5C)
            --sTeleport_Level_550F5C;
    }
    else if (field_38_input_pressed & InputCommands::eGameSpeak2)
    {
        if (sTeleport_Level_550F5C < static_cast<int>(LevelIds::eCredits_16))
        {
            ++sTeleport_Level_550F5C;
        }
    }
    else if (field_38_input_pressed & InputCommands::eUp)
    {
        ++sTeleport_Path_550F5E;
    }
    else if (field_38_input_pressed & InputCommands::eDown)
    {
        if (sTeleport_Path_550F5E > 1)
        {
            --sTeleport_Path_550F5E;
        }
    }
    else if (field_38_input_pressed & InputCommands::eLeft)
    {
        if (sTeleport_Cam_550F60 > 1)
        {
            --sTeleport_Cam_550F60;
        }
    }
    else if (field_38_input_pressed & InputCommands::eRight)
    {
        ++sTeleport_Cam_550F60;
    }
    else if (field_38_input_pressed & InputCommands::eSneak)
    {
        sTeleport_Level_550F5C = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
        sTeleport_Path_550F5E = gMap_5C3030.sCurrentPathId_5C3032;
        sTeleport_Cam_550F60 = gMap_5C3030.sCurrentCamId_5C3034;
    }
    else if (field_38_input_pressed & InputCommands::eUnPause_OrConfirm)
    {
        sDDCheat_FlyingEnabled_5C2C08 = true;
        
        gMap_5C3030.SetActiveCam_480D30(static_cast<LevelIds>(sTeleport_Level_550F5C), sTeleport_Path_550F5E, sTeleport_Cam_550F60, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        field_3C_flags |= 0x4u;
    }
}

void DDCheat::Menu_Movies_416000()
{
    if (field_38_input_pressed & InputCommands::eLeft)
    {
        sDDCheat_MovieSelectIdx_5BBFF0--;
    }
    else if (field_38_input_pressed & InputCommands::eRight)
    {
        sDDCheat_MovieSelectIdx_5BBFF0++;
    }

    if (Path_Get_FMV_Record_460F70(gMap_5C3030.sCurrentLevelId_5C3030, sDDCheat_MovieSelectIdx_5BBFF0)->field_4_id <= 0 )
    {
        sDDCheat_MovieSelectIdx_5BBFF0 = 1;
    }

    if (field_38_input_pressed & InputCommands::eDown)
    {
        auto movieId = Path_Get_FMV_Record_460F70(gMap_5C3030.sCurrentLevelId_5C3030, sDDCheat_MovieSelectIdx_5BBFF0)->field_4_id;
        movieId--;
    }
    if (field_38_input_pressed & InputCommands::eUp)
    {
        FmvInfo *movieToPlayInfo = Path_Get_FMV_Record_460F70(gMap_5C3030.sCurrentLevelId_5C3030, sDDCheat_MovieSelectIdx_5BBFF0);
        DWORD pos;
        Get_fmvs_sectors_494460(movieToPlayInfo->field_0_pName, 0, 0, &pos, 0, 0);
        sLevelId_dword_5CA408 = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
        auto movieToPlay = alive_new<Movie>();
        if (movieToPlay != nullptr)
        {
            movieToPlay->ctor_4DFDE0(
                movieToPlayInfo->field_4_id,
                pos,
                movieToPlayInfo->field_6_flags & 1,
                static_cast<unsigned __int16>(movieToPlayInfo->field_8),
                movieToPlayInfo->field_A_volume
            );
        }
    }
    FmvInfo* fmvInfo = &sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_4_pFmvArray[sDDCheat_MovieSelectIdx_5BBFF0];
    DDCheat::DebugStr_4F5560("\n<- Movie -> %d %d %s \n", sDDCheat_MovieSelectIdx_5BBFF0, fmvInfo->field_4_id, fmvInfo->field_0_pName);
    field_20 += 6;
}

DDCheat::DDCheat()
{

}

DDCheat* DDCheat::ctor_4153C0()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x544518);
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset);
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);
    field_3C_flags &= 0xFFF8u;
    field_4_typeId = Types::eDDCheat_19;
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

#if FORCE_DDCHEAT
    sCommandLine_DDCheatEnabled_5CA4B5 = true;
#endif

    return this;
}


void DDCheat::dtor_415530()
{
    BaseGameObject_dtor_4DBEC0();
}


BaseGameObject* DDCheat::vdtor_415500(signed int flags)
{
    dtor_415530();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void DDCheat::AddPropertyEntry_004162C0(const char * text, int unknown, int * valuePtr)
{
    for (int i = 0; i < DDCHEAT_PROPERTY_LIST_SIZE; i++)
    {
        if (DDCheatProperties_5BBF78[i].Name == nullptr)
        {
            DDCheatProperties_5BBF78[i].Name = text;
            DDCheatProperties_5BBF78[i].Unknown = unknown; // property type ?
            DDCheatProperties_5BBF78[i].ValuePtr = valuePtr;
            break;
        }
    }
}

void CC DDCheat::ClearProperties_415390()
{
    for (int i = 0; i < DDCHEAT_PROPERTY_LIST_SIZE; i++)
    {
        DDCheatProperties_5BBF78[i].Name = nullptr;
    }
}

void DDCheat::DebugStr_4F5560(const char* pFormatStr, ...)
{
    char buffer[1024] = {};
    va_list va;

    va_start(va, pFormatStr);
    vsprintf(buffer, pFormatStr, va);
    DebugFont_Printf_4F8B60(0, buffer);
}

void DDCheat::Update_415780()
{
    if (sScreenshotOnNextFrame_5BC008)
    {
        DDCheat_SaveScreenshot_415550();
    }

    field_20 = 0;

    auto activePadPressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (sCommandLine_DDCheatEnabled_5CA4B5)
    {
        if (field_3C_flags & 4) // On Teleport?
        {
            field_3C_flags &= ~0x4;
            if (sActiveHero_5C1B68)
            {
                PSX_Point pos;
                gMap_5C3030.GetCurrentCamCoords_480680(&pos);
                sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(pos.field_0_x + 184);
                sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(pos.field_2_y + 60);
                sActiveHero_5C1B68->field_106_current_motion = 3;
                sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_Bit7);
                sActiveHero_5C1B68->field_C2_lvl_number = static_cast<LevelIds>(sTeleport_Level_550F5C);
                sActiveHero_5C1B68->field_C0_path_number = sTeleport_Path_550F5E;
                sDDCheat_FlyingEnabled_5C2C08 = false;
                sControlledCharacter_5C1B8C->field_100_pCollisionLine = nullptr;
                sControlledCharacter_5C1B8C->field_F8_LastLineYPos = sControlledCharacter_5C1B8C->field_BC_ypos;
                field_3C_flags &= ~0x1;
            }
        }

        if ((gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eMenu_0 && gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eNone) && sActiveHero_5C1B68 && activePadPressed & InputCommands::eCheatMode)
        {
            sDDCheat_FlyingEnabled_5C2C08 = !sDDCheat_FlyingEnabled_5C2C08;
            if (!sDDCheat_FlyingEnabled_5C2C08)
            {
                if (sControlledCharacter_5C1B8C == sActiveHero_5C1B68)
                {
                    sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_Bit7);
                }
                sControlledCharacter_5C1B8C->field_100_pCollisionLine = nullptr;
                sControlledCharacter_5C1B8C->field_F8_LastLineYPos = sControlledCharacter_5C1B8C->field_BC_ypos;
            }

            sDDCheat_ShowAI_Info_5C1BD8 = false;

            switch (sControlledCharacter_5C1B8C->field_4_typeId)
            {
            case Types::eGlukkon_67:
            case Types::eSlig_125:
                sControlledCharacter_5C1B8C->field_106_current_motion = 7;
                break;
            case Types::eAbe_69:
                sControlledCharacter_5C1B8C->field_106_current_motion = eAbeStates::State_3_Fall_459B60;
                break;
            case Types::eScrab_112:
                sControlledCharacter_5C1B8C->field_106_current_motion = 8;
                break;
            default:
                break;
            }
        }

        // Unused
        //if (!(sGnFrame_5C1B84 % 10))
        //{
        //    sDDCheat_Unknown_5BC004 = 0;
        //    for (int i = 0; i < 24; i++)
        //    {
        //        if (sub_4FA7C0()) // Always returns 0
        //            ++sDDCheat_Unknown_5BC004;
        //    }
        //}

        if (sDDCheat_FlyingEnabled_5C2C08 || sDDCheat_ShowAI_Info_5C1BD8 || sDDCheat_AlwaysShow_5BC000)
        {
            DebugStr_4F5560(
                "\n%sP%dC%d gnframe=%5d",
                sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_18_lvl_name,
                gMap_5C3030.sCurrentPathId_5C3032,
                gMap_5C3030.sCurrentCamId_5C3034,
                sGnFrame_5C1B84);

            

#if DEVELOPER_MODE
            if (sActiveHero_5C1B68 && gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eMenu_0)
            {
                // HACK When quitting sControlledCharacter_5C1B8C becomes a dangling pointer
                // probably this should be removed as there is no sane way to check this pointer is still valid
                DebugStr_4F5560(
                    "\n[obj %i] xy=%.3f,%.3f flags=%x",
                    sControlledCharacter_5C1B8C->field_4_typeId,
                    FP_GetDouble(sControlledCharacter_5C1B8C->field_B8_xpos),
                    FP_GetDouble(sControlledCharacter_5C1B8C->field_BC_ypos),
                    sControlledCharacter_5C1B8C->field_6_flags);

                DebugStr_4F5560("\nLine=%X\nState=%i", sControlledCharacter_5C1B8C->field_100_pCollisionLine, sControlledCharacter_5C1B8C->field_106_current_motion);

                if (sControlledCharacter_5C1B8C->field_4_typeId == Types::eAbe_69)
                {
                    DebugStr_4F5560("\nStateName=%s", sAbeStateNames[sControlledCharacter_5C1B8C->field_106_current_motion]);
                }
            }
#else
            DebugStr_4F5560(
                "\nheroxy=%4d,%4d",
                FP_GetExponent(sActiveHero_5C1B68->field_B8_xpos),
                FP_GetExponent(sActiveHero_5C1B68->field_BC_ypos));
#endif

            field_20 = 6;

            if (sDDCheat_FlyingEnabled_5C2C08)
            {
                if (activePadPressed & InputCommands::eDoAction)
                {
                    sDDCheat_ShowAI_Info_5C1BD8 = !sDDCheat_ShowAI_Info_5C1BD8;
                }

                if ((activePadPressed & InputCommands::eThrowItem) != 0)
                {
                    sPeakedManagedMemUsage_AB4A08 = sManagedMemoryUsedSize_AB4A04;
                    sDDCheat_Unused1_AB4A00 = sDDCheat_Unused2_AB49FC;
                }

                if (activePadPressed & InputCommands::eHop)
                {
                    sDDCheat_AlwaysShow_5BC000 = !sDDCheat_AlwaysShow_5BC000;
                }

                if (sControlledCharacter_5C1B8C == sActiveHero_5C1B68)
                {
                    sActiveHero_5C1B68->field_1AC_flags.Set(Abe::e1AC_Bit7);
                }

                sControlledCharacter_5C1B8C->field_100_pCollisionLine = nullptr;
                sControlledCharacter_5C1B8C->field_F8_LastLineYPos = sControlledCharacter_5C1B8C->field_BC_ypos;
            }

            /*DebugStr_4F5560("\n[Memory]");
            DebugStr_4F5560("\nUsed: %ikb", sManagedMemoryUsedSize_AB4A04 / 1024);
            DebugStr_4F5560("\nPeak: %ikb", sPeakedManagedMemUsage_AB4A08 / 1024);*/
        }

        field_38_input_pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE == 0].field_C_held;
        if (sDDCheat_PrevDebugInput_5BBFF4 == sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE == 0].field_0_pressed
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
            sDDCheat_PrevDebugInput_5BBFF4 = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE == 0].field_0_pressed;
            sDDCheat_DebugInputDelay_550FA8 = 10;
        }

        if (field_38_input_pressed & InputCommands::ePause)
        {
            field_3C_flags ^= ((unsigned __int8)field_3C_flags ^ (unsigned __int8)~(unsigned __int8)field_3C_flags) & 1;
        }

        if (field_3C_flags & 1)
        {
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE == 0].field_0_pressed & InputCommands::eCheatMode)
            {
                field_24_fn_idx = 0;
            }
            else if (field_38_input_pressed & InputCommands::eCheatMode)
            {
                field_3C_flags = field_3C_flags ^ ((unsigned __int8)field_3C_flags ^ (unsigned __int8)~(BYTE)field_3C_flags) & 2;
                field_26_next_fn_idx = field_24_fn_idx;
            }

            // Using hop instead looks like the only way to actually change the menu properly
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE == 0].field_0_pressed & InputCommands::eHop /*field_3C_flags & 2*/)
            {
                if (field_38_input_pressed & InputCommands::eDown)
                {
                    field_26_next_fn_idx++;
                }
                else if (field_38_input_pressed & InputCommands::eUp)
                {
                    field_26_next_fn_idx--;
                }

                if (field_38_input_pressed & InputCommands::eUnPause_OrConfirm)
                {
                    //field_24_fn_idx = field_26_next_fn_idx;
                    field_3C_flags &= ~0x2;
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

        if (field_20)
        {
            pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
        }
    }
}

BaseGameObject* DDCheat::VDestructor(signed int flags)
{
    return vdtor_415500(flags);
}

void DDCheat::VUpdate()
{
    Update_415780();
}

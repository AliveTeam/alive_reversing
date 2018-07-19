#include "stdafx.h"
#include "MainMenu.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0xbb4400, int, sMainMenuObjectCounter_BB4400, 0);
ALIVE_VAR(1, 0x5c1b92, __int16, sMainMenuFontLoaded_5C1B92, 0);

unsigned char pal_560F80[] =
{
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x94, 0xE7, 0x9C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E
};

ALIVE_VAR(1, 0x5c1bee, char, sEnableCheatLevelSelect_5C1BEE, 0);

ALIVE_VAR_EXTERN(__int16, sRescuedMudokons_5C1BC2);
ALIVE_VAR_EXTERN(__int16, sKilledMudokons_5C1BC0);

ALIVE_VAR(1, 0x5C1BEC, short, word_5C1BEC, {});
ALIVE_VAR(1, 0x5C1BC6, short, word_5C1BC6, {});
ALIVE_VAR(1, 0x5C1BC8, short, word_5C1BC8, {});
ALIVE_VAR(1, 0x5c1b9e, __int16, word_5C1B9E, 0);
ALIVE_VAR(1, 0x5C1B64, byte, byte_5C1B64, {});

ALIVE_VAR(1, 0x561538, short, sMenuItemCount_561538, 0);
ALIVE_ARY(1, 0x5C1B50, BYTE, 20, sSavedKilledMudsPerPath_5C1B50, {});
ALIVE_VAR(1, 0xbb4414, void *, pDemosOrFmvs_BB4414, 0);

MenuFMV sFmvs_561540[28] =
{
    { "GT Logo", 0, 65535, 65535, 3, 65535, 65535 },
    { "Oddworld Intro", 0, 65535, 65535, 1, 65535, 65535 },
    { "Abe's Exoddus", 0, 65535, 65535, 5, 65535, 65535 },
    { "Backstory", 0, 65535, 65535, 4, 65535, 65535 },
    { "Prophecy", 1, 65535, 65535, 1, 65535, 65535 },
    { "Vision", 1, 65535, 65535, 24, 65535, 65535 },
    { "Game Opening", 1, 65535, 65535, 2, 65535, 65535 },
    { "Brew", 1, 65535, 65535, 26, 65535, 65535 },
    { "Brew Transition", 1, 65535, 65535, 31, 65535, 65535 },
    { "Escape", 1, 65535, 65535, 25, 65535, 65535 },
    { "Reward", 2, 65535, 65535, 22, 65535, 65535 },
    { "FeeCo", 5, 65535, 65535, 4, 65535, 65535 },
    { "Information Booth", 5, 65535, 65535, 3, 65535, 65535 },
    { "Train 1", 6, 65535, 65535, 5, 65535, 65535 },
    { "Train 2", 9, 65535, 65535, 15, 65535, 65535 },
    { "Train 3", 8, 65535, 65535, 6, 65535, 65535 },
    { "Aslik Info", 5, 65535, 65535, 2, 65535, 65535 },
    { "Aslik Explodes", 5, 65535, 65535, 1, 65535, 65535 },
    { "Dripek Info", 6, 65535, 65535, 4, 65535, 65535 },
    { "Dripek Explodes", 6, 65535, 65535, 3, 65535, 65535 },
    { "Phleg Info", 8, 65535, 65535, 4, 65535, 65535 },
    { "Phleg Explodes", 8, 65535, 65535, 5, 65535, 65535 },
    { "Soulstorm Info", 9, 65535, 65535, 14, 65535, 65535 },
    { "Ingredient", 9, 65535, 65535, 16, 65535, 65535 },
    { "Conference", 9, 65535, 65535, 13, 65535, 65535 },
    { "Happy Ending", 9, 65535, 65535, 17, 65535, 65535 },
    { "Sad Ending", 9, 65535, 65535, 18, 65535, 65535 },
    { "Credits", 16, 65535, 65535, 65535, 65535, 65535 }
};


PerLvlData sDemos_5617F0[23] =
{
    { "Mudokons 1", 1, 8, 5, 0, 0, 0 },
    { "Mudokons 2", 1, 8, 32, 1, 0, 0 },
    { "Mudokons 3", 1, 8, 21, 2, 0, 0 },
    { "Flying Slig", 1, 9, 18, 4, 0, 0 },
    { "Blind Mudokons 1", 1, 11, 27, 5, 0, 0 },
    { "Blind Mudokons 2", 1, 11, 22, 3, 0, 0 },
    { "Minecar", 1, 12, 2, 6, 0, 0 },
    { "Fleeches", 2, 7, 1, 7, 0, 0 },
    { "Paramite Chase", 3, 2, 13, 8, 0, 0 },
    { "Paramites Talk", 3, 6, 8, 27, 0, 0 },
    { "Scrab and Fleeches", 4, 12, 2, 9, 0, 0 },
    { "Invisibility", 4, 13, 5, 10, 0, 0 },
    { "Farts-a-poppin'", 5, 6, 3, 12, 0, 0 },
    { "Flying Sligs 2", 5, 12, 1, 14, 0, 0 },
    { "Baggage Claim", 12, 13, 1, 15, 0, 0 },
    { "Shrykull", 6, 3, 10, 16, 0, 0 },
    { "Crawling Sligs", 6, 4, 6, 17, 0, 0 },
    { "Slogs Attack", 8, 11, 7, 18, 0, 0 },
    { "Glukkon", 14, 13, 9, 19, 0, 0 },
    { "Angry Mudokons", 9, 13, 10, 22, 0, 0 },
    { "Sligs", 9, 26, 4, 23, 0, 0 },
    { "Tortured Mudokons", 9, 27, 7, 24, 0, 0 },
    { "Greeters Go Boom", 9, 28, 4, 25, 0, 0 }
};

MainMenuController * MainMenuController::ctor_4CE9A0(int a2, int a3)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x547958);
    SetVTable(&field_158_animation, 0x544290);

    ++sMainMenuObjectCounter_BB4400;
    field_6_flags |= 0x200u;

    if (!sMainMenuFontLoaded_5C1B92)
    {
        sFont1Context_5BC5C8.LoadFontType_433400(1);
        sMainMenuFontLoaded_5C1B92 = 1;
    }

    field_120_font.ctor_433590(240, pal_560F80, &sFont1Context_5BC5C8);
    memset(&field_F4_resources, 0, sizeof(field_F4_resources));

    field_F4_resources.field_4_res_abespek2 = reinterpret_cast<AnimHeader *>(ResourceManager::GetLoadedResource_49C2A0(
        ResourceManager::Resource_Animation, kAbespek2ResID, 1u, 0));

    field_F4_resources.field_10_res_optflare = reinterpret_cast<AnimHeader *>(ResourceManager::GetLoadedResource_49C2A0(
        ResourceManager::Resource_Animation, kOptflareResID, 1u, 0));

    field_F4_resources.field_14 = nullptr;
    field_F4_resources.field_0_res_abespeak = nullptr;

    if (gMap_5C3030.sCurrentCamId_5C3034 == 6)
    {
        field_F4_resources.field_8_res_abeintro = nullptr;
        field_F4_resources.field_C_res_door = nullptr;
        Animation_Init_424E10(247808, 233, 0x7Du, field_F4_resources.field_4_res_abespek2, 1, 1u);
    }
    else
    {
        ResourceManager::LoadResourceFile_49C170("ABEINTRO.BAN", 0);
        field_F4_resources.field_8_res_abeintro = reinterpret_cast<AnimHeader *>(ResourceManager::GetLoadedResource_49C2A0(
            ResourceManager::Resource_Animation, kAbeintroResID, 1u, 0));
        ResourceManager::LoadResourceFile_49C170("STDOOR.BAN", 0);
        field_F4_resources.field_C_res_door = reinterpret_cast<AnimHeader *>(ResourceManager::GetLoadedResource_49C2A0(
            ResourceManager::Resource_Animation, kDoorResID, 1u, 0));
        Animation_Init_424E10(50212, 233, 0x7Du, field_F4_resources.field_C_res_door, 1, 1u);
    }

    field_20_animation.field_A_b = 127;
    field_20_animation.field_9_g = 127;
    field_20_animation.field_8_r = 127;

    field_F4_resources.field_18_res_highlite = reinterpret_cast<AnimHeader *>(ResourceManager::GetLoadedResource_49C2A0(
        ResourceManager::Resource_Animation, kHighliteResID, 1u, 0));
    field_158_animation.Init_40A030(13912, gObjList_animations_5C1A24, this, 150, 0x41u, reinterpret_cast<BYTE**>(field_F4_resources.field_18_res_highlite), 1u, 0, 0);

    field_158_animation.field_14_scale = field_CC_sprite_scale;
    field_158_animation.field_4_flags &= 0x3FFF;
    field_158_animation.field_4_flags |= 0x4000;
    field_158_animation.field_C_render_layer = 38;
    field_158_animation.field_B_render_mode = 1;

    field_1FE = 40;
    field_200 = -8;
    field_1F0 = a3;

    field_23C_T80 &= 0xFF1C0000u;
    field_214_page_index = MainMenuController::GetPageIndexFromCam_4D05A0(gMap_5C3030.sCurrentCamId_5C3034);
    field_21C = 1;
    field_21E = 0;
    field_1F8 = 0;
    field_220 = 1;
    field_228 = 0;
    field_22A = 0;
    field_22C_T80 = 0;
    field_224 = 0;
    field_202 = 15;
    field_204 = 0;
    field_230 = 0; // Double check

    if (gMap_5C3030.sCurrentCamId_5C3034 == 1)
    {
        MainMenuController::sub_4D05E0(9, 0);
        field_23C_T80 |= 0x100u;
    }

    field_23C_T80 &= 0xFEEB0000u;

    field_1FC_button_index = 0;
    field_208 = 0;
    field_20C = 0;
    field_210 = 0;
    field_23A = 0;
    field_234 = (char *)0x5C2F68;
    field_238 = 0;
    field_25C = 0;
    field_25E = 0;

    memset(sSavedKilledMudsPerPath_5C1B50, 0, 20);

    word_5C1BEC = 0;
    sEnableCheatLevelSelect_5C1BEE = 0;
    sKilledMudokons_5C1BC0 = 0;
    sRescuedMudokons_5C1BC2 = 0;
    word_5C1BA0 = 0;
    byte_5C1B64 = 0;
    word_5C1BC6 = 0;
    word_5C1BC8 = 0;

    if (gMap_5C3030.sCurrentCamId_5C3034 == 6)
    {
        field_1FC_button_index = 0;
        field_250 = 0;
        field_254 = 0;
        field_258 = 0;
        field_25C = 1;
        pDemosOrFmvs_BB4414 = &sFmvs_561540;
        sMenuItemCount_561538 = 28;
        field_20_animation.Set_Animation_Data_409C80(247808, (BYTE **)field_F4_resources.field_4_res_abespek2);
        sub_4D06A0(&field_20_animation);
        return this;
    }
    if (gMap_5C3030.sCurrentCamId_5C3034 == 30)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbespeakResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C130("ABESPEAK.BAN", reinterpret_cast<ResourceManager::TLoaderFn>(callback_4D06E0), reinterpret_cast<Camera *>(this), 0);
        }
        
        pResourceManager_5C1BB0->LoadingLoop_465590(false);
        field_1FC_button_index = 0;
        field_250 = word_5C1B9E;
        field_254 = 0;
        field_258 = 0;
        pDemosOrFmvs_BB4414 = &sDemos_5617F0;
        sMenuItemCount_561538 = 23;
        field_230 = word_5C1B9E;
        field_20_animation.Set_Animation_Data_409C80(247808, (BYTE **)field_F4_resources.field_4_res_abespek2);
        sub_4D06A0(&field_20_animation);
        return this;
    }

    return this;
}

int MainMenuController::GetPageIndexFromCam_4D05A0(int camId)
{
    NOT_IMPLEMENTED();
    return 0;
}

void MainMenuController::sub_4D05E0(__int16 a2, __int16 a3)
{
    NOT_IMPLEMENTED();
}

void MainMenuController::sub_4D06A0(AnimationEx * a3)
{
    NOT_IMPLEMENTED();
}

void MainMenuController::callback_4D06E0(MainMenuController * a1)
{
    NOT_IMPLEMENTED();
}

void MainMenu_ForceLink()
{
}

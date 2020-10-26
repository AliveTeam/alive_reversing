#include "stdafx_ao.h"
#include "Function.hpp"
#include "MainMenu.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "DDCheat.hpp"
#include "CheatController.hpp"
#include "LCDScreen.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "CameraSwapper.hpp"
#include "PauseMenu.hpp"
#include "DemoPlayback.hpp"
#include "Grenade.hpp"

START_NS_AO

const int dword_4BB1B8[4] = { 6152, 6140, 6164, 0 };


struct Menu_Button
{
    __int16 field_0_xpos;
    __int16 field_2_ypos;
    int field_4_frame_table;
};

const Menu_Button sMainScreenButtons_4D00B0[5] =
{
    { 33, 64, 6152 },
    { 33, 85, 6152 },
    { 33, 107, 6152 },
    { 335, 215, 6152 },
    { 335, 240, 6152 }
};

const Menu_Button stru_4D0148[3] ={ { 33, 66, 6152 }, { 33, 87, 6152 }, { 288, 238, 6152 } };

const Menu_Button stru_4D01C0[3] = { { 33, 66, 6152 }, { 33, 87, 6152 }, { 289, 238, 6152 } };

ALIVE_VAR(1, 0x507694, short, gDemoPlay_507694, 0);
ALIVE_VAR(1, 0x50769C, BYTE, sJoyResId_50769C, 0);

ALIVE_VAR(1, 0x9F2DE0, int, gMainMenuInstanceCount_9F2DE0, 0);
ALIVE_VAR(1, 0x507688, short, sFontLoaded_507688, 0);
ALIVE_VAR(1, 0x4D0228, short, sListCount_4D0228, -1);

ALIVE_VAR(1, 0x4CE598, int, dword_4CE598, 0);
ALIVE_VAR(1, 0x5079A4, int, dword_5079A4, 0);

ALIVE_VAR(1, 0x507690, short, sSoundMono_507690, 0);


struct MenuFMV;

ALIVE_VAR(1, 0x9F2DE4, const MenuFMV*, sActiveList_9F2DE4, nullptr);

const BYTE byte_4D0090[32] =
{
    0u,
    0u,
    33u,
    132u,
    66u,
    136u,
    99u,
    140u,
    132u,
    144u,
    165u,
    148u,
    231u,
    156u,
    8u,
    33u,
    41u,
    37u,
    74u,
    41u,
    107u,
    45u,
    140u,
    49u,
    173u,
    53u,
    239u,
    61u,
    16u,
    66u,
    115u,
    78u
};

struct MenuFMV
{
    const char* field_0_name;
    LevelIds field_4_level_id;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A_fmv_id;
    __int16 field_C;
    __int16 field_E;
};

const MenuFMV gFmvs_4D0230[13] =
{
    { "Oddworld Intro", LevelIds::eMenu_0, -1, -1, 1, -1, -1 },
    { "This is RuptureFarms", LevelIds::eRuptureFarms_1, -1, -1, 1, -1, -1 },
    { "Barrel Ride", LevelIds::eStockYards_5, -1, -1, 4, -1, -1 },
    { "Abe's Moon", LevelIds::eStockYards_5, -1, -1, 2, -1, -1 },
    { "Paramite Scar", LevelIds::eLines_2, -1, -1, 3, -1, -1 },
    { "Scrab Scar", LevelIds::eLines_2, -1, -1, 4, -1, -1 },
    { "Shrykull Revealed", LevelIds::eLines_2, -1, -1, 16, -1, -1 },
    { "Mullock Watches", LevelIds::eRuptureFarmsReturn_13, -1, -1, 2, -1, -1 },
    { "The Factory Halts", LevelIds::eRuptureFarmsReturn_13, -1, -1, 1, -1, -1 },
    { "What a Drag", LevelIds::eRuptureFarmsReturn_13, -1, -1, 3, -1, -1 },
    { "Bad Ending", LevelIds::eRuptureFarmsReturn_13, -1, -1, 4, -1, -1 },
    { "Good Ending", LevelIds::eRuptureFarmsReturn_13, -1, -1, 16, -1, -1 },
    { "Credits", LevelIds::eCredits_10, -1, -1, -1, -1, -1 }
};

struct MenuLevel
{
    const char* field_0_name;
    LevelIds field_4_level_id;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E;
};

// TODO: Should be MenuFMV or there is a generic structure for "lists" of things?
const MenuFMV sLevelList_4D0300[15] =
{
    { "RuptureFarms", LevelIds::eRuptureFarms_1, 15, 1, -1, 1390, 245 },
    { "Stockyard Escape", LevelIds::eStockYards_5, 6, 6, -1, 5656, 133 },
    { "Monsaic Lines", LevelIds::eLines_2, 1, 14, -1, 11810, 681 },
    { "Paramonia", LevelIds::eForest_3, 1, 1, -1, 330, 660 },
    { "Paramonian Temple", LevelIds::eForestTemple_4, 1, 1, -1, 565, 170 },
    { "Paramonian Nests", LevelIds::eForestChase, 9, 1, -1, 2439, 2621 },
    { "Scrabania", LevelIds::eDesert_8, 1, 1, -1, 4677, 750 },
    { "Scrabanian Temple", LevelIds::eDesertTemple_9, 1, 1, -1, 4410, 203 },
    { "Scrabanian Nests", LevelIds::eDesertEscape, 11, 1, -1, 466, 2124 },
    { "Stockyards", LevelIds::eStockYardsReturn_6, 4, 7, -1, 540, 640 },
    { "Rescue Zulag 1", LevelIds::eRuptureFarmsReturn_13, 19, 3, -1, 2589, 193 },
    { "Rescue Zulag 2", LevelIds::eRuptureFarmsReturn_13, 1, 1, -1, 1514, 705 },
    { "Rescue Zulag 3", LevelIds::eRuptureFarmsReturn_13, 13, 1, -1, 1389, 694 },
    { "Rescue Zulag 4", LevelIds::eRuptureFarmsReturn_13, 14, 1, -1, 1390, 700 },
    { "The Boardroom", LevelIds::eBoardRoom_12, 6, 1, -1, 592, 157 }
};

MainMenuFade* MainMenuFade::ctor_42A5A0(__int16 xpos, __int16 ypos, unsigned __int16 idx_1, __int16 bDestroyOnDone)
{
    ctor_417C10();
    SetVTable(this, 0x4BB1C8);

    field_4_typeId = Types::MainMenuFade_44;

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8002, 1, 0);
    Animation_Init_417FD0(
        dword_4BB1B8[idx_1],
        99,
        43,
        ppRes,
        1);

    field_10_anim.field_B_render_mode = 1;
    field_A8_xpos = FP_FromInteger(xpos);
    field_AC_ypos = FP_FromInteger(ypos);
    field_E8_bDestroyOnDone = bDestroyOnDone;
    field_E4 = 40;
    field_E6 = 8;

    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }
        if (pObj->field_4_typeId == Types::MainMenuFade_44 &&
            static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj)->field_A8_xpos == field_A8_xpos &&
            static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj)->field_AC_ypos == field_AC_ypos)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    return this;
}



void MainMenuFade::VUpdate()
{
    VUpdate_42A730();
}

void MainMenuFade::VUpdate_42A730()
{
    field_E4 += field_E6;

    if (field_E4 < 40)
    {
        field_E4 += -field_E6;
        field_E6 = -field_E6;

        if (field_E8_bDestroyOnDone)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    if (field_E4 > 127)
    {
        field_E4 -= field_E6;
        field_E6 = -field_E6;
    }

    field_10_anim.field_A_b = static_cast<BYTE>(field_E4);
    field_10_anim.field_9_g = static_cast<BYTE>(field_E4);
    field_10_anim.field_8_r = static_cast<BYTE>(field_E4);
}

void MainMenuFade::VRender(int** pOrderingTable)
{
    VRender_42A7A0(pOrderingTable);
}

void MainMenuFade::VRender_42A7A0(int** ppOt)
{
    field_10_anim.vRender(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        ppOt,
        0,
        0);
    
    PSX_RECT rect = {};
    field_10_anim.Get_Frame_Rect_402B50(&rect);
    pScreenManager_4FF7C8->InvalidateRect_406E40(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_4FF7C8->field_2E_idx);
}

BaseGameObject* MainMenuTransition::VDestructor(signed int flags)
{
    return Vdtor_4369F0(flags);
}

void MainMenuTransition::VScreenChanged_4369D0()
{
    if (gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void MainMenuTransition::VScreenChanged()
{
    VScreenChanged_4369D0();
}

void MainMenuTransition::VUpdate_4365C0()
{
    if (!field_16_bDone && !field_1A)
    {
        field_10_current_Value += field_12_change_by_speed;
        if (field_14_fade_direction)
        {
            if (field_10_current_Value > 255)
            {
                field_10_current_Value = 255;
                field_246_colour_fade_value--;
                return;
            }
        }
        else if (field_10_current_Value < 0)
        {
            field_10_current_Value = 0;
        }
        field_246_colour_fade_value--;
    }
}

void MainMenuTransition::VUpdate()
{
    VUpdate_4365C0();
}

MainMenuTransition* MainMenuTransition::ctor_436370(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 speed, char abr)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB7A0);
    field_4_typeId = Types::eDeathFadeOut_80;

    gObjList_drawables_504618->Push_Back(this);
   
    field_6_flags.Set(Options::eDrawable_Bit4);

    Init_SetTPage_495FB0(&field_21C_tPage[0], 0, 1, PSX_getTPage_4965D0(2, abr, 0, 0));
    Init_SetTPage_495FB0(&field_21C_tPage[1], 0, 1, PSX_getTPage_4965D0(2, abr, 0, 0));

    for (int i = 0; i < 8; i++)
    {
        Prim_Init_Poly_G3_498870(&field_1C_polys[0].field_0_polys[i]);
        Poly_Set_SemiTrans_498A40(&field_1C_polys[0].field_0_polys[i].mBase.header, 1);

        Prim_Init_Poly_G3_498870(&field_1C_polys[1].field_0_polys[i]);
        Poly_Set_SemiTrans_498A40(&field_1C_polys[1].field_0_polys[i].mBase.header, 1);
    }

    field_23C_layer = layer;

    if (fadeDirection)
    {
        field_10_current_Value = 0;
    }
    else
    {
        field_10_current_Value = 255;
    }

    field_242 = 0;
    field_246_colour_fade_value = 0;
    field_23E_width = 320;
    field_240_k120 = 120;
    StartTrans_436560(layer, fadeDirection, bKillWhenDone, speed);

    return this;
}

BaseGameObject* MainMenuTransition::dtor_436500()
{
    SetVTable(this, 0x4BB7A0);
    gObjList_drawables_504618->Remove_Item(this);
    return dtor_487DF0();
}

MainMenuTransition* MainMenuTransition::Vdtor_4369F0(signed int flags)
{
    dtor_436500();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void MainMenuTransition::StartTrans_436560(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 speed)
{
    field_23C_layer = layer;
    field_14_fade_direction = fadeDirection;
    field_16_bDone = 0;

    if (2 * speed)
    {
        field_1A = 0;
    }
    else
    {
        field_1A = 1;
    }

    field_18_bKillOnDone = bKillWhenDone;

    if (!fadeDirection)
    {
        field_12_change_by_speed = -2 * speed;
    }
    else
    {
        field_12_change_by_speed = 2 * speed;

    }

    if (fadeDirection)
    {
        SFX_Play_43AD70(SoundEffect::MenuTransition_8, 0, 0);
    }
}

void MainMenuTransition::VRender(int** ppOt)
{
    VRender_436610(ppOt);
}

void MainMenuTransition::VRender_436610(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

Menu* Menu::ctor_47A6F0(Path_TLV* /*pTlv*/, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BCE78);
    SetVTable(&field_134_anim, 0x4BA2B8);
    
    gMainMenuInstanceCount_9F2DE0++;
    
    if (sFontLoaded_507688 == 0)
    {
        sFontContext_4FFD68.LoadFontType_41C040(1);
        sFontLoaded_507688 = 1;
    }

    field_FC_font.ctor_41C170(240, byte_4D0090, &sFontContext_4FFD68);
    field_E4_res_array[0] = nullptr;
    field_E4_res_array[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 132, 1, 0);
    field_E4_res_array[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 367, 1, 0);
    field_E4_res_array[5] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 8003, 1, 0);

    // 30 = fmv select
    if ( gMap_507BA8.field_4_current_camera == 30)
    {
        field_E4_res_array[2] = nullptr;
        field_E4_res_array[3] = nullptr;
        Animation_Init_417FD0(201508, 134, 90, field_E4_res_array[1], 1);
    }
    else
    {
        ResourceManager::LoadResourceFile_455270("ABEINTRO.BAN", nullptr);
        field_E4_res_array[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 131, 1, 0);
        ResourceManager::LoadResourceFile_455270("DOOR.BAN", nullptr);
        field_E4_res_array[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8001, 1, 0);
        Animation_Init_417FD0(41420, 134, 90, field_E4_res_array[3], 1);
    }

    field_10_anim.field_A_b = 127;
    field_10_anim.field_9_g = 127;
    field_10_anim.field_8_r = 127;

    BYTE** v8 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8002, 1, 0);
    field_134_anim.Init_402D20(6152, gObjList_animations_505564, this, 99, 43, v8, 1, 0, 0);

    field_134_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    field_134_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_134_anim.field_C_layer = 38;
    field_134_anim.field_14_scale = field_BC_sprite_scale;
    field_134_anim.field_B_render_mode = 1;
    field_134_anim.LoadPal_403090(field_E4_res_array[5], 0);
    field_204_flags &= ~6u;
    field_1E2_rgb = 40;
    field_1E4_colour_counter = -8;
    field_1D4_tlvInfo = tlvInfo;
    field_1D8_timer = gnFrameCount_507670 + 150;

    // copy right boot screen
    field_1CC_fn_update = &Menu::CopyRight_Update_47B4C0;


    // 1 == abe hello screen
    if (gMap_507BA8.field_4_current_camera == 1)
    {
        field_1CC_fn_update = &Menu::WaitForDoorToOpen_47B550;
        field_204_flags |= 2;
        field_10_anim.Set_Animation_Data_402A40(41420, field_E4_res_array[3]);
    }

    field_1D0_fn_render = &Menu::Empty_Render_47AC80;
    field_1E0_selected_index = 1;
    field_1E8_pMenuTrans = 0;
    field_1EC_pObj1 = 0;
    field_1F0_pObj2 = 0;
    field_200 = 0;
    field_1FA = 0;
    field_1F4_text = "";
    field_1F8 = 0;
    field_202 = 0;
    field_224_bToFmvSelect = 0;
    field_226_bToLevelSelect = 0;
    field_20C_bStartInSpecificMap = 0;

    sEnableFartGasCheat_507704 = 0;
    sVoiceCheat_507708 = 0;
    sEnableCheatFMV_50770C = 0;
    sEnableCheatLevelSelect_507710 = 0;
    sKilledMudokons_5076BC = 0;
    sRescuedMudokons_5076C0 = 0;
    gRestartRuptureFarmsKilledMuds_5076C4 = 0;
    gRestartRuptureFarmsSavedMuds_5076C8 = 0;

    // 30 = fmv select
    if (gMap_507BA8.field_4_current_camera == 30)
    {
        field_204_flags &= ~2u;
        field_224_bToFmvSelect = 1;
        field_21C = 0;
        field_1CC_fn_update = &Menu::FMV_Select_Update_47E8D0;
        field_1D0_fn_render = &Menu::FMV_Or_Level_Select_Render_47EEA0;
        field_1E0_selected_index = 0;
        field_218 = 0;
        field_220 = 0;

        sActiveList_9F2DE4 = gFmvs_4D0230;
        sListCount_4D0228 = ALIVE_COUNTOF(gFmvs_4D0230);

        field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);

        FrameInfoHeader* pFrameInfoHeader = field_10_anim.Get_FrameHeader_403A00(0);
        auto pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
        field_10_anim.LoadPal_403090(
            field_10_anim.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset);
    }

    dword_4CE598 = (dword_5079A4 != 0) + 1;
    if (sJoystickEnabled_508A60 > (dword_5079A4 != 0) + 1)
    {
        sJoystickEnabled_508A60 = 0;
    }

    return this;
}

BaseGameObject* Menu::dtor_47AAB0()
{
    SetVTable(this, 0x4BCE78);

    gMap_507BA8.TLV_Reset_446870(field_1D4_tlvInfo, -1, 0, 0);
    field_134_anim.vCleanUp();

    for (int i = 0; i < ALIVE_COUNTOF(field_E4_res_array); i++)
    {
        if (field_10_anim.field_20_ppBlock != field_E4_res_array[i])
        {
            ResourceManager::FreeResource_455550(field_E4_res_array[i]);
        }
    }

    if (field_1E8_pMenuTrans)
    {
        field_1E8_pMenuTrans->field_6_flags.Set(Options::eDead_Bit3);
        field_1E8_pMenuTrans->field_C_refCount--;
        field_1E8_pMenuTrans = nullptr;
    }

    if (field_1EC_pObj1)
    {
        field_1EC_pObj1->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_1F0_pObj2)
    {
        field_1F0_pObj2->field_6_flags.Set(Options::eDead_Bit3);
    }

    gMainMenuInstanceCount_9F2DE0--;

    field_FC_font.dtor_41C130();

    return dtor_417D10();
}

BaseGameObject* Menu::VDestructor(signed int flags)
{
    return Vdtor_47FC40(flags);
}

Menu* Menu::Vdtor_47FC40(signed int flags)
{
    dtor_47AAB0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Menu::VUpdate()
{
    VUpdate_47ABB0();
}

void Menu::VRender_47AC00(int** ppOt)
{
    if ((field_204_flags >> 1) & 1)
    {
        field_10_anim.vRender(184, 162, ppOt, 0, 0);

        PSX_RECT rect = {};
        field_10_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
    (this->*field_1D0_fn_render)(ppOt);
}

void Menu::VScreenChanged()
{
    // Empty
}

void Menu::VRender(int** ppOt)
{
    VRender_47AC00(ppOt);
}

void Menu::VUpdate_47ABB0()
{
    field_1E2_rgb += field_1E4_colour_counter;

    if (field_1E2_rgb < 40 || field_1E2_rgb > 80)
    {
        field_1E2_rgb -= field_1E4_colour_counter;
        field_1E4_colour_counter = -field_1E4_colour_counter;
    }

    field_134_anim.field_8_r = static_cast<BYTE>(field_1E2_rgb);
    field_134_anim.field_A_b = static_cast<BYTE>(field_1E2_rgb);
    field_134_anim.field_9_g = static_cast<BYTE>(field_1E2_rgb);

    (this->*field_1CC_fn_update)();
}

// Wait for the door that abes head comes out of to open
void Menu::WaitForDoorToOpen_47B550()
{
    field_204_flags |= 2u;
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.Set_Animation_Data_402A40(37364, field_E4_res_array[2]);
        FrameInfoHeader* pFrameInfoHeader = field_10_anim.Get_FrameHeader_403A00(0);
        auto pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
        field_10_anim.LoadPal_403090(field_10_anim.field_20_ppBlock, pFrameHeader->field_0_clut_offset);
        field_1CC_fn_update = &Menu::WaitForAbesHeadPoppingThroughDoor_47B5E0;
        ResourceManager::FreeResource_455550(field_E4_res_array[3]);
        field_204_flags &= ~2u;
        field_E4_res_array[3] = 0;
        field_1D8_timer = gnFrameCount_507670 + 15;
    }
}

void Menu::WaitForAbesHeadPoppingThroughDoor_47B5E0()
{
    if (field_1D8_timer <= static_cast<int>(gnFrameCount_507670))
    {
        field_204_flags |= 2u;
        field_10_anim.Set_Animation_Data_402A40(37364, nullptr);
        field_1CC_fn_update = &Menu::AbePopThroughDoor_47B620;
    }
}

void Menu::AbePopThroughDoor_47B620()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Put abe into the bug eyed idle loop
        field_10_anim.Set_Animation_Data_402A40(201384, field_E4_res_array[1]);
        field_1CC_fn_update = &Menu::SayHelloWaitForLoading_47B690;
        field_1DC_idle_input_counter = 0;
        ResourceManager::FreeResource_455550(field_E4_res_array[2]);
        field_E4_res_array[2] = nullptr;
        ResourceManager::LoadResourceFile("ABESPEAK.BAN", Menu::OnResourceLoaded_47ADA0, this);
    }
}

void Menu::CopyRight_Update_47B4C0()
{
    if (gMap_507BA8.field_4_current_camera == 23)
    {
        if (static_cast<int>(gnFrameCount_507670) > field_1D8_timer)
        {
            field_1D8_timer = gnFrameCount_507670 + 150;
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 10, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }
    }
    else 
    {
        if (static_cast<int>(gnFrameCount_507670) > field_1D8_timer || gMap_507BA8.field_4_current_camera != 10)
        {
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect5_1_FMV, 30102, 0);
            field_1CC_fn_update = &Menu::WaitForDoorToOpen_47B550;
            field_10_anim.Set_Animation_Data_402A40(41420, field_E4_res_array[3]);
        }
    }
}

void Menu::FMV_Select_Update_47E8D0()
{
    NOT_IMPLEMENTED();
}

void Menu::Empty_Render_47AC80(int**)
{
    NOT_IMPLEMENTED();
}

void Menu::FMV_Or_Level_Select_Render_47EEA0(int**)
{
    NOT_IMPLEMENTED();
}

void Menu::SayHelloWaitForLoading_47B690()
{
    // After 1 idle anim loop
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Wait for in progress loading (gamespeak ban) to finish
        if (!field_E4_res_array[0])
        {
            while (ProgressInProgressFilesLoading())
            {
                // Wait on loading to finish
            }
        }
        Mudokon_SFX_42A4D0(MudSounds::eHello_3, 0, 0, 0);
        field_10_anim.Set_Animation_Data_402A40(201320, field_E4_res_array[1]);
        field_1CC_fn_update = &Menu::WaitForAbeSayHello_47B770;
    }
}

void Menu::WaitForAbeSayHello_47B770()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        // Abe has finished saying hello, go to main menu handler
        field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
        field_10_anim.SetFrame_402AC0(7u);
        field_1CC_fn_update = &Menu::MainScreen_Update_47AF60;
        field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
        field_1D8_timer = gnFrameCount_507670 + Math_RandomRange_450F20(300, 450);
    }
}

bool Menu::ProgressInProgressFilesLoading()
{
    for (int i = 0; gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eLoadingFile_39)
        {
            pObjIter->VUpdate();
            if (pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                i = gBaseGameObject_list_9F2DF0->RemoveAt(i);
                pObjIter->VDestructor(1);
            }
            return true;
        }
    }
    return false;
}

EXPORT void Menu::MainScreen_Render_47BED0(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}
 
EXPORT void Menu::MainScreen_Update_47AF60()
{
    // Calculate idle timers for playing game play demos
    int bSmallerTimeout = 0;
    if (sInputObject_5009E8.field_0_pads[0].field_0_pressed)
    {
        bSmallerTimeout = 0;
        field_1DC_idle_input_counter = 0;
        gDemoPlay_507694 = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
        bSmallerTimeout = gDemoPlay_507694;
    }

    // Backwards menu button
    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0x9000u) // TODO: input constants
    {
        field_1E0_selected_index--;
        if (field_1E0_selected_index < 0)
        {
            field_1E0_selected_index = 4; // TODO: Why isn't count of stru_4D00B0 ??
        }

        field_134_anim.Set_Animation_Data_402A40(sMainScreenButtons_4D00B0[field_1E0_selected_index].field_4_frame_table, nullptr);
        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
        bSmallerTimeout = gDemoPlay_507694;
    }

    // Forward menu button
    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0x6100) // TODO: input constants
    {
        field_1E0_selected_index++;
        if (field_1E0_selected_index > 4) // TODO: Why isn't count of stru_4D00B0 ??
        {
            field_1E0_selected_index = 0;
        }

        field_134_anim.Set_Animation_Data_402A40(sMainScreenButtons_4D00B0[field_1E0_selected_index].field_4_frame_table, nullptr);
        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400, 0);
        bSmallerTimeout = gDemoPlay_507694;
    }

    const int idleMax = bSmallerTimeout != 0 ? 300 : 1500;
    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0x8F0 || field_1DC_idle_input_counter > idleMax) // TODO: input constants
    {
        if (field_1DC_idle_input_counter <= idleMax)
        {
            if (field_E4_res_array[0])
            {
                field_204_flags |= 1u;

                if (field_1E0_selected_index == 1)
                {
                    // Begin/new game
                    Mudokon_SFX_42A4D0(MudSounds::eFollowMe_4, 0, 0, 0);
                    field_10_anim.Set_Animation_Data_402A40(2115300, field_E4_res_array[0]);
                }
                else if (field_1E0_selected_index == 2)
                {
                    // Quit
                    Mudokon_SFX_42A4D0(MudSounds::eGoodbye_12, 0, 0, 0);
                    field_10_anim.Set_Animation_Data_402A40(2115120, field_E4_res_array[0]);
                }
                else
                {
                    // 0 = game speak,
                    // 3 = load
                    // 4 = options
                    Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 0, 0);
                    field_10_anim.Set_Animation_Data_402A40(201632, field_E4_res_array[1]);
                }

                field_1CC_fn_update = &Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90;
            }
            else
            {
                if (field_1E8_pMenuTrans)
                {
                    field_1E8_pMenuTrans->StartTrans_436560(40, 1, 0, 16);
                }
                else
                {
                    field_1E8_pMenuTrans = ao_new<MainMenuTransition>();
                    field_1E8_pMenuTrans->ctor_436370(40, 1, 0, 16, 1);
                    field_1E8_pMenuTrans->field_C_refCount++;
                }

                field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
                field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
            }
        }
        else
        {
            // Play a demo
            gDemoPlay_507694 = 1;
            field_1E0_selected_index = 1;
            gAttract_507698 = 1;

            char fileNameBuf[20] = {};
            sprintf(fileNameBuf, "PLAYBK%02d.JOY", sJoyResId_50769C);
            ResourceManager::LoadResourceFile_4551E0(fileNameBuf, 0, 0, 0);

            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->field_C_refCount--;
                field_1E8_pMenuTrans->field_6_flags.Set(Options::eDead_Bit3);
            }

            field_1E8_pMenuTrans = ao_new<MainMenuTransition>();
            field_1E8_pMenuTrans->ctor_436370(40, 1, 0, 16, 1);
            field_1E8_pMenuTrans->field_C_refCount++;
            field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
        }
    }

    if (sEnableCheatFMV_50770C)
    {
        sEnableCheatFMV_50770C = 0;
        field_224_bToFmvSelect = 1;
        sActiveList_9F2DE4 = gFmvs_4D0230;
        sListCount_4D0228 = ALIVE_COUNTOF(gFmvs_4D0230);
        if (field_E4_res_array[0])
        {
            Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 0, 0);
            field_10_anim.Set_Animation_Data_402A40(201632, field_E4_res_array[1]);
            field_1CC_fn_update = &Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90;
        }
        else
        {
            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->StartTrans_436560(40, 1, 0, 16);
            }
            else
            {
                field_1E8_pMenuTrans = ao_new<MainMenuTransition>();
                field_1E8_pMenuTrans->ctor_436370(40, 1, 0, 16, 1);
                field_1E8_pMenuTrans->field_C_refCount++;
            }

            field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
            field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
        }
    }

    if (sEnableCheatLevelSelect_507710)
    {
        sEnableCheatLevelSelect_507710 = 0;
        field_226_bToLevelSelect = 1;
        sActiveList_9F2DE4 = sLevelList_4D0300;
        sListCount_4D0228 = ALIVE_COUNTOF(sLevelList_4D0300);
        if (field_E4_res_array[0])
        {
            Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 0, 0);
            field_10_anim.Set_Animation_Data_402A40(201632, field_E4_res_array[1]);
            field_1CC_fn_update = &Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90;
        }
        else
        {
            if (field_1E8_pMenuTrans)
            {
                field_1E8_pMenuTrans->StartTrans_436560(40, 1, 0, 16);
            }
            else
            {
                field_1E8_pMenuTrans = ao_new<MainMenuTransition>();
                field_1E8_pMenuTrans->ctor_436370(40, 1, 0, 16, 1);
                field_1E8_pMenuTrans->field_C_refCount++;
            }

            field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
            field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
        }
    }

    // Some sort of idle anim toggling
    if (((field_204_flags) >> 2) & 1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
            field_204_flags &= ~4u;
            field_1D8_timer = gnFrameCount_507670 + Math_RandomRange_450F20(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<int>(gnFrameCount_507670))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.Set_Animation_Data_402A40(201384, field_E4_res_array[1]);
            field_204_flags |= 4u;
        }
    }
}

// After fade out go to gamespeak/options/load/whatever
void Menu::GoToSelectedMenuPage_47BC50()
{
    if (field_1E8_pMenuTrans == nullptr || field_1E8_pMenuTrans->field_16_bDone)
    {
        if (field_224_bToFmvSelect)
        {
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 30, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
            return;
        }

        if (field_226_bToLevelSelect)
        {
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 31, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
            return;
        }

        switch (field_1E0_selected_index)
        {
         // Gamespeak
        case 0:
            field_204_flags &= ~1u;

            // Diff cam depending on input method ?
            if (sJoystickEnabled_508A60)
            {
                gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 3, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            }
            else
            {
                gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 33, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            }

            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;

            field_134_anim.field_A_b = 127;
            field_134_anim.field_9_g = 127;
            field_134_anim.field_8_r = 127;
            break;

        // Begin
        case 1:
            field_1CC_fn_update = &Menu::ToLoading_47B7E0;
            break;

        // Quit
        case 2:
            sBreakGameLoop_507B78 = 1;
            exit(0);
            break;

        // Load
        case 3:
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 6, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
            break;

        // Options
        case 4:
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 2, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            field_1CC_fn_update = &Menu::ToNextMenuPage_47BD80;
            break;

        default:
            LOG_ERROR("Unknown menu item " << field_1E0_selected_index);
            break;
        }
    }
}

void Menu::WaitForSpeakFinishAndStartChangeEffect_47BB90()
{
    // Abe finished speaking?
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_1E8_pMenuTrans)
        {
            field_1E8_pMenuTrans->StartTrans_436560(40, 1, 0, 16);
        }
        else
        {
            field_1E8_pMenuTrans = ao_new<MainMenuTransition>();
            field_1E8_pMenuTrans->ctor_436370(40, 1, 0, 16, 1);
            field_1E8_pMenuTrans->field_C_refCount++;
        }

        field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
        field_1CC_fn_update = &Menu::GoToSelectedMenuPage_47BC50;
    }
}

void Menu::ToNextMenuPage_47BD80()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (field_224_bToFmvSelect || field_226_bToLevelSelect)
        {
            field_204_flags &= ~2u;
            field_21C = 0;
            field_1CC_fn_update = &Menu::To_FMV_Or_Level_Select_Update_47EC30;
            field_1D0_fn_render = &Menu::FMV_Or_Level_Select_Render_47EEA0;
            field_1E0_selected_index = 0;
            field_218 = 0;
            field_220 = 0;
        }
        else
        {
            switch (field_1E0_selected_index)
            {
            // Gamespeak
            case 0:
            {
                FrameInfoHeader* pFrameInfoHeader = field_134_anim.Get_FrameHeader_403A00(0);
                auto pHeader = reinterpret_cast<FrameHeader*>(&(*field_134_anim.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
                field_134_anim.LoadPal_403090(field_134_anim.field_20_ppBlock, pHeader->field_0_clut_offset);
                field_1CC_fn_update = &Menu::ToGameSpeak_Update_47D620;
                field_1D0_fn_render = &Menu::GameSpeak_Render_47D700;
                field_1E0_selected_index = 0;
                break;
            }

            // ??
            case 1:
                field_1CC_fn_update = &Menu::Update_47E3C0;
                field_1D0_fn_render = &Menu::Render_47E5B0;
                field_1E0_selected_index = 0;
                break;

            // Load
            case 3:
                field_204_flags &= ~2u;
                field_1CC_fn_update = &Menu::To_Load_Update_47D8E0;
                field_1D0_fn_render = &Menu::Load_Render_47DDA0;
                field_1E0_selected_index = 0;
                break;

             // Options
            case 4:
                field_1CC_fn_update = &Menu::To_Options_Update_47C250;
                field_1D0_fn_render = &Menu::Options_Render_47C190;
                field_134_anim.Set_Animation_Data_402A40(stru_4D0148[0].field_4_frame_table, nullptr);
                field_1E0_selected_index = 0;
                break;

            default:
                LOG_ERROR("Unknown menu item " << field_1E0_selected_index);
                break;
            }
        }
        field_1E8_pMenuTrans->StartTrans_436560(40, 0, 0, 16);
    }
}

void Menu::ToLoading_47B7E0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1E8_pMenuTrans->StartTrans_436560(40, 0, 0, 16);
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 21, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            field_204_flags &= ~2u;
            field_1CC_fn_update = &Menu::Loading_Update_47B870;
            field_1D0_fn_render = &Menu::Empty_Render_47AC80;
        }
    }
}


void Menu::ToGameSpeak_Update_47D620()
{
    NOT_IMPLEMENTED();
}


void Menu::GameSpeak_Render_47D700(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void Menu::To_FMV_Or_Level_Select_Update_47EC30()
{
    NOT_IMPLEMENTED();
}


void Menu::Update_47E3C0()
{
    NOT_IMPLEMENTED();
}


void Menu::Render_47E5B0(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}


void Menu::To_Load_Update_47D8E0()
{
    NOT_IMPLEMENTED();
}


void Menu::Load_Render_47DDA0(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void Menu::To_Options_Update_47C250()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            field_1CC_fn_update = &Menu::Options_Update_47BF90;
            field_1DC_idle_input_counter = 0;
        }
    }
}


void Menu::Options_Render_47C190(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void Menu::Loading_Update_47B870()
{
    if (!gAttract_507698 || ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Plbk, sJoyResId_50769C, 0, 0))
    {
        if (field_1E8_pMenuTrans)
        {
            if (field_1E8_pMenuTrans->field_16_bDone)
            {
                if (gAttract_507698)
                {
                    char buffer[92] = {};
                    sprintf(buffer, "loading Joy # %d\n", sJoyResId_50769C);
                    // Never used ??
                    LOG_INFO(buffer);
                }

                field_1E8_pMenuTrans->field_C_refCount--;
                field_1E8_pMenuTrans->field_6_flags.Set(Options::eDead_Bit3);
                field_1E8_pMenuTrans = nullptr;

                if (!field_E4_res_array[0])
                {
                    while (!ProgressInProgressFilesLoading())
                    {
                        // Wait for loading
                    }
                }

                field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
                ResourceManager::FreeResource_455550(field_E4_res_array[0]);
                field_E4_res_array[0] = nullptr;
                ResourceManager::Reclaim_Memory_455660(0);
                field_1CC_fn_update = &Menu::NewGameStart_47B9C0;
            }
        }
    }
}

void Menu::NewGameStart_47B9C0()
{
    if (!sActiveHero_507678)
    {
        sActiveHero_507678 = ao_new<Abe>();
        sActiveHero_507678->ctor_420770(55888, 85, 57, 55);
    }

    if (gAttract_507698)
    {
        auto pDemoPlayBackMem = ao_new<DemoPlayback>();
        if (pDemoPlayBackMem)
        {
            BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Plbk, sJoyResId_50769C, 1, 0);
            pDemoPlayBackMem->ctor_4517B0(ppRes, 0);
        }
    }
    else
    {
        if (!pPauseMenu_5080E0)
        {
            pPauseMenu_5080E0 = ao_new<PauseMenu>();
            pPauseMenu_5080E0->ctor_44DEA0();
        }

        if (field_20C_bStartInSpecificMap)
        {
            field_20C_bStartInSpecificMap = FALSE;
            gMap_507BA8.SetActiveCam_444660(field_20E_level, field_210_path, field_212_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            sActiveHero_507678->field_A8_xpos = FP_FromInteger(field_214_abe_xpos);
            sActiveHero_507678->field_AC_ypos = FP_FromInteger(field_216_abe_ypos);
        }
        else
        {
            // Start the game in the biggest meat processing plant
            gInfiniteGrenades_5076EC = FALSE;
            gMap_507BA8.SetActiveCam_444660(LevelIds::eRuptureFarms_1, 15, 1, CameraSwapEffects::eEffect5_1_FMV, 102, 0);

            // What if someone made a level editor and wanted to change where abe spawns on the first map? Well... hard luck pal
            sActiveHero_507678->field_A8_xpos = FP_FromInteger(1378);
            sActiveHero_507678->field_AC_ypos = FP_FromInteger(83);
        }
    }
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Menu::Options_Update_47BF90()
{
    // Idle time calculate
    if (sInputObject_5009E8.field_0_pads[0].field_0_pressed)
    {
        field_1DC_idle_input_counter = 0;
    }
    else
    {
        field_1DC_idle_input_counter++;
    }

    // Menu backwards
    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0x1000) // TODO: Input constants
    {
        if (field_1E0_selected_index <= 0)
        {
            field_1E0_selected_index = 1;
        }
        else
        {
            field_1E0_selected_index--;
        }

        field_134_anim.Set_Animation_Data_402A40(stru_4D0148[field_1E0_selected_index].field_4_frame_table, nullptr);
        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400);
    }

    // Menu forwards
    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0x4100) // TODO: Input constants
    {
        if (field_1E0_selected_index >= 1)
        {
            field_1E0_selected_index = 0;
        }
        else
        {
            field_1E0_selected_index++;
        }

        field_134_anim.Set_Animation_Data_402A40(stru_4D0148[field_1E0_selected_index].field_4_frame_table, nullptr);
        SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400);
    }

    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0xC0) // TODO: Input constants
    {
        Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 0, 0);
        field_10_anim.Set_Animation_Data_402A40(201632, field_E4_res_array[1]);
        field_1CC_fn_update = &Menu::Options_WaitForAbeSpeak_Update_47C280;
    }

    if (sInputObject_5009E8.field_0_pads[0].field_6_held & 0x810 || field_1DC_idle_input_counter > 900) // TODO: Input constants
    {
        // Back to main menu
        field_1E0_selected_index = 2;
        field_134_anim.Set_Animation_Data_402A40(stru_4D0148[2].field_4_frame_table, 0);
        Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 0, 0);
        field_10_anim.Set_Animation_Data_402A40(201632, field_E4_res_array[1]);
        field_1CC_fn_update = &Menu::Options_WaitForAbeSpeak_Update_47C280;
    }

    // Some sort of idle anim toggle?
    if (((field_204_flags) >> 2) & 1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
            field_204_flags &= ~4u;
            field_1D8_timer = gnFrameCount_507670 + Math_RandomRange_450F20(120, 450);
        }
    }
    else if (field_1D8_timer <= static_cast<int>(gnFrameCount_507670))
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.Set_Animation_Data_402A40(201384, field_E4_res_array[1]);
            field_204_flags |= 4u;
        }
    }
}

void Menu::Options_WaitForAbeSpeak_Update_47C280()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);
        field_1E8_pMenuTrans->StartTrans_436560(40, 1, 0, 16);
        field_1CC_fn_update = &Menu::Option_GoTo_Selected_Update_47C2C0;
    }
}

void Menu::Option_GoTo_Selected_Update_47C2C0()
{
    if (field_1E8_pMenuTrans)
    {
        if (field_1E8_pMenuTrans->field_16_bDone)
        {
            switch (field_1E0_selected_index)
            {
            // Controller
            case 0:
                gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 40, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
                break;

            // Sound
            case 1:
                gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 5, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
                break;

            // Back to main menu screen
            case 2:
                gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
                break;

            default:
                LOG_ERROR("Unknown menu idx " << field_1E0_selected_index);
                break;
            }

            field_1CC_fn_update = &Menu::Options_To_Selected_After_Cam_Change_Update_47C330;
        }
    }
}

void Menu::Options_To_Selected_After_Cam_Change_Update_47C330()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        switch (field_1E0_selected_index)
        {
        // To controller options
        case 0:
            field_204_flags &= ~2u;
            field_228 = 0;
            field_1CC_fn_update = &Menu::To_Options_Controller_Update_47F2E0;
            field_1D0_fn_render = &Menu::Options_Controller_Render_47F430;
            field_1E0_selected_index = static_cast<short>(sJoystickEnabled_508A60);
            field_230 = -1;
            field_22C = 0;
            break;

        // To sound options
        case 1:
            field_1CC_fn_update = &Menu::To_Options_Sound_Update_47C6F0;
            field_1D0_fn_render = &Menu::Options_Sound_Render_47C630;
            field_1E0_selected_index = sSoundMono_507690;
            field_134_anim.Set_Animation_Data_402A40(stru_4D01C0[sSoundMono_507690].field_4_frame_table, nullptr);
            break;

        // Back to main menu
        case 2:
            field_1CC_fn_update = &Menu::To_MainScreen_Update_47BB60;
            field_1D0_fn_render = &Menu::MainScreen_Render_47BED0;
            field_1E0_selected_index = 4;
            field_134_anim.Set_Animation_Data_402A40(sMainScreenButtons_4D00B0[4].field_4_frame_table, nullptr);
            break;

        default:
            LOG_ERROR("Unknown menu idx " << field_1E0_selected_index);
            break;
        }

        field_1E8_pMenuTrans->StartTrans_436560(40, 0, 0, 16);
    }
}

void Menu::To_Options_Controller_Update_47F2E0()
{
    NOT_IMPLEMENTED();
}

void Menu::Options_Controller_Render_47F430(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void Menu::To_Options_Sound_Update_47C6F0()
{
    NOT_IMPLEMENTED();
}

void Menu::To_MainScreen_Update_47BB60()
{
    NOT_IMPLEMENTED();
}

void Menu::Options_Sound_Render_47C630(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void CC Menu::OnResourceLoaded_47ADA0(Menu* pMenu)
{
    pMenu->field_E4_res_array[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 130, 1, 0);
}

END_NS_AO


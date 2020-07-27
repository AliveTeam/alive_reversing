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

START_NS_AO

const int dword_4BB1B8[4] = { 6152, 6140, 6164, 0 };

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
        SFX_Play_43AD70(8u, 0, 0);
    }
}

ALIVE_VAR(1, 0x9F2DE0, int, gMainMenuInstanceCount_9F2DE0, 0);
ALIVE_VAR(1, 0x507688, short, sFontLoaded_507688, 0);
ALIVE_VAR(1, 0x4D0228, short, sListCount_4D0228, -1);


ALIVE_VAR(1, 0x508A60, int, dword_508A60, 0);
ALIVE_VAR(1, 0x4CE598, int, dword_4CE598, 0);
ALIVE_VAR(1, 0x5079A4, int, dword_5079A4, 0);

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

    if ( gMap_507BA8.field_4_current_camera == 30)
    {
        field_E4_res_array[2] = nullptr;
        field_E4_res_array[3] = nullptr;
        Animation_Init_417FD0(201508, 134, 90, field_E4_res_array[1], 1);
    }
    else
    {
        ResourceManager::LoadResourceFile_455270("ABEINTRO.BAN", 0, 0);
        field_E4_res_array[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 131, 1, 0);
        ResourceManager::LoadResourceFile_455270("DOOR.BAN", 0, 0);
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
    field_1CC_fn_update = &Menu::CopyRight_Update_47B4C0;
    field_1D8_timer = gnFrameCount_507670 + 150;

    if (gMap_507BA8.field_4_current_camera == 1)
    {
        field_1CC_fn_update = &Menu::AbeHeadDoorOpen_47B550;
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
    field_224 = 0;
    field_226 = 0;
    field_20C = 0;

    sEnableFartGasCheat_507704 = 0;
    sVoiceCheat_507708 = 0;
    sEnableCheatFMV_50770C = 0;
    sEnableCheatLevelSelect_507710 = 0;
    sKilledMudokons_5076BC = 0;
    sRescuedMudokons_5076C0 = 0;
    gRestartRuptureFarmsKilledMuds_5076C4 = 0;
    gRestartRuptureFarmsSavedMuds_5076C8 = 0;

    if (gMap_507BA8.field_4_current_camera == 30)
    {
        field_204_flags &= ~2u;
        field_224 = 1;
        field_21C = 0;
        field_1CC_fn_update = &Menu::FMV_Select_Update_47E8D0;
        field_1D0_fn_render = &Menu::FMV_Select_Render_47EEA0;
        field_1E0_selected_index = 0;
        field_218 = 0;
        field_220 = 0;

        sActiveList_9F2DE4 = gFmvs_4D0230;
        sListCount_4D0228 = ALIVE_COUNTOF(gFmvs_4D0230);

        field_10_anim.Set_Animation_Data_402A40(201508, field_E4_res_array[1]);

        FrameInfoHeader* v16 = field_10_anim.Get_FrameHeader_403A00(0);
        field_10_anim.LoadPal_403090(
            field_10_anim.field_20_ppBlock,
            *reinterpret_cast<int*>(&(*field_10_anim.field_20_ppBlock)[v16->field_0_frame_header_offset]));
    }

    dword_4CE598 = (dword_5079A4 != 0) + 1;
    if (dword_508A60 > (dword_5079A4 != 0) + 1)
    {
        dword_508A60 = 0;
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

void Menu::AbeHeadDoorOpen_47B550()
{
    NOT_IMPLEMENTED();
}

void Menu::CopyRight_Update_47B4C0()
{
    NOT_IMPLEMENTED();
}

void Menu::FMV_Select_Update_47E8D0()
{
    NOT_IMPLEMENTED();
}

void Menu::Empty_Render_47AC80(int**)
{
    NOT_IMPLEMENTED();
}

void Menu::FMV_Select_Render_47EEA0(int**)
{
    NOT_IMPLEMENTED();
}

END_NS_AO

#include "stdafx.h"
#include "MainMenu.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "PathData.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "CreditsController.hpp"
#include "MusicController.hpp"
#include "Sfx.hpp"

MainMenuController * MainMenuController::gMainMenuController = nullptr;

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
ALIVE_VAR(1, 0x5c2f68, const char, byte_5C2F68, 0);
ALIVE_ARY(1, 0x561960, MainMenuPage, 24, sMainMenuPages_561960, {});

inline std::int16_t operator "" _s(unsigned long long value)
{
    return static_cast<std::int16_t>(value);
}

MenuFMV sFmvs_561540[28] =
{
    { "GT Logo", 0_s, 65535_s, 65535_s, 3_s, 65535_s, 65535_s },
    { "Oddworld Intro", 0_s, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Abe's Exoddus", 0_s, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Backstory", 0_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Prophecy", 1_s, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Vision", 1_s, 65535_s, 65535_s, 24, 65535_s, 65535_s },
    { "Game Opening", 1_s, 65535_s, 65535_s, 2, 65535_s, 65535_s },
    { "Brew", 1_s, 65535_s, 65535_s, 26, 65535_s, 65535_s },
    { "Brew Transition", 1_s, 65535_s, 65535_s, 31, 65535_s, 65535_s },
    { "Escape", 1_s, 65535_s, 65535_s, 25, 65535_s, 65535_s },
    { "Reward", 2_s, 65535_s, 65535_s, 22, 65535_s, 65535_s },
    { "FeeCo", 5_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Information Booth", 5_s, 65535_s, 65535_s, 3, 65535_s, 65535_s },
    { "Train 1", 6_s, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Train 2", 9_s, 65535_s, 65535_s, 15, 65535_s, 65535_s },
    { "Train 3", 8_s, 65535_s, 65535_s, 6, 65535_s, 65535_s },
    { "Aslik Info", 5_s, 65535_s, 65535_s, 2, 65535_s, 65535_s },
    { "Aslik Explodes", 5_s, 65535_s, 65535_s, 1, 65535_s, 65535_s },
    { "Dripek Info", 6_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Dripek Explodes", 6_s, 65535_s, 65535_s, 3, 65535_s, 65535_s },
    { "Phleg Info", 8_s, 65535_s, 65535_s, 4, 65535_s, 65535_s },
    { "Phleg Explodes", 8_s, 65535_s, 65535_s, 5, 65535_s, 65535_s },
    { "Soulstorm Info", 9_s, 65535_s, 65535_s, 14, 65535_s, 65535_s },
    { "Ingredient", 9_s, 65535_s, 65535_s, 16, 65535_s, 65535_s },
    { "Conference", 9_s, 65535_s, 65535_s, 13, 65535_s, 65535_s },
    { "Happy Ending", 9_s, 65535_s, 65535_s, 17, 65535_s, 65535_s },
    { "Sad Ending", 9_s, 65535_s, 65535_s, 18, 65535_s, 65535_s },
    { "Credits", 16_s, 65535_s, 65535_s, 65535_s, 65535_s, 65535_s }
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

void MainMenuController::ctor_4CE9A0(Path_TLV* /*pTlv*/, TlvItemInfoUnion tlvOffsetLevelIdPathId)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x547958);
    SetVTable(&field_158_animation, 0x544290);

    ++sMainMenuObjectCounter_BB4400;
    field_6_flags |= 0x200u;

    gMainMenuController = this;

    if (!sMainMenuFontLoaded_5C1B92)
    {
        sFont1Context_5BC5C8.LoadFontType_433400(1);
        sMainMenuFontLoaded_5C1B92 = 1;
    }

    field_120_font.ctor_433590(240, pal_560F80, &sFont1Context_5BC5C8);
    memset(&field_F4_resources, 0, sizeof(field_F4_resources));

    field_F4_resources.field_4_res_abespek2 = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbespek2ResID, 1u, 0);
    field_F4_resources.field_10_res_optflare = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kOptflareResID, 1u, 0);

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
        field_F4_resources.field_8_res_abeintro = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbeintroResID, 1u, 0);
        ResourceManager::LoadResourceFile_49C170("STDOOR.BAN", 0);
        field_F4_resources.field_C_res_door = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kDoorResID, 1u, 0);
        Animation_Init_424E10(50212, 233, 0x7Du, field_F4_resources.field_C_res_door, 1, 1u);
    }

    field_20_animation.field_A_b = 127;
    field_20_animation.field_9_g = 127;
    field_20_animation.field_8_r = 127;

    field_F4_resources.field_18_res_highlite = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kHighliteResID, 1u, 0);
    field_158_animation.Init_40A030(13912, gObjList_animations_5C1A24, this, 150, 0x41u, field_F4_resources.field_18_res_highlite, 1u, 0, 0);

    field_158_animation.field_14_scale = field_CC_sprite_scale;
    field_158_animation.field_4_flags &= 0x3FFF;
    field_158_animation.field_4_flags |= 0x4000;
    field_158_animation.field_C_render_layer = 38;
    field_158_animation.field_B_render_mode = 1;

    field_1FE_highlite_alpha = 40;
    field_200_highlite_glow_speed = -8;
    field_1F0 = tlvOffsetLevelIdPathId.all; // TODO: Should probably be using the same types here, depending on how it gets used

    field_23C_T80 &= 0xFF1C0000u;
    field_214_page_index = static_cast<short>(GetPageIndexFromCam_4D05A0(gMap_5C3030.sCurrentCamId_5C3034));
    field_21C = 1;
    field_21E = 0;
    field_1F8 = 0;
    field_220 = 1;
    field_228 = 0;
    field_22A = 0;
    field_22C_T80 = 0;
    field_224 = 0;
    field_202_input_hold_down_timer = 15;
    field_204_prev_pressed = 0;
    field_230_fmv_index = 0; // Double check

    if (gMap_5C3030.sCurrentCamId_5C3034 == 1)
    {
        MainMenuController::sub_4D05E0(9, 0);
        field_23C_T80 |= 0x10000u;
    }

    field_23C_T80 &= 0xFEEB0000u;

    field_1FC_button_index = 0;
    field_208 = 0;
    field_20C = 0;
    field_210 = 0;
    field_23A = 0;
    field_234 = &byte_5C2F68;
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
        return;
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
        field_230_fmv_index = word_5C1B9E;
        field_20_animation.Set_Animation_Data_409C80(247808, (BYTE **)field_F4_resources.field_4_res_abespek2);
        sub_4D06A0(&field_20_animation);
    }
}

void MainMenuController::Render_4CF4C0(int ** ot)
{
    const auto animFlags = field_20_animation.field_4_flags;
    if (animFlags & 2 && sMainMenuPages_561960[field_214_page_index].field_E_show_character && animFlags & 4)
    {
        field_20_animation.Animation_v_40B820(184, 162, (int)ot, 0, 0);
        PSX_RECT pRect;
        field_20_animation.Get_Frame_Rect_409E10(&pRect);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(pRect.x, pRect.y, pRect.w, pRect.h, pScreenManager_5BB5F4->field_3A);
    }

    const auto buttons = sMainMenuPages_561960[field_214_page_index].field_18_buttons;
    if (buttons)
    {
        if (!(field_23C_T80 & 0x10000))
        {
            const int index = field_1FC_button_index;
            if (index != -1)
            {
                field_158_animation.Animation_v_40B820(buttons[index].field_2_x, buttons[index].field_4_y, (int)ot, 0, 0);
                PSX_RECT pRect;
                field_158_animation.Get_Frame_Rect_409E10(&pRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(pRect.x, pRect.y, pRect.w, pRect.h, pScreenManager_5BB5F4->field_3A);
            }
        }
    }

    auto renderFunc = sMainMenuPages_561960[this->field_214_page_index].field_14_fn_render;
    if (renderFunc)
        renderFunc(this, ot);
}

// Todo: needs cleanup
void MainMenuController::Update_4CF010()
{
    if (sDoesCreditsControllerExist_5C1B90)
    {
        if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed & 0x200000)
        {
            sDoesCreditsControllerExist_5C1B90 = 0;
            gMap_5C3030.SetActiveCam_480D30(0, 1, 6, 0, 0, 0);
            return;
        }

        if (field_1F4_credits_next_frame <= sGnFrame_5C1B84)
        {
            const auto currentCam = field_240_credits_current_cam + 1;
            field_240_credits_current_cam = currentCam;
            field_1F4_credits_next_frame = sGnFrame_5C1B84 + 160;
            if (gMap_5C3030.sCurrentPathId_5C3032 == 2)
            {
                if (currentCam > 22)
                {
                    field_240_credits_current_cam = 1;
                    gMap_5C3030.SetActiveCam_480D30(16, 1, field_240_credits_current_cam, 3, 0, 0);
                }
                else
                {
                    gMap_5C3030.SetActiveCam_480D30(16, 2, currentCam, 3, 0, 0);
                }
            }
            else
            {
                if (currentCam > 36)
                {
                    sDoesCreditsControllerExist_5C1B90 = 0;
                    gMap_5C3030.SetActiveCam_480D30(0, 1, 6, 0, 0, 0);
                    return;
                }
                gMap_5C3030.SetActiveCam_480D30(16, 1, field_240_credits_current_cam, 3, 0, 0);
            }
        }
    }
    else
    {
        if (gMap_5C3030.sCurrentCamId_5C3034 == 5
            || gMap_5C3030.sCurrentCamId_5C3034 == 11
            || gMap_5C3030.sCurrentCamId_5C3034 == 4)
        {
            MusicController::sub_47FD60(4, this, 0, 0);
        }
        else if (gMap_5C3030.sCurrentCamId_5C3034 == 12 || gMap_5C3030.sCurrentCamId_5C3034 == 13)
        {
            MusicController::sub_47FD60(8, this, 0, 0);
        }
        else
        {
            MusicController::sub_47FD60(0, this, 0, 0);
        }

        MainMenuController::UpdateHighliteGlow_4D0630();

        if (MainMenuController::sub_4CF640())
        {
            MainMenuController::sub_4CFE80();
            return;
        }

        MainMenuController::sub_4CFE80();

        if (!(field_23C_T80 & 0x10000))
        {
            auto currentCamId = sMainMenuPages_561960[field_214_page_index].field_0_cam_id;
            if (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed
                && currentCamId != 25
                && currentCamId != 20
                && currentCamId != 23)
            {
                field_1F8 = 0;
            }
            else
            {
                ++field_1F8;
            }

            auto v8 = 0;

            if (sMainMenuPages_561960[field_214_page_index].field_4 <= 0 || sMainMenuPages_561960[field_214_page_index].field_8 <= 0 || field_1F8 <= sMainMenuPages_561960[field_214_page_index].field_4)
            {
                auto pagesBtns = sMainMenuPages_561960[field_214_page_index].field_18_buttons;
                auto btnsHeld = sInputObject_5BD4E0.field_0_pads[0].field_C_held;

                if (pagesBtns)
                {
                    auto btnIndex = field_1FC_button_index;
                    if (btnIndex != -1)
                    {
                        if (sInputObject_5BD4E0.field_0_pads[0].field_C_held & 5)
                        {
                            // btnIndex = (signed __int16)btnIndex;
                            if (sMainMenuPages_561960[field_214_page_index].field_0_cam_id != 4)
                            {
                                // Todo: Fix this up
                                do
                                {
                                    if (btnIndex)
                                    {
                                        --btnIndex;
                                    }
                                    else
                                    {
                                        for (auto i = pagesBtns + 1; i->field_0; ++btnIndex)
                                            ++i;
                                    }
                                } while (pagesBtns[btnIndex].field_0 != 1 && btnIndex != field_1FC_button_index);
                            }
                            field_1FC_button_index = btnIndex;

                            field_158_animation.Set_Animation_Data_409C80(
                                sMainMenuPages_561960[field_214_page_index].field_18_buttons[btnIndex].field_8_anim_frame_offset, 0);

                            SFX_Play_46FBA0(0x34u, 35, 400, 0x10000);
                        }

                        if (btnsHeld & (eRight | eDown))
                        {
                            auto btnIndex = field_1FC_button_index;
                            auto v14 = field_214_page_index;
                            if (sMainMenuPages_561960[v14].field_0_cam_id != 4)
                            {
                                auto btnArray = sMainMenuPages_561960[v14].field_18_buttons;
                                do
                                {
                                    if (!btnArray[++btnIndex].field_0)
                                        btnIndex = 0;
                                } while (btnArray[btnIndex].field_0 != 1 && btnIndex != field_1FC_button_index);
                            }
                            field_1FC_button_index = btnIndex;
                            field_158_animation.Set_Animation_Data_409C80(
                                sMainMenuPages_561960[v14].field_18_buttons[(signed __int16)btnIndex].field_8_anim_frame_offset,
                                0);
                            SFX_Play_46FBA0(0x34u, 35, 400, 0x10000);
                        }
                    }
                }

                if (field_21E)
                    return;
                if (field_23C_T80 & 0x200000)
                    return;

                auto v16 = sMainMenuPages_561960[field_214_page_index].field_10_fn_update;
                if (!v16)
                    return;
                auto v18 = v16(this, btnsHeld);

                if (v18 <= 0 || v18 == gMap_5C3030.sCurrentCamId_5C3034)
                    return;

                field_218_target_page_index = MainMenuController::GetPageIndexFromCam_4D05A0((unsigned __int8)v18);

                // Seems to always be zero cause of this... but removing the shifting causes errors? Strange.
                auto v19 = (v18 >> 16) & 0xFF;
                field_21A_target_cam = v19;
                if (v19 == 255)
                    field_21A_target_cam = -1;
                v8 = BYTE1(v18);
            }
            else
            {
                field_1F8 = 0;
                field_218_target_page_index = MainMenuController::GetPageIndexFromCam_4D05A0(sMainMenuPages_561960[field_214_page_index].field_8);
                field_21A_target_cam = sMainMenuPages_561960[field_214_page_index].field_C;
                v8 = sMainMenuPages_561960[field_214_page_index].field_A;
            }

            field_21C = v8;
            field_21E = 1;
            return;
        }
    }
}

int __stdcall MainMenuController::GetPageIndexFromCam_4D05A0(int camId)
{
    for (int i = 0; i < 24; i++)
    {
        if (camId == sMainMenuPages_561960[i].field_0_cam_id)
        {
            return i;
        }
    }

    return 0;
}

void MainMenuController::sub_4D05E0(__int16 /*a2*/, __int16 /*a3*/)
{
    NOT_IMPLEMENTED();
}

void MainMenuController::sub_4D06A0(AnimationEx * /*a3*/)
{
    NOT_IMPLEMENTED();
}

signed int MainMenuController::sub_4CF640()
{
    NOT_IMPLEMENTED();
}

void MainMenuController::sub_4CFE80()
{
    NOT_IMPLEMENTED();
}

void MainMenuController::UpdateHighliteGlow_4D0630()
{
    field_1FE_highlite_alpha += field_200_highlite_glow_speed;

    // Invert glow speed to make ping pong effect.
    if (field_1FE_highlite_alpha < 40 || field_1FE_highlite_alpha > 80)
    {
        field_1FE_highlite_alpha += -field_200_highlite_glow_speed;
        field_200_highlite_glow_speed = -field_200_highlite_glow_speed;
    }

    field_158_animation.field_8_r = field_1FE_highlite_alpha;
    field_158_animation.field_A_b = field_1FE_highlite_alpha;
    field_158_animation.field_9_g = field_1FE_highlite_alpha;
}

void MainMenuController::callback_4D06E0(MainMenuController * a1)
{
    a1->field_F4_resources.field_0_res_abespeak = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kAbespeakResID, 1u, 0);
}

int MainMenuController::DrawMenuText_4D20D0(MainMenuText * /*array*/, int ** /*ot*/, Font * /*font*/, int * /*polyIndex*/, char /*a5*/)
{
    NOT_IMPLEMENTED();

    //bool charIndex; // bl
    //char *v7; // eax
    //signed int v8; // edi
    //int x; // esi
    //signed int measure; // ecx
    //int xOffset; // edx
    //unsigned int v12; // edx
    //int v13; // esi
    //unsigned int v14; // edx
    //char v15; // bl
    //signed int v16; // eax
    //int result; // eax
    //char dst[32]; // [esp+10h] [ebp-20h]
    //bool char2Index; // [esp+34h] [ebp+4h]
    //int y; // [esp+34h] [ebp+4h]
    //signed int op2a; // [esp+44h] [ebp+14h]

    //charIndex = strstr(array->field_8_text, "\x18") != 0;
    //char2Index = strstr(array->field_8_text, "\x19") != 0;
    //String_FormatString_4969D0(array->field_8_text, dst, 32, array->field_14 == 0);
    //if (a5)
    //{
    //    v7 = strchr(dst, '+');
    //    if (v7)
    //        strcpy(dst, v7 + 1);
    //}
    //if (!array->field_14 && dst[0] && charIndex != char2Index)
    //    strcat(dst, "+");
    //if (array->field_10_scale == 0.0)
    //{
    //    op2a = 55050;
    //    v8 = 55050;
    //}
    //else
    //{
    //    v8 = (signed __int64)(array->field_10_scale * 65536.0);
    //    op2a = (signed __int64)(array->field_10_scale * 65536.0);
    //}
    //x = array->field_0_x;
    //y = array->field_4_y;
    //if (array->field_C_align == 1)
    //{
    //    v14 = (signed int)(40 * x + 11 + ((unsigned __int64)(-1307163959i64 * (40 * x + 11)) >> 32)) >> 4;
    //    v13 = (v14 >> 31) + v14;
    //}
    //else
    //{
    //    measure = font->MeasureWidth_4336C0(dst, (FP)v8);
    //    xOffset = measure / -2;
    //    if (array->field_C_align == 2)
    //        xOffset = -measure;
    //    v8 = op2a;
    //    v12 = (signed int)(40 * (xOffset + x) + 11 + ((unsigned __int64)(-1307163959i64 * (40 * (xOffset + x) + 11)) >> 32)) >> 4;
    //    v13 = strlen(dst) + v12 + (v12 >> 31);
    //}
    //v15 = byte_5CA4B4;
    //v16 = Math_FixedPoint_Multiply_496C50(-655360, v8);
    //byte_5CA4B4 = 1;
    //result = font->DrawString_4337D0(
    //    ot,
    //    dst,
    //    v13,
    //    v16 / 0x10000 + y + 1,
    //    0,
    //    1,
    //    0,
    //    39,
    //    0,
    //    255,
    //    0,
    //    *polyIndex,
    //    (FP)v8,
    //    640,
    //    0);
    //*polyIndex = result;
    //byte_5CA4B4 = v15;
    //return result;
}

void MainMenu_ForceLink()
{
}

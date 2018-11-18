#include "stdafx.h"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"

TintEntry sTintMap_UXB_563A3C[19] =
{
    { 1, 127u, 127u, 127u },
    { 2, 137u, 137u, 137u },
    { 3, 127u, 127u, 127u },
    { 4, 127u, 127u, 127u },
    { 5, 127u, 127u, 127u },
    { 6, 127u, 127u, 127u },
    { 7, 127u, 127u, 127u },
    { 8, 127u, 127u, 127u },
    { 9, 127u, 127u, 127u },
    { 10, 127u, 127u, 127u },
    { 11, 127u, 127u, 127u },
    { 12, 127u, 127u, 127u },
    { 13, 127u, 127u, 127u },
    { 14, 127u, 127u, 127u },
    { 255, 127u, 127u, 127u },
    { 0, 0u, 0u, 0u },
    { 0, 0u, 0u, 0u },
    { 0, 0u, 0u, 0u },
    { 1, 0u, 0u, 0u }
};

void UXB_ForceLink() {
}

void UXB::sub_4DEED0(AnimationEx *a2)
{
    NOT_IMPLEMENTED();
}

void UXB::PlaySFX_4DE930(unsigned __int8 sfxIdx)
{
    NOT_IMPLEMENTED();
}

void UXB::VRender(int ** pOrderingTable)
{
    Render_4DF3D0(pOrderingTable);
}

UXB * UXB::ctor_4DE9A0(Path_UXB * tlv_params, TlvItemInfoUnion itemInfo)
{
    BaseAliveGameObject::ctor_408240(0);
    SetVTable(this, 0x547E80);
    SetVTable(&field_128_animation, 0x544290);
    field_4_typeId = eUXB;

    auto pResource = BaseGameObject::Add_Resource_4DC130(ResourceManager::Resource_Animation, 1037);

    Animation_Init_424E10(8048, 59, 0x13u, pResource, 1, 1u);
    
    field_20_animation.field_4_flags.Set(AnimFlags::eBit7);
    field_20_animation.field_B_render_mode = 0;

    SetTint_425600(sTintMap_UXB_563A3C, gMap_5C3030.sCurrentLevelId_5C3030);

    field_6_flags.Set(BaseGameObject::Options::eInteractive);
    field_1C8 &= 0xFFFEu;
    field_118 = 0;

    field_1C0_num_patterns = tlv_params->field_10_num_patterns;
    if (tlv_params->field_10_num_patterns < 1 || tlv_params->field_10_num_patterns > 4)
    {
        field_1C0_num_patterns = 1;
    }

    field_1C4_pattern = tlv_params->field_12_pattern;
    if (!tlv_params->field_12_pattern) // If no pattern set, go to a default one.
    {
        field_1C4_pattern = 11111;
    }

    field_1C2 = 0;

    auto v7 = field_1C4_pattern;
    
    if (field_1C0_num_patterns - 1)
    {
        for (int i = field_1C0_num_patterns - 1; i > 0; i--)
        {
            v7 /= 10;
        }
    }

    field_1C6 = v7 % 10;

    if (tlv_params->field_14_scale)
    {
        if (tlv_params->field_14_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 16;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1.0);
        field_20_animation.field_C_render_layer = 35;
        field_D6_scale = 1;
    }

    sub_4DEED0(&field_128_animation);

    if ((tlv_params->field_0_mBase.field_0_flags.Raw().all & 0xFF00) == 256)
    {
        if (!tlv_params->field_16_state)
        {
            field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 1006, 0, 0), 0);
            field_1C8 &= 0xFFFDu;
            field_128_animation.Set_Animation_Data_409C80(544, 0);
            PlaySFX_4DE930(2);
            field_20_animation.Set_Animation_Data_409C80(0x2000, 0);
            field_118 = 3;
            field_11A = 0;
        }
        else
        {
            field_11A = 3;
        }
    }
    else
    {
        if (!tlv_params->field_16_state)
        {
            field_11A = 0;
        }
        else
        {
            field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 1006, 0, 0), 0);
            field_1C8 &= 0xFFFDu;
            field_128_animation.Set_Animation_Data_409C80(544, 0);
            field_20_animation.Set_Animation_Data_409C80(0x2000, 0);
            field_11A = 3;
            field_118 = 3;
        }
    }

    const auto x_middle = FP_FromInteger<int>((tlv_params->field_0_mBase.field_8_top_left.field_0_x + tlv_params->field_0_mBase.field_C_bottom_right.field_0_x) / 2);

    FP hitX;
    FP hitY;
    
    field_B8_xpos = x_middle;
    field_BC_ypos = FP_FromInteger<int>(tlv_params->field_0_mBase.field_8_top_left.field_2_y);

    // Raycasts on ctor to place perfectly on the floor.
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        x_middle,
        FP_FromInteger<int>(tlv_params->field_0_mBase.field_8_top_left.field_2_y),
        x_middle,
        FP_FromInteger<int>(tlv_params->field_0_mBase.field_8_top_left.field_2_y + 24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }

    field_120_tlv = itemInfo;
    field_124_next_state_frame = sGnFrame_5C1B84;
    field_11C_disabled_resources = tlv_params->field_18_disabled_resources;

    Add_Resource_4DC130(ResourceManager::Resource_Animation, 13);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 1105);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 300);
    Add_Resource_4DC130(ResourceManager::Resource_Palt, 1006);

    if (!(field_11C_disabled_resources & 1))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 25);
    }
    if (!(field_11C_disabled_resources & 2))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 576);
    }
    FP gridSnap = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    field_E4 = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_EC = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_6_flags.Set(Options::eInteractive);
    field_DC_bApplyShadows |= 2u;
    field_E8 = field_BC_ypos - gridSnap;
    field_F0_prev_base = field_BC_ypos;

    return this;
}

void UXB::Update_4DF030()
{
    NOT_IMPLEMENTED();
}

void UXB::Render_4DF3D0(int ** pOt)
{
    if (this->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            this->field_C2_lvl_number,
            this->field_C0_path_number,
            this->field_B8_xpos,
            this->field_BC_ypos,
            0))
        {
            this->field_128_animation.vRender_40B820(
                FP_GetExponent((this->field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
                FP_GetExponent((this->field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y - FP_Round(field_CC_sprite_scale * FP_FromDouble(17)))),
                pOt,
                0,
                0);

            PSX_RECT frameRect;
            this->field_128_animation.Get_Frame_Rect_409E10(&frameRect);

            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_5BB5F4->field_3A_idx);

            Render_424B90(pOt);
        }
    }
}

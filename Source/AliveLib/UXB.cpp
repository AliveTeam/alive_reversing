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

UXB * UXB::ctor_4DE9A0(Path_UXB * tlv_params, TlvItemInfoUnion itemInfo)
{
    NOT_IMPLEMENTED();

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

    this->field_1C8 &= 0xFFFEu;
    this->field_118 = 0;
    this->field_1C0_num_patterns = tlv_params->field_10_num_patterns;
    if (tlv_params->field_10_num_patterns < 1 || tlv_params->field_10_num_patterns > 4)
    {
        this->field_1C0_num_patterns = 1;
    }
    this->field_1C4_pattern = tlv_params->field_12_pattern;
    if (!tlv_params->field_12_pattern)
    {
        this->field_1C4_pattern = 11111;
    }

    auto v7 = this->field_1C4_pattern;
    const auto v8 = this->field_1C0_num_patterns - 1;
    this->field_1C2 = 0;
    auto v9 = 0;
    if (v8)
    {
        v9 = v8;
        do
        {
            --v9;
            v7 = (unsigned __int16)v7 / 10;
        } while (v9);
    }
    this->field_1C6 = (unsigned __int16)v7 % 10;
    if (tlv_params->field_14_scale)
    {
        if (tlv_params->field_14_scale == 1)
        {
            this->field_CC_sprite_scale = FP_FromDouble(0.5);
            this->field_20_animation.field_C_render_layer = 16;
            this->field_D6_scale = 0;
        }
    }
    else
    {
        this->field_CC_sprite_scale = FP_FromDouble(1.0);
        this->field_20_animation.field_C_render_layer = 35;
        this->field_D6_scale = 1;
    }

    sub_4DEED0(&this->field_128_animation);
    if ((tlv_params->field_0_mBase.field_0_flags.Raw().all & 0xFF00) == 256)
    {
        if (!tlv_params->field_16_state)
        {
            auto v10 = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 1006, 0, 0);
            field_128_animation.Load_Pal_40A530(v10, 0);
            this->field_1C8 &= 0xFFFDu;
            field_128_animation.Set_Animation_Data_409C80( 544, 0);
            PlaySFX_4DE930(2);
            field_20_animation.Set_Animation_Data_409C80( 0x2000, 0);
            this->field_118 = 3;
        LABEL_16:
            this->field_11A = 0;
            goto LABEL_17;
        }
        this->field_11A = 3;
    }
    else
    {
        if (!tlv_params->field_16_state)
        {
            goto LABEL_16;
        }
        auto v21 = ResourceManager::GetLoadedResource_49C2A0(1953259856, 1006, 0, 0);
        this->field_128_animation.Load_Pal_40A530(v21, 0);
        this->field_1C8 &= 0xFFFDu;
        field_128_animation.Set_Animation_Data_409C80(544, 0);
        field_20_animation.Set_Animation_Data_409C80(0x2000, 0);
        this->field_11A = 3;
        this->field_118 = 3;
    }

LABEL_17:
    FP hitY;
    FP a8;
    const int v12 = tlv_params->field_0_mBase.field_8_top_left.field_0_x + tlv_params->field_0_mBase.field_C_bottom_right.field_0_x;
    field_B8_xpos = FP_FromInteger<int>(v12 / 2);
    this->field_BC_ypos = FP_FromInteger<int>(tlv_params->field_0_mBase.field_8_top_left.field_2_y);
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger<int>(v12 / 2),
        FP_FromInteger<int>(tlv_params->field_0_mBase.field_8_top_left.field_2_y),
        FP_FromInteger<int>(v12 / 2),
        FP_FromInteger<int>(tlv_params->field_0_mBase.field_8_top_left.field_2_y + 24),
        &this->field_100_pCollisionLine,
        &hitY,
        &a8,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        this->field_BC_ypos = a8;
    }
    this->field_120 = itemInfo;
    this->field_124_next_state_frame = sGnFrame_5C1B84;
    this->field_11C_disabled_resources = tlv_params->field_18_disabled_resources;
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 13);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 1105);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 300);
    Add_Resource_4DC130(ResourceManager::Resource_Palt, 1006);

    if (!(this->field_11C_disabled_resources & 1))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 25);
    }
    if (!(this->field_11C_disabled_resources & 2))
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

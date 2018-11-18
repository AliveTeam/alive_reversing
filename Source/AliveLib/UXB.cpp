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

void UXB::InitBlinkAnim_4DEED0(AnimationEx *pAnimation)
{
    if (pAnimation->Init_40A030(544, gObjList_animations_5C1A24, this, 36, 0x15u, Add_Resource_4DC130(ResourceManager::Resource_Animation, 1011), 1u, 0, 0))
    {
        pAnimation->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnimation->field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        pAnimation->field_4_flags.Set(AnimFlags::eBit16_bBlending);
        pAnimation->field_14_scale = field_CC_sprite_scale;
        pAnimation->field_8_r = 128;
        pAnimation->field_9_g = 128;
        pAnimation->field_A_b = 128;
        pAnimation->field_B_render_mode = 1;
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed);
    }
}

void UXB::PlaySFX_4DE930(unsigned __int8 sfxIdx)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        this->field_C2_lvl_number,
        this->field_C0_path_number,
        this->field_B8_xpos,
        this->field_BC_ypos,
        0))
    {
        SFX_Play_46FA90(sfxIdx, 35, 0x10000);
    }
}

signed int UXB::sub_4DF630()
{
    NOT_IMPLEMENTED();
}

void UXB::VRender(int ** pOrderingTable)
{
    Render_4DF3D0(pOrderingTable);
}

UXB * UXB::ctor_4DE9A0(Path_UXB * tlv_params, TlvItemInfoUnion itemInfo)
{
    ctor_408240(0);
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

    InitBlinkAnim_4DEED0(&field_128_animation);

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
        int v2; // eax
        int v3; // eax
        BaseAnimatedWithPhysicsGameObject *v4; // eax
        __int16 v5; // ax
        __int16 v6; // ax
        BYTE **v7; // eax
        unsigned __int16 v8; // ax
        int v9; // ecx
        unsigned __int16 v10; // ax
        int v11; // edi
        unsigned int v12; // edx
        __int16 v13; // ax

        v2 = (unsigned __int16)this->field_118;
        if (v2)
        {
            v3 = v2 - 1;
            if (v3)
            {
                if (v3 == 1 && sGnFrame_5C1B84 >= this->field_124_next_state_frame)
                {
                    v4 = (BaseAnimatedWithPhysicsGameObject *)malloc_4954D0(0xF8u);
                    if (v4)
                    {
                        // Todo: make it go boom
                        /*BaseBomb::ctor_423E70(
                            v4,
                            this->field_B8_xpos,
                            this->field_BC_ypos,
                            0,
                            (DWORD *)this->field_CC_sprite_scale);*/
                    }
                    field_6_flags.Set(Options::eDead);
                }
                goto LABEL_29;
            }
            if (sub_4DF630())
            {
                this->field_118 = 2;
            LABEL_28:
                this->field_124_next_state_frame = sGnFrame_5C1B84 + 2;
                goto LABEL_29;
            }
            if (this->field_124_next_state_frame <= sGnFrame_5C1B84)
            {
                v5 = this->field_1C6;
                if (v5)
                {
                    v6 = v5 - 1;
                    this->field_1C6 = v6;
                    if (!v6)
                    {
                        v7 = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 1006, 0, 0);
                        this->field_128_animation.Load_Pal_40A530(v7, 0);
                        this->field_1C8 &= 0xFFFDu;
                    }
                }
                else
                {
                    this->field_128_animation.Load_Pal_40A530(
                        this->field_128_animation.field_20_ppBlock,
                        *(DWORD *)&(*this->field_128_animation.field_20_ppBlock)[*(DWORD *)&(*this->field_128_animation.field_20_ppBlock)[*((DWORD *)*this->field_128_animation.field_20_ppBlock + 138)]]);
                    this->field_1C8 |= 2u;
                    ++this->field_1C2;
                    v8 = this->field_1C0_num_patterns;
                    if (this->field_1C2 >= v8)
                    {
                        this->field_1C2 = 0;
                    }
                    v9 = this->field_1C4_pattern;
                    v10 = v8 - this->field_1C2 - 1;
                    if (v10)
                    {
                        v11 = v10;
                        do
                        {
                            --v11;
                            v9 = (unsigned __int16)v9 / 10;
                        } while (v11);
                    }
                    this->field_1C6 = (unsigned __int16)v9 % 10;
                }
                this->field_128_animation.Set_Animation_Data_409C80( 544, 0);
                if (this->field_1C8 & 2)
                {
                    PlaySFX_4DE930(3);
                }
                else
                {
                    PlaySFX_4DE930(2);
                }
                v12 = sGnFrame_5C1B84;
                this->field_118 = 0;
                this->field_124_next_state_frame = v12 + 10;
            }
        }
        else
        {
            if (sub_4DF630())
            {
                this->field_118 = 2;
                this->field_124_next_state_frame = sGnFrame_5C1B84 + 2;
                goto LABEL_29;
            }
            if (this->field_124_next_state_frame <= sGnFrame_5C1B84)
            {
                this->field_118 = 1;
                this->field_128_animation.Set_Animation_Data_409C80(556, 0);
                goto LABEL_28;
            }
        }
    LABEL_29:
        if (this->field_118 != 2)
        {
            if (Event_Get_422C00(kEventDeathReset))
            {
                v13 = this->field_11A;
                if (v13 != 3 || this->field_118 == 3)
                {
                    if (v13 || this->field_118 != 3)
                    {
                        Path::TLV_Reset_4DB8E0(this->field_120_tlv.all, 0, 1, 0);
                    }
                    else
                    {
                        Path::TLV_Reset_4DB8E0(this->field_120_tlv.all, 1, 1, 0);
                    }
                }
                else
                {
                    Path::TLV_Reset_4DB8E0(this->field_120_tlv.all, 1, 1, 0);
                }
                field_6_flags.Set(Options::eDead);
            }
        }
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
                FP_GetExponent((this->field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y - FP_NoFractional(field_CC_sprite_scale * FP_FromDouble(17)))),
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

struct SaveState_UXB
{
    BYTE field_0[4];
    TlvItemInfoUnion field_4_tlv;
    DWORD field_8_next_state_frame;
    WORD field_c_uxb_118;
    WORD field_e_uxb_11a;
    WORD field_10_disabled_resources;
    WORD field_12_uxb_1c2;
    WORD field_14_uxb_1c6;
    WORD field_16_unknown;
};
ALIVE_ASSERT_SIZEOF(SaveState_UXB, 24);

EXPORT int CC Uxb__CreateFromSaveState_4DFAE0(const BYTE* __pSaveState)
{
        const SaveState_UXB * pSaveState = reinterpret_cast<const SaveState_UXB *>(__pSaveState);

        Path_UXB *uxbPath = reinterpret_cast<Path_UXB *>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pSaveState->field_4_tlv.all));

        if (!(uxbPath->field_18_disabled_resources & 1) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", 0);
        }
        if (!(uxbPath->field_18_disabled_resources & 2) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("DOGBLOW.BAN", 0);
        }
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 1037, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("UXB.BND", 0);
        }
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 300, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170("EXPLODE.BND", 0);
        }

        UXB* pUXB = alive_new<UXB>();
        if (pUXB)
        {
            pUXB->ctor_4DE9A0(uxbPath, pSaveState->field_4_tlv);
        }

        if (pSaveState->field_c_uxb_118 == 3)
        {
            pUXB->field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 1006, 0, 0), 0);
            pUXB->field_128_animation.Set_Animation_Data_409C80(544, 0);
            pUXB->field_20_animation.Set_Animation_Data_409C80(0x2000, 0);
        }

        pUXB->field_124_next_state_frame = pSaveState->field_8_next_state_frame;
        pUXB->field_118 = pSaveState->field_c_uxb_118;
        pUXB->field_11A = pSaveState->field_e_uxb_11a;
        pUXB->field_11C_disabled_resources = pSaveState->field_10_disabled_resources;
        pUXB->field_1C2 = pSaveState->field_12_uxb_1c2;
        pUXB->field_1C6 = pSaveState->field_14_uxb_1c6;
        pUXB->field_1C8 = pUXB->field_1C8 & 0xFFFD | 2 * (pSaveState->field_16_unknown & 1);

        return sizeof(SaveState_UXB); // 24
}
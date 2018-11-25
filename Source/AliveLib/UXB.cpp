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
#include "Midi.hpp"
#include "Abe.hpp"

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
    { -1, 127u, 127u, 127u },
    { 0, 0u, 0u, 0u },
    { 0, 0u, 0u, 0u },
    { 0, 0u, 0u, 0u },
    { 1, 0u, 0u, 0u }
};

void UXB_ForceLink() {
}

void UXB::InitBlinkAnim_4DEED0(AnimationEx *pAnimation)
{
    if (pAnimation->Init_40A030(544, gObjList_animations_5C1A24, this, 36, 0x15u, Add_Resource_4DC130(ResourceManager::Resource_Animation, kBombflshResID), 1u, 0, 0))
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

signed int UXB::IsColliding_4DF630()
{
    PSX_RECT uxbBound;
    vGetBoundingRect_424FD0(&uxbBound, 1);

    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject * pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        // e114_Bit6 May be "can set off explosives?"
        if (pObj->field_114_flags.Get(e114_Bit6_SetOffExplosives) && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            PSX_RECT objBound;
            pObj->vGetBoundingRect_424FD0(&objBound, 1);

            int objX = FP_GetExponent(pObj->field_B8_xpos);
            int objY = FP_GetExponent(pObj->field_BC_ypos);

            if (objX > uxbBound.x &&
                objX < uxbBound.w &&
                objY < uxbBound.h + 5 &&
                uxbBound.x <= objBound.w &&
                uxbBound.w >= objBound.x &&
                uxbBound.h >= objBound.y &&
                uxbBound.y <= objBound.h &&
                pObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                return 1;
            }
        }
    }

    return 0;
}

void UXB::VUpdate()
{
    Update_4DF030();
}

void UXB::VRender(int ** pOrderingTable)
{
    Render_4DF3D0(pOrderingTable);
}

void UXB::VDestructor(signed int flags)
{
    dtor_4DEEA0(flags);
}

void UXB::VScreenChanged()
{
    ScreenChanged_4DF9C0();
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
    field_1C8_flags.Clear(UXB_Flags_1C8::e1C8_Bit0);
    field_118 = 0;

    field_1C0_pattern_length = tlv_params->field_10_num_patterns;
    if (tlv_params->field_10_num_patterns < 1 || tlv_params->field_10_num_patterns > 4)
    {
        field_1C0_pattern_length = 1;
    }


    field_1C4_pattern = tlv_params->field_12_pattern;
    if (!tlv_params->field_12_pattern) // If no pattern set, go to a default one.
    {
        field_1C4_pattern = 11111;
    }

    field_1C2_pattern_index = 0;
    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    field_1C6_red_blink_count = (field_1C4_pattern / static_cast<int>(pow(10, field_1C0_pattern_length - 1))) % 10;

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

    if ((tlv_params->field_0_mBase.field_0_flags.Raw().all & 0xFF00) == 256) // Checking if 9th bit is set?
    {
        if (!tlv_params->field_16_state)
        {
            field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, kGrenflshResID, 0, 0), 0);
            field_1C8_flags.Clear(UXB_Flags_1C8::e1C8_Bit1_IsRed);
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
            field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, kGrenflshResID, 0, 0), 0);
            field_1C8_flags.Clear(UXB_Flags_1C8::e1C8_Bit1_IsRed);
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
    field_11C_disabled_resources = static_cast<WORD>(tlv_params->field_18_disabled_resources);

    Add_Resource_4DC130(ResourceManager::Resource_Animation, kAbebombResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, kDebrisID00);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, kBgexpldResID);
    Add_Resource_4DC130(ResourceManager::Resource_Palt, kGrenflshResID);

    if (!(field_11C_disabled_resources & 1))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, kAbeblowResID);
    }
    if (!(field_11C_disabled_resources & 2))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, kSlogBlowResID);
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

void UXB::dtor_4DEF60()
{
    SetVTable(this, 0x547E80);

    if (field_118 != 2 || sGnFrame_5C1B84 < field_124_next_state_frame)
    {
        Path::TLV_Reset_4DB8E0(field_120_tlv.all, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_120_tlv.all, -1, 0, 1);
    }

    field_128_animation.vCleanUp_40C630();

    field_6_flags.Clear(Options::eInteractive);

    dtor_4080B0();
}

void UXB::dtor_4DEEA0(signed int flags)
{
    dtor_4DEF60();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void UXB::Update_4DF030()
{
    if (field_118)
    {
        const int v3 = field_118 - 1;
        if (v3)
        {
            if (v3 == 1 && sGnFrame_5C1B84 >= field_124_next_state_frame)
            {
                auto explosion = alive_new<BaseBomb>();
                if (explosion)
                {
                    explosion->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
                }
                field_6_flags.Set(Options::eDead);
            }
        }
        else if (IsColliding_4DF630())
        {
            field_118 = 2;
            field_124_next_state_frame = sGnFrame_5C1B84 + 2;
        }
        else if (field_124_next_state_frame <= sGnFrame_5C1B84)
        {
            if (field_1C6_red_blink_count)
            {
                field_1C6_red_blink_count--;
                if (!field_1C6_red_blink_count)
                {
                    field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, kGrenflshResID, 0, 0), 0);
                    field_1C8_flags.Clear(UXB_Flags_1C8::e1C8_Bit1_IsRed);
                }
            }
            else
            {
                const FrameInfoHeader* pFrameInfo = field_128_animation.Get_FrameHeader_40B730(-1);
                const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_128_animation.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);
                field_128_animation.Load_Pal_40A530(field_128_animation.field_20_ppBlock, pFrameHeader->field_0_clut_offset);

                field_1C8_flags.Set(UXB_Flags_1C8::e1C8_Bit1_IsRed);

                field_1C2_pattern_index++;

                if (field_1C2_pattern_index >= field_1C0_pattern_length)
                {
                    field_1C2_pattern_index = 0;
                }

                // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                field_1C6_red_blink_count = (field_1C4_pattern / static_cast<int>(pow(10, field_1C0_pattern_length - field_1C2_pattern_index - 1))) % 10;
            }

            field_128_animation.Set_Animation_Data_409C80(544, 0);

            if (field_1C8_flags.Get(UXB_Flags_1C8::e1C8_Bit1_IsRed))
            {
                PlaySFX_4DE930(Type1SFX::eUXBRed);
            }
            else
            {
                PlaySFX_4DE930(Type1SFX::eUXBGreen);
            }

            field_118 = 0;
            field_124_next_state_frame = sGnFrame_5C1B84 + 10; // UXB change color delay
        }
    }
    else
    {
        if (IsColliding_4DF630())
        {
            field_118 = 2;
            field_124_next_state_frame = sGnFrame_5C1B84 + 2;
        }
        else if (field_124_next_state_frame <= sGnFrame_5C1B84)
        {
            field_118 = 1;
            field_128_animation.Set_Animation_Data_409C80(556, 0);
            field_124_next_state_frame = sGnFrame_5C1B84 + 2;
        }
    }

    if (field_118 != 2)
    {
        if (Event_Get_422C00(kEventDeathReset))
        {
            if (field_11A != 3 || field_118 == 3)
            {
                if (field_11A || field_118 != 3)
                {
                    Path::TLV_Reset_4DB8E0(field_120_tlv.all, 0, 1, 0);
                }
                else
                {
                    Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
                }
            }
            else
            {
                Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
            }
            field_6_flags.Set(Options::eDead);
        }
    }
}

void UXB::Render_4DF3D0(int ** pOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            field_128_animation.vRender_40B820(
                FP_GetExponent((field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
                FP_GetExponent((field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y - FP_NoFractional(field_CC_sprite_scale * FP_FromDouble(17)))),
                pOt,
                0,
                0);

            PSX_RECT frameRect;
            field_128_animation.Get_Frame_Rect_409E10(&frameRect);

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

void UXB::ScreenChanged_4DF9C0()
{
    ScreenChanged_4DC0A0();

    FP x_distance = sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos;

    // This is pretty much the same as using Abs()
    if (x_distance < FP_FromInteger(0))
    {
        x_distance = field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos;
    }

    FP y_distance = sControlledCharacter_5C1B8C->field_BC_ypos - field_BC_ypos;

    // This is pretty much the same as using Abs()
    if (y_distance < FP_FromInteger(0))
    {
        y_distance = field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos;
    }

    if (y_distance > FP_FromInteger(520) || x_distance > FP_FromInteger(750))
    {
        if (field_11A != 3 || field_118 == 3)
        {
            if (field_11A || field_118 != 3)
            {
                Path::TLV_Reset_4DB8E0(field_120_tlv.all, 0, 1, 0);
                field_6_flags.Set(Options::eDead);
            }
            else
            {
                Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
                field_6_flags.Set(Options::eDead);
            }
        }
        else
        {
            Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
            field_6_flags.Set(Options::eDead);
        }
    }
}

int UXB::GetSaveState_4DFD40(BYTE * __pSaveBuffer)
{
    SaveState_UXB * pSaveState = reinterpret_cast<SaveState_UXB *>(__pSaveBuffer);

    pSaveState->field_0_id = 143;
    pSaveState->field_4_tlv = field_120_tlv;
    pSaveState->field_8_next_state_frame = field_124_next_state_frame;
    pSaveState->field_c_uxb_118 = field_118;
    pSaveState->field_e_uxb_11a = field_11A;
    pSaveState->field_10_disabled_resources = field_11C_disabled_resources;
    pSaveState->field_12_pattern_index = field_1C2_pattern_index;
    pSaveState->field_14_red_blink_count = field_1C6_red_blink_count;
    pSaveState->field_16_is_red = field_1C8_flags.Get(UXB_Flags_1C8::e1C8_Bit1_IsRed);

    return sizeof(SaveState_UXB);
}

EXPORT int CC UXB::CreateFromSaveState_4DFAE0(const BYTE* __pSaveState)
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
    pUXB->field_1C2_pattern_index = pSaveState->field_12_pattern_index;
    pUXB->field_1C6_red_blink_count = pSaveState->field_14_red_blink_count;

    pUXB->field_1C8_flags.Clear(UXB_Flags_1C8::e1C8_Bit1_IsRed);
    
    if (pSaveState->field_16_is_red)
    {
        pUXB->field_1C8_flags.Set(UXB_Flags_1C8::e1C8_Bit1_IsRed);
    }

    return sizeof(SaveState_UXB); // 24
}

int UXB::GetSaveState_4DC110(BYTE * __pSaveBuffer)
{
    return GetSaveState_4DFD40(__pSaveBuffer);
}

BaseBomb * BaseBomb::ctor_423E70(FP x, FP y, int /*unused*/, FP scale)
{
    NOT_IMPLEMENTED();

    BYTE **v6; // eax
    int v16; // [esp+18h] [ebp-4h]

    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    v16 = 0;
    SetVTable(this, 0x544C54);
    field_4_typeId = eBaseBomb;


    v6 = BaseGameObject::Add_Resource_4DC130(ResourceManager::Resource_Animation, 300);
    Animation_Init_424E10(51588, 214, 0x31u, v6, 1, 1u);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame); // Double Check
    field_20_animation.field_4_flags.Set(AnimFlags::eBit24);
    field_20_animation.field_B_render_mode = 1;
    field_20_animation.field_A_b = 128;
    field_20_animation.field_9_g = 128;
    field_20_animation.field_8_r = 128;
    this->field_f4_scale = scale;
    if (scale == FP_FromDouble(1.0))
    {
        field_20_animation.field_C_render_layer = 36;
    }
    else
    {
        field_20_animation.field_C_render_layer = 17;
    }
    field_DC_bApplyShadows &= 0xFFFEu;
    field_CC_sprite_scale = scale * FP_FromDouble(2.75);
    field_B8_xpos = x;
    field_BC_ypos = y;

    /*v8 = (BaseGameObject *)malloc_4954D0(0x48u);
    if (v8)
    {
        ScreenShake::ctor_4ACF70(v8, 1, 0);
    }*/
    if (word_5CC88C <= 3846)
    {
        /*v9 = malloc_4954D0(0x108u);
        if (v9)
        {
            sub_41CF50(
                v9,
                this->field_0_base.field_B8_xpos,
                this->field_0_base.field_BC_ypos,
                35,
                this->field_f4_scale,
                0,
                13);
        }*/
    }
    
    /*v12 = (signed int)Math_FixedPoint_Multiply_496C50(-655360, this->field_f4_scale) / 0x10000;
    v14 = (signed int)Math_FixedPoint_Multiply_496C50(655360, this->field_f4_scale) / 0x10000;
    v13 = (signed int)Math_FixedPoint_Multiply_496C50(-655360, this->field_f4_scale) / 0x10000;
    v15 = (signed int)Math_FixedPoint_Multiply_496C50(655360, this->field_f4_scale) / 0x10000;
    BaseAnimatedWithPhysicsGameObject::sub_4247A0(this, &v12);*/

    static int dword_5BC1FC = 0;
    dword_5BC1FC = (dword_5BC1FC + 1) % 2;
    SND_SEQ_PlaySeq_4CA960(static_cast<short>(14 +  dword_5BC1FC), 1, 1);

    return this;
}
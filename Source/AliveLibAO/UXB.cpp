#include "stdafx_ao.h"
#include "Function.hpp"
#include "UXB.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "BaseBomb.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"

namespace AO {

UXB* UXB::ctor_488C80(Path_UXB* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(&field_11C_anim, 0x4BA2B8);
    SetVTable(this, 0x4BD680);

    field_4_typeId = Types::eUXB_99;

    BYTE** ppRes_1 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kTbombResID, 1, 0);
    Animation_Init_417FD0(
        7740,
        59,
        19,
        ppRes_1,
        1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = 0;

    field_6_flags.Set(Options::eInteractive_Bit8);
    field_1BC_flags &= ~1u;
    field_10C_state = 0;

    field_1B4_pattern_length = pTlv->field_18_num_patterns;
    if (pTlv->field_18_num_patterns < 1u || pTlv->field_18_num_patterns > 4u)
    {
        field_1B4_pattern_length = 1;
    }

    field_1B8_pattern = pTlv->field_1A_pattern;
    if (!pTlv->field_1A_pattern) // If no pattern set, go to a default one.
    {
        field_1B8_pattern = 11111;
    }

    field_1B6_pattern_index = 0;

    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    field_1BA_red_blink_count = (field_1B8_pattern / static_cast<int>(pow(10, field_1B4_pattern_length - 1))) % 10;

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 16;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 35;
        field_C6_scale = 1;
    }

    InitBlinkAnim();

    if (pTlv->field_1_unknown)  // Stores the activated/deactivated state for UXB
    {
        if (!pTlv->field_1E_state)
        {
            BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kGrenflshResID, 0, 0);
            field_11C_anim.LoadPal_403090(ppRes, 0);
            field_1BC_flags &= ~2u;
            field_11C_anim.Set_Animation_Data_402A40(372, 0);
            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                SFX_Play_43AD70(SoundEffect::GreenTick_3, 35, 0);
            }
            field_10_anim.Set_Animation_Data_402A40(7884, 0);
            field_10C_state = 3;
            field_10E_starting_state = 0;
        }
        else
        {
            field_10E_starting_state = 3;
        }
    }
    else
    {
        if (!pTlv->field_1E_state)
        {
            field_10E_starting_state = 0;
        }
        else
        {
            BYTE** ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kGrenflshResID, 0, 0);
            field_11C_anim.LoadPal_403090(ppPal, 0);
            field_1BC_flags &= ~2u;
            field_11C_anim.Set_Animation_Data_402A40(372, 0);
            field_10_anim.Set_Animation_Data_402A40(7884, 0);
            field_10E_starting_state = 3;
            field_10C_state = 3;
        }
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 24);

    field_114_tlvInfo = tlvInfo;
    field_118_next_state_frame = gnFrameCount_507670;
    field_110_disabled_resources = static_cast<short>(pTlv->field_20_disabled_resources);

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbebombResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDebrisID00, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBgexpldResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kGrenflshResID, 1, 0);

    if (!(field_110_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 1, 0);
    }

    if (!(field_110_disabled_resources & 4))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, 1, 0);
    }

    if (!(field_110_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, 1, 0);
    }

    if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
    {
        field_1BC_flags &= ~2u;
        field_C0_r = 80;
        field_C2_g = 90;
        field_C4_b = 110;
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, 1, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, 1, 0);
    }

    const FP gridSnap = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    field_6_flags.Set(Options::eInteractive_Bit8);

    field_D4_collection_rect.x = field_A8_xpos - (gridSnap / FP_FromInteger(2));
    field_D4_collection_rect.y = field_AC_ypos - gridSnap;
    field_D4_collection_rect.w = field_A8_xpos + (gridSnap / FP_FromInteger(2));
    field_D4_collection_rect.h = field_AC_ypos;

    return this;
}

void UXB::InitBlinkAnim()
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBombflshResID, 1, 0);
    if (field_11C_anim.Init_402D20(
        372,
        gObjList_animations_505564,
        this,
        37,
        21,
        ppRes,
        1,
        0,
        0))
    {
        field_11C_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_11C_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);

        field_11C_anim.field_C_layer = field_10_anim.field_C_layer;
        field_11C_anim.field_14_scale = field_BC_sprite_scale;
        field_11C_anim.field_8_r = 128;
        field_11C_anim.field_9_g = 128;
        field_11C_anim.field_A_b = 128;
        field_11C_anim.field_B_render_mode = 1;
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }
}


BaseGameObject* UXB::dtor_4891B0()
{
    SetVTable(this, 0x4BD680);
    if (field_10C_state != 2 || static_cast<int>(gnFrameCount_507670) < field_118_next_state_frame)
    {
        gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, -1, 0, 1);
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbebombResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDebrisID00, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBgexpldResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kGrenflshResID, 0, 0));

    if (!(field_110_disabled_resources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 0, 0));
    }

    if (!(field_110_disabled_resources & 4))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, 0, 0));
    }

    if (!(field_110_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, 0, 0));
    
    field_11C_anim.vCleanUp();

    field_6_flags.Clear(Options::eInteractive_Bit8);

    return dtor_401000();
}

BaseGameObject* UXB::Vdtor_489C70(signed int flags)
{
    dtor_4891B0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void UXB::VScreenChanged()
{
    VScreenChanged_489BD0();
}

void UXB::VScreenChanged_489BD0()
{

    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        if (field_10E_starting_state == 3 && field_10C_state != 3)
        {
            gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        else if (field_10E_starting_state != 0 || field_10C_state != 3)
        {
            gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, 0, 1u, 0);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

__int16 UXB::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_489AB0(pFrom);
}

__int16 UXB::VTakeDamage_489AB0(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eAbe_43:
        if (field_10C_state == 3)
        {
            return 0;
        }
        break;

    case Types::eAbilityRing_69:
    case Types::eExplosion_74:
    case Types::eShrykull_85:
        break;

    default:
        return 0;
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);

    auto pBaseBomb = ao_new<BaseBomb>();
    if (pBaseBomb)
    {
        pBaseBomb->ctor_4173A0(
            field_A8_xpos,
            field_AC_ypos,
            0,
            field_BC_sprite_scale);
    }

    field_10C_state = 2;
    field_118_next_state_frame = gnFrameCount_507670;

    return 1;
}

void UXB::VOnThrowableHit(BaseGameObject* pFrom)
{
    VOnThrowableHit_489A30(pFrom);
}

void UXB::VOnThrowableHit_489A30(BaseGameObject* /*pFrom*/)
{
    auto pBaseBomb = ao_new<BaseBomb>();
    if (pBaseBomb)
    {
        pBaseBomb->ctor_4173A0(
            field_A8_xpos,
            field_AC_ypos,
            0,
            field_BC_sprite_scale);
    }
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_10C_state = 2;
    field_118_next_state_frame = gnFrameCount_507670;
}

void UXB::VOnPickUpOrSlapped()
{
    VOnPickUpOrSlapped_4897E0();
}

void UXB::VOnPickUpOrSlapped_4897E0()
{
    if (field_10C_state != 2)
    {
        if (field_10C_state != 3 || field_118_next_state_frame > static_cast<int>(gnFrameCount_507670))
        {
            if (field_1BA_red_blink_count)
            {
                field_10C_state = 2;
                field_118_next_state_frame = gnFrameCount_507670 + 2;
            }
            else
            {
                field_11C_anim.Set_Animation_Data_402A40(372, 0);
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
                {
                    SFX_Play_43AD70(SoundEffect::GreenTick_3, 35, 0);
                }
                field_10_anim.Set_Animation_Data_402A40(7812, 0);
                field_10C_state = 3;
                field_118_next_state_frame = gnFrameCount_507670 + 10;
            }
        }
        else
        {
            field_10C_state = 0;
            field_8_update_delay = 6;
            field_10_anim.Set_Animation_Data_402A40(7740, 0);
            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                SFX_Play_43AD70(SoundEffect::RedTick_4, 35, 0);
            }
        }
    }
}


void UXB::VUpdate()
{
    VUpdate_489380();
}

void UXB::VUpdate_489380()
{
    switch (field_10C_state)
    {
    case 0:
        if (IsColliding_489900())
        {
            field_10C_state = 2;
            field_118_next_state_frame = gnFrameCount_507670 + 2;
        }
        else if (field_118_next_state_frame <= static_cast<int>(gnFrameCount_507670))
        {
            field_10C_state = 1;
            field_11C_anim.Set_Animation_Data_402A40(384, 0);
        }
        break;

    case 1:
        if (IsColliding_489900())
        {
            field_10C_state = 2;
            field_118_next_state_frame = gnFrameCount_507670 + 2;
        }
        else if (field_118_next_state_frame <= static_cast<int>(gnFrameCount_507670))
        {
            if (field_1BA_red_blink_count)
            {
                field_1BA_red_blink_count--;
                if (field_1BA_red_blink_count == 0)
                {
                    field_11C_anim.LoadPal_403090(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kGrenflshResID, 0, 0), 0);
                    field_1BC_flags &= ~2u;
                }
            }
            else
            {
                const FrameInfoHeader* pFrameInfo = field_11C_anim.Get_FrameHeader_403A00(-1);

                const FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_11C_anim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

                field_11C_anim.LoadPal_403090(
                    field_11C_anim.field_20_ppBlock,
                    pFrameHeader->field_0_clut_offset);

                field_1BC_flags |= 2u;

                field_1B6_pattern_index++;

                if (field_1B6_pattern_index >= field_1B4_pattern_length)
                {
                    field_1B6_pattern_index = 0;
                }

                // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                field_1BA_red_blink_count = (field_1B8_pattern / static_cast<int>(pow(10, field_1B4_pattern_length - field_1B6_pattern_index - 1))) % 10;
            }

            field_11C_anim.Set_Animation_Data_402A40(372, 0);

            if (((field_1BC_flags) >> 1) & 1)
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
                {
                    SFX_Play_43AD70(SoundEffect::RedTick_4, 35, 0);
                }
            }
            else if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                SFX_Play_43AD70(SoundEffect::GreenTick_3, 35, 0);
            }
            field_10C_state = 0;
            field_118_next_state_frame = gnFrameCount_507670 + 10; // UXB change color delay
        }
        break;

    case 2:
        if (static_cast<int>(gnFrameCount_507670) >= field_118_next_state_frame)
        {
            auto explosion = ao_new<BaseBomb>();
            if (explosion)
            {
                explosion->ctor_4173A0(
                    field_A8_xpos,
                    field_AC_ypos,
                    0,
                    field_BC_sprite_scale);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        break;
    }

    if (field_10C_state != 2)
    {
        if (Event_Get_417250(kEventDeathReset_4))
        {
            if (field_10E_starting_state != 3 || field_10C_state == 3)
            {
                if (field_10E_starting_state != 0 || field_10C_state != 3)
                {
                    gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, 0, 1u, 0);
                }
                else
                {
                    gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
                }
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

__int16 UXB::IsColliding_489900()
{
    PSX_RECT uxbBound = {};
    VGetBoundingRect(&uxbBound, 1);

    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_10A_flags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
        {
            if (pObj->field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                PSX_RECT objBound = {};
                pObj->VGetBoundingRect(&objBound, 1);

                const int objX = FP_GetExponent(pObj->field_A8_xpos);
                const int objY = FP_GetExponent(pObj->field_AC_ypos);

                if (objX > uxbBound.x &&
                    objX < uxbBound.w &&
                    objY < uxbBound.h + 5 &&
                    uxbBound.x <= objBound.w &&
                    uxbBound.w >= objBound.x &&
                    uxbBound.h >= objBound.y &&
                    uxbBound.y <= objBound.h &&
                    pObj->field_BC_sprite_scale == field_BC_sprite_scale)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}


void UXB::VRender(PrimHeader** ppOt)
{
    VRender_4896C0(ppOt);
}

void UXB::VRender_4896C0(PrimHeader** ppOt)
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_11C_anim.vRender(
            FP_GetExponent(field_A8_xpos
                + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
                - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos
                + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - FP_NoFractional(field_BC_sprite_scale * FP_FromInteger(12)))
                - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_11C_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

BaseGameObject* UXB::VDestructor(signed int flags)
{
    return Vdtor_489C70(flags);
}

}

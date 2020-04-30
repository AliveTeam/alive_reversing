#include "stdafx.h"
#include "MineCar.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Sound/Midi.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "Function.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "ParticleBurst.hpp"
#include "ScreenShake.hpp"

MineCar* MineCar::ctor_46BC80(Path_MineCar* pTlv, int tlvInfo, int /*a4*/, int /*a5*/, int /*a6*/)
{
    ctor_408240(0);
    SetVTable(&field_124_anim, 0x544290);
    SetVTable(this, 0x5461FC);
    field_4_typeId = Types::eMineCar_89;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kUnknownID_6013); // TODO: Id
    Animation_Init_424E10(
        20812,
        128,
        62,
        ppRes,
        1,
        1);

    field_11E_scale = pTlv->field_10_scale;
    field_120_max_damage = pTlv->field_12_max_damage;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_DC_bApplyShadows |= 2u;
    field_1BE = 0;
    field_20_animation.field_C_render_layer = 26;
    field_CC_sprite_scale = FP_FromInteger(1);
    field_D6_scale = 1;

    if (field_11E_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 7;
        field_D6_scale = 0;
    }

    MapFollowMe_408D10(TRUE);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }
    
    field_118_tlvInfo = tlvInfo;
    field_11C_state = MineCarStates::eState_0_ParkedWithoutAbe;

    LoadAnimation_46BF80(&field_124_anim);

    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeCarResId);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMetalResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);

    field_1C0 = 0;
    field_1C2_falling_counter = 0;

    // What was pressed before we started to move
    field_1D4_previous_input = static_cast<short>(sInputKey_ThrowItem_5550F4);

    // Set to a key that keeps the car moving, for example if moving right and we hit a right wall that a car
    // can travel "up" then we set this key to "up" such that holding down "right" automatically moves the car up.
    field_1D6_continue_move_input = static_cast<short>(sInputKey_ThrowItem_5550F4);

    field_1CC_spawned_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_1CE_spawned_camera = gMap_5C3030.sCurrentCamId_5C3034;
    field_1D0_sound_channels_mask = 0;
    field_1C4 = 0;

    return this;
}

BaseGameObject* MineCar::VDestructor(signed int flags)
{
    return vdtor_46BF50(flags);
}

void MineCar::VUpdate()
{
    vUpdate_46C010();
}

void MineCar::VRender(int** pOrderingTable)
{
    vRender_46E760(pOrderingTable);
}

void MineCar::VScreenChanged()
{
    vScreenChanged_46F800();
}

void MineCar::VStopAudio()
{
    vStopAudio_46F9C0();
}

int MineCar::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_467E10(reinterpret_cast<MineCar_SaveState*>(pSaveBuffer));
}

__int16 MineCar::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_46F7D0(pFrom);
}

int CC MineCar::CreateFromSaveState_467740(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const MineCar_SaveState*>(pBuffer);
    auto pTlv = static_cast<Path_MineCar*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4C_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kUnknownID_6013, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("BAYROLL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeCarResId, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABECAR.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMetalResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("METAL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    auto pMineCar = alive_new<MineCar>();
    if (pMineCar)
    {
        pMineCar->ctor_46BC80(pTlv, pState->field_4C_tlvInfo, 0, 0, 0);
    }

    if (pState->field_5A_bAbeInCar)
    {
        sControlledCharacter_5C1B8C = pMineCar;
    }

    pMineCar->field_FC_pPathTLV = nullptr;
    pMineCar->field_100_pCollisionLine = nullptr;

    pMineCar->field_B8_xpos = pState->field_4_xpos;
    pMineCar->field_BC_ypos = pState->field_8_ypos;

    pMineCar->field_C4_velx = pState->field_C_velx;
    pMineCar->field_C8_vely = pState->field_10_vely;

    pMineCar->field_C0_path_number = pState->field_18_path_number;
    pMineCar->field_C2_lvl_number = pState->field_1A_lvl_number;

    pMineCar->field_CC_sprite_scale = pState->field_14_sprite_scale;
    
    pMineCar->field_D0_r = pState->field_1C_r;
    pMineCar->field_D2_g = pState->field_1E_g;
    pMineCar->field_D4_b = pState->field_20_b;
    
    pMineCar->field_106_current_motion = pState->field_28_current_motion;

    int remapped1 = 0;
    switch (pState->field_24_frame_table)
    {
    case 10860:
        remapped1 = 20788;
        break;
    case 10884:
        remapped1 = 20812;
        break;
    case 10896:
        remapped1 = 20824;
        break;
    case 10908:
        remapped1 = 20836;
        break;
    case 10920:
        remapped1 = 20848;
        break;
    case 10944:
        remapped1 = 20872;
        break;
    case 10972:
        remapped1 = 20900;
        break;
    default:
        break;
    }

    pMineCar->field_20_animation.Set_Animation_Data_409C80(remapped1, nullptr);
    pMineCar->field_20_animation.field_92_current_frame = pState->field_2A_current_anim_frame;


    pMineCar->field_20_animation.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pMineCar->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_xFlip & 1);
    pMineCar->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E & 1);

    pMineCar->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F & 1);

    if (IsLastFrame(&pMineCar->field_20_animation))
    {
        pMineCar->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    int remapped2 = 0;
    switch (pState->field_38_frame_table_offset2)
    {
    case 10860:
        remapped2 = 20788;
        break;
    case 10884:
        remapped2 = 20812;
        break;
    case 10896:
        remapped2 = 20824;
        break;
    case 10908:
        remapped2 = 20836;
        break;
    case 10920:
        remapped2 = 20848;
        break;
    case 10944:
        remapped2 = 20872;
        break;
    case 10972:
        remapped2 = 20900;
        break;
    default:
        break;
    }

    pMineCar->field_124_anim.Set_Animation_Data_409C80(remapped2, nullptr);
    pMineCar->field_124_anim.field_92_current_frame = pState->field_2A_current_anim_frame;


    // TODO: OG Bug same flags but save state saves 2 sets one for each anim ??
    pMineCar->field_124_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_xFlip & 1);
    pMineCar->field_124_anim.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E & 1);


    pMineCar->field_124_anim.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pMineCar->field_10C_health = pState->field_3C_health;
    pMineCar->field_106_current_motion = pState->field_40;
    
    pMineCar->field_108_next_motion = pState->field_42_next_motion;

    pMineCar->field_F8_LastLineYPos = FP_FromInteger(pState->field_44_last_line_ypos);

    pMineCar->field_114_flags.Set(Flags_114::e114_Bit9);

    pMineCar->field_104_collision_line_type = pState->field_46_collision_line_type;
    pMineCar->field_118_tlvInfo = pState->field_4C_tlvInfo;

    pMineCar->field_11C_state = pState->field_50_state;
    pMineCar->field_turnDirection_1BC = pState->field_52;
    pMineCar->field_1BE = pState->field_54;
    pMineCar->field_1C0 = pState->field_56;

    pMineCar->field_1C2_falling_counter = pState->field_58;

    pMineCar->field_1C8_frame_mod_16 = pState->field_5C;

    pMineCar->field_1CC_spawned_path = pState->field_60_spawned_path;
    pMineCar->field_1CE_spawned_camera = pState->field_62_spanwed_camera;

    pMineCar->field_1D4_previous_input = pState->field_64_throw_item_key1;
    pMineCar->field_1D6_continue_move_input = pState->field_66_continue_move_input;

    if (pMineCar->field_C4_velx < (ScaleToGridSize_4498B0(FP_FromInteger(1)) / FP_FromInteger(4)))
    {
        pMineCar->field_1C4 = 7;
    }

    return sizeof(MineCar_SaveState);
}

void MineCar::LoadAnimation_46BF80(Animation* pAnim)
{
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kUnknownID_6013);
    if (pAnim->Init_40A030(20824, gObjList_animations_5C1A24, this, 130, 62u, ppRes, 1, 0, 0))
    {
        pAnim->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnim->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        pAnim->field_14_scale = field_CC_sprite_scale;
        pAnim->field_8_r = 128;
        pAnim->field_9_g = 128;
        pAnim->field_A_b = 128;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void MineCar::vStopAudio_46F9C0()
{
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
}

void MineCar::vScreenChanged_46F800()
{
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    BaseGameObject::VScreenChanged();
}

MineCar* MineCar::vdtor_46BF50(signed int flags)
{
    dtor_46F2A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void MineCar::dtor_46F2A0()
{
    SetVTable(this, 0x5461FC);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    field_124_anim.vCleanUp_40C630();
    dtor_4080B0();
}

BOOL MineCar::CheckRoofCollision_46F6B0(FP hitX, FP hitY)
{
    PathLine *pPathLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(
        hitX + field_B8_xpos,
        field_BC_ypos,
        hitX + field_B8_xpos,
        field_BC_ypos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 8 : 128) != 0;
}

BOOL MineCar::CheckFloorCollision_46F730(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;

    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        hitX + field_B8_xpos,
        field_BC_ypos - FP_FromInteger(4),
        hitX + field_B8_xpos,
        field_BC_ypos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16))
    {
        return 0;
    }

    if (field_BC_ypos > hitY)
    {
        field_BC_ypos = hitY;
    }

    return 1;
}

void MineCar::vRender_46E760(int** pOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        short r = field_D2_g;
        short g = field_D0_r;
        short b = field_D4_b;
        
        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        if (field_DC_bApplyShadows & 1)
        {
           ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                FP_GetExponent(field_B8_xpos),
                (bRect.h + bRect.y) / 2,
                field_D6_scale,
                &r,
                &g,
                &b);
        }

        field_124_anim.field_8_r = static_cast<BYTE>(r);
        field_124_anim.field_9_g = static_cast<BYTE>(g);
        field_124_anim.field_A_b = static_cast<BYTE>(b);

        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos + FP_FromInteger(30), field_BC_ypos, 0)|| 
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)), 0) ||
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos - FP_FromInteger(30), field_BC_ypos, 0))
        {
            field_124_anim.vRender_40B820(
                FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                pOt,
                0,
                0);
        }

        PSX_RECT frameRect = {};
        field_124_anim.Get_Frame_Rect_409E10(&frameRect);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_5BB5F4->field_3A_idx);
        BaseAnimatedWithPhysicsGameObject::VRender(pOt);
    }
}

void MineCar::Stop_46E570()
{
    field_11C_state = MineCarStates::eState_1_ParkedWithAbe;
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    SFX_Play_46FA90(101u, 127, field_CC_sprite_scale);
    field_20_animation.Set_Animation_Data_409C80(20836, 0);
    field_124_anim.Set_Animation_Data_409C80(20824, 0);
    field_1C4 = 0;
    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
}

void MineCar::Move_46E640(unsigned __int16 frameTabeOffset, FP velX, FP velY, InputCommands input, MineCarDirs turnDirection, char bChangeDirection)
{
    field_20_animation.Set_Animation_Data_409C80(frameTabeOffset, nullptr);
    field_11C_state = MineCarStates::eState_2_Moving;
    field_1C8_frame_mod_16 = static_cast<int>(sGnFrame_5C1B84) % 16;

    if (!field_1D0_sound_channels_mask)
    {
        field_1D0_sound_channels_mask = SFX_Play_46FA90(100, 127, field_CC_sprite_scale);
    }

    field_124_anim.Set_Animation_Data_409C80(20848, nullptr);
    field_C4_velx = velX;
    field_C8_vely = velY;

    if (sInputObject_5BD4E0.isPressed(input))
    {
        field_1D4_previous_input = static_cast<short>(input);
    }

    field_turnDirection_1BC = turnDirection;

    field_124_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards, bChangeDirection);
}

__int16 MineCar::IsBlocked_46F4A0(__int16 a2, int /*a3*/)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP k12Scaled = field_CC_sprite_scale * FP_FromInteger(12);

    switch (a2)
    {
    case 0:
        {
            const FP k60Scaled = field_CC_sprite_scale * FP_FromInteger(60);
            if (!CheckRoofCollision_46F6B0(FP_FromInteger(4) - (k12Scaled + kGridSize), -(k60Scaled) + FP_FromInteger(1)))
            {
                if (!CheckRoofCollision_46F6B0(k12Scaled + kGridSize - FP_FromInteger(4), -((field_CC_sprite_scale * FP_FromInteger(60)) + FP_FromInteger(1))))
                {
                    return 0;
                }
            }
        }
        break;

    case 3:
        {
            if (!CheckFloorCollision_46F730(FP_FromInteger(4) - (k12Scaled + kGridSize), FP_FromInteger(1)))
            {
                if (!CheckFloorCollision_46F730(k12Scaled + kGridSize - FP_FromInteger(4), FP_FromInteger(1)) &&
                    !CheckFloorCollision_46F730(-FP_FromInteger(10), FP_FromInteger(1)) &&
                    !CheckFloorCollision_46F730(FP_FromInteger(10), FP_FromInteger(1)))
                {
                    return 0;
                }
            }
        }
        break;
    }

    return 1;
}

__int16 MineCar::FollowDirection_46EA00()
{
    const FP k60Scaled = field_CC_sprite_scale * FP_FromInteger(60);
    const FP k12Scaled = field_CC_sprite_scale * FP_FromInteger(12);
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP k1 = FP_FromInteger(1);
    const FP k4 = FP_FromInteger(4);
    const FP k2 = FP_FromInteger(2);
    const FP k0 = FP_FromInteger(0);

    if ((WallHit_408750(k60Scaled * FP_FromDouble(0.5), k12Scaled + kGridSize + k1) && field_C4_velx > k0) ||
        (WallHit_408750(k60Scaled * FP_FromDouble(0.5), -(k12Scaled + kGridSize)) && field_C4_velx < k0))
    {
        if (field_turnDirection_1BC == MineCarDirs::eUp_3)
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Up_5550D8;
        }
        else
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Down_5550DC;
        }
        return 1;
    }

    if ((CheckFloorCollision_46F730(k0, k1) && field_C8_vely > k0) ||
        (CheckRoofCollision_46F6B0(k0, -k60Scaled) && field_C8_vely < k0))
    {
        if (field_turnDirection_1BC == MineCarDirs::eLeft_2)
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Left_5550D4;
        }
        else
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Right_5550D0;
        }
        return 1;
    }

    if (field_turnDirection_1BC == MineCarDirs::eUp_3)
    {
        const bool bNoFloorRight =
            CheckFloorCollision_46F730(k4 - (k12Scaled + kGridSize), k4) ||
            CheckFloorCollision_46F730(k12Scaled + kGridSize + k2, k4);

        const bool bNoFloorLeft =
            CheckFloorCollision_46F730(-(k12Scaled + kGridSize + k2), k4) ||
            CheckFloorCollision_46F730(k12Scaled + kGridSize - k4, k4);

        if ((field_C4_velx > k0 && !bNoFloorRight) || (field_C4_velx < k0 && !bNoFloorLeft))
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Down_5550DC;
            return 1;
        }
    }
    else if (field_turnDirection_1BC == MineCarDirs::eDown_0)
    {
        const bool bRoofRight = 
            CheckRoofCollision_46F6B0(k4 - (k12Scaled + kGridSize), -k60Scaled) ||
            CheckRoofCollision_46F6B0(k12Scaled + kGridSize + k2, -k60Scaled);

        const bool bRoofLeft = 
            CheckRoofCollision_46F6B0(-(k12Scaled + kGridSize + k2), -k60Scaled) ||
            CheckRoofCollision_46F6B0(k12Scaled + kGridSize - k4, -k60Scaled);

        if ((field_C4_velx > k0 && !bRoofRight) || (field_C4_velx < k0 && !bRoofLeft))
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Up_5550D8;
            return 1;
        }
    }
    else if (field_turnDirection_1BC == MineCarDirs::eLeft_2)
    {
        const bool bWall1 =
            WallHit_408750(k60Scaled - k2, k12Scaled + kGridSize + k4) ||
            WallHit_408750(-k2, k12Scaled + kGridSize + k4);

        const bool bWall2 =
            WallHit_408750(k60Scaled + k1, k12Scaled + kGridSize + k4) ||
            WallHit_408750(k1, k12Scaled + kGridSize + k4);

        if ((field_C8_vely > k0 && !bWall1) || (field_C8_vely < k0 && !bWall2))
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Right_5550D0;
            return 1;
        }
    }
    else if (field_turnDirection_1BC == MineCarDirs::eRight_1)
    {
        const bool bWall1 =
            WallHit_408750(k60Scaled - k2, -(k12Scaled + kGridSize + k4)) ||
            WallHit_408750(-k2, -(k12Scaled + kGridSize + k4));

        const bool bWall2 =
            WallHit_408750(k60Scaled + k1, -(k12Scaled + kGridSize + k4)) ||
            WallHit_408750(k1, -(k12Scaled + kGridSize + k4));

        if ((field_C8_vely > k0 && !bWall1) || (field_C8_vely < k0 && !bWall2))
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Left_5550D4;
            return 1;
        }
    }

    return 0;
}

void MineCar::RunThingsOver_46F380()
{
    PSX_RECT ourRect = {};
    vGetBoundingRect_424FD0(&ourRect, 1);
    ourRect.h += 6;

    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // You can't run yourself over with a mine car it seems.
            if (pObj->field_4_typeId != Types::eAbe_69)
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
                if ((pAliveObj->field_CC_sprite_scale == field_CC_sprite_scale || pAliveObj->field_4_typeId == Types::eSlog_126) && field_CC_sprite_scale != FP_FromDouble(0.5))
                {
                    PSX_RECT targetRect = {};
                    pAliveObj->vGetBoundingRect_424FD0(&targetRect, 1);

                    if (PSX_Rects_overlap_no_adjustment(&ourRect, &targetRect))
                    {
                        // Get run over by the mine car
                        pAliveObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }
}

__int16 MineCar::vTakeDamage_46F7D0(BaseGameObject* /*pFrom*/)
{
    if (!field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int MineCar::vGetSaveState_467E10(MineCar_SaveState* pState)
{
    pState->field_0_type = Types::eMineCar_89;

    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;

    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_18_path_number = field_C0_path_number;
    pState->field_1A_lvl_number = field_C2_lvl_number;

    pState->field_14_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_r = field_D0_r;
    pState->field_1E_g = field_D2_g;
    pState->field_20_b = field_D4_b;

    pState->field_28_current_motion = field_106_current_motion;
    pState->field_2A_current_anim_frame = field_20_animation.field_92_current_frame;
    pState->field_2C_frame_change_counter = field_20_animation.field_E_frame_change_counter;

    pState->field_2F = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_22_xFlip = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_2E = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);


    switch (field_20_animation.field_18_frame_table_offset)
    {
    case 20788:
        pState->field_24_frame_table = 10860;
        break;
    case 20812:
        pState->field_24_frame_table = 10884;
        break;
    case 20824:
        pState->field_24_frame_table = 10896;
        break;
    case 20836:
        pState->field_24_frame_table = 10908;
        break;
    case 20848:
        pState->field_24_frame_table = 10920;
        break;
    case 20872:
        pState->field_24_frame_table = 10944;
        break;
    case 20900:
        pState->field_24_frame_table = 10972;
        break;
    default:
        break;
    }

    pState->field_34 = field_124_anim.field_92_current_frame;
    pState->field_36 = field_124_anim.field_E_frame_change_counter;

    pState->field_32 = field_124_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_30 = field_124_anim.field_4_flags.Get(AnimFlags::eBit3_Render);

    switch (field_124_anim.field_18_frame_table_offset)
    {
    case 20788:
        pState->field_38_frame_table_offset2 = 10860;
        break;
    case 20812:
        pState->field_38_frame_table_offset2 = 10884;
        break;
    case 20824:
        pState->field_38_frame_table_offset2 = 10896;
        break;
    case 20836:
        pState->field_38_frame_table_offset2 = 10908;
        break;
    case 20848:
        pState->field_38_frame_table_offset2 = 10920;
        break;
    case 20872:
        pState->field_38_frame_table_offset2 = 10944;
        break;
    case 20900:
        pState->field_38_frame_table_offset2 = 10972;
        break;
    default:
        break;
    }

    pState->field_3C_health = field_10C_health;
    pState->field_42_next_motion = field_108_next_motion;

    pState->field_40 = field_106_current_motion;

    pState->field_44_last_line_ypos = FP_GetExponent(FP_Abs(field_F8_LastLineYPos));

    if (field_100_pCollisionLine)
    {
        pState->field_46_collision_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_46_collision_line_type = -1;
    }

    pState->field_5A_bAbeInCar = (this == sControlledCharacter_5C1B8C);
    pState->field_4C_tlvInfo = field_118_tlvInfo;
    pState->field_50_state = field_11C_state;

    pState->field_52 = field_turnDirection_1BC;
    pState->field_54 = field_1BE;
    pState->field_56 = field_1C0;

    pState->field_58 = field_1C2_falling_counter;
    pState->field_5C = field_1C8_frame_mod_16;

    pState->field_60_spawned_path = field_1CC_spawned_path;
    pState->field_62_spanwed_camera = field_1CE_spawned_camera;

    pState->field_64_throw_item_key1 = field_1D4_previous_input;
    pState->field_66_continue_move_input = field_1D6_continue_move_input;

    return sizeof(MineCar_SaveState);
}

void MineCar::vUpdate_REAL_46C010()
{
    NOT_IMPLEMENTED();
}

void MineCar::vUpdate_46C010()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type != -1)
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
            field_104_collision_line_type = 0;
        }
        else
        {
            field_100_pCollisionLine = nullptr;
            field_104_collision_line_type = 0;
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const FP kGridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    switch (field_11C_state)
    {
    case MineCarStates::eState_0_ParkedWithoutAbe:
        State_0();
        break;

    case MineCarStates::eState_1_ParkedWithAbe:
        State_1();
        break;

    case MineCarStates::eState_2_Moving:
        State_2();
        break;

    case MineCarStates::eState_3_Falling:
        State_3(); 
        break;

    default:
        break;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_DoAction_5550E4))
    {
        if ((field_turnDirection_1BC != MineCarDirs::eUp_3 && !IsBlocked_46F4A0(3, 0)) || !IsBlocked_46F4A0(3, 0))
        {
            if (field_11C_state != MineCarStates::eState_0_ParkedWithoutAbe)
            {
                field_11C_state = MineCarStates::eState_3_Falling;
            }
        }
    }

    if (field_turnDirection_1BC != MineCarDirs::eUp_3 || IsBlocked_46F4A0(3, 0))
    {
        return;
    }

    const FP k12Scaled = field_CC_sprite_scale * FP_FromInteger(12);

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos - (kGridScale + k12Scaled), 
        field_BC_ypos + field_C8_vely - ((k12Scaled + kGridScale) * FP_FromDouble(0.5)),
        field_B8_xpos + k12Scaled + kGridScale,
        field_BC_ypos + field_C8_vely - ((k12Scaled + kGridScale) * FP_FromDouble(0.5)),
        &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x1000 : 0x8000))
    {
        field_11C_state = MineCarStates::eState_3_Falling;
    }
}

void MineCar::State_0()
{
    PSX_RECT carRect = {};
    vGetBoundingRect_424FD0(&carRect, 1);

    PSX_RECT abeRect = {};
    sActiveHero_5C1B68->vGetBoundingRect_424FD0(&abeRect, 1);

    if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_117_In_MineCar_4587C0 &&
        PSX_Rects_overlap_4FA0B0(&carRect, &abeRect) && 
        sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        field_20_animation.Set_Animation_Data_409C80(20836, 0);
        field_11C_state = MineCarStates::eState_1_ParkedWithAbe;
        sControlledCharacter_5C1B8C = this;
        field_20_animation.field_C_render_layer = 35;
        field_124_anim.field_C_render_layer = 35;
        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 16;
            field_124_anim.field_C_render_layer = 16;
        }
        SFX_Play_46FBA0(57u, 100, 500, field_CC_sprite_scale);
    }
}

const FP mineCarHeightUnscaled = FP_FromInteger(60);
const FP mineCarWidthUnscaled = FP_FromInteger(12);

void MineCar::State_1()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;

    VCheckCollisionLineStillValid_408A40(10);

    //Abe exits minecar
    if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_117_In_MineCar_4587C0)
    {
        sActiveHero_5C1B68->field_B8_xpos = field_B8_xpos;
        sActiveHero_5C1B68->field_BC_ypos = field_BC_ypos;
        field_11C_state = MineCarStates::eState_0_ParkedWithoutAbe;
        field_124_anim.Set_Animation_Data_409C80(20824, 0);
        field_20_animation.Set_Animation_Data_409C80(20812, 0);
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        field_1CC_spawned_path = gMap_5C3030.sCurrentPathId_5C3032;
        field_1CE_spawned_camera = gMap_5C3030.sCurrentCamId_5C3034;
        sActiveHero_5C1B68->VCheckCollisionLineStillValid_408A40(10);

        SFX_Play_46FBA0(57u, 100, 500, field_CC_sprite_scale);

        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = 7;
            field_124_anim.field_C_render_layer = 7;
        }
        else
        {
            field_20_animation.field_C_render_layer = 26;
            field_124_anim.field_C_render_layer = 26;
        }

        return;
    }

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    InputCommands inputKey = sInputKey_Right_5550D0;

    if ((sInputObject_5BD4E0.isPressed(inputKey) ||
        (sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
        (unsigned __int16) field_1D6_continue_move_input == inputKey &&
        field_1D4_previous_input != (unsigned __int16) inputKey &&
        field_turnDirection_1BC != MineCarDirs::eUp_3 &&
        field_turnDirection_1BC != MineCarDirs::eDown_0))
        && !(WallHit_408750(mineCarHeight * FP_FromDouble(0.5), mineCarWidthAdjusted + FP_FromInteger(1)) &&
        WallHit_408750(mineCarHeight - FP_FromInteger(8), mineCarWidthAdjusted + FP_FromInteger(1))))
    {
        const FP rayCast1 = (kGridSize / FP_FromInteger(4)) + field_B8_xpos;
        const FP rayCast2 = field_BC_ypos - mineCarHeight;
        const FP rayCast3 = (kGridSize / FP_FromInteger(4)) + field_B8_xpos;
        const FP rayCast4 = field_BC_ypos;
        const FP velX = kGridSize / FP_FromInteger(4);
        const FP velY = FP_FromInteger(0);
        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(2);
        const FP hitX2 = FP_FromInteger(4) - mineCarWidthAdjusted;
        const unsigned short frameTableOffset = 20872u;

        if (HandleState1Move(&MineCar::CheckFloorCollision_46F730, hitX, FP_FromInteger(4), hitX2, frameTableOffset, MineCarDirs::eUp_3, 0,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x800, 0x4000, velX, velY, inputKey, false, false) ||
            HandleState1Move(&MineCar::CheckRoofCollision_46F6B0, hitX, -mineCarHeight, hitX2, frameTableOffset, MineCarDirs::eDown_0, 1,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x2000, 0x10000, velX, velY, inputKey, false, false))
        {
            return;
        }
    }
    else
    {
        if (WallHit_408750(mineCarHeight * FP_FromDouble(0.5), mineCarWidthAdjusted + FP_FromInteger(1)))
        {
            if (sInputObject_5BD4E0.isPressed(inputKey))
            {
                if (field_turnDirection_1BC != MineCarDirs::eLeft_2 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
    }

    inputKey = sInputKey_Left_5550D4;

    if ((sInputObject_5BD4E0.isPressed(inputKey) ||
        (sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
        (unsigned __int16) field_1D6_continue_move_input == inputKey &&
        field_1D4_previous_input != (unsigned __int16) inputKey &&
        field_turnDirection_1BC != MineCarDirs::eUp_3 &&
        field_turnDirection_1BC != MineCarDirs::eDown_0))
        && !(WallHit_408750(mineCarHeight * FP_FromDouble(0.5), -mineCarWidthAdjusted)))
    {
        const FP rayCast1 = field_B8_xpos - (kGridSize / FP_FromInteger(4));
        const FP rayCast2 = field_BC_ypos - mineCarHeight;
        const FP rayCast3 = field_B8_xpos - (kGridSize / FP_FromInteger(4));
        const FP rayCast4 = field_BC_ypos;
        const FP velX = -(kGridSize / FP_FromInteger(4));
        const FP velY = FP_FromInteger(0);
        const FP hitX = mineCarWidthAdjusted - FP_FromInteger(4);
        const FP hitX2 = -(mineCarWidthAdjusted + FP_FromInteger(2));
        const unsigned short frameTableOffset = 20900u;

        if (HandleState1Move(&MineCar::CheckFloorCollision_46F730, hitX, FP_FromInteger(4), hitX2, frameTableOffset, MineCarDirs::eUp_3, 1,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x800, 0x4000, velX, velY, inputKey, false, false) ||
            HandleState1Move(&MineCar::CheckRoofCollision_46F6B0, hitX, -mineCarHeight, hitX2, frameTableOffset, MineCarDirs::eDown_0, 0,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x2000, 0x10000, velX, velY, inputKey, false, false))
        {
            return;
        }
    }
    else
    {
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_turnDirection_1BC != MineCarDirs::eRight_1 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
            {
                SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
            }
        }
    }

    HandleUpDown();
}

bool MineCar::HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3,
    unsigned short moveArgument1, MineCarDirs moveArgument2, const char bChangeDir, FP rayCast1, FP rayCast2, FP rayCast3, FP rayCast4, const int ModelMask1, const int ModelMask2,
    FP velX, FP velY, InputCommands key, bool isVertical, bool verticalFlipXCond)
{
    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        rayCast1,
        rayCast2,
        rayCast3,
        rayCast4,
        &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? ModelMask1 : ModelMask2))
    {
        if ((this->*func)(mineCarFPFuncArg1, mineCarFPFuncArg2) ||
            (this->*func)(mineCarFPFuncArg3, mineCarFPFuncArg2))
        {
            if (isVertical)
            {
                if (verticalFlipXCond)
                {
                    if (hitX > field_B8_xpos)
                    {
                        Move_46E640(moveArgument1, velX, velY, key, moveArgument2, bChangeDir);
                    }

                }
                else if (hitX < field_B8_xpos)
                {
                    Move_46E640(moveArgument1, velX, velY, key, moveArgument2, bChangeDir);
                }
            }
            else
            {
                Move_46E640(moveArgument1, velX, velY, key, moveArgument2, bChangeDir);
            }
            if (sInputObject_5BD4E0.isPressed(key))
            {
                return true;
            }
        }
    }
    return false;
}

void MineCar::HandleUpDown()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;
    const FP k5Scaled = FP_FromInteger(5) * field_CC_sprite_scale;

    const FP rayCast1 = field_B8_xpos - (kGridSize + mineCarWidth);
    const FP rayCast2 = field_BC_ypos - (k5Scaled) -((mineCarWidthAdjusted) * FP_FromDouble(0.5));
    const FP rayCast3 = mineCarWidthAdjusted + field_B8_xpos;
    const FP rayCast4 = field_BC_ypos - (k5Scaled) -((mineCarWidthAdjusted) * FP_FromDouble(0.5));
    const FP velX = FP_FromInteger(0);
    const FP velY = k5Scaled;
    const unsigned short frameTableOffset = 20836u;

    InputCommands inputKey = sInputKey_Up_5550D8;

    if (sInputObject_5BD4E0.isPressed(inputKey) || (sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
        (unsigned __int16) field_1D6_continue_move_input == inputKey &&
        field_1D4_previous_input != (unsigned __int16) sInputKey_Down_5550DC &&
        field_turnDirection_1BC != MineCarDirs::eLeft_2 &&
        field_turnDirection_1BC != MineCarDirs::eRight_1 &&
        !IsBlocked_46F4A0(0, 0)))
    {
        const FP hitX = FP_FromInteger(1);
        const FP hitY = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitX2 = mineCarHeight + FP_FromInteger(1);

        if(HandleState1Move(&MineCar::WallHit_408750, hitX, hitY, hitX2, frameTableOffset, MineCarDirs::eLeft_2, 0,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x1000, 0x8000, velX, -velY, inputKey, true, true) ||
            HandleState1Move(&MineCar::WallHit_408750, hitX, -hitY, hitX2, frameTableOffset, MineCarDirs::eRight_1, 1,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x1000, 0x8000, velX, -velY, inputKey, true, false))
        {
            return;
        }
    }
    else if (IsBlocked_46F4A0(0, 0))
    {
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_turnDirection_1BC != MineCarDirs::eDown_0)
            {
                if (!(static_cast<int>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
    }

    inputKey = sInputKey_Down_5550DC;

    if ((sInputObject_5BD4E0.isPressed(inputKey) || (sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
        (unsigned __int16) field_1D6_continue_move_input == inputKey &&
        field_1D4_previous_input != (unsigned __int16) sInputKey_Up_5550D8 &&
        field_turnDirection_1BC != MineCarDirs::eLeft_2 &&
        field_turnDirection_1BC != MineCarDirs::eRight_1)) &&
        !IsBlocked_46F4A0(3, 0))
    {
        const FP hitX = -FP_FromInteger(2);
        const FP hitY = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitX2 = mineCarHeight - FP_FromInteger(1);

        if( HandleState1Move(&MineCar::WallHit_408750, hitX, hitY, hitX2, frameTableOffset, MineCarDirs::eLeft_2, 1,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x1000, 0x8000, velX, velY, inputKey, true, true) ||
            HandleState1Move(&MineCar::WallHit_408750, hitX, -hitY, hitX2, frameTableOffset, MineCarDirs::eRight_1, 0,
            rayCast1, rayCast2, rayCast3, rayCast4, 0x1000, 0x8000, velX, velY, inputKey, true, false))
        {
            return;
        }
    }
    else if (IsBlocked_46F4A0(3, 0))
    {
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_turnDirection_1BC != MineCarDirs::eUp_3 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
            {
                SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
            }
        }
    }
}

// TODO: Probably 7?
const FP dword_5461D8[9] =
{ 
    FP_FromInteger(6),
    FP_FromInteger(5),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(3),
    FP_FromInteger(2),
    FP_FromInteger(1),
    FP_FromInteger(0),
    FP_FromInteger(0)
};


void MineCar::State_2()
{
    const FP kGridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    sActiveHero_5C1B68->field_B8_xpos = field_B8_xpos;
    sActiveHero_5C1B68->field_BC_ypos = field_BC_ypos;

    if (!field_1D0_sound_channels_mask)
    {
        // Play the mine car moving sound
        field_1D0_sound_channels_mask = SFX_Play_46FA90(100u, 127, field_CC_sprite_scale);
    }

    if (FollowDirection_46EA00() && !field_1C4)
    {
        Stop_46E570();
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos - (kGridScale + (field_CC_sprite_scale * FP_FromInteger(12))), 
        field_BC_ypos + field_C8_vely - (((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5)),
        (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + field_B8_xpos,
        field_BC_ypos + field_C8_vely - (((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5)),
        &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 4096 : 0x8000) && field_C8_vely > FP_FromInteger(0))
    {
        field_11C_state = MineCarStates::eState_3_Falling;
        return;
    }

    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_C4_velx + field_B8_xpos,
        field_BC_ypos,
        field_C4_velx + field_B8_xpos,
        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)),
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 0x2000 : 0x10000)
        && field_turnDirection_1BC == MineCarDirs::eDown_0)
    {
        field_11C_state = MineCarStates::eState_3_Falling;
        return;
    }

    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_C4_velx + field_B8_xpos,
        field_BC_ypos,
        field_C4_velx + field_B8_xpos,
        field_BC_ypos - field_CC_sprite_scale * FP_FromInteger(60),
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 0x800 : 0x4000)
        && field_turnDirection_1BC == MineCarDirs::eUp_3)
    {
        Stop_46E570();
        return;
    }

    if (field_1C4)
    {
        if (field_1C4 >= 7)
        {
            Stop_46E570();
            return;
        }

        if (field_C4_velx <= FP_FromInteger(0))
        {
            field_C4_velx = -dword_5461D8[field_1C4];
        }
        else
        {
            field_C4_velx = dword_5461D8[field_1C4];
        }

        if (++field_1C4 == 2)
        {
            ++field_124_anim.field_10_frame_delay;
        }
    }

    if (sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
    {
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;
        setActiveCameraDelayedFromDir_408C40();
        RunThingsOver_46F380();
        return;
    }

    if (field_turnDirection_1BC == MineCarDirs::eUp_3 || field_turnDirection_1BC == MineCarDirs::eDown_0)
    {
        if (field_B8_xpos == FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos))) && !field_1C4)
        {
            if (field_C4_velx <= FP_FromInteger(0))
            {
                field_C4_velx = -dword_5461D8[0];
            }
            else
            {
                field_C4_velx = dword_5461D8[0];
            }
            ++field_1C4;
        }
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;
        setActiveCameraDelayedFromDir_408C40();
        RunThingsOver_46F380();
        return;
    }

    Stop_46E570();
}

void MineCar::State_3()
{
    setActiveCameraDelayedFromDir_408C40();
    field_1C2_falling_counter++;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    const FP kGridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(60), kGridScale + (field_CC_sprite_scale * FP_FromInteger(12))) ||
            WallHit_408750(FP_FromInteger(0), (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale))
        {
            field_C4_velx = FP_FromInteger(0);
            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + sControlledCharacter_5C1B8C->field_B8_xpos, 
                    sControlledCharacter_5C1B8C->field_BC_ypos - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale * FP_FromDouble(0.5)), 
                    4u, 
                    field_CC_sprite_scale, BurstType::eBigRedSparks_3, 9);
            }
            SFX_Play_46FA90(47u, 80, field_CC_sprite_scale);
        }
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)), -((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale)) ||
            WallHit_408750(FP_FromInteger(0), -((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale)))
        {
            field_C4_velx = FP_FromInteger(0);
            auto pParticleBurst2 = alive_new<ParticleBurst>();
            if (pParticleBurst2)
            {
                pParticleBurst2->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale), 
                    sControlledCharacter_5C1B8C->field_BC_ypos - (((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5)), 
                    4u, 
                    field_CC_sprite_scale, BurstType::eBigRedSparks_3, 9);
            }
            SFX_Play_46FA90(47u, 80, field_CC_sprite_scale);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    if (InAirCollision_408810(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        field_BC_ypos = hitY;
        field_F8_LastLineYPos = hitY;
        field_C8_vely = (-field_C8_vely *  FP_FromDouble(0.2));
        auto pParticleBurst3 = alive_new<ParticleBurst>();
        if (pParticleBurst3)
        {
            pParticleBurst3->ctor_41CF50(sControlledCharacter_5C1B8C->field_B8_xpos, sControlledCharacter_5C1B8C->field_BC_ypos, 5u, FP_FromInteger(1), BurstType::eBigRedSparks_3, 9);
        }

        if (field_1C2_falling_counter > 4)
        {
            SFX_Play_46FBA0(101u, 127, 0, field_CC_sprite_scale);
            SFX_Play_46FBA0(47u, 127, 0, field_CC_sprite_scale);
            auto pScreenShake = alive_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4ACF70(0, 0);
            }
        }

        field_1C2_falling_counter = 0;

        if (field_C8_vely > -FP_FromInteger(1))
        {
            SFX_Play_46FBA0(101u, 120, 0, field_CC_sprite_scale);
            SFX_Play_46FBA0(47u, 70, -800, field_CC_sprite_scale);
            field_C8_vely = FP_FromInteger(0);
            field_100_pCollisionLine = pPathLine;
            field_BC_ypos = hitY;
            if (FP_GetExponent(field_C4_velx))
            {
                field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
            }
            field_F8_LastLineYPos = field_BC_ypos;
            Stop_46E570();
            sActiveHero_5C1B68->field_100_pCollisionLine = field_100_pCollisionLine;
            sActiveHero_5C1B68->field_B8_xpos = field_B8_xpos;
            sActiveHero_5C1B68->field_BC_ypos = field_BC_ypos;
            field_turnDirection_1BC = MineCarDirs::eUp_3;
        }
    }

    RunThingsOver_46F380();
    return;
}

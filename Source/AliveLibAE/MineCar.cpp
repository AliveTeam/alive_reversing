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
#include "Grid.hpp"

const FP mineCarHeightUnscaled = FP_FromInteger(60);
const FP mineCarWidthUnscaled = FP_FromInteger(12);

MineCar* MineCar::ctor_46BC80(Path_MineCar* pTlv, s32 tlvInfo, s32 /*a4*/, s32 /*a5*/, s32 /*a6*/)
{
    ctor_408240(0);
    SetVTable(&field_124_anim, 0x544290);
    SetVTable(this, 0x5461FC);
    SetType(AETypes::eMineCar_89);

    const AnimRecord& rec = AnimRec(AnimId::Mine_Car_Open);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_11E_scale = pTlv->field_10_scale;
    field_120_max_damage = pTlv->field_12_max_damage;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_DC_bApplyShadows |= 2u;
    field_1BE_unused = 0;
    field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_26;
    field_CC_sprite_scale = FP_FromInteger(1);
    field_D6_scale = 1;

    if (field_11E_scale != Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_Half_7;
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
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        field_BC_ypos = hitY;
    }

    field_118_tlvInfo = tlvInfo;
    field_11C_state = MineCarStates::eParkedWithoutAbe_0;

    LoadAnimation_46BF80(&field_124_anim);

    mFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_1C0_unused = 0;
    field_1C2_falling_counter = 0;

    // What was pressed before we started to move
    field_1D4_previous_input = static_cast<s16>(sInputKey_ThrowItem_5550F4);

    // Set to a key that keeps the car moving, for example if moving right and we hit a right wall that a car
    // can travel "up" then we set this key to "up" such that holding down "right" automatically moves the car up.
    field_1D6_continue_move_input = static_cast<s16>(sInputKey_ThrowItem_5550F4);

    field_1CC_spawned_path = gMap.mCurrentPath;
    field_1CE_spawned_camera = gMap.field_4_current_camera;
    field_1D0_sound_channels_mask = 0;
    field_1C4_velx_index = 0;

    return this;
}

BaseGameObject* MineCar::VDestructor(s32 flags)
{
    return vdtor_46BF50(flags);
}

void MineCar::VUpdate()
{
    vUpdate_46C010();
}

void MineCar::VRender(PrimHeader** ppOt)
{
    vRender_46E760(ppOt);
}

void MineCar::VScreenChanged()
{
    vScreenChanged_46F800();
}

void MineCar::VStopAudio()
{
    vStopAudio_46F9C0();
}

s32 MineCar::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_467E10(reinterpret_cast<MineCar_SaveState*>(pSaveBuffer));
}

s16 MineCar::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_46F7D0(pFrom);
}

const AnimId sMineCarFrameTable[7] = {
    AnimId::Mine_Car_Closed,
    AnimId::Mine_Car_Open,
    AnimId::Mine_Car_Shake_A,
    AnimId::Mine_Car_Shake_B,
    AnimId::Mine_Car_Tread_Idle,
    AnimId::Mine_Car_Tread_Move_A,
    AnimId::Mine_Car_Tread_Move_B};

s32 CC MineCar::CreateFromSaveState_467740(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MineCar_SaveState*>(pBuffer);
    auto pTlv = static_cast<Path_MineCar*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_4C_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBayrollResID_6013, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("BAYROLL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABECAR.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("METAL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    auto pMineCar = ae_new<MineCar>();
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

    s32 remapped1 = 0;
    switch (pState->field_24_frame_table)
    {
        case 10860:
            remapped1 = 6;
            break;
        case 10884:
            remapped1 = 1;
            break;
        case 10896:
            remapped1 = 4;
            break;
        case 10908:
            remapped1 = 0;
            break;
        case 10920:
            remapped1 = 5;
            break;
        case 10944:
            remapped1 = 2;
            break;
        case 10972:
            remapped1 = 3;
            break;
        default:
            break;
    }

    const AnimRecord& animRec = AnimRec(sMineCarFrameTable[remapped1]);
    pMineCar->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    pMineCar->field_20_animation.field_92_current_frame = pState->field_2A_current_anim_frame;


    pMineCar->field_20_animation.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pMineCar->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_xFlip & 1);
    pMineCar->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E_render & 1);

    pMineCar->mFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_drawable & 1);

    if (IsLastFrame(&pMineCar->field_20_animation))
    {
        pMineCar->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    s32 remapped2 = 0;
    switch (pState->field_38_frame_table_offset2)
    {
        case 10860:
            remapped2 = 6;
            break;
        case 10884:
            remapped2 = 1;
            break;
        case 10896:
            remapped2 = 4;
            break;
        case 10908:
            remapped2 = 0;
            break;
        case 10920:
            remapped2 = 5;
            break;
        case 10944:
            remapped2 = 2;
            break;
        case 10972:
            remapped2 = 3;
            break;
        default:
            break;
    }

    const AnimRecord& animRec2 = AnimRec(sMineCarFrameTable[remapped2]);
    pMineCar->field_124_anim.Set_Animation_Data_409C80(animRec2.mFrameTableOffset, nullptr);
    pMineCar->field_124_anim.field_92_current_frame = pState->field_2A_current_anim_frame;


    // TODO: OG Bug same flags but save state saves 2 sets one for each anim ??
    pMineCar->field_124_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_xFlip & 1);
    pMineCar->field_124_anim.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E_render & 1);


    pMineCar->field_124_anim.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pMineCar->field_10C_health = pState->field_3C_health;
    pMineCar->field_106_current_motion = pState->field_40_current_motion;

    pMineCar->field_108_next_motion = pState->field_42_next_motion;

    pMineCar->field_F8_LastLineYPos = FP_FromInteger(pState->field_44_last_line_ypos);

    pMineCar->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pMineCar->field_104_collision_line_type = pState->field_46_collision_line_type;
    pMineCar->field_118_tlvInfo = pState->field_4C_tlvInfo;

    pMineCar->field_11C_state = pState->field_50_state;
    pMineCar->field_1BC_turn_direction = pState->field_52_turn_direction;
    pMineCar->field_1BE_unused = pState->field_54_unused;
    pMineCar->field_1C0_unused = pState->field_56_unused;

    pMineCar->field_1C2_falling_counter = pState->field_58_falling_counter;

    pMineCar->field_1C8_frame_mod_16 = pState->field_5C_frame_mod_16;

    pMineCar->field_1CC_spawned_path = pState->field_60_spawned_path;
    pMineCar->field_1CE_spawned_camera = pState->field_62_spanwed_camera;

    pMineCar->field_1D4_previous_input = pState->field_64_throw_item_key1;
    pMineCar->field_1D6_continue_move_input = pState->field_66_continue_move_input;

    if (pMineCar->field_C4_velx < (ScaleToGridSize_4498B0(FP_FromInteger(1)) / FP_FromInteger(4)))
    {
        pMineCar->field_1C4_velx_index = 7;
    }

    return sizeof(MineCar_SaveState);
}

void MineCar::LoadAnimation_46BF80(Animation* pAnim)
{
    const AnimRecord& rec = AnimRec(AnimId::Mine_Car_Tread_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);

    if (pAnim->Init_40A030(rec.mFrameTableOffset, gObjList_animations_5C1A24, this, rec.mMaxW, rec.mMaxH, ppRes, 1, 0, 0))
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
        mFlags.Set(BaseGameObject::eListAddFailed_Bit1);
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

MineCar* MineCar::vdtor_46BF50(s32 flags)
{
    dtor_46F2A0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
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

Bool32 MineCar::CheckRoofCollision_46F6B0(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos + hitX,
        field_BC_ypos,
        field_B8_xpos + hitX,
        field_BC_ypos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 8 : 128
    );
}

Bool32 MineCar::CheckFloorCollision_46F730(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos + hitX,
            field_BC_ypos - FP_FromInteger(4),
            field_B8_xpos + hitX,
            field_BC_ypos + hitY,
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16
        )
    )
    {
        return FALSE;
    }

    if (field_BC_ypos > hitY)
    {
        field_BC_ypos = hitY;
    }

    return TRUE;
}

void MineCar::vRender_46E760(PrimHeader** ppOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        s16 r = field_D2_g;
        s16 g = field_D0_r;
        s16 b = field_D4_b;

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

        field_124_anim.field_8_r = static_cast<u8>(r);
        field_124_anim.field_9_g = static_cast<u8>(g);
        field_124_anim.field_A_b = static_cast<u8>(b);

        if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos + FP_FromInteger(30), field_BC_ypos, 0) || gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)), 0) || gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos - FP_FromInteger(30), field_BC_ypos, 0))
        {
            field_124_anim.vRender_40B820(
                FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                ppOt,
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
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MineCar::Stop_46E570()
{
    const AnimRecord& animRec = AnimRec(AnimId::Mine_Car_Closed);
    const AnimRecord& animRec2 = AnimRec(AnimId::Mine_Car_Tread_Idle);

    field_11C_state = MineCarStates::eParkedWithAbe_1;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    SFX_Play_46FA90(SoundEffect::MinecarStop_101, 127, field_CC_sprite_scale);
    
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    field_124_anim.Set_Animation_Data_409C80(animRec2.mFrameTableOffset, nullptr);

    field_1C4_velx_index = 0;

    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
}

void MineCar::Move_46E640(u16 frameTabeOffset, FP velX, FP velY, InputCommands::Enum input, MineCarDirs turnDirection, s8 bChangeDirection)
{
    field_20_animation.Set_Animation_Data_409C80(frameTabeOffset, nullptr);

    field_11C_state = MineCarStates::eMoving_2;
    field_1C8_frame_mod_16 = static_cast<s32>(sGnFrame_5C1B84) % 16;

    if (!field_1D0_sound_channels_mask)
    {
        field_1D0_sound_channels_mask = SFX_Play_46FA90(SoundEffect::MinecarMovement_100, 127, field_CC_sprite_scale);
    }

    const AnimRecord& animRec2 = AnimRec(AnimId::Mine_Car_Tread_Move_A);

    field_124_anim.Set_Animation_Data_409C80(animRec2.mFrameTableOffset, nullptr);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (sInputObject_5BD4E0.isPressed(input))
    {
        field_1D4_previous_input = static_cast<s16>(input);
    }

    field_1BC_turn_direction = turnDirection;

    field_124_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards, bChangeDirection);
}

s16 MineCar::IsBlocked_46F4A0(MineCarDirs a2, s32 /*a3*/)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    switch (a2)
    {
        case MineCarDirs::eDown_0:
        {
            if (!CheckRoofCollision_46F6B0(FP_FromInteger(4) - mineCarWidthAdjusted, -(mineCarHeight) + FP_FromInteger(1)))
            {
                if (!CheckRoofCollision_46F6B0(mineCarWidthAdjusted - FP_FromInteger(4), -(mineCarHeight + FP_FromInteger(1))))
                {
                    return FALSE;
                }
            }
        }
        break;

        case MineCarDirs::eUp_3:
        {
            if (!CheckFloorCollision_46F730(FP_FromInteger(4) - mineCarWidthAdjusted, FP_FromInteger(1)))
            {
                if (
                    !CheckFloorCollision_46F730(mineCarWidthAdjusted - FP_FromInteger(4), FP_FromInteger(1)) &&
                    !CheckFloorCollision_46F730(-FP_FromInteger(10), FP_FromInteger(1)) &&
                    !CheckFloorCollision_46F730(FP_FromInteger(10), FP_FromInteger(1))
                )
                {
                    return FALSE;
                }
            }
        }
        break;
    }

    return TRUE;
}

s16 MineCar::FollowDirection_46EA00()
{
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP stepSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    const FP halfHeight = mineCarHeight * FP_FromDouble(0.5);
    const FP step = mineCarWidth + stepSize;

    // If we're moving horizontally and hit a wall...
    //
    if (
        (field_C4_velx > FP_FromInteger(0) && WallHit_408750(halfHeight, step + FP_FromInteger(1))) ||
        (field_C4_velx < FP_FromInteger(0) && WallHit_408750(halfHeight, -step))
    )
    {
        // ...continue vertically
        //
        if (field_1BC_turn_direction == MineCarDirs::eUp_3)
        {
            field_1D6_continue_move_input = (u16) sInputKey_Up_5550D8;
        }
        else
        {
            field_1D6_continue_move_input = (u16) sInputKey_Down_5550DC;
        }

        return TRUE;
    }

    // If we're moving vertically and hit and floor or roof...
    //
    if (
        (
            CheckFloorCollision_46F730(FP_FromInteger(0), FP_FromInteger(1)) &&
            field_C8_vely > FP_FromInteger(0)
        ) ||
        (
            CheckRoofCollision_46F6B0(FP_FromInteger(0), -mineCarHeight) &&
            field_C8_vely < FP_FromInteger(0)
        )
    )
    {
        // ...continue horizontally
        //
        if (field_1BC_turn_direction == MineCarDirs::eLeft_2)
        {
            field_1D6_continue_move_input = (u16) sInputKey_Left_5550D4;
        }
        else
        {
            field_1D6_continue_move_input = (u16) sInputKey_Right_5550D0;
        }

        return TRUE;
    }

    switch (field_1BC_turn_direction)
    {
        case MineCarDirs::eDown_0:
        {
            const bool bRoofRight =
                CheckRoofCollision_46F6B0(FP_FromInteger(4) - step, -mineCarHeight) ||
                CheckRoofCollision_46F6B0(step + FP_FromInteger(2), -mineCarHeight);
            const bool bRoofLeft =
                CheckRoofCollision_46F6B0(-(step + FP_FromInteger(2)), -mineCarHeight) ||
                CheckRoofCollision_46F6B0(step - FP_FromInteger(4),    -mineCarHeight);

            if (
                (field_C4_velx > FP_FromInteger(0) && !bRoofRight) ||
                (field_C4_velx < FP_FromInteger(0) && !bRoofLeft)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Up_5550D8;
                return TRUE;
            }
        }
        
        break;

        case MineCarDirs::eRight_1:
        {
            const bool bLeftWall1 =
                WallHit_408750(mineCarHeight - FP_FromInteger(2), -(step + FP_FromInteger(4))) ||
                WallHit_408750(-FP_FromInteger(2),                -(step + FP_FromInteger(4)));
            const bool bLeftWall2 =
                WallHit_408750(mineCarHeight + FP_FromInteger(1), -(step + FP_FromInteger(4))) ||
                WallHit_408750(FP_FromInteger(1),                 -(step + FP_FromInteger(4)));

            if (
                (field_C8_vely > FP_FromInteger(0) && !bLeftWall1) ||
                (field_C8_vely < FP_FromInteger(0) && !bLeftWall2)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Left_5550D4;
                return TRUE;
            }
        }
        
        break;

        case MineCarDirs::eLeft_2:
        {
            const bool bRightWall1 =
                WallHit_408750(mineCarHeight - FP_FromInteger(2), step + FP_FromInteger(4)) ||
                WallHit_408750(-FP_FromInteger(2),                step + FP_FromInteger(4));
            const bool bRightWall2 =
                WallHit_408750(mineCarHeight + FP_FromInteger(1), step + FP_FromInteger(4)) ||
                WallHit_408750(FP_FromInteger(1),                 step + FP_FromInteger(4));

            if (
                (field_C8_vely > FP_FromInteger(0) && !bRightWall1) ||
                (field_C8_vely < FP_FromInteger(0) && !bRightWall2)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Right_5550D0;
                return TRUE;
            }
        }
        
        break;

        case MineCarDirs::eUp_3:
        {
            const bool bFloorRight =
                CheckFloorCollision_46F730(FP_FromInteger(4) - step, FP_FromInteger(4)) ||
                CheckFloorCollision_46F730(step + FP_FromInteger(2), FP_FromInteger(4));

            const bool bFloorLeft =
                CheckFloorCollision_46F730(-(step + FP_FromInteger(2)), FP_FromInteger(4)) ||
                CheckFloorCollision_46F730(step - FP_FromInteger(4), FP_FromInteger(4));

            if (
                (field_C4_velx > FP_FromInteger(0) && !bFloorRight) ||
                (field_C4_velx < FP_FromInteger(0) && !bFloorLeft)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Down_5550DC;
                return TRUE;
            }
        }
        
        break;
    }

    return FALSE;
}

void MineCar::RunThingsOver_46F380()
{
    PSX_RECT ourRect = {};
    vGetBoundingRect_424FD0(&ourRect, 1);
    ourRect.h += 6;

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        if (pObj->mFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // You can't run yourself over with a mine car it seems.
            if (pObj->Type() != AETypes::eAbe_69)
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                if (pAliveObj->field_CC_sprite_scale == field_CC_sprite_scale ||
                   (pAliveObj->Type() == AETypes::eSlog_126 && field_CC_sprite_scale != FP_FromDouble(0.5)))
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

s16 MineCar::vTakeDamage_46F7D0(BaseGameObject* /*pFrom*/)
{
    return !mFlags.Get(BaseGameObject::eDead);
}

s32 MineCar::vGetSaveState_467E10(MineCar_SaveState* pState)
{
    pState->field_0_type = AETypes::eMineCar_89;

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

    pState->field_2F_drawable = mFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_22_xFlip = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_2E_render = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);


    switch (field_20_animation.field_18_frame_table_offset)
    {
        case 20788: // Mine_Car_Tread_Move_B
            pState->field_24_frame_table = 10860;
            break;
        case 20812: // Mine_Car_Open
            pState->field_24_frame_table = 10884;
            break;
        case 20824: // Mine_Car_Tread_Idle
            pState->field_24_frame_table = 10896;
            break;
        case 20836: // Mine_Car_Closed
            pState->field_24_frame_table = 10908;
            break;
        case 20848: // Mine_Car_Tread_Move_A
            pState->field_24_frame_table = 10920;
            break;
        case 20872: // Mine_Car_Shake_A
            pState->field_24_frame_table = 10944;
            break;
        case 20900: // Mine_Car_Shake_B
            pState->field_24_frame_table = 10972;
            break;
        default:
            break;
    }

    pState->field_34_unused = field_124_anim.field_92_current_frame;
    pState->field_36_unused = field_124_anim.field_E_frame_change_counter;

    pState->field_32_unused = field_124_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_30_unused = field_124_anim.field_4_flags.Get(AnimFlags::eBit3_Render);
    
    switch (field_124_anim.field_18_frame_table_offset)
    {
        case 20788: // Mine_Car_Tread_Move_B
            pState->field_38_frame_table_offset2 = 10860;
            break;
        case 20812: // Mine_Car_Open
            pState->field_38_frame_table_offset2 = 10884;
            break;
        case 20824: // Mine_Car_Tread_Idle
            pState->field_38_frame_table_offset2 = 10896;
            break;
        case 20836: // Mine_Car_Closed
            pState->field_38_frame_table_offset2 = 10908;
            break;
        case 20848: // Mine_Car_Tread_Move_A
            pState->field_38_frame_table_offset2 = 10920;
            break;
        case 20872: // Mine_Car_Shake_A
            pState->field_38_frame_table_offset2 = 10944;
            break;
        case 20900: // Mine_Car_Shake_B
            pState->field_38_frame_table_offset2 = 10972;
            break;
        default:
            break;
    }

    pState->field_3C_health = field_10C_health;
    pState->field_42_next_motion = field_108_next_motion;

    pState->field_40_current_motion = field_106_current_motion;

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

    pState->field_52_turn_direction = field_1BC_turn_direction;
    pState->field_54_unused = field_1BE_unused;
    pState->field_56_unused = field_1C0_unused;

    pState->field_58_falling_counter = field_1C2_falling_counter;
    pState->field_5C_frame_mod_16 = field_1C8_frame_mod_16;

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
    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

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
                1 << field_104_collision_line_type
            );

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
        mFlags.Set(BaseGameObject::eDead);
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    switch (field_11C_state)
    {
        case MineCarStates::eParkedWithoutAbe_0:
            State_0_ParkedWithoutAbe();
            break;

        case MineCarStates::eParkedWithAbe_1:
            State_1_ParkedWithAbe();
            break;

        case MineCarStates::eMoving_2:
            State_2_Moving();
            break;

        case MineCarStates::eFalling_3:
            State_3_Falling();
            break;

        default:
            break;
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_DoAction_5550E4))
    {
        if (
            (
                field_1BC_turn_direction != MineCarDirs::eUp_3 &&
                !IsBlocked_46F4A0(MineCarDirs::eUp_3, 0)
            ) ||
            !IsBlocked_46F4A0(MineCarDirs::eUp_3, 0)
        )
        {
            if (field_11C_state != MineCarStates::eParkedWithoutAbe_0)
            {
                field_11C_state = MineCarStates::eFalling_3;
            }
        }
    }

    if (
        field_1BC_turn_direction != MineCarDirs::eUp_3 ||
        IsBlocked_46F4A0(MineCarDirs::eUp_3, 0)
    )
    {
        return;
    }

    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - mineCarWidthAdjusted,
            field_BC_ypos + field_C8_vely - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            field_B8_xpos + mineCarWidthAdjusted,
            field_BC_ypos + field_C8_vely - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 0x1000 : 0x8000
        )
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
    }
}

void MineCar::State_0_ParkedWithoutAbe()
{
    PSX_RECT carRect = {};
    vGetBoundingRect_424FD0(&carRect, 1);

    PSX_RECT abeRect = {};
    sActiveHero_5C1B68->vGetBoundingRect_424FD0(&abeRect, 1);

    if (
        sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_117_InMineCar_4587C0 &&
        PSX_Rects_overlap_4FA0B0(&carRect, &abeRect) &&
        sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale
    )
    {
        const AnimRecord& animRec = AnimRec(AnimId::Mine_Car_Closed);

        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        field_11C_state = MineCarStates::eParkedWithAbe_1;
        sControlledCharacter_5C1B8C = this;
        field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_35;
        field_124_anim.field_C_render_layer = Layer::eLayer_BombMineCar_35;

        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_Half_16;
            field_124_anim.field_C_render_layer = Layer::eLayer_BombMineCar_Half_16;
        }

        SFX_Play(SoundEffect::DoorEffect_57, 100, 500, field_CC_sprite_scale);
    }
}

void MineCar::State_1_ParkedWithAbe()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    VCheckCollisionLineStillValid_408A40(10);

    // Abe exits minecar
    //
    if (sActiveHero_5C1B68->field_106_current_motion != eAbeMotions::Motion_117_InMineCar_4587C0)
    {
        const AnimRecord& animRec = AnimRec(AnimId::Mine_Car_Open);
        const AnimRecord& animRec2 = AnimRec(AnimId::Mine_Car_Tread_Idle);

        sActiveHero_5C1B68->field_B8_xpos = field_B8_xpos;
        sActiveHero_5C1B68->field_BC_ypos = field_BC_ypos;
        field_11C_state = MineCarStates::eParkedWithoutAbe_0;
        
        field_124_anim.Set_Animation_Data_409C80(animRec2.mFrameTableOffset, nullptr);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        field_1CC_spawned_path = gMap.mCurrentPath;
        field_1CE_spawned_camera = gMap.field_4_current_camera;

        sActiveHero_5C1B68->VCheckCollisionLineStillValid_408A40(10);

        SFX_Play(SoundEffect::DoorEffect_57, 100, 500, field_CC_sprite_scale);

        if (field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_Half_7;
            field_124_anim.field_C_render_layer = Layer::eLayer_Shadow_Half_7;
        }
        else
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_26;
            field_124_anim.field_C_render_layer = Layer::eLayer_Shadow_26;
        }

        return;
    }

    // Peform any movements
    //
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    // Horizontal movements
    //
    const FP rayCastY1 = field_BC_ypos - mineCarHeight;
    const FP rayCastY2 = field_BC_ypos;
    const FP velX = kGridSize / FP_FromInteger(4);
    const FP velY = FP_FromInteger(0);

    InputCommands::Enum inputKey = sInputKey_Right_5550D0;

    if (
        (
            sInputObject_5BD4E0.isPressed(inputKey) ||
            (
                sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) sInputKey_Left_5550D4 &&
                field_1BC_turn_direction != MineCarDirs::eUp_3 &&
                field_1BC_turn_direction != MineCarDirs::eDown_0
            )
        ) &&
        !(
            WallHit_408750(mineCarHeight * FP_FromDouble(0.5), mineCarWidthAdjusted + FP_FromInteger(1)) &&
            WallHit_408750(mineCarHeight - FP_FromInteger(8), mineCarWidthAdjusted + FP_FromInteger(1))
        )
    )
    {
        const FP rayCastX = field_B8_xpos + velX;
        
        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(2);
        const FP hitX2 = FP_FromInteger(4) - mineCarWidthAdjusted;
        const u16 frameTableOffset = 20872u;

        if (
            HandleState1Move(
                &MineCar::CheckFloorCollision_46F730,
                hitX,
                FP_FromInteger(4),
                hitX2,
                frameTableOffset,
                MineCarDirs::eUp_3,
                FALSE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                0x800,
                0x4000,
                velX,
                velY,
                inputKey,
                false,
                false
            ) ||
            HandleState1Move(
                &MineCar::CheckRoofCollision_46F6B0,
                hitX,
                -mineCarHeight,
                hitX2,
                frameTableOffset,
                MineCarDirs::eDown_0,
                TRUE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                0x2000,
                0x10000,
                velX,
                velY,
                inputKey,
                false,
                false
            )
        )
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
                if (field_1BC_turn_direction != MineCarDirs::eLeft_2 && !(static_cast<s32>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(SoundEffect::MinecarStuck_102, 127, field_CC_sprite_scale);
                }
            }
        }
    }

    inputKey = sInputKey_Left_5550D4;

    if (
        (
            sInputObject_5BD4E0.isPressed(inputKey) ||
            (
                sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) sInputKey_Right_5550D0 &&
                field_1BC_turn_direction != MineCarDirs::eUp_3 &&
                field_1BC_turn_direction != MineCarDirs::eDown_0
            )
        ) &&
        !(
            WallHit_408750(mineCarHeight * FP_FromDouble(0.5), -mineCarWidthAdjusted) &&
            WallHit_408750(mineCarHeight - FP_FromInteger(8), -mineCarWidthAdjusted)
        )
    )
    {
        const FP rayCastX = field_B8_xpos - velX;
        
        const FP hitX = mineCarWidthAdjusted - FP_FromInteger(4);
        const FP hitX2 = -(mineCarWidthAdjusted + FP_FromInteger(2));
        const u16 frameTableOffset = 20900u;

        if (
            HandleState1Move(
                &MineCar::CheckFloorCollision_46F730,
                hitX,
                FP_FromInteger(4),
                hitX2,
                frameTableOffset,
                MineCarDirs::eUp_3,
                TRUE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                0x800,
                0x4000,
                -velX,
                velY,
                inputKey,
                false,
                false
            ) ||
            HandleState1Move(
                &MineCar::CheckRoofCollision_46F6B0,
                hitX,
                -mineCarHeight,
                hitX2,
                frameTableOffset,
                MineCarDirs::eDown_0,
                FALSE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                0x2000,
                0x10000,
                -velX,
                velY,
                inputKey,
                false,
                false
            )
        )
        {
            return;
        }
    }
    else
    {
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eRight_1 && !(static_cast<s32>(sGnFrame_5C1B84) % 6))
            {
                SFX_Play_46FA90(SoundEffect::MinecarStuck_102, 127, field_CC_sprite_scale);
            }
        }
    }

    HandleUpDown();
}

bool MineCar::HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3,
                               u16 frameTableOffset, MineCarDirs mineCarDir, const s8 bChangeDir, FP rayCastX1, FP rayCastY1, FP rayCastX2, FP rayCastY2, const s32 ModelMask1, const s32 ModelMask2,
                               FP velX, FP velY, InputCommands::Enum key, bool isVertical, bool verticalFlipXCond)
{
    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    if (
        sCollisions_DArray_5C1128->Raycast_417A60(
            rayCastX1,
            rayCastY1,
            rayCastX2,
            rayCastY2,
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? ModelMask1 : ModelMask2
        )
    )
    {
        if ((this->*func)(mineCarFPFuncArg1, mineCarFPFuncArg2) || (this->*func)(mineCarFPFuncArg3, mineCarFPFuncArg2))
        {
            if (
                !isVertical ||
                (
                    (verticalFlipXCond  && hitX > field_B8_xpos) ||
                    (!verticalFlipXCond && hitX < field_B8_xpos)
                )
            )
            {
                Move_46E640(frameTableOffset, velX, velY, key, mineCarDir, bChangeDir);
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

    const FP rayCastX1 = field_B8_xpos - mineCarWidthAdjusted;
    const FP rayCastX2 = field_B8_xpos + mineCarWidthAdjusted;
    
    const FP velX = FP_FromInteger(0);
    const FP velY = k5Scaled;
    const u16 frameTableOffset = 20836u;

    InputCommands::Enum inputKey = sInputKey_Up_5550D8;

    if (
        sInputObject_5BD4E0.isPressed(inputKey) ||
        (
            sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) sInputKey_Down_5550DC &&
            field_1BC_turn_direction != MineCarDirs::eLeft_2 &&
            field_1BC_turn_direction != MineCarDirs::eRight_1 &&
            !IsBlocked_46F4A0(MineCarDirs::eDown_0, 0)
        )
    )
    {
        const FP rayCastY = field_BC_ypos - velY - (mineCarWidthAdjusted * FP_FromDouble(0.5));

        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitY = FP_FromInteger(1);
        const FP hitY2 = mineCarHeight + FP_FromInteger(1);

        if (
            HandleState1Move(
                &MineCar::WallHit_408750,
                hitY,
                hitX,
                hitY2,
                frameTableOffset,
                MineCarDirs::eLeft_2,
                FALSE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                0x1000,
                0x8000,
                velX,
                -velY,
                inputKey,
                true,
                true
            ) ||
            HandleState1Move(
                &MineCar::WallHit_408750,
                hitY,
                -hitX,
                hitY2,
                frameTableOffset,
                MineCarDirs::eRight_1,
                TRUE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                0x1000,
                0x8000,
                velX,
                -velY,
                inputKey,
                true,
                false
            )
        )
        {
            return;
        }
    }
    else if (IsBlocked_46F4A0(MineCarDirs::eDown_0, 0))
    {
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eDown_0)
            {
                if (!(static_cast<s32>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(SoundEffect::MinecarStuck_102, 127, field_CC_sprite_scale);
                }
            }
        }
    }

    inputKey = sInputKey_Down_5550DC;

    if (
        sInputObject_5BD4E0.isPressed(inputKey) ||
        (
            sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) sInputKey_Up_5550D8 &&
            field_1BC_turn_direction != MineCarDirs::eLeft_2 &&
            field_1BC_turn_direction != MineCarDirs::eRight_1 &&
            !IsBlocked_46F4A0(MineCarDirs::eUp_3, 0)
        )
    )
    {
        const FP rayCastY = field_BC_ypos + velY;

        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitY = -FP_FromInteger(2);
        const FP hitY2 = mineCarHeight - FP_FromInteger(1);

        if (
            HandleState1Move(
                &MineCar::WallHit_408750,
                hitY,
                hitX,
                hitY2,
                frameTableOffset,
                MineCarDirs::eLeft_2,
                TRUE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                0x1000,
                0x8000,
                velX,
                velY,
                inputKey,
                true,
                true
            ) ||
            HandleState1Move(
                &MineCar::WallHit_408750,
                hitY,
                -hitX,
                hitY2,
                frameTableOffset,
                MineCarDirs::eRight_1,
                FALSE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                0x1000,
                0x8000,
                velX,
                velY,
                inputKey,
                true,
                false
            )
        )
        {
            return;
        }
    }
    else if (IsBlocked_46F4A0(MineCarDirs::eUp_3, 0))
    {
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eUp_3 && !(static_cast<s32>(sGnFrame_5C1B84) % 6))
            {
                SFX_Play_46FA90(SoundEffect::MinecarStuck_102, 127, field_CC_sprite_scale);
            }
        }
    }
}

// TODO: Probably 7?
const FP velXTable_5461D8[9] = {
    FP_FromInteger(6),
    FP_FromInteger(5),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(3),
    FP_FromInteger(2),
    FP_FromInteger(1),
    FP_FromInteger(0),
    FP_FromInteger(0)};


void MineCar::State_2_Moving()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    sActiveHero_5C1B68->field_B8_xpos = field_B8_xpos;
    sActiveHero_5C1B68->field_BC_ypos = field_BC_ypos;

    if (!field_1D0_sound_channels_mask)
    {
        // Play the mine car moving sound
        //
        field_1D0_sound_channels_mask = SFX_Play_46FA90(SoundEffect::MinecarMovement_100, 127, field_CC_sprite_scale);
    }

    if (FollowDirection_46EA00() && !field_1C4_velx_index)
    {
        Stop_46E570();
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - mineCarWidthAdjusted,
            field_BC_ypos + field_C8_vely - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            field_B8_xpos + mineCarWidthAdjusted,
            field_BC_ypos + field_C8_vely - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 4096 : 0x8000
        ) &&
        field_C8_vely > FP_FromInteger(0)
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos + field_C4_velx,
            field_BC_ypos,
            field_B8_xpos + field_C4_velx,
            field_BC_ypos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 0x2000 : 0x10000
        ) &&
        field_1BC_turn_direction == MineCarDirs::eDown_0
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos + field_C4_velx,
            field_BC_ypos,
            field_B8_xpos + field_C4_velx,
            field_BC_ypos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 0x800 : 0x4000
        ) &&
        field_1BC_turn_direction == MineCarDirs::eUp_3
    )
    {
        Stop_46E570();
        return;
    }

    if (field_1C4_velx_index)
    {
        if (field_1C4_velx_index >= 7)
        {
            Stop_46E570();
            return;
        }

        if (field_C4_velx <= FP_FromInteger(0))
        {
            field_C4_velx = -velXTable_5461D8[field_1C4_velx_index];
        }
        else
        {
            field_C4_velx = velXTable_5461D8[field_1C4_velx_index];
        }

        if (++field_1C4_velx_index == 2)
        {
            ++field_124_anim.field_10_frame_delay;
        }
    }

    if (sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
    {
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;

        SetActiveCameraDelayedFromDir_408C40();
        RunThingsOver_46F380();

        return;
    }

    if (
        field_1BC_turn_direction == MineCarDirs::eUp_3 ||
        field_1BC_turn_direction == MineCarDirs::eDown_0
    )
    {
        if (
            field_B8_xpos == FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos))) &&
            !field_1C4_velx_index
        )
        {
            if (field_C4_velx <= FP_FromInteger(0))
            {
                field_C4_velx = -velXTable_5461D8[0];
            }
            else
            {
                field_C4_velx = velXTable_5461D8[0];
            }

            ++field_1C4_velx_index;
        }

        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;

        SetActiveCameraDelayedFromDir_408C40();
        RunThingsOver_46F380();

        return;
    }

    Stop_46E570();
}

void MineCar::State_3_Falling()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP mineCarHeight = field_CC_sprite_scale * mineCarHeightUnscaled;
    const FP mineCarWidth = field_CC_sprite_scale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    SetActiveCameraDelayedFromDir_408C40();
    field_1C2_falling_counter++;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (
            WallHit_408750(mineCarHeight, mineCarWidthAdjusted) ||
            WallHit_408750(FP_FromInteger(0), mineCarWidthAdjusted)
        )
        {
            field_C4_velx = FP_FromInteger(0);
            auto pParticleBurst = ae_new<ParticleBurst>();

            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos + mineCarHeight + kGridSize,
                    sControlledCharacter_5C1B8C->field_BC_ypos - ((mineCarHeight + kGridSize) * FP_FromDouble(0.5)),
                    4u,
                    field_CC_sprite_scale,
                    BurstType::eBigRedSparks_3,
                    9
                );
            }

            SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 80, field_CC_sprite_scale);
        }
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (
            WallHit_408750(mineCarHeight, -mineCarWidthAdjusted) ||
            WallHit_408750(FP_FromInteger(0), -mineCarWidthAdjusted)
        )
        {
            field_C4_velx = FP_FromInteger(0);
            auto pParticleBurst2 = ae_new<ParticleBurst>();

            if (pParticleBurst2)
            {
                pParticleBurst2->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos - (mineCarHeight + kGridSize),
                    sControlledCharacter_5C1B8C->field_BC_ypos - ((mineCarHeight + kGridSize) * FP_FromDouble(0.5)),
                    4u,
                    field_CC_sprite_scale,
                    BurstType::eBigRedSparks_3,
                    9
                );
            }

            SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 80, field_CC_sprite_scale);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (InAirCollision_408810(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        field_BC_ypos = hitY;
        field_F8_LastLineYPos = hitY;
        field_C8_vely = (-field_C8_vely * FP_FromDouble(0.2));
        auto pParticleBurst3 = ae_new<ParticleBurst>();

        if (pParticleBurst3)
        {
            pParticleBurst3->ctor_41CF50(
                sControlledCharacter_5C1B8C->field_B8_xpos,
                sControlledCharacter_5C1B8C->field_BC_ypos,
                5u,
                FP_FromInteger(1),
                BurstType::eBigRedSparks_3,
                9
            );
        }

        if (field_1C2_falling_counter > 4)
        {
            SFX_Play(SoundEffect::MinecarStop_101, 127, 0, field_CC_sprite_scale);
            SFX_Play(SoundEffect::FallingItemHit_47, 127, 0, field_CC_sprite_scale);
            auto pScreenShake = ae_new<ScreenShake>();

            if (pScreenShake)
            {
                pScreenShake->ctor_4ACF70(FALSE, FALSE);
            }
        }

        field_1C2_falling_counter = 0;

        if (field_C8_vely > -FP_FromInteger(1))
        {
            SFX_Play(SoundEffect::MinecarStop_101, 120, 0, field_CC_sprite_scale);
            SFX_Play(SoundEffect::FallingItemHit_47, 70, -800, field_CC_sprite_scale);

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
            field_1BC_turn_direction = MineCarDirs::eUp_3;
        }
    }

    RunThingsOver_46F380();

    return;
}

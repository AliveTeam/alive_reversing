#include "stdafx.h"
#include "MineCar.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
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
    field_11C_state = States::eState_0_ParkedWithoutAbe;

    LoadAnimation_46BF80(&field_124_anim);

    field_6_flags.Set(BaseGameObject::eCanExplode);

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
        field_6_flags.Set(BaseGameObject::eListAddFailed);
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

__int16 MineCar::CheckFloorCollision_46F730(FP hitX, FP hitY)
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
    field_11C_state = States::eState_1_ParkedWithAbe;
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

void MineCar::Move_46E640(unsigned __int16 frameTabeOffset, FP velX, FP velY, unsigned __int16 input, __int16 unknown, char bChangeDirection)
{
    field_20_animation.Set_Animation_Data_409C80(frameTabeOffset, nullptr);
    field_11C_state = States::eState_2_Moving;
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
        field_1D4_previous_input = input;
    }

    field_1BC = unknown;

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
        if (field_1BC == 3)
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
        if (field_1BC == 2)
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Left_5550D4;
        }
        else
        {
            field_1D6_continue_move_input = (unsigned short)sInputKey_Right_5550D0;
        }
        return 1;
    }

    if (field_1BC == 3)
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
    else if (field_1BC == 0)
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
    else if (field_1BC == 2)
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
    else if (field_1BC == 1)
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

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject))
        {
            // You can't run yourself over with a mine car it seems.
            if (pObj->field_4_typeId != Types::eType_Abe_69)
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

void MineCar::vUpdate_Real_46C010()
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
        field_6_flags.Set(BaseGameObject::eDead);
    }

    //const auto pState1 = &field_11C_state;
    //const auto pState2 = &field_11C_state;

    const FP kGridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    switch (field_11C_state)
    {
    case States::eState_0_ParkedWithoutAbe:
        State_0();
        break;

    case States::eState_1_ParkedWithAbe:
        State_1();
        //vUpdate_Real_46C010();
        break;

    case States::eState_2_Moving:
        State_2();
        break;

    case States::eState_3_Falling:
        State_3(); 
        break;

    default:
        //goto LABEL_165;
        break;
    }

//LABEL_165:
    if (sInputObject_5BD4E0.isPressed(sInputKey_DoAction_5550E4))
    {
        if (field_1BC != 3 && !IsBlocked_46F4A0(3, 0) || !IsBlocked_46F4A0(3, 0))
        {
            if (field_11C_state != States::eState_0_ParkedWithoutAbe)
            {
                field_11C_state = States::eState_3_Falling;
            }
        }
    }

    if (field_1BC != 3 || IsBlocked_46F4A0(3, 0))
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
        field_11C_state = States::eState_3_Falling;
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
        field_11C_state = States::eState_1_ParkedWithAbe;
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

void MineCar::State_1()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP k12Scaled = (field_CC_sprite_scale * FP_FromInteger(12));
    const FP k60Scaled = field_CC_sprite_scale * FP_FromInteger(60);

    VCheckCollisionLineStillValid_408A40(10);

    if (sActiveHero_5C1B68->field_106_current_motion != eAbeStates::State_117_In_MineCar_4587C0)
    {
        sActiveHero_5C1B68->field_B8_xpos = field_B8_xpos;
        sActiveHero_5C1B68->field_BC_ypos = field_BC_ypos;
        field_11C_state = States::eState_0_ParkedWithoutAbe;
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

    bool enterRightBlock = false;
    if (!sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        if (!sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
        {
            enterRightBlock = true;
        }

        if ((unsigned __int16)field_1D6_continue_move_input != sInputKey_Right_5550D0)
        {
            enterRightBlock = true;
        }

        if (field_1BC == 3)
        {
            enterRightBlock = true;
        }

        if (field_1BC == 0)
        {
            enterRightBlock = true;
        }

        if ((field_1D4_previous_input & 0xFFFF) == (int)sInputKey_Left_5550D4)
        {
            enterRightBlock = true;
        }
    }

    if (enterRightBlock ||
        WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)) * FP_FromDouble(0.5), k12Scaled + kGridSize + FP_FromInteger(1)) ||
        WallHit_408750(k60Scaled - FP_FromInteger(8), k12Scaled + kGridSize + FP_FromInteger(1)))
    {
        if (WallHit_408750(k60Scaled * FP_FromDouble(0.5), k12Scaled + kGridSize + FP_FromInteger(1)))
        {
            if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
            {
                if (field_1BC != 2 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
    }
    else
    {
        PathLine* pPathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            (kGridSize / FP_FromInteger(4)) + field_B8_xpos,
            field_BC_ypos - k60Scaled,
            (kGridSize / FP_FromInteger(4)) + field_B8_xpos,
            field_BC_ypos,
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 2048 : 0x4000))
        {
            if (CheckFloorCollision_46F730(k12Scaled + kGridSize + FP_FromInteger(2), FP_FromInteger(4)) ||
                CheckFloorCollision_46F730(FP_FromInteger(4) - (k12Scaled + kGridSize), FP_FromInteger(4)))
            {
                Move_46E640(20872u, kGridSize / FP_FromInteger(4), FP_FromInteger(0), (unsigned short)sInputKey_Right_5550D0, 3, 0);
                if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    return;
                }
            }
        }

        if (sCollisions_DArray_5C1128->Raycast_417A60(
            (kGridSize / FP_FromInteger(4)) + field_B8_xpos,
            field_BC_ypos - k60Scaled,
            (kGridSize / FP_FromInteger(4)) + field_B8_xpos,
            field_BC_ypos,
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x2000 : 0x10000))
        {
            if (CheckRoofCollision_46F6B0(k12Scaled + kGridSize + FP_FromInteger(2), -k60Scaled) ||
                CheckRoofCollision_46F6B0(FP_FromInteger(4) - (k12Scaled + kGridSize), -k60Scaled))
            {
                Move_46E640(20872u, kGridSize / FP_FromInteger(4), FP_FromInteger(0), (unsigned short)sInputKey_Right_5550D0, 0, 1);
                if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    return;
                }
            }
        }
    }
    
    if (!(sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) && 
        ((!(sInputObject_5BD4E0.isPressed(field_1D4_previous_input))) ||
        (unsigned __int16)field_1D6_continue_move_input != sInputKey_Left_5550D4 ||
        field_1BC == 3 ||
        field_1D4_previous_input == (unsigned __int16)sInputKey_Right_5550D0 ||
        field_1BC == 0) ||
        WallHit_408750(k60Scaled * FP_FromDouble(0.5), -(k12Scaled + kGridSize)))
    {
        if (WallHit_408750(k60Scaled * FP_FromDouble(0.5), -(k12Scaled + kGridSize)))
        {
            if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
            {
                if (field_1BC != 1 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
        HandleUpDown();
        return;
    }

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos - (kGridSize / FP_FromInteger(4)),
        field_BC_ypos - (k60Scaled),
        field_B8_xpos - (kGridSize / FP_FromInteger(4)),
        field_BC_ypos,
        &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x800 : 0x4000) ||
        (!CheckFloorCollision_46F730(k12Scaled + kGridSize - FP_FromInteger(4), FP_FromInteger(4))) &&
        (!CheckFloorCollision_46F730(-(k12Scaled + kGridSize + FP_FromInteger(2)), FP_FromInteger(4))) ||
        (Move_46E640(20900u, -(kGridSize / FP_FromInteger(4)), FP_FromInteger(0), (unsigned short)sInputKey_Left_5550D4, 3, 1), // TODO: Split out
            !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)))
    {
        if (!sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - (kGridSize / FP_FromInteger(4)),
            field_BC_ypos - k60Scaled,
            field_B8_xpos - (kGridSize / FP_FromInteger(4)),
            field_BC_ypos,
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x2000 : 0x10000))
        {
            HandleUpDown();
            return;
        }

        if (!CheckRoofCollision_46F6B0(k12Scaled + kGridSize - FP_FromInteger(4), -k60Scaled))
        {
            if (!CheckRoofCollision_46F6B0(-(k12Scaled + kGridSize + FP_FromInteger(2)), -k60Scaled))
            {
                HandleUpDown();
                return;
            }
        }

        Move_46E640(20900u, -(kGridSize / FP_FromInteger(4)), FP_FromInteger(0), (unsigned short)sInputKey_Left_5550D4, 0, 0);
        if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
        {
            HandleUpDown();
            return;
        }
    }
}

void MineCar::HandleUpDown()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP k12Scaled = field_CC_sprite_scale * FP_FromInteger(12);
    const FP k60Scaled = field_CC_sprite_scale * FP_FromInteger(60);
    const FP k5Scaled = FP_FromInteger(5) * field_CC_sprite_scale;

    if ((sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8) || (sInputObject_5BD4E0.isPressed(field_1D4_previous_input)) &&
        (unsigned __int16)field_1D6_continue_move_input == sInputKey_Up_5550D8 &&
        field_1D4_previous_input != (unsigned __int16)sInputKey_Down_5550DC &&
        field_1BC != 2 &&
        field_1BC != 1) && 
        !IsBlocked_46F4A0(0, 0))
    {
        PathLine* pPathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - (kGridSize + k12Scaled),
            field_BC_ypos - (k5Scaled) - ((k12Scaled + kGridSize) * FP_FromDouble(0.5)),
            k12Scaled + kGridSize + field_B8_xpos,
            field_BC_ypos - (k5Scaled) - ((k12Scaled + kGridSize) * FP_FromDouble(0.5)),
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x1000 : 0x8000))
        {
            if (WallHit_408750(FP_FromInteger(1), kGridSize + k12Scaled + FP_FromInteger(4))||
                WallHit_408750(k60Scaled + FP_FromInteger(1), kGridSize + k12Scaled + FP_FromInteger(4)))
            {
                if (hitX > field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), -k5Scaled, (unsigned short)sInputKey_Up_5550D8, 2, 0);
                }
            }

            if (WallHit_408750(FP_FromInteger(1), -(kGridSize + k12Scaled + FP_FromInteger(4))) || 
                WallHit_408750(k60Scaled + FP_FromInteger(1), -(kGridSize + k12Scaled + FP_FromInteger(4))))
            {
                if (hitX < field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), -k5Scaled, (unsigned short)sInputKey_Up_5550D8, 1, 1);
                }
            }
        }
    }
    else if (IsBlocked_46F4A0(0, 0))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            if (field_1BC)
            {
                if (!(static_cast<int>(sGnFrame_5C1B84) % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
    }

    if ((sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC) || (sInputObject_5BD4E0.isPressed(field_1D4_previous_input)) &&
        (unsigned __int16)field_1D6_continue_move_input == sInputKey_Down_5550DC && 
        field_1D4_previous_input != (unsigned __int16)sInputKey_Up_5550D8 &&
        field_1BC != 2 &&
        field_1BC != 1) && 
        !IsBlocked_46F4A0(3, 0))
    {
        PathLine* pPathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - (kGridSize + k12Scaled),
            field_BC_ypos + (k5Scaled) - ((k12Scaled + kGridSize) * FP_FromDouble(0.5)),
            k12Scaled + kGridSize + field_B8_xpos,
            field_BC_ypos + (k5Scaled) - ((k12Scaled + kGridSize) * FP_FromDouble(0.5)),
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 4096 : 0x8000))
        {
            if (WallHit_408750(-FP_FromInteger(2), kGridSize + k12Scaled + FP_FromInteger(4)) ||
                WallHit_408750(k60Scaled - FP_FromInteger(1), kGridSize + k12Scaled + FP_FromInteger(4)))
            {
                if (hitX > field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), k5Scaled, (unsigned short)sInputKey_Down_5550DC, 2, 1);
                }
            }

            if (WallHit_408750(-FP_FromInteger(2), -(kGridSize + k12Scaled + FP_FromInteger(4))) ||
                WallHit_408750(k60Scaled - FP_FromInteger(1), -(kGridSize + k12Scaled + FP_FromInteger(4))))
            {
                if (hitX < field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), k5Scaled, (unsigned short)sInputKey_Down_5550DC, 1, 0);
                }
            }
        }
    }
    else if (IsBlocked_46F4A0(3, 0))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            if (field_1BC != 3 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
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
        //goto LABEL_165;
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
        field_11C_state = States::eState_3_Falling;
        //goto LABEL_165;
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
        && field_1BC == 0)
    {
        field_11C_state = States::eState_3_Falling;
        //goto LABEL_165;
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
        && field_1BC == 3)
    {
        Stop_46E570();
        //goto LABEL_165;
        return;
    }

    if (!field_1C4)
    {
        goto LABEL_127;
    }

    if (field_1C4 >= 7)
    {
        Stop_46E570();
        //goto LABEL_165;
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

LABEL_127:
    if (sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
    {
        goto LABEL_137;
    }

    if (field_1BC == 3 || field_1BC == 0)
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
    LABEL_137:
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;
        sub_408C40();
        //goto LABEL_164;
        RunThingsOver_46F380();
        return;
    }

    Stop_46E570();

    //goto LABEL_165;
    return;
}

void MineCar::State_3()
{
    sub_408C40();
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
            field_1BC = 3;
        }
    }
    //LABEL_164:
    RunThingsOver_46F380();
    //goto LABEL_165;
    return;
}

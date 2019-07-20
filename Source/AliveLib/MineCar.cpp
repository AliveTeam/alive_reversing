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

__int16 MineCar::IsBlocked_46F4A0(__int16 /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 MineCar::sub_46EA00()
{
    NOT_IMPLEMENTED();
    return 0;
}

void MineCar::RunThingsOver_46F380()
{
    NOT_IMPLEMENTED();
}

void MineCar::vUpdate_Real_46C010()
{
    NOT_IMPLEMENTED();
}

void MineCar::vUpdate_46C010()
{
   // NOT_IMPLEMENTED();

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
    //goto LABEL_165;
}

void MineCar::State_1()
{
    const FP kGridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);

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

        //goto LABEL_165;
        return;
    }

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);


    FP v62 = {};
    FP v63 = {};
    FP v64 = {};
    FP v65 = {};
    FP v66 = {};
    FP v67 = {};
    
    FP v19 = {};
    FP v20 = {};
    FP v21 = {};

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    int v13 = 0;

    const auto inputPressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (!(inputPressed & sInputKey_Right_5550D0))
    {
        //HIWORD(v13) = 0;
        if (!(field_1D4_previous_input & (unsigned __int16)inputPressed))
        {
            goto LABEL_177;
        }
        if ((unsigned __int16)field_1D6_continue_move_input != sInputKey_Right_5550D0)
        {
            goto LABEL_177;
        }
        const auto field_1bc_local = field_1BC;
        if (field_1bc_local == 3)
        {
            goto LABEL_177;
        }
        if (!field_1bc_local)
        {
            goto LABEL_177;
        }
        //LOWORD(v13) = field_1D4_previous_input;
        v13 = field_1D4_previous_input & 0xFFFF;
        if (v13 == (int)sInputKey_Left_5550D4)
        {
            goto LABEL_177;
        }
    }

    const FP v15 = (field_CC_sprite_scale * FP_FromInteger(12));
    const FP v16 = v15 + ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(1);
    const FP v17 = (field_CC_sprite_scale * FP_FromInteger(60));
    const FP v18 = (v17 * FP_FromDouble(0.5));


    if (WallHit_408750(v18, v16)
        || (v19 = (field_CC_sprite_scale * FP_FromInteger(12)),
            v20 = v19 + ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(1),
            v21 = (field_CC_sprite_scale * FP_FromInteger(60)),
            WallHit_408750(v21 - FP_FromInteger(8), v20)))
    {
    LABEL_177:
        const FP v46 = (field_CC_sprite_scale * FP_FromInteger(12));
        const FP v47 = v46 + ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(1);
        const FP v48 = (field_CC_sprite_scale * FP_FromInteger(60));
        const FP v49 = (v48 * FP_FromDouble(0.5));
        if (WallHit_408750(v49, v47))
        {
            if (sInputKey_Right_5550D0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                if (field_1BC != 2 && !((signed int)sGnFrame_5C1B84 % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
    }
    else
    {
        const FP v22 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v23 = (v22 / FP_FromInteger(4)) + field_B8_xpos;
        const FP v24 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60));
        const FP v25 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v26 = (v25 / FP_FromInteger(4));
        if (sCollisions_DArray_5C1128->Raycast_417A60(v26 + field_B8_xpos, v24, v23, field_BC_ypos, &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 2048 : 0x4000))
        {
            const FP v27 = (field_CC_sprite_scale * FP_FromInteger(12));
            const FP v28 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            FP v29 = {};
            FP v30 = {};
            if (CheckFloorCollision_46F730(v27 + v28 + FP_FromInteger(2), FP_FromInteger(4))
                || (v29 = (field_CC_sprite_scale * FP_FromInteger(12)), v30 = ScaleToGridSize_4498B0(field_CC_sprite_scale), CheckFloorCollision_46F730(FP_FromInteger(4) - (v29 + v30), FP_FromInteger(4))))
            {
                const FP v31 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP v32 = (v31 / FP_FromInteger(4));
                Move_46E640(20872u, v32, FP_FromInteger(0), (unsigned short)sInputKey_Right_5550D0, 3, 0);
                if (sInputKey_Right_5550D0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    //goto LABEL_165;
                    return; 
                }
            }
        }

        const FP v33 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v34 = (v33 / FP_FromInteger(4)) + field_B8_xpos;
        const FP v35 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60));
        const FP v36 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v37 = (v36 / FP_FromInteger(4));
        if (sCollisions_DArray_5C1128->Raycast_417A60(v37 + field_B8_xpos, v35, v34, field_BC_ypos, &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x2000 : 0x10000))
        {
            const FP v38 = -(field_CC_sprite_scale * FP_FromInteger(60));
            const FP v39 = (field_CC_sprite_scale * FP_FromInteger(12));
            const FP v40 = ScaleToGridSize_4498B0(field_CC_sprite_scale);

            FP v41 = {};
            FP v42 = {};
            FP v43 = {};
            if (CheckRoofCollision_46F6B0(v39 + v40 + FP_FromInteger(2), v38)
                || (v41 = -(field_CC_sprite_scale * FP_FromInteger(60)),
                    v42 = (field_CC_sprite_scale * FP_FromInteger(12)),
                    v43 = ScaleToGridSize_4498B0(field_CC_sprite_scale),
                    CheckRoofCollision_46F6B0(FP_FromInteger(4) - (v42 + v43), v41)))
            {
                const FP v44 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP v45 = (v44 / FP_FromInteger(4));
                Move_46E640(20872u, v45, FP_FromInteger(0), (unsigned short)sInputKey_Right_5550D0, 0, 1);
                if (sInputKey_Right_5550D0 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    //goto LABEL_165;
                    return;
                }
            }
        }
    }
    
    PSX_Point v51 = {};
    __int16 v52 = 0;
    FP v53 = {};
    FP v54 = {};
    FP v55 = {};
    FP v56 = {};
    const auto v50 = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
    if (!(v50 & sInputKey_Left_5550D4)
        && ((v51.field_2_y = 0, !(field_1D4_previous_input & (unsigned __int16)v50)) || (unsigned __int16)field_1D6_continue_move_input != sInputKey_Left_5550D4 || (v52 = field_1BC, v52 == 3) || (v51.field_0_x = field_1D4_previous_input, v51.field_0_x == (unsigned __int16)sInputKey_Right_5550D0) || !v52) // v51.field_0_x == sInputKey_Right_5550D0 X added
        || (v53 = (field_CC_sprite_scale * FP_FromInteger(12)),
            v54 = -(v53 + ScaleToGridSize_4498B0(field_CC_sprite_scale)),
            v55 = (field_CC_sprite_scale * FP_FromInteger(60)),
            v56 = (v55 * FP_FromDouble(0.5)),
            WallHit_408750(v56, v54)))
    {
        const FP v81 = (field_CC_sprite_scale * FP_FromInteger(12));
        const FP v82 = -(v81 + ScaleToGridSize_4498B0(field_CC_sprite_scale));
        const FP v83 = (field_CC_sprite_scale * FP_FromInteger(60));
        const FP v84 = (v83 * FP_FromDouble(0.5));
        if (WallHit_408750(v84, v82))
        {
            if (sInputKey_Left_5550D4 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
            {
                if (field_1BC != 1 && !((signed int)sGnFrame_5C1B84 % 6))
                {
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
        goto LABEL_63;
    }

    const FP v57 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP v58 = field_B8_xpos - (v57 / FP_FromInteger(4));
    const FP v59 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60));
    const FP v60 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP v61 = (v60 / FP_FromInteger(4));


    if (!sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos - v61, v59, v58, field_BC_ypos, &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x800 : 0x4000)
        || (v62 = (field_CC_sprite_scale * FP_FromInteger(12)), v63 = ScaleToGridSize_4498B0(field_CC_sprite_scale), !CheckFloorCollision_46F730(v62 + v63 - FP_FromInteger(4), FP_FromInteger(4)))
        && (v64 = (field_CC_sprite_scale * FP_FromInteger(12)), v65 = ScaleToGridSize_4498B0(field_CC_sprite_scale), !CheckFloorCollision_46F730(-(v64 + v65 + FP_FromInteger(2)), FP_FromInteger(4)))
        || (v66 = ScaleToGridSize_4498B0(field_CC_sprite_scale),
            v67 = (v66 / FP_FromInteger(4)),
            Move_46E640(20900u, -v67, FP_FromInteger(0), (unsigned short)sInputKey_Left_5550D4, 3, 1),
            !(sInputKey_Left_5550D4 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)))
    {
        const FP v68 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v69 = field_B8_xpos - (v68 / FP_FromInteger(4));
        const FP v70 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60));
        const FP v71 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v72 = (v71 / FP_FromInteger(4));
        if (!sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos - v72, v70, v69, field_BC_ypos, &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x2000 : 0x10000))
        {
            goto LABEL_63;
        }
        const FP v73 = -(field_CC_sprite_scale * FP_FromInteger(60));
        const FP v74 = (field_CC_sprite_scale * FP_FromInteger(12));
        const FP v75 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        if (!CheckRoofCollision_46F6B0(v74 + v75 - FP_FromInteger(4), v73))
        {
            const FP v76 = -(field_CC_sprite_scale * FP_FromInteger(60));
            const FP v77 = (field_CC_sprite_scale * FP_FromInteger(12));
            const FP v78 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            if (!CheckRoofCollision_46F6B0(-(v77 + v78 + FP_FromInteger(2)), v76))
            {
                goto LABEL_63;
            }
        }
        const FP v79 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        const FP v80 = (v79 / FP_FromInteger(4));
        Move_46E640(20900u, -v80, FP_FromInteger(0), (unsigned short)sInputKey_Left_5550D4, 0, 0);
        if (!(sInputKey_Left_5550D4 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed))
        {
        LABEL_63:
            const auto inputPressed2 = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            PSX_Point v86 = {};
            __int16 v87 = 0;
            if ((inputPressed2 & sInputKey_Up_5550D8
                || (v86.field_2_y = 0, field_1D4_previous_input & (unsigned __int16)inputPressed2) && (unsigned __int16)field_1D6_continue_move_input == sInputKey_Up_5550D8 && (v86.field_0_x = field_1D4_previous_input, v86.field_0_x != (unsigned __int16)sInputKey_Down_5550DC) && (v87 = field_1BC, v87 != 2) && v87 != 1) // X added (Same as above)
                && !IsBlocked_46F4A0(0, 0))
            {
                const FP v88 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v89 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP v90 = (v88 + v89) * FP_FromDouble(0.5);
                const FP v91 = field_BC_ypos - (FP_FromInteger(5) * field_CC_sprite_scale) - v90;
                const FP v92 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v93 = v92 + ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
                FP xy_2 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v94 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP v95 = (xy_2 + v94) * FP_FromDouble(0.5);
                const FP v96 = field_BC_ypos - (FP_FromInteger(5) * field_CC_sprite_scale) - v95;
                xy_2 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v97 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                if (sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos - (v97 + xy_2), v96, v93, v91, &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x1000 : 0x8000))
                {
                    const FP v98 = (field_CC_sprite_scale * FP_FromInteger(12));
                    const FP v99 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                    FP v100 = {};
                    FP v101 = {};
                    FP v102 = {};
                    if (WallHit_408750(FP_FromInteger(1), v99 + v98 + FP_FromInteger(4))
                        || (v100 = (field_CC_sprite_scale * FP_FromInteger(12)),
                            v101 = ScaleToGridSize_4498B0(field_CC_sprite_scale) + v100 + FP_FromInteger(4),
                            v102 = (field_CC_sprite_scale * FP_FromInteger(60)),
                            WallHit_408750(v102 + FP_FromInteger(1), v101)))
                    {
                        if (hitX > field_B8_xpos)
                        {
                            const FP v103 = (FP_FromInteger(5) * field_CC_sprite_scale);
                            Move_46E640(20836u, FP_FromInteger(0), -v103, (unsigned short)sInputKey_Up_5550D8, 2, 0);
                        }
                    }

                    const FP v104 = (field_CC_sprite_scale * FP_FromInteger(12));
                    const FP v105 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                    FP v106 = {};
                    FP v107 = {};
                    FP v108 = {};
                    if (WallHit_408750(FP_FromInteger(1), -(v105 + v104 + FP_FromInteger(4)))
                        || (v106 = (field_CC_sprite_scale * FP_FromInteger(12)),
                            v107 = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) + v106 + FP_FromInteger(4)),
                            v108 = (field_CC_sprite_scale * FP_FromInteger(60)),
                            WallHit_408750(v108 + FP_FromInteger(1), v107)))
                    {
                        if (hitX < field_B8_xpos)
                        {
                            const FP v109 = (FP_FromInteger(5) * field_CC_sprite_scale);
                            Move_46E640(20836u, FP_FromInteger(0), -v109, (unsigned short)sInputKey_Up_5550D8, 1, 1);
                        }
                    }
                }
            }
            else if (IsBlocked_46F4A0(0, 0))
            {
                if (sInputKey_Up_5550D8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    if (field_1BC)
                    {
                        if (!((signed int)sGnFrame_5C1B84 % 6))
                        {
                            SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                        }
                    }
                }
            }
            
            const auto v110 = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            __int16 v112 = 0;
            PSX_Point v111 = {};

            if ((v110 & sInputKey_Down_5550DC
                || (v111.field_2_y = 0, field_1D4_previous_input & (unsigned __int16)v110) && (unsigned __int16)field_1D6_continue_move_input == sInputKey_Down_5550DC && (v111.field_0_x = field_1D4_previous_input, v111.field_0_x != (unsigned __int16)sInputKey_Up_5550D8) && (v112 = field_1BC, v112 != 2) && v112 != 1) // X added as above
                && !IsBlocked_46F4A0(3, 0))
            {
                const FP v113 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v114 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP v115 = (v113 + v114) * FP_FromDouble(0.5);
                const FP v116 = field_BC_ypos + (FP_FromInteger(5) * field_CC_sprite_scale) - v115;
                const FP v117 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v118 = v117 + ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
                FP xy_2 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v119 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP v120 = (xy_2 + v119) * FP_FromDouble(0.5);
                const FP v121 = field_BC_ypos + (FP_FromInteger(5) * field_CC_sprite_scale) - v120;
                xy_2 = (field_CC_sprite_scale * FP_FromInteger(12));
                const FP v122 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                if (sCollisions_DArray_5C1128->Raycast_417A60(field_B8_xpos - (v122 + xy_2), v121, v118, v116, &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 4096 : 0x8000))
                {
                    const FP v123 = (field_CC_sprite_scale * FP_FromInteger(12));
                    const FP v124 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                    FP v125 = {};
                    FP v126 = {};
                    FP v127 = {};
                    if (WallHit_408750(-FP_FromInteger(2), v124 + v123 + FP_FromInteger(4))
                        || (v125 = (field_CC_sprite_scale * FP_FromInteger(12)),
                            v126 = ScaleToGridSize_4498B0(field_CC_sprite_scale) + v125 + FP_FromInteger(4),
                            v127 = (field_CC_sprite_scale * FP_FromInteger(60)),
                            WallHit_408750(v127 - FP_FromInteger(1), v126)))
                    {
                        if (hitX > field_B8_xpos)
                        {
                            const FP v128 = (FP_FromInteger(5) * field_CC_sprite_scale);
                            Move_46E640(20836u, FP_FromInteger(0), v128, (unsigned short)sInputKey_Down_5550DC, 2, 1);
                        }
                    }
                    const FP v129 = (field_CC_sprite_scale * FP_FromInteger(12));
                    const FP v130 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                    FP v131 = {};
                    FP v132 = {};
                    FP v133 = {};
                    if (WallHit_408750(-FP_FromInteger(2), -(v130 + v129 + FP_FromInteger(4)))
                        || (v131 = (field_CC_sprite_scale * FP_FromInteger(12)), // comma removed ??
                            v132 = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) + v131 + FP_FromInteger(4)),
                            v133 = (field_CC_sprite_scale * FP_FromInteger(60)),
                            WallHit_408750(v133 - FP_FromInteger(1), v132)))
                    {
                        if (hitX < field_B8_xpos)
                        {
                            const FP v134 = (FP_FromInteger(5) * field_CC_sprite_scale);
                            Move_46E640(20836u, FP_FromInteger(0), v134, (unsigned short)sInputKey_Down_5550DC, 1, 0);
                        }
                    }
                }
            }
            else if (IsBlocked_46F4A0(3, 0))
            {
                if (sInputKey_Down_5550DC & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    if (field_1BC != 3 && !((signed int)sGnFrame_5C1B84 % 6))
                    {
                        SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                    }
                }
            }
            //goto LABEL_165;
            return;
        }
    }
//    goto LABEL_165;
    return;

    /*
    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0) || sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
    {
        LOG_INFO("Right pressed");

        if (field_1D6_continue_move_input != (__int16)sInputKey_Right_5550D0 ||
            field_1BC == 3 ||
            field_1BC == 0 ||
            field_1D4_previous_input == (__int16)sInputKey_Left_5550D4)
        {
            if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)) - FP_FromInteger(8), (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + FP_FromInteger(1)))
            {
                LOG_INFO("Right far wall hit");
                if (WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60) * FP_FromDouble(0.5)), (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + FP_FromInteger(1)))
                {
                    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                    {
                        if (field_1BC != 2 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
                        {
                            LOG_INFO("Right BLOCKED");

                            // Can't move right
                            SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                        }
                    }
                }
            }
            else
            {
                LOG_INFO("Right checks...");

                FP hitX = {};
                FP hitY = {};
                PathLine* pPathLine = nullptr;

                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    (kGridScale / FP_FromInteger(4)) + field_B8_xpos,
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)),
                    (kGridScale / FP_FromInteger(4)) + field_B8_xpos,
                    field_BC_ypos,
                    &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 2048 : 0x4000))
                {
                    if (CheckFloorCollision_46F730((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + FP_FromInteger(2), FP_FromInteger(4)) ||
                        CheckFloorCollision_46F730(FP_FromInteger(4) - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale), FP_FromInteger(4)))
                    {
                        LOG_INFO("Right PATH 1...");

                        Move_46E640(20872u, (kGridScale / FP_FromInteger(4)), FP_FromInteger(0), (unsigned short)sInputKey_Right_5550D0, 3, FALSE);
                        if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                        {
                            //goto LABEL_165;
                            return;
                        }
                    }
                }

                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    (kGridScale / FP_FromInteger(4)) + field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)),
                    (kGridScale / FP_FromInteger(4)) + field_B8_xpos, field_BC_ypos,
                    &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x2000 : 0x10000))
                {
                    if (CheckRoofCollision_46F6B0(-(field_CC_sprite_scale * FP_FromInteger(60)) + kGridScale + FP_FromInteger(2), (field_CC_sprite_scale * FP_FromInteger(12))) ||
                        CheckRoofCollision_46F6B0(FP_FromInteger(4) - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale), -(field_CC_sprite_scale * FP_FromInteger(60))))
                    {
                        LOG_INFO("Right PATH 2...");

                        Move_46E640(20872u, (kGridScale / FP_FromInteger(4)), FP_FromInteger(0), (unsigned short)sInputKey_Right_5550D0, 0, TRUE);
                        if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                        {
                            //goto LABEL_165;
                            return;
                        }
                    }
                }
            }
        }
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4) || sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
    {
        LOG_INFO("Left pressed");
        if (field_1D6_continue_move_input != (__int16)sInputKey_Left_5550D4 ||
            field_1BC == 3 ||
            field_1BC == 0 ||
            field_1D4_previous_input == (__int16)sInputKey_Right_5550D0)
        {
            if (WallHit_408750(((field_CC_sprite_scale * FP_FromInteger(60)) * FP_FromDouble(0.5)), -((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale)))
            {
                LOG_INFO("Left far wall hit");

                if (WallHit_408750(((field_CC_sprite_scale * FP_FromInteger(60)) * FP_FromDouble(0.5)), -((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale)))
                {
                    if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                    {
                        if (field_1BC != 1 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
                        {
                            LOG_INFO("left BLOCKED");
                            // Can't move left
                            SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);

                        }
                    }
                }
            }
            else
            {
                LOG_INFO("Left checks...");

                FP hitX = {};
                FP hitY = {};
                PathLine* pPathLine = nullptr;
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    field_B8_xpos - (kGridScale / FP_FromInteger(4)),
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)),
                    field_B8_xpos - (kGridScale / FP_FromInteger(4)),
                    field_BC_ypos,
                    &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x800 : 0x4000))
                  
                {
                    if (CheckFloorCollision_46F730((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale - FP_FromInteger(4), FP_FromInteger(4)) ||
                        CheckFloorCollision_46F730(-((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + FP_FromInteger(2)), FP_FromInteger(4)))
                    {
                        LOG_INFO("Left PATH 1...");

                        Move_46E640(20900u, -(kGridScale / FP_FromInteger(4)), FP_FromInteger(0), (unsigned short)sInputKey_Left_5550D4, 3, TRUE);
                        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                        {
                            return;
                        }
                    }
                }

                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    field_B8_xpos - (kGridScale / FP_FromInteger(4)),
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(60)),
                    field_B8_xpos - (kGridScale / FP_FromInteger(4)),
                    field_BC_ypos,
                    &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x2000 : 0x10000))
                {
                    if (CheckRoofCollision_46F6B0((field_CC_sprite_scale *  FP_FromInteger(12)) + kGridScale - FP_FromInteger(4), -(field_CC_sprite_scale * FP_FromInteger(60))) ||
                        CheckRoofCollision_46F6B0(-((field_CC_sprite_scale *  FP_FromInteger(12)) + kGridScale + FP_FromInteger(2)), -(field_CC_sprite_scale * FP_FromInteger(60))))
                    {
                        LOG_INFO("Left PATH 2...");

                        Move_46E640(20900u, -(kGridScale / FP_FromInteger(4)), FP_FromInteger(0), (unsigned short)sInputKey_Left_5550D4, 0, FALSE);
                        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                        {
                            return;
                        }
                    }
                }
            }
        }
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8) ||
        (sInputObject_5BD4E0.isPressed(field_1D4_previous_input)) &&
        (unsigned __int16)field_1D6_continue_move_input == sInputKey_Up_5550D8 &&
        (field_1D4_previous_input != (unsigned short)sInputKey_Down_5550DC) &&
        (field_1BC != 2 && field_1BC != 1) &&
        !IsBlocked_46F4A0(0, 0))
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pPathLine = nullptr;
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - (kGridScale + (field_CC_sprite_scale * FP_FromInteger(12))),
            field_BC_ypos - (FP_FromInteger(5) * field_CC_sprite_scale) - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5),
            (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(5) * field_CC_sprite_scale) - (((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5)),
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 0x1000 : 0x8000))
        {
            if (WallHit_408750(FP_FromInteger(1), kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4)) ||
                WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)) + FP_FromInteger(1), kGridScale + (field_CC_sprite_scale * FP_FromInteger(12) + FP_FromInteger(4))))
            {
                if (hitX > field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), -(FP_FromInteger(5) * field_CC_sprite_scale), (unsigned short)sInputKey_Up_5550D8, 2, FALSE);
                }
            }

            if (WallHit_408750(FP_FromInteger(1), -(kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4))) ||
                WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)) + FP_FromInteger(1), -(kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4))))
            {
                if (hitX < field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), -(FP_FromInteger(5) * field_CC_sprite_scale), (unsigned short)sInputKey_Up_5550D8, 1, TRUE);
                }
            }
        }
    }
    else if (IsBlocked_46F4A0(0, 0))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            if (field_1BC != 0)
            {
                if (!(static_cast<int>(sGnFrame_5C1B84) % 6))
                {
                    // Can't move up
                    SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
                }
            }
        }
    }

    if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC) ||
        (sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
        (unsigned __int16)field_1D6_continue_move_input == sInputKey_Down_5550DC &&
            (field_1D4_previous_input != (unsigned short)sInputKey_Up_5550D8) &&
            (field_1BC != 2) && field_1BC != 1) &&
        !IsBlocked_46F4A0(3, 0))
    {

        FP hitX = {};
        FP hitY = {};
        PathLine* pPathLine = nullptr;
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - (kGridScale + (field_CC_sprite_scale * FP_FromInteger(12))),
            field_BC_ypos + (FP_FromInteger(5) * field_CC_sprite_scale) - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5),
            (field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale + field_B8_xpos,
            field_BC_ypos + (FP_FromInteger(5) * field_CC_sprite_scale) - ((field_CC_sprite_scale * FP_FromInteger(12)) + kGridScale) * FP_FromDouble(0.5),
            &pPathLine, &hitX, &hitY, field_D6_scale != 0 ? 4096 : 0x8000))
        {
            if (WallHit_408750(-FP_FromInteger(2), kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4)) ||
                WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)) - FP_FromInteger(1), kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4)))
            {
                if (hitX > field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), (FP_FromInteger(5) * field_CC_sprite_scale), (unsigned short)sInputKey_Down_5550DC, 2, TRUE);
                }
            }

            if (WallHit_408750(-FP_FromInteger(2), -(kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4))) ||
                WallHit_408750((field_CC_sprite_scale * FP_FromInteger(60)) - FP_FromInteger(1), -(kGridScale + (field_CC_sprite_scale * FP_FromInteger(12)) + FP_FromInteger(4))))
            {
                if (hitX < field_B8_xpos)
                {
                    Move_46E640(20836u, FP_FromInteger(0), (FP_FromInteger(5) * field_CC_sprite_scale), (unsigned short)sInputKey_Down_5550DC, 1, FALSE);
                }
            }
        }
    }
    else if (IsBlocked_46F4A0(3, 0))
    {
        if (sInputKey_Down_5550DC & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
        {
            if (field_1BC != 3 && !(static_cast<int>(sGnFrame_5C1B84) % 6))
            {
                // Can't move down
                SFX_Play_46FA90(102u, 127, field_CC_sprite_scale);
            }
        }
    }
    */

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

    if (sub_46EA00() && !field_1C4)
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

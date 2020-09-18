#include "stdafx.h"
#include "Slurg.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5C1C08, WORD, sSlurg_Step_Watch_Points_Idx_5C1C08, 0);
ALIVE_ARY(1, 0x5BD4DC, char, 2, sSlurg_Step_Watch_Points_Count_5BD4DC, {});
ALIVE_ARY(1, 0x5C1B28, Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28, {});

EXPORT void CC Slurg::Clear_Slurg_Step_Watch_Points_449A90()
{
    sSlurg_Step_Watch_Points_Idx_5C1C08 = !sSlurg_Step_Watch_Points_Idx_5C1C08;
    sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;

    for (auto& point : sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08].field_0_points)
    {
        point = {};
    }
}

TintEntry stru_560BCC[18] =
{
    { 1u, 102u, 127u, 118u },
    { 2u, 102u, 127u, 118u },
    { 3u, 102u, 127u, 118u },
    { 4u, 102u, 127u, 118u },
    { 5u, 102u, 127u, 118u },
    { 6u, 102u, 127u, 118u },
    { 7u, 102u, 127u, 118u },
    { 8u, 102u, 127u, 118u },
    { 9u, 102u, 127u, 118u },
    { 10u, 102u, 127u, 118u },
    { 11u, 102u, 127u, 118u },
    { 12u, 102u, 127u, 118u },
    { 13u, 102u, 127u, 118u },
    { 14u, 102u, 127u, 118u },
    { -1, 102u, 127u, 118u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

Slurg* Slurg::ctor_4C84E0(Path_Slurg* pTlv, DWORD tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x547720);

    field_128_pTlv = pTlv;

    field_11C_state = Slurg_States::State_0_Moving;

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSlurgResID);
    Animation_Init_424E10(2708, 46, 15, field_10_resources_array.ItemAt(0), 1, 1);

    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);
    field_4_typeId = Types::eSlurg_129;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    
    field_12C_tlvInfo = tlvInfo;

    if (pTlv->field_10.field_4_scale == 1)
    {
        field_130_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 14;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10.field_4_scale == 0)
    {
        field_130_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 33;
        field_D6_scale = 1;
    }

    field_11E_delay_timer = pTlv->field_10.field_0_pause_delay;
    field_120_delay_random = pTlv->field_10.field_0_pause_delay;

    SetTint_425600(&stru_560BCC[0], gMap_5C3030.field_0_current_level);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_124_pLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }

    field_11A_switch_id = pTlv->field_10.field_6_id;

    field_118_flags.Clear();

    if (pTlv->field_10.field_2_direction)
    {
        field_118_flags.Set(SlurgFlags::Bit1_Direction);
    }

    vStackOnObjectsOfType_425840(Types::eSlurg_129);
    field_DC_bApplyShadows |= 2u;
    field_E0_pShadow = ae_new<Shadow>();

    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* Slurg::VDestructor(signed int flags)
{
    return vdtor_4C8760(flags);
}

void Slurg::VUpdate()
{
    vUpdate_4C8790();
}

int Slurg::VGetSaveState(BYTE* pSaveBuffer)
{
    return vSaveState_4C8FC0(reinterpret_cast<Slurg_State*>(pSaveBuffer));
}

__int16 Slurg::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_4C8BF0(pFrom);
}

void Slurg::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_TLV_Collision_4C8C20(pTlv);
}


signed int CC Slurg::CreateFromSaveState_4C8DF0(const BYTE* pData)
{
    auto pState = reinterpret_cast<const Slurg_State*>(pData);
    auto pTlv = static_cast<Path_Slurg*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_24_tlvInfo));
    
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSlurgResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLURG.BAN", nullptr);
    }

    auto pSlurg = ae_new<Slurg>();
    if (pSlurg)
    {
        pSlurg->ctor_4C84E0(pTlv, pState->field_24_tlvInfo);
    }

    pSlurg->field_B8_xpos = pState->field_4_xpos;
    pSlurg->field_BC_ypos = pState->field_8_ypos;
    pSlurg->field_C4_velx = pState->field_C_velx;
    pSlurg->field_20_animation.field_E_frame_change_counter = pState->field_1A_anim_frame_change_counter;

    // OG BUG: This wasn't restored
    pSlurg->field_20_animation.field_92_current_frame = pState->field_18_anim_current_frame;
    pSlurg->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_14_flipX & 1);
    pSlurg->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_1C & 1);

    pSlurg->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_1D & 1);

    if (IsLastFrame(&pSlurg->field_20_animation))
    {
        pSlurg->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pSlurg->field_11C_state = pState->field_28_state;

    pSlurg->field_118_flags.Set(SlurgFlags::Bit1_Direction, pState->field_2A_flags.Get(SlurgFlags::Bit1_Direction));
    pSlurg->field_118_flags.Set(SlurgFlags::Bit2_StartToMove, pState->field_2A_flags.Get(SlurgFlags::Bit2_StartToMove));
    return sizeof(Slurg_State);
}

Slurg* Slurg::vdtor_4C8760(signed int flags)
{
    dtor_4C8A40();

    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Slurg::dtor_4C8A40()
{
    SetVTable(this, 0x547720);
    if (field_12C_tlvInfo == -1)
    {
        Path::TLV_Reset_4DB8E0(0xFFFFFFFF, -1, 0, field_11C_state == Slurg_States::State_2_Burst);
    }
    dtor_4080B0();
}

void Slurg::Burst_4C8AE0()
{
    field_11C_state = Slurg_States::State_2_Burst;
    field_20_animation.Set_Animation_Data_409C80(2808, 0);
    
    auto pBlood = ae_new<Blood>();
    if (pBlood)
    {
        pBlood->ctor_40F0B0(
            field_B8_xpos,
            field_BC_ypos,
            FP_FromInteger(0),
            FP_FromInteger(5),
            field_130_scale,
            20);
    }

    Event_Broadcast_422BC0(kEventLoudNoise, this);
    SFX_Play_46FA90(SoundEffect::SlurgKill_89, 127, field_130_scale);

    if (field_11A_switch_id > 1)
    {
        SwitchStates_Add_466060(field_11A_switch_id, 1);
    }
}

void Slurg::vUpdate_4C8790()
{
    const FP oldXPos = field_B8_xpos;
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_11E_delay_timer == 0)
    {
        field_11E_delay_timer = Math_RandomRange_496AB0(field_120_delay_random, field_120_delay_random + 20);
        field_11C_state = Slurg_States::State_1_Stopped;
        field_20_animation.Set_Animation_Data_409C80(2740, 0);
    }

    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);

    if (field_11C_state != Slurg_States::State_2_Burst)
    {
        const int idx = sSlurg_Step_Watch_Points_Idx_5C1C08 == 0;
        const int max_count = sSlurg_Step_Watch_Points_Count_5BD4DC[idx];
        for (int i=0; i<max_count; i++)
        {
            const Slurg_Step_Watch_Point* pPoint = &sSlurg_Step_Watch_Points_5C1B28[idx].field_0_points[i];
            if (pPoint->field_0_xPos > bRect.x - 2 &&
                pPoint->field_0_xPos < bRect.w + 2 &&
                pPoint->field_2_yPos > bRect.y - 4 &&
                pPoint->field_2_yPos < bRect.h + 4
                )
            {
                Burst_4C8AE0();
                break;
            }
        }
    }

    if (field_11C_state == Slurg_States::State_0_Moving)
    {
        field_C4_velx = FP_FromInteger(1);
        field_11E_delay_timer--;
        if (field_118_flags.Get(SlurgFlags::Bit1_Direction))
        {
            field_C4_velx = -FP_FromInteger(1);
        }

        field_118_flags.Toggle(SlurgFlags::Bit2_StartToMove);

        if (field_118_flags.Get(SlurgFlags::Bit2_StartToMove))
        {
            field_B8_xpos += field_C4_velx;
        }
    }
    else if (field_11C_state == Slurg_States::State_1_Stopped)
    {
        field_C4_velx = FP_FromInteger(0);
        if (!field_20_animation.field_92_current_frame
            && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            SFX_Play_46FA90(SoundEffect::SlurgStop_90, 0);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_11C_state = Slurg_States::State_0_Moving;
            field_20_animation.Set_Animation_Data_409C80(2708, 0);
        }
    }
    else if (field_11C_state == Slurg_States::State_2_Burst)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    if (oldXPos != field_B8_xpos)
    {
        field_128_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);

        VOn_TLV_Collision_4087F0(field_128_pTlv);
    }
}

__int16 Slurg::vTakeDamage_4C8BF0(BaseGameObject* pFrom)
{
    // Slurgs are tough little dudes, only paramites can smack 'em up
    if (pFrom->field_4_typeId == Types::eParamite_96)
    {
        Burst_4C8AE0();
        return 1;
    }

    return 0;
}

void Slurg::vOn_TLV_Collision_4C8C20(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::ScrabLeftBound_43)
        {
            if (field_118_flags.Get(SlurgFlags::Bit1_Direction))
            {
                GoLeft();
            }
        }
        else if (pTlv->field_4_type == TlvTypes::ScrabRightBound_44)
        {
            if (!field_118_flags.Get(SlurgFlags::Bit1_Direction))
            {
                GoRight();
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }

    if (field_118_flags.Get(SlurgFlags::Bit1_Direction))
    {
        if (WallHit_408750(field_130_scale * FP_FromInteger(8), -(field_130_scale * FP_FromInteger(6))) || Check_IsOnEndOfLine_408E90(1, 1))
        {
            GoLeft();
        }
    }
    else
    {
        if (WallHit_408750(field_130_scale * FP_FromInteger(8), field_130_scale * FP_FromInteger(6))|| Check_IsOnEndOfLine_408E90(0, 1))
        {
            GoRight();
        }
    }
}

signed int Slurg::vSaveState_4C8FC0(Slurg_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = Types::eSlurg_129;
    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_scale = field_130_scale;
    pState->field_14_flipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_16_current_motion = field_106_current_motion;
    pState->field_18_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_1A_anim_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_1D = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_1C = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_20_frame_table_offset = field_20_animation.field_18_frame_table_offset;
    pState->field_24_tlvInfo = field_12C_tlvInfo;
    pState->field_28_state = field_11C_state;
    pState->field_2A_flags.Set(SlurgFlags::Bit1_Direction, field_118_flags.Get(SlurgFlags::Bit1_Direction));
    pState->field_2A_flags.Set(SlurgFlags::Bit2_StartToMove, field_118_flags.Get(SlurgFlags::Bit2_StartToMove));
    return sizeof(Slurg_State);
}

void Slurg::GoLeft()
{
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    field_118_flags.Clear(SlurgFlags::Bit1_Direction);

    field_11C_state = Slurg_States::State_1_Stopped;
    field_20_animation.Set_Animation_Data_409C80(2740, 0);
}

void Slurg::GoRight()
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    field_118_flags.Set(SlurgFlags::Bit1_Direction);

    field_11C_state = Slurg_States::State_1_Stopped;
    field_20_animation.Set_Animation_Data_409C80(2740, 0);
}

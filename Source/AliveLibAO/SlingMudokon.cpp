#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlingMudokon.hpp"
#include "GameSpeak.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"
#include "Bullet.hpp"
#include "stdlib.hpp"

void SlingMud_ForceLink() { }

START_NS_AO

using TSlingMudStateFunction = decltype(&SlingMudokon::State_0_Idle_46FCB0);

const TSlingMudStateFunction gSlingMudMotionTable_4CFCB0[] =
{
    &SlingMudokon::State_0_Idle_46FCB0,
    &SlingMudokon::State_1_Angry_46FCF0,
    &SlingMudokon::State_2_Speak_46FD70,
    &SlingMudokon::State_3_ShootStart_46FD90,
    &SlingMudokon::State_4_ShootEnd_46FEA0,
    &SlingMudokon::State_5_AngryToIdle_46FD50
};

const int sSlingMudFrameTables_4CFCC8[6] = { 22636, 22676, 22744, 22700, 22720, 22780 };

using TSlingMudBrain = decltype(&SlingMudokon::tsub_46FEC0);

const TSlingMudBrain gSlingMudBrainTable_4CFCE0[] =
{ 
    &SlingMudokon::tsub_46FEC0,
    &SlingMudokon::tsub_470230,
    &SlingMudokon::tsub_4707B0
};

SlingMudokon* SlingMudokon::ctor_46F940(Path_SlingMud* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BCB40);

    field_4_typeId = Types::SlingMud_90;

    field_120 = -1;
    field_134_buffer_start = 0;
    field_136 = -1;
    field_150_res = 0;

    field_150_res = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 508, 1, 0);
    Animation_Init_417FD0(
        22636,
        96,
        55,
        field_150_res,
        1);

    field_FC_current_motion = eSlingMudStates::State_0_Idle_46FCB0;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10) == 1)
    {
        field_A8_xpos = hitX;
        field_AC_ypos = hitY;
    }

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    if (pTlv->field_1A_silent)
    {
        field_138_brain_state = 2;
    }
    else
    {
        field_138_brain_state = 1;
    }

    field_118_code_converted = Code_Convert_476000(pTlv->field_1C_code_1, pTlv->field_1E_code_2);
    field_11C_code_length = Code_Length_475FD0(field_118_code_converted);

    field_154 = 99;
    field_156 = 99;

    field_110_tlvInfo = tlvInfo;
    field_F8_pLiftPoint = nullptr;
    field_13A_brain_state = 0;
    field_15A = 0;

    field_11E_flags &= ~0xA;
    field_11E_flags |= 1;

    return this;
}

BaseGameObject* SlingMudokon::dtor_46FB30()
{
    SetVTable(this, 0x4BCB40);

    if (field_11E_flags & 1)
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 1);
    }

    // TODO: Check it isn't 2 resources freed here
    if (field_10_anim.field_20_ppBlock != field_150_res)
    {
        if (field_150_res)
        {
            ResourceManager::FreeResource_455550(field_150_res);
        }
    }

    return dtor_401000();
}

BaseGameObject* SlingMudokon::VDestructor(signed int flags)
{
    return Vdtor_470F30(flags);
}

BaseGameObject* SlingMudokon::Vdtor_470F30(signed int flags)
{
    dtor_46FB30();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SlingMudokon::VScreenChanged()
{
    VScreenChanged_46FBE0();
}

void SlingMudokon::VScreenChanged_46FBE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SlingMudokon::VUpdate_46FBF0()
{
    const auto old_motion = field_FC_current_motion;
    
    VCallBrain_46F880();

    const auto old_x = field_A8_xpos;
    const auto old_y = field_AC_ypos;

    VCallMotion_46F8E0();

    if (old_x != field_A8_xpos || old_y != field_AC_ypos)
    {
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
            nullptr,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);
  
        VOn_TLV_Collision(field_F0_pTlv);
    }

    field_106_shot = 0;

    if (old_motion == field_FC_current_motion)
    {
        if (field_13C)
        {
            field_FC_current_motion = field_E4_previous_motion;
            VUpdateAnimData_46F8F0();
            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            field_13C = 0;
        }
    }
    else
    {
        VUpdateAnimData_46F8F0();
    }
}

void SlingMudokon::VCallBrain_46F880()
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_13A_brain_state = (this->*gSlingMudBrainTable_4CFCE0[field_138_brain_state])();
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void SlingMudokon::VCallMotion_46F8E0()
{
    (this->*gSlingMudMotionTable_4CFCB0[field_FC_current_motion])();
}

void SlingMudokon::VUpdateAnimData_46F8F0()
{
    BYTE** ppResBlock = nullptr;

    if (field_FC_current_motion >= 6)
    {
        ppResBlock = nullptr;
    }
    else
    {
        ppResBlock = field_150_res;
    }

    field_10_anim.Set_Animation_Data_402A40(sSlingMudFrameTables_4CFCC8[field_FC_current_motion], ppResBlock);
}

void SlingMudokon::State_0_Idle_46FCB0()
{
    if (field_FE_next_state == eSlingMudStates::State_1_Angry_46FCF0)
    {
        field_FC_current_motion = eSlingMudStates::State_1_Angry_46FCF0;
        field_FE_next_state = -1;
    }
    else if (!field_10_anim.field_92_current_frame && field_FE_next_state == eSlingMudStates::State_2_Speak_46FD70)
    {
        field_FC_current_motion = eSlingMudStates::State_2_Speak_46FD70;
        field_FE_next_state = -1;
    }
}

void SlingMudokon::State_1_Angry_46FCF0()
{
    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_43AD70(SoundEffect::SlingshotExtend_83, 0, 0);
    }

    if (field_FE_next_state == eSlingMudStates::State_3_ShootStart_46FD90)
    {
        field_FC_current_motion = eSlingMudStates::State_3_ShootStart_46FD90;
        field_FE_next_state = -1;
    }
    else if (field_FE_next_state == eSlingMudStates::State_5_AngryToIdle_46FD50)
    {
        field_FC_current_motion = eSlingMudStates::State_5_AngryToIdle_46FD50;
        field_FE_next_state = -1;
    }
}

void SlingMudokon::State_2_Speak_46FD70()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10_anim.field_18_frame_table_offset == 22744)
        {
            field_FC_current_motion = eSlingMudStates::State_0_Idle_46FCB0;
        }
    }
}

void SlingMudokon::State_3_ShootStart_46FD90()
{
    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_43AD70(SoundEffect::SlingshotShoot_84, 0, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10_anim.field_18_frame_table_offset == 22700)
        {
            const FP frame_x = FP_FromInteger(field_10_anim.Get_FrameHeader_403A00(-1)->field_8_data.points[2].x);

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                auto pBullet = ao_new<Bullet>();
                if (pBullet)
                {
                    pBullet->ctor_409380(
                        this,
                        BulletType::Type_1,
                        field_A8_xpos - frame_x,
                        field_AC_ypos - FP_FromInteger(24),
                        -FP_FromInteger(640),
                        0,
                        field_BC_sprite_scale,
                        0);
                }
            }
            else
            {
                auto pBullet = ao_new<Bullet>();
                if (pBullet)
                {
                    pBullet->ctor_409380(
                        this,
                        BulletType::Type_1,
                        frame_x + field_A8_xpos,
                        field_AC_ypos - FP_FromInteger(24),
                        FP_FromInteger(640),
                        0,
                        field_BC_sprite_scale,
                        0);
                }
            }
            field_FC_current_motion = eSlingMudStates::State_4_ShootEnd_46FEA0;
        }
    }
}

void SlingMudokon::State_4_ShootEnd_46FEA0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlingMudStates::State_0_Idle_46FCB0;
    }
}

void SlingMudokon::State_5_AngryToIdle_46FD50()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlingMudStates::State_0_Idle_46FCB0;
    }
}

__int16 SlingMudokon::tsub_46FEC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 SlingMudokon::tsub_470230()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 SlingMudokon::tsub_4707B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

END_NS_AO

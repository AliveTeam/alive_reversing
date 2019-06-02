#include "stdafx.h"
#include "Fleech.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Events.hpp"

ALIVE_VAR(1, 0x5BC20C, BYTE, sFleechRandomIdx_5BC20C, 0);
ALIVE_VAR(1, 0x5BC20E, short, sFleechCount_5BC20E, 0);

const TFleechMotionFn sFleech_motion_table_551798[19] =
{
    &Fleech::M_0_42F0B0,
    &Fleech::M_1_42F270,
    &Fleech::M_2_42F2F0,
    &Fleech::M_3_42E850,
    &Fleech::M_4_42E960,
    &Fleech::M_5_42E810,
    &Fleech::M_6_42EAF0,
    &Fleech::M_7_42EBB0,
    &Fleech::M_8_42EB20,
    &Fleech::M_9_42ECD0,
    &Fleech::M_10_42F330,
    &Fleech::M_11_42F590,
    &Fleech::M_12_42F7F0,
    &Fleech::M_13_42FB00,
    &Fleech::M_14_42FBD0,
    &Fleech::M_15_42FC40,
    &Fleech::M_16_42FCE0,
    &Fleech::M_17_42F370,
    &Fleech::M_18_42FDF0
};

const TFleechAIFn sFleech_ai_table_551830[4] =
{
    &Fleech::AI_0_430BA0,
    &Fleech::AI_1_428760,
    &Fleech::AI_2_42D310,
    &Fleech::AI_3_42D1E0
};

static BYTE Fleech_NextRandom()
{
    return sRandomBytes_546744[sFleechRandomIdx_5BC20C++];
}

Fleech* Fleech::ctor_429DC0(Path_Fleech* pTlv, int tlvInfo)
{
    ctor_408240(2);
    SetVTable(this, 0x544F28);
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_C_objectId = tlvInfo;

    if (pTlv->field_10_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
    }

    Init_42A170();

    field_118_tlvInfo = tlvInfo;
    field_124_brain_state = 0;
    field_11C_obj_id = -1;
    field_170 = -1;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == 0);

    field_174_flags.Set(Flags_174::eBit5_asleep, pTlv->field_14_asleep & 1);
    field_174_flags.Set(Flags_174::eBit6_goes_to_sleep, pTlv->field_24_goes_to_sleep & 1);
    field_174_flags.Set(Flags_174::eBit7_persistant, pTlv->field_2C_persistant & 1);

    field_140 = 2;
    field_158 = 10;

    field_142_attack_anger = pTlv->field_1A_attack_anger + 2;
    field_144_wake_up_id = pTlv->field_1E_wake_up_id;
    field_146_tlv_28 = pTlv->field_28_unused;
    field_148_wake_up_switch_value = SwitchStates_Get_466020(pTlv->field_1E_wake_up_id) & 0xFFFF;
    field_14A_allow_wake_up_id = pTlv->field_2A_allow_wake_up_id;
    field_150_patrol_range = FP_GetExponent(FP_FromInteger(pTlv->field_26_patrol_range_in_grids) * ScaleToGridSize_4498B0(field_CC_sprite_scale));
    field_15C_lost_target_timeout = pTlv->field_22_lost_target_timeout;

    InitPolys_42B6E0();

    field_13E_anger = 0;

    if (pTlv->field_20_hanging)
    {
        field_160 = (pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2;
        field_166_angle = Fleech_NextRandom();
        field_BC_ypos += FP_FromInteger(pTlv->field_8_top_left.field_2_y - pTlv->field_C_bottom_right.field_2_y);
        sub_42B9A0((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2, pTlv->field_8_top_left.field_2_y);
        field_106_current_motion = 17;
        sub_429D80();
    }
    else if (field_174_flags.Get(Flags_174::eBit5_asleep))
    {
        field_106_current_motion = 0;
        sub_429D80();
    }
    else
    {
        field_106_current_motion = 3;
        field_13E_anger = field_140 + (field_142_attack_anger - field_140) / 2;
    }

    return this;
}

BaseGameObject* Fleech::VDestructor(signed int flags)
{
    return vdtor_42A140(flags);
}

void Fleech::VUpdate()
{
    vUpdate_42AB20();
}

void Fleech::M_0_42F0B0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_1_42F270()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_2_42F2F0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_3_42E850()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_4_42E960()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_5_42E810()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_6_42EAF0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_7_42EBB0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_8_42EB20()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_9_42ECD0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_10_42F330()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_11_42F590()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_12_42F7F0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_13_42FB00()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_14_42FBD0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_15_42FC40()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_16_42FCE0()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_17_42F370()
{
    NOT_IMPLEMENTED();
}

void Fleech::M_18_42FDF0()
{
    NOT_IMPLEMENTED();
}

__int16 Fleech::AI_0_430BA0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_1_428760()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_2_42D310()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_3_42D1E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Fleech::dtor_42A3A0()
{
    SetVTable(this, 0x544F28);
    field_170 = -1;
    if (field_118_tlvInfo != 0xFFFF)
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, field_10C_health <= FP_FromInteger(0));
    }

    MusicController::sub_47FD60(0, this, 0, 0);
    sub_42CF70();

    if (field_106_current_motion == 18)
    {
        if (sActiveHero_5C1B68)
        {
            if (field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id)
            {
                sActiveHero_5C1B68->sub_459430(); // kills abe ??
            }
        }
    }
    field_11C_obj_id = -1;

    if (!field_174_flags.Get(Flags_174::eBit3))
    {
        sFleechCount_5BC20E--;
    }

    dtor_4080B0();
}

Fleech* Fleech::vdtor_42A140(signed int flags)
{
    dtor_42A3A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Fleech::vUpdate_42AB20()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
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
        }
        field_104_collision_line_type = 0;
        field_11C_obj_id = BaseGameObject::Find_Flags_4DC170(field_11C_obj_id);
        field_170 = BaseGameObject::Find_Flags_4DC170(field_170);
        field_110_id = BaseGameObject::Find_Flags_4DC170(field_110_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if ((FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos) <= FP_FromInteger(750) &&
        FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos) <= FP_FromInteger(520)) ||
        field_174_flags.Get(Flags_174::eBit7_persistant))
    {
        const auto oldMotion = field_106_current_motion;
        field_126_state = (this->*sFleech_ai_table_551830[field_124_brain_state])();

        sub_42BD30();

        if (field_BC_ypos < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        const FP oldY = field_BC_ypos;
        const FP oldX = field_B8_xpos;

        (this->*sFleech_motion_table_551798[field_106_current_motion])();

        if (oldX != field_B8_xpos || oldY != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != field_106_current_motion)
        {
            LOG_INFO("Fleech: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        if (oldMotion == field_106_current_motion)
        {
            if (field_128)
            {
                field_106_current_motion = field_F4;
                sub_429D80();
                field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                field_128 = 0;
            }
        }
        else
        {
            sub_429D80();
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Fleech::Init_42A170()
{
    NOT_IMPLEMENTED();
}

void Fleech::InitPolys_42B6E0()
{
    NOT_IMPLEMENTED();
}

void Fleech::sub_429D80()
{
    NOT_IMPLEMENTED();
}

void Fleech::sub_42CF70()
{
    NOT_IMPLEMENTED();
}

void Fleech::sub_42B9A0(__int16 a2, __int16 a3)
{
    field_18A |= 2u;
    field_178 = 2;
    field_186 = a3;
    field_184 = a2;
    field_188 = 0;
}

void Fleech::sub_42BD30()
{
    NOT_IMPLEMENTED();
}

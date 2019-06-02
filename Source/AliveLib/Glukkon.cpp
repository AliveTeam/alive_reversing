#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Game.hpp"

const TGlukkonMotionFn sGlukkon_motion_table_5544C0[25] = 
{
    &Glukkon::M_0_442D10,
    &Glukkon::M_1_442D30,
    &Glukkon::M_2_442F10,
    &Glukkon::M_3_442F40,
    &Glukkon::M_4_443030,
    &Glukkon::M_5_4434C0,
    &Glukkon::M_6_4434E0,
    &Glukkon::M_7_443510,
    &Glukkon::M_8_443760,
    &Glukkon::M_9_443790,
    &Glukkon::M_10_443B50,
    &Glukkon::M_11_4437D0,
    &Glukkon::M_12_4438F0,
    &Glukkon::M_13_443930,
    &Glukkon::M_14_443950,
    &Glukkon::M_15_443970,
    &Glukkon::M_16_4439B0,
    &Glukkon::M_17_4439D0,
    &Glukkon::M_18_443A00,
    &Glukkon::M_19_443A30,
    &Glukkon::M_20_442FC0,
    &Glukkon::M_21_443A60,
    &Glukkon::M_22_443010,
    &Glukkon::M_23_443910,
    &Glukkon::M_24_443990
};

Glukkon* Glukkon::ctor_43F030(Path_Glukkon* pTlv, int tlvInfo)
{
    ctor_408240(0);
    field_1EC = -1;
    SetVTable(this, 0x5452E0);

    field_1A8_tlvData = *pTlv;

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_214_tlv_info = tlvInfo;

    // TODO: Resource IDs
    switch (field_1A8_tlvData.field_22_glukkon_type)
    {
    case GlukkonTypes::Normal_0:
    case GlukkonTypes::Normal_4:
    case GlukkonTypes::Normal_5:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 801);
        Animation_Init_424E10(169608, 163, 79, Add_Resource_4DC130(ResourceManager::Resource_Animation, 800), 1, 1);
        break;

    case GlukkonTypes::Aslik_1:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 803);
        Animation_Init_424E10(171356, 157, 76, Add_Resource_4DC130(ResourceManager::Resource_Animation, 802), 1, 1);
        break;

    case GlukkonTypes::Drpik_2:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 805);
        Animation_Init_424E10(193064,162,78, Add_Resource_4DC130(ResourceManager::Resource_Animation, 804), 1, 1);
        break;

    case GlukkonTypes::Phleg_3:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 807);
        Animation_Init_424E10(199500, 145, 72, Add_Resource_4DC130(ResourceManager::Resource_Animation, 806), 1, 1);
        break;
    default:
        break;
    }
    
    Init_43F260();

    return this;
}

BaseGameObject* Glukkon::VDestructor(signed int flags)
{
    return vdtor_43F230(flags);
}

void Glukkon::VUpdate()
{
    vUpdate_43F770();
}

void Glukkon::M_0_442D10()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_1_442D30()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_2_442F10()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_3_442F40()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_4_443030()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_5_4434C0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_6_4434E0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_7_443510()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_8_443760()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_9_443790()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_10_443B50()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_11_4437D0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_12_4438F0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_13_443930()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_14_443950()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_15_443970()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_16_4439B0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_17_4439D0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_18_443A00()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_19_443A30()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_20_442FC0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_21_443A60()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_22_443010()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_23_443910()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_24_443990()
{
    NOT_IMPLEMENTED();
}

void Glukkon::Init_43F260()
{
    NOT_IMPLEMENTED();
}

Glukkon* Glukkon::vdtor_43F230(signed int flags)
{
    dtor_43F570();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Glukkon::dtor_43F570()
{
    SetVTable(this, 0x5452E0);

    if (field_10C_health <= FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_214_tlv_info, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_214_tlv_info, -1, 0, 0);
    }

    field_6_flags.Set(BaseGameObject::eDrawable); // Seems wrong to do this here ??

    if (this == sControlledCharacter_5C1B8C)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
    }

    dtor_4080B0();
}

void Glukkon::vUpdate_43F770()
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

            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                PSX_Point xy = { bRect.x, bRect.y };
                PSX_Point wh = { bRect.w, bRect.h };
                xy.field_2_y += 5;
                wh.field_2_y += 5;

                vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        field_208_obj_id = BaseGameObject::Find_Flags_4DC170(field_208_obj_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!Input_IsChanting_45F260())
        {
            field_1E2_bUnknown = 0;
        }

        const auto oldMotion = field_106_current_motion;

        field_210 = (this->*field_20C_brain_state_fn)();

        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;

        (this->*sGlukkon_motion_table_5544C0[field_106_current_motion])();

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != field_106_current_motion)
        {
            LOG_INFO("Glukkon: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(pTlv);
        }
        
        sub_440600();
        
        if (sControlledCharacter_5C1B8C == this && field_110_id != -1)
        {
            field_C8_vely = field_BC_ypos - field_1DC;
            sub_408C40();
        }
        field_1DC = field_BC_ypos;
    }
}

void Glukkon::sub_440600()
{
    NOT_IMPLEMENTED();
}

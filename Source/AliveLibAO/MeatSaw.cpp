#include "stdafx_ao.h"
#include "Function.hpp"
#include "MeatSaw.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Abe.hpp"
#include "Game.hpp"

START_NS_AO

void MeatSaw::VScreenChanged()
{
    VScreenChanged_43A060();
}

MeatSaw* MeatSaw::Vdtor_43A0B0(signed int flags)
{
    dtor_4398F0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* MeatSaw::VDestructor(signed int flags)
{
    return Vdtor_43A0B0(flags);
}

BaseGameObject* MeatSaw::dtor_4398F0()
{
    SetVTable(this, 0x4BB990);
    if ((field_1A8_flags & 1) && SwitchStates_Get(field_EE_switch_id) != field_F2_switch_value)
    {
        gMap_507BA8.TLV_Reset_446870(field_100_tlvInfo, 1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_100_tlvInfo, 0, 0, 0);
    }

    field_110_anim.vCleanUp();
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return dtor_417D10();
}

MeatSaw* MeatSaw::ctor_439570(Path_MeatSaw* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB990);
    field_4_typeId = Types::eMeatSaw_56;

    SetVTable(&field_110_anim, 0x4BA2B8);

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6003, 1, 0);
    Animation_Init_417FD0(15200, 104, 36, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = 0;

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 5;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 24;
        field_C6_scale = 1;
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 8);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    field_F6_min_time_off1 = pTlv->field_1A_min_time_off1;
    field_F8_field_1C_max_time_off1 = pTlv->field_1C_max_time_off1;

    field_E6_max_rise_time = pTlv->field_1E_max_rise_time;
    field_EE_switch_id = pTlv->field_20_id;

    field_C8 = 0;
    field_F4 = 0;

    if (pTlv->field_22_type == 1)
    {
        field_1A8_flags &= ~2;
        field_1A8_flags |= 1;
    }
    else if (pTlv->field_22_type == 2)
    {
        field_1A8_flags |= 3u;
    }
    else // 0
    {
        field_1A8_flags &= ~3u;
       
    }

    field_EA_speed1 = pTlv->field_24_speed;
    field_E8_speed2 = pTlv->field_24_speed;

    if (pTlv->field_26_start_state == 0)
    {
        field_F0_switch_value = SwitchStates_Get(pTlv->field_20_id) == 0;
    }
    else
    {
        if (pTlv->field_20_id)
        {
            field_F0_switch_value = SwitchStates_Get(pTlv->field_20_id);
        }
        else
        {
            field_F0_switch_value = 0;
        }
    }

    if (pTlv->field_20_id)
    {
        field_F2_switch_value = SwitchStates_Get(pTlv->field_20_id);
    }
    else
    {
        field_F2_switch_value = 0;
    }

    field_FA_min_time_off2 = pTlv->field_2A_min_time_off2;

    field_EC_off_speed = pTlv->field_28_off_speed;
    field_FC_max_time_off2 = pTlv->field_2C_max_time_off2;

    if (pTlv->field_1_unknown)
    {
        field_F0_switch_value = field_F0_switch_value == 0;
        field_F2_switch_value = field_F2_switch_value == 0;
    }

    field_104 = 0;
    field_E4_state = 0;
    field_10C = 0;
    field_AC_ypos -= FP_FromInteger(pTlv->field_1E_max_rise_time);
    field_100_tlvInfo = tlvInfo;

    if (!pTlv->field_2E_inital_position == 0)
    {
        field_E4_state = 2;
        field_F4 = pTlv->field_1E_max_rise_time + pTlv->field_24_speed - pTlv->field_1E_max_rise_time % pTlv->field_24_speed;
    }

    BYTE** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6003, 1, 0);
    if (field_110_anim.Init_402D20(
        15252,
        gObjList_animations_505564,
        this,
        104,
        36,
        ppRes2,
        1,
        0,
        0))
    {
        field_110_anim.field_C_layer = field_10_anim.field_C_layer;
        field_110_anim.field_14_scale = field_BC_sprite_scale;

        field_110_anim.field_8_r = static_cast<BYTE>(field_C0_r);
        field_110_anim.field_9_g = static_cast<BYTE>(field_C2_g);
        field_110_anim.field_A_b = static_cast<BYTE>(field_C4_b);

        field_110_anim.field_B_render_mode = 0;

        field_110_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_110_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_D0_pShadow = ao_new<Shadow>();
        if (field_D0_pShadow)
        {
            field_D0_pShadow->ctor_461FB0();
        }
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 1, 0);
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }
    return this;
}

void MeatSaw::VScreenChanged_43A060()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        FP_Abs(sControlledCharacter_50767C->field_A8_xpos - field_A8_xpos) > FP_FromInteger(1024))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

END_NS_AO


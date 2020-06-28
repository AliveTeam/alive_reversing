#include "stdafx_ao.h"
#include "Function.hpp"
#include "ElectricWall.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "stdlib.hpp"

START_NS_AO

ElectricWall* ElectricWall::ctor_40FCF0(Path_ElectricWall* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA8C0);
    field_4_typeId = Types::eElectricWall_25;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6000, 1, 0);
    Animation_Init_417FD0(14536, 50, 80, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = 1;
    field_10_anim.field_C_layer = 36;

    if (field_10_anim.Get_Frame_Count_403540() > 0)
    {
        field_10_anim.SetFrame_402AC0(Math_NextRandom() % field_10_anim.Get_Frame_Count_403540());
    }

    field_C4_b = 80;
    field_C2_g = 80;
    field_C0_r = 80;
    field_E4_tlv = tlvInfo;
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

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

    field_E8_switch_id = pTlv->field_1A_id;
    field_EA_start_state = pTlv->field_1C_start_state;

    if (SwitchStates_Get(pTlv->field_1A_id) == field_EA_start_state)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_EC_sound_timer = 0;
    return this;
}

BaseGameObject* ElectricWall::dtor_40FE80()
{
    SetVTable(this, 0x4BA8C0);
    gMap_507BA8.TLV_Reset_446870(field_E4_tlv, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* ElectricWall::VDestructor(signed int flags)
{
    return Vdtor_410280(flags);
}

ElectricWall* ElectricWall::Vdtor_410280(signed int flags)
{
    dtor_40FE80();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void ElectricWall::VScreenChanged()
{
    VScreenChanged_410220();
}

void ElectricWall::VScreenChanged_410220()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos) == CameraPos::eCamInvalid_m1)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

END_NS_AO

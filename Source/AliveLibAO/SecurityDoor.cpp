#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityDoor.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "GameSpeak.hpp"

START_NS_AO

void SecurityDoor::VScreenChanged()
{
    VScreenChanged_461F80();
}

SecurityDoor* SecurityDoor::vdtor_461F90(signed int flags)
{
    dtor_4619C0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* SecurityDoor::VDestructor(signed int flags)
{
    return vdtor_461F90(flags);
}

BaseGameObject* SecurityDoor::dtor_4619C0()
{
    SetVTable(this, 0x4BC918);
    if (field_E8_state != 1)
    {
        field_E8_state = 0;
    }
    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, field_E8_state + 1, 0, 0);
    return dtor_417D10();
}

SecurityDoor* SecurityDoor::ctor_461840(Path_SecurityDoor* pTlv, int tlvInfo)
{
    ctor_417C10();
    field_F4_event_idx = -1;
    field_108_max_idx = 0;
    field_10A_event_idx = -1;

    SetVTable(this, 0x4BC918);

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6027, 1, 0);
    Animation_Init_417FD0(976, 70, 19, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_E4_tlvInfo = tlvInfo;

    field_C8_yOffset = 0;
    field_10_anim.field_C_layer = 22;

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

    field_EA_id = pTlv->field_1A_id;
    field_EC_code_converted = Code_Convert_476000(pTlv->field_1C_code_1, pTlv->field_1E_code2);
    field_F0_code_len = Code_Length_475FD0(field_EC_code_converted);
    field_10C_top_left = pTlv->field_10_top_left;
    field_110_bottom_right = pTlv->field_14_bottom_right;
    field_A8_xpos = FP_FromInteger(pTlv->field_20_xpos);
    field_AC_ypos = FP_FromInteger(pTlv->field_22_ypos);

    if (pTlv->field_1_unknown)
    {
        field_E8_state = pTlv->field_1_unknown - 1;
    }
    else
    {
        field_E8_state = 0;
    }

    if (field_E8_state != 1)
    {
        field_114_timer = gnFrameCount_507670 + 10;
    }

    return this;
}

void SecurityDoor::VScreenChanged_461F80()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

END_NS_AO


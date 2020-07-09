#include "stdafx_ao.h"
#include "Function.hpp"
#include "BackgroundGlukkon.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"

START_NS_AO

void BackgroundGlukkon::VScreenChanged()
{
    VScreenChanged_41E0E0();
}

BackgroundGlukkon* BackgroundGlukkon::Vdtor_41E0F0(signed int flags)
{
    dtor_41DCE0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* BackgroundGlukkon::VDestructor(signed int flags)
{
    return Vdtor_41E0F0(flags);
}

BaseGameObject* BackgroundGlukkon::dtor_41DCE0()
{
    SetVTable(this, 0x4BAF70);

    if (field_100_health <= FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }

    return dtor_401000();
}

BackgroundGlukkon* BackgroundGlukkon::ctor_41DBD0(Path_Glukkon* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BAF70);

    field_4_typeId = Types::eBackgroundGlukkon_42;

    Animation_Init_417FD0(
        46096,
        68,
        60,
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 800, 1, 0),
        1);

    field_10C_tlvInfo = tlvInfo;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_BC_sprite_scale = FP_FromInteger(pTlv->field_18_scale_percent) / FP_FromInteger(100);

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, pTlv->field_1A_pal_id, 0, 0);
    if (ppRes)
    {
        field_10_anim.LoadPal_403090(ppRes, 0);
    }

    field_120_target_id = pTlv->field_1C_target_id;
    field_11C_voice_adjust = pTlv->field_1E_voice_adjust;
    field_110_state = 0;
    return this;
}

void BackgroundGlukkon::VScreenChanged_41E0E0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

END_NS_AO


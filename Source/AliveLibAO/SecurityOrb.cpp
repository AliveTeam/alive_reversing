#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityOrb.hpp"
#include "ResourceManager.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"

START_NS_AO

SecurityOrb* SecurityOrb::ctor_436C80(Path_SecurityOrb* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BB7D0);

    field_6_flags.Set(Options::eCanExplode_Bit7);

    field_4_typeId = Types::SecurityOrb_53;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2006, 1, 0);
    Animation_Init_417FD0(
        10864,
        52,
        28,
        ppRes,
        1);

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_10C_tlvInfo = tlvInfo;

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

    field_110_state = 0;
    field_118_sound_channels = 0;

    return this;
}

BaseGameObject* SecurityOrb::dtor_436D60()
{
    SetVTable(this, 0x4BB7D0);

    if (field_118_sound_channels)
    {
        SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
    }

    if (field_100_health > FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    return dtor_401000();
}

BaseGameObject* SecurityOrb::VDestructor(signed int flags)
{
    return Vdtor_4373B0(flags);
}

SecurityOrb* SecurityOrb::Vdtor_4373B0(signed int flags)
{
    dtor_436D60();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SecurityOrb::VScreenChanged()
{
    VScreenChanged_4373A0();
}

void SecurityOrb::VScreenChanged_4373A0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

signed __int16 SecurityOrb::VTakeDamage_437280(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::e69:
    case Types::eShrykull_85:
    {
        auto pExplosion = ao_new<Explosion>();
        if (pExplosion)
        {
            pExplosion->ctor_458B80(
                field_A8_xpos,
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(5)),
                field_BC_sprite_scale);
        }

        auto pGibs = ao_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_407B20(
                5,
                field_A8_xpos,
                field_AC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_BC_sprite_scale);
        }
        field_100_health = FP_FromInteger(0);
    }
        break;

    default:
        break;
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);

    return 1;
}

END_NS_AO

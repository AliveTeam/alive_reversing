#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneyDrip.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

START_NS_AO

void HoneyDrip::VUpdate()
{
    VUpdate_431DC0();
}

HoneyDrip* HoneyDrip::ctor_431D10(FP xpos, FP ypos)
{
    ctor_417C10();
    SetVTable(this, 0x4BB2C0);

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 337, 1, 0);
    Animation_Init_417FD0(3628, 62, 126, ppRes, 1);
    field_AC_ypos = ypos;
    field_A8_xpos = xpos;

    field_10_anim.field_C_layer = 22;

    field_E4_bSoundPlaying = FALSE;

    return this;
}

void HoneyDrip::VUpdate_431DC0()
{
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    if (field_10_anim.field_92_current_frame == 7 && !field_E4_bSoundPlaying)
    {
        SFX_Play_43AD70(SoundEffect::Unknown_26, 0, 0);
        field_E4_bSoundPlaying = TRUE;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

BaseGameObject* HoneyDrip::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO

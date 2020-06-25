#include "stdafx_ao.h"
#include "Function.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"

START_NS_AO

EXPORT Particle* CC New_Particle_419D00(FP /*xpos*/, FP /*ypos*/, FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}


Particle* Particle::ctor_478880(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData)
{
    ctor_417C10();
    SetVTable(this, 0x4BCDB0);

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    field_4_typeId = Types::eParticle_93;

    Animation_Init_417FD0(
        animFrameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1);

    if (field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_E4_scale_amount = 0;

    return this;
}

BaseGameObject* Particle::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO